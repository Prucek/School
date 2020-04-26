/**
 * @file proj2.h
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

#define EXIT_ERROR -1 //for error return values

/**
 * @brief court_t
 * Current situation in the court house
 */
typedef struct court
{
    int entered;                    //number of immigrants in the court
    int checked;                    //number of checked immigrants in the court
    int waiting_for_registration;   //number of checked immigrants, who are waiting for registration
    int go_out_before_judge_comes;  //number of immigrants, that have to leave before judge enters the court
    bool isJudge;                   //bool, if judge is in the court
    bool index_in_court[];          //indexes of immigrants who are confirmed
} court_t;


/**
 * @brief immigrants_t
 * Informations about immigrants
 */
typedef struct immigrants
{
    int imm_count;          //total number of immigrants to come
    int actual_index;       //index of the immigrant
    int waiting_in_queue;   //number of immigrants wainting in front of the court
    sem_t confirmed[];      //each immigrant has its own semaphor, whether he is confrimed or not

} immigrants_t;


//global variables
court_t *fh; //Faneuil Hall
immigrants_t *imm;

int *line_count; //column index
sem_t *noJudge, *mutex, *allSignedIn; //semaphores from the book
sem_t *data; //locks data
sem_t *allOut; //allows judge to enter if all confirmed immigrants are out

FILE *fd;   //file descriptor


/**
 * @brief random_delay
 * Sleeps for random time in interval <0,base>
 * 
 * @param base upper limit of interval
 */
void random_delay(int base);

/**
 * @brief lock_data_and_printing
 * Locking shared data or printing into file, so only one process handles with it
 */
void lock_data_and_printing();

/**
 * @brief unlock_data_and_printing
 * Unlocking shared data or printing into file
 */
void unlock_data_and_printing();

/**
 * @brief is_uint
 * Checks whether string is unsigned number
 *
 * @param number string
 * 
 * @return true if number is uint, else false
 */
bool is_uint(char number[]);

/**
 * @brief semaphores_constructor
 * Creates all semaphores
 */
void semaphores_constructor();

/**
 * @brief semaphores_destructor
 * Deletes all semaphores
 */
void semaphores_destructor();

/**
 * @brief court_constructor
 * Initializes court values
 * 
 * @param imm_count number of immigrants
 */
void court_constructor(int imm_count);

/**
 * @brief shared_memory_constructor
 * Maps shared memory and initializes global variables
 * 
 * @param imm_count number of immigrants
 * 
 * @return if one or more mmap failed -1 is returned, on success 0
 */
int shared_memory_constructor(int imm_count);

/**
 * @brief shared_memory_destructor
 * Unmaps shared memory
 * 
 * @param imm_count number of immigrants
 */
void shared_memory_destructor(int imm_count);

/**
 * @brief chceck_argument
 * Checks if string is uint and if it is in interval <0,2000>
 * 
 * @param arg string to test
 * 
 * @return on success returns the result of the conversion, else -1
 */
int check_argument(char* arg);

/**
 * @brief chceck_argument
 * Checks if string is uint and if it is in interval <1,Inf)
 * 
 * @param arg string to test
 * 
 * @return on success returns the result of the conversion, else -1
 */
int check_imm_count(char *arg);

#endif //__PROJ2_H__