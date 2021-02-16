# Makefile for all examples

NAME = test-vm
PROGRAMS = test-vm pagefaults

CFLAGS = -m32 -Wall

all: $(PROGRAMS)

run: $(PROGRAMS)
	./0-get-maps.sh

pack:
	tar czvf ips-mem-examples.tar.gz Makefile *.c *.sh results

clean:
	rm -f $(PROGRAMS) $(NAME)-maps $(NAME).out

