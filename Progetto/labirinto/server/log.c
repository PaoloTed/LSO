#include "log.h"

#include <fcntl.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static int log_fd = -1;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

int log_open(const char *path) {
    log_fd = open(path, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP);
    return (log_fd < 0) ? -1 : 0;
}

void log_event(const char *fmt, ...) {
    if (log_fd < 0)
        return;

    char msg[768];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msg, sizeof(msg), fmt, args);
    va_end(args);

    char line[900];
    int n;

    pthread_mutex_lock(&log_mutex);

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

    n = snprintf(line, sizeof(line), "[%s] %s\n", timestamp, msg);
    if (n > 0)
        write(log_fd, line, (size_t)n);

    pthread_mutex_unlock(&log_mutex);
}
