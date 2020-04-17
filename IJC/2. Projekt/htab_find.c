//  htab_find.c
//  Riesenie IJC-DU2, příklad b) 12.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

/**
 * @brief htab_find
 * Finds key in hash table
 * 
 * @param t Pointer to hash table
 * 
 * @param key String to find
 * 
 * @return Iterator with all informations
 */
htab_iterator_t htab_find(htab_t * t, htab_key_t key)
{
    if (t == NULL || key == NULL) 
    {
        fprintf(stderr, "Error: htab_lookup_add: t or key is NULL!\n");
        return htab_end(t);
    }
    size_t index = htab_hash_fun(key) % htab_bucket_count(t);
    htab_iterator_t it = { .t = t, .idx = index, .ptr = NULL};

    if (t->array[index] != NULL)
    {
        //index is not empty
        it.ptr = t->array[index];
        while (it.ptr != NULL)
        {
            if(!strcmp(key,it.ptr->key))
                break;
            it.ptr = it.ptr->next;
        }   
    }
    else
    {
        //index is empty
        it = htab_end(t);
    }
    return it;
}