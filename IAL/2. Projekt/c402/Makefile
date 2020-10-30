
PRJ=c402
#
PROGS=$(PRJ)-test $(PRJ)-atest
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm

all: $(PROGS)

$(PRJ)-test: $(PRJ).c $(PRJ)-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-test.c

$(PRJ)-atest: $(PRJ).c $(PRJ)-test.c
	$(CC) $(CFLAGS) -o $@ $(PRJ).c $(PRJ)-atest.c

test:
	./$(PRJ)-test 1>myfile.out
	diff myfile $(PRJ)-test.out
	./$(PRJ)-atest 1>amyfile.out
	diff amyfile $(PRJ)-atest.out

clean:
	rm -f *.o *.out $(PROGS)
#