#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "auth.h"
#include "game.h"
#include "log.h"
#include "protocol.h"

#define DEFAULT_TIMEOUT 180
#define DEFAULT_INTERVAL 20
#define LOG_FILE "server.log"

/* Crea la socket TCP in ascolto. Ritorna il fd oppure -1. */
static int create_listening_socket(int port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
        return -1;

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
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

static void send_text(int fd, uint8_t type, const char *text) {
    TextPayload t;
    memset(&t, 0, sizeof(t));
    strncpy(t.text, text, MAX_TEXT - 1);
    send_message(fd, type, &t, (uint16_t)sizeof(t));
}

/* Corpo del thread che gestisce un singolo client. */
static void *client_thread(void *arg) {
    int fd = *(int *)arg;
    free(arg);

    uint8_t type;
    unsigned char payload[MAX_PAYLOAD];

    int len = recv_message(fd, &type, payload, sizeof(payload));
    if (len < 0) {
        close(fd);
        return NULL;
    }

    if (type != MSG_REGISTER && type != MSG_LOGIN) {
        send_text(fd, MSG_ERROR, "Autenticazione richiesta.");
        close(fd);
        return NULL;
    }

    AuthPayload *auth = (AuthPayload *)payload;
    auth->nickname[MAX_NICK - 1] = '\0';
    auth->password[MAX_PASS - 1] = '\0';

    if (type == MSG_REGISTER) {
        int rc = auth_register(auth->nickname, auth->password);
        if (rc == -1) {
            send_text(fd, MSG_ERROR, "Nickname gia' registrato.");
            close(fd);
            return NULL;
        }
        if (rc != 0) {
            send_text(fd, MSG_ERROR, "Registrazione non valida.");
            close(fd);
            return NULL;
        }
        log_event("Nuovo utente registrato: %s.", auth->nickname);
    }

    if (auth_login(auth->nickname, auth->password) != 1) {
        send_text(fd, MSG_ERROR, "Nickname o password errati.");
        close(fd);
        return NULL;
    }

    int index = game_add_player(fd, auth->nickname);
    if (index < 0) {
        if (index == -1)
            send_text(fd, MSG_ERROR, "La partita e' terminata.");
        else if (index == -2)
            send_text(fd, MSG_ERROR, "Nickname gia' connesso.");
        else
            send_text(fd, MSG_ERROR, "Server pieno.");
        close(fd);
        return NULL;
    }

    send_message(fd, MSG_OK, NULL, 0);
    send_text(fd, MSG_INFO, "Benvenuto! Comandi: w/a/s/d, l, m, g, h, q.");
    game_send_local_map(index);

    while (1) {
        len = recv_message(fd, &type, payload, sizeof(payload));
        if (len < 0)
            break;

        if (type == MSG_MOVE && len >= 1)
            game_move(index, (char)payload[0]);
        else if (type == MSG_LIST)
            game_send_player_list(index);
        else if (type == MSG_QUIT)
            break;
    }

    game_remove_player(index);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <porta> [timeout] [intervallo]\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    int timeout = (argc > 2) ? atoi(argv[2]) : DEFAULT_TIMEOUT;
    int interval = (argc > 3) ? atoi(argv[3]) : DEFAULT_INTERVAL;

    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Porta non valida.\n");
        return 1;
    }
    if (timeout <= 0)
        timeout = DEFAULT_TIMEOUT;
    if (interval <= 0)
        interval = DEFAULT_INTERVAL;

    /* Senza questa ignorazione una scrittura su socket chiusa terminerebbe il server. */
    signal(SIGPIPE, SIG_IGN);

    if (log_open(LOG_FILE) < 0) {
        fprintf(stderr, "Impossibile aprire il file di log.\n");
        return 1;
    }

    game_init(timeout, interval);

    int listen_fd = create_listening_socket(port);
    if (listen_fd < 0) {
        fprintf(stderr, "Impossibile creare la socket in ascolto.\n");
        return 1;
    }
    g_game.listen_fd = listen_fd;

    log_event("Server avviato sulla porta %d (timeout=%d, intervallo=%d).",
              port, timeout, interval);

    game_start_timer();

    for (;;) {
        struct sockaddr_in client_addr;
        socklen_t clen = sizeof(client_addr);
        int fd = accept(listen_fd, (struct sockaddr *)&client_addr, &clen);

        if (fd < 0) {
            if (errno == EINTR)
                continue;
            log_event("Errore in accept: %s.", strerror(errno));
            continue;
        }

        int *arg = malloc(sizeof(int));
        if (!arg) {
            close(fd);
            continue;
        }
        *arg = fd;

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_thread, arg) != 0) {
            free(arg);
            close(fd);
            continue;
        }
        pthread_detach(tid);
    }

    close(listen_fd);
    return 0;
}
