# Terza revisione indipendente e conclusiva — LSO “Labirinto”

**Data:** 25 ottobre 2026; verifiche integrative dello stesso giorno. **Oggetto unico:** `C:\Users\giova\LSO\Progetto\labirinto`. **Metodo:** esame in sola lettura dell'originale, build e prove solo su copia isolata. Questa revisione è stata condotta nell'ordine progetto → traccia e lezioni → audit 1 e 2 → test decisivi.

## A. Verdetto generale

**NON CONFORME ALLA TRACCIA.** Il programma compila e il nucleo del gioco funziona in molti percorsi osservati, ma viola requisiti espressi senza ambiguità: ammette e registra al login TCP utenti non registrati in precedenza su un sito (T:11), mostra più della sola cella iniziale (T:5), non accetta un nome host (T:14), tronca la lista quando i collegati superano 64 (T:11,14) e, quando escono più giocatori, può eleggere un non uscito (T:9). Manca inoltre la copertina della documentazione con i componenti (T:26). La conclusione dipende da codice e nuove prove di rete, non dall'autorità dei due audit.

“Non conforme” non significa che ogni funzionalità sia guasta: movimento, raccolta, mappe periodiche, timeout e un'uscita semplice sono riusciti nelle condizioni provate. Il blocco degli invii periodici e del timeout provocato da un client che non legge è ora **osservato** con server reale e harness TCP (§ G.1). Race sullo stato, invii mescolati, fd riutilizzato e doppia notifica restano rischi da analisi statica, senza guasto dinamico dimostrato.

## B. Perimetro, inventario, fonti e limiti

Abbreviazioni usate nel report: **P** = `C:\Users\giova\LSO\Progetto\labirinto`; **T** = `C:\Users\giova\LSO\Progetto\Progetto Lso.txt`; **M** = `C:\Users\giova\LSO(Deprecated)\Materiale`; **A1** e **A2** = rispettivamente i due audit nominati nella richiesta. `T:n` indica riga della traccia; `P\file:n` indica riga del file originale; le pagine dei PDF sono numeri di pagina del documento originale, contando dalla prima pagina PDF.

Ho inventariato e calcolato SHA-256 **prima** di rileggere gli audit in questa ripresa, confrontandoli con la tabella della stesura precedente e ricontrollandoli dopo le prove. I 14 file originali coincidono 14/14 con la stesura precedente e con il controllo finale: **nessun file del progetto è cambiato**. Non sono apparsi binari, log o altri file in P. La tabella riporta anche la dimensione originale.

| File relativo a P | Byte | SHA-256 iniziale = finale |
|---|---:|---|
| `.gitignore` | 24 | `9B121A86375A894EA6822578F896BF07C9C50F28D4002B342311B5329DFB093D` |
| `Makefile` | 499 | `5C941C7FD8A47656BFAC096F6000140D3D135BF8124502C519F2AB8B688559A2` |
| `README.md` | 9417 | `BB24118EEFC75D0C776B310BF50EBFB12030EDFEB597343D2610F9E287A3A0B4` |
| `users.txt` | 56 | `753C6B004C2442E3A866398B64B84F8C23870612E1ABA56329D3395725E4357D` |
| `client/client.c` | 7498 | `793340CCD0D4576642DA6519920EE5F634FA2CC007928C1B3C2B4BB3B5B78357` |
| `client/display.c` | 2842 | `9F0C94C02D3B0426C9DF2732F4EE21B2FF2418DC676440B62A203C9D77A63238` |
| `client/display.h` | 424 | `9D1EB15D6CFEEF8A7298D9669B8C1847E2970CAA905D8CB5D4CA30F550887362` |
| `common/protocol.c` | 992 | `67BABC517D41B46F2DD143E3C421384D37DAAD020EB74C93FBDFA2A063BC4048` |
| `common/protocol.h` | 2391 | `C67B63573257B5504F0C53EEA180B75DE959A3B822F0915F7F3BA58B846E38FF` |
| `server/game.c` | 14908 | `FB7329F99EBCE9B1E0B9EBB9B12C5DA6D64E6B502859DE3362F3F4D2F403896B` |
| `server/game.h` | 2259 | `C895CF3A2D0D07F675F3E1564285E61CDE36D92F7A5215A2BC23461873C93E61` |
| `server/log.c` | 858 | `16D87EB130731FF3B87ADF655B33916E8F2D177799989A81A26ABE72AFC7BDD4` |
| `server/log.h` | 171 | `D5B2F647954A35A6F735891927955C87AB28F98415DDE1BF2C359507399E843A` |
| `server/server.c` | 6409 | `67E86B12599C1955ED5CF3EED91680FA14C10AFD3174F45EF1C9AAF029BF44B6` |

Ho letto integralmente questi 14 file e T (29 righe). In M ho censito **44 PDF** (43 nella sottocartella `PDF Lezioni\Separati`, uno `PDF Lezioni\Unificato\slieds_laboratori_sistemi_operativi.pdf` di 1441 pagine), indicizzando il testo estratto **direttamente dai PDF originali** e rileggendo nei PDF le pagine pertinenti. Ho anche verificato visivamente campioni delle pagine originali: `Lezione024_socket-comandi.pdf` p. 22, `Lezione021_socket4.pdf` p. 39 e `Lezione019_socket-1.pdf` p. 1. Le estrazioni preesistenti e `Appunti\Guida_Completa_LSO.md`/`Piano_Studio_LSO.md` sono strumenti di ricerca, non testimonianze del docente. A1 e A2 sono stati letti solo dopo questa ricostruzione.

**Lezioni raster e limiti:** ho riesaminato gli originali dei quattro PDF quasi privi di testo estraibile rendendo e sottoponendo a OCR **tutte** le loro 108 pagine: `Lezione013_pipe1.pdf` (18), `Lezione014_pipe2.pdf` (33), `Lezione017_thread-cond-var.pdf` (17), `Lezione019_socket-1.pdf` (40). Per i punti pertinenti ho controllato l'OCR di `pipe1` pp. 17-18 (read/write, SIGPIPE), `pipe2` pp. 23-27 (FIFO), `thread-cond-var` pp. 1-9, 11-13 (mutex/condition variable) e `socket-1` pp. 3-8, 15-16, 25-28, 37-38 (socket, server concorrente, short I/O). Sono lezioni/esempi, non prescrizioni aggiuntive per Labirinto. **Non verificabili con affidabilità tramite OCR** restano le pagine prevalentemente schematiche `pipe1` pp. 10-11, `pipe2` pp. 11-13 e `socket-1` p. 13; i dettagli di sintassi nei frammenti raster di `thread-cond-var` pp. 6, 9-10, 14-16 non sono usati come prova letterale. Il PDF unificato di 1441 pagine è stato riesaminato tramite indice testuale (fra gli altri: progetto p. 6, thread p. 822/850, I/O p. 1085/1120, DNS p. 1217-1223); 288 pagine hanno estrazione quasi vuota e non sono state tutte verificate a vista. Le sottocartelle comprendono inoltre le estrazioni `.txt`, che restano ausili di ricerca, e il manuale Bash, non pertinente a un requisito specifico C/TCP. Nessuna attribuzione decisiva qui si fonda su una pagina OCR incerta o su appunti AI. Non dispongo di due macchine distinte o ABI diverse; il runtime TSan già tentato su WSL dava `unexpected memory mapping`, e Docker Desktop non aveva un daemon Linux attivo. Non ho ripetuto la stessa prova TSan nelle stesse condizioni.

**Ambiente e contenimento:** README:25-79 documenta `make`, `./bin/server <porta> [timeout] [intervallo]` e `./bin/client <indirizzo_ip> <porta>`. Questa ripresa ha usato esclusivamente la copia `C:\Users\giova\AppData\Local\Temp\lso-labirinto-review-658ce781425b4a3985b642764126d53d`, esterna sia a P sia a M: `wsl --exec sh -lc 'cd /mnt/c/Users/giova/AppData/Local/Temp/lso-labirinto-review-658ce781425b4a3985b642764126d53d && make'`. Binari, script, credenziali e log dei nuovi test sono rimasti lì. La stesura precedente aveva usato `C:\Users\giova\LSO\_terza_revisione_labirinto_tmp`; al controllo odierno tale vecchia directory è **assente**. Il precedente rifiuto automatico della sua rimozione è un fatto storico del report, non descrive lo stato attuale. Lo stato degli artefatti di questa ripresa è in § B.1.

### B.1 Artefatti temporanei della ripresa

La pulizia ricorsiva della copia è stata tentata una sola volta, dopo aver verificato il percorso assoluto sotto `%TEMP%` e fuori da P/M. La revisione automatica l'ha respinta con `blocked by policy`. Non ho aggirato il blocco. Restano 37 file sotto la radice assoluta `C:\Users\giova\AppData\Local\Temp\lso-labirinto-review-658ce781425b4a3985b642764126d53d`. Ogni riga seguente è relativa a tale radice e determina un percorso assoluto esatto tramite concatenazione `radice\riga`:

```text
__pycache__\open_tests.cpython-312.pyc
.gitignore
bin\client
bin\server
cases\burst\server.log
cases\burst\stderr.txt
cases\burst\stdout.txt
cases\burst\users.txt
cases\slow_variant\server.log
cases\slow_variant\stderr.txt
cases\slow_variant\stdout.txt
cases\slow_variant\users.txt
cases\slow\server.log
cases\slow\stderr.txt
cases\slow\stdout.txt
cases\slow\users.txt
client\client.c
client\display.c
client\display.h
common\protocol.c
common\protocol.h
Makefile
ocr4.json
ocr4.py
open_tests.py
pdfpages.py
pdfscan.py
README.md
server\game.c
server\game.h
server\log.c
server\log.h
server\server.c
slow_variant.py
unified.py
update_report.py
users.txt
```

## C. Analisi autonoma del codice, compiuta prima degli audit

**Architettura.** `Makefile:1-21` costruisce due eseguibili C/POSIX con GCC e pthread. `server/server.c:create_server_socket:20-45` ascolta su IPv4 `INADDR_ANY`. `main:185-219` accetta in ciclo e avvia un thread distaccato per connessione; `timer_thread:113-138` dorme un secondo per iterazione e gestisce timeout e broadcast. Non ci sono processi figli, pipe o memoria condivisa fra processi; i thread condividono lo stesso processo. `game.h:8-36` definisce matrice, oggetti, array dinamico dei giocatori, matrice `scoperte` per ciascuno, stato di fine, punteggi e mutex globale. `log.c:8-35` apre `server.log`, aggiunge timestamp e usa un mutex dedicato.

**Mappa e regole.** `game.c:genera_labirinto:15-59` crea una griglia 21×41, perimetro murato, ostacoli random soltanto in alcune coordinate pari, quattro uscite **fisse** ai punti medi dei bordi e 15 oggetti in celle interne libere random. `aggiungi_giocatore:223-241` sceglie uno spawn interno libero random, ma chiama già `scopri_dintorni` (fino a 5×5). `muovi_giocatore:263-334` aggiorna posizione per `w/a/s/d`, impedisce il muro, rivela l'area, rimuove l'oggetto dalla matrice globale, incrementa punteggio e marca `uscito` quando si raggiunge E. `invia_mappa_locale:357-379` e `invia_mappa_globale:381-414` compongono viste con celle non scoperte come `?`. Le scoperte sono personali e persistenti; gli oggetti sono comuni e si rimuovono per tutti.

**Protocollo, autenticazione e client.** `protocol.h:24-65` definisce tipi e una `Messaggio` C fissa; su questa ABI vale 3664 byte. `protocol.c:5-40` invia/riceve l'intera struct con `write/read`, ripetendo su short I/O ed `EINTR`, senza serializzazione portabile di interi o padding. `server.c:47-110` attende `MSG_ACCESSO`, poi `MSG_OK` e prima mappa, quindi movimenti, lista e uscita. `game.c:autentica_utente:61-90` legge `users.txt`; se il nickname non esiste, lo aggiunge al file durante il medesimo accesso TCP. Le credenziali sono in chiaro. `client/client.c:28-238` chiede nickname/password, usa soltanto `inet_pton(AF_INET,...)`, multiplexa stdin e socket con `select`, gestisce `w/a/s/d,l,h,q`. `display.c:11-75` stampa mappe, lista e colori ANSI.

**Fine e risultati.** `game.c:trova_vincitore_bloccato:115-141` sceglie l'unico uscito se esattamente uno. Negli altri casi cerca il punteggio massimo fra tutti gli slot `partecipante`, senza filtrare `uscito` quando gli usciti sono più di uno. `tutti_usciti_bloccato:143-155` considera soltanto gli slot attivi; il controllo è fatto durante il movimento che raccoglie o esce (`game.c:309-314`), non alla disconnessione. `rimuovi_giocatore:251-260` mette `attivo=0`; un login successivo può riusare lo slot e cancellarne risultato e stato d'uscita (`game.c:198-241`). Il timeout conclude la partita (`server.c:118-130`); il processo continua ad accettare connessioni, ma rifiuta il login a partita finita (`server.c:71-87,194-219`). La traccia non chiarisce se la fine della partita debba spegnere il processo.

**Problemi identificati prima degli audit.** Il codice mostra direttamente registrazione automatica, rivelazione iniziale anticipata, filtro errato del vincitore, riuso degli slot, limite della lista, solo IP numerico, descrizioni README inesatte e input `scanf/fscanf("%s")` senza larghezza. Le letture di `partita_finita` (`server.c:118`) e dei giocatori nel broadcast (`game.c:442-446`) non sono sincronizzate con scritture/realloc. Gli invii locali e globali copiano fd sotto lock e scrivono dopo lo sblocco (`game.c:375-378,410-413`), mentre la rimozione può chiudere e il sistema riutilizzare il numero; thread diversi possono scrivere sullo stesso fd senza serializzazione. `broadcast_fine_partita:449-468` fa I/O bloccante sotto mutex globale; il timer ignora il risultato di `termina_partita()` e potrebbe duplicare il finale (`server.c:123-129`). La prova nuova ha **osservato** il timer bloccato su un client lento, con altri client privati delle mappe periodiche e senza scadenza del timeout (§ G.1). Gli altri interleaving citati restano possibilità statiche, senza evento di rete osservato.

## D. Checklist indipendente della traccia

Stati: **OK** = requisito soddisfatto nel perimetro provato; **PARZIALE** = implementazione presente ma con limite o semantica non chiarita; **ERRORE** = conflitto con il testo; **NON VERIFICABILE** = manca una prova per l'adempimento. La fonte normativa è sempre T, non un audit. “4×4” in T:12 è preceduto da “es.”; non impone quella dimensione.

| Requisito della traccia | Stato | Prova precisa |
|---|---|---|
| Sistema C su UNIX/Linux, client/server TCP (T:3,16) | OK | `P\Makefile:1-14`; `server.c:create_server_socket:20-45`; `client.c:44-67`; build WSL riuscita. |
| Mappa 2D generata randomicamente all'avvio, muri/libere/alcune uscite/oggetti (T:3,5) | OK | `P\server\game.c:genera_labirinto:15-59, inizializza_gioco:92-101`; ostacoli e oggetti random, uscite fisse: T non richiede uscite random. |
| Mappa intera solo al server e scoperte per giocatore (T:3,12) | OK | `P\server\game.h:8-36`; `game.c:397-409` maschera nel messaggio. |
| Più partecipanti contemporanei, spawn casuale in cella libera (T:3,5) | OK | `P\server\server.c:194-219`; `game.c:223-241`; 65 login contemporanei provati. |
| All'inizio solo propria cella visibile (T:5) | **ERRORE** | `P\server\game.c:103-113,240-241`; prima mappa del nuovo test: **20** celle visibili (spawn vicino al bordo). Il 25 di A1/A2 era un altro spawn, non un'invariante. |
| Quattro direzioni e aggiornamento movimento (T:6) | OK | `P\server\game.c:263-290`; `client.c:158-164`; percorso reale fino a E. |
| Raccolta dell'oggetto e rimozione dal server (T:6) | OK | `P\server\game.c:292-297`; test reale: 0→1, ritorno sulla cella 1→1. |
| Dopo il movimento rivelazione attorno e permanenza delle celle (T:6,12) | OK | `P\server\game.c:103-113,288-290,397-409`; test: celle prima visibili restano tali dopo movimento; l'avvio prematuro è riga distinta. |
| Vista locale centrata e limitata alle celle scoperte a ogni mossa (T:3,12) | OK | `P\server\game.c:330,336-379`; messaggio locale 5×5. |
| Mappa globale intera mascherata ogni T, indipendente dai comandi (T:3,13) | PARZIALE | `P\server\server.c:113-135`, `game.c:381-446`; invio autonomo riuscito con lettori. Nuova prova § G.1: un peer che non legge satura il proprio socket; il client veloce riceve **zero** mappe per 11,07 s con intervallo 1 s. |
| Timeout globale (T:7) | PARZIALE | `P\server\server.c:118-130`; notifica in condizioni ordinarie e “nessun vincitore” senza client. Nuova prova § G.1: con invio bloccato a un client lento, **nessuna scadenza** registrata dopo 11,07 s con timeout 10 s; il timer è lo stesso thread degli invii. |
| Fine quando tutti usciti (T:7) | PARZIALE | `P\server\game.c:143-155,299-334`; con un solo giocatore l'uscita termina. Dopo disconnessione la condizione non è rivalutata (`251-260`); T non precisa se “tutti” includa i disconnessi. |
| Un solo uscito vince (T:8) | OK | `P\server\game.c:115-127`; uscita reale di admin con mario dentro → vince admin. Conservazione del risultato dopo distacco: problema distinto. |
| Più usciti: vince il più ricco **tra gli usciti** (T:9) | **ERRORE** | `P\server\game.c:129-140` filtra `partecipante`, non `uscito`; test reale: due usciti, luigi dentro con 1, annunciato luigi. |
| Nessun uscito: più oggetti (T:10) | OK | `P\server\game.c:129-140`; zero usciti e parità 0–0: annunciato admin. T non disciplina la parità né i disconnessi. |
| Registrazione **prima** dell'accesso **al sito**, nickname/password (T:11) | **ERRORE** | `P\server\game.c:61-90,177-188`; login TCP nuovo riceve `MSG_OK` e viene scritto in `users.txt`. Nessun sito nel perimetro; anche ipotizzandone uno esterno, il server non pretende preregistrazione. Nessuna tecnologia web specifica imposta da T. |
| Nessun limite a priori di utenti collegabili (T:11) | OK | `P\server\game.c:198-220` espande l'array; 65 login ammessi. Restano limiti fisici di thread/fd/RAM. |
| Lista degli utenti collegati (T:14) | **ERRORE** | `P\common\protocol.h:22,59-60`; `game.c:416-440`; 65 connessi, risposta con 64 e ultimo `tr3u063`. |
| Nuove connessioni, comandi e invii concorrenti senza bloccare gli altri (T:13) | PARZIALE | Thread separati `P\server\server.c:47-137,194-219`; accessi fuori lock `server.c:118`, `game.c:442-446`; finale sotto lock `game.c:449-468`. § G.1 mostra un **FAIL osservato**: un peer lento ferma il timer e gli aggiornamenti a un altro peer, mentre il thread del client veloce può ancora rispondere a `MSG_RICHIEDI_LISTA`. |
| Logging attività principali in file (T:13) | OK | `P\server\log.c:11-35`; `game.c:245-258,317-327,449-462`; file temporaneo con timestamp e vittoria/assenza vincitore. L'IP in T:13 è un **esempio**, non obbligo autonomo; esiti di `write` non verificati. |
| Client: nome **o** indirizzo IP e porta a riga di comando (T:14) | **ERRORE** | `P\client\client.c:28-67`; `./bin/client localhost 5200` → “Indirizzo IP non valido”, exit 1. |
| Client: nickname, movimento, disconnessione, lista, mappe (T:14) | PARZIALE | `P\client\client.c:71-238`, `display.c:22-75`; client reale PTY: login/lista/uscita 0; lista incompleta >64 e input credenziali lunghi insicuri. |
| Solo libreria standard C e system call UNIX; server senza stdin/stdout, stderr solo alla terminazione (T:17) | OK | `P\Makefile:1-14`, `server.c:140-219`; cattura stdout/stderr server 0/0 byte nella prova. `system("clear")` è nel client, non viola il vincolo sul server. |
| Relazione: guida, protocollo applicativo, dettagli interessanti (T:18-24) | PARZIALE | `P\README.md:25-197` ha queste sezioni, ma contiene affermazioni false (§ H). T non richiede espressamente un file con estensione diversa dal README. |
| Relazione con copertina e componenti (T:26) | **ERRORE** | `P\README.md:1-197` non ha copertina o nomi. |
| Lunghezza indicativa di circa 10 pagine (T:26) | NON VERIFICABILE | Il Markdown di `P\README.md:1-197` non è impaginato; “dovrebbe” rende indicativa la lunghezza. |
| Archivio zip/rar, invio via Teams entro scadenza (T:28-29) | NON VERIFICABILE | È adempimento esterno/futuro; nessuna prova di consegna nel perimetro. |
| Test orale su almeno due macchine (T:29) | NON VERIFICABILE | Solo WSL/loopback; `P\common\protocol.h:38-61` e `protocol.c:7-8,25-26` usano layout nativo, rischio di interoperabilità fra ABI diversi, non fallimento provato su due Linux omogenei. |

## E. Lezioni: obblighi, esempi e interpretazioni

La traccia è il riferimento per gli obblighi; la lezione di presentazione conferma che si consegnano software e relazione (`Lezione01_presentazioneCorso25-26.pdf` pp. 6–7). Le citazioni qui sono state ricontrollate nel PDF originale, non dedotte dagli appunti. L'eventuale difetto indicato nell'ultima colonna non trasforma automaticamente un esempio in prescrizione.

| Tema e fonte originale | Natura | Confronto con P |
|---|---|---|
| Software Linux client/server e relazione, `Lezione01_presentazioneCorso25-26.pdf` pp. 6–7 | Indicazione esplicita, ribadita in T:16,18-26 | Software presente; README copre parte della relazione; copertina/componenti assenti. |
| GCC e `-Wall`, `Lezione08_C-gcc.pdf` p. 6 | Metodo illustrato | `P\Makefile:1-14` usa entrambi; p. 7 è sostanzialmente una slide titolo, non fondamento di una regola sui warning. |
| Thread per client, `Lezione016_thread2.pdf` p. **10**; esempio di `malloc` controllato e detach, `Lezione021_socket4.pdf` pp. 20–21 | Architettura/esempio | Server usa thread e detach (`server.c:209-218`), ma dereferenzia `malloc` senza controllo (`209-210`). La p. 9 del primo PDF non mostra quel testo. |
| Mutex a protezione dello stato, `Lezione018_thread-synch-esempi.pdf` pp. 5–6 | Principio presentato tramite bounded buffer | `game.c:442-446` e `server.c:118` lasciano letture fuori lock. Nessun obbligo dimostrato di usare più mutex o distruggerli a fine partita. |
| Cicli I/O completo, `Lezione021_socket4.pdf` pp. 37–40; `EINTR` e `SIGPIPE`, `Lezione022_socket5.pdf` pp. 1–8 | Metodo illustrato | `protocol.c:5-40` ripete letture/scritture; `server.c:164-165` e `client.c:41-42` ignorano SIGPIPE. `read/write` su TCP sono ammissibili; molti chiamanti ignorano il risultato finale di invio. |
| Buffer TCP e blocco di `write`, `Lezione021_socket4.pdf` p. 39; timeout socket, pp. 31–33 | Spiegazione e alternativa mostrata | Spiega il meccanismo; la prova del blocco del timer è nel test § G.1. Non impone uno specifico timeout. |
| `select` e ricostruzione del set, `Lezione022_socket5.pdf` pp. 9–13, 18–20 | Metodo/esempi di server e client | `client.c:125-147` usa master/read set e `max_fd+1`; `select` nel server non è prescritto, poiché il server usa thread. |
| `inet_pton` per IP, `Lezione020_socket3.pdf` p. 10; `getaddrinfo` per nomi, `Lezione024_socket-comandi.pdf` pp. 21–22 | Due metodi diversi | `client.c:56-60` sceglie il primo e non risolve nomi. Il requisito del nome deriva da T:14, non dalla preferenza per `getaddrinfo`. |
| Tipi fissi, network byte order e framing, `Lezione019_socket-2.pdf` pp. 9–11 | Spiegazione/esempio di protocollo binario | `protocol.h:38-61`, `protocol.c:7-8` mandano la struct nativa; portabilità non garantita, ma T non vieta letteralmente le struct grezze. |
| Errori di syscall, `Lezione09_io.pdf` p. 16; controllo `malloc`, `Lezione021_socket4.pdf` p. 21 | Metodo illustrato | Controlli parziali: mancano `malloc` in `server.c:209-210`, scritture file in `game.c:81-83`, log `log.c:33` e molti invii. |
| `system()` in C, `Lezione08_C-gcc.pdf` p. 14 | Funzione mostrata, non preferenza per cancellare terminale | `display.c:31-53` usa `system("clear")`; dipende da ambiente/TERM, ma nessun divieto docente dimostrato. |
| Guide Markdown in `M\Appunti` | Interpretazione di altre AI | Utili per localizzare argomenti, non fonte delle preferenze del professore. Il commento `client.c:125` che cita una guida non cambia questa distinzione. |

Non ho trovato una prescrizione verificabile su naming, indentazione, `fork` obbligatorio, numero dei mutex o sostituzione di `read/write` con `recv/send`. Le 108 pagine raster dei quattro PDF sono state ora passate a rendering/OCR; le pagine schematiche e i frammenti indicati in § B restano non verificabili **letteralmente** con affidabilità. Non ne deduco obblighi. Il PDF unificato è stato indicizzato, senza controllo visivo esaustivo delle 1441 pagine.

## F. Registro comparativo dei due audit, rilievo per rilievo

Solo a questo punto della revisione ho letto integralmente A1 (172 righe) e A2 (228 righe). Stati: **CONFERMATO** = tesi sostanzialmente supportata, sempre al livello di prova indicato; **DA CORREGGERE** = frase, attribuzione, prova o gravità eccessiva; **NON DIMOSTRATO** = conclusione non sostenuta dalle fonti disponibili; **NON PIÙ APPLICABILE** = il progetto è cambiato. Le righe di codice della tabella sono quelle di P. Per i test storici non ho assunto veri i risultati perché scritti negli audit: li ho confrontati con codice e, quando decisivi, ripetuti in § G.

| Rilievo A1 (§ F), ripreso da A2 (§ E) | A1: giudizio terza revisione | A2: giudizio terza revisione | Prova/precisazione |
|---|---|---|---|
| 1. Registrazione preventiva/sito | CONFERMATO | CONFERMATO | T:11; `game.c:61-90,177-188`; nuovo accesso TCP ammesso e persistito. Non è prescritto un framework web specifico. |
| 2. Vincitore con più usciti | CONFERMATO, prova originaria solo **unitaria artificiale** | CONFERMATO, test di rete con **harness TCP** | T:9; `game.c:115-140`; nuovo test: due usciti e non uscito con 1, vince quest'ultimo. Non chiamare end-to-end il solo stato artificiale di A1. |
| 3. Prima mappa | CONFERMATO, numero 25 da contestualizzare | CONFERMATO, numero 25 da contestualizzare | T:5; `game.c:103-113,240-241`; qui 20 celle per uno spawn vicino al bordo. L'errore è “più di una”, non “sempre 25”. |
| 4. Host/IP | CONFERMATO | CONFERMATO | T:14; `client.c:51-60`; client reale rifiuta `localhost`. `Lezione024_socket-comandi.pdf` p. 22 verificata. |
| 5. Lista >64 | CONFERMATO | CONFERMATO | T:11,14; `protocol.h:22`, `game.c:426-439`; 65 accessi, 64 nomi. Non esiste limite di accesso pari a 64. |
| 6. Relazione autonoma assente | **DA CORREGGERE** | CONFERMATO nella formulazione prudente di A2 | T:18-26; `README.md:25-197` copre tre sezioni. Nessun obbligo testuale di file separato; mancano copertina e componenti, e il testo è parzialmente falso. Numero pagine non misurabile sul Markdown. |
| 7. Race su flag/array e potenziale `realloc` | CONFERMATO come analisi statica | CONFERMATO come analisi statica | `server.c:118`; `game.c:207-220,251-260,442-446`; TSan fallisce all'avvio in WSL, non c'è crash attribuibile osservato. |
| 8. Invii concorrenti stesso fd | CONFERMATO come possibilità statica | CONFERMATO come possibilità statica | `game.c:357-440,449-468`; `protocol.c:6-20` non serializza chiamanti distinti. Nessun flusso corrotto catturato. |
| 9. Peer lento blocca timer/finale sotto mutex | CONFERMATO; il blocco del **timer** è ora osservato | CONFERMATO; il blocco del **timer** è ora osservato | `game.c:442-468`; `protocol.c:10-19`. Il test precedente di 6 s non saturava la connessione; il nuovo flood a peer non lettore produce zero mappe per l'altro client e nessuna scadenza al timeout 10 s (§ G.1). Il blocco del broadcast **finale sotto mutex** resta rischio statico distinto. |
| 10. fd chiuso e riutilizzato | CONFERMATO come interleaving possibile | CONFERMATO come interleaving possibile | `game.c:251-260,375-378,410-413`; `server.c:198`. Nessuna consegna al destinatario sbagliato osservata. |
| 11. Overflow credenziali | CONFERMATO per client; server era dedotto | CONFERMATO per entrambi | ASan nuovo: `client.c:77` con nickname 1024 byte e `game.c:68` con record `users.txt` 1024 byte; entrambi `stack-buffer-overflow`. |
| 12. Protocollo nativo | CONFERMATO come rischio di portabilità | CONFERMATO come rischio di portabilità | `protocol.h:38-61`, `protocol.c:7-8,25-26`; `Lezione019_socket-2.pdf` pp. 9-11 controllate. Nessun fallimento provato fra due host. |
| 13. Esiti I/O e allocazione ignorati | CONFERMATO come percorso d'errore | CONFERMATO come percorso d'errore | `server.c:26-27,209-210`; `game.c:81-83,378,413,439,466`; `log.c:31-33`. Guasti disco/RAM non simulati. |
| 14. Riuso slot cancella uscito | CONFERMATO, in A1 solo statico | CONFERMATO, A2 lo prova in rete | `game.c:198-241,251-260`; nuovo test: admin uscito poi disconnesso, luigi riusa slot e vince senza essere uscito. L'interpretazione di T sui disconnessi va dichiarata; la perdita della storia è reale. |
| 15. Possibile doppia notifica finale | CONFERMATO come interleaving | CONFERMATO come interleaving | `server.c:123-129` ignora il valore di `termina_partita`; `game.c:309-334` può già inviare. Non riprodotto dinamicamente. |
| 16. README inesatto | CONFERMATO | CONFERMATO | `README.md:18,21,82-83,100-115,142-143,184,193` contro `game.c:44-48,61-90,223-241`, `client.c:75-81`, `protocol.h:28`. Uscite fisse e spawn interno non sono di per sé violazioni di T. |
| 17. Parsing argomenti/credenziali | CONFERMATO, gravità subordinata ai difetti principali | CONFERMATO | `server.c:146-162`; `client.c:28-39`; `game.c:61-89`. Nuovo `5200junk` passa la validazione e tenta la connessione. Nomi con spazi non sono coerenti con `fscanf("%s %s")`. |
| 18. IP non loggato prima del login | CONFERMATO come differenza dall'esempio | CONFERMATO come differenza dall'esempio | `server.c:194-207`; T:13 dice “ad esempio”; **non** è violazione testuale autonoma. |
| 19. Nessuno shutdown ordinato server | CONFERMATO come limite/scelta; **DA CORREGGERE** se presentato come obbligo | CONFERMATO nella gravità ridotta | `server.c:185-223`; il test senza client trova processo ancora vivo dopo il timeout. T:7 richiede termine partita, non chiusura processo. |
| 20. Flag `int` nell'handler | CONFERMATO come rilievo tecnico minore | CONFERMATO come rilievo tecnico minore | `client.c:16-21,132-146`; `volatile sig_atomic_t` è il tipo appropriato; nessun guasto di segnale osservato, nessuna regola docente specifica. |
| Stile: `read/write` anziché `recv/send` | CONFERMATO: nessuna violazione | CONFERMATO: nessuna violazione | `protocol.c:5-40`; `Lezione021_socket4.pdf` pp. 37-40 usa anche esempi di I/O completo, senza prescrivere un'unica coppia di chiamate. |
| Stile: `system("clear")` vietato/sgradito | NON DIMOSTRATO come preferenza docente | NON DIMOSTRATO come preferenza docente | `display.c:31-53`; `Lezione08_C-gcc.pdf` p. 14 mostra `system()` come funzione di libreria. Dipendenza da TERM è limite tecnico minore. |
| Stile: naming, numero di mutex, `fork` obbligatorio | NON DIMOSTRATO | NON DIMOSTRATO | Nessun obbligo in T o nelle pagine leggibili. `Lezione016_thread2.pdf` p. **10**, non p. 9, mostra un thread per client. |
| A1 checklist: “tutti attivi usciti” completamente OK | DA CORREGGERE | CONFERMATO nella correzione di A2 | `game.c:143-155,251-260`; la rimozione non rivaluta il fine e T:7 non definisce esplicitamente disconnessione/partecipazione. |

**Accordi verificati:** entrambi identificano correttamente i cinque difetti funzionali principali, l'overflow, i rischi di sincronizzazione/I/O e le inesattezze README. Il rischio di client lento da essi segnalato è ora riprodotto; i loro test brevi positivi rimangono corretti solo per la durata osservata. **Contraddizioni o revisioni fra audit:** A1 tratta come assenza di relazione autonoma il README, A2 distingue file separato da copertina/componenti; A1 cita erroneamente p. 9 di `Lezione016_thread2.pdf`, A2 corregge a p. 10; A1 aveva solo unità artificiale per il vincitore e deduzione per il riuso, A2 dichiara test di rete (entrambi ora ripetuti nel percorso essenziale). **Non più applicabile / risolto dopo modifica verificata:** nessun rilievo sul software: gli hash attuali coincidono con quelli pubblicati da A2 e con la stesura precedente. È cambiato soltanto il livello di prova del blocco per peer lento, non il codice. Non posso dimostrare bit a bit lo stato storico al momento di A1, che non pubblica un inventario hash completo.

## G. Test e forza delle prove

Ogni “PASS/FAIL” si riferisce all'atteso scritto nella riga. **Client/server reali** significa entrambi gli eseguibili del progetto; **harness TCP con server reale** sostituisce solo l'interfaccia client e usa il formato `Messaggio`; **unitario artificiale** prepara stato senza rete; **statico** è lettura del codice. Le prove nella tabella G.2 appartengono alla stesura precedente e restano pertinenti poiché i 14 hash non sono cambiati; non le presento come nuove esecuzioni.

### G.1 Verifiche aggiuntive del 25 ottobre 2026

Comandi eseguiti nella copia isolata: `wsl --exec sh -lc 'cd /mnt/c/Users/giova/AppData/Local/Temp/lso-labirinto-review-658ce781425b4a3985b642764126d53d && make'`; `wsl --exec python3 /mnt/c/Users/giova/AppData/Local/Temp/lso-labirinto-review-658ce781425b4a3985b642764126d53d/open_tests.py`; `wsl --exec python3 /mnt/c/Users/giova/AppData/Local/Temp/lso-labirinto-review-658ce781425b4a3985b642764126d53d/slow_variant.py`. Gli script hanno avviato `bin/server <porta_libera> <timeout> 1` in sottodirectory temporanee con `users.txt` proprio, usato socket su loopback, chiuso i socket e terminato i processi dopo una durata massima. Il server è quello compilato senza modifiche ai sorgenti. `SO_RCVBUF=1024` è stato applicato **solo al peer di prova** che non legge, per saturare il buffer in un tempo delimitato.

| Tipo; comando/modalità | Atteso | Ottenuto | Esito |
|---|---|---|---|
| Build documentata: `make` nella copia | Due binari senza errori/warning ai flag previsti | `bin/server` e `bin/client` compilati, nessun warning | **PASS** |
| Harness TCP, `open_tests.py` scenario `burst`: due connessioni (`admin`, `mario`), 400 richieste ciascuna, timer 7 s/intervallo 1 s | Frame leggibili, risposte e finale per entrambi nel caso delimitato | 400+400 richieste inviate; ciascuno ha ricevuto 266 mappe locali, 134 liste, 6 globali e 1 finale; nessun frame con tipo inatteso | **PASS nel caso provato**; non esclude interleaving raro o invii sovrapposti |
| Harness TCP, `open_tests.py` scenario `slow`: `admin` non legge dopo login (`SO_RCVBUF=1024`), 756 richieste lista, `mario` legge; timeout 10 s/intervallo 1 s, osservazione 11,07 s | `mario` riceve mappe ogni T e il finale; timeout viene registrato | `mario`: **0 mappe globali, 0 finali**; nel log nessuna scadenza fino alla chiusura del test; il processo restava vivo. L'invio del flood si è fermato per timeout del socket mittente | **FAIL** per invio periodico e liveness del timeout; blocco del timer osservato |
| Harness TCP, `slow_variant.py`: stesso peer lento, 843 richieste, client veloce invia `MSG_RICHIEDI_LISTA` a 3,5 s; timeout 6 s, osservazione 7 s | Servizio dei comandi e invii/timer indipendenti | Il veloce riceve `MSG_LISTA` a 3,503 s, ma **nessuna globale né finale**; log senza scadenza | **PASS** del solo thread comandi; **FAIL** di periodico/timeout. Il server non era interamente fermo |
| ThreadSanitizer su Linux compatibile | Diagnosi dinamica di race | Solo WSL Ubuntu disponibile; il precedente runtime falliva con `unexpected memory mapping`. Docker Desktop installato ma daemon Linux non attivo; nessuna condizione nuova compatibile | **NON ESEGUITO**; il fallimento storico del runtime non diagnostica P |
| Client e server reali su due macchine distinte; ABI/endian differenti | Connessione e interoperabilità | Nessuna seconda macchina accessibile; loopback e `sizeof(Messaggio)=3664` su questa ABI non sono prova di interoperabilità | **NON ESEGUITO / NON VERIFICABILE** |
| Riproduzione deterministica di fd riutilizzato, frame mescolati o doppia notifica | Evento di rete verificabile | Stress ordinario senza tipo inatteso; nessun caso deterministico osservato. Il codice mantiene le finestre di race indicate in C/H | **NON ESEGUITO** come riproduzione deterministica; rilievi statici confermati |

### G.2 Prove della stesura precedente, conservate per lo stesso identico software

I comandi e percorsi temporanei di questa tabella appartengono alla stesura precedente; quella directory di prova oggi è assente. Le prove con harness attraversano server e TCP, ma non l'interfaccia del client umano. Le prove ASan e l'unità artificiale sono qualificate come tali. Il test di 6 s con non lettore aveva mostrato buon esito **solo** nel suo intervallo; G.1 ha ora osservato il fallimento sotto saturazione maggiore.

| Tipo e modalità/comando | Atteso | Ottenuto | Esito |
|---|---|---|---|
| Build, `wsl make` nella copia | `bin/server`, `bin/client` | Entrambi compilati senza warning ai flag del Makefile | PASS |
| Harness TCP, `MSG_ACCESSO` nuovo `third_review_new/trialpass` | Rifiuto senza registrazione preventiva (T:11) | `MSG_OK`, riga aggiunta a `users.txt` temporaneo | **FAIL** |
| Harness TCP, prima `MSG_MAPPA_GLOBALE` | Una sola cella visibile | 20 celle, spawn (1,7) nel caso osservato | **FAIL** |
| Harness TCP, movimento e successiva globale | Posizione cambia, scoperte persistono | (1,7)→(1,6); celle visibili 20→24; nessuna vecchia cella torna `?` | PASS |
| Harness TCP, intervallo 1 s senza comandi successivi | Globale periodica autonoma | `MSG_MAPPA_GLOBALE` ricevuta nel caso provato | PASS |
| Harness TCP, password errata admin | `MSG_ERRORE` | Tipo 11, “Password errata.” | PASS |
| Harness TCP, timeout 5 s connessi | Finale | Tipo 15; vincitore riportato | PASS |
| Harness TCP, 65 accessi | 65 ammessi | 65 login OK | PASS |
| Harness TCP, `MSG_RICHIEDI_LISTA` con 65 connessi | 65 elencati | Lista 64, ultimo `tr3u063` | **FAIL** |
| Harness TCP, nessun uscito, admin/mario 0–0, timeout 2 s | Termine e scelta osservabile; parità non definita da T | Vincitore admin (primo slot); non prova giustizia del tie-break | PASS |
| Harness TCP, admin esce con mario dentro | Vince unico uscito | Uscita (0,20), vince admin | PASS |
| Harness TCP, admin esce/disconnette; luigi riusa slot; mario resta | Conservazione del risultato storico, se l'uscito resta partecipante | Login luigi OK, log indica slot 0, finale annuncia luigi; T non specifica il distacco, dunque vale la semantica esplicitata nell'atteso | **FAIL** |
| Harness TCP, percorrenza per trovare O, ritorno sulla cella | Oggetto incrementa una sola volta | 0→1; seconda entrata resta 1 | PASS |
| Harness TCP, due usciti 0/0 e luigi ancora dentro con 1 | Vince uno degli usciti (T:9) | Annunciato luigi, non uscito | **FAIL** |
| Harness TCP, client non lettore `SO_RCVBUF=1024`, altro lettore, 6 s | Secondo riceve aggiornamenti/finale nel tempo delimitato | 5 mappe e finale in 5,98 s; nessuna prova oltre 6 s | PASS **solo in quel test breve**; G.1 documenta il fallimento a saturazione maggiore |
| E2E client reale su PTY, `./bin/client 127.0.0.1 <porta>` | Prompt credenziali, login, lista, uscita | Prompt, login, lista e codice d'uscita 0; non prova tutti i comandi | PASS |
| Client reale, `./bin/client localhost 5200` | Nome host ammesso (T:14) | “Indirizzo IP non valido: localhost”, exit 1 | **FAIL** |
| Client reale, `./bin/client 127.0.0.1 5200junk` | Porta malformata rifiutata sintatticamente | Tenta `connect`, poi “Connection refused” | **FAIL** |
| ASan client, nickname di 1024 byte su stdin | Nessuna corruzione memoria | `stack-buffer-overflow` in `client.c:77`, exit 1 | **FAIL** |
| ASan server, `users.txt` temporaneo con campo 1024 byte | Nessuna corruzione memoria | `stack-buffer-overflow` in `game.c:68`, exit 1 | **FAIL** |
| Server reale senza client, timeout 2 s; lettura log | Fine senza vincitore; processo distinto dalla partita | Log datato: “Fine partita. Nessun vincitore.”; processo ancora vivo | PASS |
| Server reale, cattura stdout/stderr ordinari | Nessun output durante normale servizio (T:17) | 0 byte stdout, 0 byte stderr nella prova | PASS |
| TSan `gcc -fsanitize=thread` e server WSL con durata limitata | Diagnosi delle race | Compila, ma runtime: `FATAL: ThreadSanitizer: unexpected memory mapping` | **NON ESEGUITO** |
| Interleaving di fd riusato, scritture sovrapposte, doppio finale | Prova dinamica conclusiva | Nessuna riproduzione deterministica; solo codice analizzato | **NON ESEGUITO** |
| Client/server su due macchine fisiche o ABI differenti | Interoperabilità | Ambiente non disponibile | **NON ESEGUITO** |
| Hash e processi a fine prove | P invariato; nessun processo di prova | 14/14 hash iniziali uguali ai finali, nessun `bin/server` di prova attivo | PASS |

La prova “oggetto e seconda entrata” attraversa server e socket, ma l'interfaccia di rete è un harness: non prova da sola la resa del client umano. La prova con stato sintetico riportata dagli audit è utile per localizzare il ramo del vincitore, ma è **unitaria**, non end-to-end. Lo stress ordinario senza frame anomali non dimostra assenza di race. La prova G.1 del peer lento, invece, chiude **la questione di verifica del blocco del timer**: non corregge il software.

## H. Problemi confermati, gravità e natura

### H.1 Violazioni della traccia o risultato funzionale errato

1. **Alta — registrazione preventiva assente.** T:11 contro `P\server\game.c:autentica_utente:61-90`, `aggiungi_giocatore:177-188`; nuovo login ammesso e persistito. Va introdotto un passo di registrazione al sito e fatto rispettare dal login. T non sceglie il software del sito.
2. **Alta — classifica con più usciti errata.** T:9 contro `P\server\game.c:trova_vincitore_bloccato:115-140`; prova di rete con due usciti e un non uscito con un oggetto: vince il non uscito. È un errore funzionale indipendente dal tie-break.
3. **Alta — informazione di uscita cancellabile dal riuso slot.** `P\server\game.c:198-241,251-260`; test di rete conferma risultato sovrascritto. La traccia non formalizza il caso del distacco, dunque la qualificazione normativa resta legata alla lettura naturale che un uscito sia un partecipante alla classifica fino alla fine; la perdita del record è certa.
4. **Alta — mappa iniziale troppo rivelata.** T:5 contro `P\server\game.c:103-113,240-241`; 20 celle osservate, variabili fino a 25 secondo lo spawn.
5. **Alta — lista incompleta oltre 64 e nomi host rifiutati.** T:11,14; `P\common\protocol.h:22`, `game.c:426-439`, `client.c:51-60`; 65 connessi/64 elencati; `localhost` rifiutato.
6. **Media — copertina e componenti mancanti nella documentazione richiesta.** T:18-26; `P\README.md:1-197`. Non è dimostrato l'obbligo di un file distinto o il numero esatto di pagine.

### H.2 Rischi tecnici e difetti di robustezza

7. **Alta — overflow riprodotti.** `P\client\client.c:72-87` e `P\server\game.c:63-73` usano `%s` senza larghezza in buffer di 32 byte. ASan conferma entrambi. Il client ignora anche il valore di ritorno di `scanf`.
8. **Alta, statica — sincronizzazione incompleta.** `P\server\server.c:118` e `P\server\game.c:442-446` leggono stato globale fuori lock mentre `game.c:207-220,251-260,310-315` lo modifica. Il possibile puntatore d'array invalidato da `realloc` è un interleaving, non un crash osservato.
9. **Alta, statica — durata fd e serializzazione degli invii.** `P\server\game.c:251-260,375-378,410-413,449-468`; `P\common\protocol.c:10-19`. Chiusura/riuso tra snapshot e `write`, oltre a due writer sul medesimo stream, possono mescolare frame o inviare al destinatario errato. Non riprodotto.
10. **Alta, guasto osservato per il timer; rischio statico distinto per il finale sotto mutex.** `P\server\game.c:442-468` invia in sequenza con I/O bloccante, `P\common\protocol.c:10-19`; § G.1: peer non lettore con 756 richieste satura il socket e priva l'altro client di globali e finale oltre il timeout. Il timer non registra la scadenza. L'invio del finale sotto `g_gioco.mutex` resta un rischio ulteriore non riprodotto.
11. **Media, statica — possibile doppio finale.** `P\server\server.c:123-129` invia anche se `termina_partita()` torna 0; `game.c:309-334` può aver già notificato. Non osservato.
12. **Media, percorsi d'errore — ritorni ignorati.** `P\server\server.c:26-27,209-210` (`malloc` nullo dereferenziato); `game.c:81-83,378,413,439,466`; `log.c:31-33`. Errori di disco, rete o RAM non simulati, ma i rami non li trattano.
13. **Media — formato wire nativo.** `P\common\protocol.h:38-61`, `protocol.c:7-8,25-26` dipendono da dimensioni, padding ed endian. È rischio per T:29, non prova che due host Linux omogenei falliscano.
14. **Bassa/media — parsing e semantica non precisati.** `P\client\client.c:28-39`, `server.c:146-162` accettano suffissi numerici via `atoi`; `game.c:61-89` non valida forme compatibili con il file. Disconnessi non usciti restano `partecipante=1` e possono ancora concorrere nel ramo senza usciti (`game.c:130-139,251-260`): comportamento certo del codice, intenzione non definita da T.

### H.3 Errori documentali

Il README parla di **uscite random** e **otto spawn perimetrali** (`README.md:18,111-115`), ma il codice usa quattro uscite fisse e spawn interni random (`game.c:44-48,223-228`). Dice che dopo la connessione serve “solamente” il nickname (`README.md:100`), ma il client chiede anche password (`client.c:75-81`). Dice “nessun dato permanente” (`README.md:82-83`), ma `users.txt` è persistente (`game.c:77-83`). Nomina `MSG_QUIT` (`README.md:21`), mentre `protocol.h:28` definisce `MSG_ESCI`. La garanzia di assenza race grazie a “un unico mutex” (`README.md:184`) è smentita dalle letture fuori lock. Il “blocco” della lista descritto a `README.md:142-143` non è seguito da paginazione in `game.c:426-440`. Sono errori del documento, da correggere prima di usarlo come relazione; la posizione fissa delle uscite, da sola, non viola T.

### H.4 Preferenze stilistiche o scelte ammissibili

Nessuna correzione obbligatoria è fondata solo su `read/write` anziché `recv/send`, `system("clear")`, thread anziché `fork`, numero di mutex, naming o indentazione. `system("clear")` può dipendere dal terminale (`display.c:31-53`) ed è un limite di usabilità, non un divieto dimostrato del professore. La fine del gioco senza arresto automatico del server (`server.c:185-223`) non è una violazione testuale autonoma.

## I. Errori, eccessi e omissioni degli audit

- **A1, attribuzione errata:** l'architettura un thread per client è a `Lezione016_thread2.pdf` p. **10**; p. 9 contiene un'altra slide. A2 lo corregge. La citazione di A1 a `Lezione08_C-gcc.pdf` pp. 6–7 è troppo ampia per un'affermazione precisa sui warning: il testo utile è p. 6, p. 7 è quasi solo un titolo.
- **A1, gravità/formulazione eccessiva:** “relazione autonoma assente” aggiunge un requisito di file separato non scritto in T. La mancanza certa è copertina/componenti e l'inesattezza del README. A2 distingue correttamente questi aspetti.
- **Entrambi, numero della prima mappa:** 25 è risultato dei loro spawn, non misura universale. Questa prova ha osservato 20. La violazione “solo una cella” resta certa.
- **Qualità delle prove:** A1 non pretendeva propriamente che la sua unità sintetica fosse un test con client/server; in ogni riassunto va comunque marcata **unitaria artificiale**. A2 usa server reale con harness TCP per la classifica e il riuso: è una prova della logica via rete, ma non del client interattivo. I test brevi dei due audit e della stesura precedente non dimostrano assenza di stallo; G.1 ha ora osservato il blocco del timer sotto saturazione. Né A1, né A2, né la stesura precedente di questa revisione hanno osservato direttamente fd riusato, stream corrotto o doppio finale.
- **Contrasti chiariti:** A1 dà OK senza riserva a “tutti i giocatori attivi usciti”; A2 considera il distacco e la semantica incerta di T:7. A2 è più preciso. A1 classifica riuso slot come deduzione; A2 e questa revisione lo riproducono via rete. Non ho riscontrato differenze del progetto che spieghino divergenze: i 14 hash corrispondono a quelli di A2.
- **Omissioni importanti comuni:** il comportamento del timer sotto un peer lento era stato correttamente segnalato come rischio, ma non verificato: G.1 lo riproduce. Non ho trovato un ulteriore difetto indipendente dimostrabile con la stessa forza dei difetti principali. Un dettaglio non trattato esplicitamente è che un disconnesso non uscito resta `partecipante=1` e può essere selezionato al timeout senza usciti se ha il punteggio massimo (`game.c:130-139,251-260`); T non fissa se ciò sia scorretto. Inoltre `fgets(buffer,64)` nel client (`client.c:149-179`) può interpretare i pezzi successivi di una riga molto lunga come comandi separati: deduzione statica minore, non testata né elevata a violazione autonoma.
- **Attribuzioni del docente:** A1/A2 per lo più evitano di presentare `fork`, `recv/send`, `system("clear")` o più mutex come obblighi. Concordo. Le pagine raster sono state passate a OCR, ma quelle schematiche e i frammenti indicati in § B non consentono attribuzioni letterali affidabili; il PDF unificato non è stato controllato a vista pagina per pagina.

## J. Correzioni realmente necessarie prima della consegna

1. Fare sì che un account possa entrare **solo dopo** registrazione al sito con nickname/password, come T:11; testare sia utente registrato sia nome nuovo. La specifica non impone framework, database o stack web particolari.
2. Correggere la classifica per più usciti confrontando gli usciti e conservando i risultati della partita separatamente dagli slot riutilizzabili. Esplicitare nella documentazione come sono trattati distacchi e parità, senza attribuire alla traccia una regola che non enuncia.
3. Segnare inizialmente solo la cella dello spawn, poi rivelare l'area attorno ai movimenti; verificare prima mappa e mappe successive.
4. Trasmettere la lista completa per un numero di connessi superiore a 64 e risolvere i nomi host oltre agli IP numerici; verificare entrambi con client/server reali.
5. Eliminare gli overflow `scanf/fscanf`, validare lunghezze/formato delle credenziali e propagare gli errori di file/rete/allocazione che possono falsare accesso, log e notifica.
6. Sincronizzare ogni accesso allo stato condiviso, coordinare la vita dei fd e un solo writer per socket, **isolare il timer dai peer lenti** con una politica di invio limitata, evitare I/O potenzialmente indefinito sotto il mutex globale e fare inviare un solo finale. Il blocco del timer è ora riprodotto (§ G.1); gli altri interleaving restano non riprodotti.
7. Rendere la documentazione fedele all'implementazione e aggiungere copertina/componenti e sezioni richieste da T:18-26. Verificare il numero indicativo di pagine su un documento impaginato; non occorre inventare l'obbligo di un file separato.
8. Prima dell'orale provare davvero client e server su almeno due macchine (T:29). Se ABI/endian non coincidono, definire un formato wire portabile; anche su macchine omogenee occorre verificarne l'uso reale. L'esempio didattico non impone uno specifico codec.

Non considero necessarie prima della consegna modifiche per sole preferenze di stile, l'arresto automatico del server o la scelta di `recv/send` in luogo di `read/write`.

## K. Risposte esplicite

1. **Conformità alla traccia:** **NO**; T:5,9,11,14,26 hanno scostamenti dimostrati. La condizione di fine dopo disconnessione resta in parte ambigua, senza alterare questo verdetto.
2. **Funzionamento nei test:** **PARZIALE**; build, login esistente, movimento, raccolta/rimozione, mappe periodiche, timeout e uscita singola riescono nei casi eseguiti. Falliscono registrazione preventiva, prima mappa, lista >64, vincitore con più usciti, nome host e input lunghi con ASan; ora anche la periodicità e la scadenza sotto peer lento (§ G.1). Nessuna prova su due macchine o di assenza race.
3. **Aderenza ai metodi mostrati dal professore:** **PARZIALE**; GCC, thread per client, `select`, mutex e cicli I/O seguono esempi verificati nei PDF. Risoluzione nomi, protezione completa dello stato e controlli degli errori sono incompleti. Non è dimostrabile una preferenza obbligatoria su stile, numero mutex o `fork`.
4. **Necessità di correzioni:** **SÌ**, nell'ordine di § J.
5. **Prontezza per la consegna:** **NO**, nello stato degli hash in § B. L'esito non dipende da preferenze personali né da ipotetici guasti: bastano i requisiti testuali e i test di rete falliti.

## Aggiornamento delle verifiche — 25 ottobre 2026

**Differenze di hash:** nessuna, 14/14 file originali identici alla precedente stesura; non esiste una modifica del software da segnare RISOLTA. **Nuovi test:** build nella copia sotto `%TEMP%`; 400 richieste concorrenti per ciascuno di due client con ricezione integra nel caso delimitato; due scenari di saturazione di un peer non lettore. **Questione di verifica chiusa:** il peer lento può arrestare il timer e privare un altro client delle globali e del finale oltre la scadenza, pur lasciando operativo il thread che risponde alla lista. È un guasto osservato del progetto invariato, non una correzione. **Aperte:** TSan su Linux compatibile, due macchine/ABI differenti, riproduzione deterministica di fd riutilizzato, invii mescolati e doppio finale; la loro analisi statica resta valida. Le pagine raster elencate in § B non supportano attribuzioni letterali affidabili.

**Chiusura del perimetro:** nessun file originale modificato; 14/14 SHA-256 invariati, nessun binario o log in P e nessun processo di prova attivo. La directory temporanea usata in questa ripresa è rimasta fuori da P/M perché la rimozione è stata respinta dalla revisione automatica con `blocked by policy`; § B.1 elenca esattamente i file rimasti.