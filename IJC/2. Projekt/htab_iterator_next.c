// iterator = htab_iterator_next(iterator)
//     posun iterátoru na další záznam v tabulce (nebo na htab_end(t))
//     Poznámka: toto dělá i C++ operátor ++ pro iterátory

#include "htab.h"
#include "htab_private.h"

htab_iterator_t htab_iterator_next(htab_iterator_t it)
{
    if(!htab_iterator_valid(it))
        return it;
    if(it.ptr->next == NULL)
    {
        for(size_t i = it.idx+1; i < htab_bucket_count(it.t); i++)
        {
            if(it.t->array[i] != NULL)
            {
                it.ptr = it.t->array[i];
                it.idx = i;
                return it;
            }
        }

        return htab_end(it.t);
    }

    it.ptr = it.ptr->next;
    return it;
}