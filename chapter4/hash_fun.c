/*********************************************************************************
 * FileName:	hash_fun.c
 * Author:		gehan
 * Date:		07/12/2017
 * Description:	Functions for hash table
**********************************************************************************/

#include "algo.h"

/*
 * hash function for int data
 * @param UINT uKey
 * @param UINT uBucketCount -- the size of hash table (count of slots)
 * @return UINT
 */
UINT HashInt(UINT uKey, UINT uBucketCount)
{
	return uKey % uBucketCount;
}

/*
 * hash function for string data, folding every 5 characters of string into a paragraph,
 * and add these paragraphs as Octal integer
 * @param void *pStr
 * @param UINT uBucketCount -- the size of hash table (count of slots)
 * @return UINT
 */
UINT HashString(void *pStr, UINT uBucketCount)
{
	unsigned char *psz;
	UINT uHashValue, uRet, i;
	psz = (unsigned char *)pStr;
	uHashValue = 0;
	i = 0;
	uRet = 0;

	while ('\0' != *psz)
	{
		if (5 == i)
		{
			i = 0;
			uRet += uHashValue;
			uHashValue = 0;
		}

		uHashValue += uHashValue << 3;
		uHashValue += (UINT)(*psz);
		psz++;
		i++;
	}
	uRet += uHashValue;
	return (uRet % uBucketCount);
}