//htab.c

#include "htab.h"

struct htab
{
    size_t size;
    size_t arr_size;
    struct htab_item *array[];
};

struct htab_item
{
    char *key;
    size_t data;
    struct htab_item *next;
};



