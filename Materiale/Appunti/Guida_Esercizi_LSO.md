# Guida alla risoluzione degli esercizi LSO

> **Analisi di `Guida_Completa_LSO.md` e creazione di esercizi simili alle prove d'esame**
> Corso di Laurea in Informatica — Laboratorio di Sistemi Operativi
> A.A. 2025-2026 — Prof. Alberto Finzi

---

## 0. Come usare questa guida

Questo documento è diviso in tre parti:

1. **Analisi del documento originale** (`Guida_Completa_LSO.md`, 8.131 righe, 26 capitoli) e del corpus d'esame contenuto nel §25.
2. **Metodo di risoluzione** per ciascuna delle cinque famiglie di esercizi che ricorrono nelle prove.
3. **Nuovi esercizi**, simili per struttura e difficoltà a quelli d'esame, con soluzione, spiegazione e trappole.

Le soluzioni sono pensate per **GNU/Linux + Bash + gcc/pthread**; le pipeline usano `ps`, `awk`, `sort`, `uniq`, `grep`, `sed`. I nomi dei file negli esempi `ls -l` non contengono spazi: il caso con spazi è discusso come trappola.

**Avvertenza importante:** la guida originale è molto utile, ma contiene almeno una soluzione ufficiale sbagliata (Compito A, Esercizio 2) e qualche imprecisione. Le correzioni sono nella §1.6 e sono usate in tutto il resto del documento.

---

# PARTE I — Analisi del documento e delle prove

## 1. Che cosa contiene la guida

### 1.1 Scheda sintetica

| Voce | Valore |
|---|---|
| File analizzato | `Guida_Completa_LSO.md` (copia nella root del workspace) |
| Lunghezza | 8.131 righe, circa 329 KB |
| Capitoli | 27 sezioni numerate 0–26, più indice dettagliato |
| Copia alternativa | `Materiale/Appunti/Guida_Completa_LSO.md` (8.139 righe): versione annotata, leggermente diversa |
| Argomenti | Bash, grep/sed/awk, C e system call, processi/segnali/IPC, thread e sincronizzazione, socket/select, virtualizzazione/container/Docker, corpus d'esame |
| Corpus pratico | §25: 3 file `_ERR`, 5 template C `_TODO`, 3 script Bash, 1 simulazione PA, 2 compiti ufficiali A/B |
| Prerequisiti | C, puntatori, compilazione, shell Unix di base |

### 1.2 Mappa dei contenuti

| Capitoli | Contenuto | Uso tipico in prova |
|---|---|---|
| 0 | Informazioni sul corso, modalità d'esame, ambiente | Contesto |
| 1–2 | Sistemi operativi, kernel, file system, inode, permessi | Teoria e basi per `ls -l` |
| 3–4 | Bash, comandi Unix, redirezioni, pipe, `ps` | Pipeline |
| 5 | `grep`, espressioni regolari | Filtri |
| 6–8 | Script Bash, `sed`, `awk`, funzioni | Pipeline e script |
| 9–10 | GCC, I/O a basso livello, file descriptor | Esercizi C |
| 11 | Processi, `fork`, `exec`, `wait`, zombie/orfani | Traccia C |
| 12 | Segnali, `SIGPIPE`, `SIGCHLD`, `sigaction` | Pipe/socket |
| 13 | Pipe, FIFO, `mmap` | Esercizio pipe |
| 14–16 | Thread POSIX, mutex, condition variable, problemi classici | Esercizio thread |
| 17–21 | Socket TCP/UDP, `select`, `SIGPIPE`, multicast, DNS | Teoria ed esempi |
| 22–24 | Virtualizzazione, namespace, cgroup, OverlayFS, Docker | Esercizio container |
| 25 | Corpus d'esame e soluzioni | **Il più importante per la prova** |
| 26 | Glossario/cheat sheet | Ripasso |

### 1.3 Il corpus d'esame del §25

| Sezione | Contenuto | Tipo di prova |
|---|---|---|
| 25.1 | `es_cond_wait_if_ERR.c`, `es2_2_ERR.c`, `es_client_merge_ERR.c` | Bug-hunting |
| 25.2 | 5 template C (`cascata`, `pipe_select`, `chat_multi`, `whois`, `condvar`) + 3 script Bash | Completamento codice |
| 25.3 | Simulazione Prova Parziale: 5 quesiti | Pipeline, `ls`, sezione critica, pipe/select, fork+thread |
| 25.4 | Compito A: 5 esercizi | Pipeline tempo, `ls` permessi, fork/thread, pipe/select, Docker |
| 25.5 | Compito B: 5 esercizi | Pipeline stato, `ls` dimensione, fork/thread, pipe/EOF, Docker |

### 1.4 Struttura ricorrente delle prove

Le due tracce ufficiali hanno **la stessa struttura in cinque esercizi**:

| # | Famiglia | Competenza valutata | Tempo consigliato |
|---|---|---|---|
| 1 | Pipeline Bash con `ps`, `awk`, `sort`, `uniq` | Selezione righe, estrazione colonne, conteggio, deduplica | 10–15 min |
| 2 | Filtri su `ls -l` | Lettura della stringa permessi, regex, campi, dimensione | 10–15 min |
| 3 | Processi + thread + sincronizzazione in C | Contare processi/thread, simulare output, trovare deadlock | 25–30 min |
| 4 | Pipe + `fork` + `select` | FD, EOF, read parziali, zombie, SIGPIPE | 20–25 min |
| 5 | Namespace e container Docker | Isolamento, PID 1, hostname, OverlayFS, volumi | 10–15 min |

**Totale indicativo:** 75–100 minuti, più il tempo di rilettura. La guida non riporta una durata ufficiale: verificare sempre il regolamento dell'appello.

### 1.5 Cosa vuole vedere il docente

- **Precisione sintattica**: una pipeline che funziona ma sbaglia una colonna non prende punteggio pieno.
- **Motivazione**: non basta scrivere il comando; bisogna spiegare perché `sort` precede `uniq`, perché serve `while` e non `if`, perché `select` ritorna su EOF.
- **Modello mentale**: albero dei processi, thread per processo, tabella dei file descriptor, strati OverlayFS.
- **Capacità di trovare il bug**: la prova `_ERR` chiede di indicare *riga*, *causa* e *correzione*.
- **Attenzione ai casi limite**: `ECHILD`, zombie, nomi con spazi, header di `ps`, `SIGPIPE`.

### 1.6 Punti di forza del documento

- Copertura ampia e ordinata: dalla shell ai container.
- Esempi di codice completi e commentati.
- Il §25 è un vero archivio di prove svolte, non solo teoria.
- Le spiegazioni dei casi di deadlock e di `select`/EOF sono generalmente corrette.
- Il glossario finale è utile per il ripasso rapido.

### 1.7 Errori e imprecisioni verificate

Questa sezione è importante: **non copiare alla cieca le soluzioni della guida**.

#### Verifica rapida delle soluzioni ufficiali

| Prova | Esercizio | Esito | Nota |
|---|---|---|---|
| Compito A | A1 pipeline `etimes` | ✅ | Corretta |
| Compito A | A2 filtri `ls -l` | ❌ | Regex dei permessi sbagliata: vedere (1) |
| Compito A | A3 fork/thread | ✅ | Corretta; `wait` nel figlio genera `ECHILD` |
| Compito A | A4 pipe/select | ✅ | Corretta |
| Compito A | A5 Docker | ✅ | Corretta |
| Compito B | B1 pipeline `stat` | ✅ | Corretta |
| Compito B | B2 filtri `ls -l` | ✅ | Corretta con nomi semplici |
| Compito B | B3 fork/thread | ✅ | Corretta |
| Compito B | B4 pipe/EOF | ✅ | Corretta |
| Compito B | B5 Docker | ✅ | Corretta |
| Simulazione | Q1–Q5 | ✅ | Q4: aggiungere `wait` per i figli |

#### (1) Compito A, Esercizio 2: regex dei permessi sbagliata

La guida propone:

```bash
ls -l | grep '^-..w..[^w]..[^w]' | grep '\.sh$' | awk '{print $9}'
```

e spiega che il permesso di scrittura del proprietario sarebbe il **quarto** carattere della terna. Non è così.

Nella stringa `ls -l`:

```text
- r w x r - x r - x
1 2 3 4 5 6 7 8 9 10
```

- posizione 1: tipo (`-` file regolare, `d` directory, `l` link);
- posizioni 2–4: permessi **user** (`r`, `w`, `x`);
- posizioni 5–7: permessi **group**;
- posizioni 8–10: permessi **other**.

Quindi **write** è la seconda posizione di ogni terna: **3, 6, 9**. La regex della guida controlla `w` in posizione 4 (execute dell'owner) e `[^w]` in posizione 7 e 10 (execute di group/other), non le write.

Controprova: il file `script.sh` con permessi `-rwxr-xr-x` soddisfa “file regolare, `.sh`, solo il proprietario può scrivere”, ma la regex della guida **non lo trova**; la regex corretta sì.

**Regex corretta per “solo il proprietario può scrivere”** (owner `w`, group e other senza `w`):

```regex
^-.w..[^w]..[^w].
```

Spiegazione: `^-` tipo; `.` pos2; `w` pos3; `.` pos4; `.` pos5; `[^w]` pos6; `.` pos7; `.` pos8; `[^w]` pos9; `.` pos10.

La soluzione corretta dell'esercizio diventa:

```bash
ls -l | awk '$1 ~ /^-.w..[^w]..[^w]./ && $9 ~ /\.sh$/ {print $9}'
```

#### (2) Script Bash 2: commento `//` dentro `awk`

Nel §25.2.6, lo script `es_file_bash2_TODO.sh` contiene:

```awk
count[pid]++; // Incrementa il numero di LWP per questo PID
```

In `awk` il commento è `#`, non `//`. `//` è una regex vuota e il testo successivo può causare un errore di sintassi. Correzione:

```awk
count[pid]++   # Incrementa il numero di LWP per questo PID
```

#### (3) `whois_TODO.c`: uso di `argv[1]` quando manca l'argomento

```c
if (argc < 2) {
    fprintf(stderr, "Uso: %s <dominio/IP> [server_whois]\n", argv[1]);
    return 1;
}
```

Se `argc < 2`, `argv[1]` è `NULL`: stamparlo con `%s` è comportamento indefinito. Va usato `argv[0]`.

#### (4) `ls -l` e nomi con spazi

Tutte le soluzioni usano `$9` come nome del file. Funziona con i nomi semplici delle tracce, ma **non** con nomi che contengono spazi: in quel caso il nome occupa più campi e `$9` stampa solo la prima parola. Per una soluzione robusta usare `find`, `stat` o estrarre il resto della riga. Nella prova, verificare se i nomi possono contenere spazi.

#### (5) `wait(NULL)` nel figlio

Nel Compito A/B la `wait(NULL)` è fuori dal ramo del padre: anche il figlio la esegue e riceve `-1` con `errno = ECHILD`. La guida lo nota correttamente, ma è utile ribadire che **non è un errore fatale** e non blocca il figlio: la chiamata ritorna subito. La correzione è spostarla nel ramo del padre o usare `if (pid > 0) wait(NULL);`.

#### (6) “Thread” e “LWP”: contare il main thread

Quando la traccia chiede “quanti thread”, bisogna contare anche il **thread principale** di ogni processo. In Linux ogni thread è un LWP. Esempio: padre con `main` + 2 thread creati = 3 thread; figlio senza `pthread_create` = 1 thread; totale 4, non 2.

#### (7) `Quesito 4`: figli non raccolti

Nella soluzione con due figli che scrivono sulle pipe, il padre chiude le pipe e termina senza `wait()`. I figli possono diventare zombie finché il padre resta vivo; alla terminazione del padre vengono adottati. In una soluzione completa aggiungere `wait(NULL)` per ciascun figlio o gestire `SIGCHLD`.

#### (8) Dettagli minori

- Nel Compito B, Esercizio 3 compare un refuso `$ o$` nella spiegazione: è un errore di battitura, non di logica.
- Nel Compito B, Esercizio 2, una riga di esempio ha una stringa permessi di 11 caratteri (`-rw-r---rw-`): è un refuso; le stringhe `ls -l` corrette sono di 10 caratteri.
- `sed -i` è GNU sed: su Linux va bene, su macOS richiede `sed -i ''`.
- `inet_ntoa` restituisce un buffer statico: è accettabile in un programma single-thread, ma non è thread-safe.

### 1.8 Come studiare la guida in una settimana

| Giorno | Attività |
|---|---|
| 1 | Capitoli 3–8: Bash, grep, sed, awk. Rifare tutte le pipeline del §25.3–25.5 senza guardare. |
| 2 | Capitoli 10–13: file descriptor, processi, `fork`, `wait`, pipe, segnali. Disegnare alberi e tabelle FD. |
| 3 | Capitoli 14–16: thread, mutex, condition variable. Risolvere i tre `_ERR` e i template `cascata`/`condvar`. |
| 4 | Capitoli 17–19: socket e `select`; rifare gli esercizi pipe/select A4/B4/Q4. |
| 5 | Capitoli 22–24: namespace, cgroup, OverlayFS, Docker; riscrivere le risposte A5/B5 a memoria. |
| 6 | Simulazione completa A e B in tempo, senza appunti. |
| 7 | Correzione, glossario §26, ripasso degli errori personali. |

---

## 2. Metodo generale per la prova

### 2.1 Cinque passi

1. **Leggere tutti e cinque gli esercizi** prima di iniziare: alcuni sono più rapidi e conviene prendere i punti facili.
2. **Classificare** ogni esercizio: pipeline, `ls`, C, pipe/select, container.
3. **Scrivere lo scheletro** della risposta: comando/pipeline, oppure struttura del programma, oppure elenco di risposte a–b–c.
4. **Verificare i casi limite**: header, righe non numeriche, EOF, `ECHILD`, zombie, PID 1.
5. **Rileggere** chiedendosi: “la mia spiegazione dimostra che ho capito, o solo che ho copiato una formula?”

### 2.2 Budget del tempo

| Attività | Minuti |
|---|---|
| Lettura e pianificazione | 5 |
| Esercizio 1 (pipeline) | 10 |
| Esercizio 2 (`ls`) | 10 |
| Esercizio 3 (C) | 30 |
| Esercizio 4 (pipe/select) | 20 |
| Esercizio 5 (container) | 10 |
| Rilettura | 10 |

Se un esercizio C non viene, scrivere comunque: conteggio processi/thread, schema delle variabili, bug probabile, correzione parziale. I punti parziali contano.

### 2.3 Formato di risposta consigliato

Per ogni esercizio:

```text
1. Interpretazione: cosa chiede esattamente la traccia.
2. Assunzioni: formato dei dati, header, nomi senza spazi.
3. Svolgimento: pipeline/ codice / tabella.
4. Risultato: output o valori.
5. Verifica: perché funziona e quali casi limite ho considerato.
```

---

# PARTE II — Metodo per famiglia di esercizi

## 3. Pipeline Bash

### 3.1 Modello mentale

Una pipeline è una catena di trasformazioni:

```text
sorgente  →  filtro righe  →  estrazione chiave  →  ordinamento  →  conteggio  →  filtro finale
ps ...       awk 'cond'       awk '{print $1}'      sort            uniq -c        awk '$1>=N'
```

L'errore più frequente è **saltare `sort` prima di `uniq`**: `uniq` elimina/ conta solo righe **adiacenti** uguali.

### 3.2 Colonne di `ps` utili

| Campo | Significato | Esempio |
|---|---|---|
| `user` / `USER` | proprietario | `alice` |
| `stat` / `STAT` | stato | `S`, `Ss`, `Sl`, `R`, `Z` |
| `etimes` / `ELAPSED` | secondi dall'avvio | `45` |
| `nlwp` / `NLWP` | numero di thread (LWP) | `3` |
| `tty` / `TT` | terminale | `pts/1`, `?` |
| `pid` / `PID` | PID | `1234` |
| `comm` / `CMD` | nome comando | `bash` |

Per sopprimere l'header si può usare `--no-headers` oppure la sintassi con `=`:

```bash
ps -eo user=,stat=,etimes=
```

### 3.2.1 Cosa significa esattamente `ps -eo`

`ps -eo` **non è una singola opzione**: è la combinazione di due opzioni.

| Pezzo | Nome | Significato |
|---|---|---|
| `ps` | process status | mostra una fotografia dei processi in quel momento |
| `-e` | every process | seleziona **tutti** i processi del sistema (equivale a `-A`) |
| `-o formato` | output format | definisce **quali colonne** stampare, nell'ordine indicato |
| `user,stat,etimes` | formato | nome utente, stato, secondi trascorsi dall'avvio |

Quindi:

```bash
ps -eo user,stat,etimes
```

è equivalente a:

```bash
ps -e -o user,stat,etimes
```

Il significato è: **“elenca tutti i processi e, per ciascuno, mostra solo le colonne user, stat ed etimes, in questo ordine”**.

#### Perché si scrive tutto attaccato

Nella forma compatta `-eo`, la `e` e la `o` sono raggruppate, ma `-o` continua a pretendere un argomento: il formato. Il formato è **un'unica parola** (senza spazi) passata come argomento successivo.

```bash
ps -eo user,stat,etimes       # corretto: un solo argomento di formato
ps -eo user, stat, etimes     # sbagliato: "stat," ed "etimes" diventano altri argomenti
ps -eo                        # errore: manca l'argomento di -o
```

Le virgole separano i campi; gli spazi no. Se si vogliono colonne con nomi personalizzati o header soppressi, si usa `=`:

```bash
ps -eo user=,stat=,etimes=        # nessun header
ps -eo user=UTENTE,stat=,etimes=  # header personalizzato (procps-ng)
ps -eo user,stat,etimes --no-headers  # alternativa: sopprime tutti gli header
```

#### Come si legge l'output

```text
$ ps -eo user=,stat=,etimes=
root     Ss   400
alice    S     45
bob      R     12
```

Ogni riga è un processo; le colonne diventano i campi di `awk`:

- `$1` = utente;
- `$2` = stato;
- `$3` = secondi dall'avvio.

Per questo, nelle pipeline d'esame, `ps -eo ...` si usa quasi sempre con `=` o `--no-headers`: altrimenti la prima riga è l'header e va scartata con `NR > 1`.

#### `ps -eo` vs `ps -ef` vs `ps aux`

| Comando | Cosa mostra |
|---|---|
| `ps -ef` | tutti i processi, formato “full” standard: UID, PID, PPID, C, STIME, TTY, TIME, CMD |
| `ps aux` | sintassi BSD: utente, PID, %CPU, %MEM, VSZ, RSS, TTY, STAT, START, TIME, COMMAND |
| `ps -eo ...` | tutti i processi, **colonne scelte da noi**, nell'ordine scelto |

Per una pipeline con `awk`, `-eo` è preferibile perché i campi sono fissi e noti. Con `ps aux` o `ps -ef` il comando finale può contenere spazi e spostare le colonne, rendendo `$9` o `$11` fragili.

#### `-e`, `-A`, `-a`, `-x`: non confonderli

| Opzione | Significato |
|---|---|
| `-e` | tutti i processi (stesso effetto di `-A`) |
| `-A` | tutti i processi |
| `-a` | processi con terminale, tranne i session leader |
| `-x` | include anche i processi senza terminale (sintassi BSD) |
| `-u utente` | solo i processi di un utente |
| `-L` | mostra un thread (LWP) per riga |

Nelle tracce d'esame `-e` è la scelta standard quando serve “tutti gli utenti”. `ps -L -u "$USER"` serve invece quando si contano i thread di un utente.

#### Campi utili di `-o`

I nomi dei campi sono quelli di `ps` (non quelli dell'header). I più usati:

| Campo | Significato |
|---|---|
| `user` | username del proprietario |
| `uid` | UID numerico |
| `pid` | PID |
| `ppid` | PID del padre |
| `stat` | stato del processo |
| `etimes` | secondi trascorsi dall'avvio |
| `nlwp` | numero di thread (LWP) |
| `tty` | terminale associato |
| `comm` | nome del comando |
| `args` | riga di comando completa (può contenere spazi) |

L'ordine in `-o` determina l'ordine dei campi in `awk`: se scrivo `ps -eo stat,user`, allora `$1` è lo stato e `$2` è l'utente. **Non c'è un ordine “standard”**: l'ordine è quello che ho scritto.

#### In sintesi

`ps -eo user,stat,etimes` significa:

> “Fai una fotografia di **tutti** i processi (`-e`) e stampa per ognuno **solo** le colonne `user`, `stat` ed `etimes` (`-o`), in quest'ordine.”

È il mattone di partenza delle pipeline d'esame; poi `awk` filtra e `sort | uniq -c` conta.

### 3.3 Ricetta universale

```bash
ps -eo CAMPO1=,CAMPO2=,CAMPO3= \
  | awk 'CONDIZIONE {print $CHIAVE}' \
  | sort \
  | uniq -c \
  | awk '$1 >= SOGLIA {print $2}'
```

Passo per passo:

1. **Sorgente**: `ps -eo ...` con i campi esatti richiesti.
2. **Filtro**: `awk` con confronti numerici (`$2 < 90`) o regex (`$2 ~ /^S/`).
3. **Chiave**: stampare solo la colonna che identifica l'utente/processo.
4. **`sort`**: indispensabile prima di `uniq`.
5. **`uniq -c`**: produce `conteggio chiave`.
6. **Filtro finale**: `awk '$1 >= 2 {print $2}'`.

### 3.4 Esempi dalle tracce ufficiali

**Compito A, Esercizio 1** — utenti con almeno 2 processi avviati da meno di 20 secondi:

```bash
ps -eo user,etimes --no-headers | awk '$2 < 20 {print $1}' | sort | uniq -c | awk '$1 >= 2 {print $2}'
```

**Compito B, Esercizio 1** — utenti con almeno 2 processi in stato `S` e `etimes < 90`:

```bash
ps -eo user=,stat=,etimes= | awk '$2 ~ /^S/ && $3 < 90 {print $1}' | sort | uniq -c | awk '$1 >= 2 {print $2}'
```

### 3.5 Varianti utili

- **Solo valori distinti**: `... | sort -u`.
- **Conteggio senza `uniq`**: `awk '{c[$1]++} END {for (k in c) ...}'`.
- **Ordinare per conteggio**: `... | sort -k2,2nr`.
- **Escludere header**: `NR > 1` o `--no-headers`.
- **Stato che inizia per S**: `$2 ~ /^S/` (copre `S`, `Ss`, `Sl`, `S+`).
- **Conteggio di thread per processo**: `ps -L -u "$USER" -o pid,lwp --no-headers | awk '{c[$1]++} END {...}'`.

### 3.6 Trappole tipiche

| Trappola | Correzione |
|---|---|
| `uniq` senza `sort` | anteporre `sort` |
| Confronto stringa vs numero | in `awk` usare `$2 + 0 < 90` se necessario |
| Header interpretato come dato | `NR > 1` o `--no-headers` |
| `uniq -c` output con spazi iniziali | `$1` è comunque il conteggio; `$2` la chiave |
| Contare processi invece di thread | `nlwp`/`ps -L`, non `ps -e` |
| Regex dello stato troppo stretta | `^S` non `S` (gli stati possono essere `Ss`, `Sl`) |
| `grep` su più condizioni | `awk` con `&&` è più leggibile |

### 3.7 Checklist

- [ ] Ho usato i campi `ps` esatti della traccia?
- [ ] Ho soppresso l'header?
- [ ] Ho filtrato prima di contare?
- [ ] Ho messo `sort` prima di `uniq`?
- [ ] Ho stampato una sola riga per utente?
- [ ] Ho verificato con un esempio numerico?

---

## 4. Filtri su `ls -l`

### 4.1 Campi di `ls -l`

```text
-rwxr-xr-x 1 student student 4100 Jul 15 10:15 avvio.sh
$1         $2 $3      $4      $5   $6 $7  $8    $9
```

| Campo | Significato |
|---|---|
| `$1` | tipo + permessi (10 caratteri) |
| `$2` | numero di hard link |
| `$3` | proprietario |
| `$4` | gruppo |
| `$5` | dimensione in byte |
| `$6`–`$8` | data/ora |
| `$9` | nome (solo se non contiene spazi) |

### 4.2 Posizioni nella stringa permessi

```text
Posizione:  1  2  3  4  5  6  7  8  9  10
Significato: t  r  w  x  r  w  x  r  w  x
             ^  ^-----^  ^-----^  ^-----^
             |   user      group     other
             tipo
```

- **Tipo**: `-` file regolare, `d` directory, `l` symlink.
- **Lettura**: posizioni 2, 5, 8.
- **Scrittura**: posizioni 3, 6, 9.
- **Esecuzione**: posizioni 4, 7, 10.
- **Setuid/setgid/sticky**: `s`, `S`, `t`, `T` possono sostituire `x` in posizioni 4, 7, 10.

### 4.3 Costruzione della regex

| Condizione | Regex (solo permessi) |
|---|---|
| File regolare | `^-` |
| Directory | `^d` |
| Link simbolico | `^l` |
| Owner può scrivere (file regolare) | `^-.w` |
| Group può leggere | `^-...r` |
| Other NON può scrivere | `^-.......[^w].` |
| Owner scrive, group/other no | `^-.w..[^w]..[^w].` |
| Tutti possono eseguire | `^-..x..x..x` |
| Estensione `.txt` | `\.txt$` |
| Dimensione > 3000 | `$5 > 3000` |
| Hard link > 1 | `$2 > 1` |

**Attenzione:** `^..w` da solo controlla la posizione 3, ma va combinato con il resto della stringa per evitare corrispondenze parziali. Nei comandi completi si usa `$1 ~ /.../` per limitare la regex al campo permessi.

### 4.4 L'esercizio A2 e la correzione

Testo: file regolari, estensione `.sh`, permesso di scrittura **solo** per il proprietario.

Soluzione corretta:

```bash
ls -l | awk '$1 ~ /^-.w..[^w]..[^w]./ && $9 ~ /\.sh$/ {print $9}'
```

La regex della guida `^-..w..[^w]..[^w]` sbaglia le posizioni. Controprova:

```bash
printf '%s\n' '-rwxr-xr-x 1 student student 4100 Jul 15 10:15 avvio.sh' \
  | awk '$1 ~ /^-.w..[^w]..[^w]./ && $9 ~ /\.sh$/ {print $9}'
# output: avvio.sh
```

Con la regex della guida non si ottiene nulla.

### 4.5 Altre condizioni

- **Dimensione**: `$5 > 3000`; ricordare che `ls -l` può usare `-h` (allora `$5` diventa `4,0K`, non numerico). Nella prova usare `ls -l` senza `-h`.
- **Hard link**: `$2 > 1`.
- **Owner/group**: `$3 == "student"`, `$4 == "staff"`.
- **Data**: `$6`, `$7`, `$8` sono scomodi; meglio `find -newermt` o `stat` se richiesto.
- **Nomi con spazi**: `$9` non basta; usare `find` o `stat`.

### 4.6 Alternativa con `find`

Per file regolari `.sh` eseguibili da tutti e con dimensione 3000–5000 byte:

```bash
find . -maxdepth 1 -type f -name '*.sh' -perm -0111 -size +2999c -size -5001c -print
```

Per “solo il proprietario può scrivere”:

```bash
find . -maxdepth 1 -type f -name '*.sh' -perm -0200 ! -perm /0022 -print
```

`-perm -0200` richiede il bit di scrittura owner; `! -perm /0022` esclude group/other write. È meno leggibile all'esame, ma più robusto con i nomi.

### 4.7 Checklist

- [ ] Ho identificato il tipo (regolare/directory/link)?
- [ ] Ho contato le posizioni 1–10 correttamente?
- [ ] Ho controllato write in 3, 6, 9 (non 4, 7, 10)?
- [ ] Ho usato `$1` per i permessi e `$5` per la dimensione?
- [ ] Ho gestito l'estensione con `\.sh$`?
- [ ] Ho considerato i nomi con spazi?

---

## 5. Processi, thread e sincronizzazione in C

### 5.1 Fatti chiave

1. `fork()` crea **un nuovo processo**; il figlio è una copia del padre, ma **solo il thread chiamante** viene duplicato.
2. Le variabili globali sono copiate (copy-on-write): le modifiche del figlio **non** si vedono nel padre e viceversa.
3. `pthread_create()` crea un thread **nello stesso processo**: condivide heap e globali, ha il proprio stack.
4. Il thread `main` è un thread: nel conteggio va incluso.
5. `wait()`/`waitpid()` aspettano **processi figli**; un figlio che chiama `wait()` riceve `-1`/`ECHILD`.
6. `pthread_join()` aspetta un **thread** dello stesso processo.
7. Un processo figlio termina ma resta **zombie** finché il padre non lo raccoglie.

### 5.2 Contare processi e thread

```text
processi totali = 1 (main) + numero di fork() eseguiti
thread nel processo P = 1 (main) + pthread_create() eseguiti in P
```

Attenzione: un `pthread_create()` dentro un ramo `if (pid == 0)` viene eseguito **solo dal figlio**; dentro `else` solo dal padre.

Esempio:

```c
pid = fork();
if (pid == 0) {
    /* figlio: ha solo il main thread */
} else {
    pthread_create(&t1, ...);
    pthread_create(&t2, ...);
}
```

- processi: 2;
- figlio: 1 thread;
- padre: 3 thread (main + t1 + t2);
- totale contesti: 4.

### 5.3 Analizzare la sincronizzazione

Per ogni `pthread_cond_wait` verificare:

1. **Predicato**: la condizione è una variabile condivisa protetta dal mutex?
2. **`while`**: il wait è dentro un `while (!predicato)` e non un `if`.
3. **Cambio di stato**: chi modifica la condizione lo fa **sotto mutex**?
4. **Segnale**: chi modifica lo stato chiama `signal`/`broadcast` **dopo** la modifica e **prima** di rilasciare il mutex?
5. **Join**: il `pthread_join` avviene **dopo** `pthread_mutex_unlock`?
6. **Lock**: nessun `pthread_mutex_lock` doppio sullo stesso mutex non ricorsivo.
7. **I/O bloccante**: nessuna `recv`, `read`, `write` bloccante mentre si tiene un mutex che serve all'altro thread.

### 5.4 Calcolare l'output

Procedimento:

1. Elencare i valori iniziali delle variabili globali.
2. Separare i rami del padre e del figlio: il figlio ha una copia indipendente.
3. Per il padre, considerare i due ordini possibili dei thread:
   - t1 entra per primo e si blocca;
   - t2 entra per primo e modifica lo stato.
4. Applicare le operazioni in ordine, rispettando mutex e predicati.
5. Verificare se l'output è deterministico o dipende dallo scheduling.
6. Ricordare che il padre spesso `wait()` il figlio: il figlio stampa prima.

### 5.5 Bug classici

| Bug | Sintomo | Correzione |
|---|---|---|
| `if` invece di `while` | risveglio spuro/consumo multiplo | `while (!pred)` |
| Nessun `signal` dopo il cambio di stato | attesa infinita | `pthread_cond_signal` dopo l'update |
| `signal` senza cambiare il predicato | lost wakeup | aggiornare la variabile sotto mutex |
| Doppio `lock` sullo stesso mutex | deadlock con sé stessi | rimuovere il secondo lock |
| `join` mentre si tiene il mutex | il worker non riacquisisce il mutex | `unlock` prima di `join` |
| Lock in ordine opposto | deadlock tra due thread | ordine globale dei lock |
| `recv`/`read` bloccante sotto mutex | deadlock per attesa circolare | non tenere il lock durante l'I/O |
| `wait()` nel figlio | `-1`, `ECHILD` | spostare nel padre |

### 5.6 Risposta tipo

```text
a) Processi: 2 (padre + figlio). Thread: 2 creati solo nel padre; il figlio ha 1 thread;
   il padre ha 3 thread (main + 2 worker); totale 4 contesti.
b) Sì/No. Anomalia: ... Causa: ... Correzione: ...
c) Figlio: x=..., y=... → output ...
   Padre: sequenza ... → output ...
   Ordine: il padre fa wait, quindi ...
```

---

## 6. Pipe, fork e `select()`

### 6.1 Fatti chiave sulle pipe

- `pipe(p)`: `p[0]` è il lato **lettura**, `p[1]` il lato **scrittura**.
- `read` su pipe vuota **blocca** finché ci sono dati o tutti i writer sono chiusi.
- **EOF**: `read` ritorna `0` quando **tutti** i descrittori di scrittura sono chiusi e il buffer è vuoto.
- `select` considera un fd **pronto in lettura** se ci sono dati **oppure** se è stato raggiunto l'EOF.
- `write` su una pipe senza lettori genera `SIGPIPE` (default: terminazione).
- La pipe ha capacità limitata (tipicamente 64 KB): se il lettore non legge, il writer si blocca.
- `fork` duplica i descrittori: ogni processo deve chiudere le estremità che non usa.

### 6.2 Calcolare le read

Dati `N` byte scritti e read di `k` byte:

```text
read1: min(k, N)
read2: min(k, N - read1)
...
read_finale: 0 (EOF)
```

Esempio: 7 byte, read da 3:

| read | disponibili | letti | buffer |
|---|---:|---:|---|
| 1 | 7 | 3 | ABC |
| 2 | 4 | 3 | DEF |
| 3 | 1 | 1 | G |
| 4 | 0 + EOF | 0 | — |

### 6.3 `select`: blocca o ritorna subito?

| Situazione | Comportamento |
|---|---|
| Pipe vuota, writer aperti | `select` **blocca** |
| Pipe con dati | `select` **ritorna subito** |
| Pipe vuota, tutti i writer chiusi | `select` **ritorna subito** (EOF) |
| Due fd pronti | ritorna, controllare entrambi con `FD_ISSET` |
| `select` modifica il set | usare un master set e copiarlo ogni giro |

### 6.4 Zombie e SIGPIPE

- Se il padre non fa `wait()`, il figlio terminato resta **zombie** finché il padre termina.
- Correzione: `wait(NULL)`/`waitpid`, oppure `signal(SIGCHLD, SIG_IGN)` (con attenzione alla portabilità).
- Se il padre chiude la lettura e il figlio scrive, il figlio riceve `SIGPIPE`; un handler può stampare un messaggio e terminare con `_exit`.

### 6.5 Risposta tipo

```text
1) select 1: blocca finché il figlio scrive; select 2/3: ritornano subito perché restano dati;
   select 4: ritorna subito perché tutti i writer sono chiusi (EOF).
2) n1=..., n2=..., n3=..., n4=0.
3) EOF = read 0; il figlio diventa zombie perché manca wait; correzione ...
```

---

## 7. Namespace e container

### 7.1 Fenomeno → namespace

| Osservazione | Namespace / meccanismo |
|---|---|
| `bash` ha PID 1 nel container | **PID** (`CLONE_NEWPID`) |
| `hostname` diverso dall'host | **UTS** (`CLONE_NEWUTS`) |
| `ps` vede solo i processi del container | **PID** |
| `/esame` non è visibile sull'host | **Mount** (`CLONE_NEWNS`) + OverlayFS |
| `/esame` non è visibile in un altro container | **upperdir** separata di OverlayFS |
| IP/porte separate | **NET** (`CLONE_NEWNET`) |
| utenti/UID separati | **USER** (`CLONE_NEWUSER`) |
| limiti CPU/RAM/PID | **cgroup** |
| IPC separati | **IPC** (`CLONE_NEWIPC`) |

### 7.2 OverlayFS

```text
lowerdir  = immagine base, sola lettura, condivisa
upperdir  = layer privato del container, lettura/scrittura
merged    = vista unificata presentata come /
```

- I file creati/modificati finiscono nell'**upperdir** del container.
- Un altro container ha un'**upperdir nuova e vuota**: non vede i file del primo.
- `docker rm` elimina l'upperdir (e quindi i dati) **se non sono in un volume**.
- Un **volume** o un **bind mount** è esterno all'upperdir e persiste.

### 7.3 Ciclo di vita

- Il processo con PID 1 nel namespace è il “init” del container.
- Quando PID 1 termina, il kernel invia `SIGKILL` agli altri processi del namespace e il container passa a `Exited`.
- `docker run --rm` elimina il container alla fine; senza `--rm` resta in stato `Exited` e può essere rimosso con `docker rm`.

### 7.4 Risposta tipo

```text
(a) PID 1 nel namespace PID del container; sull'host ha un PID diverso.
(b) hostname del namespace UTS: ...
(c) /esame non è visibile perché è nell'upperdir del container 1; il container 2 ha
    un upperdir separato. Il file si trova in /var/lib/docker/overlay2/<id>/diff/...
(d) All'uscita di PID 1 il container si arresta; i processi restanti vengono terminati.
```

---

## 8. Bug-hunting `_ERR`

### 8.1 Checklist

| Cosa cercare | Domanda | Correzione |
|---|---|---|
| `if` su condition variable | Può esserci un risveglio spuro? | `while` |
| `signal` mancante | Chi modifica lo stato sveglia qualcuno? | `signal`/`broadcast` |
| `signal` prima del wait senza predicato | Il segnale può andare perso? | predicato + `while` |
| doppio `lock` | Lo stesso thread riacquisisce un mutex non ricorsivo? | rimuovere/riordinare |
| lock inversion | Due thread prendono i lock in ordine opposto? | ordine globale |
| `join` sotto lock | Il worker può riacquisire il mutex? | `unlock` prima di `join` |
| I/O bloccante sotto lock | L'altro thread deve prendere lo stesso lock per sbloccare? | spostare l'I/O fuori |
| dati non protetti | Chi legge/scrive la variabile condivisa? | mutex/atomic |

### 8.2 Come rispondere

1. Indicare **file e riga** (o numero di riga).
2. Spiegare **la sequenza** che porta al blocco/errore.
3. Distinguere **deadlock**, **lost wakeup**, **spurious wakeup**, **data race**.
4. Fornire la **correzione minima** e, se utile, quella robusta.
5. Verificare che la correzione non introduca un altro problema (es. `signal` vs `broadcast`).

---

## 9. Script Bash d'esame

### 9.1 Struttura robusta

```bash
#!/bin/bash
set -u

if [ "$#" -ne 2 ]; then
    echo "Uso: $0 <directory> <stringa>" >&2
    exit 1
fi

dir=$1
needle=$2

if [ ! -d "$dir" ]; then
    echo "Errore: '$dir' non è una directory" >&2
    exit 1
fi
```

### 9.2 Punti critici

- **Virgolette**: `"$dir"`, `"$needle"`, `"$file"` per spazi e glob.
- **`find -print0` + `read -d ''`**: nomi con spazi/newline.
- **`grep -Fq --`**: stringa fissa, non regex; `--` evita opzioni.
- **`awk` commenti**: `#`, non `//`.
- **`set -e`**: evitare se si usa `grep` in un `if` (exit status nonzero è atteso).
- **`$(...)`** per command substitution.
- **`sleep 5`** nei monitoraggi.
- **Controllo argomenti** e directory/file.

### 9.3 Correzione dello script 2 della guida

```awk
# corretto
count[pid]++   # Incrementa il numero di LWP per questo PID
```

### 9.4 Script 3: attenzione a `sed -i`

`sed -i` è GNU; su Linux va bene. Su macOS: `sed -i ''`. Inoltre `\b` è GNU sed; in una prova Linux è accettabile.

---

# PARTE III — Nuovi esercizi con soluzioni

> Gli esercizi sono ordinati per famiglia. Ogni esercizio ha: **testo**, **dati** (se servono), **svolgimento**, **soluzione**, **spiegazione** e **trappole**.

## Mappa degli esercizi

| ID | Famiglia | Difficoltà | Ispirato a |
|---|---|---|---|
| B1 | Pipeline `ps`/`awk` | Base | Compito A, Es. 1 |
| B2 | Pipeline conteggio | Media | Quesito 1 / Compito B, Es. 1 |
| B3 | Pipeline con valori distinti | Media | Estensione originale |
| L1 | `ls -l` permessi | Base | Compito B, Es. 2 |
| L2 | `ls -l` tipo/dimensione | Base | Compito A, Es. 2 |
| L3 | `ls -l` hard link | Media | Estensione originale |
| C1 | Fork + thread + condvar | Media | Compito A, Es. 3 |
| C2 | Fork dopo thread | Media | Estensione originale |
| C3 | Bug lost wakeup | Media | §25.1 |
| P1 | Pipe + select + EOF | Media | Compito A/B, Es. 4 |
| P2 | Due pipe + select | Alta | Quesito 4 |
| D1 | Due container, file | Base | Compito A, Es. 5 |
| D2 | Volume vs OverlayFS | Media | Compito B, Es. 5 |
| D3 | Namespace senza Docker | Media | §23 |
| E1 | Lock inversion | Base | §25.1 |
| E2 | Mutex + I/O bloccante | Media | §25.1.3 |
| S1 | Script Bash robusto | Media | §25.2.6 |

## 10. Pipeline Bash

### Esercizio B1 — Utenti con processi in esecuzione recenti

**Testo.** Scrivere una pipeline che stampi una sola volta il nome degli utenti che hanno **almeno 2 processi** nello stato `R` (running) avviati da **meno di 60 secondi**.

**Dati.**

```text
root     R  120
alice    R   10
bob      R    5
alice    R   20
bob      R   70
carlo    R   15
dina     R   30
dina     S   40
dina     R   50
```

**Svolgimento.**

1. `ps -eo user=,stat=,etimes=` produce tre colonne senza header.
2. Filtro: `$2 ~ /^R/` (stato che inizia per R) e `$3 < 60`.
3. Estraggo `$1` (utente).
4. `sort` + `uniq -c` contano le occorrenze.
5. Tengo solo `$1 >= 2` e stampo `$2`.

**Soluzione.**

```bash
ps -eo user=,stat=,etimes= \
  | awk '$2 ~ /^R/ && $3 < 60 {print $1}' \
  | sort \
  | uniq -c \
  | awk '$1 >= 2 {print $2}'
```

**Output atteso.**

```text
alice
dina
```

**Spiegazione.** Alice ha due processi `R` con 10 e 20 secondi; Dina ne ha due con 30 e 50 (il processo `S` a 40 è escluso); Bob ha solo il processo da 5 secondi (quello da 70 è troppo vecchio); Carlo ne ha uno solo.

**Trappole.** `$2 ~ /^R/` non deve diventare `$2 == "R"`: gli stati possono essere `R+`, `Rl`, ecc. Il confronto `$3 < 60` è numerico.

---

### Esercizio B2 — Conteggio processi multithread per utente

**Testo.** Stampare, per ogni utente con **almeno 2 processi** con `NLWP >= 4`, il nome dell'utente e il numero di tali processi, ordinando per conteggio decrescente.

**Dati.**

```text
root  12
alice  3
bob    8
alice  5
carlo  1
bob    4
dina   7
dina   4
```

**Svolgimento.**

1. `ps -eo user=,nlwp=` → utente, numero LWP.
2. Filtro `$2 >= 4`.
3. Estraggo l'utente, ordino, conto.
4. `uniq -c` produce `conteggio utente`.
5. Tengo `$1 >= 2`, stampo `utente conteggio`.
6. Ordino per la seconda colonna numericamente decrescente.

**Soluzione.**

```bash
ps -eo user=,nlwp= \
  | awk '$2 >= 4 {print $1}' \
  | sort \
  | uniq -c \
  | awk '$1 >= 2 {print $2, $1}' \
  | sort -k2,2nr
```

**Output atteso.**

```text
bob 2
dina 2
```

**Spiegazione.** Bob ha due processi (8 e 4 LWP), Dina due (7 e 4), Alice solo uno (5), Root uno (12), Carlo nessuno.

**Trappole.** `sort -k2,2nr` ordina per la seconda colonna; senza `n` l'ordinamento sarebbe lessicografico. Se due utenti hanno lo stesso conteggio, l'ordine non è specificato; aggiungere `-k1,1` per un tie-break stabile.

---

### Esercizio B3 — Utenti su più terminali

**Testo.** Stampare gli utenti che hanno processi in stato **sleeping** (`STAT` che inizia per `S`) su **almeno 2 terminali distinti**, escludendo il terminale `?`.

**Dati.**

```text
root     Ss   ?
alice    S    pts/1
bob      S    pts/2
alice    Sl   pts/2
carlo    S    pts/3
bob      S    pts/4
bob      S    pts/4
dina     R    pts/5
dina     S    pts/6
```

**Svolgimento.**

1. `ps -eo user=,stat=,tty=`.
2. Filtro `$2 ~ /^S/` e `$3 != "?"`.
3. Stampo coppie `utente tty`.
4. `sort -u` elimina le coppie duplicate (stesso utente sullo stesso tty).
5. `awk` conta quante coppie distinte ha ogni utente.
6. Stampo chi ha almeno 2 coppie.

**Soluzione.**

```bash
ps -eo user=,stat=,tty= \
  | awk '$2 ~ /^S/ && $3 != "?" {print $1, $3}' \
  | sort -u \
  | awk '{c[$1]++} END {for (u in c) if (c[u] >= 2) print u}' \
  | sort
```

**Output atteso.**

```text
alice
bob
```

**Spiegazione.** Alice ha `pts/1` e `pts/2`; Bob ha `pts/2` e `pts/4` (la doppia riga su `pts/4` viene eliminata da `sort -u`); Carlo e Dina hanno un solo terminale distinto.

**Variante con `sort` + `uniq -c`.** È possibile anche estrarre l'utente e contare con `uniq -c`:

```bash
ps -eo user=,stat=,tty= \
  | awk '$2 ~ /^S/ && $3 != "?" {print $1, $3}' \
  | sort -u \
  | awk '{print $1}' \
  | sort \
  | uniq -c \
  | awk '$1 >= 2 {print $2}'
```

Questa versione funziona, ma attenzione a due dettagli:

- il comando è `uniq`, non `unique`;
- `uniq -c` produce **prima il conteggio e poi la chiave**: `2 alice`. Quindi il filtro è `$1 >= 2` e la stampa è `$2`. La forma `awk '$2 >= 2 {print $1}'` confronterebbe il nome utente con 2 e stamperebbe il conteggio, non l'utente.

Il secondo `sort` è spesso ridondante perché `sort -u` ha già ordinato le righe `utente tty`, ma è innocuo e rende il comando più chiaro.

**Trappole.** Senza `sort -u`, le due righe di Bob su `pts/4` conterebbero come due terminali e darebbero un falso positivo. `$3 != "?"` esclude i processi senza terminale.

---

## 11. Filtri `ls -l`

**Dati comuni.**

```text
-rw-r----- 1 student student 6400 Jul 15 10:10 errori.log
-rw-rw-r-- 1 student student 9200 Jul 15 10:11 debug.log
-rw-r--r-- 2 student student 5300 Jul 15 10:12 condiviso.log
-rw------- 1 student student 8000 Jul 15 10:13 privato.log
-rw-r----- 1 student student 1500 Jul 15 10:14 breve.log
-rwxr-xr-x 1 student student 4100 Jul 15 10:15 avvio.sh
drwxr-xr-x 2 student student 4096 Jul 15 10:16 archivio.log
-rw-r----- 1 alice   staff   7200 Jul 15 10:17 sistema.txt
lrwxrwxrwx 1 student student   10 Jul 15 10:18 ultimo.log -> errori.log
-rw-r--r-- 2 student student 2500 Jul 15 10:19 note.txt
-r--r----- 1 student student 1800 Jul 15 10:20 sola.txt
-rw-r--r-- 3 student student 3300 Jul 15 10:22 dati.txt
```

### Esercizio L1 — Log leggibili dal gruppo

**Testo.** Visualizzare i nomi dei file che sono: file regolari, estensione `.log`, leggibili dal gruppo, **non** scrivibili dagli altri, dimensione **maggiore di 1000 byte**.

**Svolgimento.**

- tipo: `^-`;
- group read: posizione 5 = `r`;
- other no write: posizione 9 ≠ `w`;
- dimensione: `$5 > 1000`;
- estensione: `$9 ~ /\.log$/`.

Regex permessi: `^-...r...[^w].` (tipo, 3 caratteri user, `r` group, 3 caratteri, `[^w]` other write, `.` other execute).

**Soluzione.**

```bash
ls -l | awk '$1 ~ /^-...r...[^w]./ && $5 > 1000 && $9 ~ /\.log$/ {print $9}'
```

**Output atteso.**

```text
errori.log
debug.log
condiviso.log
breve.log
```

**Spiegazione.** `privato.log` è escluso perché il gruppo non ha lettura; `archivio.log` è una directory; `ultimo.log` è un symlink; `sistema.txt` ha estensione sbagliata. `breve.log` ha 1500 byte, quindi supera 1000: se la traccia dicesse “almeno 1500” andrebbe gestito con `>=`.

**Trappole.** La posizione 9 è **other write**, non other execute. La regex `[^w]` accetta `-`, `r`, `x`; va bene perché vogliamo solo “non w”.

---

### Esercizio L2 — Script eseguibili

**Testo.** Visualizzare i nomi dei file regolari con estensione `.sh`, eseguibili da proprietario, gruppo e altri, con dimensione tra 3000 e 5000 byte inclusi.

**Svolgimento.**

- tipo `^-`;
- `x` in posizioni 4, 7, 10: pattern `^-..x..x..x`;
- `$5 >= 3000 && $5 <= 5000`;
- `$9 ~ /\.sh$/`.

**Soluzione.**

```bash
ls -l | awk '$1 ~ /^-..x..x..x/ && $5 >= 3000 && $5 <= 5000 && $9 ~ /\.sh$/ {print $9}'
```

**Output atteso.**

```text
avvio.sh
```

**Spiegazione.** `avvio.sh` ha permessi `-rwxr-xr-x` e 4100 byte. Gli altri `.sh` non rispettano dimensione o permessi.

**Trappole.** Un file con setuid (`-rwsr-xr-x`) ha `s` in posizione 4: è eseguibile, ma la regex `..x` non lo riconosce. Se la traccia dice “eseguibile” e non “x esplicito”, gestire anche `s`/`S`. Nella prova, chiedersi sempre se il bit speciale conta.

---

### Esercizio L3 — Hard link e gruppo

**Testo.** Visualizzare nome e numero di hard link dei file regolari con estensione `.txt`, leggibili dal gruppo e con **più di un hard link**.

**Svolgimento.**

- tipo `^-` e group read posizione 5: `^-...r`;
- `$2 > 1`;
- `$9 ~ /\.txt$/`;
- stampa `$9, $2`.

**Soluzione.**

```bash
ls -l | awk '$1 ~ /^-...r/ && $2 > 1 && $9 ~ /\.txt$/ {print $9, $2}'
```

**Output atteso.**

```text
note.txt 2
dati.txt 3
```

**Spiegazione.** `note.txt` ha 2 link e `dati.txt` 3; entrambi hanno group read. Gli altri `.txt` hanno un solo link o non sono group-readable.

**Trappole.** `$2` è il numero di link, non la dimensione. `$5` è la dimensione. Non confonderli.

---

## 12. Processi, thread e sincronizzazione

### Esercizio C1 — Fork, thread e condition variable

**Testo.** Dato il programma:

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
int x = 6, y = 2;

void *t1(void *arg) {
    pthread_mutex_lock(&m);
    while (x <= y)
        pthread_cond_wait(&c, &m);
    y = x + y;
    pthread_mutex_unlock(&m);
    return NULL;
}

void *t2(void *arg) {
    pthread_mutex_lock(&m);
    x = y + 3;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
    return NULL;
}

int main(void) {
    pthread_t th[2];
    pid_t pid = fork();

    if (pid == 0) {
        x = 10;
        y = 20;
    } else {
        x = y;
        pthread_create(&th[0], NULL, t1, NULL);
        pthread_create(&th[1], NULL, t2, NULL);
        pthread_join(th[0], NULL);
        pthread_join(th[1], NULL);
    }

    wait(NULL);
    printf("%d %d\n", x, y);
    return 0;
}
```

Rispondere:

a) Quanti processi e quanti thread vengono creati?  
b) Il programma termina correttamente? Segnalare anomalie e correzioni.  
c) Cosa stampa? Motivare.

**Svolgimento.**

- `fork()` → 2 processi.
- `pthread_create` sono nel ramo `else`, quindi solo nel padre: 2 thread worker + main = 3 thread nel padre.
- Il figlio ha solo il main thread: 1 thread.
- Totale contesti: 4.

**Soluzione.**

a) **2 processi**; **2 nuovi thread** (solo nel padre); figlio 1 thread; padre 3 thread.

b) Sì, termina, ma `wait(NULL)` viene eseguita anche dal figlio: non avendo figli, ritorna `-1` con `errno = ECHILD`. Correzione: spostare `wait(NULL)` nel ramo del padre:

```c
if (pid > 0)
    wait(NULL);
```

Non c'è deadlock: `t2` modifica `x` e segnala la condition; `t1` usa `while` e ricontrolla il predicato.

c) Output:

```text
10 20
5 7
```

- **Figlio**: `x=10`, `y=20`, nessun thread → stampa `10 20`.
- **Padre**: `x = y = 2`; `t1` trova `2 <= 2`, si sospende; `t2` imposta `x = 2 + 3 = 5` e segnala; `t1` si sveglia, `5 <= 2` è falso, esegue `y = 5 + 2 = 7`; stampa `5 7`.
- Il padre fa `wait`, quindi il figlio stampa prima.

**Trappole.** Se `t2` parte prima, `x=5` e `t1` non si blocca, ma il risultato è lo stesso. Il `wait` del figlio non è un deadlock.

---

### Esercizio C2 — Fork dopo la creazione di un thread

**Testo.**

```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void *worker(void *arg) {
    printf("worker %ld\n", (long)arg);
    return NULL;
}

int main(void) {
    pthread_t a, b;

    pthread_create(&a, NULL, worker, (void *)1);

    pid_t pid = fork();
    if (pid == 0) {
        printf("child\n");
        return 0;
    }

    pthread_create(&b, NULL, worker, (void *)2);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    wait(NULL);
    printf("parent end\n");
    return 0;
}
```

Quanti processi e quanti thread esistono? Il figlio esegue `worker`? L'output è deterministico?

**Soluzione.**

- Processi: **2**.
- Padre: `main` + `a` + `b` = **3 thread**.
- Figlio: solo il thread che ha chiamato `fork` = **1 thread**.
- Totale: **4** thread/contesti.
- Il figlio **non** esegue `worker`: `fork` duplica solo il thread chiamante, non gli altri thread del processo. Il thread `a` esiste solo nel padre.
- L'output non è deterministico: `worker 1` può stampare prima o dopo `fork`; `child`, `worker 2` e `parent end` possono interlearsi. L'unica cosa certa è che `worker 1` non appare nel figlio.

**Spiegazione.** Dopo `fork`, il figlio eredita lo spazio di indirizzi e i descrittori aperti, ma non i thread. Eventuali mutex bloccati da altri thread nel padre possono restare bloccati nel figlio: è un motivo per cui `fork` in processi multithread è delicato.

**Trappole.** Se `stdout` è bufferizzato e si fa `fork`, il buffer può essere duplicato e alcune righe possono apparire due volte. In un esame, concentrarsi sul conteggio; per evitare il problema usare `fflush(stdout)` prima di `fork` o `_exit` nel figlio.

---

### Esercizio C3 — Bug: lost wakeup e `if`

**Testo.** Trovare e correggere gli errori:

```c
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
int ready = 0;

void *waiter(void *arg) {
    pthread_mutex_lock(&m);
    if (!ready)
        pthread_cond_wait(&c, &m);
    printf("waiter: entro nella sezione critica\n");
    pthread_mutex_unlock(&m);
    return NULL;
}

int main(void) {
    pthread_t t;
    pthread_create(&t, NULL, waiter, NULL);

    pthread_mutex_lock(&m);
    pthread_cond_signal(&c);   /* manca ready = 1! */
    pthread_mutex_unlock(&m);

    pthread_join(t, NULL);
    return 0;
}
```

**Analisi.**

1. `main` invia `signal` **senza modificare `ready`**. Se il segnale arriva prima che `waiter` entri in `wait`, il segnale viene perso e `waiter` dorme per sempre: **lost wakeup**.
2. Anche se il segnale arrivasse dopo, `ready` resta 0; il `waiter` esce dal wait e non ricontrolla perché c'è un `if` invece di un `while`: **violazione del predicato** e rischio con risvegli spuri.
3. `ready` è letto/scritto sotto mutex, quindi la protezione c'è; manca la modifica.

**Correzione.**

```c
void *waiter(void *arg) {
    pthread_mutex_lock(&m);
    while (!ready)
        pthread_cond_wait(&c, &m);
    printf("waiter: entro nella sezione critica\n");
    pthread_mutex_unlock(&m);
    return NULL;
}

int main(void) {
    pthread_t t;
    pthread_create(&t, NULL, waiter, NULL);

    pthread_mutex_lock(&m);
    ready = 1;                 /* cambia il predicato */
    pthread_cond_signal(&c);   /* poi sveglia */
    pthread_mutex_unlock(&m);

    pthread_join(t, NULL);
    return 0;
}
```

**Regola d'oro.** `pthread_cond_wait` va sempre usata con un predicato protetto da mutex e dentro un `while`; il segnale va inviato **dopo** aver cambiato il predicato, mentre si tiene il mutex.

---

## 13. Pipe, fork e `select`

### Esercizio P1 — Quattro read su una pipe

**Testo.**

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(void) {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[0]);
        write(p[1], "ABCDEFG", 7);
        close(p[1]);
        _exit(0);
    }

    close(p[1]);

    char buf[4];
    fd_set set;
    int n[4];

    for (int i = 0; i < 4; i++) {
        FD_ZERO(&set);
        FD_SET(p[0], &set);
        select(p[0] + 1, &set, NULL, NULL, NULL);

        n[i] = read(p[0], buf, 3);
        if (n[i] > 0) {
            buf[n[i]] = '\0';
            printf("read %d: n=%d buf=%s\n", i + 1, n[i], buf);
        } else {
            printf("read %d: n=%d EOF\n", i + 1, n[i]);
        }
    }

    close(p[0]);
    wait(NULL);
    return 0;
}
```

Domande: quali `select` bloccano? Quanto valgono `n[0..3]`? Cosa stampa? C'è rischio di zombie?

**Soluzione.**

| read | select | disponibili | letti | output |
|---|---|---:|---:|---|
| 1 | blocca finché il figlio scrive | 7 | 3 | `ABC` |
| 2 | ritorna subito | 4 | 3 | `DEF` |
| 3 | ritorna subito | 1 | 1 | `G` |
| 4 | ritorna subito (EOF) | 0 | 0 | EOF |

Output:

```text
read 1: n=3 buf=ABC
read 2: n=3 buf=DEF
read 3: n=1 buf=G
read 4: n=0 EOF
```

**Spiegazione.** La prima `select` attende i dati; la seconda e la terza trovano byte ancora nel buffer della pipe; la quarta ritorna subito perché tutti i writer sono chiusi e `read` restituisce 0 (EOF). Il figlio diventa zombie fino al `wait(NULL)` del padre; la soluzione lo include.

**Trappole.** `char buf[4]` è sufficiente per read da 3 più `'\0'`. Se si leggesse 4 byte, `buf[n]` sarebbe fuori limite quando `n=4`.

---

### Esercizio P2 — Due pipe e `select`

**Testo.** Due figli scrivono un carattere ogni 50 ms: il figlio 1 scrive `'X'` su `p1`, il figlio 2 scrive `'Y'` su `p2`. Il padre usa `select()` per leggere da entrambe le pipe finché non ha ricevuto **8 caratteri** in totale; poi stampa i conteggi, chiude le pipe e attende i figli. I figli devono gestire `SIGPIPE` e terminare.

**Soluzione.**

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/wait.h>

static void on_sigpipe(int sig) {
    (void)sig;
    const char msg[] = "[figlio] SIGPIPE: pipe chiusa dal padre\n";
    write(STDERR_FILENO, msg, sizeof(msg) - 1);
    _exit(1);
}

int main(void) {
    int p1[2], p2[2];
    char buf;
    int c1 = 0, c2 = 0;

    if (pipe(p1) < 0 || pipe(p2) < 0) {
        perror("pipe");
        exit(1);
    }

    if (fork() == 0) {                 /* figlio 1 */
        signal(SIGPIPE, on_sigpipe);
        close(p1[0]);
        close(p2[0]);
        close(p2[1]);
        while (1) {
            if (write(p1[1], "X", 1) <= 0)
                break;
            usleep(50000);
        }
        _exit(0);
    }

    if (fork() == 0) {                 /* figlio 2 */
        signal(SIGPIPE, on_sigpipe);
        close(p2[0]);
        close(p1[0]);
        close(p1[1]);
        while (1) {
            if (write(p2[1], "Y", 1) <= 0)
                break;
            usleep(50000);
        }
        _exit(0);
    }

    /* padre */
    close(p1[1]);
    close(p2[1]);

    int maxfd = (p1[0] > p2[0] ? p1[0] : p2[0]) + 1;

    while (c1 + c2 < 8) {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(p1[0], &set);
        FD_SET(p2[0], &set);

        if (select(maxfd, &set, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(p1[0], &set)) {
            if (read(p1[0], &buf, 1) == 1)
                c1++;
        }
        if (FD_ISSET(p2[0], &set)) {
            if (read(p2[0], &buf, 1) == 1)
                c2++;
        }
    }

    printf("X=%d Y=%d totale=%d\n", c1, c2, c1 + c2);

    close(p1[0]);
    close(p2[0]);
    wait(NULL);   /* evita zombie */
    wait(NULL);
    return 0;
}
```

**Spiegazione.** Ogni figlio chiude le estremità che non usa. Il padre chiude i lati di scrittura e usa `select` per multiplexare le due read. Quando il padre chiude le read, i figli al successivo `write` ricevono `SIGPIPE`; l'handler li termina. I conteggi non sono deterministici: dipende da quale figlio scrive prima; l'unica cosa certa è `c1 + c2 = 8`.

**Trappole.** Se i figli non chiudono le estremità inutilizzate, il padre non vedrà mai l'EOF (quando servirà). Se il padre non fa `wait`, i figli possono restare zombie. `maxfd` va calcolato una volta, ma se si aggiungono fd dinamicamente va aggiornato.

---

## 14. Namespace e container Docker

### Esercizio D1 — Due container e un file

**Testo.**

```bash
docker run -it --name c1 --hostname h1 ubuntu bash
# dentro c1:
ps -ef
hostname
mkdir /esame
echo "prova" > /esame/nota.txt
exit

docker run -it --name c2 --hostname h2 ubuntu bash
# dentro c2:
ps -ef
hostname
ls /esame
exit
```

Domande:

a) Che PID ha `bash` in `c1` e in `c2`?  
b) Che valore restituisce `hostname` nei due container?  
c) `/esame` è visibile in `c2`? Perché?  
d) Dove si trova fisicamente il file? Cosa succede con `docker rm c1`?  
e) Cosa succede agli altri processi quando `bash` (PID 1) termina?

**Soluzione.**

a) In entrambi i container `bash` è **PID 1** del rispettivo **PID namespace**; sull'host ha un PID globale diverso.

b) `h1` in `c1`, `h2` in `c2`; l'hostname dell'host non cambia (UTS namespace).

c) **No**: `/esame` è stato creato nell'upperdir di `c1`; `c2` ha un upperdir nuovo e vuoto. Il mount namespace + OverlayFS impediscono la visibilità.

d) Il file si trova nell'upperdir di `c1`, tipicamente sotto `/var/lib/docker/overlay2/<id>/diff/esame/nota.txt`. `docker rm c1` elimina l'upperdir e quindi il file, **a meno che** `/esame` non sia un volume o un bind mount.

e) Alla morte di PID 1 il kernel invia `SIGKILL` agli altri processi del namespace; il container passa a `Exited`.

**Spiegazione.** I namespace coinvolti sono PID (numerazione e visibilità dei processi), UTS (hostname), MNT (tabella dei mount) e il layer OverlayFS. Il file non è “finto”: è scritto davvero, ma in un layer privato del container.

**Trappole.** Non dire che il file “non esiste”: esiste, ma è in un altro layer. Non dire che l'hostname cambia sull'host: cambia solo nel namespace UTS.

---

### Esercizio D2 — Volume vs OverlayFS

**Testo.**

```bash
docker run -d --name c3 -v /tmp/lso:/dati ubuntu sleep 1000
docker exec c3 sh -c 'echo "ciao" > /dati/prova.txt'
docker rm -f c3
docker run --rm -v /tmp/lso:/dati ubuntu cat /dati/prova.txt
```

Domande:

a) Il secondo container vede `/dati/prova.txt`?  
b) Dove si trova il file sull'host?  
c) Che differenza c'è rispetto a scrivere in `/esame` senza volume?  
d) Come si limita la memoria del container a 128 MB?

**Soluzione.**

a) **Sì**: `/tmp/lso` è un bind mount dell'host montato in `/dati`; non è nell'upperdir del container.

b) Su `/tmp/lso/prova.txt` dell'host.

c) Senza volume, il file finirebbe nell'upperdir di `c3` e verrebbe eliminato con `docker rm -f c3`. Con un volume/bind mount, i dati sono esterni al ciclo di vita del container e persistono.

d) `docker run --memory=128m ...` (cgroup v2 limita la memoria); per la CPU `--cpus`, per i processi `--pids-limit`.

**Spiegazione.** OverlayFS gestisce lo strato scrivibile del container; un volume è un mount esterno che “buca” l'OverlayFS e scrive direttamente sul filesystem host (o su un volume gestito da Docker). I cgroup limitano le risorse, non l'isolamento dei nomi.

**Trappole.** `/tmp/lso` deve esistere sull'host; il bind mount non crea la directory host. `docker rm -f` elimina il container, non il bind mount.

---

### Esercizio D3 — Namespace senza Docker

**Testo.**

```bash
sudo unshare --pid --uts --mount --fork --mount-proc bash
# dentro:
ps -ef
hostname laboratorio
exit
```

Domande:

a) Perché `bash` ha PID 1?  
b) La modifica di `hostname` influisce sull'host?  
c) Perché `ps` vede pochi processi?  
d) Cosa succede all'uscita?  
e) Quali flag di namespace sono stati usati?

**Soluzione.**

a) `--pid` crea un nuovo PID namespace; il primo processo (`bash`) diventa PID 1. `--fork` è necessario perché `unshare` stesso resti fuori dal nuovo namespace e `bash` sia il primo processo.

b) No: `--uts` crea un UTS namespace privato; `hostname laboratorio` cambia solo quel namespace.

c) `--mount-proc` monta una nuova `/proc` nel mount namespace; `ps` vede solo i processi del PID namespace, non quelli dell'host.

d) All'uscita di PID 1 il namespace viene distrutto; i processi restanti vengono terminati; l'host resta invariato.

e) `--pid` (`CLONE_NEWPID`), `--uts` (`CLONE_NEWUTS`), `--mount` (`CLONE_NEWNS`); `--mount-proc` è un'operazione di mount, non un namespace separato.

**Spiegazione.** È la versione “manuale” di ciò che fa Docker con `clone()`. Docker aggiunge anche NET, IPC, USER, cgroup e OverlayFS.

---

## 15. Bug-hunting

### Esercizio E1 — Lock inversion

**Testo.** Trovare il deadlock:

```c
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void *A(void *arg) {
    pthread_mutex_lock(&m1);
    usleep(1000);
    pthread_mutex_lock(&m2);
    /* ... */
    pthread_mutex_unlock(&m2);
    pthread_mutex_unlock(&m1);
    return NULL;
}

void *B(void *arg) {
    pthread_mutex_lock(&m2);
    usleep(1000);
    pthread_mutex_lock(&m1);
    /* ... */
    pthread_mutex_unlock(&m1);
    pthread_mutex_unlock(&m2);
    return NULL;
}
```

**Analisi.** A tiene `m1` e attende `m2`; B tiene `m2` e attende `m1`: attesa circolare. È un deadlock da **lock ordering** invertito.

**Correzione.** Stabilire un ordine globale: entrambi prendono prima `m1` e poi `m2` (o usare un solo lock). Se l'ordine non è esprimibile, usare `pthread_mutex_trylock` con backoff, ma è più complesso.

```c
/* B corretto */
pthread_mutex_lock(&m1);
pthread_mutex_lock(&m2);
/* ... */
pthread_mutex_unlock(&m2);
pthread_mutex_unlock(&m1);
```

**Trappola.** `usleep` non risolve il deadlock: lo rende solo più probabile. La correzione deve cambiare l'ordine dei lock.

---

### Esercizio E2 — Mutex e I/O bloccante

**Testo.** Spiegare perché il programma può bloccarsi:

```c
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
int sock;

void *receiver(void *arg) {
    char buf[256];
    pthread_mutex_lock(&m);
    recv(sock, buf, sizeof(buf), 0);   /* bloccante */
    pthread_mutex_unlock(&m);
    return NULL;
}

void *sender(void *arg) {
    pthread_mutex_lock(&m);
    send(sock, "ping", 4, 0);
    pthread_mutex_unlock(&m);
    return NULL;
}
```

**Analisi.** Il receiver tiene il mutex mentre attende dati dal socket; il sender non può inviare perché serve lo stesso mutex. Se il server risponde solo dopo aver ricevuto il messaggio, si crea un'attesa circolare: receiver aspetta il pacchetto, sender aspetta il mutex.

**Correzione.** Non tenere il mutex durante l'I/O bloccante. Proteggere solo le strutture dati condivise; il socket è già full-duplex e le operazioni di lettura/scrittura possono essere concorrenti (con le dovute cautele).

```c
void *receiver(void *arg) {
    char buf[256];
    ssize_t n = recv(sock, buf, sizeof(buf), 0);
    if (n > 0) {
        pthread_mutex_lock(&m);
        /* aggiorna stato condiviso */
        pthread_mutex_unlock(&m);
    }
    return NULL;
}
```

**Trappola.** Un lock globale attorno a `recv` sembra “sicuro”, ma trasforma un'attesa di rete in un deadlock applicativo.

---

## 16. Script Bash

### Esercizio S1 — Monitorare file `.txt` che contengono una stringa

**Testo.** Scrivere uno script Bash che, dati una directory e una stringa, ogni 5 secondi conti i file `.txt` nella directory che contengono la stringa e stampi un messaggio **solo quando il conteggio cambia**. Gestire directory inesistenti e nomi con spazi.

**Soluzione.**

```bash
#!/bin/bash
set -u

if [ "$#" -ne 2 ]; then
    echo "Uso: $0 <directory> <stringa>" >&2
    exit 1
fi

dir=$1
needle=$2

if [ ! -d "$dir" ]; then
    echo "Errore: '$dir' non è una directory" >&2
    exit 1
fi

prev=-1

while :; do
    count=0

    while IFS= read -r -d '' file; do
        if grep -Fq -- "$needle" "$file"; then
            count=$((count + 1))
        fi
    done < <(find "$dir" -maxdepth 1 -type f -name '*.txt' -print0)

    if [ "$count" -ne "$prev" ]; then
        printf '[%s] %d file .txt contengono la stringa\n' "$(date +%T)" "$count"
        prev=$count
    fi

    sleep 5
done
```

**Spiegazione.**

- `find ... -print0` + `read -d ''` gestiscono nomi con spazi e newline.
- `IFS= read -r` evita interpretazioni di backslash.
- `grep -Fq --` cerca la stringa letterale, non una regex; `--` protegge da stringhe che iniziano con `-`.
- `set -u` segnala variabili non impostate; non uso `set -e` perché `grep` restituisce 1 quando non trova (è un caso normale).
- `prev=-1` forza la stampa al primo giro.
- Il contatore è locale al ciclo; `count=$((count + 1))` funziona in Bash.

**Trappole.** Il glob `"$dir"/*.txt` con nessun file produce la stringa letterale e va gestito; `find -print0` evita il problema. `grep "$needle"` senza `-F` interpreta metacaratteri; senza `--` una stringa come `-n` diventa un'opzione.

---

# PARTE IV — Formulario e autovalutazione

## 17. Formulario rapido

### 17.1 Pipeline

```bash
ps -eo user=,stat=,etimes= | awk '...' | sort | uniq -c | awk '$1 >= N {print $2}'
ps -eo user=,nlwp=        | awk '$2 >= 4 {print $1}' | sort | uniq -c
ps -eo user=,stat=,tty=   | awk '$2 ~ /^S/ && $3 != "?" {print $1, $3}' | sort -u
```

### 17.2 `ls -l`

| Posizione | Permesso |
|---:|---|
| 1 | tipo (`-`, `d`, `l`) |
| 2, 5, 8 | read user/group/other |
| 3, 6, 9 | write user/group/other |
| 4, 7, 10 | execute user/group/other |

```bash
# regolare, group read, other no write
ls -l | awk '$1 ~ /^-...r...[^w]./ {print $9}'
# solo owner write
ls -l | awk '$1 ~ /^-.w..[^w]..[^w]./ {print $9}'
# tutti eseguibili
ls -l | awk '$1 ~ /^-..x..x..x/ {print $9}'
```

### 17.3 C

```c
/* fork */
pid_t pid = fork();
if (pid == 0) { /* figlio */ } else { /* padre */ }

/* thread */
pthread_create(&t, NULL, worker, arg);
pthread_join(t, NULL);

/* attesa condizione */
pthread_mutex_lock(&m);
while (!pred)
    pthread_cond_wait(&c, &m);
/* ... */
pthread_mutex_unlock(&m);

/* wait */
if (pid > 0)
    wait(NULL);
```

### 17.4 Pipe/select

```c
pipe(p);          /* p[0] read, p[1] write */
close(p[0]);      /* nel writer */
close(p[1]);      /* nel reader */

FD_ZERO(&set);
FD_SET(fd, &set);
select(fd + 1, &set, NULL, NULL, NULL);
if (FD_ISSET(fd, &set)) {
    ssize_t n = read(fd, buf, size);
    if (n == 0) { /* EOF */ }
}
```

### 17.5 Container

| Domanda | Risposta |
|---|---|
| PID di `bash` nel container | 1 (PID namespace) |
| Hostname | quello del container (UTS) |
| File creato nel container | upperdir OverlayFS |
| File visibile in un altro container | no (upperdir diversa) |
| File con volume | sì, persiste |
| Morte di PID 1 | container si arresta |

---

## 18. Autovalutazione

### 18.1 Griglia

| Competenza | Non ancora | Base | Buono | Ottimo |
|---|---|---|---|---|
| Pipeline `ps`/`awk` | | | | |
| `ls -l` e regex permessi | | | | |
| Conteggio processi/thread | | | | |
| Condition variable | | | | |
| Pipe/select/EOF | | | | |
| Namespace/OverlayFS | | | | |
| Bug-hunting | | | | |

### 18.2 Test finale (90 minuti)

1. Senza guardare: risolvere A1, A2, A3, A4, A5.
2. Confrontare con la guida; segnare ogni errore.
3. Rifare gli esercizi B1–B3, L1–L3, C1–C3, P1–P2, D1–D3 di questo documento.
4. Ripetere il test dopo 24 ore: conta solo ciò che viene senza appunti.

### 18.3 Errori da non ripetere

- Dimenticare `sort` prima di `uniq`.
- Usare `$4` invece di `$3` per la write dell'owner.
- Contare solo i worker e dimenticare il main thread.
- Usare `if` invece di `while` con le condition variable.
- Dimenticare che `select` è pronto anche su EOF.
- Dire che il file del container non esiste: è nell'upperdir.
- Chiamare `wait()` nel figlio e aspettarsi che blocchi.

---

## 19. Conclusione

La guida originale è una buona base, ma il modo più efficace di usarla è **rifare gli esercizi**, non leggerli. Le cinque famiglie sono ripetitive: una volta interiorizzati lo schema della pipeline, la mappa dei permessi, il modello fork/thread, la semantica EOF/select e la mappa namespace/OverlayFS, la prova diventa un esercizio di precisione più che di memoria.

La correzione più importante da ricordare è quella del **Compito A, Esercizio 2**: la write dell'owner è in posizione **3**, non 4. Da lì derivano tutte le regex corrette.
