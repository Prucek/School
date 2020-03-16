# Makefile
# Riesenie IJC-DU1, příklad a), b), 7.3.2020
# Autor: Peter Rucek, xrucek00, FIT 
# GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)

OPT=-O2
CFLAGS=-std=c11 -Wall -Wextra -pedantic -fsanitize=address
LDFLAGS=-lm -fsanitize=address

all: primes primes-i steg-decode

run: primes primes-i
	./primes
	./primes-i

bit_set-i.o: bit_set.c bit_set.h error.h
	gcc $(CFLAGS) $(OPT) -DUSE_INLINE -c bit_set.c -o bit_set-i.o

bit_set.o: bit_set.c bit_set.h error.h
	gcc $(CFLAGS) $(OPT) -c bit_set.c -o bit_set.o

error.o: error.c error.h
	gcc $(CFLAGS) $(OPT) -c error.c -o error.o

eratosthenes.o: eratosthenes.c bit_set.h error.h
	gcc $(CFLAGS) $(OPT) -c eratosthenes.c -o eratosthenes.o 

eratosthenes-i.o: eratosthenes.c bit_set.h error.h eratosthenes.h
	gcc $(CFLAGS) $(OPT) -DUSE_INLINE -c eratosthenes.c -o eratosthenes-i.o

primes.o: primes.c bit_set.h error.h eratosthenes.h
	gcc $(CFLAGS) $(OPT) -c primes.c -o primes.o $(LDFLAGS)

primes-i.o: primes.c bit_set.h error.h eratosthenes.h
	gcc $(CFLAGS) $(OPT) -DUSE_INLINE -c primes.c -o primes-i.o $(LDFLAGS)

primes: eratosthenes.o primes.o error.o bit_set.o
	gcc $(CFLAGS) $(OPT) eratosthenes.o primes.o error.o bit_set.o -o primes $(LDFLAGS)

primes-i: eratosthenes-i.o primes-i.o error.o bit_set-i.o 
	gcc $(CFLAGS) $(OPT) -DUSE_INLINE eratosthenes-i.o primes-i.o error.o bit_set-i.o -o primes-i $(LDFLAGS)

ppm.o: ppm.c ppm.h error.h
	gcc $(CFLAGS) $(OPT) -c ppm.c -o ppm.o

steg-decode: ppm.o steg-decode.c ppm.h eratosthenes.h bit_set.h
	gcc $(CFLAGS) $(OPT) ppm.o steg-decode.c error.o eratosthenes.o -o steg-decode $(LDFLAGS)

clean:
	rm *.o
	rm primes
	rm primes-i
	rm steg-decode