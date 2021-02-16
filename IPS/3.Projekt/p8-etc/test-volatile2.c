
// Benchmark

// Inspired by:
// https://eli.thegreenplace.net/2013/12/03/intel-i7-loop-performance-anomaly

// Use:
//   gcc -O2 this.c; time ./a.out
//   gcc -O2 -DCALL this.c; time ./a.out

// RESULTS for i5-4690: cca 1.75s vs 1.35s  !

// empty function
void f(void) {}

// test of CPU performance
int main(void) {
    // NOTE: volatile
    volatile unsigned long count=0;

    for(unsigned long i=0; i<1000000000UL; ++i) {
#ifdef CALL
        __asm__("call f");      // not optimized away
#endif
        count++;
    }

    return count % 16;
}
