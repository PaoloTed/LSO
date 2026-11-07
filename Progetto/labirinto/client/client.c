#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "display.h"
#include "protocol.h"

static int loop = 1;

static void sigint_handler(int sig) {
  (void)sig;
  loop = 0;
}

static void mostra_prompt(void) { printf("Inserisci il comando + ENTER: "); }

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
  hints.ai_family = AF_INET;       /* Il server ascolta su IPv4 */
  hints.ai_socktype = SOCK_STREAM; /* TCP */

  char port_str[16];
  snprintf(port_str, sizeof(port_str), "%d", port);

  int err = getaddrinfo(host, port_str, &hints, &res);
  if (err != 0) {
    fprintf(stderr, "Impossibile risolvere l'host %s: %s\n", host,
            gai_strerror(err));
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

  /* Se il ciclo e' terminato senza break nessun indirizzo ha accettato la
   * connessione */
  if (p == NULL) {
    fprintf(stderr, "Impossibile connettersi a %s:%d\n", host, port);
    freeaddrinfo(res);
    return 1;
  }
  freeaddrinfo(res);

  printf("Connesso al server %s:%d\n", host, port);

  /* Menu iniziale: Accesso, Registrazione o Uscita */
  int scelta = 0;
  while (scelta < 1 || scelta > 3) {
    system("clear");
    printf("\n=== Labirinto ===\n");
    printf("1. Accedi\n");
    printf("2. Registrati\n");
    printf("3. Esci\n");
    printf("Seleziona un'opzione: ");
    if (scanf("%d", &scelta) != 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      scelta = 0;
    } else {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
    }
    if (scelta < 1 || scelta > 3) {
      printf("Scelta non valida. Riprova.\n");
    }
  }

  if (scelta == 3) {
    printf("Uscita dal gioco.\n");
    close(sock_fd);
    return 0;
  }

  /* Richiesta credenziali */
  char nickname[MAX_NICK] = {0};
  char password[MAX_PASS] = {0};

  if (scelta == 1) {
    printf("\n--- Accesso ---\n");
  } else {
    printf("\n--- Registrazione ---\n");
  }

  printf("Inserisci il tuo nickname: ");
  scanf("%31s", nickname);
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }

  printf("Inserisci la tua password: ");
  scanf("%31s", password);
  {
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }

  /* Invio richiesta di accesso / registrazione */
  Messaggio msg = {0};
  switch (scelta) {
  case 1:
    msg.tipo = MSG_ACCESSO;
    break;
  case 2:
    msg.tipo = MSG_REGISTRAZIONE;
    break;
  }
  strncpy(msg.nickname, nickname, MAX_NICK - 1);
  strncpy(msg.password, password, MAX_PASS - 1);

  if (invia_messaggio(sock_fd, &msg) < 0) {
    perror("invia_messaggio");
    close(sock_fd);
    return 1;
  }

  /* Attesa esito accesso / registrazione */
  if (ricevi_messaggio(sock_fd, &msg) <= 0) {
    fprintf(stderr, "Disconnessione dal server durante l'autenticazione.\n");
    close(sock_fd);
    return 1;
  }

  if (msg.tipo == MSG_ERRORE) {
    printf("Errore: %s\n", msg.testo);
    close(sock_fd);
    return 1;
  }
  if (msg.tipo != MSG_OK) {
    printf("Risposta inattesa dal server (tipo=%d).\n", msg.tipo);
    close(sock_fd);
    return 1;
  }

  switch (scelta) {
  case 1:
    printf("\nAccesso eseguito con successo!\n");
    break;
  case 2:
    printf("\nRegistrazione completata con successo!\n");
    break;
  }

  /* Menu di gioco: Gioca o Esci */
  int scelta_gioco = 0;
  while (scelta_gioco < 1 || scelta_gioco > 2) {
    system("clear");
    printf("\n=== Menu Gioco ===\n");
    printf("1. Gioca\n");
    printf("2. Esci\n");
    printf("Seleziona un'opzione: ");
    if (scanf("%d", &scelta_gioco) != 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      scelta_gioco = 0;
    } else {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
    }
    if (scelta_gioco < 1 || scelta_gioco > 2) {
      printf("Scelta non valida. Riprova.\n");
    }
  }

  if (scelta_gioco == 2) {
    printf("\nDisconnessione in corso...\n");
    Messaggio msg_uscita = {0};
    msg_uscita.tipo = MSG_ESCI;
    invia_messaggio(sock_fd, &msg_uscita);
    close(sock_fd);
    return 0;
  }

  /* Ricezione della mappa globale iniziale */
  if (ricevi_messaggio(sock_fd, &msg) <= 0) {
    fprintf(stderr, "Errore nella ricezione della mappa iniziale.\n");
    close(sock_fd);
    return 1;
  }
  if (msg.tipo != MSG_MAPPA_GLOBALE) {
    printf("Risposta inattesa dal server (tipo=%d).\n", msg.tipo);
    close(sock_fd);
    return 1;
  }
  mostra_mappa_globale(msg.riga, msg.colonna, msg.mappa_globale);
  mostra_mappa_locale(msg.riga, msg.colonna, msg.punteggio, msg.mappa_locale);
  mostra_comandi();

  fd_set master_set, read_set;
  FD_ZERO(&master_set);
  FD_SET(STDIN_FILENO, &master_set);
  FD_SET(sock_fd, &master_set);
  int max_fd = (sock_fd > STDIN_FILENO) ? sock_fd : STDIN_FILENO;

  /* intercetta SIGINT (Ctrl+C) e SIGTERM per inviare MSG_ESCI */
  signal(SIGINT, sigint_handler);
  signal(SIGTERM, sigint_handler);

  int uscito = 0;
  int punteggio_giocatore = msg.punteggio;

  while (loop) {
    read_set = master_set;
    int activity = select(max_fd + 1, &read_set, NULL, NULL, NULL);
    if (activity < 0) {
      if (errno == EINTR)
        continue;
      perror("select");
      close(sock_fd);
      return 1;
    }

    /* Input tastiera */
    if (FD_ISSET(STDIN_FILENO, &read_set)) {
      char buffer[64];
      fgets(buffer, sizeof(buffer), stdin);

      char comando = tolower(buffer[0]);
      if (uscito) {
        if (comando == 'q') {
          loop = 0;
          break;
        }
        mostra_schermata_uscito(punteggio_giocatore);
        continue;
      }

      Messaggio msg_invio = {0};
      if (comando == 'w' || comando == 'a' || comando == 's' ||
          comando == 'd') {
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
        loop = 0;
        break;
      } else if (comando != '\n' && comando != '\r') {
        printf("Comando non riconosciuto (premi 'h' per l'aiuto).\n");
        mostra_prompt();
        fflush(stdout);
      }
    }

    /* Messaggio dal server */
    if (FD_ISSET(sock_fd, &read_set)) {
      int ret = ricevi_messaggio(sock_fd, &msg);
      if (ret <= 0) {
        printf("Connessione chiusa dal server.\n");
        close(sock_fd);
        return 1;
      }

      switch (msg.tipo) {
      case MSG_MAPPA_LOCALE:
        punteggio_giocatore = msg.punteggio;
        if (strcmp(msg.testo, "Hai raggiunto l'uscita!") == 0) {
          uscito = 1;
        }

        if (uscito) {
          mostra_schermata_uscito(punteggio_giocatore);
        } else {
          system("clear");
          mostra_mappa_locale(msg.riga, msg.colonna, msg.punteggio,
                              msg.mappa_locale);
          mostra_avviso(msg.testo);
          mostra_comandi();
        }
        break;

      case MSG_MAPPA_GLOBALE:
        punteggio_giocatore = msg.punteggio;
        if (uscito) {
          mostra_schermata_uscito(punteggio_giocatore);
        } else {
          mostra_mappa_globale(msg.riga, msg.colonna, msg.mappa_globale);
          sleep(1);
          mostra_mappa_locale(msg.riga, msg.colonna, msg.punteggio,
                              msg.mappa_locale);
          mostra_avviso(msg.testo);
          mostra_comandi();
        }
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

          if (ricevi_dati(sock_fd, lista,
                          msg.num_giocatori * sizeof(InfoGiocatore)) <= 0) {
            free(lista);
            printf("Connessione chiusa dal server.\n");
            close(sock_fd);
            return 1;
          }
        }
        if (!uscito) {
          mostra_lista_giocatori(lista, msg.num_giocatori);
          mostra_prompt();
        }
        free(lista);
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
        system("clear");
        if (msg.nickname[0] != '\0')
          printf("\n*** Partita terminata. Vincitore: %s con %d oggetti "
                 "raccolti! ***\n",
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

  /* Invio disconnessione in uscita */
  printf("\nDisconnessione in corso...\n");
  Messaggio msg_uscita = {0};
  msg_uscita.tipo = MSG_ESCI;
  invia_messaggio(sock_fd, &msg_uscita);

  close(sock_fd);
  return 0;
}
