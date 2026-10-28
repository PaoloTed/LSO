# Guida alle Correzioni: Protocollo di Rete e Concorrenza
### Progetto LSO — Laboratorio di Sistemi Operativi (Traccia A: Labirinto)

---

## 1. Il Protocollo di Rete: Dal Modello Monolitico a "Header + Payload"

### 1.1 Il Problema Attuale (Overhead e Asimmetria)

Nel progetto corrente (`labirinto/common/protocol.h`), ogni messaggio viaggia tramite una maxi-struttura fissa:
```c
typedef struct {
    int  tipo;
    char nickname[MAX_NICK];
    char password[MAX_PASS];
    char direzione;
    char testo[MAX_TESTO];
    int  riga, colonna, punteggio;
    char mappa_locale[5][5];
    char mappa_globale[21][41];
    int  num_giocatori;
} Messaggio;
```

- **Dimensione fissa:** `sizeof(Messaggio)` = **1104 byte** (incluso il padding di allineamento a 4 byte).
- **Inconveniente 1 (Spreco di banda):** Quando il client invia un semplice comando di movimento come `'w'`, trasmette **1104 byte** per inviare solo 2 byte utili (il tipo e la direzione). Oltre il **99.8% del pacchetto è polistirolo vuoto** (array di mappe azzerati, stringhe vuote, coordinate non usate).
- **Inconveniente 2 (L'accrocchio per la lista giocatori):** Poiché la traccia richiede di non porre limiti a priori al numero di utenti collegabili, una lista di giocatori non può avere una dimensione fissa nota a priori. Nel codice attuale è stata definita in `server/game.c` una struct con *flexible array member*:
  ```c
  typedef struct {
      Messaggio msg;
      InfoGiocatore lista[];
  } MessaggioLista;
  ```
  Questo crea una grave **asimmetria nel protocollo**:
  - Per i messaggi normali il client fa una sola `read` di 1104 byte.
  - Per `MSG_LISTA` il client deve fare prima una `read` di 1104 byte e poi una **seconda `read` disaccoppiata** per recuperare la coda di byte rimasta appesa sul socket TCP.

---

### 1.2 La Soluzione: Il Pattern "Busta + Contenuto" (Header + Payload)

Invece di un unico container fisso per qualsiasi dato, si adotta il pattern canonico dei protocolli applicativi di rete Unix (approccio Type-Length-Value / TLV):

1. **L'Intestazione fissa (`HeaderMsg`):** Pesa **soli 8 byte** ed è presente in testa a ogni pacchetto. Specifica:
   - `tipo` (4 byte): Che tipo di messaggio è (`MSG_MOVIMENTO`, `MSG_LISTA`, `MSG_MAPPA_LOCALE`, ecc.).
   - `lunghezza` (4 byte): Quanti byte di dati seguono immediatamente dopo.
2. **Il Contenuto (`Payload`):** È grande **esattamente quanto serve**.

```
CONFRONTO TRASMISSIONE LISTA GIOCATORI (Esempio con 2 giocatori = 80 byte utili):

PRIMA (Codice Attuale):
[ 1104 byte Messaggio (99% zeri vuoti) ] + [ 80 byte InfoGiocatore ] = 1184 byte

DOPO (Con Header + Payload):
[ 8 byte HeaderMsg ] + [ 80 byte InfoGiocatore ]                     = 88 byte (Risparmio di 1096 byte!)
```

---

### 1.3 Modifiche al Codice: Passo dopo Passo

#### Passo A: Il nuovo `common/protocol.h`

```c
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stddef.h>

#define RIGHE_MAPPA       21
#define COLONNE_MAPPA     41
#define VISTA_LOCALE      5
#define RAGGIO_VISIBILITA (VISTA_LOCALE / 2)

#define MAX_NICK          32
#define MAX_PASS          32
#define MAX_TESTO         128

/* Tipi di Messaggio */
#define MSG_ACCESSO         1
#define MSG_REGISTRAZIONE   2
#define MSG_MOVIMENTO       3
#define MSG_RICHIEDI_LISTA  4
#define MSG_ESCI            5

#define MSG_OK              10
#define MSG_ERRORE          11
#define MSG_MAPPA_LOCALE    12
#define MSG_MAPPA_GLOBALE   13
#define MSG_LISTA           14
#define MSG_FINE_PARTITA    15

/* 1. L'Intestazione Comune (Header) — 8 byte esatti */
typedef struct {
    int tipo;       /* Tipo di messaggio */
    int lunghezza;  /* Dimensione in byte del payload che segue */
} HeaderMsg;

/* 2. Strutture specifiche di Payload */

typedef struct {
    char nickname[MAX_NICK];
    char password[MAX_PASS];
} MsgCredenziali;

typedef struct {
    char direzione; /* 'w', 'a', 's', 'd' */
} MsgMovimento;

typedef struct {
    int  riga;
    int  colonna;
    int  punteggio;
    char vista[VISTA_LOCALE][VISTA_LOCALE];
    char avviso[MAX_TESTO];
} MsgMappaLocale;

typedef struct {
    int  riga;
    int  colonna;
    int  punteggio;
    char griglia[RIGHE_MAPPA][COLONNE_MAPPA];
} MsgMappaGlobale;

typedef struct {
    char nickname[MAX_NICK];
    int  punteggio;
    int  uscito;
} InfoGiocatore;

typedef struct {
    char vincitore[MAX_NICK];
    int  punteggio;
} MsgFinePartita;

/* Funzioni di I/O affidabile su socket (in protocol.c) */
int invia_dati(int fd, const void *buf, size_t len);
int ricevi_dati(int fd, void *buf, size_t len);

#endif
```

---

#### Passo B: Confronto Invio Lista Giocatori (Lato Server)

##### PRIMA (Allocazione mista `MessaggioLista` con 1104 byte sprecati):
```c
/* In server/game.c */
void invia_lista_giocatori(int indice) {
    int totale = ...; // conteggio giocatori attivi
    size_t dimensione = sizeof(MessaggioLista) + totale * sizeof(InfoGiocatore);
    MessaggioLista *ml = malloc(dimensione);
    memset(ml, 0, dimensione);
    ml->msg.tipo = MSG_LISTA;
    ml->msg.num_giocatori = totale;

    for (int i = 0; i < totale; i++) {
        ml->lista[i] = ...;
    }
    invia_dati(fd, ml, dimensione); // Spedisce 1104 + (totale * 40) byte!
    free(ml);
}
```

##### DOPO (Invio snello e lineare):
```c
/* In server/game.c */
void invia_lista_giocatori(int indice) {
    pthread_mutex_lock(&partita.mutex);

    int totale = 0;
    for (int i = 0; i < partita.num_giocatori; i++) {
        if (partita.giocatori[i].attivo) totale++;
    }

    InfoGiocatore *lista = malloc(totale * sizeof(InfoGiocatore));
    if (!lista) {
        pthread_mutex_unlock(&partita.mutex);
        return;
    }

    int k = 0;
    for (int i = 0; i < partita.num_giocatori; i++) {
        if (!partita.giocatori[i].attivo) continue;
        strncpy(lista[k].nickname, partita.giocatori[i].nickname, MAX_NICK - 1);
        lista[k].punteggio = partita.giocatori[i].punteggio;
        lista[k].uscito = partita.giocatori[i].uscito;
        k++;
    }
    int fd = partita.giocatori[indice].fd;
    pthread_mutex_unlock(&partita.mutex);

    /* Prepariamo la busta (8 byte) */
    HeaderMsg hdr;
    hdr.tipo = MSG_LISTA;
    hdr.lunghezza = totale * sizeof(InfoGiocatore);

    /* 1. Invio della sola busta */
    invia_dati(fd, &hdr, sizeof(HeaderMsg));
    /* 2. Invio dei soli dati dei giocatori */
    if (hdr.lunghezza > 0) {
        invia_dati(fd, lista, hdr.lunghezza);
    }

    free(lista);
}
```

---

#### Passo C: Confronto Ricezione Lista Giocatori (Lato Client)

##### PRIMA (Ricezione frammentata e asimmetrica):
```c
/* In client/client.c */
Messaggio msg;
// Legge 1104 byte
ricevi_messaggio(sock_fd, &msg);

switch (msg.tipo) {
    case MSG_LISTA: {
        // Sul socket c'è ancora una coda non letta!
        InfoGiocatore *lista = malloc(msg.num_giocatori * sizeof(InfoGiocatore));
        // Seconda lettura asimmetrica
        ricevi_dati(sock_fd, lista, msg.num_giocatori * sizeof(InfoGiocatore));
        mostra_lista(lista, msg.num_giocatori);
        free(lista);
        break;
    }
}
```

##### DOPO (Ricezione uniforme e prevedibile per qualsiasi messaggio):
```c
/* In client/client.c */
HeaderMsg hdr;

// 1. Si legge SEMPRE prima la busta di 8 byte
if (ricevi_dati(sock_fd, &hdr, sizeof(HeaderMsg)) <= 0) {
    printf("Server disconnesso.\n");
    return;
}

// 2. Si gestisce il contenuto in base a tipo e lunghezza esatti
switch (hdr.tipo) {
    case MSG_LISTA: {
        int num = hdr.lunghezza / sizeof(InfoGiocatore);
        InfoGiocatore *lista = NULL;
        if (hdr.lunghezza > 0) {
            lista = malloc(hdr.lunghezza);
            ricevi_dati(sock_fd, lista, hdr.lunghezza); // Legge esattamente il payload dichiarato
        }
        mostra_lista_giocatori(lista, num);
        free(lista);
        break;
    }

    case MSG_MAPPA_LOCALE: {
        MsgMappaLocale m;
        ricevi_dati(sock_fd, &m, hdr.lunghezza);
        mostra_mappa_locale(m.riga, m.colonna, m.punteggio, m.vista, m.avviso, 1);
        break;
    }

    case MSG_FINE_PARTITA: {
        MsgFinePartita fp;
        ricevi_dati(sock_fd, &fp, hdr.lunghezza);
        printf("\n*** Partita terminata. Vincitore: %s (%d oggetti) ***\n", fp.vincitore, fp.punteggio);
        break;
    }
}
```

---

## 2. Correzione della Concorrenza (Punti 1 e 2)

### 2.1 Eliminare l'I/O di Rete sotto Mutex (`broadcast_fine_partita`)

#### Errore nel codice attuale
Nel file `server/game.c:481-499`, il server tiene acquisito `partita.mutex` mentre esegue il ciclo di `write()` su tutti i client:
```c
pthread_mutex_lock(&partita.mutex);
for (int i = 0; i < partita.num_giocatori; i++) {
    if (partita.giocatori[i].attivo)
        invia_messaggio(partita.giocatori[i].fd, &msg); // ERRORE CRITICO: I/O bloccante con mutex bloccato!
}
pthread_mutex_unlock(&partita.mutex);
```
Se un client è lento o ha una connessione instabile, la `write()` si blocca; il mutex rimane bloccato e tutti gli altri thread del server vanno in deadlock.

#### Codice Corretto (Snapshot locale prima dell'invio):
```c
void broadcast_fine_partita(void) {
    HeaderMsg hdr;
    hdr.tipo = MSG_FINE_PARTITA;
    hdr.lunghezza = sizeof(MsgFinePartita);

    MsgFinePartita fp = {0};

    /* 1. SEZIONE CRITICA BREVE: lettura stato protetta da lock */
    pthread_mutex_lock(&partita.mutex);
    if (partita.vincitore >= 0) {
        int v = partita.vincitore;
        strncpy(fp.vincitore, partita.giocatori[v].nickname, MAX_NICK - 1);
        fp.punteggio = partita.giocatori[v].punteggio;
    }

    /* Copia locale dei soli socket dei client attivi */
    int count = 0;
    int *fds = malloc(partita.num_giocatori * sizeof(int));
    if (fds) {
        for (int i = 0; i < partita.num_giocatori; i++) {
            if (partita.giocatori[i].attivo) {
                fds[count++] = partita.giocatori[i].fd;
            }
        }
    }
    /* RILASCIO IMMEDIATO DEL MUTEX */
    pthread_mutex_unlock(&partita.mutex);

    /* 2. I/O DI RETE COMPLETAMENTE FUORI DAL MUTEX */
    if (fds) {
        for (int i = 0; i < count; i++) {
            invia_dati(fds[i], &hdr, sizeof(HeaderMsg));
            invia_dati(fds[i], &fp, sizeof(MsgFinePartita));
        }
        free(fds);
    }
}
```

---

### 2.2 Evitare che un Client Lento blocchi il Timer Thread

#### Errore nel codice attuale
In `server/server.c:118-140`, il thread timer esegue `sleep(1)` e `secondi++`, ma ogni $T$ secondi chiama `broadcast_mappe_globali()`, che fa scritture TCP sincrone. Se un client non legge, il timer si blocca, i secondi non avanzano e il timeout della partita non scatterà mai.

#### Codice Corretto

1. **Timeout sulle Socket con `SO_SNDTIMEO`:**
   In `server/server.c`, subito dopo l'accettazione della connessione (`accept`):
   ```c
   struct timeval tv = { .tv_sec = 1, .tv_usec = 0 }; // 1 secondo massimo di blocco in scrittura
   setsockopt(client_fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
   ```

2. **Timer basato su Timestamp Assoluto (`time(NULL)`):**
   ```c
   static void *timer_thread(void *arg) {
       (void)arg;
       time_t start_time = time(NULL);
       time_t last_broadcast = start_time;

       while (1) {
           sleep(1);

           pthread_mutex_lock(&partita.mutex);
           if (partita.partita_finita) {
               pthread_mutex_unlock(&partita.mutex);
               break;
           }

           time_t now = time(NULL);
           int trascorso = (int)(now - start_time);

           /* Verifica timeout globale */
           if (trascorso >= partita.timeout) {
               termina_partita();
               pthread_mutex_unlock(&partita.mutex);

               char buf[128];
               snprintf(buf, sizeof(buf), "Fine partita per timeout (%d sec).", partita.timeout);
               log_event(buf);

               broadcast_fine_partita();
               break;
           }

           /* Verifica invio periodico */
           int deve_inviare = 0;
           if (now - last_broadcast >= partita.intervallo) {
               last_broadcast = now;
               deve_inviare = 1;
           }
           pthread_mutex_unlock(&partita.mutex);

           if (deve_inviare) {
               broadcast_mappe_globali();
           }
       }
       return NULL;
   }
   ```

---

## 3. Conformità alla Traccia d'Esame (Punto 4)

### 3.1 Registrazione Preventiva (Traccia, riga 11)
La traccia richiede: *"Per accedere al gioco ogni utente dovrà prima registrarsi al sito indicando password e nickname"*.

In `server/game.c` separare nettamente le due funzioni:
```c
/* Controlla le credenziali in users.txt: 1 se corrette, 0 altrimenti */
int verifica_credenziali(const char *nickname, const char *password) {
    FILE *f = fopen(FILE_UTENTI, "r");
    if (!f) return 0;
    char n[MAX_NICK], p[MAX_PASS];
    int valido = 0;
    while (fscanf(f, "%31s %31s", n, p) == 2) {
        if (strcmp(n, nickname) == 0 && strcmp(p, password) == 0) {
            valido = 1;
            break;
        }
    }
    fclose(f);
    return valido;
}

/* Registra un nuovo utente: 1 successo, 0 se nickname già presente, -1 errore file */
int registra_utente(const char *nickname, const char *password) {
    FILE *f = fopen(FILE_UTENTI, "r");
    if (f) {
        char n[MAX_NICK], p[MAX_PASS];
        while (fscanf(f, "%31s %31s", n, p) == 2) {
            if (strcmp(n, nickname) == 0) {
                fclose(f);
                return 0; /* Nickname già in uso */
            }
        }
        fclose(f);
    }

    f = fopen(FILE_UTENTI, "a");
    if (!f) return -1;
    fprintf(f, "%s %s\n", nickname, password);
    fclose(f);
    return 1;
}
```

---

### 3.2 Conservazione Permanente del Vincitore (Traccia, righe 7-10)
Nel codice attuale, alla disconnessione di un giocatore (`game.c:261`), lo slot viene riusato e sovrascritto azzerando punteggio e stato d'uscita.

**Regola di correzione:**  
1. Quando un nuovo utente si collega, si alloca **sempre un nuovo slot** in fondo all'array `giocatori`, senza riciclare gli slot di chi è uscito.
2. Se un utente con lo stesso nickname si ricollega, viene riagganciato al suo slot preservando il punteggio e lo stato `uscito`.
3. In questo modo, quando scatta il timeout o tutti escono, la funzione `trova_vincitore()` ha a disposizione l'elenco completo e intatto di tutti i partecipanti storici della partita.

