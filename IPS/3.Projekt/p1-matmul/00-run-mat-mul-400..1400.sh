#!/bin/sh

# test script for matrix multiplication demo

export LC_ALL=C

LOG="ips-mat-mul-demo.log"      ## output file
CFLAGS="-std=c99 -O3 -Wall"     ## set GCC options

if [ -f $LOG ]; then
    mv $LOG $LOG-
fi

##################################################################
LIBS="-lgsl"            # slow default for GSL
LIBS="-lgsl -lblas"
LIBS="-lgsl -lcblas"    # install ATLAS (package libatlas-base-dev)
# Compile with $LIBS
gcc -Wno-incompatible-pointer-types $CFLAGS ips-mat-mul-demo.c $LIBS -lm

##################################################################
( # Info: comments in data file
  echo "Measuring matrix multiplication algorithms"
  date
  uname -a
  grep 'model name[^:]*:' /proc/cpuinfo | uniq | sed 's/^[^:]*:/CPU:/'
  gcc --version| head -n 1
  echo "CFLAGS=$CFLAGS     LIBS=$LIBS"
  echo
) | sed 's/^/# /' >$LOG

##################################################################
# run
##################################################################
for N in `seq 400 16 1400`; do
    echo "N=$N"
    ./a.out $N                  >>$LOG
done
rm ./a.out
##################################################################
( echo; echo -n "# "; date; echo )       >>$LOG

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
set pointsize 0.5
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
