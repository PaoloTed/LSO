#include "game.h"
#include "log.h"
#include "protocol.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define FILE_UTENTI "users.txt"

StatoGioco g_gioco;

/* Generazione labirinto */
static void genera_labirinto(Labirinto *m) {
    /* Celle libere */
    for (int r = 0; r < RIGHE_MAPPA; r++) {
        for (int c = 0; c < COLONNE_MAPPA; c++) {
            m->celle[r][c] = CELLA_LIBERA;
            m->oggetti[r][c] = 0;
        }
    }

    /* Muri perimetrali */
    for (int c = 0; c < COLONNE_MAPPA; c++) {
        m->celle[0][c] = CELLA_MURO;
        m->celle[RIGHE_MAPPA - 1][c] = CELLA_MURO;
    }
    for (int r = 0; r < RIGHE_MAPPA; r++) {
        m->celle[r][0] = CELLA_MURO;
        m->celle[r][COLONNE_MAPPA - 1] = CELLA_MURO;
    }

    /* Ostacoli interni */
    for (int r = 2; r < RIGHE_MAPPA - 2; r += 2) {
        for (int c = 2; c < COLONNE_MAPPA - 2; c += 2) {
            if (rand() % 2 == 0) {
                m->celle[r][c] = CELLA_MURO;
            }
        }
    }

    /* Uscite */
    m->celle[0][COLONNE_MAPPA / 2] = CELLA_USCITA;
    m->celle[RIGHE_MAPPA - 1][COLONNE_MAPPA / 2] = CELLA_USCITA;
    m->celle[RIGHE_MAPPA / 2][0] = CELLA_USCITA;
    m->celle[RIGHE_MAPPA / 2][COLONNE_MAPPA - 1] = CELLA_USCITA;

    /* Oggetti */
    for (int o = 0; o < 15; ) {
        int r = 1 + rand() % (RIGHE_MAPPA - 2);
        int c = 1 + rand() % (COLONNE_MAPPA - 2);
        if (m->celle[r][c] == CELLA_LIBERA && m->oggetti[r][c] == 0) {
            m->oggetti[r][c] = 1;
            o++;
        }
    }
}

/* Verifica credenziali o registrazione */
static int autentica_utente(const char *nickname, const char *password) {
    FILE *f = fopen(FILE_UTENTI, "r");
    char nick_salvato[MAX_NICK];
    char pass_salvata[MAX_PASS];

    if (f != NULL) {
        while (fscanf(f, "%s %s", nick_salvato, pass_salvata) == 2) {
            if (strcmp(nick_salvato, nickname) == 0) {
                fclose(f);
                return (strcmp(pass_salvata, password) == 0);
            }
        }
        fclose(f);
    }

    /* Registrazione utente */
    f = fopen(FILE_UTENTI, "a");
    if (!f)
        return -1;
    fprintf(f, "%s %s\n", nickname, password);
    fflush(f);
    fclose(f);

    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Nuovo utente registrato: '%s'.", nickname);
    log_event(log_buf);

    return 1;
}

void inizializza_gioco(int timeout, int intervallo) {
    srand(time(NULL));
    free(g_gioco.giocatori);
    memset(&g_gioco, 0, sizeof(g_gioco));
    pthread_mutex_init(&g_gioco.mutex, NULL);
    g_gioco.timeout = timeout;
    g_gioco.intervallo = intervallo;
    g_gioco.vincitore = -1;
    genera_labirinto(&g_gioco.labirinto);
}

/* Celle scoperte attorno al giocatore */
static void scopri_dintorni(Giocatore *p) {
    for (int dr = -RAGGIO_VISIBILITA; dr <= RAGGIO_VISIBILITA; dr++) {
        for (int dc = -RAGGIO_VISIBILITA; dc <= RAGGIO_VISIBILITA; dc++) {
            int r = p->riga + dr;
            int c = p->colonna + dc;
            if (r >= 0 && r < RIGHE_MAPPA && c >= 0 && c < COLONNE_MAPPA)
                p->scoperte[r][c] = 1;
        }
    }
}

/* Calcola il vincitore */
static int trova_vincitore_bloccato(void) {
    int conteggio_usciti = 0, indice_uscito = -1;
    for (int i = 0; i < g_gioco.num_giocatori; i++) {
        if (g_gioco.giocatori[i].uscito) {
            conteggio_usciti++;
            indice_uscito = i;
        }
    }

    /* Singolo giocatore uscito */
    if (conteggio_usciti == 1)
        return indice_uscito;

    /* Confronto punteggi */
    int migliore = -1, punteggio_migliore = -1;
    for (int i = 0; i < g_gioco.num_giocatori; i++) {
        Giocatore *p = &g_gioco.giocatori[i];
        if (!p->partecipante)
            continue;
        if (p->punteggio > punteggio_migliore) {
            punteggio_migliore = p->punteggio;
            migliore = i;
        }
    }
    return migliore;
}

/* Controlla se tutti i giocatori partecipanti sono usciti */
static int tutti_usciti_bloccato(void) {
    int almeno_uno = 0;
    for (int i = 0; i < g_gioco.num_giocatori; i++) {
        Giocatore *p = &g_gioco.giocatori[i];
        if (!p->attivo)
            continue;
        almeno_uno = 1;
        if (!p->uscito)
            return 0;
    }
    return almeno_uno;
}

int termina_partita(void) {
    pthread_mutex_lock(&g_gioco.mutex);
    if (g_gioco.partita_finita) {
        pthread_mutex_unlock(&g_gioco.mutex);
        return 0;
    }
    g_gioco.partita_finita = 1;
    g_gioco.vincitore = trova_vincitore_bloccato();
    pthread_mutex_unlock(&g_gioco.mutex);
    return 1;
}

int aggiungi_giocatore(int fd, const char *nickname, const char *password) {
    pthread_mutex_lock(&g_gioco.mutex);

    if (g_gioco.partita_finita) {
        pthread_mutex_unlock(&g_gioco.mutex);
        return -1;
    }

    /* Verifica credenziali */
    int esito_auth = autentica_utente(nickname, password);
    if (esito_auth == 0) {
        pthread_mutex_unlock(&g_gioco.mutex);
        char log_buf[128];
        snprintf(log_buf, sizeof(log_buf), "Tentativo di login fallito per '%s': password errata.", nickname);
        log_event(log_buf);
        return -4; /* Password errata */
    } else if (esito_auth < 0) {
        pthread_mutex_unlock(&g_gioco.mutex);
        return -3;
    }

    /* Controllo nickname */
    for (int i = 0; i < g_gioco.num_giocatori; i++) {
        if (g_gioco.giocatori[i].attivo && strcmp(g_gioco.giocatori[i].nickname, nickname) == 0) {
            pthread_mutex_unlock(&g_gioco.mutex);
            return -2; /* Gia' connesso */
        }
    }

    /* Cerca slot libero */
    int slot = -1;
    for (int i = 0; i < g_gioco.num_giocatori; i++) {
        if (!g_gioco.giocatori[i].attivo) {
            slot = i;
            break;
        }
    }

    /* Espansione array */
    if (slot < 0) {
        int nuovo_totale = 8;
        if (g_gioco.num_giocatori > 0)
            nuovo_totale = g_gioco.num_giocatori * 2;
        Giocatore *nuovi_giocatori = realloc(g_gioco.giocatori, nuovo_totale * sizeof(Giocatore));
        if (!nuovi_giocatori) {
            pthread_mutex_unlock(&g_gioco.mutex);
            return -3;
        }
        memset(nuovi_giocatori + g_gioco.num_giocatori, 0, (nuovo_totale - g_gioco.num_giocatori) * sizeof(Giocatore));
        slot = g_gioco.num_giocatori;
        g_gioco.giocatori = nuovi_giocatori;
        g_gioco.num_giocatori = nuovo_totale;
    }

    /* Posiziona il giocatore in una cella libera casuale */
    int riga, colonna;
    do {
        riga = 1 + rand() % (RIGHE_MAPPA - 2);
        colonna = 1 + rand() % (COLONNE_MAPPA - 2);
    } while (g_gioco.labirinto.celle[riga][colonna] != CELLA_LIBERA);

    Giocatore *p = &g_gioco.giocatori[slot];
    p->fd = fd;
    p->attivo = 1;
    p->partecipante = 1;
    p->uscito = 0;
    p->riga = riga;
    p->colonna = colonna;
    p->punteggio = 0;
    strncpy(p->nickname, nickname, MAX_NICK - 1);
    p->nickname[MAX_NICK - 1] = '\0';
    memset(p->scoperte, 0, sizeof(p->scoperte));
    scopri_dintorni(p);

    pthread_mutex_unlock(&g_gioco.mutex);

    char log_buf[128];
    snprintf(log_buf, sizeof(log_buf), "Nuova connessione: '%s' (riga %d, colonna %d, slot %d).", nickname, riga, colonna, slot);
    log_event(log_buf);
    return slot;
}

void rimuovi_giocatore(int indice) {
    pthread_mutex_lock(&g_gioco.mutex);
    if (g_gioco.giocatori[indice].attivo) {
        g_gioco.giocatori[indice].attivo = 0;
        close(g_gioco.giocatori[indice].fd);
        char log_buf[128];
        snprintf(log_buf, sizeof(log_buf), "Disconnessione: '%s'.", g_gioco.giocatori[indice].nickname);
        log_event(log_buf);
    }
    pthread_mutex_unlock(&g_gioco.mutex);
}

void muovi_giocatore(int indice, char direzione) {
    pthread_mutex_lock(&g_gioco.mutex);
    Giocatore *p = &g_gioco.giocatori[indice];

    if (!p->attivo || p->uscito || g_gioco.partita_finita) {
        pthread_mutex_unlock(&g_gioco.mutex);
        return;
    }

    int nuova_riga = p->riga, nuova_colonna = p->colonna;
    int non_valido = 0;
    switch (tolower(direzione)) {
        case 'w': nuova_riga--; break;
        case 's': nuova_riga++; break;
        case 'a': nuova_colonna--; break;
        case 'd': nuova_colonna++; break;
        default: non_valido = 1; break;
    }

    if (non_valido || g_gioco.labirinto.celle[nuova_riga][nuova_colonna] == CELLA_MURO) {
        pthread_mutex_unlock(&g_gioco.mutex);
        invia_mappa_locale(indice, "Sei andato verso un muro, prova un'altra direzione.");
        return;
    }

    p->riga = nuova_riga;
    p->colonna = nuova_colonna;
    scopri_dintorni(p);

    int raccolto = 0;
    if (g_gioco.labirinto.oggetti[nuova_riga][nuova_colonna]) {
        g_gioco.labirinto.oggetti[nuova_riga][nuova_colonna] = 0;
        p->punteggio++;
        raccolto = 1;
    }

    int raggiunta_uscita = 0;
    if (g_gioco.labirinto.celle[nuova_riga][nuova_colonna] == CELLA_USCITA) {
        p->uscito = 1;
        raggiunta_uscita = 1;
    }

    char nick[MAX_NICK];
    strncpy(nick, p->nickname, sizeof(nick));
    int punteggio = p->punteggio;

    int terminata = 0;
    if ((raccolto || raggiunta_uscita) && tutti_usciti_bloccato()) {
        g_gioco.partita_finita = 1;
        g_gioco.vincitore = trova_vincitore_bloccato();
        terminata = 1;
    }
    pthread_mutex_unlock(&g_gioco.mutex);

    const char *avviso = NULL;
    char log_buf[128];
    if (raccolto) {
        snprintf(log_buf, sizeof(log_buf), "%s ha raccolto un oggetto in (%d,%d), punteggio=%d.", nick, nuova_riga, nuova_colonna, punteggio);
        log_event(log_buf);
        avviso = "Hai raccolto un oggetto! (+1 punto)";
    }
    if (raggiunta_uscita) {
        snprintf(log_buf, sizeof(log_buf), "%s ha raggiunto l'uscita con punteggio=%d.", nick, punteggio);
        log_event(log_buf);
        avviso = "Hai raggiunto l'uscita!";
    }

    invia_mappa_locale(indice, avviso);

    if (terminata)
        broadcast_fine_partita();
}

/* Costruisce la vista locale 5x5 */
static void componi_vista_locale(const Giocatore *p, char mappa_locale[VISTA_LOCALE][VISTA_LOCALE]) {
    for (int dr = -RAGGIO_VISIBILITA; dr <= RAGGIO_VISIBILITA; dr++) {
        for (int dc = -RAGGIO_VISIBILITA; dc <= RAGGIO_VISIBILITA; dc++) {
            int r = p->riga + dr;
            int c = p->colonna + dc;
            int vr = dr + RAGGIO_VISIBILITA;
            int vc = dc + RAGGIO_VISIBILITA;

            if (r < 0 || r >= RIGHE_MAPPA || c < 0 || c >= COLONNE_MAPPA || !p->scoperte[r][c])
                mappa_locale[vr][vc] = CELLA_NASCOSTA;
            else if (r == p->riga && c == p->colonna)
                mappa_locale[vr][vc] = CELLA_GIOCATORE;
            else if (g_gioco.labirinto.oggetti[r][c])
                mappa_locale[vr][vc] = CELLA_OGGETTO;
            else
                mappa_locale[vr][vc] = g_gioco.labirinto.celle[r][c];
        }
    }
}

void invia_mappa_locale(int indice, const char *avviso) {
    Messaggio msg = {0};
    msg.tipo = MSG_MAPPA_LOCALE;
    if (avviso)
        strncpy(msg.testo, avviso, MAX_TESTO - 1);

    pthread_mutex_lock(&g_gioco.mutex);
    Giocatore *p = &g_gioco.giocatori[indice];
    if (!p->attivo) {
        pthread_mutex_unlock(&g_gioco.mutex);
        return;
    }

    msg.riga = p->riga;
    msg.colonna = p->colonna;
    msg.punteggio = p->punteggio;
    componi_vista_locale(p, msg.mappa_locale);

    int fd = p->fd;
    pthread_mutex_unlock(&g_gioco.mutex);

    invia_messaggio(fd, &msg);
}

void invia_mappa_globale(int indice) {
    Messaggio msg = {0};
    msg.tipo = MSG_MAPPA_GLOBALE;

    pthread_mutex_lock(&g_gioco.mutex);
    Giocatore *p = &g_gioco.giocatori[indice];
    if (!p->attivo) {
        pthread_mutex_unlock(&g_gioco.mutex);
        return;
    }

    msg.riga = p->riga;
    msg.colonna = p->colonna;
    msg.punteggio = p->punteggio;
    componi_vista_locale(p, msg.mappa_locale);

    /* Mappa globale */
    for (int r = 0; r < RIGHE_MAPPA; r++) {
        for (int c = 0; c < COLONNE_MAPPA; c++) {
            if (!p->scoperte[r][c])
                msg.mappa_globale[r][c] = CELLA_NASCOSTA;
            else if (r == p->riga && c == p->colonna)
                msg.mappa_globale[r][c] = CELLA_GIOCATORE;
            else if (g_gioco.labirinto.oggetti[r][c])
                msg.mappa_globale[r][c] = CELLA_OGGETTO;
            else
                msg.mappa_globale[r][c] = g_gioco.labirinto.celle[r][c];
        }
    }
    int fd = p->fd;
    pthread_mutex_unlock(&g_gioco.mutex);

    invia_messaggio(fd, &msg);
}

void invia_lista_giocatori(int indice) {
    Messaggio msg = {0};
    msg.tipo = MSG_LISTA;

    pthread_mutex_lock(&g_gioco.mutex);
    if (!g_gioco.giocatori[indice].attivo) {
        pthread_mutex_unlock(&g_gioco.mutex);
        return;
    }

    int conteggio = 0;
    for (int i = 0; i < g_gioco.num_giocatori && conteggio < MAX_LISTA_GIOCATORI; i++) {
        if (!g_gioco.giocatori[i].attivo)
            continue;
        strncpy(msg.giocatori[conteggio].nickname, g_gioco.giocatori[i].nickname, MAX_NICK - 1);
        msg.giocatori[conteggio].punteggio = g_gioco.giocatori[i].punteggio;
        msg.giocatori[conteggio].uscito = g_gioco.giocatori[i].uscito;
        conteggio++;
    }
    msg.num_giocatori = conteggio;
    int fd = g_gioco.giocatori[indice].fd;
    pthread_mutex_unlock(&g_gioco.mutex);

    invia_messaggio(fd, &msg);
}

void broadcast_mappe_globali(void) {
    for (int i = 0; i < g_gioco.num_giocatori; i++) {
        if (g_gioco.giocatori[i].attivo)
            invia_mappa_globale(i);
    }
}

void broadcast_fine_partita(void) {
    Messaggio msg = {0};
    msg.tipo = MSG_FINE_PARTITA;

    pthread_mutex_lock(&g_gioco.mutex);
    if (g_gioco.vincitore >= 0) {
        strncpy(msg.nickname, g_gioco.giocatori[g_gioco.vincitore].nickname, MAX_NICK - 1);
        msg.punteggio = g_gioco.giocatori[g_gioco.vincitore].punteggio;
        char log_buf[128];
        snprintf(log_buf, sizeof(log_buf), "Fine partita. Vincitore: %s con %d oggetti.", msg.nickname, msg.punteggio);
        log_event(log_buf);
    } else {
        log_event("Fine partita. Nessun vincitore.");
    }

    for (int i = 0; i < g_gioco.num_giocatori; i++) {
        if (g_gioco.giocatori[i].attivo)
            invia_messaggio(g_gioco.giocatori[i].fd, &msg);
    }
    pthread_mutex_unlock(&g_gioco.mutex);
}
