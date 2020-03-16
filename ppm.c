// ppm.c
// Riesenie IJC-DU1, příklad b), 7.3.2020
// Autor: Peter Rucek, xrucek00, FIT 
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)


#include "ppm.h"


struct ppm * ppm_read(const char * filename)
{
    FILE *map;
    char buf[BUF_SIZE], *ret_value;
    unsigned width,height,max_color_value;
    int ret_v_int;

    //opening of the file
    map = fopen(filename,"rb");

    if(!map)
    {
        warning_msg("Cannot open file!");
        goto failure;
    }

    //read first line
    ret_value = fgets(buf,BUF_SIZE, map);

    if ((ret_value == NULL) || ( strncmp(buf, "P6", 2) != 0 ))
    {
     
       warning_msg("File is not .ppm!");
       goto closing;
    }

    //read width and height
    ret_v_int = fscanf(map, "%u %u", &width, &height);

    if ( ret_v_int < 2 )
    {
        warning_msg("File is not .ppm!");
        goto closing;
    }
        
    //read maximum color value
    ret_v_int = fscanf(map, "%u", &max_color_value);

    if ( (ret_v_int < 1) || ( max_color_value != 255 )   )
    {
        warning_msg("File is not correct!");
        goto closing;
    }
         
    fseek(map, 1, SEEK_CUR); // skip one whitespace
  
    unsigned data_size = 3*width*height;

    if(data_size > LIMIT)
    {
        warning_msg("File is too big!");
        goto closing;
    }

    //memory allocation
    struct ppm *my_ppm = malloc(sizeof(struct ppm) + data_size * sizeof(char));
    if(!my_ppm)
    {
        warning_msg("Allocation failed!");
        goto closing;
    }

    my_ppm->xsize = width;
    my_ppm->ysize = height;

    //read binary data
    if(fread(my_ppm->data,sizeof(char),data_size,map) != data_size || ferror(map))
    {
        warning_msg("File is not correct!");
        free(my_ppm);
        goto closing;
    }

    fclose(map);
    return my_ppm;

    //error labels
    closing:
        if (fclose(map) == EOF)
            warning_msg("Closing of file unsuccsesful!");
    failure:
        return NULL;
}


void ppm_free(struct ppm *p)
{
    free(p);
}