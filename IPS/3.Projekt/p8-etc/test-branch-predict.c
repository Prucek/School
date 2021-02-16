
// test of branch predictor effects
// source: stackoverflow
// use gcc -O2
// Warning: GCC optimizations (CMOV) eliminates the branch

#include <stdio.h>
#include <time.h>               // clock
#include <stdlib.h>             // qsort, rand

int intcmp(const void *p1, const void *p2)
{
    int i1 = *(int *) p1;
    int i2 = *(int *) p2;
    if (i1 < i2)
        return -1;
    if (i1 > i2)
        return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    const unsigned arraySize = 32768;
    int data[arraySize];

    // Generate (pseudo)random data
    for (unsigned c = 0; c < arraySize; ++c)
        data[c] = rand() % 256;

    // With this, the next loop runs faster (with branch predictor)
    if (argc > 1) {
        printf("sorted\t");
        qsort(data, arraySize, sizeof(data[0]), &intcmp);
    }
    // Test
    clock_t start = clock();
    long long sum = 0;

    for (unsigned i = 0; i < 100000; ++i) {
        // Primary loop
        for (unsigned c = 0; c < arraySize; ++c) {
            if (data[c] >= 128) // conditional jump
                sum += data[c];
        }
    }

    double elapsedTime = (double) (clock() - start) / CLOCKS_PER_SEC;

    printf("time= %gs\n", elapsedTime);
    printf("sum= %lld\n", sum);
    return 0;
}
