// 1 noJudge = Semaphore (1)
// 2 entered = 0
// 3 checked = 0
// 4 mutex = Semaphore (1)
// 5 confirmed = Semaphore (0)
// #=============================================
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

// #===============================================
// $ ./proj2 PI IG JG IT JT
// kde
// • PI je poˇcet procesu vygenerovaných v kategorii p ˚ ˇristˇehovalcu; bude postupn ˚ ˇe vytvoˇreno PI
// immigrants.
// P >= 1
// • IG je maximální hodnota doby (v milisekundách), po které je generován nový proces immigrant.
// IG >= 0 && IG <= 2000.
// • JG je maximální hodnota doby (v milisekundách), po které soudce opˇet vstoupí do budovy.
// JG >= 0 && JG <= 2000.
// • IT je maximální hodnota doby (v milisekundách), která simuluje trvání vyzvedávání certifikátu
// pˇristˇehovalcem.
// IT >= 0 && IT <= 2000.
// • JT je maximální hodnota doby (v milisekundách), která simuluje trvání vydávání rozhodnutí
// soudcem.
// JT >= 0 && JT <= 2000.
// • Všechny parametry jsou celá ˇcís
// #===============================================
// pid=fork();
// if (pid==0) {
// // k´od pro proces potomka
// // exec(....), exit(exitcode)
// } else if (pid==-1) {
// // k´od pro rodiˇce, nastala chyba pˇri fork()
// // errno obsahuje bliˇzˇs´ı informace
// } else {
// // k´od pro rodiˇce, pid = PID potomka
// // pid2 = wait(&stav);
// }

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
    //checking arguments
    if (argc != 6)
        return 1;
    for (int i = 1; i < 6; i++ )  
        if(!isUInt(argv[i]))
            return 1;
    printf("OK\n");

    int imigrants_count = strtol(argv[1],NULL,10);

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




    sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, 
                                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    sem_init(sem, 1, 1);

    for(int i = 0; i < imigrants_count; i++)
    { 
        pid_t pid = fork();
        if (pid == 0) 
        {
            sem_wait(sem);

            printf("Starting\n");
            usleep(100000);
            //critical section
            //printf("Proces: %d\n",getpid());
            

            printf("    Exiting...\n");

            sem_post(sem);
            exit(0);

        } 
        else 
        {
            //printf("Main process\n");
        }
    }

    for(int i = 0; i < imigrants_count; i++) 
        wait(NULL);

    sem_destroy(sem);

    munmap(sem,sizeof(sem_t));

    return 0;
}

//=========================================================

// Using mmap you can create a shared memory block in your parent process. This is a basic example removing error checking for brevity.

// You want to sure the proper protections and flags are set for your needs. Then hand off the address returned by mmap to your child process.

// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/mman.h>
// #include <sys/wait.h>
// #include <unistd.h>

// #define LIMIT_MAP 5

// void child_worker(void *map)
// {
//     int map_value = -1;
//     int idx = 0;

//     while (map_value != LIMIT_MAP) {
//         map_value = *((int *) map + (idx * sizeof(int)));
//         printf("Map value: %d\n", map_value);
//         idx++;
//         sleep(2);
//     }
// }

// int main(void)
// {
//     printf("Starting Parent Process...\n");

//     long page_size = sysconf(_SC_PAGESIZE);

//     void *memory_map = mmap(0, page_size, PROT_WRITE | PROT_READ, 
//                                           MAP_SHARED | MAP_ANONYMOUS, 0, 0);

//     printf("Memory map created: <%p>\n", memory_map);

//     pid_t pid = fork();

//     if (pid == 0) {
//         sleep(1);
//         printf("Starting child process\n");
//         child_worker(memory_map);
//         printf("Exiting child process...\n");
//         return 0;

//     } else {

//         printf("Continuing in parent process\n");

//         int set_values[5] = { 1, 2, 3, 4, 5 };

//         for (int i=0; i < 5; i++) {
//             printf("Setting value: %d\n", set_values[i]);
//             *((int *) memory_map + (sizeof(int) * i)) = set_values[i];
//             sleep(1);
//         }

//         waitpid(pid, NULL, 0);

//         printf("Child process is finished!\n");
//     }

//     return 0;
// }