/*********************************************************************************
 * FileName:	binary_search.c
 * Author:		gehan
 * Date:		07/08/2017
 * Description: Binary search in a sorted table
**********************************************************************************/

#include "algo.h"

typedef struct SORTTABLE_st {
	void **ppData;		/*the pointer array that storage data pointer*/
	UINT uCursorCount;
	UINT uMaxCount;
}SORTTABLE;

/*
 * Binary search for sort table
 * @param SORTTABLE *pTable	-- the sort table's pointer
 * @param void *pData -- the match data
 * @param COMPAREFUNC CompareFunc -- the comparison function
 * @return void * -- return match data if successfully, or return NULL
 */
void * SortTable_BinarySearch(SORTTABLE *pTable, void *pData, COMPAREFUNC CompareFunc)
{
	UINT uLow, uHigh, uMid;
	if (NULL == pTable || NULL == CompareFunc || NULL == pData || 0 == pTable->uCursorCount)
	{
		return NULL;
	}
	uLow = 0;
	uHigh = pTable->uCursorCount - 1;
	uMid = 0;
	while ((INT)uLow <= (INT)uHigh)
	{
		INT nResult;
		uMid = (uLow + uHigh) / 2;
		nResult = (*CompareFunc)(pTable->ppData[uMid], pData);
		if (nResult > 0)
		{
			uHigh = uMid - 1;
		}
		else if (nResult < 0)
		{
			uLow = uMid + 1;
		}
		else
		{
			return pTable->ppData[uMid];
		}
	}
	return NULL;
}