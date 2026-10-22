#ifndef PROTOCOL_H
#define PROTOCOL_H

/* Dimensioni */
#define MAP_ROWS    21
#define MAP_COLS    41
#define LOCAL_VIEW  5
#define VIEW_RADIUS (LOCAL_VIEW / 2)

/* Simboli del labirinto */
#define CELL_WALL   '#'
#define CELL_FREE   '.'
#define CELL_EXIT   'E'
#define CELL_OBJECT 'O'
#define CELL_PLAYER 'P'
#define CELL_HIDDEN '?'

/* Limiti buffer */
#define MAX_NICK         32
#define MAX_PASS         32
#define MAX_TEXT         128
#define MAX_LIST_PLAYERS 64

/* Tipi di messaggio */
#define MSG_LOGIN          1   /* Login */
#define MSG_MOVE           2   /* Movimento */
#define MSG_LIST           3   /* Richiesta lista */
#define MSG_QUIT           4   /* Uscita */

#define MSG_OK             10  /* Conferma */
#define MSG_ERROR          11  /* Errore */
#define MSG_MAPPA_LOCALE   12  /* Mappa locale */
#define MSG_MAPPA_GLOBALE  13  /* Mappa globale */
#define MSG_LISTA          14  /* Elenco giocatori */
#define MSG_FINE_PARTITA   15  /* Fine partita */
#define MSG_INFO           16  /* Notifica */

/* Info giocatore per lista */
typedef struct {
    char nickname[MAX_NICK];
    int  score;
    int  exited;
} InfoGiocatore;

/* Messaggio di rete */
typedef struct {
    int  type;                              /* Tipo messaggio */
    char nickname[MAX_NICK];                /* Nickname */
    char password[MAX_PASS];                /* Password */
    char direction;                         /* Direzione */
    char text[MAX_TEXT];                    /* Testo */
    int  row;                               /* Riga */
    int  col;                               /* Colonna */
    int  score;                             /* Punteggio */
    
    char local_map[LOCAL_VIEW][LOCAL_VIEW]; /* Vista locale */
    char global_map[MAP_ROWS][MAP_COLS];    /* Mappa globale */
    
    int  num_players;                       /* Numero giocatori */
    InfoGiocatore players[MAX_LIST_PLAYERS];/* Giocatori */
} Messaggio;

/* Invio e ricezione messaggi */
int invia_messaggio(int fd, const Messaggio *msg);
int ricevi_messaggio(int fd, Messaggio *msg);

#endif
