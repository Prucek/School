//  htab_bucket_count.c
//  Riesenie IJC-DU2, příklad b) 12.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

/**
 * @brief htab_begin
 * Shifts to the first valid item
 * 
 * @param t Pointer to hash table
 * 
 * @return Iterator with all informations
 */
htab_iterator_t htab_begin(const htab_t * t)
{
    htab_iterator_t it = {.ptr = NULL, .t = NULL , .idx = 0 };

    if(t == NULL)
    {
        fprintf(stderr,"Error: htab_begin: t is NULL!\n");
        return it;
    }
    it.t = t;
    
    for(size_t i = 0; i< htab_bucket_count(t); i++)
    {
        //find first valid
        if(t->array[i] != NULL)
        {
            it.ptr = t->array[i];
            it.idx = i;
            break;
        }
    }
    return it;
}