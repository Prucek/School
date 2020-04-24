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
    {
        fprintf(stderr,"ERROR: Wrong number of arguments!\n");
        return EXIT_FAILURE;
    }
        
    int imm_count = chceck_imm_count(argv[1]);
    int imm_delay = chceck_argument(argv[2]);
    int ju_delay = chceck_argument(argv[3]);
    int imm_cert_delay = chceck_argument(argv[4]);
    int ju_cert_delay = chceck_argument(argv[5]);

    if(imm_count == EXIT_ERROR|| imm_delay == EXIT_ERROR|| ju_delay == EXIT_ERROR|| 
                        imm_cert_delay == EXIT_ERROR || ju_cert_delay == EXIT_ERROR)
    {
        fprintf(stderr,"ERROR: Wrong arguments!\n");
        return EXIT_FAILURE;
    }
        

    //alloccing shared memory
    if(shared_memory_constructor(imm_count) == EXIT_ERROR)
        return EXIT_FAILURE;

    semaphores_constructor();

    //file opening
    fd = fopen("proj2.out","w");
    fclose(fd);
    if(fd == NULL)
        fprintf(stderr,"ERROR: File opening failed!\n");

    //creating childs => judge and imm_generator
    pid_t child1, child2;

    child1 = fork();

    if (child1 == 0) 
    {
        judge_process(ju_delay,ju_cert_delay);
    } 
    else 
    {
        child2 = fork();

        if (child2 == 0) 
        {
            if(imm_generator(imm_delay,imm_cert_delay) == EXIT_ERROR)
            {
                wait(&child1);
                wait(&child2);
                semaphores_destructor();
                shared_memory_destructor(imm_count);
                kill(-getppid(), SIGINT); //TODO
                return EXIT_FAILURE;
            }
                
        } else 
        {
            //Parent Code =>do nothing
        }
    }

    wait(&child1);
    wait(&child2);

    semaphores_destructor();

    //freeing alloccated memory
    shared_memory_destructor(imm_count);

    return EXIT_SUCCESS;
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


int chceck_argument(char* arg)
{
    if(!isUInt(arg))
        return EXIT_ERROR;
    int result = strtol(arg,NULL,10);
    if(!(result >= 0 && result <= 2000))
        return EXIT_ERROR;
    return result;
}

int chceck_imm_count(char *arg)
{
    if(!isUInt(arg))
        return EXIT_ERROR;
    int result = strtol(arg,NULL,10);
    if(!(result >= 1))
        return EXIT_ERROR;
    return result;
}

bool isUInt(char number[])
{
    for (int i = 0; number[i] != 0; i++)
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


int shared_memory_constructor(int imm_count)
{
    line_count = mmap(NULL, sizeof(int),PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    noJudge = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    mutex = mmap(NULL, sizeof(sem_t),   PROT_WRITE | PROT_READ,
                                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    allSignedIn = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printing = mmap(NULL, sizeof(sem_t),    PROT_WRITE | PROT_READ,
                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    allOut = mmap(NULL, sizeof(sem_t),      PROT_WRITE | PROT_READ,
                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    imm = mmap(NULL, sizeof(immigrants_t)+sizeof(sem_t)*imm_count, PROT_WRITE | PROT_READ,
                                                                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    fh = mmap(NULL, sizeof(court_t)+sizeof(int)*imm_count, PROT_WRITE | PROT_READ,
                                                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    if(line_count == MAP_FAILED || noJudge == MAP_FAILED || mutex == MAP_FAILED || allSignedIn == MAP_FAILED || 
    printing == MAP_FAILED || allOut == MAP_FAILED || imm == MAP_FAILED || fh == MAP_FAILED)
    {
        fprintf(stderr,"ERROR: Alloccation failed!\n");
        shared_memory_destructor(imm_count);
        return EXIT_ERROR;
    }
        
    *line_count = 1;
    imm->actual_index = 1;
    court_constructor(imm_count);        

    return EXIT_SUCCESS;                                
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
