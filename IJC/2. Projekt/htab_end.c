// iterator = htab_end(t)
//     vrací iterátor označující (neexistující) první záznam za koncem

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

htab_iterator_t htab_end(const htab_t * t)
{
    htab_iterator_t it = {.ptr = NULL, .t = NULL };

    if(t == NULL)
    {
        fprintf(stderr,"htab_end: t is NULL!\n");
        return it;
    }

    it.t = t;
    
    for(size_t i = htab_bucket_count(t)-1; i>0; i--)
    {
        //find last valid and return it
        if(t->array[i] != NULL)
        {
            //index after last existing                
            it.idx = i+1;
            break;
        }
    }

    return it;
}