// author : erhan akagunduz
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "SSoT_Scheduler.h"
#include "SSoT_Queue.h"

static void* SSoT_Scheduler_Thread(void* arg);
static void* SSoT_Runner(void* arg);

static SSoT_SyncType s_scheduler_synch_object;

static SSoT_ControlBlockType s_low_priority_thread_list[MAX_LOW_PRIORITY_THREAD_COUNT] = {};
static SSoT_QueueType* s_p_low_priority_ready_queue = NULL;
static SSoT_ControlBlockType s_high_priority_thread_list[MAX_HIGH_PRIORITY_THREAD_COUNT] = {};
static SSoT_QueueType* s_p_high_priority_ready_queue = NULL;
static SSoT_IdType s_high_priority_running_thread_id = -1;
static SSoT_IdType s_low_priority_running_thread_id = -1;

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
            if (UNINITIALIZED == s_low_priority_thread_list[i].state)
            {
                s_low_priority_thread_list[i].id = i;
                s_low_priority_thread_list[i].name = name;
                s_low_priority_thread_list[i].method = method;
                s_low_priority_thread_list[i].priority = LOW_PRIORITY;
                s_low_priority_thread_list[i].pSchedulerSynchObject = &s_scheduler_synch_object;

                sem_init(&s_low_priority_thread_list[i].synchObject, 0, 0);
                
                s_low_priority_thread_list[i].state = INITIALIZED;

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
            if (UNINITIALIZED == s_high_priority_thread_list[i].state)
            {
                s_high_priority_thread_list[i].id = i;
                s_high_priority_thread_list[i].name = name;
                s_high_priority_thread_list[i].method = method;
                s_high_priority_thread_list[i].priority = HIGH_PRIORITY;
                s_high_priority_thread_list[i].pSchedulerSynchObject = &s_scheduler_synch_object;

                sem_init(&s_high_priority_thread_list[i].synchObject, 0, 0);

                s_high_priority_thread_list[i].state = INITIALIZED;

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
    pThreadControlBlock->state = READY_TO_RUN;
    SSoT_SyncType* pSynchObject = &pThreadControlBlock->synchObject;
    while (1U == 1U)
    {
        sem_wait(pSynchObject);
        pThreadControlBlock->state = RUNNING;
        return_value = pThreadControlBlock->method(arg);
        pThreadControlBlock->state = READY_TO_RUN;
        sem_post(pThreadControlBlock->pSchedulerSynchObject);
    }

    return return_value;
}

void initialize_SSoT_Scheduler(void)
{
    s_p_low_priority_ready_queue = create_SSoT_Queue((sizeof(s_low_priority_thread_list)) / (sizeof(s_low_priority_thread_list[0])));
    s_p_high_priority_ready_queue = create_SSoT_Queue((sizeof(s_high_priority_thread_list)) / (sizeof(s_high_priority_thread_list[0])));

    sem_init(&s_scheduler_synch_object, 0, 1U);
}

static void* SSoT_Scheduler_Thread(void* arg)
{
    while (1U == 1U)
    {
        sem_wait(&s_scheduler_synch_object);
        //usleep(1000U);

        SSoT_QueueErrorType queue_error = QUEUE_ERR_OK;
        for (uint32_t i = 0; i < ((sizeof(s_high_priority_thread_list)) / (sizeof(s_high_priority_thread_list[0]))); ++i) 
        {
            if (WAITING == s_high_priority_thread_list[i].state)
            {
                // Thread is waiting for something, can't be running anymore
                if (s_high_priority_running_thread_id == s_high_priority_thread_list[i].id)
                {
                    //printf("SCHEDULER : %s was running thread, but waiting for I/O and can't run anymore\n", s_high_priority_thread_list[i].name);
                    s_high_priority_running_thread_id = -1;
                }

                if (0 != s_high_priority_thread_list[i].waitingTime)
                {
                    s_high_priority_thread_list[i].waitingTime--;
                }
                else
                {
                    //printf("SCHEDULER : %s can run, waiting for I/O state finished\n", s_high_priority_thread_list[i].name);
                    s_high_priority_thread_list[i].state = READY_TO_RUN;
                }
            }

            if (READY_TO_RUN == s_high_priority_thread_list[i].state) {
                s_high_priority_thread_list[i].state = READY;
                queue_error =  enqueue(s_p_high_priority_ready_queue, s_high_priority_thread_list[i].id);
                if (QUEUE_ERR_OK != queue_error) 
                {
                    printf("\nSCHEDULER : Error occured while enqueueing high priority thread, %d\n", queue_error);
                }
                else
                {
                    //printf("SCHEDULER : Scheduler added %s to high priority thread queue\n",s_high_priority_thread_list[i].name);
                }
            }
        }

        for (uint32_t i = 0; i < ((sizeof(s_low_priority_thread_list)) / (sizeof(s_low_priority_thread_list[0]))); ++i) 
        {
            if (WAITING == s_low_priority_thread_list[i].state)
            {
                // Thread is waiting for something, can't be running anymore
                if (s_low_priority_running_thread_id == s_low_priority_thread_list[i].id)
                {
                    //printf("SCHEDULER : %s was running thread, but waiting for I/O and can't run anymore\n", s_low_priority_thread_list[i].name);
                    s_low_priority_running_thread_id = -1;
                }

                if (0 != s_low_priority_thread_list[i].waitingTime)
                {
                    s_low_priority_thread_list[i].waitingTime--;
                }
                else
                {
                    //printf("SCHEDULER : %s can run, waiting for I/O state finished\n", s_low_priority_thread_list[i].name);
                    s_low_priority_thread_list[i].state = READY_TO_RUN;
                }
            }

            if (READY_TO_RUN == s_low_priority_thread_list[i].state) {
                s_low_priority_thread_list[i].state = READY;
                queue_error =  enqueue(s_p_low_priority_ready_queue, s_low_priority_thread_list[i].id);
                if (QUEUE_ERR_OK != queue_error) 
                {
                    printf("SCHEDULER : Error occured while enqueueing low priority thread, %d\n", queue_error);
                }
                else
                {
                    //printf("SCHEDULER : Scheduler added %s to low priority thread queue\n",s_low_priority_thread_list[i].name);
                }
            }
        }

        if (-1 == s_high_priority_running_thread_id)
        {
            SSoT_IdType id;
            queue_error = dequeue(s_p_high_priority_ready_queue, &id);
            if (QUEUE_ERR_OK == queue_error)
            {
                // Schedule high priority threads here
                s_high_priority_running_thread_id = id;
                if (-1 != s_low_priority_running_thread_id)
                {
                    // wait first sync point inside the thread to emulate preemption
                    // Here thread will be finished or at a synch point
                    if (SYNCH_POINT != s_low_priority_thread_list[s_low_priority_running_thread_id].state)
                    {
                        s_low_priority_running_thread_id = -1;
                    }
                    else if(SYNCH_POINT == s_low_priority_thread_list[s_low_priority_running_thread_id].state)
                    {
                        printf("\nSCHEDULER : %s preempted\n", s_low_priority_thread_list[s_low_priority_running_thread_id].name);
                    }
                }
                sem_post(&s_high_priority_thread_list[s_high_priority_running_thread_id].synchObject);
            }
            else if (QUEUE_ERR_ZERO_SIZE == queue_error)
            {
                if (-1 == s_low_priority_running_thread_id)
                {
                    queue_error = dequeue(s_p_low_priority_ready_queue, &id);
                    if (QUEUE_ERR_OK == queue_error)
                    {
                        s_low_priority_running_thread_id = id;
                        sem_post(&s_low_priority_thread_list[s_low_priority_running_thread_id].synchObject);
                    }
                    else if (QUEUE_ERR_ZERO_SIZE == queue_error) 
                    {
                        // There is no low priority thread to run
                        // Nothing to do
                        // But run scheduler to catch I/O completion
                        sem_post(&s_scheduler_synch_object);
                    }
                    else
                    {
                        printf("Error occured while dequeueing low priority thread, %d\n", queue_error);
                    }
                }
                else
                {
                    // There is running low priority thread.
                    // It is running or at a synch point and waiting sem post
                    if (SYNCH_POINT == s_low_priority_thread_list[s_low_priority_running_thread_id].state)
                    {
                        // Continue to working from synch point
                        sem_post(&s_low_priority_thread_list[s_low_priority_running_thread_id].synchObject);
                    }
                    else
                    {
                        // thread finished its working, we need to re-schedule
                        s_low_priority_running_thread_id = -1;
                        sem_post(&s_scheduler_synch_object);
                    }
                }
            }
            else
            {
                // Unexpected queue error occured!
                printf("Error occured while dequeueing high priority thread, %d\n", queue_error);
            }
        }
        else
        {
            // wait high priority thread to finish its work or wait for the synch point
            if (SYNCH_POINT == s_high_priority_thread_list[s_high_priority_running_thread_id].state)
            {
                // Continue to working from synch point
                sem_post(&s_high_priority_thread_list[s_high_priority_running_thread_id].synchObject);
            }
            else
            {
                // Finished its work, re-schedule
                s_high_priority_running_thread_id = -1;
                sem_post(&s_scheduler_synch_object);
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