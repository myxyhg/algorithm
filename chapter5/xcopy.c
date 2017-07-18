/*********************************************************************************
* FileName:		xcopy.c
* Author:		gehan
* Date:			07/18/2017
* Description:	The implementation of xcopy feature in DOS system
**********************************************************************************/

#include <windows.h>
#include <stdio.h>

void CopyCurrentDir(char *pszSrcDir, char *pszTargetDir, BOOL bOverWrite);

/*
 * Copy all files in source dir to target dir
 * @param char *pszSrcDir
 * @param char *pszTargetDir
 * @param BOOL bOverWrite  -- To over write original files if it is FALSE
 * @return void
 */
void CopyCurrentDir(char *pszSrcDir, char *pszTargetDir, BOOL bOverWrite)
{
	char szBaseSearch[MAX_PATH];
	HANDLE hFindFile;
	WIN32_FIND_DATA FindData;
	sprintf(szBaseSearch,"%s",pszSrcDir);
	hFindFile = FindFirstFile(szBaseSearch,&FindData);
	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		return;
	}
	do
	{
		CreateDirectory(pszTargetDir, NULL);
		if (!strcmp(FindData.cFileName, ".") || !strcmp(FindData.cFileName, ".."))
		{
			continue;
		}

		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//a directory
			continue;
		}
		else
		{
			//plain file
			char szSrcDir[MAX_PATH];
			char szTargetDir[MAX_PATH];
			sprintf(szSrcDir, "%s", FindData.cFileName);
			sprintf(szTargetDir, "%s\\%s", pszTargetDir, FindData.cFileName);
			CopyFile(szSrcDir,szTargetDir,bOverWrite);
		}
	} while (FindNextFile(hFindFile,&FindData));
	FindClose(hFindFile);
}

/*
 * Copy all files in source dir and its sub-folder into another dir
 * @param char *pszSrcDir
 * @param char *pszTargetDir
 * @param BOOL bOverWrite  -- To over write original files if it is FALSE
 * @return void
 */
void Xcopy(char *pszSrcDir, char *pszTargetDir, BOOL bOverWrite)
{
	char szBaseSearch[MAX_PATH];
	char szCurrentPath[MAX_PATH];
	HANDLE hFindFile;
	WIN32_FIND_DATA FindData;
	memset(szCurrentPath, 0, MAX_PATH);
	GetCurrentDirectory(MAX_PATH, szCurrentPath);
	sprintf(szBaseSearch, "%s\\*.*", szCurrentPath);
	hFindFile = FindFirstFile(szBaseSearch, &FindData);
	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		return;
	}
	do
	{
		CreateDirectory(pszTargetDir, NULL);
		if (!strcmp(FindData.cFileName, ".") || !strcmp(FindData.cFileName, ".."))
		{
			continue;
		}

		if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//a directory
			char szBaseDir[MAX_PATH];
			char szTargetDir[MAX_PATH];
			sprintf(szBaseDir, "%s\\%s", szCurrentPath, FindData.cFileName);
			SetCurrentDirectory(szBaseDir);
			_fullpath(szTargetDir, pszTargetDir, MAX_PATH);
			sprintf(szTargetDir, "%s\\%s", FindData.cFileName);
			Xcopy(szBaseDir, szTargetDir, bOverWrite);
			SetCurrentDirectory(szCurrentPath);
		}
	} while (FindNextFile(hFindFile, &FindData));
	FindClose(hFindFile);
	CopyCurrentDir(pszSrcDir, pszTargetDir, bOverWrite);
}