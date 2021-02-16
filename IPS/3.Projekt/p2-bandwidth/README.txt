
Simple memcpy/memset/read memory bandwidth benchmark
====================================================

This code is only for demonstration purposes

Possible explanation of 1-2KiB fast memcpy on i5:
 - haswell: 72 load / 42 store buffers (see Intel® 64 and IA-32 Architectures Optimization
   Reference Manual, sections "Load and Store Operation Enhancements")

Program mbw - simple memory benchmark (debian package mbw)

TODO:

possible improvements: use avg, not fastest timing

try to test 4 algorithms in single program:
 - memcpy
 - loop copy memory + unroll
 - memset
 - loop set memory + unroll
 - loop read memory + unroll

