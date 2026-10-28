#ifndef GAME_H
#define GAME_H

#include <pthread.h>
#include <time.h>
#include "protocol.h"

/* Struttura del labirinto */
typedef struct {
    char celle[RIGHE_MAPPA][COLONNE_MAPPA];
    char oggetti[RIGHE_MAPPA][COLONNE_MAPPA];
} Labirinto;

/* Dati del giocatore */
typedef struct {
    int  fd;                                        /* Socket */
    int  attivo;                                    /* Giocatore attivo */
    int  partecipante;                              /* Partecipante */
    int  uscito;                                    /* Raggiunta uscita */
    char nickname[MAX_NICK];
    int  riga, colonna;                             /* Posizione */
    int  punteggio;                                 /* Punteggio */
    char scoperte[RIGHE_MAPPA][COLONNE_MAPPA];      /* Celle scoperte */
} Giocatore;

/* Stato globale della partita */
typedef struct {
    Labirinto labirinto;
    Giocatore *giocatori;                           /* Array giocatori */
    int  num_giocatori;                             /* Numero giocatori */
    int  partita_finita;                            /* Fine partita (valore 1 se terminata) */
    int  vincitore;                                 /* Indice vincitore */
    int  timeout;                                   /* Timeout partita */
    int  intervallo;                                /* Intervallo broadcast */
    pthread_mutex_t mutex;                          /* Mutex per sincronizzazione */
} StatoGioco;

extern StatoGioco partita;

void inizializza_gioco(int timeout, int intervallo);

/* Aggiunge un giocatore verificando le credenziali */
int  aggiungi_giocatore(int fd, const char *nickname, const char *password);

void rimuovi_giocatore(int indice);

void muovi_giocatore(int indice, char direzione);

/* Invio messaggi */
void invia_mappa_locale(int indice, const char *avviso);
void invia_mappa_globale(int indice);
void invia_lista_giocatori(int indice);

/* Invio broadcast */
void broadcast_mappe_globali(void);
void broadcast_fine_partita(void);

/* Conclude la partita e calcola il vincitore */
int  termina_partita(void);

#endif
