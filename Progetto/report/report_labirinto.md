# Report di analisi — Progetto LSO, Traccia A “Labirinto con oggetti e uscita”

- **Oggetto analizzato:** `Progetto/labirinto` (client/server C su socket TCP)
- **Data:** 21 ottobre 2026
- **Metodo:** revisione statica di tutto il codice (1.105 righe `.c` + 153 righe di header), confronto con la traccia (`Progetto Lso.txt`), con il `README.md`, con il `Makefile`, con il log di esercizio e con la cronologia Git.
- **Limite dell’analisi:** in questo ambiente non è stato possibile compilare o eseguire il progetto: `gcc` non è installato, WSL risponde `WSL_E_ACCESS_DENIED` e il daemon Docker non è attivo. I binari presenti in `bin/` sono ELF precompilati e sono più vecchi di alcune sorgenti (`display.c`, `game.c`). Le osservazioni sul comportamento sono quindi statiche, basate sul codice e sul log.

---

## 1. Sintesi esecutiva

Il progetto è una **buona base universitaria**: struttura modulare chiara, codice leggibile e commentato, protocollo condiviso, logging thread-safe, `select()` nel client, gestione di `SIGPIPE`, `SIGINT`, short read/write ed `EINTR`. Le funzionalità centrali della traccia (labirinto casuale, movimento, oggetti, uscite, nebbia di guerra, mappa globale periodica, timeout, lista giocatori, log) sono tutte presenti e il log conservato dimostra una partita completa.

Le criticità principali non sono nella logica del gioco, ma in **tre aree**:

1. **Sicurezza dell’input del client**: `scanf("%s", nickname)` su un buffer di 32 byte è un buffer overflow banale.
2. **Concorrenza sull’I/O di rete**: più thread possono scrivere sulla stessa socket senza mutex; il timer può bloccarsi su un client lento; una fd può essere chiusa e riusata mentre un altro thread la sta usando.
3. **Aderenza alla traccia**: mancano registrazione/password, il limite di 32 utenti contraddice “nessun limite a priori”, il client non risolve hostname, la vista iniziale è 5×5 invece della sola cella, e la “relazione” da consegnare non è stata redatta (il README non è una relazione da ~10 pagine con copertina).

**Giudizio sintetico:** progetto funzionante e ben organizzato, ma **non ancora pronto per una consegna senza interventi** sulle criticità P0 (input, invio concorrente, ciclo di vita delle socket, timer bloccante). Con le correzioni indicate nei §10–11 diventa solido.

---

## 2. Numeri e stato del repository

| Elemento | Valore |
|---|---|
| Sorgenti C | 6 file, **1.105 righe** |
| Header | 4 file, **153 righe** |
| Totale codice | **1.258 righe** |
| `server/server.c` | 224 righe |
| `server/game.c` | 493 righe |
| `server/log.c` | 34 righe |
| `client/client.c` | 231 righe |
| `client/display.c` | 74 righe |
| `common/protocol.c` | 49 righe |
| Labirinto | 21 × 41 = 861 celle |
| Oggetti | 15, fissi (`game.c:87`) |
| Uscite | 4, in posizioni fisse (`game.c:81-84`) |
| Giocatori massimi | 32 (`MAX_PLAYERS`, `game.h:21`) |
| Dimensione `Messaggio` | **2.352 byte** (calcolo in §4.1) |
| Binari | `bin/server` 46.352 B, `bin/client` 32.416 B, ELF |
| Evidenza di test | `server.log`: una sola partita, un solo giocatore (`paolo`) |
| Git | ultimo commit `8ff8f8b`; `server/game.c` modificato solo in un commento |

Nota: i binari sono **stale** rispetto a `client/display.c` (12/10 16:16) e `server/game.c` (12/10 16:48), quindi vanno ricompilati prima di qualsiasi prova. `make` li ricostruirebbe per via delle date, ma non per modifiche ai soli header (vedi §9.5).

---

## 3. Architettura

### 3.1 Moduli

```text
labirinto/
├── Makefile                 build unica per client e server
├── common/
│   ├── protocol.h           costanti, struct Messaggio, tipi MSG_*
│   └── protocol.c           invia_messaggio / ricevi_messaggio (loop su short I/O)
├── server/
│   ├── server.c             main, accept, thread per client, timer thread
│   ├── game.c/h             labirinto, giocatori, movimento, mappe, fine partita
│   └── log.c/h              log su file con timestamp e mutex
└── client/
    ├── client.c             connessione, login, select(), segnali
    └── display.c/h          rendering ANSI di mappe e lista
```

La separazione è sensata: `common/` evita duplicazione del protocollo, `game.c` isola la logica di gioco, `log.c` isola l’I/O su file, `display.c` isola la presentazione. È il punto di forza strutturale del progetto.

### 3.2 Modello di esecuzione del server

```text
main thread
  └─ accept() in while(1)
       └─ pthread_create(client_thread) + pthread_detach
            ├─ ricevi MSG_LOGIN
            ├─ game_add_player()            [mutex]
            ├─ invia MSG_OK + mappa globale
            └─ while (ricevi) { MOVE/LIST/QUIT } → game_remove_player()

timer_thread (uno solo)
  └─ ogni 1 s: timeout? → game_finish + broadcast_game_over
               intervallo? → broadcast_global_maps

g_game.mutex  → protegge maze, players, punteggi, discovered, game_over, winner
log_mutex     → serializza le scritture su server.log
```

Il modello è quello classico “thread-per-connection + thread timer”, coerente con la traccia e con la guida del corso. Il costo è un thread per connessione, senza limite e senza timeout di lettura: un client che si connette e non invia mai il login occupa un thread per sempre (§8.7).

### 3.3 Stato condiviso e sincronizzazione

| Dato | Protezione | Valutazione |
|---|---|---|
| `maze.cells`, `maze.objects` | `g_game.mutex` | corretto |
| `players[]` (posizione, score, exited, discovered) | `g_game.mutex` | corretto |
| `game_over`, `winner` | `g_game.mutex` in scrittura | **lettura non protetta nel timer** (`server.c:119`) |
| `timeout`, `interval` | immutabili dopo `game_init` | ok di fatto |
| fd delle socket | nessuna | **scritture concorrenti e close/reuse race** |
| `server.log` | `log_mutex` | corretto |
| `listen_fd` | solo main | ok, ma campo inutilizzato |

Il README afferma che l’unico mutex garantisce “assenza di race condition e prevenzione di deadlock” (`README.md:183`). La prima parte è **falsa** per le socket e per `game_over`; la seconda è vera solo perché non esiste un ordine di lock inverso, non per virtù del mutex unico.

### 3.4 Flusso di una partita

1. Il server genera il labirinto e avvia il timer.
2. Il client si connette, invia `MSG_LOGIN` con il nickname.
3. Il server verifica unicità tra i giocatori attivi, assegna uno degli 8 punti di partenza, rivela il 5×5 iniziale, risponde `MSG_OK` e invia subito la mappa globale mascherata.
4. Ogni comando `w/a/s/d` produce un `MSG_MAPPA_LOCALE`; su raccolta oggetto o uscita viene aggiornato lo stato e, se tutti i giocatori attivi sono usciti, parte il `MSG_FINE_PARTITA`.
5. Ogni `T` secondi il timer invia a tutti la mappa globale mascherata; allo scadere del timeout termina la partita.
6. `l` restituisce la lista, `q`/Ctrl+C/Ctrl+D inviano `MSG_QUIT`.

---

## 4. Protocollo applicativo

### 4.1 Formato

È una **struct C fissa** inviata così com’è con `write()` e letta con `read()` (`protocol.h:45-59`):

| Campo | Byte | Note |
|---|---:|---|
| `int type` | 4 | tipo `MSG_*` |
| `char nickname[32]` | 32 | login / vincitore |
| `char direction` | 1 | +3 di padding |
| `char text[128]` | 128 | messaggi |
| `int row, col, score` | 12 | stato |
| `char local_map[5][5]` | 25 | vista locale |
| `char global_map[21][41]` | 861 | mappa mascherata |
| `int num_players` | 4 | +2 di padding prima |
| `InfoGiocatore players[32]` | 1.280 | 40 B × 32 |
| **Totale** | **2.352** | allineamento a 4 byte |

**Conseguenze:** ogni movimento del client invia 2.352 byte e ogni risposta ne invia altrettanti, anche quando basterebbero pochi byte. È accettabile per 32 giocatori, ma è una scelta inefficiente e rigida.

### 4.2 Tipi di messaggio

| Codice | Costante | Direzione | Uso effettivo |
|---:|---|---|---|
| 1 | `MSG_LOGIN` | C→S | sì |
| 2 | `MSG_MOVE` | C→S | sì |
| 3 | `MSG_LIST` | C→S | sì |
| 4 | `MSG_QUIT` | C→S | sì |
| 10 | `MSG_OK` | S→C | sì |
| 11 | `MSG_ERROR` | S→C | solo login |
| 12 | `MSG_MAPPA_LOCALE` | S→C | sì |
| 13 | `MSG_MAPPA_GLOBALE` | S→C | sì |
| 14 | `MSG_LISTA` | S→C | sì |
| 15 | `MSG_FINE_PARTITA` | S→C | sì |
| 16 | `MSG_INFO` | S→C | **mai inviato** (`game_send_info` non usata) |

### 4.3 Punti di forza e limiti

**Pro:**

- `invia_messaggio`/`ricevi_messaggio` gestiscono correttamente short write/read ed `EINTR` (`protocol.c:9-49`).
- Tutte le struct sono azzerate prima dell’invio, quindi i padding sono deterministici.
- Il protocollo è semplice da spiegare all’orale e la sequenza è documentata nel README.

**Contro:**

- Nessuna serializzazione esplicita: dipende da padding, allineamento e **endianness** della macchina. Funziona tra due Linux x86_64, ma non è portabile.
- Nessun campo di versione, lunghezza o magic number.
- Il client si fida di `num_players` ricevuto dal server: un server malevolo potrebbe inviare un valore > 32 e far leggere `players[]` fuori dai limiti (`display.c:69-73`).
- Il costo fisso di 2.352 byte per messaggio è sproporzionato per un comando di un carattere.

---

## 5. Conformità alla traccia

Riferimento: [`Progetto Lso.txt`](<Progetto Lso.txt>).

| Requisito | Stato | Note |
|---|---|---|
| C su UNIX/Linux, socket TCP | ✅ | `socket/bind/listen/accept/connect` |
| Server concorrente | ✅ | thread per client + timer |
| Labirinto 2D generato randomicamente | ✅ | random walk da 8 punti |
| Muri, celle libere, uscite, oggetti | ✅ | `# . E O` |
| Vista locale a ogni mossa | ✅ | 5×5 |
| Rivelazione permanente delle celle | ✅ | `discovered[][]` per giocatore |
| Mappa globale mascherata ogni T secondi | ✅ | `?` per le celle non viste |
| Timeout globale | ✅ | thread timer |
| Regole del vincitore | ⚠️ | implementate, ma con casi limite sui disconnessi (§7) |
| Logging di connessioni e conquiste | ✅ | data/ora, nickname, raccolte, uscite, fine partita |
| Server senza stdout/stdin | ✅ | solo `stderr` in caso di errore fatale all’avvio |
| Solo libc + system call | ⚠️ | `system("clear")` nel client è libc standard ma dipende da `/usr/bin/clear` e da una shell |
| Registrazione con password e nickname | ❌ | **non implementata**: solo nickname, nessuna password, nessun archivio utenti |
| Nessun limite a priori di utenti | ❌ | `MAX_PLAYERS 32` e array fisso nella struct |
| Client con nome **o** IP del server | ❌ | solo IPv4 numerico con `inet_pton` (`client.c:57`), niente hostname |
| Posizionamento in una **cella libera casuale** | ⚠️ | solo 8 punti fissi; più giocatori possono sovrapporsi |
| Il giocatore vede inizialmente **solo la propria cella** | ❌ | all’ingresso viene rivelato un 5×5 (`game.c:220`) |
| Relazione ~10 pagine con copertina e componenti | ❌ | il README è una buona guida, non una relazione; mancano copertina, gruppo, sezioni formali |
| Consegna in archivio zip/rar | — | non verificabile dal repository |

**Sintesi:** il **nucleo ludico** della traccia è rispettato; mancano requisiti “di contorno” che però vengono spesso valutati (registrazione, limite utenti, hostname, relazione). Da chiarire con il docente se la registrazione “al sito” sia esterna al progetto; in caso affermativo va comunque documentato.

---

## 6. Punti di forza

- **Modularità reale**, non solo dichiarata: `common/` condiviso, `game.c` separato da `server.c`, `log.c` e `display.c` isolati.
- **Uso corretto del mutex** per lo stato di gioco: movimento, raccolta oggetti, assegnazione slot e unicità del nickname sono serializzati. Due giocatori non possono raccogliere lo stesso oggetto due volte.
- **Protocollo robusto sull’I/O**: loop su short read/write, gestione `EINTR`, `SIGPIPE` ignorato su entrambi i lati.
- **Logging** con `open(..., 0644)`, `O_APPEND`, timestamp `strftime`, `snprintf` e mutex dedicato: è esattamente l’impianto richiesto dal corso.
- **Client non bloccante sull’input**: `select()` su stdin e socket permette di ricevere la mappa globale mentre l’utente sta digitando.
- **Segnali**: Ctrl+C/Ctrl+D provocano `MSG_QUIT` e disconnessione pulita (dopo il login).
- **Makefile unico** e README con compilazione, avvio, comandi, regole e protocollo.
- **Codice leggibile**, nomi coerenti, commenti in italiano, nessun `TODO`/`FIXME` lasciato.

---

## 7. Criticità — quadro riassuntivo

| ID | Gravità | Posizione | Problema | Impatto |
|---|---|---|---|---|
| C1 | 🔴 Critica | `client.c:76` | `scanf("%s", nickname)` senza limite su buffer di 32 B | stack overflow con nickname lungo |
| C2 | 🔴 Critica | `game.c:360-363`, `414-417`, `491-492` | più thread scrivono sulla stessa socket senza mutex | messaggi interlacciati/corrotti con write parziali |
| C3 | 🔴 Critica | `game.c:230-248` + send | fd catturata, unlock, poi `close()` da un altro thread | scrittura su fd chiusa/riusata, dati a un client sbagliato |
| C4 | 🔴 Critica | `server.c:134`, `game.c:457-464` | il timer esegue invii bloccanti | un client lento blocca timer, timeout e broadcast di tutti |
| C5 | 🟠 Alta | `server.c:119` vs `game.c:299` | lettura di `game_over` senza mutex | data race / timer che non vede la fine partita |
| C6 | 🟠 Alta | `server.c:128-130` | `game_finish()` ignorata e broadcast comunque eseguito | doppio `MSG_FINE_PARTITA`, log “timeout” errato |
| C7 | 🟠 Alta | `server.c:56`, `206-219` | nessun timeout di login, thread illimitati | DoS con connessioni che non inviano nulla |
| C8 | 🟡 Media | `game.c:238`, `game.c:294` | `strncpy(..., 31)` su buffer locali non inizializzati | stringa non terminata con nickname di 31 caratteri |
| C9 | 🟡 Media | `log.c:29-31` | `write` con `n` di `snprintf` > buffer | over-read latente in caso di troncamento |
| C10 | 🟡 Media | `client.c:15-20`, `122-123` | `int loop` non `volatile sig_atomic_t`; `signal()` | comportamento non garantito con ottimizzazioni |
| C11 | 🟡 Media | `client.c:57` | solo IP numerico | hostname non supportato, contro la traccia |
| C12 | 🟡 Media | `game.c:123-148` | il vincitore include giocatori disconnessi | un client che ha lasciato la partita può vincere |
| C13 | 🟡 Media | `game.c:443-455`, `game.h:31-35` | codice/campi morti (`game_send_info`, `MSG_INFO`, `start_time`, `listen_fd`) | confusione e documentazione incoerente |
| C14 | 🔵 Bassa | `display.c:72` | etichetta “disconnesso” per chi è **uscito** | informazione sbagliata all’utente |
| C15 | 🔵 Bassa | `display.c:51-67` | mappa globale larga 84 colonne | wrap su terminale 80 colonne |
| C16 | 🔵 Bassa | `Makefile:5-14` | nessuna dipendenza dagli header | modifiche agli header non ricompilano |
| C17 | 🔵 Bassa | `README.md:115`, `:183` | affermazioni non vere (uscite casuali, assenza di race) | documentazione fuorviante in sede d’esame |
| C18 | 🔵 Bassa | varie | `system("clear")`, colori senza `isatty`, ridondanza mappa locale dopo la globale | portabilità e UX migliorabili |

---

## 8. Analisi dettagliata delle criticità principali

### 8.1 C1 — Buffer overflow nel nickname del client

```c
/* client.c:73-76 */
char nickname[MAX_NICK];          /* 32 byte */
printf("Inserisci il tuo nickname: ");
scanf("%s", nickname);            /* nessun limite di larghezza */
```

Con un nickname di 40 caratteri, `scanf` scrive 40 byte + `'\0'` in un buffer di 32, sovrascrivendo lo stack (variabili locali e, potenzialmente, indirizzo di ritorno). È il bug più grave dal punto di vista della sicurezza, anche se l’input arriva da tastiera locale: un esame con valigia di test o un `printf` lungo lo fanno emergere subito.

Inoltre il valore di ritorno di `scanf` **non è controllato**: con `Ctrl+D` al prompt, `nickname` resta non inizializzato e viene inviato al server (`client.c:84`).

**Correzione:**

```c
if (scanf("%31s", nickname) != 1) {
    fprintf(stderr, "Nickname mancante.\n");
    close(sock_fd);
    return 1;
}
if (nickname[0] == '\0') { /* ... */ }
```

Meglio ancora: leggere con `fgets(nickname, sizeof nickname, stdin)`, togliere il newline e validare lunghezza e caratteri.

---

### 8.2 C2 — Scritture concorrenti sulla stessa socket

Il server ha almeno due thread che possono inviare allo stesso client:

- il `client_thread`, che risponde a un movimento con `game_send_local_map()`;
- il `timer_thread`, che invia `game_broadcast_global_maps()` ogni T secondi.

Entrambi finiscono in `invia_messaggio(fd, ...)`, ma **non esiste alcun mutex per socket**. `write()` su TCP non garantisce atomicità: se il buffer di invio è pieno, la write restituisce un valore parziale e il loop di `protocol.c:14-23` esegue altre write. Due thread possono quindi interlacciare i byte dei rispettivi messaggi da 2.352 byte, e il client interpreta una struct corrotta (tipo sbagliato, mappa spazzatura, possibile crash).

Il caso è realistico proprio quando il client è lento a leggere (terminale occupato, Ctrl+Z, rete lenta): il buffer si riempie, le write diventano parziali e il timer e il thread del client si sovrappongono.

**Correzione minima:** un mutex di invio per giocatore, usato da **tutti** i send verso quella socket:

```c
typedef struct {
    ...
    pthread_mutex_t send_mutex;
} Giocatore;

int send_to_player(Giocatore *p, const Messaggio *m) {
    int rc;
    pthread_mutex_lock(&p->send_mutex);
    rc = p->active ? invia_messaggio(p->fd, m) : -1;
    pthread_mutex_unlock(&p->send_mutex);
    return rc;
}
```

**Correzione robusta:** coda di output per client + thread dedicato all’invio (vedi C4), che elimina anche il blocco del timer.

---

### 8.3 C3 — Close della fd e riuso del descrittore

`game_send_global_map()` e `game_send_local_map()` estraggono `fd` sotto mutex, poi **rilasciano il mutex** e inviano:

```c
/* game.c:360-363 */
int fd = p->fd;
pthread_mutex_unlock(&g_game.mutex);
invia_messaggio(fd, &msg);
```

Nello stesso intervallo, il thread del client può eseguire `game_remove_player()` e `close(fd)` (`game.c:242-243`). Se nel frattempo il main thread accetta una nuova connessione, il kernel può riassegnare **lo stesso numero di fd** al nuovo client. Il broadcast del timer scriverebbe quindi un intero `Messaggio` nel flusso del client sbagliato, desincronizzandolo.

**Correzione:** il ciclo di vita della socket va protetto insieme all’invio. Opzioni:

- tenere `send_mutex` anche nel `close()` e marcare `active = 0` prima di chiudere, in modo che nessun send possa partire dopo la chiusura;
- usare `shutdown(fd, SHUT_RDWR)` sotto lock e lasciare che sia il thread proprietario a fare `close()`;
- in alternativa, un contatore di riferimenti per socket / un oggetto “connessione” con flag `closing`.

La soluzione “un mutex per socket, preso sia dal send sia dal close” è la più semplice da difendere all’orale, purché il close non venga eseguito mentre un send è bloccato (da qui l’esigenza di timeout o I/O non bloccante, C4).

---

### 8.4 C4 — Il timer può bloccarsi su un client lento

```c
/* server.c:133-136 */
if (seconds % g_game.interval == 0) {
    game_broadcast_global_maps();   /* invii bloccanti, in sequenza */
}
```

`game_broadcast_global_maps()` chiama `game_send_global_map()` per ogni giocatore, e ogni chiamata esegue `invia_messaggio()` su una socket **bloccante** senza timeout. Se un client smette di leggere (processo sospeso, terminale bloccato, rete assente), il buffer TCP si riempie e la `write` del timer resta bloccata **a tempo indeterminato**.

Conseguenze:

- il timer non incrementa più `seconds`;
- il timeout globale non scatta mai;
- nessun altro client riceve più la mappa globale;
- la partita può restare appesa per sempre.

È una negazione del servizio banale, ma può capitare anche involontariamente (client in pausa con Ctrl+Z).

**Correzioni:**

1. **Architettura consigliata:** ogni client ha una coda di messaggi e un thread “sender”; il timer accoda soltanto e non fa mai I/O. La coda va limitata: se piena, il client è troppo lento e viene disconnesso.
2. **Alternativa più rapida:** socket non bloccante + buffer di invio per client; `send` con `EAGAIN` accoda, e un thread/sender drains.
3. **Minimo indennizzo:** `SO_SNDTIMEO` (es. 2 s) e disconnessione del client al timeout; non risolve l’interlacciamento (C2) ma evita il blocco eterno. Da non usare da solo come soluzione “pulita”.

---

### 8.5 C5 — `game_over` letto senza mutex

```c
/* server.c:119-120 */
while (!g_game.game_over) {
    sleep(1);
```

`game_over` è scritto sotto `g_game.mutex` (`game.c:299`) ma letto qui senza lock. In C è un data race: comportamento indefinito, e il compilatore potrebbe in teoria cacheare il valore. Nella pratica su x86 funziona, ma non è difendibile all’orale e con `-O2`/TSan viene segnalato.

**Correzione:** leggere sotto mutex a ogni iterazione, oppure usare `pthread_cond_timedwait` con una variabile di condizione segnalata alla fine partita, oppure `atomic_int` con `memory_order_acquire`.

```c
pthread_mutex_lock(&g_game.mutex);
int over = g_game.game_over;
pthread_mutex_unlock(&g_game.mutex);
if (over) break;
```

---

### 8.6 C6 — Doppio broadcast di fine partita

```c
/* server.c:124-130 */
if (seconds >= g_game.timeout) {
    log_event(... "Partita terminata per timeout" ...);
    game_finish();                    /* può restituire 0 se già finita */
    game_broadcast_game_over();       /* eseguito comunque */
    break;
}
```

Se la partita è già terminata per uscita di tutti i giocatori (`game_move` imposta `game_over` e chiama `game_broadcast_game_over`), il timer può svegliarsi, vedere il timeout, chiamare `game_finish()` che ritorna 0, e **trasmettere comunque** un secondo `MSG_FINE_PARTITA`, oltre a scrivere nel log “terminata per timeout” in modo falso. La finestra è fino a 1 secondo.

**Correzione:**

```c
if (!game_finish())
    break;                    /* già finita: qualcun altro ha già broadcastato */
log_event("Partita terminata per timeout.");
game_broadcast_game_over();
break;
```

---

### 8.7 C7 — Login senza timeout e thread illimitati

Il main accetta la connessione e crea subito un thread (`server.c:206-219`). Il thread poi si blocca in `ricevi_messaggio()` in attesa del login (`server.c:56`), senza `SO_RCVTIMEO` né limite di connessioni. Un attaccante (o semplicemente un client malfunzionante) può aprire migliaia di connessioni e non inviare nulla: i thread restano bloccati, la memoria cresce, e il limite `MAX_PLAYERS = 32` non protegge perché viene applicato solo **dopo** il login.

**Correzioni:** `SO_RCVTIMEO` di pochi secondi prima del login; contatore atomico delle connessioni in accettazione con limite esplicito; chiusura immediata oltre il limite; eventualmente `poll()` con timeout invece di `read()` bloccante.

---

### 8.8 C8 — `strncpy` e nickname non terminati

```c
/* game.c:293-294 */
char nick[MAX_NICK];                          /* non inizializzata */
strncpy(nick, p->nickname, MAX_NICK - 1);
```

Se il nickname è lungo esattamente 31 caratteri, `strncpy` copia 31 byte e **non scrive il `'\0'`** (il byte 31 di `nick` resta indeterminato). Il successivo `snprintf("%s", nick)` legge oltre i limiti dell’array.

Lo stesso schema è in `game_remove_player()` (`game.c:236-238`). In `game_add_player()` il problema è mascherato dal fatto che `p->nickname[31]` resta 0 dal `memset` globale (`game.c:101`), ma è un’invariante fragile.

**Correzione uniforme:**

```c
snprintf(nick, sizeof nick, "%s", p->nickname);
/* oppure */
strncpy(nick, p->nickname, sizeof nick - 1);
nick[sizeof nick - 1] = '\0';
```

---

### 8.9 C9 — `write` con lunghezza di `snprintf` non clampata

```c
/* log.c:29-31 */
int n = snprintf(line, sizeof(line), "[%s] %s\n", ts, msg);
if (n > 0)
    write(log_fd, line, (size_t)n);
```

`snprintf` restituisce il numero di caratteri **che sarebbero stati scritti**. Se `n >= sizeof(line)`, `write` legge oltre il buffer `line`. Oggi i chiamanti passano messaggi brevi (≤ ~128 byte) e il buffer è 512, quindi non è raggiungibile; è comunque un difetto latente da correggere, tanto più che il README vanta l’uso “a prova di buffer overflow” di `snprintf`.

**Correzione:**

```c
size_t len = (n > 0) ? (size_t)n : 0;
if (len >= sizeof(line))
    len = sizeof(line) - 1;
write(log_fd, line, len);
```

---

### 8.10 C10 — Flag dei segnali nel client

```c
/* client.c:15-20 */
static int loop = 1;                 /* non volatile sig_atomic_t */
static void handle_sigint(int sig) { loop = 0; }
```

Il tipo corretto per una variabile modificata da un handler è `volatile sig_atomic_t`. Inoltre `signal()` ha semantica variabile tra sistemi; è preferibile `sigaction()`. Con `-O2` il compilatore può assumere che `loop` non cambi fuori dal flusso normale. Oggi il `select()` interrotto da `EINTR` fa comunque uscire dal ciclo, ma la correttezza non è garantita dal codice.

Inoltre gli handler sono installati **dopo** il login (`client.c:122-123`): un Ctrl+C durante l’inserimento del nickname termina il processo con l’azione di default, senza `MSG_QUIT` (il server se ne accorge dalla chiusura, ma il comportamento non è quello documentato nel README).

---

### 8.11 C11 — Hostname non supportato

```c
/* client.c:57 */
if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
    fprintf(stderr, "Indirizzo IP non valido: %s\n", host);
```

La traccia richiede “il nome o l’indirizzo IP” del server; `inet_pton` accetta **solo** IPv4 in forma numerica. `localhost` viene rifiutato. La correzione è `getaddrinfo()` (con supporto IPv4/IPv6), che è anche la scelta più corretta didatticamente.

---

### 8.12 C12 — Il vincitore può essere un giocatore disconnesso

`find_winner_locked()` considera `joined && exited` e, in alternativa, `joined` con lo score più alto (`game.c:123-148`). `game_remove_player()` mette `active = 0` ma **non azzera `joined`**, `exited` o `score`. Quindi:

- un giocatore uscito e poi disconnesso continua a contare come “uscito” e può far scattare la regola “un solo uscente” al timeout;
- un giocatore che ha abbandonato la partita può essere dichiarato vincitore per punteggio, pur non essendo più connesso.

Non è chiaramente specificato dalla traccia, ma è un caso limite da gestire esplicitamente: escludere i non attivi dal calcolo, oppure congelare il risultato alla disconnessione e documentare la scelta.

---

## 9. Altri difetti e aspetti migliorabili

### 9.1 Logica di gioco

- **Vista iniziale:** `game_add_player()` chiama `reveal_around()` (`game.c:220`), quindi il giocatore vede subito un 5×5. La traccia dice “vede inizialmente solo la propria cella”.
- **Spawn:** solo 8 punti fissi (`game.c:205-208`), non una cella libera casuale; più giocatori possono condividere la stessa cella.
- **Uscite:** 4 posizioni fisse (centro dei lati, `game.c:81-84`), non casuali. Il README (`README.md:115`, `:191`) afferma il contrario.
- **Giocatore uscito:** `game_move()` ritorna in silenzio (`game.c:254-257`); il client non riceve alcun messaggio e non sa perché i comandi non hanno effetto.
- **Oggetto sulla cella di spawn:** non viene raccolto finché il giocatore non rientra nella cella; dettaglio minore ma incoerente con “passando su una cella con O l’oggetto viene raccolto”.
- **Nickname:** confronto case-sensitive; `Paolo` e `paolo` sono distinti. Non è un bug, ma va documentato.
- **Caratteri di controllo nel nickname:** il server accetta qualsiasi byte non nullo; un nickname con escape ANSI può alterare il terminale degli altri client nella lista. Sanificare o limitare a `[A-Za-z0-9_-]`.

### 9.2 Visualizzazione

- `display_player_list()` stampa “disconnesso” per `exited == 1` (`display.c:72`): `exited` significa “ha raggiunto l’uscita”, non “disconnesso”. Etichetta sbagliata.
- La mappa globale occupa `2 + 41*2 = 84` colonne (`display.c:56-64`): su un terminale classico da 80 colonne va a capo. Usare una cella per carattere, o una larghezza dinamica.
- Dopo `MSG_MAPPA_GLOBALE` il client stampa la mappa globale **e** di nuovo la vista locale (`client.c:186-189`): ridondante, anche se non dannoso.
- `system("clear")` (`display.c:33`, `:52`) dipende da `/usr/bin/clear`, dalla shell e dalla variabile `TERM`; meglio una sequenza ANSI (`"\033[2J\033[H"`), eventualmente solo se `isatty(STDOUT_FILENO)`.
- I colori ANSI vengono emessi anche con output rediretto su file; controllare `isatty`.

### 9.3 Makefile e build

- `bin/server` e `bin/client` non dipendono dagli header (`Makefile:10-14`): modificare `protocol.h` o `game.h` non provoca ricompilazione. Aggiungere `-MMD -MP` e gli `-include *.d`, oppure elencare gli header come prerequisiti.
- Manca `-std=c11` (o `-std=gnu11`), `-O2` e un target `debug` con sanitizzatori. Consigliato:

```make
CFLAGS  = -std=c11 -Wall -Wextra -O2 -g -pthread -Icommon
debug: CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
debug: LDFLAGS += -fsanitize=address,undefined
```

- Manca un target per i test o per `valgrind`/`helgrind`.
- Il `Makefile` ha line endings **CRLF** mentre i sorgenti sono LF. GNU make recente tende a tollerarlo, ma è igiene convertirla a LF, soprattutto in un progetto WSL/Linux.
- `clean` cancella anche `server.log`: comodo, ma va documentato (il README non lo dice).

### 9.4 Codice morto e incoerenze

- `game_send_info()` (`game.c:443-455`) non è mai chiamata; `MSG_INFO` (`protocol.h:35`) è documentato nel README ma mai inviato.
- `g_game.start_time` (`game.h:34`) è impostato ma mai usato: il timeout si basa sul contatore del timer, non sul timestamp.
- `g_game.listen_fd` (`game.h:35`) è assegnato in `main` ma mai letto.
- `main()` termina con `close(listen_fd); return 0;` in un `while (1)` irraggiungibile (`server.c:222-223`): innocuo ma segnala che non esiste uno shutdown pulito.
- Il server non gestisce `SIGINT`/`SIGTERM`: alla chiusura non scrive un evento di log né chiude esplicitamente le socket. Per un progetto d’esame è accettabile, ma un handler che logga “server terminato” e fa `close(listen_fd)` sarebbe più curato.
- Il server non registra l’IP del client, né i tentativi di login falliti. La traccia chiede nome **o** IP; il nickname c’è sempre, ma l’IP sarebbe utile per il debug e per gli errori.

### 9.5 Documentazione

- Il README è ben fatto come **guida d’uso**, ma non sostituisce la relazione richiesta: mancano copertina, componenti del gruppo, struttura formale in ~10 pagine, byte layout del protocollo, scelte di concorrenza discusse criticamente, risultati dei test.
- Affermazioni da correggere:
  - “uscite posizionate casualmente” (`README.md:115`, `:191`) → sono fisse;
  - “assenza di race condition” (`README.md:183`) → falso per socket e `game_over`;
  - “prevenendo deadlock” → il mutex unico non è una garanzia di deadlock-freedom, e le send bloccanti sono un problema diverso ma più concreto;
  - “Ctrl+C invia MSG_QUIT” → solo dopo il login.
- Il README non riporta la dimensione della struct (2.352 byte) né il problema di endianness/padding, che sono ottimi argomenti da relazione.

---

## 10. Test consigliati prima della consegna

Non essendoci test automatici nel repository, il progetto va verificato con una matrice minima:

### 10.1 Build e analisi statica

```bash
make clean && make
gcc -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only -Icommon server/*.c
gcc -std=c11 -Wall -Wextra -Wpedantic -fsyntax-only -Icommon client/*.c
cppcheck --enable=all --inconclusive --std=c11 -Icommon .
```

### 10.2 Funzionali

- avvio con `./bin/server 5200 60 5` e 1 client: login, movimento, raccolta, uscita, fine partita;
- 2–4 client: lista, punteggi, uscita singola e multipla, timeout;
- nickname duplicato, vuoto, 31 caratteri, 200 caratteri (test C1);
- `q`, Ctrl+C, Ctrl+D, chiusura brutale del terminale;
- server con `timeout=5 interval=2` per verificare timeout e broadcast frequenti;
- partita con 32 client (limite) e 33° client (errore “Server pieno”).

### 10.3 Concorrenza e robustezza

- client che smette di leggere (Ctrl+Z o `kill -STOP`): verificare che il timer continui e che il client lento venga disconnesso;
- `kill -9` del client durante un broadcast: nessun crash del server, nessuna fd riusata in modo improprio;
- 10 client che si connettono e non inviano il login: il server non deve esaurire i thread (dopo la correzione C7);
- invio di struct malformate (tipo sconosciuto, direzione invalida, `num_players` enorme): nessun crash;
- esecuzione sotto `valgrind --leak-check=full` e `valgrind --tool=helgrind` (o TSan) per data race e fd leak;
- test su due macchine diverse, come richiesto dalla traccia.

### 10.4 Sanitizer

```bash
make clean
make CFLAGS="-std=c11 -Wall -Wextra -g -pthread -Icommon -fsanitize=address,undefined" \
     LDFLAGS="-pthread -fsanitize=address,undefined"
```

Il test con nickname di 40 caratteri deve fallire in modo pulito dopo la correzione C1, non con un crash.

---

## 11. Piano di intervento consigliato

### P0 — da fare assolutamente

1. **C1**: `scanf("%31s")` + controllo del ritorno; validazione del nickname.
2. **C2**: mutex di invio per giocatore (o coda + sender thread).
3. **C3**: proteggere insieme send e close; niente `close()` mentre un altro thread può inviare.
4. **C4**: eliminare l’I/O bloccante dal timer; timeout di invio e disconnessione dei client lenti.
5. **C7**: timeout di login e limite alle connessioni in accettazione.

### P1 — prima della consegna

6. **C5**: `game_over` sotto mutex o condizione/atomic.
7. **C6**: usare il valore di ritorno di `game_finish()`.
8. **C8/C9**: terminazione esplicita delle stringhe e clamp della `write` del log.
9. **C11**: `getaddrinfo()` per hostname.
10. **C12**: escludere i disconnessi dal calcolo del vincitore o documentare la scelta.
11. **Requisiti mancanti**: decidere con il docente su registrazione/password; documentare il limite di 32 utenti o eliminarlo con allocazione dinamica; correggere la vista iniziale e lo spawn se si vuole aderenza stretta.

### P2 — qualità e documentazione

12. **C10**: `volatile sig_atomic_t` + `sigaction`, handler installati prima del login.
13. **C14/C15**: etichetta “uscito”, larghezza mappa, ANSI clear, `isatty`.
14. **C13**: rimuovere o implementare `MSG_INFO`/`game_send_info`, `start_time`, `listen_fd`.
15. **C16**: dipendenze header nel Makefile, target `debug`/sanitizer, LF per il Makefile.
16. **C17**: correggere il README e scrivere la relazione con copertina, gruppo, protocollo byte-per-byte, scelte di concorrenza e test.

---

## 12. Giudizio complessivo

| Area | Valutazione | Commento |
|---|---|---|
| Architettura e leggibilità | 8/10 | modularità e commenti buoni |
| Funzionalità di gioco | 8/10 | tutte le meccaniche principali presenti |
| Correttezza concorrente | 5/10 | race su socket, fd e `game_over`; timer bloccante |
| Sicurezza dell’input | 3/10 | overflow banale nel client |
| Aderenza alla traccia | 6/10 | mancano registrazione, limite utenti, hostname, vista iniziale, relazione |
| Documentazione | 7/10 | README ottimo come guida, insufficiente come relazione |
| Test e robustezza | 4/10 | nessun test automatico, log con una sola partita |

**Conclusione:** è un progetto **ben impostato e già giocabile**, con una demo funzionante nel log. Le correzioni P0 sono poche e circoscritte: input del nickname, un mutex per socket, ciclo di vita delle fd, I/O non bloccante nel timer, timeout di login. Affrontate queste, il progetto passa da “buona esercitazione” a “consegna solida”. Restano poi da chiudere i punti di conformità formale (relazione, registrazione/password, limite utenti, hostname), che pesano quanto i bug in sede di valutazione.

---

## 13. Spunti per l’orale

1. Perché un thread per client e non `select()`/`poll()` anche nel server? Quali sono i costi con 32+ client?
2. Perché un unico mutex globale è semplice ma limita la concorrenza? Come lo separereste (maze, players, send)?
3. Come avete gestito short read/write e `EINTR`? Mostrate `protocol.c`.
4. Cosa succede se un client smette di leggere mentre il timer invia la mappa globale? Quale correzione avete previsto?
5. Perché `SIGPIPE` va ignorato? Cosa succede senza?
6. Perché `game_over` è un data race e come lo eliminereste?
7. Perché la struct `Messaggio` non è portabile tra architetture diverse? Come serializzereste i campi?
8. Come determinare il vincitore se un giocatore esce e poi si disconnette? Quale regola avete scelto e perché?
9. Perché la vista iniziale 5×5 non rispetta la traccia e come la cambiereste?
10. Come testereste il progetto su due macchine diverse e con client lenti o malintenzionati?
