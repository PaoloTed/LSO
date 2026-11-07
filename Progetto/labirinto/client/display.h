#ifndef DISPLAY_H
#define DISPLAY_H

#include "protocol.h"

void mostra_aiuto(void);
void mostra_comandi(void);
void mostra_avviso(const char *msg);
void mostra_mappa_locale(int riga, int colonna, int punteggio,
                         const char mappa[VISTA_LOCALE][VISTA_LOCALE]);
void mostra_mappa_globale(int riga, int colonna,
                          const char mappa[RIGHE_MAPPA][COLONNE_MAPPA]);
void mostra_lista_giocatori(const InfoGiocatore *giocatori, int totale);
void mostra_schermata_uscito(int punteggio);

#endif
