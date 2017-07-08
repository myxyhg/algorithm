/*********************************************************************************
 * FileName:	quickSort2.c
 * Author:		gehan
 * Date:		07/08/2017
 * Description: The quick sort with non-recursive method
**********************************************************************************/

#include "algo.h"
#include "stack.c"
#include "quickSort.c"

/*
 * Use stack to implement the quick sort function of sort table with non-recusively method
 * @param SORTTABLE *pTable	-- the sort table's pointer
 * @param UINT uStart
 * @param UINT uEnd
 * @param COMPAREFUNC CompareFunc -- the comparison function
 * @return void
 */
void SortTable_QuickSort2(SORTTABLE *pTable, UINT uStart, UINT uEnd, COMPAREFUNC CompareFunc)
{
	STACK *pStack;
	UINT uLow = uStart;
	UINT uHigh = uEnd;
	UINT uMid;
	pStack = Stack_Create(uHigh - uLow);
	(void)Stack_Push(pStack, (void *)uLow);
	(void)Stack_Push(pStack, (void *)uHigh);
	while (!Stack_IsEmpty(pStack))
	{
		uHigh = (UINT)Stack_Pop(pStack);
		uLow = (UINT)Stack_Pop(pStack);
		if (uLow < uHigh)
		{
			uMid = SortTable_Split(pTable, uLow, uHigh, CompareFunc);
			if (uMid > uLow)
			{
				(void)Stack_Push(pStack, (void *)uLow);
				(void)Stack_Push(pStack, (void *)(uMid - 1));
			}
			if (uHigh > uMid)
			{
				(void)Stack_Push(pStack, (void *)(uMid + 1));
				(void)Stack_Push(pStack, (void *)uHigh);
			}
		}
		Stack_Destroy(pStack, NULL);
	}
}

/*
 * Don't use stack to implement the quick sort function of sort table with non-recusively method
 * @param SORTTABLE *pTable	-- the sort table's pointer
 * @param UINT uStart
 * @param UINT uEnd
 * @param COMPAREFUNC CompareFunc -- the comparison function
 * @return void
 */
void SortTable_QuickSort3(SORTTABLE *pTable, UINT uStart, UINT uEnd, COMPAREFUNC CompareFunc)
{
	UINT *puStack;
	UINT uStackTop = 0;
	UINT uLow = uStart;
	UINT uHigh = uEnd;
	UINT uMid;
	if (uEnd - uStart < 1)
	{
		return;
	}

	puStack = (UINT *)malloc((uHigh - uLow + 1) * sizeof(UINT));
	if (NULL == puStack)
	{
		return CAPI_FAILED;
	}
	puStack[uStackTop] = uLow;
	++uStackTop;
	puStack[uStackTop] = uHigh;
	++uStackTop;
	while (0 != uStackTop)
	{
		--uStackTop;
		uHigh = puStack[uStackTop];
		--uStackTop;
		uLow = puStack[uStackTop];
		if (uLow < uHigh)
		{
			uMid = SortTable_Split(pTable, uLow, uHigh, CompareFunc);
			if (uMid - uLow > uHigh - uMid)
			{
				puStack[uStackTop] = uLow;
				++uStackTop;
				puStack[uStackTop] = uMid - 1;
				++uStackTop;
				if (uHigh > uMid)
				{
					puStack[uStackTop] = uMid + 1;
					++uStackTop;
					puStack[uStackTop] = uHigh;
					++uStackTop;
				}
			}
			else
			{
				puStack[uStackTop] = uMid + 1;
				++uStackTop;
				puStack[uStackTop] = uHigh;
				++uStackTop;
				if (uMid > uLow)
				{
					puStack[uStackTop] = uLow;
					++uStackTop;
					puStack[uStackTop] = uMid - 1;
					++uStackTop;
				}
			}
		}
	}
	free(puStack);
}