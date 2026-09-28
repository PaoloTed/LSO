#ifndef DISPLAY_H
#define DISPLAY_H

#include "protocol.h"

void display_help(void);
void display_local_map(int row, int col, int score, const char map[LOCAL_VIEW][LOCAL_VIEW], const char *notice, int clear_screen);
void display_global_map(int row, int col, const char map[MAP_ROWS][MAP_COLS]);
void display_player_list(const InfoGiocatore *players, int count);

#endif
