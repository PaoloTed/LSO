#ifndef LOG_H
#define LOG_H

/* Apertura file di log */
int log_open(const char *path);

/* Scrittura evento nel log */
void log_event(const char *msg);

#endif
