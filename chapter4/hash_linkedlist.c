/*********************************************************************************
* FileName:		hash_linkedlist.c
* Author:		gehan
* Date:			07/13/2017
* Description:	The implementation of hash table with linkedlist
**********************************************************************************/

#include "algo.h"

#define MINIUM_BUCKET_COUNT 0x100

typedef struct HASHLISTNODE_st {
	struct HASHLISTNODE_st *pListNext;
	struct HASHLISTNODE_st *pListPrev;
	struct HASHLISTNODE_st *pBucketNext;	/*the linkedlist's pointer of hash table*/
	void *pData;
}HASHLISTNODE;

typedef struct HASHLIST_st {
	HASHLISTNODE **ppBuckets;
	UINT uBucketCount;
	HASHLISTNODE *pHead;	/*the linkedlist's head pointer*/
	HASHLISTNODE *pTail;
	UINT uNodeCount;		/*the node number of hash table with linkedlist*/
}HASHLIST;

/*
 * The create function of hash list
 * @param UINT uBucketCount
 * @return HASHLIST *
 */
HASHLIST * HashList_Create(UINT uBucketCount)
{
	HASHLIST *pHashList;
	if (uBucketCount < MINIUM_BUCKET_COUNT)
	{
		uBucketCount = MINIUM_BUCKET_COUNT;
	}
	pHashList = (HASHLIST *)malloc(sizeof(HASHLIST));
	if (NULL != pHashList)
	{
		/*Create hash table's index list in Hash linkedlist*/
		pHashList->ppBuckets = (HASHLISTNODE **)malloc(uBucketCount * sizeof(HASHLISTNODE *));
		if (NULL == pHashList->ppBuckets)
		{
			free(pHashList);
			return NULL;
		}

		memset((void *)pHashList->ppBuckets, 0, uBucketCount * sizeof(HASHLISTNODE *));
		/*Init double-way linkedlist in hash table*/
		pHashList->pHead = NULL;
		pHashList->pTail = NULL;
		pHashList->uBucketCount = uBucketCount;
		pHashList->uNodeCount = 0;
	}
	return pHashList;
}

/*
 * The destroy function of hash list
 * @param HASHLIST *pHashList
 * @param DESTROYFUNC DestroyFunc
 * @return void
 */
void HashList_Destroy(HASHLIST *pHashList, DESTROYFUNC DestroyFunc)
{
	UINT uIndex;
	if (NULL == pHashList)
	{
		return;
	}

	for (uIndex = 0; uIndex < pHashList->uBucketCount; ++uIndex)
	{
		HASHLISTNODE *pNode;
		pNode = pHashList->ppBuckets[uIndex];
		while (NULL != pNode)
		{
			HASHLISTNODE *pNodeToFree;
			pNodeToFree = pNode;
			pNode = pNode->pBucketNext;
			if (NULL != DestroyFunc && NULL != pNodeToFree->pData)
			{
				(*DestroyFunc)(pNodeToFree->pData);
			}
			free(pNodeToFree);
		}
	}
	free(pHashList->ppBuckets);
	free(pHashList);
	return;
}

/*
 * To insert node into hash list, and must be in head of linked list
 * @param HASHLIST *pHashList
 * @param void *pData
 * @param HASHFUNC HashFunc
 * @return INT
 */
INT HashList_InsertHead(HASHLIST *pHashList, void *pData, HASHFUNC HashFunc)
{
	HASHLISTNODE *pNode;
	UINT uBucketIndex;
	if (NULL == pHashList || NULL == pData)
	{
		return CAPI_FAILED;
	}

	pNode = (HASHLISTNODE *)malloc(sizeof(HASHLISTNODE));
	if (NULL == pNode)
	{
		return CAPI_FAILED;
	}
	pNode->pData = pData;
	pNode->pBucketNext = NULL;
	pNode->pListPrev = NULL;
	pNode->pListNext = pHashList->pHead;

	/*Insert into hash list*/
	uBucketIndex = (*HashFunc)(pData, pHashList->uBucketCount);
	if (NULL == pHashList->ppBuckets[uBucketIndex])
	{
		pHashList->ppBuckets[uBucketIndex] = pNode;
	}
	else
	{
		HASHLISTNODE *pTempNode;
		pTempNode = pHashList->ppBuckets[uBucketIndex];
		while (NULL != pTempNode->pBucketNext)
		{
			pTempNode = pTempNode->pBucketNext;
		}
		pTempNode->pBucketNext = pNode;
	}

	/*Insert into linkedlist*/
	if (NULL == pHashList->pHead)
	{
		pHashList->pHead = pNode;
		pHashList->pTail = pNode;
	}
	else
	{
		pNode->pListNext = pHashList->pHead;
		pHashList->pHead->pListPrev = pNode;
		pHashList->pHead = pNode;
	}
	pHashList->uNodeCount += 1;
	return CAPI_SUCCESS;
}

/*
 * To delete node in hash list, delete it from hash table and linkedlist at same time
 * @param HASHLIST *pHashList
 * @param void *pData
 * @param HASHFUNC HashFunc
 * @param COMPAREFUNC CompareFunc
 * @param DESTROYFUNC DestroyFunc
 * @return INT
 */
INT HashList_Delete(HASHLIST *pHashList, void *pData, HASHFUNC HashFunc,
	COMPAREFUNC CompareFunc, DESTROYFUNC DestroyFunc)
{
	HASHLISTNODE *pNode, *pPrevNode;
	UINT uIndex;
	if (NULL == pHashList || NULL == HashFunc || NULL == CompareFunc)
	{
		return CAPI_FAILED;
	}

	uIndex = (*HashFunc)(pData, pHashList->uBucketCount);
	pNode = pHashList->ppBuckets[uIndex];
	pPrevNode = NULL;
	while (NULL != pNode)
	{
		if (0 == (*CompareFunc)(pNode->pData, pData))
		{
			if (NULL == pPrevNode)
			{
				pHashList->ppBuckets[uIndex] = pNode->pBucketNext;
			}
			else
			{
				pPrevNode->pBucketNext = pNode->pBucketNext;
			}

			/*Delete node from linked list*/
			if (NULL != pNode->pListPrev)
			{
				pNode->pListPrev->pListNext = pNode->pListNext;
			}
			else
			{
				/*pNode is the head of linked list*/
				pHashList->pHead = pNode->pListNext;
			}

			if (NULL != pNode->pListNext)
			{
				pNode->pListNext->pListPrev = pNode->pListPrev;
			}
			else
			{
				/*Now at tail of linked list*/
				pHashList->pTail = pNode;
			}

			if (NULL != pNode->pData && NULL != DestroyFunc)
			{
				(*DestroyFunc)(pNode->pData);
			}
			free(pNode);
			pHashList->uNodeCount -= 1;
			return CAPI_SUCCESS;
		}
		else
		{
			pPrevNode = pNode;
			pNode = pNode->pBucketNext;
		}
	}
	return CAPI_FAILED;
}

/*
 * To find node in hash list
 * @param HASHLIST *pHashList
 * @param void *pData
 * @param HASHFUNC HashFunc
 * @param COMPAREFUNC CompareFunc
 * @return HASHLISTNODE *
 */
HASHLISTNODE * HashList_FindNode(HASHLIST *pHashList, void *pData,
	HASHFUNC HashFunc, COMPAREFUNC CompareFunc)
{
	HASHLISTNODE *pNode;
	UINT uIndex;
	if (NULL == pHashList || NULL == HashFunc || NULL == CompareFunc)
	{
		return NULL;
	}

	uIndex = (*HashFunc)(pData, pHashList->uBucketCount);
	pNode = pHashList->ppBuckets[uIndex];
	while (NULL != pNode)
	{
		if (0 == (*CompareFunc)(pNode->pData, pData))
		{
			return pNode;
		}
		else
		{
			pNode = pNode->pBucketNext;
		}
	}
	return NULL;
}

/*
 * To find data in hash list
 * @param HASHLIST *pHashList
 * @param void *pData
 * @param HASHFUNC HashFunc
 * @param COMPAREFUNC CompareFunc
 * @return void *
 */
HASHLISTNODE * HashList_FindData(HASHLIST *pHashList, void *pData,
	HASHFUNC HashFunc, COMPAREFUNC CompareFunc)
{
	HASHLISTNODE *pNode;
	UINT uIndex;
	if (NULL == pHashList || NULL == HashFunc || NULL == CompareFunc)
	{
		return NULL;
	}

	uIndex = (*HashFunc)(pData, pHashList->uBucketCount);
	pNode = pHashList->ppBuckets[uIndex];
	while (NULL != pNode)
	{
		if (0 == (*CompareFunc)(pNode->pData, pData))
		{
			return pNode->pData;
		}
		else
		{
			pNode = pNode->pBucketNext;
		}
	}
	return NULL;
}

/*
 * The insert sort function for hash list
 * @param HASHLIST *pHashList
 * @param COMPAREFUNC CompareFunc
 * @return INT
 */
INT HashList_InsertSort(HASHLIST *pHashList, COMPAREFUNC CompareFunc)
{
	HASHLISTNODE *pNode;
	if (NULL == pHashList || NULL == CompareFunc)
	{
		return CAPI_FAILED;
	}

	pNode = pHashList->pHead;
	if (NULL == pNode)
	{
		/*No node in hash list, it already sorted*/
		return CAPI_SUCCESS;
	}

	while (NULL != pNode->pListNext)
	{
		if ((*CompareFunc)(pNode->pListNext->pData, pNode->pData) < 0)
		{
			HASHLISTNODE *pTempNode;
			pTempNode = pNode->pListPrev;
			while (NULL != pTempNode)
			{
				if ((*CompareFunc)(pNode->pListNext->pData, pTempNode->pData) >= 0)
				{
					HASHLISTNODE *pCurNode;
					pCurNode = pNode->pListNext;
					pNode->pListNext = pNode->pListNext->pListNext;
					if (NULL != pCurNode->pListNext)
					{
						pCurNode->pListNext->pListPrev = pNode;
					}
					pCurNode->pListNext = pTempNode->pListNext;
					pCurNode->pListPrev = pTempNode;
					pTempNode->pListNext->pListPrev = pCurNode;
					pTempNode->pListNext = pCurNode;
					break;
				}
				else
				{
					pTempNode = pTempNode->pListPrev;
				}

				/*If all data is greater than this node's data, then insert it to head*/
				if (NULL == pTempNode)
				{
					HASHLISTNODE *pCurNode;
					pCurNode = pNode->pListNext;
					pNode->pListNext = pNode->pListNext->pListNext;
					if (NULL != pCurNode->pListNext)
					{
						pCurNode->pListNext->pListPrev = pNode;
					}
					/*Insert node into head*/
					pCurNode->pListPrev = NULL;
					pCurNode->pListNext = pHashList->pHead;
					pHashList->pHead = pCurNode;
				}
			}
		}
		else
		{
			pNode = pNode->pListNext;
		}
	}
	return CAPI_SUCCESS;
}