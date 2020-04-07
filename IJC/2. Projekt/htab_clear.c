//htab_clear(t)             zrušení všech položek, tabulka zůstane prázdná

#include "htab.h"
#include "htab_private.h"

void htab_clear(htab_t * t)
{
    if(t == NULL)
        return;

    struct htab_item *tmp;
    for(int i = 0; i < t->arr_size; i++)
    {
        t->array[i]->data = 0;
        t->array[i]->key = NULL;
        struct htab_item *tmp = t->array[i]->next;
        free(t->array[i]);
    }
    t->arr_size = 0;
    t->size = 0;

}