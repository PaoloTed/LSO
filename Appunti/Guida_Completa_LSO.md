# Laboratorio di Sistemi Operativi — Guida Completa allo Studio

> **Corso di Laurea in Informatica — A.A. 2025-2026**  
> **Prof. Alberto Finzi**  
> Questa guida copre **tutti gli argomenti** delle lezioni 1–31 ed è pensata per essere **completamente sostitutiva** allo studio delle slide.

---

## Indice

1. [Introduzione ai Sistemi Operativi e Unix](#1-introduzione-ai-sistemi-operativi-e-unix)
2. [Il File System Unix](#2-il-file-system-unix)
3. [La Shell Bash](#3-la-shell-bash)
4. [Comandi Unix Fondamentali](#4-comandi-unix-fondamentali)
5. [Grep e le Espressioni Regolari](#5-grep-e-le-espressioni-regolari)
6. [Script Shell](#6-script-shell)
7. [Sed e Awk](#7-sed-e-awk)
8. [Funzioni in Bash](#8-funzioni-in-bash)
9. [Compilazione C e GCC](#9-compilazione-c-e-gcc)
10. [I/O di Basso Livello (System Call)](#10-io-di-basso-livello-system-call)
11. [Processi Unix](#11-processi-unix)
12. [Segnali](#12-segnali)
13. [IPC: Pipe, FIFO e Memoria Condivisa (mmap)](#13-ipc-pipe-fifo-e-memoria-condivisa-mmap)
14. [Thread e Concorrenza](#14-thread-e-concorrenza)
15. [Sincronizzazione: Mutex, Condition Variable, Semafori](#15-sincronizzazione-mutex-condition-variable-semafori)
16. [Problemi Classici di Sincronizzazione](#16-problemi-classici-di-sincronizzazione)
17. [Socket — Comunicazione di Rete](#17-socket--comunicazione-di-rete)

---

## 1. Introduzione ai Sistemi Operativi e Unix

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

La **pipe** (`|`) collega l'output di un comando all'input del successivo:

```bash
cat file | sort          # ordina il contenuto del file
ls | less                # pagina la lista dei file
ls | grep -v pluto | tail -3 | head -1
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

### 4.1 Gestione Directory

| Comando | Funzione |
|---------|----------|
| `mkdir [-p] [-m mode] dir` | Crea directory (`-p` crea percorsi intermedi) |
| `rmdir [-p] dir` | Rimuove directory vuota |
| `pwd` | Stampa la working directory |
| `cd [dir]` | Cambia directory (senza argomenti → home) |
| `ls [opzioni] [dir]` | Elenca contenuto directory |
| `du [-s] [-k] file` | Mostra spazio disco utilizzato |

**Opzioni di `ls`:**
- `-a` → anche file nascosti (dotfiles)
- `-l` → formato esteso
- `-s` → dimensione in blocchi
- `-t` → ordine per data modifica
- `-R` → ricorsivo
- `-F` → aggiunge `/` alle directory e `*` agli eseguibili
- `-i` → mostra i-number

### 4.2 Gestione File

| Comando | Funzione |
|---------|----------|
| `cp [-r] [-i] source target` | Copia file/directory |
| `mv [-i] source dest` | Sposta/rinomina file |
| `rm [-i] [-R] file` | Rimuove file/directory |
| `touch [-a] [-c] [-m] file` | Aggiorna timestamp (o crea file vuoto) |
| `file filename` | Determina il tipo di file |
| `ln [-s] name1 name2` | Crea link (hard o simbolico con `-s`) |
| `chmod permissions file` | Cambia permessi |
| `chown user[:group] file` | Cambia proprietario/gruppo |
| `find path -name pattern` | Cerca file ricorsivamente |

### 4.3 Comandi di Utilità su Testo

| Comando | Funzione |
|---------|----------|
| `cat [-n] [-b] file` | Concatena e visualizza file |
| `wc [-l] [-w] [-c] file` | Conta righe, parole, caratteri |
| `cut -d: -f1,5 file` | Estrae colonne |
| `paste file1 file2` | Compone file affiancandoli |
| `sort [-n] [-r] [-t:] [-k] file` | Ordina righe |
| `diff file1 file2` | Mostra differenze tra file |
| `head [-n N] file` | Prime N righe |
| `tail [-n N] file` | Ultime N righe |

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

### 5.1 Il Comando `grep`

```bash
grep [opzioni] pattern [file]
```

Stampa le righe che corrispondono al pattern. Se non si specifica un file, legge da stdin (utilizzabile in pipe).

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
| `exp\{N,M\}` | exp compare da N a M volte |

**Classi POSIX:**
- `[[:alpha:]]` → caratteri alfabetici
- `[[:alnum:]]` → alfanumerici
- `[[:digit:]]` → cifre
- `[[:upper:]]` → maiuscole
- `[[:lower:]]` → minuscole

### 5.3 Espressioni Regolari Estese (ERE)

In `grep` si usano con backslash (`\+`, `\|`, `\(...\)`). In `egrep` si usano direttamente.

| Sintassi | Significato |
|----------|-------------|
| `exp+` | Una o più occorrenze |
| `exp?` | Zero o una occorrenza |
| `exp1 \| exp2` | exp1 oppure exp2 |
| `\( exp \)` | Raggruppamento |

### 5.4 Esempi Pratici

```bash
# Righe che iniziano con 'a' e finiscono con 'b'
grep '^a.*b$' file

# File con permesso di esecuzione per il proprietario
ls -l | grep '^-..x'

# Directory con nome che inizia per maiuscola
ls -d */ | grep '^[[:upper:]]'

# Utenti che usano bash come shell
grep "bash$" /etc/passwd

# File .txt nella directory corrente e sottodirectory
ls -R | grep "\.txt$"
# oppure
find . -name "*.txt"
```

---

## 6. Script Shell

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

### 6.2 Variabili Predefinite negli Script

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
cmd1 && cmd2        # esegue cmd2 solo se cmd1 ha successo (exit=0)
cmd1 || cmd2        # esegue cmd2 solo se cmd1 fallisce (exit≠0)
```

### 6.5 Strutture di Controllo

**if-then-else:**
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
| Stringhe | `==`, `!=`, `-z` (vuota) |
| Interi | `-lt`, `-le`, `-eq`, `-ne`, `-ge`, `-gt` |
| File | `-e` (esiste), `-f` (file regolare), `-d` (directory), `-r`, `-w`, `-x` (permessi) |

```bash
if [ $# -lt 4 ]; then
    echo "Servono 4 argomenti."
    exit 1
elif [ ! -e "$1" ]; then
    echo "Il file $1 non esiste."
    exit 1
fi
```

**while:**
```bash
i=0
while [ $i -lt 10 ]; do
    i=$(( i + 1 ))
done
```

**until** (esegue finché la condizione è **falsa**):
```bash
COUNTER=20
until [ $COUNTER -lt 10 ]; do
    echo COUNTER: $COUNTER
    COUNTER=$((COUNTER-1))
done
```

**for:**
```bash
for a in 1 2 3; do
    echo $a
done

for a in $(ls); do
    echo $a
done

for a in "$@"; do
    echo $a
done

for a in *.txt; do
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
echo $(( a + 1 ))      # 8
echo $(( a * 3 > 8 ))  # 1 (vero)
echo $(( a++ ))         # 7, poi a diventa 8
```

Operatori: `+`, `-`, `/`, `*`, `%`, `**`, `<<`, `>>`, `&`, `|`, `~`, `<`, `<=`, `==`, `!=`, `>`, `>=`, `&&`, `||`, `!`

---

## 7. Sed e Awk

### 7.1 Sed — Stream Editor

**sed** è un editor non interattivo di file di testo. **Non modifica l'input** — l'output va allo stdout.

**Sintassi:** `sed [opzioni] 'comando' [file]`

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
- Range → `sed '2,4d' file` (cancella righe 2-4)
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

# Aggiunge indentazione
sed 's/^/   /' file > file.indent

# & ripete l'ultimo match
sed -e 's/.*/lui dice: &/' file
```

### 7.2 Awk — Linguaggio di Elaborazione Testuale

**awk** è un linguaggio data-driven che suddivide ogni riga in **campi** separati da spazi/tab.

**Sintassi:** `awk 'programma' file` o `awk -f script.awk file`

**Struttura del programma:**
```
BEGIN { azioni_iniziali }
/pattern/ { azioni }
END { azioni_finali }
```

**Variabili predefinite:**

| Variabile | Significato |
|-----------|-------------|
| `$0` | Riga intera |
| `$1, $2, ..., $N` | Campo 1, 2, ..., N |
| `FS` | Field Separator (default: spazio/tab) |
| `OFS` | Output Field Separator (default: spazio) |
| `ORS` | Output Record Separator (default: `\n`) |
| `NR` | Numero di record processati |
| `NF` | Numero di campi nella riga corrente |

**Esempi:**
```bash
# Stampa primo e terzo campo
df | awk '{ print $1, $3 }'

# Con espressione regolare
df | awk '/dev\/hd/ { print "Partizione:" $1 "\t usata al " $5 }'

# BEGIN e END
df | awk 'BEGIN {print "Report"} /dev/ {print $1, $5} END {print "Fine"}'

# Cambiare separatore
awk 'BEGIN { FS=":" } { print $1, $3 }' /etc/passwd

# Output formattato con printf
awk 'BEGIN { printf "%d %4.3f %s\n", 5, 3, "abc" }'

# Somma con script awk
# somma.awk:
# BEGIN { FS=":"; print "Calcolo Subtotali" }
# { subtotale=$1*$2; totale = totale+subtotale; print "Sub per " $3 "=" subtotale }
# END { print "Totale =" totale }
awk -f somma.awk dati.txt
```

---

## 8. Funzioni in Bash

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
open("prova.txt", O_RDONLY);
open("prova.txt", O_RDONLY | O_CREAT, S_IRWXU);
open("prova.txt", O_RDWR | O_CREAT | O_EXCL, S_IRWXU);
```

#### `creat` — Creazione di un file
```c
int creat(const char *pathname, mode_t mode);
// Equivalente a: open(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode);
```

#### `close` — Chiusura di un file
```c
#include <unistd.h>
int close(int filedes);
// Restituisce: 0 successo, -1 errore
```

#### `read` — Lettura da file
```c
#include <unistd.h>
ssize_t read(int filedes, void *buf, size_t nbytes);
// Restituisce: byte letti, 0 se fine file, -1 errore
```

#### `write` — Scrittura su file
```c
#include <unistd.h>
ssize_t write(int filedes, void *buf, size_t nbytes);
// Restituisce: byte scritti, -1 errore
```

### 10.3 Offset e `lseek`

L'**offset** è la posizione (in byte dall'inizio) dove avviene la prossima operazione I/O.

```c
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int filedes, off_t offset, int whence);
// Restituisce: nuovo offset, -1 errore
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

### 10.4 Gestione Errori con `perror` e `errno`

```c
int fd = open("prova.txt", O_RDONLY);
if (fd < 0)
    perror("errore di open");  // stampa "errore di open: No such file or directory"
```

### 10.5 Implementazione nel Kernel

Il kernel usa tre strutture dati:
1. **Tabella dei processi**: ogni processo ha un vettore di file descriptor
2. **File table**: per ogni file aperto: flag di stato, offset, puntatore al v-node
3. **V-node table**: informazioni sul tipo di file e sulle funzioni operative (dati dall'i-node)

> Due processi che aprono lo stesso file hanno **entry diverse nella file table** (offset separati) ma condividono lo **stesso v-node**.

### 10.6 Duplicazione File Descriptor — `dup` e `dup2`

```c
#include <unistd.h>
int dup(int filedes);       // ritorna il minimo fd non utilizzato
int dup2(int filedes, int filedes2);  // specifica quale fd usare (operazione atomica)
```

**Esempio di redirezione stdout su file:**
```c
int fd = open("testfile", O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
dup2(fd, STDOUT_FILENO);  // ora stdout scrive su testfile
```

### 10.7 Struttura `stat`

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

### 10.8 Esempio Completo: Copia tra File

```c
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#define BUFDIM 1000

int main(int argc, char **argv) {
    int infile, outfile, nread;
    char buffer[BUFDIM];
    if (argc != 3) { printf("Uso: copia dest sorg\n"); exit(1); }
    if ((infile = open(argv[2], O_RDONLY)) < 0) { perror("apertura sorgente"); exit(1); }
    if ((outfile = creat(argv[1], 0777)) < 0) { perror("apertura dest"); close(infile); exit(1); }
    while ((nread = read(infile, buffer, BUFDIM)) > 0) {
        if (write(outfile, buffer, nread) == -1) {
            close(infile); close(outfile); exit(1);
        }
    }
    close(infile); close(outfile);
    return 0;
}
```

---

## 11. Processi Unix

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

Il figlio eredita dal padre: codice, dati, heap, stack, file descriptor, variabili di ambiente, working directory, permessi, ecc.

```c
pid_t pid = fork();
if (pid < 0) {
    perror("fork failed");
} else if (pid == 0) {
    // Codice eseguito dal FIGLIO
    printf("Sono il figlio, PID=%d\n", getpid());
} else {
    // Codice eseguito dal PADRE
    printf("Sono il padre, figlio PID=%d\n", pid);
    wait(NULL);  // aspetta la terminazione del figlio
}
```

### 11.4 Terminazione di Processi

**Terminazione normale:**
- `return` da main (equivale a `exit`)
- `exit(int status)` — chiude stream I/O, invoca exit handler
- `_exit(int status)` — ritorna al kernel immediatamente

**Terminazione anormale:**
- Ricezione di certi segnali (SIGKILL, SIGSEGV, ecc.)
- `abort()` — genera SIGABRT

**Azioni del kernel alla terminazione:**
- Rimozione della memoria del processo
- Chiusura dei descrittori aperti
- Notifica al padre tramite SIGCHLD

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

**Argomento `pid` di `waitpid`:**
- `pid > 0` → attende il figlio con quel PID
- `pid == -1` → come wait (qualsiasi figlio)
- `pid == 0` → figlio con stesso process group
- `pid < -1` → figlio con process group ID = |pid|

**Opzione `WNOHANG`**: non si blocca se nessun figlio ha terminato.

**Macro per ispezionare status:**
- `WIFEXITED(status)` → terminazione normale
- `WEXITSTATUS(status)` → exit status
- `WIFSIGNALED(status)` → terminazione da segnale

### 11.7 La Famiglia `exec`

Le `exec` **sovrascrivono** la memoria del processo con un nuovo programma. Il PID **non cambia**.

```c
int execl(char *pathname, char *arg0, ... );       // argomenti come lista
int execv(char *pathname, char *argv[]);            // argomenti come array
int execlp(char *filename, char *arg0, ... );       // cerca nel PATH
int execvp(char *filename, char *argv[]);           // cerca nel PATH
int execle(char *pathname, char *arg0, ..., char *envp[]); // con ambiente
int execve(char *pathname, char *argv[], char *envp[]);    // unica vera syscall
```

| Suffisso | Significato |
|----------|-------------|
| `l` | Argomenti come lista (terminata da NULL) |
| `v` | Argomenti come array argv[] |
| `p` | Cerca nel PATH |
| `e` | Ambiente specificato esplicitamente |

**Esempio fork + exec:**
```c
pid_t pid = fork();
if (pid == 0) {
    execl("/bin/ls", "ls", "-l", (char *)0);
    perror("exec failed");  // eseguito solo se exec fallisce
    exit(1);
} else {
    wait(NULL);
    printf("ls completato\n");
}
```

**Proprietà ereditate da `exec`:**
- process ID e parent process ID
- real uid e real gid, supplementary gid
- process group ID, session ID, terminale di controllo
- current working directory, root directory
- umask, file locks, maschera dei segnali, segnali in attesa

**Proprietà NON ereditate da `exec`:**
- effective user ID e effective group ID (reimpostati dai bit di protezione del file)
- File descriptor con flag `FD_CLOEXEC` (`close-on-exec`) attivo → vengono chiusi automaticamente
```

### 11.8 `vfork()`

Simile a `fork()`, ma:
- **Non copia** lo spazio di indirizzamento
- Il figlio esegue nello spazio del padre
- Il figlio esegue **per primo** fino a `exec()` o `_exit()`
- Usato tipicamente prima di `exec()` per efficienza

### 11.9 La funzione `system()`

```c
int system(char *command);
```
- Esegue un comando shell (`/bin/sh -c command`)
- **Attende la terminazione** del comando
- È una funzione di libreria (non una system call)

### 11.10 Ambiente di un Processo

```c
// Accedere all'ambiente
char *getenv(const char *name);        // ottiene valore di una variabile
int putenv(char *string);              // "variabile=valore"
extern char **environ;                 // variabile globale con tutto l'ambiente

// Accesso tramite main
int main(int argc, char **argv, char **envp) { ... }

// Passare ambiente custom a exec
execle(path, arg0, arg1, (char*)0, envp);  // con lista argomenti
execve(path, argv, envp);                  // con array argomenti
```

**Cambiare directory e root del processo:**
```c
#include <unistd.h>
int chdir(const char *path);   // cambia la CWD del processo (ereditata dai figli)
int chroot(const char *path);  // cambia la root directory del processo
// chroot: utile per sandboxing (es. nei container prima di pivot_root)
```

**`exit()` vs `_exit()`:**

| Funzione | Comportamento |
|----------|---------------|
| `exit(status)` | Invoca exit handlers registrati, chiude stream I/O, poi chiama `_exit()` |
| `_exit(status)` | Ritorna immediatamente al kernel senza flush dei buffer |

> Nei processi figli dopo `fork()` si usa `_exit()` (mai `exit()`) per evitare di fluscare buffer del padre che non appartengono al figlio.

---

## 12. Segnali

### 12.1 Cos'è un Segnale

Un **segnale** è un **interrupt software** che consente la comunicazione **asincrona** tra processi e/o tra device e processo.

- Ogni segnale ha un nome che inizia con `SIG` (definiti in `<signal.h>`)
- Associati a interi positivi
- Inviati in modo **asincrono**

### 12.2 Segnali Principali

| Nome | Significato | Default |
|------|-------------|---------|
| `SIGINT` | Interruzione da tastiera (Ctrl-C) | Terminare |
| `SIGSTOP` | Stop al processo* | Fermare |
| `SIGKILL` | Terminazione forzata* | Terminare |
| `SIGQUIT` | Quit da tastiera (Ctrl-\) | Terminare |
| `SIGTERM` | Terminazione | Terminare |
| `SIGCHLD` | Figlio terminato o fermato | Ignorare |
| `SIGALRM` | Sveglia (alarm) | Terminare |
| `SIGSEGV` | Segmentation fault | Terminare |
| `SIGUSR1/2` | A disposizione dell'utente | Terminare |
| `SIGPIPE` | Scrittura su pipe senza lettore | Terminare |

> *`SIGKILL` e `SIGSTOP` **non possono** essere catturati o ignorati.

### 12.3 Azioni Possibili

1. **Ignorare** il segnale: `signal(SIGINT, SIG_IGN)`
2. **Catturare** il segnale: `signal(SIGINT, handler_function)`
3. **Azione di default**: `signal(SIGINT, SIG_DFL)`

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
    signal(SIGUSR1, foo);
    signal(SIGUSR2, foo);
    signal(SIGINT, foo);
    // signal(SIGKILL, foo); // ERRORE: non si può catturare SIGKILL
    for (;;) { pause(); }  // attende segnali
}
```

### 12.5 Inviare Segnali

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

```c
#include <signal.h>
int sigemptyset(sigset_t *set);     // set vuoto
int sigfillset(sigset_t *set);      // set con tutti i segnali
int sigaddset(sigset_t *set, int sig);  // aggiunge segnale
int sigdelset(sigset_t *set, int sig);  // rimuove segnale
int sigismember(const sigset_t *set, int sig);  // test appartenenza

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
// how: SIG_BLOCK (unione), SIG_UNBLOCK (intersezione), SIG_SETMASK (sostituzione)
```

La **signal mask** blocca la consegna dei segnali mascherati al processo.

---

## 13. IPC: Pipe, FIFO e Memoria Condivisa (mmap)

### 13.1 Pipe Ordinarie

Le **pipe** sono canali di comunicazione **unidirezionali** tra processi con relazione parentale.

```c
#include <unistd.h>
int pipe(int fd[2]);
// fd[0] = estremità di lettura (read-end)
// fd[1] = estremità di scrittura (write-end)
```

**Funzionamento:**
- Modello **produttore-consumatore**
- Il produttore scrive su `fd[1]`, il consumatore legge da `fd[0]`
- **Occorre chiudere le imboccature non utilizzate** (altrimenti il lettore non riceve EOF)

**Esempio:**
```c
int fd[2];
pipe(fd);
pid_t pid = fork();
if (pid == 0) {         // Figlio (lettore)
    close(fd[1]);       // chiude la scrittura
    char buf[64];
    while (read(fd[0], buf, sizeof(buf)) > 0)
        write(STDOUT_FILENO, buf, 5);
    close(fd[0]);
    _exit(0);
} else {                // Padre (scrittore)
    close(fd[0]);       // chiude la lettura
    write(fd[1], "ciao\n", 5);
    close(fd[1]);       // segnala EOF
    wait(NULL);
}
```

### 13.2 Pipe con Nome (FIFO)

Le **Named Pipes** (FIFO) sono più potenti delle pipe ordinarie:
- **Non richiedono** relazione parentale tra i processi
- Creano un **file speciale** nel filesystem
- Devono essere cancellate con `unlink()`
- Usate in modalità unidirezionale

```c
#include <sys/stat.h>
int mkfifo(const char *pathname, mode_t mode);
```

**Comportamento dell'open su FIFO:**

| Operazione | Comportamento |
|------------|---------------|
| `open("fifo", O_RDONLY)` | Blocca finché non c'è un writer |
| `open("fifo", O_WRONLY)` | Blocca finché non c'è un reader |
| `open("fifo", O_RDWR)` | Non blocca mai |
| Con `O_NONBLOCK` | Non blocca, ma errore se nessun peer |

### 13.3 Memoria Condivisa con `mmap`

**mmap** mappa in memoria un file o un device, permettendo accesso diretto.

```c
#include <sys/mman.h>
void *mmap(void *address, size_t length, int protect, int flags, int filedes, off_t offset);
int munmap(void *addr, size_t length);
```

| Parametro | Significato |
|-----------|-------------|
| `protect` | `PROT_READ`, `PROT_WRITE`, `PROT_EXEC`, `PROT_NONE` |
| `flags` | `MAP_SHARED` (visibile ad altri), `MAP_PRIVATE`, `MAP_ANONYMOUS` |

**Esempio — mmap anonima tra padre e figlio:**
```c
char *shared = mmap(NULL, 256,
    PROT_READ | PROT_WRITE,
    MAP_SHARED | MAP_ANONYMOUS, -1, 0);

pid_t pid = fork();
if (pid == 0) {
    strcpy(shared, "Ciao dal figlio via mmap anonima!");
    _exit(0);
}
waitpid(pid, NULL, 0);
printf("[Padre] Letto: \"%s\"\n", shared);
munmap(shared, 256);
```

**Confronto Pipe vs Memoria Condivisa:**

| Aspetto | Pipe / Message Passing | Memoria Condivisa |
|---------|----------------------|-------------------|
| Velocità | Più lento (ogni scambio passa per il kernel) | Più veloce (accesso diretto) |
| Coordinazione | Gestita dal kernel | Responsabilità dei processi |
| Complessità | Più semplice | Richiede sincronizzazione |

---

## 14. Thread e Concorrenza

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

#### Detach
```c
int pthread_detach(pthread_t thread);
// Il thread si "auto-raccoglie" alla terminazione (no join necessario)
```

**Esempio completo:**
```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *tbody(void *arg) {
    int *pi = (int *)arg;
    printf("Thread: valore ricevuto = %d\n", *pi);
    *pi = 10;  // modifica dato condiviso
    int *ret = malloc(sizeof(int));
    *ret = 50;
    pthread_exit((void *)ret);
}

int main(void) {
    pthread_t mythread;
    int i = 0;
    void *result;
    pthread_create(&mythread, NULL, tbody, (void *)&i);
    pthread_join(mythread, &result);
    printf("Main: i = %d, thread restituito %d\n", i, *(int *)result);
    free(result);
    return 0;
}
```

> **Compilazione**: `gcc -pthread -o prog prog.c`

### 14.5 Thread in Linux

- Linux tratta i thread come **task** (unifica processi e thread)
- `pthread_create` internamente usa la system call **`clone()`**
- Flag di `clone()`:  `CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD`
- Per vedere i kernel thread: `ps -Lf <PID>`, `ps -T -p <PID>`, `ls /proc/<PID>/task`

### 14.6 Cancellazione Thread

```c
int pthread_cancel(pthread_t thread);  // richiede la cancellazione
int pthread_setcancelstate(int state, int *oldstate);
// PTHREAD_CANCEL_ENABLE o PTHREAD_CANCEL_DISABLE
void pthread_testcancel(void);  // cancellation point
```

Due approcci:
- **Asincrona**: terminazione immediata
- **Deferred** (default): cancellazione solo ai *cancellation point* (`pthread_testcancel()`)

---

## 15. Sincronizzazione: Mutex, Condition Variable, Semafori

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
pthread_mutex_lock(&mutex);     // acquisisce il lock (bloccante)
pthread_mutex_unlock(&mutex);   // rilascia il lock
pthread_mutex_destroy(&mutex);  // distrugge il mutex
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

Permettono a un thread di **attendere** che una condizione diventi vera, senza busy waiting.

```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Attendi che la condizione sia vera
pthread_cond_wait(&cond, &mutex);
// ATOMICAMENTE: rilascia il mutex + si mette in attesa
// Quando si risveglia, riacquisisce il mutex

// Attendi con timeout
pthread_cond_timedwait(&cond, &mutex, &timeout);

// Segnala un thread in attesa
pthread_cond_signal(&cond);

// Segnala tutti i thread in attesa
pthread_cond_broadcast(&cond);

// Distruzione
pthread_cond_destroy(&cond);
```

> **Pattern fondamentale**: si usa **sempre** `while` (non `if`) per controllare la condizione:
> ```c
> pthread_mutex_lock(&mtx);
> while (!condizione_soddisfatta)
>     pthread_cond_wait(&cond, &mtx);
> // ... sezione critica ...
> pthread_mutex_unlock(&mtx);
> ```

**`pthread_cond_timedwait` — attesa con timeout:**

```c
#include <time.h>

// Calcola deadline: ora + N secondi
struct timespec ts;
clock_gettime(CLOCK_REALTIME, &ts);
ts.tv_sec += 2;  // deadline tra 2 secondi

pthread_mutex_lock(&mtx);
int rc = 0;
while (!ready && rc == 0) {
    rc = pthread_cond_timedwait(&cond, &mtx, &ts);
    // rc == 0        → segnalato prima della deadline (ricontrolla condizione)
    // rc == ETIMEDOUT → deadline scaduta
}
if (ready) {
    printf("Evento ricevuto entro la deadline\n");
} else if (rc == ETIMEDOUT) {
    printf("Timeout scaduto\n");
}
pthread_mutex_unlock(&mtx);
```

> Si usa comunque il `while` per proteggersi da **spurious wakeup** anche con `timedwait`.

### 15.4 Semafori POSIX

Un **semaforo** è una variabile intera modificata con due operazioni atomiche: `wait()` (P) e `signal()` (V).

```c
#include <semaphore.h>

sem_t sem;
sem_init(&sem, 0, valore_iniziale);  // 0 = tra thread dello stesso processo
sem_wait(&sem);    // decrementa; se S <= 0, blocca (P/wait)
sem_post(&sem);    // incrementa; sveglia un thread bloccato (V/signal)
sem_destroy(&sem);
```

**Tipi:**
- **Semaforo binario** (valore 0 o 1): equivale a un mutex
- **Semaforo contatore**: controlla accesso a N risorse

**Esempio — sincronizzazione di scheduling:**
```c
sem_t synch;
sem_init(&synch, 0, 0);

// Thread 1                    // Thread 2
S1;                            sem_wait(&synch);  // attende
sem_post(&synch);              S2;                // esegue dopo S1
```

---

## 16. Problemi Classici di Sincronizzazione

### 16.1 Bounded-Buffer (Produttore-Consumatore)

**Problema:** N produttori inseriscono in un buffer circolare di dimensione fissa, M consumatori lo svuotano.

**Soluzione con mutex + condition variable:**
```c
#define BUF_SIZE 10
typedef struct {
    int buf[BUF_SIZE];
    int in, out, count;
    pthread_mutex_t mtx;
    pthread_cond_t not_full, not_empty;
} bbuff_t;

void put_item(bbuff_t *b, int item) {
    pthread_mutex_lock(&b->mtx);
    while (b->count == BUF_SIZE)
        pthread_cond_wait(&b->not_full, &b->mtx);
    b->buf[b->in] = item;
    b->in = (b->in + 1) % BUF_SIZE;
    b->count++;
    pthread_cond_signal(&b->not_empty);
    pthread_mutex_unlock(&b->mtx);
}

int get_item(bbuff_t *b) {
    pthread_mutex_lock(&b->mtx);
    while (b->count == 0)
        pthread_cond_wait(&b->not_empty, &b->mtx);
    int item = b->buf[b->out];
    b->out = (b->out + 1) % BUF_SIZE;
    b->count--;
    pthread_cond_signal(&b->not_full);
    pthread_mutex_unlock(&b->mtx);
    return item;
}
```

**Soluzione con semafori:**
```c
sem_t empty;   // slot liberi (init = BUF_SIZE)
sem_t full;    // elementi presenti (init = 0)
sem_t mutex;   // protezione buffer (init = 1)

void put_item(int item) {
    sem_wait(&empty);       // attende slot libero
    sem_wait(&mutex);       // sezione critica
    buffer[in_idx] = item;
    in_idx = (in_idx + 1) % BUF_SIZE;
    sem_post(&mutex);
    sem_post(&full);        // segnala nuovo elemento
}

int get_item(void) {
    sem_wait(&full);        // attende elemento
    sem_wait(&mutex);       // sezione critica
    int item = buffer[out_idx];
    out_idx = (out_idx + 1) % BUF_SIZE;
    sem_post(&mutex);
    sem_post(&empty);       // segnala slot libero
    return item;
}
```

### 16.2 Readers-Writers

**Problema:** dati condivisi tra lettori (solo lettura) e scrittori (lettura+scrittura). Lettori multipli simultanei OK, ma uno scrittore deve avere accesso esclusivo.

```c
pthread_mutex_t mutex;      // protegge read_count
pthread_mutex_t rw_mutex;   // accesso esclusivo ai dati
int read_count = 0;

// Scrittore
pthread_mutex_lock(&rw_mutex);
// ... scrittura ...
pthread_mutex_unlock(&rw_mutex);

// Lettore
pthread_mutex_lock(&mutex);
read_count++;
if (read_count == 1)           // primo lettore
    pthread_mutex_lock(&rw_mutex);  // blocca gli scrittori
pthread_mutex_unlock(&mutex);

// ... lettura ...

pthread_mutex_lock(&mutex);
read_count--;
if (read_count == 0)           // ultimo lettore
    pthread_mutex_unlock(&rw_mutex);  // sblocca gli scrittori
pthread_mutex_unlock(&mutex);
```

---

## 17. Socket — Comunicazione di Rete

### 17.1 Concetti Fondamentali

Le **socket** sono endpoint di comunicazione. Permettono la comunicazione tra processi, sia sulla stessa macchina che in rete.

**Tipi di socket:**

| Tipo | Costante | Protocollo | Caratteristiche |
|------|----------|-----------|-----------------|
| Stream | `SOCK_STREAM` | TCP | Connessione, affidabile, flusso di byte |
| Datagram | `SOCK_DGRAM` | UDP | Senza connessione, non affidabile, pacchetti |

**Domini di comunicazione:**

| Dominio | Costante | Uso |
|---------|----------|-----|
| Locale | `AF_LOCAL` / `AF_UNIX` | Processi sulla stessa macchina |
| Internet IPv4 | `AF_INET` | Comunicazione in rete IPv4 |
| Internet IPv6 | `AF_INET6` | Comunicazione in rete IPv6 |

### 17.2 Socket Locali (AF_LOCAL) — Stream

**Server:**
```c
#include <sys/socket.h>
#include <sys/un.h>

int listen_sd = socket(AF_LOCAL, SOCK_STREAM, 0);

struct sockaddr_un my_addr;
memset(&my_addr, 0, sizeof(my_addr));
my_addr.sun_family = AF_LOCAL;
strncpy(my_addr.sun_path, "/tmp/mysock", sizeof(my_addr.sun_path) - 1);

unlink("/tmp/mysock");  // rimuove socket precedente
bind(listen_sd, (struct sockaddr*)&my_addr, sizeof(my_addr));
listen(listen_sd, 5);

int connect_sd = accept(listen_sd, NULL, NULL);
// ... comunica con connect_sd via read/write ...
close(connect_sd);
close(listen_sd);
unlink("/tmp/mysock");
```

**Client:**
```c
int sd = socket(AF_LOCAL, SOCK_STREAM, 0);
struct sockaddr_un srv_addr;
memset(&srv_addr, 0, sizeof(srv_addr));
srv_addr.sun_family = AF_LOCAL;
strncpy(srv_addr.sun_path, "/tmp/mysock", sizeof(srv_addr.sun_path) - 1);

connect(sd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
// ... comunica ...
close(sd);
```

### 17.3 Socket TCP (AF_INET) — Indirizzi e Byte Order

**Struttura degli indirizzi IPv4:**
```c
struct sockaddr_in {
    sa_family_t    sin_family;   // AF_INET
    in_port_t      sin_port;     // porta (network byte order)
    struct in_addr sin_addr;     // indirizzo IP (4 byte)
};
```

**Conversione byte order:**
- **Network byte order**: Big Endian (standard TCP/IP)
- **Host byte order**: dipende dall'architettura (x86 = Little Endian)

| Funzione | Conversione |
|----------|-------------|
| `htons()` | Host to Network (short, 16 bit) — per porte |
| `htonl()` | Host to Network (long, 32 bit) — per indirizzi |
| `ntohs()` | Network to Host (short) |
| `ntohl()` | Network to Host (long) |

**Conversione indirizzi IP:**
```c
#include <arpa/inet.h>
int inet_pton(int af, const char *src, void *dst);  // stringa → binario
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size); // binario → stringa
```

### 17.4 Schema di Connessione TCP

```
        SERVER                              CLIENT
    ┌──────────────┐                   ┌──────────────┐
    │ socket()     │                   │ socket()     │
    │ bind()       │                   │              │
    │ listen()     │                   │              │
    │ accept()  ←──│── 3-way handshake │── connect()  │
    │              │                   │              │
    │ recv/send    │ ←────────────────→│ recv/send    │
    │              │                   │              │
    │ close()      │                   │ close()      │
    └──────────────┘                   └──────────────┘
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

### 17.8 Scambio Dati Binari — Network Byte Order

```c
// Invio
uint32_t val = 42;
uint32_t val_net = htonl(val);
write(sd, &val_net, sizeof(val_net));

// Ricezione
uint32_t val_net;
read(sd, &val_net, sizeof(val_net));
uint32_t val = ntohl(val_net);
```

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

### 17.10 Socket UDP

UDP non stabilisce connessioni. Si usano `sendto()` e `recvfrom()`.

```c
// Server UDP
int s = socket(AF_INET, SOCK_DGRAM, 0);
struct sockaddr_in addr = {0};
addr.sin_family      = AF_INET;
addr.sin_port        = htons(5200);
addr.sin_addr.s_addr = htonl(INADDR_ANY);
bind(s, (struct sockaddr *)&addr, sizeof(addr));

struct sockaddr_in from;
socklen_t flen = sizeof(from);
int n = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&from, &flen);
sendto(s, buf, n, 0, (struct sockaddr *)&from, flen);  // echo

// Client UDP
int s = socket(AF_INET, SOCK_DGRAM, 0);
// ... imposta addr del server ...
sendto(s, "ciao", 4, 0, (struct sockaddr *)&addr, sizeof(addr));
int n = recvfrom(s, buf, sizeof(buf), 0, NULL, NULL);
```

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

> **Importante**: `select()` modifica i set passati. Bisogna **ricreare** il set ad ogni iterazione.

---

## 19. Gestione Avanzata dei Segnali — `sigaction()` e SIGPIPE

### 19.1 `sigaction()` vs `signal()`

```c
struct sigaction {
    void     (*sa_handler)(int);
    sigset_t sa_mask;
    int      sa_flags;
};

struct sigaction sa = {0};
sa.sa_handler = mio_handler;
sa.sa_flags   = SA_RESTART;    // riavvia syscall interrotte
sigemptyset(&sa.sa_mask);
sigaction(SIGINT, &sa, NULL);
```

**`SA_RESTART`**: se una syscall bloccante (`accept`, `recv`, `read`) viene interrotta da un segnale, viene **riavviata automaticamente** invece di ritornare `-1` con `errno = EINTR`.

### 19.2 Gestione di `EINTR` manuale

```c
for (;;) {
    int sd = accept(listen_sd, ...);
    if (sd >= 0) break;
    if (errno == EINTR) continue;  // interrotto da segnale, riprova
    perror("accept"); break;
}
```

### 19.3 Gestione di `SIGPIPE`

Quando un processo scrive su un socket il cui peer ha chiuso la connessione, riceve **SIGPIPE** (default: terminazione).

**Approccio tipico:** ignorare SIGPIPE e gestire l'errore con `errno`:
```c
signal(SIGPIPE, SIG_IGN);

ssize_t w = send(sd, buf, len, 0);
if (w < 0 && errno == EPIPE) {
    // peer ha chiuso → chiudi il socket
    close(sd);
}

// Per la lettura:
ssize_t n = recv(sd, buf, sizeof(buf), 0);
if (n == 0) {
    // peer ha chiuso la connessione (FIN)
    close(sd);
}
```

---

## 20. Broadcast e Multicast UDP

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

**Indirizzi multicast IPv4:** `224.0.0.0` – `239.255.255.255`
- `224.0.0.x` → link-local (non escono dalla LAN)
- `239.x.x.x` → amministrativi locali (consigliati per applicazioni)

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

Funzione moderna e portabile per risolvere nomi simbolici in indirizzi IP.

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

---

## 22. Virtualizzazione

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

## 24. Docker

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
