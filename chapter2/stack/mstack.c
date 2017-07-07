#include "algo.h"

typedef struct STACK_st{
    void **ppBase;
    UINT uTop;
    unsigned int uStackSize;
}STACK;

typedef struct MSTACK_st{
    STACK *pStack;
    LOCK pLock;
}MSTACK;

/*
 * create multi-tasks stack function
 * @param UINT uStackSize
 * @return MSTACK *
 */
MSTACK *MSTACK_Create(UINT uStackSize)
{
    MSTACK *pMStack = (MSTACK *)malloc(sizeof(MSTACK));
    if(pMStack != NULL)
    {
        pMStack->pStack = Stack_Create(uStackSize);
        if(pMStack->pStack != NULL)
        {
            /* create lock */
            pMStack->pLock = LockCreate();
            if(pMStack->pLock != NULL)
                return pMStack;
            free(pMStack->pLock);
        }
        free(pMStack);
    }
    return NULL;
}

/*
 * free function of multi-tasks stack
 * @param MSTACK *pMStack
 * @param DESTROYFUNC DestroyFunc
 */
void MStack_Destroy(MSTACK *pMStack,DESTROYFUNC DestroyFunc)
{
    if(pMStack != NULL)
    {
        Lock(pMStack->pLock);
        Stack_Destroy(pMStack->pStack,DestroyFunc);
        LockClose(pMStack->pLock);
        free(pMStack);
    }
}

/*
 * pop function of multi-tasks stack
 * @param MSTACK *pMStack
 * @return void *
 */
void *MStack_Pop(MSTACK *pMStack)
{
    void *pData;
    Lock(pMStack->pLock);
    pData = Stack_Pop(pMStack->pStack);
    Unlock(pMStack->pLock);
    return pData;
}

/*
 * @param MSTACK *pMStack
 * @param void *pData
 * @return INT
 */
INT MStack_Push(MSTACK *pMStack,void *pData)
{
    INT nRet;
    Lock(pMStack->pLock);
    nRet = Stack_Push(pMStack->pStack,pData);
    Unlock(pMStack->pLock);
    return nRet;
}

/*
 * check stack is empty or not
 * @param MSTACK *pMStack
 * @return INT -- 0 means empty, 1 means non-empty
 */
INT MStack_IsEmpty(MSTACK *pMStack)
{
    INT nRet;
    Lock(pMStack->pLock);
    nRet = Stack_IsEmpty(pMStack->pStack);
    Unlock(pMStack->pLock);
    return nRet;
}
