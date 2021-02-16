
all:
	bash 00-run.sh

clean:
	rm -f cachegrind.out.[0-9]* objdump.out *.log

pack:
	tar czvf ips-mat-mul-cachegrind.tar.gz *.c *.sh *.txt Makefile results-*

