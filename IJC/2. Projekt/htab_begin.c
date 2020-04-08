// iterator = htab_begin(t)
//     vrací iterátor odkazující na první záznam

#include "htab.h"
#include "htab_private.h"

htab_iterator_t htab_begin(const htab_t * t)
{
    htab_iterator_t iterator = {.ptr = NULL, .t = NULL , .idx = 0 };

    if(t == NULL)
        return iterator;
    
    for(size_t i = 0; i< htab_bucket_count(t); i++)
    {
        //find first valid and return it
        if(t->array[i] != NULL)
        {
            iterator.ptr = t->array[i];
            iterator.idx = i;
            break;
        }
    }

    return iterator;
}