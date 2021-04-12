#!/bin/bash

while test $? -eq 0 
do
    # python3 fileget.py -n 147.229.176.19:3333 -f fsp://blue.server/000110.jpg
    python3 fileget.py -n 127.0.0.1:1234 -f fsp://file.server.one/test.txt
    echo $?
    sleep 2
done
exit 0
