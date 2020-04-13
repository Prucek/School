//  htab_hash_fun.c
//  Riesenie IJC-DU2, příklad b) 7.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include <stdint.h>
#include "htab.h"

#ifndef HASHTEST

// (http://www.cse.yorku.ca/~oz/hash.html - varianta sdbm)
size_t htab_hash_fun(const char *str) {
    uint32_t h=0;     // musí mít 32 bitů
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h;
}

#else

// (http://www.cse.yorku.ca/~oz/hash.html - varianta djb2)
size_t htab_hash_fun(const char*str)
{
    uint32_t hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

#endif