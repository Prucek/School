/**
 * @author Peter Rucek, xrucek00
 *  
 * @date 21.4.2020
 * 
 * @version 1.5
 * 
 * @brief Faneuil Hall Problem Implementation
 */

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

//TODO
int *line_count, *actual_imm_count;
sem_t *noJudge, *mutex, *allSignedIn, *printing, *time_sem;//, *confirmed;
int *entered, *checked;
bool *isJudge;
//sem_t *conf_arr;

//TODO
//================================
typedef struct in_court
{
    int imm_count;
    int index_in_court[];
} in_court_t;

in_court_t *in;

typedef struct confirmed
{
    int blah;
    sem_t conf_arr[];

} confirmed_t;

confirmed_t *c;
//================================

void random_delay(int base)
{
    int ms = (rand() % (base+1))*1000;
    usleep(ms);
}

void immigrant(int index,int certificate_delay)
{                   
    sem_wait(printing);
    printf("%d    : IMM %d    : starts\n",(*line_count)++,index);
    sem_post(printing);

    sem_wait(noJudge); 

    (*entered)++;
    
    sem_wait(printing);
    printf("%d    : IMM %d    : enters\n",(*line_count)++,index);
    sem_post(printing);

    sem_post(noJudge); 

    sem_wait(mutex); 

    (*checked)++;
    in->index_in_court[index-1] = 1;
    
    sem_wait(printing);
    printf("%d    : IMM %d    : checks\n",(*line_count)++,index);
    sem_post(printing);

    if(*isJudge == true && *entered == *checked)
    {
        sem_post(allSignedIn);
    }
    else 
        sem_post(mutex);
   
    sem_wait(&(c->conf_arr[index-1]));

    sem_wait(printing);
    printf("%d    : IMM %d    : wants certificate\n",(*line_count)++,index);
    sem_post(printing);

    random_delay(certificate_delay);

    sem_wait(printing);
    printf("%d    : IMM %d    : got certificate\n",(*line_count)++,index);
    sem_post(printing);

    sem_wait(noJudge);
    
    sem_wait(printing);
    printf("%d    : IMM %d    : leaves\n",(*line_count)++,index);
    sem_post(printing);

    sem_post(noJudge);
    (*actual_imm_count)--;

}

void judge(int certificate_delay)
{
    sem_wait(printing);
    printf("%d    : JUDGE    : wants to enter\n",(*line_count)++);
    sem_post(printing);
    
    sem_wait(noJudge);
    sem_wait(mutex);  

    sem_wait(printing);
    printf("%d    : JUDGE    : enters\n",(*line_count)++);
    sem_post(printing);

    *isJudge = true;  

    if(*entered > *checked)
    {
        sem_wait(printing);
        printf("%d    : JUDGE    : waits for imm\n",(*line_count)++);
        sem_post(printing);

        sem_post(mutex); 
        sem_wait(allSignedIn);

    } 

    sem_wait(printing);
    printf("%d    : JUDGE    : starts confirmation\n",(*line_count)++);
    sem_post(printing);

    for(int i = 0; i < in->imm_count; i++)
    {
        if(in->index_in_court[i] == 1)
        {
            sem_post(&(c->conf_arr[i]));
        }
    }

    random_delay(certificate_delay);

    sem_wait(printing);
    printf("%d    : JUDGE    : ends confirmation\n",(*line_count)++);
    sem_post(printing);

    *entered = *checked = 0; 

    random_delay(certificate_delay);

    sem_wait(printing);
    printf("%d    : JUDGE    : leaves\n",(*line_count)++);
    sem_post(printing);

    *isJudge = false;
    
    sem_post(mutex);
    sem_post(noJudge);
}



void create_immigrants(int number_of_imm, int delay, int certificate_delay)
{
    for(int i = 1; i <= number_of_imm; i++)
    {    
        pid_t pid = fork();
        srand(time(0)); //KEEP
        if(!delay)
            random_delay(delay);
        if (pid == 0) 
        {                     
            immigrant(i,certificate_delay);
            exit(0);
        }
    }
}


void delete_immigrants(int number_of_imm)
{
   for(int i = 0; i < number_of_imm; i++)
        wait(NULL);
}


void judge_process(int delay, int certificate_delay)
{
    srand(time(0)); //KEEP
    while (*actual_imm_count)
    {
        if(!(*actual_imm_count))
            break;
        random_delay(delay);
        if(!(*actual_imm_count))
            break;
        judge(certificate_delay);
    }
    printf("%d    : JUDGE    : finishes\n",(*line_count)++);
    exit(0);
}

bool isUInt(char number[])
{
    int i = 0;

    for (; number[i] != 0; i++)
        if (!isdigit(number[i]))
            return false;
    
    return true;
}

int main(int argc, char *argv[])
{
    //TODO
    line_count = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ,
                                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *line_count = 1;
    entered = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ,
                                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *entered = 0;
    checked = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ,
                                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *checked = 0;

    noJudge = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    mutex = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    //time_sem = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
    //                                     MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    isJudge = mmap(NULL, sizeof(bool), PROT_WRITE | PROT_READ,
                                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *isJudge = false;
    allSignedIn = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    printing = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    //TODO
    sem_init(noJudge, 1, 1);
    sem_init(mutex, 1, 1);
    //sem_init(time_sem, 1, 1);
    sem_init(allSignedIn, 1, 0);
    sem_init(printing, 1, 1);

    
    printf("OK\n");

    //checking arguments
    if (argc != 6)
        return 1;
    for (int i = 1; i < 6; i++ )  
        if(!isUInt(argv[i]))
            return 1;
    printf("OK\n");

    //TODO
    int imm_count = strtol(argv[1],NULL,10);
    actual_imm_count = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ,
                                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *actual_imm_count = imm_count;

    int im_delay = strtol(argv[2],NULL,10);     //max time to generate new imigrant 
    if(!(im_delay >= 0 && im_delay <= 2000))
        return 1;
    int ju_delay = strtol(argv[3],NULL,10);     //max time, when the judge re-enters the court
    if(!(ju_delay >= 0 && ju_delay <= 2000))
        return 1;
    int ti_im_delay = strtol(argv[4],NULL,10);  //max time to recieve ticket
    if(!(ti_im_delay >= 0 && ti_im_delay <= 2000))
        return 1;
    int ti_ju_delay = strtol(argv[5],NULL,10);  //max time to give ticket
    if(!(ti_ju_delay >= 0 && ti_ju_delay <= 2000))
        return 1;

    printf("OK\n");

 //=======================  
 //TODO 
    c = mmap(NULL, sizeof(confirmed_t)+sizeof(sem_t)*imm_count, PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for(int i = 0; i < imm_count; i++)
    {
        sem_init(&(c->conf_arr[i]),1,0);
    }
    in = mmap(NULL, sizeof(in_court_t)+sizeof(int)*imm_count, PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for(int i = 0; i < imm_count; i++)
    {
        in->index_in_court[i] = 0;
    } 
    in->imm_count = imm_count;
//=======================

//TODO
    pid_t child1, child2;

    child1 = fork();

    if (child1 == 0) 
    {
        judge_process(ju_delay,ti_ju_delay);
    } 
    else 
    {
        child2 = fork();

        if (child2 == 0) 
        {
            create_immigrants(imm_count,im_delay,ti_im_delay);
        } else 
        {
            /* Parent Code */
        }
    }


//TODO
//=======================================================
    delete_immigrants(imm_count);
    wait(&child1);
    wait(&child2);

    munmap(line_count,sizeof(int));
    munmap(isJudge,sizeof(bool));
    munmap(actual_imm_count,sizeof(int));

    sem_destroy(noJudge);
    sem_destroy(mutex);
    //sem_destroy(time_sem);
    sem_destroy(allSignedIn);
    sem_destroy(printing);

    for(int i = 0; i < imm_count; i++)
    {
        sem_destroy(&(c->conf_arr[i]));
    }
    munmap(c,sizeof(confirmed_t)+sizeof(sem_t)*imm_count);
    munmap(in,sizeof(in_court_t)+sizeof(int)*imm_count);

    munmap(noJudge,sizeof(sem_t));
    munmap(mutex,sizeof(sem_t));
    //munmap(time_sem,sizeof(sem_t));
    munmap(allSignedIn,sizeof(sem_t));
    munmap(printing,sizeof(sem_t));

//=======================================================

    return 0;
}