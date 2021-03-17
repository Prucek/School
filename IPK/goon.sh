#!/bin/bash

while test $? -eq 0 
do
    echo python3 fileget.py -n 147.229.176.19:3333 -f fsp://blue.server/000110.jpg
    echo $?
    sleep 5
done
exit 0
