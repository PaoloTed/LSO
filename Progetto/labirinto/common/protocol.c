#include "protocol.h"
#include <unistd.h>
#include <errno.h>

/* Invia len byte gestendo short write */
int invia_dati(int fd, const void *buf, size_t len) {
    const char *p = (const char *)buf;
    size_t rimanenti = len;

    while (rimanenti > 0) {
        ssize_t n = write(fd, p, rimanenti);
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

/* Riceve len byte gestendo short read */
int ricevi_dati(int fd, void *buf, size_t len) {
    char *p = (char *)buf;
    size_t rimanenti = len;

    while (rimanenti > 0) {
        ssize_t n = read(fd, p, rimanenti);
        if (n == 0)
            return 0;
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        p += n;
        rimanenti -= n;
    }
    return 1;
}

int invia_messaggio(int fd, const Messaggio *msg) {
    return invia_dati(fd, msg, sizeof(Messaggio));
}

int ricevi_messaggio(int fd, Messaggio *msg) {
    return ricevi_dati(fd, msg, sizeof(Messaggio));
}
