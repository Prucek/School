// htab_private.h
// Riesenie IJC-DU2, příklad b), 7.4.2020
// Autor: Peter Rucek, xrucek00, FIT
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)
// Encapsulation of the htab and htab item structures

#ifndef __HTAB_PRIVATE_H__
#define __HTAB_PRIVATE_H__

#include "htab.h"

/**
 * @brief htab 
 * Hash table structure with flexible array
 */
struct htab
{
    size_t size;                //size is number of nonempty array indexes
    size_t arr_size;            //total length of the array
    struct htab_item *array[];  //array of pointers to the htab_item
};

/**
 * @brief htab_item 
 * Hash table item structure
 */
struct htab_item
{
    htab_key_t key;         //key is const char*
    size_t data;            //data is number of appearance in file
    struct htab_item *next; //next is pointer to next item, if hashed on same index
};

#endif // __HTAB_PRIVATE_H__