/**
 * @file print.cpp
 * @author Peter Rucek, xrucek00
 * @date 16.4.2021
 * Header for packet sniffer print library
 */

#ifndef __PRINT_HPP__
#define __PRINT_HPP__

#include <iostream>
#include <cstring>
#include <sys/time.h>

using std::cout;
using std::cerr;
using std::endl;

/**
 * Prints date and time in format: rfc3339
 */
void print_time_rfc3339();

/**
 * Prints sequence 0xabcd, where abcd is hex format of number
 * After each call abcd is increased by 16
 * @param next resets abcd to 0000
 */
void print_byte_offset(bool next);

/**
 * Prints data in format: byte_offset    data_in_hex    data_in_ascii
 * @param data are bytes to be printed
 * @param size is size of data
 */
void print_data(const unsigned char *data, int size);

#endif