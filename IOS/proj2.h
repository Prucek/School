/**
 * @file proj2.h
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

#ifndef __PROJ2_H__
#define __PROJ2_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#include <errno.h>
#include <string.h>


typedef struct court
{
    int entered;
    int checked;
    int waiting_in_queue;
    int waiting_for_registration;
    int go_out_before_judge_comes;
    int imm_count;
    bool isJudge;
    int index_in_court[];
} court_t;

typedef struct immigrants
{
    int actual_index;
    sem_t confirmed[];

} immigrants_t;


court_t *fh;
immigrants_t *imm;


int *line_count;
sem_t *noJudge, *mutex, *allSignedIn, *printing, *allOut;
FILE *fd;

void random_delay(int base);
void lock_data_and_printing();
void unlock_data_and_printing();
bool isUInt(char number[]);
void semaphores_constructor();
void semaphores_destructor();
void court_constructor(int imm_count);
void shared_memory_constructor(int imm_count);
void shared_memory_destructor(int imm_count);

#endif //__PROJ2_H__