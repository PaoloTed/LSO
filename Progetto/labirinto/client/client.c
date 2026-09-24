#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "display.h"
#include "protocol.h"

#define MAX_LIST 64

static int sock_fd = -1;

/* Ultima mappa ricevuta, conservata per poterla rivedere con m/g. */
static char local_grid[LOCAL_VIEW * LOCAL_VIEW];
static int  local_rows, local_cols, local_row, local_col, local_score;
static int  have_local = 0;

static char global_grid[MAP_ROWS * MAP_COLS];
static int  global_rows, global_cols;
static int  have_global = 0;

static void read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, (int)size, stdin)) {
        buf[0] = '\0';
        return;
    }
    size_t n = strlen(buf);
    if (n > 0 && buf[n - 1] == '\n')
        buf[n - 1] = '\0';
}

/* Si connette al server risolvendo anche i nomi simbolici. */
static int connect_to_server(const char *host, int port) {
    char portstr[16];
    snprintf(portstr, sizeof(portstr), "%d", port);

    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(host, portstr, &hints, &res);
    if (err != 0) {
        fprintf(stderr, "Risoluzione di %s fallita: %s\n", host, gai_strerror(err));
        exit(1);
    }

    int s = -1;
    for (p = res; p != NULL; p = p->ai_next) {
        s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (s < 0)
            continue;
        if (connect(s, p->ai_addr, p->ai_addrlen) == 0)
            break;
        close(s);
        s = -1;
    }
    freeaddrinfo(res);

    if (s < 0) {
        fprintf(stderr, "Connessione al server %s:%d fallita.\n", host, port);
        exit(1);
    }
    return s;
}

/* Gestisce un messaggio ricevuto dal server.
 * Ritorna 0 per continuare, 1 per fine partita, -1 per disconnessione. */
static int handle_server_message(void) {
    uint8_t type;
    unsigned char payload[MAX_PAYLOAD];

    int len = recv_message(sock_fd, &type, payload, sizeof(payload));
    if (len < 0)
        return -1;

    switch (type) {
        case MSG_OK:
            printf("[server] Accesso effettuato.\n");
            break;

        case MSG_INFO:
        case MSG_ERROR: {
            TextPayload *t = (TextPayload *)payload;
            t->text[MAX_TEXT - 1] = '\0';
            printf("[server] %s\n", t->text);
            break;
        }

        case MSG_LOCAL_MAP: {
            uint32_t row, col, score, rows, cols;
            memcpy(&row, payload + 0, 4);
            memcpy(&col, payload + 4, 4);
            memcpy(&score, payload + 8, 4);
            memcpy(&rows, payload + 12, 4);
            memcpy(&cols, payload + 16, 4);
            row = ntohl(row); col = ntohl(col); score = ntohl(score);
            rows = ntohl(rows); cols = ntohl(cols);

            int n = (int)(rows * cols);
            if (n > 0 && n <= (int)sizeof(local_grid)) {
                memcpy(local_grid, payload + LOCAL_MAP_HDR, (size_t)n);
                local_rows = (int)rows;
                local_cols = (int)cols;
                local_row = (int)row;
                local_col = (int)col;
                local_score = (int)score;
                have_local = 1;
                display_local_map(local_row, local_col, local_score,
                                  local_rows, local_cols, local_grid);
            }
            break;
        }

        case MSG_GLOBAL_MAP: {
            uint32_t rows, cols;
            memcpy(&rows, payload + 0, 4);
            memcpy(&cols, payload + 4, 4);
            rows = ntohl(rows); cols = ntohl(cols);

            int n = (int)(rows * cols);
            if (n > 0 && n <= (int)sizeof(global_grid)) {
                memcpy(global_grid, payload + GLOBAL_MAP_HDR, (size_t)n);
                global_rows = (int)rows;
                global_cols = (int)cols;
                have_global = 1;
                display_global_map(global_rows, global_cols, global_grid);
            }
            break;
        }

        case MSG_PLAYER_LIST: {
            uint32_t count;
            memcpy(&count, payload + 0, 4);
            count = ntohl(count);
            if (count > MAX_LIST)
                count = MAX_LIST;

            PlayerEntry entries[MAX_LIST];
            for (uint32_t i = 0; i < count; i++) {
                memcpy(&entries[i], payload + 4 + i * sizeof(PlayerEntry),
                       sizeof(PlayerEntry));
                entries[i].score = ntohl(entries[i].score);
                entries[i].exited = ntohl(entries[i].exited);
                entries[i].nickname[MAX_NICK - 1] = '\0';
            }
            display_player_list(entries, (int)count);
            break;
        }

        case MSG_GAME_OVER: {
            char winner[MAX_NICK];
            uint32_t score;
            memcpy(winner, payload, MAX_NICK);
            winner[MAX_NICK - 1] = '\0';
            memcpy(&score, payload + MAX_NICK, 4);
            score = ntohl(score);

            if (winner[0] != '\0')
                printf("\n*** Partita terminata. Vincitore: %s con %u oggetti. ***\n",
                       winner, score);
            else
                printf("\n*** Partita terminata. Nessun vincitore. ***\n");
            return 1;
        }

        default:
            break;
    }
    return 0;
}

static void send_auth(int choice, const char *nick, const char *pass) {
    AuthPayload auth;
    memset(&auth, 0, sizeof(auth));
    strncpy(auth.nickname, nick, MAX_NICK - 1);
    strncpy(auth.password, pass, MAX_PASS - 1);

    uint8_t type = (choice == 2) ? MSG_REGISTER : MSG_LOGIN;
    send_message(sock_fd, type, &auth, (uint16_t)sizeof(auth));
}

static void game_loop(void) {
    fd_set master, readfds;

    FD_ZERO(&master);
    FD_SET(STDIN_FILENO, &master);
    FD_SET(sock_fd, &master);
    int max_fd = (sock_fd > STDIN_FILENO) ? sock_fd : STDIN_FILENO;

    for (;;) {
        readfds = master;

        int n = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (n < 0) {
            if (errno == EINTR)
                continue;
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            char line[64];
            if (!fgets(line, sizeof(line), stdin)) {
                send_message(sock_fd, MSG_QUIT, NULL, 0);
                break;
            }

            size_t l = strlen(line);
            while (l > 0 && (line[l - 1] == '\n' || line[l - 1] == '\r'))
                line[--l] = '\0';
            if (l == 0)
                continue;

            char cmd = line[0];
            if (cmd == 'w' || cmd == 'a' || cmd == 's' || cmd == 'd' ||
                cmd == 'W' || cmd == 'A' || cmd == 'S' || cmd == 'D') {
                MovePayload m;
                m.direction = cmd;
                send_message(sock_fd, MSG_MOVE, &m, (uint16_t)sizeof(m));
            } else if (cmd == 'l' || cmd == 'L') {
                send_message(sock_fd, MSG_LIST, NULL, 0);
            } else if (cmd == 'm' || cmd == 'M') {
                if (have_local)
                    display_local_map(local_row, local_col, local_score,
                                      local_rows, local_cols, local_grid);
                else
                    printf("Nessuna mappa locale disponibile.\n");
            } else if (cmd == 'g' || cmd == 'G') {
                if (have_global)
                    display_global_map(global_rows, global_cols, global_grid);
                else
                    printf("Nessuna mappa globale disponibile.\n");
            } else if (cmd == 'h' || cmd == 'H' || cmd == '?') {
                display_help();
            } else if (cmd == 'q' || cmd == 'Q') {
                send_message(sock_fd, MSG_QUIT, NULL, 0);
                printf("Arrivederci!\n");
                break;
            } else {
                printf("Comando non riconosciuto (premi 'h' per l'aiuto).\n");
            }
        }

        if (FD_ISSET(sock_fd, &readfds)) {
            int rc = handle_server_message();
            if (rc == 1) {
                printf("Connessione chiusa.\n");
                break;
            }
            if (rc < 0) {
                printf("Connessione chiusa dal server.\n");
                break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <host> <porta>\n", argv[0]);
        return 1;
    }

    const char *host = argv[1];
    int port = atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Porta non valida.\n");
        return 1;
    }

    signal(SIGPIPE, SIG_IGN);

    sock_fd = connect_to_server(host, port);
    printf("Connesso al server %s:%d\n", host, port);

    int choice = 0;
    while (choice != 1 && choice != 2) {
        printf("1) Accedi\n2) Registrati\nScelta: ");
        fflush(stdout);
        char line[16];
        if (!fgets(line, sizeof(line), stdin)) {
            close(sock_fd);
            return 1;
        }
        choice = atoi(line);
    }

    char nick[MAX_NICK], pass[MAX_PASS];
    read_line("Nickname: ", nick, sizeof(nick));
    read_line("Password: ", pass, sizeof(pass));
    send_auth(choice, nick, pass);

    uint8_t resp;
    unsigned char payload[MAX_PAYLOAD];
    int len = recv_message(sock_fd, &resp, payload, sizeof(payload));
    if (len < 0) {
        printf("Connessione chiusa dal server.\n");
        close(sock_fd);
        return 1;
    }
    if (resp == MSG_ERROR) {
        TextPayload *t = (TextPayload *)payload;
        t->text[MAX_TEXT - 1] = '\0';
        printf("Accesso negato: %s\n", t->text);
        close(sock_fd);
        return 1;
    }
    if (resp != MSG_OK) {
        printf("Risposta inattesa dal server.\n");
        close(sock_fd);
        return 1;
    }

    printf("Accesso effettuato come '%s'.\n", nick);
    display_help();
    game_loop();

    close(sock_fd);
    return 0;
}
