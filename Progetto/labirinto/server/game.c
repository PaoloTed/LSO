#include "game.h"
#include "log.h"
#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

StatoGioco g_game;

/* Punti di partenza per la generazione: 4 angoli e 4 centri dei lati (coordinate interne) */
static const struct {
    int r, c;
} PUNTI_PARTENZA[8] = {
    {1, 1},                       /* Angolo superiore sinistro */
    {1, MAP_COLS - 2},            /* Angolo superiore destro */
    {MAP_ROWS - 2, 1},            /* Angolo inferiore sinistro */
    {MAP_ROWS - 2, MAP_COLS - 2}, /* Angolo inferiore destro */
    {1, MAP_COLS / 2},            /* Centro lato superiore */
    {MAP_ROWS - 2, MAP_COLS / 2}, /* Centro lato inferiore */
    {MAP_ROWS / 2, 1},            /* Centro lato sinistro */
    {MAP_ROWS / 2, MAP_COLS - 2}  /* Centro lato destro */
};

/* Genera proceduralmente il labirinto con cammini casuali a croce dagli 8 punti */
static void generate_maze(Labirinto *m) {
    /* 1. Inizializza l'intera matrice con muri '#' */
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            m->cells[r][c] = CELL_WALL;
            m->objects[r][c] = 0;
        }
    }

    /* Centro del labirinto */
    int center_r = MAP_ROWS / 2;
    int center_c = MAP_COLS / 2;
    m->cells[center_r][center_c] = CELL_FREE;

    /* Direzioni a croce: Su, Giù, Sinistra, Destra */
    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    /* 2. Scava partendo dagli 8 punti (angoli e punti centrali dei lati) */
    for (int i = 0; i < 8; i++) {
        int r = PUNTI_PARTENZA[i].r;
        int c = PUNTI_PARTENZA[i].c;
        m->cells[r][c] = CELL_FREE;

        /* Corridoio verso il centro per garantire mappa sempre connessa */
        int cr = r, cc = c;
        while (cr != center_r || cc != center_c) {
            if (cr < center_r) cr++;
            else if (cr > center_r) cr--;
            else if (cc < center_c) cc++;
            else if (cc > center_c) cc--;
            m->cells[cr][cc] = CELL_FREE;
        }

        /* Walk a croce: scava celle libere con pari probabilità nelle 4 direzioni */
        int curr_r = r;
        int curr_c = c;
        int passi = 100;
        for (int step = 0; step < passi; step++) {
            int dir = rand() % 4;
            int nr = curr_r + dr[dir];
            int nc = curr_c + dc[dir];

            /* Se incontra i limiti esterni non li oltrepassa */
            if (nr <= 0 || nr >= MAP_ROWS - 1 || nc <= 0 || nc >= MAP_COLS - 1)
                continue;

            curr_r = nr;
            curr_c = nc;
            m->cells[curr_r][curr_c] = CELL_FREE;
        }
    }

    /* 3. Posiziona 4 uscite 'E' sul perimetro collegate ai corridoi dei punti centrali */
    m->cells[0][MAP_COLS / 2] = CELL_EXIT;            /* Uscita superiore */
    m->cells[MAP_ROWS - 1][MAP_COLS / 2] = CELL_EXIT; /* Uscita inferiore */
    m->cells[MAP_ROWS / 2][0] = CELL_EXIT;            /* Uscita sinistra */
    m->cells[MAP_ROWS / 2][MAP_COLS - 1] = CELL_EXIT; /* Uscita destra */

    /* 4. Distribuisce casualmente 15 oggetti raccoglibili 'O' su celle libere*/
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
        if (p->score > best_score) {
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

    /* Assegna casualmente uno degli 8 punti di partenza dei walker */
    int idx = rand() % 8;
    int row = PUNTI_PARTENZA[idx].r;
    int col = PUNTI_PARTENZA[idx].c;

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

    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Nuova connessione: '%s' (riga %d, colonna %d).", nickname, row, col);
    log_event(log_buf);
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
        char log_buf[128];
        snprintf(log_buf, sizeof(log_buf), "Disconnessione: '%s'.", nick);
        log_event(log_buf);
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
    char log_buf[128];
    if (collected) {
        snprintf(log_buf, sizeof(log_buf), "%s ha raccolto un oggetto in (%d,%d), punteggio=%d.", nick, nr, nc, score);
        log_event(log_buf);
        notice = "Hai raccolto un oggetto! (+1 punto)";
    }
    if (reached_exit) {
        snprintf(log_buf, sizeof(log_buf), "%s ha raggiunto l'uscita con punteggio=%d.", nick, score);
        log_event(log_buf);
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
        char log_buf[128];
        snprintf(log_buf, sizeof(log_buf), "Fine partita. Vincitore: %s con %d oggetti.", msg.nickname, msg.score);
        log_event(log_buf);
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
