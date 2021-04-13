
CPPFLAGS= -std=c++17 -Wall -Wextra -pedantic -g 
LIBS= -lpcap

all: clean ipk-sniffer

ipk-sniffer: $(SRC)
	g++ $(CPPFLAGS) ipk-sniffer.cpp -o ipk-sniffer $(LIBS)

.PHONY: clean

clean:
	rm -f ipk-sniffer