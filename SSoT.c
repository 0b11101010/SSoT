#include <stdint.h>
#include <stdio.h>
#include <semaphore.h>
#include "SSoT_Scheduler.h"

void* thread_A(void* arg)
{
    SSoT_ControlBlockType* pThreadControlBlock = (SSoT_ControlBlockType*)arg;
    sem_t* pSynchObject = &pThreadControlBlock->synchObject;
    while (1U == 1U)
    {
        sem_wait(pSynchObject);
        printf("%s\n",pThreadControlBlock->name);
    }

    return NULL;
}

void* thread_B(void* arg)
{
    sem_t* pSynchObject = &(((SSoT_ControlBlockType*)arg)->synchObject);
    while (1U == 1U)
    {
        sem_wait(pSynchObject);
        printf("thread_B called!\n");
    }

    return NULL;
}

void* thread_C(void* arg)
{
    sem_t* pSynchObject = &(((SSoT_ControlBlockType*)arg)->synchObject);
    while (1U == 1U)
    {
        sem_wait(pSynchObject);
        printf("thread_C called!\n");
    }

    return NULL;
}

int32_t main(int32_t argc, char* argv[])
{
    create_SSoT_thread(LOW_PRIORITY, "Thread_A", thread_A);
    create_SSoT_thread(LOW_PRIORITY, "Thread_B", thread_B);
    create_SSoT_thread(HIGH_PRIORITY, "Thread_C", thread_C);

    pthread_t scheduler_thread_id = run_SSoT_Scheduler();

    pthread_join(scheduler_thread_id, NULL);

    return 0;
}