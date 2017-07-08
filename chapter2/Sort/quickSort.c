/*********************************************************************************
 * FileName:	quickSort.c
 * Author:		gehan
 * Date:		07/08/2017
 * Description: quick sort with recursive method
**********************************************************************************/

#include "algo.h"

typedef struct SORTTABLE_st {
	void **ppData;		/*the pointer array that storage data pointer*/
	UINT uCursorCount;
	UINT uMaxCount;
}SORTTABLE;

/*
 * the constructure of sort table
 * @param UINT uMaxCount
 * @return SORTTABLE *
 */
SORTTABLE * SortTable_Create(UINT uMaxCount)
{
	SORTTABLE *pTable;
	if (0 == uMaxCount)
	{
		return NULL;
	}
	pTable = (SORTTABLE *)malloc(sizeof(struct SORTTABLE_st));
	if (NULL != pTable)
	{
		pTable->ppData = (void **)malloc(sizeof(uMaxCount * sizeof(void *)));
		if (NULL != pTable->ppData)
		{
			pTable->ppData[0] = NULL;
			pTable->uMaxCount = uMaxCount;
			pTable->uCursorCount = 0;
		}
		else
		{
			free(pTable);
			pTable = NULL;
		}
	}
	return pTable;
}

/*
 * the destructor of sort table
 * @param SORTTABLE *pTable	-- the sort table's pointer
 * @param DESTROYFUNC DestroyFunc -- the destructor function
 * @return void
 */
void SortTable_Destroy(SORTTABLE *pTable, DESTROYFUNC DestroyFunc)
{
	if (NULL != pTable)
	{
		if (NULL != DestroyFunc)
		{
			UINT i;
			for (i = 0; i < pTable->uCursorCount; ++i)
			{
				(*DestroyFunc)(pTable->ppData[i]);
			}
		}
		free(pTable->ppData);
		free(pTable);
	}
}

/*
 * the split function of sort table, it use the first data as pivot
 * @param SORTTABLE *pTable	-- the sort table's pointer
 * @param UINT uStart
 * @param UINT uEnd
 * @param COMPAREFUNC CompareFunc -- the comparison function
 * @return UINT
 */
static UINT SortTable_Split(SORTTABLE *pTable, UINT uStart, UINT uEnd, COMPAREFUNC CompareFunc)
{
	void *pSelData;
	UINT uLow = uStart;
	UINT uHigh = uEnd;
	pSelData = pTable->ppData[uLow];

	while (uLow < uHigh)
	{
		while ((*CompareFunc)(pTable->ppData[uHigh], pSelData) > 0
			&& uLow != uHigh)
		{
			--uHigh;
		}
		if (uLow != uHigh)
		{
			pTable->ppData[uLow] = pTable->ppData[uHigh];
			++uLow;
		}

		while ((*CompareFunc)(pTable->ppData[uLow], pSelData) < 0
			&& uLow != uHigh)
		{
			++uLow;
		}
		if (uLow != uHigh)
		{
			pTable->ppData[uHigh] = pTable->ppData[uLow];
			--uHigh;
		}
	}
	pTable->ppData[uLow] = pSelData;
	return uLow;
}

/*
 * the quick sort function of sort table with recusively method
 * @param SORTTABLE *pTable	-- the sort table's pointer
 * @param UINT uStart
 * @param UINT uEnd
 * @param COMPAREFUNC CompareFunc -- the comparison function
 * @return void
 */
void SortTable_QuickSort(SORTTABLE *pTable, UINT uStart, UINT uEnd, COMPAREFUNC CompareFunc)
{
	UINT uMid = SortTable_Split(pTable, uStart, uEnd, CompareFunc);
	if (uMid > uStart)
	{
		(void)SortTable_QuickSort(pTable, uStart, uMid - 1, CompareFunc);
	}
	if (uEnd > uMid)
	{
		(void)SortTable_QuickSort(pTable, uMid + 1, uEnd, CompareFunc);
	}
}