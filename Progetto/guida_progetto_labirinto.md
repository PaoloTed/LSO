# 🧩 Guida Completa al Progetto LSO — Labirinto con Oggetti e Uscita

> **Traccia A — Sistema Client-Server in C su UNIX**  
> Basata esclusivamente sui contenuti del corso LSO (Laboratorio di Sistemi Operativi)

---

## Indice

1. [Panoramica del Sistema](#1-panoramica-del-sistema)
2. [Architettura e Struttura dei File](#2-architettura-e-struttura-dei-file)
3. [Strutture Dati Fondamentali](#3-strutture-dati-fondamentali)
4. [Protocollo Applicativo Client-Server](#4-protocollo-applicativo-client-server)
5. [Implementazione del Server](#5-implementazione-del-server)
6. [Implementazione del Client](#6-implementazione-del-client)
7. [Generazione del Labirinto](#7-generazione-del-labirinto)
8. [Gestione della Concorrenza](#8-gestione-della-concorrenza)
9. [Segnali e Timer Periodico](#9-segnali-e-timer-periodico)
10. [Logging](#10-logging)
11. [Gestione della Fine Partita](#11-gestione-della-fine-partita)
12. [Cosa Studiare — Sezioni Chiave della Guida LSO](#12-cosa-studiare--sezioni-chiave-della-guida-lso)
13. [Scelte Tecniche Consigliate](#13-scelte-tecniche-consigliate)
14. [Compilazione e Makefile](#14-compilazione-e-makefile)
15. [Traccia per la Relazione](#15-traccia-per-la-relazione)

---

## 1. Panoramica del Sistema

Il sistema è composto da **due programmi separati**:

```
┌────────────────────────────────────────────────────────────────────┐
│                         SERVER                                     │
│  - Genera il labirinto (matrice 2D)                                │
│  - Gestisce posizione e stato di ogni giocatore                    │
│  - Elabora i comandi dei client (movimento)                        │
│  - Invia mappa locale ad ogni mossa                               │
│  - Invia mappa globale mascherata ogni T secondi                  │
│  - Gestisce il timeout di fine partita                             │
│  - Fa logging su file                                              │
└────────────────────────────────────────────────────────────────────┘
         ↑↓  TCP Socket  ↑↓
┌──────────────────────────────────────────────────────┐
│                        CLIENT                        │
│  - Si connette al server (IP:porta da riga comando)  │
│  - Invia nickname                                    │
│  - Invia comandi (W/A/S/D per movimento)             │
│  - Riceve e visualizza la mappa locale               │
│  - Riceve e visualizza la mappa globale              │
│  - Mostra lista giocatori connessi                   │
└──────────────────────────────────────────────────────┘
```

**Regole fondamentali dalla traccia:**
- Linguaggio: **C su Unix/Linux**
- Comunicazione: **Socket TCP** (`AF_INET`, `SOCK_STREAM`)
- Il server **non legge da stdin e non scrive su stdout** (solo stderr in caso di errore fatale)
- Il server deve gestire più client **simultaneamente** (server concorrente)
- Si possono usare solo system call UNIX e la **libreria standard del C**

---

## 2. Architettura e Struttura dei File

### Struttura consigliata del progetto

```
progetto_lso/
├── server/
│   ├── server.c          ← main del server
│   ├── maze.c / maze.h   ← generazione e gestione labirinto
│   ├── game.c / game.h   ← logica di gioco (movimento, oggetti)
│   ├── protocol.c / protocol.h  ← funzioni di invio/ricezione messaggi
│   └── log.c / log.h     ← sistema di logging su file
├── client/
│   ├── client.c          ← main del client
│   ├── display.c / display.h   ← visualizzazione mappa
│   └── protocol.c / protocol.h ← (condiviso o duplicato)
├── common/
│   └── protocol.h        ← tipi e costanti del protocollo condivise
└── Makefile
```

---

## 3. Strutture Dati Fondamentali

### 3.1 Cella del Labirinto

```c
// Valori delle celle della mappa
#define CELL_WALL     '#'   // Muro
#define CELL_FREE     '.'   // Cella libera
#define CELL_EXIT     'E'   // Uscita
#define CELL_OBJECT   'O'   // Oggetto raccoglibile
#define CELL_PLAYER   'P'   // Posizione giocatore (per visualizzazione)
#define CELL_HIDDEN   '?'   // Cella non ancora scoperta (mappa mascherata)

#define MAP_ROWS 20         // Righe del labirinto
#define MAP_COLS 40         // Colonne del labirinto
#define VIEW_RADIUS 2       // Raggio di visione (es. finestra 5x5 centrata)
#define MAP_SEND_INTERVAL 5 // Secondi tra un invio di mappa globale e l'altro
#define GAME_TIMEOUT 120    // Secondi di durata massima della partita
#define MAX_CLIENTS 32      // Numero massimo di client connessi

typedef struct {
    char cells[MAP_ROWS][MAP_COLS]; // La mappa del labirinto
    int  objects[MAP_ROWS][MAP_COLS]; // 1 se c'è un oggetto, 0 altrimenti
} Maze;
```

### 3.2 Stato del Giocatore

```c
typedef struct {
    int  fd;                           // File descriptor della socket
    char nickname[32];                 // Nickname del giocatore
    int  row, col;                     // Posizione corrente
    int  score;                        // Oggetti raccolti
    int  active;                       // 1 = connesso, 0 = disconnesso
    int  exited;                       // 1 = ha raggiunto l'uscita
    char discovered[MAP_ROWS][MAP_COLS]; // Mappa scoperta da questo giocatore
    pthread_mutex_t player_mutex;      // Mutex per accesso sicuro al giocatore
} Player;
```

### 3.3 Stato Globale del Server

```c
typedef struct {
    Maze            maze;
    Player          players[MAX_CLIENTS];
    int             num_players;
    int             game_over;         // 1 = partita terminata
    time_t          start_time;        // Ora di inizio partita
    pthread_mutex_t game_mutex;        // Mutex per accesso alle strutture condivise
    int             listen_fd;         // Socket di ascolto
    int             log_fd;            // File descriptor del file di log
} GameState;

GameState g_game;  // Variabile globale del server
```

---

## 4. Protocollo Applicativo Client-Server

> Questa è la sezione critica per la relazione (sezione 2 richiesta dalla traccia).  
> Il protocollo è a livello applicativo, **non** a livello TCP.

### 4.1 Formato dei Messaggi

Usa un protocollo **binario a lunghezza fissa** per ogni messaggio:

```c
// Tipi di messaggio (dal client al server)
#define MSG_JOIN        0x01   // Richiesta di entrare con nickname
#define MSG_MOVE        0x02   // Comando di movimento
#define MSG_LIST        0x03   // Richiesta lista giocatori
#define MSG_QUIT        0x04   // Disconnessione volontaria

// Tipi di messaggio (dal server al client)
#define MSG_OK          0x10   // Conferma operazione
#define MSG_ERROR       0x11   // Errore (es. nickname già in uso)
#define MSG_LOCAL_MAP   0x12   // Mappa locale dopo ogni movimento
#define MSG_GLOBAL_MAP  0x13   // Mappa globale mascherata (periodica)
#define MSG_PLAYER_LIST 0x14   // Lista giocatori
#define MSG_GAME_OVER   0x15   // Fine partita + vincitore
#define MSG_COLLECT     0x16   // Notifica raccolta oggetto

// Direzioni di movimento
#define DIR_UP    'W'
#define DIR_DOWN  'S'
#define DIR_LEFT  'A'
#define DIR_RIGHT 'D'
```

### 4.2 Struttura dell'Header

```c
// Ogni messaggio è composto da:
// [header 4 byte] [payload variabile]
typedef struct {
    uint8_t  type;      // Tipo di messaggio
    uint8_t  flags;     // Flag aggiuntivi (0 se non usati)
    uint16_t length;    // Lunghezza del payload in byte (network byte order)
} MsgHeader;
```

> **Attenzione al Byte Order!** Usa sempre `htons()` / `ntohs()` per il campo `length`.  
> Riferimento: **Sezione 17.3** della guida LSO.

### 4.3 Sequenza di Connessione

```
CLIENT                                  SERVER
  |                                        |
  |------- TCP connect() ----------------> |
  |                                        |
  |-- MSG_JOIN (nickname="Mario") -------> |
  |                                        |
  |<-- MSG_OK (posizione iniziale) ------- |
  |<-- MSG_LOCAL_MAP (mappa iniziale) ---- |
  |                                        |
  |-- MSG_MOVE (DIR_UP) ----------------> |
  |                                        |
  |<-- MSG_LOCAL_MAP (nuova mappa) ------- |
  |                                        |
  |       [ ogni T secondi ]               |
  |<-- MSG_GLOBAL_MAP (mappa mascherata) - |
  |                                        |
  |-- MSG_QUIT --------------------------> |
  |                                        |
  |<-- [chiusura connessione] ------------ |
```

### 4.4 Formato dei Payload Principali

```c
// Payload MSG_JOIN (client → server)
typedef struct {
    char nickname[32];
} PayloadJoin;

// Payload MSG_MOVE (client → server)
typedef struct {
    char direction; // 'W', 'A', 'S', 'D'
} PayloadMove;

// Payload MSG_LOCAL_MAP (server → client)
typedef struct {
    int  player_row;
    int  player_col;
    int  score;
    int  rows;
    int  cols;
    // Seguono rows*cols byte della mappa locale visibile
} PayloadLocalMap;

// Payload MSG_GLOBAL_MAP (server → client)
typedef struct {
    int  rows;
    int  cols;
    // Seguono rows*cols byte della mappa globale mascherata
} PayloadGlobalMap;

// Payload MSG_GAME_OVER (server → client)
typedef struct {
    char winner_nick[32];
    int  winner_score;
} PayloadGameOver;
```

---

## 5. Implementazione del Server

### 5.1 Schema del main() del Server

```c
int main(int argc, char *argv[]) {
    // 1. Parse argomenti (porta)
    if (argc < 2) { fprintf(stderr, "Uso: server <porta>\n"); exit(1); }
    int port = atoi(argv[1]);

    // 2. Apertura file di log
    open_log_file();

    // 3. Generazione labirinto
    generate_maze(&g_game.maze);

    // 4. Inizializzazione mutex globale
    pthread_mutex_init(&g_game.game_mutex, NULL);

    // 5. Creazione socket di ascolto TCP
    g_game.listen_fd = create_listening_socket(port);

    // 6. Gestione segnali (SIGPIPE ignorato, SIGCHLD se usi fork)
    signal(SIGPIPE, SIG_IGN);

    // 7. Avvio thread del timer periodico (mappa globale + timeout partita)
    pthread_t timer_tid;
    pthread_create(&timer_tid, NULL, timer_thread, NULL);
    pthread_detach(timer_tid);

    // 8. Loop principale: accetta connessioni
    for (;;) {
        int client_fd = accept(g_game.listen_fd, NULL, NULL);
        if (client_fd < 0) { if (errno == EINTR) continue; break; }

        // Crea un thread per gestire il client
        int *arg = malloc(sizeof(int));
        *arg = client_fd;
        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, arg);
        pthread_detach(tid);  // nessun join necessario
    }
    return 0;
}
```

### 5.2 Creazione della Listening Socket

```c
int create_listening_socket(int port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); exit(1); }

    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }
    if (listen(s, 10) < 0) {
        perror("listen"); exit(1);
    }
    return s;
}
```

> Riferimento: **Sezione 17.5** della guida LSO (Server TCP Completo).

### 5.3 Thread di Gestione Client

```c
void *client_handler(void *arg) {
    int fd = *(int *)arg;
    free(arg);

    // Leggi MSG_JOIN e registra il giocatore
    Player *p = register_player(fd);
    if (!p) { close(fd); return NULL; }

    // Invia posizione iniziale e mappa locale
    send_local_map(p);

    // Loop: leggi comandi dal client
    while (1) {
        MsgHeader hdr;
        ssize_t n = recv_all(fd, &hdr, sizeof(hdr));
        if (n <= 0) break;  // client disconnesso

        hdr.length = ntohs(hdr.length);

        switch (hdr.type) {
            case MSG_MOVE:
                handle_move(p, hdr);
                break;
            case MSG_LIST:
                send_player_list(p);
                break;
            case MSG_QUIT:
                goto disconnect;
        }
    }
disconnect:
    disconnect_player(p);
    close(fd);
    return NULL;
}
```

### 5.4 Gestione del Movimento

```c
void handle_move(Player *p, MsgHeader hdr) {
    PayloadMove payload;
    recv_all(p->fd, &payload, sizeof(payload));

    pthread_mutex_lock(&g_game.game_mutex);

    int new_row = p->row, new_col = p->col;
    switch (payload.direction) {
        case DIR_UP:    new_row--; break;
        case DIR_DOWN:  new_row++; break;
        case DIR_LEFT:  new_col--; break;
        case DIR_RIGHT: new_col++; break;
    }

    // Controlla validità mossa (muri, bordi)
    char cell = g_game.maze.cells[new_row][new_col];
    if (cell != CELL_WALL && new_row >= 0 && new_row < MAP_ROWS
        && new_col >= 0 && new_col < MAP_COLS) {
        p->row = new_row;
        p->col = new_col;

        // Rivela zona attorno al giocatore
        reveal_area(p);

        // Raccolta oggetto
        if (g_game.maze.objects[new_row][new_col]) {
            g_game.maze.objects[new_row][new_col] = 0;
            p->score++;
            log_event("Giocatore %s ha raccolto un oggetto in (%d,%d)",
                       p->nickname, new_row, new_col);
        }

        // Controllo uscita
        if (cell == CELL_EXIT) {
            p->exited = 1;
            log_event("Giocatore %s è uscito dal labirinto!", p->nickname);
            check_game_over();
        }
    }

    pthread_mutex_unlock(&g_game.game_mutex);
    send_local_map(p);  // Invia mappa aggiornata
}
```

---

## 6. Implementazione del Client

### 6.1 Schema del main() del Client

```c
int main(int argc, char *argv[]) {
    // Uso: client <IP_server> <porta>
    if (argc < 3) { fprintf(stderr, "Uso: client <ip> <porta>\n"); exit(1); }

    // 1. Connessione al server
    int fd = connect_to_server(argv[1], atoi(argv[2]));

    // 2. Lettura nickname dall'utente (stdin)
    char nickname[32];
    printf("Inserisci il tuo nickname: ");
    fgets(nickname, sizeof(nickname), stdin);
    nickname[strcspn(nickname, "\n")] = 0; // rimuovi newline

    // 3. Invio MSG_JOIN
    send_join(fd, nickname);

    // 4. Ricezione risposta iniziale
    receive_and_display(fd);

    // 5. Loop di gioco: usa select() per gestire stdin e socket
    game_loop(fd);

    close(fd);
    return 0;
}
```

### 6.2 Loop di Gioco con select()

Il client deve gestire **due sorgenti di input simultanee**:
- **stdin**: comandi dell'utente (WASD, lista, quit)
- **socket del server**: mappe periodiche o notifiche

Si usa `select()` per non bloccarsi su uno solo dei due.

```c
void game_loop(int fd) {
    fd_set master_set, read_set;
    FD_ZERO(&master_set);
    FD_SET(STDIN_FILENO, &master_set);
    FD_SET(fd, &master_set);
    int max_fd = fd;

    while (1) {
        read_set = master_set; // Copia (select è distruttiva!)
        int n = select(max_fd + 1, &read_set, NULL, NULL, NULL);
        if (n < 0) { if (errno == EINTR) continue; break; }

        // Input da tastiera
        if (FD_ISSET(STDIN_FILENO, &read_set)) {
            char input[8];
            fgets(input, sizeof(input), stdin);
            handle_user_input(fd, input);
        }

        // Messaggio dal server (mappa globale, game over, ecc.)
        if (FD_ISSET(fd, &read_set)) {
            if (receive_and_display(fd) <= 0) {
                printf("Connessione chiusa dal server.\n");
                break;
            }
        }
    }
}
```

> Riferimento: **Sezione 18** della guida LSO (I/O Multiplexing — select).

### 6.3 Connessione al Server

```c
int connect_to_server(const char *ip, int port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); exit(1); }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0) {
        fprintf(stderr, "Indirizzo IP non valido: %s\n", ip);
        exit(1);
    }

    if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect"); exit(1);
    }
    printf("Connesso al server %s:%d\n", ip, port);
    return s;
}
```

> Riferimento: **Sezione 17.6** della guida LSO (Client TCP Completo).

---

## 7. Generazione del Labirinto

### 7.1 Algoritmo Semplice (Maze con muri casuali)

```c
void generate_maze(Maze *m) {
    srand(time(NULL));

    // Inizializza tutto come muri
    for (int r = 0; r < MAP_ROWS; r++)
        for (int c = 0; c < MAP_COLS; c++)
            m->cells[r][c] = CELL_WALL;

    // Scava corridoi usando DFS o algoritmo casuale
    // Versione semplice: celle casuali libere (30% muri)
    for (int r = 1; r < MAP_ROWS - 1; r++) {
        for (int c = 1; c < MAP_COLS - 1; c++) {
            m->cells[r][c] = (rand() % 10 < 3) ? CELL_WALL : CELL_FREE;
        }
    }

    // Posiziona uscite (2-3)
    m->cells[1][MAP_COLS - 2]           = CELL_EXIT;
    m->cells[MAP_ROWS - 2][MAP_COLS - 2] = CELL_EXIT;

    // Posiziona oggetti (10-20)
    int placed = 0;
    while (placed < 15) {
        int r = 1 + rand() % (MAP_ROWS - 2);
        int c = 1 + rand() % (MAP_COLS - 2);
        if (m->cells[r][c] == CELL_FREE) {
            m->objects[r][c] = 1;
            placed++;
        }
    }
}
```

### 7.2 Rivelazione della Zona (Fog of War)

```c
void reveal_area(Player *p) {
    // Rivela un'area quadrata di raggio VIEW_RADIUS
    for (int dr = -VIEW_RADIUS; dr <= VIEW_RADIUS; dr++) {
        for (int dc = -VIEW_RADIUS; dc <= VIEW_RADIUS; dc++) {
            int r = p->row + dr;
            int c = p->col + dc;
            if (r >= 0 && r < MAP_ROWS && c >= 0 && c < MAP_COLS) {
                p->discovered[r][c] = 1; // Segnala come scoperta
            }
        }
    }
}
```

### 7.3 Costruzione della Mappa Mascherata

```c
void build_masked_map(Player *p, char *output) {
    // output deve essere grande MAP_ROWS * MAP_COLS
    for (int r = 0; r < MAP_ROWS; r++) {
        for (int c = 0; c < MAP_COLS; c++) {
            if (p->discovered[r][c]) {
                // Mostra la cella reale (con o senza oggetto)
                if (r == p->row && c == p->col)
                    output[r * MAP_COLS + c] = CELL_PLAYER;
                else if (g_game.maze.objects[r][c])
                    output[r * MAP_COLS + c] = CELL_OBJECT;
                else
                    output[r * MAP_COLS + c] = g_game.maze.cells[r][c];
            } else {
                output[r * MAP_COLS + c] = CELL_HIDDEN; // '?'
            }
        }
    }
}
```

---

## 8. Gestione della Concorrenza

### 8.1 Scelta: Thread (consigliata)

Il server usa **un thread per client** (`pthread`), che è il modello più diretto da implementare con le API studiate nel corso.

> Riferimento: **Sezione 14** della guida LSO (Thread e Concorrenza) e **Sezione 17.11** (Server Concorrente con thread).

**Compilare con:** `gcc -pthread -o server server.c ...`

### 8.2 Protezione dei Dati Condivisi

Tutti gli accessi alle strutture condivise (`g_game`) devono essere protetti da mutex:

```c
// PATTERN FONDAMENTALE:
pthread_mutex_lock(&g_game.game_mutex);
// --- accesso sicuro alle strutture condivise ---
pthread_mutex_unlock(&g_game.game_mutex);
```

> ⚠️ **Attenzione alle Race Condition**: ogni accesso in lettura o scrittura alla mappa, alla lista giocatori, al punteggio, agli oggetti deve avvenire sotto lock.

> Riferimento: **Sezione 15.2** della guida LSO (Mutex).

### 8.3 Invio Sicuro su Socket con Thread

**Problema**: più thread potrebbero voler scrivere sulla stessa socket nello stesso momento (es. il thread del client e il thread del timer vogliono entrambi mandare dati al client).

**Soluzione**: ogni `Player` ha un proprio mutex `player_mutex` che protegge la sua socket:

```c
void send_to_player(Player *p, void *data, size_t len) {
    pthread_mutex_lock(&p->player_mutex);
    send_all(p->fd, data, len);
    pthread_mutex_unlock(&p->player_mutex);
}
```

---

## 9. Segnali e Timer Periodico

### 9.1 Thread del Timer (approccio consigliato)

Invece di usare `SIGALRM` (complicato con i thread), usa un **thread dedicato** che dorme in loop:

```c
void *timer_thread(void *arg) {
    while (1) {
        sleep(MAP_SEND_INTERVAL);  // Aspetta T secondi

        pthread_mutex_lock(&g_game.game_mutex);
        int over = g_game.game_over;

        // Controlla timeout globale
        time_t now = time(NULL);
        if (!over && difftime(now, g_game.start_time) >= GAME_TIMEOUT) {
            g_game.game_over = 1;
            over = 1;
            log_event("Timeout partita scaduto.");
        }
        pthread_mutex_unlock(&g_game.game_mutex);

        if (over) {
            broadcast_game_over();
            break;
        }

        // Invia mappa globale mascherata a tutti i giocatori
        broadcast_global_map();
    }
    return NULL;
}
```

### 9.2 Gestione di SIGPIPE

Le operazioni di scrittura su una socket chiusa generano `SIGPIPE`, che per default termina il processo. È **obbligatorio ignorarlo**:

```c
signal(SIGPIPE, SIG_IGN);
// Oppure (raccomandato):
struct sigaction sa = {0};
sa.sa_handler = SIG_IGN;
sigaction(SIGPIPE, &sa, NULL);
```

Dopo questo, le chiamate `write()`/`send()` su socket chiuse restituiranno `-1` con `errno = EPIPE`, che possiamo gestire.

> Riferimento: **Sezione 19** della guida LSO (SIGPIPE ed EINTR).

### 9.3 Broadcast della Mappa Globale

```c
void broadcast_global_map(void) {
    pthread_mutex_lock(&g_game.game_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (g_game.players[i].active) {
            // Costruisci mappa mascherata personalizzata per questo giocatore
            char masked[MAP_ROWS * MAP_COLS];
            build_masked_map(&g_game.players[i], masked);
            pthread_mutex_unlock(&g_game.game_mutex);

            send_global_map_to(&g_game.players[i], masked);

            pthread_mutex_lock(&g_game.game_mutex);
        }
    }
    pthread_mutex_unlock(&g_game.game_mutex);
}
```

---

## 10. Logging

Il server deve loggare su file le attività principali (**non su stdout**).

```c
// File di log globale
static int log_fd = -1;

void open_log_file(void) {
    log_fd = open("server.log",
                  O_WRONLY | O_CREAT | O_APPEND,
                  S_IRUSR | S_IWUSR | S_IRGRP);
    if (log_fd < 0) {
        perror("apertura file di log");
        exit(1);
    }
}

void log_event(const char *fmt, ...) {
    // Ottieni timestamp
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    // Formatta il messaggio
    char msg[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    // Scrittura su file (thread-safe con write atomica per righe brevi)
    char line[600];
    int len = snprintf(line, sizeof(line), "[%s] %s\n", timestamp, msg);
    write(log_fd, line, len);
}
```

> Riferimento: **Sezione 10.2** (open, write) e **Sezione 10.4** (errno e perror).

**Cosa loggare (dalla traccia):**
- Data e ora di connessione dei client con il loro nickname (o IP)
- Data e ora di raccolta degli oggetti
- Timeout della partita
- Disconnessioni

---

## 11. Gestione della Fine Partita

### 11.1 Condizioni di Fine Partita

```c
void check_game_over(void) {
    // Già dentro mutex
    // 1. Tutti i giocatori sono usciti?
    int all_exited = 1;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (g_game.players[i].active && !g_game.players[i].exited) {
            all_exited = 0;
            break;
        }
    }

    if (all_exited) {
        g_game.game_over = 1;
        log_event("Tutti i giocatori sono usciti. Fine partita.");
        // Calcola vincitore in broadcast_game_over()
    }
}
```

### 11.2 Determinazione del Vincitore

```c
Player *find_winner(void) {
    Player *winner = NULL;
    int max_score = -1;
    int exited_count = 0;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (g_game.players[i].active || g_game.players[i].exited) {
            if (g_game.players[i].exited) exited_count++;
        }
    }

    // Se un solo giocatore è uscito → vince lui
    if (exited_count == 1) {
        for (int i = 0; i < MAX_CLIENTS; i++)
            if (g_game.players[i].exited) return &g_game.players[i];
    }

    // Altrimenti (nessuno o più usciti) → vince chi ha più oggetti
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if ((g_game.players[i].active || g_game.players[i].exited)
            && g_game.players[i].score > max_score) {
            max_score = g_game.players[i].score;
            winner = &g_game.players[i];
        }
    }
    return winner;
}
```

---

## 12. Cosa Studiare — Sezioni Chiave della Guida LSO

Questa tabella indica le sezioni della guida `Guida_Completa_LSO.md` che sono **direttamente necessarie** per il progetto:

| Sezione | Argomento | Perché Serve |
|---------|-----------|--------------|
| **§9** | Compilazione C e GCC | Compilare con `-pthread`, `-Wall`, capire i flag |
| **§10** | I/O di Basso Livello | `open`, `write`, `close` per il file di log |
| **§10.2** | `read`, `write` | Lettura/scrittura safe |
| **§10.4** | `errno`, `perror` | Gestione degli errori |
| **§11** | Processi Unix | Capire `fork()` se si sceglie l'approccio a processi |
| **§11.3** | `fork()` | Alternativa ai thread per il server |
| **§12** | Segnali | `SIGPIPE`, `SIGCHLD`, `signal()`, `sigaction()` |
| **§12.4-12.8** | Gestione segnali | `sigaction()`, `SA_RESTART` |
| **§14** | Thread POSIX | `pthread_create`, `pthread_join`, `pthread_detach` |
| **§14.4** | API Pthreads | Thread per client, thread timer |
| **§15.2** | Mutex | Protezione strutture condivise |
| **§15.3** | Condition Variable | Attesa senza busy waiting (se necessario) |
| **§17** | Socket TCP | Cuore del progetto |
| **§17.2** | Fasi TCP | `socket`, `bind`, `listen`, `accept`, `connect` |
| **§17.3** | Byte Order | `htons`, `ntohs`, `htonl`, `ntohl`, `inet_pton` |
| **§17.5** | Server TCP | Codice del server |
| **§17.6** | Client TCP | Codice del client |
| **§17.7** | `send`/`recv` | Funzioni di comunicazione |
| **§17.9** | `recv_all`/`send_all` | Lettura/scrittura affidabile (FONDAMENTALE) |
| **§17.11** | Server Concorrente | Pattern con thread o fork |
| **§17.12** | `setsockopt` | `SO_REUSEADDR`, `SO_RCVTIMEO` |
| **§17.14** | Pattern safe | Gestione short read/write |
| **§17.15** | Anti-Zombie | Se usi fork: gestione SIGCHLD |
| **§18** | `select()` | Loop del client (stdin + socket) |
| **§18.2-18.6** | Multiplexing | Pattern master set + working set |
| **§19** | SIGPIPE/EINTR | Robustezza delle operazioni di rete |

---

## 13. Scelte Tecniche Consigliate

### 13.1 Server: Thread vs Fork

| Criterio | Thread (`pthread`) | Fork (processi) |
|----------|--------------------|-----------------|
| **Condivisione dati** | Naturale (memoria condivisa) | Richiede IPC esplicito |
| **Overhead** | Basso | Più alto |
| **Sincronizzazione** | Mutex obbligatori | Più semplice (processi isolati) |
| **Gestione zombie** | Non necessaria | Richiede SIGCHLD handler |
| **Consiglio per il progetto** | ✅ **Preferita** | Alternativa valida |

**→ Usa i thread** (`§14` e `§17.11`). È più semplice condividere la struttura `GameState`.

### 13.2 Protocollo: Binario vs Testo

| Criterio | Binario (struct) | Testuale (es. "MOVE W\n") |
|----------|------------------|---------------------------|
| **Efficienza** | Alta | Bassa |
| **Semplicità di debug** | Bassa | Alta |
| **Portabilità byte order** | Richiede htons/ntohl | Non applicabile |
| **Consiglio** | ✅ **Per il progetto** | Accettabile ma meno professionale |

**→ Usa un protocollo binario** con header fisso + payload. Dimostra padronanza del materiale.

### 13.3 Rivelazione della Mappa

- **Finestra locale (4×4)**: inviata dopo ogni movimento, centrata sul giocatore
- **Mappa globale mascherata**: inviata ogni T secondi, con `'?'` per le celle non scoperte

Tieni separata la logica di "celle scoperte" (`discovered[][]`) per ogni giocatore.

### 13.4 Gestione del Timer

**→ Usa un thread dedicato** con `sleep(T)` in loop anziché `SIGALRM` (che è difficile da gestire in ambienti multithreaded e può interrompere system call).

### 13.5 Evitare Errori Comuni

| Errore Comune | Come Evitarlo |
|---------------|---------------|
| Scrittura su socket chiusa → crash | `signal(SIGPIPE, SIG_IGN)` all'inizio del server |
| `recv()` restituisce meno byte del previsto | Usa sempre `recv_all()` (§17.9/§17.14) |
| `select()` modifica il set | Usa pattern master_set + copia locale (§18.6) |
| Zombie process (se usi fork) | Handler SIGCHLD con `waitpid(-1, NULL, WNOHANG)` (§17.15) |
| Race condition sulla mappa | Ogni accesso alla struttura globale sotto `game_mutex` |
| Byte order sbagliato | `htons()`/`ntohs()` su ogni campo numerico del protocollo |
| Buffer overflow stringhe | Usa `strncpy`, `snprintf` — mai `strcpy` su dati dalla rete |

---

## 14. Compilazione e Makefile

```makefile
CC      = gcc
CFLAGS  = -Wall -Wextra -g -pthread
LDFLAGS = -pthread

# Compilazione server
server: server/server.c server/maze.c server/game.c server/protocol.c server/log.c
	$(CC) $(CFLAGS) -o server $^ $(LDFLAGS)

# Compilazione client
client: client/client.c client/display.c client/protocol.c
	$(CC) $(CFLAGS) -o client $^ $(LDFLAGS)

all: server client

clean:
	rm -f server client *.log

.PHONY: all clean
```

**Per compilare e testare in locale:**
```bash
# Terminale 1: avvia il server sulla porta 5200
./server 5200

# Terminale 2: avvia il primo client
./client 127.0.0.1 5200

# Terminale 3: avvia il secondo client
./client 127.0.0.1 5200
```

---

## 15. Traccia per la Relazione

La relazione deve essere di **~10 pagine** e contenere:

### Sezione 1 — Guida d'uso (compilazione e utilizzo)

```
Compilazione:
  make all

Avvio del server:
  ./server <porta>
  Esempio: ./server 5200

Avvio del client:
  ./client <indirizzo_IP_server> <porta>
  Esempio: ./client 192.168.1.10 5200

Comandi del client:
  W / A / S / D  → Movimento (su/sinistra/giù/destra)
  L              → Lista giocatori connessi
  Q              → Disconnessione
```

### Sezione 2 — Protocollo Applicativo

Descrivere:
- Il formato dell'header (tipo, flags, lunghezza)
- Ogni tipo di messaggio con la struttura del payload
- La sequenza di connessione e autenticazione
- La gestione del byte order (perché `htons`/`ntohs`)
- Il meccanismo di invio periodico della mappa globale

### Sezione 3 — Dettagli Implementativi

Descrivere (con frammenti di codice):
1. **Generazione del labirinto**: algoritmo usato
2. **Gestione della concorrenza**: thread per client + mutex globale
3. **Rivelazione della mappa**: come viene calcolata la zona visibile
4. **Il timer periodico**: implementazione con thread dedicato
5. **Gestione disconnessioni**: cosa succede quando un client si disconnette inaspettatamente
6. **Determinazione del vincitore**: logica delle 3 condizioni
7. **Gestione di SIGPIPE**: perché è necessaria e come è implementata

---

## Appendice — Pattern di Codice Fondamentali

### A. send_all / recv_all (da usare SEMPRE)

```c
ssize_t send_all(int fd, const void *buf, size_t n) {
    size_t sent = 0;
    const char *p = buf;
    while (sent < n) {
        ssize_t w = send(fd, p + sent, n - sent, 0);
        if (w <= 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        sent += (size_t)w;
    }
    return (ssize_t)sent;
}

ssize_t recv_all(int fd, void *buf, size_t n) {
    size_t received = 0;
    char *p = buf;
    while (received < n) {
        ssize_t r = recv(fd, p + received, n - received, 0);
        if (r < 0) { if (errno == EINTR) continue; return -1; }
        if (r == 0) return 0;  // EOF: connessione chiusa
        received += (size_t)r;
    }
    return (ssize_t)received;
}
```

### B. Invio di un Messaggio Completo

```c
int send_message(int fd, uint8_t type, const void *payload, uint16_t payload_len) {
    MsgHeader hdr;
    hdr.type   = type;
    hdr.flags  = 0;
    hdr.length = htons(payload_len);

    if (send_all(fd, &hdr, sizeof(hdr)) < 0) return -1;
    if (payload_len > 0 && send_all(fd, payload, payload_len) < 0) return -1;
    return 0;
}
```

### C. Ricezione di un Messaggio Completo

```c
int recv_message(int fd, uint8_t *type, void *payload, size_t max_payload) {
    MsgHeader hdr;
    if (recv_all(fd, &hdr, sizeof(hdr)) <= 0) return -1;

    *type = hdr.type;
    uint16_t len = ntohs(hdr.length);

    if (len > 0 && len <= max_payload) {
        if (recv_all(fd, payload, len) <= 0) return -1;
    }
    return (int)len;
}
```

---

> [!IMPORTANT]
> **Ricorda**: Il server non deve mai scrivere su `stdout` e non deve mai leggere da `stdin`. Tutto l'output di debug va su file di log o su `stderr` solo per errori fatali.

> [!TIP]
> **Per l'esame orale**: il server e il client verranno eseguiti su macchine diverse. Assicurati che il client accetti l'IP del server come argomento da riga di comando, non hardcodato.

> [!NOTE]
> **Registro delle attività da loggare**: connessione (con timestamp + nickname/IP), raccolta oggetti (timestamp + chi + dove), uscita dal labirinto (timestamp + chi), fine partita (timestamp + vincitore + score).
