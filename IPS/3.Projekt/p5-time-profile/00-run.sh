#!/bin/bash
echo "Memory access timing experiments"
# Includes page-faults
# The accuracy depends on: RDTSC, cpufreq, interrupts, ...

# get cache line size:
CACHE_LINE=`getconf LEVEL1_DCACHE_LINESIZE`
PAGE_SIZE=`getconf PAGE_SIZE`

# compile
P=time-profile   # program name
gcc -Wall -O2 $P.c -o $P

##################################################################
runplot() {
N=$1
# Run program
./$P $N >$P.out
# Run Gnuplot
gnuplot <<__END__
set title "Program time-profile output (N=$N)"
#set term postscript color; set output "$P.ps"
set term pdf; set output "$P.pdf"
set logscale y
set ylabel "interruption time [clk]"
set xlabel "time [clk]"
set nokey
set grid
set style data impulses
plot "$P.out" using 2:3 lw 1
__END__
}
##################################################################

runplot 300

rm -f $P

