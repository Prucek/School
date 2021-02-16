
# run perf

CFLAGS="-O3 -g -Wall"                 ## set GCC options
N=512

# List of events to watch (see "perf list")
STATS=task-clock
STATS=$STATS,context-switches
STATS=$STATS,page-faults
STATS=$STATS,cycles
STATS=$STATS,instructions
STATS=$STATS,branches
STATS=$STATS,branch-misses
#STATS=$STATS,cache-misses
STATS=$STATS,L1-dcache-loads
STATS=$STATS,L1-dcache-load-misses
#STATS=$STATS,dTLB-loads
STATS=$STATS,dTLB-load-misses

### compile
gcc $CFLAGS -DSIZE=$N ips-mat-mul-demo.c -lgsl -lcblas -latlas -lm


run() {
    echo "perf stat -e $STATS \\"
    echo "     ./a.out $1 (CFLAGS=$CFLAGS)"
    echo ==================================================
    perf stat -e $STATS ./a.out $1
}

(
### run
run 1
run 3
run 4
run 5

echo "**************************************************"

run 2
CFLAGS="-O3 -g -funroll-loops"
gcc $CFLAGS -DSIZE=$N ips-mat-mul-demo.c -lgsl -lcblas -latlas -lm
run 2
) >$0.output 2>&1

rm ./a.out

