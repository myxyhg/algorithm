/*********************************************************************************
 * FileName:	hook.c
 * Author:		gehan
 * Date:		07/08/2017
 * Description: The introduction and implement of hook manage in Windows NT system
**********************************************************************************/

#include "algo.h"

/*
 * This stucture records the hook data of single function
 */
typedef struct APIHOOKDATA_st {
	DWORD dwSrcFuncAddr;	/* Source Function's Address */
	DWORD dwNewFuncAddr;	/* Hook Function's Address */
	BYTE  byHeadCode[6];	/* the first 6 bytes of Source Function */
	WORD  wFlag;			/* the flag shows whether set hook */
}APIHOOKDATA;

/*
 * This stucture be used to manages several hook functions
 */
typedef struct APIHOOK_st {
	APIHOOKDATA *pHookData;
	UINT uMaxFunctions;
}APIHOOK;

/*
 * ApiHook module's init function
 * @param INT uMaxHookFuncCounts
 * @return INT(by default) -- return NULL if fails
 */
HANDLE ApiHook_Init(UINT uMaxHookFuncCounts)
{
	APIHOOK *pApiHook = (APIHOOK *)malloc(sizeof(APIHOOK));
	if (pApiHook)
	{
		pApiHook->uMaxFunctions = uMaxHookFuncCounts;
		pApiHook->pHookData = (APIHOOKDATA *)malloc(sizeof(APIHOOKDATA) * uMaxHookFuncCounts);
		if (NULL != pApiHook->pHookData)
		{
			memset(pApiHook->pHookData,0,sizeof(APIHOOKDATA) * uMaxHookFuncCounts);
			return (HANDLE)pApiHook;
		}
		free(pApiHook);
	}
	return NULL;
}

/*
 * To Set a function's hook function via address
 * @param HANDLE hApiHook -- the handle generated from ApiHook_Init()
 * @param DWORD dwSrcFuncAddr -- Source function's address
 * @param DWORD dwNewFuncAddr -- Hook function's address
 * @return INT(by default) -- return -1 if fails, return 0 means the index in HOOK array
 */
INT ApiHook_SetByAddr(HANDLE hApiHook, DWORD dwSrcFuncAddr, DWORD dwNewFuncAddr)
{
	DWORD dwOldProtect;
	DWORD dwNewProtect;
	DWORD lpSrcFunc;
	DWORD lppNewFunc;
	UINT  i;
	INT   nAlreadyFlag = 0;

	if (NULL == hApiHook)
	{
		return -1;
	}
	APIHOOK *pApiHook = (APIHOOK *)hApiHook;
	lpSrcFunc = dwSrcFuncAddr;
	/*To find whether sets hook or not*/
	for (i = 0; i < pApiHook->uMaxFunctions; ++i)
	{
		if (pApiHook->pHookData[i].dwSrcFuncAddr == lpSrcFunc)
		{
			/*Just to change if it has set hook*/
			nAlreadyFlag = 1;
			break;
		}
	}

	/*Find a location in table if it still not set hook function for src function*/
	if (i == pApiHook->uMaxFunctions)
	{
		for (i = 0; i < pApiHook->uMaxFunctions; ++i)
		{
			if (0 == pApiHook->pHookData[i].wFlag)
			{
				break;
			}
		}
		if (i == pApiHook->uMaxFunctions)
		{
			return -1;
		}
	}

	/*To record hook function's address in table*/
	pApiHook->pHookData[i].dwNewFuncAddr = dwNewFuncAddr;

	/*Below code will saves the first 6 bytes of source function in table*/
	lppNewFunc = (DWORD)(&(pApiHook->pHookData[i].dwNewFuncAddr));
	if (!nAlreadyFlag)
	{
		/*To save the first 6 bytes data of source function into byHeaderCode*/
		memcpy(pApiHook->pHookData[i].byHeadCode,(const void *)lpSrcFunc,6);
	}
	/*
	 * Below code will change the first 6 bytes data of source function
	 * to a command that it can jump to new function's address
	 */
	if (0 == VirtualProtect((LPVOID)lpSrcFunc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		return -1;
	}
	*(unsigned char *)lpSrcFunc = (unsigned char)0xff;
	*(((unsigned char *)lpSrcFunc) + 1) = (unsigned char)0x25;
	memcpy((void *)(lpSrcFunc + 2),(const void *)&lppNewFunc,4);
	if (0 == VirtualProtect((LPVOID)lpSrcFunc, 6, dwOldProtect, &dwNewProtect))
	{
		return -1;
	}
	pApiHook->pHookData[i].wFlag = 1;
	pApiHook->pHookData[i].dwNewFuncAddr = lpSrcFunc;
	return (INT)i;
}

/*
* To cancel a function's hook function via address
* @param HANDLE hApiHook -- the handle generated from ApiHook_Init()
* @param DWORD dwSrcFuncAddr -- Source function's address
* @return INT(by default) -- return -1 if fails, return 0 means the index in HOOK array
*/
INT ApiHook_UnSetByAddr(HANDLE hApiHook, DWORD dwSrcFuncAddr)
{
	DWORD dwOldProtect;
	DWORD dwNewProtect;
	DWORD lpSrcFunc;
	UINT  i;

	APIHOOK *pApiHook = (APIHOOK *)hApiHook;
	if (NULL == hApiHook)
	{
		return -1;
	}
	
	for (i = 0; i < pApiHook->uMaxFunctions; ++i)
	{
		if (pApiHook->pHookData[i].dwSrcFuncAddr == dwSrcFuncAddr)
		{
			break;
		}
	}

	/*Find a location in table if it still not set hook function for src function*/
	if (i == pApiHook->uMaxFunctions)
	{
		return -1;
	}

	lpSrcFunc = pApiHook->pHookData[i].dwSrcFuncAddr;

	/*To set lpSrcFunc's memory attribute to read-write*/
	if (0 == VirtualProtect((LPVOID)lpSrcFunc, 6, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		return -1;
	}
	memcpy((void *)lpSrcFunc, pApiHook->pHookData[i].byHeadCode, 6);
	if (0 == VirtualProtect((LPVOID)lpSrcFunc, 6, dwOldProtect, &dwNewProtect))
	{
		return -1;
	}
	pApiHook->pHookData[i].wFlag = 0;
	return (INT)i;
}