//  htab_iterator_get_value.c
//  Riesenie IJC-DU2, příklad b) 9.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "htab.h"
#include "htab_private.h"
#include <assert.h>

/**
 * @brief htab_iterator_get_value
 * Gets value of data in item
 * 
 * @param it Iterator where to get
 * 
 * @return Getted value on success
 */ 
htab_value_t htab_iterator_get_value(htab_iterator_t it)
{
    assert(htab_iterator_valid(it));
    return it.ptr->data;
}