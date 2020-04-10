//iterator=htab_find(t,key)         vyhledávání - viz dále
// htab_iterator_t htab_find(htab_t *t, htab_key_t key);
// V tabulce  t  vyhledá záznam odpovídající řetězci  key  a
//   - pokud jej nalezne, vrátí iterátor na záznam
//   - pokud nenalezne, vrátí iterátor htab_end(t)

#include "htab.h"
#include "htab_private.h"

htab_iterator_t htab_find(htab_t * t, htab_key_t key)
{
    size_t index = htab_hash_fun(key) % htab_bucket_count(t);
    htab_iterator_t it = { .t = t, .idx = index};

    if (t->array[index] != NULL)
    {
        it.ptr = t->array[index];
        while (it.ptr != NULL)
        {
            if(!strcmp(key,it.ptr->key))
                break;
            it.ptr = it.ptr->next;
        }
        
    }
    else
    {
        it = htab_end(t);
    }

    return it;
}