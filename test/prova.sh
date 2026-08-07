#!/bin/bash
echo "Hello world!"
ls
if [ $# -lt 1 ]; then
	echo "Serve un argomento"
	exit 1
else
	echo "$1"
fi
