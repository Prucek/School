//  htab_erase.c
//  Riesenie IJC-DU2, příklad b) 12.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "htab.h"
#include "htab_private.h"
#include <stdlib.h>

/**
 * @brief htab_erase
 * Deletes one item
 * 
 * @param t Pointer to hash table 
 * 
 * @param it Iterator where to delete
 */
void htab_erase(htab_t * t, htab_iterator_t it)
{
    free((char *)it.ptr->key);
    free(it.ptr);
}