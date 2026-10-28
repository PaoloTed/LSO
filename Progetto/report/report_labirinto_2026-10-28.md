# Report di verifica del progetto Labirinto

**Data:** 28 ottobre 2026  
**Oggetto:** implementazione presente in `Progetto/labirinto`  
**Esito:** il gioco funziona nei percorsi ordinari, ma **non è ancora pronto per la consegna**: sono presenti errori di memoria, concorrenza e conservazione dei risultati, oltre a requisiti non rispettati.

## 1. Riferimenti e metodo

La verifica usa la [guida LSO](../../Materiale/Appunti/Guida_Completa_LSO.md) e la [Traccia A](../TRACCIAPROGETTO.txt), confrontate con tutti i sorgenti, il Makefile e il README attuali. Le indicazioni dei precedenti report non sono state assunte come prova dello stato corrente.

Le esecuzioni sono avvenute su Linux x86_64, GCC 15.2.0, usando una **copia temporanea** e archivi utenti separati. Sono stati impiegati test TCP, client reali in pseudoterminale, harness C sul modulo originale, AddressSanitizer/UndefinedBehaviorSanitizer e ThreadSanitizer. Il confronto finale conferma che i **14 file originali sono invariati**.

Nel seguito, **riprodotto** indica un comportamento osservato; **analisi statica** indica un difetto ricavato dal codice senza averne osservato tutte le conseguenze possibili. I riferimenti `file:numero` identificano le righe dei sorgenti attuali.

## 2. Prove eseguite

| Area | Prova e risultato |
| --- | --- |
| Compilazione | `make -B`: server e client compilano senza avvisi con i flag previsti. L'analisi GCC aggiuntiva segnala, fra l'altro, un `malloc` non controllato. |
| Accesso | Registrazione automatica, password corretta/errata, nickname duplicato e credenziali vuote verificati. Persistenza verificata anche dopo riavvio. |
| Concorrenza ordinaria | **64 e poi 96 client TCP contemporanei**: accesso, liste complete, movimenti e disconnessioni riusciti; successivo accesso ancora funzionante. |
| Trasporto TCP | Accesso inviato a frammenti di 7 byte e due richieste accorpate: correttamente ricostruiti. Verificati anche messaggio troncato, comando prima del login e tipo sconosciuto. |
| Generazione | **1.000 seed**: sempre 15 oggetti distinti su celle libere, quattro uscite e connessione fra tutte le celle percorribili. |
| Regole di movimento | Quattro direzioni, maiuscole, muro, direzione invalida, scoperta permanente e individuale: verificati. Raccolta simultanea dello stesso oggetto: assegnato un solo punto complessivo. |
| Partita completa TCP | Due bot hanno esplorato, raccolto oggetti e raggiunto l'uscita: 60 e 23 mosse, punteggi 2 e 1; vincitore corretto. |
| Termine ordinario | Verificate le tre regole del vincitore su partecipanti conservati, il termine quando tutti escono, il timeout e il rifiuto di nuovi accessi a partita conclusa. |
| Client reale | Collegamento con `localhost` e `127.0.0.1`, aiuto, lista, uscita con `q`, EOF, SIGINT e SIGTERM: riusciti in condizioni ordinarie. |
| Robustezza | Riprodotti: blocco del timer per client lento, overflow, race rilevate da ThreadSanitizer, perdita del risultato per riuso dello slot e blocco del client su frame parziale. |
| Logging e console | Timestamp ed eventi ordinari presenti; durante la prova base il server produce **0 byte su stdout e stderr**. Nel codice non legge da stdin. |

Le prove su più client usano processi e connessioni locali: **non equivalgono al collaudo su due macchine fisiche** previsto per l'orale. Non certificano tutte le pianificazioni possibili dei thread né l'assenza generale di perdite di memoria. Gli harness C completano anche il controllo delle perdite senza segnalazioni; non è una verifica equivalente sull'intero server.

## 3. Errori da correggere prima della consegna

### 3.1 Priorità alta — Input senza limiti e overflow

**Riprodotto con AddressSanitizer.** In `client/client.c:87` e `:91`, `scanf("%s", ...)` scrive in array da 32 byte senza limite. Un nickname di 500 caratteri provoca uno **stack-buffer-overflow**. In `server/game.c:68`, lo stesso problema riguarda `fscanf`: una riga malformata nell'archivio può far terminare l'intero server. Quest'ultima prova usa un archivio di test alterato, non dimostra da sola un attacco remoto.

**Correzione:** leggere righe con lunghezza controllata, rifiutare campi troppo lunghi e verificare EOF/errori; analizzare l'archivio per righe, con limiti e validazione di entrambi i campi. Il solo troncamento successivo con `strncpy` non protegge le letture precedenti.

### 3.2 Priorità alta — Un client lento ferma mappe periodiche e timeout

**Riprodotto sul server originale compilato normalmente.** Un client invia richieste di lista e smette di leggere le risposte. Con timeout impostato a 4 secondi, dopo 6 secondi il secondo client non ha ricevuto né mappe periodiche né fine partita; manca anche il log della scadenza. La sua richiesta di lista continua invece a funzionare.

Il timer esegue invii bloccanti in sequenza (`server/game.c:470`, `server/server.c:137`): basta una socket satura per arrestarlo. Inoltre `broadcast_fine_partita`, a `game.c:481`, mantiene il mutex globale durante gli invii. Un test isolato con socket satura conferma che quel mutex resta occupato.

**Correzione:** separare l'avanzamento del tempo dagli invii, usare una scadenza temporale effettiva e gestire code/timeout di scrittura per connessione. Non mantenere il mutex dello stato durante I/O bloccante. Il problema viola la richiesta di servire simultaneamente connessioni, comandi e aggiornamenti senza bloccare gli altri client; richiama anche l'errore discusso nella guida, §25.1.3.

### 3.3 Priorità alta — Accessi concorrenti non protetti

**Riprodotto con ThreadSanitizer:** due segnalazioni in `server/game.c:471` e `:472`, relative al broadcast che legge il numero dei giocatori e l'array mentre altri thread lo aggiornano o riallocano. Altre letture di `partita_finita` avvengono senza mutex, per esempio in `server/server.c:118`.

**Correzione:** applicare una disciplina coerente di sincronizzazione a tutte le letture e scritture condivise, comprese iterazione e durata di vita dell'array. Proteggere soltanto gli aggiornamenti non basta: il lettore deve usare la medesima protezione. Riferimento didattico: guida, §15.1–15.2.

### 3.4 Priorità alta — La riconnessione può cancellare un risultato valido

**Riprodotto nell'harness sul modulo originale.** `server/game.c:208` riusa il primo slot inattivo; a `:244` e `:247` azzera uscita e punteggio. Se chi era uscito con 10 oggetti si disconnette e arriva un nuovo utente, il suo risultato scompare. Nel caso controllato il vincitore diventa il nuovo arrivato con zero punti.

Le tre regole in `trova_vincitore` sono corrette **finché i dati dei partecipanti restano conservati**; il problema è la perdita di quei dati.

**Correzione:** separare la sessione TCP dal record del partecipante alla partita e mantenere i risultati fino alla conclusione, definendo anche il comportamento del rientro dello stesso nickname.

### 3.5 Priorità alta — Invii sulla stessa connessione senza coordinamento

**Analisi statica.** Timer e thread del client possono scrivere contemporaneamente sulla medesima socket: per esempio mappa globale (`game.c:420`) e lista (`:466`). Il ciclo che completa una scrittura parziale non rende indivisibile un intero messaggio. Con scritture parziali, i byte possono mescolarsi e il client perdere l'allineamento del protocollo.

Esiste inoltre una finestra fra copia del descrittore, rilascio del mutex e invio: una disconnessione può chiudere quel descrittore e consentirne il riutilizzo. **Non sono stati riprodotti né il mescolamento dei byte né l'invio a un descrittore riutilizzato.**

**Correzione:** assegnare a ogni connessione un proprietario degli invii, oppure serializzare l'intero messaggio con una protezione dedicata, coordinando anche chiusura e durata della connessione. Un mutex aggiuntivo non deve reintrodurre il blocco globale descritto sopra.

## 4. Aderenza alla traccia e alla guida

| Requisito | Valutazione attuale |
| --- | --- |
| C, UNIX/Linux e socket TCP | **Rispettato.** Thread POSIX, mutex, `select`, `getaddrinfo` e I/O su descrittori appartengono agli argomenti della guida. |
| Nessun limite prefissato agli utenti | **Rispettato nell'impostazione:** array e lista dinamici; provati 96 client. Restano i normali limiti di risorse, che non contraddicono il requisito. |
| Inizialmente visibile soltanto la propria cella | **Non rispettato:** `game.c:251` chiama già `scopri_dintorni`. Nel test erano visibili 20 celle. All'accesso va marcata soltanto la cella iniziale. |
| Mappa locale e globale mascherata per giocatore | **Funzionanti nell'uso ordinario.** La vista 5×5 è compatibile con l'esempio non vincolante 4×4 della traccia. Gli invii periodici hanno il difetto descritto in §3.2. |
| Oggetti, uscite e vincitore | **Funzionanti nei casi ordinari**, con la perdita di risultati descritta in §3.4. Le quattro uscite fisse non sono vietate dalla traccia. |
| Registrazione preventiva «al sito» | **Non realizzata letteralmente:** `game.c:62` registra automaticamente durante l'accesso TCP; nel progetto non c'è un sito o un flusso preventivo distinto. Occorre adeguare il flusso richiesto o ottenere dal docente conferma dell'equivalenza. Password e persistenza sono invece presenti. |
| Ordine di rete dei dati binari | **Non conforme alla guida, §17.8.** `common/protocol.c:45` e `:50` trasmettono direttamente le struct: interi nativi, dimensioni e padding dipendenti dall'ABI. Un accesso con tipo `1` codificato in ordine di rete viene rifiutato sull'host provato. Occorre serializzare campi di dimensione definita e convertirli con `htonl`/`ntohl`, anche nella lista. |
| Logging delle connessioni | **Parziale:** connessioni autenticate, oggetti e uscite sono registrati; una connessione senza login o con primo messaggio invalido viene chiusa senza registrarne l'IP (`server.c:55`). Registrare anche l'accettazione TCP, poi associare il nickname. |
| Relazione richiesta | Il README copre compilazione/uso, protocollo e dettagli implementativi, ma **mancano copertina e componenti del gruppo**. Il numero indicativo di circa 10 pagine va verificato su una versione impaginata. La traccia non impone un file distinto dal README. |

## 5. Altri problemi e convenzioni

- **Segnali e frame parziali — riprodotto:** dopo la ricezione di un solo byte del messaggio, il client resta bloccato anche dopo SIGINT; termina solo quando il server completa il frame. `select` garantisce disponibilità di dati, non dell'intero messaggio. Serve ricezione progressiva integrata nel ciclo degli eventi; usare `volatile sig_atomic_t` per il flag e preferire `sigaction` secondo la guida, §12.8. La sola sostituzione di `signal` non risolve il blocco.
- **Credenziali con spazi — riprodotto:** il server accetta `a b`, ma il formato dell'archivio lo spezza in token; due accessi hanno prodotto due righe uguali. Rifiutare spazi e caratteri di controllo, oppure adottare un formato che li rappresenti senza ambiguità. La password resta in chiaro: è una debolezza, ma la traccia non prescrive una libreria crittografica.
- **Errori non controllati — analisi statica:** `server.c:214` dereferenzia il risultato di `malloc` senza verificarlo; molte scritture di rete e le scritture di log/credenziali ignorano l'esito. Gestire fallimenti e scritture incomplete senza dichiarare riuscita un'operazione non completata.
- **Fine partita dopo disconnessione — riprodotto nell'harness:** se A è già uscito e B, ultimo giocatore ancora dentro, si disconnette, `rimuovi_giocatore` non rivaluta la fine. Il gioco prosegue fino al timeout, pur essendo tutti gli attivi usciti secondo il criterio interno di `tutti_usciti`. Va definita e applicata coerentemente la politica sulle disconnessioni.
- **Makefile — riprodotto:** dopo aver aggiornato il timestamp di `protocol.h` nella copia, `make -n` dichiara che non c'è nulla da ricompilare. Mancano le dipendenze dagli header; aggiungerle esplicitamente o generarle automaticamente.
- **Stile:** modularità, costanti, include guard e commenti rendono il codice leggibile. Uniformare nomi italiani/inglesi, indentazione e nomi come `vincitoreIndice` è utile, ma **la guida non stabilisce una convenzione obbligatoria su questi aspetti**. `fopen` appartiene alla libreria standard; `system("clear")` è trattata nel corso (§11.9), quindi non è di per sé vietata, pur introducendo una dipendenza dal comando esterno e dal terminale.

Il README va aggiornato: a riga 196 promette I/O sempre fuori dal mutex, contraddetto dal broadcast finale; a riga 204 descrive il flag dei segnali come sicuro senza le necessarie garanzie; a riga 122 afferma che inizialmente si vede solo la propria cella, contrariamente all'implementazione.

## 6. Ordine consigliato degli interventi

1. Eliminare overflow e gestire correttamente errori di lettura, allocazione e scrittura.
2. Correggere accessi concorrenti, invii e chiusure delle connessioni; rendere timer e aggiornamenti indipendenti dai client lenti.
3. Conservare i risultati della partita separatamente dalle sessioni TCP.
4. Adeguare vista iniziale, serializzazione e flusso di registrazione; completare il logging.
5. Correggere gestione dei frame parziali, dipendenze del Makefile e documentazione; ripetere infine il collaudo su macchine diverse.
