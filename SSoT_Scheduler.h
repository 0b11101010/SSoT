// author : erhan akagunduz
#ifndef SSOT_SCHEDULER_H
#define SSOT_SCHEDULER_H

#include <stdint.h>
#include "SSoT_ControlBlock.h"

#define EXECUTE_SSoT_LINE(LINE) do{\
                                    LINE;\
                                    SSoT_ControlBlockType* const pTCB = ((SSoT_ControlBlockType*)arg);\
                                    pTCB->state = SYNCH_POINT;\
                                    sem_post(pTCB->pSchedulerSynchObject);\
                                    sem_wait(&pTCB->synchObject);\
                                    pTCB->state = RUNNING;\
                                }while(0)

#define WAIT_FOR_IO(TIME_TO_WAIT_IO)    do{\
                                            SSoT_ControlBlockType* const pTCB = ((SSoT_ControlBlockType*)arg);\
                                            pTCB->state = WAITING;\
                                            pTCB->waitingTime = TIME_TO_WAIT_IO;\
                                            sem_post(pTCB->pSchedulerSynchObject);\
                                            sem_wait(&pTCB->synchObject);\
                                            pTCB->state = RUNNING;\
                                        }while(0)

#define MAX_LOW_PRIORITY_THREAD_COUNT       ((uint32_t)16U)
#define MAX_HIGH_PRIORITY_THREAD_COUNT      ((uint32_t)16U)

void initialize_SSoT_Scheduler(void);

int32_t create_SSoT_thread(
    SSoT_PriorityType priority,
    SSoT_NameType* name,
    SSoT_ThreadMethodType method
    );

pthread_t run_SSoT_Scheduler(void);

#endif
