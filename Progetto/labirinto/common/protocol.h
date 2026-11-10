#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>

/* Dimensioni */
#define RIGHE_MAPPA 21
#define COLONNE_MAPPA 41
#define VISTA_LOCALE 5
#define RAGGIO_VISIBILITA (VISTA_LOCALE / 2)

/* Simboli del labirinto */
#define CELLA_MURO '#'
#define CELLA_LIBERA '.'
#define CELLA_USCITA 'E'
#define CELLA_OGGETTO 'O'
#define CELLA_GIOCATORE 'P'
#define CELLA_NASCOSTA '?'

/* Limiti buffer */
#define MAX_NICK 32
#define MAX_PASS 32
#define MAX_TESTO 128

/* Tipi di messaggio */
#define MSG_ACCESSO 1 /* Login */
#define MSG_MOVIMENTO 2
#define MSG_RICHIEDI_LISTA 3 /* Richiesta lista dal client */
#define MSG_ESCI 4
#define MSG_REGISTRAZIONE 5 /* Registrazione nuovo utente */

#define MSG_OK 10
#define MSG_ERRORE 11
#define MSG_MAPPA_LOCALE 12
#define MSG_MAPPA_GLOBALE 13
#define MSG_LISTA 14 /* Elenco giocatori */
#define MSG_FINE_PARTITA 15

/* Info giocatore per lista */
typedef struct {
  char nickname[MAX_NICK];
  int punteggio;
  int uscito;
} InfoGiocatore;

/* Messaggio tra client e server */
typedef struct {
  int tipo; /* Tipo messaggio */
  char nickname[MAX_NICK];
  char password[MAX_PASS];
  char direzione;
  char testo[MAX_TESTO];
  int riga;
  int colonna;
  int punteggio;

  char mappa_locale[VISTA_LOCALE][VISTA_LOCALE];
  char mappa_globale[RIGHE_MAPPA][COLONNE_MAPPA];

  int num_giocatori; /* Numero di InfoGiocatore che seguono il messaggio
                        (MSG_LISTA) */
} Messaggio;

int invia_messaggio(int fd, const Messaggio *msg);
int ricevi_messaggio(int fd, Messaggio *msg);

int invia_dati(int fd, const void *buf, size_t len);
int ricevi_dati(int fd, void *buf, size_t len);

#endif
