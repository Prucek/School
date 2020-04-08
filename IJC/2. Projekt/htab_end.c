// iterator = htab_end(t)
//     vrací iterátor označující (neexistující) první záznam za koncem

#include "htab.h"
#include "htab_private.h"

htab_iterator_t htab_end(const htab_t * t)
{
    htab_iterator_t iterator = {.ptr = NULL, .t = NULL , .idx = htab_bucket_count(t)-1 };

    if(t == NULL)
        return iterator;

    iterator.t = t;
    
    for(size_t i = htab_bucket_count(t)-1; i>0; i--)
    {
        //find last valid and return it
        if(t->array[i] != NULL)
        {
            iterator.idx = i;
            break;
        }
    }

    return iterator;
}