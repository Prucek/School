/**
 * @file print.cpp
 * @author Peter Rucek, xrucek00
 * @date 16.4.2021
 * Packet sniffer print library
 */

#include "print.hpp"

void print_time_rfc3339()
{
    time_t now = time(NULL);
    struct tm *tm;

    // just for milisec
    struct timeval tv;
    gettimeofday(&tv, NULL); 

    int off_sign;
    int off;

    if ((tm = localtime(&now)) == NULL) 
    {
        exit(1);
    }
    off_sign = '+';
    off = (int) tm->tm_gmtoff;
    if (tm->tm_gmtoff < 0) 
    {
        off_sign = '-';
        off = -off;
    }
    printf("%d-%d-%dT%02d:%02d:%2.3f%c%02d:%02d ",
           tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
           tm->tm_hour, tm->tm_min, tm->tm_sec + tv.tv_usec / 1000000.0,
           off_sign, off / 3600, off % 3600);
}

void print_byte_offset(bool next)
{
    static int i = 0;
    if (next)
        i = 0;
    printf("0x%04x", i);
    i += 16;
}

void print_data(const unsigned char *data, int size)
{
    for(int i = 0; i < size; i++)
    {
        if(i != 0 && i % 16 == 0) // if hex part is done
        {
            cout << "         ";
            for(int j = i - 16; j < i; j++)
            {
                if (isprint(data[j]) != 0)
                {
                    cout << (unsigned char)data[j];
                }
                else
                {
                    cout << "."; // else dot
                }
            }
            cout << endl;
        }
        
        if(i % 16 == 0)
        {
            print_byte_offset((i == 0)? true : false);
            cout << "   ";
        }

        printf(" %02x",(unsigned int)data[i]); // hex part

        if(i == size - 1) // print the last line
        {
            for(int j = 0; j < 15 - i % 16; j++) // how many extra spaces
            {
                cout << "   ";
            }

            cout << "         ";

            for(int j = i - i % 16; j <= i; j++)
            {
                if (isprint(data[j]) != 0)
                {
                    cout << (unsigned char)data[j];
                }
                else
                {
                    cout << "."; // else dot
                }
            }
            cout << endl;
        }
    }
}
