//wordcount.c

#include <stdio.h>
#include "htab.h"

int main(void)
{
    htab_t *my = htab_init(5);
    htab_free(my);
    return 0;
}