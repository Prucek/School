//iterator=htab_lookup_add(t,key)   vyhledávání+přidání - viz dále
// htab_iterator_t htab_lookup_add(htab_t *t, htab_key_t key);
// V tabulce  t  vyhledá záznam odpovídající řetězci  key  a
// - pokud jej nalezne, vrátí iterátor na záznam
// - pokud nenalezne, automaticky přidá záznam a vrátí iterátor
// Poznámka: Dobře promyslete chování této funkce k parametru key.
// Poznámka: podobně se chová C++ operátor [] pro std::unordered_map

// - Když htab_init nebo htab_lookup_add nemohou alokovat paměť,
//     vrací NULL nebo nevalidní iterátor htab_end
//     (a uživatel musí testovat výsledek těchto operací)

#include "htab.h"

htab_iterator_t htab_lookup_add(htab_t * t, htab_key_t key)
{

}