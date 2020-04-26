/**
 * @file immigrants.h
 * 
 * @author Peter Rucek, xrucek00
 *  
 * @date 26.4.2020
 * 
 * @version 3.1
 * 
 * @brief IOS 2. Project 
 * Faneuil Hall Problem Implementation
 */

#ifndef __IMMIGRANTS_H__
#define __IMMIGRANTS_H__

#include "proj2.h"

/**
 * @brief imm_generator
 * Generates immigrants
 * 
 * @param delay max time (in ms) for for which a new immigrant process is generated
 * 
 * @param certificate_delay max time (in ms) that simulates the duration of a certificate retrieval by an immigrant
 * 
 * @return on success 0, if one of the fork-s failed -1
 */
int imm_generator(int delay, int certificate_delay);

/**
 * @brief immigrant
 * Immigrant lifecycle
 * 
 * @param certificate_delay max time (in ms) that simulates the duration of a certificate retrieval by an immigrant
 */
void immigrant(int certificate_delay);

/**
 * @brief delete_immigrants
 * Waits for all immigrants so imm_generator can exit
 */ 
void delete_immigrants();

//small function that simulates the actions of the immigrant
int starts(); //returns index of immigrant
void imm_enter(int index);
void check(int index);
void wants_certificate(int index);
void got_certificate(int index,int certificate_delay);
void imm_leave(int index);

#endif //__IMMIGRANTS_H__