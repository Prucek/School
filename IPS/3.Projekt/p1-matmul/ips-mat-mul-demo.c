
// Test locality of reference
// Multiplication of matrix NxN
// Depends on GSL (GNU Scientific Library, Debian package libgsl-dev)
// Compile using optimization -O2 or -O3
//   gcc -O3 ips-mat-mul-demo.c -lgsl -lcblas -lm

#define _GNU_SOURCE
#include <stdio.h>              // fprintf()
#include <stdlib.h>             // rand(), atoi()
#include <string.h>             // memset()
#include <time.h>               // clock()
#include <math.h>               // fabs()
#include <gsl/gsl_blas.h>       // GSL

//////////////////////////////////////////////////////////////////
#if defined(__linux)    // Linux-only implementation
#include <unistd.h>     // getpid()
#include <sched.h>      // sched_setaffinity, cpu_set_t, CPU_SET
void lock_on_core1(void) {
    cpu_set_t set;
    CPU_SET(1, &set);
    if (sched_setaffinity(getpid(), sizeof(set), &set) == -1)
        perror("sched_setaffinity");
}
#else
#  define lock_on_core1()
#endif
//////////////////////////////////////////////////////////////////

// textbook implementation O(N^3)
void mat_mul1(int n, double m1[n][n], double m2[n][n], double m3[n][n])
{
    int i, j, k;
    memset(m3, 0, sizeof(double)*n*n);
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            for (k = 0; k < n; ++k) {
                m3[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

/*

From Wikipedia:

By switching the looping order for j and k, the speedup in large matrix
multiplications becomes dramatic, at least for languages that put contiguous
array elements in the last dimension. This will not change the mathematical
result, but it improves efficiency. In this case, "large" means, approximately,
more than 100,000 elements in each matrix, or enough addressable memory such
that the matrices will not fit in L1 and L2 caches.

for i in 0..n
  for k in 0..p
    for j in 0..m
      C[i][j] = C[i][j] + A[i][k] * B[k][j];

The reason for this speed up is that in the first case, the reads of A[i][k]
are in cache (since the k index is the contiguous, last dimension), but B[k][j]
is not, so there is a cache miss penalty on B[k][j]. C[i][j] is irrelevant,
because it can be factored out of the inner loop. In the second case, the reads
and writes of C[i][j] are both in cache, the reads of B[k][j] are in cache, and
the read of A[i][k] can be factored out of the inner loop. Thus, the second
example has no cache miss penalty in the inner loop while the first example has
a cache penalty.

On a year 2014 processor, the second case is approximately five times faster
that the first case, when written in C and compiled with gcc -O3. (A careful
examination of the disassembled code shows that in the first case, gcc uses
SIMD instructions and in the second case it does not, but the cache penalty is
much worse than the SIMD gain).^[citation needed]

*/

// optimized textbook implementation O(N^3)
void mat_mul2(int n, double m1[n][n], double m2[n][n], double m3[n][n])
{
    int i, j, k;
    memset(m3, 0, sizeof(double)*n*n);
    for (i = 0; i < n; ++i) {
        for (k = 0; k < n; ++k) {
            for (j = 0; j < n; ++j) {
                m3[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

// slightly optimized textbook implementation O(N^3)
void mat_mul3(int n, double m1[n][n], double m2[n][n], double m3[n][n])
{
    int i, j, k;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            double sum = 0.0;
            for (k = 0; k < n; ++k) {
                sum += m1[i][k] * m2[k][j];
            }
            m3[i][j] = sum;
        }
    }
}

// BLAS (cblas, ATLAS, ...)
void mat_mul4(int n, double m1[n][n], double m2[n][n], double m3[n][n])
//void mat_mul4(int n, double *m1, double *m2, double *m3)
{
/*
int  gsl_blas_dgemm (CBLAS_TRANSPOSE_t TransA,
                     CBLAS_TRANSPOSE_t TransB,
                     double alpha,
                     const gsl_matrix * A,
                     const gsl_matrix * B,
                     double beta,
                     gsl_matrix * C);
*/
    gsl_matrix_view _m1 = gsl_matrix_view_array((double *) m1, n, n);
    gsl_matrix_view _m2 = gsl_matrix_view_array((double *) m2, n, n);
    gsl_matrix_view _m3 = gsl_matrix_view_array((double *) m3, n, n);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
                   1, &_m1.matrix,
                      &_m2.matrix,
                   0, &_m3.matrix);
}

inline size_t min(size_t a, size_t b) {
    return (a<b)? a : b;
}

void mat_mul5(int n, double m1[n][n], double m2[n][n], double m3[n][n])
{
    const size_t step=64;
    size_t i,j,k,jj,kk;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            m3[i][j] = 0.0;
        }
    }
    for (kk = 0; kk < n; kk += step) {
        for (jj = 0; jj < n; jj += step) {
            for (i = 0; i < n; i++) {
                for (k = kk; k < min(kk + step, n); k++) {
                    double x = m1[i][k];
                    for (j = jj; j < min(jj + step, n); j++) {
                        m3[i][j] += x * m2[k][j];
                    }
                }
            }
        }
    }
}

// prepare pseudorandom values
void mat_fill_random(int n, double m[n][n])
{
    int i, j;
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            m[i][j] = 10.0 * rand() / (RAND_MAX + 1.0);
        }
    }
}

// for testing only
static void mat_print(int n, double m[n][n])
{
    int i, j;
    printf("{\n");
    for (i = 0; i < n; ++i) {
        for (j = 0; j < n; ++j) {
            printf("%10.6f ", m[i][j]);
        }
        printf("\n");
    }
    printf("}\n");
}

int main(int argc, char *argv[])
{
    int n = 0;
    if (argc == 2)
        n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr,
                "Usage: %s N\n"
                "       where N is matrix size\n", argv[0]);
        return 1;
    }
    // Print header
    //printf("# N alg1 alg2 alg3 alg4 alg5\n");
    // Print matrix size
    printf("%d ", n);
    lock_on_core1();
    double *m1 = malloc(sizeof(double) * n * n);
    double *m2 = malloc(sizeof(double) * n * n);
    if (m1 == NULL || m2 == NULL)
        exit(1);
    mat_fill_random(n, m1);
    mat_fill_random(n, m2);

    const int NALG=5;
    double *m3[NALG];
    for(int alg = 0; alg < NALG; ++alg) {
        m3[alg] = malloc(sizeof(double) * n * n);
        if (m3 == NULL)
            exit(2);
    }

    for (int alg = 0; alg < NALG; ++alg) {
        clock_t start = clock();
        switch (alg+1) {
        case 1:
            mat_mul1(n, m1, m2, m3[alg]);
            break;
        case 2:
            mat_mul2(n, m1, m2, m3[alg]);
            break;
        case 3:
            mat_mul3(n, m1, m2, m3[alg]);
            break;
        case 4:
            mat_mul4(n, m1, m2, m3[alg]);
            break;
        case 5:
            mat_mul5(n, m1, m2, m3[alg]);
            break;
        default:
            fprintf(stderr, "Error: invalid algorithm number\n");
            break;
        };
        clock_t end = clock();
        // print time elapsed
        printf("%.2f ", (double) (end - start) / CLOCKS_PER_SEC);
    }

    printf("\n");

    free(m1);
    free(m2);

    for(int alg = 1; alg < NALG; ++alg) {
#if 0
        // Check if the results are equal
        // Warning: 
        //  - BLAS algorithms can have small errors (different rounding?)

        if (memcmp(m3[0], m3[alg], sizeof(double) * n * n)) {
            fprintf(stderr, "ERROR in algorithm %d\n", alg+1);
            double sum=0;
            for(long i=0; i < n * n; ++i)
                sum+=fabs(m3[alg][i]-m3[0][i]);
            printf(" error = %g\n", sum);
            printf("==================================================================\n");
            mat_print(n,m3[0]);
            printf("==================================================================\n");
            mat_print(n,m3[alg]);
            printf("==================================================================\n");
        }
#endif
        free(m3[alg]);
    }
    free(m3[0]);        // reference result

    return 0;
}
