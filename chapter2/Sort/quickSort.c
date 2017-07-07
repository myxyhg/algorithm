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