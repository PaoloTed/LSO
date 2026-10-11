#include "protocol.h"
#include <unistd.h>
#include <errno.h>

/*
 * Invia un Messaggio attraverso la socket.
 * Gestisce short write ed eventuali interruzioni da segnali (EINTR).
 */
int invia_messaggio(int fd, const Messaggio *msg) {
    const char *ptr = (const char *)msg;
    size_t rimanenti = sizeof(Messaggio);
    size_t inviati = 0;

    while (rimanenti > 0) {
        ssize_t scritti = write(fd, ptr + inviati, rimanenti);
        if (scritti <= 0) {
            if (scritti < 0 && errno == EINTR)
                continue;
            return -1;
        }
        inviati += scritti;
        rimanenti -= scritti;
    }
    return 0;
}

/*
 * Riceve un Messaggio dalla socket.
 * Gestisce short read ed eventuali interruzioni da segnali (EINTR).
 */
int ricevi_messaggio(int fd, Messaggio *msg) {
    char *ptr = (char *)msg;
    size_t rimanenti = sizeof(Messaggio);
    size_t ricevuti = 0;

    while (rimanenti > 0) {
        ssize_t letti = read(fd, ptr + ricevuti, rimanenti);
        if (letti == 0)
            return 0;   /* connessione chiusa dall'interlocutore */
        if (letti < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        ricevuti += letti;
        rimanenti -= letti;
    }
    return 1;
}
