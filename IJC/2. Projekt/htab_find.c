//  htab_find.c
//  Riesenie IJC-DU2, příklad b) 12.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "htab.h"
#include "htab_private.h"

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
    size_t index = htab_hash_fun(key) % htab_bucket_count(t);
    htab_iterator_t it = { .t = t, .idx = index};

    if (t->array[index] != NULL)
    {
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
        it = htab_end(t);
    }
    return it;
}