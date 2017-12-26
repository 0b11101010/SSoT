#include <stdint.h>
#include <stdio.h>
#include <semaphore.h>
#include "SSoT_Scheduler.h"
#include "SSoT_Queue.h"

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
    SSoT_QueueType* pSSoT_Queue = create_SSoT_Queue(16U);
    
    uint32_t i;
    SSoT_QueueErrorType error;
    for (i = 0; i < 16U; i++)
    {
        error = enqueue(pSSoT_Queue, (SSoT_IdType)i);
        if (QUEUE_ERR_OK != error)
        {
            printf("Error Occured while enqueue %d\n", i);
        }
        else
        {
            printf("Enqueued : %d\n", i);
        }
    }

    error = enqueue(pSSoT_Queue, (SSoT_IdType)i);
    if (QUEUE_ERR_OK != error)
    {
        printf("Error Occured while enqueue %d\n", i);
    }
    else
    {
        printf("Enqueued : %d\n", i);
    }

    SSoT_IdType id = -1;
    for (i = 0; i < 16U; i++)
    {
        id = -1;
        error = dequeue(pSSoT_Queue, &id);
        if (QUEUE_ERR_OK != error)
        {
            printf("Error Occured while dequeue %d\n", i);
        }
        else
        {
            printf("Dequeued : %d\n", id);
        }
    }

    id = -1;
    error  = dequeue(pSSoT_Queue, &id);
    if (QUEUE_ERR_OK != error)
    {
        printf("Error Occured while dequeue");
    }
    else
    {
        printf("Dequeued : %d\n", id);
    }

    create_SSoT_thread(LOW_PRIORITY, "Thread_A", thread_A);
    create_SSoT_thread(LOW_PRIORITY, "Thread_B", thread_B);
    create_SSoT_thread(HIGH_PRIORITY, "Thread_C", thread_C);

    pthread_t scheduler_thread_id = run_SSoT_Scheduler();

    pthread_join(scheduler_thread_id, NULL);

    return 0;
}