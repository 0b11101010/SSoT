// author : erhan akagunduz
#ifndef SSOT_SCHEDULER_H
#define SSOT_SCHEDULER_H

#include <stdint.h>
#include "SSoT_ControlBlock.h"

#define EXECUTE_SSoT_LINE(LINE) do{\
                                    LINE;\
                                    ((SSoT_ControlBlockType*)arg)->state = SYNCH_POINT;\
                                    sem_post(((SSoT_ControlBlockType*)arg)->pSchedulerSynchObject);\
                                    sem_wait(&((SSoT_ControlBlockType*)arg)->synchObject);\
                                    ((SSoT_ControlBlockType*)arg)->state = RUNNING;\
                                }while(0)

#define WAIT_FOR_IO(TIME_TO_WAIT_IO)    do{\
                                            ((SSoT_ControlBlockType*)arg)->state = WAITING;\
                                            ((SSoT_ControlBlockType*)arg)->waitingTime = TIME_TO_WAIT_IO;\
                                            sem_post(((SSoT_ControlBlockType*)arg)->pSchedulerSynchObject);\
                                            sem_wait(&((SSoT_ControlBlockType*)arg)->synchObject);\
                                            ((SSoT_ControlBlockType*)arg)->state = RUNNING;\
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
