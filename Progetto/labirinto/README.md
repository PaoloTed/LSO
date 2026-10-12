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
│   ├── server.c   main, accept (while 1), timer thread, thread per client
│   ├── game.c/h   labirinto dinamico, spawn, uscite casuali, movimento, fine partita
│   └── log.c/h    logging su file (open 0644, write, snprintf, mutex)
└── client/        client interattivo
    ├── client.c   connessione, select(), segnali (SIGINT/Ctrl+C -> MSG_QUIT)
    └── display.c/h  stampa delle mappe e delle liste a colori ANSI
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

Avvia il **server** in un terminale (resta in esecuzione; non stampa nulla a video, scrive su
`server.log`):

```powershell
wsl ./bin/server 5200 600 15
```

Avvia uno o più **client**, ciascuno in un terminale diverso:

```powershell
wsl ./bin/client 127.0.0.1 5200
```

- `5200` è la porta TCP.
- `600` è il timeout della partita in secondi.
- `15` è l'intervallo di invio periodico della mappa globale in secondi.

> Su Linux nativo gli stessi comandi sono `make`, `./bin/server 5200 600 15` e
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

Il server non scrive sullo standard output e non legge dallo standard input (come da specifica).
Le attività vengono registrate nel file `server.log`. Non viene salvato alcun dato permanente su disco (nessun database utenti).

## Uso del client

```bash
./bin/client <indirizzo_ip> <porta>
```

- `indirizzo_ip`: indirizzo IPv4 del server (es. `127.0.0.1`).
- `porta`: porta TCP del server (es. `5200`).

Esempio:

```bash
./bin/client 127.0.0.1 5200
```

Dopo la connessione viene richiesto solamente il proprio nickname. Comandi disponibili durante la partita:

| Comando | Azione |
|---------|--------|
| `w` `a` `s` `d` | muovi su / sinistra / giù / destra |
| `l` | lista dei giocatori connessi e punteggi |
| `h` | aiuto |
| `q` | esci (oppure `Ctrl+C` / `Ctrl+D`) |

## Come giocare e regole del gioco

1. **Accesso**: alla connessione inserisci il nickname (univoco tra i giocatori attualmente connessi).
2. **Spawn**: ciascun giocatore entra nella mappa posizionandosi casualmente in uno degli **8 punti di partenza perimetrali** del labirinto.
3. **Movimento**: muoviti con `w`/`a`/`s`/`d` e premi Invio. Ad ogni passo il server scopre l'area attorno a te e invia la vista locale 5×5.
4. **Oggetti (`O`)**: passando su una cella con `O` l'oggetto viene raccolto, rimosso dalla mappa e il tuo punteggio incrementa di 1.
5. **Uscite (`E`)**: le **4 uscite** sono posizionate casualmente sui 4 bordi del labirinto (in celle garantite adiacenti a corridoi interni).
6. **Mappa Globale**: ogni T secondi il server invia a ciascun giocatore l'intera mappa con le sole celle da lui scoperte (le altre sono mascherate con `?`).
7. **Fine Partita**: la partita termina allo scadere del timeout globale oppure quando tutti i giocatori connessi sono usciti.
   - Se esce un solo giocatore → vince lui;
   - Se escono più giocatori o nessuno → vince chi ha raccolto più oggetti.

Simboli della mappa:
- `#` muro, `.` cella libera, `E` uscita, `O` oggetto, `P` posizione del giocatore, `?` cella nascosta.

## Protocollo applicativo

La comunicazione si basa su una struttura C fissa (`Messaggio`) scambiata tramite socket TCP:

```c
typedef struct {
    int  type;                              /* Tipo di messaggio (MSG_*) */
    char nickname[MAX_NICK];                /* Nickname per login o vincitore */
    char direction;                         /* Direzione: 'w', 'a', 's', 'd' */
    char text[MAX_TEXT];                    /* Testo descrittivo o messaggio di errore */
    int  row;                               /* Riga corrente */
    int  col;                               /* Colonna corrente */
    int  score;                             /* Punteggio corrente */
    
    char local_map[LOCAL_VIEW][LOCAL_VIEW]; /* Vista locale 5x5 */
    char global_map[MAP_ROWS][MAP_COLS];    /* Mappa globale 21x41 mascherata */
    
    int  num_players;                       /* Numero di giocatori nella lista */
    InfoGiocatore players[MAX_PLAYERS];     /* Array dei giocatori connessi */
} Messaggio;
```

L'invio e la ricezione avvengono tramite funzioni dedicate (`invia_messaggio` e `ricevi_messaggio`) che gestiscono short read/write ed eventuali interruzioni da segnali (`EINTR`) con cicli `while`.

### Tipi di messaggio

| Tipo | Costante | Descrizione |
|------|:--------:|-------------|
| `MSG_LOGIN` | `1` | Client invia il proprio nickname per accedere |
| `MSG_MOVE` | `2` | Client invia la direzione di movimento (`w`/`a`/`s`/`d`) |
| `MSG_LIST` | `3` | Client richiede la lista dei giocatori connessi |
| `MSG_QUIT` | `4` | Client notifica la disconnessione volontaria |
| `MSG_OK` | `10` | Server conferma il login con successo |
| `MSG_ERROR` | `11` | Server segnala errore (es. nickname duplicato o partita finita) |
| `MSG_MAPPA_LOCALE` | `12` | Server invia la vista 5x5 centrata sul giocatore |
| `MSG_MAPPA_GLOBALE` | `13` | Server invia periodicamente la mappa globale mascherata |
| `MSG_LISTA` | `14` | Server invia l'elenco dei giocatori e punteggi |
| `MSG_FINE_PARTITA` | `15` | Server notifica la fine della partita e il vincitore |
| `MSG_INFO` | `16` | Server invia un messaggio informativo |

### Sequenza tipica di comunicazione

```
CLIENT                                  SERVER
  |------- TCP connect() ---------------->|
  |-- MSG_LOGIN (nickname) -------------->|
  |<-- MSG_OK ----------------------------|
  |<-- MSG_MAPPA_GLOBALE (iniziale) ------|
  |-- MSG_MOVE (w) ---------------------->|
  |<-- MSG_MAPPA_LOCALE ------------------|
  |           ...                         |
  |<-- MSG_MAPPA_GLOBALE (ogni T sec) ----|
  |-- MSG_QUIT (o Ctrl+C) --------------->|
  |<-- chiusura socket -------------------|
```

## Note implementative e scelte tecniche

- **Concorrenza Multithread**: il server gestisce le connessioni con un ciclo `while (1)` e alloca un thread per client (`pthread_create` + `pthread_detach`). Un thread dedicato `timer_thread` gestisce il conteggio dei secondi per il timeout e i broadcast periodici.
- **Sincronizzazione**: un **unico mutex globale** `g_game.mutex` protegge lo stato della partita, il labirinto e i giocatori, garantendo assenza di race condition e prevenendo deadlock.
- **Logging Thread-Safe (`log.c`)**:
  - Usa la classica system call a basso livello `open("server.log", O_WRONLY | O_CREAT | O_APPEND, 0644)` e `write()`.
  - Data e ora formattate con `strftime` (`[AAAA-MM-GG HH:MM:SS]`).
  - Composizione sicura con `snprintf` a prova di buffer overflow.
  - Protetto da un mutex dedicato `log_mutex` per impedire l'interleaving dei caratteri sul file tra thread concorrenti.
- **Generazione Labirinto Dinamico (`game.c`)**:
  - Algoritmo a cammini casuali (random walk) da 8 punti perimetrali verso l'hub centrale, garantendo percorsi sempre aperti e connessi.
  - 4 uscite (`E`) posizionate casualmente sui 4 bordi, verificate come adiacenti a celle calpestabili e distinte dai punti di spawn.
- **Gestione Segnali nel Client (`client.c`)**:
  - `signal(SIGINT, handle_sigint)` e `signal(SIGTERM, handle_sigint)`: alla pressione di `Ctrl+C` il client invia automaticamente `MSG_QUIT` al server prima di terminare, garantendo la rimozione pulita del giocatore dalla partita.
- **I/O Multiplexing con `select()`**:
  - Il client usa `select()` su `STDIN_FILENO` e `sock_fd` per ricevere la mappa globale periodica dal server senza bloccarsi nell'attesa dell'input da tastiera.
- **I/O di Rete Robusto (`protocol.c`)**:
  - Cicli `while (rimanenti > 0)` su `write()` e `read()` con gestione delle short read/write e ripresa su `errno == EINTR`.
