// cstr = htab_iterator_get_key(iterator);
//     vrací klíč, cíl musí existovat

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

htab_key_t htab_iterator_get_key(htab_iterator_t it)
{
    if (it.ptr == NULL)
    {
        fprintf(stderr,"htab_iterator_get_key: it.ptr is NULL");
        return NULL;
    }
    return it.ptr->key;
}