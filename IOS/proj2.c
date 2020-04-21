// 1 noJudge = Semaphore (1)
// 2 entered = 0
// 3 checked = 0
// 4 mutex = Semaphore (1)
// 5 confirmed = Semaphore (0)
// #=============================================
// IMM
// 1 noJudge . wait ()
// 2 enter ()
// 3 entered ++
// 4 noJudge . signal ()
// 5
// 6 mutex . wait ()
// 7 checkIn ()
// 8 checked ++
// 9
// 10 if judge = 1 and entered == checked :
// 11 allSignedIn . signal ()
// # and pass the mutex
// 12 else :
// 13 mutex . signal ()
// 14
// 15 sitDown ()
// 16 confirmed . wait ()
// 17
// 18 swear ()
// 19 getCertificate ()
// 20
// 21 noJudge . wait ()
// 22 leave ()
// 23 noJudge . signal ()
// #===============================================
// JUDGE
// 1 noJudge . wait ()
// 2 mutex . wait ()
// 3
// 4 enter ()
// 5 judge = 1
// 6
// 7 if entered > checked :
// 8 mutex . signal ()
// 9 allSignedIn . wait ()
// # and get the mutex back .
// 10
// 11 confirm ()
// 12 confirmed . signal ( checked )
// 13 entered = checked = 0
// 14
// 15 leave ()
// 16 judge = 0
// 17
// 18 mutex . signal ()
// 19 noJudge . signal ()
//=========================================================

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

int *line_count, *actual_imm_count;
sem_t *noJudge, *mutex, *allSignedIn, *printing;//, *confirmed;
int *entered, *checked;
bool *isJudge;
//sem_t *conf_arr;


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


void immigrant(int index)
{  
    sem_wait(printing);
    printf("%d    : IMM %d    : starts\n",(*line_count)++,index);
    sem_post(printing);
    sem_wait(noJudge);  // 1 noJudge . wait ()
    // 2 enter ()
    (*entered)++;          // 3 entered ++
    
    sem_wait(printing);
    printf("%d    : IMM %d    : enters\n",(*line_count)++,index);
    sem_post(printing);
    //usleep(2000000);
    sem_post(noJudge);   // 4 noJudge . signal ()
                        // 5
    //usleep(2000000);
    sem_wait(mutex);    // 6 mutex . wait ()
    // 7 checkIn ()
    (*checked)++;          // 8 checked ++
                        // 9
    in->index_in_court[index-1] = 1;
    
    sem_wait(printing);
    printf("%d    : IMM %d    : checks\n",(*line_count)++,index);
    sem_post(printing);
    //usleep(2000000);
    //printf("Entered %d Checked %d\n",*entered,*checked);
    if(*isJudge == true && *entered == *checked)   // 10 if judge = 1 and entered == checked :
    {
        sem_post(allSignedIn);  // 11 allSignedIn . signal ()
        //sem_post(mutex);        // # and pass the mutex
    }
    else                // 12 else :
        sem_post(mutex);// 13 mutex . signal ()
                        // 14
    // 15 sitDown ()
    //sem_wait(confirmed);    // 16 confirmed . wait ()
    printf("%d    : IMM %d    : wants certificate\n",(*line_count)++,index);
    sem_wait(&(c->conf_arr[index-1]));                   // 17
    // 18 swear ()
    // 19 getCertificate ()
    printf("%d    : IMM %d    : got certificate\n",(*line_count)++,index);
                        // 20
    sem_wait(noJudge);  // 21 noJudge . wait ()
    sem_wait(printing);
    printf("%d    : IMM %d    : leaves\n",(*line_count)++,index);   // 22 leave ()
    sem_post(printing);
    sem_post(noJudge);  // 23 noJudge . signal ()
    (*actual_imm_count)--;

}

void judge()
{
    //usleep(2000000);
    sem_wait(printing);
    printf("%d    : JUDGE    : wants to enter\n",(*line_count)++);
    sem_post(printing);
    
    sem_wait(noJudge);  // 1noJudge . wait ()
    sem_wait(mutex);    // 2 mutex . wait ()
                        // 3
    sem_wait(printing);
    printf("%d    : JUDGE    : enters\n",(*line_count)++); // 4 enter ()
    sem_post(printing);
    *isJudge = true;     // 5 judge = 1
                        // 6
    //printf("Judge Entered %d Checked %d\n",*entered,*checked);
    if(*entered > *checked)   // 7 if entered > checked :
    {
        sem_wait(printing);
        printf("%d    : JUDGE    : waits for imm\n",(*line_count)++);
        sem_post(printing);
        sem_post(mutex);    // 8 mutex . signal ()
        sem_wait(allSignedIn);  // 9 allSignedIn . wait ()
        //sem_post(mutex);    // # and get the mutex back .
    } 
                        // 10
    // 11 confirm ()
    printf("%d    : JUDGE    : starts confirmation\n",(*line_count)++);
    
    for(int i = 0; i < in->imm_count; i++)
    {
        if(in->index_in_court[i] == 1)
        {
            sem_post(&(c->conf_arr[i]));
        }
    }
    //sem_post(confirmed);// 12 confirmed . signal ( checked )
    //sem_post(confirmed);
    *entered = *checked = 0;  // 13 entered = checked = 0
                        // 14
    sem_wait(printing);
    printf("%d    : JUDGE    : leaves\n",(*line_count)++);   // 15 leave ()
    sem_post(printing);
    *isJudge = false;    // 16 judge = 0
                        // 17
    
    sem_post(mutex);    // 18 mutex . signal ()
    sem_post(noJudge);  // 19 noJudge . signal ()
    //sem_wait(confirmed);
}



void create_immigrants(int number_of_imm, int delay)
{
    for(int i = 1; i <= number_of_imm; i++)
    {    
        
        pid_t pid = fork();
        srand(time(NULL));
        int ms = (rand() % (delay+1))*1000;
        usleep(ms);
        if (pid == 0) 
        {        
                
            immigrant(i);
            exit(0);
        }
    }
}


void delete_immigrants(int number_of_imm)
{
   for(int i = 0; i < number_of_imm; i++)
        wait(NULL);
}


void judge_process(int delay)
{
    while (*actual_imm_count)
    {
        if(!(*actual_imm_count))
            break;
        int ms = (rand() % (delay+1))*1000;
        usleep(ms);
        if(!(*actual_imm_count))
            break;
        judge();
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
    //confirmed = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
    //                                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    isJudge = mmap(NULL, sizeof(bool), PROT_WRITE | PROT_READ,
                                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *isJudge = false;
    allSignedIn = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    printing = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ,
                                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sem_init(noJudge, 1, 1);
    sem_init(mutex, 1, 1);
    //sem_init(confirmed, 1, 0);
    sem_init(allSignedIn, 1, 0);
    sem_init(printing, 1, 1);

    srand(time(NULL));
    printf("OK\n");

    //checking arguments
    if (argc != 6)
        return 1;
    for (int i = 1; i < 6; i++ )  
        if(!isUInt(argv[i]))
            return 1;
    printf("OK\n");

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
    pid_t child1, child2;

    child1 = fork();

    if (child1 == 0) 
    {
        judge_process(ju_delay);
    } 
    else 
    {
        child2 = fork();

        if (child2 == 0) 
        {
            create_immigrants(imm_count,im_delay);
        } else 
        {
            /* Parent Code */
        }
    }

    delete_immigrants(imm_count);
    wait(&child1);
    wait(&child2);

    munmap(line_count,sizeof(int));
    munmap(isJudge,sizeof(bool));
    munmap(actual_imm_count,sizeof(int));

    sem_destroy(noJudge);
    sem_destroy(mutex);
    //sem_destroy(confirmed);
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
    //munmap(confirmed,sizeof(sem_t));
    munmap(allSignedIn,sizeof(sem_t));
    munmap(printing,sizeof(sem_t));

    return 0;
}