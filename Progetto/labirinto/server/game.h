#ifndef GAME_H
#define GAME_H

#include <pthread.h>
#include <time.h>

#include "protocol.h"

#define MAX_PLAYERS 64

typedef struct {
    char cells[MAP_ROWS][MAP_COLS];
    char objects[MAP_ROWS][MAP_COLS];
} Maze;

typedef struct {
    int  fd;              /* socket del giocatore */
    int  active;          /* 1 se attualmente connesso */
    int  joined;          /* 1 se ha partecipato alla partita corrente */
    int  exited;          /* 1 se ha raggiunto l'uscita */
    char nickname[MAX_NICK];
    int  row, col;
    int  score;
    char discovered[MAP_ROWS][MAP_COLS];
    pthread_mutex_t send_mtx;   /* protegge send() e close() sulla socket */
} Player;

typedef struct {
    Maze maze;
    Player players[MAX_PLAYERS];
    int  game_over;
    int  winner;          /* indice del vincitore, -1 se nessuno */
    int  timeout;         /* durata massima della partita in secondi */
    int  interval;        /* intervallo di invio della mappa globale */
    time_t start_time;
    int  listen_fd;
    pthread_mutex_t mutex;   /* protegge lo stato condiviso della partita */
} GameState;

extern GameState g_game;

void game_init(int timeout, int interval);
void game_start_timer(void);

/* Aggiunge un giocatore. Ritorna l'indice dello slot, oppure:
 *   -1 partita terminata, -2 nickname gia' connesso, -3 server pieno. */
int  game_add_player(int fd, const char *nickname);

void game_remove_player(int index);
void game_move(int index, char direction);
void game_send_local_map(int index);
void game_send_player_list(int index);

#endif
