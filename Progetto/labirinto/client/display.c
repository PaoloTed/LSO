#include "display.h"

#include <stdio.h>
#include <stdlib.h>

#define COL_BLUE   "\033[34m"
#define COL_RED    "\033[31m"
#define COL_YELLOW "\033[33m"
#define COL_RESET  "\033[0m"

/* Stampa una cella scegliendo il colore: rosso per il giocatore, giallo per
 * gli oggetti, blu se appartiene alla vista locale, altrimenti senza colore. */
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
           "  m             : rivedi la mappa locale\n"
           "  g             : rivedi la mappa globale\n"
           "  h             : mostra questo aiuto\n"
           "  q             : esci dal gioco\n\n");
}

void display_local_map(int row, int col, int score, int rows, int cols, const char *grid) {
    printf("\nPosizione: (riga %d, colonna %d)   Oggetti raccolti: %d\n", row, col, score);
    for (int r = 0; r < rows; r++) {
        printf("  ");
        for (int c = 0; c < cols; c++)
            print_cell(grid[r * cols + c], 1);
        putchar('\n');
    }
}

void display_global_map(int rows, int cols, const char *grid) {
    printf("\n--- Mappa globale (celle non esplorate: '%c') ---\n", CELL_HIDDEN);

    /* Trova il giocatore per evidenziare in blu solo la sua vista locale. */
    int pr = -1, pc = -1;
    for (int r = 0; r < rows && pr < 0; r++)
        for (int c = 0; c < cols; c++)
            if (grid[r * cols + c] == CELL_PLAYER) {
                pr = r;
                pc = c;
                break;
            }

    for (int r = 0; r < rows; r++) {
        printf("  ");
        for (int c = 0; c < cols; c++) {
            int in_view = (pr >= 0 && abs(r - pr) <= VIEW_RADIUS && abs(c - pc) <= VIEW_RADIUS);
            print_cell(grid[r * cols + c], in_view);
        }
        putchar('\n');
    }
}

void display_player_list(const PlayerEntry *entries, int count) {
    printf("\nGiocatori connessi: %d\n", count);
    printf("  %-20s %8s  %s\n", "Nickname", "Oggetti", "Uscito");
    for (int i = 0; i < count; i++) {
        printf("  %-20s %8u  %s\n",
               entries[i].nickname,
               entries[i].score,
               entries[i].exited ? "si" : "no");
    }
}
