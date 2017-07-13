/*********************************************************************************
* FileName:		hashtable.c
* Author:		gehan
* Date:			07/13/2017
* Description:	Functions for hash table
**********************************************************************************/

#include "algo.h"
#include "OneWay_LinkedList.c"

/*
 * The structure of Hash table with linkedlist
 */
typedef struct HASHTABLE
{
	SINGLENODE **ppBucket;		/*the pointer of Index table*/
	UINT uBucketCount;
	UINT uNodeCount;			/*The actual number of node in table*/
	UINT uCurBucketNo;
	SINGLENODE *pCurEntry;
}HASHTABLE;

/*
 * The create function of hash table
 * @param UINT uBucketCount
 * @return HASHTABLE *
 */
HASHTABLE * HashTable_Create(UINT uBucketCount)
{
	HASHTABLE *pTable;
	if (0 == uBucketCount)
	{
		return NULL;
	}
	
	pTable = (HASHTABLE *)malloc(sizeof(HASHTABLE));
	if (NULL == pTable)
	{
		return NULL;
	}
	pTable->uNodeCount = 0;
	pTable->uBucketCount = uBucketCount;

	pTable->ppBucket = (SINGLENODE *)malloc(uBucketCount * sizeof(SINGLENODE *));
	if (NULL == pTable->ppBucket)
	{
		free(pTable);
		return NULL;
	}

	memset(pTable->ppBucket, 0, uBucketCount * sizeof(SINGLENODE *));
	pTable->pCurEntry = NULL;
	pTable->uCurBucketNo = 0;
	return pTable;
}

/*
 * The destroy function of hash table
 * @param HASHTABLE *pTable
 * @param DESTROYFUNC DestroyFunc
 * @return void
 */
void HashTable_Destroy(HASHTABLE *pTable, DESTROYFUNC DestroyFunc)
{
	SINGLENODE **ppBucket;
	SINGLENODE *pNode;
	SINGLENODE *pFreeNode;
	UINT i;
	if (NULL == pTable || NULL == DestroyFunc)
	{
		return;
	}

	ppBucket = pTable->ppBucket;
	for (i = 0; i < pTable->uBucketCount; ++i)
	{
		pNode = ppBucket[i];
		while (NULL != pNode)
		{
			(*DestroyFunc)(pNode->pData);
			pFreeNode = pNode;
			pNode = pNode->pNext;
			free(pFreeNode);
		}
	}
	free(ppBucket);
	pTable->ppBucket = NULL;
	free(pTable);
}

/*
 * The insert function of hash table
 * @param HASHTABLE *pTable
 * @param void *pData
 * @param HASHFUNC HashFunc
 * @return INT
 */
INT HashTable_Insert(HASHTABLE *pTable, void *pData, HASHFUNC HashFunc)
{
	UINT uIndex;
	SINGLENODE *pNode, *pNewNode;
	if (NULL == pTable || NULL == pData || NULL == HashFunc)
	{
		return CAPI_FAILED;
	}

	uIndex = (*HashFunc)(pData, pTable->uBucketCount);
	pNode = (pTable->ppBucket)[uIndex];
	pNewNode = (SINGLENODE *)malloc(sizeof(SINGLENODE));
	if (NULL == pNewNode)
	{
		return CAPI_FAILED;
	}

	/*Insert new node to list's head*/
	pNewNode->pData = pData;
	pNewNode->pNext = pNode;
	(pTable->ppBucket)[uIndex] = pNewNode;
	pTable->uNodeCount += 1;
	return CAPI_SUCCESS;
}

/*
 * The find function of hash table
 * @param HASHTABLE *pTable
 * @param void *pData
 * @param HASHFUNC HashFunc
 * @param COMPAREFUNC CompareFunc
 * @return void *
 */
void * HashTable_Find(HASHTABLE *pTable, void *pData, HASHFUNC HashFunc, COMPAREFUNC CompareFunc)
{
	UINT uIndex;
	SINGLENODE *pNode;
	if (NULL == pTable || NULL == pData || NULL == HashFunc || NULL == CompareFunc)
	{
		return NULL;
	}

	uIndex = (*HashFunc)(pData, pTable->uBucketCount);
	pNode = (pTable->ppBucket)[uIndex];

	/*Search in hash table*/
	while (NULL != pNode)
	{
		if (0 == (*CompareFunc)(pNode->pData, pData))
		{
			return pNode->pData;
		}
		pNode = pNode->pNext;
	}
	return NULL;
}

/*
 * The delete function of hash table
 * @param HASHTABLE *pTable
 * @param void *pData
 * @param HASHFUNC HashFunc
 * @param COMPAREFUNC CompareFunc
 * @param DESTROYFUNC DataDestroyFunc
 * @return INT
 */
INT HashTable_Delete(HASHTABLE *pTable, void *pData, HASHFUNC HashFunc,
	COMPAREFUNC CompareFunc, DESTROYFUNC DataDestroyFunc)
{
	UINT uIndex;
	SINGLENODE *pNode, *pPrevNode;
	if (NULL == pTable || NULL == pData || NULL == HashFunc || NULL == CompareFunc)
	{
		return CAPI_FAILED;
	}

	uIndex = (*HashFunc)(pData, pTable->uBucketCount);
	pNode = (pTable->ppBucket)[uIndex];
	pPrevNode = pNode;

	while (NULL != pNode)
	{
		if (0 == (*CompareFunc)(pNode->pData, pData))
		{
			if (pPrevNode == pNode)
			{
				pTable->ppBucket[uIndex] = pNode->pNext;
			}
			else
			{
				pPrevNode->pNext = pNode->pNext;
			}

			/*delete corresponding node*/
			if (NULL != DataDestroyFunc)
			{
				(*DataDestroyFunc)(pNode->pData);
			}
			free(pNode);
			pTable->uNodeCount -= 1;
			return CAPI_SUCCESS;
		}
		pPrevNode = pNode;
		pNode = pNode->pNext;
	}
	return CAPI_SUCCESS;
}

/*
 * The enum init function of hash table
 * @param HASHTABLE *pTable
 * @return void
 */
void HashTable_EnumBegin(HASHTABLE *pTable)
{
	pTable->uCurBucketNo = 0;
	pTable->pCurEntry = pTable->ppBucket[0];
}

/*
 * To enum next node of hash table
 * @param HASHTABLE *pTable
 * @return void *
 */
void * HashTable_EnumNext(HASHTABLE *pTable)
{
	void *pData;
	while (NULL == pTable->pCurEntry)
	{
		pTable->uCurBucketNo += 1;
		if (pTable->uCurBucketNo >= pTable->uBucketCount)
		{
			return NULL;
		}
		pTable->pCurEntry = pTable->ppBucket[pTable->uCurBucketNo];
	}
	pData = pTable->pCurEntry->pData;
	pTable->pCurEntry = pTable->pCurEntry->pNext;
	return pData;
}