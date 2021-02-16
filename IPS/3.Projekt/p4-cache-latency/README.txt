
Testing memory access time _without_ page-faults (demo)

Program measures memory read timing using RDTSC instruction.
There is some noise (and maybe some Spectre workarounds effects?),

See:
 - L1 cache-hit time
 - L1 cache-miss time

