//author : erhan akagunduz
#include <stdint.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include "SSoT_Scheduler.h"
#include "SSoT_Queue.h"

void* thread_low_priority(void* arg)
{
    SSoT_ControlBlockType* pThreadControlBlock = (SSoT_ControlBlockType*)arg;
    EXECUTE_SSoT_LINE(printf("%s is WORKING\n",pThreadControlBlock->name));
    EXECUTE_SSoT_LINE(printf("\t\t\t"));
    for (uint32_t i = 0; i < 10U; ++i)
    {
        if (9 == i)
        {
           EXECUTE_SSoT_LINE(printf("%d\n", i)); 
        }
        else
        {
            EXECUTE_SSoT_LINE(printf("%d,", i));
        }
        usleep(100000U);
    }
    return NULL;
}

void* thread_high_priority(void* arg)
{
    SSoT_ControlBlockType* pThreadControlBlock = (SSoT_ControlBlockType*)arg;
    EXECUTE_SSoT_LINE(
        printf("%s is WORKING - 0\n",pThreadControlBlock->name)
    );
    EXECUTE_SSoT_LINE(
        printf("%s will wait IO - 1\n",pThreadControlBlock->name)
    );
    WAIT_FOR_IO(35U);
    EXECUTE_SSoT_LINE(
        printf("%s returned from IO - 1\n",pThreadControlBlock->name)
    );
    EXECUTE_SSoT_LINE(
        printf("%s will wait IO - 2\n",pThreadControlBlock->name)
    );
    WAIT_FOR_IO(25U);
    EXECUTE_SSoT_LINE(
        printf("%s returned from IO - 2\n",pThreadControlBlock->name)
    );
        EXECUTE_SSoT_LINE(
        printf("%s will wait IO - 3\n",pThreadControlBlock->name)
    );
    WAIT_FOR_IO(15U);
    EXECUTE_SSoT_LINE(
        printf("%s returned from IO - 3\n",pThreadControlBlock->name)
    );
    return NULL;
}

int32_t main(int32_t argc, char* argv[])
{
    initialize_SSoT_Scheduler();

    create_SSoT_thread(LOW_PRIORITY, "Thread_l1", thread_low_priority);
    create_SSoT_thread(LOW_PRIORITY, "Thread_l2", thread_low_priority);
    create_SSoT_thread(LOW_PRIORITY, "Thread_l3", thread_low_priority);
    create_SSoT_thread(LOW_PRIORITY, "Thread_l4", thread_low_priority);
    create_SSoT_thread(LOW_PRIORITY, "Thread_l5", thread_low_priority);
    create_SSoT_thread(LOW_PRIORITY, "Thread_l6", thread_low_priority);
    create_SSoT_thread(LOW_PRIORITY, "Thread_l7", thread_low_priority);
    create_SSoT_thread(LOW_PRIORITY, "Thread_l8", thread_low_priority);
    create_SSoT_thread(HIGH_PRIORITY, "Thread_H1", thread_high_priority);

    pthread_t scheduler_thread_id = run_SSoT_Scheduler();

    pthread_join(scheduler_thread_id, NULL);

    return 0;
}