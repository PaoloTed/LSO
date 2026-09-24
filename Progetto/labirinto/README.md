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

Apri un terminale nella cartella del progetto (quella con il `Makefile`) e compila:

```bash
make
```

Su Windows, dalla stessa cartella aperta in PowerShell, basta prefissare con `wsl`:

```powershell
wsl make
```

Produce i due eseguibili `bin/server` e `bin/client`. Per ripulire: `make clean`.

## Avvio rapido

Apri PowerShell e portati nella cartella del progetto (dove sta il `Makefile`). `wsl`
mantiene la cartella corrente, quindi non servono percorsi assoluti.

Avvia il **server** in un terminale (resta in esecuzione; non stampa nulla, scrive su
`server.log`):

```powershell
wsl ./bin/server 5200 600 15
```

Avvia uno o più **client**, ciascuno in un terminale diverso:

```powershell
wsl ./bin/client 127.0.0.1 5200
```

`5200` è la porta, `600` il timeout in secondi, `15` il periodo della mappa globale.
Avviati e connettiti subito: il timeout parte all'avvio del server, quindi usa un timeout
lungo (es. `600`) per giocare con calma.

> Su Linux, senza `wsl`, gli stessi comandi sono `make`, `./bin/server 5200 600 15` e
> `./bin/client 127.0.0.1 5200`.

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

## Come giocare

1. Alla connessione scegli `2` per registrarti (solo la prima volta), inserendo nickname e
   password; alle volte successive scegli `1` e accedi con le stesse credenziali. Ogni
   client deve usare un nickname diverso.
2. Muoviti con `w`/`a`/`s`/`d`: digita **una lettera per riga** e premi Invio. Ad ogni passo
   il server scopre l'area attorno a te e ti invia la vista locale.
3. Passando su una cella con `O` l'oggetto viene raccolto e il punteggio aumenta.
4. Obiettivo: raggiungere una cella `E` (uscita). La partita termina allo scadere del
   timeout oppure quando tutti i giocatori connessi sono usciti. Vince l'unico giocatore
   uscito; altrimenti chi ha raccolto più oggetti.
5. Ogni T secondi ricevi la mappa globale con le sole celle che hai già scoperto.

Simboli e colori:

- `#` muro, `.` cella libera, `E` uscita, `O` oggetto, `P` la tua posizione, `?` non scoperta.
- Nella mappa locale le celle sono **blu**, `P` è **rosso**, `O` è **giallo**.
- Nella mappa globale è **blu** solo la finestra locale attuale (5×5 attorno a te).

Esempio di sessione:

```
Scelta: 2
Nickname: mario
Password: ****
Accesso effettuato come 'mario'.
...
w
Posizione: (riga 4, colonna 12)   Oggetti raccolti: 0
  . . # # #
  . # . . .
  . # P . #
  . . . # .
  # . # . .
```

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
