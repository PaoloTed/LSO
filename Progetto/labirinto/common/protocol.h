#ifndef PROTOCOL_H
#define PROTOCOL_H

/* Dimensioni del labirinto e della vista locale */
#define MAP_ROWS    21
#define MAP_COLS    41
#define LOCAL_VIEW  5
#define VIEW_RADIUS (LOCAL_VIEW / 2)

/* Simboli utilizzati per le celle del labirinto */
#define CELL_WALL   '#'
#define CELL_FREE   '.'
#define CELL_EXIT   'E'
#define CELL_OBJECT 'O'
#define CELL_PLAYER 'P'
#define CELL_HIDDEN '?'

/* Limiti dimensionali */
#define MAX_NICK    32
#define MAX_TEXT    128
#define MAX_PLAYERS 32

/* Tipi di messaggio (protocollo applicativo) */
#define MSG_LOGIN          1   /* Client -> Server: invio nickname */
#define MSG_MOVE           2   /* Client -> Server: tasto direzione */
#define MSG_LIST           3   /* Client -> Server: richiesta lista giocatori */
#define MSG_QUIT           4   /* Client -> Server: disconnessione */

#define MSG_OK             10  /* Server -> Client: operazione confermata */
#define MSG_ERROR          11  /* Server -> Client: messaggio di errore */
#define MSG_MAPPA_LOCALE   12  /* Server -> Client: invio vista 5x5 */
#define MSG_MAPPA_GLOBALE  13  /* Server -> Client: invio mappa globale periodica */
#define MSG_LISTA          14  /* Server -> Client: elenco giocatori connessi */
#define MSG_FINE_PARTITA   15  /* Server -> Client: vincitore e fine gioco */
#define MSG_INFO           16  /* Server -> Client: messaggio informativo */

/* Struttura per ciascun giocatore nella lista giocatori */
typedef struct {
    char nickname[MAX_NICK];
    int  score;
    int  exited;
} InfoGiocatore;

/* Struttura unica per tutti i messaggi scambiati tra Client e Server */
typedef struct {
    int  type;                              /* Tipo di messaggio (MSG_*) */
    char nickname[MAX_NICK];                /* Nickname per login o vincitore */
    char direction;                         /* Direzione: 'w', 'a', 's', 'd' */
    char text[MAX_TEXT];                    /* Testo descrittivo o messaggio di errore */
    int  row;                               /* Riga corrente */
    int  col;                               /* Colonna corrente */
    int  score;                             /* Punteggio corrente */
    
    char local_map[LOCAL_VIEW][LOCAL_VIEW]; /* Vista locale 5x5 */
    char global_map[MAP_ROWS][MAP_COLS];    /* Mappa globale 21x41 mascherata */
    
    int  num_players;                       /* Numero di giocatori nella lista */
    InfoGiocatore players[MAX_PLAYERS];     /* Array dei giocatori connessi */
} Messaggio;

/* Funzioni di I/O affidabili per socket TCP */
int invia_messaggio(int fd, const Messaggio *msg);
int ricevi_messaggio(int fd, Messaggio *msg);

#endif
