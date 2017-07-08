/*********************************************************************************
 * FileName:	DeQue.c
 * Author:		gehan
 * Date:		06/10/2017
 * Description: The circular queue program
**********************************************************************************/

#include "algo.h"

/* the definition of circuit queue structure in DeQue */
typedef struct DEQUEBLOCK_st
{
    UINT uHead;
    UINT uTail;
    UINT uMapPos;   /* position in map */
    void **ppData;
}DEQUEBLOCK;

/* the definition of DeQue */
typedef struct DEQUE_st
{
    DEQUEBLOCK **ppMap;     /* the pointer of map array */
    DEQUEBLOCK *pFirst;     /* the first pointer to circuit queue */
    DEQUEBLOCK *pLast;
    UINT uMapSize;
    UINT uBlockSize;        /* the size of each circuit queue */
}DEQUE;

/*
 * the constructure of circuit queue in DeQue
 * @param UINT uBlockSize
 * @param DEQUEBLOCK * -- the pointer to circuit queue
 */
DEQUEBLOCK *DeQueBlock_Create(UINT uBlockSize)
{
    DEQUEBLOCK *pBlock;
    pBlock = (DEQUEBLOCK *)malloc(sizeof(DEQUEBLOCK));
    if(NULL != pBlock)
    {
        pBlock->ppData = (void **)malloc(uBlockSize * sizeof(void *));
        if(NULL == pBlock->ppData)
        {
            free(pBlock);
            return NULL;
        }
        pBlock->uHead = 0;
        pBlock->uTail = 0;
    }
    return pBlock;
}

/*
 * DeQue's function for insert data in tail
 * @param DEQUE *pQue -- the pointer to DeQue
 * @param void *pData
 * @return INT
 */
INT DeQue_InsertTail(DEQUE *pQue, void *pData)
{
    DEQUEBLOCK *pBlock;
    UINT uTailNext;
    pBlock = pQue->pLast;
    if(pBlock->uTail == pQue->uBlockSize-1)
        uTailNext = 0;
    else
        uTailNext = pBlock->uTail+1;

    /* the queue is not full */
    if(pBlock->uHead != uTailNext)
    {
        pBlock->ppData[pBlock->uTail] = pData;
        pBlock->uTail = uTailNext;
    }
    else
    {
        DEQUEBLOCK *pNewBlock = DeQueBlock_Create(pQue->uBlockSize);
        if(pNewBlock == NULL)
            return CAPI_FAILED;

        if(pBlock->uMapPos >= pQue->uMapSize-1)
        {
            /* reallocate map */
            DEQUEBLOCK **ppMap = (DEQUEBLOCK **)malloc(pQue->uMapSize * 2 * sizeof(DEQUEBLOCK *));
            if(ppMap == NULL)
                return CAPI_FAILED;

            memcpy(ppMap,pQue->ppMap,pQue->uMapSize*sizeof(DEQUEBLOCK *));
            pQue->uMapSize *= 2;
            free(pQue->ppMap);
            pQue->ppMap = ppMap;
        }
        pNewBlock->uMapPos = pBlock->uMapPos + 1;
        pNewBlock->ppData[0] = pData;
        pNewBlock->uTail += 1;
        pQue->ppMap[pNewBlock->uMapPos] = pNewBlock;
        pQue->pLast = pNewBlock;
    }
    return CAPI_SUCCESS;
}

/*
 * pop head data function of DeQue
 * @param DEQUE *pQue
 * @return void *
 */
void *DeQue_PopHead(DEQUE *pQue)
{
    DEQUEBLOCK *pBlock;
    UINT uHead;
    pBlock = pQue->pFirst;
    uHead = pBlock->uHead;
    if(uHead != pBlock->uTail)
    {
        if(uHead == pQue->uBlockSize-1)
            pBlock->uHead = 0;
        else
            pBlock->uHead += 1;

        if(pBlock->uHead == pBlock->uTail)
        {
            if(pQue->pLast != pQue->pFirst)
            {
                UINT uMapPos = pBlock->uMapPos;
                DeQueBlock_Destroy(pQue,pBlock,NULL);
                pBlock = pQue->ppMap[uMapPos+1];
                pQue->pFirst = pBlock;
            }
        }
        return pBlock->ppData[uHead];
    }
    else
        return NULL;
}
