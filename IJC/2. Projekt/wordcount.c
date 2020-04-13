//  wordcount.c
//  Riesenie IJC-DU2, příklad b) 7.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include <stdio.h>
#include "htab.h"
#include "io.h"

#define MAX 127
#define ARR_MAX 200000 //TODO

int main(void)
{
    //initialization hash table
    htab_t *ht = htab_init(ARR_MAX);

    //reading words
    char word[MAX] = {0};
    int count = 0;
    bool warning = false;
    while((count = get_word(word,MAX,stdin)) != EOF)
    {
        if(count >= MAX && !warning)
        {
            warning = true;
            fprintf(stderr,"Warning: Too long word!\n");
        }
        //add word to hash table
        htab_lookup_add(ht,word);
    }

    //printing of words and and their count
    for(htab_iterator_t it = htab_begin(ht); !htab_iterator_equal(it,htab_end(ht)); it = htab_iterator_next(it))
    {
       htab_value_t result =  htab_iterator_get_value(it);
       htab_key_t key  = htab_iterator_get_key(it);
       fprintf(stdout,"%s\t%d\n",key,result);
    }

    //destruction of the hash table
    htab_free(ht);
    return 0;
}