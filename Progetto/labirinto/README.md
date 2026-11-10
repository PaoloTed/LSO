# Progetto LSO — Traccia A: Labirinto con oggetti e uscita

Sistema client–server in C (UNIX/Linux) per un gioco di esplorazione di un labirinto 2D.
Il server genera il labirinto, gestisce più giocatori contemporaneamente in thread dedicati (modello *thread-per-client*), invia a ciascuno una vista locale e, periodicamente, una mappa globale mascherata.
La comunicazione avviene tramite socket TCP con un protocollo applicativo binario.

## Struttura

```
labirinto/
├── Makefile
├── users.txt          archivio persistente delle credenziali, creato alla prima registrazione se assente
├── server.log         file delle attività del server, creato all'avvio se assente
├── common/            definizioni e funzioni di I/O del protocollo condiviso
│   ├── protocol.h     definizioni costanti, tipi di messaggi e strutture dati
│   └── protocol.c     invio e ricezione dati con gestione short read/write
├── server/            logica del server
│   ├── server.c       main, socket di ascolto, accept, timer thread, thread per client
│   ├── game.c/h       labirinto, spawn casuale, autenticazione, movimento, punteggi, vincitore
│   └── log.c/h        logging su file a basso livello (open 0644, O_APPEND, write, log_mutex)
├── client/            client interattivo
│   ├── client.c       risoluzione getaddrinfo, autenticazione, multiplexing select(), segnali (SIGINT -> MSG_ESCI)
│   └── display.c/h    rendering delle mappe con colori ANSI e visualizzazione della lista giocatori
└── docker/            configurazione Docker Compose, Dockerfile e script di avvio dei client
```

## Compilazione

Con `gcc` e `make` installati, compila dalla cartella che contiene il `Makefile`.

```bash
make
```

Su Windows con WSL (dalla stessa cartella aperta in PowerShell):

```powershell
wsl make
```

Il comando produce i due eseguibili `bin/server` e `bin/client`.
`make clean` elimina la cartella `bin/` e il file `server.log`.

## Avvio rapido

1. Avvia il **server** in un terminale. Resta in primo piano e scrive le attività di gioco in `server.log`. Gli errori di avvio vengono stampati su `stderr`.

```bash
./bin/server 5200 600 15
```

Su Windows con WSL:
```powershell
wsl ./bin/server 5200 600 15
```

- `5200` è la porta TCP di ascolto.
- `600` è il timeout globale della partita in secondi, contato dall'avvio del server.
- `15` è l'intervallo di invio periodico della mappa globale in secondi.

2. Avvia uno o più **client**, ciascuno in un terminale separato:

```bash
./bin/client localhost 5200
```

Su Windows con WSL:
```powershell
wsl ./bin/client 127.0.0.1 5200
```

Nel menu iniziale scegli **Accedi** oppure **Registrati**, inserisci nickname e password e poi seleziona **Gioca**.

## Avvio con Docker Compose

### Avvio con lo script

Per avviare il server e aprire più client in finestre separate, usa [launch_clients.sh](docker/launch_clients.sh).

```bash
cd docker
bash launch_clients.sh 0 3
```

Il primo argomento è `0` per compilare le immagini o `1` per riutilizzare quelle già disponibili. Il secondo è il numero di client. Lo script arresta i container del progetto, avvia il server e apre i client con `x-terminal-emulator`, `alacritty`, `konsole` o `xterm`. Se usi WSL, serve un emulatore grafico disponibile nella distribuzione e lo script deve avere terminatori di riga LF.

### Avvio manuale

Compila le immagini e avvia il server dalla cartella del progetto, in una shell Linux o WSL.

```bash
touch users.txt server.log
docker compose -f docker/docker-compose.yml build
docker compose -f docker/docker-compose.yml up -d server
```

Il server ascolta sulla porta `8080`, con timeout di `180` secondi e aggiornamenti della mappa ogni `20` secondi. I file `users.txt` e `server.log` vengono montati dalla cartella del progetto.

Esegui questo comando dalla stessa cartella in ogni terminale in cui vuoi avviare un client.

```bash
docker compose -f docker/docker-compose.yml run --rm client
```

Per arrestare il server, esegui `docker compose -f docker/docker-compose.yml down`.

## Uso del server

```bash
./bin/server <porta> [timeout] [intervallo]
```

- `porta`: porta TCP di ascolto (obbligatoria).
- `timeout`: durata massima della partita in secondi dall'avvio del server (opzionale, default 180).
- `intervallo`: periodo di invio automatico della mappa globale in secondi (opzionale, default 20).

Esempio:
```bash
./bin/server 5200 300 15
```

- Il server non produce output su `stdout` e non legge da `stdin`.
- Il file `server.log` registra con timestamp gli ingressi in partita, le disconnessioni, le raccolte di oggetti, il raggiungimento delle uscite e l'esito della partita.
- Nickname e password vengono salvati in chiaro in `users.txt`. Dal menu iniziale del client è possibile registrare un nuovo utente (`MSG_REGISTRAZIONE`) oppure accedere con un profilo già registrato (`MSG_ACCESSO`).
- A partita terminata il server continua ad ascoltare e rifiuta nuovi ingressi. Per iniziare una nuova partita occorre riavviarlo.

## Uso del client

```bash
./bin/client <nome_host|indirizzo_ip> <porta>
```

- `nome_host|indirizzo_ip`: nome host con un indirizzo IPv4 (es. `localhost`) oppure indirizzo IPv4 numerico (es. `127.0.0.1`), risolto tramite `getaddrinfo()`.
- `porta`: porta TCP del server (es. `5200`).

Esempio:
```bash
./bin/client localhost 5200
```

Dopo la connessione TCP, il menu iniziale offre **Accedi**, **Registrati** ed **Esci**. Per accedere o registrarsi, inserisci nickname e password. Dopo la conferma, il menu successivo offre **Gioca** ed **Esci**.

Durante la partita, conferma ogni comando digitato con Invio.

| Comando | Azione |
|:-------:|--------|
| `w` `a` `s` `d` | Muovi su / sinistra / giù / destra |
| `l` | Visualizza i giocatori connessi, con punteggio e stato (in gioco o usciti dal labirinto) |
| `h` | Mostra il menu di aiuto con i comandi disponibili |
| `q` | Disconnessione e uscita ordinata (disponibile anche tramite `Ctrl+C` o `Ctrl+D`) |

## Regole del gioco

1. **Accesso e registrazione**: la registrazione richiede un nickname assente da `users.txt`. Per accedere serve un account registrato che non sia già connesso.
2. **Spawn**: il server posiziona ciascun giocatore in una cella interna percorribile, scelta a caso tra quelle senza oggetti. All'inizio il giocatore vede soltanto la propria cella.
3. **Movimento**: digitando `w`/`a`/`s`/`d` e premendo Invio, il giocatore si sposta. Ad ogni passo valido il server "rivela" una zona attorno a lui (raggio di 2 celle, vista 5×5) che resta permanentemente scoperta.
4. **Oggetti (`O`)**: entrando in una cella contenente un oggetto, il giocatore lo raccoglie: l'oggetto viene rimosso dalla mappa e il punteggio del giocatore aumenta di 1.
5. **Uscite (`E`)**: sono presenti **4 uscite** posizionate al centro dei 4 bordi del labirinto (nord, sud, ovest, est).
6. **Mappa Globale**: periodicamente (ogni T secondi), il server invia a ogni giocatore l'intera mappa mascherata, in cui le celle non ancora esplorate da quel giocatore sono contrassegnate dal simbolo `?`.
7. **Fine partita e vincitore**: chi raggiunge un'uscita può restare connesso in attesa del risultato oppure disconnettersi con `q`. La partita termina allo scadere del timeout oppure quando, dopo un'uscita dal labirinto o una disconnessione, nessun giocatore connesso è ancora nel labirinto.
   - Se un solo giocatore ha raggiunto un'uscita, vince lui.
   - Se più giocatori hanno raggiunto un'uscita, vince chi ha raccolto più oggetti **tra quelli usciti**.
   - Se nessuno ha raggiunto un'uscita, vince chi ha raccolto più oggetti **tra tutti i partecipanti**, inclusi quelli disconnessi.
   - A parità di punteggio vince chi è entrato prima. Se non ci sono stati partecipanti, non c'è un vincitore.
   - Il client mostra l'esito e attende Invio prima di chiudere la connessione.

Simboli della mappa:
- `#` muro, `.` cella libera, `E` uscita, `O` oggetto, `P` posizione del giocatore, `?` cella nascosta.

## Protocollo applicativo

La comunicazione si basa sulla struttura binaria fissa `Messaggio` scambiata tramite socket TCP. I dati vengono inviati nella rappresentazione nativa delle strutture C, perciò client e server devono usare layout e ordine dei byte compatibili.

```c
typedef struct {
    int  tipo;                                          /* Tipo di messaggio (MSG_*) */
    char nickname[MAX_NICK];                            /* Nickname per login o vincitore */
    char password[MAX_PASS];                            /* Password per autenticazione */
    char direzione;                                     /* Direzione: 'w', 'a', 's', 'd' */
    char testo[MAX_TESTO];                              /* Notifiche, avvisi o messaggi di errore */
    int  riga;                                          /* Riga corrente del giocatore */
    int  colonna;                                       /* Colonna corrente del giocatore */
    int  punteggio;                                     /* Punteggio corrente */
    
    char mappa_locale[VISTA_LOCALE][VISTA_LOCALE];      /* Vista locale 5x5 centrata sul giocatore */
    char mappa_globale[RIGHE_MAPPA][COLONNE_MAPPA];     /* Mappa globale 21x41 mascherata */
    
    int  num_giocatori;                                 /* Numero di InfoGiocatore che seguono il messaggio (MSG_LISTA) */
} Messaggio;
```

L'invio e la ricezione avvengono tramite funzioni dedicate (`invia_messaggio`, `ricevi_messaggio`, `invia_dati`, `ricevi_dati`) che gestiscono short read/write ed eventuali interruzioni da segnali (`EINTR`) con cicli iterativi `while`.

Per `MSG_LISTA`, il server invia la struttura `Messaggio` seguita, nello stream TCP, da una struttura `InfoGiocatore` per ogni giocatore connesso. Il client legge `num_giocatori` e usa `malloc` per allocare la memoria necessaria a ricevere la lista.

### Tipi di messaggio

| Costante | Valore | Descrizione |
|----------|:------:|-------------|
| `MSG_ACCESSO` | `1` | Client invia nickname e password per accedere |
| `MSG_MOVIMENTO` | `2` | Client invia la direzione di movimento (`w`/`a`/`s`/`d`) |
| `MSG_RICHIEDI_LISTA` | `3` | Client richiede la lista dei giocatori connessi |
| `MSG_ESCI` | `4` | Client notifica la disconnessione volontaria |
| `MSG_REGISTRAZIONE` | `5` | Client invia nickname e password per registrare un nuovo utente |
| `MSG_OK` | `10` | Server conferma l'accesso o la registrazione con successo |
| `MSG_ERRORE` | `11` | Server segnala errore (password errata, nickname duplicato, partita finita) |
| `MSG_MAPPA_LOCALE` | `12` | Server invia la vista 5×5 centrata sul giocatore |
| `MSG_MAPPA_GLOBALE` | `13` | Server invia la mappa globale mascherata e la vista locale all'ingresso e periodicamente |
| `MSG_LISTA` | `14` | Server invia l'elenco dei giocatori e dei punteggi |
| `MSG_FINE_PARTITA` | `15` | Server notifica la fine della partita e il vincitore |
| `MSG_NOTIFICA` | `16` | Tipo previsto dal protocollo e gestito dal client. Gli avvisi di gioco del server arrivano nel campo `testo` di `MSG_MAPPA_LOCALE` |

### Sequenza tipica di comunicazione

```
CLIENT                                  SERVER
  |------- TCP connect() ---------------->|
  |-- MSG_ACCESSO (nick, pass) ---------->|
  |<-- MSG_OK ----------------------------|
  |<-- MSG_MAPPA_GLOBALE (iniziale) ------|
  |-- MSG_MOVIMENTO ('w') --------------->|
  |<-- MSG_MAPPA_LOCALE ------------------|
  |           ...                         |
  |<-- MSG_MAPPA_GLOBALE (ogni T sec) ----|
  |-- MSG_ESCI (o Ctrl+C) --------------->|
  |    Entrambi chiudono la connessione   |
```

## Note implementative e scelte tecniche

- **Concorrenza Multithread**: il server gestisce le connessioni con un ciclo `while (1)` su `accept()` e alloca un thread dedicato per ciascun client connesso (`pthread_create` + `pthread_detach`) affidato alla funzione `client_handler`. Un thread separato `timer_thread` gestisce il conteggio dei secondi per il timeout e i broadcast periodici della mappa globale.
- **Sincronizzazione**: il mutex `partita.mutex` protegge l'accesso concorrente allo stato di gioco (`StatoGioco partita`), alle coordinate dei giocatori, alla matrice del labirinto e agli invii sui descrittori dei client. Per evitare che un client lento o con buffer TCP pieno possa trattenere il mutex, `invia_dati()` utilizza `send(..., MSG_DONTWAIT)` non bloccante.
- **Logging Thread-Safe (`log.c`)**:
  - Utilizza le system call UNIX a basso livello `open("server.log", O_WRONLY | O_CREAT | O_APPEND, 0644)` e `write()`.
  - Timestamp formattato con `strftime` nel formato `[AAAA-MM-GG HH:MM:SS]`.
  - Protetto da un mutex dedicato `log_mutex` per evitare l'interleaving dei record tra thread concorrenti.
- **Risoluzione Indirizzi e Nomi Host (`client.c`)**:
  - Usa `getaddrinfo()` con `AF_INET` per risolvere nomi host e indirizzi IPv4.
- **Gestione Segnali nel Client (`client.c`)**:
  - Nel ciclo di gioco, i gestori di `SIGINT` (`Ctrl+C`) e `SIGTERM` azzerano il flag del ciclo. All'uscita dal ciclo, il client invia `MSG_ESCI` e chiude il socket.
  - `signal(SIGPIPE, SIG_IGN)`: ignora `SIGPIPE` sia nel server che nel client per gestire le disconnessioni improvvise tramite valore di ritorno delle primitive di rete anziché tramite terminazione forzata del processo.
- **I/O Multiplexing con `select()`**:
  - Nel ciclo di gioco, il client usa `select()` con `master_set` e `read_set` per attendere input da tastiera o dati dal socket. La lettura di un messaggio può attendere i byte mancanti.
- **I/O di rete (`protocol.c`)**:
  - `invia_dati()` e `ricevi_dati()` proseguono dopo un trasferimento parziale e riprovano in caso di `EINTR`. Se il buffer di invio è pieno (`EAGAIN` o `EWOULDBLOCK`), `invia_dati()` restituisce errore anche quando una parte del messaggio è già stata inviata.
