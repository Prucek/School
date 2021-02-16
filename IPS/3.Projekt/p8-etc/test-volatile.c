
// Benchmark

// Inspired by:
// https://eli.thegreenplace.net/2013/12/03/intel-i7-loop-performance-anomaly

// Use:
//   gcc -O2 this.c; time ./a.out

// RESULTS for i5-4690: with call: 1.555s, no call:   1.677s

#include <time.h>
#include <stdio.h>

// empty function
void f(void)
{
}

// test of CPU performance
int main(void)
{
    // NOTE: volatile
    volatile unsigned long count;
    clock_t start;
    clock_t end;

    printf("Test of volatile counter performance:\n");

    for (int i = 0; i < 3; ++i) {
        // test with call of f()
        count = 0;
        start = clock();
        for (unsigned long i = 0; i < 1000000000UL; ++i) {
            __asm__("call f");  // not optimized away
            count++;
        }
        end = clock();
        printf("with call: %.3fs\n",
               ((double) (end - start) / CLOCKS_PER_SEC));

        // test without call of f()
        count = 0;
        start = clock();
        for (unsigned long i = 0; i < 1000000000UL; ++i) {
            count++;
        }
        end = clock();
        printf("no call:   %.3fs\n",
               ((double) (end - start) / CLOCKS_PER_SEC));

        printf("\n");
    }

    return count % 16;
}
