#!/usr/bin/env bash
set -e

build_flag=$1
num_clients=$2

touch ../users.txt ../server.log

docker compose down

if [ "$build_flag" -eq 0 ]; then
    docker compose build
fi

docker compose up -d server
sleep 1

TERM_CMD=$(command -v x-terminal-emulator || command -v alacritty || command -v konsole || command -v xterm)

for ((i = 1; i <= num_clients; i++)); do
   "$TERM_CMD" -e bash -c "docker compose run -it --rm client" &
done

