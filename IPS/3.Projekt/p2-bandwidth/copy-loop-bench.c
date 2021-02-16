/*
 *  copy-loop-bench.c --- basic test of memory access speed
 *  Warning: measuring it is not exact!
 *
 */

/* max size of buffers */
#define MSIZE (256*1024*1024L)

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//////////////////////////////////////////////////////////////////
#ifdef __linux__
// Linux-only implementation
#include <unistd.h>             // getpid()
#include <sched.h>              // sched_setaffinity, cpu_set_t, CPU_SET
void lock_on_core1(void)
{
    cpu_set_t set;
    CPU_SET(1, &set);
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        perror("sched_setaffinity");
}
#else
#  define lock_on_core1()
#endif
//////////////////////////////////////////////////////////////////

/* buffers */
char *buffer1;
char *buffer2;

void print_time(double dt, long size)
{
    printf("%10.3f %12.0f\n", size / 1024.0, size / (1024.0 * 1024.0) / dt);    /* KB, MiB/s */
    fflush(stdin);
}

// TODO: try restrict
// Warning: not universal:
//   - needs sizeof(long) aligned sz
//   - sz is limited to UINT_MAX*sizeof(long)
static void Copy(void *dst, void *src, size_t sz) {
    long *p=dst;
    long *s=src;
    for(unsigned u=0; u<sz/sizeof(*p); ++u)
        *p++ = *s++;
}

volatile long l;
void MEASURE(size_t size)
{
    clock_t start;
    clock_t delta;
    clock_t delta_min=(unsigned)-1;
    static long n = 10000000;
    Copy(buffer2, buffer1, size);     // cache fill
    //////////////////////////////////////////////////////////////////
    start = clock();
    for (long i = 0; i < n; ++i)
        Copy(buffer2, buffer1, size);
    delta = clock() - start;    // time
    if(delta<delta_min) delta_min=delta;
    //////////////////////////////////////////////////////////
    start = clock();
    for (long i = 0; i < n; ++i)
        Copy(buffer2, buffer1, size);
    delta = clock() - start;    // time
    if(delta<delta_min) delta_min=delta;
    //////////////////////////////////////////////////////////
    double t = (double) delta_min / CLOCKS_PER_SEC; //      [s]
    print_time(t / n, size);    // print time
    if (t > 0.5)
        n /= 2;                 // keep measuring time low
}

/* main test */
int main(int argc, const char *argv[])
{
    lock_on_core1();
    size_t n;
    size_t size;
    size_t step;
    size_t end = MSIZE;
    if (argc > 1)
        end = atoi(argv[1]) * 1024 * 1024L;
    if (end > MSIZE)
        end = MSIZE;
    buffer1 = malloc(MSIZE);
    buffer2 = malloc(MSIZE);
    if (buffer1 == NULL || buffer2 == NULL) {
        fprintf(stderr, "Error: no memory\n");
        return 1;
    }
#if 1
    /* try without this initialization -- interesting results */
    unsigned x = 666;
    for (n = 0; n < MSIZE; n++) {
        buffer1[n] = x = x * 13 + 111;
        buffer2[n] = x = x * 7 + 333;
    }
#endif
    printf("# data for GNUPLOT\n");
    printf("# PARAMETERS: MSIZE = %zu KB\n", (size_t) MSIZE / 1024UL);
    printf("# block size in KB      transfer rate in MB/s\n\n");

    for (size = 128; size < end; size += step) {
        MEASURE(size);
        unsigned long i;
        for (i = 1L << 8; i < size; i <<= 1)
            /* empty */ ;
        step = i >> 5;
    }
    return 0;
}
