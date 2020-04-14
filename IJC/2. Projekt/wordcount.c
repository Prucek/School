//  wordcount.c
//  Riesenie IJC-DU2, příklad b) 7.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include <stdio.h>
#include "htab.h"
#include "io.h"

#define MAX 127

//  http://cseweb.ucsd.edu/~kube/cls/100/Lectures/lec16/lec16-8.html
//  The hash table should be an array with length about 1.3 times the maximum number of keys that will actually be in the table,
//  and size of hash table array should be a prime number   
//  https://planetmath.org/goodhashtableprimes
//  (Using primes for hash tables is a good idea because it minimizes clustering in the hashed table).
//  Number should be as far as possible from the nearest two powers of two, 
//  which has, allegedly, been shown to yield especially good results in practice.

//  I don't know the maximum number of keys, but I assume about 20000.
//  So I chose the number 24593, which is about 1.3 times bigger then my assume and
//  which is far as possible form the nearest two powers of two (2^14 and 2^15).
//  It is a compromise between speed and space.

#define ARR_MAX 24593

int main(void)
{
    //initialization hash table
    htab_t *ht = htab_init(ARR_MAX);
    if(ht == NULL)
        return 1;

    //reading words
    char word[MAX] = {0};
    int count = 0;
    bool warning = false;
    while((count = get_word(word,MAX,stdin)) != EOF)
    {
        if(count > MAX && !warning)
        {
            warning = true;
            fprintf(stderr,"Warning: Too long word!\n");
        }
        //add word to hash table
        htab_iterator_t it = htab_lookup_add(ht,word);
        if(!htab_iterator_valid(it))
            return 1;
    }

    //printing of words and and their count
    for(htab_iterator_t it = htab_begin(ht); !htab_iterator_equal(it,htab_end(ht)); it = htab_iterator_next(it))
    {
        if(!htab_iterator_valid(it))
            return 1;
        
        htab_value_t result =  htab_iterator_get_value(it);
        htab_key_t key  = htab_iterator_get_key(it);
        fprintf(stdout,"%s\t%d\n",key,result);
    }

    //destruction of the hash table
    htab_free(ht);
    return 0;
}