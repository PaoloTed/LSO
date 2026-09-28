#include "game.h"
#include "log.h"
#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

StatoGioco g_game;

/* Template generico del labirinto (21 righe x 41 colonne, 100% connesso) */
static const char MAZE_TEMPLATE[MAP_ROWS][MAP_COLS + 1] = {
    "#########################################",
    "#.#.....#.....#...#...............#.#...#",
    "#.###.#.#.###.#.#.#.#.#####.#####.#.#.#.#",
    "#...#.#...#...#.#...#.....#.#...#.#...#.#",
    "###.#.#.#######.###.#.#####.###.#.#####.#",
    "#.#.#.#.#.....#.#.....#...#.#...#.#.....#",
    "#.#.#.###.###.#.#####.#.#.#.#.#.#.#.###.#",
    "#.#.#...#.#.#.#...#...#.#.#.#.#.#...#...#",
    "#.#.#...#.#.#.###.#.#...#.#.#.#######.###",
    "#.#.#.#...#.#.......#.#.#.#...........#.#",
    "#.#.#.#####.###########.#.#.###.#.###.#.#",
    "#...#.#...#...........#.#.#...#.....#...#",
    "#.###.#.#.#.#.###.#.#.#...#.#.#####.###.#",
    "#.#.....#...........#...#.#.#.....#...#.#",
    "#.#####.#####.###.#.###.#.#.###.#.###.#.#",
    "#...#...#...#.#.....#...#.....#.#...#.#.#",
    "###.#####.#.#.#.#.#.#.#.#####.#####.#...#",
    "#.#.......#.#.#...#.#.#.#...#.....#.#...#",
    "#.#########.#.#.#.###.#...#.#####.#.###.#",
    "#.............#.#.....#...#.........#...#",
    "#########################################"
};

/* 10 uscite predefinite distribuite sul perimetro del labirinto */
static const struct {
    int r, c;
} USCITE_PREDEFINITE[10] = {
    {0, 5},   {0, 20},  {0, 35},   /* 3 sul bordo superiore */
    {20, 5},  {20, 20}, {20, 35},  /* 3 sul bordo inferiore */
    {5, 0},   {15, 0},             /* 2 sul bordo sinistro */
    {5, 40},  {15, 40}             /* 2 sul bordo destro */
};

/* Genera il labirinto dal template, randomizzando uscite e oggetti */
static void generate_maze(Labirinto *m) {
    /* 1. Copia la struttura del labirinto dal template */
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            m->cells[r][c] = MAZE_TEMPLATE[r][c];
            m->objects[r][c] = 0;
        }
    }

    /* 2. Sceglie casualmente 4 uscite distinte tra le 10 predefinite */
    int uscite_scelte = 0;
    while (uscite_scelte < 4) {
        int idx = rand() % 10;
        int r = USCITE_PREDEFINITE[idx].r;
        int c = USCITE_PREDEFINITE[idx].c;
        if (m->cells[r][c] != CELL_EXIT) {
            m->cells[r][c] = CELL_EXIT;
            uscite_scelte++;
        }
    }

    /* 3. Distribuisce casualmente 15 oggetti raccoglibili 'O' su celle libere */
    int objects = 15;
    while (objects > 0) {
        int r = 1 + rand() % (MAP_ROWS - 2);
        int c = 1 + rand() % (MAP_COLS - 2);
        if (m->cells[r][c] == CELL_FREE && m->objects[r][c] == 0) {
            m->objects[r][c] = 1;
            objects--;
        }
    }
}

void game_init(int timeout, int interval) {
    srand((unsigned)time(NULL));
    memset(&g_game, 0, sizeof(g_game));
    pthread_mutex_init(&g_game.mutex, NULL);
    g_game.timeout = timeout;
    g_game.interval = interval;
    g_game.winner = -1;
    g_game.start_time = time(NULL);
    generate_maze(&g_game.maze);
}

/* Rende visibili le celle nel raggio VIEW_RADIUS attorno al giocatore */
static void reveal_around(Giocatore *p) {
    for (int dr = -VIEW_RADIUS; dr <= VIEW_RADIUS; dr++) {
        for (int dc = -VIEW_RADIUS; dc <= VIEW_RADIUS; dc++) {
            int r = p->row + dr;
            int c = p->col + dc;
            if (r >= 0 && r < MAP_ROWS && c >= 0 && c < MAP_COLS)
                p->discovered[r][c] = 1;
        }
    }
}

/* Calcola il vincitore (da chiamare con g_game.mutex bloccato) */
static int find_winner_locked(void) {
    int exited_count = 0, exited_idx = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (g_game.players[i].joined && g_game.players[i].exited) {
            exited_count++;
            exited_idx = i;
        }
    }

    /* Se un solo giocatore e' uscito, vince lui */
    if (exited_count == 1)
        return exited_idx;

    /* Altrimenti vince chi ha raccolto piu' oggetti */
    int best = -1, best_score = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        Giocatore *p = &g_game.players[i];
        if (!p->joined)
            continue;
        int better = (p->score > best_score);
        int tie_and_exited = (p->score == best_score && p->exited &&
                              best >= 0 && !g_game.players[best].exited);
        if (better || tie_and_exited) {
            best_score = p->score;
            best = i;
        }
    }
    return best;
}

/* Controlla se tutti i giocatori partecipanti sono usciti */
static int all_exited_locked(void) {
    int any = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        Giocatore *p = &g_game.players[i];
        if (!p->active || !p->joined)
            continue;
        any = 1;
        if (!p->exited)
            return 0;
    }
    return any;
}

int game_finish(void) {
    pthread_mutex_lock(&g_game.mutex);
    if (g_game.game_over) {
        pthread_mutex_unlock(&g_game.mutex);
        return 0;
    }
    g_game.game_over = 1;
    g_game.winner = find_winner_locked();
    pthread_mutex_unlock(&g_game.mutex);
    return 1;
}

int game_add_player(int fd, const char *nickname) {
    pthread_mutex_lock(&g_game.mutex);

    if (g_game.game_over) {
        pthread_mutex_unlock(&g_game.mutex);
        return -1;
    }

    /* Verifica se il nickname e' gia' occupato da un giocatore connesso */
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (g_game.players[i].active && strcmp(g_game.players[i].nickname, nickname) == 0) {
            pthread_mutex_unlock(&g_game.mutex);
            return -2;
        }
    }

    /* Cerca uno slot libero */
    int slot = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (!g_game.players[i].active) {
            slot = i;
            break;
        }
    }
    if (slot < 0) {
        pthread_mutex_unlock(&g_game.mutex);
        return -3;
    }

    /* Trova una posizione iniziale libera */
    int row = 1, col = 1;
    for (int t = 0; t < 5000; t++) {
        int r = 1 + rand() % (MAP_ROWS - 2);
        int c = 1 + rand() % (MAP_COLS - 2);
        if (g_game.maze.cells[r][c] != CELL_FREE || g_game.maze.objects[r][c])
            continue;

        /* Assicura che ci sia almeno una via di movimento adiacente */
        if (g_game.maze.cells[r - 1][c] != CELL_FREE &&
            g_game.maze.cells[r + 1][c] != CELL_FREE &&
            g_game.maze.cells[r][c - 1] != CELL_FREE &&
            g_game.maze.cells[r][c + 1] != CELL_FREE)
            continue;

        int busy = 0;
        for (int j = 0; j < MAX_PLAYERS; j++) {
            if (g_game.players[j].active && g_game.players[j].row == r && g_game.players[j].col == c)
                busy = 1;
        }
        if (!busy) {
            row = r;
            col = c;
            break;
        }
    }

    Giocatore *p = &g_game.players[slot];
    p->fd = fd;
    p->active = 1;
    p->joined = 1;
    p->exited = 0;
    p->row = row;
    p->col = col;
    p->score = 0;
    strncpy(p->nickname, nickname, MAX_NICK - 1);
    memset(p->discovered, 0, sizeof(p->discovered));
    reveal_around(p);

    pthread_mutex_unlock(&g_game.mutex);

    log_event("Nuova connessione: '%s' (riga %d, colonna %d).", nickname, row, col);
    return slot;
}

void game_remove_player(int index) {
    char nick[MAX_NICK];
    int fd = -1;

    pthread_mutex_lock(&g_game.mutex);
    if (g_game.players[index].active) {
        g_game.players[index].active = 0;
        fd = g_game.players[index].fd;
        strncpy(nick, g_game.players[index].nickname, MAX_NICK - 1);
    }
    pthread_mutex_unlock(&g_game.mutex);

    if (fd >= 0) {
        close(fd);
        log_event("Disconnessione: '%s'.", nick);
    }
}

void game_move(int index, char direction) {
    pthread_mutex_lock(&g_game.mutex);
    Giocatore *p = &g_game.players[index];

    if (!p->active || p->exited || g_game.game_over) {
        pthread_mutex_unlock(&g_game.mutex);
        return;
    }

    int nr = p->row, nc = p->col;
    int invalid = 0;
    switch (direction) {
        case 'w': case 'W': nr--; break;
        case 's': case 'S': nr++; break;
        case 'a': case 'A': nc--; break;
        case 'd': case 'D': nc++; break;
        default: invalid = 1; break;
    }

    if (invalid || nr < 0 || nr >= MAP_ROWS || nc < 0 || nc >= MAP_COLS ||
        g_game.maze.cells[nr][nc] == CELL_WALL) {
        pthread_mutex_unlock(&g_game.mutex);
        game_send_local_map(index, "Sei andato verso un muro, prova un'altra direzione.");
        return;
    }

    p->row = nr;
    p->col = nc;
    reveal_around(p);

    int collected = 0;
    if (g_game.maze.objects[nr][nc]) {
        g_game.maze.objects[nr][nc] = 0;
        p->score++;
        collected = 1;
    }

    int reached_exit = 0;
    if (g_game.maze.cells[nr][nc] == CELL_EXIT) {
        p->exited = 1;
        reached_exit = 1;
    }

    char nick[MAX_NICK];
    strncpy(nick, p->nickname, MAX_NICK - 1);
    int score = p->score;

    int over = 0;
    if ((collected || reached_exit) && all_exited_locked()) {
        g_game.game_over = 1;
        g_game.winner = find_winner_locked();
        over = 1;
    }
    pthread_mutex_unlock(&g_game.mutex);

    const char *notice = NULL;
    if (collected) {
        log_event("%s ha raccolto un oggetto in (%d,%d), punteggio=%d.", nick, nr, nc, score);
        notice = "Hai raccolto un oggetto! (+1 punto)";
    }
    if (reached_exit) {
        log_event("%s ha raggiunto l'uscita con punteggio=%d.", nick, score);
        notice = "Hai raggiunto l'uscita!";
    }

    game_send_local_map(index, notice);

    if (over)
        game_broadcast_game_over();
}

void game_send_local_map(int index, const char *notice) {
    Messaggio msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_MAPPA_LOCALE;
    if (notice)
        strncpy(msg.text, notice, MAX_TEXT - 1);

    pthread_mutex_lock(&g_game.mutex);
    Giocatore *p = &g_game.players[index];
    if (!p->active) {
        pthread_mutex_unlock(&g_game.mutex);
        return;
    }

    msg.row = p->row;
    msg.col = p->col;
    msg.score = p->score;

    /* Costruisce la vista locale 5x5 */
    for (int dr = -VIEW_RADIUS; dr <= VIEW_RADIUS; dr++) {
        for (int dc = -VIEW_RADIUS; dc <= VIEW_RADIUS; dc++) {
            int r = p->row + dr;
            int c = p->col + dc;
            int vr = dr + VIEW_RADIUS;
            int vc = dc + VIEW_RADIUS;

            if (r < 0 || r >= MAP_ROWS || c < 0 || c >= MAP_COLS || !p->discovered[r][c])
                msg.local_map[vr][vc] = CELL_HIDDEN;
            else if (r == p->row && c == p->col)
                msg.local_map[vr][vc] = CELL_PLAYER;
            else if (g_game.maze.objects[r][c])
                msg.local_map[vr][vc] = CELL_OBJECT;
            else
                msg.local_map[vr][vc] = g_game.maze.cells[r][c];
        }
    }
    int fd = p->fd;
    pthread_mutex_unlock(&g_game.mutex);

    invia_messaggio(fd, &msg);
}

void game_send_global_map(int index) {
    Messaggio msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_MAPPA_GLOBALE;

    pthread_mutex_lock(&g_game.mutex);
    Giocatore *p = &g_game.players[index];
    if (!p->active) {
        pthread_mutex_unlock(&g_game.mutex);
        return;
    }

    msg.row = p->row;
    msg.col = p->col;
    msg.score = p->score;

    /* Costruisce la vista locale 5x5 */
    for (int dr = -VIEW_RADIUS; dr <= VIEW_RADIUS; dr++) {
        for (int dc = -VIEW_RADIUS; dc <= VIEW_RADIUS; dc++) {
            int r = p->row + dr;
            int c = p->col + dc;
            int vr = dr + VIEW_RADIUS;
            int vc = dc + VIEW_RADIUS;

            if (r < 0 || r >= MAP_ROWS || c < 0 || c >= MAP_COLS || !p->discovered[r][c])
                msg.local_map[vr][vc] = CELL_HIDDEN;
            else if (r == p->row && c == p->col)
                msg.local_map[vr][vc] = CELL_PLAYER;
            else if (g_game.maze.objects[r][c])
                msg.local_map[vr][vc] = CELL_OBJECT;
            else
                msg.local_map[vr][vc] = g_game.maze.cells[r][c];
        }
    }

    /* Costruisce la mappa globale mascherata */
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            if (!p->discovered[r][c])
                msg.global_map[r][c] = CELL_HIDDEN;
            else if (r == p->row && c == p->col)
                msg.global_map[r][c] = CELL_PLAYER;
            else if (g_game.maze.objects[r][c])
                msg.global_map[r][c] = CELL_OBJECT;
            else
                msg.global_map[r][c] = g_game.maze.cells[r][c];
        }
    }
    int fd = p->fd;
    pthread_mutex_unlock(&g_game.mutex);

    invia_messaggio(fd, &msg);
}

void game_send_player_list(int index) {
    Messaggio msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_LISTA;

    pthread_mutex_lock(&g_game.mutex);
    int count = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        Giocatore *p = &g_game.players[i];
        if (!p->active)
            continue;
        strncpy(msg.players[count].nickname, p->nickname, MAX_NICK - 1);
        msg.players[count].score = p->score;
        msg.players[count].exited = p->exited;
        count++;
    }
    msg.num_players = count;
    int fd = g_game.players[index].fd;
    pthread_mutex_unlock(&g_game.mutex);

    invia_messaggio(fd, &msg);
}

void game_send_info(int index, const char *text) {
    Messaggio msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_INFO;
    strncpy(msg.text, text, MAX_TEXT - 1);

    pthread_mutex_lock(&g_game.mutex);
    int fd = g_game.players[index].active ? g_game.players[index].fd : -1;
    pthread_mutex_unlock(&g_game.mutex);

    if (fd >= 0)
        invia_messaggio(fd, &msg);
}

void game_broadcast_global_maps(void) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        pthread_mutex_lock(&g_game.mutex);
        int active = g_game.players[i].active;
        pthread_mutex_unlock(&g_game.mutex);
        if (active)
            game_send_global_map(i);
    }
}

void game_broadcast_game_over(void) {
    Messaggio msg;
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_FINE_PARTITA;

    pthread_mutex_lock(&g_game.mutex);
    if (g_game.winner >= 0) {
        strncpy(msg.nickname, g_game.players[g_game.winner].nickname, MAX_NICK - 1);
        msg.score = g_game.players[g_game.winner].score;
        log_event("Fine partita. Vincitore: %s con %d oggetti.", msg.nickname, msg.score);
    } else {
        log_event("Fine partita. Nessun vincitore.");
    }

    int fds[MAX_PLAYERS];
    int count = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (g_game.players[i].active)
            fds[count++] = g_game.players[i].fd;
    }
    pthread_mutex_unlock(&g_game.mutex);

    for (int i = 0; i < count; i++)
        invia_messaggio(fds[i], &msg);
}
