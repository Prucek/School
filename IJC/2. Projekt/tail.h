// tail.h
// Riesenie IJC-DU1, příklad a), 28.2.2020
// Autor: Peter Rucek, xrucek00, FIT
// GCC 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04.1)

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef _TAIL_H_
#define _TAIL_H_

#define MAX 1023 //maximal size of line
#define EXIT_PLUS -1
#define DEFAULT 10 //on default print last 10 lines of file

/**
 * @brief ListNode
 * Structure of the node of the circular buffer
 */
struct ListNode
{
    struct ListNode *next;
    char data[MAX];
};

/**
 * @brief CircularBuffer
 * Structure of circular buffer
 */
struct CircularBuffer
{
    int size;
    struct ListNode *end;
};

/**
 * @brief listConstructor
 * Construcor
 * 
 * @param numOfNodes Number of nodes in the circular buffer
 * 
 * @return On success returns pointer to the CircularBufer, else NULL
 */
struct CircularBuffer *listConstructor(int numOfNodes);

/**
 * @brief listDestructor
 * Destructor
 * 
 * @param list Pointer to the CircularBuffer
 */
void listDestructor(struct CircularBuffer *list);

/**
 * @brief addNode
 * Adds @param line into the proper place in Circularbuffer
 * 
 * @param list Pointer to a CircularBuffer
 * 
 * @param line String to add into a CircularBuffer
 */
void addNode(struct CircularBuffer *list, char *line);

/**
 * @brief printList
 * Prints the whole CircularBuffer
 *
 * @param list Pointer to a CircularBuffer
 */
void printList(struct CircularBuffer *list);

/**
 * @brief tailWithPlus
 * Printing every line from @param file, starting from @param num
 * 
 * @param file Pointer to a file
 * 
 * @param num Starting line
 */
void tailWithPlus(FILE *file,int num);

/**
 * @brief tail
 * Printing last @param num lines from @param file
 * 
 * @param file Pointer to a file
 * 
 * @param num Number of lines
 */
void tail(FILE* file, int num);

/**
 * @brief checkParameters
 * Checks if -n parameter is correct
 * 
 * @param flag Should be "-n"
 * 
 * @param number Should be a number, can start with +
 * 
 * @return If parameters are correct and @param number is starting with '+', -1 is returned,
 *         if parameters are correct and @param number is only number, 0 is returned,
 *         if parameters are incorrect -1 is returned
 */
int checkParameters(char *flag,char *number);

#endif //_TAIL_H_