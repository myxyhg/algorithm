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