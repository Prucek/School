#!/usr/bin/env bash

POSIXLY_CORRECT=yes

echo Root directory: $(pwd)
echo Directories: $(ls -lR | grep "^d" | wc -l)
echo All files: $(find $(pwd) -type f | wc -l)
echo File size histogram:
y=$(ls -lR | grep "^-" | sed -E 's/([^0-9]*([0-9]*)){2}.*/\2/')

echo \<100 B
echo \<1 KiB
echo \<10 KiB
echo \<100 KiB
echo \<1 MiB
echo \<10 MiB
echo \<100 MiB
echo \<1 GiB
echo \>=1 GiB
