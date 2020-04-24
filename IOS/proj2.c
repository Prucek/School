/**
 * @file proj2.c
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


#include "proj2.h"
#include "judge.h"
#include "immigrants.h"

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
            if(imm_generator(im_delay,ti_im_delay) == -1)
            {
                delete_immigrants();
                return 1;
            }
                
        } else 
        {
            //Parent Code =>do nothing
        }
    }

    wait(&child1);
    wait(&child2);

    semaphores_destructor();

    shared_memory_destructor(imm_count);

    return 0;
}

void random_delay(int base)
{
    if(base)
    {
        srand(time(NULL)^ getpid());
        int ms = (rand() % (base+2))*1000;
        usleep(ms);
    }
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

    for(int i = 0; i < fh->imm_count; i++)
    {
        sem_init(&(imm->confirmed[i]),1,0);
    }
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


void court_constructor(int imm_count)
{
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
    imm->actual_index = 1;

    fh = mmap(NULL, sizeof(court_t)+sizeof(int)*imm_count, PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    court_constructor(imm_count);                                        
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
