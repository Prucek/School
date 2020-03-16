// ppm.h
// Riesenie IJC-DU1, příklad b), 7.3.2020
// Autor: Peter Rucek, xrucek00, FIT 
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)


#ifndef _PPM_H_
#define _PPM_H_


#include <stdio.h>
#include "error.h"
#include <string.h>


#define BUF_SIZE 100
#define LIMIT 8000*8000*3 //Limit of the size of the .ppm file


/**
 * @brief ppm struct
 * Structure of the .ppm file
 */
struct ppm {
        unsigned xsize;
        unsigned ysize;
        char data[];    // RGB bajty, celkem 3*xsize*ysize
     };


/**
 * @brief ppm_read
 * Reads the .ppm file into the ppm structure
 * 
 * @param filename Name of the .ppm file
 * 
 * @return On success returns pointer to the ppm structure, else return NULL
 */
struct ppm * ppm_read(const char * filename);


/**
 * @brief ppm_free
 * Frees the memory of the allocated ppm structure.
 */
void ppm_free(struct ppm *p);


#endif

