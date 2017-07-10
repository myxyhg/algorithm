/*********************************************************************************
 * FileName:	LinkedList_Sort.c
 * Author:		gehan
 * Date:		07/10/2017
 * Description: The Sort implement of One-Way LinkedList
 * Functions:	SingleList_InsertSort;
				SingleList_Split;
				SingleList_Merge;
				SingleList_MergeSort;
**********************************************************************************/

#include "algo.h"
#include "OneWay_LinkedList.c"

/*
 * The insert sort of one-way linkedlist
 * @param SINGLELIST *pSingleList
 * COMPAREFUNC CompareFunc
 * @return INT
 */
INT SingleList_InsertSort(SINGLELIST *pSingleList, COMPAREFUNC CompareFunc)
{
	SINGLENODE *pNode, *pPrevNode;
	if (NULL == pSingleList || NULL == CompareFunc)
	{
		return CAPI_FAILED;
	}

	pNode = pSingleList->pHead;
	pPrevNode = NULL;
	if (NULL == pNode)
	{
		return CAPI_FAILED;
	}
	while (NULL != pNode->pNext)
	{
		SINGLENODE *pTempNode;
		pTempNode = pSingleList->pHead;
		pPrevNode = NULL;
		while (pTempNode != pNode->pNext)
		{
			if ((*CompareFunc)(pNode->pNext->pData, pTempNode->pData) < 0)
			{
				SINGLENODE *pCurNode = pNode->pNext;
				if (NULL != pPrevNode)	/*Not insert before head*/
				{
					/*Pop pCurNode*/
					pNode->pNext = pNode->pNext->pNext;
					/*inset pNode->pNext before pTempNode*/
					pPrevNode->pNext = pCurNode;
					pCurNode->pNext = pTempNode;
				}
				else   /*Insert before head*/
				{
					/*Pop pCurNode*/
					pNode->pNext = pNode->pNext->pNext;
					/*Make pNode->pNext to be head*/
					pSingleList->pHead = pCurNode;
					pCurNode->pNext = pTempNode;
				}
				/*Change tail node's pointer*/
				if (pCurNode == pSingleList->pTail)
				{
					pSingleList->pTail = pNode;
				}
				break;
			}
			pPrevNode = pTempNode;
			pTempNode = pTempNode->pNext;
		}
		if (pTempNode == pNode->pNext)	/*No insert*/
		{
			pNode = pNode->pNext;
		}
		else   /*Already insert*/
		{
			/*don't need back move pNode because it already move in before operation*/
		}
	}
	return CAPI_SUCCESS;
}

/*
 * Merge-Insertion Sort for One-Way LinkedList: it need 2 auxiliary functions,
 * one split linkedlist to 2 sub-list, one used to merge Sorted sub-lists
 */

/*
 * Split one-way linkedlist to 2 sub-lists from middle location, the original linkedlist
 * becomes the first sub-list
 * @param SINGLELIST *pSingleList
 * @param UINT nCount -- the node count of first sub-list
 * @return SINGLELIST * -- return the 2nd sub-list if split successfully, fail return NULL
 */
SINGLELIST * SingleList_Split(SINGLELIST *pSingleList, UINT uCount)
{
	SINGLENODE *pNode;	/*Save the split node*/
	SINGLELIST *pSecondList;
	UINT uIndex;
	if (NULL == pSingleList || 0 == uCount || pSingleList->uCount <= uCount)
	{
		return NULL;
	}

	/*Create a new empty linkedlist*/
	pSecondList = SingleList_Create();
	if (NULL == pSecondList)
	{
		return NULL;
	}

	/*Get the split node location*/
	pNode = pSingleList->pHead;
	for (uIndex = 1; uIndex < uCount; ++uIndex)
	{
		pNode = pNode->pNext;
	}

	/*fill the 2nd sub-list's contents*/
	pSecondList->pHead = pNode->pNext;
	pSecondList->pTail = pSingleList->pTail;
	pSecondList->uCount = pSingleList->uCount - uCount;

	/*modify the 1st sub-list*/
	pSingleList->pTail = pNode;
	pSingleList->uCount = uCount;
	pSingleList->pTail->pNext = NULL;

	return pSecondList;
}

/*
 * Merge 2 sorted sub-list
 * @param SINGLELIST *pSingleListA
 * @param SINGLELIST *pSingleListB
 * @param COMPAREFUNC CompareFunc
 * @return INT
 */
INT SingleList_Merge(SINGLELIST *pSingleListA, SINGLELIST *pSingleListB,
	COMPAREFUNC CompareFunc)
{
	SINGLENODE *pNodeA, *pNodeB;
	SINGLENODE *pPrevA;		/*Prev node of pNodeA*/
	if (NULL == pSingleListA || NULL == pSingleListB || NULL == CompareFunc)
	{
		return CAPI_FAILED;
	}

	pNodeA = pSingleListA->pHead;
	pNodeB = pSingleListB->pHead;
	pPrevA = NULL;
	while (NULL != pNodeB)
	{
		while (NULL != pNodeA)
		{
			if ((*CompareFunc)(pNodeA->pData, pNodeB->pData) >= 0)
			{
				SINGLENODE *pNode = NULL;
				/*Pop pNodeB and save it in pNode*/
				pNode = pNodeB;
				pNodeB = pNodeB->pNext;
				/*Insert pNode before pNodeA*/
				if (NULL == pPrevA)
				{
					pSingleListA->pHead = pNode;
					pNode->pNext = pNodeA;
				}
				else
				{
					pPrevA->pNext = pNode;
					pNode->pNext = pNodeA;
				}
				pPrevA = pNode;
				break;
			}
			pPrevA = pNodeA;
			pNodeA = pNodeA->pNext;
		}
		/*If all data in pSingleListB greater than pSingleListA, just insert B after A*/
		if (NULL == pNodeA)
		{
			pSingleListA->pTail->pNext = pNodeB;
			pSingleListA->pTail = pSingleListB->pTail;
			break;
		}
	}
	pSingleListA->uCount += pSingleListB->uCount;
	free(pSingleListB);
	return CAPI_SUCCESS;
}

/*
 * Use Merge-Insertion sort for one-way linkedlist, use merge sort first, than use insertion
 * sort when the count less than a specify value in list
 * @param SINGLELIST *pSingleList
 * @param COMPAREFUNC CompareFunc
 * @param UINT uInsertSortCount
 * @return INT
 */
INT SingleList_MergeSort(SINGLELIST *pSingleList, COMPAREFUNC CompareFunc, UINT uInsertSortCount)
{
	SINGLELIST *pSecondList;
	if (NULL == pSingleList || NULL == CompareFunc)
	{
		return CAPI_FAILED;
	}
	if (pSingleList->uCount < 2)
	{
		return CAPI_SUCCESS;
	}

	if (pSingleList->uCount <= uInsertSortCount)
	{
		(void)SingleList_InsertSort(pSingleList, CompareFunc);
	}
	else
	{
		pSecondList = SingleList_Split(pSingleList, (pSingleList->uCount) / 2);
		(void)SingleList_MergeSort(pSingleList,CompareFunc,uInsertSortCount);
		(void)SingleList_MergeSort(pSecondList, CompareFunc, uInsertSortCount);
		(void)SingleList_Merge(pSingleList, pSecondList, CompareFunc);
	}
	return CAPI_SUCCESS;
}

/*
 * Distribute linkedlist according with uKeyIndex's data, and put them into corresponding box
 * @param SINGLELIST *pSingleList
 * @param UINT uRadix -- the base of radix sort and it related to specific data type. In general,
						 its value is 16 for int type and for string its max value is 255
 * @param UINT uKeyIndex
 * @param SINGLENODE **ppHead -- the box's head pointer
 * @param SINGLENODE **ppTail
 * @param GETKEYFUNC GetKeyFunc
 * @return void
 */
static void SingleList_Distribute(SINGLELIST *pSingleList, UINT uRadix, UINT uKeyIndex,
	SINGLENODE **ppHead, SINGLENODE **ppTail, GETKEYFUNC GetKeyFunc)
{
	SINGLENODE *pNode;
	UINT i;
	/*Initial boxes*/
	for (i = 0; i < uRadix; ++i)
	{
		ppHead[i] = NULL;
		ppTail[i] = NULL;
	}
	pNode = pSingleList->pHead;
	while (NULL != pNode)
	{
		UINT uRadixIndex = (*GetKeyFunc)(pNode->pData, uKeyIndex);
		if (NULL == ppHead[uRadixIndex])
		{
			ppHead[uRadixIndex] = pNode;
			ppTail[uRadixIndex] = pNode;
			pNode = pNode->pNext;
			ppTail[uRadixIndex]->pNext = NULL;
		}
		else
		{
			ppTail[uRadixIndex]->pNext = pNode;
			ppTail[uRadixIndex] = ppTail[uRadixIndex]->pNext;
			pNode = pNode->pNext;
			ppTail[uRadixIndex]->pNext = NULL;
		}
	}
}

/*
 * To collect all sorted datas in boxes and link them into a linkedlist
 * @param SINGLELIST *pSingleList
 * @param UINT uRadix
 * @param SINGLENODE **ppHead -- the box's head pointer
 * @param SINGLENODE **ppTail
 * @return void
 */
static void SingleList_Collect(SINGLELIST *pSingleList, UINT uRadix,
	SINGLENODE **ppHead, SINGLENODE **ppTail)
{
	SINGLENODE *pHead;
	SINGLENODE *pTail;
	UINT uRadixIndex = 0;
	/*To find the first non-empty box*/
	while (uRadixIndex < uRadix)
	{
		if (NULL == ppHead[uRadixIndex])
		{
			uRadixIndex++;
			continue;
		}
		else
		{
			break;
		}
	}
	if (uRadixIndex == uRadix)
	{
		/*Can't find non-empty box*/
		return;
	}
	pHead = ppHead[uRadixIndex];
	pTail = ppTail[uRadixIndex];
	while (uRadixIndex < uRadix - 1)
	{
		/*Continue to find next non-empty box*/
		++uRadixIndex;
		if (NULL == ppHead[uRadixIndex])
		{
			continue;
		}
		if (uRadixIndex < uRadix)
		{
			pTail->pNext = ppHead[uRadixIndex];
			pTail = ppTail[uRadixIndex];
		}
	}
	pSingleList->pHead = pHead;
	pSingleList->pTail = pTail;
}

/*
 * RadixSort function of one-way linkedlist
 * @param SINGLELIST *pSingleList
 * @param UINT uRadix
 * @param UINT uMaxKeyLen
 * @param GETKEYFUNC GetKeyFunc
 * @return INT
 */
INT SingleList_RadixSort(SINGLELIST *pSingleList, UINT uRadix, UINT uMaxKeyLen, GETKEYFUNC GetKeyFunc)
{
	SINGLENODE **ppHead;
	SINGLENODE **ppTail;
	UINT i;
	/*allocate memory for box*/
	ppHead = (SINGLENODE **)malloc(uRadix * sizeof(SINGLENODE *));
	ppTail = (SINGLENODE **)malloc(uRadix * sizeof(SINGLENODE *));
	if (NULL == ppHead || NULL == ppTail)
	{
		return CAPI_FAILED;
	}
	/*In order to do distribite and collect operation for ith keyword*/
	for(i = 0; i< uMaxKeyLen; ++i)
	{
		pSingleList_Distribute(pSingleList, uRadix, i, ppHead, ppTail, GetKeyFunc);
		SingleList_Collect(pSingleList, uRadix, ppHead, ppTail);
	}
	free(ppHead);
	free(ppTail);
	return CAPI_SUCCESS;
}