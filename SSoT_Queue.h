#ifndef SSOT_QUEUE_H
#define SSOT_QUEUE_H

#include <stdint.h>
#include "SSoT_ControlBlock.h"

struct SSoT_QueueNode
{
    struct SSoT_QueueNode* pPrevious;
    SSoT_IdType id;
};

typedef struct SSoT_QueueNode SSoT_QueueNodeType;

struct SSoT_Queue
{
    uint32_t limit;
    uint32_t size;
    SSoT_QueueNodeType* pFront;
    SSoT_QueueNodeType* pTail;
};

typedef struct SSoT_Queue SSoT_QueueType;

typedef enum
{
    QUEUE_ERR_OK = 0,
    QUEUE_ERR_MEMORY_ALLOCATION = 1,
    QUEUE_ERR_LIMIT_REACHED = 2,
    QUEUE_ERR_ZERO_SIZE = 3
}SSoT_QueueErrorType;


SSoT_QueueType* create_SSoT_Queue(uint32_t limit);

SSoT_QueueErrorType enqueue(SSoT_QueueType* pQueue, SSoT_IdType id);
SSoT_QueueErrorType dequeue(SSoT_QueueType* pQueue, SSoT_IdType* const pId);

#endif