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
#include <netdb.h>

#include "protocol.h"
#include "display.h"

static int loop = 1;

static void sigint_handler(int sig) {
    (void)sig;
    loop = 0;
}

static void mostra_prompt(void) {
    printf("Inserisci il comando + ENTER: ");
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <nome_host|indirizzo_ip> <porta>\n", argv[0]);
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

    /* Risoluzione del nome host o dell'indirizzo IP con getaddrinfo() */
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family   = AF_INET;      /* Il server ascolta su IPv4 */
    hints.ai_socktype = SOCK_STREAM;  /* TCP */

    char port_str[16];
    snprintf(port_str, sizeof(port_str), "%d", port);

    int err = getaddrinfo(host, port_str, &hints, &res);
    if (err != 0) {
        fprintf(stderr, "Impossibile risolvere l'host %s: %s\n", host, gai_strerror(err));
        return 1;
    }

    /* Creazione socket TCP e connessione al primo indirizzo utilizzabile */
    int sock_fd;
    for (p = res; p != NULL; p = p->ai_next) {
        sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock_fd < 0)
            continue;
        if (connect(sock_fd, p->ai_addr, p->ai_addrlen) == 0)
            break;
        close(sock_fd);
    }

    /* Se il ciclo e' terminato senza break nessun indirizzo ha accettato la connessione */
    if (p == NULL) {
        fprintf(stderr, "Impossibile connettersi a %s:%d\n", host, port);
        freeaddrinfo(res);
        return 1;
    }
    freeaddrinfo(res);

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

    /* Invio richiesta di accesso */
    Messaggio msg = {0};
    msg.tipo = MSG_ACCESSO;
    strncpy(msg.nickname, nickname, MAX_NICK - 1);
    strncpy(msg.password, password, MAX_PASS - 1);

    if (invia_messaggio(sock_fd, &msg) < 0) {
        perror("invia_messaggio");
        close(sock_fd);
        return 1;
    }

    /* Attesa esito accesso */
    if (ricevi_messaggio(sock_fd, &msg) <= 0) {
        fprintf(stderr, "Disconnessione dal server durante l'accesso.\n");
        close(sock_fd);
        return 1;
    }

    if (msg.tipo == MSG_ERRORE) {
        printf("Accesso rifiutato: %s\n", msg.testo);
        close(sock_fd);
        return 1;
    }
    if (msg.tipo != MSG_OK) {
        printf("Risposta inattesa dal server (tipo=%d).\n", msg.tipo);
        close(sock_fd);
        return 1;
    }

    printf("Accesso eseguito con successo!\n");

    /* Ricezione della mappa globale iniziale */
    if (ricevi_messaggio(sock_fd, &msg) <= 0 || msg.tipo != MSG_MAPPA_GLOBALE) {
        fprintf(stderr, "Errore nella ricezione della mappa iniziale.\n");
        close(sock_fd);
        return 1;
    }

    mostra_mappa_globale(msg.riga, msg.colonna, msg.mappa_globale);
    mostra_mappa_locale(msg.riga, msg.colonna, msg.punteggio, msg.mappa_locale, NULL, 0);

    fd_set master_set, read_set;
    FD_ZERO(&master_set);
    FD_SET(STDIN_FILENO, &master_set);
    FD_SET(sock_fd, &master_set);
    int max_fd = (sock_fd > STDIN_FILENO) ? sock_fd : STDIN_FILENO;

    /* intercetta SIGINT (Ctrl+C) e SIGTERM per inviare MSG_ESCI */
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);
    
    while (loop) {
        read_set = master_set;
        int activity = select(max_fd + 1, &read_set, NULL, NULL, NULL);
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
        if (FD_ISSET(STDIN_FILENO, &read_set)) {
            char buffer[64];
            /* Disconnessione su EOF */
            if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
                loop = 0;
                break;
            }

            char comando = tolower(buffer[0]);
            Messaggio msg_invio = {0};
            if (comando == 'w' || comando == 'a' || comando == 's' || comando == 'd') {
                msg_invio.tipo = MSG_MOVIMENTO;
                msg_invio.direzione = comando;
                invia_messaggio(sock_fd, &msg_invio);
            } else if (comando == 'l') {
                msg_invio.tipo = MSG_RICHIEDI_LISTA;
                invia_messaggio(sock_fd, &msg_invio);
            } else if (comando == 'h') {
                mostra_aiuto();
                mostra_prompt();
            } else if (comando == 'q') {
                msg_invio.tipo = MSG_ESCI;
                invia_messaggio(sock_fd, &msg_invio);
                printf("Arrivederci!\n");
                break;
            } else if (comando != '\n' && comando != '\r') {
                printf("Comando non riconosciuto (premi 'h' per l'aiuto).\n");
                mostra_prompt();
            }
        }

        /* Messaggio dal server */
        if (FD_ISSET(sock_fd, &read_set)) {
            int ret = ricevi_messaggio(sock_fd, &msg);
            if (ret <= 0) {
                printf("Connessione chiusa dal server.\n");
                break;
            }

            switch (msg.tipo) {
                case MSG_MAPPA_LOCALE:
                    mostra_mappa_locale(msg.riga, msg.colonna, msg.punteggio, msg.mappa_locale, msg.testo, 1);
                    break;

                case MSG_MAPPA_GLOBALE:
                    mostra_mappa_globale(msg.riga, msg.colonna, msg.mappa_globale);
                    mostra_mappa_locale(msg.riga, msg.colonna, msg.punteggio, msg.mappa_locale, msg.testo, 0);
                    break;

                case MSG_LISTA: {
                    /* Dopo il messaggio arrivano num_giocatori strutture InfoGiocatore */
                    InfoGiocatore *lista = NULL;
                    if (msg.num_giocatori > 0) {
                        lista = malloc(msg.num_giocatori * sizeof(InfoGiocatore));
                        if (!lista) {
                            perror("malloc");
                            close(sock_fd);
                            return 1;
                        }

                        if (ricevi_dati(sock_fd, lista, msg.num_giocatori * sizeof(InfoGiocatore)) <= 0) {
                            free(lista);
                            printf("Connessione chiusa dal server.\n");
                            close(sock_fd);
                            return 1;
                        }
                    }
                    mostra_lista_giocatori(lista, msg.num_giocatori);
                    free(lista);
                    mostra_prompt();
                    break;
                }

                case MSG_NOTIFICA:
                    printf("\n[server] %s\n", msg.testo);
                    mostra_prompt();
                    break;

                case MSG_ERRORE:
                    printf("\n[server] Errore: %s\n", msg.testo);
                    mostra_prompt();
                    break;

                case MSG_FINE_PARTITA:
                    if (msg.nickname[0] != '\0')
                        printf("\n*** Partita terminata. Vincitore: %s con %d oggetti raccolti! ***\n",
                               msg.nickname, msg.punteggio);
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
        printf("\n Disconnessione in corso...\n");
        Messaggio msg_uscita = {0};
        msg_uscita.tipo = MSG_ESCI;
        invia_messaggio(sock_fd, &msg_uscita);
    }

    close(sock_fd);
    return 0;
}
