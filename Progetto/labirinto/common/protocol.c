#include "protocol.h"
#include <unistd.h>
#include <errno.h>

/* Invia un messaggio gestendo short write */
int invia_messaggio(int fd, const Messaggio *msg) {
    const char *p = (const char *)msg;
    size_t remaining = sizeof(Messaggio);

    while (remaining > 0) {
        ssize_t n = write(fd, p, remaining);
        if (n <= 0) {
            if (n < 0 && errno == EINTR)
                continue;
            return -1;
        }
        p += n;
        remaining -= n;
    }
    return 0;
}

/* Riceve un messaggio gestendo short read */
int ricevi_messaggio(int fd, Messaggio *msg) {
    char *p = (char *)msg;
    size_t remaining = sizeof(Messaggio);

    while (remaining > 0) {
        ssize_t n = read(fd, p, remaining);
        if (n == 0)
            return 0;
        if (n < 0) {
            if (errno == EINTR)
                continue;
            return -1;
        }
        p += n;
        remaining -= n;
    }
    return 1;
}
