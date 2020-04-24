/**
 * @file immigrants.h
 * 
 * @author Peter Rucek, xrucek00
 *  
 * @date 24.4.2020
 * 
 * @version 3.0
 * 
 * @brief IOS 2. Project 
 * Faneuil Hall Problem Implementation
 */

#ifndef __IMMIGRANTS_H__
#define __IMMIGRANTS_H__

#include "proj2.h"

void immigrant(int certificate_delay);
void delete_immigrants();
int imm_generator(int delay, int certificate_delay);

int starts();
void imm_enter(int index);
void check(int index);
void wants_certificate(int index);
void got_certificate(int index,int certificate_delay);
void imm_leave(int index);

#endif //__IMMIGRANTS_H__