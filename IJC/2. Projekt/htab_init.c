//  htab_init.c
//  Riesenie IJC-DU2, příklad b) 7.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include <stdlib.h>
#include <stdio.h>
#include "htab.h"
#include "htab_private.h"

/**
 * @brief htab_init
 * Constructor of the hash table
 * 
 * @param n Array size
 * 
 * @return Pointer to hash table
 */
htab_t *htab_init(size_t n)
{
    if (n <= 0)
    {
        fprintf(stderr,"Error: htab_init: n <= 0!\n");
        return NULL;
    }
    //alloc of metadata
    htab_t *new_htab = malloc(sizeof(htab_t)+sizeof(char *)*n);
    if(new_htab == NULL)
    {
        fprintf(stderr,"Error: htab_init: Malloc failed!\n");
        return NULL;
    }

    //ititialization of the hash table values
    new_htab->size = 0;
    new_htab->arr_size = n;
    for (size_t i = 0; i < n; i++)
    {
        new_htab->array[i] = NULL;
    }

    return new_htab;
}