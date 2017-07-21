/*********************************************************************************
 * FileName:		binary_tree.c
 * Author:		gehan
 * Date:			07/21/2017
 * Description:	The implementation of binary tree
**********************************************************************************/

#include "algo.h"

typedef struct BINTREEBASENODE_st {
	struct BINTREEBASENODE_st *pLeft;
	struct BINTREEBASENODE_st *pRight;
	struct BINTREEBASENODE_st *pParent;
	INT nMagic;
	void *pData;
}BINTREEBASENODE;

typedef struct BINTREE_st {
	BINTREEBASENODE *pRoot;
	UINT uNodeCount;
}BINTREE;

/*
* Find Function of binary search tree
* @param BINTREEBASENODE *pRoot
* @param void *pData
* @param COMPAREFUNC CompareFunc
* @return void *
*/
void * BinTree_Find(BINTREEBASENODE *pRoot, void *pData, COMPAREFUNC CompareFunc)
{
	BINTREEBASENODE *pNode;
	pNode = pRoot;
	while (NULL != pRoot)
	{
		INT nRet = (*CompareFunc)(pNode->pData, pData);
		if (nRet < 0)
		{
			pNode = pNode->pRight;
		}
		else if (nRet > 0)
		{
			pNode = pNode->pLeft;
		}
		else
		{
			return pNode->pData;
		}
	}
	return NULL;
}

/*
* Insert Function of binary search tree
* @param BINTREEBASENODE **pRoot
* @param void *pData
* @param COMPAREFUNC CompareFunc
* @param INT nMagic
* @return INT
*/
INT BinTree_Insert(BINTREEBASENODE **pRoot, void *pData, 
	COMPAREFUNC CompareFunc, INT nMagic)
{
	BINTREEBASENODE *pNode, *pNewNode;
	INT nRet = 0;
	if (NULL == pData || NULL == CompareFunc)
	{
		return CAPI_FAILED;
	}
	pNode = *pRoot;
	while (NULL != pNode)
	{
		nRet = (*CompareFunc)(pNode->pData, pData);
		if (nRet < 0)
		{
			if (NULL != pNode->pRight)
			{
				pNode = pNode->pRight;
				continue;
			}
		}
		else
		{
			if (NULL != pNode->pLeft)
			{
				pNode = pNode->pLeft;
				continue;
			}
		}
		break;
	}

	pNewNode = (BINTREEBASENODE *)malloc(sizeof(BINTREEBASENODE));
	if (NULL == pNewNode)
	{
		return CAPI_FAILED;
	}
	pNewNode->pLeft = NULL;
	pNewNode->pRight = NULL;
	pNewNode->pData = pData;
	pNewNode->nMagic = nMagic;

	if (NULL == pNode)
	{
		*pRoot = pNewNode;
		pNewNode->pParent = NULL;
	}
	else
	{
		if (nRet < 0)
		{
			pNode->pRight = pNewNode;
		}
		else
		{
			pNode->pLeft = pNewNode;
		}
		pNewNode->pParent = pNode;
	}
	return CAPI_SUCCESS;
}

/*
* Delete Function of binary search tree
* @param BINTREE *pBinTree
* @param void *pData
* @param COMPAREFUNC CompareFunc
* @param DESTROYFUNC DestroyFunc
* @return INT
*/
INT BinTree_Delete(BINTREE *pBinTree, void *pData,
	COMPAREFUNC CompareFunc, DESTROYFUNC DestroyFunc)
{
	BINTREEBASENODE *pNode, *pParentNode, *pMaxNode, *pParentMaxNode;
	INT nRet;
	if (NULL == pData || NULL == CompareFunc
		|| NULL == pBinTree || NULL == pBinTree->pRoot)
	{
		return CAPI_FAILED;
	}

	pNode = pBinTree->pRoot;
	pParentNode = NULL;
	while (NULL != pNode)
	{
		pParentNode = pNode;
		nRet = (*CompareFunc)(pNode->pData, pData);
		if (nRet < 0)
		{
			pNode = pNode->pRight;
		}
		else if (nRet > 0)
		{
			pNode = pNode->pLeft;
		}
		else
		{
			break;
		}
	}
	if (NULL == pNode)
	{
		// did not find specific node
		return CAPI_FAILED;
	}

	/* The found pNode has 2 sub-nodes */
	if (NULL != pNode->pLeft && NULL != pNode->pRight)
	{
		pMaxNode = pNode->pLeft;
		pParentMaxNode = pNode;
		while (NULL != pMaxNode->pRight)
		{
			pParentMaxNode = pMaxNode;
			pMaxNode = pMaxNode->pRight;
		}
		if (NULL != DestroyFunc && NULL != pNode->pData)
		{
			(*DestroyFunc)(pNode->pData);
		}
		pNode->pData = pMaxNode->pData;
		if (pMaxNode == pNode->pLeft)
		{
			pNode->pLeft = pMaxNode->pLeft;
		}
		else
		{
			pParentMaxNode->pRight = pMaxNode->pLeft;
		}
		free(pMaxNode);
		return CAPI_SUCCESS;
	}

	/* The found pNode has 1 sub-node at most */
	if (NULL != pNode->pLeft)
	{
		pMaxNode = pNode->pLeft;
	}
	else
	{
		pMaxNode = pNode->pRight;
	}

	if (pNode == pBinTree->pRoot)
	{
		pBinTree->pRoot = pMaxNode;
	}
	else
	{
		if (pParentNode->pLeft == pNode)
		{
			pParentNode->pLeft = pMaxNode;
		}
		else
		{
			pParentNode->pRight = pMaxNode;
		}
	}
	if (NULL != DestroyFunc && NULL != pNode->pData)
	{
		(*DestroyFunc)(pNode->pData);
	}
	free(pNode);
	return CAPI_SUCCESS;
}

/*
* In-order traverse Function of binary search tree
* @param BINTREE *pBinTree
* @param VISITFUNC VisitFunc
* @return void
*/
void BinTree_InOrderTraverse(BINTREE *pBinTree, VISITFUNC VisitFunc)
{
	BINTREEBASENODE *pNode;
	void *pData;
	pNode = pBinTree->pRoot;
	if (NULL == pNode)
	{
		return;
	}
	while (NULL != pNode->pLeft)
	{
		pNode = pNode->pLeft;
	}
	while (NULL != pNode)
	{
		pData = pNode->pData;
		(*VisitFunc)(pData);
		if (NULL != pNode->pRight)
		{
			pNode = pNode->pRight;
			while (NULL != pNode->pLeft)
			{
				pNode = pNode->pLeft;
			}
		}
		else
		{
			/* Go back to the parent node */
			BINTREEBASENODE *pParent = pNode->pParent;
			while (NULL != pParent && pParent->pRight == pNode)
			{
				pNode = pParent;
				pParent = pParent->pParent;
			}
			pNode = pParent;
		}
	}
}

/*
* Rotate left Function of binary search tree
* @param BINTREEBASENODE *pAnode -- the node need to be rotated
* @param BINTREEBASENODE **ppRoot
* @return void
*/
void BinTree_RotateLeft(BINTREEBASENODE *pANode, BINTREEBASENODE **ppRoot)
{
	BINTREEBASENODE *pBNode;
	pBNode = pANode->pRight;
	/* Let node B's left sub-node to node A's right sub-node */
	pANode->pRight = pBNode->pLeft;
	if (NULL != pBNode->pLeft)
	{
		pBNode->pLeft->pParent = pANode;
	}
	/* Change relation between node A's parent and node B */
	pBNode->pParent = pANode->pParent;
	if (pANode == *ppRoot)
	{
		*ppRoot = pBNode;
	}
	else if (pANode == pANode->pParent->pLeft)
	{
		pANode->pParent->pLeft = pBNode;
	}
	else
	{
		pANode->pParent->pRight = pBNode;
	}
	/* Change node A to be node B's left sub-node */
	pBNode->pLeft = pANode;
	pANode->pParent = pBNode;
}

/*
* Rotate right Function of binary search tree
* @param BINTREEBASENODE *pAnode -- the node need to be rotated
* @param BINTREEBASENODE **ppRoot
* @return void
*/
void BinTree_RotateRight(BINTREEBASENODE *pANode, BINTREEBASENODE **ppRoot)
{
	BINTREEBASENODE *pBNode;
	pBNode = pANode->pLeft;
	/* Let node B's right sub-node to node A's left sub-node */
	pANode->pLeft = pBNode->pRight;
	if (NULL != pBNode->pRight)
	{
		pBNode->pRight->pParent = pANode;
	}
	/* Change relation between node A's parent and node B */
	pBNode->pParent = pANode->pParent;
	if (pANode == *ppRoot)
	{
		*ppRoot = pBNode;
	}
	else if (pANode == pANode->pParent->pRight)
	{
		pANode->pParent->pRight = pBNode;
	}
	else
	{
		pANode->pParent->pLeft = pBNode;
	}
	/* Change node A to be node B's left sub-node */
	pBNode->pRight = pANode;
	pANode->pParent = pBNode;
}