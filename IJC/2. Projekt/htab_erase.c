// void htab_erase(htab_t *t, htab_iterator_t it);
// ruší zadaný záznam v tabulce (dávejte pozor na zneplatnění it touto
// operací - je to potenciální problém i v C++)

#include "htab.h"
#include "htab_private.h"
#include <stdlib.h>

void htab_erase(htab_t * t, htab_iterator_t it)
{
    free((char *)it.ptr->key);
    free(it.ptr);
}