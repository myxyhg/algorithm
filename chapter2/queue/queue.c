#include "algo.h"

typedef struct QUEUE_st{
    void **ppData;      /* the pointer array for saving data pointer */
    UINT uMaxCount;     /* the maximum quantity in queue */
    UINT uHead;
    UINT uTail;
}QUEUE;

/*
 * insert data to queue's tail, double size if queue is full
 * @param QUEUE *pQueue
 * @param void *pData
 * @return INT -- return CAPI_FAILED means can not apply for memory
 * and insertion operation was failed; return CAPi_SUCCESS means
 * insert successfully.
 */
INT Queue_InsertTail(QUEUE *pQueue, void *pData)
{
    UINT uTailNext;
    /* get next location after tail */
    if(pQueue->uTail == pQueue->uMaxCount-1)
    {
        /*
         * next location need to be counted from array's head
         * when pointer arrival array's tail
         */
        uTailNext = 0;
    }
    else
        uTailNext = pQueue->uTail + 1;

    /* the queue is not full */
    if(uTailNext != pQueue->uHead)
    {
        pQueue->ppData[pQueue->uTail] = pData;
        pQueue->uTail = uTailNext;
    }
    /* queue is full and double size */
    else
    {
        void **ppData = (void **)malloc(pQueue->uMaxCount * 2 * sizeof(void *));
        if(ppData == NULL)
            return CAPI_FAILED;
        if(pQueue->uHead > pQueue->Tail)
        {
            UINT i;
            /* copy data from uHead to queue's tail */
            for(i = pQueue->uHead;i < pQueue->uMaxCount; i++)
                ppData[i] = pQueue->ppData[i];
            /* copy data from 0 to uTail */
            for(i = 0; i < pQueue->uTail; i++)
                ppData[i+pQueue->uMaxCount] = pQueue->ppData[i];
            pQueue->uTail += pQueue->uMaxCount;
        }
        else
        {
            UINT i;
            for(i = pQueue->uHead;i < pQueue->uTail;i++)
                ppData[i] = pQueue->ppData[i];
        }
    /* insert data into new allocate memory */
    ppData[pQueue->uTail] = pData;
    pQueue->uTail += 1;
    pQueue->uMaxCount *= 2;
    free(pQueue->ppData);
    pQueue->ppData = ppData;
    }
    return CAPI_SUCCESS;
}

/*
 * pop head's data in queue
 * @param QUEUE *pQueue
 * @return void *
 */
void *Queue_PopHead(QUEUE *pQueue)
{
    UINT uHead;
    uHead = pQueue->uHead;
    if(uHead != pQueue->uTail)
    {
        if(uHead == pQueue->uMaxCount-1)
            pQueue->uHead = 0;
        else
            pQueue->uHead += 1;
        return pQueue->ppData[uHead];
    }
    else
        return NULL;
}
