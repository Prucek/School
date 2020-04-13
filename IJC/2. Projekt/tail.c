// tail.c
// Riesenie IJC-DU2, příklad 1, 5.4.2020
// Autor: Peter Rucek, xrucek00, FIT
//  GCC 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)
// Implementation of the POSIX tail function

#include "tail.h"

struct CircularBuffer *listConstructor(int numOfNodes)
{
    //List allocation
    struct CircularBuffer *myList = malloc(sizeof(struct CircularBuffer));
    if (myList == NULL)
        return NULL;
    myList->size = numOfNodes;

    //first node allocation
    struct ListNode *firstNode = malloc(sizeof(struct ListNode));
    if (firstNode == NULL)
        return NULL;
    myList->end = firstNode;

    //allocation of all other nodes of the CyclisList
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

void listDestructor(struct CircularBuffer *list)
{
    for(int i = 0; i < list->size; i++)
    {
        struct ListNode *tmp = list->end->next;
        free(list->end);
        list->end = tmp; 
    }
    free(list);
}

void addNode(struct CircularBuffer *list, char *line)
{
    strcpy(list->end->data,line);
    list->end = list->end->next;
}

void printList(struct CircularBuffer *list)
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

void tailWithPlus(FILE *file,int num)
{
    int count = 0;
    char buffer[MAX];
    bool wasHere = false;

    while (fgets(buffer,MAX,file) != NULL)
    {
        count++;
        if (strchr(buffer, '\n') == NULL && !feof(file))
        {
            if(!wasHere)
                fprintf(stderr,"Error: Too long line!\n");
        
            wasHere = true;

            buffer[MAX-1] = '\n';
            buffer[MAX] = '\0';

            // removing the rest part of the long line
            int c; while ((c = fgetc(file)) != EOF && c != '\n') {}
        }
        if(count >= num)
            printf("%s",buffer);
    }
}

void tail(FILE* file, int num) 
{
    struct CircularBuffer *myList = listConstructor(num);

    char buffer[MAX];
    bool wasHere = false;

    while (fgets(buffer,MAX,file) != NULL)
    {
        if (strchr(buffer, '\n') == NULL && !feof(file))
        {
            if(!wasHere)
                fprintf(stderr,"Error: Too long line!\n");
        
            wasHere = true;

            buffer[MAX-1] = '\n';
            buffer[MAX] = '\0';

            // removing the rest part of the long line
            int c; while ((c = fgetc(file)) != EOF && c != '\n') {}
        }
        addNode(myList,buffer);
    }

    printList(myList);
    listDestructor(myList);
} 


int checkParameters(char *flag,char *number)
{
    bool isPlus = false;

    if (strcmp(flag,"-n"))
    {
        fprintf(stderr,"Error: Wrong arguments!\n");
        return EXIT_FAILURE;
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
        return EXIT_FAILURE;
    }

    if (isPlus)
        return EXIT_PLUS;

    return EXIT_SUCCESS;
}

int main(int argc,char *argv[])
{
    FILE *myFile;
    
    //without "-n number" parameters
    if (argc == 1 || argc == 2)
    {
        if (argc == 2)
        {
            myFile=fopen(argv[1],"r");
            if (!myFile)
            {
                fprintf(stderr,"Error: Cannot open file %s!\n",argv[1]);
                return 1;
            }

            tail(myFile,DEFAULT);
        }
        else 
        {
            tail(stdin,DEFAULT);
        }
        
    }
    //with "-n number" parameters
    else if (argc == 3 || argc == 4)
    {
        bool isPlus = false;
        int result = 0;
        if((result = checkParameters(argv[1],argv[2])) == 1)
            return EXIT_FAILURE;
        if (result == -1)
        {
            isPlus = true; 
        }

        if(argc == 4)
        {
            myFile=fopen(argv[3],"r");
            if (!myFile)
            {
                fprintf(stderr,"Error: Cannot open file %s!\n",argv[3]);
                return EXIT_FAILURE;
            }

            if(isPlus)
                tailWithPlus(myFile,atoi(argv[2]));
            else
                tail(myFile,atoi(argv[2]));
        }
        else
        {
            if(isPlus)
                tailWithPlus(stdin,atoi(argv[2]));
            else
                tail(stdin,atoi(argv[2]));
        }   
    }
    else
    {
        fprintf(stderr,"Error: Wrong number of parameters!\n");
        return EXIT_FAILURE;
    }
        
    return EXIT_SUCCESS;
}