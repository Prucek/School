make
(
perf stat ./pagefaults 1
perf stat ./pagefaults 10000
) >/dev/null 2>pagefaults-perf.out
