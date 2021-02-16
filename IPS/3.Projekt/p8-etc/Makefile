# Makefile for simple benchmarks

# we need to disable CMOV usage by new GCC to demonstrate the branch predictor
# effects on conditional command performance
# inspired by: https://github.com/xiadz/cmov
DISABLE_CMOV_OPT = -fno-tree-loop-if-convert -fno-tree-loop-if-convert-stores -fno-if-conversion -fno-if-conversion2

CFLAGS = -std=c99 -O2 $(DISABLE_CMOV_OPT) -Wall -pedantic
CXXFLAGS = -std=c++11 -O2 -Wall -pedantic
PROGS  = test-branch-predict test-volatile test-containers

all: $(PROGS)

run: $(PROGS)
	./test-branch-predict           >test-branch-predict.out
	./test-branch-predict sort      >>test-branch-predict.out
	./test-volatile                 >test-volatile.out
	./test-containers >data
	./test-containers-plot.sh

clean:
	rm -f $(PROGS) test-branch-predict.out data test-containers.pdf test-volatile.out

pack:
	tar czvf ips-etc.tar.gz *.c *.cc *.sh *.txt Makefile results

