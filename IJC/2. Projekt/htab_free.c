//htab_free(t)              destruktor: zrušení tabulky (volá htab_clear())

#include "htab.h"

void htab_free(htab_t * t)
{
    htab_clear(t);
    free(t);
}