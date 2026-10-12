#ifndef LOG_H
#define LOG_H

/* Apre (o crea) il file di log. Ritorna 0 in caso di successo, -1 altrimenti. */
int log_open(const char *path);

/* Scrive una riga di testo nel file di log con timestamp. */
void log_event(const char *msg);

#endif
