// eratosthenes.c
// Riesenie IJC-DU1, příklad a), 1.3.2020
// Autor: Peter Rucek, xrucek00, FIT
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)
// Implementation of Sieve of Eratosthenes


#include "eratosthenes.h"


void Eratosthenes(bitset_t pole)
{
    bitset_setbit(pole,0,1);
    bitset_setbit(pole,1,1);
    bitset_index_t last_index = bitset_size(pole);
    bitset_index_t look_for_prime_number_to = sqrt(last_index); 
    
    for(bitset_index_t i = 2; i <= look_for_prime_number_to; i++)
    {
        if(!bitset_getbit(pole,i))
        {
            //is prime number
            for(bitset_index_t n = 2; n*i < last_index; n++)
            {
                bitset_setbit(pole,n*i,1);
            }
        }
    }
}
