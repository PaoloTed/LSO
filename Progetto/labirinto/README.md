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
│   └── log.c/h    logging su file (open/write)
└── client/        client interattivo
    ├── client.c   connessione, inserimento nickname, loop con select()
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
| `l` | lista dei giocatori connessi |
| `h` | aiuto |
| `q` | esci |

## Come giocare

1. Alla connessione inserisci il tuo nickname. Ogni client deve usare un nickname non
   attualmente in uso da un altro giocatore connesso.
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
Inserisci il tuo nickname: mario
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

L'invio e la ricezione avvengono con due funzioni ausiliarie (`invia_messaggio` e `ricevi_messaggio`) che gestiscono short read/write ed eventuali interruzioni (`EINTR`) con un semplice ciclo `while`.

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
| `MSG_INFO` | `16` | Server invia un messaggio testuale (es. oggetto raccolto) |

### Simboli della mappa

`#` muro, `.` cella libera, `E` uscita, `O` oggetto, `P` posizione del giocatore,
`?` cella non ancora scoperta.

### Sequenza di connessione

```
CLIENT                                  SERVER
  |------- TCP connect ------------------>|
  |-- MSG_LOGIN ------------------------->|
  |<-- MSG_OK ----------------------------|
  |<-- MSG_MAPPA_GLOBALE (iniziale) ------|
  |-- MSG_MOVE (w) ---------------------->|
  |<-- MSG_MAPPA_LOCALE ------------------|
  |           ...                         |
  |<-- MSG_MAPPA_GLOBALE (ogni T sec) ----|
  |-- MSG_QUIT -------------------------->|
  |<-- chiusura --------------------------|
```

## Note implementative

- **Concorrenza**: un thread per client (`pthread_create` + `pthread_detach`); un thread timer periodico controlla il timeout della partita e invia la mappa globale ogni T secondi.
- **Sincronizzazione**: un **unico mutex globale** `g_game.mutex` protegge lo stato della partita, il labirinto e i giocatori, evitando qualsiasi rischio di deadlock.
- **I/O di rete affidabile**: funzioni `invia_messaggio` e `ricevi_messaggio` con cicli `write`/`read` per gestire short read/write ed `EINTR`.
- **Client non bloccante**: il client utilizza la primitiva `select()` su `STDIN_FILENO` e `sock_fd` per ricevere la mappa globale periodica senza bloccarsi nell'attesa dell'input da tastiera.
- **Nessun dato persistente**: autenticazione rimossa; ai client è richiesto solo un nickname non duplicato.
- **Fine partita**: per timeout oppure quando tutti i giocatori connessi sono usciti. Vince l'unico giocatore uscito; altrimenti chi ha raccolto più oggetti.
