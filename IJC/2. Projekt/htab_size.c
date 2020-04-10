//size_t s=htab_size(t)           vrátí počet prvků tabulky (.size)

#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

size_t htab_size(const htab_t * t)
{
    if (t == NULL)
    {
        fprintf(stderr,"htab_size: t is NULL ");
        return 0;
    }
    return t->size;
}

