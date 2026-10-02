# Laboratorio di Sistemi Operativi — Guida Completa allo Studio

> **Corso di Laurea in Informatica — A.A. 2025-2026**  
> **Prof. Alberto Finzi**  
> Questa guida copre **tutti gli argomenti** delle lezioni 1–31.

---

## Indice


0. [Informazioni sul Corso e Modalità d'Esame (A.A. 2025-2026)](#0-informazioni-sul-corso-e-modalità-desame-aa-2025-2026) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-0)


1. [Introduzione ai Sistemi Operativi e Unix](#1-introduzione-ai-sistemi-operativi-e-unix) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-1)


2. [Il File System Unix](#2-il-file-system-unix) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-2)


3. [La Shell Bash](#3-la-shell-bash) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-3)


4. [Comandi Unix Fondamentali](#4-comandi-unix-fondamentali) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-4)


5. [Grep e le Espressioni Regolari](#5-grep-e-le-espressioni-regolari) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-5)


6. [Script Shell](#6-script-shell) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-6)


7. [Sed e Awk](#7-sed-e-awk) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-7)


8. [Funzioni in Bash](#8-funzioni-in-bash) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-8)


9. [Compilazione C e GCC](#9-compilazione-c-e-gcc) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-9)


10. [I/O di Basso Livello (System Call)](#10-io-di-basso-livello-system-call) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-10)


11. [Processi Unix](#11-processi-unix) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-11)


12. [Segnali](#12-segnali) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-12)


13. [IPC: Pipe, FIFO e Memoria Condivisa (mmap)](#13-ipc-pipe-fifo-e-memoria-condivisa-mmap) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-13)


14. [Thread e Concorrenza](#14-thread-e-concorrenza) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-14)


15. [Sincronizzazione: Mutex, Condition Variable, Semafori](#15-sincronizzazione-mutex-condition-variable-semafori) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-15)


16. [Problemi Classici di Sincronizzazione](#16-problemi-classici-di-sincronizzazione) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-16)


17. [Socket — Comunicazione di Rete](#17-socket--comunicazione-di-rete) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-17)


18. [I/O Multiplexing — `select()`](#18-io-multiplexing--select) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-18)


19. [Segnali nelle Socket di Rete — SIGPIPE ed EINTR](#19-segnali-nelle-socket-di-rete--sigpipe-ed-eintr) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-19)


20. [Broadcast e Multicast UDP](#20-broadcast-e-multicast-udp) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-20)


21. [Comandi di Rete e Risoluzione DNS](#21-comandi-di-rete-e-risoluzione-dns) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-21)


22. [Virtualizzazione](#22-virtualizzazione) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-22)


23. [Container: Namespace, Cgroups e OverlayFS](#23-container-namespace-cgroups-e-overlayfs) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-23)


24. [Docker](#24-docker) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-24)


25. [Corpus dei File Pratici d'Esame: Debugging (_ERR), Template (_TODO) e Prove Parziali](#25-corpus-dei-file-pratici-desame-debugging-_err-template-_todo-e-prove-parziali) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-25)


26. [Guida Rapida alle Parole Chiave](#26-guida-rapida-alle-parole-chiave) — [Sottosezioni](#indice-delle-sottosezioni-capitolo-26)


---

## 0. Informazioni sul Corso e Modalità d'Esame (A.A. 2025-2026)
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

> **Corso:** Laboratorio di Sistemi Operativi (LSO)  
> **Docente:** Prof. Alberto Finzi (Studio: via Claudio 21, Napoli — email: `alberto.finzi@unina.it`, specificare sempre `[LSO]` nell'oggetto)  
> **Crediti:** 8 CFU — Corso di Laurea in Informatica, Università degli Studi di Napoli Federico II  
> **Propedeuticità:** Sistemi Operativi I, Algebra  
> **Team del Corso:** *LSO (Canale 2: Finzi) A.A. 2025-26* (Codice: `v3546h5`)

### 0.1 Obiettivi del Corso
Il corso fornisce gli strumenti teorici e le metodologie pratiche per:
1. **Gestione del Sistema Operativo:** Comandi Unix avanzati, espressioni regolari (Grep), filtri di elaborazione stream (Sed e Awk) e Shell Programming (Bash).
2. **Programmazione Avanzata in Unix (System Programming C):** Chiamate di sistema a basso livello, I/O su file e descrittori, gestione dei processi (`fork`, `exec`, `wait`), segnali asincroni, comunicazione tra processi (Pipe, FIFO, `mmap`), multithreading concorrente POSIX (`pthread`, mutex, condition variable, semafori), e programmazione di rete socket (TCP, UDP, `select`, I/O multiplexing, Broadcast e Multicast).
3. **Virtualizzazione e Tecnologie a Container:** Meccanismi del kernel Linux per l'isolamento (Namespaces, Cgroups, OverlayFS), creazione di container via software, utilizzo di Docker e Docker Compose, gestione dei vincoli di risorsa.

### 0.2 Modalità d'Esame e Criteri di Valutazione
L'esame di Laboratorio di Sistemi Operativi è composto da **due parti obbligatorie**:
1. **Prova Scritta / Laboratorio al Calcolatore:**
   - Esercizi di scripting su pipeline di comandi Unix, script Bash, filtri `sed` e `awk`.
   - Esercizi di programmazione di sistema in C: fork, exec, pipe, multithreading, sincronizzazione con mutex/condition variable, programmazione socket con `select()`.
   - Quesiti teorici e di analisi di codice ("trova l'errore" `_ERR`, tracing dell'albero dei processi, calcolo valori stampati e rilevamento di deadlock/race condition).
2. **Progetto Linux (Architettura Client-Server):**
   - Realizzazione di un'applicazione software completa in C in ambiente Unix (tipicamente un sistema distribuito client-server concorrente basato su socket, thread o processi).
   - Redazione di una **relazione tecnica** allegata che documenti l'architettura, le problematiche affrontate, le scelte implementative e i protocolli adottati.
   - I progetti vengono svolti in **gruppi composti da al più 2 studenti** (ammessi singoli solo in casi particolari, es. studenti lavoratori).
   - **Discussione Orale del Progetto:** Tutti i membri del gruppo devono partecipare alla discussione congiunta del codice, illustrando i dettagli implementativi e rispondendo a domande teoriche collegate.
   - **Regola di consegna:** Il progetto deve essere consegnato prima della prova scritta (con deroga per la sessione anticipata/primo appello). Il voto finale valuta congiuntamente lo scritto e il progetto.

### 0.3 Prove Parziali vs Appelli Ordinari
- **Prove Parziali (PA):** Durante il semestre di lezione vengono erogate prove intermedie parziali al calcolatore. Il superamento delle prove parziali esonera lo studente dalla prova scritta complessiva negli appelli della sessione d'esame.
- **Appelli Ordinari:** Negli appelli ufficiali (Gennaio/Febbraio, Giugno/Luglio, Settembre) è possibile sostenere la prova scritta completa in un'unica soluzione. I membri di uno stesso gruppo di progetto possono sostenere la prova scritta in appelli diversi, ferma restando la discussione congiunta del progetto.

### 0.4 Ambiente di Sviluppo di Riferimento
- **Sistema Operativo:** Distribuzione Linux raccomandata **Ubuntu LTS** (22.04 LTS o 24.04 LTS) o Debian.
- **Toolchain di Compilazione e Debug:**
  - Compilatore: `gcc` (GNU Compiler Collection) con flag raccomandati `-Wall -Wextra -pthread`.
  - Debugger e Tracing: `gdb`, `strace`, `valgrind` (rilevamento memory leak).
  - Shell: GNU `bash` versione 5+.
- **Opzioni di Configurazione su PC Personale:**
  1. *Dual Boot nativo:* Partizione Linux dedicata (consigliata per le massime prestazioni e pieno accesso alle chiamate di sistema del kernel).
  2. *Macchina Virtuale:* Linux ospitato tramite VirtualBox, VMware Workstation o KVM.
  3. *WSL2 (Windows Subsystem for Linux 2):* Ubuntu su WSL2 con supporto a systemd e Docker Desktop abilitato.

---

## 1. Introduzione ai Sistemi Operativi e Unix
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 1.1 Cos'è un Sistema Operativo

Il **Sistema Operativo (SO)** è un programma che:
- **Gestisce le risorse** di un calcolatore (CPU, memoria, periferiche)
- Fa da **intermediario** tra l'utente e l'hardware
- Rende **efficace ed efficiente** l'utilizzo del calcolatore

Il SO ha tre ruoli fondamentali:

| Ruolo | Descrizione |
|-------|-------------|
| **Allocatore di risorse** | Gestisce tutte le risorse, decide tra richieste conflittuali |
| **Programma di controllo** | Controlla l'esecuzione dei programmi, evita errori e usi impropri |
| **Macchina estesa** | Fornisce un'astrazione uniforme per l'esecuzione dei programmi |

### 1.2 Il Kernel

Il **kernel** è il nucleo del sistema operativo:
- È il **solo programma** eseguito in **modalità privilegiata** (accesso completo all'hardware)
- Tutti gli altri programmi sono eseguiti in **modalità protetta** (user mode)
- Gestisce: CPU, memoria, periferiche

**Risorse gestite dal kernel:**
- **Gestione processi**: creazione/cancellazione, sospensione/ripresa, sincronizzazione, comunicazione, deadlock
- **Gestione memoria**: spazi di indirizzamento virtuali, allocazione/deallocazione, protezione
- **Gestione file**: creazione/cancellazione, mapping in memoria secondaria, backup
- **Gestione I/O**: interfaccia uniforme verso i dispositivi
- **Gestione cache**: utilizzo del dato più recente

### 1.3 Storia di Unix

| Anno | Evento |
|------|--------|
| 1965 | Bell Labs lavora su **Multics** (multi-utente, multi-processo, file system gerarchico) |
| 1969 | AT&T abbandona Multics. Thompson, Ritchie, Canaday e McIlroy creano **Unix** |
| 1 Gen 1970 | **Inizio di Unix** (epoch) |
| 1973 | Unix **riscritto in C** da Dennis Ritchie |

**Caratteristiche di Unix:**
- Architettura semplice ed elegante
- Ambiente di programmazione (implementato in C)
- Indipendenza dall'hardware
- **Architettura monolitica** del kernel

### 1.4 Architettura di Sistema Unix

Il kernel si occupa di:
- **CPU**: Lo *scheduler* stabilisce quale processo mandare in esecuzione
- **Memoria**: *Memoria virtuale* — ogni processo ha il suo spazio di indirizzi virtuale
- **Periferiche**: Interfaccia astratta — **"everything is a file"** (le interfacce di rete fanno eccezione)

### 1.5 System Call e Libreria Standard C

Le **chiamate al sistema (system call)** sono l'interfaccia con cui i programmi accedono all'hardware:
- Il programma chiama una funzione → viene generata un'interruzione → il controllo passa dal programma al kernel
- Le system call sono rimappate in funzioni della **Libreria Standard del C**

**Meccanismo della system call** (esempio `read(fd, buffer, nbytes)`):
1. Il programma mette i parametri nello stack (passi 1–3)
2. La procedura di libreria mette il codice della syscall in un registro (passo 4)
3. Istruzione **TRAP** → passaggio al kernel mode (passi 5-6)
4. Il kernel trova l'handler tramite una tabella (passi 7-8)
5. Finita l'esecuzione, il risultato torna al chiamante (passo 9)
6. Il chiamante riprende in user mode e pulisce lo stack (passi 10-11)

> **Nota**: Le procedure POSIX non si mappano sempre uno a uno con le system call. Alcune operano senza fare TRAP al kernel.

### 1.6 Sistema Multi-utente

- Ogni utente ha uno **username** e un **uid** (user id numerico)
- Esiste l'utente speciale **root** (superuser, uid = 0)
- Meccanismi di **permessi e protezioni** impediscono che utenti diversi si danneggino

---

## 2. Il File System Unix
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 2.1 Caratteristiche Generali

- **Struttura gerarchica** (albero con radice `/`)
- I file sono **sequenze di byte** (byte streams) senza struttura imposta dal sistema
- Protezione da accessi non autorizzati

> *"On a UNIX system, everything is a file; if something is not a file, it is a process."*

### 2.2 Tipi di File

| Tipo | Descrizione |
|------|-------------|
| **File ordinari** | Sequenze di byte (dati, sorgenti, eseguibili...) |
| **Directory** | Contengono *directory entries*: associazioni tra i-number e filename |
| **File speciali** | Rappresentano dispositivi di I/O |

Ogni file ha un **i-number** (index number) univoco che identifica il suo **i-node** (che contiene i metadati).

### 2.3 i-node e Metadati

L'**i-node** (file descriptor interno) contiene:
- Tipo di file, permessi
- Proprietario (UID) e gruppo (GID)
- Dimensione, numero di link
- Timestamp: ultimo accesso, ultima modifica, ultimo cambio metadati, creazione (se disponibile)
- Puntatori ai blocchi dati su disco

### 2.4 Directory

- Contengono *directory entries*: associazione tra **i-number** (usati dal sistema) e **filename** (usati dall'utente)
- Ogni directory ha almeno due entry: `.` (sé stessa) e `..` (directory padre)
- Un file può avere **più nomi** (link) ma sempre **un solo i-number**

### 2.5 Pathname

- **Pathname assoluto**: cammino dalla root `/` al file (es. `/home/lso/lezione1`)
- **Pathname relativo**: cammino dalla *working directory* corrente (es. `lezione1/file.txt`)

### 2.6 Directory Tipiche

| Directory | Contenuto |
|-----------|-----------|
| `/bin` | Comandi eseguibili |
| `/dev` | File speciali (dispositivi I/O) |
| `/etc` | File di configurazione e amministrazione (es. `/etc/passwd`) |
| `/lib` | Librerie di programmi |
| `/tmp` | Area temporanea |
| `/home` | Home directory degli utenti |
| `/usr` | Programmi, librerie, documentazione per utenti |

### 2.7 Home e Working Directory

- **Home directory**: assegnata dall'amministratore, abbreviata con `~`
- **Working directory**: la directory corrente, inizialmente la home dopo il login, cambiabile con `cd`

### 2.8 Permessi dei File

Ogni file ha tre categorie di permessi, ciascuna con tre diritti:

```
rwx rwx rwx
│   │   └── Others (altri utenti)
│   └────── Group (gruppo del proprietario)
└────────── Owner (proprietario)
```

| Simbolo | Significato per file | Significato per directory |
|---------|---------------------|-------------------------|
| `r` | Leggere il contenuto | Elencare il contenuto |
| `w` | Scrivere/modificare | Creare/cancellare file |
| `x` | Eseguire | Accedere (attraversare) |

**Permessi iniziali:**
- File ordinari non eseguibili: `rw-rw-rw-` (666)
- File eseguibili e directory: `rwxrwxrwx` (777)

**Rappresentazione ottale:** ogni gruppo di 3 bit = un numero 0-7

```
rwx = 7, rw- = 6, r-x = 5, r-- = 4, -wx = 3, -w- = 2, --x = 1, --- = 0
```

### 2.9 Link

**Hard link** (`ln name1 name2`):
- Crea un nuovo nome (link) per un file esistente
- Tutti i link hanno identico status — non si distingue l'originale dai link
- Non possono essere fatti tra file system diversi
- Non possono linkare directory

**Link simbolico** (`ln -s name1 name2`):
- Crea un file speciale che **contiene il path** del file linkato
- Può linkare directory
- Può linkare tra file system diversi
- Il link ha il suo proprio i-node

### 2.10 File System Montabile

- Un file system deve essere **montato** (`mount`) per essere accessibile
- Viene montato su un **punto di montaggio** (directory vuota)
- Va **smontato** (`umount`) prima di rimuovere il supporto

### 2.11 Comando `stat`

Mostra informazioni dettagliate di un file (metadati dall'inode):

| Campo | Significato |
|-------|-------------|
| `Size` | Dimensione logica in byte |
| `Blocks` | Spazio su disco in blocchi da 512 B |
| `IO Block` | Dimensione blocco di I/O del filesystem |
| `Type` | Tipo file (regular, directory, symlink…) |
| `Device / Inode` | Identificatori interni |
| `Links` | Numero di hard link |
| `Access` | Permessi (numerici e simbolici) |
| `Uid / Gid` | Proprietario (utente e gruppo) |
| Timestamps | Access, Modify, Change, Birth |

---

## 3. La Shell Bash
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 3.1 Cos'è la Shell

La **shell** è un **interprete di comandi** che si interpone tra l'utente e il SO. Funzionalità:
- Gestione del **main command loop**
- Analisi sintattica dei comandi
- Esecuzione di comandi *built-in*, file eseguibili e *script*
- Gestione di **standard I/O** e **standard error**
- Gestione dei processi da terminale

**Shell disponibili:**

| Shell | Path | Origine |
|-------|------|---------|
| Bourne shell | `/bin/sh` | Bell Labs |
| **Bourne-again shell** | `/bin/bash` | Linux |
| C shell | `/bin/csh` | Berkeley |
| Korn shell | `/bin/ksh` | Bell Labs |
| TENEX C shell | `/bin/tcsh` | BBN tech |

### 3.2 File di Inizializzazione

- Shell **interattiva, non login** → legge `~/.bashrc`
- Shell **interattiva, login** → legge uno tra: `/etc/profile`, `~/.bash_profile`, `~/.bash_login`, `~/.profile`

### 3.3 Variabili di Shell

```bash
# Definizione (SENZA spazi intorno a =)
a=3

# Lettura
echo $a
echo ${a}
```

**Variabili di ambiente predefinite:**

| Variabile | Significato |
|-----------|-------------|
| `HOME` | Path della home directory |
| `PATH` | Path di ricerca degli eseguibili |
| `PS1` | Stringa del prompt (`$` per utente, `#` per root) |
| `HOSTNAME` | Nome del computer |
| `SHELL` | Shell corrente |

### 3.4 Formato dei Comandi

```
comando [argomento ...]
```

- Gli argomenti possono essere **opzioni/flag** (preceduti da `-`) o **parametri**
- L'ordine delle opzioni è generalmente **irrilevante**
- L'ordine dei parametri è generalmente **rilevante**
- **Unix è CASE SENSITIVE**

### 3.5 Redirezione I/O

Ogni programma ha 3 canali di comunicazione:

| Canale | Codice | Default |
|--------|--------|---------|
| Standard input | 0 | Tastiera |
| Standard output | 1 | Schermo |
| Standard error | 2 | Schermo |

**Operatori di redirezione:**
Redireziona l'output di un processo verso un FILE non verso un altro processo.

```bash
# Redirezione output (sovrascrive)
ls -a > listaFile.txt

# Redirezione output (append)
echo $PATH >> listaFile.txt

# Redirezione input
sort < dati.txt

# Redirezione errori
comando 2> errori.txt

# Redirezione output + errori
comando > output.txt 2>&1

# Here document
cat <<EOF
testo
EOF
```

### 3.6 Pipe

La **pipe** (`|`) collega l'output di un comando all'input di un altro comando:

```bash
cat file | sort          # ordina il contenuto del file
ls | less                # pagina la lista dei file
ls | grep -v pluto | tail -3 | head -1  # come funziona e cosa fa verra' spiegato successivamente
```

### 3.7 Metacaratteri e Wildcard

| Metacarattere | Significato |
|---------------|-------------|
| `*` | Qualsiasi sequenza di caratteri (anche vuota) |
| `?` | Un singolo carattere qualsiasi |
| `[abc]` | Un carattere tra quelli elencati |
| `[a-z]` | Un carattere nell'intervallo |
| `[!abc]` o `[^abc]` | Un carattere NON tra quelli elencati |

### 3.8 Quoting

| Tipo | Effetto |
|------|---------|
| `\` (backslash) | Protegge il carattere successivo |
| `'...'` (apici singoli) | Protegge tutto il contenuto |
| `"..."` (apici doppi) | Protegge tutto tranne `$`, `` ` `` e `\` |

---

## 4. Comandi Unix Fondamentali
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 4.1 Gestione Directory

| Comando | Funzione |
|---------|----------|
| `mkdir [-p] [-m mode] dir` | Crea directory (`-p`: crea percorsi intermedi se mancanti, `-m`: imposta permessi iniziali) |
| `rmdir [-p] dir` | Rimuove directory vuota (`-p`: rimuove anche i percorsi genitori se vuoti) |
| `pwd` | Stampa la working directory attuale (`Print Working Directory`)| 
| `cd [dir]` | Cambia directory (senza argomenti → home) |
| `ls [opzioni] [dir]` | Elenca contenuto directory (`-a`: file nascosti, `-l`: formato esteso, `-s`: dimensione in blocchi, `-t`: ordine per data modifica, `-R`: ricorsivo, `-F`: aggiunge `/` a dir e `*` ad eseguibili, `-i`: mostra i-number) |
| `du [-s] [-k] file` | Mostra spazio disco utilizzato (`-s`: solo totale/sommario, `-k`: mostra taglia in Kilobytes) |

### 4.2 Gestione File

| Comando | Funzione |
|---------|----------|
| `cp [-r] [-i] source target` | Copia file/dir (`-r`: ricorsivo, `-i`: chiedi conferma prima di sovrascrivere) |
| `mv [-i] source dest` | Sposta/rinomina file (`-i`: chiedi conferma) |
| `rm [-i] [-R] file` | Rimuove file/dir (`-i`: chiedi conferma, `-R` o `-r`: rimuovi dir e contenuto) |
| `touch [-a] [-c] [-m] file` | Aggiorna timestamp o crea vuoto (`-a`: solo accesso, `-m`: solo modifica, `-c`: non creare nuovo) |
| `file filename` | Determina il tipo di file |
| `ln [-s] name1 name2` | Crea link (hard di default, `-s`: crea link simbolico) |
| `chmod permissions file` | Cambia permessi |
| `chown user[:group] file` | Cambia proprietario/gruppo |
| `find path -name "pattern"` | Cerca file ricorsivamente |

> [!IMPORTANT]
> **Uso delle virgolette in find:** Quando si usa `find -name`, è essenziale racchiudere il pattern di ricerca (ad esempio `"*.txt"`) tra virgolette. Se non lo si fa, la shell espanderà `*.txt` *prima* di passare l'argomento a `find`, causando comportamenti inattesi o errori.

**Esempio completo con opzioni:**
```bash
# Cerca nella cartella /home (e sottocartelle) tutti i file chiamati "*.sh", chiedendo conferma per cancellarli
find /home -name "*.sh" -exec rm -i {} \;
```

### 4.3 Comandi di Utilità su Testo <span style="float:right; font-size: 0.6em;">[Torna all'indice](#indice)</span>

| Comando | Funzione |
|---------|----------|
| `cat [-n] [-b] file` | Concatena e visualizza (`-n`: numera tutte le righe, `-b`: numera solo righe non vuote) |
| `wc [-l] [-w] [-c] file` | Conta: righe (`-l`), parole (`-w`), caratteri (`-c`) |
| `cut -d: -f1,5 file` | Estrae colonne (`-d:`: definisce delimitatore, `-f`: indica quali campi/colonne estrarre) |
| `paste file1 file2` | Compone file affiancandoli |
| `sort [-n] [-r] [-t:] [-k] file` | Ordina righe (`-n`: numerico, `-r`: inverso, `-t:`: delimitatore, `-k`: numero del campo da usare come chiave di ordinamento) |
| `uniq [-c] [-d] [-u] [-i] file` | Rimuove o segnala duplicati adiacenti (`-c`: conta occorrenze, `-d`: solo duplicati, `-u`: solo unici, `-i`: ignora maiusc./minusc.) |
| `diff file1 file2` | Mostra differenze tra file |
| `head [-n N] file` | Visualizza le prime N righe |
| `tail [-n N] file` | Visualizza le ultime N righe |

> [!IMPORTANT]
> **Funzionamento di `uniq` e combinazione con `sort`:**  
> Il comando `uniq` opera **esclusivamente su righe adiacenti (consecutive)**: non individua duplicati sparsi se non si trovano su righe contigue. Per questo motivo, per eliminare o contare i duplicati di un intero file o stream, viene quasi sempre posto in pipeline a valle di `sort`:
> ```bash
> sort file.txt | uniq       # Rimuove tutti i duplicati (mantiene una sola istanza)
> sort file.txt | uniq -c    # Conta le occorrenze di ciascuna riga (formato: "  <conteggio> <riga>")
> sort file.txt | uniq -d    # Mostra SOLO le righe duplicate
> sort file.txt | uniq -u    # Mostra SOLO le righe uniche (che compaiono 1 sola volta)
> ```
> Nelle prove d'esame (es. [Capitolo 25](#25-corpus-dei-file-pratici-desame-debugging-_err-template-_todo-e-prove-parziali)), la combinazione `sort | uniq -c | awk '$1 >= N {print $2}'` è il pattern fondamentale per estrarre elementi con frequenza maggiore o uguale a una certa soglia.

### 4.4 Listing di Processi

```bash
ps                    # processi del terminale corrente
ps -f                 # informazioni complete (UID, PID, PPID, CMD...)
ps -ef                # tutti i processi con dettagli
ps aux                # tutti (BSD style, con stato e % CPU/MEM)
pstree                # albero dei processi
top                   # visualizzazione dinamica in tempo reale
htop                  # versione interattiva di top
```

**Campi di `top`:**
- `PR` → priorità, `NI` → nice value
- `VIRT` → memoria virtuale, `RES` → memoria residente, `SHR` → memoria condivisa
- `S` → stato (R=running, S=sleeping, T=stopped, Z=zombie)
- `%CPU`, `%MEM`, `TIME+`, `COMMAND`

### 4.5 Il file `/etc/passwd`

Database degli utenti. Formato di ogni riga:
```
Username:Password:UserID:GroupID:Info:HomeDirectory:Shell
```
Esempio: `root:x:0:0:root:/root:/bin/bash`

---

## 5. Grep e le Espressioni Regolari
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 5.1 Il Comando `grep`

```bash
grep [opzioni] "pattern" [file]
```

Stampa le righe che corrispondono al pattern. Se non si specifica un file, legge da stdin (utilizzabile in pipe).

> [!IMPORTANT]
> **Uso delle virgolette:** È fondamentale racchiudere sempre il pattern tra virgolette doppie (`"..."`) o apici singoli (`'...'`). Questo impedisce alla shell di interpretare spazi o caratteri speciali al loro interno (ad esempio il carattere `*` o lo spazio) prima che vengano passati al comando.
> 
> *Consiglio:* Di norma è preferibile usare i **singoli apici (`'...'`)** per le regex di grep, per evitare del tutto interferenze della shell. Usa i **doppi apici (`"..."`)** solo se hai bisogno che Bash espanda una variabile (es. `$MIOPATTERN`) all'interno della ricerca.

**Esempio completo con opzioni:**
```bash
# Cerca "errore di sistema" ignorando il case (-i) e mostrando i numeri di riga (-n) nel file syslog
grep -in "errore di sistema" /var/log/syslog
# Oppure in alternativa separando le opzioni (non c'è differenza in output)
grep -i -n "errore di sistema" /var/log/syslog
```

**Opzioni principali:**

| Opzione | Significato |
|---------|-------------|
| `-v` | Stampa le righe che **non** corrispondono |
| `-c` | Solo il numero di occorrenze |
| `-i` | Case-insensitive |
| `-n` | Mostra numero di riga |

### 5.2 Espressioni Regolari Base (BRE)

| Sintassi | Significato |
|----------|-------------|
| `.` | Qualunque carattere |
| `exp*` | Zero o più occorrenze di exp |
| `^exp` | exp a inizio riga |
| `exp$` | exp a fine riga |
| `[a-z]` | Un carattere nell'intervallo |
| `[^a-z]` | Un carattere fuori dall'intervallo |
| `\<exp` | exp a inizio parola |
| `exp\>` | exp a fine parola |
| `exp\{N\}` | exp compare esattamente N volte |
| `exp\{N,\}` | exp compare almeno N volte |
| `exp\{N,M\}` | exp compare da N a M volte |

> [!NOTE]
> **A cosa serve il backslash (`\`) in grep?**
> In `grep` (che di default usa le espressioni regolari "Base" o BRE), il backslash ha un ruolo fondamentale e bidirezionale:
> 
> 1. **Attivare caratteri speciali (Metacaratteri):** Caratteri come `{`, `}`, `<`, `>` o `(` vengono interpretati da `grep` come testo normale. Per fargli assumere il loro significato speciale, devi farli precedere dal backslash.
>    - *Esempio:* `grep "A{3}" file` cercherà letteralmente la stringa "A{3}". Invece `grep "A\{3\}" file` cercherà la lettera "A" ripetuta esattamente 3 volte ("AAA").
> 2. **Disattivare caratteri speciali (Escape):** Al contrario, caratteri come `.`, `*`, `^` e `$` hanno un significato speciale di default. Se vuoi cercare nel testo il simbolo letterale (es. un vero punto), devi usare il backslash per "scappare" (escape) dal suo significato speciale.
>    - *Esempio:* `grep "file.txt" file` cercherà "file-txt", "file0txt" (perché il punto vale "qualsiasi carattere"). Invece `grep "file\.txt" file` cercherà solo l'esatta stringa "file.txt".

**Classi POSIX:**
- `[[:alpha:]]` → caratteri alfabetici
- `[[:alnum:]]` → alfanumerici
- `[[:digit:]]` → cifre
- `[[:upper:]]` → maiuscole
- `[[:lower:]]` → minuscole

### 5.3 Espressioni Regolari Estese (ERE)

In `grep` si usano con backslash (`\+`, `\|`, `\(...\)`). In `egrep` si usano direttamente senza l'uso del backslash.

Sintassi usabili solamente in `egrep`:

| Sintassi | Significato |
|----------|-------------|
| `exp+` | Una o più occorrenze |
| `exp?` | Zero o una occorrenza |
| `exp{n,m}` | Da n a m occorrenze |
| `exp1 \| exp2` | exp1 oppure exp2 |
| `( exp )` | Raggruppamento |

### 5.4 Esempi Pratici

> [!CAUTION]
> **Attenzione alla Brace Expansion in Bash con `{n,m}`**:
> Quando usi il moltiplicatore numerico, è **obbligatorio** racchiudere l'intera espressione in apici singoli (es. `egrep '1{1,2}'`). Se lasci le parentesi graffe fuori (es. `egrep '1'{1,2}`), Bash le espanderà prima di eseguire il comando (diventando `egrep 11 12`), causando errori come `grep: 12: File non trovato`.
> 
> Ricorda inoltre che `grep` cerca **sottostringhe** nell'intera riga: `egrep '1{1,2}'` applicato a `ls -l` stamperà quasi tutte le righe perché troverà sempre l'1 del numero di hard-link, e "strapperà" porzioni valide anche da stringhe più lunghe (es. in `111` troverà `11`). Per un match esatto si devono usare i word boundaries (`\b1{1,2}\b`) o ancoraggi.


```bash
# Righe che iniziano con 'a' e finiscono con 'b'
grep '^a.*b$' file

# File con permesso di esecuzione per il proprietario
ls -l | grep '^-..x'

# Directory con nome che inizia per maiuscola
ls -d */ | grep '^[[:upper:]]'

# Utenti che usano bash come shell
grep 'bash$' /etc/passwd

# File .txt nella directory corrente e sottodirectory
ls -R | grep '\.txt$'
# oppure
find . -name '*.txt'
```

---

## 6. Script Shell
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 6.1 Struttura Base

Uno script Bash è un file di testo che:
1. Inizia con `#!/bin/bash` (shebang)
2. Ha il **permesso di esecuzione** (`chmod +x script.sh`)
3. Contiene comandi di shell

```bash
#!/bin/bash
echo "Hello world!"
ls
```

### 6.2 Variabili negli Script

In Bash, le variabili si assegnano **senza spazi attorno al segno di uguale (`=`)**. 
Se si inseriscono spazi, Bash interpreterà la prima parola come un comando da eseguire.

> [!WARNING]
> **Corretto:** `ris=$(somma 4 7)` oppure `nome="Paolo"`
> **Sbagliato:** `ris = $(somma 4 7)` *(restituisce l'errore `comando non trovato`)*

#### Variabili Predefinite

| Variabile | Significato |
|-----------|-------------|
| `$0` | Nome dello script (argv[0]) |
| `$1` … `$9` | Parametri da riga di comando |
| `$#` | Numero di parametri ricevuti |
| `$*` | Tutti i parametri in una singola stringa |
| `$@` | Tutti i parametri in stringhe separate |
| `$$` | PID del processo corrente |
| `$?` | Exit status dell'ultimo comando |

**Differenza tra `$*` e `$@`:**
- `"$*"` → `"1 2 3"` (una sola stringa)
- `"$@"` → `"1" "2" "3"` (stringhe separate)

### 6.3 Exit Status

- Ogni comando restituisce un **exit status** (intero)
- `0` = terminazione regolare (successo)
- Diverso da `0` = errore
- `$?` contiene l'exit status dell'ultimo comando

### 6.4 Operatori su Comandi

```bash
cmd1; cmd2          # esegue cmd1 poi cmd2
cmd1 && cmd2        # esegue cmd2 solo se cmd1 ha successo (exit = 0)
cmd1 || cmd2        # esegue cmd2 solo se cmd1 fallisce    (exit != 0)
```

### 6.5 Strutture di Controllo

**if-then-else:** attenzione all'uso di `if` e `fi`
```bash
if comando; then
    lista_comandi
elif comando; then
    lista_comandi
else
    lista_comandi
fi
```

**Espressioni condizionali** (`test exp` o `[ exp ]`):

| Tipo | Operatori |
|------|-----------|
| Stringhe | `==`, `!=`, `-z` (stringa vuota) |
| Interi | `-lt`, `-le`, `-eq`, `-ne`, `-ge`, `-gt` |
| File | `-e` (esiste), `-f` (file regolare), `-d` (directory), `-r`, `-w`, `-x` (permessi) |
| Logici / Negazione | `!` (NOT / negazione logica), `-a` (AND), `-o` (OR) |

> **Nota sull'operatore di negazione (`!`):** Inverte il valore di verità della condizione successiva (es. `[ ! -e "$1" ]` risulta vero se il file `$1` **non** esiste).

```bash
if [ $# -lt 4 ]; then
    echo "Servono 4 argomenti."
    exit 1
elif [ ! -e "$1" ]; then
    echo "Il file $1 non esiste."
    exit 1
fi
```

**while:** (esegue finché la condizione è **VERA**)
```bash
i=0
while [ $i -lt 10 ]; do
    i=$(( i + 1 ))
done
```

**until** (esegue finché la condizione è **FALSA**):
```bash
COUNTER=20
until [ $COUNTER -lt 10 ]; do
    echo COUNTER: $COUNTER
    COUNTER=$((COUNTER-1))
done
```

**for:**
```bash
for a in 1 2 3; do   output: argomenti su righe separate
    echo $a
done

for a in $(ls); do    output: argomenti su righe separate
    echo $a
done

for a in "$@"; do output: argomenti su righe separate
    echo $a
done

for a in "$*"; do output: argomenti su una sola stringa
    echo $a
done

for a in *.txt; do    output: file .txt nella directory corrente
    echo $a
done
```

**case:**
```bash
case $variabile in
    pattern1) comandi ;;
    pattern2) comandi ;;
    *) comandi_default ;;
esac
```

### 6.6 Sostituzione Aritmetica

```bash
a=7
echo $(( a + 1 ))         # 8
echo $(( a * 3 > 8 ))     # 1 (vero)
echo $(( a++ ))  echo $a  # 7, poi a diventa 8
```

Operatori: `+`, `-`, `/`, `*`, `%`, `**`, `<<`, `>>`, `&`, `|`, `~`, `<`, `<=`, `==`, `!=`, `>`, `>=`, `&&`, `||`, `!`

---

## 7. Sed e Awk
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 7.1 Sed — Stream Editor

**sed** è un editor non interattivo di file di testo. **NON MODIFICA L'INPUT** — l'output va allo stdout.
Per modificare il file di testo si deve **reindirizzare l'output in un altro file ( > )** o usare il comando **`-i`**.

**Sintassi:** `sed [opzioni] 'comando' [file]`

> [!IMPORTANT]
> **Uso degli apici/virgolette:** Come per grep, è vitale racchiudere il comando di `sed` (ad esempio la stringa di sostituzione) tra apici singoli (`'...'`) o virgolette doppie (`"..."`) per evitare che la shell modifichi o interpreti i caratteri speciali prima dell'esecuzione.

**Esempio completo con opzioni:**
```bash
# Sostituisce tutte le occorrenze (-e 's/.../g') e lo fa senza stampare l'output standard (-n)
# ma stampando solo le righe in cui è avvenuta la sostituzione (/p). 
sed -n -e 's/"vecchio valore"/"nuovo valore"/gp' file.txt
```

**Opzioni principali:**
* `-n`: Sopprime l'output automatico (di default `sed` stampa ogni riga processata). Utile in combinazione con il comando `p` per stampare solo le righe modificate o cercate.
* `-e`: Permette di concatenare più comandi `sed` (es. `sed -e 'comando1' -e 'comando2'`).
* `-f script_file`: Legge i comandi `sed` da un file specificato.
* `-i`: Modifica il file direttamente (*in-place*) sovrascrivendo l'originale.

**Funzionamento:**
1. Copia ciclicamente una linea di input nel **pattern space**
2. Applica tutti i comandi con address selezionati
3. Copia il pattern space sullo stdout
4. Cancella il pattern space

**Comandi principali:**

| Comando | Significato |
|---------|-------------|
| `d` | Cancella linea |
| `p` | Stampa linea |
| `s/old/new/` | Sostituisce prima occorrenza |
| `s/old/new/g` | Sostituisce tutte le occorrenze |
| `a\` | Append testo dopo la riga corrente |
| `i\` | Inserisci testo prima della riga corrente |

**Indirizzamento:**
- Nessun indirizzo → ogni linea
- Numero di riga → `sed '1d' file` (cancella riga 1)
- Range → `sed '2,4d' file` (cancella righe da 2 a 4)
- Regex → `sed '/^#/d' file` (cancella righe che iniziano con #)
- `$` → ultima riga

**Esempi fondamentali:**
```bash
# Cancella tutte le righe
sed 'd' file

# Cancella righe da 1 a 10
sed '1,10d' file

# Cancella commenti
sed '/^#/d' file

# Cancella righe vuote
sed '/^$/d' file

# Stampa solo righe che contengono "errore"
sed -n '/errore/p' file

# Sostituzione
sed 's/erore/errore/g' file

# Sostituzioni multiple
sed -e 's/erore/errore/g' -e 's/^/> /g' file

# Cancella parola
sed 's/parola//g' file

# Aggiunge indentazione, redirige l'output in file.indent
sed 's/^/   /' file > file.indent

# Il simbolo & nella stringa di sostituzione rappresenta l'esatto testo trovato dal match.
# In questo caso, .* (espressione regolare per "tutta la riga") trova l'intera riga. 
# Quindi & viene sostituito con il contenuto originale della riga stessa.
# Effetto pratico: aggiunge il prefisso "lui dice: " a ogni riga del file.
sed -e 's/.*/lui dice: &/' file

# Un altro esempio per capire meglio: se vuoi mettere tra parentesi quadre la parola 'errore'
# ovunque si trovi, & prenderà esattamente la parola trovata.
sed 's/errore/[&]/g' file
```

### 7.2 Awk — Linguaggio di Elaborazione Testuale

**awk** è un vero e proprio **linguaggio di programmazione** interpretato, progettato per elaborare file di testo strutturati a campi. Ogni riga del file viene chiamata **record**, ogni "parola" separata dal delimitatore è un **campo**.

**Sintassi base:**
```bash
awk 'programma' file          # programma inline
awk -f script.awk file        # programma da file
awk -F: 'programma' file      # specifica il delimitatore (qui ':')
```

> [!IMPORTANT]
> **Uso degli apici:** In `awk` è imperativo racchiudere il codice inline del programma tra apici singoli (`'...'`). Questo perché il codice `awk` utilizza spesso caratteri come `$1` o `$` che la shell di bash interpreterebbe come variabili d'ambiente (espandendole o sostituendole) ancor prima di passare il programma ad `awk`.

**Esempio completo con opzioni:**
```bash
# Assegna una variabile interna awk usando -v e definisce un diverso separatore di campo con -F
awk -v limite=1000 -F: '$3 > limite { print "Utente:", $1, "ID:", $3 }' /etc/passwd
```

---

#### Struttura di un programma awk

Un programma awk è composto da **blocchi** nella forma `pattern { azione }`.
Awk legge il file riga per riga e, per ogni riga, esegue le azioni di tutti i blocchi il cui pattern è soddisfatto.

```
BEGIN  { ... }      # eseguito UNA volta, prima di leggere qualsiasi riga
/regex/ { ... }     # eseguito per ogni riga che matcha la regex
expr    { ... }     # eseguito per ogni riga in cui l'espressione è vera
END    { ... }      # eseguito UNA volta, dopo aver letto tutte le righe
```

Se manca il pattern, l'azione viene eseguita per **ogni riga**.

```bash
# Esempio: report su /etc/passwd
awk -F: '
    BEGIN { print "=== Utenti del sistema ===" }
    { print "Utente:", $1, "| Shell:", $NF }
    END   { print "Totale righe:", NR }
' /etc/passwd
```

---

#### Variabili predefinite

| Variabile | Significato |
|-----------|-------------|
| `$0` | L'intera riga corrente |
| `$1`, `$2`, ..., `$NF` | Il campo 1, 2, ..., ultimo |
| `NF` | Numero di campi nella riga corrente |
| `NR` | Numero di record (righe) letti finora (globale) |
| `FNR` | Numero del record (riga) lette nel file corrente (si azzera ad ogni nuovo file) |
| `FS` | Field Separator in input (default: spazio/tab) |
| `OFS` | Output Field Separator (default: spazio) |
| `RS` | Record Separator in input (default: `\n`) |
| `ORS` | Output Record Separator (default: `\n`) |
| `FILENAME` | Nome del file corrente in elaborazione |

```bash
# OFS: cambia il separatore di output
awk -F: 'BEGIN { OFS=" -> " } { print $1, $3 }' /etc/passwd
# output: root -> 0

# NF: stampa solo l'ultimo campo di ogni riga
awk '{ print $NF }' file.txt
# output: stampa esclusivamente l'ultima parola di ogni riga

# FNR vs NR con due file
awk '{ print FILENAME, FNR, NR, $0 }' file1.txt file2.txt
# output es:
# file1.txt 1 1 prima_riga_di_file1
# file1.txt 2 2 seconda_riga_di_file1
# file2.txt 1 3 prima_riga_di_file2
# file2.txt 2 4 seconda_riga_di_file2
# (FNR si azzera a ogni nuovo file, NR è globale e cresce sempre)
```

---

#### Variabili utente e operatori

Puoi creare le tue variabili in qualsiasi blocco. Non serve dichiararle: le variabili numeriche partono da `0`, quelle stringa da `""`.

```bash
# Conta le righe più lunghe di 80 caratteri
awk '{ if (length($0) > 80) count++ } END { print count }' file.txt

# Somma il terzo campo di tutte le righe
awk '{ somma += $3 } END { print "Totale:", somma }' dati.txt

# Media
awk '{ somma += $1; n++ } END { print "Media:", somma/n }' numeri.txt
```

**Operatori disponibili:** `+`, `-`, `*`, `/`, `%`, `^` (potenza), `++`, `--`, `+=`, `-=`, `==`, `!=`, `<`, `>`, `<=`, `>=`, `&&`, `||`, `!`, `? :` (ternario).

---

#### Condizionali e Cicli

Awk supporta le stesse strutture di controllo del C.

```bash
# if / else if / else
awk '{
    if ($3 > 1000)
        print $1, "-> ricco"
    else if ($3 > 500)
        print $1, "-> medio"
}' stipendi.txt

# while

# for (stile C)

# for ... in (itera sugli array associativi)
awk '{
    conteggio[$1]++
} END {
    for (parola in conteggio)
        print parola, "appare", conteggio[parola], "volte"
}' testo.txt
```

---

#### Array Associativi

Gli array in awk sono **associativi**: la chiave può essere qualsiasi stringa o numero.

```bash
# Conta quante volte appare ogni parola nel file
awk '{ for (i=1; i<=NF; i++) freq[$i]++ }
     END { for (w in freq) print freq[w], w }' testo.txt | sort -rn


# Verifica se una chiave esiste nell'array
awk '{ if ("admin" in utenti) print "admin trovato" }' file.txt

```

---

#### Funzioni Built-in di Awk

Awk include molte funzioni predefinite. Ecco le più importanti:

##### Funzioni per le Stringhe

| Funzione | Descrizione |
|----------|-------------|
| `length(s)` | Lunghezza della stringa `s`. Senza argomento: lunghezza di `$0`. |
| `substr(s, m, n)` | Sottostinga di `s` a partire dalla posizione `m`, lunga `n` caratteri. |
| `index(s, t)` | Posizione della prima occorrenza di `t` in `s` (0 se non trovata). |
| `split(s, a, sep)` | Divide `s` usando `sep` come delimitatore e popola l'array `a`. Ritorna il numero di elementi. |
| `sub(regex, repl, s)` | Sostituisce la **prima** occorrenza di `regex` con `repl` in `s`. |
| `gsub(regex, repl, s)` | Sostituisce **tutte** le occorrenze di `regex` con `repl` in `s`. |
| `match(s, regex)` | Cerca `regex` in `s`. Imposta `RSTART` e `RLENGTH`. Ritorna la posizione (0 se non trovata). |
| `sprintf(fmt, ...)` | Formatta una stringa come `printf` ma la ritorna invece di stamparla. |
| `tolower(s)` | Converte `s` in minuscolo. |
| `toupper(s)` | Converte `s` in maiuscolo. |

```bash
# length: lunghezza di ogni riga
# Stampa il numero di riga, la lunghezza della riga e la riga stessa
awk '{ print NR, length($0), $0 }' file.txt

# substr: estrai i primi 5 caratteri
awk '{ print substr($0, 1, 5) }' file.txt

# index: trova posizione di "@" in una email
awk '{ pos = index($1, "@"); print "@ è alla posizione:", pos }' email.txt

# split: divide un IP nei suoi ottetti
awk '{
    n = split($1, parti, ".")
    for (i=1; i<=n; i++) print "Ottetto", i, "=", parti[i]
}' ips.txt

# sub: sostituisce solo la prima occorrenza di "foo" con "bar" in $0
awk '{ sub(/foo/, "bar"); print }' file.txt

# gsub: sostituisce tutte le occorrenze di spazi multipli con uno solo
awk '{ gsub(/  +/, " "); print }' file.txt

# match: trova una parola che inizia con maiuscola
awk '{
    if (match($0, /[A-Z][a-z]+/))
        print "Trovato:", substr($0, RSTART, RLENGTH)
}' file.txt

# sprintf: formatta senza stampare
awk '{
    riga_formattata = sprintf("%-20s %5d", $1, $2)
    print riga_formattata
}' dati.txt

# tolower / toupper
awk '{ print toupper($1), tolower($2) }' file.txt
```

---

#### Funzioni Definite dall'Utente

Puoi definire le tue funzioni in awk. La sintassi è simile al C. Le variabili dichiarate come parametri extra (dopo uno spazio) fungono da **variabili locali**.

```bash
# Sintassi:
 function nome(param1, param2,    locale1, locale2) {
    ...
     return valore
 }

```

```bash
# Funzione con variabile locale (il doppio spazio è una convenzione)
awk ' 
function fattoriale(n,    risultato) {
    risultato = 1
    for (i = 2; i <= n; i++)
        risultato *= i
    return risultato
}

{ print $1, "! =", fattoriale($1) }
' numeri.txt
```

Esempio di funzionamento: 
Se il file numeri.txt contiene:
```bash
3
5
```
allora l'output prodotto a terminale sarà:

```bash
3! = 6  
5! = 120
```

---

#### Pipe e Interazione con la Shell

Awk può comunicare con comandi shell tramite pipe.

```bash
# Pipe in output: invia l'output di print a un comando
awk '{ print $1 | "sort -u" }' file.txt
```

---

## 8. Funzioni in Bash
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 8.1 Definizione e Chiamata

```bash
# Definizione POSIX
nome_funzione () {
    # comandi
}

# Definizione bash alternativa
function nome_funzione {
    # comandi
}

# Chiamata (senza parentesi!)
nome_funzione arg1 arg2
```

### 8.2 Parametri Interni

| Parametro | Significato |
|-----------|-------------|
| `$1, $2, ...` | Argomenti passati alla funzione |
| `$#` | Numero di argomenti |
| `$@` | Tutti gli argomenti |
| `$?` | Ultimo codice di uscita |
| `return <val>` | Valore di uscita (0 = ok) |

### 8.3 Esempi

```bash
# Funzione somma
somma () {
    echo $(($1 + $2))
}
ris=$(somma 4 7)
echo "Risultato: $ris"    # 11

# Funzione check file
check_file () {
    if [[ -f "$1" ]]; then
        echo "Il file '$1' esiste!"
    else
        echo "Il file '$1' NON esiste."
    fi
}
check_file /etc/passwd

# Contare processi di un utente
count_processes () {
    user=$1
    num=$(ps -u "$user" --no-headers | wc -l)
    echo "L'utente '$user' ha $num processi attivi."
}
```

---

## 9. Compilazione C e GCC
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 9.0 Basi di Programmazione C (vs Java)

Per chi proviene da linguaggi ad alto livello come Java, il C presenta alcune differenze architetturali critiche:
- **Paradigma**: Il C è puramente procedurale. Non ha classi o oggetti nativi, solo funzioni e `struct` (record di dati).
- **Gestione Memoria Manuale**: Non c'è il Garbage Collector. La memoria va allocata (`malloc`, `calloc`) e liberata (`free`) esplicitamente, pena memory leak.
- **Puntatori**: Una variabile che memorizza l'indirizzo fisico di memoria di un'altra variabile (o funzione). 
- **Array e Stringhe**: Gli array sono solo blocchi contigui di memoria, privi di controlli di limite (nessun "Index Out of Bounds" automatico; se sfori, corrompi la memoria). Le stringhe sono array di char terminati dal carattere nullo `'\0'`.
- **Il Preprocessore**: Fase precedente alla compilazione in cui vengono risolte le macro e le inclusioni. Ad esempio, `#include <stdio.h>` copia e incolla letteralmente le firme delle funzioni, e `#define MAX 10` sostituisce testualmente ogni "MAX" con "10" prima ancora che il compilatore veda il codice.

### 9.1 Il Compilatore GCC

GCC è il compilatore standard del progetto GNU. Supporta C, C++, Fortran, Ada, Go.

**Fasi della compilazione:**

```
Sorgente.c → [Preprocessore] → [Compilazione] → [Assembler] → [Linker] → Eseguibile
                 (#include,       codice          file .o      unisce
                  #define)        assembly                     librerie
```

1. **Preprocessore**: espande macro (`#define`), include file (`#include`)
2. **Compilazione**: traduce in codice assembly
3. **Assembler**: crea codice oggetto (file `.o`)
4. **Linker**: unisce le funzioni con il `main()` per creare l'eseguibile

### 9.2 Opzioni di GCC

```bash
gcc source.c                        # produce a.out
gcc -o eseguibile source.c          # specifica nome output
gcc -Wall -Wextra -o prog source.c  # tutti i warning
gcc -g -O0 -o prog source.c        # debug senza ottimizzazione
gcc -c source.c                     # solo compilazione (no linking)
gcc -E source.c                     # solo preprocessore
gcc -v                              # versione
```

| Flag | Significato |
|------|-------------|
| `-o` | Nome del file di output |
| `-Wall`, `-Wextra` | Abilita tutti i warning |
| `-w` | Disabilita tutti i warning |
| `-g` | Informazioni di debug |
| `-O0`, `-O1`, `-O2`, `-Os`, `-Ofast` | Livello di ottimizzazione |
| `-c` | Solo compilazione, no linking |
| `-E` | Solo preprocessore |

### 9.3 Debug con GDB

```bash
gcc -g -O0 -o prog source.c
gdb ./prog
```

| Comando GDB | Significato |
|-------------|-------------|
| `break main` | Breakpoint all'inizio di main |
| `run` | Lancia il programma |
| `list` | Mostra righe di codice |
| `next` | Esegui prossima istruzione (step over) |
| `step` | Entra dentro la funzione chiamata |
| `print x` | Stampa valore di una variabile |
| `info locals` | Tutte le variabili locali |
| `display x` | Aggiorna automaticamente x |
| `set var x = 5` | Cambia il valore |
| `continue` | Riprendi fino alla fine |
| `quit` | Esci |

### 9.4 Funzione Main in C

```c
// Senza argomenti
int main(void) { return 0; }

// Con argomenti da riga di comando
int main(int argc, char *argv[]) {
    // argc = numero di argomenti (incluso il nome del programma)
    // argv = array di stringhe con gli argomenti
    return 0;  // 0 = successo, diverso da 0 = errore
}
```

### 9.5 Layout di Memoria di un Processo

```
┌──────────────┐ Indirizzi alti
│    Stack     │ ← cresce verso il basso (variabili locali, parametri)
│      ↓       │
│              │
│      ↑       │
│    Heap      │ ← cresce verso l'alto (malloc, memoria dinamica)
├──────────────┤
│   BSS        │ ← variabili non inizializzate (azzerate in RAM)
├──────────────┤
│   Data       │ ← variabili inizializzate
├──────────────┤
│   Text       │ ← codice del programma (read-only)
└──────────────┘ Indirizzi bassi
```

### 9.6 Formato ELF e Linking

- GCC produce file nel formato **ELF** (Executable and Linkable Format)
- `readelf -h a.out` → mostra l'header ELF
- `readelf -S a.out` → mostra le sezioni
- `ldd a.out` → mostra le librerie condivise

**Tipi di file ELF:**
- **Relocatable** (`.o`): destinati al linking
- **Executable**: caricabili ed eseguibili
- **Shared Object** (`.so`): librerie dinamiche
- **Core dump**: stato della memoria di un processo terminato in errore

**Linking statico vs dinamico:**
- **Statico**: librerie incluse nell'eseguibile (ogni programma ha una copia)
- **Dinamico**: librerie caricate una volta sola in memoria, più efficiente

---

## 10. I/O di Basso Livello (System Call)
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 10.1 Concetti Fondamentali

Il kernel vede tutti i file come **flussi non formattati di byte**. Ogni file aperto è associato a un **file descriptor** (intero).

| File Descriptor | Costante | Significato |
|-----------------|----------|-------------|
| 0 | `STDIN_FILENO` | Standard input |
| 1 | `STDOUT_FILENO` | Standard output |
| 2 | `STDERR_FILENO` | Standard error |

### 10.2 Le Cinque System Call Fondamentali

#### `open` — Apertura di un file
```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *pathname, int oflag, ... /* mode_t mode */);
// Restituisce: fd in caso di successo, -1 in caso di errore
```

**Flag di apertura:**

| Flag | Significato |
|------|-------------|
| `O_RDONLY` | Sola lettura |
| `O_WRONLY` | Sola scrittura |
| `O_RDWR` | Lettura e scrittura |
| `O_APPEND` | Append alla fine |
| `O_CREAT` | Crea il file se non esiste |
| `O_EXCL` | Con O_CREAT, errore se il file esiste |
| `O_TRUNC` | Tronca il file se esiste |

**Permessi per mode (con O_CREAT):**
- `S_IRUSR`, `S_IWUSR`, `S_IXUSR` — lettura/scrittura/esecuzione per owner
- `S_IRWXU` — tutti i permessi per owner

**Esempi:**
```c
int fd = open("prova.txt", O_RDONLY);
// Se prova.txt non esiste => errore (segnalato da errno)

int fd = open("prova.txt", O_RDONLY | O_CREAT, S_IRWXU);
// Se prova.txt non esiste => viene creato con permessi rwx all'owner

int fd = open("prova.txt", O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
// Se prova.txt non esiste => viene creato con permessi rwx all'owner e aperto in lettura e scrittura
// Se prova.txt esiste => errore (segnalato da errno)

int fd = open("prova.txt", O_CREAT, S_IRWXU);
// Se prova.txt non esiste => viene creato con permessi rwx all'owner e aperto in sola lettura
```

#### `creat` — Creazione di un file
```c
int creat(const char *pathname, mode_t mode);
// Equivalente a: open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode);
```
**Esempio:**
```c
int fd = creat("prova.txt", S_IRWXU);
// Crea il file con permessi rwx all'owner
```

#### `close` — Chiusura di un file
```c
#include <unistd.h>
int close(int filedes);
// Restituisce: 0 successo, -1 errore
```
**Esempio:**
```c
int fd = open("prova.txt", O_RDONLY);
int result = close(fd);
```

#### `read` — Lettura da file
```c
#include <unistd.h>
ssize_t read(int filedes, void *buf, size_t nbytes);
// Restituisce: byte letti, 0 se fine file, -1 errore
//  Legge dal file e inserisce nel buffer (buf) fino a nbytes byte
```
**Esempio:**
```c
int fd = open("prova.txt", O_RDONLY);
char buf[10];
ssize_t nbytes = read(fd, buf, 10);
if (nbytes == -1) perror("read");  // errore
if (nbytes == 0)  printf("EOF\n"); // fine file
```

#### `write` — Scrittura su file
```c
#include <unistd.h>
ssize_t write(int filedes, void *buf, size_t nbytes);
// Restituisce: byte scritti, -1 errore
// Scrive nel file il contenuto del buffer (buf) per nbytes byte
```
**Esempio:**
```c
int fd = open("prova.txt", O_WRONLY | O_CREAT, S_IRWXU);
char buf[] = "Hello, world!";
ssize_t nbytes = write(fd, buf, strlen(buf));
if (nbytes == -1) perror("write"); // errore
```

> **Nota su `ssize_t` e `size_t`:**
> - `size_t` — intero **senza segno** (`unsigned`). Usato per le *dimensioni* passate come argomento (non può essere negativo).
> - `ssize_t` — intero **con segno** (`signed`). Usato come *valore di ritorno* perché deve poter restituire `-1` in caso di errore, `0` per EOF, o un numero positivo di byte letti/scritti.
> - Su sistemi a **32 bit** corrispondono a `unsigned int` / `int`. Su **64 bit** a `unsigned long` / `long`.
> - Il formato `printf` corretto è **`%zu`** per `size_t` e **`%zd`** per `ssize_t`.

### 10.3 Offset e `lseek`

L'**offset** è la posizione (in byte dall'inizio) dove avviene la prossima operazione I/O.

```c
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int filedes, off_t offset, int whence);
// Restituisce: nuovo offset, -1 errore
// Sposta l'offset del file descriptor in base a whence e offset
```

| `whence` | Significato |
|----------|-------------|
| `SEEK_SET` | Offset dall'inizio del file |
| `SEEK_CUR` | Offset dalla posizione corrente |
| `SEEK_END` | Offset dalla fine del file |

```c
// Conoscere l'offset corrente
off_t currpos = lseek(fd, 0, SEEK_CUR);

// Posizionarsi all'inizio
lseek(fd, 0, SEEK_SET);

// Posizionarsi alla fine
lseek(fd, 0, SEEK_END);
```

**Esempio:**
```c
int fd = open("prova.txt", O_RDONLY);
off_t currpos = lseek(fd, 0, SEEK_CUR);
```

### 10.4 Gestione Errori con `errno` e `perror`

Quando una system call fallisce, **non lancia un'eccezione** come in altri linguaggi. Invece:
1. La funzione restituisce **-1** (o `NULL` per funzioni che ritornano puntatori).
2. Il kernel imposta la variabile globale **`errno`** con un codice numerico che identifica il tipo di errore.

```c
#include <errno.h>

int fd = open("inesistente.txt", O_RDONLY);
if (fd == -1) {
    // errno contiene adesso il codice dell'errore
    printf("Codice errore: %d\n", errno);  // es. stampa 2
}
```
---

#### `perror()` — stampa il messaggio di errore

`perror()` è una funzione di libreria che legge automaticamente `errno` e stampa su **stderr** una riga nel formato:
```
<stringa_prefisso>: <messaggio_errore_in_italiano/inglese>
```

```c
#include <stdio.h>

int fd = open("prova.txt", O_RDONLY);
if (fd == -1) {
    perror("Apertura file");  
    // Stampa su stderr: "Apertura file: No such file or directory"
}
```

### 10.5 Implementazione nel Kernel

Il kernel usa tre strutture dati:
1. **Tabella dei processi**: ogni processo ha un vettore di file descriptor
2. **File table**: per ogni file aperto: flag di stato, offset, puntatore al v-node
3. **V-node table**: informazioni sul tipo di file e sulle funzioni operative (dati dall'i-node)

> Due processi che aprono lo stesso file hanno **entry diverse nella file table** (offset separati) ma condividono lo **stesso v-node**.

### 10.6 Duplicazione File Descriptor — `dup` e `dup2`

Le chiamate di sistema `dup` e `dup2` duplicano un file descriptor esistente.

```c
#include <unistd.h>

int dup(int oldfd);              // Duplica oldfd assegnando il MINIMO numero di fd disponibile
int dup2(int oldfd, int newfd);  // Duplica oldfd esattamente su newfd (operazione ATOMICA)
// Entrambe restituiscono: il nuovo fd in caso di successo, -1 in caso di errore
```

#### Come funziona `dup2(oldfd, newfd)` a livello di Kernel
1. Se `newfd` è già aperto, il kernel **lo chiude automaticamente** prima di riassegnarlo.
2. La voce all'indice `newfd` nella tabella dei file descriptor del processo viene fatta puntare alla **stessa voce nella File Table** a cui punta `oldfd`.
3. Di conseguenza, `oldfd` e `newfd`:
   - Condividono lo **stesso offset** di lettura/scrittura (se uno legge/scrive, l'offset avanza anche per l'altro).
   - Condividono gli stessi flag di stato (es. `O_APPEND`).
4. **Atomicità:** `dup2` chiude `newfd` e duplica `oldfd` in un'unica operazione atomica (evita condizioni di corsa che si avrebbero facendo `close(newfd)` seguito da `dup()`).
5. **Casi particolari:** Se `oldfd == newfd`, `dup2` non fa nulla e restituisce semplicemente `newfd` (senza chiuderlo). Se `oldfd` non è un descrittore valido, fallisce con `EBADF`.

---

#### Esempio 1: Redirezione di `stdout` su file e RIPRISTINO

> [!CAUTION]
> **Errore comune:** Chiudere `close(fd)` **NON** ripristina `stdout` sulla console! `STDOUT_FILENO` (fd 1) continuerà a puntare al file finché non viene esplicitamente ripristinato.

---

#### Esempio: Tipico pattern Shell (`fork` + `dup2` + `exec`)
Quando la shell esegue `ls > output.txt`, il processo padre fa `fork()`, e il processo figlio ridirige `stdout` prima di rimpiazzarsi con `exec`:

```c
pid_t pid = fork();
if (pid == 0) {
    // Processo FIGLIO:
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { perror("open"); _exit(1); }

    dup2(fd, STDOUT_FILENO); // Sovrascrive stdout con il file
    close(fd);               // Chiude il fd ridondante

    execlp("ls", "ls", "-l", NULL); // L'output di 'ls' andrà dentro output.txt
    perror("execlp");
    _exit(1);
}
// Il padre NON subisce la redirezione: il suo stdout punta ancora al terminale
wait(NULL);
```

### 10.7 Struttura `stat`

La structura `stat` contiene informazioni sul tipo di file e sulle sue proprietà. Viene restituita dalle funzioni `stat()`, `fstat()` e `lstat()`.
E' la rappresentazione di un **i-node** in C, la sua dimensione, i permessi, chi lo possiede, ecc.

```c
#include <sys/stat.h>
int stat(const char *pathname, struct stat *buf);
int fstat(int fd, struct stat *buf);
int lstat(const char *pathname, struct stat *buf);  // non segue symlink

struct stat {
    mode_t    st_mode;     // tipo file & permessi
    uid_t     st_uid;      // user ID proprietario
    gid_t     st_gid;      // group ID proprietario
    ino_t     st_ino;      // numero i-node
    nlink_t   st_nlink;    // numero di link
    off_t     st_size;     // dimensione in byte
    time_t    st_atime;    // ultimo accesso
    time_t    st_mtime;    // ultima modifica
    time_t    st_ctime;    // ultimo cambio metadati
    blksize_t st_blksize;  // blocco I/O ottimale
    blkcnt_t  st_blocks;   // blocchi allocati
};
```

**Esempio pratico**

> [!WARNING]
> I campi della struttura `stat`, in particolare `st_mode`, sono **tipi interi**. Non usare **mai** lo specificatore `%s` con `printf` su questi campi, altrimenti otterrai un **Segmentation Fault**. Usa `%o` (base ottale, formato standard per i permessi Unix) o `%d` (decimale).

```c
struct stat filestat;        // dichiariamo una variabile di tipo stat
stat("file.txt", &filestat); // eseguiamo la stat
printf("permessi: %o\n", filestat.st_mode); // %o per stamparlo in ottale!

int fd = open("file.txt", O_RDONLY);
struct stat filestat_fd;     // dichiariamo una variabile di tipo stat
fstat(fd, &filestat_fd);     // eseguiamo la stat sul file descriptor
printf("permessi: %o\n", filestat_fd.st_mode);
```

### 10.8 Esempio Completo: Copia tra File

```c
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#define BUFDIM 1000

int main(int argc, char **argv) {
    int infile, outfile, nread;
    char buffer[BUFDIM];
    if (argc != 3) { 
        printf("Uso: copia dest sorg\n"); 
        exit(1); 
    }
    if ((infile = open(argv[2], O_RDONLY)) < 0) {
        perror("apertura sorgente");
        exit(1); 
    }
    if ((outfile = creat(argv[1], 0777)) < 0) { 
        perror("apertura dest"); 
        close(infile); 
        exit(1); 
    }
    while ((nread = read(infile, buffer, BUFDIM)) > 0) {
        if (write(outfile, buffer, nread) == -1) {
            close(infile); 
            close(outfile);
            exit(1);
        }
    }
    close(infile); 
    close(outfile);
    return 0;
}
```

---

## 11. Processi Unix
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 11.1 Concetti Fondamentali

Un **processo** è un programma in esecuzione, caratterizzato da:
- **PID** (Process ID): identificativo univoco
- **PPID** (Parent Process ID): PID del padre
- Layout in memoria: text, data, BSS, heap, stack

PID riservati: `0` = scheduler, `1` = init, `2` = pagedaemon

### 11.2 Ottenere PID

```c
#include <unistd.h>
pid_t getpid(void);   // PID del processo corrente
pid_t getppid(void);  // PID del processo padre
```

### 11.3 Creazione di Processi — `fork()`

```c
#include <unistd.h>
pid_t fork(void);
```

- Crea una **copia esatta** del processo chiamante
- **Restituisce:**
  - `0` al processo **figlio**
  - PID del figlio al processo **padre**
  - `-1` in caso di errore

**Cosa ereditano e memorie separate:**
- **Condividono:** Il codice sorgente (Text segment), le variabili di ambiente, la working directory e i **File Descriptor** aperti prima della `fork()`.
- **Copia indipendente:** Il figlio riceve una *copia* esatta dei dati (variabili, heap, stack) del padre. Dopo la fork, queste memorie sono isolate: se un processo modifica una variabile, l'altro non vedrà la modifica.

**Come gestire il flusso (Valore di ritorno):**
Dato che da questo punto in poi ci sono *due* processi che eseguono lo stesso codice, si usa un costrutto `if-else` basato sul valore restituito da `fork()` per far prendere loro strade diverse:

```c
pid_t pid;
int variabile_condivisa = 10;
printf("Inizio del programma. (Eseguito solo dal padre)\n");
// Chiamata a fork
pid = fork();
// Da qui in poi, ci sono DUE processi che eseguono lo stesso codice!

if (pid < 0) {
    // Errore: la clonazione è fallita
    perror("fork failed");
    
} else if (pid == 0) {
    // --- CODICE ESEGUITO SOLO DAL FIGLIO ---
    // La fork() restituisce 0 al processo figlio
    printf("Sono il FIGLIO, PID=%d (Padre PID=%d)\n", getpid(), getppid());
    
} else {
    // --- CODICE ESEGUITO SOLO DAL PADRE ---
    // La fork() restituisce il PID del nuovo figlio al processo padre
    printf("Sono il PADRE, ho appena creato il figlio PID=%d\n", pid);
    wait(NULL);  // Il padre si mette in pausa e aspetta la terminazione del figlio
}
```

### 11.3.1 `vfork()`

Simile a `fork()`, ma:
- **Non copia** lo spazio di indirizzamento
- Il figlio esegue nello spazio del padre
- Il figlio esegue **per primo** fino a `exec()` o `_exit()`
- Usato tipicamente prima di `exec()` per efficienza

**Esempio vfork vs fork:**

```c
// Esempio fork (copia memoria)
int variabile_condivisa = 10;
pid_t pid = fork();
if (pid == 0) {
    printf("Figlio (fork): modifica variabile...");
    variabile_condivisa = 100; // modifica locale, non impatta il padre
    _exit(0);
}
wait(NULL); // Non ci interessa il valore del figlio
printf("Padre (fork): variabile=%d\n", variabile_condivisa); // sempre 10

// Esempio vfork (condivide memoria finché non c'è exec)
int variabile_condivisa = 10;
pid_t pid = vfork();
if (pid == 0) {
    printf("Figlio (vfork): modifica variabile...");
    variabile_condivisa = 100; // modifica VISIBILE al padre!
    _exit(0);
}
wait(NULL);
printf("Padre (vfork): variabile=%d\n", variabile_condivisa); // vedrà 100
```

### 11.4 Terminazione di Processi

Un processo può terminare in due modalità:

**Terminazione normale:**
- `return <status>` da `main()` (equivale a invocare `exit(status)`).
- `exit(int status)` — funzione di libreria standard C (`<stdlib.h>`).
- `_exit(int status)` — system call POSIX (`<unistd.h>`).
- `_Exit(int status)` — funzione standard ISO C99 (`<stdlib.h>`, equivalente a `_exit`).

**Terminazione anormale:**
- Ricezione di un segnale di terminazione non gestito o fatale (es. `SIGKILL`, `SIGSEGV`, `SIGINT`).
- Chiamata esplicita ad `abort()` — genera il segnale `SIGABRT` con creazione del file di core dump.

**Azioni del kernel alla terminazione:**
1. Deallocazione della memoria virtuale del processo (text, data, heap, stack).
2. Chiusura automatica di tutti i File Descriptor aperti.
3. Invio del segnale `SIGCHLD` al processo padre.
4. Mantenimento del PCB (Process Control Block) e del PID nella Process Table finché il padre non chiama `wait()`/`waitpid()` (stato zombie).

---

#### Confronto: `exit()` vs `_exit()`

| Proprietà | `exit(status)` | `_exit(status)` / `_Exit(status)` |
| :--- | :--- | :--- |
| **Tipo** | Funzione di libreria C (`<stdlib.h>`) | System Call diretta (`<unistd.h>`) / C99 (`<stdlib.h>`) |
| **Livello** | Spazio Utente (User-space) | Spazio Kernel (Kernel-space) |
| **Exit Handlers** | Esegue le funzioni registrate con `atexit()` e `on_exit()` | **Non esegue** alcun handler |
| **Buffer I/O (`FILE*`)** | Esegue il **flush** (`fflush`) e chiude tutti gli stream `stdio` | **Nessun flush**: termina immediatamente |
| **File temporanei** | Cancella i file aperti con `tmpfile()` | Non cancella i file temporanei |

>  **Regola fondamentale dopo `fork()`:**  
> Nei processi figli creati con `fork()` (o `vfork()`) si deve usare **`_exit()`** (e **MAI** `exit()`) nei percorsi di errore o prima di `exec()`:
> - Con la `fork()`, il figlio eredita una **copia** dei buffer I/O dello spazio utente del padre (es. stringhe stampate con `printf` ma non ancora inviate al terminale perché prive di `\n`).
> - Se il figlio chiamasse `exit()`, fluscerebbe anche lui questi buffer duplicati, causando **doppie stampe o scritture duplicate** su file condivisi.

### 11.5 Processi Zombie e Orfani

**Processo zombie:**
- Un figlio che termina prima che il padre faccia `wait()`
- Il kernel mantiene: PID, termination status, tempo CPU
- Resta zombie finché il padre non chiama `wait()`/`waitpid()`

**Processo orfano:**
- Un figlio il cui padre è terminato
- Viene "adottato" da **init** (PID=1)
- init chiama `wait()` → niente zombie

### 11.6 `wait()` e `waitpid()`

```c
#include <sys/wait.h>
pid_t wait(int *status);
// Si blocca finché un qualsiasi figlio non termina

pid_t waitpid(pid_t pid, int *status, int options);
// Può attendere un figlio specifico

```
*status* è un puntatore a intero che wait() usa come parametro di output: tu gli passi l'indirizzo di una variabile intera, e il kernel ci scrive informazioni su come il figlio è terminato,

**Esempio**
```c
int status;                 // variabile dove il kernel scriverà le info
pid_t pid = wait(&status);  // il kernel riempie 'status'
pid_t pid2 = waitpid(123, &status, 0); // Attende il figlio con PID=123 e memorizza lo stato in &status

pid_t pid3 = wait(NULL); // Attende qualsiasi figlio senza salvare il valore di ritorno
pid_t pid4 = waitpid(-1, NULL, 0); // Idem (attende qualsiasi figlio senza salvare lo stato)

```

**Argomento `pid` di `waitpid`:**
- `pid > 0` → attende il figlio con quel PID
- `pid == -1` → come wait (qualsiasi figlio)
- `pid == 0` → figlio con stesso process group
- `pid < -1` → figlio con process group ID = |pid|

**Opzione `WNOHANG`**:
Normalmente `waitpid` è bloccante: se si invoca su un figlio che è ancora in esecuzione, il processo padre viene "addormentato" dal sistema finché quel figlio non termina.
Passando la costante `WNOHANG` come opzione, si ordina alla system call di comportarsi in modo **non bloccante** (polling). La `waitpid` controllerà lo stato del figlio:
- Se il figlio è terminato, restituisce il suo PID (come al solito).
- Se il figlio sta ancora lavorando, **non si blocca**, ma restituisce immediatamente `0`, permettendo al padre di continuare a eseguire altre operazioni nel frattempo.

**Macro per ispezionare status:**
- `WIFEXITED(status)` → terminazione normale
- `WEXITSTATUS(status)` → exit status
- `WIFSIGNALED(status)` → terminazione da segnale

### 11.7 La Famiglia `exec`

#### A cosa serve?

`exec` serve a **trasformare un processo in un programma completamente diverso**. È la syscall che la shell usa ogni volta che scrivi un comando: prima fa una `fork()` per creare un figlio, poi il figlio chiama `exec()` per diventare il programma richiesto.

> **Regola fondamentale:** `exec` NON crea un nuovo processo. Prende il processo corrente e lo **svuota** completamente (codice, dati, stack, heap), poi ci carica dentro il nuovo programma. Il PID rimane lo stesso.

> Se `exec` ha successo, **non ritorna mai**: il vecchio codice è stato cancellato. Il codice dopo `exec()` viene eseguito solo in caso di errore.

```c
// Ogni exec deve essere PRECEDUTA da una fork() o vfork(), viene chiamato dal figlio generato da fork()
int execl(char *pathname, char *arg0, ... );            // lista argomenti, percorso esatto
int execv(char *pathname, char *argv[]);                // array argomenti, percorso esatto
int execlp(char *filename, char *arg0, ... );           // lista argomenti, cerca nel PATH
int execvp(char *filename, char *argv[]);               // array argomenti, cerca nel PATH
int execle(char *pathname, char *arg0, ..., char *envp[]); // lista argomenti + ambiente custom
int execve(char *pathname, char *argv[], char *envp[]);    // unica vera syscall del kernel
```

**Come scegliere quale usare:**

| Suffisso | Cosa cambia | Quando usarlo |
|----------|-------------|---------------|
| `l` (list) | Argomenti come lista variabile terminata da `(char*)NULL` | Quando conosci gli argomenti a compile-time |
| `v` (vector) | Argomenti come array `char *argv[]` | Quando gli argomenti variano a runtime (es. letti dall'utente) |
| `p` (path) | Cerca l'eseguibile nel `PATH` automaticamente | Quando usi comandi standard di sistema (`ls`, `grep`, ...) |
| `e` (env) | Passi un ambiente custom invece di ereditare quello del padre | Quando vuoi controllare le variabili d'ambiente del nuovo processo |

```c
// execl: percorso esatto + argomenti come lista
execl("/bin/ls", "ls",       "-l", "-a", (char *)NULL);
//     ↑ percorso  ↑ argv[0]  ↑ argv[1]  ↑ terminatore obbligatorio

// execlp: cerca "ls" nel PATH automaticamente
execlp("ls", "ls", "-l", (char *)NULL);

// execvp: argomenti come array (utile quando non sai quanti sono)
char *args[] = {"ls", "-l", "-a", NULL};  // NULL come terminatore
execvp("ls", args);

// execle: percorso esatto + ambiente personalizzato
char *env[] = {"HOME=/tmp", "PATH=/bin", NULL};
execle("/bin/ls", "ls", "-l", (char *)NULL, env);
```

**Le Variabili d'Ambiente (`envp`)**
Oltre agli argomenti, un processo riceve un array di stringhe chiamato *ambiente* (environment), accessibile tramite la variabile globale `extern char **environ;` (o come terzo argomento del main `int main(int argc, char *argv[], char *envp[])`).
Queste variabili sono nella forma `CHIAVE=VALORE` e configurano il comportamento dei programmi. Esempi chiave:
- `PATH`: Lista di directory in cui la shell cerca i comandi eseguibili.
- `HOME`: La directory personale dell'utente.
- `TERM`: Il tipo di terminale (es. `xterm-256color`).
Quando si usa una `exec()` senza il suffisso `e` (es. `execl`, `execvp`), il nuovo programma *eredita* automaticamente l'ambiente del programma chiamante. Usando `execle` o `execve`, lo sviluppatore inietta un ambiente (`envp`) completamente personalizzato.

#### Esempio di Utilizzo di `envp` (`execve` + lettura)

**1. Il programma target (`stampa_env.c`) che riceve e legge `envp`:**
```c
#include <stdio.h>
#include <stdlib.h>

// envp è un array di puntatori a stringhe terminato da NULL
int main(int argc, char *argv[], char *envp[]) {
    printf("=== Variabili d'ambiente ricevute via envp ===\n");
    for (int i = 0; envp[i] != NULL; i++) {
        printf("envp[%d] = %s\n", i, envp[i]);
        // Stampa le variabili d'ambiente:
        // envp[0] = UTENTE_APP=Paolo
        // envp[1] = MODALITA=DEBUG
        // envp[2] = VERSIONE=2.0
        // envp[3] = PATH=/bin:/usr/bin
        // envp[4] = (NULL)
    }

    // Le variabili passate in envp sono leggibili anche tramite getenv()!
    char *user = getenv("UTENTE_APP");
    char *mode = getenv("MODALITA");
    printf("\nValori letti con getenv():\n");
    printf("UTENTE_APP: %s\n", user ? user : "NON DEFINITA");
    printf("MODALITA:   %s\n", mode ? mode : "NON DEFINITA");
    return 0;
}
```

**2. Il programma chiamante (`main_execve.c`) che costruisce `envp` e invoca `execve`:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork fallita");
        exit(1);
    }

    if (pid == 0) {
        // 1. Argomenti da passare (argv): terminati obbligatoriamente da NULL
        char *my_argv[] = {"./stampa_env", "argomento1", "argomento2", NULL};

        // 2. Ambiente personalizzato (envp): formato "CHIAVE=VALORE", terminato da NULL
        char *my_envp[] = {"UTENTE_APP=Paolo", "MODALITA=DEBUG", "VERSIONE=2.0",
                           "PATH=/bin:/usr/bin", NULL};

        printf("[Figlio] Invoco execve passando argv ed envp custom...\n");
        // execve(pathname, argv, envp)
        execve("./stampa_env", my_argv, my_envp);

        // Questa riga viene eseguita SOLO se execve fallisce
        perror("execve fallita");
        exit(1);
    } else {
        // Il padre attende la terminazione del figlio
        wait(NULL);
        printf("[Padre] Processo figlio terminato con successo.\n");
    }

    return 0;
}
```

---

**Pattern completo fork + exec:**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) {
        // --- FIGLIO: si trasforma in "ls -l" ---
        // "/bin/ls" = percorso esatto del programma
        // "ls"      = argv[0] (convenzione: nome del programma)
        // "-l"      = argv[1] (primo argomento reale)
        // (char*)0  = terminatore obbligatorio della lista
        execl("/bin/ls", "ls", "-l", (char *)0);

        // Questa riga viene raggiunta SOLO se execl fallisce
        perror("execl failed");
        exit(1);
    }
    // --- PADRE: aspetta che il figlio (cioè ls) finisca ---
    wait(NULL);
    printf("ls completato\n");
    return 0;
}
```

**Cosa il nuovo programma eredita (dopo exec):**
- PID, PPID, process group ID, session ID
- UID, GID reali
- Current working directory, root directory
- Umask, file lock, maschera dei segnali
- File descriptor aperti (tranne quelli con `FD_CLOEXEC`)

**Cosa NON eredita (viene reimpostato):**
- Effective UID/GID (reimpostati secondo i bit setuid/setgid del nuovo eseguibile)
- File descriptor con flag `FD_CLOEXEC` → vengono chiusi automaticamente al momento di exec
- Handler dei segnali personalizzati → tornano a `SIG_DFL`


### 11.9 La funzione `system()`

`system()` è una funzione di libreria (`stdlib.h`) che esegue un comando di sistema invocando la shell (`/bin/sh -c <comando>`). Internamente incapsula le chiamate fondamentali: **`fork()` + `execl("/bin/sh", ...)` + `waitpid()`**.

```c
#include <stdlib.h>
int system(const char *command);
// Ritorna: exit status del comando, -1 se fork/wait fallisce
```

#### Esempio Base con `system()`
```c
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    // Esecuzione diretta con system():
    int ret = system("ls -l /tmp");
    if (ret == -1) {
        perror("system");
    }
    return 0;
}
```

#### Implementazione Equivalente con `fork()`, `execl()` e `waitpid()`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int my_system(const char *command) {
    // Se il comando è NULL, verifica se la shell è presente nel sistema
    if (command == NULL) {
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        // Errore nella creazione del processo figlio
        return -1;
    }

    if (pid == 0) {
        // --- PROCESSO FIGLIO ---
        // Invoca la shell /bin/sh passando il flag "-c" e il comando come stringa
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);

        // Se execl fallisce (es. /bin/sh inesistente), esce con codice 127 (standard POSIX)
        _exit(127);
    }

    // --- PROCESSO PADRE ---
    int status;
    // Attende specificamente la terminazione del figlio generato
    while (waitpid(pid, &status, 0) < 0) {
        if (errno != EINTR) {
            // Errore diverso da un segnale di interruzione
            return -1;
        }
        // Se interrotto da un segnale (EINTR), ripete la waitpid
    }

    return status; // Restituisce lo stato grezzo (ispezionabile con WIFEXITED, WEXITSTATUS, ecc.)
}

int main() {
    printf("=== Test implementazione equivalente di system() ===\n");

    int ret = my_system("ls -l | grep .c");

    if (ret == -1) {
        perror("my_system fallita");
    } else if (WIFEXITED(ret)) {
        printf("Comando completato con successo (exit code: %d)\n", WEXITSTATUS(ret));
    } else if (WIFSIGNALED(ret)) {
        printf("Comando terminato dal segnale %d\n", WTERMSIG(ret));
    }

    return 0;
}
```

#### Note su `system()`
1. **Perché passa attraverso `/bin/sh -c`?**  
   Permette di interpretare automaticamente costrutti della shell come pipe (`|`), ridirezioni (`>`, `<`), wildcard (`*`) e variabili d'ambiente.
2. **Efficienza e Sicurezza:**
   - Meno efficiente: crea due processi (la shell `/bin/sh` + il comando effettivo).
   - >  **Non usare `system()` in programmi con privilegi elevati (setuid):** Un utente malintenzionato potrebbe alterare `PATH` o altre variabili d'ambiente per eseguire codice arbitrario con privilegi di root.

### 11.10 Ambiente di un Processo: `getenv` e `putenv`

Ogni processo ha un **ambiente**: una lista di coppie `NOME=valore` (es. `PATH=/bin:/usr/bin`, `HOME=/home/user`) che vengono passate automaticamente a tutti i processi figli.

#### `getenv` — leggere una variabile d'ambiente

```c
#include <stdlib.h>
char *getenv(const char *name);
// Ritorna: puntatore al valore, oppure NULL se la variabile non esiste
```

`getenv` cerca nell'ambiente del processo la variabile con quel nome e restituisce il suo valore come stringa. **Non copiare mai il risultato in un puntatore che poi modifichi**: il puntatore punta direttamente alla memoria dell'ambiente.

```c
#include <stdlib.h>
#include <stdio.h>

// Leggo la variabile HOME
char *home = getenv("HOME");
if (home != NULL)
    printf("Home directory: %s\n", home);  // es. /home/paolo
else
    printf("Variabile HOME non trovata\n");

// Leggo il PATH
char *path = getenv("PATH");
printf("PATH = %s\n", path);  // es. /bin:/usr/bin:/usr/local/bin

// Uso pratico: configurare il programma senza argomenti a riga di comando
char *debug = getenv("MY_APP_DEBUG");
if (debug != NULL && strcmp(debug, "1") == 0) {
    printf("Modalità debug attivata\n");
}
```

#### `putenv` — aggiungere o modificare una variabile d'ambiente

```c
#include <stdlib.h>
int putenv(char *string);  // string deve essere nel formato "NOME=valore"
// Ritorna: 0 successo, non-zero errore
```

`putenv` aggiunge la variabile all'ambiente del processo corrente. La modifica è visibile anche a tutti i processi figli che verranno creati con `fork()` dopo questa chiamata.

```c
// Aggiungo una nuova variabile
putenv("MY_VAR=12345");
printf("%s\n", getenv("MY_VAR"));  // stampa: 12345

// Modifico una variabile esistente
putenv("HOME=/tmp");
printf("%s\n", getenv("HOME"));    // stampa: /tmp
```

> ⚠️ **Attenzione critica:** `putenv` non copia la stringa, ma inserisce direttamente il puntatore nell'ambiente. Se usi una variabile locale (stack), questa viene distrutta al termine della funzione e l'ambiente punta a memoria invalida!
> ```c
> // SBAGLIATO: variabile locale, viene distrutta!
> void imposta() {
>     char buf[] = "NOME=valore";  // sullo stack!
>     putenv(buf);  // PERICOLOSO: buf viene distrutto al return
> }
>
> // CORRETTO: stringa letterale (in memoria statica, non viene distrutta)
> putenv("NOME=valore");
>
> // CORRETTO: memoria heap (sopravvive alla funzione)
> char *s = strdup("NOME=valore");  // malloc + strcpy
> putenv(s);
> // NON fare free(s) finché la variabile è nell'ambiente!
> ```

#### Accedere all'intero ambiente

```c
extern char **environ;  // array di stringhe "NOME=valore", terminato da NULL

// Stampa tutte le variabili d'ambiente
for (int i = 0; environ[i] != NULL; i++)
    printf("%s\n", environ[i]);

// Accesso tramite il terzo parametro del main
int main(int argc, char *argv[], char *envp[]) {
    for (int i = 0; envp[i] != NULL; i++)
        printf("%s\n", envp[i]);
}
```

#### Passare un ambiente custom a `exec`

Con `execle` o `execve` puoi passare un ambiente completamente diverso al nuovo programma:

```c
// Creo un ambiente minimale per il processo figlio
char *env_figlio[] = {
    "PATH=/bin:/usr/bin",
    "HOME=/tmp",
    "LANG=it_IT.UTF-8",
    NULL  // terminatore obbligatorio
};
execle("/bin/ls", "ls", "-l", (char*)NULL, env_figlio);
// ls verrà eseguito con SOLO quelle variabili d'ambiente
```

---

### 11.11 `chdir` e `chroot` — Cambiare directory e root

#### `chdir` — Cambia la Current Working Directory (CWD)

```c
#include <unistd.h>
int chdir(const char *path);  // ritorna 0 successo, -1 errore
```

`chdir` cambia la **directory corrente** del processo (quella che si vede con `pwd` nella shell). La nuova CWD viene **ereditata dai figli** creati con `fork()` dopo la chiamata.

```c
#include <unistd.h>
#include <stdio.h>

// Sposto il processo nella directory /tmp
if (chdir("/tmp") == -1) {
    perror("chdir");
    exit(1);
}

// Ora un open("file.txt", ...) cerca /tmp/file.txt
int fd = open("file.txt", O_RDONLY);

// Stampa la directory corrente
char cwd[256];
getcwd(cwd, sizeof(cwd));
printf("CWD attuale: %s\n", cwd);  // /tmp
```

**Uso tipico:** I daemon (servizi) di solito chiamano `chdir("/")` all'avvio per non "bloccare" il filesystem da cui sono stati lanciati (impedendo di smontarlo).

#### `chroot` — Cambia la Root Directory (Sandboxing)

```c
#include <unistd.h>
int chroot(const char *path);  // richiede privilegi root!
// ritorna 0 successo, -1 errore
```

`chroot` cambia quella che il processo percepisce come la **directory radice `/`**. Dopo la chiamata, il processo non riesce più ad accedere a nessun file al di fuori della nuova root: è **intrappolato** in quella directory e nelle sue sottodirectory. Questo è il concetto alla base del **sandboxing**.

```
Filesystem reale:          Dopo chroot("/jail"):
/                          / (= /jail nel filesystem reale)
├── bin/                   ├── bin/
├── etc/                   ├── etc/
├── jail/           →      └── lib/
│   ├── bin/
│   ├── etc/
│   └── lib/
└── home/   ← inaccessibile al processo!
```

```c
// Richiede privilegi di root (UID 0 o capability CAP_SYS_CHROOT)
if (chroot("/var/jail") == -1) {
    perror("chroot");
    exit(1);
}

// OBBLIGATORIO: Spostarsi all'interno della nuova root!
if (chdir("/") == -1) {
    perror("chdir");
    exit(1);
}

// Da qui in poi, il processo è bloccato dentro /var/jail
// Questo cercherà /var/jail/etc/passwd nel filesystem reale:
open("/etc/passwd", O_RDONLY);
```

> **Perché dopo `chroot()` bisogna fare SEMPRE `chdir("/")`?**  
> La system call `chroot(path)` modifica **solo** il riferimento alla root directory (`/`) del processo, ma **NON tocca la directory di lavoro corrente (CWD)**!  
> Se non si esegue subito `chdir("/")`, la CWD del processo rimane la cartella in cui si trovava prima della chiamata (che ora è *esterna* alla jail). Un attaccante potrebbe risalire l'albero con percorsi relativi (`../../`) ed **evadere dal sandbox** (*chroot escape / jailbreak*).

---

## 12. Segnali
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 12.1 Cos'è un Segnale

Un **segnale** è un **interrupt software** che consente la comunicazione **asincrona** tra processi e/o tra device e processo.

- Ogni segnale ha un nome che inizia con `SIG` (definiti in `<signal.h>`)
- Associati a interi positivi
- Inviati in modo **asincrono**

### 12.2 Segnali Principali

| Numero | Nome | Significato | Azione di Default |
|:---:|------|-------------|-------------------|
| **2** | `SIGINT` | Interruzione da tastiera (Ctrl-C) | Terminare |
| **3** | `SIGQUIT` | Quit da tastiera (Ctrl-\) | Terminare |
| **9** | `SIGKILL` | Terminazione forzata* | Terminare |
| **10** | `SIGUSR1` | A disposizione dell'utente | Terminare |
| **11** | `SIGSEGV` | Segmentation fault | Terminare |
| **12** | `SIGUSR2` | A disposizione dell'utente | Terminare |
| **13** | `SIGPIPE` | Scrittura su pipe senza lettore | Terminare |
| **14** | `SIGALRM` | Sveglia (alarm) | Terminare |
| **15** | `SIGTERM` | Terminazione | Terminare |
| **17** | `SIGCHLD` | Figlio terminato o fermato | Ignorare |
| **19** | `SIGSTOP` | Stop al processo* | Fermare |

> *`SIGKILL` (9) e `SIGSTOP` (19) **non possono** essere catturati o ignorati.

### 12.3 Azioni Possibili

Quando un processo riceve un segnale, può reagire in uno dei seguenti tre modi:

1. **Ignorare il segnale (`SIG_IGN`):**  
   Il segnale viene scartato immediatamente; l'esecuzione del programma continua indisturbata.  
   ```c
   signal(SIGINT, SIG_IGN); // Ignora Ctrl+C
   ```

2. **Catturare il segnale (Custom Handler):**  
   L'esecuzione ordinaria viene temporaneamente sospesa per eseguire una funzione personalizzata definita dall'utente (*signal handler*).  
   ```c
   signal(SIGINT, mio_handler); // Esegue la funzione 'mio_handler' su Ctrl+C
   ```

3. **Azione di Default (`SIG_DFL`):**  
   Ripristina il comportamento predefinito del sistema operativo per quel segnale (es. per `SIGINT` il default è la terminazione del processo). Serve ad annullare un precedente handler o una `SIG_IGN`.  
   ```c
   signal(SIGINT, SIG_DFL); // Ripristina il default (terminazione immediata su Ctrl+C)
   ```


### 12.4 Catturare un Segnale — `signal()`

```c
#include <signal.h>
typedef void (*sighandler_t)(int);
sighandler_t signal(int signum, sighandler_t handler);
// Restituisce: handler precedente, o SIG_ERR in caso di errore
```

**Esempio completo:**
```c
void foo(int num_segnale) {
    if (num_segnale == SIGINT)
        printf("Ricevuto SIGINT (%d)\n", num_segnale);
    if (num_segnale == SIGUSR1)
        printf("Ricevuto SIGUSR1 (%d)\n", num_segnale);
}

int main(void) {
    // SIGUSR1 e SIGUSR2 sono segnali generici definiti dall'utente
    // SIGINT viene inviato dal terminale premendo Ctrl+C e di default termina il processo.
    signal(SIGUSR1, foo);
    signal(SIGUSR2, foo);
    signal(SIGINT, foo);
    // signal(SIGKILL, foo); // ERRORE: non si può catturare SIGKILL
    for (;;) { pause(); }  // attende segnali
}
```
### 12.4.1 Ignorare un Segnale — `signal()`

"Ignorare" significa dire al kernel: "quando arriva questo segnale, fai finta di niente". Il segnale viene ricevuto ma scartato immediatamente, come se non fosse mai arrivato. Il processo continua la sua esecuzione senza interruzioni.

```c
// SIG_IGN: ignora completamente il segnale
signal(SIGINT, SIG_IGN);
// Ora se premi Ctrl+C, non succede nulla.
// Il processo continua a girare indisturbato.
```


### 12.4.2 Azione di Default — `signal()`

L'azione di default è quella che avviene se non catturi o ignori il segnale. Per ogni segnale, l'azione di default è definita in modo diverso, vedi tabella in 12.2.

```c
// Imposta l'azione di default per SIGINT
signal(SIGINT, SIG_DFL);
// Ora Ctrl+C causerà la terminazione del processo, come farebbe normalmente
// Si comporta come se non avessi mai chiamato signal()
```


### 12.5 Inviare Segnali

**Nota**
Il comando anche chiamandosi `kill` in realtà **non uccide** necessariamente il processo, ma invia un segnale generico al processo. Una volta inviato il segnale, il comportamento del processo dipende da come è stato configurato. Ad esempio, se viene inviato un segnale che non è stato catturato, il comportamento del processo dipenderà dall'azione di default per quel segnale. 
Per "uccidere" un processo es. `1234`, si dovrebbe inviare il segnale `SIGKILL`.
es: `kill -SIGKILL 1234`  oppure  `kill -9 1234`
**Ragioni Storiche**
Quando Unix fu creato nel 1969-1973, il meccanismo dei segnali era molto più primitivo. Le prime versioni supportavano pochissimi segnali, e l'utilizzo pratico era quasi esclusivamente quello di terminare processi che si erano bloccati o comportavano male.


**Dalla shell:**
```bash
kill -SIGUSR1 2043    # invia SIGUSR1 al processo 2043
kill -l               # elenca tutti i segnali
kill 127              # equivale a kill -TERM 127
```

**In C:**
```c
#include <signal.h>
int kill(pid_t pid, int sig);
// pid > 0: processo specifico
// pid = 0: tutti con stesso group ID
// pid < -1: tutti con group ID = |pid|
// sig = 0: verifica solo se il processo esiste
```

### 12.6 Alarm — Sveglia

```c
unsigned int alarm(unsigned int seconds);
// Prenota SIGALRM tra N secondi
// alarm(0) cancella la prenotazione
// Esiste un'UNICA sveglia per processo
```

> **Attenzione**: definire l'handler **prima** di chiamare `alarm()`, perché il default di SIGALRM è la terminazione.

### 12.7 Insiemi di Segnali e Maschere

La **maschera dei segnali (signal mask)** di un processo è una lista di segnali che il processo decide temporaneamente di "bloccare".
Quando un segnale viene generato ed inviato al processo, il sistema controlla questa maschera:
- Se il segnale **è presente** nella maschera, viene bloccato e messo "in attesa" (pending). Il segnale non viene perso, ma sarà consegnato solo quando (e se) verrà sbloccato.
- Se il segnale **non è presente**, viene consegnato e gestito immediatamente.

Per manipolare questi gruppi di segnali in C, si utilizza il tipo di dato **`sigset_t`** e una serie di funzioni dedicate:

```c
#include <signal.h>
// Funzioni per preparare e gestire gli insiemi (sigset_t)
int sigemptyset(sigset_t *set);                 // Svuota l'insieme (nessun segnale presente)
int sigfillset(sigset_t *set);                  // Inserisce nell'insieme tutti i segnali supportati
int sigaddset(sigset_t *set, int sig);          // Aggiunge un segnale specifico all'insieme
int sigdelset(sigset_t *set, int sig);          // Rimuove un segnale specifico dall'insieme
int sigismember(const sigset_t *set, int sig);  // Verifica se un segnale è presente nell'insieme
```

**Esempio:**
```c
sigset_t mask;

// 1. Inizializzo la maschera a "vuota"
sigemptyset(&mask);  // mask ora non contiene nessun segnale

// 2. Aggiungo SIGINT (segnale 2) alla maschera
sigaddset(&mask, SIGINT);

// Ora "mask" contiene solo SIGINT. Qualsiasi invio di SIGINT al processo
// verrà bloccato fino a quando non rimuoverò SIGINT dalla maschera.
```

Una volta preparato l'insieme (`sigset_t`), si deve utilizzare la funzione `sigprocmask` per applicarlo e modificare effettivamente la maschera attiva del processo:

```c
int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
```
I parametri fondamentali sono:
- **`how`**: Definisce *come* il nuovo insieme modificherà la maschera attuale:
  - `SIG_BLOCK`: **Unione**. Aggiunge i segnali di `set` a quelli attualmente bloccati.
  - `SIG_UNBLOCK`: **Rimozione**. Rimuove i segnali di `set` dalla maschera (li sblocca).
  - `SIG_SETMASK`: **Sostituzione**. Sostituisce interamente la vecchia maschera con il nuovo `set`.
- **`set`**: Il puntatore all'insieme di segnali da applicare.
- **`oldset`**: Se diverso da `NULL`, la funzione vi salverà la vecchia maschera prima di effettuare la modifica (molto utile se si vuole ripristinarla in seguito).

**Esempio:**
```c
sigset_t mask, oldmask;

// 1. Inizializzo la maschera a "vuota"
sigemptyset(&mask);  // mask ora non contiene nessun segnale

// 2. Aggiungo SIGINT (segnale 2) alla maschera
sigaddset(&mask, SIGINT);
if (sigprocmask(SIG_BLOCK, &mask, &oldmask) == -1) {
    perror("sigprocmask");
    exit(EXIT_FAILURE);
}

// Ora "mask" contiene solo SIGINT. Qualsiasi invio di SIGINT al processo
// verrà bloccato fino a quando non rimuoverò SIGINT dalla maschera.
```

> **Nota fondamentale sull'ambito di `sigprocmask`:**  
> La funzione `sigprocmask()` modifica la maschera del processo in modo **permanente** durante il normale flusso di esecuzione (il blocco dura finché non invochi esplicitamente `SIG_UNBLOCK` o `SIG_SETMASK`).  
> Se invece vuoi bloccare dei segnali in modo **temporaneo e atomico solo durante l'esecuzione di un handler**, non si usa `sigprocmask`, ma il campo `sa_mask` di **`sigaction()`** (spiegato subito nella sezione successiva [12.8](#128-gestione-moderna-dei-segnali--sigaction)).

### 12.8 Gestione Moderna dei Segnali — `sigaction()`

Sebbene `signal()` sia la funzione storica del C per catturare i segnali, nei moderni sistemi operativi e nelle applicazioni concorrenti/di rete è considerata **obsoleta e inaffidabile**. Lo standard **POSIX** ha introdotto **`sigaction()`** per risolvere tutti i problemi di sincronizzazione e non-determinismo della vecchia interfaccia.

> **L'intuizione chiave:**  
> In un certo senso, **`sigaction()` combina `signal()` e `sigprocmask()` in un'unica operazione atomica**:
> 1. Specifica quale funzione eseguire all'arrivo del segnale (`sa_handler` $\rightarrow$ il compito di `signal`).
> 2. Definisce un insieme di segnali da **bloccare temporaneamente** mentre l'handler è in esecuzione (`sa_mask` $\rightarrow$ il compito di `sigprocmask`).
> 3. Il kernel applica la maschera **istantaneamente e atomicamente** appena salta all'handler, e la ripristina da solo all'uscita, eliminando qualsiasi finestra di vulnerabilità (*race condition*).

#### I 4 Problemi Critici di `signal()` risolti da `sigaction()`

1. **Reset Automatico a `SIG_DFL` (Comportamento One-Shot e Race Condition):**  
   Nei vecchi sistemi Unix (System V), quando un segnale veniva ricevuto, il kernel resettava immediatamente l'azione al valore di default (`SIG_DFL`) prima di chiamare l'handler. Per continuare a gestirlo, il programmatore doveva richiamare `signal()` dentro l'handler stesso. Se un secondo segnale arrivava nella finestra temporale prima della re-installazione, il processo veniva terminato in modo anomalo!  
   *Con `sigaction()`:* L'handler rimane registrato in modo **permanente e affidabile** finché non viene esplicitamente modificato.

2. **Mascheramento Atomico dei Segnali Concorrenti (`sa_mask`):**  
   Con `signal()`, se arrivava un altro segnale mentre l'handler era in esecuzione, l'handler veniva interrotto nel mezzo con conseguente corruzione di dati o deadlock.  
   *Con `sigaction()`:* Il campo `sa_mask` consente di elencare quali segnali il kernel deve **bloccare automaticamente** per tutta la durata dell'handler. Inoltre, il segnale stesso che ha scatenato l'handler viene bloccato di default (evitando ricorsioni non volute).

3. **Controllo delle System Call Interrotte (`SA_RESTART` vs `EINTR`):**  
   Se il processo si trova bloccato su una chiamata lenta di I/O (es. `read()`, `write()`, `accept()`, `recv()`) e arriva un segnale:
   - Con `signal()`, il comportamento dipendeva dall'implementazione (alcuni Unix riavviavano, altri restituivano errore `EINTR`).
   - Con `sigaction()`, il programmatore ha il pieno controllo tramite i flag:
     - **Con `SA_RESTART`**: il kernel riavvia automaticamente la system call non appena l'handler termina.
     - **Senza `SA_RESTART`**: la system call fallisce restituendo `-1` e impostando `errno = EINTR`, permettendo al programma di gestire manualmente l'interruzione.

4. **Metadati Avanzati sul Mittente (`SA_SIGINFO`):**  
   `signal()` passa all'handler solo un intero (`int signum`). Con `sigaction()`, abilitando il flag `SA_SIGINFO`, si può usare la firma estesa `sa_sigaction(int sig, siginfo_t *info, void *ucontext)`, potendo così ispezionare il **PID del processo mittente** (`info->si_pid`), l'UID, o la causa del segnale.

---

#### Struttura e Configurazione di `sigaction`

```c
#include <signal.h>

struct sigaction {
    void     (*sa_handler)(int);                  // Handler standard (come signal)
    void     (*sa_sigaction)(int, siginfo_t *, void *); // Handler avanzato (con SA_SIGINFO)
    sigset_t sa_mask;                             // Segnali bloccati DURANTE l'handler
    int      sa_flags;                            // Flag (es. SA_RESTART, SA_SIGINFO)
    void     (*sa_restorer)(void);                // Riservato / obsoleto
};

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
// Restituisce: 0 in caso di successo, -1 in caso di errore
```

#### Esempio Completo

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void gestore_sigint(int sig) {
    printf("\n[Handler] Ricevuto SIGINT (%d). Gestione sicura in corso...\n", sig);
    sleep(1);
    printf("[Handler] Fine gestione.\n");
}

int main(void) {
    struct sigaction sa = {0};

    // 1. Azione da eseguire (funzione gestore)
    sa.sa_handler = gestore_sigint;

    // 2. Maschera atomica: blocca temporaneamente anche SIGQUIT e SIGUSR1
    //    mentre questo handler è in esecuzione
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sigaddset(&sa.sa_mask, SIGUSR1);

    // 3. Flag: riavvia le chiamate bloccanti lente senza farle fallire con EINTR
    sa.sa_flags = SA_RESTART;

    // 4. Registrazione sicura su SIGINT (Ctrl+C)
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("In attesa di segnali (premi Ctrl+C per testare, o Ctrl+\\ per uscire)...\n");
    while (1) {
        pause(); // Attende un segnale
    }

    return 0;
}
```
---

## 13. IPC: Pipe, FIFO e Memoria Condivisa (mmap)
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 13.0 Cos'è l'IPC (Inter-Process Communication)

L'acronimo **IPC** sta per **Inter-Process Communication** (*Comunicazione Inter-Processo* o *tra Processi*).

> **Definizione e Motivazione:**  
> Nei sistemi operativi moderni, ogni processo viene eseguito all'interno di uno **spazio di memoria virtuale protetto e isolato**. Per ragioni di sicurezza e stabilità, un processo non può accedere arbitrariamente alla memoria di un altro processo.  
> L'**IPC** è l'insieme dei meccanismi forniti dal kernel che permettono a due o più processi distinti di:
> 1. **Scambiarsi informazioni e dati** (mediante *Message Passing* o *Shared Memory*).
> 2. **Sincronizzare** la propria esecuzione e l'accesso alle risorse.

**Principali Meccanismi IPC in Unix/Linux:**
* **Pipe Ordinarie (Anonime):** Canali unidirezionali in RAM tra processi imparentati (`pipe()`).
* **FIFO (Named Pipes):** Canali unidirezionali rappresentati come file speciali nel filesystem per processi indipendenti (`mkfifo()`).
* **Memoria Condivisa (`mmap` / `shm_open`):** Spazio di memoria comune mappato direttamente negli indirizzi virtuali (il più veloce, zero-copy).
* **Segnali:** Notifiche asincrone di eventi software (`signal()`, `kill()`).
* **Code di Messaggi (Message Queues):** Scambio di messaggi discreti e strutturati.
* **Socket:** Comunicazione bidirezionale locale (Unix domain) o remota in rete (TCP/UDP).

---

### 13.1 Pipe Ordinarie

Le **pipe** sono canali di comunicazione **unidirezionali** tra processi con relazione parentale.

```c
#include <unistd.h>
int pipe(int pipeChildFather[2]);
// pipeChildFather[0] = estremità di lettura (read-end)
// pipeChildFather[1] = estremità di scrittura (write-end)
```

**Funzionamento:**
- Modello **produttore-consumatore**
- Il produttore scrive su `pipeChildFather[1]`, il consumatore legge da `pipeChildFather[0]`
- **Occorre chiudere le imboccature non utilizzate** (altrimenti il lettore non riceve EOF)

**Esempio:**
```c
int pipeChildFather[2];
pipe(pipeChildFather);
pid_t pid = fork();
//Si creano due pipe una padre-figlio e una figlio-padre per comunicare tra i due processi 
if (pid == 0) {                      // Figlio (lettore)
    close(pipeChildFather[1]);       // chiude la scrittura nel processo figlio non del padre
    char buf[64];
    while (read(pipeChildFather[0], buf, sizeof(buf)) > 0)// LEGGE DALLA PIPE
        write(STDOUT_FILENO, buf, 5);                     // SCRIVE NELLO STDOUT 5 byte del buffer
    close(pipeChildFather[0]);      // chiude la lettura nel processo figlio non del padre
    _exit(0);
} else {                             // Padre (scrittore)
    close(pipeChildFather[0]);       // chiude la lettura nel processo padre non del figlio
    write(pipeChildFather[1], "ciao\n", 5);               // SCRIVE SULLA PIPE
    close(pipeChildFather[1]);       // segnala EOF nel processo padre non del figlio
    wait(NULL);
}
```

Il figlio puo' leggere anche se il padre ha chiuso la lettura perche' sono due file descriptors distinti. 
Dopo la fork(), sia il Padre che il Figlio hanno accesso in lettura e scrittura alla stessa pipe! Ci sono in totale 4 file descriptor aperti verso la stessa pipe.
Il padre per comunicare col figlio deve chiudere l'estremo di lettura e il figlio deve chiudere l'estremo di scrittura.

#### Variante Atomica: `pipe2()`
Nelle versioni moderne di Linux (kernel 2.6.27+) e nelle prove d'esame di LSO (es. *Lezione 28 - Prova Parziale*), si utilizza frequentemente la system call **`pipe2()`**, che estende `pipe()` consentendo di impostare atomicamente flag sui descrittori di file generati:

```c
#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>

int pipe2(int pipefd[2], int flags);
```

- **`flags` supportati:**
  - `0`: Comportamento identico alla normale `pipe()`.
  - `O_NONBLOCK`: Imposta il flag di I/O non bloccante su entrambe le estremità (`pipefd[0]` e `pipefd[1]`). Le operazioni di `read()` o `write()` che si bloccherebbero ritornano immediatamente con errore `EAGAIN` o `EWOULDBLOCK`, evitando una successiva chiamata esplicita a `fcntl(pipefd[i], F_SETFL, O_NONBLOCK)`.
  - `O_CLOEXEC`: Imposta il flag *Close-on-Exec* (`FD_CLOEXEC`) su entrambi i descrittori. Se uno dei processi invoca successivamente una funzione della famiglia `exec()`, i descrittori della pipe vengono chiusi automaticamente nel nuovo programma, prevenendo fughe involontarie di descrittori (*descriptor leak*).
  - I flag possono essere combinati con l'operatore OR bit a bit: `O_NONBLOCK | O_CLOEXEC`.

**Esempio di utilizzo d'esame:**
```c
int pfd[2];
if (pipe2(pfd, O_NONBLOCK | O_CLOEXEC) == -1) {
    perror("pipe2");
    exit(EXIT_FAILURE);
}
```


### 13.2 Pipe con Nome (FIFO)

Le **Named Pipes** (o FIFO) superano il limite principale delle pipe ordinarie: permettono la comunicazione tra processi **senza alcuna relazione di parentela** (es. client e server indipendenti).

**Caratteristiche Principali:**
- **File Speciale:** Una FIFO appare come un file speciale nel file system. Qualsiasi processo con i giusti permessi vi può accedere usando le normali syscall (`open`, `read`, `write`, `close`).
- **Persistenza nel File System:** Il nodo sul file system esiste finché non viene esplicitamente eliminato (con `unlink()` o tramite il comando shell `rm`). Tuttavia, i *dati* passati nella FIFO risiedono in memoria (buffer gestito dal kernel) e non sul disco.
- **Unidirezionali:** Come le pipe ordinarie, il flusso dati è unidirezionale. Per una comunicazione bidirezionale servono due FIFO distinte.

**Creazione e Cancellazione:**
```c
#include <sys/stat.h>
#include <unistd.h>

// Crea una FIFO. Ritorna 0 in caso di successo, -1 in caso di errore
int mkfifo(const char *pathname, mode_t mode);

// Elimina la FIFO dal file system
int unlink(const char *pathname);
```
- `pathname`: Il percorso nel file system dove creare la FIFO.
- `mode`: I permessi del file speciale (es. `0666` per lettura/scrittura per tutti).

**Comportamento dell'open() su FIFO:**
L'apertura di una FIFO prevede una **sincronizzazione intrinseca** tra lettore e scrittore. Se uno dei due manca, l'altro si blocca in attesa.

| Operazione | Comportamento Default (Bloccante) | Con flag `O_NONBLOCK` |
|------------|-----------------------------------|-----------------------|
| `open("f", O_RDONLY)` | **Blocca** finché un processo non apre la FIFO in scrittura | Non blocca (ha successo immediato) |
| `open("f", O_WRONLY)` | **Blocca** finché un processo non apre la FIFO in lettura | Fallisce con errore `ENXIO` (se non c'è già un lettore) |
| `open("f", O_RDWR)`   | **Sconsigliata** / Comportamento indefinito in POSIX (se supportato non blocca) | - |

**Esempio di Utilizzo (Scrittore e Lettore separati):**
```c
// --- PROCESSO SCRITTORE ---
mkfifo("mia_fifo", 0666);
int fd = open("mia_fifo", O_WRONLY); // Si blocca qui se non c'è ancora un lettore
write(fd, "Messaggio!", 11);
close(fd);
```
```c
// --- PROCESSO LETTORE ---
int fd = open("mia_fifo", O_RDONLY); // Si blocca qui se non c'è ancora uno scrittore
char buf[128];
read(fd, buf, sizeof(buf));
printf("Ricevuto: %s\n", buf);
close(fd);
unlink("mia_fifo"); // Pulizia finale
```

### 13.3 Memoria Condivisa con `mmap`

La **memoria condivisa** è il meccanismo IPC più veloce perché permette a più processi di mappare la stessa area di memoria nel proprio spazio di indirizzamento virtuale. Qualsiasi modifica effettuata da un processo è immediatamente visibile agli altri, senza overhead di chiamate di sistema (`read`/`write`) e senza copiare i dati tra user-space e kernel-space (zero-copy).

La system call `mmap` mappa file, dispositivi o memoria anonima nello spazio di indirizzamento di un processo.

**Definizione e Parametri:**
```c
#include <sys/mman.h>

void *mmap(void *address, size_t length, int protect, int flags, int filedes, off_t offset);
int munmap(void *addr, size_t length);              // Rilascia la memoria mappata
int msync(void *addr, size_t length, int flags);     // Sincronizza la memoria su disco
```

| Parametro | Significato |
|-----------|-------------|
| `address` | Indirizzo virtuale suggerito per il mapping. Solitamente si passa `NULL` per far decidere liberamente al kernel. |
| `length`  | Dimensione dell'area di memoria da mappare in byte (il kernel allocherà multipli della *page size*, es. 4096 byte). |
| `protect` | Permessi di accesso della CPU/MMU sulle pagine (`PROT_READ`, `PROT_WRITE`, `PROT_EXEC`, `PROT_NONE`). |
| `flags`   | Tipo di mapping e condivisione (`MAP_SHARED`, `MAP_PRIVATE`, combinabili con `MAP_ANONYMOUS`). |
| `filedes` | File descriptor del file da mappare (oppure `-1` se si usa `MAP_ANONYMOUS`). |
| `offset`  | Punto di partenza all'interno del file (deve essere rigorosamente multiplo della page size del sistema). |

---

#### 1. I Flag di Protezione (`protect` / `PROT_*`)

Il parametro `protect` imposta i bit di protezione nella tabella delle pagine del processo. Se la CPU/processo tenta un'operazione non consentita, la **MMU** (Memory Management Unit) solleva un'eccezione hardware che il kernel converte nel segnale **`SIGSEGV`** (*Segmentation Fault*).

I flag possono essere combinati con l'operatore OR bit a bit (`|`):

| Flag | Significato | Dettagli Tecnici / Casi d'Uso |
| :--- | :--- | :--- |
| **`PROT_READ`** | **Lettura** | Il processo può leggere byte dall'area di memoria. |
| **`PROT_WRITE`** | **Scrittura** | Il processo può modificare l'area di memoria. *(Nota: per mappare un file con `PROT_WRITE`, il file deve essere aperto in modalità scrittura come `O_RDWR`, a meno di non usare `MAP_PRIVATE`).* |
| **`PROT_EXEC`** | **Esecuzione** | La CPU può eseguire istruzioni macchina presenti in quell'area (usato per caricare codice da binari ELF e librerie condivise `.so`, o nei compilatori JIT). |
| **`PROT_NONE`** | **Nessun Accesso** | La pagina non può essere letta, scritta o eseguita. |

> **A cosa serve `PROT_NONE`?**
> 1. **Guard Pages (Pagine Sentinella):** Creare una pagina inaccessibile ai margini di uno stack o di un buffer. Se un buffer overflow sconfina nella guard page, il kernel invia istantaneamente un `SIGSEGV`, bloccando l'attacco prima della corruzione di altri dati.
> 2. **Prenotazione di Indirizzi Virtuali:** Riservare un intervallo contiguo nello spazio di indirizzamento virtuale senza consumare RAM fisica fino all'effettivo utilizzo.

---

#### 2. I Flag di Mappatura (`flags`)

Ogni chiamata a `mmap()` deve contenere **obbligatoriamente** o `MAP_SHARED` o `MAP_PRIVATE`:

* **`MAP_SHARED` (Condivisione Reale):**  
  - Le modifiche apportate alla memoria sono **visibili a tutti gli altri processi** che mappano la stessa area.  
  - Se la mappatura è associata a un file (`fd >= 0`), le scritture in RAM vengono **salvate nel file su disco** (dal kernel o tramite `msync()`).

* **`MAP_PRIVATE` (Copy-on-Write / Isolamento):**  
  - L'area di memoria è **privata** del processo chiamante.  
  - Le modifiche **NON sono visibili** agli altri processi e **NON vengono riversate nel file sottostante**.  
  - **Meccanismo Copy-on-Write (CoW):** Inizialmente i processi condividono le stesse pagine fisiche in sola lettura. Appena un processo prova a scrivere, la MMU intercetta l'operazione, il kernel **duplica solo quella specifica pagina di 4KB** in RAM e permette la scrittura sulla copia privata isolata.

* **`MAP_ANONYMOUS` (o `MAP_ANON` — Memoria RAM Anonima):**  
  - La mappatura **non è associata ad alcun file** (`filedes` deve essere posto a `-1` e `offset` a `0`).  
  - Alloca blocchi di RAM vergine direttamente dal kernel, garantendo che siano **completamente azzerati** (tutti i byte a `0`) per motivi di sicurezza.

---

#### 3. Matrice delle 4 Combinazioni Fondamentali

| Tipo di Mapping | `MAP_SHARED` | `MAP_PRIVATE` |
| :--- | :--- | :--- |
| **File-backed**<br>(con `fd` valido) | **IPC tra processi indipendenti + Persistenza:**<br>Modifiche condivise tra processi e salvate permanentemente sul file su disco. | **Caricamento Codice / CoW:**<br>Modifica il file in RAM senza alterare il file su disco (es. caricamento del segmento dati/testo delle librerie `.so`). |
| **`MAP_ANONYMOUS`**<br>(con `fd = -1`) | **IPC tra Padre e Figlio (RAM):**<br>Memoria condivisa in RAM tra processi imparentati generati con `fork()`. | **Allocazione Pura di RAM:**<br>Memoria privata e azzerata. È il meccanismo usato internamente da `malloc()` per blocchi grandi (>128 KB). |

---

#### 4. `msync()` su `MAP_PRIVATE` vs `MAP_SHARED`

La funzione `msync(addr, length, flags)` forza il flush immediato delle pagine modificate dalla RAM al disco:

* **Su `MAP_SHARED`:**  
  - Le normali scritture in memoria (`*map = ...`, `strcpy()`) modificano istantaneamente la **RAM (Page Cache)** e marcano le pagine come *dirty* (visibili subito agli altri processi).  
  - Il kernel trasferisce i dati sul supporto fisico in modo **asincrono** (in background).  
  - Invocare `msync(..., MS_SYNC)` **forza la scrittura fisica immediata** sul disco/SSD, bloccando il processo finché l'I/O non è completato.
* **Su `MAP_PRIVATE`:**  
  - Se chiami `msync()` su una mappatura privata, la chiamata **ritorna 0 (successo) ma è una No-Op (non fa nulla)**. Le pagine modificate sono state clonate via Copy-on-Write e sono totalmente disconnesse dal file di origine; il file su disco **rimane al 100% inalterato**.

---

#### 5. Esempi Pratici di Codice

**Esempio A — `MAP_SHARED | MAP_ANONYMOUS` (IPC Padre-Figlio in RAM):**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    // Alloca 4KB di RAM condivisa tra padre e figlio (nessun file su disco)
    char *shared_mem = mmap(NULL, 4096,
                            PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    strcpy(shared_mem, "Valore iniziale del Padre");

    if (fork() == 0) {
        // FIGLIO: modifica la memoria condivisa
        sprintf(shared_mem, "Saluti dal Figlio (PID %d)!", getpid());
        _exit(0);
    }

    wait(NULL); // Attende il figlio
    printf("[Padre] Letto dalla memoria condivisa: \"%s\"\n", shared_mem);
    munmap(shared_mem, 4096);
    return 0;
}
```

**Esempio B — `MAP_SHARED` su File (Persistenza su disco):**
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int main() {
    int fd = open("dati.bin", O_RDWR | O_CREAT, 0666);
    ftruncate(fd, 4096); // Estende il file a 4096 byte

    // Mappa il file in modalità SHARED
    char *map = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd); // Il descrittore può essere chiuso subito dopo mmap

    // 1. Modifica la pagina in RAM (nella Page Cache del kernel).
    // NOTA: Non scrive fisicamente su disco in questo istante!
    strcpy(map, "Testo modificato nella memoria condivisa");

    // 2. È msync() che riversa fisicamente e subito i byte dalla RAM al disco:
    msync(map, 4096, MS_SYNC);
    munmap(map, 4096);
    return 0;
}
```

---

**Confronto IPC: Pipe / FIFO vs Memoria Condivisa (`mmap`)**

| Aspetto | Pipe / FIFO (Message Passing) | Memoria Condivisa (`mmap`) |
|---------|-------------------------------|-----------------------------|
| **Velocità** | Più lenta (ogni I/O richiede `read()`/`write()` e un context switch nel kernel) | Molto più veloce (accesso diretto in RAM, zero-copy IPC) |
| **Coordinazione** | Sincronizzazione automatica gestita dal kernel (lettore attende se vuoto, ecc.) | **Responsabilità dello sviluppatore!** Occorre usare meccanismi come semafori o mutex per evitare race conditions |
| **Formato Dati** | Flusso di byte non strutturato (stream unidirezionale) | Spazio di memoria indirizzabile, ideale per strutture dati complesse |
| **Complessità** | Più semplice | Richiede sincronizzazione esplicita |

---

## 14. Thread e Concorrenza
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 14.1 Motivazioni

Un **thread** è l'unità base di utilizzo della CPU. Rispetto ai processi:
- I thread **condividono** codice, dati e file aperti
- Ogni thread ha il proprio **PC**, **registri** e **stack**
- Il **context switch** tra thread è molto più veloce

**Vantaggi del multithreading:**
- **Risposta**: l'applicazione resta responsiva se un thread è bloccato
- **Condivisione risorse**: più semplice di IPC
- **Economia**: creazione e switching più leggeri
- **Scalabilità**: sfrutta architetture multicore

### 14.2 Concorrenza vs Parallelismo

- **Concorrenza**: più task fanno progresso contemporaneamente (anche su single-core via scheduling)
- **Parallelismo**: più task eseguiti simultaneamente su più unità di calcolo

### 14.3 Modelli di Multithreading

| Modello | Descrizione |
|---------|-------------|
| **Many-to-One** | Molti thread user → 1 kernel thread. Un blocco blocca tutto. |
| **One-to-One** | 1 thread user → 1 kernel thread. Maggiore concorrenza. (Windows, Linux) |
| **Many-to-Many** | Molti thread user → molti kernel thread. Il SO gestisce il numero. |

Linux usa il modello **One-to-One** (ogni pthread corrisponde a un task del kernel).

### 14.4 API POSIX Pthreads

#### Creazione di un thread
```c
#include <pthread.h>

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*start_routine)(void *), void *arg);
// thread: puntatore al thread ID
// attr: attributi (NULL per default)
// start_routine: funzione da eseguire
// arg: argomento passato alla funzione
```

#### Attesa di un thread
```c
int pthread_join(pthread_t thread, void **retval);
// Si blocca finché il thread non termina
// retval: puntatore al valore restituito dal thread
```

#### Terminazione di un thread
```c
void pthread_exit(void *retval);
// Termina il thread corrente, restituendo retval
```

#### Identificazione
```c
pthread_t pthread_self(void);  // ID del thread corrente
```

#### Detach (Scollegamento)
```c
int pthread_detach(pthread_t thread);
```
**Spiegazione:**
Di default, un thread è "joinable", ovvero le sue risorse (come lo stack e l'exit status) non vengono liberate finché un altro thread non chiama `pthread_join()` su di esso (simile all'attesa dei processi zombie).
La funzione `pthread_detach()` scollega il thread in modo che, al momento della sua terminazione, le sue risorse vengano **automaticamente e immediatamente rilasciate** dal sistema, senza bisogno di alcun `pthread_join()`.
- **Uso:** È utile per thread eseguiti in background di cui non ci interessa attendere la fine né raccogliere il valore di ritorno.
- *Nota:* Una volta "detached", un thread non può più essere "joinato".

**Esempio completo:**
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Corpo del thread, ossia la funzione che verrà eseguita dal thread
void *tbody(void *arg) {
    int *pi = (int *)arg;
    printf("Thread: valore ricevuto = %d\n", *pi);
    *pi = 10;  // modifica dato condiviso
    int *ret = malloc(sizeof(int)); // alloco memoria per il valore di ritorno
    *ret = 50;
    pthread_exit((void *)ret); // si deve fare il cast a void * in quanto pthread_exit accetta solo void *
}

int main(void) {
    pthread_t mythread;
    int i = 0;
    void *result;
    pthread_create(&mythread, NULL, tbody, (void *)&i);
    pthread_join(mythread, &result);
    // *(int *)result fa il cast a puntatore a int del puntatore result e ne dereferenzia il valore
    // Serve in quanto pthread_exit accetta solo void * e in main vogliamo stampare un int
    printf("Main: i = %d, thread restituito %d\n", i, *(int *)result);
    // Stampa: "Main: i = 10, thread restituito 50" 
    // (l'iniziale 0 è stato modificato dal thread in 10 e il thread ha restituito 50)
    free(result); // libero la memoria allocata dal thread
    return 0;
}
```

> **Compilazione**: `gcc -pthread -o prog prog.c`

### 14.5 Thread in Linux

- Linux tratta i thread come **task** (unifica processi e thread)
- `pthread_create` internamente usa la system call **`clone()`**
- Flag di `clone()`:  `CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD`
- Per vedere i kernel thread: `ps -Lf <PID>`, `ps -T -p <PID>`, `ls /proc/<PID>/task`

#### Architettura LWP (Lightweight Processes) in Linux
In Linux il kernel adotta un modello di multithreading **1:1**: non esiste una distinzione ontologica tra processi e thread a livello di scheduler. Ogni thread creato tramite `pthread_create()` corrisponde internamente a un **Lightweight Process (LWP)**, cioè un task (`task_struct`) dotato di:
- Un proprio identificatore univoco nel kernel chiamato **TID (Thread ID)** o **SPID**, ottenuto tramite la system call Linux `gettid()`.
- Un proprio stato di scheduling, registri CPU e stack utente privato.
- Lo stesso **PID** (che per il kernel corrisponde al *Thread Group ID*, TGID) e la condivisione dello spazio di memoria (`CLONE_VM`), della tabella dei descrittori (`CLONE_FILES`) e dei gestori di segnale (`CLONE_SIGHAND`).

> **Differenza cruciale da ricordare all'esame:**
> - `pthread_t` (restituito da `pthread_self()`): È un identificatore opaco gestito dalla libreria POSIX Pthreads, valido **solo all'interno dello spazio d'indirizzamento del processo**.
> - `TID / LWP`: È un intero assegnato dal kernel Linux globale al sistema, visibile agli strumenti di amministrazione (`ps`, `top`) e mappato nella directory `/proc/<PID>/task/<TID>`.

#### Comandi di Monitoraggio Thread da Shell
Nelle esercitazioni e nelle prove d'esame (es. *Lezione 25 ed Esercitazione 2 Lezione 27*), viene richiesto frequentemente di ispezionare i thread dei processi:
1. **`ps -Lf <PID>`**: Mostra tutti i thread (LWP) del processo.
   - Colonna `LWP`: ID del Lightweight Process (TID).
   - Colonna `NLWP`: Numero totale di thread attivi nel processo (*Number of LWPs*).
2. **`ps -T -p <PID>`**: Visualizza i thread con la colonna `SPID` (TID).
3. **`top -H`**: Avvia `top` in modalità thread, mostrando ciascun LWP come riga indipendente allocata sulla CPU.
4. **`ls /proc/<PID>/task`**: Elenca le sottocartelle corrispondenti a ciascun TID del processo.

#### Pipeline Bash per l'Ispezione dei Thread (Quesito d'Esame Tipico)
Trovare i primi 3 processi del sistema che hanno il maggior numero di thread (LWP), mostrando PID, numero di thread e nome del comando:
```bash
ps -eo pid,nlwp,comm | sort -k2 -nr | head -n 3
```
- `ps -eo pid,nlwp,comm`: Estrae solo PID, conteggio LWP e nome comando.
- `sort -k2 -nr`: Ordina numericamente (`-n`) e in modo decrescente (`-r`) sulla seconda colonna (`-k2`, cioè `NLWP`).
- `head -n 3`: Seleziona i primi 3 processi con più thread.


### 14.6 Cancellazione Thread

La cancellazione permette a un thread di forzare la terminazione di un altro thread. Questo meccanismo richiede la cooperazione del thread "bersaglio", poiché terminare bruscamente un thread potrebbe lasciare dati in stati inconsistenti o risorse bloccate (come i mutex).

```c
int pthread_cancel(pthread_t thread);
```
- **Scopo:** Invia una **richiesta** di cancellazione al `thread` specificato. La richiesta non ferma istantaneamente il thread, ma il modo e il momento in cui reagirà dipendono dal suo stato e tipo di cancellazione.

```c
int pthread_setcancelstate(int state, int *oldstate);
```
- **Scopo:** Imposta lo stato di cancellabilità del thread chiamante.
- **`state`:**
  - `PTHREAD_CANCEL_ENABLE` (default): Il thread accetta e gestisce le richieste di cancellazione.
  - `PTHREAD_CANCEL_DISABLE`: Le richieste di cancellazione rimangono in sospeso. Il thread le ignorerà fino a quando non riabiliterà la cancellazione.
- **`oldstate`:** Se non è `NULL`, vi viene salvato lo stato precedente, utile per ripristinarlo in seguito.

```c
void pthread_testcancel(void);
```
- **Scopo:** Crea esplicitamente un **cancellation point** (punto di cancellazione). Se c'è una richiesta di cancellazione in sospeso per il thread (e lo stato è `ENABLE`), chiamando questa funzione il thread terminerà in quel preciso istante.


***EXTRA!!!***
**Tipi di Cancellazione (quando abilitata):**
Il tipo di cancellazione si imposta dall'interno del thread con la funzione:
```c
int pthread_setcanceltype(int type, int *oldtype);
```
- **`type`:**
  1. `PTHREAD_CANCEL_DEFERRED` (Ritardata - Default): Il thread viene terminato solo quando raggiunge un *cancellation point*. Molte system call bloccanti (`sleep`, `wait`, `pthread_cond_wait`) fungono automaticamente da cancellation point. Nei calcoli intensivi (senza system call bloccanti), si usa `pthread_testcancel()` per creare dei checkpoint manuali in cui è sicuro interrompere l'esecuzione.
  2. `PTHREAD_CANCEL_ASYNCHRONOUS` (Asincrona): Il thread può essere cancellato in **qualsiasi istante**. È molto pericolosa e sconsigliata, tranne per thread che non allocano risorse e non usano lock, perché rischia di interrompere il thread a metà di un'operazione critica lasciando lock presi o memoria pendente.
- **`oldtype`:** Se non è `NULL`, vi viene salvato il tipo di cancellazione precedente.

#### Routine di Pulizia: `pthread_cleanup_push()` e `pthread_cleanup_pop()`
Se un thread viene terminato forzatamente mentre detiene un mutex o risorse allocate (memoria `malloc`, file aperti), il mutex rimarrà bloccato per sempre, causando un **deadlock permanente** per gli altri thread.

Per prevenire questo scenario, lo standard POSIX fornisce uno stack di **cleanup handlers** registrati tramite le macro `pthread_cleanup_push()` e `pthread_cleanup_pop()`.

```c
#include <pthread.h>

void pthread_cleanup_push(void (*routine)(void *), void *arg);
void pthread_cleanup_pop(int execute);
```

- **Come funzionano:**
  - `pthread_cleanup_push()` registra la funzione `routine` (passandole `arg`) in cima a uno stack LIFO associato al thread chiamante.
  - Se il thread **viene cancellato** (raggiunge un cancellation point con cancellazione abilitata) oppure invoca **`pthread_exit()`**, il runtime esegue automaticamente tutte le funzioni di pulizia registrate, dall'ultima alla prima.
  - `pthread_cleanup_pop(execute)` rimuove l'ultimo handler registrato. Se l'argomento `execute` è diverso da zero (`1`), la routine viene eseguita prima di essere rimossa; se è `0`, viene solo rimossa senza eseguirla.
  - **Vincolo sintattico obbligatorio:** Nello standard C, `pthread_cleanup_push` e `pthread_cleanup_pop` sono implementate come macro che aprono e chiudono blocchi con parentesi graffe `{` e `}`. Devono **sempre** apparire in coppia nello stesso blocco e allo stesso livello di annidamento!

**Esempio di sblocco sicuro del mutex in caso di cancellazione:**
```c
void unlock_mutex(void *arg) {
    pthread_mutex_t *m = (pthread_mutex_t *)arg;
    pthread_mutex_unlock(m);
    printf("[cleanup] Mutex sbloccato dal cleanup handler!
");
}

void *worker_sicuro(void *arg) {
    pthread_mutex_lock(&mymutex);
    // Registra la funzione di sblocco
    pthread_cleanup_push(unlock_mutex, (void *)&mymutex);

    while (1) {
        // Operazione critica con potenziale punto di cancellazione
        pthread_testcancel();
        sleep(1);
    }

    // Se il ciclo finisse normalmente, rimuove ed esegue lo sblocco
    pthread_cleanup_pop(1);
    return NULL;
}
```

#### Esempio Completo di Cancellazione Controllata (`thread_canc_test.c`)
Dal materiale didattico delle slide (*Slide 821, 826*), ecco il pattern d'esame per gestire la cancellazione:

```c
// thread_canc_test.c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

/* Thread worker */
void *worker(void *arg) {
    int oldstate;
    /* 1. Disabilito temporaneamente la cancellazione (sezione protetta) */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldstate);
    printf("[thread] Cancellazione disabilitata
");
    sleep(2);

    /* 2. Riabilito la cancellazione */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
    printf("[thread] Cancellazione abilitata, pronto a ricevere cancellazioni
");

    /* 3. Loop con cancellation point esplicito */
    while (1) {
        printf("[thread] Lavoro in corso...
");
        sleep(1);
        pthread_testcancel();  // Checkpoint manuale di cancellazione
    }
    return NULL;
}

int main(void) {
    pthread_t tid;
    pthread_create(&tid, NULL, worker, NULL);

    sleep(3);
    printf("[main] Invio richiesta di cancellazione con pthread_cancel()...
");
    pthread_cancel(tid);

    /* Il join raccoglie il thread cancellato */
    void *res;
    pthread_join(tid, &res);
    if (res == PTHREAD_CANCELED) {
        printf("[main] Il thread worker e' stato correttamente terminato per cancellazione!
");
    }
    return 0;
}
```


---


### 14.7 Thread-Specific Data (TSD) — Dati Specifici del Thread
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

In un programma multithread, tutte le variabili globali e statiche sono condivise tra tutti i thread. Tuttavia, vi sono scenari in cui ogni thread ha bisogno di memorizzare una **variabile privata con visibilità globale** (accessibile da qualsiasi funzione chiamata dal thread, senza doverla passare esplicitamente come parametro).

Lo standard POSIX risolve questo problema con i **Thread-Specific Data (TSD)** (noti anche come *Thread-Local Storage*).

#### Funzioni dell'API POSIX TSD
```c
#include <pthread.h>

// 1. Creazione della chiave globale
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));

// 2. Associazione del valore per il thread chiamante
int pthread_setspecific(pthread_key_t key, const void *value);

// 3. Recupero del valore per il thread chiamante
void *pthread_getspecific(pthread_key_t key);

// 4. Distruzione della chiave globale
int pthread_key_delete(pthread_key_t key);
```

- **`pthread_key_create(&key, destructor)`**: Alloca una chiave globale di tipo `pthread_key_t`. La chiave è visibile a tutti i thread, ma ciascun thread potrà associarvi un puntatore indipendente. Il parametro `destructor` (opzionale, può essere `NULL`) specifica una funzione invocata automaticamente alla terminazione del thread per liberare la memoria puntata dal valore associato.
- **`pthread_setspecific(key, value)`**: Associa il puntatore `value` alla chiave `key` **esclusivamente per il thread che effettua la chiamata**.
- **`pthread_getspecific(key)`**: Restituisce il valore precedentemente associato alla chiave dal thread corrente (ritorna `NULL` se non è ancora stato impostato).
- **`pthread_key_delete(key)`**: Dealloca la chiave globale. Non invoca automaticamente i distruttori.

#### Codice d'Esempio Completo (`thread_tsd.c`, Slide 829, 860)
```c
// thread_tsd.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/* Chiave globale per i dati specifici del thread */
pthread_key_t chiave;

/* Funzione eseguita da ogni thread */
void *worker(void *arg) {
    int *id = (int *)arg;   // ID passato dal main
    int *val = malloc(sizeof(int));
    *val = *id * 10;        // Calcolo un valore privato solo per questo thread

    /* Associo il valore alla chiave globale (specifico per questo thread) */
    pthread_setspecific(chiave, val);

    /* Recupero il valore dalla chiave e lo stampo */
    int *mio_val = (int *)pthread_getspecific(chiave);
    printf("[thread %d] Valore recuperato da TSD = %d
", *id, *mio_val);

    free(val);  // Libero la memoria allocata
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    /* Creo la chiave globale */
    pthread_key_create(&chiave, NULL);

    /* Creo due thread, ciascuno con il proprio id */
    pthread_create(&t1, NULL, worker, &id1);
    pthread_create(&t2, NULL, worker, &id2);

    /* Attendo la fine dei thread */
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    /* Elimino la chiave */
    pthread_key_delete(chiave);
    return 0;
}
```

#### Caso d'Uso d'Esame: Associazione Socket Descriptor per Handler SIGPIPE (Slide 1142–1143)
In un server di rete multithread, quando un client chiude bruscamente la connessione mentre un thread tenta di scrivervi con `write()` o `send()`, il kernel genera il segnale **`SIGPIPE`**. Se il server gestisce `SIGPIPE` con un signal handler personalizzato, come fa l'handler a sapere **quale socket** ha generato l'errore tra le decine di thread concorrenti?

La soluzione elegante presentata dal docente nelle slide consiste nell'usare una chiave TSD globale `key_sd`:
```c
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdint.h>

pthread_key_t key_sd;

static void sigpipe_handler(int signo) {
    (void)signo;
    // Recupera il file descriptor del socket associato al thread corrente
    int sd = (int)(intptr_t)pthread_getspecific(key_sd);
    if (sd >= 0) {
        close(sd);
        // Evita doppie chiusure impostando il valore a -1
        pthread_setspecific(key_sd, (void *)(intptr_t)-1);
    }
    const char msg[] = "SIGPIPE intercettato: chiuso socket del thread corrente
";
    write(STDERR_FILENO, msg, sizeof(msg) - 1);
}

// Nel main del server:
pthread_key_create(&key_sd, NULL);
signal(SIGPIPE, sigpipe_handler);

// All'interno della routine di ciascun thread worker:
void *client_handler(void *arg) {
    int client_sd = *(int *)arg;
    // Salva il descrittore nella memoria locale del thread
    pthread_setspecific(key_sd, (void *)(intptr_t)client_sd);

    // Esegue letture e scritture sul socket...
    // Se scatta SIGPIPE, il gestore chiuderà esattamente client_sd!
    return NULL;
}
```

---

## 15. Sincronizzazione: Mutex, Condition Variable, Semafori
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 15.1 Il Problema della Sezione Critica

Quando più thread accedono a **dati condivisi**, possono verificarsi **race condition** (corse critiche).

**Requisiti per la soluzione:**
1. **Mutua esclusione**: un solo processo alla volta nella sezione critica
2. **Progresso**: la decisione di chi entra non può essere posticipata indefinitamente
3. **Attesa limitata** (bounded waiting): limite al numero di sorpassi

### 15.2 Mutex — Mutua Esclusione

```c
#include <pthread.h>

// Inizializzazione statica
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Inizializzazione runtime
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL);

// Operazioni
pthread_mutex_lock(&mutex);     // Acquisisce il lock. Se è già bloccato da un altro thread, il thread chiamante si sospende in attesa (bloccante).
pthread_mutex_unlock(&mutex);   // Rilascia il lock, permettendo a uno dei thread in attesa di sbloccarsi e acquisirlo.
pthread_mutex_destroy(&mutex);  // Distrugge il mutex

***EXTRA***
pthread_mutex_trylock(&mutex);  // Tenta di acquisire il lock. Se è già bloccato, NON si sospende ma ritorna immediatamente un errore (EBUSY).
```

**Esempio:**
```c
int myglobal = 0;
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_function(void *arg) {
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&mymutex);
        myglobal = myglobal + 1;
        pthread_mutex_unlock(&mymutex);
        sleep(1);
    }
    return NULL;
}
```

### 15.3 Condition Variable — Variabili di Condizione

Permettono a un thread di **attendere** che una determinata condizione (definita dal programmatore tramite normali variabili condivise) diventi vera, senza fare **busy waiting** (il busy waiting è un ciclo infinito a vuoto, che tiene la CPU costantemente occupata al 100% per controllare ripetutamente una variabile). Le condition variables, invece, addormentano il thread (0% CPU) finché non viene esplicitamente "svegliato" da un altro.

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Attendi che la condizione sia vera
// Viene sempre preceduto da `pthread_mutex_lock(&mutex)`
pthread_cond_wait(&cond, &mutex);
// ATOMICAMENTE: rilascia il mutex + si mette in attesa
// Quando si risveglia, deve riacquisire il mutex prima di proseguire.
// Se il mutex è tenuto da un altro thread, si blocca in attesa di poterlo prendere (come una normale lock)

// Attendi con timeout (simile a wait, ma con deadline assoluta)
pthread_cond_timedwait(&cond, &mutex, &timeout);

// Risveglia UN SOLO thread in attesa
pthread_cond_signal(&cond);

// Risveglia TUTTI i thread in attesa
pthread_cond_broadcast(&cond);

// Distruzione della condition variable
pthread_cond_destroy(&cond);
```

**Spiegazione delle Funzioni:**
*   `pthread_cond_signal(&cond)`: Risveglia **uno solo** dei thread che sono in attesa. Si usa quando la modifica dei dati permette a un solo thread alla volta di poter lavorare (es. hai inserito 1 solo nuovo elemento in una coda).
*   `pthread_cond_broadcast(&cond)`: Risveglia **tutti** i thread attualmente in attesa. Ognuno di essi proverà ad acquisire il mutex (uno alla volta). Si usa quando un evento cambia radicalmente lo stato e permette a più thread di sbloccarsi contemporaneamente (es. impostazione di una variabile "sistema_pronto = true").
*   `pthread_cond_destroy(&cond)`: Elimina la condition variable liberando la memoria e le risorse allocate dal SO. Da chiamare solo alla fine, quando nessun thread è più in attesa su di essa.
*   `pthread_cond_timedwait(&cond, ...)`: Fa esattamente la stessa cosa di `wait`, ma accetta un parametro aggiuntivo (`timeout` di tipo `timespec`) che rappresenta un orario assoluto (una "deadline"). Se il thread non riceve nessuna signal entro quell'orario esatto, si sveglia da solo e la funzione ritorna un codice d'errore speciale (`ETIMEDOUT`).

> **Pattern fondamentale**: si usa **sempre**  `while` (mai `if` o nessuna condizione) attorno a `pthread_cond_wait`:
> ```c
> pthread_mutex_lock(&mtx);
> while (!condizione_soddisfatta) {
>     pthread_cond_wait(&cond, &mtx);
> }
> // ... sezione critica ...
> pthread_mutex_unlock(&mtx);
> ```
> **Perché si usa il `while`?**
> 1. **Spurious Wakeups (Risvegli Spuri):** Le specifiche POSIX permettono al sistema operativo di risvegliare un thread in attesa anche se nessuno ha esplicitamente chiamato una signal/broadcast. Se non ci fosse il while, il thread proseguirebbe con la condizione non valida.
> 2. **Competizione (Signal Stealing):** Tra il momento in cui un thread viene risvegliato (riceve la signal) e il momento in cui riesce effettivamente a ri-acquisire il mutex per procedere, un **altro** thread in esecuzione potrebbe aver acquisito il mutex e modificato di nuovo lo stato (falsificando la condizione). Il `while` garantisce che il thread proceda **solo** se la condizione è *effettivamente* vera al momento esatto in cui ha riottenuto il lock.

```c
#include <time.h>

// Variabile di stato condivisa (predicato booleano: 0 = non pronto, 1 = pronto)
// Le condition variable non hanno memoria interna, serve sempre una variabile di stato!
int ready = 0; 

// Calcola deadline: orario assoluto attuale + N secondi
struct timespec ts;
clock_gettime(CLOCK_REALTIME, &ts);
ts.tv_sec += 2;  // deadline tra 2 secondi

pthread_mutex_lock(&mtx);
int rc = 0;
// Ciclo di attesa: continua a dormire finché 'ready' è 0 E il tempo non è scaduto (rc == 0)
while (!ready && rc == 0) {
    rc = pthread_cond_timedwait(&cond, &mtx, &ts);
    // rc == 0        → svegliato da signal/broadcast prima della deadline
    // rc == ETIMEDOUT → tempo massimo scaduto
}

if (ready) {
    printf("Evento completato con successo entro la deadline (ready = 1)\n");
} else if (rc == ETIMEDOUT) {
    printf("Timeout scaduto: il dato non è diventato pronto in tempo\n");
}
pthread_mutex_unlock(&mtx);
```

> Si usa comunque il `while` per proteggersi da **spurious wakeup** anche con `timedwait`.

**Esempio Pratico (Produttore / Consumatore):**
```c
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// 1. LA NOSTRA CONDIZIONE VERA E PROPRIA (variabile condivisa)
int dati_pronti = 0; 

// 2. STRUMENTI DEL SO per la sincronizzazione
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* consumatore(void* arg) {
    pthread_mutex_lock(&mtx);
    
    // Il consumatore controlla la condizione
    while (dati_pronti == 0) {
        printf("[Consumatore] Dati non pronti. Vado a dormire...\n");
        // Come se si iscrivesse a una newsletter tramite la Condition Variable (cond)
        // e si addormentasse e rilascia il mutex. Una volta che riceve la signal, si risveglia e deve riprendersi il mutex 
        // per poter leggere la variabile condivisa (dati_pronti).
        pthread_cond_wait(&cond, &mtx); 
    }
    
    // Se siamo usciti dal while, significa che abbiamo ripreso il mutex 
    // E che dati_pronti è diventato > 0
    printf("[Consumatore] Risvegliato! Condizione soddisfatta. Consumo...\n");
    dati_pronti = 0; // Consumiamo il dato, "falsificando" di nuovo la condizione
    
    pthread_mutex_unlock(&mtx);
    return NULL;
}

void* produttore(void* arg) {
    sleep(2); // Simula un lavoro lungo 2 secondi
    
    pthread_mutex_lock(&mtx);
    
    printf("[Produttore] Ho prodotto un dato! Modifico la variabile condivisa...\n");
    dati_pronti = 1; // MODIFICHIAMO LA CONDIZIONE
    
    printf("[Produttore] Suono il campanello per svegliare un thread in attesa!\n");
    // Come se mandasse una notifica su quella "newsletter" per risvegliare chi era in attesa
    // in caso di signal solamente 1 thread viene risvegliato
    // in caso di broadcast vengono risvegliati TUTTI i thread in attesa
    pthread_cond_signal(&cond); // Sveglia il consumatore
    
    pthread_mutex_unlock(&mtx); // Rilascia il mutex permettendo al consumatore di prenderlo
    return NULL;
}
```

#### Attesa con Scadenza Temporale: `pthread_cond_timedwait()` e Deadline
In molti sistemi reali e tracce d'esame (*Slide 961, `cond_var_deadline.c`*), un thread non può rimanere bloccato indefinitamente in attesa di un evento: occorre impostare un tempo massimo di attesa (**deadline** o *timeout*).

La funzione POSIX predisposta è:
```c
#include <pthread.h>
#include <time.h>

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
                           const struct timespec *abstime);
```

- **Parametro `abstime` (Tempo Assoluto):** A differenza di funzioni che accettano un intervallo relativo (es. `sleep(2)` o `usleep()`), `pthread_cond_timedwait()` richiede un **tempo assoluto** nel futuro (secondi e nanosecondi dall'Epoch, 1 gennaio 1970).
- **Calcolo della Deadline:**
  ```c
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);
  ts.tv_sec += 2; // Scadenza tra esattamente 2 secondi da adesso
  ```
- **Valore di ritorno:**
  - Ritorna `0` se la condizione è stata segnalata prima della scadenza (`pthread_cond_signal` o `broadcast`).
  - Ritorna `ETIMEDOUT` se il tempo massimo è trascorso senza che l'evento si sia verificato.
  - Al risveglio (sia per segnale che per timeout), **il mutex viene sempre riacquisito automaticamente** prima che la funzione ritorni.

**Esempio Completo (`cond_var_deadline.c`, Slide 961):**
```c
// cond_var_deadline.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
int ready = 0;   // Condizione da attendere

void* worker(void* arg) {
    // Simula una computazione: provando sleep(1) l'evento arriva in tempo,
    // con sleep(3) il main scatterà per timeout!
    sleep(3);
    pthread_mutex_lock(&mtx);
    ready = 1;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main(void) {
    pthread_t th;
    pthread_create(&th, NULL, worker, NULL);

    pthread_mutex_lock(&mtx);

    // Calcolo la deadline assoluta: ora corrente + 2 secondi
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += 2;

    int rc = 0;
    while (!ready && rc == 0) {
        rc = pthread_cond_timedwait(&cond, &mtx, &ts);
    }

    if (ready) {
        printf("[main] Evento segnalato dal worker PRIMA della deadline!
");
    } else if (rc == ETIMEDOUT) {
        printf("[main] Timeout scaduto (ETIMEDOUT): il worker ha impiegato troppo tempo!
");
    } else {
        printf("[main] Errore su timedwait: %d
", rc);
    }

    pthread_mutex_unlock(&mtx);
    pthread_join(th, NULL);
    return 0;
}
```


### 15.4 Semafori POSIX

Un **semaforo** è una variabile intera gestita dal kernel che rappresenta un numero di "gettoni" (o permessi) disponibili. Viene modificato unicamente tramite due operazioni **atomiche** sicure: `wait()` (tradizionalmente chiamata **P**) e `post()` (tradizionalmente chiamata **V** o `signal`).

**Come funziona concettualmente?**
Immagina un semaforo come un contenitore di gettoni:
- **`sem_wait()` (P):** Il thread chiede un gettone. Se ce n'è almeno uno (> 0), lo prende (decrementa il contatore) e prosegue senza interruzioni. Se il contenitore è vuoto (0), il thread **si blocca e si addormenta** finché qualcuno non inserisce un gettone.
- **`sem_post()` (V):** Il thread inserisce un gettone nel contenitore (incrementa il contatore). Se c'erano thread addormentati in attesa di un gettone, il sistema operativo ne **sveglia uno**, che prenderà il gettone appena inserito e riprenderà l'esecuzione.

```c
#include <semaphore.h>

sem_t sem;

// 1. Inizializzazione: (semaforo, pshared, valore_iniziale)
// pshared = 0 indica che il semaforo è condiviso tra i thread dello STESSO processo
// pshared = 1 indica che è condiviso tra processi DIVERSI (memoria condivisa)
sem_init(&sem, 0, valore_iniziale);

// 2. Operazioni
sem_wait(&sem);    // Decrementa (se = 0 si blocca in attesa)
sem_post(&sem);    // Incrementa (e sveglia un thread in attesa, se c'è)

// 3. Distruzione
sem_destroy(&sem); // Libera le risorse
```

**Tipi di Semaforo:**
1. **Semaforo Contatore (Counting Semaphore):** Può assumere qualsiasi valore intero positivo (es. `valore_iniziale = 5`). È perfetto per controllare l'accesso a **N risorse identiche** (es. gestire un parcheggio con 5 posti auto, o un server che ammette massimo 5 connessioni simultanee).
2. **Semaforo Binario:** Può valere solo `0` o `1`. Viene spesso usato per garantire la mutua esclusione come un Mutex, o per il coordinamento stretto (1-a-1).

**Qual è la differenza logica tra un Mutex e un Semaforo Binario?**
Anche se sembrano fare la stessa cosa (evitare l'accesso simultaneo), c'è una differenza architetturale fondamentale: **L'Ownership (proprietà)**.
- Un **Mutex** ha il concetto di "proprietario": *solo* il thread che ha fatto `lock()` è autorizzato a fare l' `unlock()`.
- Un **Semaforo** NON ha proprietari: il thread A può fare `sem_wait()`, e un thread B completamente diverso può fare `sem_post()`. Questa caratteristica lo rende lo strumento perfetto per la **sincronizzazione dell'ordine di eventi** (dove un thread deve sbloccarne un altro).

**Esempio 1 — Sincronizzazione dell'ordine di esecuzione (Scheduling):**
Vogliamo essere sicuri che l'istruzione `S2` del Thread 2 avvenga *sempre e solo dopo* l'istruzione `S1` del Thread 1.
```c
sem_t synch;
sem_init(&synch, 0, 0); // Inizializzato a 0 gettoni (chi fa wait si bloccherà subito)

// --- THREAD 1 ---            // --- THREAD 2 ---
S1;                            sem_wait(&synch); // T2 arriva qui e si blocca (ci sono 0 gettoni)
sem_post(&synch); // Sblocca   S2;               // T2 si sveglia ed esegue S2 SOLO DOPO S1
```

**Esempio 2 — Gestire risorse limitate (Counting):**
Immaginiamo di avere 3 stampanti condivise.
```c
sem_t stampanti;
sem_init(&stampanti, 0, 3); // Inizializziamo il semaforo con 3 gettoni (3 stampanti)

void* usa_stampante(void* arg) {
    sem_wait(&stampanti); // Prende 1 stampante. Il 4° thread che arriva qui in contemporanea si bloccherà.
    
    // ... Usa la stampante (stampa il documento) ...
    
    sem_post(&stampanti); // Ha finito di stampare, restituisce il gettone e sblocca l'eventuale 4° thread
    return NULL;
}
```

---

## 16. Problemi Classici di Sincronizzazione
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

Questi sono i classici problemi teorici e pratici che si affrontano studiando la programmazione concorrente.

### 16.1 Bounded-Buffer (Produttore-Consumatore)

**Il Problema:** 
Abbiamo $N$ **Produttori** che creano dati e li inseriscono in un buffer condiviso (una coda) di dimensione fissa (es. 10 posti), e $M$ **Consumatori** che prelevano questi dati per elaborarli.
*Regole d'oro:*
1. I produttori **non possono** inserire dati se il buffer è **pieno** (devono aspettare che si liberi spazio).
2. I consumatori **non possono** prelevare dati se il buffer è **vuoto** (devono aspettare che arrivino nuovi dati).
3. L'accesso al buffer (inserimento/rimozione) deve essere in **mutua esclusione** (un solo thread alla volta), per evitare di sovrascrivere dati o sballare gli indici.

#### Soluzione 1: Mutex + Condition Variable
In questa soluzione usiamo:
- **1 Mutex** per garantire la mutua esclusione (nessuno tocca il buffer contemporaneamente).
- **2 Condition Variables** per le code di attesa: una per i produttori (`not_full`) e una per i consumatori (`not_empty`).

```c
#define BUF_SIZE 10
typedef struct {
    int buf[BUF_SIZE];
    int in, out, count; // in=indice di scrittura, out=indice di lettura, count=numero elementi
    pthread_mutex_t mtx;
    pthread_cond_t not_full, not_empty;
} bbuff_t;

void put_item(bbuff_t *b, int item) {
    pthread_mutex_lock(&b->mtx);
    
    // Se il buffer è pieno, il produttore aspetta sulla condition 'not_full'
    while (b->count == BUF_SIZE) {
        pthread_cond_wait(&b->not_full, &b->mtx);
    }
        
    // --- Sezione Critica ---
    b->buf[b->in] = item;
    b->in = (b->in + 1) % BUF_SIZE; // Logica circolare
    b->count++;
    
    // Sveglia un eventuale consumatore in attesa che il buffer si riempisse
    pthread_cond_signal(&b->not_empty);
    
    pthread_mutex_unlock(&b->mtx);
}

int get_item(bbuff_t *b) {
    pthread_mutex_lock(&b->mtx);
    
    // Se il buffer è vuoto, il consumatore aspetta sulla condition 'not_empty'
    while (b->count == 0) {
        pthread_cond_wait(&b->not_empty, &b->mtx);
    }
        
    // --- Sezione Critica ---
    int item = b->buf[b->out];
    b->out = (b->out + 1) % BUF_SIZE;
    b->count--;
    
    // Sveglia un eventuale produttore in attesa che si liberasse spazio
    pthread_cond_signal(&b->not_full);
    
    pthread_mutex_unlock(&b->mtx);
    return item;
}
```

#### Soluzione 2: Semafori
In questa soluzione i semafori contatori tengono traccia automaticamente di quanti slot sono liberi e quanti sono pieni, eliminando la necessità delle variabili condition e del controllo tramite `while` esplicito.

```c
sem_t empty;   // slot liberi (Inizializzato a BUF_SIZE, es. 10)
sem_t full;    // elementi presenti (Inizializzato a 0, all'inizio è vuoto)
sem_t mutex;   // protezione buffer (Inizializzato a 1, agisce da Mutex)

void put_item(int item) {
    // 1. Chiedo uno slot libero. Se empty è 0 (pieno), mi blocco qui.
    sem_wait(&empty);       
    
    // 2. Chiedo l'accesso esclusivo al buffer.
    sem_wait(&mutex);       
    buffer[in_idx] = item;
    in_idx = (in_idx + 1) % BUF_SIZE;
    sem_post(&mutex);       // Rilascio l'accesso esclusivo
    
    // 3. Avviso che c'è un elemento IN PIÙ (incremento full) e sblocco eventuali consumatori.
    sem_post(&full);        
}

int get_item(void) {
    // 1. Chiedo un elemento. Se full è 0 (vuoto), mi blocco qui.
    sem_wait(&full);        
    
    // 2. Chiedo l'accesso esclusivo al buffer.
    sem_wait(&mutex);       
    int item = buffer[out_idx];
    out_idx = (out_idx + 1) % BUF_SIZE;
    sem_post(&mutex);       // Rilascio l'accesso esclusivo
    
    // 3. Avviso che c'è uno slot libero IN PIÙ (incremento empty) e sblocco eventuali produttori.
    sem_post(&empty);       
    return item;
}
```

### 16.2 Readers-Writers (Lettori-Scrittori)

**Il Problema:** 
Abbiamo una base di dati condivisa (es. un file o un array). Ci sono thread che vogliono solo *leggere* (Lettori) e thread che vogliono *modificare* (Scrittori).
*Regole d'oro:*
1. Più Lettori possono leggere **contemporaneamente** senza darsi fastidio a vicenda.
2. Quando uno Scrittore accede, deve avere **accesso esclusivo assoluto** (nessun altro scrittore e *nessun* lettore può accedere finché non ha finito).

*Soluzione (con priorità ai lettori):* Usiamo un contatore di lettori. Il primo lettore che arriva "chiude la porta" in faccia agli scrittori usando un lucchetto. Finché ci sono lettori che continuano ad arrivare e leggere, la porta rimane chiusa agli scrittori. Solo l'ultimo lettore che se ne va, riapre la porta rimuovendo il lucchetto.

```c
pthread_mutex_t mutex;      // Protegge la modifica della variabile 'read_count'
pthread_mutex_t rw_mutex;   // Agisce come un "lucchetto gigante" per la risorsa/dati
int read_count = 0;         // Quanti lettori ci sono attualmente dentro

// --- SCRITTORE ---
pthread_mutex_lock(&rw_mutex); // Chiede l'accesso esclusivo. Se c'è anche un solo lettore dentro, si blocca.
// ... Scrive/Modifica i dati in totale solitudine ...
pthread_mutex_unlock(&rw_mutex);


// --- LETTORE ---
// FASE DI INGRESSO
pthread_mutex_lock(&mutex); // Proteggiamo il contatore
read_count++;
if (read_count == 1) { 
    // Sono il PRIMO lettore ad entrare! 
    // Metto il lucchetto gigante agli scrittori, così nessuno può modificare mentre leggiamo.
    pthread_mutex_lock(&rw_mutex);  
}
pthread_mutex_unlock(&mutex);

// ... Legge i dati (possono esserci N lettori qui dentro contemporaneamente!) ...

// FASE DI USCITA
pthread_mutex_lock(&mutex); // Proteggiamo di nuovo il contatore per uscire
read_count--;
if (read_count == 0) {
    // Sono l'ULTIMO lettore ad uscire! Non c'è più nessuno che legge.
    // Tolgo il lucchetto gigante, ora gli scrittori possono rientrare.
    pthread_mutex_unlock(&rw_mutex);  
}
pthread_mutex_unlock(&mutex);
```


### 16.3 Problema del Ponte a Senso Unico Alternato (Bridge Problem — `sem_ponte.c`)
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

Nelle slide (*Slide 935–936, 968*), viene presentato un classico problema di concorrenza spesso assegnato negli esami di laboratorio:

**Specifica del Problema:**
- Un ponte a campata unica collega due sponde e può sostenere al massimo $N$ automobili contemporaneamente (`MAX_SUL_PONTE = 3`).
- La carreggiata è a corsia singola.
- Ogni automobile è modellata come un **thread** concorrente che:
  1. Arriva al ponte e attende il permesso di salire.
  2. Entra nel ponte e lo attraversa (simulato tramite `sleep()`).
  3. Esce dal ponte sul lato opposto, lasciando libero il posto per un'altra automobile.

**Implementazione con Semafori POSIX Contatori (`sem_ponte.c`):**
Un semaforo contatore inizializzato a $N$ (`MAX_SUL_PONTE`) regola la capacità massima:
- Ogni auto che entra esegue `sem_wait(&bridge)`, decrementando il contatore. Se il contatore è $0$, il thread si blocca.
- Ogni auto che esce esegue `sem_post(&bridge)`, incrementando il contatore e risvegliando una delle auto in coda.

```c
// sem_ponte.c - Esercizio ufficiale slide 968
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define N_AUTO 10
#define MAX_SUL_PONTE 3

sem_t bridge;  // Semaforo contatore di capacità del ponte

void* car(void* arg) {
    long id = (long)arg;
    printf("[Auto %2ld] Arriva in prossimita' del ponte
", id);

    // 1. Attesa permesso di salire sul ponte (capacità residua > 0)
    sem_wait(&bridge);

    printf(">>> [Auto %2ld] ENTRA nel ponte (auto sul ponte: max %d)
", id, MAX_SUL_PONTE);
    sleep(1 + rand() % 2);  // Tempo di attraversamento simulato

    printf("<<< [Auto %2ld] ESCE dal ponte sul lato opposto
", id);

    // 2. Segnala l'uscita liberando uno slot per le auto in attesa
    sem_post(&bridge);

    return NULL;
}

int main(void) {
    pthread_t th[N_AUTO];
    srand(time(NULL));

    // Inizializzazione semaforo: pshared=0 (condiviso tra thread), valore iniziale=MAX_SUL_PONTE
    if (sem_init(&bridge, 0, MAX_SUL_PONTE) != 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // Creazione dei thread automobile
    for (long i = 0; i < N_AUTO; i++) {
        pthread_create(&th[i], NULL, car, (void*)(i + 1));
    }

    // Attesa completamento attraversamento di tutte le auto
    for (int i = 0; i < N_AUTO; i++) {
        pthread_join(th[i], NULL);
    }

    // Distruzione del semaforo al termine
    sem_destroy(&bridge);
    printf("[main] Tutte le %d auto hanno attraversato il ponte con successo.
", N_AUTO);
    return 0;
}
```


---

## 17. Socket — Comunicazione di Rete
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 17.1 Concetti Fondamentali

Le **socket** sono lo strumento standard per far comunicare due processi, sia che si trovino sullo stesso computer, sia che si trovino in due continenti diversi tramite Internet.

**Domini di comunicazione (Dove comunichiamo?):**
- `AF_LOCAL` (o `AF_UNIX`): Comunicazione tra processi sullo STESSO COMPUTER, NON su COMPUTER DIVERSI in LOCALE. Utilizza un percorso del file system come indirizzo (es. `/tmp/my-socket`).
- `AF_INET`: Comunicazione tra processi su computer diversi tramite RETE LOCALE o INTERNET, utilizzando indirizzi IPv4 (es. `192.168.1.5`).

**Tipi di socket (Come comunichiamo?):**
- `SOCK_STREAM` (Protocollo **TCP**): Si stabilisce prima una connessione, e finché non viene chiusa, c'è un flusso di dati bidirezionale **affidabile**.
- `SOCK_DGRAM` (Protocollo **UDP**): Non c'è una connessione fissa, ogni messaggio viene inviato come un pacchetto isolato, con mittente e destinatario specificati ad ogni invio. È veloce, ma **non affidabile**: i pacchetti possono arrivare in disordine o andare persi durante la trasmissione.

### 17.2 Le fasi di una connessione TCP (SOCK_STREAM)

La connessione TCP segue un'architettura client-server ben definita, in cui il server si mette in ascolto di richieste di connessione e il client le avvia.

**Il lato SERVER:**
1. **`socket()`**: Crea l'endpoint di comunicazione, allocando le risorse necessarie nel kernel.
   `int socket(int domain, int type, int protocol);`
   - `domain`: La famiglia di indirizzi (es. `AF_INET` per IPv4, `AF_LOCAL` per percorsi fisici).
   - `type`: Il tipo (es. `SOCK_STREAM` per TCP, `SOCK_DGRAM` per UDP).
   - `protocol`: Spesso `0` per far scegliere il protocollo di default in base al tipo.
   - **Ritorna**: Il File Descriptor (FD) della socket o `-1` (errore).
2. **`bind()`**: Associa la socket a un indirizzo locale specifico (IP e Porta). Definisce dove il server sarà raggiungibile.
   `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
   - `sockfd`: Il FD della socket appena creata.
   - `addr`: Puntatore alla struttura con l'indirizzo (es. `sockaddr_in`), castata genericamente.
   - `addrlen`: La dimensione in byte della struttura (`sizeof(addr)`).
3. **`listen()`**: Configura la socket in modalità "passiva" di ascolto, pronta ad accettare richieste.
   `int listen(int sockfd, int backlog);`
   - `sockfd`: La socket appena "bindata".
   - `backlog`: Dimensione massima della coda delle connessioni in attesa di essere smaltite da `accept()`.
4. **`accept()`**: Estrae la prima connessione dalla coda e la accetta. 
   `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);`
   - `sockfd`: La socket in ascolto.
   - `addr`: Struttura vuota che il kernel *riempirà* con i dati del client connesso (spesso `NULL` se non interessano).
   - `addrlen`: Puntatore alla dimensione della struttura (spesso `NULL`).
   - **Attenzione:** `accept` ritorna un **nuovo file descriptor** per una *nuova socket dedicata* unicamente a quel client. La socket originale continua solo ad ascoltare.

**Il lato CLIENT:**
1. **`socket()`**: Crea l'endpoint di comunicazione (stessi parametri visti sopra).
2. **`connect()`**: Avvia il 3-way handshake verso l'indirizzo del Server.
   `int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
   - `sockfd`: La socket del client.
   - `addr`: La struttura con l'indirizzo IP e la porta del **server** a cui si vuole puntare.
   - `addrlen`: La dimensione della struttura (`sizeof(addr)`).

```text
    SERVER                               CLIENT
    -----------------                  ----------------
    │ socket()      │                  │               │
    │ bind()        │                  │ socket()      │
    │ listen()      │                  │               │
    │ accept()      │                  │               │
    │ [si blocca]   │ <-- RICHIESTA -->│ connect()     │
    │               │                  │               │
    │ [crea socket  │                  │               │
    │  dedicata]    │                  │               │
    │ read() /      │                  │ read() /      │
    │ write()       │ <-- COMUNICANO-->│ write()       │
    │ close()       │                  │ close()       │
    -----------------                  -----------------
```

### 17.3 Indirizzi e il problema del "Byte Order" (Endianness)

Quando comunichiamo in rete usando `AF_INET`, dobbiamo specificare Indirizzo IP e Porta per la `bind` e la `connect` usando questa struct:
```c
struct sockaddr_in {
    sa_family_t    sin_family;   // Sempre AF_INET
    in_port_t      sin_port;     // Porta (in Network byte order)
    struct in_addr sin_addr;     // Indirizzo IP (in Network byte order)
};
```

**Cos'è il Byte Order?**
I computer Intel/AMD (x86) leggono i numeri in formato **Little Endian** (il byte meno significativo precede gli altri). Tuttavia, i protocolli di rete di Internet sono stati standardizzati decenni fa per trasmettere i numeri in formato **Big Endian** (il byte più significativo precede gli altri).
Se inviassimo il numero di porta "5200" sulla rete senza convertirla, i dispositivi di rete (che si aspettano Big Endian) la leggerebbero in modo inverso, causando il fallimento del routing.

**La soluzione: Le funzioni di conversione universali:**
È **sempre obbligatorio** convertire porte e IP in "Network byte order" (ordine di rete standard) prima di assegnarli alla struct:
*   `htons(porta)`: **H**ost **TO** **N**etwork **S**hort (converte la porta a 16-bit)
*   `htonl(ip)`: **H**ost **TO** **N**etwork **L**ong (converte l'indirizzo IP a 32-bit).

*(Viceversa, per leggere un indirizzo ricevuto dalla rete nel formato dell'host locale, si usano `ntohs()` e `ntohl()`: Network TO Host).*

**Conversione facilitata degli IP (stringa → numero): `inet_pton` e `inet_ntop`**
Gli indirizzi IP sono comunemente espressi come stringhe testuali leggibili (es. `"127.0.0.1"` o `"192.168.1.1"`), ma la struct `sockaddr_in` richiede che il campo `sin_addr` contenga i 4 byte in formato binario di rete (Network Byte Order).
Si usa `inet_pton` (**P**resentation **TO** **N**etwork) per convertire e validare l'indirizzo:

```c
#include <arpa/inet.h>

struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port   = htons(5200);

// Converte da stringa (Presentation) a binario di rete (Network):
int res = inet_pton(AF_INET, "192.168.1.1", &addr.sin_addr); 
if (res <= 0) {
    if (res == 0) fprintf(stderr, "Formato IP non valido!\n");
    else perror("inet_pton");
}

// Operazione inversa (Network TO Presentation):
char ip_str[INET_ADDRSTRLEN];
inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str));
printf("Indirizzo IP: %s\n", ip_str);
```

#### Focus: Differenza Fondamentale tra `inet_pton()` e `ntohl()` / `htonl()`

Spesso si fa confusione tra queste funzioni perché entrambe hanno a che fare con il "Network Byte Order". In realtà operano a livelli concettuali e su tipi di dato completamente distinti:

```text
┌────────────────────────────────────────────────────────────────────────────────────────┐
│ 1. Conversione INDIRIZZI IP (Testo <──► Binario di Rete)                               │
│                                                                                        │
│   Stringa ASCII: "192.168.1.1" ───[ inet_pton() ]───► struct in_addr (4 Byte Big Endian)│
│   Stringa ASCII: "192.168.1.1" ◄───[ inet_ntop() ]─── struct in_addr (4 Byte Big Endian)│
└────────────────────────────────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────────────────────────────────┐
│ 2. Conversione DATI / INTERI (CPU Little Endian <──► Rete Big Endian)                  │
│                                                                                        │
│   Intero CPU (es. 1500)       ───[   htonl()   ]───► 4 Byte invertiti per la rete      │
│   Intero CPU (es. 1500)       ◄───[   ntohl()   ]─── 4 Byte ricevuti dal socket        │
└────────────────────────────────────────────────────────────────────────────────────────┘
```

| Caratteristica | `inet_pton()` | `htonl()` / `ntohl()` |
| :--- | :--- | :--- |
| **Etimologia** | **P**resentation **TO** **N**etwork | **H**ost **TO** **N**etwork **L**ong / **N**etwork **TO** **H**ost **L**ong |
| **Input** | Stringa testuale ASCII (es. `"127.0.0.1"`) | Intero numerico a 32 bit (`uint32_t`, `int`) |
| **Operazione** | **Parsing e validazione**: analizza caratteri e punti, verifica che i 4 ottetti siano 0-255 e li memorizza in memoria già in Big Endian. | **Byte-swapping**: non legge stringhe né valida nulla. Inverte semplicemente l'ordine dei 4 byte in memoria della CPU. |
| **Quando si usa?** | **Fase di Setup**: per inizializzare `addr.sin_addr` prima di `connect()` o `bind()`. | **Fase di I/O (Dati)**: per inviare e ricevere numeri binari nel payload (es. `msg.type`, punteggi, lunghezze). |
| **Supporto IPv6?** | Sì (`AF_INET6` su buffer a 128 bit `in6_addr`). | No (opera unicamente su numeri interi a 32 bit). |

> [!CAUTION]
> **Il tipico dubbio/errore d'esame:** *"Un indirizzo IPv4 è un numero a 32 bit, quindi posso usare `htonl()` / `ntohl()` per impostare un IP?"*
> 
> * **SÌ, ma SOLO se l'IP è già una costante numerica intera in C**, come `INADDR_ANY` (`0x00000000`) o `INADDR_LOOPBACK` (`0x7F000001`):
>   ```c
>   addr.sin_addr.s_addr = htonl(INADDR_ANY);      // CORRETTO: INADDR_ANY è un intero
>   ```
> * **NO se hai una stringa testuale come `"192.168.1.1"`**:
>   ```c
>   // ERRORE GRAVE: un letterale stringa è un puntatore 'char *'!
>   // addr.sin_addr.s_addr = htonl("192.168.1.1"); // ERRORE! Invertirebbe i byte dell'indirizzo del puntatore in memoria!
>   
>   // CORRETTO:
>   inet_pton(AF_INET, "192.168.1.1", &addr.sin_addr);
>   ```

**Il Casting a `(struct sockaddr *)` (Polimorfismo)**
Nei codici successivi noterai che chiamate come `bind` o `connect` prendono l'indirizzo tramite un puntatore castato a `(struct sockaddr *)`.
Questo accade perché le librerie di rete in C sono nate *prima* dell'invenzione del puntatore generico `void *`. Per fare in modo che `bind()` potesse accettare indirizzi IPv4 (`sockaddr_in`), IPv6 (`sockaddr_in6`) o Locali (`sockaddr_un`), i progettisti idearono la struttura base `struct sockaddr` usandola come tipo jolly. È un rudimentale esempio di polimorfismo nel C.

### 17.4 Socket Locali (AF_LOCAL / Unix Domain Sockets)

Se i processi che devono comunicare si trovano sulla stessa macchina locale, lo stack TCP/IP introduce un overhead non necessario. In questo caso è preferibile usare `AF_LOCAL`.
In questo dominio, l'indirizzo di comunicazione non è definito da un IP e una porta, ma dal **percorso di un file speciale** sul file system (es. `/tmp/mysock`), che funge da punto d'incontro per i processi.

**La struttura `sockaddr_un`**
Per i socket locali, l'indirizzo viene definito tramite la struttura `sockaddr_un` (in `<sys/un.h>`), dove il suffisso `_un` sta storicamente per UNIX Domain.
```c
struct sockaddr_un {
    sa_family_t sun_family;       /* AF_UNIX o AF_LOCAL */
    char        sun_path[108];    /* Percorso del file socket */
};
```
A differenza dei socket di rete, il campo vitale qui è `sun_path`. Quando il server invoca la `bind()`, il sistema operativo crea fisicamente un file in quel percorso. I client dovranno popolare una `sockaddr_un` identica per connettersi.

**Server (AF_LOCAL):**
```c
#include <sys/socket.h>
#include <sys/un.h>

int listen_sd = socket(AF_LOCAL, SOCK_STREAM, 0);

// Prepara l'indirizzo associato al percorso del file sul disco
struct sockaddr_un my_addr = {0}; // Inizializza la struttura a 0
my_addr.sun_family = AF_LOCAL;
strncpy(my_addr.sun_path, "/tmp/mysock", sizeof(my_addr.sun_path) - 1);

unlink("/tmp/mysock");  // Rimuove eventuali socket orfane da esecuzioni precedenti
bind(listen_sd, (struct sockaddr*)&my_addr, sizeof(my_addr)); // "Crea" fisicamente il file 
listen(listen_sd, 5);

int connect_sd = accept(listen_sd, NULL, NULL); // Attende la prima richiesta
// ... comunica ...
close(connect_sd);
close(listen_sd);
unlink("/tmp/mysock"); // Pulisce il file alla chiusura
```

**Client (AF_LOCAL):**
```c
int sd = socket(AF_LOCAL, SOCK_STREAM, 0);
struct sockaddr_un srv_addr = {0};
srv_addr.sun_family = AF_LOCAL;
strncpy(srv_addr.sun_path, "/tmp/mysock", sizeof(srv_addr.sun_path) - 1);

connect(sd, (struct sockaddr*)&srv_addr, sizeof(srv_addr)); // Richiede la connessione
// ... comunica ...
close(sd);
```

### 17.5 Server TCP Completo

```c
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main(void) {
    int s = socket(AF_INET, SOCK_STREAM, 0);

    // Opzione SO_REUSEADDR per evitare "address already in use"
    int opt = 1;
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(5200);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  // qualsiasi interfaccia

    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    listen(s, 5);

    int c = accept(s, NULL, NULL);
    // ... comunica con c usando read/write o send/recv ...
    close(c);
    close(s);
}
```

### 17.6 Client TCP Completo

```c
int s = socket(AF_INET, SOCK_STREAM, 0);

struct sockaddr_in addr = {0};
addr.sin_family = AF_INET;
addr.sin_port   = htons(5200);
inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

connect(s, (struct sockaddr *)&addr, sizeof(addr));
// ... comunica ...
close(s);
```

### 17.7 `send()` e `recv()`

```c
ssize_t send(int sock, const void *buf, size_t len, int flags);
ssize_t recv(int sock, void *buf, size_t len, int flags);
```

| Flag | Significato |
|------|-------------|
| `MSG_DONTWAIT` | Non blocca |
| `MSG_PEEK` | Legge senza consumare |
| `MSG_WAITALL` | Aspetta che tutto `len` sia raggiunto |
| `MSG_NOSIGNAL` | Evita SIGPIPE |

Senza flag, `send`/`recv` si comportano come `write`/`read`.

### 17.8 Scambio Dati Binari — Network Byte Order (`htonl` / `ntohl`)

Quando scambiamo strutture o campi numerici (`short`, `int`, `long`, `uint16_t`, `uint32_t`) tramite socket, macchine con architetture diverse (o processori x86 rispetto allo standard Big Endian di Internet) interpreterebbero i byte in ordine inverso.

* **Tipi a singolo byte (`char`, `uint8_t`, stringhe di caratteri):** **NON** richiedono alcuna conversione. L'ordine dei byte si applica solo quando un dato è composto da 2 o più byte.
* **Tipi multibyte (`short`, `int`, `uint32_t`, ecc.):** devono essere **sempre** convertiti prima della spedizione con `htons()` / `htonl()` e riconvertiti alla ricezione con `ntohs()` / `ntohl()`.

#### Esempio: Invio e Ricezione di un Intero a 32 bit

```c
// =================== LATO TRASMITTENTE (Mittente) ===================
int s = socket(AF_INET, SOCK_STREAM, 0);
// ... connect(s, ...) ...

uint32_t punteggio_locale = 1500;
// 1. Converte dal formato nativo CPU (Little Endian) al formato di rete (Big Endian):
uint32_t punteggio_rete = htonl(punteggio_locale);

// 2. Invia i 4 byte sulla socket:
write(s, &punteggio_rete, sizeof(punteggio_rete));


// =================== LATO RICEVENTE (Destinatario) ==================
// ... accept(...) ...
uint32_t dato_grezzo_rete;

// 1. Riceve i 4 byte grezzi:
read(c, &dato_grezzo_rete, sizeof(dato_grezzo_rete));

// 2. Se usassimo 'dato_grezzo_rete' direttamente su x86, leggeremmo un valore sballato!
// Converte da Network Byte Order (Big Endian) a Host Byte Order (Little Endian):
uint32_t punteggio_effettivo = ntohl(dato_grezzo_rete);

printf("Punteggio ricevuto: %u\n", punteggio_effettivo); // Stampa correttamente: 1500
```

#### Esempio con Struttura / Header di Protocollo
Se trasmetti un pacchetto contenente sia testo che interi:
```c
struct Pacchetto {
    uint32_t tipo_msg;     // Multibyte -> richiede htonl / ntohl
    uint32_t lunghezza;    // Multibyte -> richiede htonl / ntohl
    char     payload[64];  // Singolo byte -> NON richiede conversione!
};
```
* **Prima di `write`:** `pkt.tipo_msg = htonl(TIPO_LOGIN); pkt.lunghezza = htonl(len);`
* **Dopo `read`:** `tipo = ntohl(pkt.tipo_msg); len = ntohl(pkt.lunghezza);`

### 17.9 Lettura e Scrittura Safe

```c
ssize_t recv_all(int fd, void *buf, size_t n) {
    size_t received = 0;
    char *p = buf;
    while (received < n) {
        ssize_t r = recv(fd, p + received, n - received, 0);
        if (r < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (r == 0) return received;  // connessione chiusa
        received += (size_t)r;
    }
    return (ssize_t)received;
}
```

### 17.10 Socket UDP (`sendto` e `recvfrom`)

A differenza di TCP (`SOCK_STREAM`), **UDP (`SOCK_DGRAM`) è connectionless (senza connessione)**: non c'è handshake a 3 vie (`connect` e `accept` non sono necessarie per stabilire una sessione) e ogni messaggio viaggia come un datagramma indipendente a pacchetto intero.

Dato che non esiste un canale virtuale permanente associato al socket:
* Chi **invia** deve specificare per ogni pacchetto l'indirizzo del destinatario (`dest_addr`).
* Chi **riceve** può estrarre dal pacchetto l'indirizzo del mittente (`src_addr`) per sapere chi lo ha inviato ed eventualmente rispondergli.

#### Firme di Sistema POSIX

```c
#include <sys/socket.h>

// Invio di un datagramma:
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);

// Ricezione di un datagramma:
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen);
```

#### Analisi Dettagliata dei Parametri

| Parametro | In `sendto()` | In `recvfrom()` |
| :--- | :--- | :--- |
| `sockfd` | File descriptor del socket creato con `socket(AF_INET, SOCK_DGRAM, 0)`. | File descriptor del socket (associato alla porta con `bind()` nel server). |
| `buf` | Puntatore al buffer contenente i dati da trasmettere. | Puntatore al buffer in cui memorizzare il messaggio in arrivo. |
| `len` | Quanti byte inviare da `buf`. | Dimensione massima (capacità) del buffer `buf` (evita buffer overflow). |
| `flags` | Opzioni speciali (normalmente **`0`**; es. `MSG_DONTWAIT`). | Opzioni speciali (normalmente **`0`**; es. `MSG_PEEK`). |
| `dest_addr` / `src_addr` | **Input**: puntatore all'indirizzo del **destinatario** (`const struct sockaddr *`). | **Output**: puntatore a struct vuota che il kernel **riempirà con l'IP e la porta del mittente**.<br>*(Se non interessa sapere chi invia, si può passare `NULL`)*. |
| `addrlen` | **Per valore** (`socklen_t`): dimensione in byte dell'indirizzo del destinatario (`sizeof(dest_addr)`). | **Per puntatore** (`socklen_t *`, **Value-Result**):<br>• *In ingresso:* deve contenere `sizeof(src_addr)`.<br>• *In uscita:* il kernel scrive i byte effettivi dell'indirizzo memorizzato.<br>*(Se `src_addr` è `NULL`, va passato `NULL`)*. |

> [!IMPORTANT]
> **Perché `addrlen` è un valore in `sendto` ma un puntatore in `recvfrom`?**
> Si tratta del classico argomento **Value-Result**:
> * In `sendto()`, siamo noi a comunicare al kernel la dimensione della struct che gli stiamo passando (`sizeof(struct sockaddr_in)`).
> * In `recvfrom()`, il kernel deve scrivere dentro la nostra variabile `src_addr`. Tramite `*addrlen` il kernel legge quanto spazio ha a disposizione prima di scrivere e, al termine, aggiorna la variabile con la reale dimensione dei dati scritti.

#### Diagramma di Flusso: Il Ciclo Richiesta-Risposta (Echo)

```text
┌─────────────────────────────────┐                             ┌─────────────────────────────────┐
│           CLIENT UDP            │                             │           SERVER UDP            │
│  (Porta effimera es. 49152)     │                             │       (Porta nota es. 5200)     │
└────────────────┬────────────────┘                             └────────────────┬────────────────┘
                 │                                                               │
                 │ 1. sendto(..., &srv_addr, sizeof(srv_addr))                   │
                 │ ─────────────────────────────────────────────────────────────►│ 2. recvfrom(..., &from, &flen)
                 │    [Payload: "ciao"]                                          │    • Il kernel memorizza in 'from'
                 │                                                               │      l'IP e la porta del Client!
                 │                                                               │
                 │ 4. recvfrom(..., NULL, NULL)                                  │ 3. sendto(..., &from, flen)
                 │ ◄─────────────────────────────────────────────────────────────│    • Per rispondere (Echo), 'from'
                 │    [Risposta: "ciao"]                                         │      diventa il DESTINATARIO!
```

#### Esempio Pratico Completo: Server Echo e Client UDP

```c
// ============================================================================
// SERVER UDP (Echo Server)
// ============================================================================
int s = socket(AF_INET, SOCK_DGRAM, 0);

struct sockaddr_in addr = {0};
addr.sin_family      = AF_INET;
addr.sin_port        = htons(5200);
addr.sin_addr.s_addr = htonl(INADDR_ANY); // Ascolta su qualsiasi interfaccia locale
bind(s, (struct sockaddr *)&addr, sizeof(addr));

char buf[1024];
struct sockaddr_in from;              // Buffer per memorizzare l'indirizzo di chi ci contatta
socklen_t flen = sizeof(from);        // Inizializzato con la capacità massima della struct

// 1. Attende un messaggio da QUALSIASI mittente:
// Al ritorno, 'from' conterrà l'IP e la porta del client mittente.
int n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&from, &flen);

// 2. Risponde con lo stesso messaggio (Echo) al client:
// NOTA BENE: per la risposta, la struct 'from' (che conteneva il mittente)
// diventa il parametro 'dest_addr' per sendto()!
sendto(s, buf, n, 0, (struct sockaddr *)&from, flen);


// ============================================================================
// CLIENT UDP
// ============================================================================
int s = socket(AF_INET, SOCK_DGRAM, 0);

// Configura l'indirizzo del server (destinatario del client)
struct sockaddr_in srv_addr = {0};
srv_addr.sin_family = AF_INET;
srv_addr.sin_port   = htons(5200);
inet_pton(AF_INET, "127.0.0.1", &srv_addr.sin_addr);

// 1. Spedisce il messaggio al server passando 'srv_addr' come 'dest_addr':
sendto(s, "ciao", 4, 0, (struct sockaddr *)&srv_addr, sizeof(srv_addr));

// 2. Attende la risposta dal server:
// Passiamo NULL, NULL come src_addr e addrlen perché il client sta già comunicando
// col server noto e non ha bisogno di memorizzare l'indirizzo di provenienza.
int n = recvfrom(s, buf, sizeof(buf), 0, NULL, NULL);
```

#### Caratteristiche Cruciali di UDP per l'Esame:
* **Preservazione dei confini (Record-Oriented):** Ogni chiamata a `sendto()` spedisce un datagramma atomico. Ogni chiamata a `recvfrom()` estrae **esattamente un datagramma intero**. Non esiste lo stream continuo di byte tipico di TCP, quindi in UDP **non** serve fare letture a ciclo continuo tipo `recvall()`/`readn()`.
* **Troncamento Silenzioso:** Se il buffer specificato a `recvfrom()` ha una dimensione `len` inferiore alla dimensione del datagramma arrivato, il kernel legge i primi `len` byte e **scarta definitivamente il resto del pacchetto** (senza che venga riletto alla chiamata successiva).
* **Nessun ACK o Connessione:** `sendto()` ritorna con successo se il pacchetto è stato consegnato al driver di rete locale. Non garantisce in alcun modo che il destinatario sia acceso, in ascolto o abbia ricevuto il pacchetto.

### 17.11 Server Concorrente

**Con fork:**
```c
for (;;) {
    int c = accept(s, NULL, NULL);
    pid_t pid = fork();
    if (pid == 0) {       // FIGLIO
        close(s);         // non serve la listening socket
        handle_client(c); // gestisci il client
        close(c);
        _exit(0);
    }
    close(c);             // il padre torna ad accettare
}
```

**Con thread:**
```c
for (;;) {
    int connect_sd = accept(listen_sd, NULL, NULL);
    int *thread_sd = malloc(sizeof(int));
    *thread_sd = connect_sd;
    pthread_t tid;
    pthread_create(&tid, NULL, gestisci, thread_sd);
    pthread_detach(tid);  // nessun join necessario
}
```

### 17.12 Opzioni Socket (`setsockopt`)

```c
int opt = 1;
setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

struct timeval tv = {5, 0};  // 5 secondi
setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
```

**`SO_REUSEADDR`** — evita "address already in use" dopo riavvio server:
```c
// TCP rimane in TIME_WAIT (~1-4 min) dopo close();
// SO_REUSEADDR permette di bindare quella porta lo stesso
int opt = 1;
setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));  // prima di bind()
```

**`SO_REUSEPORT`** — load balancing nativo (ogni thread il suo listening socket):
```c
int opt = 1;
setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
bind(fd, ...); listen(fd, ...);
// Kernel distribuisce le connessioni con round-robin/hash tra i socket
```

**`SO_SNDTIMEO` / `SO_RCVTIMEO`** — timeout su operazioni bloccanti:
```c
struct timeval tv = { .tv_sec = 5, .tv_usec = 0 };  // 5 secondi
setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
// Se scade: recv()/send() ritornano -1, errno = EAGAIN o EWOULDBLOCK
```

**`SO_KEEPALIVE`** — keepalive TCP a livello kernel:
```c
int on = 1;
setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on));
// Default Linux: ~2 ore idle prima del primo probe TCP
// Per timeout precisi usare SO_RCVTIMEO, non SO_KEEPALIVE
```

**`SO_LINGER`** — controllo di `close()` con dati in sospeso:
```c
struct linger opt = { .l_onoff = 1, .l_linger = 5 };
setsockopt(fd, SOL_SOCKET, SO_LINGER, &opt, sizeof(opt));
// l_onoff=0           → close() torna subito, kernel invia i dati in bg (default)
// l_onoff=1, linger=X → close() si BLOCCA fino a X sec, poi RST se fallisce
// l_onoff=1, linger=0 → close() invia RST immediato, dati scartati
```

**`SO_SNDBUF` / `SO_RCVBUF`** — dimensione buffer kernel:
```c
int size = 65536;  // 64 KB
setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));  // send buffer
setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));  // recv buffer
// Buffer più grandi riducono short-write/short-read, consumano più RAM
```


### 17.13 Socket Non Bloccante

```c
#include <fcntl.h>
int flags = fcntl(fd, F_GETFL, 0);
fcntl(fd, F_SETFL, flags | O_NONBLOCK);
// Ora recv/send restituiscono -1 con errno=EAGAIN se non possono procedere
// Funziona su TCP, UDP, pipe
```

### 17.14 Pattern `recv_all` / `send_all` (lettura/scrittura safe)

Su TCP, `recv()` e `send()` possono restituire meno byte del richiesto (**short read/write**). Le funzioni safe gestiscono questo:

```c
// Legge esattamente n byte (gestisce EINTR e short-read)
ssize_t recv_all(int fd, void *buf, size_t n) {
    size_t received = 0;
    char *p = buf;
    while (received < n) {
        ssize_t r = recv(fd, p + received, n - received, 0);
        if (r > 0)  { received += (size_t)r; continue; }
        if (r == 0) return (ssize_t)received;      // EOF: peer ha chiuso
        if (errno == EINTR)   continue;             // interrotto da segnale
        if (errno == EAGAIN || errno == EWOULDBLOCK) return -2;  // timeout
        return -1;                                  // altro errore
    }
    return (ssize_t)received;   // == n
}

// Invia esattamente n byte
ssize_t send_all(int fd, const void *buf, size_t n) {
    size_t sent = 0;
    const char *p = buf;
    while (sent < n) {
        ssize_t w = send(fd, p + sent, n - sent, 0);
        if (w > 0)  { sent += (size_t)w; continue; }
        if (errno == EINTR)   continue;
        if (errno == EAGAIN || errno == EWOULDBLOCK) return -2;
        return -1;
    }
    return (ssize_t)sent;   // == n
}
```

### 17.15 Server Concorrente — Anti-Zombie con SIGCHLD

Il server con fork crea un figlio per ogni client. Bisogna evitare zombie:

```c
// Handler per raccogliere i figli terminati
static void reap(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0) {}  // raccoglie TUTTI i figli terminati
}

int main(void) {
    // Installa handler SIGCHLD con SA_RESTART
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = reap;
    sa.sa_flags   = SA_RESTART;  // riavvia accept/recv interrotti dal segnale
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, NULL);

    // ... bind, listen ...
    for (;;) {
        int c = accept(s, NULL, NULL);
        if (c < 0) { if (errno == EINTR) continue; break; }
        pid_t pid = fork();
        if (pid == 0) {
            close(s);          // figlio non usa la listening socket
            handle_client(c); // non ritorna
            _exit(0);          // usa _exit, non exit!
        }
        close(c);              // padre chiude il socket del client
    }
}
```

> **`SA_RESTART`**: fa sì che `accept()` (e altre syscall bloccanti) vengano riavviate automaticamente se interrotte da SIGCHLD, anziché ritornare `-1/EINTR`.

### 17.16 `connect()` con UDP

`connect()` può essere usata anche su socket UDP (SOCK_DGRAM):

```c
// Fissa la destinazione di default e abilita ricezione errori ICMP
connect(sd, (struct sockaddr*)&servaddr, sizeof(servaddr));

// Dopo connect() si può usare send()/write() invece di sendto():
send(sd, buf, len, 0);    // invia a servaddr (come sendto con NULL indirizzo)
recv(sd, buf, len, 0);    // riceve solo da servaddr (filtraggio implicito)

// Con sendto si può omettere l'indirizzo:
sendto(sd, buf, len, 0, NULL, 0);
```

Vantaggi: filtraggio automatico del mittente, ricezione di errori ICMP (es. host unreachable).

---

## 18. I/O Multiplexing — `select()`
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 18.1 Problema

Le funzioni `accept()`, `recv()` e `read()` sono **bloccanti**: un thread resta fermo su un singolo file descriptor. Se si devono monitorare più sorgenti (es. stdin + socket), occorre un meccanismo di **multiplexing**.

### 18.2 La Funzione `select()`

```c
#include <sys/select.h>
int select(int numfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
           struct timeval *timeout);
// Restituisce: numero di fd pronti, 0 se timeout, -1 se errore
```

| Parametro | Significato |
|-----------|-------------|
| `numfds` | Valore massimo fd + 1 (o `FD_SETSIZE`) |
| `readfds` | Insieme di fd da controllare in **lettura** |
| `writefds` | Insieme di fd da controllare in **scrittura** |
| `exceptfds` | Condizioni eccezionali (raramente usato, OOB data) |
| `timeout` | `NULL` → blocca indefinitamente; `{0,0}` → polling; `{N,M}` → attende N.M sec |

### 18.3 Macro per `fd_set`

```c
FD_ZERO(&set);          // svuota l'insieme
FD_SET(fd, &set);       // aggiunge fd all'insieme
FD_CLR(fd, &set);       // rimuove fd dall'insieme
FD_ISSET(fd, &set);     // controlla se fd è pronto (dopo select)
```

### 18.4 Quando un fd è "pronto"

**Pronto in lettura:**
- Ci sono dati nel buffer di ricezione
- Il peer ha chiuso il suo lato di scrittura (EOF)
- Si è verificato un errore sul socket
- Su listening socket: connessione pendente (accept pronto)

**Pronto in scrittura:**
- C'è spazio nel buffer di invio
- La connessione TCP è stabilita
- C'è errore pendente

### 18.5 Esempio — Select su stdin con timeout

```c
fd_set readfds;
struct timeval tv;
FD_ZERO(&readfds);
FD_SET(STDIN_FILENO, &readfds);
tv.tv_sec = 2;
tv.tv_usec = 500000;  // 2.5 secondi

int n = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
if (n == 0) {
    printf("Timeout scaduto.\n");
} else if (FD_ISSET(STDIN_FILENO, &readfds)) {
    char buf[128];
    ssize_t r = read(STDIN_FILENO, buf, sizeof(buf));
    write(STDOUT_FILENO, buf, r);
}
```

### 18.6 Server Multiplexing con `select()`

Un **unico thread** gestisce listening socket + tutti i socket dei client:

1. Riempie `fd_set` con la listening socket e tutti i client connessi
2. Chiama `select()` → attende finché qualcuno è pronto
3. Se la listening socket è pronta → `accept()` nuovo client
4. Se un client socket è pronto → `recv()` / `send()`
5. Se `recv()` restituisce 0 → client disconnesso → `close()` + `FD_CLR()`

> **Importante — Pattern Master Set vs Working Set**:  
> Poiché `select()` modifica i set in-place, nei server con molti client non si reinseriscono manualmente tutti i descrittori a ogni giro con un ciclo. Si mantiene un **`master_set`** permanente e lo si copia nel set di lavoro temporaneo prima di invocare `select()`:
```c
int main(void) {
    // 1. Creazione e bind del socket del server
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 10);
    // 2. Inizializzazione dei set
    fd_set master_set;
    fd_set read_set;
    FD_ZERO(&master_set);
    FD_ZERO(&read_set);
    // Inseriamo la listening socket nel master_set
    FD_SET(server_fd, &master_set);
    int max_fd = server_fd; // Al momento il fd più alto è quello del server
    char buf[BUF_SIZE];
    printf("Server avviato sulla porta %d. In attesa con select()...\n", PORT);
    while (1) {
        // A ogni giro COPIAMO il master_set nel working set temporaneo
        read_set = master_set;
        // select blocca finché ALMENO UNO dei descrittori ha novità
        int activity = select(max_fd + 1, &read_set, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select");
            break;
        }
        // Scansioniamo tutti i descrittori da 0 a max_fd per trovare chi è pronto
        for (int fd = 0; fd <= max_fd; fd++) {
            
            // Se questo specifico 'fd' non ha eventi, passa al prossimo
            if (!FD_ISSET(fd, &read_set)) {
                continue;
            }
            // ========================================================
            // CASO A: Il socket pronto è la LISTENING SOCKET del server
            // Significa che c'è una NUOVA CONNESSIONE in arrivo!
            // ========================================================
            if (fd == server_fd) {
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int new_client = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
                if (new_client != -1) {
                    // Aggiungiamo il nuovo client all'elenco permanente
                    FD_SET(new_client, &master_set);
                    // Aggiorniamo max_fd se il nuovo descrittore è più grande
                    if (new_client > max_fd) {
                        max_fd = new_client;
                    }
                    printf("[+] Nuovo client connesso (fd: %d)\n", new_client);
                }
            }
            // ========================================================
            // CASO B: Il socket pronto è un CLIENT già connesso
            // Significa che il client ha inviato DATI o ha CHIUSO
            // ========================================================
            else {
                ssize_t n = recv(fd, buf, sizeof(buf) - 1, 0);
                if (n <= 0) {
                    // n == 0 significa EOF (connessione chiusa ordinatamente dal client)
                    // n < 0 significa errore di rete o reset improvviso
                    if (n == 0) {
                        printf("[-] Client disconnesso (fd: %d)\n", fd);
                    } else {
                        perror("recv error");
                    }
                    // 1. Chiudiamo il socket del client
                    close(fd);
                    // 2. Lo RIMUOVIAMO dal master_set così select non lo controllerà più!
                    FD_CLR(fd, &master_set);
                } 
                else {
                    // Dati ricevuti con successo: elaboriamo (es. Echo al client)
                    buf[n] = '\0';
                    printf("[Dati da fd %d]: %s", fd, buf);
                    send(fd, buf, n, 0); // Rispedisce indietro il messaggio
                }
            }
        } // Fine ciclo for sui descrittori
    }
    close(server_fd);
    return 0;
}
 ```

---

## 19. Segnali nelle Socket di Rete — SIGPIPE ed EINTR
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

Nello sviluppo di applicazioni di rete, la gestione dei segnali presenta due problematiche critiche:
1. **L'interruzione delle system call bloccanti (`EINTR`)**, ad esempio durante l'attesa su `accept()` o `recv()`.
2. **La chiusura improvvisa della connessione da parte del peer durante la scrittura (`SIGPIPE`)**.

> *Nota di riferimento:* Per la teoria completa su `sigaction()`, il confronto con `signal()`, e l'uso atomico di `sa_mask`, consultare il [Capitolo 12.8 — Gestione Moderna dei Segnali — `sigaction()`](#128-gestione-moderna-dei-segnali--sigaction).

### 19.1 Gestione delle Syscall Bloccanti e `EINTR`

Quando un server è bloccato in attesa di connessioni (`accept()`) o di dati (`recv()`, `read()`), l'arrivo di un qualsiasi segnale (come `SIGCHLD` emesso da un processo figlio che termina) interrompe la chiamata.

Esistono due approcci standard per gestire questa situazione:

#### Metodo A: Riavvio Automatico con `SA_RESTART` (Raccomandato)
Configurando il gestore con `sigaction()` e impostando il flag `SA_RESTART`, il sistema operativo riavvia automaticamente la chiamata interrotta non appena l'handler termina:
```c
struct sigaction sa = {0};
sa.sa_handler = gestore_sigchld;
sa.sa_flags = SA_RESTART; // Le syscall bloccanti interrotte NON falliscono con EINTR, ma ripartono
sigemptyset(&sa.sa_mask);
sigaction(SIGCHLD, &sa, NULL);
```

#### Metodo B: Gestione Manuale nel Ciclo con `EINTR`
Se `SA_RESTART` non è impostato, la chiamata fallisce restituendo `-1` e impostando `errno = EINTR`. In tal caso, il server deve verificare la condizione e ripetere l'operazione:
```c
for (;;) {
    int sd = accept(listen_sd, ...);
    if (sd >= 0) {
        // Connessione accettata con successo
        break;
    }
    if (errno == EINTR) {
        // Interrotto da segnale: non è un errore fatale, si riprova!
        continue;
    }
    perror("accept");
    break;
}
```

### 19.2 Gestione di `SIGPIPE` nelle Socket TCP

Quando un processo tenta di inviare dati con `write()` o `send()` verso un socket TCP il cui lato remoto è già stato chiuso dal peer (connessione interrotta o caduta), il kernel invia al processo mittente il segnale **`SIGPIPE`**.

* **Comportamento di default:** Terminazione immediata del processo! In un server multi-client o web, questo causerebbe il crash dell'intero applicativo a causa della semplice disconnessione di un singolo client.
* **Soluzione standard:** Ignorare `SIGPIPE` all'avvio del programma e gestire l'interruzione controllando il codice d'errore `EPIPE`:

```c
// 1. Ignorare SIGPIPE all'inizio del programma:
signal(SIGPIPE, SIG_IGN);
// (oppure specificando MSG_NOSIGNAL in send: send(sd, buf, len, MSG_NOSIGNAL);)

// 2. Verificare l'errore EPIPE su send/write:
ssize_t w = send(sd, buf, len, 0);
if (w < 0 && errno == EPIPE) {
    // Il peer ha chiuso la connessione: chiudiamo il descrittore locale
    close(sd);
}

// 3. Per la lettura (recv/read), la chiusura del peer non genera segnali,
//    ma restituisce semplicemente 0 (EOF):
ssize_t n = recv(sd, buf, sizeof(buf), 0);
if (n == 0) {
    // Il peer ha chiuso la connessione in modo ordinato (FIN)
    close(sd);
}
```

---

## 20. Broadcast e Multicast UDP
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 20.1 Broadcast

Invio di pacchetti a **tutti i nodi** di una rete locale. Solo in **IPv4**, solo con **UDP** (`SOCK_DGRAM`).

**Indirizzi broadcast:**
- **Limitato**: `255.255.255.255` (non esce dalla LAN, spesso bloccato dai router)
- **Di subnet**: es. `192.168.1.255` per la rete `192.168.1.0/24`

**Sender:**
```c
int sock = socket(AF_INET, SOCK_DGRAM, 0);
int yes = 1;
setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));  // obbligatorio

struct sockaddr_in addr = {0};
addr.sin_family = AF_INET;
addr.sin_port = htons(PORT);
addr.sin_addr.s_addr = inet_addr("255.255.255.255");
sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&addr, sizeof(addr));
```

**Receiver:**
```c
int sock = socket(AF_INET, SOCK_DGRAM, 0);
int yes = 1;
setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

struct sockaddr_in addr = {0};
addr.sin_family = AF_INET;
addr.sin_port = htons(PORT);
addr.sin_addr.s_addr = htonl(INADDR_ANY);
bind(sock, (struct sockaddr*)&addr, sizeof(addr));
recv(sock, buffer, sizeof(buffer)-1, 0);
```

### 20.2 Multicast

Invio di pacchetti a un **gruppo selezionato** di destinatari che si iscrivono a un indirizzo multicast.

> [!NOTE]
> **L'IP Multicast come "Frequenza Radio":**  
> Un indirizzo multicast (es. `239.255.0.1`) **non appartiene a nessun computer fisico**, ma rappresenta un "canale radio" virtuale:
> * Il **Sender** trasmette su quel canale senza sapere chi o quanti siano i destinatari.
> * I **Receiver** si "sintonizzano" sulla frequenza unendosi al gruppo (`IP_ADD_MEMBERSHIP`). I computer non iscritti ignorano i pacchetti a livello hardware senza pesare sulla CPU.
> * Il range **`239.x.x.x`** è riservato all'uso privato/amministrativo locale (l'equivalente di `192.168.x.x` per il multicast), perfetto da usare liberamente nelle proprie applicazioni.

**Indirizzi multicast IPv4:** `224.0.0.0` – `239.255.255.255` (Classe D)
- `224.0.0.x` → link-local (protocolli di rete di sistema, es. OSPF o mDNS `224.0.0.251`)
- `239.x.x.x` → amministrativi locali (consigliati per applicazioni utente)

**Sender multicast:**
```c
int sock = socket(AF_INET, SOCK_DGRAM, 0);
unsigned char ttl = 1;  // TTL = 1 → solo LAN locale
setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));

struct sockaddr_in addr = {0};
addr.sin_family = AF_INET;
addr.sin_port = htons(5000);
addr.sin_addr.s_addr = inet_addr("239.255.0.1");
sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&addr, sizeof(addr));
```

**Receiver multicast (con JOIN):**
```c
int sock = socket(AF_INET, SOCK_DGRAM, 0);
int reuse = 1;
setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

struct sockaddr_in addr = {0};
addr.sin_family = AF_INET;
addr.sin_port = htons(5000);
addr.sin_addr.s_addr = htonl(INADDR_ANY);
bind(sock, (struct sockaddr*)&addr, sizeof(addr));

// JOIN al gruppo multicast
struct ip_mreq mreq;
mreq.imr_multiaddr.s_addr = inet_addr("239.255.0.1");
mreq.imr_interface.s_addr = htonl(INADDR_ANY);
setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

recv(sock, buf, sizeof(buf)-1, 0);
```

### 20.3 Protocolli di Rete per il Multicast

| Protocollo | Ruolo |
|-----------|-------|
| **IGMP** | L'host annuncia al router di voler ricevere un gruppo multicast (JOIN/LEAVE) |
| **PIM** | I router costruiscono l'albero di distribuzione tra sottoreti |
| **IGMP Snooping** | Lo switch filtra il multicast inviandolo solo alle porte con host iscritti |

---

## 21. Comandi di Rete e Risoluzione DNS
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 21.1 Comandi di Diagnostica

| Comando | Funzione |
|---------|----------|
| `netstat -tanp` | Mostra tutte le connessioni TCP con PID e indirizzi numerici |
| `ss -tln` | Socket Statistics — mostra socket TCP in ascolto |
| `ip addr` (o `ip a`) | Mostra indirizzi IP delle interfacce |
| `ip link` | Elenca tutte le interfacce di rete |
| `ip route` | Mostra le rotte e il gateway |
| `nslookup <dominio>` | Interroga DNS per un dominio |
| `dig <dominio>` | Query DNS avanzata |
| `dig +trace <dominio>` | Traccia completa della risoluzione DNS |
| `dig -x <IP>` | Reverse DNS lookup |
| `telnet <host> <port>` | Test connessione TCP |
| `nc <host> <port>` | Netcat — client/server TCP/UDP minimale |

### 21.2 Risoluzione DNS in C — `getaddrinfo()`

Funzione moderna e portabile per risolvere nomi simbolici in indirizzi IP. A differenza di vecchie funzioni (come `gethostbyname`), restituisce una lista concatenata di strutture `addrinfo` già pronte per l'uso:

```c
struct addrinfo {
    int              ai_flags;
    int              ai_family;    // Es. AF_INET, AF_INET6, AF_UNSPEC
    int              ai_socktype;  // Es. SOCK_STREAM, SOCK_DGRAM
    int              ai_protocol;
    socklen_t        ai_addrlen;   // Dimensione di ai_addr
    struct sockaddr *ai_addr;      // L'indirizzo vero e proprio pronto per bind/connect
    char            *ai_canonname; // Nome canonico
    struct addrinfo *ai_next;      // Puntatore al prossimo risultato (linked list)
};
```

```c
#include <netdb.h>
#include <arpa/inet.h>

struct addrinfo hints, *res, *p;
memset(&hints, 0, sizeof(hints));
hints.ai_family   = AF_UNSPEC;    // IPv4 + IPv6
hints.ai_socktype = SOCK_STREAM;  // TCP

int err = getaddrinfo("www.example.com", "80", &hints, &res);
if (err != 0) {
    fprintf(stderr, "Errore: %s\n", gai_strerror(err));
    exit(1);
}

for (p = res; p != NULL; p = p->ai_next) {
    int sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sock < 0) continue;
    if (connect(sock, p->ai_addr, p->ai_addrlen) == 0) {
        printf("Connesso!\n");
        break;
    }
    close(sock);
}
freeaddrinfo(res);
```

> **Vantaggi**: supporta IPv4/IPv6 senza differenze per il programmatore, restituisce strutture pronte per `socket()` e `connect()`.

**Esempio Pratico: Client WHOIS**
Un uso classico di `getaddrinfo()` è interrogare server esterni. Questo frammento mostra come connettersi al server WHOIS IANA sulla porta 43 e inviare un nome a dominio (come spiegato nella Lezione 24):
```c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM; // TCP
    getaddrinfo("whois.iana.org", "43", &hints, &res);
    
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    connect(sock, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);
    
    char query[256];
    snprintf(query, sizeof(query), "%s\r\n", argv[1]);
    send(sock, query, strlen(query), 0);
    
    char buf[1024];
    int n;
    while ((n = recv(sock, buf, sizeof(buf)-1, 0)) > 0) {
        buf[n] = '\0';
        printf("%s", buf);
    }
    close(sock);
    return 0;
}
```

---

## 22. Virtualizzazione
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 22.1 Concetti Base

La **virtualizzazione** crea una versione virtuale di una risorsa normalmente fisica (CPU, memoria, disco, rete, SO).

| Termine | Significato |
|---------|-------------|
| **Host** | Sistema/macchina fisico che ospita le VM |
| **Guest** | Sistema operativo eseguito nella VM |
| **Hypervisor / VMM** | Software che crea, gestisce e isola le VM |

**Motivazioni:**
- **Isolamento**: software difettoso non tocca l'host
- **Testing**: prove e rollback facili
- **Portabilità**: spostare una VM ovunque
- **Consolidamento**: più server sulla stessa macchina
- **Compatibilità**: SO vecchi su hardware nuovo

### 22.2 Simulazione, Emulazione, Virtualizzazione

| Approccio | Descrizione | Esempio |
|-----------|-------------|---------|
| **Simulazione** | Modello astratto, non fedele | Simulatori di rete |
| **Emulazione** | Riproduce un'architettura diversa (lento) | QEMU (ARM su x86) |
| **Virtualizzazione** | Più SO della stessa architettura sull'hardware reale (veloce) | VMware, KVM |

### 22.3 Tipi di Hypervisor

| Tipo | Descrizione | Esempi |
|------|-------------|--------|
| **Tipo 1** (bare-metal) | Hypervisor direttamente sull'hardware | VMware ESXi, Xen, Hyper-V, **KVM** |
| **Tipo 2** (hosted) | Hypervisor sopra un SO host | VMware Workstation, VirtualBox |

### 22.4 Tecniche di Virtualizzazione

| Tecnica | Descrizione |
|---------|-------------|
| **Full Virtualization** | Guest non sa di essere virtualizzato (massima compatibilità, più overhead) |
| **Paravirtualization** | Guest collabora con l'hypervisor via API (meno overhead, OS modificato) |
| **HW-Assisted** | CPU fornisce istruzioni speciali (Intel VT-x, AMD-V, ARM EL2) — **usata oggi** |

### 22.5 Virtualizzazione HW-Assisted

Le CPU moderne aggiungono modalità per la virtualizzazione:
- **VMX Root Mode** → Hypervisor
- **VMX Non-Root Mode** → Guest VM

**Traduzione indirizzi a due livelli (EPT/NPT):**
```
Processo della VM → Guest Virtual Address
    → Guest Page Tables (kernel guest)
        → Guest Physical Address
            → EPT/NPT/S2 Tables (hypervisor)
                → Host Physical Memory (RAM reale)
```

### 22.6 KVM (Linux), Hyper-V (Windows), macOS

**KVM** (Kernel-based Virtual Machine):
- Integrato nel kernel Linux (tipo 1)
- Le VM sono processi user-space che accedono a `/dev/kvm`
- Linux diventa hypervisor tramite le estensioni hardware (Intel VT-x / AMD-V)
- Usato da: QEMU, libvirt, Docker Desktop, Firecracker

**Hyper-V** (Windows):
- Tipo 1 bare-metal. **Windows NON è l'host** — è una VM privilegiata
- **Root Partition**: SO privilegiato che gestisce driver, I/O, networking, storage e avvia le VM  
  (equivalente a *Dom0* in Xen, *Host Linux* in KVM, *VMkernel* in ESXi)
- **Guest Partition**: le VM vere e proprie (WSL2, VM Hyper-V, Docker)

```
Hardware
    ↓
Hyper-V (TYPE 1)
    ├── Windows (Root Partition) → gestisce driver, I/O, storage
    └── Guest Partitions → WSL2, VM Hyper-V, Docker
```

**macOS (Apple Silicon):**
- Usa `Hypervisor.framework` (API user-space)
- Le VM sono gestite da applicazioni user-space (UTM, Docker Desktop, VMware Fusion)
- Il kernel macOS resta in **EL1** — **NON** è un hypervisor come KVM o Hyper-V
- Il supporto hardware ARM gestisce: modalità hypervisor (**EL2**) e **Stage-2 paging** (traduzione indirizzi a due livelli)

### 22.7 VM vs Container

| Aspetto | Virtual Machine | Container |
|---------|----------------|-----------|
| Kernel | Proprio kernel guest | Condivide kernel host |
| Avvio | Lento (boot completo) | Immediato |
| Peso | GB | MB |
| Isolamento | Fortissimo (hardware) | Kernel-level (namespace/cgroups) |
| Gestione | Hypervisor | Container runtime (Docker, Podman) |

### 22.8 Gestione delle Risorse nell'Hypervisor

L'hypervisor assegna **risorse virtuali** alle VM (vCPU, vRAM, vDisk, vNIC):

| Risorsa | Tecnica |
|---------|---------|
| **CPU** | Scheduler dell'hypervisor gestisce la contesa tra VM |
| **Memoria** | **Ballooning** (VM restituisce memoria all'host), **Deduplica pagine KSM** (Kernel Same-page Merging) |
| **Storage** | Dischi virtuali astratti: **VMDK** (VMware), **QCOW2** (QEMU) |

> **Overcommit**: si possono allocare più risorse virtuali di quelle fisiche (es. due VM da 6 GB su un host con 8 GB RAM). Funziona finché non tutte le VM saturano le risorse simultaneamente. Può causare latenze se la contesa è elevata.

### 22.9 Confronto WSL2 vs VMware Workstation

| Caratteristica | WSL2 | VMware Workstation |
|----------------|------|--------------------|
| **Avvio** | Istantaneo | Lento (boot completo) |
| **Integrazione Windows** | Molto alta (filesystem condiviso, GUI Linux) | Limitata |
| **Configurabilità** | Bassa (CPU/RAM fissi) | Alta (CPU, RAM, rete, dischi) |
| **Snapshot** | No | Sì |
| **Sistemi supportati** | Solo Linux | Qualunque OS |
| **Networking** | NAT + Mirrored Mode | NAT, Bridged, Host-only |
| **Prestazioni Linux** | Molto alte (kernel nativo) | Buone ma con overhead |
| **Isolamento** | Parziale | Elevato |

---

## 23. Container: Namespace, Cgroups e OverlayFS
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 23.1 Architettura dei Container

I container isolano i processi usando funzionalità del **kernel Linux**:
- **Namespace** → isolano *cosa* un processo può vedere
- **Cgroups** → controllano *quante* risorse un processo può consumare
- **OverlayFS** → forniscono un filesystem a strati

### 23.2 Namespace

Ogni tipo di namespace isola un aspetto diverso:

| Namespace | Flag `clone()` | Cosa isola |
|-----------|----------------|------------|
| **PID** | `CLONE_NEWPID` | Tabella dei processi |
| **NET** | `CLONE_NEWNET` | Stack di rete (IP, porte, routing, firewall) |
| **MNT** | `CLONE_NEWNS` | Tabella dei mount point |
| **UTS** | `CLONE_NEWUTS` | Hostname e domain name |
| **IPC** | `CLONE_NEWIPC` | Code di messaggi, semafori |
| **USER** | `CLONE_NEWUSER` | UID/GID mapping |
| **CGROUP** | `CLONE_NEWCGROUP` | Vista dei cgroup |

**System call per i namespace:**

| Funzione | Scopo |
|----------|-------|
| `clone(fn, stack, flags, arg)` | Crea un processo con nuovi namespace |
| `unshare(flags)` | Il processo corrente entra in nuovi namespace |
| `setns(fd, nstype)` | Entra in un namespace esistente (via `/proc/<PID>/ns/`) |

**Esempio — creare un mini-container dalla shell:**
```bash
sudo unshare --pid --uts --ipc --net --mount --fork --mount-proc bash
# Il processo diventa PID 1 (come init in un container reale)
hostname mio-container
ps aux    # vede solo i propri processi
exit      # distrugge il namespace
```

**Struttura kernel dei Namespace:**

Ogni processo ha un campo `nsproxy` che punta a tutti i suoi namespace:

```c
struct task_struct {
    struct nsproxy *nsproxy;  // punta a tutti i namespace del processo
    // nsproxy contiene puntatori a:
    // struct pid_namespace, struct uts_namespace,
    // struct net, struct user_namespace, ...
};
```

**Mount Namespace — cosa isola e cosa NON isola:**

| Isola | NON Isola |
|-------|-----------|
| Tabella dei mount point visibili dal processo | Contenuto dei file sui filesystem reali |
| Disposizione dei filesystem montati (/, /proc, /home…) | Il dispositivo fisico sottostante |
| Permette montare/smontare senza influenzare altri | Modifiche ai file reali restano reali |

**NET Namespace — cosa isola e cosa NON isola:**

| Isola | NON Isola |
|-------|-----------|
| Tabelle di routing | La scheda di rete fisica vera |
| Interfacce di rete (IP, porte TCP/UDP) | La banda reale disponibile |
| Firewall (iptables/nftables) | Le connessioni fisiche |
| Stato delle connessioni | — |

> Due processi in NET namespace differenti possono bindare la stessa IP e porta senza conflitti (ogni namespace ha la propria istanza dello stack TCP/IP).

**Per creare un filesystem privato** (il mount namespace da solo non basta):

```c
mount()       // montare FS in un mount namespace
umount2()     // smontare
pivot_root()  // cambiare la radice del FS ("/" del container)
```

### 23.3 PID Namespace — Gerarchia

I PID namespace formano un **albero gerarchico**:

```
Global Namespace      PID = 24133
    └── Namespace A       PID = 1
        └── Namespace B   PID = 1
```

- Un processo è visibile in **tutti i namespace antenato** (ma non nei discendenti)
- Il kernel mantiene un array `numbers[]` con il PID in ogni livello della gerarchia:

```
Namespace B (corrente)  → numbers[0] = 1
Namespace A (genitore)  → numbers[1] = 47
Global namespace        → numbers[2] = 24322
```

- La struttura `struct pid` nel kernel gestisce la traduzione dei PID lungo la gerarchia
- Si può ispezionare via `/proc/<PID>/ns/` e `/proc/<PID>/status` → campo `NSpid: 24133  1  1`


#### Process Subreaper e Adozione dei Processi Orfani (`adopt_test.c`)
Nelle slide (*Slide 1301–1303, 1316*) viene trattato un meccanismo fondamentale per l'architettura dei container e dei processi di supervisione (come `systemd` o Docker): il **Process Subreaper**.

**Il problema classico:**
Nei sistemi Unix tradizionali, se un processo padre termina prima dei suoi figli, questi ultimi diventano **orfani** e vengono automaticamente "adottati" dal processo radice del sistema (**PID 1**, `init` / `systemd`), che si fa carico di chiamare `wait()` ed eliminare i loro stati zombie.

**Il meccanismo Subreaper (`PR_SET_CHILD_SUBREAPER`):**
In scenari a container o servizi complessi, non si desidera che gli orfani risalgano fino a PID 1 dell'host. Con la chiamata:
```c
#include <sys/prctl.h>

prctl(PR_SET_CHILD_SUBREAPER, 1);
```
Un processo si dichiara **subreaper**: d'ora in avanti, se un qualsiasi processo discendente all'interno del suo sottoalbero muore lasciando figli orfani, questi ultimi verranno adottati **da questo processo**, e non da PID 1!

**Comportamento nei PID Namespace:**
Quando si crea un nuovo PID namespace (es. con `sudo unshare --pid --fork --mount-proc bash`):
- Il processo shell avviato dentro il namespace assume **PID 1** locale.
- Se all'interno del namespace viene lanciato un processo che a sua volta crea un figlio e poi termina (es. `bash -c 'sleep 100 & exit'`), il processo figlio `sleep 100` diventa orfano e viene **adottato automaticamente dal PID 1 del namespace locale** (il subreaper implicito del namespace), mantenendo l'isolamento completo rispetto all'host!

**Codice di Test dell'Adozione (`adopt_test.c`, Slide 1316):**
```c
// adopt_test.c - Verifica adozione orfano
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
    printf("[Padre] Start:  PID=%d  PPID=%d
", getpid(), getppid());
    pid_t child = fork();
    if (child < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child == 0) {
        // PROCESSO FIGLIO
        printf("[Figlio] Start: PID=%d  PPID=%d
", getpid(), getppid());
        for (int i = 0; i < 6; i++) {
            sleep(1);
            printf("[Figlio] t=%d sec -> PID=%d, PPID=%d
", i + 1, getpid(), getppid());
        }
        return 0;
    } else {
        // PROCESSO PADRE: attende 2 secondi poi esce volontariamente
        printf("[Padre] Figlio creato con PID=%d. Il padre esce tra 2 secondi...
", child);
        sleep(2);
        printf("[Padre] Uscita padre!
");
        _exit(0);
    }
}
```
*Osservazione:* Se lanciato da terminale host, dopo $t=2$ il figlio mostrerà `PPID = 1` (o il PID del subreaper `systemd --user`). Se lanciato dentro un namespace isolato `unshare --pid --fork bash`, il PPID diventerà esattamente `1` (il processo shell interno al namespace).

#### Rappresentazione dei Namespace nel Kernel e `/proc/<PID>/ns/`
A livello kernel (*Slide 1332*), i namespace sono entità anonime collegate a ciascun task:
- Ogni `task_struct` contiene un puntatore `struct nsproxy *nsproxy`.
- La struttura `nsproxy` punta alle tabelle dei singoli namespace del processo:
  - `uts_ns` (UTS: hostname)
  - `ipc_ns` (IPC: code di messaggi, semafori, shm)
  - `mnt_ns` (Mount: punti di montaggio)
  - `pid_ns_for_children` (PID: albero dei processi)
  - `net_ns` (Rete: interfacce, tabelle di routing, porte)

Nel filesystem virtuale `/proc`, il kernel espone i namespace del processo corrente come **file descriptor / link simbolici speciali** nella directory `/proc/<PID>/ns/`:
```bash
ls -l /proc/$$/ns
```
Output tipico:
```text
lrwxrwxrwx 1 user user 0 cgroup -> 'cgroup:[4026531835]'
lrwxrwxrwx 1 user user 0 ipc    -> 'ipc:[4026531839]'
lrwxrwxrwx 1 user user 0 mnt    -> 'mnt:[4026531840]'
lrwxrwxrwx 1 user user 0 net    -> 'net:[4026531992]'
lrwxrwxrwx 1 user user 0 pid    -> 'pid:[4026531836]'
lrwxrwxrwx 1 user user 0 user   -> 'user:[4026531837]'
lrwxrwxrwx 1 user user 0 uts    -> 'uts:[4026531838]'
```
I numeri tra parentesi quadre rappresentano gli **identificatori univoci (inode)** del namespace nel kernel. Se due processi hanno lo stesso numero di inode per un namespace (es. `net:[4026531992]`), significa che condividono lo stesso stack di rete. Con la system call `setns(int fd, int nstype)` un processo può entrare nel namespace di un altro aprendo il corrispondente file in `/proc/<PID>/ns/`.


### 23.4 Cgroups (Control Groups)

I **cgroups** limitano, monitorano e isolano l'uso delle risorse di un gruppo di processi.

Gestiti tramite lo pseudo-filesystem `/sys/fs/cgroup/`:

```bash
# Limitare la memoria a 100 MB
sudo mkdir /sys/fs/cgroup/mygroup
echo 100M | sudo tee /sys/fs/cgroup/mygroup/memory.max
echo <PID> | sudo tee /sys/fs/cgroup/mygroup/cgroup.procs

# Limitare la CPU al 20%
echo "20000 100000" | sudo tee /sys/fs/cgroup/mycpu/cpu.max
# quota=20000 µs, period=100000 µs → 20%

# Limitare il numero di processi
echo 50 | sudo tee /sys/fs/cgroup/mylimit/pids.max
```

**Nel kernel:**
```c
struct task_struct {
    struct css_set *cgroups;  // punta ai cgroup del task
};
struct cgroup {
    struct cgroup *parent;    // gerarchia ad albero
    struct kernfs_node *kn;   // entry nel filesystem
};
```

#### Stress-Test di un Cgroup v2 in C (`test_cgroup.c`, Slide 1359–1360)
Nelle slide viene fornito il codice C completo per creare programmaticamente un cgroup v2, confinarvi un processo figlio e verificare il triggering dell'Out-Of-Memory (OOM) Killer del kernel:

```c
// test_cgroup.c - Gestione e test cgroup v2 da C
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#define CGROUP_ROOT "/sys/fs/cgroup"
#define CGROUP_NAME "demo_cgroup"
#define CGROUP_PATH CGROUP_ROOT "/" CGROUP_NAME

static int write_file(const char *path, const char *value) {
    int fd = open(path, O_WRONLY | O_CLOEXEC);
    if (fd < 0) {
        fprintf(stderr, "open(%s) fallita: %s\n", path, strerror(errno));
        return -1;
    }
    ssize_t len = strlen(value);
    if (write(fd, value, len) != len) {
        fprintf(stderr, "write(%s) fallita: %s\n", path, strerror(errno));
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int main(void) {
    // 1) Controllo che esista la root di cgroup v2
    struct stat st;
    if (stat(CGROUP_ROOT, &st) == -1) {
        perror("stat cgroup root");
        return 1;
    }

    // 2) Creo la directory del cgroup (il kernel istanzia automaticamente i file di controllo)
    if (mkdir(CGROUP_PATH, 0755) == -1 && errno != EEXIST) {
        perror("mkdir cgroup");
        return 1;
    }
    printf("[parent] Cgroup creato in: %s\n", CGROUP_PATH);

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        // === PROCESSO FIGLIO ===
        printf("[child] PID figlio: %d. Attendo spostamento nel cgroup...\n", getpid());
        sleep(2);
        printf("[child] Inizio allocazione aggressiva di memoria...\n");
        const size_t step = 10 * 1024 * 1024; // 10 MB per iterazione
        size_t total = 0;
        while (1) {
            void *p = malloc(step);
            if (!p) { perror("[child] malloc fallita"); break; }
            memset(p, 'A', step); // Forza l'allocazione fisica delle pagine RAM
            total += step;
            printf("[child] Allocati fisicamente ~%zu MB\n", total / (1024 * 1024));
            sleep(1);
        }
        return 0;
    }

    // === PROCESSO PADRE ===
    // 3) Sposta il processo figlio dentro il cgroup scrivendo il suo PID in cgroup.procs
    char procs_path[] = CGROUP_PATH "/cgroup.procs";
    char buf[32];
    snprintf(buf, sizeof(buf), "%d\n", pid);
    if (write_file(procs_path, buf) < 0) {
        kill(pid, SIGKILL);
        return 1;
    }
    printf("[parent] Processo PID %d inserito in %s\n", pid, CGROUP_PATH);

    // 4) Imposta il limite di memoria massima a 50 MB (cgroup v2 memory.max)
    const char *mem_limit = "52428800\n"; // 50 MB in byte
    if (write_file(CGROUP_PATH "/memory.max", mem_limit) < 0) {
        kill(pid, SIGKILL);
        return 1;
    }
    printf("[parent] Impostato memory.max = 50 MB. Il figlio verra' terminato per OOM al superamento!\n");

    // 5) Attende la terminazione del figlio e ne analizza lo stato
    int status;
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status)) {
        printf("[parent] Figlio terminato da SEGNALE %d (SIGKILL=%d per OOM Killer!)\n", 
               WTERMSIG(status), SIGKILL);
    } else {
        printf("[parent] Figlio terminato con exit status %d\n", WEXITSTATUS(status));
    }
    return 0;
}
```


### 23.5 OverlayFS — Filesystem a Strati

**OverlayFS** è un filesystem union usato dai container:
- **lowerdir** = layer immagine (read-only)
- **upperdir** = layer del container (read-write)
- **workdir** = area di lavoro temporanea del kernel
- **merged** = vista finale (RO + RW)

**Principio: Copy-on-Write**
- Quando il container modifica un file, il file viene copiato nel `upperdir`
- L'immagine originale (`lowerdir`) non viene mai toccata
- Al rollback basta cancellare l'`upperdir`

```bash
mkdir lower upper work merged
echo "Hello dal lower" > lower/file.txt
sudo mount -t overlay overlay \
    -o lowerdir=lower,upperdir=upper,workdir=work merged
echo "MODIFICATO" > merged/file.txt
cat upper/file.txt   # modificato
cat lower/file.txt   # invariato
sudo umount merged
```

**Meccanismo Whiteout:**

Quando si cancella un file nel layer `merged`, OverlayFS non tocca il `lowerdir` ma crea un file speciale nel `upperdir` chiamato **whiteout** (`.wh.<nomefile>`):

```bash
rm merged/file.txt           # cancella il file nel container
ls -a upper/                 # appare: .wh.file.txt
# Il whiteout “nasconde” il file nel lowerdir senza modificarlo
cat lower/file.txt           # il file originale è ancora intatto
```

> Il **whiteout** è un file di tipo `character device (0,0)` che segnala a OverlayFS di ignorare il corrispondente file del lowerdir. Al rollback basta cancellare l'`upperdir` (inclusi i whiteout) e il file originale torna visibile.

---


### 23.6 Mini-Container Didattico in C tramite `clone()` e `mount()` (`test_container.c`)
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

Nelle slide (*Slide 1361–1362*), viene presentata l'implementazione in C di un **container funzionante da zero** utilizzando esclusivamente le chiamate native del kernel Linux (`clone`, `sethostname`, `mount`):

```c
// test_container.c - Mini container in C
#define _GNU_SOURCE
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define STACK_SIZE (1024 * 1024) // 1 MB di stack dedicato

static int child_main(void *arg) {
    (void)arg;
    printf("[child] Esecuzione nel nuovo namespace. PID locale: %d\n", getpid());

    // 1) Modifica l'hostname isolato (UTS namespace)
    const char *new_hostname = "minikube-lso";
    if (sethostname(new_hostname, strlen(new_hostname)) == -1) {
        perror("[child] sethostname");
        return 1;
    }

    // 2) Assicura che la directory /proc esista
    mkdir("/proc", 0555);

    // 3) Rimonta /proc nel nuovo Mount namespace privato
    // In questo modo comandi come 'ps' leggeranno i soli processi del nuovo PID namespace!
    if (mount("proc", "/proc", "proc", 0, "") == -1) {
        perror("[child] mount /proc");
        return 1;
    }

    // Verifica hostname aggiornato
    struct utsname uts;
    uname(&uts);
    printf("[child] Nuovo Hostname interno: %s\n", uts.nodename);
    printf("[child] Lancio shell /bin/sh interna al container...\n\n");

    // 4) Sostituisce il processo con una shell interattiva isolata
    char *const args[] = { "/bin/sh", NULL };
    execvp(args[0], args);

    perror("[child] execvp fallita");
    return 1;
}

int main(void) {
    // A differenza di fork(), clone() richiede uno stack esplicitamente allocato
    char *stack = malloc(STACK_SIZE);
    if (!stack) { perror("malloc stack"); exit(EXIT_FAILURE); }
    char *stack_top = stack + STACK_SIZE; // Su x86 lo stack cresce verso il basso

    printf("[parent] PID host del processo principale: %d\n", getpid());
    printf("[parent] Avvio del mini-container tramite clone() con flag di namespace...\n");

    // Flag di isolamento combinati:
    // - CLONE_NEWPID: Nuovo PID namespace (il figlio vedrà getpid() == 1)
    // - CLONE_NEWUTS: Nuovo UTS namespace (hostname privato)
    // - CLONE_NEWNS:  Nuovo Mount namespace (punti di mount privati)
    // - CLONE_NEWNET: Nuovo Network namespace (interfacce di rete isolate)
    // - SIGCHLD:      Invia SIGCHLD al padre quando termina (permettendo la waitpid)
    int flags = CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | CLONE_NEWNET | SIGCHLD;

    pid_t child_pid = clone(child_main, stack_top, flags, NULL);
    if (child_pid == -1) {
        perror("clone");
        free(stack);
        exit(EXIT_FAILURE);
    }

    printf("[parent] clone() completata: PID host del child = %d\n", child_pid);
    printf("[parent] All'interno del namespace il processo figlio sara' PID 1 (Init locale).\n\n");

    // Attende la chiusura della shell del container
    int status;
    waitpid(child_pid, &status, 0);
    printf("[parent] Container terminato con status %d\n", status);

    free(stack);
    return 0;
}
```

---

## 24. Docker
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

### 24.1 Cos'è Docker

Docker è una piattaforma di **containerizzazione** che usa namespace, cgroups e OverlayFS per creare container leggeri e portabili.

**Pipeline Docker:**
```
Dockerfile → docker build → immagine → docker run → container → processi
```

**Architettura Docker Engine:**
- **Docker daemon** (`dockerd`): riceve comandi dal client
- **containerd**: supervisore dei container (start, stop, pause, delete)
- **runc**: runtime di basso livello (interfaccia al kernel)
- **shim**: disaccoppia runc da containerd

**Installazione:**

```bash
# Linux (Ubuntu/Debian/Mint)
sudo apt update && sudo apt install docker.io
sudo systemctl enable --now docker
sudo usermod -aG docker $USER   # usa Docker senza sudo (riloggare dopo)

# macOS / Windows: Docker Desktop
# https://www.docker.com/products/docker-desktop/
# Windows richiede: WSL2 abilitato + Virtualization nel BIOS
```

### 24.2 Comandi Base

```bash
docker --version                    # verifica installazione
docker run hello-world              # test base

docker pull ubuntu                  # scarica immagine
docker images                       # lista immagini
docker rmi <image-id>               # rimuove immagine

docker run -it ubuntu bash          # container interattivo
docker run --rm -it ubuntu bash     # auto-rimuovi all'uscita
docker run -d ubuntu sleep 1000     # esecuzione in background

docker ps                           # container attivi
docker ps -a                        # tutti (anche fermati)
docker stop <container-id>
docker start <container-id>
docker rm <container-id>
docker container prune              # rimuove tutti i container fermati

docker exec -it <container> bash              # entra in un container attivo
docker exec --user root <container> bash      # come utente specifico
docker exec <container> ps aux                # esegue un comando senza terminale

docker inspect <container/image>              # dettagli in formato JSON
docker inspect <container> --format '{{.State.Pid}}'  # PID reale del container

docker history <image>                        # mostra i layer di un'immagine
docker stats                                  # CPU/RAM in tempo reale

docker cp file.txt container:/path            # copia file HOST → container
docker cp container:/path file.txt            # copia file container → HOST
```

> **Registri**: Le immagini si scaricano da un **registry** (deposito). Il default è Docker Hub (`docker.io/library/<nome>`). Registri alternativi: `quay.io/bitnami/python`, `ghcr.io/...`

### 24.3 Networking Docker

```bash
docker network ls                   # lista reti esistenti
docker network create miarete       # crea rete bridge
docker network inspect miarete      # dettagli rete

# Collegare container alla rete
docker run --name server --network miarete ubuntu
docker run --name client --network miarete ubuntu
# "client" può contattare "server" per nome (DNS interno Docker)

docker network rm miarete           # rimuove rete
```

Docker crea un **DNS interno** per ogni rete: l'hostname è il nome del container.

### 24.4 Dockerfile

Un **Dockerfile** è un file di testo che descrive come costruire un'immagine.

**Istruzioni principali:**

| Istruzione | Significato |
|------------|-------------|
| `FROM` | Immagine base |
| `RUN` | Esegue comandi durante la build (crea layer) |
| `COPY` | Copia file dal build context nell'immagine |
| `WORKDIR` | Imposta la directory di lavoro |
| `EXPOSE` | Dichiara porte esposte |
| `CMD` | Comando di default all'avvio del container |
| `ENTRYPOINT` | Comando fisso (non sostituibile) all'avvio |

**Esempio — Server C:**
```dockerfile
FROM ubuntu:22.04
RUN apt update && apt install -y gcc
WORKDIR /app
COPY server.c .
RUN gcc server.c -o server
CMD ["./server"]
```

**Build e run:**
```bash
docker build -t c-server .
docker run --rm --name server-cont --network miarete c-server
```

**Layer caching:** ogni istruzione crea un layer immutabile. Se un'istruzione non cambia, Docker riusa il layer → build veloci.

> **`COPY` vs `docker cp`**: `COPY` nel Dockerfile opera a **build-time** (crea un layer immutabile dell'immagine). `docker cp` opera a **runtime** sul container già avviato (agisce sul layer scrivibile del container, non crea layer permanenti).


#### Direttive Dockerfile: Confronto Approfondito `ADD` vs `COPY`
Nelle slide (*Slide 1381–1383*), viene evidenziata una distinzione fondamentale tra le istruzioni di copia:

| Proprietà | `COPY <src> <dest>` | `ADD <src> <dest>` |
|:---|:---|:---|
| **Scopo principale** | Copia trasparente e diretta di file/directory locali dall'host all'immagine. | Copia con funzionalità avanzate di estrazione e download. |
| **Archivi compressi (`.tar`, `.tar.gz`, `.tgz`, `.bz2`)** | Li copia tal quali come file binari compressi dentro la destinazione. | **Li decomprime ed estrae automaticamente** all'interno della directory di destinazione nel container. |
| **Sorgenti remote (URL HTTP/HTTPS)** | Non supportate (richiede file locali presenti nel build context). | **Supportate**: può scaricare file direttamente da un URL web nella destinazione. |
| **Best Practice Docker** | **Raccomandata al 99% dei casi** per trasparenza, chiarezza e sicurezza. | Da utilizzare **esclusivamente quando serve la decompressione automatica di tarball locali** direttamente nell'immagine. Per URL remoti, è preferibile usare `RUN curl ... && tar ... && rm ...` per mantenere compatto il layer. |

**Esempio di differenza pratica:**
```dockerfile
# Con COPY: il file rimane un archivio compresso da 5 MB dentro /app
COPY archivio.tar.gz /app/

# Con ADD: il file viene decompresso automaticamente, e dentro /app/ compaiono i file estratti!
ADD archivio.tar.gz /app/
```


### 24.5 Docker Compose

**Docker Compose** permette di descrivere più container in un unico file `docker-compose.yml`.

**Concetto di Service**: un **service** è un modello astratto di container; permette di lanciare più istanze dello stesso tipo. In un service si definisce:
- `image` / `build` — immagine da usare o da costruire
- `command` — entrypoint/comando da eseguire
- `environment` — variabili d'ambiente
- `networks` — reti a cui collegare il container
- `volumes` — volumi da montare
- `depends_on` — dipendenze di avvio (il service parte dopo quelli elencati)

Per ogni service, Compose: costruisce l'immagine, crea il container con `--network`, gli assegna hostname interno (nome del service o `container_name`) e IP interno (es. `172.18.0.x`), poi lo avvia.

**Esempio `docker-compose.yml`:**

```yaml
services:
  server:
    build: ./server
    container_name: server-cont
    networks:
      - miarete
  client:
    build: ./client
    container_name: client-cont
    networks:
      - miarete
    depends_on:
      - server              # il client parte dopo il server
    command: ["server-cont"]
networks:
  miarete:
```

**Comandi Compose:**
```bash
docker compose build             # costruisce le immagini
docker compose up                # avvia tutti i servizi
docker compose up --scale client=5  # lancia 5 istanze del client
docker compose down              # ferma e rimuove tutto
docker compose down -v           # rimuove anche i volumi
```

### 24.6 Limiti sulle Risorse (cgroups in Docker)

**CPU limits:**

```bash
# CPU share/quota (quanto tempo CPU può usare)
docker run --rm -it --cpus=0.5 ubuntu bash          # 50% di 1 core
docker run --cpu-period=100000 --cpu-quota=50000 myapp   # equivalente a --cpus=0.5
docker run --cpu-period=100000 --cpu-quota=200000 myapp  # 2 core logici

# CPU affinity (su quali core fisici può girare)
docker run --cpuset-cpus="0" myapp           # solo core 0
docker run --cpuset-cpus="0,2" myapp         # core 0 e 2
docker run --cpuset-cpus="0-3" myapp         # core 0,1,2,3
docker run --cpuset-cpus="2,3" --cpus=0.5 myapp  # affinity + quota combinati

# Limitare RAM
docker run --rm -it --memory=50m ubuntu bash
docker run --memory=256m --memory-swap=512m myapp       # 256 MB RAM + 256 MB swap
docker run --memory=512m --memory-reservation=256m myapp # soft limit 256 MB

# Limitare processi
docker run --rm -it --pids-limit=10 ubuntu bash
```

> `--cpu-period` = durata della finestra di controllo (default 100.000 µs = 100 ms). `--cpu-quota` = tempo CPU disponibile in quel periodo. **OOM Killer**: se il container supera il limite RAM, il kernel termina il processo.

**In docker-compose.yml:**
```yaml
services:
  server:
    build: ./server
    mem_limit: 100m
    memswap_limit: 100m       # RAM+swap = 100m → nessuno swap
    memory_reservation: 50m   # soft limit (può essere superato)
    cpus: 0.2                 # 20% di un core
    cpu_shares: 1024          # peso relativo di scheduling (default 1024)
    pids_limit: 15
```

| Direttiva Compose | Significato |
|---|---|
| `mem_limit` | Limite rigido RAM (OOM Killer se superato) |
| `memswap_limit` | Limite RAM+Swap (uguale a `mem_limit` → no swap) |
| `memory_reservation` | Soft limit (il kernel prova a restare sotto) |
| `cpus` | Frazione di core (1.0 = 1 core, 0.5 = metà core) |
| `cpu_shares` | Priorità di scheduling — default 1024; peso relativo tra container |
| `pids_limit` | Max processi+thread nel container |


#### Suite di Verifica Sperimentale dei Limiti (`test-limiti`, Slide 1438–1440)
Per testare e verificare empiricamente l'efficacia dei vincoli cgroups imposti da Docker (`--cpus`, `--memory`, `--pids-limit`), nelle slide del corso vengono utilizzati tre programmi C mirati:

##### 1. Stress CPU (`test_cpu.c` / `cpu-eater.c`)
Ciclo infinito a computazione pura senza chiamate bloccanti. Se eseguito con `docker run --cpus 0.5`, il comando `docker stats` o `top` confermerà che il processo non supererà mai il 50% di utilizzo di un singolo core CPU:
```c
// test_cpu.c - Consumo intensivo CPU
#include <stdio.h>

int main(void) {
    unsigned long long x = 0;
    while (1) {
        x++; // Loop infinito su registro CPU
        if (x % 1000000000ULL == 0) {
            printf("[cpu-eater] x = %llu miliardi di cicli\n", x / 1000000000ULL);
        }
    }
    return 0;
}
```

##### 2. Stress RAM e Trigger OOM Killer (`test_ram.c` / `mem-eater.c`)
Alloca 1 MB per iterazione e vi scrive forzatamente con `memset()`. Se eseguito con `docker run --memory 100m`, al superamento della soglia il kernel interviene terminando bruscamente il container con stato `OOMKilled: true` (verificabile con `docker inspect`):
```c
// test_ram.c - Allocazione progressiva RAM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BLOCK_SIZE (1024 * 1024) // 1 MB

int main(void) {
    size_t m = 0;
    while (1) {
        char *p = (char *)malloc(BLOCK_SIZE);
        if (!p) {
            perror("malloc fallita");
            return 1;
        }
        // Fondamentale: memset forza il sistema operativo a mappare le pagine fisiche di RAM
        memset(p, 'A', BLOCK_SIZE);
        m++;
        printf("[mem-eater] Memoria allocata e popolata: %zu MB\n", m);
        usleep(100000); // Pausa di 100 ms tra un blocco e il successivo
    }
    return 0;
}
```

##### 3. Stress Numero di Processi (`test_pid.c` / `fork-bomber.c`)
Genera processi figli a catena senza attendere. Se eseguito con `docker run --pids-limit 10`, dopo la creazione del decimo processo la `fork()` fallisce ritornando `-1` (`EAGAIN: Resource temporarily unavailable`), impedendo il blocco totale del sistema host:
```c
// test_pid.c - Fork controllata per test vincolo pids-limit
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int count = 0;
    while (1) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("[fork-bomber] fork() bloccata dal limite cgroup pids.max");
            printf("[fork-bomber] Numero massimo di processi raggiunti: %d\n", count);
            sleep(10);
            return 1;
        }
        if (pid == 0) {
            // Processo figlio: rimane inattivo occupando uno slot PID
            sleep(1000);
            exit(0);
        }
        count++;
        printf("[fork-bomber] Processo figlio #%d creato (PID: %d)\n", count, pid);
    }
    return 0;
}
```


### 24.7 Volumi — Persistenza dei Dati

I container sono **effimeri**: al riavvio i dati vengono persi. I **volumi** rendono i dati persistenti.

**Tipi di storage:**

| Tipo | Descrizione | Uso tipico |
|------|-------------|-----------|
| **Bind Mount** | Collega una directory dell'host al container | Sviluppo (sincronizzazione codice) |
| **Named Volume** | Area gestita da Docker, indipendente dal filesystem host | Produzione (database, log) |

**Bind Mount:**
```bash
docker run -it --rm --mount type=bind,source=$(pwd)/demo,target=/data alpine sh
# oppure
docker run -it --rm -v $(pwd)/demo:/data alpine sh
```

**Named Volume:**
```bash
docker volume create mio_volume
docker run -it --mount type=volume,source=mio_volume,target=/data alpine sh
echo "test" > /data/file.txt
# Il file persiste anche dopo la rimozione del container

docker volume ls                # lista volumi
docker volume inspect mio_volume
docker volume rm mio_volume     # rimuove volume
docker volume prune             # rimuove volumi non usati
```

**In docker-compose.yml:**
```yaml
services:
  server:
    build: ./server
    volumes:
      - app_data:/app/config     # Named volume
      - ./log:/app/log           # Bind mount
volumes:
  app_data:                       # Dichiarazione globale
```

**Driver dei Volumi:**

Docker supporta driver diversi (default: `local`):

```yaml
# Volume NFS (file server di rete)
volumes:
  volume_nfs:
    driver: local
    driver_opts:
      type: nfs
      o: addr=192.168.1.20,rw
      device: ":/exports/data"
```

| Driver | Uso |
|--------|-----|
| `local` | Storage locale sul filesystem host (default) |
| `local` + NFS opts | Montaggio NFS (file server di rete) |
| `local` + CIFS opts | Condivisioni Windows (SMB) |
| Plugin cloud | AWS EFS, GCP Filestore, Azure Blob, ecc. |

**Comandi utili per i volumi:**
```bash
docker volume create mio_volume
docker volume ls
docker volume inspect mio_volume    # mostra MountPoint sul filesystem host
docker volume rm mio_volume         # rimuove il volume (anche con file interni)
docker volume prune                 # rimuove tutti i volumi non usati
docker compose down -v              # ferma e rimuove anche i volumi dichiarati
```

---

> **Fine della guida.** Questo documento copre **tutti** gli argomenti trattati nelle lezioni 1–32 del corso di Laboratorio di Sistemi Operativi, inclusi: Unix, file system, shell, comandi, grep/regex, scripting, sed/awk, funzioni bash, compilazione C/GCC, I/O di basso livello, processi, segnali, IPC (pipe/FIFO/mmap), thread, sincronizzazione, problemi classici, socket (locali/TCP/UDP), I/O multiplexing (select), broadcast/multicast, comandi di rete, DNS (getaddrinfo), virtualizzazione, container (namespace/cgroups/OverlayFS) e Docker.

---

---

## 25. Corpus dei File Pratici d'Esame: Debugging (_ERR), Template (_TODO) e Prove Parziali
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

Nelle lezioni del corso (in particolare *Lezioni 24, 25, 27 e 28*), il docente fornisce un ricco corpus di file pratici suddivisi in:
1. **File `_ERR.c` (Bug-Hunting):** Codici sorgente realistici che contengono errori tipici di concorrenza, deadlock o violazione di protocolli di rete. Lo studente deve individuare la riga incriminata, spiegare perché il programma si blocca o fallisce, e fornire la correzione.
2. **File `_TODO.c` e `_TODO.sh` (Template d'Esame):** Scheletri di programmi C e script Bash con parti mancanti contrassegnate da `// TODO` da completare durante la prova.
3. **Simulazione della Prova Parziale (Lezione 28):** La prova d'esame ufficiale proposta dal docente per la verifica intermedia al calcolatore.

Di seguito vengono riportati tutti i codici completi, le analisi dettagliate dei bug e le soluzioni risolte.

---

### 25.1 Esercizi di Bug-Hunting ("Trova e Correggi l'Errore" — File _ERR.c)

#### 25.1.1 Risveglio Spuro: `es_cond_wait_if_ERR.c` (Slide 1261)
Questo è uno degli errori concettuali più gravi e ricorrenti nelle prove d'esame sulla sincronizzazione thread.

**Codice con Errore (`es_cond_wait_if_ERR.c`):**
```c
// es_cond_wait_if_ERR.c - Trova e correggi l'errore!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
int ready = 0;

void* worker(void* arg) {
    int id = (int)(long)arg;
    pthread_mutex_lock(&m);

    // ERRORE CRITICO: uso di if invece di while!
    if (!ready)
        pthread_cond_wait(&c, &m);

    printf("Thread %d entra nella sezione critica\n", id);
    // Questo thread "consuma" la condizione e la rimette a 0
    ready = 0;

    pthread_mutex_unlock(&m);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, (void*)1);
    pthread_create(&t2, NULL, worker, (void*)2);

    sleep(1);  // Tempo per far posizionare entrambi i thread in attesa

    pthread_mutex_lock(&m);
    ready = 1;
    pthread_cond_broadcast(&c); // Risveglia ENTRAMBI i thread
    pthread_mutex_unlock(&m);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
```

**Analisi del Bug:**
1. Il `main` esegue `pthread_cond_broadcast(&c)`, risvegliando sia `t1` che `t2`.
2. Uno dei due thread (es. `t1`) acquisisce per primo il mutex `m`, esce dalla `pthread_cond_wait()`, stampa il messaggio ed esegue `ready = 0;`, consumando la condizione e rilasciando il mutex.
3. Il secondo thread (`t2`) acquisisce a sua volta il mutex ed esce dalla `pthread_cond_wait()`. **Poiché c'è un `if` anziché un `while`, `t2` non riverifica la condizione `ready`!** `t2` entra illegittimamente nella sezione critica anche se `ready == 0`, violando il predicato di sincronizzazione. Inoltre, lo standard POSIX ammette i cosiddetti *spurious wakeups* (risvegli senza segnale).

**Correzione:**
Sostituire tassativamente l'`if` con il ciclo `while`:
```c
// CORREZIONE:
while (!ready) {
    pthread_cond_wait(&c, &m);
}
```

---

#### 25.1.2 Deadlock da Doppio Lock Consecutivo: `es2_2_ERR.c` (Slide 1318)
Questo esercizio evidenzia il deadlock causato dalla riacquisizione dello stesso mutex non ricorsivo.

**Codice con Errore (`es2_2_ERR.c`):**
```c
// es2_2_ERR.c - Trova e correggi l'errore!
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
int ready = 0;

void* worker(void* arg) {
    pthread_mutex_lock(&m);
    while (!ready)
        pthread_cond_wait(&c, &m);
    printf("Worker: condizione soddisfatta (OK)\n");
    pthread_mutex_unlock(&m);
    return NULL;
}

int main(void) {
    pthread_t t;
    pthread_create(&t, NULL, worker, NULL);

    pthread_mutex_lock(&m);
    // Simula elaborazione
    sleep(2);

    // ERRORE 1: Tentativo di riacquisire lo stesso mutex già posseduto!
    pthread_mutex_lock(&m);

    ready = 1;
    pthread_cond_signal(&c);

    // ERRORE 2: join chiamato prima di rilasciare il mutex
    pthread_join(t, NULL);
    pthread_mutex_unlock(&m);
    return 0;
}
```

**Analisi del Bug:**
1. Il `main` invoca `pthread_mutex_lock(&m)`.
2. Dopo `sleep(2)`, il `main` invoca nuovamente `pthread_mutex_lock(&m)` sullo stesso mutex. I mutex POSIX di default (`PTHREAD_MUTEX_DEFAULT` o `NORMAL`) non sono ricorsivi: un thread che cerca di ribloccare un mutex che già possiede **va in deadlock con se stesso**, bloccandosi all'infinito!
3. Inoltre, anche rimuovendo il doppio lock, il `main` chiama `pthread_join(t, NULL)` **prima** di aver rilasciato il mutex con `pthread_mutex_unlock(&m)`. Il worker non potrà mai uscire dalla `pthread_cond_wait(&c, &m)` perché ha bisogno di riacquisire `m`, generando un secondo deadlock!

**Correzione:**
Eliminare il secondo lock nel `main` e sbloccare il mutex prima della `pthread_join`:
```c
// CORREZIONE NEL MAIN:
pthread_mutex_lock(&m);
sleep(2);
ready = 1;
pthread_cond_signal(&c);
pthread_mutex_unlock(&m); // Rilascia PRIMA del join!

pthread_join(t, NULL);
```

---

#### 25.1.3 Deadlock tra Mutex e Socket Bloccante: `es_client_merge_ERR.c` (Slide 1259–1266)
Un classico scenario da sistemi distribuiti: interazione errata tra primitive di concorrenza locale e chiamate di I/O di rete bloccanti.

**Codice con Errore (`es_client_merge_ERR.c`):**
```c
// es_client_merge_ERR.c - Trova e correggi l'errore!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

int sockfd;
struct sockaddr_in servaddr;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void* sender(void* arg) {
    const char *msg1 = "ciao";
    const char *msg2 = "mondo";

    pthread_mutex_lock(&mtx);
    sendto(sockfd, msg1, strlen(msg1), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("sender: inviato '%s'\n", msg1);
    sendto(sockfd, msg2, strlen(msg2), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("sender: inviato '%s'\n", msg2);
    pthread_mutex_unlock(&mtx);
    return NULL;
}

void* receiver(void* arg) {
    char buf[256];

    // ERRORE CRITICO: acquisisce il lock prima di una chiamata di rete bloccante!
    pthread_mutex_lock(&mtx);
    ssize_t n = recvfrom(sockfd, buf, sizeof(buf)-1, 0, NULL, NULL);
    if (n > 0) {
        buf[n] = '\0';
        printf("receiver: ricevuto <%s>\n", buf);
    }
    pthread_mutex_unlock(&mtx);
    return NULL;
}
```

**Analisi del Bug:**
1. Il thread `receiver` parte e acquisisce immediatamente `pthread_mutex_lock(&mtx)`.
2. `receiver` si blocca sulla system call `recvfrom()`, in attesa che il server risponda.
3. Il server risponde solo se riceve i messaggi dal client; tuttavia, il thread `sender` **non può inviare nulla** perché per fare la `sendto()` deve acquisire `pthread_mutex_lock(&mtx)`, che è detenuto da `receiver`!
4. **Deadlock per attesa circolare:** `sender` aspetta il mutex posseduto da `receiver`; `receiver` aspetta il pacchetto che solo `sender` può innescare. Il programma si congela per sempre.

**Correzione:**
I socket UDP sono full-duplex e thread-safe per operazioni simmetriche separate: la `recvfrom()` **non deve essere protetta dallo stesso mutex della trasmissione**, oppure il lock va limitato alla sola manipolazione di strutture dati condivise in memoria:
```c
// CORREZIONE NEL RECEIVER:
// La recvfrom deve avvenire SENZA detenere il lock del sender!
char buf[256];
ssize_t n = recvfrom(sockfd, buf, sizeof(buf)-1, 0, NULL, NULL);
if (n > 0) {
    buf[n] = '\0';
    pthread_mutex_lock(&mtx);
    printf("receiver: ricevuto <%s>\n", buf);
    pthread_mutex_unlock(&mtx);
}
```

---

### 25.2 Template d'Esame da Completare (File _TODO.c e _TODO.sh)

#### 25.2.1 Sincronizzazione a Cascata: `es_thread_cascata_TODO.c` (Slide 1267–1268)
**Consegna:** Tre thread $T_1$, $T_2$, $T_3$ devono eseguire in sequenza rigorosa: $T_1$ deve partire per primo; al termine deve svegliare $T_2$; al termine $T_2$ deve svegliare $T_3$.

**Soluzione Completa:**
```c
// es_thread_cascata_TODO.c - Risolto
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int stato = 1;  // 1 = turno di t1, 2 = turno di t2, 3 = turno di t3

void* thread1(void* arg) {
    pthread_mutex_lock(&mtx);
    while (stato != 1) {
        pthread_cond_wait(&cond, &mtx);
    }
    printf("[T1] Inizia -> elabora -> finisce\n");
    stato = 2; // Passa il testimone a T2
    pthread_cond_broadcast(&cond); // Sveglia gli altri thread
    pthread_mutex_unlock(&mtx);
    return NULL;
}

void* thread2(void* arg) {
    pthread_mutex_lock(&mtx);
    while (stato != 2) {
        pthread_cond_wait(&cond, &mtx);
    }
    printf("[T2] Inizia -> elabora -> finisce\n");
    stato = 3; // Passa il testimone a T3
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mtx);
    return NULL;
}

void* thread3(void* arg) {
    pthread_mutex_lock(&mtx);
    while (stato != 3) {
        pthread_cond_wait(&cond, &mtx);
    }
    printf("[T3] Inizia -> elabora -> cascata completata con successo!\n");
    pthread_mutex_unlock(&mtx);
    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;
    // Creazione in ordine sparso per verificare la robustezza della sincronizzazione
    pthread_create(&t3, NULL, thread3, NULL);
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);
    return 0;
}
```

---

#### 25.2.2 Multiplexing tra Pipe Anonima e Tastiera con `select()`: `es_pipe_select_TODO.c` (Slide 1262–1265)
**Consegna:** Il processo padre crea una pipe e un figlio. Il padre legge stringhe dall'utente o invia dati sulla pipe; il figlio usa `select()` per fare multiplexing tra input tastiera (`STDIN_FILENO = 0`) e la pipe anonima, gestendo la chiusura della pipe e terminando quando entrambi gli stream sono chiusi.

**Soluzione Completa:**
```c
// es_pipe_select_TODO.c - Risolto
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/wait.h>

#define BUF_SIZE 64

int main(void) {
    int pfd[2];
    if (pipe(pfd) == -1) { perror("pipe"); exit(1); }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0) {
        // === PROCESSO FIGLIO ===
        close(pfd[1]); // Chiude scrittura pipe
        int pipe_fd = pfd[0];
        int pipe_closed = 0;
        int stdin_closed = 0;
        char buf[BUF_SIZE];

        while (!pipe_closed || !stdin_closed) {
            fd_set readfds;
            FD_ZERO(&readfds);
            int maxfd = -1;

            if (!stdin_closed) {
                FD_SET(STDIN_FILENO, &readfds);
                if (STDIN_FILENO > maxfd) maxfd = STDIN_FILENO;
            }
            if (!pipe_closed) {
                FD_SET(pipe_fd, &readfds);
                if (pipe_fd > maxfd) maxfd = pipe_fd;
            }

            int ret = select(maxfd + 1, &readfds, NULL, NULL, NULL);
            if (ret < 0) { perror("select"); break; }

            // 1. Canale Pipe
            if (!pipe_closed && FD_ISSET(pipe_fd, &readfds)) {
                ssize_t n = read(pipe_fd, buf, sizeof(buf) - 1);
                if (n > 0) {
                    buf[n] = '\0';
                    printf("[Figlio da PIPE]: %s", buf);
                } else if (n == 0) {
                    printf("[Figlio] EOF su pipe: canale chiuso dal padre.\n");
                    close(pipe_fd);
                    pipe_closed = 1;
                }
            }

            // 2. Canale Tastiera (STDIN)
            if (!stdin_closed && FD_ISSET(STDIN_FILENO, &readfds)) {
                ssize_t n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
                if (n > 0) {
                    buf[n] = '\0';
                    printf("[Figlio da TASTIERA]: %s", buf);
                } else if (n == 0) {
                    printf("[Figlio] EOF su tastiera (Ctrl+D).\n");
                    stdin_closed = 1;
                }
            }
        }
        printf("[Figlio] Entrambi i canali chiusi: uscita regolare.\n");
        _exit(0);
    } else {
        // === PROCESSO PADRE ===
        close(pfd[0]); // Chiude lettura pipe
        const char *m1 = "Messaggio 1 dal padre\n";
        const char *m2 = "Messaggio 2 dal padre\n";
        sleep(1);
        write(pfd[1], m1, strlen(m1));
        sleep(2);
        write(pfd[1], m2, strlen(m2));
        close(pfd[1]); // Invia EOF al figlio sulla pipe
        wait(NULL);
    }
    return 0;
}
```

---

#### 25.2.3 Chat UDP Multicast con `select()`: `chat_multi_TODO.c` (Slide 1225–1228)
**Consegna:** Implementare un'applicazione di chat di gruppo distribuita che trasmette e riceve messaggi usando UDP Multicast (gruppo `239.255.0.1`, porta `12345`), monitorando contemporaneamente input tastiera e messaggi di rete con `select()`.

**Soluzione Completa:**
```c
// chat_multi_TODO.c - Risolto
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MCAST_GROUP "239.255.0.1"
#define MCAST_PORT 12345
#define BUF_SIZE 512

int main(void) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { perror("socket"); exit(1); }

    // Permette il riuso dell'indirizzo e della porta per più client sulla stessa macchina
    int reuse = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MCAST_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        exit(1);
    }

    // Unione al gruppo Multicast (IGMP join)
    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_GROUP);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        perror("setsockopt IP_ADD_MEMBERSHIP");
        close(sock);
        exit(1);
    }

    // Indirizzo di destinazione per le trasmissioni
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(MCAST_PORT);
    dest_addr.sin_addr.s_addr = inet_addr(MCAST_GROUP);

    printf("Connesso alla chat Multicast %s:%d. Digita un messaggio:\n", MCAST_GROUP, MCAST_PORT);

    char buf[BUF_SIZE];
    while (1) {
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        FD_SET(sock, &rfds);
        int maxfd = (sock > STDIN_FILENO) ? sock : STDIN_FILENO;

        int ret = select(maxfd + 1, &rfds, NULL, NULL, NULL);
        if (ret < 0) { perror("select"); break; }

        // Messaggio ricevuto dal gruppo
        if (FD_ISSET(sock, &rfds)) {
            struct sockaddr_in from;
            socklen_t fromlen = sizeof(from);
            ssize_t n = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr*)&from, &fromlen);
            if (n > 0) {
                buf[n] = '\0';
                printf("[%s:%d]: %s", inet_ntoa(from.sin_addr), ntohs(from.sin_port), buf);
            }
        }

        // Messaggio digitato dall'utente
        if (FD_ISSET(STDIN_FILENO, &rfds)) {
            if (fgets(buf, sizeof(buf), stdin) != NULL) {
                sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
            } else {
                printf("Uscita dalla chat.\n");
                break;
            }
        }
    }

    // Abbandono del gruppo multicast
    setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
    close(sock);
    return 0;
}
```

---

#### 25.2.4 Client WHOIS TCP su Porta 43: `whois_TODO.c` (Slide 1229–1230)
**Consegna:** Interrogare un server WHOIS (es. `whois.ripe.net` sulla porta TCP 43) per ottenere informazioni su un dominio, inviando la query formattata con `\r\n` e leggendo la risposta completa fino a EOF.

**Soluzione Completa:**
```c
// whois_TODO.c - Risolto
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define WHOIS_PORT "43"
#define DEFAULT_SERVER "whois.ripe.net"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <dominio/IP> [server_whois]\n", argv[1]);
        return 1;
    }
    const char *query = argv[1];
    const char *server = (argc >= 3) ? argv[2] : DEFAULT_SERVER;

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_STREAM;  // TCP

    int err = getaddrinfo(server, WHOIS_PORT, &hints, &res);
    if (err != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        return 1;
    }

    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd < 0) { perror("socket"); freeaddrinfo(res); return 1; }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
        perror("connect");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }
    freeaddrinfo(res);

    // Invio della query terminata da CRLF come previsto dal protocollo WHOIS
    char qbuf[256];
    snprintf(qbuf, sizeof(qbuf), "%s\r\n", query);
    write(sockfd, qbuf, strlen(qbuf));

    // Lettura ciclica della risposta fino a EOF
    char rbuff[1024];
    ssize_t n;
    while ((n = read(sockfd, rbuff, sizeof(rbuff) - 1)) > 0) {
        rbuff[n] = '\0';
        printf("%s", rbuff);
    }
    close(sockfd);
    return 0;
}
```

---

#### 25.2.5 Turni Rigidi tra Thread A e B: `es2_1_condvar_TODO.c` (Slide 1317)
**Consegna:** Coordinare due thread affinché stampino alternativamente `A` e `B` (`A B A B A B...`) per 5 turni esatti tramite mutex e condition variable.

**Soluzione Completa:**
```c
// es2_1_condvar_TODO.c - Risolto
#include <stdio.h>
#include <pthread.h>

#define NUM_TURNI 5

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
int turn = 0;   // 0 = tocca ad A, 1 = tocca a B

void* threadA(void* arg) {
    for (int i = 0; i < NUM_TURNI; i++) {
        pthread_mutex_lock(&m);
        while (turn != 0) {
            pthread_cond_wait(&c, &m);
        }
        printf("A\n");
        turn = 1; // Cede il turno a B
        pthread_cond_signal(&c);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

void* threadB(void* arg) {
    for (int i = 0; i < NUM_TURNI; i++) {
        pthread_mutex_lock(&m);
        while (turn != 1) {
            pthread_cond_wait(&c, &m);
        }
        printf("B\n");
        turn = 0; // Cede il turno ad A
        pthread_cond_signal(&c);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(void) {
    pthread_t tA, tB;
    pthread_create(&tA, NULL, threadA, NULL);
    pthread_create(&tB, NULL, threadB, NULL);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);

    pthread_mutex_destroy(&m);
    pthread_cond_destroy(&c);
    return 0;
}
```

---

#### 25.2.6 Script Bash d'Esame Svolti (`es_file_bash1/2/3_TODO.sh`)

##### Script 1: Monitoraggio File con Modifiche (`es_file_bash1_TODO.sh`, Slide 1269)
```bash
#!/bin/bash
# Monitora file .txt contenenti una stringa in una cartella
if [ $# -ne 2 ]; then
    echo "Uso: $0 <directory> <stringa>"
    exit 1
fi

DIR="$1"
STR="$2"

if [ ! -d "$DIR" ]; then
    echo "Errore: '$DIR' non e' una directory valida."
    exit 1
fi

old_count=-1
while true; do
    count=0
    for f in "$DIR"/*.txt; do
        if [ -f "$f" ]; then
            if grep -q "$STR" "$f"; then
                count=$((count + 1))
            fi
        fi
    done

    if [ "$count" -ne "$old_count" ]; then
        echo "[$(date +%T)] Trovati $count file .txt contenenti '$STR' (variazione rilevata!)"
        old_count=$count
    fi
    sleep 5
done
```

##### Script 2: Conteggio Thread LWP per Utente (`es_file_bash2_TODO.sh`, Slide 1270)
```bash
#!/bin/bash
if [ $# -ne 2 ]; then
    echo "Uso: $0 <utente> <soglia_LWP>"
    exit 1
fi

USER_NAME="$1"
THRESHOLD="$2"

ps -L -u "$USER_NAME" -o pid,lwp --no-headers | awk -v soglia="$THRESHOLD" -v user="$USER_NAME" '
BEGIN {
    proc_over = 0;
}
{
    pid = $1;
    count[pid]++; // Incrementa il numero di LWP per questo PID
}
END {
    printf("Processi dell utente %s con piu di %d LWP:\n\n", user, soglia);
    for (pid in count) {
        if (count[pid] > soglia) {
            printf("PID %-6d -> %3d LWP\n", pid, count[pid]);
            proc_over++;
        }
    }
    printf("\nTotale processi sopra soglia: %d\n", proc_over);
}'
```

##### Script 3: Sostituzione Dinamica con Sed (`es_file_bash3_TODO.sh`, Slide 1271)
```bash
#!/bin/bash
if [ $# -ne 1 ]; then
    echo "Uso: $0 <file>"
    exit 1
fi

FILE="$1"
if [ ! -f "$FILE" ]; then
    echo "Errore: file '$FILE' inesistente."
    exit 1
fi

BASENAME=$(basename "$FILE")
# Estrae prima lettera
FIRST=$(echo "$BASENAME" | cut -c1)
# Estrae ultima lettera (eliminando eventuale estensione o dall'intera stringa)
LAST=$(echo "$BASENAME" | sed 's/.*\(.\)$/\1/')

echo "Basename: $BASENAME, First: $FIRST, Last: $LAST"
# Sostituisce tutte le parole che iniziano per FIRST e finiscono per LAST con BASENAME
sed -i "s/\b${FIRST}[a-zA-Z0-9_]*${LAST}\b/$BASENAME/g" "$FILE"
```

---

### 25.3 Simulazione Ufficiale Prova Parziale (Lezione 28 — Prova PA)

Questa sezione riporta fedelmente la **simulazione ufficiale della prova parziale** erogata dal docente Alberto Finzi nella **Lezione 28** (*Slide 1322–1325*), con tutti i quesiti e le relative soluzioni complete.

#### Quesito 1 (Pipeline Bash — Processi e Thread LWP)
**Testo:** Scrivere una pipeline di comandi che stampi la lista di tutti gli utenti che hanno **almeno un processo con almeno 2 thread (LWP)**. Ogni utente deve comparire al massimo una volta nell'output.  
*Comandi utili:* `ps`, `awk`, `sort`.  
*Suggerimento:* Il comando `ps -eo user,nlwp` stampa l'utente e il numero di thread (`NLWP`).

**Soluzione:**
```bash
ps -eo user,nlwp --no-headers | awk '$2 >= 2 {print $1}' | sort -u
```
**Spiegazione:**
1. `ps -eo user,nlwp --no-headers`: Estrae le colonne relative allo username del proprietario e al numero di LWP del task.
2. `awk '$2 >= 2 {print $1}'`: Seleziona solo le righe dove la seconda colonna (`NLWP`) è maggiore o uguale a 2, stampando lo username (`$1`).
3. `sort -u`: Ordina ed elimina tutti i duplicati, garantendo che ogni utente compaia esattamente una sola volta.

---

#### Quesito 2 (Pipeline Filtri su Metadati File: `ls -l`)
**Testo:** Dato l'output di `ls -l`, visualizzare i nomi dei file che soddisfano **tutte** le seguenti condizioni:
a. Sono file regolari (non directory, non link, ecc.);  
b. Hanno estensione `.txt`;  
c. Sono leggibili sia dal proprietario (`user`) sia dal gruppo (`group`).

**Soluzione:**
```bash
ls -l | awk '/^-r..r/ && $9 ~ /\.txt$/ {print $9}'
```
In alternativa con combinazione di `grep`:
```bash
ls -l | grep '^-r..r' | grep '\.txt$' | awk '{print $9}'
```
**Spiegazione:**
- `^-r..r`:
  - `^` = inizio riga.
  - `-` = primo carattere per indicare file regolare (non `d` o `l`).
  - `r` = secondo carattere: permesso di lettura per il proprietario abilitato.
  - `..` = caratteri 3 e 4: scrittura ed esecuzione proprietario indifferenti.
  - `r` = quinto carattere: permesso di lettura per il gruppo abilitato.
- `$9 ~ /\.txt$/`: la nona colonna (nome file) deve terminare con `.txt`.

---

#### Quesito 3 (Sezione Critica con Limite di Concorrenza: Massimo 3 Thread)
**Testo:** Si vuole realizzare un meccanismo che permette a **non più di 3 thread alla volta** di entrare in una sezione critica. Sono definite le variabili globali:
```c
int dentro = 0; // Thread attualmente in sezione critica
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  c = PTHREAD_COND_INITIALIZER;
```
Completare le funzioni `inizio()` e `fine()`:
```c
void *worker(void *arg) {
    while (1) {
        inizio();
        sezione_critica();
        fine();
    }
}
```

**Soluzione:**
```c
void inizio(void) {
    pthread_mutex_lock(&m);
    /* B: attende finché dentro è già al valore massimo (3) */
    while (dentro >= 3) {
        pthread_cond_wait(&c, &m);
    }
    /* D: incrementa il numero di thread presenti */
    dentro++;
    pthread_mutex_unlock(&m);
}

void fine(void) {
    pthread_mutex_lock(&m);
    /* G: decrementa il numero di thread presenti */
    dentro--;
    /* H: sveglia uno dei thread in attesa */
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
}
```

---

#### Quesito 4 (Processi Figli su Pipe Distinte e Padre con `select()` fino a 10 Caratteri)
**Testo:** Due figli scrivono periodicamente un carattere su una propria pipe: il figlio 1 scrive `'A'` su `p1`, il figlio 2 scrive `'B'` su `p2`. Il padre usa `select()` per leggere dai due descrittori, conta quanti caratteri arrivano da ciascuna pipe, termina dopo aver ricevuto in totale 10 caratteri e stampa i conteggi. Aggiungere inoltre la gestione di pipe rotte nei figli con handler `SIGPIPE`.

**Soluzione Completa:**
```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <signal.h>

void sigpipe_handler(int signo) {
    (void)signo;
    const char msg[] = "[Figlio] Ricevuto SIGPIPE: pipe chiusa dal padre! Termino.\n";
    write(STDERR_FILENO, msg, sizeof(msg) - 1);
    _exit(1);
}

int main(void) {
    int p1[2], p2[2];
    char buf;
    int c1 = 0, c2 = 0;

    // Utilizzo di pipe2() o pipe()
    if (pipe(p1) == -1 || pipe(p2) == -1) { perror("pipe"); exit(1); }

    // FIGLIO 1
    if (fork() == 0) {
        signal(SIGPIPE, sigpipe_handler);
        close(p1[0]); // Chiude lettura p1
        close(p2[0]); close(p2[1]); // Non usa p2
        while (1) {
            if (write(p1[1], "A", 1) <= 0) break;
            usleep(50000);
        }
        _exit(0);
    }

    // FIGLIO 2
    if (fork() == 0) {
        signal(SIGPIPE, sigpipe_handler);
        close(p2[0]); // Chiude lettura p2
        close(p1[0]); close(p1[1]); // Non usa p1
        while (1) {
            if (write(p2[1], "B", 1) <= 0) break;
            usleep(50000);
        }
        _exit(0);
    }

    // PADRE
    close(p1[1]); close(p2[1]); // Chiude le estremità di scrittura

    fd_set set;
    int maxfd = (p1[0] > p2[0] ? p1[0] : p2[0]) + 1;

    while (c1 + c2 < 10) {
        FD_ZERO(&set);
        FD_SET(p1[0], &set);
        FD_SET(p2[0], &set);

        int ret = select(maxfd, &set, NULL, NULL, NULL);
        if (ret < 0) { perror("select"); break; }

        if (FD_ISSET(p1[0], &set)) {
            if (read(p1[0], &buf, 1) > 0) {
                c1++;
                printf("[Padre] Ricevuto '%c' da Pipe 1 (Tot p1: %d)\n", buf, c1);
            }
        }
        if (FD_ISSET(p2[0], &set)) {
            if (read(p2[0], &buf, 1) > 0) {
                c2++;
                printf("[Padre] Ricevuto '%c' da Pipe 2 (Tot p2: %d)\n", buf, c2);
            }
        }
    }

    printf("\nConteggio finale: pipe1 = %d, pipe2 = %d (Totale: %d)\n", c1, c2, c1 + c2);
    // Chiudendo le letture, i successivi write dei figli riceveranno SIGPIPE
    close(p1[0]); close(p2[0]);
    return 0;
}
```

---

#### Quesito 5 (Analisi Concorrente C: Fork e Multithreading)
**Testo:** Dato il frammento:
```c
#define NUM_THREADS 2
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
int v1 = 1, v2 = 5;

void *f1(void* param) {
    pthread_mutex_lock(&m);
    while (v1 != v2)
        pthread_cond_wait(&c, &m);
    v1 = 1 + v2;
    pthread_mutex_unlock(&m);
    return 0;
}

void *f2(void* param) {
    pthread_mutex_lock(&m);
    v2 = v1 + 1; v1++;
    pthread_mutex_unlock(&m);
    return 0;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int pid;
    pid = fork();
    if (pid != 0) {
        pthread_create(&threads[0], NULL, f1, NULL);
        pthread_create(&threads[1], NULL, f2, NULL);
        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
    }
    printf("%d %d\n", v1, v2);
    return 0;
}
```

**Quesiti e Risposte:**
a) **Quanti processi e quanti thread genera il frammento?**
- Processi: **2** (il processo padre e 1 processo figlio generato da `fork()`).
- Thread: **2 nuovi thread** generati tramite `pthread_create()` **esclusivamente nel processo padre** (poiché invocati dentro `if (pid != 0)`). In totale nel sistema vi sono 4 contesti di esecuzione (il main thread del figlio, e il main thread del padre + i suoi 2 thread `f1` e `f2`).

b) **Il programma termina correttamente? Segnalare e correggere eventuali anomalie.**
- **NO, il programma può andare in DEADLOCK (attesa indefinita).**
- *Motivo:* In `f1`, il thread si mette in attesa con `while (v1 != v2) pthread_cond_wait(&c, &m)`. Tuttavia, in `f2`, dopo aver modificato `v2 = v1 + 1; v1++;`, **non viene mai invocata `pthread_cond_signal(&c)` o `pthread_cond_broadcast(&c)`!** Se `f1` entra in `pthread_cond_wait()` prima che `f2` termini, non verrà mai risvegliato, bloccando la `pthread_join(threads[0], NULL)` all'infinito.
- *Inoltre:* Anche se `f2` inviasse la signal, `f2` imposta `v1 = 2` e `v2 = 2` (se parte con `v1=1`). Quindi la condizione `v1 == v2` sarebbe soddisfatta, ma senza signal non si propaga.
- *Correzione:* Aggiungere `pthread_cond_signal(&c)` in `f2` prima di rilasciare il mutex.

c) **Cosa stampa il programma?**
- **Il processo figlio (`pid == 0`):** Non entra nel blocco `if (pid != 0)`. Esegue direttamente la `printf("%d %d\n", v1, v2)` stampando i valori iniziali non modificati: **`1 5`**.
- **Il processo padre (`pid != 0`):** Se corretto con la signal:
  1. `f2` esegue: `v2 = 1 + 1 = 2`, `v1 = 2`, sblocca `f1`.
  2. `f1` si sveglia con `v1 == v2 == 2`, calcola `v1 = 1 + 2 = 3`.
  3. Il padre termina le join e stampa: **`3 2`**.

---

### 25.4 Traccia d'Esame Ufficiale Svolta — Compito A
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

Questa sezione riporta integralmente la traccia ufficiale d'esame del **Compito A** (presente nei documenti d'esame del corso), con tutti i 5 esercizi risolti e commentati passo-passo.

---

#### Esercizio 1 (Pipeline di Comandi Bash: `ps, awk, sort, uniq`)
**Testo:**
Scrivere una pipeline di comandi che stampi il nome degli utenti che hanno **almeno 2 processi in esecuzione da meno di 20 secondi**. Ogni utente deve comparire **una sola volta** nell'output.

*Comandi utili:* `ps, awk, sort, uniq`.  
*Suggerimento dalla traccia:* Il comando `ps -eo user,etimes` stampa per ogni processo l'utente (`USER`) e i secondi trascorsi dall'avvio (`ELAPSED` / `etimes`):
```text
ps -eo user,etimes
USER     ELAPSED
root     120
root     3600
alice    45
bob      900
alice    720
carlo    15
```

**Soluzione Canonica:**
```bash
ps -eo user,etimes | awk 'NR>1 && $2 < 20 {print $1}' | sort | uniq -c | awk '$1 >= 2 {print $2}'
```

**Soluzione Alternativa (con opzione `--no-headers`):**
```bash
ps -eo user,etimes --no-headers | awk '$2 < 20 {print $1}' | sort | uniq -c | awk '$1 >= 2 {print $2}'
```

**Spiegazione Dettagliata Passo-Passo:**
1. **`ps -eo user,etimes`**: Elenca tutti i processi attivi nel sistema estraendo solo due colonne: lo username dell'utente (`$1`) e i secondi trascorsi dall'avvio del processo (`$2`, `etimes`).
2. **`awk 'NR>1 && $2 < 20 {print $1}'`**:
   - `NR>1`: Ignora la prima riga di intestazione (`USER ELAPSED`).
   - `$2 < 20`: Filtra solo i processi avviati da strettamente meno di 20 secondi.
   - `{print $1}`: Per ogni processo valido, estrae e stampa esclusivamente il nome dell'utente.
3. **`sort`**: Ordina alfabeticamente i nomi degli utenti estratti. Questo passaggio è **obbligatorio**, poiché il comando `uniq` è in grado di raggruppare e conteggiare solo righe consecutive adiacenti.
4. **`uniq -c`**: Raggruppa i duplicati e antepone il conteggio delle occorrenze di ciascun utente (es. producendo un flusso come `   3 carlo`, `   1 alice`).
5. **`awk '$1 >= 2 {print $2}'`**: Ispeziona il conteggio generato da `uniq`: se la prima colonna (`$1`, il conteggio) è maggiore o uguale a 2, stampa la seconda colonna (`$2`, il nome dell'utente), garantendo che ogni utente compaia una sola volta.

---

#### Esercizio 2 (Pipeline Filtri su Metadati File: `ls -l`)
**Testo:**
Dato l'output del comando `ls -l`:
```text
-rw-r--r-- 1 student student   9200 Nov 12 10:10 relazione.txt
-rw-r----- 1 student student   4500 Nov 12 10:11 appunti.txt
-rw-rw-r-- 1 student student  12000 Nov 12 10:12 dati.txt
-rwxr-xr-x 1 student student   2100 Nov 12 10:13 script.sh
drwxr-xr-x 2 student student   4096 Nov 12 10:14 documenti
-r--r----- 1 student student   1800 Nov 12 10:15 note.txt
-rw-r--r-- 1 alice   staff      512 Nov 12 10:16 elenco.txt
lrwxrwxrwx 1 student student     15 Nov 12 10:17 link -> relazione.txt
```
Si vogliono visualizzare i nomi dei file che soddisfano **tutte** le seguenti condizioni:
a. Sono **file regolari**;  
b. Hanno **estensione `.sh`**;  
c. Hanno il **permesso di scrittura solo per il proprietario** (cioè il proprietario ha `w`, mentre gruppo e altri **non** hanno `w`).

Scrivere una pipeline di comandi per ottenere l'elenco dei soli nomi dei file che soddisfano queste condizioni.  
*Comandi utili:* `ls -l, grep, awk`.

**Soluzione con `grep` e `awk`:**
```bash
ls -l | grep '^-..w..[^w]..[^w]' | grep '\.sh$' | awk '{print $9}'
```

**Soluzione Diretta con solo `awk`:**
```bash
ls -l | awk '/^-..w..[^w]..[^w]/ && $9 ~ /\.sh$/ {print $9}'
```

**Spiegazione Dettagliata dei Permessi:**
La stringa dei permessi in `ls -l` è composta da 10 caratteri: `[tipo][user][group][other]`.
- **Condizione a (File regolare):** Il primo carattere deve essere un trattino `-` (si escludono directory `d` e link `l`). Nella regex: `^-`.
- **Condizione c (Scrittura SOLO per il proprietario):**
  - **User (caratteri 2, 3, 4):** Il permesso di scrittura per l'utente è il carattere 4 (il terzo della terna user). Deve essere presente (`w`). I permessi di lettura ed esecuzione possono essere qualsiasi (`..`). La terna user corrisponde a: `..w`.
  - **Group (caratteri 5, 6, 7):** Il permesso di scrittura per il gruppo è il carattere 6 (il secondo della terna group). **Non** deve essere presente (`-` oppure negato con `[^w]`). La terna group corrisponde a: `..[^w]` (oppure `. - .`).
  - **Other (caratteri 8, 9, 10):** Il permesso di scrittura per gli altri è il carattere 9 (il secondo della terna other). **Non** deve essere presente (`-` oppure negato con `[^w]`). La terna other corrisponde a: `..[^w]`.
  - Regex risultante completa sui primi 10 caratteri: `^-..w..[^w]..[^w]` (oppure `^-..w..-..-`).
- **Condizione b (Estensione `.sh`):** Il nome del file deve terminare con `.sh`. Con `grep` usiamo `grep '\.sh$'` oppure in `awk` verifichiamo la nona colonna con `$9 ~ /\.sh$/`.
- **Estrazione nome:** `awk '{print $9}'` isola e stampa solo la colonna del nome file.

---

#### Esercizio 3 (Processi, Thread e Sincronizzazione C: `x=5, y=3`)
**Testo del Programma:**
```c
#define NUM_THREADS 2
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
int x = 5, y = 3;

void *t1(void* param) {
    pthread_mutex_lock(&m);
    while (x <= y)
        pthread_cond_wait(&c, &m);
    y = x + y;
    pthread_mutex_unlock(&m);
    return 0;
}

void *t2(void* param) {
    pthread_mutex_lock(&m);
    x = y + 2;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
    return 0;
}

int main() {
    pthread_t th[NUM_THREADS];
    int pid;
    pid = fork();
    if (pid == 0) {
        x = 20;
        y = 30;
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

**Quesiti e Risposte Dettagliate:**

##### a) Quanti processi e quanti thread vengono creati? (specificare anche "per processo")
- **Processi totali nel sistema:** **2 processi**.
  - Il processo **Padre** (il processo originario).
  - **1 processo Figlio** generato dall'invocazione della system call `fork()`.
- **Thread creati:**
  - Vengono creati **2 nuovi thread** tramite `pthread_create()` (`th[0]` che esegue `t1` e `th[1]` che esegue `t2`).
  - **Questi 2 thread vengono creati ESCLUSIVAMENTE nel processo Padre**, poiché le chiamate a `pthread_create` si trovano all'interno del ramo `else` (eseguito solo quando `pid != 0`).
- **Dettaglio per processo:**
  - **Processo Figlio (`pid == 0`):** Ha **1 solo thread** (il main thread del figlio). Non crea alcun thread aggiuntivo.
  - **Processo Padre (`pid > 0`):** Ha **3 thread in totale** (il main thread del padre + i 2 worker thread `t1` e `t2`).
  - Totale contesti di esecuzione complessivi: **4 thread**.

##### b) Il programma termina correttamente? Segnalare e correggere eventuali anomalie.
- **Sì, il programma termina**, ma presenta un'**anomalia logica nella chiamata a `wait()`**:
  - L'istruzione `wait(NULL);` si trova all'esterno del blocco `if-else`, quindi viene eseguita **sia dal Padre che dal Figlio**.
  - Nel **Figlio**, che non ha generato alcun processo figlio a sua volta, la chiamata a `wait(NULL)` fallisce immediatamente restituendo `-1` e impostando la variabile globale `errno = ECHILD` (*No child processes*).
  - Nel **Padre**, `wait(NULL)` attende correttamente la terminazione del processo figlio.
  - **Correzione:** Spostare `wait(NULL);` all'interno del ramo `else` del padre, oppure racchiuderlo in un controllo esplicito:
    ```c
    if (pid > 0) {
        wait(NULL);
    }
    ```
- *Verifica Deadlock:* Nel padre **non si verifica deadlock**. Il padre imposta `x = y` ($x = 3, y = 3$). Se parte prima `t1`, trova `x <= y` ($3 \le 3$, vero) e si sospende rilasciando il lock. `t2` acquisisce il lock, imposta $x = 3 + 2 = 5$, invia la `signal` e sblocca il mutex. `t1` si risveglia, verifica $5 \le 3$ (falso), esce dal ciclo `while`, aggiorna $y$ e termina.

##### c) Cosa stampa il programma? (motivare)
Poiché sia il padre che il figlio raggiungono la `printf("%d %d\n", x, y);`, il programma produrrà **due righe di output**:

1. **Output del processo Figlio (`pid == 0`):**
   - Il figlio esegue il ramo `if (pid == 0)`, assegnando: `x = 20;` e `y = 30;`.
   - Non esegue alcun thread; la `wait(NULL)` ritorna subito con errore.
   - Stampa direttamente: **`20 30`**.

2. **Output del processo Padre (`pid > 0`):**
   - Nel ramo `else`, il padre esegue prima: `x = y;` $\implies x = 3, y = 3$ (ereditati dall'inizializzazione globale).
   - Crea `t1` e `t2`.
   - Il thread `t1` entra, acquisisce `m`, valuta `while (x <= y)`: poiché $3 \le 3$ è vero, si mette in attesa nella `pthread_cond_wait(&c, &m)` rilasciando il lock.
   - Il thread `t2` acquisisce `m`, calcola: `x = y + 2;` $\implies x = 3 + 2 = 5$.
   - `t2` chiama `pthread_cond_signal(&c)` e rilascia il lock con `pthread_mutex_unlock(&m)`.
   - Il thread `t1` viene risvegliato, riacquisisce il lock `m` e rivaluta il ciclo: ora $x = 5, y = 3$. La condizione $5 \le 3$ è **falsa**, quindi `t1` esce dal `while`.
   - `t1` calcola: `y = x + y;` $\implies y = 5 + 3 = 8$. Rilascia il lock e termina.
   - Il main del padre attende la fine dei due thread con `pthread_join`, attende il figlio con `wait(NULL)` e poi stampa: **`5 8`**.

3. **Ordine di stampa a schermo:**
   Dato che il padre attende esplicitamente la terminazione del figlio tramite `wait(NULL)`, il processo figlio stamperà **prima** del padre:
   ```text
   20 30
   5 8
   ```

---

#### Esercizio 4 (Pipe, Fork, Select, I/O con Buffer di Stringa)
**Testo del Programma:**
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <signal.h>
#include <stdlib.h>

int main(void) {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[0]);
        write(p[1], "ABCD", 4);
        close(p[1]);
        exit(0);
    }
    close(p[1]);

    char buff[3];
    int n1, n2;
    fd_set set;

    FD_ZERO(&set);
    FD_SET(p[0], &set);
    select(p[0] + 1, &set, NULL, NULL, NULL);
    n1 = read(p[0], buff, 2);
    buff[n1] = '\0';

    FD_ZERO(&set);
    FD_SET(p[0], &set);
    select(p[0] + 1, &set, NULL, NULL, NULL);
    n2 = read(p[0], buff, 2);
    buff[n2] = '\0';

    printf("buff=%s, n1=%d n2=%d\n", buff, n1, n2);
    close(p[0]);
    return 0;
}
```

**Quesiti e Risposte Dettagliate:**

##### 1. Le due select() del padre si bloccano oppure ritornano subito? Perché?
- **La prima `select()`:**
  - Si **blocca temporaneamente** fino a quando il processo figlio non scrive i dati nella pipe. Appena il figlio esegue `write(p[1], "ABCD", 4)`, i byte entrano nel buffer kernel della pipe; il descrittore `p[0]` diventa pronto in lettura e la `select()` si sblocca ritornando `1`.
- **La seconda `select()`:**
  - **Ritorna immediatamente senza bloccarsi**.
  - *Motivo:* Il figlio ha scritto 4 byte (`"ABCD"`). La prima `read()` ha consumato soltanto 2 byte (`"AB"`). All'interno del buffer della pipe rimangono ancora 2 byte non letti (`"CD"`). Poiché il buffer della pipe non è vuoto, il descrittore `p[0]` è già pronto per una nuova lettura immediata.

##### 2. Quanto valgono n1 e n2? E cosa stampa la printf finale?
- Il figlio inietta 4 byte nella pipe: `'A'`, `'B'`, `'C'`, `'D'`.
- La prima `read(p[0], buff, 2)` legge 2 byte (`"AB"`). Restituisce il numero di byte effettivamente letti: **`n1 = 2`**. Il buffer contiene `'A'`, `'B'`, `'\0'`.
- La seconda `read(p[0], buff, 2)` legge i restanti 2 byte (`"CD"`). Restituisce **`n2 = 2`**. Il buffer viene sovrascritto e contiene `'C'`, `'D'`, `'\0'`.
- La stampa finale `printf("buff=%s, n1=%d n2=%d\n", buff, n1, n2);` produce esattamente:
  ```text
  buff=CD, n1=2 n2=2
  ```

##### 3. Il processo figlio cosa fa dopo aver scritto "ABCD"? Il programma termina?
- Dopo la `write(p[1], "ABCD", 4)`, il processo figlio:
  1. Chiude la sua estremità di scrittura con `close(p[1]);`.
  2. Termina immediatamente la propria esecuzione invocando `exit(0);`.
- Il processo padre legge i dati, esegue la stampa, chiude `p[0]` e termina a sua volta con `return 0;`. L'intero programma **termina regolarmente**.

##### 4. C'è rischio di zombie?
- **Sì, c'è il rischio che il processo figlio rimanga in stato Zombie (`Z`).**
- *Motivo:* Il processo figlio termina con `exit(0)`, ma il processo padre **non invoca mai `wait()` o `waitpid()`** per raccoglierne il valore di uscita e rimuovere la sua voce (`task_struct`) dalla tabella dei processi del sistema operativo. Il figlio rimane quindi in stato zombie per tutto il tempo in cui il padre è in esecuzione.
- *Come evitarlo:*
  - Inserire una chiamata a `wait(NULL);` nel processo padre prima di `return 0;`.
  - Oppure impostare prima della fork l'azione del segnale `SIGCHLD` su ignorato: `signal(SIGCHLD, SIG_IGN);`, istruendo il kernel a deallocare automaticamente i figli terminati.

---

#### Esercizio 5 (Namespace e Container Docker)
**Testo dello Scenario:**
Si consideri un sistema Linux con hostname host su cui vengono eseguiti i seguenti comandi:
```bash
docker pull ubuntu
docker run -it ubuntu bash
```
All'interno del container vengono eseguite le seguenti operazioni:
```bash
ps -ef
hostname laboratorio
mkdir /esame
touch /esame/prova.txt
ls /
exit
```
Si osserva che:
- (a) Con `ps -ef` il processo `bash` ha **`PID = 1`**;
- (b) Il valore restituito da `hostname` è **diverso da quello dell'host**;
- (c) La directory `/esame` e il file `/esame/prova.txt` **non sono immediatamente visibili** nel filesystem dell'host;
- (d) Dopo `exit`, il processo `bash` del container **termina** e il container si arresta.

**Spiegazione Dettagliata dei Meccanismi di Isolamento:**

##### (a) PID Namespace (`PID = 1`)
Docker isola l'albero dei processi sfruttando la feature del kernel Linux denominata **PID Namespace** (`CLONE_NEWPID`).
- All'interno del namespace privato del container, la numerazione dei processi riparte da 1.
- Il comando `bash` avviato come entrypoint diventa il **PID 1** (il processo Init locale del container), che funge da radice e adotta gli eventuali orfani generati all'interno del container.
- Sull'host fisico, lo stesso processo possiede un normale PID globale (es. `PID = 14520`).

##### (b) UTS Namespace (`hostname`)
L'isolamento dei parametri di identificazione della macchina (nome host e nome di dominio NIS) è garantito dall'**UTS Namespace** (*UNIX Timesharing System*, flag `CLONE_NEWUTS`).
- Il container possiede una propria copia isolata della struttura `struct uts_namespace`.
- Quando all'interno del container si esegue `hostname laboratorio`, la modifica ha effetto **soltanto** all'interno dell'UTS namespace del container, lasciando completamente inalterato l'hostname reale della macchina host.

##### (c) Mount Namespace e OverlayFS (Filesystem a Strati)
L'isolamento dei file e delle cartelle create è dovuto all'interazione tra il **Mount Namespace** (`CLONE_NEWNS`) e il driver di storage **OverlayFS**:
- Il filesystem visibile nel container è il risultato della sovrapposizione di più livelli:
  1. **`lowerdir` (Immagine base Ubuntu):** Uno o più strati a **sola lettura** condivisi.
  2. **`upperdir` (Container layer):** Un livello a **lettura e scrittura privato**, isolato ed effimero, assegnato specificamente a quella singola istanza di container.
- Quando si eseguono `mkdir /esame` e `touch /esame/prova.txt`, i nuovi file vengono scritti fisicamente all'interno dell'`upperdir` del container (memorizzata in un percorso interno di Docker come `/var/lib/docker/overlay2/<id>/diff/`).
- La radice `/` dell'host non viene in alcun modo toccata perché il container opera in un proprio albero di mount separato.

##### (d) Ciclo di Vita del Container e Morte di PID 1
Nei sistemi Linux, il processo con **PID 1 è il supervisore vitale del namespace**:
- Quando l'utente digita `exit` nella shell interattiva, il processo `bash` (che è PID 1) termina.
- Per le regole del kernel Linux sui PID Namespace, la morte del processo PID 1 provoca automaticamente l'invio del segnale `SIGKILL` a tutti gli altri processi presenti nel namespace e la distruzione del namespace stesso.
- Il container passa dallo stato *Running* allo stato *Exited* (arrestato).

---

### 25.5 Traccia d'Esame Ufficiale Svolta — Compito B
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

Questa sezione riporta integralmente la traccia ufficiale d'esame del **Compito B** (presente nei documenti d'esame del corso), con tutti i 5 esercizi risolti e commentati passo-passo.

---

#### Esercizio 1 (Pipeline di Comandi: Processi Sleeping e Secondi Trascorsi)
**Testo:**
Scrivere una pipeline di comandi che stampi il nome degli utenti che hanno **almeno 2 processi attivi** che soddisfano **entrambe** le seguenti condizioni:
1. Il processo si trova nello stato *sleeping*, cioè il campo `STAT` **inizia con `S`** (es. `S`, `Ss`, `Sl`);
2. Il processo è stato avviato da **meno di 90 secondi** (`etimes < 90`).

Ogni utente deve comparire **una sola volta** nell'output.  
*Comandi utili:* `ps, awk, sort, uniq`.

Il comando `ps -eo user=,stat=,etimes=` può produrre per esempio:
```text
root     Ss   400
alice    S    45
bob      R    12
alice    Sl   70
carlo    S    130
bob      S    25
bob      S    60
student  Z    15
```
Nell'esempio, l'output atteso è:
```text
alice
bob
```

**Soluzione Canonica:**
```bash
ps -eo user=,stat=,etimes= | awk '$2 ~ /^S/ && $3 < 90 {print $1}' | sort | uniq -c | awk '$1 >= 2 {print $2}'
```

**Soluzione con `ps -eo user,stat,etimes` standard (gestendo l'header):**
```bash
ps -eo user,stat,etimes | awk 'NR>1 && $2 ~ /^S/ && $3 < 90 {print $1}' | sort | uniq -c | awk '$1 >= 2 {print $2}'
```

**Spiegazione Dettagliata:**
1. **`ps -eo user=,stat=,etimes=`**: Estrae le colonne Utente (`$1`), Stato del processo (`$2`) e Secondi trascorsi dall'avvio (`$3`). I segni `=` dopo i nomi dei campi sopprimono la riga di intestazione.
2. **`awk '$2 ~ /^S/ && $3 < 90 {print $1}'`**:
   - `$2 ~ /^S/`: Tramite espressione regolare, verifica che la stringa dello stato inizi con la lettera maiuscola `S` (include `S`, `Ss`, `Sl`, `S+`, ecc., escludendo stati come `R` (running) o `Z` (zombie)).
   - `$3 < 90`: Verifica che il tempo di vita sia inferiore a 90 secondi.
   - Per ogni riga conforme stampa il nome utente `$1`.
3. **`sort | uniq -c`**: Ordina i nomi e calcola la frequenza di occorrenza per ciascun utente.
4. **`awk '$1 >= 2 {print $2}'`**: Isola e stampa il nome dell'utente (`$2`) solo se il conteggio (`$1`) è $\ge 2$.

---

#### Esercizio 2 (Pipeline Filtri su Metadati File: `ls -l` con Dimensione)
**Testo:**
Dato l'output del comando `ls -l`:
```text
-rw-r----- 1 student student 6400 Jul 15 10:10 errori.log
-rw-rw-r-- 1 student student 9200 Jul 15 10:11 debug.log
-rw-r---rw- 1 student student 5300 Jul 15 10:12 condiviso.log
-rw------- 1 student student 8000 Jul 15 10:13 privato.log
-rw-r----- 1 student student 1500 Jul 15 10:14 breve.log
-rwxr-xr-x 1 student student 4100 Jul 15 10:15 avvio.sh
drwxr-xr-x 2 student student 4096 Jul 15 10:16 archivio.log
-rw-r----- 1 alice   staff   7200 Jul 15 10:17 sistema.txt
lrwxrwxrwx 1 student student   10 Jul 15 10:18 ultimo.log -> errori.log
```
Scrivere una pipeline di comandi che visualizzi i nomi dei file che soddisfano **tutte** le seguenti condizioni:
1. Sono **file regolari**;
2. Hanno **estensione `.log`**;
3. Hanno il **permesso di lettura per il gruppo**;
4. **Non** hanno il permesso di scrittura per gli altri utenti;
5. Hanno dimensione **maggiore di 3000 byte**.

L'output deve contenere solamente i nomi dei file.  
*Comandi utili:* `ls -l, grep, awk`.

**Soluzione con solo `awk`:**
```bash
ls -l | awk '/^-...r...[^w]/ && $5 > 3000 && $9 ~ /\.log$/ {print $9}'
```

**Soluzione Combinata con `grep` e `awk`:**
```bash
ls -l | grep '^-...r...[^w]' | awk '$5 > 3000 && $9 ~ /\.log$/ {print $9}'
```

**Spiegazione Dettagliata:**
- `^-`: Posizione 1 della riga: trattino indicante **file regolare** (esclude directory `d` come `archivio.log` e link simbolici `l` come `ultimo.log`).
- `...`: Posizioni 2, 3, 4: permessi utente proprietario (indifferenti).
- `r`: Posizione 5: primo carattere della terna group, impone il **permesso di lettura per il gruppo**.
- `...`: Posizioni 6, 7, 8: permessi scrittura/esecuzione gruppo e lettura altri.
- `[^w]`: Posizione 9: secondo carattere della terna others, impone che il permesso di scrittura per altri **non sia `w`** (deve essere `-`).
- `$5 > 3000`: La quinta colonna in `ls -l` rappresenta la dimensione logica in byte; esclude file come `breve.log` (1500 byte).
- `$9 ~ /\.log$/`: La nona colonna deve terminare con l'estensione `.log`; esclude file come `sistema.txt` e `avvio.sh`.
- Output prodotto sui dati d'esempio: `errori.log`, `debug.log`.

---

#### Esercizio 3 (Processi, Thread e Sincronizzazione C: `x=9, y=3`)
**Testo del Programma:**
```c
#define NUM_THREADS 2
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
int x = 9, y = 3;

void *t1(void* param) {
    pthread_mutex_lock(&m);
    while (x <= y)
        pthread_cond_wait(&c, &m);
    x = x - y;
    pthread_mutex_unlock(&m);
    return 0;
}

void *t2(void* param) {
    pthread_mutex_lock(&m);
    x = y + 4;
    pthread_cond_signal(&c);
    pthread_mutex_unlock(&m);
    return 0;
}

int main() {
    pthread_t th[NUM_THREADS];
    int pid;
    pid = fork();
    if (pid == 0) {
        x = 50;
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

**Quesiti e Risposte Dettagliate:**

##### a) Quanti processi e quanti thread vengono creati? (specificare anche "per processo")
- **Processi:** **2 processi in totale** (il processo **Padre** e **1 processo Figlio** generato da `fork()`).
- **Thread:** **2 nuovi thread** (`th[0]` e `th[1]`) generati **esclusivamente all'interno del processo Padre** (poiché nel blocco `else` dove `pid != 0`).
- **Per processo:**
  - Nel processo **Figlio**: **1 thread** (il solo thread principale `main`).
  - Nel processo **Padre**: **3 thread** (il thread principale `main` + i due worker `t1` e `t2`).

##### b) Il programma termina correttamente? Segnalare e correggere eventuali anomalie.
- **Sì, il programma termina.**
- **Anomalie:**
  1. `wait(NULL);` viene eseguita anche dal processo Figlio. Poiché il figlio non ha figli a sua volta, la `wait()` fallisce immediatamente restituendo `-1` con `errno = ECHILD`.  
     *Correzione:* Spostare la chiamata dentro l'else del padre o racchiuderla in `if (pid > 0) wait(NULL);`.
  2. *Sincronizzazione robusta:* Se nel padre `t2` esegue prima di `t1`, imposta $x = y + 4 = 3 + 4 = 7$. Quando `t1` parte, trova $x \le y$ ($7 \le 3$, falso) e non si blocca nella condition wait, eseguendo subito $x = 7 - 3 = 4$. Se parte prima `t1`, trova $3 \le 3$ (vero) $	o$ si sospende in `pthread_cond_wait`, viene risvegliato dalla signal di `t2` e calcola ugualmente $x = 7 - 3 = 4$. L'esecuzione nel padre è quindi deterministica e priva di deadlock.

##### c) Cosa stampa il programma? (motivare)
Entrambi i processi eseguono la `printf("%d %d\n", x, y);`:
1. **Output del processo Figlio (`pid == 0`):**
   - Esegue il ramo `if`: `x = 50; y = 20;`.
   - Nessun thread modifica le variabili nel figlio.
   - Stampa: **`50 20`**.
2. **Output del processo Padre (`pid > 0`):**
   - Inizializza: `x = y;` $\implies x = 3, y = 3$.
   - Thread `t1`: valuta $x \le y$ ($3 \le 3$, vero) $\implies$ si blocca nella condition wait rilasciando il mutex `m`.
   - Thread `t2`: acquisisce `m`, calcola `x = y + 4;` $\implies x = 3 + 4 = 7$. Invia `pthread_cond_signal(&c)` e rilascia `m`.
   - Thread `t1`: si risveglia riacquisendo `m`, valuta $x \le y$ ($7 \le 3$, falso) $\implies$ esce dal ciclo `while`.
   - `t1` calcola: `x = x - y;` $\implies x = 7 - 3 = 4$. Rilascia `m`.
   - Il main del padre esegue le join, attende il figlio e stampa: **`4 3`**.
3. **Sequenza complessiva:**
   ```text
   50 20
   4 3
   ```

---

#### Esercizio 4 (Pipe, Fork, Select in C: 3 read e Condizione di EOF)
**Testo del Programma:**
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <signal.h>
#include <stdlib.h>

int main(void) {
    int p[2];
    pipe(p);

    if (fork() == 0) {
        close(p[0]);
        write(p[1], "XYZ", 3);
        close(p[1]);
        exit(0);
    }
    close(p[1]);

    char buff[3];
    int n1, n2, n3;
    fd_set set;

    FD_ZERO(&set);
    FD_SET(p[0], &set);
    select(p[0] + 1, &set, NULL, NULL, NULL);
    n1 = read(p[0], buff, 2);

    FD_ZERO(&set);
    FD_SET(p[0], &set);
    select(p[0] + 1, &set, NULL, NULL, NULL);
    n2 = read(p[0], buff, 2);

    FD_ZERO(&set);
    FD_SET(p[0], &set);
    select(p[0] + 1, &set, NULL, NULL, NULL);
    n3 = read(p[0], buff, 2);

    printf("n1=%d n2=%d, n3=%d\n", n1, n2, n3);
    close(p[0]);
    return 0;
}
```

**Quesiti e Risposte Dettagliate:**

##### 1. Le tre select() del padre si bloccano oppure ritornano subito? Perché?
- **La prima `select()`:**
  - Si **blocca** fino a quando il processo figlio non scrive i dati nella pipe. Quando il figlio invoca `write(p[1], "XYZ", 3)`, la pipe diventa leggibile e la `select()` ritorna.
- **La seconda `select()`:**
  - **Ritorna subito senza bloccarsi**, poiché nella pipe è rimasto ancora 1 byte non letto (`'Z'`). Il descrittore è pronto in lettura.
- **La terza `select()`:**
  - **Ritorna subito anch'essa senza bloccarsi!**
  - *Motivo fondamentale (domanda classica d'esame):* Il processo figlio ha chiuso la sua estremità di scrittura con `close(p[1]);` ed è terminato. Il padre ha già chiuso `p[1]` all'inizio. **Quando tutte le estremità di scrittura di una pipe sono chiuse, la pipe viene considerata dal kernel come pronta in lettura per notificare la condizione di End-Of-File (EOF)!** Di conseguenza, `select()` non attende e ritorna immediatamente pronta.

##### 2. Quanto valgono n1, n2, n3?
- Il figlio inietta 3 byte (`"XYZ"`).
- 1ª `read(p[0], buff, 2)`: legge 2 byte (`"XY"`). **`n1 = 2`**.
- 2ª `read(p[0], buff, 2)`: legge il restante 1 byte (`"Z"`). **`n2 = 1`**.
- 3ª `read(p[0], buff, 2)`: il buffer della pipe è completamente vuoto e tutti i descrittori di scrittura sono chiusi. La `read()` rileva la fine del file e ritorna **`n3 = 0`**.
- Output stampato:
  ```text
  n1=2 n2=1, n3=0
  ```

##### 3. Che cosa indica il valore restituito dalla terza read()? Il programma termina correttamente?
- Il valore `n3 = 0` indica inequivocabilmente la condizione di **End-Of-File (EOF)**: la pipe è chiusa e non vi sono ulteriori dati da leggere.
- Il programma termina correttamente e senza blocchi.

##### 4. C'è il rischio che il processo figlio rimanga temporaneamente in stato zombie? In caso affermativo, indicare come modificare il programma per evitarlo.
- **Sì, il figlio diventa zombie.**
- Il processo figlio termina la sua esecuzione con `exit(0)`, ma il processo padre non esegue alcuna chiamata a `wait()` o `waitpid()`. Il processo figlio rimane registrato come zombie (`Z`) nella tabella dei processi fino a quando il padre non conclude l'esecuzione con `return 0;`.
- *Modifica correttiva:* Aggiungere `wait(NULL);` prima della chiusura del main nel padre, oppure impostare `signal(SIGCHLD, SIG_IGN);`.

---

#### Esercizio 5 (Namespace e Container Docker: Due Container Separati)
**Testo dello Scenario:**
Si consideri un sistema Linux con hostname `MioPC` su cui vengono eseguiti i seguenti comandi:
```bash
docker pull ubuntu
docker run -it --name contenitore1 --hostname nodo1 ubuntu bash
```
All'interno del container vengono eseguite le operazioni:
```bash
ps -ef
hostname
mkdir /lavoro
echo "prova 1" > /lavoro/info.txt
exit
```
Successivamente viene creato e avviato un **secondo container**:
```bash
docker run -it --name contenitore2 --hostname nodo2 ubuntu bash
```
Nel nuovo container:
- (a) Quale PID avrà il processo `bash` visualizzato con `ps -ef`?
- (b) Quale valore restituirà il comando `hostname`?
- (c) Saranno presenti `/lavoro` e `/lavoro/info.txt`?

Spiegare brevemente il comportamento illustrato discutendo i namespace coinvolti e l'Overlay File System.

**Risposte Dettagliate e Spiegazione dei Concetti:**

##### (a) PID del processo bash in `contenitore2`: `PID = 1`
- Il comando `docker run` genera una nuova istanza completamente isolata invocando `clone()` con il flag `CLONE_NEWPID`.
- Ogni container possiede un proprio **PID Namespace indipendente**: all'interno di questo namespace, il primo processo creato (il comando di avvio `/bin/bash`) assume sempre e invariabilmente il **`PID = 1`**.

##### (b) Valore restituito dal comando `hostname`: `nodo2`
- Docker isola il nome della macchina tramite l'**UTS Namespace** (`CLONE_NEWUTS`).
- L'opzione `--hostname nodo2` passata a `docker run` assegna la stringa `nodo2` alla struttura nodename del nuovo UTS namespace. Il comando `hostname` all'interno di `contenitore2` restituirà quindi esattamente **`nodo2`**, mentre all'interno di `contenitore1` restituiva `nodo1` e sull'host fisico restituisce `MioPC`.

##### (c) Presenza di `/lavoro` e `/lavoro/info.txt` in `contenitore2`: NO, TOTALMENTE ASSENTI
- I file creati in `contenitore1` **non saranno in alcun modo visibili** in `contenitore2`.
- **Spiegazione con l'architettura a livelli di OverlayFS:**
  - Entrambi i container condividono la stessa immagine base `ubuntu`, che risiede nel layer a sola lettura (**`lowerdir`**).
  - Tuttavia, all'atto dell'avvio di un container, Docker alloca un **nuovo layer di lettura e scrittura privato ed esclusivo (`upperdir`)** associato unicamente a quel container.
  - La creazione della cartella `/lavoro` e del file `info.txt` effettuata in `contenitore1` è stata registrata **soltanto nell'`upperdir` di `contenitore1`**.
  - Quando viene avviato `contenitore2`, Docker gli assegna una `upperdir` **nuova, vuota e completamente scorrelata**.
  - Poiché `/lavoro` non faceva parte dell'immagine base originaria (`lowerdir`), `contenitore2` vedrà unicamente il filesystem immutato di Ubuntu pulito.

---

## 26. Guida Rapida alle Parole Chiave
<div align="right"><em><a href="#indice">Torna all'indice</a></em></div>

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
* **Sort / Uniq**: `sort` ordina le righe di un flusso testuale; `uniq` filtra o conta (`-c`) le righe duplicate adiacenti (richiede quasi sempre `sort` preventivo).
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


---


---

## Indice Dettagliato delle Sottosezioni


### Indice delle Sottosezioni Capitolo 0


- [0.1 Obiettivi del Corso](#01-obiettivi-del-corso)


- [0.2 Modalità d'Esame e Criteri di Valutazione](#02-modalità-desame-e-criteri-di-valutazione)


- [0.3 Prove Parziali vs Appelli Ordinari](#03-prove-parziali-vs-appelli-ordinari)


- [0.4 Ambiente di Sviluppo di Riferimento](#04-ambiente-di-sviluppo-di-riferimento)


### Indice delle Sottosezioni Capitolo 1


- [1.1 Cos'è un Sistema Operativo](#11-cos-un-sistema-operativo)


- [1.2 Il Kernel](#12-il-kernel)


- [1.3 Storia di Unix](#13-storia-di-unix)


- [1.4 Architettura di Sistema Unix](#14-architettura-di-sistema-unix)


- [1.5 System Call e Libreria Standard C](#15-system-call-e-libreria-standard-c)


- [1.6 Sistema Multi-utente](#16-sistema-multi-utente)


### Indice delle Sottosezioni Capitolo 2


- [2.1 Caratteristiche Generali](#21-caratteristiche-generali)


- [2.2 Tipi di File](#22-tipi-di-file)


- [2.3 i-node e Metadati](#23-i-node-e-metadati)


- [2.4 Directory](#24-directory)


- [2.5 Pathname](#25-pathname)


- [2.6 Directory Tipiche](#26-directory-tipiche)


- [2.7 Home e Working Directory](#27-home-e-working-directory)


- [2.8 Permessi dei File](#28-permessi-dei-file)


- [2.9 Link](#29-link)


- [2.10 File System Montabile](#210-file-system-montabile)


- [2.11 Comando `stat`](#211-comando-stat)


### Indice delle Sottosezioni Capitolo 3


- [3.1 Cos'è la Shell](#31-cos-la-shell)


- [3.2 File di Inizializzazione](#32-file-di-inizializzazione)


- [3.3 Variabili di Shell](#33-variabili-di-shell)


- [3.4 Formato dei Comandi](#34-formato-dei-comandi)


- [3.5 Redirezione I/O](#35-redirezione-io)


- [3.6 Pipe](#36-pipe)


- [3.7 Metacaratteri e Wildcard](#37-metacaratteri-e-wildcard)


- [3.8 Quoting](#38-quoting)


### Indice delle Sottosezioni Capitolo 4


- [4.1 Gestione Directory](#41-gestione-directory)


- [4.2 Gestione File](#42-gestione-file)


- [4.3 Comandi di Utilità su Testo](#43-comandi-di-utilit-su-testo)


- [4.4 Listing di Processi](#44-listing-di-processi)


- [4.5 Il file `/etc/passwd`](#45-il-file-etcpasswd)


### Indice delle Sottosezioni Capitolo 5


- [5.1 Il Comando `grep`](#51-il-comando-grep)


- [5.2 Espressioni Regolari Base (BRE)](#52-espressioni-regolari-base-bre)


- [5.3 Espressioni Regolari Estese (ERE)](#53-espressioni-regolari-estese-ere)


- [5.4 Esempi Pratici](#54-esempi-pratici)


### Indice delle Sottosezioni Capitolo 6


- [6.1 Struttura Base](#61-struttura-base)


- [6.2 Variabili Predefinite negli Script](#62-variabili-predefinite-negli-script)


- [6.3 Exit Status](#63-exit-status)


- [6.4 Operatori su Comandi](#64-operatori-su-comandi)


- [6.5 Strutture di Controllo](#65-strutture-di-controllo)


- [6.6 Sostituzione Aritmetica](#66-sostituzione-aritmetica)


### Indice delle Sottosezioni Capitolo 7


- [7.1 Sed — Stream Editor](#71-sed--stream-editor)


- [7.2 Awk — Linguaggio di Elaborazione Testuale](#72-awk--linguaggio-di-elaborazione-testuale)


### Indice delle Sottosezioni Capitolo 8


- [8.1 Definizione e Chiamata](#81-definizione-e-chiamata)


- [8.2 Parametri Interni](#82-parametri-interni)


- [8.3 Esempi](#83-esempi)


### Indice delle Sottosezioni Capitolo 9


- [9.0 Basi di Programmazione C (vs Java)](#90-basi-di-programmazione-c-vs-java)


- [9.1 Il Compilatore GCC](#91-il-compilatore-gcc)


- [9.2 Opzioni di GCC](#92-opzioni-di-gcc)


- [9.3 Debug con GDB](#93-debug-con-gdb)


- [9.4 Funzione Main in C](#94-funzione-main-in-c)


- [9.5 Layout di Memoria di un Processo](#95-layout-di-memoria-di-un-processo)


- [9.6 Formato ELF e Linking](#96-formato-elf-e-linking)


### Indice delle Sottosezioni Capitolo 10


- [10.1 Concetti Fondamentali](#101-concetti-fondamentali)


- [10.2 Le Cinque System Call Fondamentali](#102-le-cinque-system-call-fondamentali)


- [10.3 Offset e `lseek`](#103-offset-e-lseek)


- [10.4 Gestione Errori con `errno` e `perror`](#104-gestione-errori-con-errno-e-perror)


- [10.5 Implementazione nel Kernel](#105-implementazione-nel-kernel)


- [10.6 Duplicazione File Descriptor — `dup` e `dup2`](#106-duplicazione-file-descriptor--dup-e-dup2)


- [10.7 Struttura `stat`](#107-struttura-stat)


- [10.8 Esempio Completo: Copia tra File](#108-esempio-completo-copia-tra-file)


### Indice delle Sottosezioni Capitolo 11


- [11.1 Concetti Fondamentali](#111-concetti-fondamentali)


- [11.2 Ottenere PID](#112-ottenere-pid)


- [11.3 Creazione di Processi — `fork()`](#113-creazione-di-processi--fork)


- [11.3.1 `vfork()`](#1131-vfork)


- [11.4 Terminazione di Processi](#114-terminazione-di-processi)


- [11.5 Processi Zombie e Orfani](#115-processi-zombie-e-orfani)


- [11.6 `wait()` e `waitpid()`](#116-wait-e-waitpid)


- [11.7 La Famiglia `exec`](#117-la-famiglia-exec)


- [11.9 La funzione `system()`](#119-la-funzione-system)


- [11.10 Ambiente di un Processo: `getenv` e `putenv`](#1110-ambiente-di-un-processo-getenv-e-putenv)


- [11.11 `chdir` e `chroot` — Cambiare directory e root](#1111-chdir-e-chroot--cambiare-directory-e-root)


### Indice delle Sottosezioni Capitolo 12


- [12.1 Cos'è un Segnale](#121-cos-un-segnale)


- [12.2 Segnali Principali](#122-segnali-principali)


- [12.3 Azioni Possibili](#123-azioni-possibili)


- [12.4 Catturare un Segnale — `signal()`](#124-catturare-un-segnale--signal)


- [12.4.1 Ignorare un Segnale — `signal()`](#1241-ignorare-un-segnale--signal)


- [12.4.2 Azione di Default — `signal()`](#1242-azione-di-default--signal)


- [12.5 Inviare Segnali](#125-inviare-segnali)


- [12.6 Alarm — Sveglia](#126-alarm--sveglia)


- [12.7 Insiemi di Segnali e Maschere](#127-insiemi-di-segnali-e-maschere)


- [12.8 Gestione Moderna dei Segnali — `sigaction()`](#128-gestione-moderna-dei-segnali--sigaction)


### Indice delle Sottosezioni Capitolo 13


- [13.1 Pipe Ordinarie](#131-pipe-ordinarie)


- [13.2 Pipe con Nome (FIFO)](#132-pipe-con-nome-fifo)


- [13.3 Memoria Condivisa con `mmap`](#133-memoria-condivisa-con-mmap)


### Indice delle Sottosezioni Capitolo 14


- [14.1 Motivazioni](#141-motivazioni)


- [14.2 Concorrenza vs Parallelismo](#142-concorrenza-vs-parallelismo)


- [14.3 Modelli di Multithreading](#143-modelli-di-multithreading)


- [14.4 API POSIX Pthreads](#144-api-posix-pthreads)


- [14.5 Thread in Linux](#145-thread-in-linux)


- [14.6 Cancellazione Thread](#146-cancellazione-thread)


- [14.7 Thread-Specific Data (TSD) — Dati Specifici del Thread](#147-thread-specific-data-tsd--dati-specifici-del-thread)


### Indice delle Sottosezioni Capitolo 15


- [15.1 Il Problema della Sezione Critica](#151-il-problema-della-sezione-critica)


- [15.2 Mutex — Mutua Esclusione](#152-mutex--mutua-esclusione)


- [15.3 Condition Variable — Variabili di Condizione](#153-condition-variable--variabili-di-condizione)


- [15.4 Semafori POSIX](#154-semafori-posix)


### Indice delle Sottosezioni Capitolo 16


- [16.1 Bounded-Buffer (Produttore-Consumatore)](#161-bounded-buffer-produttore-consumatore)


- [16.2 Readers-Writers (Lettori-Scrittori)](#162-readers-writers-lettori-scrittori)


- [16.3 Problema del Ponte a Senso Unico Alternato (Bridge Problem)](#163-problema-del-ponte-a-senso-unico-alternato-bridge-problem--sem_pontec)


### Indice delle Sottosezioni Capitolo 17


- [17.1 Concetti Fondamentali](#171-concetti-fondamentali)


- [17.2 Le fasi di una connessione TCP (SOCK_STREAM)](#172-le-fasi-di-una-connessione-tcp-sockstream)


- [17.3 Indirizzi e il problema del "Byte Order" (Endianness)](#173-indirizzi-e-il-problema-del-byte-order-endianness)


- [17.4 Socket Locali (AF_LOCAL / Unix Domain Sockets)](#174-socket-locali-aflocal--unix-domain-sockets)


- [17.5 Server TCP Completo](#175-server-tcp-completo)


- [17.6 Client TCP Completo](#176-client-tcp-completo)


- [17.7 `send()` e `recv()`](#177-send-e-recv)


- [17.8 Scambio Dati Binari — Network Byte Order](#178-scambio-dati-binari--network-byte-order)


- [17.9 Lettura e Scrittura Safe](#179-lettura-e-scrittura-safe)


- [17.10 Socket UDP](#1710-socket-udp)


- [17.11 Server Concorrente](#1711-server-concorrente)


- [17.12 Opzioni Socket (`setsockopt`)](#1712-opzioni-socket-setsockopt)


- [17.13 Socket Non Bloccante](#1713-socket-non-bloccante)


- [17.14 Pattern `recv_all` / `send_all` (lettura/scrittura safe)](#1714-pattern-recvall--sendall-letturascrittura-safe)


- [17.15 Server Concorrente — Anti-Zombie con SIGCHLD](#1715-server-concorrente--anti-zombie-con-sigchld)


- [17.16 `connect()` con UDP](#1716-connect-con-udp)


### Indice delle Sottosezioni Capitolo 18


- [18.1 Problema](#181-problema)


- [18.2 La Funzione `select()`](#182-la-funzione-select)


- [18.3 Macro per `fd_set`](#183-macro-per-fdset)


- [18.4 Quando un fd è "pronto"](#184-quando-un-fd--pronto)


- [18.5 Esempio — Select su stdin con timeout](#185-esempio--select-su-stdin-con-timeout)


- [18.6 Server Multiplexing con `select()`](#186-server-multiplexing-con-select)


### Indice delle Sottosezioni Capitolo 19


- [19.1 Gestione delle Syscall Bloccanti e `EINTR`](#191-gestione-delle-syscall-bloccanti-e-eintr)


- [19.2 Gestione di `SIGPIPE` nelle Socket TCP](#192-gestione-di-sigpipe-nelle-socket-tcp)


### Indice delle Sottosezioni Capitolo 20


- [20.1 Broadcast](#201-broadcast)


- [20.2 Multicast](#202-multicast)


- [20.3 Protocolli di Rete per il Multicast](#203-protocolli-di-rete-per-il-multicast)


### Indice delle Sottosezioni Capitolo 21


- [21.1 Comandi di Diagnostica](#211-comandi-di-diagnostica)


- [21.2 Risoluzione DNS in C — `getaddrinfo()`](#212-risoluzione-dns-in-c--getaddrinfo)


### Indice delle Sottosezioni Capitolo 22


- [22.1 Concetti Base](#221-concetti-base)


- [22.2 Simulazione, Emulazione, Virtualizzazione](#222-simulazione-emulazione-virtualizzazione)


- [22.3 Tipi di Hypervisor](#223-tipi-di-hypervisor)


- [22.4 Tecniche di Virtualizzazione](#224-tecniche-di-virtualizzazione)


- [22.5 Virtualizzazione HW-Assisted](#225-virtualizzazione-hw-assisted)


- [22.6 KVM (Linux), Hyper-V (Windows), macOS](#226-kvm-linux-hyper-v-windows-macos)


- [22.7 VM vs Container](#227-vm-vs-container)


- [22.8 Gestione delle Risorse nell'Hypervisor](#228-gestione-delle-risorse-nellhypervisor)


- [22.9 Confronto WSL2 vs VMware Workstation](#229-confronto-wsl2-vs-vmware-workstation)


### Indice delle Sottosezioni Capitolo 23


- [23.1 Architettura dei Container](#231-architettura-dei-container)


- [23.2 Namespace](#232-namespace)


- [23.3 PID Namespace — Gerarchia](#233-pid-namespace--gerarchia)


- [23.4 Cgroups (Control Groups)](#234-cgroups-control-groups)


- [23.5 OverlayFS — Filesystem a Strati](#235-overlayfs--filesystem-a-strati)


- [23.6 Mini-Container Didattico in C tramite `clone()` e `mount()`](#236-mini-container-didattico-in-c-tramite-clone-e-mount-test_containerc)


### Indice delle Sottosezioni Capitolo 24


- [24.1 Cos'è Docker](#241-cos-docker)


- [24.2 Comandi Base](#242-comandi-base)


- [24.3 Networking Docker](#243-networking-docker)


- [24.4 Dockerfile](#244-dockerfile)


- [24.5 Docker Compose](#245-docker-compose)


- [24.6 Limiti sulle Risorse (cgroups in Docker)](#246-limiti-sulle-risorse-cgroups-in-docker)


- [24.7 Volumi — Persistenza dei Dati](#247-volumi--persistenza-dei-dati)




### Indice delle Sottosezioni Capitolo 25

- [25.1 Esercizi di Bug-Hunting ("Trova e Correggi l'Errore" — File _ERR.c)](#251-esercizi-di-bug-hunting-trova-e-correggi-lerrore--file-_errc)
- [25.2 Template d'Esame da Completare (File _TODO.c e _TODO.sh)](#252-template-desame-da-completare-file-_todoc-e-_todosh)
- [25.3 Simulazione Ufficiale Prova Parziale (Lezione 28 — Prova PA)](#253-simulazione-ufficiale-prova-parziale-lezione-28--prova-pa)
- [25.4 Traccia d'Esame Ufficiale Svolta — Compito A](#254-traccia-desame-ufficiale-svolta--compito-a)
- [25.5 Traccia d'Esame Ufficiale Svolta — Compito B](#255-traccia-desame-ufficiale-svolta--compito-b)

### Indice delle Sottosezioni Capitolo 26

- [Concetti Generali e SO](#concetti-generali-e-so)
- [Shell, Comandi e Scripting](#shell-comandi-e-scripting)
- [Processi e Segnali](#processi-e-segnali)
- [Thread e Sincronizzazione (Pthreads)](#thread-e-sincronizzazione-pthreads)
- [IPC (Inter-Process Communication)](#ipc-inter-process-communication)
- [Reti e Socket](#reti-e-socket)
- [Virtualizzazione e Container](#virtualizzazione-e-container)
