//htab_free(t)              destruktor: zrušení tabulky (volá htab_clear())

#include <stdlib.h>
#include "htab.h"
#include "htab_private.h"
#include <stdio.h>

void htab_free(htab_t * t)
{
    htab_clear(t);
    free(t);
}