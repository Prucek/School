
 
/**
 * @author Peter Rucek, xrucek00
 *  
 * @date 23.4.2020
 * 
 * @version 2.1
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

#include <errno.h>
#include <string.h>

//TODO
int *line_count;
sem_t *noJudge, *mutex, *allSignedIn, *printing, *allOut;
FILE *fd;

//TODO
//================================
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

court_t *fh;

typedef struct immigrants
{
    int actual_index;
    sem_t confirmed[];

} immigrants_t;

immigrants_t *imm;
//================================

void random_delay(int base)
{
    int ms = (rand() % (base+1))*1000;
    usleep(ms);
}

void lock_data_and_printing()
{
    sem_wait(printing);
    fd = fopen("proj2.out","a+");
}

void unlock_data_and_printing()
{
    fclose(fd);
    sem_post(printing);
}

void immigrant(int certificate_delay)
{              

    lock_data_and_printing();
        int index = imm->actual_index++;
        fprintf(fd,"%d    : IMM %d    : starts\n",(*line_count)++,index);
    unlock_data_and_printing();

    sem_wait(noJudge); 
    
    lock_data_and_printing();
        fh->entered++;
        fh->waiting_for_registration++;
        fprintf(fd,"%d    : IMM %d    : enters :%d:%d:%d\n",
            (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();

    sem_post(noJudge); 

    sem_wait(mutex);  
    
    lock_data_and_printing();
        fh->checked++;
        fh->index_in_court[index-1] = 1;
        fprintf(fd,"%d    : IMM %d    : checks :%d:%d:%d\n",
            (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();

    if(fh->isJudge == true && fh->entered == fh->checked)
    {
        sem_post(allSignedIn);
    }
    else 
        sem_post(mutex);
   
    sem_wait(&(imm->confirmed[index-1]));   

    lock_data_and_printing();
        fh->waiting_in_queue--;
        fprintf(fd,"%d    : IMM %d    : wants certificate :%d:%d:%d\n",
            (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();

    random_delay(certificate_delay);

    lock_data_and_printing();
    fprintf(fd,"%d    : IMM %d    : got certificate :%d:%d:%d\n",
        (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();

    sem_wait(noJudge);
    
    lock_data_and_printing();
        fh->entered--;
        fh->go_out_before_judge_comes--;
        fprintf(fd,"%d    : IMM %d    : leaves :%d:%d:%d\n",
            (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
        int value = 0;
        sem_getvalue(allOut,&value);
        if(fh->go_out_before_judge_comes == 0 && value == 0)
        {
            sem_post(allOut);
        }
    unlock_data_and_printing();
   
    
    sem_post(noJudge);
}

void judge(int certificate_delay)
{
    if(fh->waiting_in_queue <= 0)
        return;

    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : wants to enter\n",(*line_count)++);
    unlock_data_and_printing();
    
    lock_data_and_printing();
    int value = 0;
    sem_getvalue(allOut,&value);
    unlock_data_and_printing();
    if(fh->go_out_before_judge_comes != 0 )
    {
        if(value == 1)
            sem_wait(allOut);
        sem_wait(allOut);
    }

    if(fh->waiting_in_queue <= 0)
        return;   

    sem_wait(noJudge);
    sem_wait(mutex);  

    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : enters :%d:%d:%d\n",
            (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();

    fh->isJudge = true;  
    fh->go_out_before_judge_comes = fh->entered;

    if(fh->entered > fh->checked)
    {
        lock_data_and_printing();
            fprintf(fd,"%d    : JUDGE    : waits for imm :%d:%d:%d\n",
                (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
        unlock_data_and_printing();

        sem_post(mutex); 
        sem_wait(allSignedIn);

    } 

    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : starts confirmation :%d:%d:%d\n",
            (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();

    for(int i = 0; i < fh->imm_count; i++)
    {
        if(fh->index_in_court[i] == 1)
        {
            sem_post(&(imm->confirmed[i]));
        }
    }

    random_delay(certificate_delay); 

    lock_data_and_printing();
        fh->waiting_for_registration = 0;
        fh->checked = 0; 
        fprintf(fd,"%d    : JUDGE    : ends confirmation :%d:%d:%d\n",
            (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();
    

    random_delay(certificate_delay);

    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : leaves :%d:%d:%d\n",
            (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();

    fh->isJudge = false;
    
    sem_post(mutex);
    sem_post(noJudge);
}



int create_immigrants(int delay, int certificate_delay)
{
    for(int i = 1; i <= fh->imm_count; i++)
    {    
        pid_t pid = fork();
        srand(time(0)); //KEEP
        if(!delay)
            random_delay(delay);
        if (pid == 0) 
        {                     
            immigrant(certificate_delay);
            exit(0);
        }
        if(pid == -1)
        {
            //TODO
            fh->waiting_in_queue = 0;
            sleep(1);
            fprintf(stdout,"ERROR: %s\n", strerror(errno));
            exit(1); 
            return -1 ;
        }
        
    }

    return 0;
}


void delete_immigrants()
{
    for(int i = 0; i < fh->imm_count; i++)
        wait(NULL);
}


void judge_process(int delay, int certificate_delay)
{
    srand(time(0)); //KEEP
    while (fh->waiting_in_queue != 0)
    {
        random_delay(delay);
        if(fh->waiting_in_queue == 0)
            break;
        judge(certificate_delay);
    }
    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : finishes\n",(*line_count)++);
    unlock_data_and_printing();
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

void semaphores_constructor()
{
    sem_init(noJudge, 1, 1);
    sem_init(mutex, 1, 1);
    sem_init(allSignedIn, 1, 0);
    sem_init(printing, 1, 1);
    sem_init(allOut,1,0);
}

void semaphores_destructor()
{
    sem_destroy(noJudge);
    sem_destroy(mutex);
    sem_destroy(allSignedIn);
    sem_destroy(printing);
    sem_destroy(allOut);

    for(int i = 0; i < fh->imm_count; i++)
    {
        sem_destroy(&(imm->confirmed[i]));
    }
}

void shared_memory_constructor(int imm_count)
{
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

    imm = mmap(NULL, sizeof(immigrants_t)+sizeof(sem_t)*imm_count, PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for(int i = 0; i < imm_count; i++)
    {
        sem_init(&(imm->confirmed[i]),1,0);
    }
    imm->actual_index = 1;

    fh = mmap(NULL, sizeof(court_t)+sizeof(int)*imm_count, PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    for(int i = 0; i < imm_count; i++)
    {
        fh->index_in_court[i] = 0;
    } 
    fh->imm_count = imm_count;
    fh->entered = 0;
    fh->checked =0;
    fh->isJudge = false;
    fh->waiting_in_queue = imm_count;
    fh->waiting_for_registration = 0;
    fh->go_out_before_judge_comes = 0;
}

void shared_memory_destructor(int imm_count)
{
    munmap(line_count,sizeof(int));

    munmap(imm,sizeof(immigrants_t)+sizeof(sem_t)*imm_count);
    munmap(fh,sizeof(court_t)+sizeof(int)*imm_count);

    munmap(noJudge,sizeof(sem_t));
    munmap(mutex,sizeof(sem_t));
    munmap(allSignedIn,sizeof(sem_t));
    munmap(printing,sizeof(sem_t));
    munmap(allOut,sizeof(sem_t));
}

int main(int argc, char *argv[])
{
    
    //checking arguments
    if (argc != 6)
        return 1;
    for (int i = 1; i < 6; i++ )  
        if(!isUInt(argv[i]))
            return 1;

    //TODO
    int imm_count = strtol(argv[1],NULL,10);
 

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


    shared_memory_constructor(imm_count);

    semaphores_constructor();

    fd = fopen("proj2.out","w");
    fclose(fd);
    if(fd == NULL)
    {
        //TODO
        fprintf(stderr,"ERROR:\n");
    }

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
            if(create_immigrants(im_delay,ti_im_delay) == -1)
            {
                delete_immigrants();
                return 1;
            }
                
        } else 
        {
            //Parent Code =>do nothing
        }
    }

    delete_immigrants();
    wait(&child1);
    wait(&child2);

    semaphores_destructor();

    shared_memory_destructor(imm_count);

    return 0;
}
