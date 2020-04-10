//wordcount.c

#include <stdio.h>
#include "htab.h"

int main(void)
{
    htab_t *my = htab_init(200);
    htab_lookup_add(my,"ahoj");
    htab_lookup_add(my,"blah");
    htab_lookup_add(my,"detva");
    htab_lookup_add(my,"ahoj");
    htab_lookup_add(my,"blah");
    htab_lookup_add(my,"ahoj");
    htab_lookup_add(my,"a");
    htab_lookup_add(my,"a");
    htab_lookup_add(my,"ah");
    htab_lookup_add(my,"aho");
    htab_lookup_add(my,"ahoj");

    for(htab_iterator_t it = htab_begin(my); !htab_iterator_equal(it,htab_end(my)); it = htab_iterator_next(it))
    {
       htab_value_t result =  htab_iterator_get_value(it);
       htab_key_t key  = htab_iterator_get_key(it);
       printf("%s na indexe %ld: %d\n",key,it.idx,result);
    }

    htab_free(my);
    return 0;
}