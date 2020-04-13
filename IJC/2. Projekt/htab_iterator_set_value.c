//  htab_iterator_set_value.c
//  Riesenie IJC-DU2, příklad b) 11.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

/**
 * @brief htab_iterator_set_value
 * Sets value of data in item
 * 
 * @param it Iterator where to set
 * 
 * @param val Value to set
 * 
 * @return Set value on success
 */ 
htab_value_t htab_iterator_set_value(htab_iterator_t it, htab_value_t val)
{
    if (it.ptr == NULL)
    {
        fprintf(stderr,"Error: htab_iterator_set_value: it.ptr is NULL!\n");
        return 0;
    }
    it.ptr->data = val;
    return val;
}