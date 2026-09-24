#include "auth.h"
#include "protocol.h"

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* Gli utenti registrati sono salvati nel file "users.db", nel formato:
 *   nickname:hash_password
 */
#define AUTH_FILE "users.db"

static pthread_mutex_t auth_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Hash molto semplice (djb2): evita di salvare le password in chiaro. */
static unsigned long hash_password(const char *s) {
    unsigned long h = 5381;
    while (*s)
        h = ((h << 5) + h) + (unsigned char)*s++;
    return h;
}

static int valid_nick(const char *nick) {
    size_t n = strlen(nick);
    if (n == 0 || n >= MAX_NICK)
        return 0;
    if (strchr(nick, ':') || strchr(nick, '\n'))
        return 0;
    return 1;
}

static int valid_pass(const char *pass) {
    size_t n = strlen(pass);
    return n > 0 && n < MAX_PASS;
}

/* Legge tutto il file utenti in memoria. Ritorna 0 se ok. */
static int load_db(char **out, size_t *out_len) {
    *out = NULL;
    *out_len = 0;

    int fd = open(AUTH_FILE, O_RDONLY);
    if (fd < 0)
        return 0;   /* file assente: nessun utente registrato */

    off_t size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        close(fd);
        return -1;
    }
    if (size == 0) {
        close(fd);
        return 0;
    }
    lseek(fd, 0, SEEK_SET);

    char *buf = malloc((size_t)size + 1);
    if (!buf) {
        close(fd);
        return -1;
    }

    size_t got = 0;
    while (got < (size_t)size) {
        ssize_t r = read(fd, buf + got, (size_t)size - got);
        if (r > 0) {
            got += (size_t)r;
            continue;
        }
        if (r < 0 && errno == EINTR)
            continue;
        break;
    }
    close(fd);

    buf[got] = '\0';
    *out = buf;
    *out_len = got;
    return 0;
}

/* Cerca un nickname nel contenuto del file. Restituisce l'hash se trovato. */
static int find_user(const char *buf, const char *nick, unsigned long *hash) {
    size_t nick_len = strlen(nick);
    const char *p = buf;

    while (p && *p) {
        const char *eol = strchr(p, '\n');
        size_t line_len = eol ? (size_t)(eol - p) : strlen(p);
        const char *colon = memchr(p, ':', line_len);

        if (colon && (size_t)(colon - p) == nick_len &&
            strncmp(p, nick, nick_len) == 0) {
            *hash = strtoul(colon + 1, NULL, 10);
            return 1;
        }

        if (!eol)
            break;
        p = eol + 1;
    }
    return 0;
}

int auth_register(const char *nickname, const char *password) {
    if (!valid_nick(nickname) || !valid_pass(password))
        return -2;

    pthread_mutex_lock(&auth_mutex);

    char *buf;
    size_t len;
    if (load_db(&buf, &len) < 0) {
        pthread_mutex_unlock(&auth_mutex);
        return -2;
    }

    unsigned long existing;
    if (find_user(buf, nickname, &existing)) {
        free(buf);
        pthread_mutex_unlock(&auth_mutex);
        return -1;
    }
    free(buf);

    char line[MAX_NICK + 32];
    int n = snprintf(line, sizeof(line), "%s:%lu\n", nickname, hash_password(password));
    if (n < 0 || (size_t)n >= sizeof(line)) {
        pthread_mutex_unlock(&auth_mutex);
        return -2;
    }

    int fd = open(AUTH_FILE, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        pthread_mutex_unlock(&auth_mutex);
        return -2;
    }

    ssize_t w = write(fd, line, (size_t)n);
    close(fd);
    pthread_mutex_unlock(&auth_mutex);

    return (w == n) ? 0 : -2;
}

int auth_login(const char *nickname, const char *password) {
    pthread_mutex_lock(&auth_mutex);

    char *buf;
    size_t len;
    if (load_db(&buf, &len) < 0) {
        pthread_mutex_unlock(&auth_mutex);
        return 0;
    }

    unsigned long stored = 0;
    int found = find_user(buf, nickname, &stored);
    free(buf);

    pthread_mutex_unlock(&auth_mutex);

    if (!found)
        return 0;
    return (stored == hash_password(password)) ? 1 : 0;
}
