
CPPFLAGS= -std=c++17 -Wall -Wextra -pedantic -g 
# LIBS= -lpcap

all: sniffer

sniffer: $(SRC)
	g++ $(CPPFLAGS) ipk-sniffer.cpp -o ipk-sniffer $(LIBS)

.PHONY: clean

clean:
	rm -f ipk-sniffer