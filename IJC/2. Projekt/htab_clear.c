//  htab_clear.c
//  Riesenie IJC-DU2, příklad b) 12.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include <stdio.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @brief htab_clear
 * Deletes every item from table
 * 
 * @param t Pointer to hash table
 */
void htab_clear(htab_t * t)
{
    if(t == NULL)
    {
        fprintf(stderr,"Error: htab_clear: t is NULL!\n");
        return;
    }

    for(htab_iterator_t it = htab_begin(t); !htab_iterator_equal(it,htab_end(t)); )
    {
        htab_iterator_t tmp = it;
        it = htab_iterator_next(it);   
        htab_erase(t, tmp);
    }   
}