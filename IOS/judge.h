/**
 * @file judge.c
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
#ifndef __JUDGE_H__
#define __JUDGE_H__

#include "proj2.h"

/**
 * @brief judge_process
 * Cover of the jufge_lifecycle, does the delay and exitting
 * 
 * @param delay max time (in ms) for the judge to re-enter the building
 * 
 * @param certificate_delay max time (in ms) that simulates the duration of the judge decision
 */
void judge_process(int delay, int certificate_delay);

/**
 * @brief judge_lifecycle
 * Judges lifecycle in the court
 * 
 * @param certificate_delay max time (in ms) that simulates the duration of the judge decision
 */
void judge_lifecycle(int certificate_delay);


//small function that simulates the actions of the judge
void wants_to_enter();
void waiting_for_immigrants_to_leave();
void enter();
void waiting_for_imm_to_check();
void starts_confirmation();
void confirmation(int certificate_delay);
void ends_confirmation();
void leave(int certificate_delay);

#endif //__JUDGE_H__