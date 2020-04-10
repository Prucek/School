//htab_clear(t)             zrušení všech položek, tabulka zůstane prázdná

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"

void htab_clear(htab_t * t)
{
    if(t == NULL)
    {
        fprintf(stderr,"htab_clear: t is NULL\n");
        return;
    }

    for(htab_iterator_t it = htab_begin(t); !htab_iterator_equal(it,htab_end(t)); )
    {
        htab_iterator_t tmp = it;
        it = htab_iterator_next(it);
        free(tmp.ptr);
    }   

    // htab_iterator_t it = htab_begin(t);
    // htab_iterator_t tmp = it;
    
    // it = htab_iterator_next(it);
    // free(tmp.ptr);
    // free(it.ptr);

}