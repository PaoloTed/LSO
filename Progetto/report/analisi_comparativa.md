# Analisi Comparativa: TicTacToe vs Labirinto
### Progetto LSO — Anno Accademico Terzo Anno

---

## 1. Panoramica Generale

| Caratteristica | TicTacToe | Labirinto |
|---|---|---|
| **Tipo di gioco** | Tris 1v1 (turn-based) | Esplorazione labirinto multiplayer |
| **Numero di giocatori** | 2 (fisso) | Fino a 32 (MAX_PLAYERS) |
| **Modalità di gioco** | A turni, sincrono | In tempo reale, asincrono |
| **Porta configurabile** | No (hardcoded `8080`) | Sì (passata come argomento) |
| **Sistema di log** | Solo `printf` a video | File dedicato `server.log` con timestamp |
| **Supporto Docker** | Sì (Dockerfile + docker-compose) | No |
| **Makefile unificato** | No (separato per client/server) | Sì (un unico Makefile radice) |
| **Numero totale di righe** | ~1500 (server) + ~848 (client funzioni) | ~225 (server) + ~494 (game) + ~232 (client) |

---

## 2. Architettura del Progetto

### 2.1 Organizzazione dei File

**TicTacToe** ha una struttura **monolitica e non condivisa**:
```
tris-lso/
├── Client/          ← cartella completamente separata
│   ├── Comunicazione.h   ← copia del file del Server
│   ├── client.c
│   ├── funzioni.c    ← 848 righe, tutta la logica client qui
│   ├── funzioni.h
│   └── strutture.h
└── Server/          ← cartella completamente separata
    ├── Comunicazione.h   ← duplicato del file del Client
    ├── server.c      ← 925 righe
    ├── funzioni.c    ← 14025 righe server logic
    ├── funzioni.h
    └── strutture.h
```

> [!WARNING]
> Il file `Comunicazione.h` è **duplicato** in entrambe le cartelle Client e Server. Le due copie potrebbero divergere nel tempo. Già ora presentano differenze minori (es. `#define PORT` assente nel client, note a piè di pagina diverse). Questo è un anti-pattern classico.

**Labirinto** ha una struttura **modulare con codice condiviso**:
```
labirinto/
├── Makefile          ← unico punto di build per tutto
├── common/           ← codice condiviso client+server
│   ├── protocol.h
│   └── protocol.c
├── server/
│   ├── server.c      ← 225 righe (solo entry point e accept loop)
│   ├── game.c/h      ← logica di gioco separata
│   └── log.c/h       ← modulo logging separato
└── client/
    ├── client.c      ← 232 righe
    └── display.c/h   ← rendering separato
```

La cartella `common/` è compilata una volta e inclusa sia dal server che dal client. Non c'è duplicazione di codice.

---

## 3. Protocollo di Comunicazione

Questa è la **differenza più sostanziale** tra i due progetti.

### 3.1 TicTacToe — Protocollo testuale a stringhe

Il server e il client si scambiano **stringhe ASCII** grezze tramite `send`/`recv` su un buffer da 4096 byte:

```c
// Esempio di invio (TicTacToe)
send(giocatore->socket, MSG_SERVER_MENU, strlen(MSG_SERVER_MENU), 0);
recv(giocatore->socket, buffer, sizeof(buffer), 0);
if (strcmp(buffer, MSG_CLIENT_CREAATE) == 0) { ... }
```

- Le costanti sono stringhe human-readable: `"MENU"`, `"CREATE"`, `"YOUR_TURN"`, ecc.
- Non esiste framing: si assume che ogni `recv` riceva esattamente un messaggio.
- Questo crea un bug latente: TCP è uno **stream protocol**, non un message protocol. Un messaggio può arrivare spezzato, o due messaggi possono arrivare in una singola `recv`. Il codice non gestisce questo caso.
- Sono presenti `usleep(100000)` (100ms) sparsi nel codice del server come "workaround" per evitare che messaggi consecutivi si fondano. Soluzione fragile.
- **Typo nel codice**: `MSG_CLIENT_CREAATE` (doppia A) in `Comunicazione.h` e nel codice client.

### 3.2 Labirinto — Protocollo binario strutturato con framing implicito

Il labirinto usa una **singola struttura C** (`Messaggio`) di dimensione fissa che viene trasmessa come un blocco binario:

```c
typedef struct {
    int  type;                              // tipo messaggio
    char nickname[MAX_NICK];               
    char direction;                         
    char text[MAX_TEXT];                    
    int  row, col, score;                   
    char local_map[LOCAL_VIEW][LOCAL_VIEW]; // vista 5x5
    char global_map[MAP_ROWS][MAP_COLS];    // mappa 21x41
    int  num_players;
    InfoGiocatore players[MAX_PLAYERS];
} Messaggio;
```

Le funzioni `invia_messaggio` e `ricevi_messaggio` in `protocol.c` gestiscono correttamente **short read/write** ed `EINTR`:

```c
while (rimanenti > 0) {
    ssize_t scritti = write(fd, ptr + inviati, rimanenti);
    if (scritti < 0 && errno == EINTR) continue;
    ...
}
```

> [!NOTE]
> L'approccio del Labirinto è quello **corretto** per TCP. Il messaggio ha dimensione nota (`sizeof(Messaggio)`), eliminando ogni problema di framing. Nel TicTacToe invece, la mancanza di framing è un bug potenzialmente silenzioso in condizioni di carico.

---

## 4. Gestione della Concorrenza (Thread e Mutex)

### 4.1 TicTacToe

- Un thread per ogni client connesso (`threadLobby`).
- Un thread separato per ogni partita in corso (`threadPartita`).
- **Un solo mutex** (`lobby.lobbyMutex`) protegge la struttura `Lobby`.
- Il problema: `Giocatori` (array dei giocatori connessi) è accessibile da più thread **senza mutex**. Ci sono accessi a `giocatori.giocatore[i].socket` e `.stato` senza lock, che costituiscono **race condition**.
- Gli `usleep` sparsi sono usati anche come forma di "sincronizzazione" (es. `usleep(100000)` dopo aver mandato un messaggio), il che non è una garanzia formale.

### 4.2 Labirinto

- Un thread per ogni client + un thread dedicato `timer_thread` per il timeout e i broadcast periodici.
- **Un unico mutex globale** (`g_game.mutex`) protegge l'intero stato di gioco (labirinto, array giocatori, flag `game_over`, vincitore).
- Il mutex viene acquisito e rilasciato in modo sistematico in ogni funzione pubblica di `game.c`.
- Non ci sono accessi non protetti allo stato condiviso.

> [!IMPORTANT]
> La gestione del mutex nel Labirinto è notevolmente più rigorosa. TicTacToe ha race condition identificabili sull'array `giocatori`.

---

## 5. Scalabilità e Modalità di Gioco

| Aspetto | TicTacToe | Labirinto |
|---|---|---|
| **Modello** | Multi-partita (lobby) | Singola sessione globale |
| **Max giocatori** | 20 (MAX_CLIENTS=20) | 32 (MAX_PLAYERS) |
| **Partite simultanee** | Sì (fino a MAX_GAMES=10 partite parallele) | No (una sola partita) |
| **Avvio partita** | Negoziale (un giocatore crea, l'altro si unisce e l'admin accetta/rifiuta) | Automatico al login |
| **Fine partita** | Vittoria/pareggio/rematch | Timeout o quando tutti escono |
| **Rematch** | Sì (gestito) | No |

Il TicTacToe implementa una **lobby multiplayer** con logica di creazione/join e negoziazione admin/guest che è concettualmente più complessa del Labirinto in termini di flusso di gioco (stati: IN_ATTESA → IN_CORSO → TERMINATA).

Il Labirinto, d'altra parte, è **più avanzato tecnicamente** (protocollo, logging, modularità, correttezza concorrenza).

---

## 6. Client

### 6.1 TicTacToe Client

- `client.c` è di sole **73 righe**: il `main` fa il login e poi chiama `funzione_menu()`.
- Tutta la logica è in `funzioni.c` (848 righe), un file monolitico.
- Il client usa **solo `recv` bloccante**: non può ricevere messaggi dal server mentre aspetta input dell'utente, a meno che i messaggi non arrivino nel momento giusto.
- IP e porta hardcoded a `127.0.0.1:8080` (non configurabili da riga di comando).
- Countdown artificiale di 6 secondi all'avvio (`for i in 5..0: sleep(1)`).

### 6.2 Labirinto Client

- `client.c` è di **232 righe** ben strutturate, tutto contenuto nel main.
- Usa `select()` su `STDIN_FILENO` e `sock_fd` per **I/O multiplexing**: può ricevere la mappa globale periodica dal server *senza bloccarsi* ad aspettare l'input da tastiera.
- IP e porta **configurabili da riga di comando** con validazione.
- Gestione `SIGINT`/`SIGTERM`: invia `MSG_QUIT` prima di chiudersi.
- Modulo `display.c` separato con **colori ANSI** per la visualizzazione.

---

## 7. Logging e Osservabilità

| | TicTacToe | Labirinto |
|---|---|---|
| **Output server** | `printf` su stdout | Nessun stdout (come da spec). File `server.log` |
| **Formato log** | Testo libero | `[AAAA-MM-GG HH:MM:SS] evento` |
| **Thread-safety log** | N/A | Mutex dedicato (`log_mutex`) |
| **System call** | `printf` | `open(O_WRONLY|O_CREAT|O_APPEND)` + `write()` |

Il TicTacToe non ha un sistema di logging: tutti i messaggi diagnostici vanno su `stdout` via `printf`. Il Labirinto ha un modulo `log.c` dedicato che usa system call POSIX di basso livello (`open`, `write`) protette da mutex, con timestamp formattati via `strftime`.

---

## 8. Portabilità e Deployment

| | TicTacToe | Labirinto |
|---|---|---|
| **Docker** | Sì (Dockerfile per client e server, docker-compose) | No |
| **Script di avvio** | Unix.sh, Mac.sh, Windows.bat, runWin.ps1 | Istruzioni `wsl make` nel README |
| **Compilazione** | Makefile separati (uno per Client, uno per Server) | Un Makefile radice unificato |
| **Flags compilatore** | `-O3 -march=native -pipe` (ottimizzazione massima) | `-Wall -Wextra -g` (debug, warning) |

Il TicTacToe è più orientato alla **portabilità e al deployment** (Docker, script multi-OS). Il Labirinto usa flags di debug (`-g`) invece di ottimizzazione, più adatto durante lo sviluppo.

---

## 9. Qualità del Codice

### TicTacToe — Problemi identificati

1. **`Comunicazione.h` duplicato** tra Client e Server.
2. **Typo** nel codice: `MSG_CLIENT_CREAATE` (doppia A).
3. **Race condition** sull'array `giocatori` senza mutex.
4. **Mancanza di framing TCP**: `recv` senza gestione di short read.
5. **`usleep` come pseudo-sincronizzazione**: approccio fragile.
6. **IP e porta hardcoded** nel client (`127.0.0.1:8080`).
7. Codice del server molto lungo (925 righe in `server.c` + 14KB di `funzioni.c`) con logica di rematch duplicata almeno 3 volte.
8. `funzioni.c` del Server è da 14025 bytes e contiene tutto, inclusa la logica di gioco.

### Labirinto — Qualità

1. Nessuna duplicazione di header: `protocol.h` è in `common/` e incluso da entrambi.
2. Protocollo robusto con gestione short read/write e `EINTR`.
3. Mutex usato correttamente e in modo sistematico.
4. Moduli ben separati: `game.c`, `log.c`, `display.c`, `protocol.c`.
5. Porta e parametri configurabili da linea di comando con validazione.
6. `select()` nel client per I/O non bloccante.

---

## 10. Sintesi: Chi è "più avanzato"?

```
CRITERIO                    TicTacToe   Labirinto
─────────────────────────────────────────────────
Complessità gameplay            ★★★         ★★
Correttezza protocollo TCP      ★           ★★★★★
Organizzazione codice           ★★          ★★★★★
Gestione concorrenza            ★★★         ★★★★
Scalabilità giocatori           ★★          ★★★★
Osservabilità (logging)         ★           ★★★★★
Portabilità/Docker              ★★★★★       ★★
I/O multiplexing client         ★           ★★★★★
Qualità codice generale         ★★          ★★★★
```

### Conclusione

I due progetti sono **complementari nelle debolezze e nei punti di forza**:

- **TicTacToe** è più ambizioso sul piano del **design di gioco** (lobby, multi-partita, negoziazione admin/guest, rematch) e del deployment (Docker, script multi-OS). Tuttavia mostra lacune tecniche importanti: protocollo TCP non robusto, race condition, codice duplicato e poco modulare.

- **Labirinto** è tecnicamente più **rigoroso e corretto**: protocollo binario con framing, mutex sistematico, moduli separati, logging su file, I/O multiplexing con `select()`. Il gioco in sé è più semplice (sessione unica, no rematch, no lobby).

> [!TIP]
> Se i due progetti sono dello stesso autore o gruppo, il Labirinto sembra essere stato scritto **dopo** aver appreso dagli errori del TicTacToe. Se sono di autori diversi, il Labirinto dimostra una comprensione più matura dei concetti di LSO (TCP, thread safety, POSIX I/O).

---

*Documento generato il 2026-10-12 — Analisi su codice sorgente C, progetti LSO Terzo Anno*
