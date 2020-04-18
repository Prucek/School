# Makefile
# Riesenie IOS, proj2, 17.4.2020
# Autor: Peter Rucek, xrucek00, FIT
# GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic 
LDFLAGS=-lpthread -lrt
 
all: proj2

proj2: proj2.c
	gcc $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	rm proj2
