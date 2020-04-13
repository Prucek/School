//  htab.c
//  Riesenie IJC-DU2, příklad b) 11.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)
//  Extern inline functions

#include "htab.h"

extern inline bool htab_iterator_valid(htab_iterator_t it);

extern inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2);