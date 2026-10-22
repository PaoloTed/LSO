#include "game.h"
#include "log.h"
#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define USERS_FILE "users.txt"

StatoGioco g_game;

/* Generazione labirinto */
static void generate_maze(Labirinto *m) {
    /* Celle libere */
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            m->cells[r][c] = CELL_FREE;
            m->objects[r][c] = 0;
        }
    }

    /* Muri perimetrali */
    for (int c = 0; c < MAP_COLS; c++) {
        m->cells[0][c] = CELL_WALL;
        m->cells[MAP_ROWS - 1][c] = CELL_WALL;
    }
    for (int r = 0; r < MAP_ROWS; r++) {
        m->cells[r][0] = CELL_WALL;
        m->cells[r][MAP_COLS - 1] = CELL_WALL;
    }

    /* Ostacoli interni */
    for (int r = 2; r < MAP_ROWS - 2; r += 2) {
        for (int c = 2; c < MAP_COLS - 2; c += 2) {
            if (rand() % 2 == 0) {
                m->cells[r][c] = CELL_WALL;
            }
        }
    }

    /* Uscite */
    m->cells[0][MAP_COLS / 2] = CELL_EXIT;
    m->cells[MAP_ROWS - 1][MAP_COLS / 2] = CELL_EXIT;
    m->cells[MAP_ROWS / 2][0] = CELL_EXIT;
    m->cells[MAP_ROWS / 2][MAP_COLS - 1] = CELL_EXIT;

    /* Oggetti */
    for (int o = 0; o < 15; ) {
        int r = 1 + rand() % (MAP_ROWS - 2);
        int c = 1 + rand() % (MAP_COLS - 2);
        if (m->cells[r][c] == CELL_FREE && m->objects[r][c] == 0) {
            m->objects[r][c] = 1;
            o++;
        }
    }
}

/* Verifica credenziali o registrazione */
static int auth_check_user(const char *nickname, const char *password) {
    FILE *f = fopen(USERS_FILE, "r");
    char stored_nick[MAX_NICK];
    char stored_pass[MAX_PASS];

    if (f != NULL) {
        while (fscanf(f, "%s %s", stored_nick, stored_pass) == 2) {
            if (strcmp(stored_nick, nickname) == 0) {
                fclose(f);
                return (strcmp(stored_pass, password) == 0);
            }
        }
        fclose(f);
    }

    /* Registrazione utente */
    f = fopen(USERS_FILE, "a");
    if (!f)
        return -1;
    fprintf(f, "%s %s\n", nickname, password);
    fflush(f);
    fclose(f);

    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Nuovo utente registrato: '%s'.", nickname);
    log_event(log_buf);

    return 1;
}

void game_init(int timeout, int interval) {
    srand((unsigned)time(NULL));
    free(g_game.players);
    memset(&g_game, 0, sizeof(g_game));
    pthread_mutex_init(&g_game.mutex, NULL);
    g_game.timeout = timeout;
    g_game.interval = interval;
    g_game.winner = -1;
    generate_maze(&g_game.maze);
}

/* Celle scoperte attorno al giocatore */
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

/* Calcola il vincitore */
static int find_winner_locked(void) {
    int exited_count = 0, exited_idx = -1;
    for (int i = 0; i < g_game.players_count; i++) {
        if (g_game.players[i].exited) {
            exited_count++;
            exited_idx = i;
        }
    }

    /* Singolo giocatore uscito */
    if (exited_count == 1)
        return exited_idx;

    /* Confronto punteggi */
    int best = -1, best_score = -1;
    for (int i = 0; i < g_game.players_count; i++) {
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
    for (int i = 0; i < g_game.players_count; i++) {
        Giocatore *p = &g_game.players[i];
        if (!p->active)
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

int game_add_player(int fd, const char *nickname, const char *password) {
    pthread_mutex_lock(&g_game.mutex);

    if (g_game.game_over) {
        pthread_mutex_unlock(&g_game.mutex);
        return -1;
    }

    /* Verifica credenziali */
    int auth_res = auth_check_user(nickname, password);
    if (auth_res == 0) {
        pthread_mutex_unlock(&g_game.mutex);
        char log_buf[128];
        snprintf(log_buf, sizeof(log_buf), "Tentativo di login fallito per '%s': password errata.", nickname);
        log_event(log_buf);
        return -4; /* Password errata */
    } else if (auth_res < 0) {
        pthread_mutex_unlock(&g_game.mutex);
        return -3;
    }

    /* Controllo nickname */
    for (int i = 0; i < g_game.players_count; i++) {
        if (g_game.players[i].active && strcmp(g_game.players[i].nickname, nickname) == 0) {
            pthread_mutex_unlock(&g_game.mutex);
            return -2; /* Gia' connesso */
        }
    }

    /* Cerca slot libero */
    int slot = -1;
    for (int i = 0; i < g_game.players_count; i++) {
        if (!g_game.players[i].active) {
            slot = i;
            break;
        }
    }

    /* Espansione array */
    if (slot < 0) {
        int new_count = 8;
        if (g_game.players_count > 0)
            new_count = g_game.players_count * 2;
        Giocatore *new_players = realloc(g_game.players, new_count * sizeof(Giocatore));
        if (!new_players) {
            pthread_mutex_unlock(&g_game.mutex);
            return -3;
        }
        memset(new_players + g_game.players_count, 0, (new_count - g_game.players_count) * sizeof(Giocatore));
        slot = g_game.players_count;
        g_game.players = new_players;
        g_game.players_count = new_count;
    }

    /* Posiziona il giocatore in una cella libera casuale */
    int row, col;
    do {
        row = 1 + rand() % (MAP_ROWS - 2);
        col = 1 + rand() % (MAP_COLS - 2);
    } while (g_game.maze.cells[row][col] != CELL_FREE);

    Giocatore *p = &g_game.players[slot];
    p->fd = fd;
    p->active = 1;
    p->joined = 1;
    p->exited = 0;
    p->row = row;
    p->col = col;
    p->score = 0;
    strncpy(p->nickname, nickname, MAX_NICK - 1);
    p->nickname[MAX_NICK - 1] = '\0';
    memset(p->discovered, 0, sizeof(p->discovered));
    reveal_around(p);

    pthread_mutex_unlock(&g_game.mutex);

    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Nuova connessione: '%s' (riga %d, colonna %d, slot %d).", nickname, row, col, slot);
    log_event(log_buf);
    return slot;
}

void game_remove_player(int index) {
    pthread_mutex_lock(&g_game.mutex);
    if (g_game.players[index].active) {
        g_game.players[index].active = 0;
        close(g_game.players[index].fd);
        char log_buf[128];
        snprintf(log_buf, sizeof(log_buf), "Disconnessione: '%s'.", g_game.players[index].nickname);
        log_event(log_buf);
    }
    pthread_mutex_unlock(&g_game.mutex);
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
    switch (tolower((unsigned char)direction)) {
        case 'w': nr--; break;
        case 's': nr++; break;
        case 'a': nc--; break;
        case 'd': nc++; break;
        default: invalid = 1; break;
    }

    if (invalid || g_game.maze.cells[nr][nc] == CELL_WALL) {
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
    strncpy(nick, p->nickname, sizeof(nick));
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

/* Costruisce la vista locale 5x5 */
static void build_local_view(const Giocatore *p, char local_map[LOCAL_VIEW][LOCAL_VIEW]) {
    for (int dr = -VIEW_RADIUS; dr <= VIEW_RADIUS; dr++) {
        for (int dc = -VIEW_RADIUS; dc <= VIEW_RADIUS; dc++) {
            int r = p->row + dr;
            int c = p->col + dc;
            int vr = dr + VIEW_RADIUS;
            int vc = dc + VIEW_RADIUS;

            if (r < 0 || r >= MAP_ROWS || c < 0 || c >= MAP_COLS || !p->discovered[r][c])
                local_map[vr][vc] = CELL_HIDDEN;
            else if (r == p->row && c == p->col)
                local_map[vr][vc] = CELL_PLAYER;
            else if (g_game.maze.objects[r][c])
                local_map[vr][vc] = CELL_OBJECT;
            else
                local_map[vr][vc] = g_game.maze.cells[r][c];
        }
    }
}

void game_send_local_map(int index, const char *notice) {
    Messaggio msg = {0};
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
    build_local_view(p, msg.local_map);

    int fd = p->fd;
    pthread_mutex_unlock(&g_game.mutex);

    invia_messaggio(fd, &msg);
}

void game_send_global_map(int index) {
    Messaggio msg = {0};
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
    build_local_view(p, msg.local_map);

    /* Mappa globale */
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
    Messaggio msg = {0};
    msg.type = MSG_LISTA;

    pthread_mutex_lock(&g_game.mutex);
    if (!g_game.players[index].active) {
        pthread_mutex_unlock(&g_game.mutex);
        return;
    }

    int count = 0;
    for (int i = 0; i < g_game.players_count && count < MAX_LIST_PLAYERS; i++) {
        if (!g_game.players[i].active)
            continue;
        strncpy(msg.players[count].nickname, g_game.players[i].nickname, MAX_NICK - 1);
        msg.players[count].score = g_game.players[i].score;
        msg.players[count].exited = g_game.players[i].exited;
        count++;
    }
    msg.num_players = count;
    int fd = g_game.players[index].fd;
    pthread_mutex_unlock(&g_game.mutex);

    invia_messaggio(fd, &msg);
}

void game_broadcast_global_maps(void) {
    for (int i = 0; i < g_game.players_count; i++) {
        if (g_game.players[i].active)
            game_send_global_map(i);
    }
}

void game_broadcast_game_over(void) {
    Messaggio msg = {0};
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

    for (int i = 0; i < g_game.players_count; i++) {
        if (g_game.players[i].active)
            invia_messaggio(g_game.players[i].fd, &msg);
    }
    pthread_mutex_unlock(&g_game.mutex);
}
