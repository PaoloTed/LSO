#!/usr/bin/env python3
import sys
import time
import subprocess
from pathlib import Path

build_flag = int(sys.argv[1])  
num_clients = int(sys.argv[2])

docker_dir = Path.cwd()

(docker_dir.parent / "users.txt").touch(exist_ok=True)
(docker_dir.parent / "server.log").touch(exist_ok=True)
subprocess.run(["docker", "compose", "down"], cwd=docker_dir)

if build_flag == 0:
    subprocess.run(["docker", "compose", "build"], cwd=docker_dir)

subprocess.run(["docker", "compose", "up", "-d", "server"], cwd=docker_dir)
time.sleep(1)

cmd = "docker compose run --rm client"

for i in range(1, num_clients + 1):
    subprocess.Popen([
        "alacritty",
        "--working-directory", str(docker_dir),
        "-e", "bash", "-c", cmd
    ])