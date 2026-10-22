#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "protocol.h"
#include "display.h"

static int loop = 1;

static void handle_sigint(int sig) {
    (void)sig;
    loop = 0;
}

static void display_prompt(void) {
    printf("Inserisci il comando + ENTER: ");
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <indirizzo_ip> <porta>\n", argv[0]);
        return 1;
    }

    const char *host = argv[1];
    int port = atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Porta non valida: %s\n", argv[2]);
        return 1;
    }

    /* Ignora SIGPIPE */
    signal(SIGPIPE, SIG_IGN);


    /* Creazione socket TCP */
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        return 1;
    }

    /* Configurazione indirizzo server */
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        fprintf(stderr, "Indirizzo IP non valido: %s\n", host);
        close(sock_fd);
        return 1;
    }

    /* Connessione al server */
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock_fd);
        return 1;
    }

    printf("Connesso al server %s:%d\n", host, port);

    /* Richiesta credenziali */
    char nickname[MAX_NICK];
    char password[MAX_PASS];

    printf("Inserisci il tuo nickname: ");
    fflush(stdout);
    scanf("%s", nickname);

    printf("Inserisci la tua password: ");
    fflush(stdout);
    scanf("%s", password);
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    /* Invio login */
    Messaggio msg = {0};
    msg.type = MSG_LOGIN;
    strncpy(msg.nickname, nickname, MAX_NICK - 1);
    strncpy(msg.password, password, MAX_PASS - 1);

    if (invia_messaggio(sock_fd, &msg) < 0) {
        fprintf(stderr, "Errore durante l'invio del login.\n");
        close(sock_fd);
        return 1;
    }

    /* Ricezione della risposta dal server */
    if (ricevi_messaggio(sock_fd, &msg) <= 0) {
        printf("Connessione chiusa dal server.\n");
        close(sock_fd);
        return 1;
    }

    if (msg.type != MSG_OK) {
        printf("Accesso rifiutato: %s\n", msg.text);
        close(sock_fd);
        return 1;
    }

    printf("Accesso effettuato come '%s'.\n", nickname);

    /* Loop principale con select */
    fd_set master_fds, read_fds;
    FD_ZERO(&master_fds);
    FD_SET(STDIN_FILENO, &master_fds);
    FD_SET(sock_fd, &master_fds);
    int max_fd = sock_fd;

    /* Gestione segnali: intercetta SIGINT (Ctrl+C) e SIGTERM per inviare MSG_QUIT */
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    
    while (loop) {
        read_fds = master_fds;
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            if (errno == EINTR) {
                if (!loop)
                    break;
                continue;
            }
            perror("select");
            break;
        }

        /* Input tastiera */
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            char buffer[64];
            /* Disconnessione su EOF */
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                loop = 0;
                break;
            }

            char cmd = tolower(buffer[0]);
            Messaggio out_msg = {0};
            if (cmd == 'w' || cmd == 'a' || cmd == 's' || cmd == 'd') {
                out_msg.type = MSG_MOVE;
                out_msg.direction = cmd;
                invia_messaggio(sock_fd, &out_msg);
            } else if (cmd == 'l') {
                out_msg.type = MSG_LIST;
                invia_messaggio(sock_fd, &out_msg);
            } else if (cmd == 'h') {
                display_help();
                display_prompt();
            } else if (cmd == 'q') {
                out_msg.type = MSG_QUIT;
                invia_messaggio(sock_fd, &out_msg);
                printf("Arrivederci!\n");
                break;
            } else if (cmd != '\n' && cmd != '\r') {
                printf("Comando non riconosciuto (premi 'h' per l'aiuto).\n");
                display_prompt();
            }
        }

        /* Messaggio dal server */
        if (FD_ISSET(sock_fd, &read_fds)) {
            int ret = ricevi_messaggio(sock_fd, &msg);
            if (ret <= 0) {
                printf("Connessione chiusa dal server.\n");
                break;
            }

            switch (msg.type) {
                case MSG_MAPPA_LOCALE:
                    display_local_map(msg.row, msg.col, msg.score, msg.local_map, msg.text, 1);
                    break;

                case MSG_MAPPA_GLOBALE:
                    display_global_map(msg.row, msg.col, msg.global_map);
                    display_local_map(msg.row, msg.col, msg.score, msg.local_map, msg.text, 0);
                    break;

                case MSG_LISTA:
                    display_player_list(msg.players, msg.num_players);
                    display_prompt();
                    break;

                case MSG_INFO:
                    printf("\n[server] %s\n", msg.text);
                    display_prompt();
                    break;

                case MSG_ERROR:
                    printf("\n[server] Errore: %s\n", msg.text);
                    display_prompt();
                    break;

                case MSG_FINE_PARTITA:
                    if (msg.nickname[0] != '\0')
                        printf("\n*** Partita terminata. Vincitore: %s con %d oggetti raccolti! ***\n",
                               msg.nickname, msg.score);
                    else
                        printf("\n*** Partita terminata. Nessun vincitore. ***\n");
                    close(sock_fd);
                    return 0;

                default:
                    break;
            }
        }
    }

    /* Invio disconnessione in uscita (Ctrl+D / Ctrl+C / SIGINT / SIGTERM)*/
    if (!loop) {
        printf("\nRicevuto segnale di interruzione: disconnessione in corso...\n");
        Messaggio quit_msg = {0};
        quit_msg.type = MSG_QUIT;
        invia_messaggio(sock_fd, &quit_msg);
    }

    close(sock_fd);
    return 0;
}
