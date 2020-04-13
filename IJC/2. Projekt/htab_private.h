#ifndef __HTAB_PRIVATE_H__
#define __HTAB_PRIVATE_H__

#include "htab.h"

struct htab
{
    size_t size;
    size_t arr_size;
    struct htab_item *array[];
};

struct htab_item
{
    htab_key_t key;
    size_t data;
    struct htab_item *next;
};

#endif // __HTAB_PRIVATE_H__

