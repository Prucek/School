// htab_iterator_set_value(iterator, nova_hodnota);
//     přepisuje hodnotu, cíl musí existovat

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

htab_value_t htab_iterator_set_value(htab_iterator_t it, htab_value_t val)
{
    if (it.ptr == NULL)
    {
        fprintf(stderr,"htab_iterator_set_value: it.ptr is NULL");
        return 0;
    }
    it.ptr->data = val;
    return val;
}