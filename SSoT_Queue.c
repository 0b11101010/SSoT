// author : erhan akagunduz
#include <stdlib.h>
#include "SSoT_Queue.h"

SSoT_QueueType* create_SSoT_Queue(uint32_t limit)
{
    SSoT_QueueType* pSSoT_Queue = (SSoT_QueueType*)malloc(sizeof(SSoT_QueueType));
    if (NULL != pSSoT_Queue)
    {
        pSSoT_Queue->limit = limit;
        pSSoT_Queue->size = 0;
        pSSoT_Queue->pFront = NULL;
        pSSoT_Queue->pTail = NULL;
    }
    return pSSoT_Queue;
}

SSoT_QueueErrorType enqueue(SSoT_QueueType* pQueue, SSoT_IdType id)
{
    SSoT_QueueErrorType error = QUEUE_ERR_OK;
    if (pQueue->size < pQueue->limit)
    {
        SSoT_QueueNodeType* pTempQueueNode = (SSoT_QueueNodeType*)malloc(sizeof(SSoT_QueueNodeType));
        if (NULL != pTempQueueNode)
        {
            pTempQueueNode->pPrevious = NULL;
            pTempQueueNode->id = id;

            if (0 == pQueue->size)
            {
                pQueue->pFront = pTempQueueNode;
                pQueue->pTail = pTempQueueNode;
            }
            else
            {
                pQueue->pTail->pPrevious = pTempQueueNode;
                pQueue->pTail = pTempQueueNode;
            }

            pQueue->size++;
        }
        else
        {
            error = QUEUE_ERR_MEMORY_ALLOCATION;
        }
    }
    else
    {
        error = QUEUE_ERR_LIMIT_REACHED;
    }
    
    return error;
}

SSoT_QueueErrorType dequeue(SSoT_QueueType* pQueue, SSoT_IdType* const pId)
{
    SSoT_QueueErrorType error = QUEUE_ERR_OK;

    if (pQueue->size > 0)
    {
        SSoT_QueueNodeType* pTempQueueNode = pQueue->pFront;
        *pId = pQueue->pFront->id;
        pQueue->pFront = (pQueue->pFront)->pPrevious;
        free((void*)pTempQueueNode);
        pQueue->size--;
    }
    else
    {
        error = QUEUE_ERR_ZERO_SIZE;
    }
    
    return error;
}