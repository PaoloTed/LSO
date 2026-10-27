# Progetto LSO — Traccia A: Labirinto con oggetti e uscita

Sistema client–server in C (UNIX/Linux) per un gioco di esplorazione di un labirinto 2D.
Il server genera il labirinto, gestisce più giocatori contemporaneamente in thread dedicati (modello *thread-per-client*), invia a ciascuno una vista locale e, periodicamente, una mappa globale mascherata.
La comunicazione avviene tramite socket TCP con un protocollo applicativo binario.

## Struttura

```
labirinto/
├── Makefile
├── users.txt          archivio persistente credenziali utenti (creato al primo avvio)
├── server.log         file di logging delle attività del server
├── common/            protocollo condiviso (header, send/recv affidabili)
│   ├── protocol.h     definizioni costanti, tipi di messaggi e strutture dati
│   └── protocol.c     invio e ricezione dati con gestione short read/write
├── server/            logica del server
│   ├── server.c       main, socket di ascolto, accept, timer thread, thread per client
│   ├── game.c/h       labirinto, spawn casuale, autenticazione, movimento, punteggi, vincitore
│   └── log.c/h        logging su file a basso livello (open 0644, O_APPEND, write, log_mutex)
└── client/            client interattivo
    ├── client.c       risoluzione getaddrinfo, autenticazione, multiplexing select(), segnali (SIGINT -> MSG_ESCI)
    └── display.c/h    rendering delle mappe e della lista giocatori con colori ANSI
```

## Compilazione

Apri un terminale nella cartella del progetto (quella contenente il `Makefile`) e compila:

```bash
make
```

Su Windows con WSL (dalla stessa cartella aperta in PowerShell):

```powershell
wsl make
```

Il comando produce i due eseguibili `bin/server` e `bin/client`.  
Per eliminare i binari e i file oggetto: `make clean`.

## Avvio rapido

1. Avvia il **server** in un terminale (resta in esecuzione in background/servizio; non stampa a video ma registra ogni evento su `server.log`):

```bash
./bin/server 5200 600 15
```

Su Windows con WSL:
```powershell
wsl ./bin/server 5200 600 15
```

- `5200` è la porta TCP di ascolto.
- `600` è il timeout globale della partita in secondi.
- `15` è l'intervallo di invio periodico della mappa globale in secondi.

2. Avvia uno o più **client**, ciascuno in un terminale separato:

```bash
./bin/client localhost 5200
```

Su Windows con WSL:
```powershell
wsl ./bin/client 127.0.0.1 5200
```

All'avvio, il client richiede l'inserimento di **nickname** e **password**.

## Uso del server

```bash
./bin/server <porta> [timeout] [intervallo]
```

- `porta`: porta TCP di ascolto (obbligatoria).
- `timeout`: durata massima della partita in secondi (opzionale, default 180).
- `intervallo`: periodo di invio automatico della mappa globale in secondi (opzionale, default 20).

Esempio:
```bash
./bin/server 5200 300 15
```

- Il server rispetta rigorosamente i requisiti della traccia: **non produce output su stdout e non legge da stdin**.
- Tutti gli eventi (connessioni, disconnessioni, raccolte oggetti, uscite, vincitore) vengono registrati nel file `server.log` con timestamp.
- Le credenziali degli utenti (nickname e password) vengono memorizzate in modo permanente nel file `users.txt`: se un utente accede per la prima volta viene registrato automaticamente; agli accessi successivi viene verificata la password.

## Uso del client

```bash
./bin/client <nome_host|indirizzo_ip> <porta>
```

- `nome_host|indirizzo_ip`: nome simbolico (es. `localhost`) oppure indirizzo numerico (es. `127.0.0.1`), risolto tramite `getaddrinfo()`.
- `porta`: porta TCP del server (es. `5200`).

Esempio:
```bash
./bin/client localhost 5200
```

Dopo la connessione TCP, il client richiede:
1. `nickname`: identificativo dell'utente (univoco tra i giocatori attualmente in partita).
2. `password`: credenziale di accesso/registrazione.

Comandi disponibili durante la partita:

| Comando | Azione |
|:-------:|--------|
| `w` `a` `s` `d` | Muovi su / sinistra / giù / destra (seguito da Invio) |
| `l` | Visualizza la lista aggiornata dei giocatori connessi e relativi punteggi |
| `h` | Mostra il menu di aiuto con i comandi disponibili |
| `q` | Disconnessione e uscita ordinata (disponibile anche tramite `Ctrl+C` o `Ctrl+D`) |

## Regole del gioco

1. **Accesso e Registrazione**: ogni giocatore inserisce nickname e password. Il nickname deve essere univoco tra i giocatori correntemente attivi in partita.
2. **Spawn**: ciascun giocatore entra nella mappa venendo posizionato in una cella libera casuale all'interno del labirinto. All'inizio vede soltanto la propria cella.
3. **Movimento**: digitando `w`/`a`/`s`/`d` e premendo Invio, il giocatore si sposta. Ad ogni passo valido il server "rivela" una zona attorno a lui (raggio di 2 celle, vista 5×5) che resta permanentemente scoperta.
4. **Oggetti (`O`)**: entrando in una cella contenente un oggetto, il giocatore lo raccoglie: l'oggetto viene rimosso dalla mappa e il punteggio del giocatore aumenta di 1.
5. **Uscite (`E`)**: sono presenti **4 uscite** posizionate al centro dei 4 bordi del labirinto (nord, sud, ovest, est).
6. **Mappa Globale**: periodicamente (ogni T secondi), il server invia a ogni giocatore l'intera mappa mascherata, in cui le celle non ancora esplorate da quel giocatore sono contrassegnate dal simbolo `?`.
7. **Fine Partita e Vincitore**: la partita termina allo scadere del timeout globale oppure quando tutti i giocatori attivi sono usciti dal labirinto. Il vincitore è determinato secondo le regole della traccia:
   - Se esce un solo giocatore &rarr; vince lui;
   - Se escono più giocatori prima del timeout &rarr; vince chi ha raccolto più oggetti **tra quelli usciti**;
   - Se nessuno esce allo scadere del timeout &rarr; vince chi ha raccolto più oggetti **tra tutti i partecipanti**.

Simboli della mappa:
- `#` muro, `.` cella libera, `E` uscita, `O` oggetto, `P` posizione del giocatore, `?` cella nascosta.

## Protocollo applicativo

La comunicazione si basa sulla struttura binaria fissa `Messaggio` scambiata tramite socket TCP:

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

Nel caso del messaggio `MSG_LISTA`, il server invia la struct `Messaggio` seguita immediatamente in memoria da un array dinamico di strutture `InfoGiocatore` (una per ogni giocatore attivo). Il client legge `num_giocatori`, alloca la memoria con `malloc` e riceve l'intera lista senza limiti statici di dimensione.

### Tipi di messaggio

| Costante | Valore | Descrizione |
|----------|:------:|-------------|
| `MSG_ACCESSO` | `1` | Client invia nickname e password per accedere |
| `MSG_MOVIMENTO` | `2` | Client invia la direzione di movimento (`w`/`a`/`s`/`d`) |
| `MSG_RICHIEDI_LISTA` | `3` | Client richiede la lista dei giocatori connessi |
| `MSG_ESCI` | `4` | Client notifica la disconnessione volontaria |
| `MSG_OK` | `10` | Server conferma l'accesso con successo |
| `MSG_ERRORE` | `11` | Server segnala errore (password errata, nickname duplicato, partita finita) |
| `MSG_MAPPA_LOCALE` | `12` | Server invia la vista 5×5 centrata sul giocatore |
| `MSG_MAPPA_GLOBALE` | `13` | Server invia periodicamente la mappa globale mascherata |
| `MSG_LISTA` | `14` | Server invia l'elenco dei giocatori e dei punteggi |
| `MSG_FINE_PARTITA` | `15` | Server notifica la fine della partita e il vincitore |
| `MSG_NOTIFICA` | `16` | Server invia un messaggio informativo / notifica |

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
  |<-- chiusura socket -------------------|
```

## Note implementative e scelte tecniche

- **Concorrenza Multithread**: il server gestisce le connessioni con un ciclo `while (1)` su `accept()` e alloca un thread dedicato per ciascun client connesso (`pthread_create` + `pthread_detach`) affidato alla funzione `client_handler`. Un thread separato `timer_thread` gestisce il conteggio dei secondi per il timeout e i broadcast periodici della mappa globale.
- **Sincronizzazione**: il mutex `partita.mutex` protegge l'accesso concorrente allo stato di gioco (`StatoGioco partita`), alle coordinate dei giocatori e alla matrice del labirinto. Le operazioni di I/O di rete potenzialmente bloccanti vengono eseguite a lock rilasciato per garantire che un client lento non blocchi l'intero server.
- **Logging Thread-Safe (`log.c`)**:
  - Utilizza le system call UNIX a basso livello `open("server.log", O_WRONLY | O_CREAT | O_APPEND, 0644)` e `write()`.
  - Timestamp formattato con `strftime` nel formato `[AAAA-MM-GG HH:MM:SS]`.
  - Protetto da un mutex dedicato `log_mutex` per evitare l'interleaving dei record tra thread concorrenti.
- **Risoluzione Indirizzi e Nomi Host (`client.c`)**:
  - Utilizza la funzione standard POSIX `getaddrinfo()`, supportando indifferentemente sia indirizzi IP numerici (es. `127.0.0.1`) sia nomi simbolici (es. `localhost`).
- **Gestione Segnali nel Client (`client.c`)**:
  - `signal(SIGINT, sigint_handler)` e `signal(SIGTERM, sigint_handler)`: alla pressione di `Ctrl+C` il flag di ciclo viene azzerato in modo asincrono-sicuro, consentendo al ciclo principale di inviare `MSG_ESCI` al server e chiudere ordinatamente il socket.
  - `signal(SIGPIPE, SIG_IGN)`: ignora `SIGPIPE` sia nel server che nel client per gestire le disconnessioni improvvise tramite valore di ritorno delle primitive di rete anziché tramite terminazione forzata del processo.
- **I/O Multiplexing con `select()`**:
  - Il client utilizza `select()` con `master_set` e `read_set` monitorando contemporaneamente `STDIN_FILENO` (input tastiera) e `sock_fd` (messaggi e mappe periodiche dal server), impedendo il blocco dell'interfaccia durante l'attesa di input utente.
- **I/O di Rete Robusto (`protocol.c`)**:
  - Primitive `invia_dati()` e `ricevi_dati()` basate su cicli `while (rimanenti > 0)` con gestione delle *short read/write* e ripresa automatica in caso di interruzione da segnale (`errno == EINTR`).
