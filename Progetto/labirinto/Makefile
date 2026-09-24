CC      = gcc
CFLAGS  = -Wall -Wextra -g -pthread -Icommon
LDFLAGS = -pthread

SERVER_SRC = server/server.c server/game.c server/auth.c server/log.c common/protocol.c
CLIENT_SRC = client/client.c client/display.c common/protocol.c

all: bin/server bin/client

bin/server: $(SERVER_SRC) | bin
	$(CC) $(CFLAGS) -o $@ $(SERVER_SRC) $(LDFLAGS)

bin/client: $(CLIENT_SRC) | bin
	$(CC) $(CFLAGS) -o $@ $(CLIENT_SRC) $(LDFLAGS)

bin:
	mkdir -p bin

clean:
	rm -rf bin server.log users.db

.PHONY: all clean
