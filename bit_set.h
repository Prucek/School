// bitset.h
// Riesenie IJC-DU1, příklad a), 28.2.2020
// Autor: Peter Rucek, xrucek00, FIT
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)
// Implementation of a bit array via macros and inline functions


#include <limits.h>
#include <stdlib.h>
#include "error.h"
#include <assert.h>


#ifndef _BIT_SET_H_
#define _BIT_SET_H_


/**
 * @brief *bitset_t
 * Data type of a bitset 
 */
typedef unsigned long *bitset_t;


/**
 * @brief bitset_index_t
 * Data type of a index to the bitset 
 */
typedef unsigned long bitset_index_t;


#define MAX 10000000000 //works for me....129s :)

#define UL_BIT_SIZE (sizeof(bitset_index_t)*CHAR_BIT)

#define size(velikost) (velikost/ UL_BIT_SIZE\
        + 1 + ((velikost% UL_BIT_SIZE)? 1 : 0 ))


/**
 * @brief bitset_create
 * Creates a bitset and sets all bits to zero.
 * 
 * @param jmeno_pole Name of the bitset.
 * 
 * @param velikost Size of the bitset.
 */
#define bitset_create(jmeno_pole,velikost)\
        static_assert(velikost > 1, "Size must be bigger than 1!");\
        bitset_index_t jmeno_pole[size(velikost)]\
        = {velikost,0,}//0th index is size, evrything else is zero


/**
 * @brief biset_alloc
 * Same function as bitset_create but instead alocates the bitset on heap.
 * 
 * @param jmeno_pole Name of the bitset.
 * 
 * @param velikost Size of the bitset.
 */ 
#define bitset_alloc(jmeno_pole,velikost)\
        static_assert(velikost < MAX,"bitset_alloc: Chyba alokace pameti");\
        bitset_t jmeno_pole = calloc(velikost/UL_BIT_SIZE+ 1 +((velikost% UL_BIT_SIZE)? 1 : 0) ,sizeof(bitset_index_t));\
        jmeno_pole[0] = velikost;


/**
 * @brief bitset_free
 * Deletes the bitset frome heap.
 * @param jmeno_pole Name of the bitset.
 */ 
#ifdef USE_INLINE
    inline void bitset_free(bitset_t jmeno_pole)
    {
        free(jmeno_pole);
    }
#else
    #define bitset_free(jmeno_pole) free(jmeno_pole)
#endif


/**
 * @brief bitset_size 
 * Returns size of the bitset.
 * @param jmeno_pole Name of the bitset.
 */
#ifdef USE_INLINE
    inline bitset_index_t bitset_size(bitset_t jmeno_pole)
    {
        return jmeno_pole[0];
    }
#else
    #define bitset_size(jmeno_pole) jmeno_pole[0]
#endif


/**
 * @brief biset_setbit
 * Sets the @param index of @param jmeno_pole to @param vyraz.
 * 
 * @param index Index of the bitset.
 * 
 * @param jmeno_pole Name of the bitset.
 * 
 * @param vyraz Expresion to define if the bit should be 1 or 0.
 */ 
#ifdef USE_INLINE
    inline void bitset_setbit(bitset_t jmeno_pole, bitset_index_t index, bitset_index_t vyraz)
    {
        if (index > bitset_size(jmeno_pole))
            error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(jmeno_pole));
        if (vyraz)
            (jmeno_pole[1 + index/ UL_BIT_SIZE] |=  (1UL << (index % UL_BIT_SIZE))); /*add 1*/
        else
            (jmeno_pole[1 + index/ UL_BIT_SIZE] &= ~(1UL << (index % UL_BIT_SIZE))); /*add 0*/
    }
#else
    #define bitset_setbit(jmeno_pole,index,vyraz)\
            do{\
				if(index >= bitset_size(jmeno_pole))\
                    error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu", (unsigned long)index, bitset_size(jmeno_pole));\
                else\
                {\
                    ((vyraz)?\
                      (jmeno_pole[1 + index/ UL_BIT_SIZE] |=  (1UL << (index % UL_BIT_SIZE))) : /*add 1*/\
                      (jmeno_pole[1 + index/ UL_BIT_SIZE] &= ~(1UL << (index % UL_BIT_SIZE))));/*add 0*/\
                }\
            }while(0)
#endif  


/**
 * @brief bitset_getbit
 * Gets the value of the @param index in @param jmeno_pole
 * 
 * @param index Index of the bitset.
 * 
 * @param jmeno_pole Name of the bitset.
 */ 
#ifdef USE_INLINE
    inline int bitset_getbit(bitset_t jmeno_pole, bitset_index_t index)
    {
        if (index > bitset_size(jmeno_pole))
            return error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu", index, bitset_size(jmeno_pole)) , -1;
        if(jmeno_pole[1 + (index/ UL_BIT_SIZE)] & (1UL<< (index % UL_BIT_SIZE)))
            return 1;
        else
            return 0;
    }       
#else
    #define bitset_getbit(jmeno_pole,index)\
            (index > bitset_size(jmeno_pole)? error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",\
            (unsigned long)index, bitset_size(jmeno_pole)), -1 \
            : ((jmeno_pole[1 + (index/ UL_BIT_SIZE)] & (1UL<< (index % UL_BIT_SIZE)))? 1 : 0))
            
#endif

#endif//_BIT_SET_H_
