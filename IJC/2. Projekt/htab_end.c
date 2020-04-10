// iterator = htab_end(t)
//     vrací iterátor označující (neexistující) první záznam za koncem

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

htab_iterator_t htab_end(const htab_t * t)
{
    htab_iterator_t iterator = {.ptr = NULL, .t = NULL , .idx = htab_size(t)-1 };

    if(t == NULL)
    {
        fprintf(stderr,"htab_end: t is NULL!\n");
        return iterator;
    }

    iterator.t = t;
    
    for(size_t i = htab_bucket_count(t)-1; i>0; i--)
    {
        //find last valid and return it
        if(t->array[i] != NULL)
        {
            //index after last existing                
            iterator.idx = i+1;
            break;
        }
    }

    return iterator;
}