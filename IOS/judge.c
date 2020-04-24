/**
 * @file judge.c
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


#include "judge.h"


void judge_process(int delay, int certificate_delay)
{
    while (fh->waiting_in_queue != 0)
    {
        random_delay(delay);
        if(fh->waiting_in_queue == 0)
            break;
        judge_lifecycle(certificate_delay);
    }

    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : finishes\n",(*line_count)++);
    unlock_data_and_printing();

    exit(EXIT_SUCCESS);
}


void judge_lifecycle(int certificate_delay)
{
    if(fh->waiting_in_queue <= 0)
        return;

    wants_to_enter();
    
    waiting_for_immigrants_to_leave();
        
    if(fh->waiting_in_queue <= 0)
        return;    

    sem_wait(noJudge);
    sem_wait(mutex);  

    enter();
  
    if(fh->entered > fh->checked)
    {   
        waiting_for_imm_to_check();
        sem_post(mutex); 
        sem_wait(allSignedIn);
    }
    
    starts_confirmation();

    confirmation(certificate_delay);
    
    ends_confirmation();

    leave(certificate_delay);
    
    sem_post(mutex);
    sem_post(noJudge);
}


void wants_to_enter()
{
    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : wants to enter\n",(*line_count)++);
    unlock_data_and_printing();
}


void waiting_for_immigrants_to_leave()
{
    if(fh->go_out_before_judge_comes != 0 )
    {
        lock_data_and_printing();
            int value = 0;
            sem_getvalue(allOut,&value);
        unlock_data_and_printing();

        if(value == 1)
            sem_wait(allOut);
        sem_wait(allOut);
    }
}


void enter()
{
    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : enters               :%d:%d:%d\n",
            (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();

    fh->isJudge = true;  
    fh->go_out_before_judge_comes = fh->entered;

}


void waiting_for_imm_to_check()
{
    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : waits for imm        :%d:%d:%d\n",
            (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();  
}


void starts_confirmation()
{
    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : starts confirmation :%d:%d:%d\n",
            (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();
}


void confirmation(int certificate_delay)
{
     for(int i = 0; i < fh->imm_count; i++)
    {
        if(fh->index_in_court[i] == 1)
        {
            sem_post(&(imm->confirmed[i]));
        }
    }
    random_delay(certificate_delay); 
}


void ends_confirmation()
{
    lock_data_and_printing();
        fh->waiting_for_registration = 0;
        fh->checked = 0; 
        fprintf(fd,"%d    : JUDGE    : ends confirmation :%d:%d:%d\n",
            (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();
}


void leave(int certificate_delay)
{
    random_delay(certificate_delay);

    lock_data_and_printing();
        fprintf(fd,"%d    : JUDGE    : leaves           :%d:%d:%d\n",
            (*line_count)++,fh->waiting_for_registration,fh->checked,fh->entered);
    unlock_data_and_printing();

    fh->isJudge = false;
}