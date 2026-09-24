# Progetto LSO — Traccia A: Labirinto con oggetti e uscita

Sistema client–server in C (UNIX/Linux) per un gioco di esplorazione di un labirinto 2D.
Il server genera il labirinto, gestisce più giocatori contemporaneamente in thread separati,
invia a ciascuno una vista locale e, periodicamente, una mappa globale mascherata.
La comunicazione avviene tramite socket TCP con un protocollo applicativo binario.

## Struttura

```
labirinto/
├── Makefile
├── common/        protocollo condiviso (header, send/recv affidabili)
│   ├── protocol.h
│   └── protocol.c
├── server/        logica del server
│   ├── server.c   main, accept, thread per client
│   ├── game.c/h   labirinto, giocatori, movimento, timer, fine partita
│   ├── auth.c/h   registrazione e login con password (file users.db)
│   └── log.c/h    logging su file (open/write)
└── client/        client interattivo
    ├── client.c   connessione, autenticazione, loop con select()
    └── display.c/h  stampa delle mappe e delle liste
```

## Compilazione

```bash
make
```

Produce i due eseguibili `bin/server` e `bin/client`. Per ripulire: `make clean`.

## Uso del server

```bash
./bin/server <porta> [timeout] [intervallo]
```

- `porta`: porta TCP di ascolto (obbligatoria).
- `timeout`: durata massima della partita in secondi (default 180).
- `intervallo`: periodo di invio della mappa globale in secondi (default 20).

Esempio:

```bash
./bin/server 5200 300 15
```

Il server non scrive sullo standard output e non legge dallo standard input.
Le attività vengono registrate nel file `server.log`; gli utenti sono salvati in `users.db`.

## Uso del client

```bash
./bin/client <host> <porta>
```

`host` può essere un indirizzo IP o un nome simbolico (es. `localhost`). Il client risolve
il nome tramite `getaddrinfo`. Esempio:

```bash
./bin/client 127.0.0.1 5200
./bin/client lab012.studenti.unina.it 5200
```

Dopo la connessione si sceglie se accedere o registrarsi, poi si inseriscono nickname e
password. Comandi disponibili durante la partita:

| Comando | Azione |
|---------|--------|
| `w` `a` `s` `d` | muovi su / sinistra / giù / destra |
| `l` | lista dei giocatori connessi |
| `m` | rivedi l'ultima mappa locale |
| `g` | rivedi l'ultima mappa globale |
| `h` | aiuto |
| `q` | esci |

## Protocollo applicativo

Ogni messaggio è composto da un header fisso di 4 byte seguito da un payload:

```
[ tipo:1 ][ flags:1 ][ lunghezza:2 ]
```

La lunghezza è la dimensione del payload ed è espressa in **network byte order**
(`htons`/`ntohs`). Tutti i campi numerici a 32 bit dei payload sono convertiti con
`htonl`/`ntohl`.

### Messaggi client → server

| Tipo | Valore | Payload |
|------|:------:|---------|
| `MSG_REGISTER` | `0x01` | nickname[32], password[32] |
| `MSG_LOGIN` | `0x02` | nickname[32], password[32] |
| `MSG_MOVE` | `0x03` | direzione (1 byte: `w`/`a`/`s`/`d`) |
| `MSG_LIST` | `0x04` | — |
| `MSG_QUIT` | `0x05` | — |

### Messaggi server → client

| Tipo | Valore | Payload |
|------|:------:|---------|
| `MSG_OK` | `0x10` | — |
| `MSG_ERROR` | `0x11` | testo[128] |
| `MSG_LOCAL_MAP` | `0x12` | riga, colonna, punteggio, righe, colonne (5×u32) + griglia righe×colonne |
| `MSG_GLOBAL_MAP` | `0x13` | righe, colonne (2×u32) + griglia righe×colonne |
| `MSG_PLAYER_LIST` | `0x14` | numero (u32) + numero × (nickname[32], punteggio u32, uscito u32) |
| `MSG_GAME_OVER` | `0x15` | vincitore[32], punteggio u32 |
| `MSG_INFO` | `0x16` | testo[128] |

### Simboli della mappa

`#` muro, `.` cella libera, `E` uscita, `O` oggetto, `P` posizione del giocatore,
`?` cella non ancora scoperta.

### Sequenza di connessione

```
CLIENT                                  SERVER
  |------- TCP connect ------------------>|
  |-- MSG_REGISTER / MSG_LOGIN ---------->|
  |<-- MSG_OK ----------------------------|
  |<-- MSG_INFO (benvenuto) --------------|
  |<-- MSG_LOCAL_MAP ---------------------|
  |-- MSG_MOVE (w) ---------------------->|
  |<-- MSG_INFO / MSG_LOCAL_MAP ----------|
  |           ...                         |
  |<-- MSG_GLOBAL_MAP (ogni T secondi) ---|
  |-- MSG_QUIT -------------------------->|
  |<-- chiusura --------------------------|
```

La mappa locale è una finestra 5×5 centrata sul giocatore e contiene solo le celle
scoperte. La mappa globale è l'intera matrice, con le celle non ancora viste indicate
da `?`.

## Note implementative

- **Concorrenza**: un thread per client (`pthread_create` + `pthread_detach`); un thread
  dedicato invia la mappa globale e controlla il timeout.
- **Sincronizzazione**: un mutex globale protegge labirinto, oggetti e stato dei giocatori;
  un mutex per giocatore serializza le scritture sulla sua socket (il thread del client e
  quello del timer possono inviare contemporaneamente).
- **I/O affidabile**: `recv_all`/`send_all` gestiscono short read/write e `EINTR`.
- **SIGPIPE** viene ignorato all'avvio, così una disconnessione non termina il server.
- **Labirinto**: generato con DFS (recursive backtracker) sulle celle dispari, con
  aperture aggiuntive per creare percorsi alternativi, uscite sul bordo e oggetti nelle
  celle libere.
- **Password**: salvate come hash (djb2) in `users.db`, non in chiaro.
- **Fine partita**: per timeout oppure quando tutti i giocatori connessi sono usciti.
  Vince l'unico giocatore uscito; altrimenti chi ha raccolto più oggetti.
