#ifndef AUTH_H
#define AUTH_H

/*
 * Registra un nuovo utente.
 * Ritorna:  0 successo, -1 nickname gia' presente, -2 errore/argomenti non validi.
 */
int auth_register(const char *nickname, const char *password);

/*
 * Verifica le credenziali di un utente.
 * Ritorna: 1 se corrette, 0 altrimenti.
 */
int auth_login(const char *nickname, const char *password);

#endif
