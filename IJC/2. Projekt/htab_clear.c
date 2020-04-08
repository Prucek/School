//htab_clear(t)             zrušení všech položek, tabulka zůstane prázdná

#include <stdio.h>
#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"

void htab_clear(htab_t * t)
{
    if(t == NULL)
        return;
    
    struct htab_item *tmp = t->array[0];
    if (tmp == NULL)
        return;
    for(; tmp == NULL; tmp->next)
    {
        tmp->data = 0;
        tmp->key = NULL;
        free(tmp);
    }
    printf("A\n");
    t->arr_size = 0;
    t->size = 0;

}