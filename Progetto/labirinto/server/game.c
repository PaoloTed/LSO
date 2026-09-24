#include "game.h"
#include "log.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

GameState g_game;

/* Spostamenti (in celle) lungo le quattro direzioni, usati per scavare il labirinto. */
static const int DIRECTIONS[4][2] = { {-2, 0}, {2, 0}, {0, -2}, {0, 2} };

/* Converte un intero in network byte order dentro un buffer. */
static void put_u32(unsigned char *p, uint32_t v) {
    v = htonl(v);
    memcpy(p, &v, 4);
}

/* ---------------------------------------------------------------- labirinto */

static void generate_maze(void) {
    Maze *m = &g_game.maze;

    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            m->cells[r][c] = CELL_WALL;
            m->objects[r][c] = 0;
        }
    }

    /* DFS iterativo (recursive backtracker): scava corridoi tra le celle dispari. */
    int stack_r[MAP_ROWS * MAP_COLS];
    int stack_c[MAP_ROWS * MAP_COLS];
    int top = 0;

    m->cells[1][1] = CELL_FREE;
    stack_r[top] = 1;
    stack_c[top] = 1;
    top++;

    while (top > 0) {
        int r = stack_r[top - 1];
        int c = stack_c[top - 1];
        int cand[4], n = 0;

        for (int d = 0; d < 4; d++) {
            int nr = r + DIRECTIONS[d][0];
            int nc = c + DIRECTIONS[d][1];
            if (nr > 0 && nr < MAP_ROWS - 1 && nc > 0 && nc < MAP_COLS - 1 &&
                m->cells[nr][nc] == CELL_WALL)
                cand[n++] = d;
        }

        if (n == 0) {
            top--;
            continue;
        }

        int d = cand[rand() % n];
        int nr = r + DIRECTIONS[d][0];
        int nc = c + DIRECTIONS[d][1];
        m->cells[(r + nr) / 2][(c + nc) / 2] = CELL_FREE;
        m->cells[nr][nc] = CELL_FREE;
        stack_r[top] = nr;
        stack_c[top] = nc;
        top++;
    }

    /* Apre qualche muro interno per creare percorsi alternativi (loop). */
    int loops = 30;
    int guard = 0;
    while (loops > 0 && guard++ < 10000) {
        int r = 1 + rand() % (MAP_ROWS - 2);
        int c = 1 + rand() % (MAP_COLS - 2);
        if (m->cells[r][c] != CELL_WALL)
            continue;

        int free_neigh = 0;
        if (m->cells[r - 1][c] == CELL_FREE) free_neigh++;
        if (m->cells[r + 1][c] == CELL_FREE) free_neigh++;
        if (m->cells[r][c - 1] == CELL_FREE) free_neigh++;
        if (m->cells[r][c + 1] == CELL_FREE) free_neigh++;

        if (free_neigh >= 2) {
            m->cells[r][c] = CELL_FREE;
            loops--;
        }
    }

    /* Posiziona alcune uscite sul bordo, dove esiste una cella libera adiacente. */
    int exits = 0;
    guard = 0;
    while (exits < 3 && guard++ < 10000) {
        int r = rand() % MAP_ROWS;
        int c = rand() % MAP_COLS;
        int border = (r == 0 || r == MAP_ROWS - 1 || c == 0 || c == MAP_COLS - 1);
        if (!border || m->cells[r][c] != CELL_WALL)
            continue;

        int ir = r, ic = c;
        if (r == 0) ir = 1;
        else if (r == MAP_ROWS - 1) ir = MAP_ROWS - 2;
        else if (c == 0) ic = 1;
        else if (c == MAP_COLS - 1) ic = MAP_COLS - 2;

        if (m->cells[ir][ic] == CELL_FREE) {
            m->cells[r][c] = CELL_EXIT;
            exits++;
        }
    }

    /* Distribuisce gli oggetti raccoglibili nelle celle libere. */
    int objects = 15;
    guard = 0;
    while (objects > 0 && guard++ < 10000) {
        int r = 1 + rand() % (MAP_ROWS - 2);
        int c = 1 + rand() % (MAP_COLS - 2);
        if (m->cells[r][c] == CELL_FREE && !m->objects[r][c]) {
            m->objects[r][c] = 1;
            objects--;
        }
    }
}

/* ------------------------------------------------------------ invio sicuro */

/* Invia un messaggio a un giocatore serializzando l'accesso alla socket. */
static int player_send_message(int index, uint8_t type, const void *payload, uint16_t len) {
    Player *p = &g_game.players[index];

    pthread_mutex_lock(&p->send_mtx);

    pthread_mutex_lock(&g_game.mutex);
    int active = p->active;
    int fd = p->fd;
    pthread_mutex_unlock(&g_game.mutex);

    int rc = -1;
    if (active)
        rc = send_message(fd, type, payload, len);

    pthread_mutex_unlock(&p->send_mtx);
    return rc;
}

static void player_send_text(int index, uint8_t type, const char *text) {
    TextPayload t;
    memset(&t, 0, sizeof(t));
    strncpy(t.text, text, MAX_TEXT - 1);
    player_send_message(index, type, &t, (uint16_t)sizeof(t));
}

/* --------------------------------------------------------------- mappe */

/* Costruisce la griglia locale (finestra centrata sul giocatore).
 * Va chiamata con g_game.mutex bloccato. */
static void build_local_grid(const Player *p, char *grid) {
    for (int wr = 0; wr < LOCAL_VIEW; wr++) {
        for (int wc = 0; wc < LOCAL_VIEW; wc++) {
            int r = p->row - VIEW_RADIUS + wr;
            int c = p->col - VIEW_RADIUS + wc;
            char ch;

            if (r < 0 || r >= MAP_ROWS || c < 0 || c >= MAP_COLS)
                ch = CELL_WALL;
            else if (!p->discovered[r][c])
                ch = CELL_HIDDEN;
            else if (r == p->row && c == p->col)
                ch = CELL_PLAYER;
            else if (g_game.maze.objects[r][c])
                ch = CELL_OBJECT;
            else
                ch = g_game.maze.cells[r][c];

            grid[wr * LOCAL_VIEW + wc] = ch;
        }
    }
}

/* Costruisce la mappa globale mascherata. Va chiamata con g_game.mutex bloccato. */
static void build_global_grid(const Player *p, char *grid) {
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            char ch;

            if (!p->discovered[r][c])
                ch = CELL_HIDDEN;
            else if (r == p->row && c == p->col)
                ch = CELL_PLAYER;
            else if (g_game.maze.objects[r][c])
                ch = CELL_OBJECT;
            else
                ch = g_game.maze.cells[r][c];

            grid[r * MAP_COLS + c] = ch;
        }
    }
}

void game_send_local_map(int index) {
    unsigned char payload[LOCAL_MAP_HDR + LOCAL_VIEW * LOCAL_VIEW];
    int len = 0;

    pthread_mutex_lock(&g_game.mutex);
    Player *p = &g_game.players[index];
    if (p->active) {
        put_u32(payload + 0, (uint32_t)p->row);
        put_u32(payload + 4, (uint32_t)p->col);
        put_u32(payload + 8, (uint32_t)p->score);
        put_u32(payload + 12, LOCAL_VIEW);
        put_u32(payload + 16, LOCAL_VIEW);
        build_local_grid(p, (char *)payload + LOCAL_MAP_HDR);
        len = LOCAL_MAP_HDR + LOCAL_VIEW * LOCAL_VIEW;
    }
    pthread_mutex_unlock(&g_game.mutex);

    if (len > 0)
        player_send_message(index, MSG_LOCAL_MAP, payload, (uint16_t)len);
}

static void broadcast_global_maps(void) {
    unsigned char payload[GLOBAL_MAP_HDR + MAP_ROWS * MAP_COLS];
    int len = GLOBAL_MAP_HDR + MAP_ROWS * MAP_COLS;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        pthread_mutex_lock(&g_game.mutex);
        Player *p = &g_game.players[i];
        int active = p->active;
        if (active) {
            put_u32(payload + 0, MAP_ROWS);
            put_u32(payload + 4, MAP_COLS);
            build_global_grid(p, (char *)payload + GLOBAL_MAP_HDR);
        }
        pthread_mutex_unlock(&g_game.mutex);

        if (active)
            player_send_message(i, MSG_GLOBAL_MAP, payload, (uint16_t)len);
    }
}

void game_send_player_list(int index) {
    unsigned char payload[MAX_PAYLOAD];
    int count = 0;

    pthread_mutex_lock(&g_game.mutex);
    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *p = &g_game.players[i];
        if (!p->active)
            continue;

        PlayerEntry entry;
        memset(&entry, 0, sizeof(entry));
        strncpy(entry.nickname, p->nickname, MAX_NICK - 1);
        entry.score = htonl((uint32_t)p->score);
        entry.exited = htonl((uint32_t)p->exited);

        memcpy(payload + 4 + count * sizeof(PlayerEntry), &entry, sizeof(entry));
        count++;
    }
    put_u32(payload, (uint32_t)count);
    pthread_mutex_unlock(&g_game.mutex);

    int len = 4 + count * (int)sizeof(PlayerEntry);
    player_send_message(index, MSG_PLAYER_LIST, payload, (uint16_t)len);
}

/* ------------------------------------------------------------- partita */

/* Deve essere chiamata con g_game.mutex bloccato. */
static int all_exited(void) {
    int any = 0;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *p = &g_game.players[i];
        if (!p->active || !p->joined)
            continue;
        any = 1;
        if (!p->exited)
            return 0;
    }
    return any;
}

/* Deve essere chiamata con g_game.mutex bloccato. */
static int find_winner_locked(void) {
    int exited_count = 0, exited_idx = -1;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *p = &g_game.players[i];
        if (p->joined && p->exited) {
            exited_count++;
            exited_idx = i;
        }
    }

    /* Un solo giocatore uscito: vince lui. */
    if (exited_count == 1)
        return exited_idx;

    /* Altrimenti vince chi ha raccolto piu' oggetti. */
    int best = -1, best_score = -1;
    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *p = &g_game.players[i];
        if (!p->joined)
            continue;

        int better = p->score > best_score;
        int tie_and_exited = (p->score == best_score && p->exited &&
                              best >= 0 && !g_game.players[best].exited);
        if (better || tie_and_exited) {
            best_score = p->score;
            best = i;
        }
    }
    return best;
}

/* Termina la partita. Ritorna 1 se la partita e' appena terminata.
 * Deve essere chiamata con g_game.mutex bloccato. */
static int finish_game_locked(void) {
    if (g_game.game_over)
        return 0;

    g_game.game_over = 1;
    g_game.winner = find_winner_locked();

    return 1;
}

static void broadcast_game_over(void) {
    unsigned char payload[MAX_NICK + 4];
    char nick[MAX_NICK];
    int score = 0;

    pthread_mutex_lock(&g_game.mutex);
    if (g_game.winner >= 0) {
        strncpy(nick, g_game.players[g_game.winner].nickname, MAX_NICK - 1);
        nick[MAX_NICK - 1] = '\0';
        score = g_game.players[g_game.winner].score;
        log_event("Fine partita. Vincitore: %s con %d oggetti.", nick, score);
    } else {
        memset(nick, 0, sizeof(nick));
        log_event("Fine partita. Nessun vincitore.");
    }
    pthread_mutex_unlock(&g_game.mutex);

    memset(payload, 0, sizeof(payload));
    memcpy(payload, nick, MAX_NICK);
    put_u32(payload + MAX_NICK, (uint32_t)score);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        pthread_mutex_lock(&g_game.mutex);
        int active = g_game.players[i].active;
        pthread_mutex_unlock(&g_game.mutex);

        if (active)
            player_send_message(i, MSG_GAME_OVER, payload, (uint16_t)sizeof(payload));
    }
}

/* Rivela le celle attorno al giocatore. Da chiamare con g_game.mutex bloccato. */
static void reveal_around(Player *p) {
    for (int dr = -VIEW_RADIUS; dr <= VIEW_RADIUS; dr++) {
        for (int dc = -VIEW_RADIUS; dc <= VIEW_RADIUS; dc++) {
            int r = p->row + dr;
            int c = p->col + dc;
            if (r >= 0 && r < MAP_ROWS && c >= 0 && c < MAP_COLS)
                p->discovered[r][c] = 1;
        }
    }
}

void game_move(int index, char direction) {
    int invalid = 0, collected = 0, reached_exit = 0, transition = 0;
    char nick[MAX_NICK];
    int row = 0, col = 0, score = 0;

    pthread_mutex_lock(&g_game.mutex);
    Player *p = &g_game.players[index];

    if (!p->active || p->exited || g_game.game_over) {
        pthread_mutex_unlock(&g_game.mutex);
        return;
    }

    int nr = p->row, nc = p->col;
    switch (direction) {
        case 'w': case 'W': nr--; break;
        case 's': case 'S': nr++; break;
        case 'a': case 'A': nc--; break;
        case 'd': case 'D': nc++; break;
        default: invalid = 1; break;
    }

    if (!invalid &&
        (nr < 0 || nr >= MAP_ROWS || nc < 0 || nc >= MAP_COLS ||
         g_game.maze.cells[nr][nc] == CELL_WALL))
        invalid = 1;

    if (!invalid) {
        p->row = nr;
        p->col = nc;
        reveal_around(p);

        if (g_game.maze.objects[nr][nc]) {
            g_game.maze.objects[nr][nc] = 0;
            p->score++;
            collected = 1;
        }

        if (g_game.maze.cells[nr][nc] == CELL_EXIT) {
            p->exited = 1;
            reached_exit = 1;
        }

        if ((collected || reached_exit) && all_exited())
            transition = finish_game_locked();
    }

    strncpy(nick, p->nickname, MAX_NICK - 1);
    nick[MAX_NICK - 1] = '\0';
    row = p->row;
    col = p->col;
    score = p->score;
    pthread_mutex_unlock(&g_game.mutex);

    if (invalid)
        player_send_text(index, MSG_INFO, "Mossa non valida: c'e' un muro.");

    if (collected) {
        log_event("%s ha raccolto un oggetto in (%d,%d), punteggio=%d.",
                  nick, row, col, score);
        player_send_text(index, MSG_INFO, "Hai raccolto un oggetto!");
    }
    if (reached_exit) {
        log_event("%s ha raggiunto l'uscita, punteggio=%d.", nick, score);
        player_send_text(index, MSG_INFO, "Hai raggiunto l'uscita!");
    }

    game_send_local_map(index);

    if (transition)
        broadcast_game_over();
}

/* ------------------------------------------------------------- timer */

static void *timer_thread(void *arg) {
    (void)arg;

    for (;;) {
        sleep((unsigned)g_game.interval);

        pthread_mutex_lock(&g_game.mutex);
        int over = g_game.game_over;
        int transition = 0;

        if (!over && difftime(time(NULL), g_game.start_time) >= (double)g_game.timeout) {
            log_event("Timeout della partita scaduto.");
            transition = finish_game_locked();
            over = g_game.game_over;
        } else {
            over = g_game.game_over;
        }
        pthread_mutex_unlock(&g_game.mutex);

        if (over) {
            if (transition)
                broadcast_game_over();
            break;
        }

        broadcast_global_maps();
    }
    return NULL;
}

void game_start_timer(void) {
    pthread_t tid;
    pthread_create(&tid, NULL, timer_thread, NULL);
    pthread_detach(tid);
}

/* -------------------------------------------------------- ciclo di vita */

void game_init(int timeout, int interval) {
    memset(&g_game, 0, sizeof(g_game));
    g_game.timeout = timeout;
    g_game.interval = interval;
    g_game.winner = -1;

    pthread_mutex_init(&g_game.mutex, NULL);
    for (int i = 0; i < MAX_PLAYERS; i++)
        pthread_mutex_init(&g_game.players[i].send_mtx, NULL);

    srand((unsigned)time(NULL));
    generate_maze();
    g_game.start_time = time(NULL);
}

int game_add_player(int fd, const char *nickname) {
    pthread_mutex_lock(&g_game.mutex);

    if (g_game.game_over) {
        pthread_mutex_unlock(&g_game.mutex);
        return -1;
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *p = &g_game.players[i];
        if (p->active && strcmp(p->nickname, nickname) == 0) {
            pthread_mutex_unlock(&g_game.mutex);
            return -2;
        }
    }

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

    /* Cerca una cella libera non occupata da altri giocatori. */
    int row = -1, col = -1;
    for (int t = 0; t < 5000 && row < 0; t++) {
        int r = 1 + rand() % (MAP_ROWS - 2);
        int c = 1 + rand() % (MAP_COLS - 2);
        if (g_game.maze.cells[r][c] != CELL_FREE || g_game.maze.objects[r][c])
            continue;

        int busy = 0;
        for (int j = 0; j < MAX_PLAYERS; j++) {
            Player *o = &g_game.players[j];
            if (o->active && o->row == r && o->col == c)
                busy = 1;
        }
        if (!busy) {
            row = r;
            col = c;
        }
    }
    if (row < 0) {
        row = 1;
        col = 1;
    }

    Player *p = &g_game.players[slot];
    p->fd = fd;
    p->active = 1;
    p->joined = 1;
    p->exited = 0;
    p->row = row;
    p->col = col;
    p->score = 0;
    memset(p->discovered, 0, sizeof(p->discovered));
    strncpy(p->nickname, nickname, MAX_NICK - 1);
    p->nickname[MAX_NICK - 1] = '\0';
    p->discovered[row][col] = 1;   /* inizialmente vede solo la propria cella */

    pthread_mutex_unlock(&g_game.mutex);

    log_event("Connessione: %s (riga=%d, colonna=%d).", nickname, row, col);
    return slot;
}

void game_remove_player(int index) {
    Player *p = &g_game.players[index];
    char nick[MAX_NICK];

    pthread_mutex_lock(&p->send_mtx);

    pthread_mutex_lock(&g_game.mutex);
    int was_active = p->active;
    int fd = p->fd;
    strncpy(nick, p->nickname, MAX_NICK - 1);
    nick[MAX_NICK - 1] = '\0';
    p->active = 0;
    pthread_mutex_unlock(&g_game.mutex);

    if (was_active) {
        close(fd);
        log_event("Disconnessione: %s.", nick);
    }

    pthread_mutex_unlock(&p->send_mtx);
}
