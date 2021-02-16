#!/bin/bash

gnuplot <<__END__
set terminal pdf; set output "test-containers.pdf"

set title "Timing of insert operation (including overhead)"
set xlabel "Container size [-]"
set ylabel "Time [ns]"
set style line 1 lt 1 lw 1
set style line 2 lt 2 lw 1
set style data linespoints
set pointsize 0.4
set key left
set grid
set logscale x
set logscale y
plot [][1:1e6] "data" using 1:2 lw 2 title "vector push back", \
               "data" using 1:3 lw 2 title "vector insert begin", \
               "data" using 1:4 title "list push back", \
               "data" using 1:5 title "list insert begin", \
               "data" using 1:6 lw 3 title "set insert", \
               "data" using 1:7 title "deque push back", \
               "data" using 1:8 title "deque insert begin"

__END__

