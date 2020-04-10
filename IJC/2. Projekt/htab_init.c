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
    htab_t *new_htab = malloc(sizeof(htab_t)+sizeof(char *)*n);//alloc of metadata
    //new_htab->array[n] = malloc(sizeof(char *)*n); //alloc of table //size of pointer ??const char * htab_key_t??
    if(new_htab == NULL)
    {
        fprintf(stderr,"htab_init: Malloc failed\n");
        return NULL;
    }

    new_htab->size = 0;
    new_htab->arr_size = n;
    for (size_t i = 0; i < htab_bucket_count(new_htab); i++)// ??n or new_htab->arr_size??
    {
        new_htab->array[i] = NULL;
    }

    return new_htab;
}