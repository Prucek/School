// primes.c
// Riesenie IJC-DU1, příklad a), 2.3.2020
// Autor: Peter Rucek, xrucek00, FIT 
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)


#include "bit_set.h"
#include "eratosthenes.h"
#include <stdio.h>
#include <time.h>


#define LIMIT 500000000


/**
 * @brief print_last_10
 * Prints the last 10 prime numbers in the bitset.
 * 
 * @param pole Bitset with 0 represented as prime numbers.
 */
void print_last_10(bitset_t pole)
{
    int count = 0;
    bitset_index_t index_to_start = 0;
    for (bitset_index_t i = bitset_size(pole)-1; count != 10 && i != 1; i--)
    {
        if(bitset_getbit(pole,i) == 0)
        {
            count++;
            index_to_start = i;
        }
    }
    for(bitset_index_t j = index_to_start; j <  bitset_size(pole); j++)
    {
         if(bitset_getbit(pole,j) == 0)
        {
            printf("%lu\n", j);
        }
    }
}


int main(void)
{
    clock_t start;
    start = clock();

    bitset_create(pole,LIMIT);

    Eratosthenes(pole);

    print_last_10(pole);

    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);

    return 0;
}
