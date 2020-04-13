//  htab_free.c
//  Riesenie IJC-DU2, příklad b) 7.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @brief htab_free
 * Destructor of the hash table
 * 
 * @param t Pointer to hash table
 */
void htab_free(htab_t * t)
{
    htab_clear(t);
    free(t);
}