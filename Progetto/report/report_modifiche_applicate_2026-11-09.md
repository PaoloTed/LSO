# Report delle Modifiche Applicate al Codice — Progetto Labirinto (LSO)

**Data:** 09/11/2026  
**File modificati:**
1. [`common/protocol.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/common/protocol.c)
2. [`server/game.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/game.c)
3. [`server/server.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/server.c)
4. [`server/log.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/log.c)
5. [`client/client.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c)

---

## 1. [`common/protocol.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/common/protocol.c#L1-L22)

### Motivo della modifica
Sostituisce `write()` bloccante con `send(..., MSG_DONTWAIT)` ([Guida_Completa_LSO.md, Cap. 17.7](file:///home/paolo/Scrivania/Universita/LSO/Materiale/Appunti/Guida_Completa_LSO.md#L4720-L4735)) per evitare che un client lento/bloccato con il buffer TCP pieno blocchi il `timer_thread` o il server.

### Righe modificate ([righe 1–22](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/common/protocol.c#L1-L22))
```diff
--- a/Progetto/labirinto/common/protocol.c
+++ b/Progetto/labirinto/common/protocol.c
@@ -1,4 +1,5 @@
 #include "protocol.h"
+#include <sys/socket.h>
 #include <unistd.h>
 #include <errno.h>
 
@@ -8,7 +9,7 @@ int invia_dati(int fd, const void *buf, size_t len) {
     size_t rimanenti = len;
 
     while (rimanenti > 0) {
-        ssize_t n = write(fd, p, rimanenti);
+        ssize_t n = send(fd, p, rimanenti, MSG_DONTWAIT);
         if (n <= 0) {
             if (n < 0 && errno == EINTR)
                 continue;
```

### Testo completo della funzione aggiornata ([`invia_dati`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/common/protocol.c#L6-L22))
```c
#include "protocol.h"
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

/* Invia len byte gestendo short write */
int invia_dati(int fd, const void *buf, size_t len) {
    const char *p = (const char *)buf;
    size_t rimanenti = len;

    while (rimanenti > 0) {
        ssize_t n = send(fd, p, rimanenti, MSG_DONTWAIT);
        if (n <= 0) {
            if (n < 0 && errno == EINTR)
                continue;
            return -1;
        }
        p += n;
        rimanenti -= n;
    }
    return 0;
}
```

---

## 2. [`server/game.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/game.c)

### 2.1 Ricerca slot libero, spawn senza oggetti e invio `MSG_OK` sotto mutex ([righe 236–290](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/game.c#L236-L290))

#### Motivo della modifica
- **Riga 239:** usa `!partita.giocatori[i].partecipante` al posto di `!partita.giocatori[i].attivo` per non sovrascrivere i risultati (punteggio e stato `uscito`) dei giocatori disconnessi.
- **Righe 268–269:** evita che lo spawn iniziale avvenga su una cella che contiene già un oggetto.
- **Righe 285–288:** invia `MSG_OK` dentro `partita.mutex` appena registrato lo slot, evitando che il `timer_thread` possa inviare una mappa globale prima di `MSG_OK`.

```diff
@@ -236,7 +236,7 @@ int aggiungi_giocatore(int fd, const char *nickname, const char *password,
   /* Cerca slot libero */
   int slot = -1;
   for (int i = 0; i < partita.num_giocatori; i++) {
-    if (!partita.giocatori[i].attivo) {
+    if (!partita.giocatori[i].partecipante) {
       slot = i;
       break;
     }
@@ -265,7 +265,8 @@ int aggiungi_giocatore(int fd, const char *nickname, const char *password,
   do {
     riga = 1 + rand() % (RIGHE_MAPPA - 2);
     colonna = 1 + rand() % (COLONNE_MAPPA - 2);
-  } while (partita.labirinto.celle[riga][colonna] != CELLA_LIBERA);
+  } while (partita.labirinto.celle[riga][colonna] != CELLA_LIBERA ||
+           partita.labirinto.oggetti[riga][colonna]);
 
   Giocatore *p = &partita.giocatori[slot];
   p->fd = fd;
@@ -280,6 +281,11 @@ int aggiungi_giocatore(int fd, const char *nickname, const char *password,
   memset(p->scoperte, 0, sizeof(p->scoperte));
   p->scoperte[riga][colonna] = 1;
 
+  /* Conferma accesso prima che il timer possa inviare mappe */
+  Messaggio ok = {0};
+  ok.tipo = MSG_OK;
+  invia_messaggio(fd, &ok);
+
   pthread_mutex_unlock(&partita.mutex);
```

---

### 2.2 Controllo fine partita in [`muovi_giocatore`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/game.c#L379-L404)

#### Motivo della modifica
Evita di leggere `partita.partita_finita` fuori dal mutex e garantisce che `broadcast_fine_partita()` venga chiamata una sola volta (solo dal thread per cui `termina_partita()` restituisce `1`).

```diff
@@ -373,8 +379,9 @@ void muovi_giocatore(int indice, char direzione) {
   strncpy(nick, p->nickname, sizeof(nick));
   int punteggio = p->punteggio;
 
+  int finita = 0;
   if (raggiunta_uscita && tutti_usciti()) {
-    termina_partita();
+    finita = termina_partita();
   }
   pthread_mutex_unlock(&partita.mutex);
 
@@ -393,7 +400,7 @@ void muovi_giocatore(int indice, char direzione) {
 
   invia_mappa_locale(indice, avviso);
 
-  if (partita.partita_finita)
+  if (finita)
     broadcast_fine_partita();
 }
```

---

### 2.3 Invio protetto da mutex in [`invia_mappa_locale`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/game.c#L429-L450), [`invia_mappa_globale`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/game.c#L452-L483), [`invia_lista_giocatori`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/game.c#L491-L528), [`broadcast_mappe_globali`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/game.c#L530-L538) e [`broadcast_fine_partita`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/game.c#L540-L569)

#### Motivo della modifica
Elimina le data race segnalate da ThreadSanitizer su `partita.num_giocatori` / `partita.giocatori` durante `realloc` e impedisce scritture concorrenti sullo stesso descrittore `fd` o su descrittori già chiusi.

```diff
@@ -438,10 +445,8 @@ void invia_mappa_locale(int indice, const char *avviso) {
   msg.punteggio = p->punteggio;
   componi_vista_locale(p, msg.mappa_locale);
 
-  int fd = p->fd;
+  invia_messaggio(p->fd, &msg);
   pthread_mutex_unlock(&partita.mutex);
-
-  invia_messaggio(fd, &msg);
 }
 
 void invia_mappa_globale(int indice) {
@@ -473,10 +478,8 @@ void invia_mappa_globale(int indice) {
         msg.mappa_globale[r][c] = partita.labirinto.celle[r][c];
     }
   }
-  int fd = p->fd;
+  invia_messaggio(p->fd, &msg);
   pthread_mutex_unlock(&partita.mutex);
-
-  invia_messaggio(fd, &msg);
 }
 
 /* Messaggio MSG_LISTA con lista dei giocatori */
@@ -519,17 +522,18 @@ void invia_lista_giocatori(int indice) {
     ml->lista[k].uscito = partita.giocatori[i].uscito;
     k++;
   }
-  int fd = partita.giocatori[indice].fd;
+  invia_dati(partita.giocatori[indice].fd, ml, dimensione);
   pthread_mutex_unlock(&partita.mutex);
-
-  invia_dati(fd, ml, dimensione);
   free(ml);
 }
 
 void broadcast_mappe_globali(void) {
-  for (int i = 0; i < partita.num_giocatori; i++) {
-    if (partita.giocatori[i].attivo)
-      invia_mappa_globale(i);
+  pthread_mutex_lock(&partita.mutex);
+  int n = partita.num_giocatori;
+  pthread_mutex_unlock(&partita.mutex);
+
+  for (int i = 0; i < n; i++) {
+    invia_mappa_globale(i);
   }
 }
 
@@ -555,23 +559,11 @@ void broadcast_fine_partita(void) {
     log_event("Fine partita. Nessun vincitore.");
   }
 
-  int count = 0;
-  int *fd_giocatori_attivi = malloc(partita.num_giocatori * sizeof(int));
-
-  if (fd_giocatori_attivi) {
-    for (int i = 0; i < partita.num_giocatori; i++) {
-      if (partita.giocatori[i].attivo) {
-        fd_giocatori_attivi[count++] = partita.giocatori[i].fd;
-      }
+  for (int i = 0; i < partita.num_giocatori; i++) {
+    if (partita.giocatori[i].attivo) {
+      invia_messaggio(partita.giocatori[i].fd, &msg);
     }
   }
 
   pthread_mutex_unlock(&partita.mutex);
-
-  if (fd_giocatori_attivi) {
-    for (int i = 0; i < count; i++) {
-      invia_messaggio(fd_giocatori_attivi[i], &msg);
-    }
-    free(fd_giocatori_attivi);
-  }
 }
```

---

## 3. [`server/server.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/server.c)

### Motivo della modifica
1. Rimuove l'invio di `MSG_OK` da `client_handler` (ora eseguito sotto mutex dentro `aggiungi_giocatore()`).
2. In [`timer_thread()`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/server.c#L124-L161), protegge la lettura di `partita.partita_finita` con `partita.mutex` e invia `broadcast_fine_partita()` solo se `termina_partita()` restituisce `1`.
3. Nel ciclo `accept()`, controlla che `malloc(sizeof(int))` non restituisca `NULL` ([righe 235–240](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/server.c#L235-L240)).

### Righe modificate ([righe 101–161](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/server.c#L101-L161) e [233–242](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/server.c#L233-L242))
```diff
--- a/Progetto/labirinto/server/server.c
+++ b/Progetto/labirinto/server/server.c
@@ -102,11 +102,6 @@ static void *client_handler(void *arg) {
     return NULL;
   }
 
-  /* Conferma accesso */
-  Messaggio ok = {0};
-  ok.tipo = MSG_OK;
-  invia_messaggio(client_fd, &ok);
-
   /* Invio mappa iniziale */
   invia_mappa_globale(indice_giocatore);
 
@@ -130,27 +125,36 @@ static void *timer_thread(void *arg) {
   (void)arg;
   int secondi = 0;
 
-  while (!partita.partita_finita) {
+  while (1) {
     sleep(1);
     secondi++;
 
+    pthread_mutex_lock(&partita.mutex);
+    if (partita.partita_finita) {
+      pthread_mutex_unlock(&partita.mutex);
+      break;
+    }
+
     /* Scadenza timeout */
     if (secondi >= partita.timeout) {
-      char log_buf[128];
-      snprintf(log_buf, sizeof(log_buf),
-               "Partita terminata per timeout (%d secondi).", partita.timeout);
-      log_event(log_buf);
-
-      pthread_mutex_lock(&partita.mutex);
-      termina_partita();
+      int finita = termina_partita();
       pthread_mutex_unlock(&partita.mutex);
 
-      broadcast_fine_partita();
-    } else {
-      /* Invio periodico mappa */
-      if (secondi % partita.intervallo == 0) {
-        broadcast_mappe_globali();
+      if (finita) {
+        char log_buf[128];
+        snprintf(log_buf, sizeof(log_buf),
+                 "Partita terminata per timeout (%d secondi).",
+                 partita.timeout);
+        log_event(log_buf);
+        broadcast_fine_partita();
       }
+      break;
+    }
+    pthread_mutex_unlock(&partita.mutex);
+
+    /* Invio periodico mappa */
+    if (secondi % partita.intervallo == 0) {
+      broadcast_mappe_globali();
     }
   }
   return NULL;
@@ -229,6 +233,10 @@ int main(int argc, char *argv[]) {
     }
 
     int *pfd = malloc(sizeof(int));
+    if (!pfd) {
+      close(client_fd);
+      continue;
+    }
     *pfd = client_fd;
 
     pthread_t tid;
```

---

## 4. [`server/log.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/log.c#L28-L37)

### Motivo della modifica
Evita che `write()` legga oltre il buffer `log_line` se `snprintf` restituisce un valore `>= sizeof(log_line)`.

### Righe modificate ([righe 28–37](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/server/log.c#L28-L37))
```diff
--- a/Progetto/labirinto/server/log.c
+++ b/Progetto/labirinto/server/log.c
@@ -28,8 +28,10 @@ void log_event(const char *msg) {
     /* Scrittura su file */
     char log_line[512];
     int n = snprintf(log_line, sizeof(log_line), "[%s] %s\n", ts, msg);
-    if (n > 0)
-        write(log_fd, log_line, n);
+    if (n > 0) {
+        size_t len = (size_t)n < sizeof(log_line) ? (size_t)n : sizeof(log_line) - 1;
+        write(log_fd, log_line, len);
+    }
 
     pthread_mutex_unlock(&log_mutex);
 }
```

---

## 5. [`client/client.c`](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c)

### Motivo della modifica
1. Dichiara `loop` come `static volatile sig_atomic_t loop = 1;` ([riga 17](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c#L17)).
2. Ripristina lo svuotamento della riga (`while ((c = getchar()) != '\n' && c != EOF);`) dopo le `scanf` dei menu e del login ([righe 82–128](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c#L82-L128) e [176–196](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c#L176-L196)), evitando il loop infinito su input non numerici.
3. Controlla `if (fgets(buffer, sizeof(buffer), stdin) == NULL)` nel ciclo `select` ([righe 249–252](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c#L249-L252)) per gestire `Ctrl+D` (`EOF`).

### Righe modificate ([righe 17](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c#L17), [80–128](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c#L80-L128), [176–196](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c#L176-L196), [246–253](file:///home/paolo/Scrivania/Universita/LSO/Progetto/labirinto/client/client.c#L246-L253))
```diff
--- a/Progetto/labirinto/client/client.c
+++ b/Progetto/labirinto/client/client.c
@@ -14,7 +14,7 @@
 #include "display.h"
 #include "protocol.h"
 
-static int loop = 1;
+static volatile sig_atomic_t loop = 1;
 
 static void sigint_handler(int sig) {
   (void)sig;
@@ -79,6 +79,7 @@ int main(int argc, char *argv[]) {
 
   /* Menu iniziale: Accesso, Registrazione o Uscita */
   int scelta = 0;
+  int c;
   while (scelta < 1 || scelta > 3) {
     system("clear");
     printf("\n=== Labirinto ===\n");
@@ -87,6 +88,13 @@ int main(int argc, char *argv[]) {
     printf("3. Esci\n");
     printf("Seleziona un'opzione: ");
     if (scanf("%d", &scelta) != 1) {
+      scelta = 0;
+    }
+    while ((c = getchar()) != '\n' && c != EOF)
+      ;
+    if (c == EOF) {
+      close(sock_fd);
+      return 0;
     }
     if (scelta < 1 || scelta > 3) {
       printf("Scelta non valida. Riprova.\n");
@@ -111,9 +119,13 @@ int main(int argc, char *argv[]) {
 
   printf("Inserisci il tuo nickname: ");
   scanf("%31s", nickname);
+  while ((c = getchar()) != '\n' && c != EOF)
+    ;
 
   printf("Inserisci la tua password: ");
   scanf("%31s", password);
+  while ((c = getchar()) != '\n' && c != EOF)
+    ;
 
   /* Invio richiesta di accesso / registrazione */
   Messaggio msg = {0};
@@ -170,6 +182,13 @@ int main(int argc, char *argv[]) {
     printf("2. Esci\n");
     printf("Seleziona un'opzione: ");
     if (scanf("%d", &scelta_gioco) != 1) {
+      scelta_gioco = 0;
+    }
+    while ((c = getchar()) != '\n' && c != EOF)
+      ;
+    if (c == EOF) {
+      close(sock_fd);
+      return 0;
     }
     if (scelta_gioco < 1 || scelta_gioco > 2) {
       printf("Scelta non valida. Riprova.\n");
@@ -227,7 +246,10 @@ int main(int argc, char *argv[]) {
     /* Input tastiera */
     if (FD_ISSET(STDIN_FILENO, &read_set)) {
       char buffer[64];
-      fgets(buffer, sizeof(buffer), stdin);
+      if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
+        loop = 0;
+        break;
+      }
 
       char comando = tolower(buffer[0]);
       if (uscito) {
```
