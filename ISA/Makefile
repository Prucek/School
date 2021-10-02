# Makefile for POP3 client
# Author: Peter Rucek, xrucek00
# Date: 1.10.2021

CPPFLAGS= -std=c++17 -Wall -Wextra -pedantic -g 
# LIBS= -lpcap
SRC= popcl.hpp

all: clean popcl

popcl: $(SRC)
	g++ $(CPPFLAGS) popcl.cpp  -o popcl

.PHONY: clean

clean:
	rm -f popcl