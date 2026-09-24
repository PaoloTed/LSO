# 📚 Piano di Studio Completo — Laboratorio di Sistemi Operativi

> **Corso:** Laboratorio di Sistemi Operativi — A.A. 2025-2026  
> **Professore:** Alberto Finzi  
> **Riferimento:** [Guida_Completa_LSO.md](file:///c:/Users/Paolo/Desktop/Universita/Terzo%20Anno/LSO/Appunti/Guida_Completa_LSO.md)

---

## 🗺️ Come Usare Questa Guida

Ogni sezione contiene:
1. **⏱️ Tempo stimato** — Quante ore dedicare all'argomento
2. **📖 Obiettivi** — Cosa devi saper fare alla fine della sessione
3. **🃏 Flashcard Anki** — Domande fronte/retro da importare su Anki
4. **❓ Domande di Autoverifica** — Per testare la comprensione
5. **💻 Esercizi Pratici** — Esercizi di codice da implementare

> **Tip Anki:** Copia ogni tabella Fronte/Retro in un file `.csv` (separatore: `;`) e importa su Anki via File → Importa.

---

## 📅 Piano di Studio Giornaliero

| Giorno | Argomenti | Ore Stimate |
|--------|-----------|-------------|
| **Giorno 1** | Cap. 1: Intro SO + Unix, Cap. 2: File System Unix | ~3h |
| **Giorno 2** | Cap. 3: Shell Bash, Cap. 4: Comandi Unix | ~3h |
| **Giorno 3** | Cap. 5: Grep + Regex, Cap. 6: Script Shell | ~3.5h |
| **Giorno 4** | Cap. 7: Sed e Awk, Cap. 8: Funzioni Bash | ~3h |
| **Giorno 5** | Cap. 9: Compilazione C e GCC, Cap. 10: I/O Basso Livello | ~4h |
| **Giorno 6** | Cap. 11: Processi Unix (fork, exec, wait) | ~4h |
| **Giorno 7** | Cap. 12: Segnali | ~3h |
| **Giorno 8** | Cap. 13: IPC — Pipe, FIFO, mmap | ~4h |
| **Giorno 9** | Cap. 14: Thread e Concorrenza, Cap. 15: Sincronizzazione | ~4.5h |
| **Giorno 10** | Cap. 16: Problemi Classici, Cap. 17: Socket | ~4h |
| **Giorno 11** | Cap. 18: I/O Multiplexing, Cap. 19: Segnali Avanzati | ~3h |
| **Giorno 12** | Cap. 20-21: UDP Broadcast, Comandi Rete | ~2.5h |
| **Giorno 13** | Cap. 22-24: Virtualizzazione, Container, Docker | ~3h |
| **Giorno 14** | Ripasso generale + Esercizi d'esame (Cap. 25) | ~4h |

> **Totale stimato: ~50 ore**

---

## 📆 GIORNO 1 — Introduzione SO e File System Unix

---

### Cap. 1 — Introduzione ai Sistemi Operativi e Unix
**⏱️ Tempo: ~1.5h**

#### 📖 Obiettivi
- Spiegare i tre ruoli del SO (allocatore, programma di controllo, macchina estesa)
- Distinguere user mode e kernel mode
- Capire come funziona una system call (esempio con `read`)
- Conoscere la storia di Unix e le sue caratteristiche

#### 🃏 Flashcard Anki (Cap. 1)

| Fronte | Retro |
|--------|-------|
| Quali sono i 3 ruoli fondamentali di un SO? | 1. Allocatore di risorse 2. Programma di controllo 3. Macchina estesa |
| Cos'è il kernel? | Il nucleo del SO, l'unico programma in esecuzione in modalità privilegiata. Gestisce CPU, memoria e periferiche. |
| Differenza tra user mode e kernel mode? | User mode: accesso limitato, processi utente. Kernel mode: accesso completo all'hardware, solo il kernel. |
| Come funziona una system call? | Il programma mette i parametri nello stack → genera un'interruzione → il kernel prende il controllo → esegue l'operazione → ritorna il risultato. |
| Quando nasce Unix e chi lo ha creato? | 1969 da Thompson, Ritchie, Canaday e McIlroy ai Bell Labs. Il 1° Gennaio 1970 è l'epoch Unix. |
| Cos'è il principio "everything is a file"? | In Unix, quasi tutto (file regolari, directory, dispositivi, pipe) è rappresentato come file e accessibile tramite la stessa interfaccia (open/read/write/close). |
| Cos'è un file descriptor? | Un numero intero non negativo che identifica un file aperto all'interno di un processo. 0=stdin, 1=stdout, 2=stderr. |

#### ❓ Domande di Autoverifica

1. Spiega la differenza tra system call e funzione di libreria C.
2. Cosa succede a livello hardware quando un processo esegue una system call?
3. Perché il kernel ha bisogno della modalità privilegiata?
4. Cosa gestisce lo scheduler del kernel?
5. Perché Unix fu riscritto in C nel 1973? Quali vantaggi portò?

---

### Cap. 2 — Il File System Unix
**⏱️ Tempo: ~1.5h**

#### 📖 Obiettivi
- Descrivere la struttura gerarchica del file system Unix
- Spiegare cos'è un inode e cosa contiene
- Distinguere hard link e symbolic link
- Conoscere i permessi e come interpretare una stringa di permessi (es. `rwxr-xr--`)

#### 🃏 Flashcard Anki (Cap. 2)

| Fronte | Retro |
|--------|-------|
| Cos'è un inode? | Una struttura dati che contiene i metadati di un file: tipo, permessi, uid, gid, dimensione, timestamp, puntatori ai blocchi dati. NON contiene il nome del file. |
| Dove è memorizzato il nome di un file? | Nella **directory** (come associazione nome → numero inode). |
| Differenza tra hard link e symbolic link? | Hard link: punta direttamente all'inode (stesso inode, contatore nlink aumenta). Soft link: file separato che contiene il percorso del file originale. |
| Cosa rappresentano i bit `rwxrwxrwx`? | 3 gruppi da 3 bit: permessi per **proprietario**, **gruppo**, **altri**. r=lettura, w=scrittura, x=esecuzione. |
| Cosa fa il comando `chmod 755 file`? | Imposta i permessi a `rwxr-xr-x` (proprietario: tutti, gruppo: r+x, altri: r+x). |
| Cos'è il `/` nel file system Unix? | La directory radice (root), da cui parte tutta la gerarchia del file system. |
| Cosa contiene `/etc`? | File di configurazione del sistema (es. /etc/passwd, /etc/hosts, /etc/fstab). |
| Cosa contiene `/proc`? | Un file system virtuale che espone informazioni sui processi e sullo stato del kernel in tempo reale. |
| Differenza percorso assoluto e relativo? | Assoluto: parte dalla radice `/`, es. `/home/user/file.txt`. Relativo: parte dalla directory corrente, es. `./file.txt` o `../altro/file`. |

#### ❓ Domande di Autoverifica

1. Disegna la struttura gerarchica del filesystem Unix con le directory principali.
2. Perché eliminare un hard link non cancella necessariamente il file?
3. Un file con permessi `640` cosa permette al proprietario? Al gruppo? Agli altri?
4. Cosa succede quando il contatore `nlink` di un inode arriva a 0?
5. Spiega la differenza tra `/bin` e `/usr/bin`.

---

## 📆 GIORNO 2 — Shell Bash e Comandi Unix

---

### Cap. 3 — La Shell Bash
**⏱️ Tempo: ~1.5h**

#### 📖 Obiettivi
- Capire il ciclo Read-Eval-Print della shell
- Conoscere variabili, espansioni e quoting
- Saper usare la redirezione dell'I/O (`>`, `>>`, `<`, `2>`, `|`)
- Conoscere il concetto di subshell

#### 🃏 Flashcard Anki (Cap. 3)

| Fronte | Retro |
|--------|-------|
| Cosa fa la shell? | Legge comandi dall'utente, li interpreta ed esegue (Read-Eval-Print Loop). È un interprete di comandi. |
| Differenza tra `>` e `>>`? | `>` sovrascrive il file. `>>` aggiunge in coda al file (append). |
| Cosa fa `2>&1`? | Redirige lo stderr (fd 2) sullo stesso posto dove va lo stdout (fd 1). |
| Differenza tra virgolette singole `'` e doppie `"`? | Single quotes: nessuna espansione, tutto letterale. Double quotes: permette espansioni di variabili `$VAR` e command substitution `$(cmd)`. |
| Come si accede al valore di una variabile? | Con il prefisso `$`: es. `$NOME` oppure `${NOME}`. |
| Cosa fa `$(comando)`? | Command substitution: esegue `comando` e sostituisce l'output al suo posto. |
| Cos'è una subshell? | Un processo figlio della shell corrente. Le variabili modificate nella subshell non impattano la shell padre. |
| Come si esportano variabili ai processi figli? | Con il comando `export NOME_VARIABILE`. |

#### ❓ Domande di Autoverifica

1. Cosa fa `ls -l 2>/dev/null`? Perché è utile?
2. Qual è la differenza tra `echo $HOME` e `echo '$HOME'`?
3. Come si fa a salvare l'output di un comando in una variabile?
4. Cosa fa la pipe `|`? Come è implementata internamente con fork/pipe?
5. Come si combina stdout e stderr in un unico file?

---

### Cap. 4 — Comandi Unix Fondamentali
**⏱️ Tempo: ~1.5h**

#### 📖 Obiettivi
- Conoscere e saper usare i comandi essenziali Unix
- Saper navigare il filesystem da riga di comando
- Gestire file e directory, permessi, processi

#### 🃏 Flashcard Anki (Cap. 4)

| Fronte | Retro |
|--------|-------|
| Differenza tra `cp`, `mv` e `rm`? | `cp`: copia file. `mv`: sposta/rinomina file. `rm`: elimina file. |
| Cosa fa `find . -name "*.c"`? | Cerca ricorsivamente nella directory corrente tutti i file con estensione `.c`. |
| Cosa fa `wc -l file.txt`? | Conta il numero di righe nel file. |
| Differenza tra `head` e `tail`? | `head -n 10`: prime 10 righe. `tail -n 10`: ultime 10 righe. `tail -f`: segue il file in tempo reale. |
| Cosa fa `sort -r file.txt`? | Ordina le righe del file in ordine inverso (decrescente). |
| Cosa fa `uniq`? | Rimuove righe duplicate consecutive. Usato dopo `sort`. |
| Cosa fa `ps aux`? | Mostra tutti i processi in esecuzione con dettagli (utente, PID, CPU%, MEM%, comando). |
| Cosa fa `kill -9 PID`? | Invia il segnale SIGKILL al processo con quel PID. Terminazione forzata e immediata. |
| Cosa fa `chmod +x file`? | Aggiunge il permesso di esecuzione (x) per tutti (proprietario, gruppo, altri). |
| Cosa fa `ln -s origine link`? | Crea un symbolic link (collegamento simbolico) che punta a `origine`. |

#### ❓ Domande di Autoverifica

1. Come trovi tutti i file `.txt` nella home directory modificati negli ultimi 7 giorni?
2. Come ordini l'output di `ls -l` per dimensione di file?
3. Qual è la differenza tra `kill` e `killall`?
4. Come si fa a vedere il manuale di una system call (es. `fork`)?
5. Cosa fa `chmod 4755`? (il primo 4 cosa indica?)

---

## 📆 GIORNO 3 — Grep, Regex e Script Shell

---

### Cap. 5 — Grep e le Espressioni Regolari
**⏱️ Tempo: ~2h**

#### 📖 Obiettivi
- Usare `grep`, `grep -E` (ERE) e `grep -P` (PCRE)
- Costruire pattern regex: ancore, classi, quantificatori, gruppi
- Distinguere BRE (Basic), ERE (Extended) e PCRE

#### 🃏 Flashcard Anki (Cap. 5)

| Fronte | Retro |
|--------|-------|
| Cosa fa `grep -n "pattern" file`? | Stampa le righe che matchano il pattern, precedute dal numero di riga. |
| Cosa fa `grep -v "pattern"`? | Stampa le righe che NON matchano il pattern (inverso). |
| Cosa fa `grep -r "pattern" dir/`? | Cerca il pattern ricorsivamente in tutte le sottodirectory. |
| Cosa significa `^` in una regex? | Inizio della riga (ancora). Es: `^ciao` matcha solo righe che iniziano con "ciao". |
| Cosa significa `$` in una regex? | Fine della riga. Es: `ciao$` matcha solo righe che finiscono con "ciao". |
| Cosa significa `.` in una regex? | Qualsiasi carattere singolo (tranne newline). |
| Cosa significa `*` in una regex? | Zero o più occorrenze del carattere/gruppo precedente. |
| Cosa significa `+` in ERE? | Una o più occorrenze. Richiede `grep -E` o `egrep`. |
| Cosa significa `[a-z]` in una regex? | Classe di caratteri: qualsiasi lettera minuscola dall'a alla z. |
| Cosa significa `[^0-9]`? | Classe negata: qualsiasi carattere che NON è una cifra. |
| Cosa significa `\b` in PCRE? | Word boundary: confine di parola. Es: `\bcat\b` matcha "cat" ma non "catch". |
| Differenza tra `grep`, `egrep` e `grep -P`? | `grep`: BRE. `egrep`/`grep -E`: ERE (extended, `+`, `?`, `|` senza escape). `grep -P`: PCRE (lookahead, lookbehind, `\d`, `\w`, ecc.). |

#### ❓ Domande di Autoverifica

1. Scrivi un comando grep che trovi tutte le righe contenenti un indirizzo email.
2. Spiega la differenza tra `a*` e `a+` e `a?`.
3. Come si cerca una stringa che contiene caratteri speciali regex (es. un punto letterale)?
4. Cosa fa `grep -c "error" file.log`?
5. Costruisci un pattern regex che valida un numero di telefono italiano (es. 333-1234567).

#### 💻 Esercizi Pratici

```bash
# Esercizio 1: Trova tutti gli utenti con shell bash in /etc/passwd
grep "/bin/bash" /etc/passwd

# Esercizio 2: Conta quante volte appare "error" (case insensitive) in un log
grep -ic "error" /var/log/syslog

# Esercizio 3: Stampa solo gli indirizzi IP trovati in un file
grep -Eo "[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}" file.txt

# Esercizio 4: Trova righe che iniziano con una maiuscola e finiscono con un punto
grep -E "^[A-Z].*\.$" file.txt
```

---

### Cap. 6 — Script Shell
**⏱️ Tempo: ~1.5h**

#### 📖 Obiettivi
- Scrivere script bash con variabili, cicli e condizionali
- Usare `if`, `while`, `for`, `case`
- Gestire argomenti dello script (`$1`, `$#`, `$@`)
- Conoscere i codici di uscita (`$?`)

#### 🃏 Flashcard Anki (Cap. 6)

| Fronte | Retro |
|--------|-------|
| Cosa è `$1`, `$2` in uno script? | Gli argomenti posizionali: `$1` è il primo argomento, `$2` il secondo, ecc. |
| Cosa contiene `$#`? | Il numero totale di argomenti passati allo script. |
| Cosa contiene `$@`? | Tutti gli argomenti come lista separata (ciascuno come parola distinta). |
| Cosa contiene `$?`? | Il codice di uscita dell'ultimo comando eseguito. 0 = successo, diverso da 0 = errore. |
| Cosa contiene `$$`? | Il PID della shell che sta eseguendo lo script. |
| Sintassi di un if in bash? | `if [ condizione ]; then ... elif [ ... ]; then ... else ... fi` |
| Come si confrontano numeri in bash? | Con `-eq`, `-ne`, `-lt`, `-le`, `-gt`, `-ge`. Es: `[ $a -gt $b ]`. |
| Come si confrontano stringhe in bash? | Con `=`, `!=`, `-z` (stringa vuota), `-n` (stringa non vuota). Es: `[ "$a" = "$b" ]`. |
| Sintassi del ciclo for in bash? | `for var in lista; do ... done` oppure `for ((i=0; i<10; i++)); do ... done` |
| Come si controlla se un file esiste? | `if [ -f file.txt ]; then ...` (-f: file regolare, -d: directory, -e: qualsiasi). |
| Cosa fa `#!/bin/bash` alla prima riga? | È lo shebang: indica al sistema operativo che questo file va eseguito con `/bin/bash`. |

#### ❓ Domande di Autoverifica

1. Scrivi uno script che conta il numero di file `.c` in una directory passata come argomento.
2. Qual è la differenza tra `[ ]` e `[[ ]]` in bash?
3. Come si fa a rendere uno script eseguibile?
4. Cosa fa `set -e` all'inizio di uno script?
5. Come si definisce e chiama una funzione in uno script bash?

#### 💻 Esercizi Pratici

```bash
#!/bin/bash
# Esercizio 1: Verifica se un numero è pari o dispari
if [ $# -ne 1 ]; then
    echo "Uso: $0 <numero>"; exit 1
fi
if [ $(($1 % 2)) -eq 0 ]; then
    echo "$1 è pari"
else
    echo "$1 è dispari"
fi

#!/bin/bash
# Esercizio 2: Somma da 1 a N
N=$1; sum=0
for ((i=1; i<=N; i++)); do sum=$((sum + i)); done
echo "Somma da 1 a $N = $sum"
```

---

## 📆 GIORNO 4 — Sed, Awk e Funzioni Bash

---

### Cap. 7 — Sed e Awk
**⏱️ Tempo: ~2h**

#### 📖 Obiettivi
- Usare `sed` per sostituzioni, cancellazioni e selezioni di righe
- Usare `awk` per elaborare campi di testo strutturati
- Costruire pipeline con sed e awk

#### 🃏 Flashcard Anki (Cap. 7)

| Fronte | Retro |
|--------|-------|
| Cosa fa `sed 's/old/new/g' file`? | Sostituisce **tutte** le occorrenze di "old" con "new" in ogni riga. Senza `g` sostituisce solo la prima. |
| Cosa fa `sed -i 's/old/new/g' file`? | Modifica il file in-place (sovrascrive il file originale). |
| Cosa fa `sed '2,5d' file`? | Cancella le righe dalla 2 alla 5. |
| Cosa fa `sed -n '3,7p' file`? | Stampa solo le righe dalla 3 alla 7 (`-n` sopprime output di default). |
| Cosa sono `$1`, `$2` in awk? | I campi della riga corrente separati dal delimitatore (default: spazio/tab). `$0` è l'intera riga. |
| Cosa fa `awk '{print $2}' file`? | Stampa il secondo campo di ogni riga. |
| Cosa fa `awk -F: '{print $1}' /etc/passwd`? | Usa `:` come delimitatore e stampa il primo campo (nomi utenti). |
| Cosa fanno i blocchi `BEGIN` e `END` in awk? | `BEGIN`: eseguito prima di leggere qualsiasi riga. `END`: eseguito dopo aver letto tutte le righe. |
| Come si filtra con awk? | `awk '/pattern/ {azione}'` oppure `awk '$3 > 100 {print}'`. |
| Cosa fa `awk '{sum += $1} END {print sum}'`? | Somma tutti i valori del primo campo e stampa il totale alla fine. |

#### ❓ Domande di Autoverifica

1. Come sostituisci con sed solo la prima occorrenza per riga vs tutte?
2. Scrivi un comando awk che stampi la media del terzo campo di un file CSV.
3. Qual è la differenza tra `sed` e `awk` in termini di casi d'uso?
4. Come si usa sed per cancellare le righe vuote da un file?
5. Come si usa awk per contare le righe che soddisfano una condizione?

#### 💻 Esercizi Pratici

```bash
# Esercizio 1: Stampa nomi utenti e shell da /etc/passwd
awk -F: '{print $1, $NF}' /etc/passwd

# Esercizio 2: Conta righe dove UID > 1000
awk -F: '$3 > 1000 {count++} END {print count}' /etc/passwd

# Esercizio 3: Sostituisci "localhost" con "127.0.0.1"
sed -i 's/localhost/127.0.0.1/g' config.txt

# Esercizio 4: Stampa numero di parole per ogni riga
awk '{print NR": "NF" parole: "$0}' file.txt
```

---

### Cap. 8 — Funzioni in Bash
**⏱️ Tempo: ~1h**

#### 🃏 Flashcard Anki (Cap. 8)

| Fronte | Retro |
|--------|-------|
| Come si definisce una funzione in bash? | `function nome() { ... }` oppure `nome() { ... }` |
| Come si accede agli argomenti di una funzione bash? | Con `$1`, `$2`, ecc. (stessi parametri posizionali dello script). |
| Come si restituisce un valore da una funzione bash? | Con `return N` (solo numeri 0-255, codice uscita). Per valori complessi usa `echo` + command substitution. |
| Cosa significa `local` in una funzione bash? | Dichiara una variabile locale alla funzione. Senza `local`, la variabile è globale. |
| Come si cattura l'output di una funzione? | `risultato=$(nome_funzione argomenti)` |

#### 💻 Esercizi Pratici

```bash
# Funzione ricorsiva per il fattoriale
fattoriale() {
    local n=$1
    if [ $n -le 1 ]; then echo 1; return; fi
    local prev=$(fattoriale $((n-1)))
    echo $((n * prev))
}
echo "5! = $(fattoriale 5)"
```

---

## 📆 GIORNO 5 — Compilazione C e I/O Basso Livello

---

### Cap. 9 — Compilazione C e GCC
**⏱️ Tempo: ~1.5h**

#### 📖 Obiettivi
- Conoscere le fasi di compilazione (preprocessing, compilazione, assembly, linking)
- Usare le opzioni principali di GCC
- Creare e usare Makefile
- Usare GDB per il debugging

#### 🃏 Flashcard Anki (Cap. 9)

| Fronte | Retro |
|--------|-------|
| Quali sono le 4 fasi della compilazione C? | 1. **Preprocessing** (`.c` → `.i`): espande macro e include. 2. **Compilazione** (`.i` → `.s`): genera assembly. 3. **Assembly** (`.s` → `.o`): codice oggetto. 4. **Linking** (`.o` → eseguibile): risolve simboli. |
| Cosa fa `gcc -Wall -o output file.c`? | Compila `file.c` abilitando tutti i warning e salva l'eseguibile in `output`. |
| Cosa fa `gcc -g file.c`? | Aggiunge informazioni di debugging (per usare GDB). |
| Cosa fa `gcc -O2 file.c`? | Abilita ottimizzazioni di livello 2. |
| Cosa fa `gcc -c file.c`? | Compila e assembla senza linkare. Produce solo il file oggetto `.o`. |
| Cos'è un Makefile? | Un file che descrive dipendenze e regole per compilare un progetto. Usato con `make`. |
| Sintassi di una regola Makefile? | `target: dipendenze` (riga 1), poi `TAB comando` (riga 2). Il TAB è obbligatorio! |
| Come si avvia GDB? | `gdb ./programma` poi `run`, `break main`, `next`/`step`, `print var`. |

#### ❓ Domande di Autoverifica

1. Perché si usano i file header `.h`?
2. Cosa succede se includi lo stesso header due volte? Come si previene?
3. Cosa fa `-lm` in `gcc file.c -lm`?
4. Cos'è la variabile `$@` in un Makefile?
5. Quando conviene usare una libreria statica vs condivisa (`.so`)?

---

### Cap. 10 — I/O di Basso Livello (System Call)
**⏱️ Tempo: ~2.5h**

#### 📖 Obiettivi
- Usare le system call: `open`, `read`, `write`, `close`, `lseek`
- Capire la differenza tra I/O di basso livello e I/O su stream (`FILE*`)
- Capire `dup` e `dup2` e la ridirezione dell'I/O
- Conoscere la struttura `stat`

#### 🃏 Flashcard Anki (Cap. 10)

| Fronte | Retro |
|--------|-------|
| Prototipo di `open()`? | `int open(const char *pathname, int flags, mode_t mode)` → ritorna fd o -1. |
| Cosa fa il flag `O_CREAT` in `open()`? | Crea il file se non esiste. Richiede il parametro `mode` per i permessi. |
| Differenza tra `O_TRUNC` e `O_APPEND`? | `O_TRUNC`: tronca il file a 0 byte all'apertura. `O_APPEND`: sposta il puntatore alla fine prima di ogni `write`. |
| Prototipo di `read()`? | `ssize_t read(int fd, void *buf, size_t count)` → byte letti, 0=EOF, -1=errore. |
| Prototipo di `write()`? | `ssize_t write(int fd, const void *buf, size_t count)` → byte scritti o -1. |
| Cosa fa `lseek(fd, 0, SEEK_END)`? | Sposta il puntatore alla fine del file. Ritorna la dimensione. |
| Differenza tra `dup` e `dup2`? | `dup(fd)`: duplica fd usando il numero più basso disponibile. `dup2(fd, newfd)`: duplica fd specificando esattamente `newfd` (chiude newfd se già aperto). |
| Come si redirige stdout su un file con dup2? | `int fd = open("file", O_WRONLY...); dup2(fd, 1); close(fd);` ora stdout va nel file. |
| Cosa fa `fstat(fd, &statbuf)`? | Riempie la struttura `stat` con i metadati del file puntato da `fd`. |

#### 💻 Esercizi Pratici

```c
/* Esercizio 1: Copia un file usando open/read/write */
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    int src = open(argv[1], O_RDONLY);
    int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    char buf[BUF_SIZE];
    ssize_t n;
    while ((n = read(src, buf, BUF_SIZE)) > 0)
        write(dst, buf, n);
    close(src); close(dst);
    return 0;
}

/* Esercizio 2: Redirige stdout su file, poi ripristina */
int saved = dup(1);
int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd, 1);
printf("Questo va nel file!\n");
fflush(stdout);
dup2(saved, 1);    // ripristina stdout
close(fd); close(saved);
printf("Questo va su schermo!\n");
```

---

## 📆 GIORNO 6 — Processi Unix ⭐ (Argomento Centrale)

---

### Cap. 11 — Processi Unix
**⏱️ Tempo: ~4h**

#### 📖 Obiettivi
- Spiegare cos'è un processo e le sue proprietà
- Usare `fork()` correttamente e capire il valore di ritorno
- Usare la famiglia `exec()` e capire le varianti (l/v, p, e)
- Gestire zombie e orfani con `wait()` e `waitpid()`
- Capire `vfork()`, `system()` e variabili d'ambiente (`getenv`/`putenv`)

#### 🃏 Flashcard Anki (Cap. 11)

| Fronte | Retro |
|--------|-------|
| Cos'è un processo? | Un programma in esecuzione con il suo spazio di indirizzamento, stack, heap, file descriptor, registri CPU. |
| Come si ottiene il PID corrente? | `pid_t pid = getpid();` |
| Come si ottiene il PID del padre? | `pid_t ppid = getppid();` |
| Cosa restituisce `fork()` al processo padre? | Il PID del figlio appena creato (intero > 0). |
| Cosa restituisce `fork()` al processo figlio? | Sempre **0**. |
| Cosa restituisce `fork()` in caso di errore? | **-1** (nessun figlio viene creato). |
| Cosa condividono padre e figlio dopo fork()? | Il codice, i file descriptor aperti, variabili d'ambiente, working directory. |
| Cosa NON condividono dopo fork()? | Le variabili (copie indipendenti). Modifiche di un processo non sono visibili all'altro. |
| Cosa fa `exec()`? | Sostituisce il programma del processo corrente con un nuovo programma. Il PID non cambia. |
| Differenza tra `execl` e `execv`? | `execl`: argomenti come lista variabile terminata da NULL. `execv`: argomenti come array `argv[]`. |
| Cosa significa la 'p' in `execlp`/`execvp`? | Cerca il programma nelle directory elencate nella variabile d'ambiente `PATH`. |
| Cos'è un processo zombie? | Un processo figlio terminato prima che il padre abbia chiamato `wait()`. Il kernel ne mantiene l'exit status. |
| Cos'è un processo orfano? | Un figlio il cui padre è terminato. Viene adottato da `init` (PID 1). |
| Cosa fa `wait(NULL)`? | Il padre si blocca finché uno qualsiasi dei figli non termina. Previene i processi zombie. |
| Differenza tra `exit()` e `_exit()`? | `exit()`: flush buffer I/O, chiude stream, esegue exit handler, poi chiama `_exit()`. `_exit()`: ritorna subito al kernel. Nel figlio dopo fork si usa `_exit()`. |
| Cosa fa `getenv("PATH")`? | Restituisce il valore della variabile d'ambiente PATH come stringa, oppure NULL se non esiste. |
| Cosa fa `putenv("NOME=valore")`? | Aggiunge/modifica una variabile d'ambiente per il processo corrente e i suoi figli. |

#### ❓ Domande di Autoverifica

1. Disegna il flusso di esecuzione di un programma che fa `fork()` + `exec()`.
2. Cosa succede se un padre non chiama mai `wait()`? Come si risolve?
3. Perché nel figlio si usa `_exit()` invece di `exit()` dopo `exec()` fallita?
4. Cosa significa "Copy-On-Write" in relazione a `fork()`?
5. Cosa fa `system("ls -l")`? Come si implementa internamente?
6. Spiega la differenza tra `vfork()` e `fork()` e quando si usa `vfork()`.
7. Perché non si devono usare variabili locali (stack) con `putenv()`?

#### 💻 Esercizi Pratici

```c
/* Esercizio 1: N figli in parallelo, padre aspetta tutti */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int N = 5;
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Figlio %d, PID=%d\n", i, getpid());
            _exit(i);
        }
    }
    int status;
    pid_t pid;
    while ((pid = wait(&status)) > 0)
        printf("Figlio %d terminato con exit=%d\n", pid, WEXITSTATUS(status));
    return 0;
}

/* Esercizio 2: Fork + exec — esegui "ls -l" come figlio */
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid = fork();
    if (pid == 0) {
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("exec fallita");
        _exit(1);
    }
    wait(NULL);
    printf("ls completato\n");
    return 0;
}

/* Esercizio 3: Pipeline manuale "ls | wc -l" */
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);
    if (fork() == 0) {      // figlio 1: ls
        close(fd[0]);
        dup2(fd[1], 1);     // stdout -> pipe
        close(fd[1]);
        execlp("ls", "ls", NULL);
        _exit(1);
    }
    if (fork() == 0) {      // figlio 2: wc -l
        close(fd[1]);
        dup2(fd[0], 0);     // stdin <- pipe
        close(fd[0]);
        execlp("wc", "wc", "-l", NULL);
        _exit(1);
    }
    close(fd[0]); close(fd[1]);
    wait(NULL); wait(NULL);
    return 0;
}
```

---

## 📆 GIORNO 7 — Segnali

---

### Cap. 12 — Segnali
**⏱️ Tempo: ~3h**

#### 📖 Obiettivi
- Capire cos'è un segnale e il suo ciclo di vita (generazione, consegna, disposizione)
- Conoscere i segnali principali
- Usare `signal()` e `sigaction()` per intercettare segnali
- Usare `kill()`, `raise()`, `alarm()`, `pause()`

#### 🃏 Flashcard Anki (Cap. 12)

| Fronte | Retro |
|--------|-------|
| Cos'è un segnale? | Un interrupt software usato per notificare un processo di un evento asincrono. |
| Cosa significa "segnale pending"? | Un segnale generato ma non ancora consegnato al processo. |
| Cosa significa "segnale mascherato/blocked"? | Un segnale temporaneamente bloccato: rimane pending finché non viene sbloccato. |
| Cosa fa `SIGKILL`? | Termina immediatamente il processo. **Non può essere intercettato né ignorato.** |
| Cosa fa `SIGSTOP`? | Sospende il processo. **Non può essere intercettato né ignorato.** |
| Cosa fa `SIGTERM`? | Richiede la terminazione. Può essere intercettato per una chiusura pulita. |
| Cosa fa `SIGCHLD`? | Inviato al padre quando un figlio termina o si ferma. |
| Cosa fa `SIGPIPE`? | Inviato quando si tenta di scrivere su una pipe il cui lettore è chiuso. |
| Cosa fa `SIGALRM`? | Inviato quando scade un timer impostato con `alarm()`. |
| Cosa fa `SIG_IGN` come handler? | Ignora il segnale (tranne SIGKILL e SIGSTOP). |
| Cosa fa `SIG_DFL` come handler? | Ripristina la disposizione di default del segnale. |
| Cosa fa `kill(pid, sig)`? | Invia il segnale `sig` al processo con PID `pid`. |
| Cosa fa `alarm(n)`? | Schedula l'invio di SIGALRM dopo `n` secondi. `alarm(0)` cancella il timer. |
| Cosa fa `pause()`? | Sospende il processo finché non arriva un segnale. |
| Perché preferire `sigaction()` a `signal()`? | Comportamento portabile e definito, permette di specificare maschera segnali durante handler e flag. |

#### ❓ Domande di Autoverifica

1. Perché non si possono intercettare SIGKILL e SIGSTOP?
2. Cosa succede se un segnale viene inviato mentre il processo è bloccato in `read()`?
3. Qual è il rischio di usare `printf()` dentro un signal handler?
4. Come si fa a inviare un segnale a tutti i processi di un gruppo?
5. Cosa fa `SA_RESTART` in `sigaction`?

#### 💻 Esercizi Pratici

```c
/* Esercizio 1: Intercetta Ctrl+C (SIGINT) per chiusura pulita */
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

volatile sig_atomic_t running = 1;

void handler(int sig) {
    running = 0;
}

int main() {
    signal(SIGINT, handler);
    printf("Premi Ctrl+C per uscire...\n");
    while (running) { sleep(1); printf("In esecuzione...\n"); }
    printf("Uscita pulita!\n");
    return 0;
}

/* Esercizio 2: Uso di sigaction per gestire SIGTERM */
#include <signal.h>
#include <stdio.h>
#include <string.h>

void handler(int sig) { /* write() è async-signal-safe, printf NON lo è */
    write(1, "Ricevuto SIGTERM!\n", 18);
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGTERM, &sa, NULL);
    pause();
    return 0;
}
```

---

## 📆 GIORNO 8 — IPC: Pipe, FIFO e mmap

---

### Cap. 13 — IPC: Pipe, FIFO e Memoria Condivisa
**⏱️ Tempo: ~4h**

#### 🃏 Flashcard Anki (Cap. 13)

| Fronte | Retro |
|--------|-------|
| Cos'è una pipe? | Un canale di comunicazione **unidirezionale** tra processi correlati. Dati in ordine FIFO. |
| Prototipo di `pipe()`? | `int pipe(int fd[2])` → `fd[0]` lettura, `fd[1]` scrittura. |
| Cosa succede se leggi da pipe vuota con lato scrittura chiuso? | `read()` ritorna 0 (EOF). |
| Cosa succede se scrivi su pipe con lato lettura chiuso? | Il processo riceve SIGPIPE (oppure write() ritorna -1 con errno=EPIPE). |
| Cos'è una FIFO (named pipe)? | Una pipe con un nome nel filesystem, che permette comunicazione tra processi non correlati. |
| Come si crea una FIFO da shell? | `mkfifo nome_fifo` |
| Come si crea una FIFO in C? | `mkfifo("nome_fifo", 0644)` poi `open()` con O_RDONLY o O_WRONLY. |
| Cosa fa `mmap()`? | Mappa un file (o memoria anonima) nello spazio di indirizzi del processo. |
| Come si usa mmap per memoria condivisa tra padre e figlio? | `mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0)` prima della fork. |
| Differenza tra `MAP_SHARED` e `MAP_PRIVATE`? | `MAP_SHARED`: modifiche visibili ad altri. `MAP_PRIVATE`: copy-on-write, modifiche locali. |
| Cos'è `PIPE_BUF`? | Dimensione massima di scrittura atomica su pipe (almeno 512B, solitamente 4096B). |

#### ❓ Domande di Autoverifica

1. Perché dopo `fork()` bisogna chiudere il lato della pipe che non si usa?
2. Come implementeresti una pipeline `cmd1 | cmd2 | cmd3` con pipe e fork?
3. Qual è la differenza tra pipe anonima e FIFO in termini di utilizzo?
4. Quando conviene usare `mmap()` invece di `read()`/`write()` per un file?

#### 💻 Esercizi Pratici

```c
/* Esercizio 1: Comunicazione padre-figlio tramite pipe */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd[2]; pipe(fd);
    if (fork() == 0) {
        close(fd[0]);
        char msg[] = "Ciao padre!";
        write(fd[1], msg, strlen(msg)+1);
        close(fd[1]); _exit(0);
    }
    close(fd[1]);
    char buf[100];
    read(fd[0], buf, sizeof(buf));
    printf("Padre riceve: %s\n", buf);
    close(fd[0]); wait(NULL);
    return 0;
}

/* Esercizio 2: Memoria condivisa con mmap */
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int *shared = mmap(NULL, sizeof(int),
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *shared = 0;
    if (fork() == 0) { *shared = 42; _exit(0); }
    wait(NULL);
    printf("Valore condiviso: %d\n", *shared); // 42
    munmap(shared, sizeof(int));
    return 0;
}
```

---

## 📆 GIORNO 9 — Thread e Sincronizzazione

---

### Cap. 14 — Thread e Concorrenza
**⏱️ Tempo: ~2.5h**

#### 🃏 Flashcard Anki (Cap. 14)

| Fronte | Retro |
|--------|-------|
| Differenza principale tra thread e processo? | Thread: condividono spazio di indirizzi (heap, globali, fd). Processi: spazi separati. Thread più leggeri. |
| Cosa condividono i thread dello stesso processo? | Codice, heap, variabili globali, file descriptor, PID. |
| Cosa è privato per ogni thread? | Stack, registri CPU, thread ID, maschera segnali. |
| Prototipo di `pthread_create()`? | `int pthread_create(pthread_t *tid, attr, void*(*func)(void*), void *arg)` |
| Cosa fa `pthread_join(tid, &retval)`? | Aspetta la terminazione del thread e recupera il valore di ritorno. |
| Cosa fa `pthread_exit(val)`? | Termina il thread corrente restituendo `val`. |
| Cos'è una race condition? | Bug causato da accesso concorrente non sincronizzato a risorsa condivisa. Risultato dipende dalla schedulazione. |
| Come si compila con pthreads? | `gcc file.c -lpthread -o output` |

---

### Cap. 15 — Sincronizzazione: Mutex, CV, Semafori
**⏱️ Tempo: ~2h**

#### 🃏 Flashcard Anki (Cap. 15)

| Fronte | Retro |
|--------|-------|
| Cos'è un mutex? | Lock binario per mutua esclusione. Un solo thread alla volta può acquisirlo. |
| Cosa fa `pthread_mutex_lock()`? | Acquisisce il mutex. Se già preso, il chiamante si blocca. |
| Cos'è una condition variable? | Permette a un thread di aspettare efficientemente che una condizione sia vera, senza busy-waiting. |
| Pattern corretto di `pthread_cond_wait()`? | Sempre dentro un ciclo `while(!condizione)`, con il mutex acquisito. |
| Cosa fa `pthread_cond_signal()`? | Sveglia UNO dei thread in attesa sulla condition variable. |
| Cosa fa `pthread_cond_broadcast()`? | Sveglia TUTTI i thread in attesa sulla condition variable. |
| Cos'è un semaforo? | Contatore atomico: `sem_wait()` decrementa (blocca se 0), `sem_post()` incrementa. |
| Differenza semaforo binario vs mutex? | Il semaforo può essere segnalato da un thread diverso da chi lo ha acquisito (utile per produttore/consumatore). |

#### 💻 Esercizi Pratici

```c
/* 4 thread incrementano un contatore condiviso */
#include <pthread.h>
#include <stdio.h>

int contatore = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *incrementa(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        contatore++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t t[4];
    for (int i = 0; i < 4; i++) pthread_create(&t[i], NULL, incrementa, NULL);
    for (int i = 0; i < 4; i++) pthread_join(t[i], NULL);
    printf("Contatore: %d\n", contatore); // deve essere 400000
    return 0;
}
```

---

## 📆 GIORNO 10 — Problemi Classici e Socket

---

### Cap. 16 — Problemi Classici di Sincronizzazione
**⏱️ Tempo: ~2h**

#### 🃏 Flashcard Anki (Cap. 16)

| Fronte | Retro |
|--------|-------|
| Cos'è il problema Produttore-Consumatore? | Produttore genera dati in buffer condiviso, consumatore li legge. Bisogna sincronizzare accesso e gestire buffer pieno/vuoto. |
| Cos'è il problema dei Filosofi a Cena? | 5 filosofi, 5 bacchette. Ogni filosofo ne prende 2 adiacenti per mangiare. Rischio di deadlock se tutti prendono la sinistra contemporaneamente. |
| Quali sono le 4 condizioni di Coffman per il deadlock? | 1. Mutua esclusione. 2. Hold-and-wait. 3. No preemption. 4. Attesa circolare. |
| Come si previene il deadlock nei filosofi? | Imporre un ordine globale di acquisizione (es. sempre la bacchetta con numero minore per prima). |
| Cos'è starvation? | Situazione in cui un thread non riesce mai ad avanzare perché altri vengono sempre preferiti. |

---

### Cap. 17 — Socket
**⏱️ Tempo: ~2h**

#### 🃏 Flashcard Anki (Cap. 17)

| Fronte | Retro |
|--------|-------|
| Differenza TCP vs UDP? | TCP: connessione, affidabile, ordinato. UDP: connectionless, non affidabile, più veloce. |
| Sequenza system call server TCP? | `socket()` → `bind()` → `listen()` → `accept()` (loop) → `recv()`/`send()` → `close()` |
| Sequenza system call client TCP? | `socket()` → `connect()` → `send()`/`recv()` → `close()` |
| Cosa fa `bind()`? | Associa il socket a un indirizzo IP e porta locale. |
| Cosa fa `listen(backlog)`? | Mette il socket in ascolto. `backlog`: lunghezza coda connessioni pending. |
| Cosa fa `accept()`? | Si blocca aspettando una connessione. Restituisce un NUOVO socket descriptor per quella connessione. |
| Cosa fa `htons(porta)`? | Converte la porta da host byte order a network byte order (big-endian). |
| Cosa fa `inet_pton(AF_INET, "127.0.0.1", &addr)`? | Converte un IP in formato stringa a formato binario. |
| Cosa è `SO_REUSEADDR`? | Opzione socket che permette di riusare la porta subito dopo la chiusura (evita "Address already in use"). |

#### 💻 Esercizi Pratici

```c
/* Server TCP minimale sulla porta 8080 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int srv = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(8080),
        .sin_addr.s_addr = INADDR_ANY
    };
    bind(srv, (struct sockaddr*)&addr, sizeof(addr));
    listen(srv, 5);
    printf("Server in ascolto su :8080...\n");
    
    while (1) {
        int cli = accept(srv, NULL, NULL);
        char buf[256];
        int n = recv(cli, buf, sizeof(buf)-1, 0);
        buf[n] = '\0';
        printf("Ricevuto: %s\n", buf);
        send(cli, "OK\n", 3, 0);
        close(cli);
    }
}
```

---

## 📆 GIORNO 11 — I/O Multiplexing e Segnali Avanzati

---

### Cap. 18 — I/O Multiplexing con `select()`
**⏱️ Tempo: ~1.5h**

#### 🃏 Flashcard Anki (Cap. 18)

| Fronte | Retro |
|--------|-------|
| Perché si usa `select()`? | Per monitorare più file descriptor contemporaneamente senza thread separati. |
| Cosa fa `FD_ZERO(&set)`? | Azzera il set di file descriptor. |
| Cosa fa `FD_SET(fd, &set)`? | Aggiunge `fd` al set da monitorare. |
| Cosa fa `FD_ISSET(fd, &set)` dopo select? | Controlla se `fd` ha dati disponibili (è pronto). |
| Differenza `select()` vs `poll()` vs `epoll()`? | `select()`: classico, limite FD_SETSIZE (~1024). `poll()`: no limite, O(n). `epoll()`: Linux, scalabile O(1). |

---

### Cap. 19 — Gestione Avanzata dei Segnali (`sigaction`)
**⏱️ Tempo: ~1.5h**

#### 🃏 Flashcard Anki (Cap. 19)

| Fronte | Retro |
|--------|-------|
| Cosa fa `SA_RESTART` in `sigaction`? | Le system call interrotte dal segnale vengono riavviate automaticamente. |
| Come si gestisce SIGPIPE in un server? | `signal(SIGPIPE, SIG_IGN)` per ignorarlo e controllare il return di write (errno=EPIPE). |
| Cosa fa `sigpending()`? | Ritorna la maschera dei segnali pendenti (generati ma ancora bloccati). |

---

## 📆 GIORNO 12 — UDP e Comandi di Rete

---

### Cap. 20-21 — Broadcast, Multicast e Comandi Rete
**⏱️ Tempo: ~2.5h**

#### 🃏 Flashcard Anki (Cap. 20-21)

| Fronte | Retro |
|--------|-------|
| Differenza Unicast/Broadcast/Multicast? | Unicast: 1→1. Broadcast: 1→tutti nella subnet. Multicast: 1→gruppo specifico. |
| Come si abilita il broadcast su socket UDP? | `setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt))` |
| Cosa fa `nslookup dominio`? | Risolve il dominio DNS e mostra l'indirizzo IP. |
| Cosa fa `netstat -tuln`? | Mostra le porte in ascolto (TCP/UDP, numeriche). |
| Cosa fa `tcpdump -i eth0`? | Cattura e mostra i pacchetti di rete sull'interfaccia eth0. |
| Cosa fa `getaddrinfo()`? | Risolve un hostname/servizio in una lista di strutture `addrinfo`. |

---

## 📆 GIORNO 13 — Virtualizzazione e Docker

---

### Cap. 22-24 — Virtualizzazione, Container e Docker
**⏱️ Tempo: ~3h**

#### 🃏 Flashcard Anki (Cap. 22-24)

| Fronte | Retro |
|--------|-------|
| Differenza tra VM e Container? | VM: virtualizza l'hardware, ha un kernel proprio. Container: condivide il kernel host, virtualizza lo user space. Più leggero e veloce. |
| Cos'è un Namespace Linux? | Meccanismo del kernel che isola risorse (PID, rete, filesystem, ecc.) in gruppi separati. Base per i container. |
| Quali tipi di Namespace esistono? | PID, Network, Mount, UTS (hostname), IPC, User. |
| Cos'è un Cgroup? | Meccanismo kernel per limitare e monitorare le risorse (CPU, memoria, I/O) di un gruppo di processi. |
| Cos'è OverlayFS? | Filesystem a strati: strati read-only (image) + strato scrivibile (container). Usato da Docker. |
| Cosa fa `docker run -it ubuntu bash`? | Avvia un container Ubuntu interattivo con una shell bash. |
| Cosa fa `docker build -t nome .`? | Costruisce un'immagine Docker dal `Dockerfile` nella directory corrente. |
| Cos'è un Dockerfile? | File con istruzioni per costruire un'immagine Docker (FROM, RUN, COPY, CMD, EXPOSE, ecc.). |
| Differenza tra `CMD` e `ENTRYPOINT`? | `ENTRYPOINT`: comando fisso sempre eseguito. `CMD`: argomenti default, sovrascrivibili con `docker run`. |
| Cosa fa `docker-compose up`? | Avvia tutti i servizi definiti in `docker-compose.yml`. |
| Cosa fa `docker ps -a`? | Mostra tutti i container (anche quelli fermi). |
| Differenza `docker stop` vs `docker kill`? | `stop`: SIGTERM + attende, poi SIGKILL. `kill`: SIGKILL immediato. |

#### ❓ Domande di Autoverifica

1. Come funziona il Copy-on-Write in OverlayFS?
2. Spiega come un Namespace PID isola i processi di un container.
3. Come si limitano le risorse di un container Docker (CPU, memoria)?
4. Come esponi una porta di un container all'host con Docker?

---

## 📆 GIORNO 14 — Ripasso e Simulazione Esame

---

### ✅ Checklist Finale di Ripasso

Prima dell'esame, verifica di saper rispondere a queste domande senza guardare gli appunti:

#### 🔴 Argomenti Critici (alta probabilità esame)

- [ ] Disegna e spiega il flusso di `fork()` + `exec()` + `wait()`
- [ ] Implementa una pipeline `cmd1 | cmd2` con pipe, fork, dup2
- [ ] Scrivi un server TCP che gestisce più client con `fork()` per ogni client
- [ ] Scrivi un handler per SIGINT con `sigaction()`
- [ ] Usa `select()` per monitorare stdin e un socket
- [ ] Implementa Produttore-Consumatore con mutex e condition variable
- [ ] Scrivi un Makefile per un progetto multi-file
- [ ] Spiega zombie e orfani e come prevenirli

#### 🟡 Argomenti Importanti

- [ ] Differenze tra varianti `exec` (l/v, p, e)
- [ ] `dup` vs `dup2` con un esempio pratico
- [ ] Pipe anonima vs FIFO: differenze
- [ ] Thread vs processo: cosa condividono, cosa no
- [ ] Mutex e deadlock: 4 condizioni di Coffman
- [ ] TCP vs UDP con esempi d'uso reali

#### 🟢 Argomenti di Supporto

- [ ] grep/sed/awk: costruisci pattern regex complessi
- [ ] Docker: build, run, ps, stop, compose
- [ ] Namespace e Cgroups: cosa sono e a cosa servono

---

### 🎯 Simulazione Esame — Esercizi Tipici

```
ESAME TIPICO 1: Server multi-processo
   Implementa un server TCP che:
   1. Accetta connessioni in loop
   2. Per ogni client, crea un processo figlio con fork()
   3. Il figlio legge una riga e la rispedisce in maiuscolo
   4. Il padre gestisce SIGCHLD (WNOHANG) per evitare zombie

ESAME TIPICO 2: Pipeline manuale
   Implementa in C l'equivalente di: cat file.txt | grep "error" | wc -l
   Usando solo pipe(), fork(), dup2(), execlp()

ESAME TIPICO 3: Thread con sincronizzazione
   N thread incrementano un contatore condiviso M volte.
   Verifica che il risultato finale sia N*M.
   Implementa con mutex, poi con semafori.

ESAME TIPICO 4: Script bash
   Scrivi uno script che riceve una directory come argomento,
   trova tutti i file .c, li compila e riporta quanti hanno
   compilato con successo e quanti hanno fallito.
```

---

## 📎 Risorse Rapide

| Risorsa | Dove trovarlo |
|---------|--------------|
| Guida Completa LSO | [Guida_Completa_LSO.md](file:///c:/Users/Paolo/Desktop/Universita/Terzo%20Anno/LSO/Appunti/Guida_Completa_LSO.md) |
| Man Pages Online | https://man7.org/linux/man-pages/ |
| Beej's Guide to Network Programming | https://beej.us/guide/bgnet/ |
| Anki (flashcard app) | https://apps.ankiweb.net/ |

> **Come importare le flashcard su Anki:**
> 1. Copia le tabelle Fronte/Retro in un file `.csv` con `;` come separatore.
> 2. Apri Anki → File → Importa.
> 3. Seleziona il file CSV, mappa i campi su Fronte/Retro.
> 4. Usa mazzi separati per argomento (es. "LSO - Processi", "LSO - Socket").
