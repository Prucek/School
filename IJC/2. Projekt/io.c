//  io.c
//  Riesenie IJC-DU2, příklad b) 11.4.2020
//  Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)

#include "io.h"

int get_word(char *s, int max, FILE *f)
{
    if(s == NULL || f == NULL || max == 0)
        return EOF;

	int c;
	int count = 0;
	while((c = fgetc(f)) != EOF)
	{
        //if white space was read add \0 and return   
        if(isspace(c))
        {
            //if count is 0, only white space was read => do nothing
            if(count)
            {
                s[count] = '\0';
                return count;
            }
        }
        else if (count == max)
        {
            //if too long word jump to the end and return  
            s[count] = '\0';
            while(c = fgetc(f))
                if (c == EOF || isspace(c))
                    break;
            return max;
        }
        //c is not a white space => add to string 
        else 
            s[count++] = c;	
	}
    //if before EOF is no \n
    if(s[0] != '\0')
        s[count] = '\0';
    //if we didn't finish above return EOF
    return EOF;
}