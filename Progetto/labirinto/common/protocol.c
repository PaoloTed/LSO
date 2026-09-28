#include "protocol.h"
#include <unistd.h>
#include <errno.h>

/*
 * Invia l'intera struttura Messaggio attraverso la socket.
 * Gestisce short write ed eventuali interruzioni da segnali (EINTR).
 * Ritorna 0 in caso di successo, -1 in caso di errore.
 */
int invia_messaggio(int fd, const Messaggio *msg) {
    const char *ptr = (const char *)msg;
    size_t rimanenti = sizeof(Messaggio);

    while (rimanenti > 0) {
        ssize_t scritti = write(fd, ptr, rimanenti);
        if (scritti <= 0) {
            if (scritti < 0 && errno == EINTR)
                continue;
            return -1;
        }
        ptr += scritti;
        rimanenti -= scritti;
    }
    return 0;
}

/*
 * Riceve l'intera struttura Messaggio dalla socket.
 * Gestisce short read ed eventuali interruzioni da segnali (EINTR).
 * Ritorna 1 se ricevuto correttamente, 0 se la socket e' chiusa, -1 per errore.
 */
int ricevi_messaggio(int fd, Messaggio *msg) {
    char *ptr = (char *)msg;
    size_t rimanenti = sizeof(Messaggio);

    while (rimanenti > 0) {
        ssize_t letti = read(fd, ptr, rimanenti);
        if (letti == 0)
            return 0;   /* connessione chiusa dall'interlocutore */
        if (letti < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        ptr += letti;
        rimanenti -= letti;
    }
    return 1;
}
