#include <unistd.h>
#include <semaphore.h>
#include "SSoT_Scheduler.h"

static void* SSoT_Scheduler_Thread(void* arg);
static void* SSoT_Runner(void* arg);

static SSoT_ControlBlockType s_low_priority_thread_list[MAX_LOW_PRIORITY_THREAD_COUNT] = {0};
static SSoT_ControlBlockType s_high_priority_thread_list[MAX_HIGH_PRIORITY_THREAD_COUNT] = {0};

int32_t create_SSoT_thread(
    SSoT_PriorityType priority,
    SSoT_NameType* name,
    SSoT_ThreadMethodType method
    )
{
    int32_t i;
    if (LOW_PRIORITY == priority)
    {
        for (i = 0; i < ((sizeof(s_low_priority_thread_list)) / (sizeof(s_low_priority_thread_list[0]))); ++i)
        {
            if (NOT_REGISTERED == s_low_priority_thread_list[i].registration)
            {
                s_low_priority_thread_list[i].registration = REGISTERED;
                s_low_priority_thread_list[i].id = i;
                s_low_priority_thread_list[i].name = name;
                s_low_priority_thread_list[i].method = method;

                sem_init(&s_low_priority_thread_list[i].synchObject, 0, 0);

                pthread_create(
                    &s_low_priority_thread_list[i].posixThreadId,
                    NULL,
                    SSoT_Runner,
                    (void*)&s_low_priority_thread_list[i]
                    );
                break;
            }
        }

        if (i == ((sizeof(s_low_priority_thread_list)) / (sizeof(s_low_priority_thread_list[0]))))
        {
            i = -1;
        }
    }
    else
    {
        for (i = 0; i < ((sizeof(s_high_priority_thread_list)) / (sizeof(s_high_priority_thread_list[0]))); ++i)
        {
            if (NOT_REGISTERED == s_high_priority_thread_list[i].registration)
            {
                s_high_priority_thread_list[i].registration = REGISTERED;
                s_high_priority_thread_list[i].id = i;
                s_high_priority_thread_list[i].name = name;
                s_high_priority_thread_list[i].method = method;

                sem_init(&s_high_priority_thread_list[i].synchObject, 0, 0);

                pthread_create(
                    &s_high_priority_thread_list[i].posixThreadId,
                    NULL,
                    SSoT_Runner,
                    (void*)&s_high_priority_thread_list[i]
                    );

                break;
            }
        }

        if (i == ((sizeof(s_high_priority_thread_list)) / (sizeof(s_high_priority_thread_list[0]))))
        {
            i = -1;
        }
    }
    return i;
}

void* SSoT_Runner(void* arg)
{
    void* return_value;
    SSoT_ControlBlockType* pThreadControlBlock = (SSoT_ControlBlockType*)arg;
    SSoT_SyncType* pSynchObject = &pThreadControlBlock->synchObject;
    while (1U == 1U)
    {
        sem_wait(pSynchObject);
        return_value = pThreadControlBlock->method(arg);
    }

    return return_value;
}

static void* SSoT_Scheduler_Thread(void* arg)
{
    while (1U == 1U)
    {
        for (uint32_t i = 0; i < ((sizeof(s_high_priority_thread_list)) / (sizeof(s_high_priority_thread_list[0]))); ++i) 
        {
            if (REGISTERED == s_high_priority_thread_list[i].registration) {
                if (READY == s_high_priority_thread_list[i].state) 
                {
                    sem_post(&s_high_priority_thread_list[i].synchObject);
                    usleep(1000000U);
                }
            }
        }

        for (uint32_t i = 0; i < ((sizeof(s_low_priority_thread_list)) / (sizeof(s_low_priority_thread_list[0]))); ++i) 
        {
            if (REGISTERED == s_low_priority_thread_list[i].registration) {
                if (READY == s_low_priority_thread_list[i].state) 
                {
                    sem_post(&s_low_priority_thread_list[i].synchObject);
                    usleep(1000000U);
                }
            }
        }
    }

    return NULL;
}


pthread_t run_SSoT_Scheduler(void)
{
    pthread_t schedulerThreadId;
    pthread_create(
        &schedulerThreadId,
        NULL,
        SSoT_Scheduler_Thread,
        NULL
        );
    return schedulerThreadId;
}