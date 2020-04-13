//  htab_lookup_add.c
//  Riesenie IJC-DU2, příklad b) 12.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @brief htab_lookup_add
 * Finds key in hash table or adds new item if key was not fount
 * 
 * @param t Pointer to hash table
 * 
 * @param key Word to find or add
 * 
 * @return Iterator with all informations
 */
htab_iterator_t htab_lookup_add(htab_t * t, htab_key_t key)
{
    if (t == NULL || key == NULL) 
    {
        fprintf(stderr, "Error: htab_lookup_add: t or key is NULL!\n");
        return htab_end(t);
    }

    //find if exist
    htab_iterator_t it = htab_find(t,key);


    if (it.ptr != NULL)
    {
        //look up
        it.ptr->data++;   
    }
    else
    {
        //add        
        bool same_index = false;
        size_t index = htab_hash_fun(key) % htab_bucket_count(t);

        struct htab_item *item = t->array[index];
        struct htab_item *tmp;
        while (item != NULL)
        {
            //if more hashes on one index, go to the last one
            same_index = true;
            tmp = item;
            item = item->next;
        }

        //allocation
        item = malloc(sizeof(struct htab_item));
        if(item == NULL)
        {
            fprintf(stderr,"Error: htab_lookup_add: Malloc failed!\n");
            return it;
        }

        //new item was added
        t->size++;

        //fill iterator and htab_item values
        it.idx = index;
        it.ptr = item;

        //allocation of the key and add value
        it.ptr->key = malloc(strlen(key)+1);
        if(it.ptr->key == NULL)
        {
            fprintf(stderr,"Error: htab_lookup_add: Malloc failed!\n");
            return it;
        }

        strcpy((char *)it.ptr->key,key);

        //first appearance
        it.ptr->data = 1;

        //rebinding
        if(same_index)
        {
            tmp->next = item;
        }
        if(!same_index)
        {
            t->array[index] = item;
        }

        it.ptr->next = NULL;       
    } 
    return it;
}