# Makefile
# Riesenie IOS, proj2, 24.4.2020
# Autor: Peter Rucek, xrucek00

CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic 
LDFLAGS=-lpthread -lrt

DEP=proj2.c judge.c immigrants.c proj2.h judge.h immigrants.h

all: proj2

proj2: $(DEP)
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean pack

clean:
	rm -f proj2
	rm -f proj2.out

pack:
	zip proj2.zip *.c *.h Makefile 