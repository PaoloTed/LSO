#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

/* Dimensioni del labirinto e della vista locale. */
#define MAP_ROWS 21
#define MAP_COLS 41
#define LOCAL_VIEW 5
#define VIEW_RADIUS (LOCAL_VIEW / 2)

/* Simboli usati per rappresentare le celle. */
#define CELL_WALL   '#'
#define CELL_FREE   '.'
#define CELL_EXIT   'E'
#define CELL_OBJECT 'O'
#define CELL_PLAYER 'P'
#define CELL_HIDDEN '?'

/* Lunghezza massima dei campi testuali scambiati. */
#define MAX_NICK 32
#define MAX_PASS 32
#define MAX_TEXT 128

/* Messaggi dal client verso il server. */
#define MSG_REGISTER 0x01
#define MSG_LOGIN    0x02
#define MSG_MOVE     0x03
#define MSG_LIST     0x04
#define MSG_QUIT     0x05

/* Messaggi dal server verso il client. */
#define MSG_OK          0x10
#define MSG_ERROR       0x11
#define MSG_LOCAL_MAP   0x12
#define MSG_GLOBAL_MAP  0x13
#define MSG_PLAYER_LIST 0x14
#define MSG_GAME_OVER   0x15
#define MSG_INFO        0x16

/* Ogni messaggio inizia con 4 byte: tipo, flag, lunghezza (big endian). */
#define MSG_HEADER_LEN 4

/* Dimensione degli header numerici dei due tipi di mappa. */
#define LOCAL_MAP_HDR  (5 * 4)
#define GLOBAL_MAP_HDR (2 * 4)

#define MAX_PAYLOAD 4096

typedef struct {
    char nickname[MAX_NICK];
    char password[MAX_PASS];
} AuthPayload;

typedef struct {
    char direction;
} MovePayload;

typedef struct {
    char text[MAX_TEXT];
} TextPayload;

typedef struct {
    char     nickname[MAX_NICK];
    uint32_t score;
    uint32_t exited;
} PlayerEntry;

ssize_t send_all(int fd, const void *buf, size_t n);
ssize_t recv_all(int fd, void *buf, size_t n);
int send_message(int fd, uint8_t type, const void *payload, uint16_t len);
int recv_message(int fd, uint8_t *type, void *payload, size_t max_len);

#endif
