#include "display.h"
#include <stdio.h>
#include <stdlib.h>

/* Colori ANSI */
#define COL_BLU    "\033[34m" /* Blu */
#define COL_ROSSO  "\033[31m" /* Rosso */
#define COL_GIALLO "\033[33m" /* Giallo */
#define COL_RESET  "\033[0m"  /* Reset */

static void stampa_cella(char cella, int in_vista_locale) {
    if (cella == CELLA_GIOCATORE)
        printf(COL_ROSSO "%c " COL_RESET, cella);
    else if (cella == CELLA_OGGETTO)
        printf(COL_GIALLO "%c " COL_RESET, cella);
    else if (in_vista_locale)
        printf(COL_BLU "%c " COL_RESET, cella);
    else
        printf("%c ", cella);
}

void mostra_aiuto(void) {
    printf("\nComandi disponibili:\n"
           "  w / a / s / d : muovi su / sinistra / giu' / destra\n"
           "  l             : lista dei giocatori connessi\n"
           "  h             : mostra questo aiuto\n"
           "  q             : esci dal gioco\n\n");
}

/* Vista locale */
void mostra_mappa_locale(int riga, int colonna, int punteggio, const char mappa[VISTA_LOCALE][VISTA_LOCALE], const char *avviso, int pulisci_schermo) {
    if (pulisci_schermo)
        system("clear");

    printf("\nPosizione: (riga %d, colonna %d)   Oggetti raccolti: %d\n", riga, colonna, punteggio);
    for (int r = 0; r < VISTA_LOCALE; r++) {
        printf("  ");
        for (int c = 0; c < VISTA_LOCALE; c++)
            stampa_cella(mappa[r][c], 1);
        putchar('\n');
    }
    if (avviso && avviso[0] != '\0') {
        printf(COL_GIALLO "\n%s" COL_RESET "\n", avviso);
    }
    printf("\nComandi: [w/a/s/d] muovi | [l] lista giocatori | [h] aiuto | [q] esci\n");
    printf("Inserisci il comando + ENTER: ");
    fflush(stdout);
}

/* Mappa globale */
void mostra_mappa_globale(int riga, int colonna, const char mappa[RIGHE_MAPPA][COLONNE_MAPPA]) {
    system("clear");
    printf("\n--- Mappa globale (celle non ancora viste: '%c') ---\n", CELLA_NASCOSTA);

    for (int r = 0; r < RIGHE_MAPPA; r++) {
        printf("  ");
        for (int c = 0; c < COLONNE_MAPPA; c++) {
            /* Controlla se la cella si trova all'interno della vista locale */
            int in_vista = (r >= riga - RAGGIO_VISIBILITA && r <= riga + RAGGIO_VISIBILITA &&
                            c >= colonna - RAGGIO_VISIBILITA && c <= colonna + RAGGIO_VISIBILITA);
            stampa_cella(mappa[r][c], in_vista);
        }
        putchar('\n');
    }
}

void mostra_lista_giocatori(const InfoGiocatore *giocatori, int totale) {
    printf("\nGiocatori connessi (%d):\n", totale);
    for (int i = 0; i < totale; i++) {
        const char *stato = "in gioco";
        if (giocatori[i].uscito)
            stato = "uscito dal labirinto";
        printf("  - %s: %d oggetti (%s)\n", giocatori[i].nickname, giocatori[i].punteggio, stato);
    }
}
