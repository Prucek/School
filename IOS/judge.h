/**
 * @file judge.c
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
#ifndef __JUDGE_H__
#define __JUDGE_H__

#include "proj2.h"

void judge_process(int delay, int certificate_delay);
void judge_lifecycle(int certificate_delay);

void wants_to_enter();
void waiting_for_immigrants_to_leave();
void enter();
void waiting_for_imm_to_check();
void starts_confirmation();
void confirmation(int certificate_delay);
void ends_confirmation();
void leave(int certificate_delay);

#endif //__JUDGE_H__