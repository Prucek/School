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
    htab_iterator_t it = htab_find(t,key);

    //lookup
    if (it.ptr != NULL)
    {
        it.ptr->data++;   
    }
    else
    {
        //add
        
        bool same_index = false;
        size_t index = htab_hash_fun(key) % htab_bucket_count(t);

        struct htab_item *item = t->array[index];
        struct htab_item *tmp;
        while (item != NULL)
        {
            same_index = true;
            tmp = item;
            item = item->next;
        }

        item = malloc(sizeof(struct htab_item));
        if(item == NULL)
        {
            fprintf(stderr,"htab_lookup_add: Malloc failed\n");
            return it;
        }
        


        it.idx = index;
        it.ptr = item;
        
        t->size++;

        //it.ptr->key = malloc(sizeof(htab_key_t));
        // void my_copy(char s[], const char s1[]) { /* s1 do s */
        // while( (*s++ = *s1++) != '\0' );
        // }
        // char *strcpy(char *s1, const char *s2) {
        // char *s = s1;
        // while( *s++ = *s2++ );
        // return s1;
        // }

        //it.ptr->key = malloc(sizeof(htab_key_t)+1);
        it.ptr->key = malloc(strlen(key)+1);
        strcpy((char *)it.ptr->key,key);
        //strcpy(it.ptr->key, key);
        it.ptr->data = 1;

        if(same_index)
        {
            tmp->next = item;
        }
        if(!same_index)
        {
            t->array[index] = item;
        }

        it.ptr->next = NULL;
         
    } 
    return it;
}