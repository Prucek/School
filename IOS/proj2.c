
 
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
int *line_count;//, *actual_imm_count;
sem_t *noJudge, *mutex, *allSignedIn, *printing, *allOut;
//int *entered, *checked;
//bool *isJudge;
//sem_t *conf_arr;

//TODO
//================================
typedef struct hall
{
    int entered;
    int checked;
    int actual_imm_count;
    int waiting_for_registration;
    int go_out_before_judge_comes;
    int imm_count;
    bool isJudge;
    int index_in_court[];
} court;

court *fh;

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

    fh->entered++;
    fh->waiting_for_registration++;
    
    sem_wait(printing);
    printf("%d    : IMM %d    : enters :%d:%d:%d\n",(*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    sem_post(printing);

    sem_post(noJudge); 

    sem_wait(mutex); 

    fh->checked++;
    fh->index_in_court[index-1] = 1;
    
    sem_wait(printing);
    printf("%d    : IMM %d    : checks :%d:%d:%d\n",(*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    sem_post(printing);

    if(fh->isJudge == true && fh->entered == fh->checked)
    {
        sem_post(allSignedIn);
    }
    else 
        sem_post(mutex);
   
    sem_wait(&(c->conf_arr[index-1]));

    sem_wait(printing);
    printf("%d    : IMM %d    : wants certificate :%d:%d:%d\n",(*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    sem_post(printing);

    random_delay(certificate_delay);

    sem_wait(printing);
    printf("%d    : IMM %d    : got certificate :%d:%d:%d\n",(*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    sem_post(printing);

    sem_wait(noJudge);
    
    sem_wait(printing);
    printf("%d    : IMM %d    : leaves :%d:%d:%d\n",(*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    sem_post(printing);

    fh->actual_imm_count--;
    fh->entered--;
    fh->go_out_before_judge_comes--;

    sem_post(noJudge);

    if(fh->go_out_before_judge_comes == 0)
    {
        sem_post(allOut);
    }
    
}

void judge(int certificate_delay)
{

    sem_wait(printing);
    printf("%d    : JUDGE    : wants to enter\n",(*line_count)++);
    sem_post(printing);
    
    if(fh->go_out_before_judge_comes != 0)
        sem_wait(allOut);

    sem_wait(noJudge);
    sem_wait(mutex);  

    sem_wait(printing);
    printf("%d    : JUDGE    : enters :%d:%d:%d\n",(*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    sem_post(printing);

    fh->isJudge = true;  
    fh->go_out_before_judge_comes = fh->entered;

    if(fh->entered > fh->checked)
    {
        sem_wait(printing);
        printf("%d    : JUDGE    : waits for imm :%d:%d:%d\n",(*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
        sem_post(printing);

        sem_post(mutex); 
        sem_wait(allSignedIn);

    } 

    sem_wait(printing);
    printf("%d    : JUDGE    : starts confirmation :%d:%d:%d\n",(*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    sem_post(printing);

    for(int i = 0; i < fh->imm_count; i++)
    {
        if(fh->index_in_court[i] == 1)
        {
            sem_post(&(c->conf_arr[i]));
        }
    }

    random_delay(certificate_delay);

    

    sem_wait(printing);
    fh->waiting_for_registration = 0;
    fh->checked = 0; 
    printf("%d    : JUDGE    : ends confirmation :%d:%d:%d\n",(*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    sem_post(printing);
    

    random_delay(certificate_delay);

    sem_wait(printing);
    printf("%d    : JUDGE    : leaves :%d:%d:%d\n",(*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    sem_post(printing);

    fh->isJudge = false;
    
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
    while (fh->actual_imm_count)
    {
        random_delay(delay);
        if(!(fh->actual_imm_count))
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

    noJudge = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    mutex = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    allSignedIn = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    printing = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    allOut = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    //TODO
    sem_init(noJudge, 1, 1);
    sem_init(mutex, 1, 1);
    sem_init(allSignedIn, 1, 0);
    sem_init(printing, 1, 1);
    sem_init(allOut,1,0);

    
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
    // actual_imm_count = mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ,
    //                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // *actual_imm_count = imm_count;

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
    fh = mmap(NULL, sizeof(court)+sizeof(int)*imm_count, PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for(int i = 0; i < imm_count; i++)
    {
        fh->index_in_court[i] = 0;
    } 
    fh->imm_count = imm_count;
    fh->entered = 0;
    fh->checked =0;
    fh->isJudge = false;
    fh->actual_imm_count = imm_count;
    fh->waiting_for_registration = 0;
    fh->go_out_before_judge_comes = 0;
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
    //munmap(actual_imm_count,sizeof(int));

    sem_destroy(noJudge);
    sem_destroy(mutex);
    sem_destroy(allSignedIn);
    sem_destroy(printing);
    sem_destroy(allOut);

    for(int i = 0; i < imm_count; i++)
    {
        sem_destroy(&(c->conf_arr[i]));
    }
    munmap(c,sizeof(confirmed_t)+sizeof(sem_t)*imm_count);
    munmap(fh,sizeof(court)+sizeof(int)*imm_count);

    munmap(noJudge,sizeof(sem_t));
    munmap(mutex,sizeof(sem_t));
    munmap(allSignedIn,sizeof(sem_t));
    munmap(printing,sizeof(sem_t));
    munmap(allOut,sizeof(sem_t));

//=======================================================

    return 0;
}
