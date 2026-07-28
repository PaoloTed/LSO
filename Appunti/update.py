import re

file_path = "Guida_Completa_LSO.md"

with open(file_path, 'r', encoding='utf-8') as f:
    content = f.read()

# 1. Update the index
index_pattern = r"(17\. \[Socket .*?\]\(#17-socket--comunicazione-di-rete\))"
new_index_entries = r"""\1
18. [I/O Multiplexing - `select()`](#18-io-multiplexing---select)
19. [Gestione Avanzata dei Segnali - `sigaction()` e SIGPIPE](#19-gestione-avanzata-dei-segnali---sigaction-e-sigpipe)
20. [Broadcast e Multicast UDP](#20-broadcast-e-multicast-udp)
21. [Comandi di Rete e Risoluzione DNS](#21-comandi-di-rete-e-risoluzione-dns)
22. [Virtualizzazione](#22-virtualizzazione)
23. [Container: Namespace, Cgroups e OverlayFS](#23-container-namespace-cgroups-e-overlayfs)
24. [Docker](#24-docker)
25. [Soluzioni Esercizi d'Esame](#25-soluzioni-esercizi-desame)
26. [Guida Rapida alle Parole Chiave](#26-guida-rapida-alle-parole-chiave)"""

content = re.sub(index_pattern, new_index_entries, content, count=1)

# Split the content before Section 25 just in case it's there
parts = content.split("## 25. Soluzioni Esercizi d'Esame")
clean_content = parts[0]

appendix = r"""## 25. Soluzioni Esercizi d'Esame

Di seguito sono riportate le soluzioni agli esercizi d'esame mostrati nelle immagini, utili per verificare la propria preparazione e ripassare i concetti.

### Esercizio 1 (Pipeline Bash)
**Consegna**: Stampare il nome degli utenti che hanno almeno 2 processi attivi nello stato *sleeping* (STAT inizia per S) e avviati da meno di 90 secondi (ELAPSED < 90). Ogni utente deve comparire una sola volta.

**Soluzione**:
```bash
ps -eo user,stat,etimes | awk 'NR>1 && $2 ~ /^S/ && $3 < 90 {print $1}' | sort | uniq -c | awk '$1 >= 2 {print $2}'
```
**Spiegazione**:
1. `ps` produce la lista dei processi. `NR>1` salta l'intestazione stampata dal comando `ps`.
2. `awk` filtra le righe la cui seconda colonna (`$2`, STAT) inizia per `S` (tramite regex `/^S/`) e la cui terza colonna (`$3`, ELAPSED) è `< 90`. Per ogni riga valida, stampa il nome dell'utente (colonna 1).
3. `sort` ordina alfabeticamente i nomi degli utenti (necessario come passo preliminare prima di usare `uniq`).
4. `uniq -c` collassa i duplicati contando le occorrenze di ciascun utente (creando un output del tipo `   3 alice`).
5. L'ultimo `awk` filtra la lista numerata, stampando il nome dell'utente (seconda colonna) solo dove il conteggio (prima colonna) è `>= 2`.

### Esercizio 2 (Pipeline ls, grep, awk)
**Consegna**: Dato l'output di `ls -l`, estrarre i soli nomi dei file che: sono regolari, hanno estensione `.log`, hanno permesso di lettura per il gruppo, non hanno permesso di scrittura per altri e pesano più di 3000 byte.

**Soluzione**:
Usando unicamente `awk` applicato ai metadati:
```bash
ls -l | awk '/^-...r...[^w]/ && $5 > 3000 && $9 ~ /\.log$/ {print $9}'
```
In alternativa usando `grep` per le stringhe dei permessi come suggerito:
```bash
ls -l | grep '^-...r...[^w]' | awk '$5 > 3000 && $9 ~ /\.log$/ {print $9}'
```
**Spiegazione**:
* La Regex `^-...r...[^w]` verifica i permessi:
  * `^` indica l'inizio della riga.
  * `-` impone che sia un file regolare (esclude directory `d` o link `l`).
  * I 3 puntini `...` ignorano i permessi *rwx* del proprietario (posizioni 2-4).
  * La `r` si posiziona al quinto carattere (primo del gruppo), imponendo il permesso di lettura per il gruppo.
  * I 3 puntini successivi ignorano il resto del gruppo e il primo flag di lettura per gli *others* (posizioni 6-8).
  * `[^w]` al nono carattere impone che il permesso in scrittura per *others* **non** sia `w`.
* `$5 > 3000` in `awk` filtra la dimensione (quinta colonna in `ls -l`).
* `$9 ~ /\.log$/` in `awk` assicura che il nome del file (nona colonna) finisca in `.log`.

### Esercizio 3 (Thread e Sincronizzazione C)
**Frammento**:
Due thread `t1` e `t2` accedono e modificano variabili globali `x` e `y`. Il main genera un figlio tramite `fork()`, modifica localmente `x` e `y` e lancia in parallelo i due thread (sia nel processo padre che nel processo figlio).

**Risposte**:
a) **Quanti processi e thread vengono creati?**
Viene creato 1 nuovo processo figlio dalla `fork()`, per un totale di **2 processi** attivi (padre e figlio).
**Per ogni processo** (padre e figlio) vengono creati tramite `pthread_create` 2 nuovi thread (`t1` e `t2`). Tali 2 thread si sommano al thread base originario, risultando in 3 thread per processo. In sintesi, a livello applicativo vengono istanziati **4 nuovi thread** (escluso il main thread e includendo sia l'esecuzione nel padre che nel figlio).

b) **Il programma termina correttamente? Segnalare e correggere eventuali anomalie.**
Sì, ma presenta alcune anomalie di rilievo:
1. **Chiamata errata a `wait()`**: L'istruzione `wait(NULL);` viene eseguita indistintamente dal main di entrambi i processi. Poiché il figlio non ne possiede a sua volta, la sua invocazione a wait(NULL) fallirà subito ritornando il codice d'errore `ECHILD`.  
   *Correzione*: Racchiudere la wait in uno scope ristretto (es. `if (pid > 0) { wait(NULL); }`) oppure concludere il path d'esecuzione del processo figlio con una `exit(0)`.
2. **Race condition nel processo figlio**: il figlio inizializza `x=50` e `y=20`. Quando avvia `t1`, la condizione del loop `while (x <= y)` (50 <= 20) è immediatamente falsa: il thread elude l'attesa condizionale ed esegue subito la sottrazione `x = x - y`. Il thread `t2`, se non eseguito prima, imposterà incondizionatamente `x = y + 4` ignorando i calcoli precedenti. In assenza di vincoli temporali forzati via condition variable (come accade nel padre), il risultato finale di `x` varia (24 oppure 4) assecondando lo scheduler, configurando un evidente non-determinismo logico.

c) **Cosa stampa il programma?**
* **Nel padre**: la fork ha re-inviato `x = y` configurando `x=3` e `y=3`. `t1` si bloccherà nella condition wait (essendo `3 <= 3`). `t2` imposterà quindi `x = 3 + 4 = 7` inviando poi un segnale per sbloccare la wait. `t1` ricalcolerà il test (`7 <= 3` ora FALSO) ed uscirà dal loop facendo `x = 7 - 3 = 4`. L'ordine deterministico farà stampare esattamente: `4 3`.
* **Nel figlio**: a causa della race condition spiegata sopra, l'output non è deterministico e potrà casualmente stampare `24 20` (se `t1` termina prima che parta `t2`) oppure `4 20` (se `t2` esegue prima, sovrascritto poi dalla sottrazione di `t1`). L'ordine temporale generale tra la printf del padre e quella del figlio sarà inoltre misto.

### Esercizio 4 (Pipe, Fork, Select in C)
**Frammento**:
Il processo padre esegue delle `read()` via multiplexing attendendo byte che un figlio inietta in un costrutto pipe `p[2]`.

**Risposte**:
1. **Le tre `select()` del padre si bloccano oppure ritornano subito?**
La **prima** `select()` si **bloccherà** attendendo che i byte giungano nel buffer di rete o che il file descriptor attiguo venga chiuso. Le **successive**, dopo l'acquisizione dei primi due byte, ritorneranno invece **subito**, rilevando la permanenza del byte mancante in circolo nel kernel buffer o lo stato latente della socket già liberata dal lato in scrittura.
2. **Quanto valgono `n1`, `n2`, `n3`?**
Il figlio ha scritto la stringa "XYZ" per un totale di 3 byte.
- La 1ª `read(p[0], buff, 2)` legge una limitazione fissata a 2 byte ("XY"), popolando il buffer e restituendo il valore effettivo `n1 = 2`.
- La 2ª `read(p[0], buff, 2)` legge il residuo decurtato ad 1 byte ("Z"). `n2 = 1`.
- La 3ª `read(p[0], buff, 2)` esegue la lettura ma non rintraccia altri flussi per intercorsa chiusura lato figli, ricevendo per prassi **End-Of-File (EOF)** che su interi equivale a `n3 = 0`.
3. **Cosa indica il valore restituito dalla terza `read()`? Il programma termina correttamente?**
Il valore **0** esprime la fine del file, la condizione di tranciamento del canale per avvenuta operazione. Il programma si spegne correttamente stampando `n1=2 n2=1 n3=0`.
4. **C'è il rischio che il processo figlio rimanga temporaneamente in stato zombie?**
**Sì**. Il processo generato fa `exit(0)` rilasciando il core per chiudere il proprio ciclo biologico, ma la routine del suo parent logico continua ignorandolo sistematicamente poiché nel costrutto priva di alcuna chiamata alla the system-call `wait()`. Egli permane nell'albero in stato `Z` (zombie) fintanto che il padre stesso terminerà il suo eseguibile, delegandone la rimozione ad _Init_ all'uscita complessiva.

### Esercizio 5 (Namespace e Docker)
**Scenario**: Innesco ravvicinato di due isolatori bash mediante container da un SO denominato _MioPC_.

**Risposte**:
(a) **Quale PID avrà il processo bash visualizzato con ps -ef nel contenitore2?**
Il processo bash deterrà senza dubbio il **PID 1**. Ogni contenimento su _Docker_ sfrutta le feature del modulo **PID Namespace** a livello di Kernel, staccandolo percettivamente dalla numerazione standard dell'OS host per instaurarlo in cima a un suo sottoramo privato partendo proprio dal PID 1.
(b) **Quale valore restituirà il comando hostname?**
Visualizzerà la stringa **nodo2**. Passando flag `--hostname` si invoca il distaccamento del **UTS Namespace** del container, il cui obiettivo è slegare i dati nominali del domain dall'effettivo server.
(c) **Saranno presenti la directory e il file `/lavoro/info.txt` creati in precedenza nel `contenitore1`?**
**No**, assolutamente assenti. Ciò deriva dalla specificità intrinseca degli **Overlay File System**. I due sub-sistemi sono originati dall'identica `ubuntu` *Read-Only* ma appositamente instradati ciascuno nel proprio *layer* virtuale indipendente detto **upperdir** (leggibile e scrivibile separato ed effimero). Qualsiasi directory forgiata in uno sfocia in un suo file d'overlay e decade appena cancellato.

---

## 26. Guida Rapida alle Parole Chiave

Questo glossario funge da *cheat sheet* riassuntivo per l'esame e lo studio, contenente le parole chiave e i concetti fondamentali di LSO.

### Concetti Generali e SO
* **Kernel**: Nucleo del sistema operativo, gira in modalità privilegiata (kernel mode).
* **System Call**: Interfaccia tra user mode e kernel mode (es. `read`, `write`, `fork`). Richiede un'interruzione/TRAP (cambio di contesto).
* **POSIX**: Standard IEEE per uniformare le API dei sistemi operativi Unix-like.
* **Inode (i-node)**: Struttura dati del filesystem che memorizza i metadati di un file (permessi, proprietario, timestamp, puntatori ai blocchi).
* **Hard Link**: Riferimento diretto e indiscriminato allo stesso inode (possibile solo sullo stesso filesystem, no directory).
* **Symlink (Link Simbolico)**: File speciale che contiene il percorso verso un altro file.

### Shell, Comandi e Scripting
* **Pipeline (`|`)**: Collega lo standard output di un processo allo standard input del successivo.
* **Redirezione (`>`, `<`, `>>`, `2>`)**: Devia l'input/output o gli errori da/verso file.
* **Grep**: Comando per cercare pattern logici testuali usando Espressioni Regolari (BRE o ERE con `egrep`).
* **Awk**: Linguaggio di elaborazione testuale per record/campi separati (variabili: `$1, $2, ...`, `NR`, `NF`).
* **Sed**: Stream editor per operare su stream testuali senza interazione (es. `s/old/new/g`).
* **Shebang (`#!/bin/bash`)**: Prima riga di uno script usata per specificare l'interprete al kernel.

### Processi e Segnali
* **PID / PPID**: Process ID (identificativo univoco) e Parent Process ID (PID del padre).
* **Processo Zombie (`Z`)**: Processo terminato ma il cui padre non ha ancora recuperato l'exit status tramite `wait()`.
* **Processo Orfano**: Processo il cui padre è terminato; viene solitamente adottato da `init` (PID 1).
* **fork()**: System call che clona integralmente il processo corrente in un processo figlio distinto.
* **wait() / waitpid()**: System call per attendere la terminazione di un figlio (scongiurando gli zombie) e leggerne l'exit status.
* **exec()**: Famiglia di funzioni che sostituisce l'immagine del processo chiamante con un nuovo eseguibile.
* **Segnale (Signal)**: Notifica software asincrona inviata a un processo (es. `SIGINT`, `SIGKILL`, `SIGPIPE`).
* **sigaction()**: API robusta per registrare custom handler dei segnali, sostituendo lo standard `signal()`.

### Thread e Sincronizzazione (Pthreads)
* **Thread**: Flusso di esecuzione leggero allocato in un processo (condividono PID, memoria logica, file descriptor).
* **Race Condition**: Anomalia scaturita quando l'ordine non deterministico di esecuzione di task concorrenti corrompe un dato condiviso.
* **Mutex (`pthread_mutex_t`)**: Lucchetto software di lock/unlock atto a garantire la mutua esclusione nelle sezioni critiche.
* **Condition Variable (`pthread_cond_t`)**: Costrutto usato per far attendere passivamente un thread finché una specifica logica applicativa diventa vera; lavora costantemente unita a un mutex.
* **Deadlock**: Stallo logico bloccante ove due o più thread si attendono a vicenda e perpetuamente, incrociando i Mutex.

### IPC (Inter-Process Communication)
* **Pipe (`pipe()`)**: Canale di comunicazione unidirezionale e anonimo tra processi imparentati (byte-stream).
* **FIFO (Named Pipe)**: Pipe persistente dotata di una path visibile nel filesystem (creata con `mkfifo`), utilizzabile anche da processi non parenti.
* **mmap()**: System Call usata per mappare file (o memorie anonime) direttamente nello spazio d'indirizzamento virtuale del processo bypassando le classiche `read`/`write`.
* **Memoria Condivisa**: Area di RAM fisicamente unica ma intercettabile da spazi d'indirizzi virtuali di più processi per comunicazione ad altissime prestazioni.

### Reti e Socket
* **Socket**: Endpoint bidirezionale per comunicare all'interno del PC locale (`AF_UNIX`) o nella rete IP (`AF_INET`/`AF_INET6`).
* **TCP (`SOCK_STREAM`)**: Protocollo per connessioni affidabili e basato sul riassemblaggio ordinato di byte-stream (Lati Server: `socket`, `bind`, `listen`, `accept`).
* **UDP (`SOCK_DGRAM`)**: Trasferimento senza connessione mediante datagrammi. Veloce ma non affidabile. (Lati Server: `socket`, `bind`, `recvfrom`).
* **I/O Multiplexing / select()**: Meccanismo che abilita un singolo thread a gestire numerosi file descriptor contemporaneamente; il thread attende finché almeno una risorsa risulta "Ready".
* **Broadcast**: Invio UDP limitato a IPv4 che instrada forzosamente a tutti gli IP della LAN sub-rete (`255.255.255.255`).
* **Multicast**: Distribuzione UDP selettiva inviata a un gruppo (`224.x.x.x`), gestita dai Router con `IGMP/PIM`.
* **getaddrinfo()**: Funzione moderna, sicura ed agnostica per tradurre domain-names (DNS) negli specifici ip (IPv4 o IPv6).

### Virtualizzazione e Container
* **Hypervisor / VMM**: Engine di gestione di macchine virtuali; può essere di **Type 1** (installato nativamente sull'Hardware) o **Type 2** (gestito via app sull'OS Host).
* **Container**: Tecnologia di isolamento OS-Level che garantisce ambienti logicamente autonomi ma accomunati e limitati dallo stesso identico Kernel Host, senza pesanti Virtual Machine hardware-assisted.
* **Namespace**: Sei/sette feature del Kernel Linux (es. *PID, NET, UTS, MNT*) in grado di occultare ad un gruppo di processi le effettive risorse globali (isolando così cosa esso "Vede").
* **Cgroups (Control Groups)**: Sotto-Sistema di Linux volto a contingentarne le capienze (limitare RAM, soglie cicli CPU, massimali I/O disco).
* **OverlayFS**: Union file system stratificato. Genera il paradigma dei Container disponendo l'immagine originaria Docker in un layer *ReadOnly* (`lowerdir`) ponendovi sopra lo strato transitorio *Writable* in cui salvare modifiche (`upperdir`).
* **Docker**: Piattaforma standard per confezionare ed eseguire container; automatizza tramite `Dockerfile` le build e coordina networking nativo e persistenza via `Volumes`.
"""

final_content = clean_content.rstrip() + "\n\n---\n\n" + appendix

with open(file_path, 'w', encoding='utf-8') as f:
    f.write(final_content)

print("Done successfully")
