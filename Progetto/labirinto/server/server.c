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

#define TIMEOUT_PREDEFINITO    180
#define INTERVALLO_PREDEFINITO 20
#define FILE_LOG               "server.log"

/* Crea e configura la socket di ascolto TCP (server_fd) */
static int create_server_socket(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        return -1;

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 16) < 0) {
        close(server_fd);
        return -1;
    }

    return server_fd;
}

/* Thread dedicato al singolo client connesso (client_handler) */
static void *client_handler(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);

    Messaggio msg = {0};

    /* Ricezione accesso */
    if (ricevi_messaggio(client_fd, &msg) <= 0 || msg.tipo != MSG_ACCESSO) {
        close(client_fd);
        return NULL;
    }

    msg.nickname[MAX_NICK - 1] = '\0';
    msg.password[MAX_PASS - 1] = '\0';
    if (strlen(msg.nickname) == 0 || strlen(msg.password) == 0) {
        Messaggio err = {0};
        err.tipo = MSG_ERRORE;
        strncpy(err.testo, "Nickname o password non validi.", MAX_TESTO - 1);
        invia_messaggio(client_fd, &err);
        close(client_fd);
        return NULL;
    }

    int indice_giocatore = aggiungi_giocatore(client_fd, msg.nickname, msg.password);
    if (indice_giocatore < 0) {
        Messaggio err = {0};
        err.tipo = MSG_ERRORE;

        if (indice_giocatore == -1)
            strncpy(err.testo, "La partita e' gia' terminata.", MAX_TESTO - 1);
        else if (indice_giocatore == -2)
            strncpy(err.testo, "Nickname gia' connesso.", MAX_TESTO - 1);
        else if (indice_giocatore == -4)
            strncpy(err.testo, "Password errata.", MAX_TESTO - 1);
        else
            strncpy(err.testo, "Errore interno del server.", MAX_TESTO - 1);

        invia_messaggio(client_fd, &err);
        close(client_fd);
        return NULL;
    }

    /* Conferma accesso */
    Messaggio ok = {0};
    ok.tipo = MSG_OK;
    invia_messaggio(client_fd, &ok);

    /* Invio mappa iniziale */
    invia_mappa_globale(indice_giocatore);

    /* Ciclo comandi client */
    while (ricevi_messaggio(client_fd, &msg) > 0) {
        if (msg.tipo == MSG_MOVIMENTO) {
            muovi_giocatore(indice_giocatore, msg.direzione);
        } else if (msg.tipo == MSG_RICHIEDI_LISTA) {
            invia_lista_giocatori(indice_giocatore);
        } else if (msg.tipo == MSG_ESCI) {
            break;
        }
    }

    rimuovi_giocatore(indice_giocatore);
    return NULL;
}

/* Thread timer per timeout e broadcast */
static void *timer_thread(void *arg) {
    (void)arg;
    int secondi = 0;

    while (!partita.partita_finita) {
        sleep(1);
        secondi++;

        /* Scadenza timeout */
        if (secondi >= partita.timeout) {
            char log_buf[128];
            snprintf(log_buf, sizeof(log_buf), "Partita terminata per timeout (%d secondi).", partita.timeout);
            log_event(log_buf);
            termina_partita();
            broadcast_fine_partita();
            break;
        }

        /* Invio periodico mappa */
        if (secondi % partita.intervallo == 0) {
            broadcast_mappe_globali();
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
    int timeout = TIMEOUT_PREDEFINITO;
    if (argc > 2)
        timeout = atoi(argv[2]);

    int intervallo = INTERVALLO_PREDEFINITO;
    if (argc > 3)
        intervallo = atoi(argv[3]);

    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Porta non valida: %s\n", argv[1]);
        return 1;
    }
    if (timeout <= 0)
        timeout = TIMEOUT_PREDEFINITO;
    if (intervallo <= 0)
        intervallo = INTERVALLO_PREDEFINITO;

    /* Ignora SIGPIPE */
    signal(SIGPIPE, SIG_IGN);

    if (log_open(FILE_LOG) < 0) {
        fprintf(stderr, "Impossibile aprire il file di log '%s'.\n", FILE_LOG);
        return 1;
    }

    inizializza_gioco(timeout, intervallo);

    int server_fd = create_server_socket(port);
    if (server_fd < 0) {
        fprintf(stderr, "Impossibile creare la socket in ascolto sulla porta %d.\n", port);
        return 1;
    }

    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Server avviato sulla porta %d (timeout=%d, intervallo=%d).",
             port, timeout, intervallo);
    log_event(log_buf);

    /* Avvio thread timer */
    pthread_t timer_tid;
    if (pthread_create(&timer_tid, NULL, timer_thread, NULL) != 0) {
        fprintf(stderr, "Impossibile avviare il timer thread.\n");
        close(server_fd);
        return 1;
    }
    pthread_detach(timer_tid);

    /* Accettazione connessioni */
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

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
        if (pthread_create(&tid, NULL, client_handler, pfd) != 0) {
            free(pfd);
            close(client_fd);
            continue;
        }
        pthread_detach(tid);
    }

    close(server_fd);
    return 0;
}
