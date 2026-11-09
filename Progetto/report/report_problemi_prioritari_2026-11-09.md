# Report di verifica e allineamento — Progetto Labirinto (LSO)

**Data:** 09/11/2026  
**Commit verificata:** `657584b` (`main`, `origin/main`)  
**Progetto:** `Progetto/labirinto/`  
**Traccia:** `Progetto/TRACCIAPROGETTO.txt` — Traccia A  
**Materiale del corso:** `Materiale/PDF Lezioni/Separati/extracted_text/`

---

## 0. Sintesi esecutiva

Il progetto è **funzionalmente vicino alla traccia**, ma **non è ancora pronto per la consegna**.

Dei tre problemi elencati nel report `Progetto/report/problemi_da_risolvere.md`:

| Problema | Stato |
|---|---|
| 1. Un client lento blocca timer e mappe periodiche | **Non risolto** |
| 2. Il riuso degli slot cancella i risultati dei giocatori | **Non risolto** |
| 3. La fine partita non viene rivalutata alla disconnessione | **Risolto** |

Sono inoltre emersi:

- una **regressione nel client** introdotta dall’ultima commit sul codice (`d6b0d43`): input non numerico nel menu → loop infinito;
- **data race confermate da ThreadSanitizer** nel broadcast delle mappe;
- possibili scritture concorrenti sullo stesso socket, con rischio di messaggi intrecciati o inviati al client sbagliato;
- **`Relazione.tex` non compila**;
- alcune affermazioni della documentazione non corrispondono al codice.

La priorità è quindi:

1. **P0** — risolvere i due problemi che violano direttamente la traccia;
2. **P1** — correggere le regressioni e le race di concorrenza;
3. **P2** — sistemare documentazione, logging, validazioni e casi limite;
4. **P3** — rifiniture e manutenzione.

---

## 1. Metodo di verifica

### 1.1 Cosa è stato fatto

- Analisi delle commit successive al report `problemi_da_risolvere.md`.
- Compilazione pulita con `make clean && make`.
- Test di integrazione con client “raw” Python e con il client reale.
- Autoplayer che esplora la mappa usando solo le viste locali e raggiunge un’uscita.
- Test unitari sul modulo `game.c` per vincitore, slot, oggetti e muri.
- Test di concorrenza con molti client e con un client lento.
- Esecuzione con AddressSanitizer/UndefinedBehaviorSanitizer.
- Esecuzione con ThreadSanitizer.
- Analisi statica con `gcc -fanalyzer`.
- Compilazione della relazione LaTeX con `pdflatex`.

### 1.2 Ambientazione dei test

I test sono stati eseguiti in directory temporanee (`/tmp/opencode/...`) per non sporcare il repository. Il repository è rimasto pulito (`git status` senza modifiche).

File di test prodotti:

- `labtest.py` — integrazione, autoplayer, lista 70 client, timeout, mappe periodiche, problemi 1/2/3;
- `game_unit.c` — test unitari su `game.c`;
- `client_input_tests.py` — input non numerico e Ctrl+D;
- `churn.py`, `tsan_exit.py` — stress e ThreadSanitizer.

### 1.3 Commit analizzate

| Commit | Contenuto |
|---|---|
| `86fad98` | Aggiunge il report con i tre problemi da risolvere |
| `7732dbc` | Gestione stato “uscito” nel client, Docker, ricontrollo `tutti_usciti()` alla disconnessione |
| `d6b0d43` | Rimuove i flush dell’input nel client, semplifica la schermata di uscita |
| `5fc0ad2` | Aggiunge `Relazione.md` |
| `657584b` | Aggiunge `Relazione.tex` |

La commit `7732dbc` risolve il problema 3. Le commit `5fc0ad2` e `657584b` sono solo documentazione. La commit `d6b0d43` introduce la regressione sul client. **Nessuna commit tocca il broadcast del timer o il riuso degli slot**: i problemi 1 e 2 restano.

---

## 2. Criteri di priorità

| Priorità | Significato | Esempi |
|---|---|---|
| **P0** | Viola un requisito esplicito della traccia o rompe una regola del gioco | client lento che blocca il server; vincitore sbagliato |
| **P1** | Bug di correttezza, regressione, race o crash possibile | input client, data race, fd riusato, `malloc` non controllata |
| **P2** | Allineamento parziale alla traccia, documentazione, logging, validazioni | IP non loggato, porta non validata, relazione che non compila |
| **P3** | Qualità, manutenzione, portabilità, pulizia della consegna | dipendenze Makefile, Docker extra, protocollo non portabile |

---

## 3. P0 — Problemi bloccanti

### 3.1 P0.1 — Un client lento blocca timer, mappe periodiche e timeout

**Stato: CONFERMATO, non risolto.**

#### Cosa richiede la traccia

La traccia chiede un server concorrente in grado di gestire simultaneamente:

- nuove connessioni;
- comandi dei client già connessi;
- invio periodico della mappa globale;

**senza bloccare i client**. Inoltre il timeout globale deve scattare indipendentemente dalle azioni dei client.

#### Cosa fa il codice

Il timer è un thread separato (`server/server.c:129-157`):

```c
while (!partita.partita_finita) {
    sleep(1);
    secondi++;

    if (secondi >= partita.timeout) {
        ...
        pthread_mutex_lock(&partita.mutex);
        termina_partita();
        pthread_mutex_unlock(&partita.mutex);
        broadcast_fine_partita();
    } else if (secondi % partita.intervallo == 0) {
        broadcast_mappe_globali();   /* riga 152 */
    }
}
```

`broadcast_mappe_globali()` (`server/game.c:529-534`) cicla sui giocatori e chiama `invia_mappa_globale()`, che a sua volta usa `invia_messaggio()` → `invia_dati()` → `write()` **bloccante** (`common/protocol.c:7-20`).

Se un client non legge, il suo buffer TCP si riempie. La `write()` del timer resta bloccata e con essa:

- l’invio delle mappe periodiche a **tutti** gli altri client;
- il controllo del timeout;
- il messaggio finale.

Il mutex viene rilasciato prima dell’invio, quindi non si blocca l’accesso allo stato di gioco, ma **si blocca comunque il thread timer**. Questa distinzione è importante: il requisito della traccia non è solo “non tenere il mutex durante la write”, ma “non bloccare il server”.

#### Evidenza sperimentale

Test:

- server con `timeout=6`, `intervallo=1`;
- un client osservatore che legge regolarmente;
- un client “pigro” che non legge e invia molte richieste `MSG_RICHIEDI_LISTA` per riempire il buffer.

Risultato:

- l’osservatore ha ricevuto **3 mappe periodiche**, poi più nulla;
- silenzio di **9,01 secondi**;
- dopo 12 secondi il timeout di 6 secondi **non era scattato**;
- il processo server era **ancora vivo**;
- nel `server.log` **nessuna riga** “Partita terminata per timeout”.

Questo è esattamente il comportamento che il report `problemi_da_risolvere.md` segnalava.

#### Perché è grave

Un solo client lento, distratto o malevolo può:

- impedire a tutti gli altri di ricevere le mappe periodiche;
- impedire la fine della partita per timeout;
- bloccare il messaggio finale.

È una violazione diretta della traccia ed è facilmente osservabile in sede d’esame.

#### Come correggere

La soluzione corretta non è “mettere un timeout alla `write`” e sperare: serve che il timer **non faccia mai I/O bloccante** sui socket dei client.

Opzioni consigliate, in ordine di robustezza:

1. **Thread sender per client con coda limitata**
   - Ogni client ha una coda di messaggi in uscita.
   - Il timer accoda la mappa e ritorna subito.
   - Un thread dedicato per client svuota la coda e scrive sul socket.
   - Se la coda è piena, si scarta la mappa periodica più vecchia (o si chiude il client lento).
   - Questo elimina anche le scritture concorrenti sullo stesso socket.

2. **Socket non bloccante + buffer di uscita per client**
   - `fcntl(fd, F_SETFL, O_NONBLOCK)`.
   - Il timer prova a inviare con `send(..., MSG_DONTWAIT)`.
   - Se il socket è pieno, scarta la mappa periodica per quel client.
   - Le risposte ai comandi possono essere gestite dal thread del client con una coda.

3. **Almeno un mutex per client e una `send` non bloccante**
   - Un mutex per socket evita l’interleaving.
   - La `send` non bloccante evita il blocco del timer.
   - Da solo non basta se si continua a usare `write` bloccante.

In ogni caso:

- il timer deve poter scattare anche se un client è lento;
- gli altri client devono continuare a ricevere le mappe;
- un client lento non deve ricevere messaggi mescolati.

#### Criterio di accettazione

Rifare il test con un client che non legge: gli altri client devono continuare a ricevere le mappe ogni T secondi e il timeout deve scattare regolarmente.

---

### 3.2 P0.2 — Il riuso degli slot cancella i risultati e falsa il vincitore

**Stato: CONFERMATO, non risolto.**

#### Cosa richiede la traccia

Le regole del vincitore sono:

- se esce **un solo giocatore** → vince lui;
- se escono **più giocatori** prima del timeout → vince chi ha raccolto più oggetti;
- se **nessuno esce** → vince chi ha raccolto più oggetti.

Per applicarle correttamente, i risultati dei partecipanti devono essere conservati fino alla fine della partita, anche se un giocatore si disconnette.

#### Cosa fa il codice

`aggiungi_giocatore()` cerca il primo slot con `attivo == 0` (`server/game.c:236-243`):

```c
int slot = -1;
for (int i = 0; i < partita.num_giocatori; i++) {
    if (!partita.giocatori[i].attivo) {
        slot = i;
        break;
    }
}
```

Poi reinizializza completamente quello slot (`server/game.c:270-281`):

```c
Giocatore *p = &partita.giocatori[slot];
p->fd = fd;
p->attivo = 1;
p->partecipante = 1;
p->uscito = 0;
...
p->punteggio = 0;
strncpy(p->nickname, nickname, MAX_NICK - 1);
```

Quando un giocatore si disconnette, `rimuovi_giocatore()` mette `attivo = 0` (`server/game.c:293-313`), ma **non conserva il record in una struttura separata**: lo slot diventa riutilizzabile e il nuovo giocatore ne cancella nickname, punteggio e stato `uscito`.

Il campo `partecipante` esiste, ma non impedisce il riuso: viene semplicemente sovrascritto.

#### Evidenza sperimentale

Scenario end-to-end:

1. Bob resta dentro il labirinto.
2. Alice raggiunge l’uscita.
3. Alice si disconnette.
4. Carlo si connette e prende lo stesso slot di Alice.
5. Bob e Carlo si disconnettono; la partita termina.

Log risultante:

```text
Fine partita. Vincitore: bob con 0 oggetti.
```

Il vincitore atteso era **Alice**, unica giocatrice uscita. Invece il suo record è stato cancellato.

Test unitario aggiuntivo:

- Alice esce con 7 oggetti;
- Alice si disconnette;
- Carlo prende il suo slot;
- a fine partita il vincitore è **Bob** (0 oggetti), non Alice.

#### Perché è grave

Non è solo un problema di “classifica”: viola direttamente la regola del vincitore della traccia. In particolare:

- se esce un solo giocatore, deve vincere lui, anche se si è disconnesso dopo l’uscita;
- se escono più giocatori, il confronto deve avvenire tra i loro punteggi conservati;
- il nuovo giocatore non deve ereditare né cancellare il risultato di chi c’era prima.

#### Come correggere

La soluzione consigliata è **separare i partecipanti dalle connessioni attive**:

```c
typedef struct {
    char nickname[MAX_NICK];
    int  punteggio;
    int  uscito;
    int  partecipante;
} Partecipante;

typedef struct {
    int  fd;
    int  attivo;
    int  indice_partecipante;
    int  riga, colonna;
    char scoperte[RIGHE_MAPPA][COLONNE_MAPPA];
} Connessione;
```

In alternativa, più semplice per il corso:

- **non riusare mai gli slot** fino alla fine della partita;
- quando un giocatore si disconnette, lascia il suo record nell’array con `attivo = 0`;
- un nuovo giocatore viene sempre aggiunto in un nuovo slot (con `realloc` se serve);
- la lista dei giocatori mostra solo gli `attivo`;
- `trova_vincitore()` usa i record storici;
- a fine partita si può liberare tutto.

Questa seconda soluzione è meno elegante ma è coerente con la traccia, che non impone un limite a priori al numero di utenti. La memoria cresce con il numero totale di partecipanti, non con quelli contemporanei: per un progetto d’esame è accettabile.

Va anche deciso e documentato il comportamento in caso di **reconnessione con lo stesso nickname**:

- nuovo partecipante con punteggio azzerato; oppure
- ripresa del record precedente.

La traccia non lo specifica, ma la scelta deve essere esplicita.

#### Criterio di accettazione

Rifare lo scenario P0.2: dopo la disconnessione di Alice e l’ingresso di Carlo, il vincitore deve essere ancora Alice.

---

## 4. P1 — Bug gravi di correttezza e robustezza

### 4.1 P1.1 — Regressione client: input non numerico nel menu → loop infinito

**Stato: CONFERMATO, introdotto da `d6b0d43`.**

#### Cosa è successo

La commit `d6b0d43` ha rimosso i blocchi che consumavano la riga dopo `scanf`. Prima esisteva un flush del tipo:

```c
int c;
while ((c = getchar()) != '\n' && c != EOF)
    ;
```

Dopo la modifica, in `client/client.c:82-94` e `client/client.c:166-177` è rimasto:

```c
if (scanf("%d", &scelta) != 1) {
}
```

Se l’utente inserisce un carattere non numerico (ad esempio `x`):

- `scanf("%d")` fallisce e **non consuma** `x`;
- `scelta` resta 0;
- il ciclo `while (scelta < 1 || scelta > 3)` riprova;
- `scanf` incontra di nuovo lo stesso `x` e fallisce di nuovo;
- il programma entra in **loop infinito**, stampando continuamente “Scelta non valida”.

#### Evidenza

Con input `x\n...`, il client ha prodotto **95.751 byte** di output in 3 secondi ed è stato terminato per timeout.

#### Perché è grave

Il client diventa inutilizzabile con un semplice errore di battitura. È una regressione rispetto alla versione precedente e va corretta prima della consegna.

#### Come correggere

Due opzioni:

1. **Ripristinare il flush** dopo ogni `scanf`, sia in caso di successo sia di errore, impostando `scelta = 0` in caso di errore.
2. **Usare `fgets` + `sscanf`** per leggere l’intera riga e validarla:

```c
char linea[64];
if (fgets(linea, sizeof(linea), stdin) == NULL) {
    /* EOF */
    return 0;
}
if (sscanf(linea, "%d", &scelta) != 1)
    scelta = 0;
```

La seconda è più robusta e gestisce anche EOF e righe lunghe.

---

### 4.2 P1.2 — `Ctrl+D` / EOF non gestito nel gioco

**Stato: CONFERMATO.**

Il `README.md:117` dichiara che `Ctrl+D` è una modalità di uscita ordinata. In realtà il client ignora il valore di ritorno di `fgets` (`client/client.c:228-231`):

```c
char buffer[64];
fgets(buffer, sizeof(buffer), stdin);
char comando = tolower(buffer[0]);
```

Su EOF:

- `fgets` restituisce `NULL`;
- `buffer` non contiene un comando valido;
- il client resta bloccato o entra in comportamento indefinito.

Testato: con stdin chiuso durante il gioco, il client non termina e va in timeout.

#### Correzione

```c
if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
    loop = 0;
    break;   /* oppure gestire l'uscita ordinata */
}
```

In alternativa, rimuovere `Ctrl+D` dalla documentazione se non si vuole gestirlo.

---

### 4.3 P1.3 — Data race nel broadcast delle mappe

**Stato: CONFERMATO da ThreadSanitizer.**

#### Cosa dice TSan

Con timer attivo ogni secondo e client che si connettono/disconnettono:

```text
WARNING: ThreadSanitizer: data race
  Read in broadcast_mappe_globali (server/game.c:531)
  Previous write in aggiungi_giocatore (server/game.c:259/272)
```

Le letture incriminate sono:

```c
void broadcast_mappe_globali(void) {
    for (int i = 0; i < partita.num_giocatori; i++) {
        if (partita.giocatori[i].attivo)   /* riga 531 */
            invia_mappa_globale(i);
    }
}
```

Queste letture avvengono **senza `partita.mutex`**, mentre `aggiungi_giocatore()` può:

- fare `realloc` dell’array (`game.c:251`);
- scrivere `partita.num_giocatori` (`game.c:260`);
- scrivere i campi del nuovo giocatore (`game.c:270-281`).

In C questo è un **data race**, quindi comportamento indefinito: nella pratica può portare a letture di memoria liberata, crash o invio di messaggi a slot sbagliati.

#### Altre letture non protette

- `timer_thread` legge `partita.partita_finita` nel `while` senza lock (`server.c:133`).
- `muovi_giocatore()` rilegge `partita.partita_finita` dopo l’unlock (`game.c:396`).
- `broadcast_fine_partita()` raccoglie gli fd sotto lock, ma poi invia dopo l’unlock (`game.c:559-574`): un client può disconnettersi e il suo fd può essere chiuso o riutilizzato.

#### Correzione

Per il broadcast:

1. Acquisire `partita.mutex`.
2. Copiare in un array locale gli indici (o gli fd) dei giocatori attivi.
3. Rilasciare il mutex.
4. Inviare solo agli elementi copiati.

Ma questo da solo non basta per il problema P0.1: se l’invio è bloccante, il timer si blocca comunque. La soluzione completa è quella descritta in P0.1 (coda/sender per client).

Per `partita_finita`, usare il mutex o una variabile atomica (`atomic_int`), oppure proteggere ogni lettura.

---

### 4.4 P1.4 — Scritture concorrenti sullo stesso socket, fd riutilizzati e mappa prima di `MSG_OK`

**Stato: da analisi del codice; non tutti riprodotti, ma plausibili e pericolosi.**

#### Problema A — Due thread scrivono sullo stesso socket

Sullo stesso fd possono scrivere contemporaneamente:

- il thread `client_handler` (risposta a un movimento, lista, ecc.);
- il thread `timer_thread` (mappa periodica);
- il thread che gestisce la fine partita.

Non esiste un mutex per-client. `invia_dati()` fa un ciclo di `write()` (`protocol.c:7-20`); se una `write()` è parziale, un altro thread può inserire il proprio messaggio nel mezzo. Il client legge `sizeof(Messaggio)` byte e può interpretare i dati nel modo sbagliato.

#### Problema B — fd chiuso mentre il timer lo usa

`rimuovi_giocatore()` esegue `close(fd)` sotto mutex (`game.c:298`). Il timer può aver già copiato lo stesso fd in `invia_mappa_globale()` e provare a scriverci dopo. Il fd può essere riutilizzato da una nuova connessione: il timer invierebbe una mappa al client sbagliato.

#### Problema C — Mappa inviata prima di `MSG_OK`

`aggiungi_giocatore()` rende il giocatore `attivo = 1` e restituisce lo slot. Subito dopo, `client_handler` invia `MSG_OK` e la mappa iniziale. Ma tra l’attivazione e l’invio di `MSG_OK` il timer può accorgersi del nuovo giocatore e inviargli una `MSG_MAPPA_GLOBALE`.

Il client, nella fase di autenticazione, si aspetta `MSG_OK` come primo messaggio (`client/client.c:138-153`). Se riceve prima una mappa, stampa “Risposta inattesa dal server” e si disconnette.

La finestra è piccola, ma esiste ed è più probabile con molti client e timer frequente.

#### Correzione

- Un **sender thread per client** con coda risolve sia A sia B.
- In alternativa, un mutex per client + `dup(fd)`/refcount prima dell’invio, e chiusura effettiva solo quando nessun sender lo usa.
- Aggiungere un flag `pronto` impostato **dopo** l’invio di `MSG_OK`; il timer e il broadcast devono ignorare i giocatori non ancora `pronto`.

---

### 4.5 P1.5 — `malloc` non controllata nel ciclo `accept`

**Stato: CONFERMATO da `gcc -fanalyzer`.**

`server/server.c:231-232`:

```c
int *pfd = malloc(sizeof(int));
*pfd = client_fd;
```

Se `malloc` fallisce, il server va in crash. Correzione:

```c
int *pfd = malloc(sizeof(int));
if (pfd == NULL) {
    close(client_fd);
    continue;
}
*pfd = client_fd;
```

---

### 4.6 P1.6 — Possibile doppio messaggio finale

**Stato: da analisi del codice; non riprodotto in modo deterministico.**

`timer_thread` chiama `broadcast_fine_partita()` **anche se** `termina_partita()` restituisce 0 perché la partita era già finita (`server.c:144-148`). Analogamente, `muovi_giocatore()` chiama `broadcast_fine_partita()` se vede `partita.partita_finita` dopo l’unlock (`game.c:396-397`).

Se due thread arrivano alla fine nello stesso istante, il messaggio finale può essere inviato due volte e il log può contenere due righe “Fine partita”. Il client chiude al primo, quindi l’impatto è limitato, ma il log e il protocollo non sono puliti.

Correzione: un flag `finale_inviato` protetto da mutex, oppure far sì che solo il thread che ottiene `termina_partita() == 1` esegua il broadcast.

---

### 4.7 P1.7 — `log.c`: possibile lettura oltre il buffer

**Stato: da analisi del codice; non riprodotto perché i messaggi attuali sono corti.**

`server/log.c:29-32`:

```c
char log_line[512];
int n = snprintf(log_line, sizeof(log_line), "[%s] %s\n", ts, msg);
if (n > 0)
    write(log_fd, log_line, n);
```

`snprintf` restituisce il numero di caratteri che **sarebbero stati scritti**. Se `n >= 512`, `write` legge oltre `log_line`. Oggi `msg` è al massimo un `log_buf[128]`, quindi `n` resta sotto 512, ma è una fragilità da correggere:

```c
if (n > 0) {
    size_t len = (size_t)n < sizeof(log_line) ? (size_t)n : sizeof(log_line) - 1;
    write(log_fd, log_line, len);
}
```

---

## 5. P2 — Allineamento alla traccia e documentazione

### 5.1 P2.1 — Logging: manca l’IP per le connessioni non autenticate

La traccia chiede di memorizzare data/ora di connessione e nickname, **oppure l’indirizzo IP se il nickname non è disponibile**.

Il server logga “Nuova connessione” solo dopo l’autenticazione (`game.c:285-289`). Ho aperto una connessione TCP senza inviare `MSG_ACCESSO`: nel log è comparsa solo la riga di avvio del server, **nessuna connessione**.

Correzione: loggare la connessione al momento dell’`accept`, usando `inet_ntop`/`getpeername` per ottenere l’IP; poi, dopo l’autenticazione, loggare anche il nickname.

---

### 5.2 P2.2 — Validazione debole della porta

`server.c:165` e `client.c:33` usano `atoi`. Questo accetta stringhe come:

```text
5200junk
```

e le interpreta come `5200`. Il server si avvia sulla porta valida, ignorando il resto. Non è un errore bloccante, ma la traccia parla di “porta” e una validazione corretta dovrebbe usare `strtol` con controllo di `endptr`, range `1..65535` e assenza di caratteri residui.

---

### 5.3 P2.3 — Protocollo non portabile tra architetture diverse

Il protocollo invia direttamente la struct C `Messaggio` (1104 byte) con:

- `int` di dimensione dipendente dall’architettura;
- padding del compilatore;
- endianness locale.

Su due macchine x86_64 Linux funziona. Su architetture diverse (o con compilatori/ABI diversi) può non funzionare. La traccia prevede il test su due o più macchine: se sono dello stesso tipo, il rischio è basso; se il docente usa architetture diverse, è un problema.

Correzione possibile:

- usare tipi a dimensione fissa (`uint32_t`);
- convertire i campi numerici in network byte order (`htonl`/`ntohl`);
- concordare un formato serializzato (o un protocollo testuale).

È un intervento abbastanza invasivo; da valutare in base al tempo disponibile. Se non si corregge, documentare esplicitamente che il progetto assume architetture con lo stesso ABI.

---

### 5.4 P2.4 — Oggetto sulla cella di spawn non raccolto

La traccia dice che quando il giocatore entra in una cella con un oggetto, lo raccoglie. Lo spawn sceglie una cella `CELLA_LIBERA` casuale (`game.c:263-268`), ma non esclude le celle con `oggetti[r][c] == 1`. Se il giocatore nasce su un oggetto:

- l’oggetto resta nella mappa;
- il giocatore non lo raccoglie finché non esce e rientra nella cella.

Non è un bug grave, ma è un caso limite. Correzione: escludere le celle con oggetto dallo spawn oppure raccogliere l’oggetto al momento del posizionamento.

---

### 5.5 P2.5 — Vincitore “disconnesso” quando nessuno esce

La traccia dice: se nessuno esce, vince chi ha raccolto più oggetti. Non specifica se i giocatori disconnessi debbano essere esclusi.

Il codice attuale, in `trova_vincitore()` (`game.c:160-168`), considera tutti i `partecipante`, anche quelli con `attivo == 0`. Quindi un giocatore disconnesso può vincere se ha più oggetti.

Il comportamento può essere accettabile se l’intenzione è conservare i risultati dei partecipanti (coerente con P0.2), ma va **deciso e documentato**. In alternativa, escludere i non attivi dal caso “nessuno esce”.

---

### 5.6 P2.6 — `users.txt` con account di test nel repository

Il file `Progetto/labirinto/users.txt` contiene account come:

```text
admin admin
mario 1234
luigi 1234
...
```

È un problema di pulizia e sicurezza: chiunque può usare quegli account. Inoltre il file non è in `.gitignore`, quindi viene versionato.

Prima della consegna:

- rimuovere gli account di test;
- creare un `users.txt` vuoto (o non includerlo e lasciarlo creare al server);
- aggiornare eventualmente Dockerfile/Compose se il file non viene più copiato;
- valutare se aggiungerlo a `.gitignore`.

---

### 5.7 P2.7 — `Relazione.tex` non compila

`pdflatex` fallisce con:

```text
! LaTeX Error: Not allowed in LR mode.
l.148 \node[box] (c1) {Client 1\\ ...};
```

I nodi TikZ usano `\\` per andare a capo, ma gli stili `box` e `server` (`Relazione.tex:141-142`) non hanno `align=center`.

Correzione verificata in una copia temporanea:

```latex
box/.style={..., align=center, ...},
server/.style={..., align=center, ...}
```

Con questa modifica il PDF compila e produce **17 pagine**.

---

### 5.8 P2.8 — Copertina incompleta

La traccia richiede i componenti del gruppo in copertina.

- `Relazione.tex:109`: manca la matricola di Giovanni Zapparella.
- `Relazione.md:12-13`: mancano matricola/email per entrambi.
- `Relazione.tex` riporta la matricola di Paolo, `Relazione.md` no: le due versioni sono incoerenti.

Da completare prima della consegna.

---

### 5.9 P2.9 — Affermazioni della documentazione non vere

Alcune frasi della relazione/README non corrispondono al codice:

| Documento | Affermazione | Realtà |
|---|---|---|
| `Relazione.md:426` | “Ogni operazione di lettura e modifica è protetta dal mutex” | Falso: `broadcast_mappe_globali()` legge senza lock; TSan conferma |
| `Relazione.md:432`, `README.md:196` | “Un client lento non blocca il server” | Falso: blocca il timer, le mappe e il timeout |
| `Relazione.md:434` | “I due mutex non vengono mai annidati” | Falso: `aggiungi_giocatore()` e `rimuovi_giocatore()` chiamano `log_event()` mentre tengono `partita.mutex`; non c’è inversione, ma sono annidati |
| `Relazione.md:643`, `Relazione.tex:745` | `static volatile sig_atomic_t loop = 1;` | Il codice ha `static int loop = 1;` (`client.c:17`) |
| `README.md:117` | `Ctrl+D` chiude il client | Non è gestito: `fgets` non controllato |

La documentazione va allineata al codice (o il codice alla documentazione). In particolare `loop` dovrebbe essere `volatile sig_atomic_t` per correttezza nei gestori di segnale.

---

### 5.10 P2.10 — Lunghezza della relazione

La relazione compilata è di **17 pagine**, mentre la traccia parla di “circa 10 pagine”. Non è un errore formale grave, ma conviene valutare se tagliare:

- la sezione Docker, non richiesta dalla traccia;
- parti ridondanti tra README e relazione;
- diagrammi molto estesi.

---

## 6. P3 — Rifiniture e manutenzione

### 6.1 Dipendenze del Makefile

Il `Makefile` non elenca gli header nelle dipendenze:

```make
bin/server: $(SERVER_SRC) | bin
```

Se si modifica `common/protocol.h` o `server/game.h`, `make` non ricompila automaticamente. Aggiungere le dipendenze degli header o usare `-MMD -MP`.

### 6.2 Docker e Python extra

La traccia non richiede Docker, Python o LaTeX. Il codice C rispetta le primitive del corso; Docker/Python sono strumenti accessori. Se il requisito è “solo ciò che abbiamo fatto nel corso”, si possono escludere dalla consegna. Attenzione: `docker/Dockerfile.server` copia `users.txt`, quindi se lo si rimuove va aggiornato anche il Dockerfile.

### 6.3 Protocollo: messaggi inutili o campi non usati

`MSG_NOTIFICA` è definito ma non usato. Non è un problema funzionale, ma il protocollo potrebbe essere semplificato. Inoltre la struct `Messaggio` contiene sempre tutti i campi (mappe comprese) anche per messaggi piccoli: è semplice ma spreca banda.

### 6.4 `termina_partita()` e log del vincitore

`broadcast_fine_partita()` viene chiamata anche quando la partita era già finita; oltre al doppio messaggio, può produrre più righe di log con lo stesso vincitore. Un flag `finale_inviato` risolve.

### 6.5 `log.c`: `localtime` e timestamp

`localtime()` restituisce un puntatore a una struttura statica; qui è protetta dal mutex del log, quindi va bene. Se in futuro si usa fuori dal mutex, usare `localtime_r`.

### 6.6 `users.txt`: parsing con `fscanf`

`fscanf(f, "%31s %31s", ...)` funziona per il formato attuale, ma non gestisce nickname/password con spazi e dipende dalla lunghezza fissa. Per il progetto va bene, ma è poco robusto. Se si vuole migliorare, usare `fgets` + parsing.

### 6.7 `MAX_NICK`/`MAX_PASS` e literal `31`

Le `scanf("%31s")` e `fscanf("%31s")` usano il numero 31 hardcoded. Se si cambia `MAX_NICK`/`MAX_PASS`, bisogna ricordarsi di aggiornare le stringhe di formato. Si può usare una macro con `#define STRINGIFY` o leggere con `fgets`.

### 6.8 Oggetti e spawn

Come detto in P2.4, escludere le celle con oggetto dallo spawn è una rifinitura semplice.

---

## 7. Cosa è già a posto

Per evitare di rifare lavoro già fatto, questi punti sono stati verificati e funzionano:

- compilazione pulita con `-Wall -Wextra`, nessun warning;
- visibilità iniziale: **1 sola cella**;
- dopo il primo movimento: rivelazione **5×5** (25 celle);
- `getaddrinfo` con `localhost` e nomi host: funziona;
- lista giocatori dinamica: **70 client** visualizzati correttamente (nessun limite a 64);
- registrazione separata dall’accesso;
- login con password errata, utente non registrato, nickname già attivo: rifiutati;
- vincitore con un solo uscito, più usciti, nessun uscito: test unitari passati;
- raccolta oggetto e incremento punteggio: testato;
- movimento contro muro: posizione invariata;
- timeout normale con client che legge: messaggio finale ricevuto;
- mappe periodiche con client normale: ricevute;
- ricontrollo alla disconnessione (problema 3): funziona;
- server senza output su stdout;
- client con `q` e `Ctrl+C`: uscita pulita;
- ASan/UBSan: nessun errore nei test eseguiti.

---

## 8. Matrice di allineamento alla traccia

| Requisito della traccia | Stato | Note |
|---|---|---|
| C su UNIX/Linux, socket TCP | OK | |
| Server concorrente | OK come thread-per-client | ma P0.1 blocca timer e broadcast |
| Gestione simultanea di connessioni, comandi e mappe periodiche | **PARZIALE** | P0.1 |
| Mappa 2D random con muri, celle libere, oggetti, uscite | OK | |
| Spawn in cella libera casuale | OK | caso oggetto su spawn: P2.4 |
| Vista iniziale solo della propria cella | OK | testato: 1 cella |
| Vista locale centrata sul giocatore | OK | 5×5 |
| Mappa globale mascherata periodica | OK con client normale | P0.1 con client lento |
| Timeout globale | OK con client normale | P0.1 con client lento |
| Fine partita quando tutti escono | OK | problema 3 risolto |
| Regola “un solo uscito vince” | **PARZIALE** | P0.2 |
| Regola “più usciti: più oggetti” | **PARZIALE** | P0.2 |
| Regola “nessuno esce: più oggetti” | OK nei casi normali | ambiguità disconnessi: P2.5 |
| Registrazione preventiva | OK | |
| Nessun limite a priori agli utenti | OK | testato 70 |
| Lista utenti collegati | OK | dinamica |
| Logging con data/ora, connessioni, conquiste | **PARZIALE** | P2.1: manca IP pre-auth |
| Server senza stdout, senza stdin | OK | |
| Client con hostname/IP e porta | OK | |
| Comandi client: muovi, lista, mappe, esci | OK | P1.1/P1.2 su input/EOF |
| Relazione: guida, protocollo, implementazione | OK nei contenuti | P2.7/P2.8/P2.9 |
| Copertina con componenti | **PARZIALE** | P2.8 |
| Relazione ~10 pagine | **PARZIALE** | 17 pagine |
| Test su due macchine | non verificato | protocollo non portabile: P2.3 |

---

## 9. Materiale del corso: cosa è stato usato

Il codice C usa primitive presenti nel materiale:

| Primitive/argomento | Uso nel progetto | Lezione indicativa |
|---|---|---|
| `open`, `write`, `O_APPEND`, `close` | `server/log.c` | Lezione 09/10 |
| `signal`, `SIGINT`, `SIGTERM`, `SIGPIPE` | client/server | Lezione 12 |
| `pthread_create`, `pthread_detach` | server | Lezioni 15–18 |
| `pthread_mutex_lock/unlock` | `partita.mutex`, `log_mutex` | Lezioni 17–18 |
| `socket`, `bind`, `listen`, `accept`, `connect` | server/client | Lezioni 19–24 |
| `select` | client | Lezione 22 |
| `getaddrinfo`, `gai_strerror`, `freeaddrinfo` | client | Lezione 24 |
| `fopen`, `fscanf`, `fprintf`, `malloc`, `realloc` | server | libreria standard C |

Non risultano usi di `epoll`, `poll`, `fork`, `pipe`, `mmap`, librerie esterne o altre primitive non coperte. Docker, Python e LaTeX sono strumenti accessori esterni al programma C.

---

## 10. Piano di lavoro consigliato

### Fase 1 — P0 (prima di tutto)

1. **P0.1**: eliminare le `write` bloccanti dal timer.
   - introdurre coda/sender per client;
   - oppure socket non bloccante + buffer;
   - aggiungere test con client lento.
2. **P0.2**: conservare i risultati dei partecipanti.
   - separare record partecipante da connessione, oppure non riusare gli slot;
   - aggiornare `trova_vincitore()`;
   - rifare i test end-to-end.

### Fase 2 — P1 (subito dopo)

3. **P1.1/P1.2**: correggere input client ed EOF.
4. **P1.3/P1.4**: eliminare le race e le scritture concorrenti.
5. **P1.5**: controllare `malloc` del `pfd`.
6. **P1.6**: evitare il doppio finale.
7. **P1.7**: correggere `log.c`.

### Fase 3 — P2 (prima della consegna)

8. Logging IP, validazione porta, protocollo portabile (o documentato), spawn su oggetto, politica disconnessi.
9. `users.txt` senza account di test.
10. `Relazione.tex` compilabile, copertina completa, affermazioni corrette, lunghezza.

### Fase 4 — P3

11. Dipendenze Makefile, pulizia Docker/Python, semplificazioni.

---

## 11. Test di accettazione consigliati

Dopo le correzioni, rifare almeno:

1. **Client normale**:
   - connessione con `localhost`;
   - registrazione e login;
   - movimento e raccolta oggetto;
   - lista giocatori;
   - timeout;
   - `q` e `Ctrl+C`.

2. **Client lento**:
   - un client non legge;
   - gli altri continuano a ricevere mappe;
   - il timeout scatta;
   - il server resta reattivo.

3. **Vincitore**:
   - un solo uscito, poi disconnesso;
   - più usciti con punteggi diversi;
   - nessuno uscito;
   - riuso slot dopo disconnessione.

4. **Concorrenza**:
   - 70+ client;
   - connessioni/disconnessioni durante il timer;
   - TSan senza warning;
   - ASan/UBSan senza errori.

5. **Documentazione**:
   - `pdflatex Relazione.tex` senza errori;
   - copertina completa;
   - nessuna affermazione non vera.

---

## 12. Conclusione

Il progetto ha già una base funzionante e molti requisiti della traccia sono rispettati. Tuttavia **non è ancora allineato** per due motivi principali:

1. un client lento può bloccare il timer e le mappe periodiche, violando il requisito di concorrenza “senza bloccare i client”;
2. il riuso degli slot cancella i risultati e può far vincere la persona sbagliata, violando le regole del vincitore.

A questi si aggiungono una regressione nel client, race di concorrenza e problemi di documentazione. Le correzioni P0/P1 sono localizzate e non richiedono di riscrivere l’architettura; la parte più delicata è il P0.1, perché richiede di cambiare il modello di invio (coda/sender per client). Una volta sistemati P0.1 e P0.2, il progetto sarà molto più vicino alla traccia e potrà essere presentato con ragionevole sicurezza.
