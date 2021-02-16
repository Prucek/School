/*
 * this is simple test of interrupt service time measuring
 *
 * Process is interrupted by timer and other hardware interrupts,
 *         it can be suspended, context can be switched, etc...
 *
 * Comments:
 *   - we display only interrupt time greater than MINCYCLES
 *   - use with GCC and optimization -O2
 *
 * Public domain. Author: peringer AT fit.vutbr.cz
 *
 */

#include <stdio.h>
#include <stdlib.h>

long MAX = 100;			/* default: how much interrupts */

const long long MINCYCLES=200;	/* minimal clock count displayed */

#ifndef __GNUC__
#error "use GNU C, please"
#endif

#define rdtsc() __builtin_ia32_rdtscp(&xxx)

long long *m;			/* duration */
long long *t;			/* time */

int main(int argc, char *argv[])
{
    unsigned xxx;               // for RDTSCP instruction

    fprintf(stderr, "Timing of interrupts >%lld TICKS ...\n", MINCYCLES);
    /* parse arguments */
    if (argc > 2) {
	fprintf(stderr, "Usage: %s number_of_ints \n", argv[0]);
	return 1;
    }
    if (argc > 1)
	MAX = atol(argv[1]);
    if (MAX < 1 || MAX > 10000) {
	fprintf(stderr, "Bad number of interrupts (1..10000 is allowed)\n");
	return 1;
    }
    /* allocate memory */
    t = (long long *) malloc(MAX * sizeof(long long));
    m = (long long *) malloc(MAX * sizeof(long long));
    if (m == NULL || t == NULL) {
	fprintf(stderr, "ERROR: not enough memory\n");
	return 1;
    }
    /* touch memory buffers to eliminate page-faults */
    for (long i = 0; i < MAX; i++) {
	t[i] = 0;
	m[i] = 0;
    }

    long long a, b;
    /* wait for first interrupt */
    a = rdtsc();
    for (;;) {
	b = rdtsc();
	long long dt = b - a;
	if (dt > MINCYCLES) {
	    break;
	}
	a = b;
    }
    /* start by an interrupt for more readable output */
    long long start = a;
    int i = 0;
    do {
	b = rdtsc();
	long long dt = b - a;
	if (dt > MINCYCLES) {   // only if long enough
	    t[i] = a;           // record the time and duration
	    m[i] = dt;
            ++i;
	}
	a = b;
    } while(i<MAX); /* end after MAX records */
    long long end = rdtsc();
    /* print time of all interrupts: */
    printf("#     time [clk]   dt [clk] \n");
    for (i = 0; i < MAX; i++)
	printf("%4d %10lld %10lld \n", i, (t[i] - start), m[i]);
    printf("# Total time: %lld clk\n", (end - start));
    return 0;
}

