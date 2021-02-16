
# Test of memory transfer rate

CFLAGS = -std=c99 -O3 -Wall -pedantic -march=native -mtune=native
PROGS  = memcpy-bench memset-bench \
         read-loop-bench write-loop-bench copy-loop-bench

all: $(PROGS)

run: $(PROGS)
	date
	./memcpy-bench 1 >/dev/null    #warm-up
	./memcpy-bench >memcpy-bench-data
	./memcpy-bench >>memcpy-bench-data
	./plot-result.sh memcpy-bench-data
	./memset-bench >memset-bench-data
	./memset-bench >>memset-bench-data
	./plot-result.sh memset-bench-data
	./read-loop-bench >read-loop-bench-data
	./read-loop-bench >>read-loop-bench-data
	./plot-result.sh read-loop-bench-data
	./write-loop-bench >write-loop-bench-data
	./write-loop-bench >>write-loop-bench-data
	./plot-result.sh write-loop-bench-data
	./copy-loop-bench >copy-loop-bench-data
	./copy-loop-bench >>copy-loop-bench-data
	./plot-result.sh copy-loop-bench-data
	./plot-all-results.sh   # sumary results
	date

clean:
	rm -f $(PROGS)

clean-all:
	rm -f $(PROGS) *-bench-data

pack:
	tar czvf ips-memcpy-memset.tar.gz *.c *.sh *.txt Makefile results

