/*********************************************************************************
* FileName:		BlockList.c
* Author:		gehan
* Date:			07/09/2017
* Description:	The implementation of BlockList
**********************************************************************************/

#include "algo.h"
#include "OneWay_LinkedList.c"
#include <stdlib.h>

typedef struct BLOCKLIST_st {
	void *pBlock;		/*the pointer of block*/
	SINGLENODE *pEmpty;	/*Free space's pointer*/
	SINGLENODE *pHead;	/*linkedlist's header pointer*/
	UINT uDataSize;
	UINT uMaxDataCount;
	UINT uFreeCount;
}BLOCKLIST;

/*
 * The constucture of BlockList
 * @param UINT uDataSize
 * @param UINT uMaxDataSize
 * @return BLOCKLIST *
 */
BLOCKLIST * BlockList_Create(UINT uDataSize, UINT uMaxDataCount)
{
	BLOCKLIST *pList;
	SINGLENODE *pNode;
	UINT i;
	pList = (BLOCKLIST *)malloc(sizeof(BLOCKLIST) + uMaxDataCount * (uDataSize + sizeof(SINGLENODE)));
	if (NULL != pList)
	{
		pList->pBlock = (void *)((char *)pList + sizeof(BLOCKLIST));
		/*Create a new empty list*/
		pList->pEmpty = (SINGLENODE *)pList->pBlock;
		pNode = pList->pEmpty;
		for (i = 0; i < uMaxDataCount - 1; ++i)
		{
			pNode->pNext = (SINGLENODE *)((char *)pNode + sizeof(SINGLENODE) + uDataSize);
			pNode = pNode->pNext;
		}
		pNode->pNext = NULL;
		pList->uFreeCount = uMaxDataCount;
		pList->uDataSize = uDataSize;
		pList->uMaxDataCount = uMaxDataCount;
		pList->pHead = NULL;
	}
	return pList;
}

/*
 * The destructure of BlockList
 * @param BLOCKLIST *pList
 * @return void
 */
void BlockList_Destroy(BLOCKLIST *pList)
{
	if (NULL != pList)
	{
		free(pList);
	}
}

/*
 * To insert node into BlockList's head
 * @param BLOCKLIST *pList
 * @param void *pData
 * @param UINT uDataLen -- length of insert datas
 * @return INT
 */
INT BlockList_InsertHead(BLOCKLIST *pList, void *pData, UINT uDataLen)
{
	SINGLENODE *pNode;
	if (uDataLen > pList->uDataSize)
	{
		return CAPI_FAILED;
	}
	pNode = pList->pEmpty;
	if (NULL != pNode)
	{
		pList->pEmpty = pNode->pNext;
	}
	else
	{
		return CAPI_FAILED;
	}

	pNode->pNext = pList->pHead;
	memcpy(pNode->pData, pData, uDataLen);
	pList->pHead = pNode;
	pList->uFreeCount -= 1;
	return CAPI_SUCCESS;
}

/*
 * To delete node into BlockList's head
 * @param BLOCKLIST *pList
 * @return void
 */
void BlockList_DeleteHead(BLOCKLIST *pList)
{
	SINGLENODE *pPopNode;
	if (NULL == pList || NULL == pList->pHead)
	{
		return;
	}

	/*Pop linkedlist's header node*/
	pPopNode = pList->pHead;
	pList->pHead = pList->pHead->pNext;

	/*add pop node into empty list's head*/
	pPopNode->pNext = pList->pEmpty;
	pList->pEmpty = pPopNode;

	pList->uFreeCount += 1;
	return;
}

/*
 * The alloc function of BlockList, pop a head from empty list and let it as alloc memory
 * @param BLOCKLIST *pList
 * @return void *
 */
void * BlockList_Alloc(BLOCKLIST *pList)
{
	SINGLENODE *pNode;
	pNode = pList->pEmpty;
	if (NULL != pNode)
	{
		pList->pEmpty = pNode->pNext;
		pList->uFreeCount -= 1;
		return (void *)pNode;
	}
	return NULL;
}

/*
 * The free function of BlockList, free alloc memory and add into empty list
 * @param BLOCKLIST *pList
 * @param void *pData -- the data pointer be free
 * @return void
 */
void BlockList_Free(BLOCKLIST *pList, void *pData)
{
	SINGLENODE *pNode;
	pNode = (SINGLENODE *)pData;
	pNode->pNext = pList->pEmpty;
	pList->pEmpty = pNode;
	pList->uFreeCount += 1;
}