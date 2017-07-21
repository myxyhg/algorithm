/*********************************************************************************
 * FileName:	DoubleWay_LinkedList.c
 * Author:		gehan
 * Date:		07/09/2017
 * Description:	DoubleWay LinkedList
**********************************************************************************/

#include "algo.h"

/*
* The structure of node in Double-Way linkedlist
*/
typedef struct DOUBLENODE_st {
	void   *pData;
	struct DOUBLENODE_st *pNext;	/*next node's pointer*/
	struct DOUBLENODE_st *pPrev;
}DOUBLENODE;

/*
* The structure Double-Way linkedlist
*/
typedef struct DOUBLELIST_st {
	DOUBLENODE	*pHead;
	DOUBLENODE	*pTail;
	DOUBLENODE	*pCur;
	UINT		uCount;		/*the counts in linkedlist*/
}DOUBLELIST;

/*
 * The constucture of double-way linkedlist, it is NULL after create successfully
 * @param void
 * @return DOUBLELIST *
 */
DOUBLELIST * DoubleList_Create(void)
{
	DOUBLELIST *pDoubleList;
	pDoubleList = (DOUBLELIST *)malloc(sizeof(struct DOUBLELIST_st));
	if (NULL != pDoubleList)
	{
		pDoubleList->pHead = NULL;
		pDoubleList->pTail = NULL;
		pDoubleList->pCur = NULL;
		pDoubleList->uCount = 0;
	}
	return pDoubleList;
}

/*
 * The destructure of doule-way linkedlist, it is NULL after create successfully
 * @param DOUBLELIST *pDoubleList
 * @param DESTROYFUNC DestroyFunc
 * @return void
 */
void DoubleList_Destroy(DOUBLELIST *pDoubleList, DESTROYFUNC DestroyFunc)
{
	DOUBLENODE *pNode;
	if (pDoubleList)
	{
		pNode = pDoubleList->pHead;
		while (NULL != pNode)
		{
			DOUBLENODE *pDelNode;
			pDelNode = pNode;
			pNode = pNode->pNext;
			if (NULL != DestroyFunc && NULL != pDelNode->pData)
			{
				(*DestroyFunc)(pDelNode->pData);
			}
			free(pDelNode);
		}
		free(pDoubleList);
	}
}

/*
 * Insert data into linkedlist's HEAD
 * @param DOUBLELIST *pDoubleList
 * @param void *pData
 * @return INT
 */
INT DoubleList_InsertHead(DOUBLELIST *pDoubleList, void *pData)
{
	DOUBLENODE *pNode;
	if (NULL == pDoubleList || NULL == pData)
	{
		return CAPI_FAILED;
	}
	/*Create a now node*/
	pNode = (DOUBLENODE *)malloc(sizeof(DOUBLENODE));
	if (NULL == pNode)
	{
		return CAPI_FAILED;
	}
	pNode->pData = pData;
	
	pNode->pNext = pDoubleList->pHead;
	pNode->pPrev = NULL;
	if (NULL != pDoubleList->pHead)
	{
		pDoubleList->pHead->pPrev = pNode;
	}
	pDoubleList->pHead = pNode;

	if (NULL == pDoubleList->pTail)
	{
		pDoubleList->pTail = pNode;
	}
	pDoubleList->uCount += 1;
	return CAPI_SUCCESS;
}

/*
 * Insert data into linkedlist's TAIL
 * @param DOUBLELIST *pDoubleList
 * @param void *pData
 * @return INT
 */
INT DoubleList_InsertTail(DOUBLELIST *pDoubleList, void *pData)
{
	DOUBLENODE *pNode;
	if (NULL == pDoubleList || NULL == pData)
	{
		return CAPI_FAILED;
	}
	/*Create a now node*/
	pNode = (DOUBLENODE *)malloc(sizeof(DOUBLENODE));
	if (NULL == pNode)
	{
		return CAPI_FAILED;
	}
	pNode->pData = pData;
	pNode->pNext = NULL;
	pNode->pPrev = pDoubleList->pTail;

	if (NULL == pDoubleList->pTail)
	{
		pDoubleList->pHead = pNode;
	}
	else
	{
		pDoubleList->pTail->pNext = pNode;
	}
	pDoubleList->pTail = pNode;
	pDoubleList->uCount += 1;
	return CAPI_SUCCESS;
}

/*
 * Pop data in linkedlist's HEAD node
 * @param DOUBLELIST *pDoubleList
 * @return void *
 */
void * DoubleList_PopHead(DOUBLELIST *pDoubleList)
{
	DOUBLENODE *pPopNode;
	void *pPopData;
	if (NULL == pDoubleList || NULL == pDoubleList->pHead)
	{
		return NULL;
	}
	pPopNode = pDoubleList->pHead;
	pPopData = pPopNode->pData;

	if (pDoubleList->pCur == pDoubleList->pHead)
	{
		pDoubleList->pCur = pDoubleList->pHead->pNext;
	}
	pDoubleList->pHead = pDoubleList->pHead->pNext;
	if (NULL != pDoubleList->pHead)
	{
		pDoubleList->pHead->pPrev = NULL;
	}

	pDoubleList->uCount -= 1;
	if (0 == pDoubleList->uCount)
	{
		pDoubleList->pTail = NULL;
	}

	free(pPopNode);
	return pPopData;
}

/*
 * Pop data in linkedlist's Tail node
 * @param DOUBLELIST *pDoubleList
 * @return void *
 */
void * DoubleList_PopTail(DOUBLELIST *pDoubleList)
{
	DOUBLENODE *pPopNode;
	void *pPopData;
	if (NULL == pDoubleList || NULL == pDoubleList->pHead)
	{
		return NULL;
	}
	pPopNode = pDoubleList->pTail;
	pPopData = pPopNode->pData;

	if (pDoubleList->pCur == pDoubleList->pTail)
	{
		pDoubleList->pCur = NULL;
	}

	if (pDoubleList->pHead == pDoubleList->pTail)
	{
		pDoubleList->pHead = NULL;
	}
	else
	{
		pDoubleList->pTail->pPrev->pNext = NULL;
	}

	pDoubleList->pTail = pDoubleList->pTail->pPrev;
	pDoubleList->uCount -= 1;
	free(pPopNode);
	return pPopData;
}

/*
 * Delete a node which data is same with parameter pMatchData. If there are many
 * nodes have same value, it just delete the first node
 * @param DOUBLELIST *pDoubleList
 * @param void *pMatchData
 * @param COMPAREFUNC CompareFunc
 * @param DESTROYFUNC DestroyFunc
 * @return INT(by default) -- return CAPI_FAILED if delete fail or there is no match node
						   -- return CAPI_SUCCESS means delete successfully
 */
INT DoubleList_Delete(DOUBLELIST *pDoubleList, void *pMatchData,
	COMPAREFUNC CompareFunc, DESTROYFUNC DestroyFunc)
{
	DOUBLENODE *pNode, *pPrevNode;
	if (NULL == pDoubleList || NULL == DestroyFunc)
	{
		return CAPI_FAILED;
	}

	pNode = pDoubleList->pHead;
	pPrevNode = pNode;
	while (NULL != pNode)
	{
		if (0 == (*CompareFunc)(pNode->pData, pMatchData))
		{
			/*If match node is HEAD node*/
			if (pDoubleList->pHead == pNode)
			{
				pDoubleList->pHead = pNode->pNext;
				if (NULL != pDoubleList->pHead)
				{
					pDoubleList->pHead->pPrev = NULL;
				}
				else
				{
					pDoubleList->pTail = NULL;
					pDoubleList->pCur = NULL;
				}
			}
			else if (pDoubleList->pTail == pNode)	/*If match node is TAIL node*/
			{
				pDoubleList->pTail = pPrevNode;
			}
			else
			{
				pPrevNode->pNext = pNode->pNext;
				if (NULL != pNode->pNext)
				{
					pNode->pNext->pPrev = pPrevNode;
				}
			}

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
 * Get node's count in Double-Way linkedlist
 * @param DOUBLELIST *pDoubleList
 * @return UINT
 */
UINT DOUBLEList_GetCount(DOUBLELIST *pDoubleList)
{
	if (NULL == pDoubleList)
	{
		return 0;
	}
	else
	{
		return pDoubleList->uCount;
	}
}

/*
 * Get head node in Double-Way linkedlist
 * @param DOUBLELIST *pDoubleList
 * @return void *
 */
void * DoubleList_GetHead(DOUBLELIST *pDoubleList)
{
	if (NULL == pDoubleList)
	{
		return NULL;
	}
	else if (NULL == pDoubleList->pHead)
	{
		return NULL;
	}
	else
	{
		return pDoubleList->pHead->pData;
	}
}

/*
 * Get tail node in Double-Way linkedlist
 * @param DOUBLELIST *pDoubleList
 * @return void *
 */
void * DoubleList_GetTail(DOUBLELIST *pDoubleList)
{
	if (NULL == pDoubleList)
	{
		return NULL;
	}
	else if (NULL == pDoubleList->pTail)
	{
		return NULL;
	}
	else
	{
		return pDoubleList->pHead->pData;
	}
}

/*
 * Find match node in Double-Way linkedlist
 * @param DOUBLELIST *pDoubleList
 * @param void *pMatchData
 * @param COMPAREFUNC CompareFunc
 * @return void *
 */
void * DoubleList_Find(DOUBLELIST *pDoubleList, void *pMatchData, COMPAREFUNC CompareFunc)
{
	DOUBLENODE *pNode;
	pNode = pDoubleList->pHead;
	while (pNode)
	{
		if (0 == (*CompareFunc)(pNode->pData, pMatchData))
		{
			void *pData;
			pData = pNode->pData;
			return pData;
		}
		else
		{
			pNode = pNode->pNext;
		}
	}
	return NULL;
}

/*
 * The enum init function of Double-Way linkedlist
 * @param DOUBLELIST *pDoubleList
 * @return void
 */
void DoubleList_EnumBegin(DOUBLELIST *pDoubleList)
{
	pDoubleList->pCur = pDoubleList->pHead;
	return;
}

/*
* To enum next node of Double-Way linkedlist, it must invoke DoubleList_EnumBegin()
* function before invoke this function first
* @param DOUBLELIST *pDoubleList
* @return void *
*/
void * DoubleList_EnumNext(DOUBLELIST *pDoubleList)
{
	DOUBLENODE *pCur;
	pCur = pDoubleList->pCur;
	if (NULL != pCur)
	{
		pDoubleList->pCur = pCur->pNext;
		return pCur->pData;
	}
	return NULL;
}

/*
 * To enum node of Double-Way linkedlist, it must invoke DoubleList_EnumBegin()
 * function before invoke this function first
 * @param DOUBLELIST *pDoubleList
 * @return void *
 */
DOUBLENODE * DoubleList_EnumNode(DOUBLELIST *pDoubleList)
{
	DOUBLENODE *pCur;
	pCur = pDoubleList->pCur;
	if (NULL != pCur)
	{
		pDoubleList->pCur = pCur->pNext;
		return pCur;
	}
	return NULL;
}

/*
 * To delete specific node
 * @param DOUBLELIST *pDoubleList
 * @DOUBLENODE *pNode
 * @return DOUBLENODE *
 */
DOUBLENODE * DoubleList_PopNode(DOUBLELIST *pDoubleList, DOUBLENODE *pNode)
{
	if (NULL != pNode->pPrev)
	{
		pNode->pPrev->pNext = pNode->pNext;
	}
	if (NULL != pNode->pNext)
	{
		pNode->pNext->pPrev = pNode->pPrev;
	}
	if (pDoubleList->pCur == pNode)
	{
		pDoubleList->pCur = pNode->pNext;
	}
	return pNode;
}

/*
 * @function INT DoubleList_InsertSort(DOUBLELIST *pDoubleList, COMPAREFUNC CompareFunc);
 * @function INT DoubleList_Merge(DOUBLELIST *pDoubleListA, DOUBLELIST *pDoubleListB, COMPAREFUNC CompareFunc);
 * @function DOUBLELIST * DoubleList_Split(DOUBLELIST *pDoubleList, UINT uCount);
 * @function INT DoubleList_MergeSort(DOUBLELIST *pDoubleList, COMPAREFUNC CompareFunc, UINT uInsertSortCount);
 * @function void DoubleList_Traverse(DOUBLELIST *pDoubleList, TRAVERSEFUNC TraverseFunc);
 * @function void DoubleList_Distribute(DOUBLELIST *pDoubleList, UINT uRadix, UINT uKeyIndex, 
		DOUBLENODE **ppHead, DOUBLENODE **ppTail, GETKEYFUNC GetKeyFunc);
 * @function void DoubleList_Collect(DOUBLELIST *pDoubleList, UINT uRadix, DOUBLENODE **ppHead, DOUBLENODE **ppTail);
 * @function INT DoubleList_RadixSort(DOUBLELIST *pDoubleList, UINT uRadix, UINT uMaxKeyLen, GETKEYFUNC GetKeyFunc);
 */