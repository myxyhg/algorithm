/*********************************************************************************
* FileName:		tree.c
* Author:		gehan
* Date:			07/18/2017
* Description:	The implementation of regular tree
**********************************************************************************/

#include "algo.h"
#include "DoubleWay_LinkedList.c"

typedef struct TREE_st
{
	DOUBLELIST *pLeafList;
	DOUBLELIST *pSubTreeList;
	void *pProperties;
}TREE;

/*
 * Create tree
 * @return TREE *
 */
TREE * Tree_Create()
{
	TREE *pNewTree;
	pNewTree = (TREE *)malloc(sizeof(TREE));
	if (NULL == pNewTree)
	{
		return NULL;
	}

	/*Create leaf list*/
	pNewTree->pLeafList = DoubleList_Create();
	if (NULL == pNewTree->pLeafList)
	{
		free(pNewTree);
		return NULL;
	}

	/*Create subtree list*/
	pNewTree->pSubTreeList = DoubleList_Create();
	if (NULL == pNewTree->pSubTreeList)
	{
		DoubleList_Destroy(pNewTree->pLeafList, NULL);
		free(pNewTree);
		return NULL;
	}
	pNewTree->pProperties = NULL;
	return pNewTree;
}

/*
 * Destroy tree
 * @param TREE *pTree
 * @param DESTROYFUNC LeafDestroyFunc
 * @param DESTROYFUNC PropDestroyFunc
 * @return void
 */
void Tree_Destroy(TREE *pTree, DESTROYFUNC LeafDestroyFunc, DESTROYFUNC PropDestroyFunc)
{
	DOUBLELIST *pList;
	void *pData;
	if (NULL == pTree || NULL == LeafDestroyFunc || NULL == PropDestroyFunc)
	{
		return;
	}

	pList = pTree->pSubTreeList;
	if (NULL == pList)
	{
		return;
	}
	DoubleList_EnumBegin(pList);
	while (NULL != (pData = DoubleList_EnumNext(pList)))
	{
		Tree_Destroy((TREE *)pData, LeafDestroyFunc, PropDestroyFunc);
	}

	if (NULL != pTree->pProperties)
	{
		PropDestroyFunc(pTree->pProperties);
	}

	DoubleList_Destroy(pTree->pLeafList, LeafDestroyFunc);
	free(pTree);
}

/*
 * Add leaf for tree
 * @param TREE *pTree
 * @param void *pLeafData
 * @return INT
 */
INT Tree_AddLeaf(TREE *pTree, void *pLeafData)
{
	if (NULL == pTree)
	{
		return CAPI_FAILED;
	}
	return DoubleList_InsertTail(pTree->pLeafList, pLeafData);
}

/*
 * Remove leaf for tree
 * @param TREE *pTree
 * @param void *pLeafData
 * @param DESTROYFUNC LeafDestroyFunc
 * @return INT
 */
INT Tree_RemoveLeaf(TREE *pTree, void *pLeafData, DESTROYFUNC LeafDestroyFunc)
{
	if (NULL == pTree)
	{
		return CAPI_FAILED;
	}
	return DoubleList_Remove(pTree->pLeafList, pLeafData, LeafDestroyFunc);
}

/*
 * Set properties for tree
 * @param TREE *pTree
 * @param void *pProperties
 * @param DESTROYFUNC PropDestroyFunc
 * @return INT
 */
INT Tree_SetProperties(TREE *pTree, void *pProperties, DESTROYFUNC PropDestroyFunc)
{
	if (NULL == pTree)
	{
		return CAPI_FAILED;
	}
	if (NULL != pTree->pProperties)
	{
		PropDestroyFunc(pTree->pProperties);
	}
	pTree->pProperties = pProperties;
	return CAPI_SUCCESS;
}

/*
 * Get properties for tree
 * @param TREE *pTree
 * @return void *
 */
void * Tree_GetProperties(TREE *pTree)
{
	if (NULL == pTree)
	{
		return NULL;
	}
	return pTree->pProperties;
}

/*
 * Add subtree for tree
 * @param TREE *pTree
 * @param Tree *pSubTree
 * @return INT
 */
INT Tree_AddSubTree(TREE *pTree, TREE *pSubTree)
{
	if (NULL == pTree || NULL == pSubTree)
	{
		return CAPI_FAILED;
	}
	return DoubleList_InsertTail(pTree->pLeafList, (void *)pSubTree);
}

/*
 * Remove subtree for tree
 * @param TREE *pTree
 * @param Tree *pSubTree
 * @param DESTROYFUNC LeafDestroyFunc
 * @param DESTROYFUNC PropDestroyFunc
 * @return INT
 */
INT Tree_RemoveSubTree(TREE *pTree, TREE *pSubTree,
	DESTROYFUNC LeafDestroyFunc, DESTROYFUNC PropDestroyFunc)
{
	DOUBLELIST *pList;
	DOUBLENODE *pNode;
	if (NULL == pTree || NULL == pSubTree || NULL == PropDestroyFunc \
		|| NULL == LeafDestroyFunc)
	{
		return CAPI_FAILED;
	}

	pList = pTree->pSubTreeList;
	if (NULL == pList)
	{
		return CAPI_FAILED;
	}

	/* 遍历子树链表查找对应的子树进行删除 */
	DoubleList_EnumBegin(pList);
	while (NULL != (pNode = DoubleList_EnumNode(pList)))
	{
		if ((TREE *)(pNode->pData) == pSubTree)
		{
			pNode = DoubleList_PopNode(pList, pNode);
			Tree_Destroy((TREE *)(pNode->pData), LeafDestroyFunc, PropDestroyFunc);
			free(pNode);
			break;
		}
	}
	return CAPI_SUCCESS;
}

/*
* Copy tree with Pre-traversal method
* @param TREE *pTree
* @param COPYFUNC LeafCopyFunc
* @param COPYFUNC PropCopyFunc
* @return TREE *
*/
TREE * Tree_Copy(TREE *pTree, COPYFUNC LeafCopyFunc, COPYFUNC PropCopyFunc)
{
	TREE *pNewTree;
	DOUBLELIST *pNewList, *pList;
	void *pData;
	if (NULL == pTree || NULL == LeafCopyFunc || NULL == PropCopyFunc)
	{
		return NULL;
	}

	pNewList = DoubleList_Create();
	if (NULL == pNewList)
	{
		return NULL;
	}

	pNewTree = Tree_Create();
	if (NULL == pNewTree)
	{
		DoubleList_Destroy(pNewList, NULL);
		reutrn NULL;
	}

	pNewTree->pLeafList = DoubleList_Copy(pTree->pLeafList, LeafCopyFunc);
	pNewTree->pProperties = (*PropCopyFunc)(pTree->pProperties);
	pList = pTree->pSubTreeList;
	/* 逐个遍历子树列表调用Tree_Copy 递归拷贝子树列表 */
	DoubleList_EnumBegin(pList);
	while (NULL != (pData = DoubleList_EnumNext(pList)))
	{
		TREE *pSubTree = Tree_Copy((TREE *)pData,LeafCopyFunc,PropCopyFunc);
		DoubleList_InsertTail(pNewList, (void *)pSubTree);
	}
	return pNewTree;
}

/*
 * To find subtree according with tree's property
 * @param TREE *pTree
 * @param void *pProperties
 * @param COMPAREFUNC CompareFunc
 * @return TREE *
 */
TREE * Tree_FindSubTreeByProp(TREE *pTree, void *pProperties, COMPAREFUNC CompareFunc)
{
	TREE *pSubTree;
	DOUBLELIST *pList;
	void *pData;
	if (NULL == pTree)
	{
		return NULL;
	}

	if (0 == (*CompareFunc)(pProperties, pTree->pProperties))
	{
		return pTree;
	}
	pList = pTree->pSubTreeList;
	DoubleList_EnumBegin(pList);
	/* 逐个遍历子树并递归调用本函数在子树中进行查找 */
	while (NULL != (pData = DoubleList_EnumNext(pList)))
	{
		pSubTree = Tree_FindSubTreeByProp((TREE *)pData,pProperties,CompareFunc);
		if (NULL != pSubTree)
		{
			return pSubTree;
		}
	}
	return NULL;
}