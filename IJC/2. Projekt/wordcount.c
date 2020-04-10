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


    // htab_iterator_t it = htab_begin(my);
    // printf("%d\n",it.ptr);
    // printf("%d\n",it.idx);
    // printf("%d\n",it.t);
    // htab_value_t result1 =  htab_iterator_get_value(it);
    //    htab_key_t key1  = htab_iterator_get_key(it);
    //    printf("%s : %d\n",key1,result1);

    // it = htab_iterator_next(it);
    // printf("%d\n",it.ptr);
    // printf("%d\n",it.idx);
    // printf("%d\n",it.t);
    // htab_value_t result =  htab_iterator_get_value(it);
    //    htab_key_t key  = htab_iterator_get_key(it);
    //    printf("%s : %d\n",key,result);

    for(htab_iterator_t it = htab_begin(my); !htab_iterator_equal(it,htab_end(my)); it = htab_iterator_next(it))
    {
       htab_value_t result =  htab_iterator_get_value(it);
       htab_key_t key  = htab_iterator_get_key(it);
       printf("%s na indexe %ld: %d\n",key,it.idx,result);
    }

    htab_free(my);
    return 0;
}