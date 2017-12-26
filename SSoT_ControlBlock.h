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

typedef enum
{
    NOT_REGISTERED = 0,
    REGISTERED = 0xEA
}SSoT_RegistrationType;

typedef pthread_t SSoT_PosixThreadId;
typedef sem_t SSoT_SyncType;

typedef enum
{
    READY = 0,
    RUNNING = 1,
    WAITING_HARDWARE = 2
}SSoT_StateType;

typedef struct 
{
    SSoT_RegistrationType registration;
    SSoT_IdType id;
    SSoT_PosixThreadId posixThreadId;
    SSoT_PriorityType priority;
    SSoT_SyncType synchObject;
    SSoT_StateType state;
    SSoT_NameType* name;
    SSoT_ThreadMethodType method;
}SSoT_ControlBlockType;

#endif
