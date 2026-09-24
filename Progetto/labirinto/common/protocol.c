#include "protocol.h"

#include <errno.h>
#include <sys/socket.h>

/* Invia esattamente n byte, gestendo scritture parziali e EINTR. */
ssize_t send_all(int fd, const void *buf, size_t n) {
    size_t sent = 0;
    const char *p = buf;

    while (sent < n) {
        ssize_t w = send(fd, p + sent, n - sent, 0);
        if (w > 0) {
            sent += (size_t)w;
            continue;
        }
        if (w < 0 && errno == EINTR)
            continue;
        return -1;
    }
    return (ssize_t)sent;
}

/* Riceve esattamente n byte. Ritorna i byte ricevuti (< n se il peer chiude). */
ssize_t recv_all(int fd, void *buf, size_t n) {
    size_t received = 0;
    char *p = buf;

    while (received < n) {
        ssize_t r = recv(fd, p + received, n - received, 0);
        if (r > 0) {
            received += (size_t)r;
            continue;
        }
        if (r == 0)
            return (ssize_t)received;   /* connessione chiusa */
        if (errno == EINTR)
            continue;
        return -1;
    }
    return (ssize_t)received;
}

/* Compone header e payload in un unico messaggio. */
int send_message(int fd, uint8_t type, const void *payload, uint16_t len) {
    uint8_t hdr[MSG_HEADER_LEN];

    hdr[0] = type;
    hdr[1] = 0;
    hdr[2] = (uint8_t)(len >> 8);
    hdr[3] = (uint8_t)(len & 0xFF);

    if (send_all(fd, hdr, MSG_HEADER_LEN) < 0)
        return -1;
    if (len > 0 && send_all(fd, payload, len) < 0)
        return -1;
    return 0;
}

/* Riceve un messaggio completo. Ritorna la lunghezza del payload o -1. */
int recv_message(int fd, uint8_t *type, void *payload, size_t max_len) {
    uint8_t hdr[MSG_HEADER_LEN];

    if (recv_all(fd, hdr, MSG_HEADER_LEN) != MSG_HEADER_LEN)
        return -1;

    *type = hdr[0];
    uint16_t len = (uint16_t)((hdr[2] << 8) | hdr[3]);

    if (len > max_len)
        return -1;
    if (len > 0 && recv_all(fd, payload, len) != (ssize_t)len)
        return -1;

    return (int)len;
}
