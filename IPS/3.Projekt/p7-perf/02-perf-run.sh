
CFLAGS="-O3 -g -Wall"                 ## set GCC options
#N=768
N=1024

gcc $CFLAGS -DSIZE=$N ips-mat-mul-demo.c -lgsl -lcblas -latlas -lm

### run
perf record \
 -e L1-dcache-load-misses,faults,cycles,dTLB-load-misses \
 -F 250 \
 -- ./a.out 1

### report
perf report --stdio --header >perf-report

### interactive
#perf report
rm ./a.out

