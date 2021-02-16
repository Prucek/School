#!/bin/sh

# test script for matrix multiplication demo

export LC_ALL=C

LOG="ips-mat-mul-demo.log"      ## output file

##################################################################
# plot
##################################################################
F="$LOG"

gnuplot <<__END__
set title "Square matrix multiplication experiment"
#set term postscript color; set output "$F.ps"
set term pdf; set output "$F-$(date -I).pdf"

set key top left box
set style data linespoints
set pointsize 0.2 ## EDIT
set xlabel "N"
set ylabel "t [s]"
set grid
plot "$F" using 1:2 title "alg1", \
     "" using 1:3 title "alg2", \
     "" using 1:4 title "alg3", \
     "" using 1:5 title "alg4", \
     "" using 1:6 title "alg5", \
     x**3/1.5e9 with lines lw 1.0 lc 0 title "k.N^3"

set logscale y
plot "$F" using 1:2 title "alg1", \
     "" using 1:3 title "alg2", \
     "" using 1:4 title "alg3", \
     "" using 1:5 title "alg4", \
     "" using 1:6 title "alg5", \
     x**3/1.5e9 with lines lw 1.0 lc 0 title "k.N^3"
__END__
# end
