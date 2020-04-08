// t=htab_init(numb)         konstruktor: vytvoření a inicializace tabulky
//                                   numb = počet prvků pole (.arr_size)

// - Když htab_init nebo htab_lookup_add nemohou alokovat paměť,
//     vrací NULL nebo nevalidní iterátor htab_end
//     (a uživatel musí testovat výsledek těchto operací)

#include <stdlib.h>
#include <stdio.h>
#include "htab.h"
#include "htab_private.h"

htab_t *htab_init(size_t n)
{
    if (n <= 0)
    {
        fprintf(stderr,"htab_init: n < 0!\n");
        return NULL;
    }
    htab_t *new_htab = malloc(sizeof(htab_t));
    if(new_htab == NULL)
    {
        fprintf(stderr,"htab_init: Malloc failed\n");
        return NULL;
    }

    new_htab->arr_size = n;
    new_htab->size = 0;
    for (size_t i = 0; i < htab_bucket_count(new_htab); i++)
    {
        new_htab->array[i] = malloc(sizeof(struct htab_item));
        new_htab->array[i] = NULL;
    }

    return new_htab;
}