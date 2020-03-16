// steg-decode.c
// Riesenie IJC-DU1, příklad b), 7.3.2020
// Autor: Peter Rucek, xrucek00, FIT 
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)


#include "ppm.h"
#include <stdio.h>
#include "bit_set.h"
#include "eratosthenes.h"
#include <string.h>


#define BUF 100000 //might be enough


/**
 * @brief print_message
 * Prints the secret message
 * 
 * @param ppm Pointer to the ppm structure
 */
void print_message(struct ppm* ppm)
{
    unsigned long size = ppm->xsize * ppm->ysize * 3;

    bitset_create(arr, LIMIT);
    Eratosthenes(arr);

    unsigned count = 0;
    char c = 0;
    char secret[BUF] = {0,};
    unsigned char_number = 0;
    for(bitset_index_t i = 23; i < size; i++)
    {
        if(!bitset_getbit(arr,i))
        {
            if(count == 8)
            {
                count = 0;
                secret[char_number] = c;
                char_number++;
                if(c == '\0')
                    break;
                c = 0;
            }
            c |= (ppm->data[i] & 1) << count; // same as bitset_set
            count++;
        }
    }

    //checking if \0 is in the end of the message
    if(secret[strlen(secret)] != '\0')
        error_exit("Incorrect message!");

    //finally printing the whole message
    fprintf(stdout,"%s\n",secret);
}


int main(int argc, char *argv[])
{
    if(argc != 2)
        return 1;
    
    struct ppm *my_ppm; 
    my_ppm = ppm_read(argv[1]);
    if(my_ppm == NULL)
        return 1;

    print_message(my_ppm);

    ppm_free(my_ppm);

    return 0;
}
