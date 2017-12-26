#pragma once

#include <stdint.h>
#include "SSoT_ControlBlock.h"

struct SSoT_QueueNode
{
    struct SSoT_QueueNode* pPrevious;
    SSoT_ControlBlockType tcb;
};

typedef SSoT_QueueNode SSoT_QueueNodeType;

struct SSoT_Queue
{
    uint32_t size;
    SSoT_QueueNode* pHead;
    SSoT_QueueNode* pTail;
}

typedef struct SSoT_Queue SSoT_QueueType;


SSoT_QueueType* create_SSoT_Queue(uint32_t size);

SSoT_QueueOpType add(SSoT_QueueType* pQueue, SSoT_QueueNode* )

