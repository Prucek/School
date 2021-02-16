#!/bin/sh
echo "Data visualization"

for F in *.out ; do
    FF=${F%%.out}
gnuplot <<__END__
set title "time-profile output ($FF)"
#set term postscript color; set output "$FF.ps"
#set term pdf; set output "$FF.pdf"
#set term x11 persist
set term wxt persist

set logscale y
#set logscale x
set ylabel "interruption time [clk]"
set xlabel "time [clk]"
set nokey
set grid
set style data points
set style data impulses
plot "$F" using 2:3 lw 1
__END__
done

