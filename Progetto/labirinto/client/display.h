#ifndef DISPLAY_H
#define DISPLAY_H

#include "protocol.h"

void display_help(void);
void display_local_map(int row, int col, int score, int rows, int cols, const char *grid);
void display_global_map(int rows, int cols, const char *grid);
void display_player_list(const PlayerEntry *entries, int count);

#endif
