# Makefile
# Riesenie IJC-DU1, příklad a), 28.2.2020
# Autor: Peter Rucek, xrucek00, FIT
# GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)


CFLAGS=-std=c11 -Wall -Werror -Wextra -pedantic 

all: tail wordcount

tail: tail.c tail.h
	gcc tail.c $(CFALGS) -o tail

wordcount: wordcount-.cc
	g++ wordcount-.cc $(CFALGS) -o wordcount

clean:
	rm tail wordcount


# (10b)

# Použijte implicitní lokalizaci (= nevolat setlocale()). Zamyslete se nad tím,
# jaké problémy by přineslo použití UTF-8 při zapnuté lokalizaci s tímto
# dnes běžně používaným kódováním.

# Napište soubor Makefile tak, aby příkaz make vytvořil programy
# "tail", "wordcount", "wordcount-dynamic" a knihovny "libhtab.a",
# "libhtab.so" (nebo "htab.dll" atd.).

# Program "wordcount" musí být staticky sestaven s knihovnou "libhtab.a".

# Program "wordcount-dynamic" musí být sestaven s knihovnou "libhtab.so".
# Tento program otestujte se stejnými vstupy jako u staticky sestavené verze.

# Porovnejte efektivitu obou (C i C++) implementací (viz např. příkaz time)
# a zamyslete se nad výsledky (pozor na vliv vyrovnávacích paměťí atd.)
# Také si zkuste překlad s optimalizací i bez ní (-O2, -O0) a porovnejte
# efektivitu pro vhodný vstup.

# Poznámky:
#  - 2) pro testy wordcount-dynamic na linuxu budete potřebovat nastavit
#       LD_LIBRARY_PATH="."   (viz "man ld.so" a odpovídající přednáška)
#  - Čtěte pokyny pro vypracování domácích úkolů (viz dále)

# ===================================================================================
#  Obecné pokyny pro vypracování domácích úkolů

# *  Pro úkoly v jazyce C používejte ISO C99 (soubory *.c)
#    Pro úkoly v jazyce C++ používejte ISO C++17 (soubory *.cc)
#    Použití nepřenositelných konstrukcí v programech není dovoleno.

# *  Úkoly zkontrolujte překladačem například takto:
#       gcc -std=c99 -pedantic -Wall -Wextra priklad1.c
#       g++ -std=c++17 -pedantic -Wall priklad.cc
#    Místo gcc můžete použít i jiný překladač - podle vašeho prostředí.
#    V  souvislosti s tím napište do poznámky na začátku
#    souboru jméno a verzi překladače, kterým byl program přeložen
#    (implicitní je GCC `g++ --version` na počítači merlin).

# *  Programy  pište, pokud je to možné, do jednoho zdrojového
#    souboru. Dodržujte předepsaná jména souborů.

# *  Na začátek každého souboru napište poznámku, která bude
#    obsahovat jméno, fakultu, označení příkladu a datum.

# * Úkoly je nutné zabalit programem zip takto:
#        zip xnovak99.zip *.c *.cc *.h Makefile

#   Jméno xnovak99 nahradíte vlastním. Formát souboru bude ZIP.
#   Archiv neobsahuje adresáře.  Každý si zkontroluje obsah ZIP archivu jeho
#   rozbalením v prázdném adresáři a napsáním "make".

# * Posílejte pouze nezbytně nutné soubory -- ne *.EXE !

# * Řešení se odevzdává elektronicky v IS FIT

# * Úkoly neodevzdané v termínu (podle WIS) budou za 0 bodů.

# * Opsané úkoly budou hodnoceny 0 bodů pro všechny zůčastněné
#   a to bez výjimky (+ bonus v podobě návštěvy u disciplinární komise).