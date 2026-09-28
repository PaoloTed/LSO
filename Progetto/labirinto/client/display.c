#include "display.h"
#include <stdio.h>
#include <stdlib.h>

/* Colori ANSI per il terminale */
#define COL_BLUE   "\033[34m" /* Blu */
#define COL_RED    "\033[31m" /* Rosso */
#define COL_YELLOW "\033[33m" /* Giallo */
#define COL_RESET  "\033[0m"  /* Ripristina i colori di default del terminale*/

static void print_cell(char ch, int in_local_view) {
    if (ch == CELL_PLAYER)
        printf(COL_RED "%c " COL_RESET, ch);
    else if (ch == CELL_OBJECT)
        printf(COL_YELLOW "%c " COL_RESET, ch);
    else if (in_local_view)
        printf(COL_BLUE "%c " COL_RESET, ch);
    else
        printf("%c ", ch);
}

void display_help(void) {
    printf("\nComandi disponibili:\n"
           "  w / a / s / d : muovi su / sinistra / giu' / destra\n"
           "  l             : lista dei giocatori connessi\n"
           "  h             : mostra questo aiuto\n"
           "  q             : esci dal gioco\n\n");
}

/* Stampa la mappa locale (se clear_screen != 0 pulisce prima lo schermo con system("clear")) */
void display_local_map(int row, int col, int score, const char map[LOCAL_VIEW][LOCAL_VIEW], const char *notice, int clear_screen) {
    if (clear_screen)
        system("clear");

    printf("\nPosizione: (riga %d, colonna %d)   Oggetti raccolti: %d\n", row, col, score);
    for (int r = 0; r < LOCAL_VIEW; r++) {
        printf("  ");
        for (int c = 0; c < LOCAL_VIEW; c++)
            print_cell(map[r][c], 1);
        putchar('\n');
    }
    if (notice && notice[0] != '\0') {
        printf(COL_YELLOW "\n%s" COL_RESET "\n", notice);
    }
    printf("\nComandi: [w/a/s/d] muovi | [l] lista giocatori | [h] aiuto | [q] esci\n");
    printf("Inserisci il comando + ENTER: ");
    fflush(stdout);
}

/* Stampa la mappa globale pulendo prima lo schermo con system("clear") */
void display_global_map(int row, int col, const char map[MAP_ROWS][MAP_COLS]) {
    system("clear");
    printf("\n--- Mappa globale (celle non ancora viste: '%c') ---\n", CELL_HIDDEN);

    for (int r = 0; r < MAP_ROWS; r++) {
        printf("  ");
        for (int c = 0; c < MAP_COLS; c++) {
            int in_view = (r >= row - VIEW_RADIUS && r <= row + VIEW_RADIUS &&
                           c >= col - VIEW_RADIUS && c <= col + VIEW_RADIUS);
            print_cell(map[r][c], in_view);
        }
        putchar('\n');
    }
}

void display_player_list(const InfoGiocatore *players, int count) {
    printf("\nGiocatori connessi (%d):\n", count);
    for (int i = 0; i < count; i++) {
        printf("  - %s: %d oggetti (%s)\n", players[i].nickname, players[i].score, players[i].exited ? "disconnesso" : "connesso");
    }
}
