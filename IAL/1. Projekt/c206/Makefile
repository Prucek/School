
PRJ=c206
#
PROGS=$(PRJ)-test $(PRJ)-a-test
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

$(PRJ)-a-test: $(PRJ).c $(PRJ)-a-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-a-test.c

clean:
	rm -f *.o *.out $(PROGS)
#