// error.h
// Riesenie IJC-DU1, příklad b), 3.3.2020
// Autor: Peter Rucek, xrucek00, FIT 
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


#ifndef _ERROR_H_
#define _ERROR_H_

/**
 * @brief warning_msg
 * Prints out a warning.
 */
void warning_msg(const char *fmt, ...);


/**
 * @brief error_exit
 * Prints out a warning and exits the program.
 */
void error_exit(const char *fmt, ...);


#endif //_ERROR_H_
