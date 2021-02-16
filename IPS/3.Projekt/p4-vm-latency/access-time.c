// This is simple test of memeory access time
// Results depend on RDTSC accuracy (it depends on CPU and OS)
// Public domain. Author: peringer AT fit.vutbr.cz

#include <stdio.h>
#include <stdlib.h>

// size of allocated memory block
const size_t SIZE = 500uL * 1024 * 1024;
const size_t PAGE_SIZE = 4096;  // TODO: use getconf()
const size_t PAGE_MASK = PAGE_SIZE-1;
const size_t PAGE_OFFSET = 4;   // start with this offset (in pages)

#ifndef __GNUC__
#error "use GNU C, please"
#endif

#define rdtsc() __builtin_ia32_rdtscp(&xxx)

int main(int argc, char *argv[])
{
    unsigned xxx;               // for RDTSCP

    // Parse arguments
    if (argc > 3) {
        printf("Usage: %s step [N] \n", argv[0]);
        return 1;
    }
    size_t STEP = 4;            // addressing step size
    if (argc > 1)
        STEP = atol(argv[1]);
    if (STEP < 1 || STEP > 65536) {
        printf("Bad step (1..65536 is allowed)\n");
        return 1;
    }
    size_t N = 10;              // number of samples
    const size_t MAX = (SIZE - PAGE_OFFSET*PAGE_SIZE) / STEP - 1;
    if (argc > 2)
        N = atol(argv[2]);
    if (N < 1 || N > MAX) {
        printf("Bad N (1..%zd is allowed)\n", MAX);
        return 2;
    }
    // Allocate memory
    unsigned char *m;           // access time test array
    m = malloc(SIZE);
    if (m == NULL) {
        printf("Error: not enough memory\n");
        return 3;
    }

#if 0
    // Touch memory buffer to eliminate page-faults
    for (int i = 0; i < SIZE; i++) {
        m[i] = 1;
    }
#endif
    unsigned atime[N];          // results
    for (int i = 0; i < N; i++) {   // fill cache
        atime[i] = 0;
    }

    // Get page-aligned address
    unsigned char *address =
        (unsigned char *) ((size_t) (m + PAGE_OFFSET*PAGE_SIZE) & ~PAGE_MASK);
    const char *name = "R";
    long long a, b, sum = 0;

    // Compute min rdtscp time
    long long correction = 1000;
    for (int i = 0; i < 200; i++) {
        a = rdtsc();
        b = rdtsc();
        unsigned long dt = b - a;
        if (dt < correction)
            correction = dt;
    }
    if (correction < 0)
        correction = 0;
    printf("# RDTSC correction=%lld\n", correction);

    // Test of memory access time
    for (int i = 0; i < N; i++) {
        char c;
        a = rdtsc();
        c = *address;           // READ
        b = rdtsc();
        long long dt = (b - a) - correction;
        if (dt < 0)
            dt = 0;
        atime[i] = dt;
        address += STEP;
        sum += c;
    }

    // Print results
    printf("# %s sum=%lld\n", name, sum);
    printf("# step=%zd\n", STEP);
    printf("# N=%zd\n", N);
    printf("# offset[-] time[clk] \n");
    for (int i = 0; i < N; i++)
        printf("%8d %8d \n", (int) (i * STEP), (int) atime[i]);
    return 0;
}
