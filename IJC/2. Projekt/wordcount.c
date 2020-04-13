//wordcount.c

#include <stdio.h>
#include "htab.h"
#include "io.h"

#define MAX 127
#define ARR_MAX 20000

int main(void)
{
    char word[MAX] = {0};
    htab_t *ht = htab_init(ARR_MAX);

    int count = 0;
    bool warning = false;
    while((count = get_word(word,MAX,stdin)) != EOF)
    {
        if(count >= MAX && !warning)
        {
            warning = true;
            fprintf(stderr,"Warning: Too long word!\n");
        }
            
        htab_lookup_add(ht,word);
    }

    for(htab_iterator_t it = htab_begin(ht); !htab_iterator_equal(it,htab_end(ht)); it = htab_iterator_next(it))
    {
       htab_value_t result =  htab_iterator_get_value(it);
       htab_key_t key  = htab_iterator_get_key(it);
       printf("%s   %d\n",key,result);
    }

    htab_free(ht);
    return 0;
}