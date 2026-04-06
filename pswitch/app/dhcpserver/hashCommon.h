/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  File Name::  DhcpHash.c
  Description: This file defines common HASH structures and HASH table operation functions
  Autor: Kniazkov Andrew
  Other Notes: This file can be used by other modules
********************************************************************************/

#ifndef _HASH_H
#define _HASH_H
#include <common/sys_def.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define HASH_NODE_IN_FREE 0
#define HASH_NODE_IN_USED 1

typedef struct tagHASH_NODE
{
    uint32 ulFlag;      /* Node occupation flag */
    void *pData;        /* Node data area pointer */
    
    struct tagHASH_NODE * pNext;/* Next node pointer */
}HASH_NODE_S;


typedef void *(*HashMallocFunc)(void *pId);
typedef void  (*HashFreeFunc)(void *pData);
typedef uint32 (*HashKeyFunc)(void *pId);
typedef uint32 (*HashCompareFunc)(void *pData,void *pId);


typedef struct tagHASH
{
    uint32 ulCount;     /* Occupied node count */
    uint32 ulListNum;   /* Number of hash zone nodes */
    uint32 ulFreeNum;    /* Number of collision zone nodes */
    
    HASH_NODE_S *pList;    /* Hash area memory pointer */
    HASH_NODE_S *pFree;   /* Collision zone memory pointer */

    HashMallocFunc HashMalloc;    /*Hash allocation function pointer*/
    HashFreeFunc HashFree;    /*Hash release function pointer*/
    HashKeyFunc HashKey;    /*Hash value generation function pointer*/
    HashCompareFunc HashCompare;    /*Hash comparison function pointer*/
}HASH_S;


extern void * HashInsert(HASH_S *,void *);
extern void * HashSearch(HASH_S *,void *);
extern uint32  HashDelete(HASH_S *,void *);
extern uint32  HashUsedCount(HASH_S *);
extern uint32  HashInit( HASH_S *,uint32,uint32,
                        HashMallocFunc,HashFreeFunc,
                        HashKeyFunc,HashCompareFunc );


#ifdef  __cplusplus
}
#endif

#endif /* _GBN_HASH_H */

