//size_t htab_hash_fun(htab_key_t str);
//     - Vhodná rozptylovací funkce pro řetězce je podle literatury
//       (http://www.cse.yorku.ca/~oz/hash.html - varianta sdbm):

#include <stdint.h>
#include "htab.h"

    size_t htab_hash_fun(const char *str) {
        uint32_t h=0;     // musí mít 32 bitů
        const unsigned char *p;
        for(p=(const unsigned char*)str; *p!='\0'; p++)
            h = 65599*h + *p;
        return h;
    }

    //   její výsledek modulo arr_size určuje index do tabulky:
    //     index = (htab_hash_fun("mystring") % arr_size);
    //   Zkuste použít i jiné podobné funkce a porovnejte efektivitu.