/*********************************************************************************
 * FileName:	OneWay_LinkedList.c
 * Author:		gehan
 * Date:		07/09/2017
 * Description: One-Way LinkedList
**********************************************************************************/

#include "algo.h"

/*
 * The structure of node in One-Way linkedlist
 */
typedef struct SINGLENODE_st {
	void   *pData;
	struct SINGLENODE_st *pNext;	/*next node's pointer*/
}SINGLENODE, *PSINGLENODE;

/*
 * The structure One-Way linkedlist
 */
typedef struct SINGLELIST_st {
	SINGLENODE	*pHead;
	SINGLENODE	*pTail;
	SINGLENODE	*pCur;
	UINT		uCount;		/*the counts in linkedlist*/
}SINGLELIST, *PSINGLELIST;

/*
 * The constucture of one-way linkedlist, it is NULL after create successfully
 * @param void
 * @return SINGLELIST *
 */
SINGLELIST * SingleList_Create(void)
{
	SINGLELIST *pSignleList;

	pSignleList = (SINGLELIST *)malloc(sizeof(SINGLELIST));
	if (NULL != pSignleList)
	{
		pSignleList->pCur = NULL;
		pSignleList->pHead = NULL;
		pSignleList->pTail = NULL;
		pSignleList->uCount = 0;
	}
	return pSignleList;
}

/*
 * The destructure of one-way linkedlist, it is NULL after create successfully
 * @param SINGLELIST *pSingleList
 * @param DESTROYFUNC DestroyFunc
 * @return void
 */
void SingleList_Destroy(SINGLELIST *pSingleList, DESTROYFUNC DestroyFunc)
{
	SINGLENODE *pNode;
	if (pSingleList)
	{
		/*From the start node, release each node in linkedlist*/
		pNode = pSingleList->pHead;
		while (NULL != pNode)
		{
			SINGLENODE *pDelNode;
			pDelNode = pNode;
			pNode = pNode->pNext;
			if (NULL != DestroyFunc && NULL != pDelNode->pData)
			{
				(*DestroyFunc)(pDelNode->pData);	/*relase data in node*/
			}
			free(pDelNode);		/*release node*/
		}
		free(pSingleList);
	}
}

/*
 * Insert data into linkedlist's HEAD
 * @param SINGLELIST *pSingleList
 * @param void *pData
 * @return INT
 */
INT SingleList_InsertHead(SINGLELIST *pSingleList, void *pData)
{
	SINGLENODE *pNode;
	if (NULL == pSingleList || NULL == pData)
	{
		return CAPI_FAILED;
	}

	/*Create a new node*/
	pNode = (SINGLENODE *)malloc(sizeof(SINGLENODE));
	if (NULL == pNode)
	{
		return CAPI_FAILED;
	}

	pNode->pData = pData;

	pNode->pNext = pSingleList->pHead;
	pSingleList->pHead = pNode;

	/*
	 * To check whether the tail node is NULL or not, if it is NULL that means there
	 * is no node in original Linkedlist, and now the tail node should pointer to 
	 * the new added node
	 */
	if (NULL == pSingleList->pTail)
	{
		pSingleList->pTail = pNode;
	}

	pSingleList->uCount += 1;
	return CAPI_SUCCESS;
}

/*
 * Insert data into linkedlist's TAIL
 * @param SINGLELIST *pSingleList
 * @param void *pData
 * @return INT
 */
INT SingleList_InsertTail(SINGLELIST *pSingleList, void *pData)
{
	SINGLENODE *pNode;
	if (NULL == pSingleList || NULL == pData)
	{
		return CAPI_FAILED;
	}

	/*Create a new node*/
	pNode = (SINGLENODE *)malloc(sizeof(SINGLENODE));
	if (NULL == pNode)
	{
		return CAPI_FAILED;
	}

	pNode->pData = pData;
	pNode->pNext = NULL;

	/*
	* To check whether the tail node is NULL or not, if it is NULL that means there
	* is no node in original Linkedlist, and now the tail node and head node both 
	* should pointer to the new added node
	*/
	if (NULL == pSingleList->pTail)
	{
		pSingleList->pTail = pNode;
		pSingleList->pHead = pNode;
	}
	else
	{
		pSingleList->pTail->pNext = pNode;
		pSingleList->pTail = pNode;
	}

	pSingleList->uCount += 1;
	return CAPI_SUCCESS;
}

/*
 * Pop data in linkedlist's HEAD node
 * @param SINGLELIST *pSingleList
 * @return void *
 */
void * SingleList_PopHead(SINGLELIST *pSingleList)
{
	SINGLENODE *pPopNode;
	void *pPopData;
	if (NULL == pSingleList || NULL == pSingleList->pHead)
	{
		return NULL;
	}

	pPopNode = pSingleList->pHead;
	pPopData = pPopNode->pData;

	if (pSingleList->pCur == pSingleList->pHead)
	{
		pSingleList->pCur = pSingleList->pHead->pNext;
	}

	pSingleList->pHead = pSingleList->pHead->pNext;
	pSingleList->uCount -= 1;

	if (pSingleList->uCount == 0)
	{
		pSingleList->pTail = NULL;
	}

	free(pPopNode);
	return pPopData;
}

/*
 * Pop data in linkedlist's TAIL node, its efficiency is very low due to it need
 * find the tail node's last node
 * @param SINGLELIST *pSingleList
 * @return void *
 */
void * SingleList_PopTail(SINGLELIST *pSingleList)
{
	SINGLENODE *pPopNode;
	SINGLENODE *pTailPrevNode;	/*to point tail node's prev node*/
	void *pPopData;
	if (NULL == pSingleList || NULL == pSingleList->pHead)
	{
		return NULL;
	}

	pPopNode = pSingleList->pTail;
	pPopData = pPopNode->pData;
	pTailPrevNode = pSingleList->pHead;

	if (pSingleList->pTail == pSingleList->pHead)
	{
		pTailPrevNode = NULL;
		pSingleList->pHead = NULL;
	}
	else
	{
		while (NULL != pTailPrevNode)
		{
			if (pTailPrevNode->pNext == pSingleList->pTail)
			{
				break;
			}
			pTailPrevNode = pTailPrevNode->pNext;
		}
	}

	if (pSingleList->pCur == pSingleList->pTail)
	{
		pSingleList->pCur = pTailPrevNode;
	}

	pSingleList->pTail = pTailPrevNode;
	pSingleList->uCount -= 1;

	if (NULL != pTailPrevNode)
	{
		pTailPrevNode->pNext = NULL;
	}

	pSingleList->uCount -= 1;
	free(pPopNode);
	return pPopData;
}

/*
 * Delete a node which data is same with parameter pMatchData. If there are many
 * nodes have same value, it just delete the first node
 * @param SINGLELIST *pSingleList
 * @param void *pMatchData
 * @param COMPAREFUNC CompareFunc
 * @param DESTROYFUNC DestroyFunc
 * @return INT(by default) -- return CAPI_FAILED if delete fail or there is no match node
						   -- return CAPI_SUCCESS means delete successfully
 */
INT SingleList_Delete(SINGLELIST *pSingleList, void *pMatchData,
	COMPAREFUNC CompareFunc, DESTROYFUNC DestroyFunc)
{
	SINGLENODE *pNode;
	SINGLENODE *pPrevNode;
	if (NULL == pSingleList || NULL == CompareFunc)
	{
		return CAPI_FAILED;
	}

	pNode = pSingleList->pHead;
	pPrevNode = pNode;
	while (NULL != pNode)
	{
		if (0 == (*CompareFunc)(pNode->pData, pMatchData))
		{
			if (pPrevNode == pNode)
			{
				/*the head node match, so need to delete head node*/
				pSingleList->pHead = pNode->pNext;
				if (pSingleList->pTail == pNode)
				{
					pSingleList->pTail = NULL;
					pSingleList->pCur = NULL;
				}
			}
			else
			{
				pPrevNode->pNext = pNode->pNext;
				if (pSingleList->pTail == pNode)
				{
					/*if tail node is same with pNode, that means it delete tail node*/
					pSingleList->pTail = pPrevNode;
				}

				if (pSingleList->pCur == pNode)
				{
					pSingleList->pCur = pNode->pNext;
				}
			}

			/*destroy node and release its data*/
			if (NULL != DestroyFunc && NULL != pNode->pData)
			{
				(*DestroyFunc)(pNode->pData);
			}
			free(pNode);
			break;
		}
		else
		{
			pPrevNode = pNode;
			pNode = pNode->pNext;
		}
	}
	return CAPI_SUCCESS;
}

/*
 * Get specify location's data
 * @param SINGLELIST *pSingleList
 * @param UINT uIndex
 * @return void *
 */
void * SingleList_GetAt(SINGLELIST *pSingleList, UINT uIndex)
{
	UINT i;
	SINGLENODE *pNode;
	if (NULL == pSingleList || pSingleList->uCount >= uIndex)
	{
		return NULL;
	}
	pNode = pSingleList->pHead;
	for (i = 0; i < uIndex; ++i)
	{
		pNode = pNode->pNext;
	}
	return pNode->pData;
}

/*
* Get node's count in One-Way linkedlist
* @param SINGLELIST *pSingleList
* @return UINT
*/
UINT SingleList_GetCount(SINGLELIST *pSingleList)
{
	if (NULL == pSingleList)
	{
		return 0;
	}
	else
	{
		return pSingleList->uCount;
	}
}

/*
 * Get head node in One-Way linkedlist
 * @param SINGLELIST *pSingleList
 * @return void *
 */
void * SingleList_GetHead(SINGLELIST *pSingleList)
{
	if (NULL == pSingleList)
	{
		return NULL;
	}
	else if (NULL == pSingleList->pHead)
	{
		return NULL;
	}
	else
	{
		return pSingleList->pHead->pData;
	}
}

/*
 * Get tail node in One-Way linkedlist
 * @param SINGLELIST *pSingleList
 * @return void *
 */
void * SingleList_GetTail(SINGLELIST *pSingleList)
{
	if (NULL != pSingleList->pTail)
	{
		return pSingleList->pTail->pData;
	}
	else
	{
		return pSingleList->pHead->pData;
	}
}

/*
 * The enum init function of One-Way linkedlist
 * @param SINGLELIST *pSingleList
 * @return void
 */
void SingleList_EnumBegin(SINGLELIST *pSingleList)
{
	pSingleList->pCur = pSingleList->pHead;
	return;
}

/*
 * To enum next node of One-Way linkedlist, it must invoke SingleList_EnumBegin()
 * function before invoke this function first
 * @param SINGLELIST *pSingleList
 * @return void *
 */
void * SingleList_EnumNext(SINGLELIST *pSingleList)
{
	SINGLENODE *pCur;
	pCur = pSingleList->pCur;
	if (NULL != pCur)
	{
		pSingleList->pCur = pCur->pNext;
		return pCur->pData;
	}
	return NULL;
}