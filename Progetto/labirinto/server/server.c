#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "protocol.h"
#include "game.h"
#include "log.h"

#define DEFAULT_TIMEOUT  180
#define DEFAULT_INTERVAL 20
#define LOG_FILE         "server.log"

/* Crea e configura la socket di ascolto TCP */
static int create_listening_socket(int port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
        return -1;

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t)port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(s);
        return -1;
    }

    if (listen(s, 16) < 0) {
        close(s);
        return -1;
    }

    return s;
}

/* Thread dedicato al singolo client connesso */
static void *client_thread(void *arg) {
    int fd = *(int *)arg;
    free(arg);

    Messaggio msg = {0};

    /* Ricezione login */
    if (ricevi_messaggio(fd, &msg) <= 0 || msg.type != MSG_LOGIN) {
        close(fd);
        return NULL;
    }

    msg.nickname[MAX_NICK - 1] = '\0';
    msg.password[MAX_PASS - 1] = '\0';
    if (strlen(msg.nickname) == 0 || strlen(msg.password) == 0) {
        Messaggio err = {0};
        err.type = MSG_ERROR;
        strncpy(err.text, "Nickname o password non validi.", MAX_TEXT - 1);
        invia_messaggio(fd, &err);
        close(fd);
        return NULL;
    }

    int player_idx = game_add_player(fd, msg.nickname, msg.password);
    if (player_idx < 0) {
        Messaggio err = {0};
        err.type = MSG_ERROR;

        if (player_idx == -1)
            strncpy(err.text, "La partita e' gia' terminata.", MAX_TEXT - 1);
        else if (player_idx == -2)
            strncpy(err.text, "Nickname gia' connesso.", MAX_TEXT - 1);
        else if (player_idx == -4)
            strncpy(err.text, "Password errata.", MAX_TEXT - 1);
        else
            strncpy(err.text, "Errore interno del server.", MAX_TEXT - 1);

        invia_messaggio(fd, &err);
        close(fd);
        return NULL;
    }

    /* Conferma login */
    Messaggio ok = {0};
    ok.type = MSG_OK;
    invia_messaggio(fd, &ok);

    /* Invio mappa iniziale */
    game_send_global_map(player_idx);

    /* Ciclo comandi client */
    while (ricevi_messaggio(fd, &msg) > 0) {
        if (msg.type == MSG_MOVE) {
            game_move(player_idx, msg.direction);
        } else if (msg.type == MSG_LIST) {
            game_send_player_list(player_idx);
        } else if (msg.type == MSG_QUIT) {
            break;
        }
    }

    game_remove_player(player_idx);
    return NULL;
}

/* Thread timer per timeout e broadcast */
static void *timer_thread(void *arg) {
    (void)arg;
    int seconds = 0;

    while (!g_game.game_over) {
        sleep(1);
        seconds++;

        /* Scadenza timeout */
        if (seconds >= g_game.timeout) {
            char log_buf[128];
            snprintf(log_buf, sizeof(log_buf), "Partita terminata per timeout (%d secondi).", g_game.timeout);
            log_event(log_buf);
            game_finish();
            game_broadcast_game_over();
            break;
        }

        /* Invio periodico mappa */
        if (seconds % g_game.interval == 0) {
            game_broadcast_global_maps();
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <porta> [timeout] [intervallo]\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int timeout = DEFAULT_TIMEOUT;
    if (argc > 2)
        timeout = atoi(argv[2]);

    int interval = DEFAULT_INTERVAL;
    if (argc > 3)
        interval = atoi(argv[3]);

    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Porta non valida: %s\n", argv[1]);
        return 1;
    }
    if (timeout <= 0)
        timeout = DEFAULT_TIMEOUT;
    if (interval <= 0)
        interval = DEFAULT_INTERVAL;

    /* Ignora SIGPIPE */
    signal(SIGPIPE, SIG_IGN);

    if (log_open(LOG_FILE) < 0) {
        fprintf(stderr, "Impossibile aprire il file di log '%s'.\n", LOG_FILE);
        return 1;
    }

    game_init(timeout, interval);

    int listen_fd = create_listening_socket(port);
    if (listen_fd < 0) {
        fprintf(stderr, "Impossibile creare la socket in ascolto sulla porta %d.\n", port);
        return 1;
    }

    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Server avviato sulla porta %d (timeout=%d, intervallo=%d).",
             port, timeout, interval);
    log_event(log_buf);

    /* Avvio thread timer */
    pthread_t t_tid;
    if (pthread_create(&t_tid, NULL, timer_thread, NULL) != 0) {
        fprintf(stderr, "Impossibile avviare il timer thread.\n");
        close(listen_fd);
        return 1;
    }
    pthread_detach(t_tid);

    /* Accettazione connessioni */
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);

        if (client_fd < 0) {
            if (errno == EINTR)
                continue;
            char err_buf[128];
            snprintf(err_buf, sizeof(err_buf), "Errore accept: %s.", strerror(errno));
            log_event(err_buf);
            continue;
        }

        int *pfd = malloc(sizeof(int));
        *pfd = client_fd;

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_thread, pfd) != 0) {
            free(pfd);
            close(client_fd);
            continue;
        }
        pthread_detach(tid);
    }

    close(listen_fd);
    return 0;
}
