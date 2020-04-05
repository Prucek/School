#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 1023

// TOOD .h
struct ListNode
{
    struct ListNode *next;
    char data[MAX];
};

struct CyclicList
{
    int size;
    struct ListNode *end;
};
//=======================================

struct CyclicList *listConstructor(int numOfNodes)
{
    struct CyclicList *myList = malloc(sizeof(struct CyclicList));
    if (myList == NULL)
        return NULL;
    myList->size = numOfNodes;
    struct ListNode *firstNode = malloc(sizeof(struct ListNode));
    if (firstNode == NULL)
        return NULL;
    myList->end = firstNode;
    for(int i = 1; i < numOfNodes; i++)
    {
        struct ListNode *newNode = malloc(sizeof(struct ListNode));
        if (newNode == NULL)
            return NULL;
        myList->end->next = newNode;
        myList->end = newNode;
    }
    myList->end->next = firstNode;
    return myList;
}

void listDestructor(struct CyclicList *list)
{
    for(int i = 0; i < list->size; i++)
    {
        struct ListNode *tmp = list->end->next;
        free(list->end);
        list->end = tmp; 
    }

    free(list);
}

void addNode(struct CyclicList *list, char *line)
{
    strcpy(list->end->data,line);
    list->end = list->end->next;
}

void printList(struct CyclicList *list)
{
    for(int i = 0; i < list->size; i++)
    {
        if (strcmp(list->end->data,""))
        {
            printf("%s",list->end->data);
        }
        list->end = list->end->next;
    }
}

void tail(FILE* file, int num, bool isPlus) 
{
    struct CyclicList *myList = listConstructor(num);

    char buffer[MAX];

    while (fgets(buffer,MAX+1,file) != NULL)
    {
        if (strchr(buffer, '\n') == NULL && !feof(file))
        {
            fprintf(stderr,"Error: Too long line!\n");

            buffer[MAX-1] = '\n';
            buffer[MAX] = '\0';
            int c; while ((c = fgetc(file)) != EOF && c != '\n') {}
        }
        addNode(myList,buffer);
    }

    printList(myList);
    listDestructor(myList);
} 


int checkParameters(char *flag,char *number)
{
    bool isPlus;

    if (strcmp(flag,"-n"))
    {
        fprintf(stderr,"Error: Wrong arguments!\n");
        return 1;//TODO
    }
    
    if(number[0] == '+')
        isPlus = true;

    int length = strlen(number);

    int i = 0;
    if (isPlus)
        i = 1;
    for (i;i<length; i++)
    if (!isdigit(number[i]))
    {
        fprintf(stderr,"Error: Argument of -n is not a number!\n");
        return 1;//TODO
    }

    if (isPlus)
        return -1;//TODO

    return 0; //TODO
}


int main(int argc,char *argv[])
{
    FILE *myFile;
    char buffer[MAX];
    fgets(buffer,MAX,stdin);
    
    if (argc == 1 || argc == 3)
    {
        //the program should read line after line in a endless cycle
        if (argc == 3)
            if (checkParameters(argv[1],argv[2]) == 1)
                return 1;
        
        char buf[MAX];
        while(true)
            fgets(buf,MAX,stdin);
    }

    else if (argc == 2)
    {
        //without parameters, only read file
        // if (argv[1][0] == '<')
        //     memmove(&argv[1][0], &argv[1][1], strlen(argv[1]));
        myFile=fopen(argv[1],"r");
        if (!myFile)
        {
            fprintf(stderr,"Error: Cannot open file %s!\n",argv[1]);
            return 1;
        }
        
        tail(myFile,10,false);
    }

    else if (argc == 4)
    {
        bool isPlus = false;
        int result;
        if(result = checkParameters(argv[1],argv[2]) == 1)
            return 1;
        if (result == -1)
            isPlus = true; 

        myFile=fopen(argv[3],"r");
        if (!myFile)
        {
            fprintf(stderr,"Error: Cannot open file %s!\n",argv[3]);
            return 1;
        }

        tail(myFile,atoi(argv[2]),isPlus);
  
    }

    else
    {
        fprintf(stderr,"Error: Wrong number of parameters!\n");
        return 1;
    }
        
    return 0;
}

