//  htab_bucket_count.c
//  Riesenie IJC-DU2, příklad b) 12.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "htab.h"
#include "htab_private.h"

/**
 * @brief htab_bucket_count
 * Calculates size of the array
 * 
 * @param t Pointer to hash table
 * 
 * @return Size of the array in hash table
 */
size_t htab_bucket_count(const htab_t * t)
{
    if (t == NULL)
        return 0;
    return t->arr_size;
}