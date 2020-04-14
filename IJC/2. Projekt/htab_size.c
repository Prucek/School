//  htab_size.c
//  Riesenie IJC-DU2, příklad b) 8.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

/**
 * @brief htab_size
 * Calculates number of items
 * 
 * @param t Pointer to htab_t
 * 
 * @return Count of non-empty fields in hash table
 */
size_t htab_size(const htab_t * t)
{
    if (t == NULL)
    {
        fprintf(stderr,"Error: htab_size: t is NULL!\n");
        return 0;
    }
    return t->size;
}