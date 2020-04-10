//iterator=htab_lookup_add(t,key)   vyhledávání+přidání - viz dále
// htab_iterator_t htab_lookup_add(htab_t *t, htab_key_t key);
// V tabulce  t  vyhledá záznam odpovídající řetězci  key  a
// - pokud jej nalezne, vrátí iterátor na záznam
// - pokud nenalezne, automaticky přidá záznam a vrátí iterátor
// Poznámka: Dobře promyslete chování této funkce k parametru key.
// Poznámka: podobně se chová C++ operátor [] pro std::unordered_map

// - Když htab_init nebo htab_lookup_add nemohou alokovat paměť,
//     vrací NULL nebo nevalidní iterátor htab_end
//     (a uživatel musí testovat výsledek těchto operací)

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"

htab_iterator_t htab_lookup_add(htab_t * t, htab_key_t key)
{
    if (t == NULL || key == NULL) {
        fprintf(stderr, "htab_lookup_add: t or key is NULL\n");
        return htab_end(t);
    }
    //together
    size_t index = htab_hash_fun(key) % htab_bucket_count(t);
    
    htab_iterator_t it = { .t = t, .idx = index};
    //lookup
    if (t->array[index] != NULL)
    {
        //TODO cmp keys

        //if keys are equal
        it.ptr = t->array[index];
        it.ptr->data++;
        
    }
    else
    {
        //add

        it.ptr = malloc(sizeof(struct htab_item));
        if( it.ptr == NULL)
        {
            fprintf(stderr,"htab_lookup_add: Malloc failed\n");
            return it;
        }

        t->array[index] = it.ptr ;
        it.ptr->key = key;
        it.ptr->data = 1;
        it.ptr->next = NULL;

        t->size++;
    } 
    return it;
}