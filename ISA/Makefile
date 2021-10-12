# Makefile for POP3 client
# Author: Peter Rucek, xrucek00
# Date: 1.10.2021

CPPFLAGS= -std=c++17 -Wall -Wextra -pedantic -g 

SRC= popcl.hpp PopOptions.hpp POP3.hpp

all: clean popcl

popcl: $(SRC)
	g++ $(CPPFLAGS) popcl.cpp  PopOptions.cpp  POP3.cpp -o popcl -lssl -lcrypto -lz 

.PHONY: clean

clean:
	rm -f popcl