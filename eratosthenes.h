// eratosthenes.h
// Riesenie IJC-DU1, příklad a), 1.3.2020
// Autor: Peter Rucek, xrucek00, FIT
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)


#include "bit_set.h"
#include <math.h>


#ifndef _ERA_H_
#define _ERA_H_

/**
 * @brief Eratosthenes
 * Is looking for prime numbers wtih the Sieve of Eratosthenes algorithm in the bitset,
 * where every index is represnted as a number.
 * In the end, the indexes of the bitset, which are 0 represent the prime numbers.
 * 
 * @param pole Bitset with all 0.
 */
void Eratosthenes(bitset_t pole);

#endif