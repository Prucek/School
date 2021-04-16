# Makefile for Packet sniffer
# Author: Peter Rucek, xrucek00
# Date: 16.4.2021

CPPFLAGS= -std=c++17 -Wall -Wextra -pedantic -g 
LIBS= -lpcap
SRC= ipk-sniffer.hpp print.hpp

all: clean ipk-sniffer

ipk-sniffer: $(SRC)
	g++ $(CPPFLAGS) ipk-sniffer.cpp print.cpp -o ipk-sniffer $(LIBS)

.PHONY: clean

clean:
	rm -f ipk-sniffer