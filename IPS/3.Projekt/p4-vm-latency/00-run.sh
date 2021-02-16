#!/bin/bash
echo "Memory access timing experiments"
# Includes page-faults
# The accuracy depends on: RDTSC, cpufreq, interrupts, ...

# get cache line size:
CACHE_LINE=`getconf LEVEL1_DCACHE_LINESIZE`
PAGE_SIZE=`getconf PAGE_SIZE`

# compile
P=access-time   # program name
gcc -Wall -O2 $P.c -o $P

##################################################################
runplot() {
STEP=$1
N=$2
F=$P-$STEP
echo STEP=$STEP N=$N
# Run program
./$P $STEP $N >$F.out
# Run Gnuplot
gnuplot <<__END__
set title "Read freshly allocated memory with STEP=$STEP"
set term pdf; set output "$F.pdf"
set logscale y
set ylabel "Access time [clk]"
set xlabel "Offset [B]"
set nokey
set grid
set style data impulses
plot [-50:][1:] "$F.out" using 1:2 lw 1
__END__
}
##################################################################

runplot 4 128

runplot 8 128

runplot 16 128

STEP=$((CACHE_LINE))
runplot $STEP 128

runplot 256 128

STEP=$((PAGE_SIZE))
runplot $STEP 128

rm -f $P

