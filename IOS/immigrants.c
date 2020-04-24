/**
 * @file immigrants.c
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


#include "immigrants.h"


int imm_generator(int delay, int certificate_delay)
{
    for(int i = 1; i <= fh->imm_count; i++)
    {    
        pid_t pid = fork();
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
            delete_immigrants();
            exit(1); //TODO
            return -1 ;
        }     
    }
    delete_immigrants();
    return 0;
}


void immigrant(int certificate_delay)
{              
    int index = starts();

    sem_wait(noJudge);  
        imm_enter(index);
    sem_post(noJudge); 

    sem_wait(mutex);  
    
    check(index);

    if(fh->isJudge == true && fh->entered == fh->checked)
    {
        sem_post(allSignedIn);
    }
    else 
        sem_post(mutex);
   
    sem_wait(&(imm->confirmed[index-1]));   

    wants_certificate(index);

    got_certificate(index,certificate_delay);

    sem_wait(noJudge);
        imm_leave(index);
    sem_post(noJudge);
}


int starts()
{
    lock_data_and_printing();
        int index = imm->actual_index++;
        fprintf(fd,"%d    : IMM %d    : starts\n",(*line_count)++,index);
    unlock_data_and_printing();
    return index;
}


void imm_enter(int index)
{
    lock_data_and_printing();
        fh->entered++;
        fh->waiting_for_registration++;
        fprintf(fd,"%d    : IMM %d    : enters              :%d:%d:%d\n",
            (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();
}

void check(int index)
{
    lock_data_and_printing();
        fh->checked++;
        fh->index_in_court[index-1] = 1;
        fprintf(fd,"%d    : IMM %d    : checks              :%d:%d:%d\n",
            (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();
}

void wants_certificate(int index)
{
    lock_data_and_printing();
        fh->waiting_in_queue--;
        fprintf(fd,"%d    : IMM %d    : wants certificate   :%d:%d:%d\n",
            (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();
}

void got_certificate(int index,int certificate_delay)
{
    lock_data_and_printing();
        random_delay(certificate_delay);
        fprintf(fd,"%d    : IMM %d    : got certificate     :%d:%d:%d\n",
            (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();
}

void imm_leave(int index)
{
    lock_data_and_printing();
        fh->entered--;
        fh->go_out_before_judge_comes--;
        fprintf(fd,"%d    : IMM %d    : leaves              :%d:%d:%d\n",
            (*line_count)++,index,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();
    lock_data_and_printing();
        int value = 0;
        sem_getvalue(allOut,&value);
        if(fh->go_out_before_judge_comes == 0 && value == 0)
        {
            sem_post(allOut);
        }
    unlock_data_and_printing();
}


void delete_immigrants()
{
    for(int i = 0; i < fh->imm_count; i++)
        wait(NULL);
}