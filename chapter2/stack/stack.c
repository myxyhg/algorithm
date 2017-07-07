#include "algo.h"

typedef struct STACK_st{
    void **ppBase;  /* the array of data */
    UINT uTop;
    unsigned uStackSize;
}STACK;

/*
 * create a stack;
 * @param UINT uStackSize
 * @return STACK * if successfully or return NULL if fail
 */
STACK *Stack_Create(UINT uStackSize)
{
    STACK *pStack;
    if(uStackSize == 0)
        return NULL;
    pStack = (STACK *)malloc(sizeof(struct STACK_st));
    if(pStack != NULL)
    {
        pStack->ppBase = (void **)malloc(uStackSize * sizeof(void *));
        if(pStack->ppBase == NULL)
        {
            free(pStack);
            pStack = NULL;
        }
        else
        {
            pStack->ppBase[0] = NULL;
            pStack->uTop = 0;
            pStack->uStackSize = uStackSize;
        }
    }
    return pStack;
}

/*
 * destroy stack and will free all data
 * @param STACK *pStack -- stack pointer
 * @param DESTROYFUNC DestroyFunc -- the callback function for free data
 * @return void
 */
void Stack_Destroy(STACK *pStack,DESTROYFUNC DestroyFunc)
{
    if(pStack != NULL)
    {
        if(pStack->ppBase != NULL && DestroyFunc != NULL)
        {
            UINT i;
            for(i = 0;i < pStack->uTop;i++)
            {
                if(pStack->ppBase[i] != NULL)
                {
                    (*DestroyFunc)(pStack->ppBase[i]);
                }
            }
            free(pStack->ppBase);
        }
    }
}

/*
 * pop top data in stack
 * @param STACK *pStack
 * @return void * -- return top stack data successfully or return NULL
 */
void *Stack_Pop(STACK *pStack)
{
    void *pData;
    if(pStack == NULL || pStack->uTop == 0)
        return NULL;
    pStack->uTop -= 1;
    pData = pStack->ppBase[pStack->uTop];
    return pData;
}

/*
 * push data into stack, and the data can be NULL
 * @param STACK *pStack
 * @param void *pData
 * @return INT -- return CAPI_SUCCESS or CAPI_FAILED
 */
INT Stack_Push(STACK *pStack, void *pData)
{
    if(pStack == NULL)
        return CAPI_FAILED;

    /* double size stack if full */
    if(pStack->uTop == pStack->uStackSize - 1)
    {
        pStack->ppBase = (void **)realloc(pStack->ppBase,
                (pStack->uStackSize * 2) * sizeof(void *));
        if(pStack->ppBase == NULL)
            return CAPI_FAILED;
        pStack->uStackSize *= 2;
    }
    pStack->ppBase[pStack->uTop] = pData;
    pStack->uTop += 1;
    return CAPI_SUCCESS;
}

/*
 * check stack is empty or not
 * @param STACK *pStack
 * @return INT -- 0 is empty
 */
INT Stack_IsEmpty(STACK *pStack)
{
    return pStack->uTop;
}
