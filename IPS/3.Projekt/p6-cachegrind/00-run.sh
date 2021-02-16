#!/bin/bash

LOG="mat-mul-demo.log"      ## output file
CFLAGS="-O3 -g -Wall"       ## set GCC options
N=512

# Compile
echo N=$N
gcc $CFLAGS -DSIZE=$N mat-mul-demo.c -lgsl -lcblas -lm

echo "=================================================================="
# get CACHE sizes
L1SZ=$(getconf LEVEL1_DCACHE_SIZE)
L1AS=$(getconf LEVEL1_DCACHE_ASSOC)
L1LS=$(getconf LEVEL1_DCACHE_LINESIZE)
echo "Data  L1: $L1SZ $L1AS $L1LS"
I1SZ=$(getconf LEVEL1_ICACHE_SIZE)
I1AS=$(getconf LEVEL1_ICACHE_ASSOC)
I1LS=$(getconf LEVEL1_ICACHE_LINESIZE)
echo "Instr L1: $I1SZ $I1AS $I1LS"
LL=3
LnSZ=$(getconf LEVEL${LL}_CACHE_SIZE)
LnAS=$(getconf LEVEL${LL}_CACHE_ASSOC)
LnLS=$(getconf LEVEL${LL}_CACHE_LINESIZE)
echo "Cache L${LL}: $LnSZ $LnAS $LnLS"

run_test() {
echo "=================================================================="
valgrind --tool=cachegrind \
         --I1=$I1SZ,$I1AS,$I1LS \
         --D1=$L1SZ,$L1AS,$L1LS \
         --LL=$LnSZ,$LnAS,$LnLS \
         ./a.out $1
}

(
run_test 1    ## textbook alg
run_test 2    ## simple optimized version
run_test 4    ## BLAS version
) &>$LOG
echo "=================================================================="
egrep '(D1  miss rate:|Command:)' $LOG

objdump -S ./a.out >objdump.out

rm ./a.out

