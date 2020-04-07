// 2) (max 10b)
//    Přepište následující C++ program do jazyka ISO C

    // wordcount-.cc
    // Použijte: g++ -std=c++11 -O2
    // Příklad použití STL kontejneru unordered_map<>
    // Program počítá četnost slov ve vstupním textu,
    // slovo je cokoli oddělené "bílým znakem"

    #include <string>
    #include <iostream>
    #include <unordered_map>

    int main() {
        using namespace std;
        unordered_map<string,int> m;  // asociativní pole
                     // mapuje klíč/string na hodnotu/int
        string word;
        while (cin >> word) // čtení slova (jako scanf "%s", ale bezpečné)
            m[word]++;      // počítání výskytů slova (zvýší hodnotu pro
                            // zadaný klíč/slovo pokud záznam existuje,
                            // jinak vytvoří nový záznam s hodnotou 0 a
                            // tu operace ++ zvýší na 1)

        for (auto &mi: m)   // pro všechny prvky kontejneru m
            cout << mi.first << "\t" << mi.second << "\n";
            //      klíč/slovo          hodnota/počet
            // prvky kontejneru typu "map" jsou dvojice (klíč,hodnota)
    }

//    Výstupy programů musí být pro stejný vstup stejné (kromě pořadí a příliš
//    dlouhých slov).

//    Výsledný program se musí jmenovat "wordcount.c".

//    Implementujte tabulku s rozptýlenými položkami (hash table) - viz dále.
//    Veškeré operace s tabulkou budou v samostatné knihovně (vytvořte statickou
//    i dynamickou/sdílenou verzi).  V knihovně musí být prakticky každá funkce ve
//    zvláštním modulu -- to například umožní případnou výměnu htab_hash_fun() ve vašem
//    staticky sestaveném programu. (V dynamicky sestaveném programu je to možné vždy.)
//    Vyzkoušejte si to: definujte svoji verzi htab_hash_fun() v programu
//    s podmíněným překladem -- použijte #ifdef HASHTEST.

//    Knihovna s tabulkou se musí jmenovat
//    "libhtab.a" (na Windows je možné i "htab.lib") pro statickou variantu,
//    "libhtab.so" (na Windows je možné i "htab.dll") pro sdílenou variantu
//    a rozhraní "htab.h".

//    Podmínky:
//     - Implementace musí být dynamická (malloc/free) a musíte zvládnout
//       správu paměti v C (použijte valgrind, nebo jiný podobný nástroj).
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


