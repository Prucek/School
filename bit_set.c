// bitset.h
// Riesenie IJC-DU1, příklad a), 28.2.2020
// Autor: Peter Rucek, xrucek00, FIT 
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)
// Extern inline functions


#include "bit_set.h"


#ifdef USE_INLINE

    extern void biset_free(bitset_t jmeno_pole);
    extern bitset_index_t bitset_size(bitset_t jmeno_pole);
    extern void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, bitset_index_t vyraz);
    extern int bitset_getbit(bitset_t jmeno_pole, bitset_index_t index);

#endif