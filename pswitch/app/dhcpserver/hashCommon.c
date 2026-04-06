/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  File Name::  DhcpHash.c
  Description: This file defines general HASH table operation functions
  Autor: Kniazkov Andrew
  Other Notes: This file can be used in other modules
********************************************************************************/
#include <common/sys_def.h>
#define GBN_INCLUDE_DHCPRELAYSERVER
#ifdef GBN_INCLUDE_DHCPRELAYSERVER
#include "hashCommon.h"


/*Function definition*/

/*******************************************************************************
Function: void HashInit
Description: Hash table initialization function
Input: HASH_S *pHashCB: Hash control block pointer
       ulong_t ulListNum: Number of nodes in the hash area
       ulong_t ulFreeNum: Number of nodes in the collision area
       HashMallocFunc HashMalloc: User-defined data block memory allocation function
       HashFreeFunc HashFree: User-defined data block memory release function
       HashKeyFunc HashKey: User-defined hash key generation function
       HashCompareFunc HashCompare: User-defined node data comparison function
Output: Initialized hash control block and hash node pool
Return: OK, ERROR
*******************************************************************************/
uint32  HashInit (HASH_S *pHashCB, uint32 ulListNum, uint32 ulFreeNum,
                HashMallocFunc HashMalloc, HashFreeFunc HashFree,
                HashKeyFunc HashKey, HashCompareFunc HashCompare)
{
    uint32 i;
    
    if(NULL == pHashCB)
    {
        return SYS_ERR_FAILED;
    }
    
    if(NULL == HashMalloc || NULL == HashFree ||
        NULL == HashKey || NULL == HashCompare)
    {
        return SYS_ERR_FAILED;
    }
    
    pHashCB->ulCount = 0;
    pHashCB->ulListNum = ulListNum;
    pHashCB->ulFreeNum = ulFreeNum;
    
    pHashCB->HashMalloc = HashMalloc;
    pHashCB->HashFree = HashFree;
    pHashCB->HashKey = HashKey;
    pHashCB->HashCompare = HashCompare;
    
   /* Allocate memory for hash table nodes */
    pHashCB->pList = (HASH_NODE_S *)osal_alloc((pHashCB->ulListNum+
                        pHashCB->ulFreeNum)*sizeof(HASH_NODE_S));
                        
    if(NULL == pHashCB->pList)
    {
        return SYS_ERR_FAILED;
    }
    
   /* Set the collision zone position */
    pHashCB->pFree = &(pHashCB->pList[ulListNum]);
    
    /* Initialize hash zone node */
    for(i=0;i<(pHashCB->ulListNum);i++)
    {
        pHashCB->pList[i].ulFlag = HASH_NODE_IN_FREE;
        pHashCB->pList[i].pData = NULL;
        pHashCB->pList[i].pNext = NULL;
    }
    
    /* Initialize the collision zone node */
    for(i=0;i<(pHashCB->ulFreeNum-1);i++)
    {
        pHashCB->pFree[i].ulFlag = HASH_NODE_IN_FREE;
        pHashCB->pFree[i].pData = NULL;
        pHashCB->pFree[i].pNext = &(pHashCB->pFree[i+1]);
    }

    pHashCB->pFree[ulFreeNum-1].ulFlag = HASH_NODE_IN_FREE;
    pHashCB->pFree[ulFreeNum-1].pData = NULL;
    pHashCB->pFree[ulFreeNum-1].pNext = NULL;
    
    return SYS_ERR_OK;
}


/*******************************************************************************
Function: ulong_t HashUsedCount
Description: Get the number of occupied nodes in the hash table
Input: HASH_S *pHashCB: Hash control block pointer
Output: None
Return: Occupied node count or 0xffffffff
*******************************************************************************/
uint32  HashUsedCount(HASH_S *pHashCB)
{
    if(NULL != pHashCB)
    {
        return pHashCB->ulCount;
    }
    
    return 0xffffffff;
}


/*******************************************************************************
Function: ulong_t HashFreeCount
Description: Get the number of free nodes in the hash table
Input: HASH_S *pHashCB: Hash control block pointer
Output: None
Return: Free node count or 0xffffffff
*******************************************************************************/
uint32  HashFreeCount(HASH_S *pHashCB)
{
    if(NULL != pHashCB)
    {
        return (pHashCB->ulListNum+pHashCB->ulFreeNum-pHashCB->ulCount);
    }
    
    return 0xffffffff;
}


/*******************************************************************************
Function: void *HashInsert
Description: Inserts a new node into the hash table
Input: HASH_S *pHashCB: Hash control block pointer
void *pId: Node data identifier structure pointer
Output: Adds a new node to the hash table or finds an existing node
Returns: Pointer to the node data area or NULL
*******************************************************************************/
void * HashInsert(HASH_S *pHashCB, void *pId)
{
    uint32 ulKey;
    HASH_NODE_S *pNode;
    
    if(NULL == pHashCB || NULL == pId)
    {
        return NULL;
    }
    
   /* Generate hash key */
    ulKey = pHashCB->HashKey(pId);
    
    if(ulKey >= pHashCB->ulListNum)
    {
        return NULL;
    }
    
    if(pHashCB->pList[ulKey].ulFlag == HASH_NODE_IN_FREE)
    {
       /* If the first node is idle, use it directly */
        if(NULL == (pHashCB->pList[ulKey].pData = pHashCB->HashMalloc(pId)))
        {
            return NULL;
        }
        pHashCB->pList[ulKey].ulFlag = HASH_NODE_IN_USED;
        pHashCB->ulCount++;
        return pHashCB->pList[ulKey].pData;
    }
    else
    {
        /*
        if(OK == pHashCB->HashCompare(pHashCB->pList[ulKey].pData,pId))
        {
            return pHashCB->pList[ulKey].pData;
        }
        */
        
        pNode = &(pHashCB->pList[ulKey]);
        /*Search the collision chain sequentially, 
        and return the data area pointer if the node already exists */
        while(NULL != pNode)
        {
            if(SYS_ERR_OK == pHashCB->HashCompare(pNode->pData,pId))
            {
                return pNode->pData;
            }
            pNode = pNode->pNext;
        }
        
        if(NULL != pHashCB->pFree)
        {
            /* Assign nodes from collision zone */
            pNode = pHashCB->pFree;
            if(NULL == (pNode->pData = pHashCB->HashMalloc(pId)))
            {
                return NULL;
            }
            pHashCB->pFree = pHashCB->pFree->pNext;
            
            /*Extract from collision area to hash area*/
            pNode->ulFlag = HASH_NODE_IN_USED;
            pNode->pNext = pHashCB->pList[ulKey].pNext;
            pHashCB->pList[ulKey].pNext = pNode;
            pHashCB->ulCount++;
            
            return pNode->pData;
        }
    }/* if(pHashCB->pList[ulKey].ulFlag == HASH_NODE_IN_FREE) */
    
    return NULL;
}


/*******************************************************************************
Function: ulong_t HashDelete
Description: Deletes a node from the hash table
Input: HASH_S *pHashCB: Hash control block pointer
void *pId: Pointer to the node data identifier structure
Output: Deletes the node from the hash table
Return: OK, ERROR
*******************************************************************************/
uint32  HashDelete(HASH_S *pHashCB,void *pId)
{
    uint32 ulKey;
    HASH_NODE_S *pNode;
    HASH_NODE_S *pTempNode;
    
    if(NULL == pHashCB || NULL == pId)
    {
        return SYS_ERR_FAILED;
    }
    
    /* Generate hash key */
    ulKey = pHashCB->HashKey(pId);
    
    if(ulKey >= pHashCB->ulListNum)
    {
        return SYS_ERR_FAILED;
    }
    
    if(pHashCB->pList[ulKey].ulFlag == HASH_NODE_IN_FREE)
    {
        /* The first node is empty, no matching node */
        return SYS_ERR_FAILED;
    }
    else
    {
        /* Determine whether the first node matches */
        if(SYS_ERR_OK == pHashCB->HashCompare(pHashCB->pList[ulKey].pData,pId))
        {
            pNode = pHashCB->pList[ulKey].pNext;
            
            if(NULL == pNode)
            {
                /* No successor node */
                pHashCB->HashFree(pHashCB->pList[ulKey].pData);
                pHashCB->pList[ulKey].ulFlag = HASH_NODE_IN_FREE;
                
                pHashCB->ulCount--;
                return SYS_ERR_OK;
            }
            else
            {
                /* Move the successor node data to the first position*/
                pHashCB->HashFree(pHashCB->pList[ulKey].pData);
                pHashCB->pList[ulKey].pData = pNode->pData;
                pHashCB->pList[ulKey].pNext = pNode->pNext;
                pNode->ulFlag = HASH_NODE_IN_FREE;
                pNode->pData = NULL;
                
                pNode->pNext = pHashCB->pFree;
                pHashCB->pFree = pNode;
                
                pHashCB->ulCount--;
                return SYS_ERR_OK;
            }
        }
        else
        {
            pNode = &(pHashCB->pList[ulKey]);
            
            while(NULL != pNode->pNext)
            {
                if(SYS_ERR_OK == pHashCB->HashCompare(pNode->pNext->pData,pId))
                {
                    pHashCB->HashFree(pNode->pNext->pData);
                    pNode->pNext->ulFlag = HASH_NODE_IN_FREE;
                    pTempNode = pNode->pNext;
                    pNode->pNext = pNode->pNext->pNext;
                    
                    pTempNode->pNext = pHashCB->pFree;
                    pHashCB->pFree = pTempNode;
                    
                    pHashCB->ulCount--;
                    return SYS_ERR_OK;
                }
                pNode = pNode->pNext;
            }
            return SYS_ERR_FAILED;
        }/* if(OK == HashCompare(pHashCB->pList[ulKey].pData,pId)) */
    }/* if(pHashCB->pList[ulKey].ulFlag == HASH_NODE_IN_FREE) */
}


/*******************************************************************************
Function: void * HashSearch
Description: Searches for a node in a hash table
Input: HASH_S *pHashCB: Hash control block pointer
void *pId: Node data identifier structure pointer
Output: Found an existing node
Return: Node data pointer or NULL
*******************************************************************************/
void * HashSearch(HASH_S *pHashCB, void *pId)
{
    uint32 ulKey;
    HASH_NODE_S *pNode;
    
    if(NULL == pHashCB || NULL == pId)
    {
        return NULL;
    }
    
    /* Generate hash key */
    ulKey = pHashCB->HashKey(pId);
    
    if(ulKey >= pHashCB->ulListNum)
    {
        return NULL;
    }

    if(pHashCB->pList[ulKey].ulFlag == HASH_NODE_IN_FREE)
    {
        /* The first node is empty, no matching node */
        return NULL;
    }
    else
    {
        if(SYS_ERR_OK == pHashCB->HashCompare(pHashCB->pList[ulKey].pData,pId))
            return pHashCB->pList[ulKey].pData;
            
        pNode = &(pHashCB->pList[ulKey]);
        /* Compare all nodes on the collision chain one by one */
        while(NULL != pNode)
        {
            if(SYS_ERR_OK == pHashCB->HashCompare(pNode->pData,pId))
            {
                return pNode->pData;
            }
            
            pNode = pNode->pNext;
        }
    }    
        
    return NULL;
}


/*******************************************************************************
Function: void HashClean
Description: Hash table cleanup function
Input: HASH_S *pHashCB: Hash control block pointer
Output: Free all node memory
Return: None
*******************************************************************************/
void HashClean(HASH_S *pHashCB)
{
    uint32 i;
    
    if(NULL == pHashCB)
    {
        return;
    }
    
    for(i=0;i<(pHashCB->ulListNum+pHashCB->ulFreeNum);i++)
    {
        /* Release node data area memory */
        if(NULL != pHashCB->pList[i].pData)
        {
            pHashCB->HashFree(pHashCB->pList[i].pData);
        }
    }
    
    /* Release all node memory */
    osal_free((void *)(pHashCB->pList));
    
    pHashCB->ulCount = 0;
    
    pHashCB->ulListNum = 0;
    pHashCB->ulFreeNum = 0;
    pHashCB->pList = NULL;
    pHashCB->pFree = NULL;
    
    pHashCB->HashMalloc = NULL;
    pHashCB->HashFree = NULL;
    pHashCB->HashKey = NULL;
    pHashCB->HashCompare = NULL;
    return;
}
#endif /*GBN_INCLUDE_DHCPRELAYSERVER*/
