// i = htab_iterator_get_value(iterator);
//     vrací hodnotu, cíl musí existovat

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

htab_value_t htab_iterator_get_value(htab_iterator_t it)
{
    if (it.ptr == NULL)
    {
        fprintf(stderr,"htab_iterator_get_key: it.ptr is NULL");
        return 0;
    }
    return it.ptr->data;
}