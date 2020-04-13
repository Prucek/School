//  io.h
//  Riesenie IJC-DU2, příklad b) 11.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#ifndef __IO_H__
#define __IO_H__

#include <stdio.h>
#include <ctype.h>

/**
 * @brief get_word
 * Read only words from file
 * 
 * @param s String where to store the word
 * 
 * @param max Maximal count of letters in string
 * 
 * @param f Stream from where to read
 * 
 * @return Number of successfully read chars or EOF if EOF is read
 */
int get_word(char *s, int max, FILE *f);

#endif //__IO_H__