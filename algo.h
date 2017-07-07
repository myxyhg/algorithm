#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int UINT;
typedef int INT;

enum INT
{
	CAPI_FAILED = 0,
	CAPI_SUCCESS,
};

/*
 * Generic data comparison function
 * @param void *pData1
 * @param void *pData2
 * @return INT	-- return negative value if pData1 less than pData2;
				-- return 0 if pData1 equals to pData2;
				-- return positive value if pData1 greater than pData2;
 */
typedef INT(*COMPAREFUNC) (void *pData1, void *pData2);

/*
 * Generic data destructor function
 * @param void *pData
 * @return
 */
typedef void(*DESTROYFUNC) (void *pData);

/*
 * Generic data traverse function
 * @param void *pData
 * @return void
 */
typedef void(*TRAVERSEFUNC) (void *pData);

/*
 * Generic data traverse function
 * @param void *pData
 * @return INT
 */
typedef INT(*VISITFUNC) (void *pData);

/*
 * Generic data copy function
 * @param void *pData
 * @return void *
 */
typedef void*(*COPYFUNC) (void *pData);

/*
 * The function of access keyword and transfer into index for radix sort
 * @param void *pData -- the keyword's pointer
 * @param UINT *uKeyIndex -- the keyword's index
 * @return UINT -- the index after keyword transfer
 */
typedef UINT(*GETKEYFUNC) (void *pData, UINT uKeyIndex);

/*
 * The callback function of calculate hash value
 * @param void *pKey -- the keyword that need to calculate hash value
 * @param UINT uBucketNum -- the size of bucket array in hash table
 * @return UINT -- the calculated hash value and it used to be the index of bucket array
 */
typedef UINT(*HASHFUNC) (void *pKey, UINT uBucketNum);