#pragma once

#include <stdint.h>
#include <pthread.h>

typedef uint32_t SSoT_IdType;
typedef uint32_t SSoT_PriorityType;
typedef char* SSoT_NameType;
typedef void* (*SSoT_ThreadMethodType)(void* arg);

typedef struct 
{
    SSoT_IdType id;
    SSoT_PriorityType priority;
    SSoT_NameType name;
    SSoT_ThreadMethodType method;
}SSoT_ControlBlockType;
