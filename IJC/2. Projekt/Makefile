# Makefile
# Riesenie IJC-DU2, příklad a),b) 13.4.2020
# Autor: Peter Rucek, xrucek00, FIT
# GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

CPPFLAGS=-std=c++17 -pedantic -Wall
CFLAGS=-std=c99 -O2 -Wall -Werror -Wextra -pedantic -g

LIB_DEP = htab_init.o htab_clear.o htab_free.o htab_erase.o
LIB_DEP+= htab_begin.o htab_end.o htab_iterator_next.o 
LIB_DEP+= htab_bucket_count.o htab_size.o 
LIB_DEP+= htab_iterator_get_key.o htab_iterator_get_value.o htab_iterator_set_value.o
LIB_DEP+= htab_hash_fun.o htab_lookup_add.o htab.o htab_find.o

LIB_DEP_DYN = htab-dynamic.o htab_init-dynamic.o htab_clear-dynamic.o htab_free-dynamic.o htab_erase-dynamic.o
LIB_DEP_DYN+= htab_begin-dynamic.o htab_end-dynamic.o htab_iterator_next-dynamic.o
LIB_DEP_DYN+= htab_size-dynamic.o htab_bucket_count-dynamic.o
LIB_DEP_DYN+= htab_iterator_get_key-dynamic.o htab_iterator_get_value-dynamic.o htab_iterator_set_value-dynamic.o
LIB_DEP_DYN+= htab_hash_fun-dynamic.o htab_lookup_add-dynamic.o htab_find-dynamic.o

all: tail libhtab.a libhtab.so wordcount wordcount-dynamic #wordcountcpp

# $@ Cíl
# $< První závislost
# $^ Všechny závislosti bez duplicit
#===============================================================

tail: tail.c tail.h
	gcc $(CFALGS) $< -o $@

#===============================================================

# dynamic

htab-dynamic.o: htab.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@

htab_init-dynamic.o: htab_init.c htab_private.h 
	gcc $(CFALGS)-fPIC -c $< -o $@

htab_clear-dynamic.o: htab_clear.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@

htab_free-dynamic.o: htab_free.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@

htab_erase-dynamic.o: htab_erase.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@

htab_begin-dynamic.o: htab_begin.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@ 

htab_end-dynamic.o: htab_end.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@ 

htab_iterator_next-dynamic.o: htab_iterator_next.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@

htab_size-dynamic.o: htab_size.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@

htab_bucket_count-dynamic.o: htab_bucket_count.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@	

htab_iterator_get_key-dynamic.o: htab_iterator_get_key.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@	

htab_iterator_get_value-dynamic.o: htab_iterator_get_value.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@	

htab_iterator_set_value-dynamic.o: htab_iterator_set_value.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@	

htab_hash_fun-dynamic.o: htab_hash_fun.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@	

htab_lookup_add-dynamic.o: htab_lookup_add.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@	

htab_find-dynamic.o: htab_find.c htab_private.h
	gcc $(CFALGS)-fPIC -c $< -o $@	

#===============================================================

# static

htab.o: htab.c htab_private.h
	gcc $(CFALGS) -c $<

htab_init.o: htab_init.c htab_private.h 
	gcc $(CFALGS) -c $<

htab_clear.o: htab_clear.c htab_private.h
	gcc $(CFALGS) -c $<

htab_free.o: htab_free.c htab_private.h
	gcc $(CFALGS) -c $< 

htab_erase.o: htab_erase.c htab_private.h
	gcc $(CFALGS) -c $< 

htab_begin.o: htab_begin.c htab_private.h
	gcc $(CFALGS) -c $<  

htab_end.o: htab_end.c htab_private.h
	gcc $(CFALGS) -c $<  

htab_iterator_next.o: htab_iterator_next.c htab_private.h
	gcc $(CFALGS) -c $< 

htab_size.o: htab_size.c htab_private.h
	gcc $(CFALGS) -c $< 

htab_bucket_count.o: htab_bucket_count.c htab_private.h
	gcc $(CFALGS) -c $<	

htab_iterator_get_key.o: htab_iterator_get_key.c htab_private.h
	gcc $(CFALGS) -c $<	

htab_iterator_get_value.o: htab_iterator_get_value.c htab_private.h
	gcc $(CFALGS) -c $<	

htab_iterator_set_value.o: htab_iterator_set_value.c htab_private.h
	gcc $(CFALGS) -c $<	

htab_hash_fun.o: htab_hash_fun.c htab_private.h
	gcc $(CFALGS) -c $<	

htab_lookup_add.o: htab_lookup_add.c htab_private.h
	gcc $(CFALGS) -c $<	

htab_find.o: htab_find.c htab_private.h
	gcc $(CFALGS) -c $<	

#===============================================================

# several wordcount programs

wordcountcpp: wordcount-.cc
	g++ $(CPPFALGS) $^ -o $@

wordcount: htab.h wordcount.c libhtab.a io.c
	ranlib libhtab.a
	gcc $(CFALGS) -static $^ -L. -lhtab -o $@

wordcount-dynamic: htab.h wordcount.c libhtab.so io.c
	gcc $(CFALGS) $^ -L. -lhtab -o $@

#===============================================================

# libraries

libhtab.a: $(LIB_DEP)
	ar rcs $@ $^

libhtab.so: $(LIB_DEP_DYN)
	gcc $(CFLAGS) -shared -fPIC $^ -o $@ 

#===============================================================

.PHONY: clean pack

clean:
	rm -f tail wordcount wordcount-dynamic wordcountcpp
	rm -f *.o
	rm -f libhtab.a libhtab.so

pack:
	zip xrucek00.zip *.c *.cc *.h Makefile