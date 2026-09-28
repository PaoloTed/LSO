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

/* Struttura per ciascun giocatore connesso */
typedef struct {
    int  fd;                                /* Socket del giocatore */
    int  active;                            /* 1 se attualmente connesso */
    int  joined;                            /* 1 se ha partecipato alla partita */
    int  exited;                            /* 1 se ha raggiunto l'uscita 'E' */
    char nickname[MAX_NICK];
    int  row, col;                          /* Posizione nel labirinto */
    int  score;                             /* Oggetti raccolti */
    char discovered[MAP_ROWS][MAP_COLS];    /* 1 per le celle gia' scoperte */
} Giocatore;

/* Stato globale della partita */
typedef struct {
    Labirinto maze;
    Giocatore players[MAX_PLAYERS];
    int  game_over;                         /* 1 se la partita e' finita */
    int  winner;                            /* Indice del vincitore (-1 se nessuno) */
    int  timeout;                           /* Durata massima partita in secondi */
    int  interval;                          /* Intervallo invio mappa globale */
    time_t start_time;                      /* Timestamp avvio partita */
    int  listen_fd;                         /* Socket di ascolto */
    pthread_mutex_t mutex;                  /* Unico mutex globale per la sincronizzazione */
} StatoGioco;

extern StatoGioco g_game;

/* Inizializza il labirinto e le strutture di gioco */
void game_init(int timeout, int interval);

/* Aggiunge un giocatore. Ritorna l'indice dello slot, oppure:
 *  -1 se partita terminata, -2 se nickname gia' in uso, -3 se server pieno */
int  game_add_player(int fd, const char *nickname);

/* Rimuove un giocatore e ne chiude la socket */
void game_remove_player(int index);

/* Esegue il movimento di un giocatore ('w','a','s','d') */
void game_move(int index, char direction);

/* Funzioni di invio messaggi */
void game_send_local_map(int index, const char *notice);
void game_send_global_map(int index);
void game_send_player_list(int index);
void game_send_info(int index, const char *text);

/* Invio in broadcast a tutti i giocatori attivi */
void game_broadcast_global_maps(void);
void game_broadcast_game_over(void);

/* Determina il vincitore e imposta lo stato di fine partita. Ritorna 1 se terminata */
int  game_finish(void);

#endif
