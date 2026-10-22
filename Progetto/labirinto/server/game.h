#ifndef GAME_H
#define GAME_H

#include <pthread.h>
#include <time.h>
#include "protocol.h"

/* Struttura del labirinto */
typedef struct {
    char cells[MAP_ROWS][MAP_COLS];
    char objects[MAP_ROWS][MAP_COLS];
} Labirinto;

/* Dati del giocatore */
typedef struct {
    int  fd;                                /* Socket */
    int  active;                            /* Giocatore attivo */
    int  joined;                            /* Partecipante */
    int  exited;                            /* Raggiunta uscita */
    char nickname[MAX_NICK];
    int  row, col;                          /* Posizione */
    int  score;                             /* Punteggio */
    char discovered[MAP_ROWS][MAP_COLS];    /* Celle scoperte */
} Giocatore;

/* Stato globale della partita */
typedef struct {
    Labirinto maze;
    Giocatore *players;                     /* Array giocatori */
    int  players_count;                     /* Numero giocatori */
    int  game_over;                         /* Fine partita (valore 1 se terminata) */
    int  winner;                            /* Indice vincitore */
    int  timeout;                           /* Timeout partita */
    int  interval;                          /* Intervallo broadcast */
    pthread_mutex_t mutex;                  /* Mutex per sincronizzazione */
} StatoGioco;

extern StatoGioco g_game;

/* Inizializza il gioco */
void game_init(int timeout, int interval);

/* Aggiunge un giocatore verificando le credenziali */
int  game_add_player(int fd, const char *nickname, const char *password);

/* Rimuove un giocatore */
void game_remove_player(int index);

/* Movimento del giocatore */
void game_move(int index, char direction);

/* Invio messaggi */
void game_send_local_map(int index, const char *notice);
void game_send_global_map(int index);
void game_send_player_list(int index);

/* Invio broadcast */
void game_broadcast_global_maps(void);
void game_broadcast_game_over(void);

/* Conclude la partita e calcola il vincitore */
int  game_finish(void);

#endif
