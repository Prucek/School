//  htab_iterator_next.c
//  Riesenie IJC-DU2, příklad b) 11.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

/**
 * @brief htab_iterator_next
 * Shifts to the next valid item
 * 
 * @param it Iterator of the current position
 * 
 * @return Iterator of the next value
 */
htab_iterator_t htab_iterator_next(htab_iterator_t it)
{
    if(!htab_iterator_valid(it))
    {
        fprintf(stderr,"Error: htab_iterator_next: it invalid!\n");
        return it;
    }
        
    if(it.ptr->next == NULL)
    {
        //find the nearest possible nonempty field in array
        for(size_t i = it.idx+1; i < htab_bucket_count(it.t); i++)
        {
            if(it.t->array[i] != NULL)
            {
                it.ptr = it.t->array[i];
                it.idx = i;
                return it;
            }
        }
        it = htab_end(it.t);
    }
    else
    {
        //if there are on same index more items
        it.ptr = it.ptr->next;
    }
    return it;
}