#include "log.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

static int log_fd = -1;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

int log_open(const char *path) {
    log_fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
    return (log_fd < 0) ? -1 : 0;
}

void log_event(const char *msg) {
    if (log_fd < 0)
        return;

    pthread_mutex_lock(&log_mutex);

    /* Data e ora correnti: [AAAA-MM-GG HH:MM:SS] */
    time_t now = time(NULL);
    char ts[32];
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", localtime(&now));

    /* Prepara la riga e la scrive con write */
    char line[512];
    int n = snprintf(line, sizeof(line), "[%s] %s\n", ts, msg);
    if (n > 0)
        write(log_fd, line, (size_t)n);

    pthread_mutex_unlock(&log_mutex);
}
