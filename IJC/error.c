// error.c
// Riesenie IJC-DU1, příklad a), b), 3.3.2020
// Autor: Peter Rucek, xrucek00, FIT 
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)


#include "error.h"


void warning_msg(const char *fmt, ...)
{
    fprintf(stderr,"CHYBA: ");

    va_list my_list;
    va_start(my_list,fmt);
    vfprintf(stderr,fmt,my_list);
    va_end(my_list);

    fprintf(stderr, "\n");
}
void error_exit(const char *fmt, ...)
{
    fprintf(stderr,"CHYBA: ");

    va_list my_list;
    va_start(my_list,fmt);
    vfprintf(stderr,fmt,my_list);
    va_end(my_list);

    fprintf(stderr, "\n");
    
    exit(1);
}
