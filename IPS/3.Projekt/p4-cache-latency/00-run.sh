#!/bin/bash
echo "Memory access timing experiments"
# No page-faults involved
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
set title "Read memory with STEP=$STEP"
set term pdf; set output "$F.pdf"
set logscale y
set ylabel "Access time [clk]"
set xlabel "Offset [B]"
set nokey
set grid
set style data impulses
plot [-10:][1:] "$F.out" using 1:2 lw 1
__END__
# histogram
gnuplot <<__END__
set title "Acces time histogram (STEP=$STEP)"
set term pdf; set output "$F-hist.pdf"

set boxwidth 1.0 absolute
set style fill solid 1.0
set nokey
set xrange [0:500]
set xlabel "access time [clk]"
set xtics 50
set ylabel "count [-]"
#set xtics font ",9"; set ytics font ",9"
set grid
plot "$F.out" using (\$2):(1)   smooth frequency with boxes

__END__
}
##################################################################

STEP=$((CACHE_LINE))
runplot $STEP 256

STEP=$((PAGE_SIZE))     ## ++ cache miss
runplot $STEP 128

rm -f $P

