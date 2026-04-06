/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  FileName:  DhcpHash.c
  Autor: Kniazkov Andrew
  Version:1.0.0        
  Date:2024/04/09
  Description: This file defines hash functions, hash allocation functions,
               hash release functions, and hash comparison functions used
               inside the DHCP module
  Other notes: None
********************************************************************************/
#define GBN_INCLUDE_DHCPRELAYSERVER
#ifdef GBN_INCLUDE_DHCPRELAYSERVER
#include "dhcpHash.h"
#include <common/sys_def.h>
#include "dhcp.h"
#include <libsal/sal_dhcp_server.h>
#ifdef __cplusplus
extern "C"{
#endif

/*Function defifition*/
/*******************************************************************************
  Function: void hash_malloc_data(void * )
  Description: This function uses a HASH method to search for VLAN ID node
               data by MAC address, allocates memory and assigns values
  Inpit: pointer to a HASH table entry
  Output: None
  Return: On success, returns the pointer to the allocated memory;
          returns NULL on failure
*******************************************************************************/
void  *hash_malloc_data(void *pId)
{
    MACHASH_S *pData,*pTmpData;
    
    if( pId == NULL )
    {
        return NULL ;
    }
    pData = (MACHASH_S *) osal_alloc ( sizeof(MACHASH_S) );

    if(NULL == pData)
    {
                    
        DHCP_SERVER_DBG("\r\n Err Malloc memory for HashData in DHCPRELAY MODULE"); 
		
        return NULL;
     }

     pTmpData = (MACHASH_S *)pId;     

    /*Store the data from pId into allocated data area*/
    /* Here, we should not concern ourselves with the members inside the structure
    otherwise, when the structure is modified, this part would also need to be 
    changed accordngly*/
    #if 0
    memcpy ( pData->ucUserMac, pTmpData->ucUserMac, 6 );
    pData->usVlanId    = pTmpData->usVlanId;
    pData->ulIpAddr    = pTmpData->ulIpAddr;
    #endif
    memcpy (pData,pTmpData,sizeof(MACHASH_S));
    
     return (void *)pData;
}
	
    
      

/*******************************************************************************
  Function: void hash_free_data(void * )
  Description: Function to release VLAN ID node data by looking it up via 
               MAC address using HASH method
  Input: Pointer to a HASH table entry
  Output: None
  Return: 0 on success, other values indicate errors
*******************************************************************************/
void  hash_free_data(void *pData)
{
    MACHASH_S *pstData;
    
    pstData = pData;
    memset( (char *)pstData, 0, sizeof(MACHASH_S) );
    osal_free(pData);
    pData = NULL;
    return ;
}

/*******************************************************************************
  Function: ulong_t hash_key(void * )
  Description: HASH index generation function to lookup VLAN ID node 
               by MAC address using HASH method 
  Input: Pointer to a HASH table entry
  Output: None
  Return: Index obtained from the key 
  Others: The index is generated using the remainder method: the lower 24 bits
          of the MAC address are used as the key, and modulo HASHFACTOR is taken
*******************************************************************************/
ulong_t  hash_key (void *pId )
{
    MACHASH_S *pData;
    ulong_t ulHashKey,ulHashIndex;
    ulong_t ulTmp[3];
    uchar_t *pUserMac;
    uchar_t i;

    if(pId == NULL)
    {
        return 0XFFFFFFFF;  /*invalid index*/
    }
    
    /*Get pointer to an array representing 48-bit MAC address*/
    pData = (MACHASH_S *)pId;
    pUserMac = &(pData->ucUserMac[0]);

    /*Extract the lower 24 bits of the MAC address to form the key*/
    /* ulHashKey=((0x0000000f&((ulong_t)*(pUserMac+3)))<<16) + ((0x000000ff&((ulong_t)*(pUserMac+4)))<<8)
      + ((ulong_t)*(pUserMac+5)); */ 
    for(i = 0; i <= 2; i++)
    {
        ulTmp[i] = (ulong_t) *(pUserMac+i+3); 
    }
    ulHashKey = ((0x000000ff&ulTmp[0]) << 16) + ((0x000000ff&ulTmp[1]) << 8) + ulTmp[2];

    /*Calculate HASH table index*/
    ulHashIndex = ulHashKey % HASH_FACTOR;

    return ulHashIndex;
}


/*******************************************************************************
  Function: ulong_t hash_compare_key(void *��void * )
  Description: A key comparison function to lookup VLAN ID node by MAC address
               using HASH method
  Input: Pointers to two node data to be compared, HASH table entry
  Output: None
  Return: OK indicates the two are the same, Error indicates they are different
*******************************************************************************/
ulong_t  hash_compare_key(void *pData,void *pId)
{
    MACHASH_S *pHashData1,*pHashData2;
    ulong_t i;

    if((pData == NULL)||(pId == NULL))
    {
         return ERROR;
    }
    pHashData1=(MACHASH_S *)pData;
    pHashData2=(MACHASH_S *)pId;

    /*Compare whether the MAC addresses in two HASH nod data structures are the same*/
    for(i = 0; i <= 5; i++)
    {
        if( pHashData1->ucUserMac[i] != pHashData2->ucUserMac[i] )
        {
            return SYS_ERR_FAILED;
        }
    }

    return SYS_ERR_OK;
}    
        
        
#ifdef __cplusplus
extern }
#endif

#endif /*GBN_INCLUDE_DHCPRELAYSERVER*/
