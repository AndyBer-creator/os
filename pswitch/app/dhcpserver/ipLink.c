/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  File Name::  ipLink.c
  Description: This file defines the functions used by the address pool management module to manage address chains, etc.
  and the interface functions provided to the DHCP server.
  Autor: Kniazkov Andrew
  Other Notes: None.
********************************************************************************/
#include <common/sys_def.h>
#include "dhcps.h"
#ifdef  GBN_INCLUDE_IPPOOL
#include "ip_pool.h"
#include "dhcprelayserver.h"
#include <libsal/sal_util.h>
#include <libsal/sal_dhcp_server.h>


extern AAA_IPPOOL_S gIpPool[IPPOOL_MAX_POOL_NUM];

#define ROW_ACTIVE        1
#undef  SUCCESS
#undef  FAILURE
#define SUCCESS 0
#define FAILURE -1
#define IPPOOL_RELEASE_FLAG 0

/*Global variable definition*/
ulong_t gulCurIndex[2] = {0,0};
ulong_t gulConflictIndex[2] = {0,0};
ulong_t gCycleTime=0,gCycleTime2=0 ;

ULONG gulDhcpClientNum = 0;
dhcp_snooping_mac_lst_t dhcp_snooping_mac_list_head;
extern AAA_IPPOOL_S gIpPool[IPPOOL_MAX_POOL_NUM];
extern ulong_t ipPool_RowStatus[IPPOOL_MAX_POOL_NUM];

STATUS ipPoolChangeIpStatusInSection(ulong_t ulPoolIndex,ulong_t ulIpSectIndex,
                                    ushort_t usIndex ,uchar_t ucStatus,uchar_t ucLeaseFlag);

#if 1
ULONG dhcp_snooping_addTo_macList(uchar_t *mac, ULONG ulIpAddress,USHORT usVlanID,char* pUserName)
{
    dhcp_snooping_mac_lst_t *head=&dhcp_snooping_mac_list_head, *node;
    int result = 0;

    if(gulDhcpClientNum >= 3000)
    {
        return 3;
    }

    for( ; head->next; head = head->next)
    {
        result = memcmp(mac, head->next->mac, 6);
        if((0 == result)&&(usVlanID == head->next->usVlanID))
        {
            return 2;
        }
        else if(result < 0)/*Find the insertion position*/
        {
            break;
        }
    }
    /*Insert a new node into the linked list*/
    node = (dhcp_snooping_mac_lst_t*)calloc(1, sizeof(dhcp_snooping_mac_lst_t));
    if(NULL == node)
    {
        return ERROR;
    }
    node->ulIpAddress = ulIpAddress;
    memcpy(node->mac, mac, 6);
    strcpy((char *)node->szUserName, pUserName);
    node->usVlanID = usVlanID;
    node->next = head->next;
    head->next = node;

    gulDhcpClientNum++;

    return OK;
}

int dhcp_snooping_delfrom_macList(uchar_t *mac,USHORT usVlanID)
{
    dhcp_snooping_mac_lst_t *head=&dhcp_snooping_mac_list_head, *node;
    uchar_t ucFound = FALSE;

    for( ; head->next; head = head->next)
    {
        if((0 == memcmp(mac, head->next->mac, 6))&&(usVlanID == head->next->usVlanID))
        {
            node = head->next;
            head->next = node->next;
            free((char*)node);
            ucFound = TRUE;
            gulDhcpClientNum--;
            break;
        }
    }
    if(ucFound == TRUE)
        return OK;
    else
        return ERROR;
}

int dhcp_snooping_clear_macList(void)
{
    dhcp_snooping_mac_lst_t *head=&dhcp_snooping_mac_list_head, *node;
    int rc = 0;

    for( node=head->next; node; node=head->next)
    {
        head->next = node->next;
        free((char*)node);
        rc++;
    }
    gulDhcpClientNum = 0;
    return rc;
}

int dhcp_snooping_macList_get(ushort_t usStart,
    ushort_t *pusNextStart, ushort_t *pusBufNum,
    dhcp_snooping_mac_lst_t nodeBuf[IPPOOL_MAC_BUF_LEN])
{
    unsigned int             i = 0, bufIndex = 0;
    dhcp_snooping_mac_lst_t *head = &dhcp_snooping_mac_list_head, *node;

    *pusBufNum = 0;
    *pusNextStart = 0;

    for(node=head->next; node; node=node->next)
    {
        if ( ++i >= usStart )
        {
            memcpy(&nodeBuf[bufIndex++], node, sizeof(dhcp_snooping_mac_lst_t));
            if ( bufIndex >= IPPOOL_MAC_BUF_LEN )
            {
                node=node->next;
                break;
            }
        }
    }

    *pusBufNum = bufIndex;
    if ( NULL != node )
    {
        *pusNextStart = i + 1;
    }

    return OK;
}

BOOL dhcp_snooping_macList_item_exist( uchar_t *mac, ulong_t ip )
{
    dhcp_snooping_mac_lst_t *head = &dhcp_snooping_mac_list_head;

    for( ; head->next; head = head->next )
    {
        if((0 == memcmp(mac, head->next->mac, 6)) && (ip == head->next->ulIpAddress))
        {
            return TRUE;
        }
    }
    return FALSE;
}

BOOL dhcps_ip_is_bound(ULONG ulIpAddress,USHORT usVlanID)
{
    dhcp_snooping_mac_lst_t *head = &dhcp_snooping_mac_list_head,*node;

    for(node=head->next; node; node=node->next)
    {
        if((ulIpAddress == node->ulIpAddress)&&(usVlanID == node->usVlanID))
        {
            return TRUE;
       }
    }
    return FALSE;
}
#endif

ULONG dhcps_fund_ipbind_by_mac(uchar_t mac[6],ULONG *pulIpAddress,USHORT usVlanID)
{
    dhcp_snooping_mac_lst_t *head = &dhcp_snooping_mac_list_head,*node;

    for(node=head->next; node; node=node->next)
    {
        if((0 == memcmp(mac, node->mac, 6))&&(usVlanID == node->usVlanID))
        {
            *pulIpAddress = node->ulIpAddress;
            return SYS_ERR_OK;
       }
    }
    return SYS_ERR_FAILED;
}


void IPPOOL_Error_static(ulong_t i,uchar_t ucStatus,uchar_t ucFlag)
{
	if(IPPOOL_RELEASE_FLAG == ucFlag)
	{
		switch(ucStatus)
		{
			case IPPOOL_IPSTATUS_CONFLICT :
	            gIpPool[i].IpPoolMsgNum[IpPool_release_IN_CONFLICT]++;
				break;
			case IPPOOL_IPSTATUS_INVALID:
	            gIpPool[i].IpPoolMsgNum[IpPool_release_IN_INVALID]++;
				break;
			case IPPOOL_IPSTATUS_DISABLE:
	            gIpPool[i].IpPoolMsgNum[IpPool_release_IN_DISABLE]++;
				break;
			case IPPOOL_IPSTATUS_UNUSED:
	            gIpPool[i].IpPoolMsgNum[IpPool_release_IN_UNUSED]++;
					break;
			default:
	            gIpPool[i].IpPoolMsgNum[IpPool_release_IN_UNKNOW]++;
				break;
		}
	}
	else
	{
		switch(ucStatus)
		{
			case IPPOOL_IPSTATUS_CONFLICT :
	            gIpPool[i].IpPoolMsgNum[IpPool_Alloc_IN_CONFLICT]++;
				break;
			case IPPOOL_IPSTATUS_INVALID:
	            gIpPool[i].IpPoolMsgNum[IpPool_Alloc_IN_INVALID]++;
				break;
			case IPPOOL_IPSTATUS_DISABLE:
	            gIpPool[i].IpPoolMsgNum[IpPool_Alloc_IN_DISABLE]++;
				break;
			case IPPOOL_IPSTATUS_USED:
	            gIpPool[i].IpPoolMsgNum[IpPool_Alloc_IN_USED]++;
					break;
			default:
	            gIpPool[i].IpPoolMsgNum[IpPool_Alloc_IN_UNKNOW]++;
				break;
		}

	}
}



/************************************************************
Function: InsertToUsedLink
Description: Inserts an index value into the used list, adding it to the head.
Input: Pointer to the linked list structure.
Output: This function is used when an address is allocated.
Return: This function must be used after the free list has been modified.
Others: Rules: When inserting into the linked list, set or initialize the lease flag.
**************************************************************/
STATUS InsertToUsedLink(IpPoolLink_S *pIpLink,ushort_t usIndex,uchar_t ucLeaseFlag)
{
    if(usIndex >= pIpLink->usMaxLength)
         return ERROR;
    ipPoolClearLeaseCount(pIpLink, usIndex);
    ipPoolSetLeaseFlag(pIpLink, usIndex, ucLeaseFlag);
    if(pIpLink->usUsedHead >= pIpLink->usMaxLength)
    {
         pIpLink->pusIpLink[usIndex].usNext = (ushort_t)-1;
         pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
         pIpLink->usUsedHead = usIndex;
         return OK;
     }
     pIpLink->pusIpLink[usIndex].usNext = pIpLink->usUsedHead;
     pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
     pIpLink->pusIpLink[pIpLink->usUsedHead].usPre = usIndex;
     pIpLink->usUsedHead = usIndex;
     return OK;
}

/************************************************************
  Function:      FreeToIdleLink
  Description:  Put an index into the free list and delete it from the used list
  Input:         Pointer to the free list structure, index value to be restored
  Output:
  Return:
  Others:
**************************************************************/
STATUS FreeToIdleLink(IpPoolLink_S *pIpLink,ushort_t usIndex)
{
    uchar_t ucFlag =0;
    if(usIndex >= pIpLink->usMaxLength)
         return FAILURE;

    ipPoolGetLeaseFlag(pIpLink, usIndex,&ucFlag);
    //This line is added to the function that deletes a node from the linked list.
 	ipPoolSetLeaseFlag(pIpLink, usIndex, LEASE_DISABLE);
    //The following is the operation on the free list, inserting the specified node into the head of the list
    ipPoolClearLeaseCount(pIpLink, usIndex);
	//If the lease does not work, put the released address at the head of the free list
	//If the lease is valid, the released address is placed at the end of the free list.
    if(ucFlag == LEASE_DISABLE)
    {
		if(pIpLink->usIdleHead >= pIpLink->usMaxLength)
	     {
			 pIpLink->pusIpLink[usIndex].usNext = (ushort_t)-1;
	         pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
	         pIpLink->usIdleHead = usIndex;
   	         pIpLink->usIdleTail = usIndex;
	         return SUCCESS;
	     }
		//The head pointer and the tail pointer point to one node, or there can be multiple nodes between the head and tail pointers.
	     pIpLink->pusIpLink[usIndex].usNext = pIpLink->usIdleHead;
	     pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
	     pIpLink->pusIpLink[pIpLink->usIdleHead].usPre = usIndex;
	     pIpLink->usIdleHead = usIndex;
	     /*This part of the operation puts the released address at the head of the free list*/
    }
    else
    {
		if(pIpLink->usIdleTail >= pIpLink->usMaxLength)
		{
			 pIpLink->pusIpLink[usIndex].usNext = (ushort_t)-1;
	         pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
	         pIpLink->usIdleHead = usIndex;
   	         pIpLink->usIdleTail = usIndex;
	         return SUCCESS;
		}
	     pIpLink->pusIpLink[usIndex].usNext =(ushort_t)-1;
	     pIpLink->pusIpLink[usIndex].usPre = pIpLink->usIdleTail;
	     pIpLink->pusIpLink[pIpLink->usIdleTail].usNext = usIndex;
	     pIpLink->usIdleTail = usIndex;
    }
     return SUCCESS;
}



/************************************************************
Function: InsertToConflictLink
Description: Inserts an index value into the active linked list, adding it to the head.
Input: Pointer to the linked list structure.
Output: This function is used when an address is allocated.
Return: This function must be used after the free linked list has been modified.
**************************************************************/
STATUS InsertToConflictLink(IpPoolLink_S *pIpLink,ushort_t usIndex,uchar_t ucLeaseFlag)
{
    if(usIndex >= pIpLink->usMaxLength)
         return FAILURE;
    ipPoolClearLeaseCount(pIpLink, usIndex);
    ipPoolSetLeaseFlag(pIpLink, usIndex, ucLeaseFlag);
    if(pIpLink->usConflictHead >= pIpLink->usMaxLength)
    {
         pIpLink->pusIpLink[usIndex].usNext = (ushort_t)-1;
         pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
         pIpLink->usConflictHead = usIndex;
         return SUCCESS;
     }
     pIpLink->pusIpLink[usIndex].usNext = pIpLink->usConflictHead;
     pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
     pIpLink->pusIpLink[pIpLink->usConflictHead].usPre = usIndex;
     pIpLink->usConflictHead= usIndex;
     return SUCCESS;
}


/************************************************************
Function: DelFromUsedLink
Description: Adds an index to the free list and deletes it from the used list.
Input: Pointer to the free list structure, index value to be restored.
Output: Since the used list and the free list share the same space,
Return: When performing linked list operations, pay attention to the order of operations on the two lists.
Others: When allocating a free address, the index value
        is deleted from the free list and placed at the head of the used list.
        When freeing an address, the node is deleted from the used list and added to the head of the free list.
**************************************************************/
STATUS DelFromUsedLink(IpPoolLink_S *pIpLink,ushort_t usIndex,ulong_t ucPool,ulong_t ucSect)
{
    (void)ucPool;
    (void)ucSect;
   /*The following is the operation on the linked list being used to delete the specified node from the linked list*/
    if(pIpLink == NULL)
    	return FAILURE;
    if(pIpLink->usUsedHead >= pIpLink->usMaxLength)
    {
        return FAILURE;
    }

    if(SUCCESS != ipPoolClearLeaseCount(pIpLink, usIndex))         //  Add judgment on function execution results
    {
        return FAILURE;
    }
    // The reason for commenting this line of code is that when processing the free list,
    // a real lease flag is required. This line of code is executed before the free list is processed.
	//IPPool_SetLeaseFlag(pIpLink, usIndex, LEASE_DISABLE);
    if(usIndex == pIpLink->usUsedHead)//Delete the head node of the linked list
    {
    	if(IPPOOL_NULL_USHORT == pIpLink->pusIpLink[usIndex].usNext)
		{//The head node and tail node are combined into one
			pIpLink->usUsedHead = IPPOOL_NULL_USHORT;
			return SUCCESS ;
    	}
        pIpLink->usUsedHead = pIpLink->pusIpLink[usIndex].usNext;
        pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usNext].usPre
                            = (ushort_t)-1;
        pIpLink->pusIpLink[usIndex].usNext = (ushort_t)-1;
        return SUCCESS;
    }
	// If the node to be deleted is the tail node of the linked list, its back pointer is connected to ground.
    // When handling such nodes, treat them differently from the middle nodes of the linked list.
    // If the program reaches this point, it means that the node to be deleted is not the head node.
	if(IPPOOL_NULL_USHORT  == pIpLink->pusIpLink[usIndex].usNext)
	{
		pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usPre].usNext = (ushort_t)-1;
		pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
		return SUCCESS;
	}
	//If the program reaches this point, it means that the node to be deleted is the middle node in the joint table.
    pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usPre].usNext
                     = pIpLink->pusIpLink[usIndex].usNext;
    pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usNext].usPre
                    = pIpLink->pusIpLink[usIndex].usPre;
    pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
    pIpLink->pusIpLink[usIndex].usNext= (ushort_t)-1;
    return SUCCESS;
}



/************************************************************
Function: DelFromConflictLink
Description: Adds an index to the free list and deletes it from the used list.
Input: Pointer to the conflict list structure, index value to be restored.
Output: Since the used list and the free list share the same space,
Return: When performing linked list operations, pay attention to the order of operations on the two lists.
Others: When allocating a free address, the index value
        is deleted from the free list and placed at the head of the used list.
        When freeing an address, the node is deleted from the used list and added to the head of the free list.
**************************************************************/
STATUS DelFromConflictLink(IpPoolLink_S *pIpLink,ushort_t usIndex,ulong_t ulPool,ulong_t ulSect)
{

    (void)ulPool;
    (void)ulSect;
    //The following is an operation on the linked list being used to delete the specified node from the linked list
    if(pIpLink == NULL)
    	return ERROR;
    if(pIpLink->usConflictHead>= pIpLink->usMaxLength)
    {
        return ERROR;
    }

    if(OK != ipPoolClearLeaseCount(pIpLink, usIndex))         //  Add judgment on function execution results
    {
        return ERROR;
    }
    /* This line of code is commented out because when processing the free list, a real lease flag is required.
        This line of code is executed before the free list is processed.        
       // ipPoolSetLeaseFlag(pIpLink, usIndex, LEASE_DISABLE);*/
    /* Delete the head node of the linked list*/
    if(usIndex == pIpLink->usConflictHead)
    {
    	if(IPPOOL_NULL_USHORT == pIpLink->pusIpLink[usIndex].usNext)
		{
			pIpLink->usConflictHead = IPPOOL_NULL_USHORT;
			return OK ;
    	}
        pIpLink->usConflictHead = pIpLink->pusIpLink[usIndex].usNext;
        pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usNext].usPre
                            = (ushort_t)-1;
        pIpLink->pusIpLink[usIndex].usNext = (ushort_t)-1;
        return OK;
    }
	/* If the node to be deleted is the tail node of the linked list, its back pointer is connected to ground.
        When handling such nodes, they need to be treated differently from the middle nodes of the linked list.
        If the program reaches this point, it means that the node to be deleted is not the head node.*/
	if(IPPOOL_NULL_USHORT  == pIpLink->pusIpLink[usIndex].usNext)
	{
		pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usPre].usNext = (ushort_t)-1;
		pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
		return OK;
	}
	//If the program reaches this point, it means that the node to be deleted is the middle node in the joint table.
    pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usPre].usNext
                     = pIpLink->pusIpLink[usIndex].usNext;
    pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usNext].usPre
                    = pIpLink->pusIpLink[usIndex].usPre;
    pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
    pIpLink->pusIpLink[usIndex].usNext= (ushort_t)-1;
    return OK;
}


/************************************************************
Function: AllocFromIdleLink
Description: Gets an index value from the free list.
Input: Pointer to the free list structure.
Output: The obtained index value, allocated starting from the head of the list.
**************************************************************/
STATUS AllocFromIdleLink
(
IpPoolLink_S *pIpLink,
ushort_t *pusIndex,
ulong_t ulPoolIndex,
ulong_t ucSectIndex,
BOOL cantBeBound,
ulong_t vid,
ulong_t startIp
)
{
    (void)ulPoolIndex;
    (void)ucSectIndex;
    if(pIpLink == NULL)
        return ERROR;
   /* The linked list is already empty*/
    if(pIpLink->usIdleHead >= pIpLink->usMaxLength)
    {
        return ERROR;
    }
    if(cantBeBound)
    {
        ushort_t cur;

        /*The free list only has the last node*/
        if(pIpLink->usIdleHead == pIpLink->usIdleTail)
        {
            if(dhcps_ip_is_bound(pIpLink->usIdleHead+startIp,vid))
            {
                return ERROR;
            }
            *pusIndex = pIpLink->usIdleHead ;
            ipPoolClearLeaseCount(pIpLink, pIpLink->usIdleHead);
            pIpLink->usIdleHead = IPPOOL_NULL_USHORT;
            pIpLink->usIdleTail = IPPOOL_NULL_USHORT;
            pIpLink->pusIpLink[*pusIndex].usNext = (ushort_t) -1;
            pIpLink->pusIpLink[*pusIndex].usPre  = (ushort_t) -1;
            return OK;
        }
        cur = pIpLink->usIdleHead;
        while(1)
        {
            if(!dhcps_ip_is_bound(cur+startIp,vid))
            {
                break;
            }
            cur = pIpLink->pusIpLink[cur].usNext;
            if( cur == (ushort_t) -1)
                return ERROR;
        }

        *pusIndex = cur ;
        ipPoolClearLeaseCount(pIpLink, cur);
        if (cur == pIpLink->usIdleHead) //头结点
        {
            pIpLink->usIdleHead = pIpLink->pusIpLink[cur].usNext;
            pIpLink->pusIpLink[cur].usPre = (ushort_t)-1;
            pIpLink->pusIpLink[cur].usNext = (ushort_t) -1;
        }
        else // 中间节点
        {
            pIpLink->pusIpLink[pIpLink->pusIpLink[cur].usPre].usNext = pIpLink->pusIpLink[cur].usNext;
            if(pIpLink->pusIpLink[cur].usNext != (ushort_t)-1)
                pIpLink->pusIpLink[pIpLink->pusIpLink[cur].usNext].usPre = pIpLink->pusIpLink[cur].usPre;

            pIpLink->pusIpLink[cur].usPre = (ushort_t)-1;
            pIpLink->pusIpLink[cur].usNext = (ushort_t) -1;
        }

        return OK;

    }
    else
    {
        /*The free list only has the last node*/
        if(pIpLink->usIdleHead == pIpLink->usIdleTail)
        {
            *pusIndex = pIpLink->usIdleHead ;
            ipPoolClearLeaseCount(pIpLink, pIpLink->usIdleHead);
            pIpLink->usIdleHead = IPPOOL_NULL_USHORT;
            pIpLink->usIdleTail = IPPOOL_NULL_USHORT;
            pIpLink->pusIpLink[*pusIndex].usNext = (ushort_t) -1;
            pIpLink->pusIpLink[*pusIndex].usPre  = (ushort_t) -1;
            return OK;
        }
        *pusIndex = pIpLink->usIdleHead ;
        ipPoolClearLeaseCount(pIpLink, pIpLink->usIdleHead);
        pIpLink->usIdleHead = pIpLink->pusIpLink[*pusIndex].usNext;
        pIpLink->pusIpLink[pIpLink->usIdleHead].usPre = (ushort_t)-1;
        pIpLink->pusIpLink[*pusIndex].usNext = (ushort_t) -1;
        return OK;
    }
}

/************************************************************
Function: AllocIpAddrFromPOOL
Description: Allocates an address from an address pool
Input: Address pool number
Output: Allocated IP address
**************************************************************/
STATUS AllocIpAddrFromPool
(
ulong_t ulPool,
ulong_t * pIpAddr,
uchar_t ucLeaseFlag,
BOOL cantBeBound,
ulong_t vid
)
{

    AAA_IPSection_S *pIpSect ;
    ulong_t i;
    uchar_t ucTemp[50];
    ulong_t ulIp;
    ulong_t ulError = IP_GENERAL_ERROR;

    if(NULL == pIpAddr)
        return IP_GENERAL_ERROR;
    if(ulPool >= IPPOOL_MAX_POOL_NUM)
    {
        DHCP_SERVER_DBG("AllocIpAddrFromPool->pool Index %d has gone beyond the range", ulPool);
        return IP_GENERAL_ERROR;
    }

    /* Empty name indicates invalid address pool record */
    if(0 == strlen((char *)gIpPool[ulPool].szName))
    {
        DHCP_SERVER_DBG("AllocIpAddrFromPool->the name of pool %d is empty",ulPool);
        return POOL_NAME_ERROR;
    }

    gIpPool[ulPool].IpPoolMsgNum[IpPool_request_alloc_Ip]++;

    pIpSect = &(gIpPool[ulPool].IpSection[0]);
    for(i = 0; i<IPPOOL_MAX_IPSection_NUM ; i++)
    {
        if((0 == (pIpSect+i)->usLength )||((pIpSect+i)->ulSectionRowStatus!=ROW_ACTIVE))
            continue ;
        ulError = AllocIpAddrFromIpSection((pIpSect+i),&ulIp,ucLeaseFlag,ulPool,(uchar_t)i, cantBeBound,vid);
        if(OK == ulError)
        {
            ipPoolIPtoStr(ulIp, ucTemp);
            *pIpAddr = ulIp;
            gIpPool[ulPool].IpPoolMsgNum[IpPool_Alloc_success]++;
            DHCP_SERVER_DBG("AllocIpAddrFromPool->alloc ip %s from section %d pool %d",
                               ucTemp,i,ulPool);
            return OK;//Successfully obtain an address from an address segment and return success
        }
    }
    /*Return on failure*/
    gIpPool[ulPool].IpPoolMsgNum[IpPool_NO_FREE_IP]++;

gIpPool[ulPool].IpPoolMsgNum[IpPool_Alloc_fail]++;
return ulError ;//No address was obtained from any address segment, and a failure was returned.

}

/************************************************************
Function: FreeIpAddrFromPOOL
Description: Frees an address
Input: Address pool number, address to be freed
Others: If the address is not in the address pool, the function returns failure.
        If the address status is conflicting, indicating that the address was not allocated from the address pool, 
        it cannot be freed and therefore returns failure. If the address is already free, the function returns success.
T       he return value of this function requires consideration.
**************************************************************/
STATUS FreeIpAddrToPool(ulong_t ulPoolIndex,ulong_t ulIpAddr)
{
    ulong_t  ulSectIndex ;
    ushort_t usIndex ;//The offset of the address in the address segment. The free list can directly use this index value.
    uchar_t  ucStatus;
    char szIp[20];
    AAA_IPSection_S *pIpSect ;

    if(ulPoolIndex >= IPPOOL_MAX_POOL_NUM)
    {
        DHCP_SERVER_DBG("FreeIpAddrToPool->pool Index %d has gone beyond the range",ulPoolIndex);
        return IP_GENERAL_ERROR;
    }

    /* Empty name indicates invalid address pool record */
    if(0 == strlen((char *)gIpPool[ulPoolIndex].szName))
    {
        return POOL_NAME_ERROR;
    }

    gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_request_release_ip]++;

    if(OK != FindIpSectionByIp(ulPoolIndex,ulIpAddr,&ulSectIndex,&usIndex))
    {
        DHCP_SERVER_DBG("FreeIpAddrToPool-> Fail to find ip %s",ipPoolIPtoStr(ulIpAddr,(uchar_t *) szIp));
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_release_ip_fail]++;
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_release_NOT_IN_POOL]++;
        return IP_NOT_IN_POOL;/* Failed to check the bitmap, abnormal return failure*/
    }

    pIpSect = &(gIpPool[ulPoolIndex].IpSection[ulSectIndex]);

    /*The address offset needs to be converted into an index in the bitmap*/
    GetIpBitmapState(&(pIpSect->Bitmap), (usIndex+SC_MIN_IPPOOL_ALLOCINDEX), &ucStatus);
    switch (ucStatus)
    {
        case IPPOOL_IPSTATUS_USED:
            FreeToIpSectionBitmap( &(pIpSect->Bitmap), (usIndex+SC_MIN_IPPOOL_ALLOCINDEX) );
            DelFromUsedLink(&(pIpSect->stIpLink), usIndex,ulPoolIndex,ulSectIndex);//First delete from the usage list
            FreeToIdleLink(&(pIpSect->stIpLink), usIndex);//Inserting into the free list
            pIpSect->ulInUsed--;
            pIpSect->ulUnUsed++;
            DHCP_SERVER_DBG("FreeIpAddrToPool->free ip to pool %d success",ulPoolIndex);
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_release_ip_success]++;
            return OK;
        /*0052 add conflicting branches*/
        case IPPOOL_IPSTATUS_CONFLICT:
            FreeToIpSectionBitmap( &(pIpSect->Bitmap), (usIndex+SC_MIN_IPPOOL_ALLOCINDEX) );
            DelFromConflictLink(&(pIpSect->stIpLink), usIndex,ulPoolIndex,ulSectIndex);//First delete from the conflict list
            FreeToIdleLink(&(pIpSect->stIpLink), usIndex);//Inserting into the free list
            pIpSect->ulConflict--;
            pIpSect->ulUnUsed++;
            DHCP_SERVER_DBG("FreeIpAddrToPool->free ip to pool %d success",ulPoolIndex);
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_release_ip_success]++;
            return OK;

        default :
           	IPPOOL_Error_static(ulPoolIndex,ucStatus, IPPOOL_RELEASE_FLAG);
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_release_ip_fail]++;
            return ERROR;
    }
}

/************************************************************
Function: AllocIpAddrFromIpSection
Description: Allocates an address from the address pool
Input: Address pool pointer
Output: Outputs an IP address. If the return value is successful, the address is valid.
**************************************************************/
STATUS AllocIpAddrFromIpSection
(
AAA_IPSection_S *pIpSect,
ulong_t *pIpAddr,
uchar_t ucLeaseFlag,
ulong_t ulPoolIndex,
uchar_t ulSectIndex,
BOOL cantBeBound,
ulong_t vid)
{
    ushort_t usIndex ;//Indexes related to linked lists
    uchar_t  ucTemp[30];

    if(pIpSect == NULL)
        return IP_GENERAL_ERROR;

    /*Assign a free index from the free list*/
    while(OK == AllocFromIdleLink(&(pIpSect->stIpLink), &usIndex,ulPoolIndex,ulSectIndex,cantBeBound,vid,pIpSect->ulStartIp))
    {
        /* If a free index is obtained, it is not possible to check whether the address is already
        If there is a conflict, modify the corresponding bitmap and statistics variables*/

        *pIpAddr = usIndex + pIpSect->ulStartIp;
        pIpSect->ulInUsed++;
        pIpSect->ulUnUsed--;

       /*Modify the linked list and insert this node into the linked list*/
        InsertToUsedLink(&(pIpSect->stIpLink), usIndex,ucLeaseFlag);

       /*Modify the corresponding bitmap to use status*/
        ipPoolChangeIpSectionBitMap(&(pIpSect->Bitmap),
                      (usIndex+SC_MIN_IPPOOL_ALLOCINDEX),
                      IPPOOL_IPSTATUS_USED);

        DHCP_SERVER_DBG("AllocIpAddrFromIpSection->success get ip %s ",ipPoolIPtoStr(*pIpAddr,ucTemp));
        return OK ;//After two checks, the address can be assigned to the user.

    }

    DHCP_SERVER_DBG ("AllocIpAddrFromIpSection->no Idle Ip Address");

    return  NO_FREE_IP;//If there is no return in the loop, the result must be failure
}

/************************************************************
Function: FindIpSectionByIp
Description: Finds the address segment of an IP address.
Input: Address segment group number, address
Output: Address segment index, address offset. This offset is not related to the bitmap.
        If it is the first address, the offset is 0.
**************************************************************/
STATUS FindIpSectionByIp(ulong_t ulPoolIndex,ulong_t ulIpAddr,ulong_t *pulSectIndex,ushort_t *pIndex)
{
    AAA_IPSection_S *pIpSect ;
    ulong_t i;
    uchar_t ucTemp[30];

    if(ulPoolIndex>= IPPOOL_MAX_POOL_NUM)
        return ERROR;
    pIpSect = &(gIpPool[ulPoolIndex].IpSection[0]);
    for(i = 0; i<IPPOOL_MAX_IPSection_NUM ; i++)
    {
        /*Address segment is empty*/

		DHCP_SERVER_DBG("IpSection %d ulStartIp %x, ulEndIp %x Status %d  ", i, (pIpSect+i)->ulStartIp, (pIpSect+i)->ulEndIp, (pIpSect+i)->ulSectionRowStatus);
        if((0 == (pIpSect+i)->usLength )||((pIpSect+i)->ulSectionRowStatus!=ROW_ACTIVE))
            continue ;
        if((ulIpAddr >= (pIpSect+i)->ulStartIp)&&(ulIpAddr <= (pIpSect+i)->ulEndIp))
        {
            *pulSectIndex = i;
            *pIndex = (ushort_t)(ulIpAddr -(pIpSect+i)->ulStartIp);//Get the offset of the address
            return OK ;
        }
    }
    //ippool_debug_printf ("\r\n FindIpSectionByIp->IPaddr %s is not in pool %d",ipPoolIPtoStr(ulIpAddr, ucTemp),ulPoolIndex);
    DHCP_SERVER_DBG ("FindIpSectionByIp->IPaddr %s is not in pool %d",ipPoolIPtoStr(ulIpAddr, ucTemp),ulPoolIndex);
    return ERROR ;
}



/*Function written for dhcp server*/

/************************************************************
Function: ipPoolGetPoolIndexByGateIp
Description: Uses the gateway IP address to find the address pool index.
Input: ulGatewayIp (gateway address pool)
       *pucPoolIndex (output address pool index)
Output:
Return: success (if successful, the output index value can be used).
If failed, the output value cannot be used.
Others:
**************************************************************/
STATUS ipPoolGetPoolIndexByGateIp(ulong_t ulGatewayIp, ulong_t *pulPoolIndex )
{
#if 1
    //STATUS rc = SYS_ERR_OK;
    ulong_t i,ulGetGatewayIp,ulGetRowstatus;
    char szIp[20];
    if(ulGatewayIp == 0)
        return ERROR ;

    for(i=0; i< IPPOOL_MAX_POOL_NUM; i++)
    {
        /*The address pool exists and the gateway meets the input requirements*/
        //rc = ccdGet("gbnL3IPPool:ipPoolRowStatus", &i,1, &ulGetRowstatus,&len);
		//ulGetRowstatus = ipPool_RowStatus[i];
        //if(ulGetRowstatus!=ROW_ACTIVE)
        //    continue;
        //rc = ccdGet("gbnL3IPPool:ipPoolGatewayIp", &i,1, &ulGetGatewayIp,&len);
        ulGetGatewayIp = gIpPool[i].ulGatewayIp;
        if(ulGatewayIp == ulGetGatewayIp)
            break;
    }
    if(i > IPPOOL_MAX_POOL_NUM)
    {
        DHCP_SERVER_DBG ("ipPoolGetPoolIndexByGateIp->this gateway ip %s does not exist", ipPoolIPtoStr(ulGatewayIp,(uchar_t *)szIp));
        return ERROR ;
    }
    *pulPoolIndex = i;
#endif
    return SYS_ERR_OK ;
}


/************************************************************
Function: ipPoolGetGatewayBySpecialIp
Description: Uses a specific IP address to find the gateway in the corresponding address pool.
Input: ulSpecialIp (specific IP address)
Output: gateway (changes the gateway in the corresponding address pool)
Return: Success (if successful). If failed, the output value is unusable.
**************************************************************/
STATUS ipPoolGetGatewayBySpecialIp(ulong_t ulSpecialIp, ulong_t *gateway)
{
#if 1
    STATUS rc = OK;
    ulong_t i,ulGetGatewayIp,ulGetSubnetMask,ulGetRowstatus;
    char szIp[20];

    if(gateway == NULL)
        return ERROR ;

    for(i=0; i< IPPOOL_MAX_POOL_NUM; i++)
    {
       /*The address pool exists and the gateway meets the input requirements*/
        //rc = ccdGet("gbnL3IPPool:ipPoolRowStatus", &i,1, &ulGetRowstatus,&len);
		ulGetRowstatus = ipPool_RowStatus[i];
        if((rc!=OK)||(ulGetRowstatus!=ROW_ACTIVE))
            continue;

        //rc = ccdGet("gbnL3IPPool:ipPoolGatewayIp", &i,1, &ulGetGatewayIp,&len);
        ulGetGatewayIp = gIpPool[i].ulGatewayIp;
        ulGetGatewayIp = ntohl(ulGetGatewayIp);
        if(rc!=OK)
        {
            continue;
		}

        //rc = ccdGet("gbnL3IPPool:ipPoolSubnetMask", &i,1, &ulGetSubnetMask,&len);
        ulGetSubnetMask = gIpPool[i].ulSubnetMask;
        ulGetSubnetMask = ntohl(ulGetSubnetMask);
        if(rc!=OK)
		{
            continue;
		}

		if( (ulGetGatewayIp & ulGetSubnetMask) == (ulSpecialIp & ulGetSubnetMask))
        {
            *gateway = ulGetGatewayIp;
			 rc = OK;
		     break;
		}

    }

    if(i > IPPOOL_MAX_POOL_NUM)
    {
        DHCP_SERVER_DBG ("ipPoolGetGatewayBySpecialIp->this special ip %s is not in any pool",ipPoolIPtoStr(ulSpecialIp,(uchar_t *)szIp));
        rc = ERROR ;
    }
#endif
  	return SYS_ERR_OK;
}

/************************************************************
Function: GetIpBitmapState
Description: This function checks the state of a bit at a specified index within a bitmap
            that represents IP address allocation segments. It validates the input bitmap
            reference and the index, calculates the position of the bit, and extracts the
            status encoded in 4 bits from the bitmap memory.
Input: Checks whether a bit wIndex is null. Note: When calling this function, 
        the input parameter cannot be less than the assigned baseline value; otherwise, the check is meaningless.
Return: Successfully read the bitmap. Returns success. The bitmap status is returned via pStatus.
**************************************************************/
STATUS GetIpBitmapState ( IpSectionBitMap_S * pRefBitmap,
                                    ushort_t wIndex ,
                                    uchar_t *pStatus)
{
    ushort_t     i, j;
    ulong_t        ulMask = SC_IP_BITMAP_MASK;
    IpSectionBitMap_S * pBitmap;

    if(NULL == pRefBitmap )
        return ERROR;/*This branch should not be taken*/
    pBitmap = pRefBitmap ;
    if(wIndex < pBitmap->wMinAllocNo)
        return ERROR ;

    wIndex -= pBitmap->wMinAllocNo;

    i = wIndex/SC_BITMAP_UNIT;//Get the index value of the ULONG
    j = wIndex%SC_BITMAP_UNIT;//Get the index value in a ULONG in the first 4 bits
    ulMask <<= (j*IPPOOL_BITMAP_LITTLE_STEP);

    *pStatus = (uchar_t)((pBitmap->pbBitmapMem[i] & ulMask)
                >>(j*IPPOOL_BITMAP_LITTLE_STEP));
    return OK;
}

/************************************************************
Function: FreeToBitmap
Description: Frees a bitmap
Input: Bitmap structure pointer, index value
**************************************************************/
STATUS FreeToIpSectionBitmap ( IpSectionBitMap_S * pRefBitmap, ushort_t wIndex )
{
    ulong_t    *pb, ulTest,ulMask;
    ushort_t    wUlongPos, wFourBitPos;
    IpSectionBitMap_S *pBitmap ;

    if(NULL == pRefBitmap )
        return FAILURE;

    pBitmap = pRefBitmap ;
    if( wIndex < pBitmap->wMinAllocNo )
    {
        return FAILURE;
    }
    if( wIndex > pBitmap->wMaxAllocNo )
    {
        return FAILURE;
    }

    wUlongPos = ( wIndex - pBitmap->wMinAllocNo ) / SC_BITMAP_UNIT;//Indicates the number of ULONG
    wFourBitPos = ( wIndex - pBitmap->wMinAllocNo ) % SC_BITMAP_UNIT;//Indicates the number of 4 BITs

    pb = &pBitmap->pbBitmapMem [ wUlongPos ];

    ulTest = ( IPPOOL_IPSTATUS_USED << (wFourBitPos*IPPOOL_BITMAP_LITTLE_STEP));
    ulMask = ( SC_IP_BITMAP_MASK << (wFourBitPos*IPPOOL_BITMAP_LITTLE_STEP));
    ulMask &= *pb ;
   /* The highest bit in 4BIT is not 0, indicating that it is in use*/
    if ( *pb & ulTest)
    {
        /* Clear all four bits */
        *pb ^= ulMask;
        pBitmap->wFreeNum++;
    }
    return SUCCESS;
}



/************************************************************
Function: ipPoolChangeSectionIpIdleLink
Description: Deletes a node from the free list, or inserts a node, as requested.
Calls: When modifying the state of a 4-bit bit in the bitmap, the corresponding linked list must also be modified.
Called By:
Input: A true flag indicates that a node is inserted into the free list, freeing an IP address.
        A false flag indicates that a node is deleted from the free list.
Output: This function only modifies the free list.
Return: The caller must ensure that the node belongs to the free list.
**************************************************************/
STATUS ipPoolChangeSectionIpIdleLink(IpPoolLink_S *pIpLink,ushort_t usIndex,uchar_t ucFlag,
											ulong_t ucPoolIndex,ulong_t  ulIpSectIndex)
{

    if(NULL == pIpLink)
        return ERROR ;

    if(ucFlag == TRUE)
    {
        //Insert a node into the free list
        if(OK != FreeToIdleLink(pIpLink,usIndex))
           return ERROR;
        return OK ;
    }
    //The following process is to delete the specified node in the free list
	//If the head pointer points to an invalid value, it means the linked list is empty
    if(pIpLink->usIdleHead >= pIpLink->usMaxLength)
    {
        return ERROR;
    }
    ipPoolClearLeaseCount(pIpLink,  usIndex);
    //The head and tail pointers both point to this node, and this node is the only one left in the free list.
    if(pIpLink->usIdleHead == pIpLink->usIdleTail)
	{
		if(pIpLink->usIdleTail == usIndex)
		{
			pIpLink->usIdleHead = IPPOOL_NULL_USHORT;
			pIpLink->usIdleTail = IPPOOL_NULL_USHORT;
			pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
		    pIpLink->pusIpLink[usIndex].usNext= (ushort_t)-1;
		    return OK ;
		}
		/*If there is only one node left in the linked list, but the input node
        is not this node, it means there is an error in the linked list*/
		return ERROR ;
	}

    /*If the node to be deleted is at the head*/
    if(usIndex == pIpLink->usIdleHead)
    {
        /*Delete the head node of the linked list*/
        pIpLink->usIdleHead = pIpLink->pusIpLink[usIndex].usNext;
        pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usNext].usPre
                            = (ushort_t)-1;
        pIpLink->pusIpLink[usIndex].usNext = (ushort_t)-1;
        return OK;
    }
   /*If the node to be flashed is at the end*/
    if(usIndex == pIpLink->usIdleTail)
	{
	    pIpLink->usIdleTail = pIpLink->pusIpLink[usIndex].usPre;
        pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usPre].usNext
                            = (ushort_t)-1;
        pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
        pIpLink->pusIpLink[usIndex].usNext= (ushort_t)-1;
        return OK;
    }
    /*in the middle*/
    pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usPre].usNext
                     = pIpLink->pusIpLink[usIndex].usNext;
    pIpLink->pusIpLink[pIpLink->pusIpLink[usIndex].usNext].usPre
                    = pIpLink->pusIpLink[usIndex].usPre;
    pIpLink->pusIpLink[usIndex].usPre = (ushort_t)-1;
    pIpLink->pusIpLink[usIndex].usNext= (ushort_t)-1;
    return OK;
}



/************************************************************
Function: ipPoolChangeIpStatusInPool
Description: Forcefully changes the status of an IP address in the address pool.
Input: Address, address pool number, status
**************************************************************/
STATUS  ipPoolChangeIpStatusInPool(ulong_t ulPoolIndex,ulong_t ulIpAddr,
                                  uchar_t ucStatus,uchar_t ucLeaseFlag)
{
    ushort_t usIndex ;//The index value of the address in the address segment. 
                    // This index value can be used directly in the linked list, but cannot be used directly in the bitmap.
    ulong_t  ulIpSectIndex;
    #if 0
    uchar_t ucOldStatus;
    ucOldStatus = 0xff;
    #endif
    if(OK != FindIpSectionByIp(ulPoolIndex, ulIpAddr,&ulIpSectIndex,&usIndex))
    {
        if(ulIpAddr != 0)
        {
/*            IPPool_SendAlarm(ALARM_EVENT_CCM_IPPOOL_IP_NOT_IN_POOL,
    			        	   (ulong_t)ulPoolIndex, ulIpAddr, 0,0);  */
            return ERROR ;
        }
    }
    #if 0/*重复*/
 	GetIpBitmapState(&gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].Bitmap,
	      usIndex + SC_MIN_IPPOOL_ALLOCINDEX, &ucOldStatus);
    #endif
    if(OK != ipPoolChangeIpStatusInSection(ulPoolIndex,ulIpSectIndex,
                                          usIndex ,ucStatus,ucLeaseFlag))
	    return ERROR ;

     return OK;
}

STATUS ipPoolChangeIpStatusInSection(ulong_t ulPoolIndex,ulong_t ulIpSectIndex,
                                    ushort_t usIndex ,uchar_t ucStatus,uchar_t ucLeaseFlag)
{

    //The index value of the address in the address segment. 
    //This index value can be used directly in the linked list, but cannot be used directly in the bitmap.
    uchar_t ucOldStatus;
    IpSectionBitMap_S * pBitmap ;
    IpPoolLink_S        * pIpLink;

    ucOldStatus = 0xff;

    /* usIndex is the result of the search, so no check is needed. */
    if(ulPoolIndex >= IPPOOL_MAX_POOL_NUM)
         return ERROR;

    if(ulIpSectIndex >= IPPOOL_MAX_IPSection_NUM)
         return ERROR;

    pBitmap = &(gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].Bitmap);
    pIpLink = &(gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].stIpLink);

    GetIpBitmapState(pBitmap,  (usIndex + SC_MIN_IPPOOL_ALLOCINDEX), &ucOldStatus);

    if(ucOldStatus == ucStatus)
        return OK;

    if ( (ucOldStatus == IPPOOL_IPSTATUS_DISABLE)&&(ucStatus == IPPOOL_IPSTATUS_CONFLICT) )
    {
        return OK;
    }

    if(OK != ipPoolChangeIpSectionBitMap(pBitmap,(usIndex + SC_MIN_IPPOOL_ALLOCINDEX),ucStatus))
    {
        DHCP_SERVER_DBG ("ipPoolChangeIpStatusInPool->change ip BitMap and fail");
        return ERROR ;
    }
    switch (ucOldStatus)
       {
    case IPPOOL_IPSTATUS_CONFLICT:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulConflict-- ;
        break;
    case IPPOOL_IPSTATUS_INVALID:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulInvalid-- ;
        break;
    case IPPOOL_IPSTATUS_DISABLE:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulDisable-- ;
        break;
    case IPPOOL_IPSTATUS_UNUSED:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulUnUsed--;
        break;
    case IPPOOL_IPSTATUS_USED:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulInUsed--;
        break ;
    default :
         break;
     }

    switch (ucStatus)
       {
    case IPPOOL_IPSTATUS_CONFLICT:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulConflict++ ;
        break;
    case IPPOOL_IPSTATUS_INVALID:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulInvalid++ ;
        break;
    case IPPOOL_IPSTATUS_DISABLE:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulDisable++ ;
        break;
    case IPPOOL_IPSTATUS_UNUSED:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulUnUsed++;
        break;
    case IPPOOL_IPSTATUS_USED:
        gIpPool[ulPoolIndex].IpSection[ulIpSectIndex].ulInUsed++;
        break ;
    default :
        break;
     }

    /*0052 Linked list insertion and deletion operations required*/
    if(ucOldStatus == IPPOOL_IPSTATUS_UNUSED)
    {
        /*Indicates that the address is in the free list and needs to be deleted from the list*/
        ipPoolChangeSectionIpIdleLink(pIpLink, usIndex, FALSE,ulPoolIndex, ulIpSectIndex);

        /*If the new state is the use state, you need to insert the index in the use list*/
        if(ucStatus == IPPOOL_IPSTATUS_USED)
            InsertToUsedLink( pIpLink, usIndex,ucLeaseFlag);
        else if(ucStatus == IPPOOL_IPSTATUS_CONFLICT)
            InsertToConflictLink(pIpLink,usIndex, ucLeaseFlag);

    }
    else if(ucOldStatus == IPPOOL_IPSTATUS_USED)//Since there is no way to add alarms on the original basis
    {//So add this processing code
         DelFromUsedLink(pIpLink, usIndex,ulPoolIndex,ulIpSectIndex);
         if(ucStatus == IPPOOL_IPSTATUS_UNUSED)
             FreeToIdleLink(pIpLink, usIndex);
         else if(ucStatus == IPPOOL_IPSTATUS_CONFLICT)
             InsertToConflictLink(pIpLink,usIndex, ucLeaseFlag);
    }
    /*0052*/
    else if(ucOldStatus == IPPOOL_IPSTATUS_CONFLICT)
    {
        DelFromConflictLink(pIpLink, usIndex,ulPoolIndex,ulIpSectIndex);
         if(ucStatus == IPPOOL_IPSTATUS_UNUSED)
         	FreeToIdleLink(pIpLink, usIndex);
         else if(ucStatus == IPPOOL_IPSTATUS_USED)
             InsertToUsedLink( pIpLink, usIndex,ucLeaseFlag);
    }
    else
    {
         if(ucStatus == IPPOOL_IPSTATUS_UNUSED)
         	FreeToIdleLink(pIpLink, usIndex);
         if(ucStatus == IPPOOL_IPSTATUS_USED)
         	InsertToUsedLink( pIpLink, usIndex,ucLeaseFlag);
         else if(ucStatus == IPPOOL_IPSTATUS_CONFLICT)
             InsertToConflictLink( pIpLink, usIndex, ucLeaseFlag);
    }
    return OK ;
}


/************************************************************
Function: ipPoolChangeIpSectionBitMap
Description: Modifies the value of a bitmap
Input: Bit value index, bitmap pointer, target value
**************************************************************/
STATUS ipPoolChangeIpSectionBitMap(IpSectionBitMap_S * pRefBitmap, ushort_t wIndex,uchar_t ucStatus)
{
  /* Example:
The current status of a ulong in a bitmap is (high) 0000, 0000, 1001, 0000, 1000, 0000, 0000, 1010 (low).
The fourth low-order bit (1000 in use) needs to be set to (1010 illegal).
Using 2, we clear the 1000 bit in 1 to 0000, resulting in a value of 3. Using 4 to set 3 to the final result of 5
1: (high) 0000, 0000, 1001, 0000, 1000, 0000, 0000, 1010 (low) 3 = 2 & 1
2: (high) 1111, 1111, 1111, 1111, 0000, 1111, 1111, 1111 (low)
3: (high) 0000, 0000, 1001, 0000, 0000, 0000, 1010 (low)
4: (high) 0000, 0000, 0000, 0000, 1010, 0000, 0000, 0000 (low) 5 = 4^3
5: (high)0000,0000，1001,0000，1010,0000，0000,1010(low) 
*/

    int     i, j;
    ulong_t     ulMask ,ulTemp,ulMapMem;
    IpSectionBitMap_S * pBitmap;

    if(NULL == pRefBitmap )
        return ERROR ;/*This branch should not be taken*/
    pBitmap = pRefBitmap ;


    if(wIndex < pBitmap->wMinAllocNo)
        return ERROR ;

    ulMask = SC_IP_BITMAP_MASK;
    wIndex -= pBitmap->wMinAllocNo;

    i = wIndex/SC_BITMAP_UNIT;
    j = wIndex%SC_BITMAP_UNIT;

    ulMask <<= (j*IPPOOL_BITMAP_LITTLE_STEP);
    ulTemp = ~ulMask;//Clear this 4 bits

    ulMapMem = pBitmap->pbBitmapMem [ i ];//Equivalent to 1
      DHCP_SERVER_DBG("ipPoolChangeIpSectionBitMap->change %d from %d to %d",
                      wIndex,(int)((ulMapMem&ulMask)>>(j*IPPOOL_BITMAP_LITTLE_STEP)),ucStatus);
    pBitmap->pbBitmapMem [ i ] = ulMapMem & ulTemp;//ulTemp is equivalent to 2

    ulTemp = (ulong_t)ucStatus ;
    ulTemp <<= (j*IPPOOL_BITMAP_LITTLE_STEP);

    pBitmap->pbBitmapMem [ i ] |= ulTemp;//Set these 4 bits to the desired value

    return OK;

}



/************************************************************
Function: ipPoolSetLeaseFlag
Description: This function sets the lease flag of a certain address in the linked list
Calls: to disable or enable.
Output: To save memory, this flag is appended to the highest byte of the lease.
Return: If the highest byte of the lease is 1, the lease is not in use.
Others: If the highest byte of the lease is 0, the lease is in use.
**************************************************************/
STATUS ipPoolSetLeaseFlag(IpPoolLink_S *pIpRefLink,ushort_t usIndex,uchar_t ucFlag)
{
    IpPoolLink_S *pIpLink;
    ulong_t ulTemp;
    ulTemp = 0;

    if(pIpRefLink == NULL)
        return ERROR;
    if(usIndex >= IPPOOL_MAX_IPPerSection_NUM)
        return ERROR;
    if((ucFlag != LEASE_ABLE)&&(ucFlag != LEASE_DISABLE))
        return ERROR ;

    pIpLink = pIpRefLink;
    ulTemp = ucFlag;
    ulTemp <<= 24;
    ulTemp &= 0xff000000;
    pIpLink->pusIpLink[usIndex].ulLeaseCount &= 0xffffff;//Clear the highest byte
    pIpLink->pusIpLink[usIndex].ulLeaseCount |= ulTemp;//Set the highest byte
    return OK ;
}

STATUS IPPOOL_GetAnyFreeIp_local
(
ulong_t  ulGatewayIp,
ulong_t *pstFreeIp ,
uchar_t ucLeaseFlag,
BOOL cantBeBound,
ulong_t vid
)
{
    ulong_t ulPoolIndex ;
    ulong_t ulFreeIp ;

   /*Initialize to invalid value*/
    ulPoolIndex = IPPOOL_MAX_POOL_NUM;
    if(OK != ipPoolGetPoolIndexByGateIp( ulGatewayIp, &ulPoolIndex ))
        return FIND_NO_POOL_INDEX ;

    if((ucLeaseFlag!= LEASE_ABLE)&&(ucLeaseFlag != LEASE_DISABLE))
        return IP_GENERAL_ERROR;
    if(OK != AllocIpAddrFromPool(ulPoolIndex, &ulFreeIp,ucLeaseFlag, cantBeBound,vid))
        return NO_FREE_IP;

    *pstFreeIp = ulFreeIp ;
     return IP_SUCCESS ;
}

STATUS IPPOOL_GetSpecialIp_local(ulong_t  ulGatewayIp, ulong_t ulSpecialIp, BOOL cantBeBound, ulong_t vid)
{
    char szIp[20];
    ushort_t usIndex = (ushort_t)-1;
    uchar_t ucStatus =  (uchar_t)-1;
    ulong_t ulSectIndex = (ulong_t)-1;
    ulong_t ulPoolIndex = IPPOOL_MAX_POOL_NUM - 1;//Initialized to an invalid value

    if(cantBeBound)
    {
        if(dhcps_ip_is_bound(ulSpecialIp,vid))
        {
            DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->this router ip %s is bound to someone other.",ipPoolIPtoStr(ulGatewayIp,(uchar_t *) szIp));
            DHCP_SERVER_DBG ( "dhcp sever: The special ip is bound to someone other " );
            return  IP_GENERAL_ERROR ;
        }
    }
    if( OK!= ipPoolGetPoolIndexByGateIp( ulGatewayIp, &ulPoolIndex ))
    {
        DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->this router ip %s does not exist",ipPoolIPtoStr(ulGatewayIp,(uchar_t *) szIp));
        return FIND_NO_POOL_INDEX ;
    }
    if(ulSpecialIp == 0)
    {
        return IP_NOT_IN_POOL;
    }

    gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_request_alloc_Ip ]++;
    if(OK != FindIpSectionByIp(ulPoolIndex, ulSpecialIp, &ulSectIndex, &usIndex))
    {
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_NOT_IN_POOL]++;
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_fail ]++;
        /*            IPPool_SendAlarm(ALARM_EVENT_CCM_IPPOOL_IP_NOT_IN_POOL, (ulong_t) ulPoolIndex,
        	                                ulSpecialIp,0,0); */
        DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->There is not ip %s in this pool",ipPoolIPtoStr(ulSpecialIp, (uchar_t *)szIp));
        return IP_NOT_IN_POOL ;
   }

   /*Failed to read the status of the specified IP*/
   if(SUCCESS != GetIpBitmapState(&(gIpPool[ulPoolIndex].IpSection[ulSectIndex].Bitmap),
   	usIndex, &ucStatus))
   {
       gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_BITMAP_ERROR]++;
       gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_fail ]++;
       DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->ip %s bitmap error",ipPoolIPtoStr(ulSpecialIp, (uchar_t *)szIp));
       return IP_BITMAP_ERROR;
   }
    ipPoolIPtoStr(ulSpecialIp, (uchar_t *)szIp);

    switch(ucStatus)
    {
    case IPPOOL_IPSTATUS_UNUSED:
        if(SUCCESS != ipPoolChangeIpStatusInPool(ulPoolIndex,ulSpecialIp,
        	IPPOOL_IPSTATUS_USED,LEASE_ABLE))
        {
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_BITMAP_ERROR]++;
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_fail ]++;
            DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->ip %s bitmap error",szIp);
            return IP_GENERAL_ERROR;
        }
        break;

     case IPPOOL_IPSTATUS_CONFLICT:
        if(SUCCESS != ipPoolChangeIpStatusInPool(ulPoolIndex,ulSpecialIp,
        	IPPOOL_IPSTATUS_USED,LEASE_ABLE))
        {
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_IN_CONFLICT]++;
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_fail ]++;
            DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->ip %s conflict",szIp);
            return IP_IN_CONFLICT;
        }
        break;

     case IPPOOL_IPSTATUS_INVALID:
         gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_IN_INVALID]++;
         gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_fail ]++;
         DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->ip %s invalid",szIp);
         return IP_IN_INVALID;

     case IPPOOL_IPSTATUS_DISABLE:
         gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_IN_DISABLE]++;
         gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_fail ]++;
         DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->ip %s disable",szIp);
         return IP_IN_DISABLE;

     case IPPOOL_IPSTATUS_USED:
         gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_IN_USED]++;
         gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_fail ]++;
         DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->ip %s in using",szIp);
        return IP_IN_USED;/*This branch indicates that DHCP requests the allocated address from the address pool*/

     default :
         gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_IN_UNKNOW]++;
         gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_fail ]++;
         DHCP_SERVER_DBG("IPPOOL_GetSpecialIp->ip %s unknown",szIp);
         return IP_IN_UNKNOW;
     }
     gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Alloc_success]++;
     return IP_SUCCESS;
}

#ifdef GBN_INCLUDE_STACK_ISF
STATUS IPPOOL_GetSpecialIp_to_others(ulong_t  ulGatewayIp, ulong_t ulSpecialIp, BOOL cantBeBound, ulong_t vid)
{
    int iRet;
    SMD_MSG msg;
    SMD_ARG args[4];

    smdMsgInit(&msg, SMD_DEV_ALL, ISF_IPPOOL_GETSPECIALIP, SMD_GLOBAL);

    args[0].pBuf = &ulGatewayIp;
    args[0].usFlags = 0;
    args[0].usLen = sizeof(ulong_t);

    args[1].pBuf = &ulSpecialIp;
    args[1].usFlags = 0;
    args[1].usLen = sizeof(ulong_t);

    args[2].pBuf = &cantBeBound;
    args[2].usFlags = 0;
    args[2].usLen = sizeof(BOOL);

    args[3].pBuf = &vid;
    args[3].usFlags = 0;
    args[3].usLen = sizeof(ulong_t);

    iRet = smdMsgRequest(&msg, args, 4, SMD_TIMEOUT_DEFAULT);
    if ( 0 != iRet )
    {
        iRet = ERROR;
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:request failed iRet = %d", __func__, iRet);
    }

    return iRet;
}

int IPPOOL_GetSpecialIp_to_others_wrap(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 4 != ulArgsNum )
    {
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return ERROR;
    }

    IPPOOL_GetSpecialIp_local(*((ulong_t *)args[0].pBuf), *((ulong_t *)args[1].pBuf),
        *((BOOL *)args[2].pBuf), *((ulong_t *)args[3].pBuf));

    return OK;
}
#endif

/************************************************************
Function: IPPOOL_GetAnyFreeIp
Description: alloc IP address according to router IP address
Called By: Passes an index IP address (usually the router IP address in the address pool parameters).
Table Accessed: Obtains a random free IP address from the address pool.
Input: ulIppoolIndexIp: The index IP address in the address pool
        *PstFreeIp: The obtained free IP address
Return: SUCCESS: Success, indicating that the value returned by *PstFreeIp is meaningful and
        the address pool has recorded the lifecycle of this IP address.
        FAILURE: Failure, indicating that the value returned by *PstFreeIp is meaningless 
        (address pools can also define multiple return values ??to indicate error reasons).
**************************************************************/
STATUS IPPOOL_GetAnyFreeIp
(
ulong_t  ulGatewayIp,
ulong_t *pstFreeIp ,
uchar_t ucLeaseFlag,
BOOL cantBeBound,
ulong_t vid
)
{
    STATUS iRet;

    iRet = IPPOOL_GetAnyFreeIp_local(ulGatewayIp, pstFreeIp, ucLeaseFlag,
            cantBeBound, vid);
#ifdef GBN_INCLUDE_STACK_ISF
    if ( (IP_SUCCESS == iRet) && (TRUE == isf_stack_enable()) && (TRUE == isf_stack_is_master()) )
    {
         IPPOOL_GetSpecialIp_to_others(ulGatewayIp, *pstFreeIp, cantBeBound, vid);
    }
#endif

    return iRet;
}

/************************************************************
Function: IPPOOL_GetSpecialIp
Description: Assigns an IP address from the address pool.
Calls: When the DHCP SERVER submodule analyzes a user's request message and finds 
        that the user has specified a desired IP address, it calls the address 
        pool module's interface function: passing in an interface IP address and 
        the user's desired IP address, allowing the address pool to determine 
        whether the IP address has already been assigned.
Input: ulGatewayIp: The index IP address in the address pool.
        ulSpecialIp: The specified IP address in network byte order.
        This IP address should belong to the same network segment as ulIppoolIndexIp.
Return: SUCCESS: Successful, indicating that the IP address is available 
        and the address pool has recorded its lifecycle.
        FAILURE: Failure indicates that the IP address cannot be used (address pools 
        can also define various return values ??indicating the cause of the error).
Others: Check the status of the IP address. If the status is idle, assign the address.
        Modify the response bitmap, free list, and used list.
**************************************************************/
STATUS IPPOOL_GetSpecialIp(ulong_t  ulGatewayIp, ulong_t ulSpecialIp, BOOL cantBeBound, ulong_t vid)
{
    STATUS iRet;

    iRet = IPPOOL_GetSpecialIp_local(ulGatewayIp, ulSpecialIp, cantBeBound, vid);
#ifdef GBN_INCLUDE_STACK_ISF
    if ( (IP_SUCCESS == iRet) && (TRUE == isf_stack_enable()) && (TRUE == isf_stack_is_master()) )
    {
         IPPOOL_GetSpecialIp_to_others(ulGatewayIp, ulSpecialIp, cantBeBound, vid);
    }
#endif

    return iRet;
}

void IPPOOL_GetbackIp_local(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    ulong_t ulPoolIndex;

    /*Initialize to invalid value*/
    ulPoolIndex = IPPOOL_MAX_POOL_NUM+1;
    if(OK != ipPoolGetPoolIndexByGateIp( ulGatewayIp, &ulPoolIndex ))
        return;
    FreeIpAddrToPool(ulPoolIndex, ulSpecialIp);
    return;
}

#ifdef GBN_INCLUDE_STACK_ISF
void IPPOOL_GetbackIp_to_others(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    int iRet;
    SMD_MSG msg;
    SMD_ARG args[2];

    smdMsgInit(&msg, SMD_DEV_ALL, ISF_IPPOOL_GETBACKIP, SMD_GLOBAL);

    args[0].pBuf = &ulGatewayIp;
    args[0].usFlags = 0;
    args[0].usLen = sizeof(ulong_t);

    args[1].pBuf = &ulSpecialIp;
    args[1].usFlags = 0;
    args[1].usLen = sizeof(ulong_t);

    iRet = smdMsgRequest(&msg, args, 2, SMD_TIMEOUT_DEFAULT);
    if ( 0 != iRet )
    {
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:request failed iRet = %d", __func__, iRet);
    }
}

int IPPOOL_GetbackIp_to_others_wrap(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 2 != ulArgsNum )
    {
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return ERROR;
    }

    IPPOOL_GetbackIp_local(*((ulong_t *)args[0].pBuf), *((ulong_t *)args[1].pBuf));

    return OK;
}
#endif

/************************************************************
Function: IPPOOL_GetbackIp
Description: Reclaims an allocated IP address.
Called By: When the DHCP Server submodule receives a message requesting the release of an IP address,
            it calls the interface function provided by the address pool module:
            Passing in an index IP address and the IP address released by the user, 
            it instructs the address pool to reclaim the IP address.
Input: ulGatewayIp: Index IP address of the address pool
        ulSpecialIp: IP address to be reclaimed
Return: Return value: Not important
**************************************************************/
void IPPOOL_GetbackIp(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    IPPOOL_GetbackIp_local(ulGatewayIp, ulSpecialIp);
#ifdef GBN_INCLUDE_STACK_ISF
    if ( (TRUE == isf_stack_enable()) && (TRUE == isf_stack_is_master()) )
    {
         IPPOOL_GetbackIp_to_others(ulGatewayIp, ulSpecialIp);
    }
#endif

    return;
}

void IPPOOL_GetbackIpWithoutGateway_local(ulong_t ulSpecialIp )
{
    ulong_t ulPoolIndex;

    for(ulPoolIndex = 0; ulPoolIndex < IPPOOL_MAX_POOL_NUM; ulPoolIndex++)
   	{
       if( SYS_ERR_OK == FreeIpAddrToPool(ulPoolIndex, ulSpecialIp))
	  	  return;
   	}

    return;
}

#ifdef GBN_INCLUDE_STACK_ISF
void IPPOOL_GetbackIpWithoutGateway_to_others(ulong_t ulSpecialIp )
{
    int iRet;
    SMD_MSG msg;
    SMD_ARG args[1];

    smdMsgInit(&msg, SMD_DEV_ALL, ISF_IPPOOL_GETBACKIPWITHOUTGATEWAP, SMD_GLOBAL);

    args[0].pBuf = &ulSpecialIp;
    args[0].usFlags = 0;
    args[0].usLen = sizeof(ulong_t);

    iRet = smdMsgRequest(&msg, args, 1, SMD_TIMEOUT_DEFAULT);
    if ( 0 != iRet )
    {
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:request failed iRet = %d", __func__, iRet);
    }
}

int IPPOOL_GetbackIpWithoutGateway_to_others_wrap(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 1 != ulArgsNum )
    {
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return ERROR;
    }

    IPPOOL_GetbackIpWithoutGateway_local(*((ulong_t *)args[0].pBuf));

    return OK;
}
#endif

/************************************************************
Function: IPPOOL_GetbackIpWithoutGateway
Description: Reclaims an assigned IP address.
Called By: When the DHCP SERVER submodule receives a user's discover message,
        if the address pool pointed to by the gateway address in the message 
        does not contain the currently assigned IP address, the address needs 
        to be reclaimed because the user may have moved to another network range.
Input: ulSpecialIp: The IP address to be reclaimed.
Return: Not important.
**************************************************************/
void IPPOOL_GetbackIpWithoutGateway(ulong_t ulSpecialIp )
{
    IPPOOL_GetbackIpWithoutGateway_local(ulSpecialIp);

#ifdef GBN_INCLUDE_STACK_ISF
    if ( (TRUE == isf_stack_enable()) && (TRUE == isf_stack_is_master()) )
    {
         IPPOOL_GetbackIpWithoutGateway_to_others(ulSpecialIp);
    }
#endif

    return;
}

STATUS IPPOOL_SetOpton43(ulong_t ulGatewayIp, IPPOOL_OPTION43 *pOption43)
{
    ulong_t ulPoolIndex ;

    if ( (NULL == pOption43) || (pOption43->ucValueLen > IPPOOL_OPTION43_MAX_LEN) )
    {
        return ERROR;
    }

    ulPoolIndex = IPPOOL_MAX_POOL_NUM - 1;//Initialized to an invalid value

    if ( OK!= ipPoolGetPoolIndexByGateIp(ulGatewayIp, &ulPoolIndex) )
    {
        return FIND_NO_POOL_INDEX;
    }

    memset(&gIpPool[ulPoolIndex].option43, 0, sizeof(IPPOOL_OPTION43));

    gIpPool[ulPoolIndex].option43.ucFormat = pOption43->ucFormat;
    gIpPool[ulPoolIndex].option43.ucValueLen = pOption43->ucValueLen;
    memcpy(gIpPool[ulPoolIndex].option43.szValue, pOption43->szValue, pOption43->ucValueLen);

    return OK;
}

STATUS IPPOOL_SetOpton43_ByIndex(ulong_t ulPoolIndex, IPPOOL_OPTION43 *pOption43)
{
    if ( (NULL == pOption43) || (pOption43->ucValueLen > IPPOOL_OPTION43_MAX_LEN) )
    {
        return ERROR;
    }

    if ( ulPoolIndex >= IPPOOL_MAX_POOL_NUM )
    {
        return ERROR;
    }

    memset(&gIpPool[ulPoolIndex].option43, 0, sizeof(IPPOOL_OPTION43));

    gIpPool[ulPoolIndex].option43.ucFormat = pOption43->ucFormat;
    gIpPool[ulPoolIndex].option43.ucValueLen = pOption43->ucValueLen;
    memcpy(gIpPool[ulPoolIndex].option43.szValue, pOption43->szValue, pOption43->ucValueLen);

    return OK;
}

STATUS IPPOOL_SetOpton43_ByName(char *szPoolName, IPPOOL_OPTION43 *pOption43)
{
    ulong_t ulPoolIndex;

    if ( (NULL == szPoolName) || (NULL == pOption43)
		|| (pOption43->ucValueLen > IPPOOL_OPTION43_MAX_LEN) )
    {
        return ERROR;
    }

	if ( OK != ipPoolGetPoolIndexByName(szPoolName, &ulPoolIndex))
	{
		return ERROR;
	}

    if ( ulPoolIndex >= IPPOOL_MAX_POOL_NUM )
    {
        return ERROR;
    }

    return IPPOOL_SetOpton43_ByIndex(ulPoolIndex, pOption43);
}

STATUS IPPOOL_GetOpton43(ulong_t ulGatewayIp, IPPOOL_OPTION43 *pOption43)
{
    ulong_t ulPoolIndex ;

    if ( NULL == pOption43 )
    {
        return ERROR;
    }

    ulPoolIndex = IPPOOL_MAX_POOL_NUM;//Initialized to an invalid value

    if ( OK!= ipPoolGetPoolIndexByGateIp(ulGatewayIp, &ulPoolIndex) )
    {
        return FIND_NO_POOL_INDEX;
    }

    memcpy(pOption43, &gIpPool[ulPoolIndex].option43, sizeof(IPPOOL_OPTION43));

    return OK;
}

STATUS IPPOOL_GetOpton43_ByIndex(ulong_t ulPoolIndex, IPPOOL_OPTION43 *pOption43)
{
    if ( NULL == pOption43 )
    {
        return ERROR;
    }

    if ( ulPoolIndex >= IPPOOL_MAX_POOL_NUM )
    {
        return ERROR;
    }

    memcpy(pOption43, &gIpPool[ulPoolIndex].option43, sizeof(IPPOOL_OPTION43));

    return OK;
}

/************************************************************
Function: IpPool_GetParameter
Description: Gets common address pool parameters.
Called By: When the DHCP Server submodule generates a DHCP response message,
        it needs to use certain common address pool parameters. 
        It will call the interface function provided by the address pool
        module to obtain these parameters.
        Currently, the common parameters are as follows: network mask, router IP,
        DNS IP, and IP address lease lifetime parameter (ulong in seconds).
**************************************************************/
STATUS IpPool_GetParameter(ulong_t ulGatewayIp,ulong_t *pulMask,
                                    ulong_t pulDnsIp[],
                                    ulong_t *pulPrimaryNBNSIp,
                                    ulong_t *pulSecondNBNSIp,
                                    ulong_t *pulLeaseTime,
                                    uchar_t *pszDnsSuffixName,
                                    ulong_t *pulRouterIp
                                    )
{
    ulong_t ulPoolIndex ;
    int i;
    if((pulDnsIp == NULL)||
        (pulPrimaryNBNSIp== NULL)||
        (pulSecondNBNSIp== NULL)||
        (pulLeaseTime== NULL)||
        (pulMask == NULL)||
        (pszDnsSuffixName == NULL ))
        return ERROR ;

    ulPoolIndex = IPPOOL_MAX_POOL_NUM -1;//Initialized to an invalid value

    if(OK!= ipPoolGetPoolIndexByGateIp(ulGatewayIp, &ulPoolIndex))
        return FIND_NO_POOL_INDEX;

    for(i=0; i < IPPOOL_MAX_DNS_NUM; i++)
    {
        pulDnsIp[i] = gIpPool[ulPoolIndex].ulDNSIp[i];
    }

    *pulPrimaryNBNSIp   = gIpPool[ulPoolIndex].ulPrimaryNBNSIp ;
    *pulSecondNBNSIp    = gIpPool[ulPoolIndex].ulSecondNBNSIp  ;
    *pulLeaseTime       = gIpPool[ulPoolIndex].ulLeaseTime     ;
    *pulMask            = gIpPool[ulPoolIndex].ulSubnetMask    ;
    *pulRouterIp = gIpPool[ulPoolIndex].ulRouterIp;
    memcpy ( pszDnsSuffixName, gIpPool[ulPoolIndex].szDnsSuffixName, IPPOOL_MAX_DNS_SUFFIX_NAME );
    return IP_SUCCESS;
}

STATUS IpPool_GetParameter_tftp(ulong_t ulGatewayIp, uchar_t *pucTftpName, uchar_t *pucTftpBootfileName)
{
    ulong_t ulPoolIndex ;

    ulPoolIndex = IPPOOL_MAX_POOL_NUM;//Initialized to an invalid value

    if (OK != ipPoolGetPoolIndexByGateIp(ulGatewayIp, &ulPoolIndex))
    {
        return FIND_NO_POOL_INDEX;
    }

    /*option66*/
    if ( NULL != pucTftpName )
    {
        strcpy((char *)pucTftpName, (char *)gIpPool[ulPoolIndex].tftpServer);
    }

    /*option67*/
    if ( NULL != pucTftpBootfileName )
    {
        strcpy((char *)pucTftpBootfileName, (char *)gIpPool[ulPoolIndex].bootFile);
    }

    return IP_SUCCESS;
}

#if 0
/************************************************************
Function: IPPOOL_SetConflictIp_local
Description: Notifies the address pool that an address is no longer usable.
Calls: During actual IP address allocation, if a user discovers that the 
        assigned IP address is unusable, a notification message is sent to the DHCP server. 
        The DHCP server submodule then notifies the address pool module to disable the IP address.
Input: ulGatewayIp: Index IP address in the address pool
        ulSpecialIp: IP address to be disabled
Return: Return value: Not important
**************************************************************/
void IPPOOL_DisableIp(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    uchar_t ucPoolIndex ;

    /*Initialize to invalid value*/
    ucPoolIndex = IPPOOL_MAX_POOL_NUM+1;
    if(OK != ipPoolGetPoolIndexByGateIp( ulGatewayIp, &ucPoolIndex ))
        return;
    ipPoolChangeIpStatusInPool( ucPoolIndex,
                          ulSpecialIp,IPPOOL_IPSTATUS_DISABLE,LEASE_DISABLE);
    return ;
}
#endif

STATUS IPPOOL_SetConflictIp_local(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    ulong_t ulPoolIndex ;
    STATUS rc;

    ulPoolIndex = IPPOOL_MAX_POOL_NUM;/*Initialize to invalid value*/

    if(OK != ipPoolGetPoolIndexByGateIp( ulGatewayIp, &ulPoolIndex ))
        return ERROR;
    rc = ipPoolChangeIpStatusInPool( ulPoolIndex,
                          ulSpecialIp,IPPOOL_IPSTATUS_CONFLICT,LEASE_ABLE);
    return rc;
}

#ifdef GBN_INCLUDE_STACK_ISF
STATUS IPPOOL_SetConflictIp_to_others(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    int iRet;
    SMD_MSG msg;
    SMD_ARG args[2];

    smdMsgInit(&msg, SMD_DEV_ALL, ISF_IPPOOL_SETCONFILICTIP, SMD_GLOBAL);

    args[0].pBuf = &ulGatewayIp;
    args[0].usFlags = 0;
    args[0].usLen = sizeof(ulong_t);

    args[1].pBuf = &ulSpecialIp;
    args[1].usFlags = 0;
    args[1].usLen = sizeof(ulong_t);

    iRet = smdMsgRequest(&msg, args, 2, SMD_TIMEOUT_DEFAULT);
    if ( 0 != iRet )
    {
        iRet = ERROR;
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:request failed iRet = %d", __func__, iRet);
    }

    return iRet;
}

int IPPOOL_SetConflictIp_to_others_wrap(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 2 != ulArgsNum )
    {
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return ERROR;
    }

    IPPOOL_SetConflictIp_local(*((ulong_t *)args[0].pBuf), *((ulong_t *)args[1].pBuf));

    return OK;
}
#endif

/************************************************************
Function: IPPOOL_SetConflictIp
Description: Notifies the address pool that an address is no longer usable.
Calls: During actual IP address allocation, if a user discovers that the 
assigned IP address is unusable, a notification message is sent to the DHCP server. 
The DHCP server submodule then notifies the address pool module to disable the IP address.
Input: ulGatewayIp: Index IP address in the address pool
        ulSpecialIp: IP address to be disabled
Return: Return value: Not important
**************************************************************/
STATUS IPPOOL_SetConflictIp(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    STATUS iRet;

    iRet = IPPOOL_SetConflictIp_local(ulGatewayIp, ulSpecialIp);
#ifdef GBN_INCLUDE_STACK_ISF
    if ( (IP_SUCCESS == iRet) && (TRUE == isf_stack_enable()) && (TRUE == isf_stack_is_master()) )
    {
         IPPOOL_SetConflictIp_to_others(ulGatewayIp, ulSpecialIp);
    }
#endif

    return iRet;
}

STATUS IPPOOL_RenewIpTime_local(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{

    uchar_t ucStatus,szIp[20];
    ushort_t usIndex ;
    ulong_t  ulPoolIndex ,ulSectIndex;
    usIndex = (ushort_t)-1;
    ucStatus = (uchar_t)-1;
    ulSectIndex = (ulong_t)-1;
    ulPoolIndex = IPPOOL_MAX_POOL_NUM -1;//Initialized to an invalid value

    if(OK != ipPoolGetPoolIndexByGateIp(ulGatewayIp, &ulPoolIndex ))
        return FIND_NO_POOL_INDEX ;

    gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_Request]++;
    if(OK != FindIpSectionByIp(ulPoolIndex, ulSpecialIp, &ulSectIndex, &usIndex))
    {
        DHCP_SERVER_DBG("IPPOOL_RenewIpTime->IPaddr %s is not in pool %d",ipPoolIPtoStr(ulSpecialIp, szIp),ulPoolIndex);
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_not_in_pool]++  ;
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_Fail]++  ;
        return IP_NOT_IN_POOL ;
    }

    if(OK != GetIpBitmapState(&(gIpPool[ulPoolIndex].IpSection[ulSectIndex].Bitmap),
                      (usIndex+SC_MIN_IPPOOL_ALLOCINDEX), &ucStatus))
    {
        DHCP_SERVER_DBG("IPPOOL_RenewIpTime->IPaddr %s bitmap error",ipPoolIPtoStr(ulSpecialIp, szIp));
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_bitmap_error]++  ;
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_Fail]++  ;
        return IP_GENERAL_ERROR;
    }

    /*Decide how to handle it based on the current status*/
    switch(ucStatus)
    {

    /*The switch is in idle state after reset*/
    case IPPOOL_IPSTATUS_UNUSED:
        if(OK != ipPoolChangeIpStatusInPool(ulPoolIndex,ulSpecialIp,
                                           IPPOOL_IPSTATUS_USED,LEASE_ABLE))
        {
            DHCP_SERVER_DBG("IPPOOL_RenewIpTime->IPaddr %s bitmap error",ipPoolIPtoStr(ulSpecialIp, szIp));
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_bitmap_error]++  ;
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_Fail]++  ;
            return IP_GENERAL_ERROR;
        }
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_unused]++  ;
        break;

    case IPPOOL_IPSTATUS_CONFLICT:
        /*Cannot determine the conflict. The conflict status is the same as INUSE.*/
        if(OK != ipPoolChangeIpStatusInPool(ulPoolIndex,ulSpecialIp,
                                           IPPOOL_IPSTATUS_USED,LEASE_ABLE))
        {
            DHCP_SERVER_DBG("IPPOOL_RenewIpTime->IPaddr %s bitmap error",ipPoolIPtoStr(ulSpecialIp, szIp));
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_bitmap_error]++  ;
            gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_Fail]++  ;
            return IP_GENERAL_ERROR;
        }
        /*Assume that this address is updated from the conflicting state*/
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_conflict]++  ;
        break;

    case IPPOOL_IPSTATUS_INVALID:
        DHCP_SERVER_DBG("IPPOOL_RenewIpTime->IPaddr %s invalid",ipPoolIPtoStr(ulSpecialIp, szIp));
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_InValid]++  ;
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_Fail]++  ;
        return IP_IN_INVALID;

    case IPPOOL_IPSTATUS_DISABLE:
        DHCP_SERVER_DBG("IPPOOL_RenewIpTime->IPaddr %s disable",ipPoolIPtoStr(ulSpecialIp, szIp));
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_disable]++  ;
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_Fail]++  ;
        return IP_IN_DISABLE;

   /*Renew lease*/
    case IPPOOL_IPSTATUS_USED:
        IpRenewLeaseTime(ulPoolIndex,ulSectIndex,usIndex);
        DHCP_SERVER_DBG("IPPOOL_RenewIpTime->IPaddr %s renew",ipPoolIPtoStr(ulSpecialIp, szIp));
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_used]++  ;
        break;
    default :
        DHCP_SERVER_DBG("IPPOOL_RenewIpTime->IPaddr %s unknown",ipPoolIPtoStr(ulSpecialIp, szIp));
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_unknown]++  ;
        gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_Fail]++  ;
        return IP_IN_UNKNOW;
      }
      gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_Renew_success]++  ;
      return IP_SUCCESS;
}

#ifdef GBN_INCLUDE_STACK_ISF
STATUS IPPOOL_RenewIpTime_to_others(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    int iRet;
    SMD_MSG msg;
    SMD_ARG args[2];

    smdMsgInit(&msg, SMD_DEV_ALL, ISF_IPPOOL_RENEWIPTIME, SMD_GLOBAL);

    args[0].pBuf = &ulGatewayIp;
    args[0].usFlags = 0;
    args[0].usLen = sizeof(ulong_t);

    args[1].pBuf = &ulSpecialIp;
    args[1].usFlags = 0;
    args[1].usLen = sizeof(ulong_t);

    iRet = smdMsgRequest(&msg, args, 2, SMD_TIMEOUT_DEFAULT);
    if ( 0 != iRet )
    {
        iRet = ERROR;
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:request failed iRet = %d", __func__, iRet);
    }

    return iRet;
}

int IPPOOL_RenewIpTime_to_others_wrap(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    if ( 2 != ulArgsNum )
    {
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return ERROR;
    }

    IPPOOL_RenewIpTime_local(*((ulong_t *)args[0].pBuf), *((ulong_t *)args[1].pBuf));

    return OK;
}
#endif

/************************************************************
Function: IPPOOL_RenewIpTime
Description: This function extends the lifetime lease of an IP address in the address pool.
Calls: Pass an index IP address (typically the router IP address in the address pool parameters) 
        and the user's IP address to have the address pool extend the lifetime lease of this IP address.
Input: ulGatewayIp: The index IP address in the address pool
        ulSpecialIp: The IP address to be renewed
        // In network byte order, it should belong to the same network segment as ulGatewayIp.
Return Value:
OK: Success, indicating that the IP address is available and the renewal was successful.
The address pool should update the lifetime lease of this IP address.
Note: This function will succeed if the address is not in use.
FAILURE: Failure, indicating that the IP address cannot be used. (The address pool can also define various return values ??to indicate the cause of the error.)
Others: // Check the status of the specified address.
        // If the address is idle or conflicting, assign the address.
        // If the address is in use, update the address expiration time.
**************************************************************/
STATUS IPPOOL_RenewIpTime(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    STATUS iRet;

    iRet = IPPOOL_RenewIpTime_local(ulGatewayIp, ulSpecialIp);
#ifdef GBN_INCLUDE_STACK_ISF
    if ( (IP_SUCCESS == iRet) && (TRUE == isf_stack_enable()) && (TRUE == isf_stack_is_master()) )
    {
         IPPOOL_RenewIpTime_to_others(ulGatewayIp, ulSpecialIp);
    }
#endif

    return iRet;
}

/************************************************************
Function: ipPoolAddLeaseCount
Description: Increment the lease counter by 1.
**************************************************************/
STATUS ipPoolAddLeaseCount(IpPoolLink_S *pIpRefLink,ushort_t usIndex,uchar_t ucPool,uchar_t ucSect)
{

    IpPoolLink_S *pIpLink;
    ulong_t ulTemp,ulTemp1;

    ulTemp = 0;
    ulTemp1 = 0;

    if(pIpRefLink == NULL)
        return FAILURE;

    if(usIndex >= IPPOOL_MAX_IPPerSection_NUM)
        return FAILURE;

    pIpLink = pIpRefLink;
    ulTemp = pIpLink->pusIpLink[usIndex].ulLeaseCount & 0xffffff;//Take out the low position
    ulTemp1 = pIpLink->pusIpLink[usIndex].ulLeaseCount & 0xff000000;
    if(ulTemp >= 0xffffff -1)//Out of counting range
    {
        DHCP_SERVER_DBG("IPPOOL: lease counter out of range");//This error should not occur when the lease period is 
                                                                //less than 999 days and the count interval is 0.5 minutes.
        return FAILURE ;//The counter is out of normal range
    }
    ulTemp++;
    pIpLink->pusIpLink[usIndex].ulLeaseCount  = ulTemp | ulTemp1 ;
    return SUCCESS ;
}

/************************************************************
Function: ipPoolClearLeaseCount
Description: Clears the lease counter.
**************************************************************/
STATUS ipPoolClearLeaseCount(IpPoolLink_S *pIpRefLink,ushort_t usIndex)
{
    IpPoolLink_S *pIpLink;

    if(pIpRefLink == NULL)
        return ERROR;
    if(usIndex >= IPPOOL_MAX_IPPerSection_NUM)
        return ERROR;
    pIpLink =   pIpRefLink;
    pIpLink->pusIpLink[usIndex].ulLeaseCount &= 0xff000000;
    return OK ;
}

/************************************************************
Function: ipPoolGetLeaseCount
Description: Retrieves the value of a counter
**************************************************************/
STATUS ipPoolGetLeaseCount(IpPoolLink_S *pIpRefLink,ushort_t usIndex,ulong_t *pulCount,uchar_t ucPool,uchar_t ucSect)
{
    IpPoolLink_S *pIpLink;
    ulong_t ulTemp;

    ulTemp = 0;

    if(pIpRefLink == NULL)
        return FAILURE;

    if(usIndex >= IPPOOL_MAX_IPPerSection_NUM)
        return FAILURE;

    pIpLink = pIpRefLink;
    ulTemp = pIpLink->pusIpLink[usIndex].ulLeaseCount & 0xffffff;//Take out the low position
    if(ulTemp >= 0xffffff -1)//Out of counting range
    {
        DHCP_SERVER_DBG("IPPOOL: lease counter out of range");//This error should not occur when the lease period is less 
                                                                //than 999 days and the count interval is 0.5 minutes.
        return FAILURE ;
    }
    *pulCount = ulTemp;
    return SUCCESS ;
}



STATUS IpRenewLeaseTime(ulong_t ulPoolIndex, ulong_t ulSectIndex,
                                ushort_t usIndex)
{
    if((ulPoolIndex >= IPPOOL_MAX_POOL_NUM)||
          (ulSectIndex >= IPPOOL_MAX_IPSection_NUM)||
          (usIndex >= IPPOOL_MAX_IPPerSection_NUM))
            return ERROR;

    ipPoolClearLeaseCount(&(gIpPool[ulPoolIndex].IpSection[ulSectIndex].stIpLink)
                         , usIndex);
    return OK ;
}


/************************************************************
Function: ipPoolGetLeaseFlag
Description: This function retrieves the lease usage flag at a specific address in the linked list.
Calls: Sets the lease usage flag to disable or enable.
Output: To save memory, this flag is appended to the highest byte of the lease.
Return: If the highest byte of the lease is 0, the lease is not in use.
Others: If the highest byte of the lease is 1, the lease is in use.
**************************************************************/
STATUS ipPoolGetLeaseFlag(IpPoolLink_S *pIpRefLink,ushort_t usIndex,uchar_t *pucFlag)
{

    IpPoolLink_S *pIpLink;
    ulong_t ulTemp;
    uchar_t ucFlag;
    ulTemp = 0;
    ucFlag = 0;
    if(pIpRefLink == NULL)
        return FAILURE;

    if(usIndex >= IPPOOL_MAX_IPPerSection_NUM)
        return FAILURE;

    pIpLink = pIpRefLink;
    ulTemp = pIpLink->pusIpLink[usIndex].ulLeaseCount & 0xff000000;//Set the highest byte
    ulTemp >>= 24;
    ulTemp &= 0xff;
    ucFlag = (uchar_t)ulTemp;
    if((ucFlag != LEASE_ABLE)&&(ucFlag != LEASE_DISABLE))
       return FAILURE ;
    *pucFlag = ucFlag;
    return SUCCESS ;
}



/************************************************************
Function: HandleIpAddrLeaseTimer()
Description: Handles IP address timer expiration.
**************************************************************/
void HandIpAddrLeaseTimer()
{
    //STATUS rc;
    uchar_t  i, j, ucStatus,ucFlag;
    ushort_t usCurrIndex,usTempIndex;
    IpPoolLink_S *pIpLink;
    IpSectionBitMap_S *pBitMap;
    ulong_t ulCount,ulFreeIp ;
    char szPoolName[IPPOOL_MAX_POOL_NAME];
	char cycleTime = 0;

    i =j =0;
    ulCount = 0;
    usCurrIndex = usTempIndex = IPPOOL_MAX_IPPerSection_NUM;
    ucStatus = IPPOOL_IPSTATUS_UNUSED;
    ucFlag = LEASE_DISABLE;
	osal_memset(&szPoolName,0, IPPOOL_MAX_POOL_NAME);
#if 0
again:
    if( gCycleTime >= IPPOOL_MAX_TOTAL_SECTION_NUM)
    {
        gCycleTime = 0;
        gulCurIndex[0] = 0;
        gulCurIndex[1] = 0;
    }
    else
    {
        gCycleTime ++;
    }

    rc = ccdGetNext("gbnL3IPPool:ipSectionIpCount", gulCurIndex, &idxCnt, &sectLength,&len);
    if( rc!= OK )
        return;
#endif

	for (i = gulCurIndex[0]; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		cycleTime ++;
		if (cycleTime > 4)
			return;

		if (gulCurIndex[0] < IPPOOL_MAX_POOL_NUM - 1)
			gulCurIndex[0] ++;
		else
			gulCurIndex[0] = 0;

		if (!strcmp(gIpPool[i].szName, szPoolName))
		{
			continue;
		}

	    if(0 == gIpPool[i].ulLeaseTime)
	    {
			continue;
	    }

		for (j = 0; j < IPPOOL_MAX_IPSection_NUM; ++j)
		{
		    if(gIpPool[i].IpSection[j].usLength == 0)
		    {
				continue;
		    }

		    /*The number of addresses in use is not 0*/
		    if(gIpPool[i].IpSection[j].ulInUsed == 0)
		    {
				continue;
		    }

		   /* Check the linked list of addresses in use in this address segment. 
           If the address is released after the timeout, consider whether to notify other modules? */
		    pIpLink = &gIpPool[i].IpSection[j].stIpLink;
		    pBitMap = &gIpPool[i].IpSection[j].Bitmap;
		    usCurrIndex =  pIpLink->usUsedHead;

		    while(usCurrIndex < IPPOOL_MAX_IPPerSection_NUM)
		    {
		        ipPoolGetLeaseFlag(pIpLink,usCurrIndex, &ucFlag);

		        /*This address does not require lease management*/
		        if(ucFlag == LEASE_DISABLE)
		        {
		            usCurrIndex =
		                  pIpLink->pusIpLink[usCurrIndex ].usNext;
		            continue;
		        }
		        ipPoolAddLeaseCount(pIpLink, usCurrIndex,i,j);

		        DHCP_SERVER_DBG("CHECK pool[%d].section[%d].ip[%d]: lease time %d",
		                             i,j,usCurrIndex,
		          pIpLink->pusIpLink[usCurrIndex ].ulLeaseCount & 0xffffff);
		        /*Not expired*/
		        ulFreeIp = gIpPool[i].IpSection[j].ulStartIp + usCurrIndex;
		        ipPoolGetLeaseCount(pIpLink, usCurrIndex, &ulCount,i,j);
		        if(( SC_IPPOOL_CYCLE_TIME * ulCount)
		                < gIpPool[i].ulLeaseTime)
		        {
					DHCP_SERVER_DBG("lease time flag_discover=%d", pIpLink->pusIpLink[usCurrIndex].flag_discover);
		            /*If the flag_discover flag is 1, it means that the server has not received the request message, and ulCount = 20, 
                    that is, the IP address will be reclaimed after a timeout of 10 minutes */
					if( pIpLink->pusIpLink[usCurrIndex].flag_discover && (ulCount == 20) )
		          	{
		  		        DHCP_SERVER_DBG("DHCP Server have not received dhcp_request package  of ip %08x",
		                                             ulFreeIp );
					}
		            else
		          	{
		            usCurrIndex =
		              pIpLink->pusIpLink[usCurrIndex ].usNext;
		            continue;
		          	}
		        }

		        /*ulLeaseTime is in seconds. If the total timeout duration of an address is greater than the lease duration, 
                the address has expired and the address will be released.*/
		        GetIpBitmapState(pBitMap,
		                            (usCurrIndex+SC_MIN_IPPOOL_ALLOCINDEX),
		                            &ucStatus);
		        switch (ucStatus)
		        {
		            case IPPOOL_IPSTATUS_USED:
		                FreeToIpSectionBitmap( pBitMap, (usCurrIndex+SC_MIN_IPPOOL_ALLOCINDEX) );
		                usTempIndex = usCurrIndex;
		                usCurrIndex = pIpLink->pusIpLink[usCurrIndex ].usNext;
		                if(SUCCESS != DelFromUsedLink(pIpLink, usTempIndex,i,j))
		                    break;
		                gIpPool[i].IpSection[j].ulInUsed--;

		                /*Reinsert the free list*/
		                if(SUCCESS == FreeToIdleLink(pIpLink, usTempIndex))
		                {
		                    gIpPool[i].IpSection[j].ulUnUsed++;
		                    gIpPool[i].IpPoolMsgNum[IpPool_release_ip_success]++;
		                    gIpPool[i].IpPoolMsgNum[IpPool_Timeout_release_ip]++;
		                    /*Notify the DHCP Server to reclaim this IP address*/
		                    dhcpserver_romovehash_by_ip(ulFreeIp);
		                }
		                break;
		            default :
		            	/*According to different error types*/
		            	IPPOOL_Error_static(i,ucStatus, IPPOOL_RELEASE_FLAG);
		                gIpPool[i].IpPoolMsgNum[IpPool_release_ip_fail]++;
		                usCurrIndex = pIpLink->pusIpLink[usCurrIndex ].usNext;
		                break;
		        }
		    }
		}
	}
}



/************************************************************
Function: HandleIpAddrConflictTimer()
Description: Handles IP address timer timeouts.
**************************************************************/
void HandIpAddrConflictTimer()
{
    uchar_t  i,j,ucStatus,ucFlag;
    ushort_t usCurrIndex,usTempIndex;
    IpPoolLink_S *pIpLink;
    IpSectionBitMap_S *pBitMap;
    ulong_t ulCount,ulFreeIp ;
    //STATUS rc;
    char szPoolName[IPPOOL_MAX_POOL_NAME];
	char cycleTime = 0;

    i =j =0;
    ulCount = 0;
    usCurrIndex = usTempIndex = IPPOOL_MAX_IPPerSection_NUM;
    ucStatus = IPPOOL_IPSTATUS_UNUSED;
    ucFlag = LEASE_DISABLE;
	osal_memset(&szPoolName,0, IPPOOL_MAX_POOL_NAME);
	#if 0
    if( gCycleTime2 >= 30)
    {
        gCycleTime2 = 0;
        gulConflictIndex[0] = 0;
        gulConflictIndex[1] = 0;
    }
    else
    {
        gCycleTime2 ++;
    }

    //rc = ccdGetNext("gbnL3IPPool:ipSectionIpCount", gulConflictIndex, &idxCnt, &sectLength,&len);
    //if( rc!= OK )
    //    return;
	#endif
	for (i = 0; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		cycleTime ++;
		if (cycleTime > 4)
			return;

		if (gulConflictIndex[0] < IPPOOL_MAX_POOL_NUM - 1)
			gulConflictIndex[0] ++;
		else
			gulConflictIndex[0] = 0;

		if (!strcmp(gIpPool[i].szName, szPoolName))
		{
			continue;
		}

	    if(0 == gIpPool[i].ulLeaseTime)
	    {
			continue;
	    }

		for (j = 0; j < IPPOOL_MAX_IPSection_NUM; ++j)
		{
		    if(gIpPool[i].IpSection[j].usLength == 0)
		    {
				continue;
		    }

		    //i = (uchar_t )(gulConflictIndex[0]-1);
		    //j = (uchar_t )(gulConflictIndex[1]-1);
		    /*The number of addresses in conflict state is not 0*/
		    if(gIpPool[i].IpSection[j].ulConflict== 0)
		        return;

		    /* Check the conflict list of this address segment */
		    pIpLink = &gIpPool[i].IpSection[j].stIpLink;
		    pBitMap = &gIpPool[i].IpSection[j].Bitmap;
		    usCurrIndex =  pIpLink->usConflictHead;

		    while(usCurrIndex < IPPOOL_MAX_IPPerSection_NUM)
		    {
		        ipPoolGetLeaseFlag(pIpLink,usCurrIndex, &ucFlag);

		        /*This address does not require lease management*/
		        if(ucFlag == LEASE_DISABLE)
		        {
		            usCurrIndex =
		              pIpLink->pusIpLink[usCurrIndex ].usNext;
		            continue;
		        }
		        ipPoolAddLeaseCount(pIpLink, usCurrIndex,i,j);

		        ipPoolGetLeaseCount(pIpLink, usCurrIndex, &ulCount,i,j);
		        if((( SC_IPPOOL_CYCLE_TIME * ulCount)< IPPOOL_CONFLICT_PERIOD)
		        	&&((SC_IPPOOL_CYCLE_TIME * ulCount)< gIpPool[i].ulLeaseTime))
		        {
		            usCurrIndex =
		              pIpLink->pusIpLink[usCurrIndex ].usNext;
		            continue;
		        }
		        GetIpBitmapState(pBitMap,
		                            (usCurrIndex+SC_MIN_IPPOOL_ALLOCINDEX),
		                            &ucStatus);
		        switch (ucStatus)
		        {
		            case IPPOOL_IPSTATUS_CONFLICT:
		                FreeToIpSectionBitmap( pBitMap, (usCurrIndex+SC_MIN_IPPOOL_ALLOCINDEX) );
		                usTempIndex = usCurrIndex;
		                usCurrIndex = pIpLink->pusIpLink[usCurrIndex ].usNext;
		                if(SUCCESS != DelFromConflictLink(pIpLink, usTempIndex,i,j))
		                    break;
		                gIpPool[i].IpSection[j].ulConflict--;

		                /*Reinsert the free list*/
		                if(SUCCESS == FreeToIdleLink(pIpLink, usTempIndex)) 
		                {
		                    gIpPool[i].IpSection[j].ulUnUsed++;
		                    gIpPool[i].IpPoolMsgNum[IpPool_release_ip_success]++;
		                    gIpPool[i].IpPoolMsgNum[IpPool_Timeout_release_ip]++;
		                    /*Notify the DHCP Server to reclaim this IP address*/
		                ulFreeIp = gIpPool[i].IpSection[j].ulStartIp + usTempIndex;
		                dhcpserver_romovehash_by_ip(ulFreeIp);
		                }
		                break;
		            default :
		            	/*According to different error types*/
		            	IPPOOL_Error_static(i,ucStatus, IPPOOL_RELEASE_FLAG);
		                gIpPool[i].IpPoolMsgNum[IpPool_release_ip_fail]++;
		                usCurrIndex = pIpLink->pusIpLink[usCurrIndex ].usNext;
		                break;
		        }
		    }

		}
	}
}


/****************************************************************************************************
Function: ipPoolSetDiscoverFlag
Description: This function is used to set the DHCP discover flag, which is used to calculate the timeout 
             value when the server sends the offer message.
Input: ulong_t ulGatewayIp //Gateway IP
        ulong_t ulIp //Applied IP address
        uchar_t flag //Flag, flag = 1, the server sends a DHCPoffer message,
                    //flag = 0, receives a reply to the DHCPoffer message.
Return value: OK if successful, error code if failed.
****************************************************************************************************/

STATUS ipPoolSetDiscoverFlag(ulong_t ulGatewayIp, ulong_t ulIp, uchar_t flag)
{
    ushort_t usIndex ;
    char szIp[20];
    ulong_t ulPoolIndex,ulSectIndex;
    IpPoolLink_S *pIpLink;
	usIndex = (ushort_t)-1;
    ulSectIndex = (ulong_t)-1;
    ulPoolIndex = IPPOOL_MAX_POOL_NUM;//Initialized to an invalid value


    if(ulIp == 0)
    {
        DHCP_SERVER_DBG("IPPOOL_ipPoolSetDiscoverFlag->the ulIp=0");
        return IP_NOT_IN_POOL;
    }

    if( OK!= ipPoolGetPoolIndexByGateIp( ulGatewayIp, &ulPoolIndex ))
    {
        DHCP_SERVER_DBG("IPPOOL_ipPoolSetDiscoverFlag->this router ip %s does not exist",ipPoolIPtoStr(ulGatewayIp,(uchar_t *) szIp));
        return FIND_NO_POOL_INDEX ;
    }

    if(OK != FindIpSectionByIp(ulPoolIndex, ulIp, &ulSectIndex, &usIndex))
    {
        DHCP_SERVER_DBG("IPPOOL_ipPoolSetDiscoverFlag->There is not ip %s in this pool",ipPoolIPtoStr(ulIp, (uchar_t *)szIp));
        return IP_NOT_IN_POOL ;
   }

    pIpLink = &gIpPool[ulPoolIndex].IpSection[ulSectIndex].stIpLink;

    pIpLink->pusIpLink[usIndex].flag_discover = flag;

    DHCP_SERVER_DBG("IPPOOL_ipPoolSetDiscoverFlag->pool[%d].section[%d].ip[%d]  flag_discover: %d",
                            ulPoolIndex,ulSectIndex,usIndex,flag);
    return OK;
}

/****************************************************************************************************
Function: IPPOOL_CheckSpecialIp
Description: Checks if a given special IP belongs to a specific IP pool associated with a gateway IP.
Input: ulGatewayIp - The gateway IP address used to identify the IP pool
        ulSpecialIp - The special IP address to check within the pool
Return: STATUS Returns FIND_NO_POOL_INDEX when the pool for gateway IP is not found
****************************************************************************************************/

STATUS IPPOOL_CheckSpecialIp(ulong_t  ulGatewayIp, ulong_t ulSpecialIp )
{
    ushort_t usIndex ;
    ulong_t ulPoolIndex,ulSectIndex;
    usIndex = (ushort_t)-1;
    ulSectIndex = (ulong_t)-1;
    ulPoolIndex = IPPOOL_MAX_POOL_NUM -1;//Initialized to an invalid value

    if( OK!= ipPoolGetPoolIndexByGateIp( ulGatewayIp, &ulPoolIndex ))
    {
        return FIND_NO_POOL_INDEX ;
    }
    if(ulSpecialIp == 0)
    {
        return IP_NOT_IN_POOL;
    }

    gIpPool[ulPoolIndex].IpPoolMsgNum[IpPool_request_alloc_Ip ]++;
    if(OK != FindIpSectionByIp(ulPoolIndex, ulSpecialIp, &ulSectIndex, &usIndex))
    {
        return IP_NOT_IN_POOL ;
   }

   return IP_SUCCESS;
}

#endif /*GBN_INCLUDE_IPPOOL*/
