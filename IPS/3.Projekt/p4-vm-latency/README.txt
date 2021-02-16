

Simple test of memory access timing
(demonstration only)

Program measures memory read timing using RDTSC instruction.
There is some noise (and maybe some Spectre workarounds effects?),
but the page-faults are clearly visible (each PAGESIZE=4096 bytes).

