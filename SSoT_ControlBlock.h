// author : erhan akagunduz
#ifndef SSOT_CONTROL_BLOCK_H
#define SSOT_CONTROL_BLOCK_H

#include <stdint.h>
#include <pthread.h>
#include <semaphore.h>

typedef int32_t SSoT_IdType;
typedef char SSoT_NameType;
typedef void* (*SSoT_ThreadMethodType)(void* arg);

typedef enum
{
    LOW_PRIORITY = 0,
    HIGH_PRIORITY = 1
}SSoT_PriorityType;

typedef pthread_t SSoT_PosixThreadId;
typedef sem_t SSoT_SyncType;

typedef uint32_t SSoTWaitingTimeType;

typedef enum
{
    UNINITIALIZED = 0,
    INITIALIZED,
    READY_TO_RUN,
    READY,
    RUNNING,
    SYNCH_POINT,
    WAITING,
    KILLED
}SSoT_StateType;

typedef struct 
{
    SSoT_StateType state;
    SSoTWaitingTimeType waitingTime;
    SSoT_IdType id;
    SSoT_PosixThreadId posixThreadId;
    SSoT_PriorityType priority;
    SSoT_SyncType synchObject;
    SSoT_SyncType* pSchedulerSynchObject;
    SSoT_NameType* name;
    SSoT_ThreadMethodType method;
}SSoT_ControlBlockType;

#endif
