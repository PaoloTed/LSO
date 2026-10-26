# Problemi del progetto "Labirinto" — spiegati in modo semplice

Questo documento riassume **solo i problemi** trovati nella revisione indipendente. Il codice non è stato modificato.

**Verdetto della revisione:** il progetto **non è conforme alla traccia** e **non è pronto per la consegna**.

---

## 1. Cose che la traccia chiede e il programma non rispetta

### 1.1 Chiunque può entrare senza essere registrato
La traccia dice che bisogna registrarsi *prima* di poter giocare. Il server invece, se il nickname non esiste, lo crea e lo salva in `users.txt` al primo accesso. Quindi la registrazione preventiva non esiste.
*(Traccia riga 11 — `server/game.c`)*

### 1.2 All'inizio il giocatore vede troppo
La traccia vuole che all'inizio si veda **solo la propria cella**. Il server rivela subito l'area intorno (fino a 5×5 celle). Nelle prove: 20 celle visibili (25 se lo spawn è al centro).
*(Traccia riga 5 — `server/game.c`)*

### 1.3 Vince la persona sbagliata quando escono in più
Se più giocatori raggiungono l'uscita, deve vincere quello con più oggetti **tra quelli usciti**. Il codice guarda invece **tutti** i giocatori, quindi può vincere uno che non è nemmeno uscito (provato: vince uno ancora dentro con 1 oggetto).
*(Traccia riga 9 — `server/game.c`)*

### 1.4 Il client non accetta nomi di host
Il client funziona solo con indirizzi numerici (`127.0.0.1`). Con `localhost` o altri nomi dà "Indirizzo IP non valido". La traccia permette nome **o** IP.
*(Traccia riga 14 — `client/client.c`)*

### 1.5 La lista dei giocatori si ferma a 64
Il server accetta più di 64 giocatori, ma la lista dei collegati ne mostra solo 64. Con 65 connessi, uno manca.
*(Traccia righe 11 e 14 — `common/protocol.h`, `server/game.c`)*

### 1.6 Relazione senza copertina né componenti del gruppo
La documentazione (README) non ha la copertina con i nomi dei componenti richiesta dalla traccia.
*(Traccia riga 26 — `README.md`)*

---

## 2. Problemi che possono far sbagliare o bloccare il gioco

### 2.1 Un client lento blocca il timer (provato)
Se un giocatore smette di leggere i messaggi, il suo canale si riempie e il server resta bloccato nell'invio. Siccome il timer usa lo stesso thread:
- gli altri giocatori **non ricevono più le mappe periodiche**;
- **il timeout della partita non scatta** (provato: nessuna scadenza dopo 11 s con timeout 10 s).

Un solo client "pigro" o malevolo può quindi rovinare la partita a tutti.

### 2.2 Chi esce e poi si disconnette può perdere il risultato
Quando un giocatore si disconnette, il suo "posto" viene riusato dal nuovo che entra, cancellando punteggio e stato "uscito". Provato: un giocatore esce, si disconnette, ne entra un altro nello stesso posto e **vince senza essere uscito**.

### 2.3 Disconnessi che possono ancora vincere
Un giocatore disconnesso (non uscito) resta tra i "partecipanti" e, se nessuno è uscito, può essere scelto vincitore al timeout. La traccia non dice cosa fare in questo caso, ma il comportamento è probabilmente non voluto.

### 2.4 La fine "tutti usciti" non viene ricontrollata alla disconnessione
Se l'ultimo giocatore non uscito si disconnette, il server non rivaluta se la partita è finita.

---

## 3. Problemi di sicurezza e robustezza

### 3.1 Buffer overflow (provato con ASan)
Nickname e password sono letti con `scanf("%s")` / `fscanf("%s")` in buffer da 32 byte, senza limite di lunghezza:
- **client:** un nickname molto lungo manda in crash il programma (`client.c`);
- **server:** una riga lunga in `users.txt` causa lo stesso problema (`game.c`).

### 3.2 Problemi di sincronizzazione tra thread (da analisi del codice, non riprodotti)
- Alcune variabili condivise (come lo stato "partita finita" e l'array dei giocatori) sono lette **senza mutex** mentre un altro thread le modifica. Se l'array viene riallocato (`realloc`) durante una lettura, si può accedere a memoria non valida.
- Il numero di socket può essere chiuso e **riutilizzato** da un'altra connessione mentre un altro thread sta ancora scrivendo: un messaggio potrebbe arrivare alla persona sbagliata.
- Due thread possono scrivere sullo stesso socket contemporaneamente, mescolando i messaggi.
- L'invio del messaggio finale avviene tenendo il mutex globale: un client lento può bloccare tutto il server.
- Il messaggio di fine partita può essere inviato **due volte** (timer e movimento finale in contemporanea).

### 3.3 Errori ignorati
Il codice non controlla il risultato di diverse operazioni importanti:
- `malloc` non controllata nel server (se fallisce, il programma va in crash);
- scritture su `users.txt` e sul log;
- molti invii di messaggi in rete.

### 3.4 Protocollo non portabile
I messaggi sono inviati copiando direttamente la struttura C (3664 byte), senza gestire dimensioni, padding ed endianness. Su due macchine con architetture diverse potrebbe non funzionare. Non è mai stato provato su due macchine, come invece richiesto per l'orale.
*(Traccia riga 29)*

### 3.5 Controlli deboli su porta e credenziali
- Una porta come `5200junk` viene accettata come valida (`atoi`), poi la connessione fallisce.
- Nickname o password con spazi non funzionano con il formato del file `users.txt`.
- Una riga molto lunga digitata nel client può essere spezzata in più comandi separati.

### 3.6 Le password sono salvate in chiaro
Le credenziali sono scritte in `users.txt` senza alcuna protezione.

---

## 4. Errori nella documentazione (README)

Il README descrive cose che il codice non fa:

| Il README dice | Il codice fa |
|---|---|
| Uscite casuali | 4 uscite fisse, al centro dei bordi |
| 8 spawn sul perimetro | Spawn casuale in una cella interna libera |
| Dopo la connessione serve solo il nickname | Il client chiede anche la password |
| Nessun dato permanente | `users.txt` è un file persistente |
| Messaggio `MSG_QUIT` | Nel codice è `MSG_ESCI` |
| Nessuna race, grazie a un unico mutex | Ci sono letture fuori dal mutex |
| La lista viene paginata/divisa in blocchi | Nessuna paginazione: si taglia a 64 |

---

## 5. Cose non verificate (lacune della revisione)

- Nessuna prova con **ThreadSanitizer** (non funzionante nell'ambiente usato).
- Nessuna prova su **due macchine** o con architetture diverse.
- Race su socket riutilizzato, messaggi mescolati e doppio finale: **mai riprodotti**, rimangono solo ipotesi dal codice.
- Lunghezza della relazione (circa 10 pagine): non misurabile sul README in Markdown.
- Consegna (zip/rar su Teams) e orale: adempimenti esterni, non verificabili.

---

## 6. Riepilogo per priorità

**Alta**
1. Registrazione preventiva assente
2. Vincitore sbagliato con più usciti
3. Risultato dell'uscito cancellato dal riuso del posto
4. Mappa iniziale troppo rivelata
5. Lista limitata a 64 e nomi host rifiutati
6. Buffer overflow in client e server
7. Timer e mappe bloccati da un client lento
8. Race e sincronizzazione incompleta

**Media**
- Copertina e componenti mancanti nel README
- Possibile doppio messaggio finale
- Errori di `malloc`/file/rete ignorati
- Protocollo non portabile tra macchine diverse
- README con affermazioni false

**Bassa**
- Validazione debole di porta e credenziali
- Disconnessi ancora candidati alla vittoria
- Password in chiaro
