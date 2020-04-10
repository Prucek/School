// htab.h -- rozhraní knihovny htab (řešení IJC-DU2)
// Licence: žádná (Public domain)

// následující řádky zabrání násobnému vložení:
#ifndef __HTAB_H__
#define __HTAB_H__


#include <string.h>     // size_t
#include <stdbool.h>    // bool

// Tabulka:
struct htab;    // neúplná deklarace struktury - uživatel nevidí obsah
typedef struct htab htab_t;     // typedef podle zadání

// Typy:
typedef const char * htab_key_t;        // typ klíče
typedef int htab_value_t;               // typ hodnoty

// Iterátor do tabulky:
struct htab_item;               // neúplná deklarace struktury

// iterátor:
typedef struct htab_iterator {
    struct htab_item *ptr;      // ukazatel na položku
    const htab_t *t;            // ve které tabulce
    size_t idx;                 // ve kterém seznamu tabulky (TODO: optimalizovat?)
} htab_iterator_t;              // typedef podle zadání

// rozptylovací (hash) funkce (stejná pro všechny tabulky v programu)
// pokud si v programu definujete stejnou funkci, použije se ta vaše
size_t htab_hash_fun(htab_key_t str);

// funkce pro práci s tabulkou:
htab_t *htab_init(size_t n);                    // konstruktor tabulky
size_t htab_size(const htab_t * t);             // počet záznamů v tabulce
size_t htab_bucket_count(const htab_t * t);     // velikost pole

htab_iterator_t htab_find(htab_t * t, htab_key_t key);  // hledání
htab_iterator_t htab_lookup_add(htab_t * t, htab_key_t key);

void htab_erase(htab_t * t, htab_iterator_t it);  // ruší zadaný záznam

htab_iterator_t htab_begin(const htab_t * t);   // iterátor na první záznam
htab_iterator_t htab_end(const htab_t * t);     // iterátor _za_ poslední záznam

htab_iterator_t htab_iterator_next(htab_iterator_t it); // iterátor++



// - Základní funkce pro testování iterátorů:
//     b = htab_iterator_valid(iterator)
//       test, zda iterátor označuje platný prvek v tabulce
//     b = htab_iterator_equal(iterator1, iterator2)
//       porovnání dvou iterátorů na rovnost

// test: iterátor != end()
inline bool htab_iterator_valid(htab_iterator_t it) { return it.ptr!=NULL; }
// test: iterátor1 == iterátor2
inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2) {
  return it1.ptr==it2.ptr && it1.t == it2.t;
}

// čtení a zápis přes iterátor
htab_key_t htab_iterator_get_key(htab_iterator_t it);
htab_value_t htab_iterator_get_value(htab_iterator_t it);
htab_value_t htab_iterator_set_value(htab_iterator_t it, htab_value_t val);

void htab_clear(htab_t * t);    // ruší všechny záznamy
void htab_free(htab_t * t);     // destruktor tabulky

#endif // __HTAB_H__
//===============================================================================


    // - Tabulka je (pro knihovnu privátní) struktura obsahující pole seznamů,
    //   jeho velikost a počet položek tabulky v následujícím pořadí:

    //      +----------+
    //      | size     | // aktuální počet záznamů [key,data,next]
    //      +----------+
    //      | arr_size | // velikost následujícího pole ukazatelů
    //      +----------+
    //      +---+
    //      |ptr|-->[key,data,next]-->[key,data,next]-->[key,data,next]--|
    //      +---+
    //      |ptr|--|
    //      +---+
    //      |ptr|-->[key,data,next]-->[key,data,next]--|
    //      +---+
    //      |ptr|--|
    //      +---+

    //   Položka .arr_size je velikost následujícího pole ukazatelů (použijte
    //   C99: "flexible array member"). Paměť pro strukturu se dynamicky alokuje
    //   tak velká, aby se do ní vešly i všechny položky pole.
    //   V programu zvolte vhodnou velikost pole a v komentáři zdůvodněte vaše
    //   rozhodnutí.
    //   (V obrázku platí velikost .arr_size==4 a počet položek .size==5.)
    //   Rozhraní knihovny obsahuje jen _neúplnou_deklaraci_ struktury, definice
    //   je uživateli knihovny skryta (jde o formu zapouzdření - "encapsulation").

//===================================================================================
    // - Vhodně zvolte typy parametrů funkcí (včetně použití const).

    // - Záznam [key,data,next] je typu
    //       struct htab_item
    //   a obsahuje položky:
    //       key .... ukazatel na dynamicky alokovaný řetězec,
    //       data ... počet výskytů a
    //       next ... ukazatel na další záznam
    //   Tento záznam je definován v privátním hlavičkovém souboru pro všechny
    //   moduly tabulky a není dostupný při použití knihovny ("Opaque data type").
    //   Každý iterátor obsahuje ukazatel na tuto strukturu.


//===================================================================================

// (10b)

// Použijte implicitní lokalizaci (= nevolat setlocale()). Zamyslete se nad tím,
// jaké problémy by přineslo použití UTF-8 při zapnuté lokalizaci s tímto
// dnes běžně používaným kódováním.

// Napište soubor Makefile tak, aby příkaz make vytvořil programy
// "tail", "wordcount", "wordcount-dynamic" a knihovny "libhtab.a",
// "libhtab.so" (nebo "htab.dll" atd.).

// Program "wordcount" musí být staticky sestaven s knihovnou "libhtab.a".

// Program "wordcount-dynamic" musí být sestaven s knihovnou "libhtab.so".
// Tento program otestujte se stejnými vstupy jako u staticky sestavené verze.

// Porovnejte efektivitu obou (C i C++) implementací (viz např. příkaz time)
// a zamyslete se nad výsledky (pozor na vliv vyrovnávacích paměťí atd.)
// Také si zkuste překlad s optimalizací i bez ní (-O2, -O0) a porovnejte
// efektivitu pro vhodný vstup.

// Poznámky:
//  - 2) pro testy wordcount-dynamic na linuxu budete potřebovat nastavit
//       LD_LIBRARY_PATH="."   (viz "man ld.so" a odpovídající přednáška)
//  - Čtěte pokyny pro vypracování domácích úkolů (viz dále)

// ===================================================================================
//  Obecné pokyny pro vypracování domácích úkolů

// *  Pro úkoly v jazyce C používejte ISO C99 (soubory *.c)
//    Pro úkoly v jazyce C++ používejte ISO C++17 (soubory *.cc)
//    Použití nepřenositelných konstrukcí v programech není dovoleno.

// *  Úkoly zkontrolujte překladačem například takto:
//       gcc -std=c99 -pedantic -Wall -Wextra priklad1.c
//       g++ -std=c++17 -pedantic -Wall priklad.cc
//    Místo gcc můžete použít i jiný překladač - podle vašeho prostředí.
//    V  souvislosti s tím napište do poznámky na začátku
//    souboru jméno a verzi překladače, kterým byl program přeložen
//    (implicitní je GCC `g++ --version` na počítači merlin).

// *  Programy  pište, pokud je to možné, do jednoho zdrojového
//    souboru. Dodržujte předepsaná jména souborů.

// *  Na začátek každého souboru napište poznámku, která bude
//    obsahovat jméno, fakultu, označení příkladu a datum.

// * Úkoly je nutné zabalit programem zip takto:
//        zip xnovak99.zip *.c *.cc *.h Makefile

//   Jméno xnovak99 nahradíte vlastním. Formát souboru bude ZIP.
//   Archiv neobsahuje adresáře.  Každý si zkontroluje obsah ZIP archivu jeho
//   rozbalením v prázdném adresáři a napsáním "make".

// * Posílejte pouze nezbytně nutné soubory -- ne *.EXE !

// * Řešení se odevzdává elektronicky v IS FIT

// * Úkoly neodevzdané v termínu (podle WIS) budou za 0 bodů.

// * Opsané úkoly budou hodnoceny 0 bodů pro všechny zůčastněné
//   a to bez výjimky (+ bonus v podobě návštěvy u disciplinární komise).