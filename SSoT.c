#include <stdint.h>
#include <stdio.h>
#include <semaphore.h>
#include "SSoT_Scheduler.h"

void* thread_A(void* arg)
{
    SSoT_ControlBlockType* pThreadControlBlock = (SSoT_ControlBlockType*)arg;
    printf("%s\n",pThreadControlBlock->name);
    return NULL;
}

void* thread_B(void* arg)
{
    SSoT_ControlBlockType* pThreadControlBlock = (SSoT_ControlBlockType*)arg;
    printf("%s\n",pThreadControlBlock->name);
    return NULL;
}

void* thread_C(void* arg)
{
    SSoT_ControlBlockType* pThreadControlBlock = (SSoT_ControlBlockType*)arg;
    printf("%s\n",pThreadControlBlock->name);
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