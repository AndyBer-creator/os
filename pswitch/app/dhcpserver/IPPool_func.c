/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  File Name:  ippool_func.c
  Autor: Kniazkov Andrew
  Description: This file defines configuration of address pools and segments, as well as parameter validation.
                Required interface functions
********************************************************************************/
#include <common/sys_def.h>
#include "dhcps.h"
#ifdef  GBN_INCLUDE_IPPOOL
#include "ip_pool.h"

#define OK SYS_ERR_OK
#define ERROR SYS_ERR_FAILED
ulong_t gulIPSectionNum;
ulong_t gulTotalIpNum;
AAA_IPPOOL_S gIpPool[IPPOOL_MAX_POOL_NUM];
ulong_t ipPool_RowStatus[IPPOOL_MAX_POOL_NUM];
IPPOOL_IPDisable_S gDisableIp[IPPOOL_ALL_MAX_DISABLEIP_NUM];

ulong_t g_bIpPoolDebug = FALSE;  /*Address pool module debugging switch*/

/*declarations*/

/************************************************************
 Function Name: ipPoolCheckNameStr
 Description: Checks the domain name for legality and illegal characters
 Input: szName: Domain name to be checked
 Output:
 Return Value: OK: Success; ERROR: Failure
**************************************************************/
STATUS ipPoolCheckNameStr(char *szName)
{
    STATUS ulErr = OK;
    char *pName;

    if(NULL == szName)
    {
        ulErr = 1;
    }
    else if (*szName == '\0')
    {
        ulErr = 2;
    }
    else
    {
        ulErr = 0;
        pName = szName;
        if(('a'<=*pName && *pName<='z')||('A'<=*pName && *pName<='Z'))
        {
            pName++;
            while(*pName)
            {
                 /* Check the validity of input characters to ensure future support for Chinese user names*/
                /* Therefore, we temporarily exclude control characters with ASCII values ??0 to 32 and characters 
                specified as illegal by cInvalid*/
                /* All other characters are considered valid*/

                 if(('a'<=*pName && *pName<='z')||('A'<=*pName && *pName<='Z')||('0'<=*pName && *pName<='9'))
                 {
                     pName++;
                 }
                 else
                 {
                     ulErr = 1;
                     break;
                 }
            }
        }
        else
        {
           ulErr = 1;
        }
    }

    if(0 == ulErr)
    {
        return OK;
    }
    else
    {
        return ERROR;
    }

}

/**************************************************************
 Function Name: IPPOOL_IP_CHECK
 Description: To distinguish whether the IP address is a normal IP address.
 Input: IP: The IP address to be checked.
 Output:
 Return Value: SUCCESS: The IP address is a normal IP address.
               FAILURE: The IP address isn't a normal IP address.
***************************************************************/
STATUS ipPoolCheckIpValid(ulong_t ulIP)
{
    ulong_t ulTemp;

    if (ulIP == 0)
    {
        return SYS_ERR_OK;
    }
    ulTemp = ulIP>>24;
    if ((ulTemp == 0) || (224 <= ulTemp) || (127 == ulTemp)) /* 0 is the network segment, 224 and above are class D and E addresses */
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

/************************************************************
Function: CheckHostIpValid(ulIp)
Description: Checks the validity of the IP address in combination with the mask.
Input: IP address
Output: Success if valid; Failure if invalid
**************************************************************/
STATUS ipPoolCheckIpAndMaskValid(ulong_t ulIpAddr,ulong_t ulMask)
{
    uchar_t IpFirstByte;

    IpFirstByte = (uchar_t)(ulIpAddr>>24);
    if ((127 == IpFirstByte) || (0 == IpFirstByte))
    {
        return HostIpType_127;
    }

    if (IpFirstByte >= 224)
    {
        return HostIpType_IP_MultiCast;
    }

    if (((ulIpAddr&(~ulMask))== 0)||((ulIpAddr|ulMask)==0xffffffff))
    {
        return HostIpType_SUBNET_NET_BROAD;
    }

    return HostIpType_NORMAL;

}


/*******************************************************************************
Function: uchar_t *ipPoolIPtoStr (ulong_t ulIPAddr, uchar_t * string)
Description: Converts an IP address represented by a ulong_t string to a string.
Input: ulong_t ulIPAddr (IP address)
Output: uchar_t * string (IP address)
Returns: OK on success, ERROR on failure
*******************************************************************************/
uchar_t  *ipPoolIPtoStr (ulong_t ulIPAddr, uchar_t * string)
{
    osal_sprintf((char *)string,"%u.%u.%u.%u",
    	ulIPAddr>>24,(ulIPAddr>>16)&0xff,
    	(ulIPAddr>>8)&0xff,ulIPAddr&0xff);
    return string;
}

STATUS ipPoolGetPoolIndexByName(char * szPoolName, ulong_t * pPoolIndex)
{
    ulong_t idx;

    if((strlen(szPoolName) >= IPPOOL_MAX_POOL_NAME)||(strlen(szPoolName)== 0))
        return SYS_ERR_FAILED;

    for (idx = 0; idx < IPPOOL_MAX_POOL_NUM; ++idx)
    {  
		if (!strcmp(gIpPool[idx].szName, szPoolName))
		{
			*pPoolIndex = idx;
			return SYS_ERR_OK;
		}
    }
    return SYS_ERR_FAILED;
}


/************************************************************
Function: ulong_t CheckPOOLIsNull(uchar_t ucIndex)
Description: Checks whether an address pool exists.
Input: Index of the address pool group
Output:
Return: Returns SUCCESS if the address pool is still in use, otherwise FAILURE.
**************************************************************/
STATUS ipPoolCheckIsIpPoolNull(ulong_t ulIndex)
{
	#if 0
    //STATUS rc;
    ulong_t ulIpcount,len,idx[2]={0,0};

    /*Index starts at 0*/
    if(ulIndex >= IPPOOL_MAX_POOL_NUM)
    {
        return SYS_ERR_FAILED ;
    }
    idx[0] = ulIndex+1;
    for(idx[1]=1 ; idx[1]<=IPPOOL_MAX_IPSection_NUM; idx[1]++)
    {

        /*If the number of addresses is not 0, the address pool is not empty*/
        rc = ccdGet("gbnL3IPPool:ipSectionIpCount", idx, 2, &ulIpcount, &len);
        if((rc==OK)&&(ulIpcount != 0))
        {
            return ERROR;
        }

    }
    #endif
    return SYS_ERR_OK;
}


/************************************************************
 Function: STATUS ipPoolCheckIsIpPoolInUse(ulong_t ulIndex)
 Description: Checks if the address pool group is still in use.
 Input: Index of the address pool group
 Output:
 Return: If it is still in use, it returns a success value (OK); otherwise, it returns a failure value (ERROR).
**************************************************************/
STATUS ipPoolCheckIsIpPoolInUse(ulong_t ulIndex)
{
    ulong_t i;
    if(ulIndex >= IPPOOL_MAX_POOL_NUM)
    {
        DHCP_SERVER_DBG("InitIpPool->pool index is out of range");
        return SYS_ERR_FAILED;
    }
    for(i=0 ; i<IPPOOL_MAX_IPSection_NUM; i++)
    {
        /*The number of addresses in use is not 0*/
        if(gIpPool[ulIndex].IpSection[i].ulInUsed != 0)
        {
            DHCP_SERVER_DBG("ipPoolCheckIsIpPoolInUse->pool %u section %u is in use", ulIndex, i );
            return SYS_ERR_OK;
        }

    }
    DHCP_SERVER_DBG("ipPoolCheckIsIpPoolInUse->pool %u is not used",ulIndex);
    return SYS_ERR_FAILED;
}



STATUS IPPOOL_CheckIfPoolValid(ulong_t ulPoolIndex)
{
    STATUS rc = SYS_ERR_OK;
    ulong_t idx,ulGatewayIp,ulMask,ulGetGatewayIp,ulGetMask;
    if( strlen((char *)gIpPool[ulPoolIndex].szName) == 0)
        return SYS_ERR_FAILED;
    if( SYS_ERR_OK != ipPoolCheckIpAndMaskValid(gIpPool[ulPoolIndex].ulGatewayIp, gIpPool[ulPoolIndex].ulSubnetMask))
        return SYS_ERR_FAILED;

    ulGatewayIp = gIpPool[ulPoolIndex].ulGatewayIp;
    ulMask = gIpPool[ulPoolIndex].ulSubnetMask;
    for(idx = 0;idx<IPPOOL_MAX_POOL_NUM;idx++)
    {
        if(idx == ulPoolIndex)
            continue;
        if( (ipPool_RowStatus[idx]==ROW_DESTROY)
        	||(gIpPool[ulPoolIndex].ulGatewayIp==0))
            continue;
        ulGetGatewayIp = gIpPool[idx].ulGatewayIp;
        ulGetMask = gIpPool[idx].ulSubnetMask;
        if( ((ulGatewayIp&ulMask)==(ulGetGatewayIp&ulMask))
        	||((ulGatewayIp&ulGetMask)==(ulGetGatewayIp&ulGetMask)))
        {
            return SYS_ERR_FAILED;
        }
    }

    return rc;
}

/************************************************************
Function: ulong_t ipPoolCheckSectionOverlap
Description: This function takes into account the issue of no upper address limit.
             When there is no upper address limit, the parameter is -1.
Input: ulong_t ulGroupIndex (address pool index)
       ulong_t ulStartIp (starting address)
Output: ulong_t ulEndIp (ending address)
Return:
**************************************************************/

ulong_t ipPoolCheckIpSectionOverlap(ulong_t ulPoolIndex,
								ulong_t ulSectionIndex,
                              	ulong_t ulStartIp,
                              	ulong_t ulEndIp)
{

    AAA_IPSection_S *pIpSect ;
    ulong_t i,j;

    for(j=0;j<IPPOOL_MAX_POOL_NUM;j++)
    {
        pIpSect = &(gIpPool[j].IpSection[0]);
        /*Check if <startIp,endIp> overlaps with the existing address pool*/
        for(i=0; i<IPPOOL_MAX_IPSection_NUM; i++)
        {
            /*Empty address segment, search for the next one*/
            if((0 == (pIpSect+i)->usLength )||((pIpSect+i)->ulSectionRowStatus!=ROW_ACTIVE))
                continue;
            if((j == ulPoolIndex)&&(i == ulSectionIndex))
            {
                if((pIpSect+i)->ulInUsed != 0)
                {
                    return Sect_InUsed;
                }
                continue;
            }
            if((ulStartIp > (pIpSect+i)->ulEndIp)||(ulEndIp < (pIpSect+i)->ulStartIp))
            {
                /*Do not overlap and continue to check the next one*/
                continue ;
            }
            else
            {
                return Sect_Overlap;
            }
        }
    	}
    return SYS_ERR_OK;//There is no address pool conflict, and the result is a success message.
}

/************************************************************
Function: CheckIpAndRouterIpIsSame
Description: Checks whether the starting and ending addresses of the address segment are the same as the gateway.
Calls: On the same network segment
Table Accessed: If the gateway address is not configured, the function will return failure.
Input:
Output:
Return:
**************************************************************/

STATUS ipPoolCheckIpAndGatewaySame(ulong_t ulPoolIndex,
                              ulong_t ulStartIp,
                              ulong_t ulEndIp)
{
    ulong_t ulRouterMask,ulStartIpmask,ulEndIpmask;
    ulRouterMask = 0;
    ulStartIpmask =0;
    ulEndIpmask = 0;

    /*This module is used internally, no longer checks parameters*/
    #if 0
    if(ulPoolIndex >= IPPOOL_MAX_POOL_NUM)
    {
        return ERROR;
    }
    #endif
    if(gIpPool[ulPoolIndex].ulGatewayIp == 0)
    {
        DHCP_SERVER_DBG("\r\n No gateway configured");
        return ERROR;
    }
    ulRouterMask  = gIpPool[ulPoolIndex].ulGatewayIp & gIpPool[ulPoolIndex].ulSubnetMask;
    ulStartIpmask = ulStartIp & gIpPool[ulPoolIndex].ulSubnetMask;
    ulEndIpmask   = ulEndIp & gIpPool[ulPoolIndex].ulSubnetMask;
    if((ulStartIpmask != ulRouterMask)||(ulEndIpmask != ulRouterMask))
    {
        #if 0
        ippool_debug_printf("\r\n The address and gateway are not in the same network segment",
                "\r\nthe ip address and router ip are not in the same subnet.");
        #endif
        return SYS_ERR_FAILED;
    }
    if((gIpPool[ulPoolIndex].ulGatewayIp >= ulStartIp)&&
       (gIpPool[ulPoolIndex].ulGatewayIp <= ulEndIp))
    {
     #if 0
        ippool_debug_printf("\r\n The gateway address should not be included in the scope's content",
              "\r\nThere should not be the router ip inside the scope.");
     #endif
        return SYS_ERR_FAILED;
    }
    return SYS_ERR_OK;
}

/************************************************************
Function: CheckIpAndRouterIpIsSame
Description: Checks whether the starting and ending addresses of the address segment are the same as the gateway.
Calls: On the same network segment
Called By:
Table Accessed: If the gateway address is not configured, the function will return failure.
Table Updated:
Input:
Output:
Return:
Other:
**************************************************************/

ulong_t CheckIpAndGatewayIpIsSame(uchar_t ucPoolIndex,
                              ulong_t ulStartIp,
                              ulong_t ulEndIp)
{
    ulong_t ulGatewayMask,ulStartIpmask,ulEndIpmask;
    ulGatewayMask = 0;
    ulStartIpmask =0;
    ulEndIpmask = 0;

    if(gIpPool[ucPoolIndex].ulGatewayIp == 0)
    {
        return ERROR_SECTION_NO_GATEWAY;
    }
    ulGatewayMask  = gIpPool[ucPoolIndex].ulGatewayIp & gIpPool[ucPoolIndex].ulSubnetMask;
    ulStartIpmask = ulStartIp & gIpPool[ucPoolIndex].ulSubnetMask;
    ulEndIpmask   = ulEndIp & gIpPool[ucPoolIndex].ulSubnetMask;
    if((ulStartIpmask != ulGatewayMask)||(ulEndIpmask != ulGatewayMask))
    {
        return ERROR_SECTION_IPGATE_NOSAMESUBNET;
    }

    return SYS_ERR_OK;
}

/*Release a bitmap*/
/************************************************************
  Function:      DeInitIpSectionBitmap
  Description:   Release address pool bitmap
  Calls:
  Called By:
  Table Accessed:
  Table Updated:
  Input:         Pointer to bitmap structure
  Output:
  Return:
  Others:
**************************************************************/

STATUS DeInitIpSectionBitmap ( IpSectionBitMap_S * pRefBitmap)
{
    IpSectionBitMap_S *pBitmap ;

    if(NULL == pRefBitmap )
       return SYS_ERR_FAILED;

    pBitmap = pRefBitmap;
    if ( pBitmap->bIsAlloc )
    {
        osal_free((void *)pBitmap->pbBitmapMem );
        pBitmap->pbBitmapMem = NULL;
        pBitmap->bIsAlloc = 0;
    }
    return SYS_ERR_OK;
}

/************************************************************
  Function:      DeInitIpSectionIpLink
  Description:   Delete the free list of ipsection to release memory
  Calls:
  Called By:
  Table Accessed:
  Table Updated:
  Input:         Pointer to the free chain structure
  Output:
  Return:
  Others:
**************************************************************/

STATUS DeInitIpSectionIpLink(IpPoolLink_S *pIpRefLink)
{
    IpPoolLink_S *pIpLink;

    if(pIpRefLink == NULL)
        return SYS_ERR_FAILED;

    pIpLink = pIpRefLink;
    /*When the head pointer points to the maximum number of addresses, it means that the address list is empty.*/
    pIpLink->usIdleHead = IPPOOL_MAX_IPPerSection_NUM;
    pIpLink->usIdleTail = IPPOOL_MAX_IPPerSection_NUM;
    pIpLink->usMaxLength = 0;
    pIpLink->usUsedHead = IPPOOL_NULL_USHORT ;/*Pointing to an illegal value indicates that the address is not in use*/

    pIpLink->usConflictHead = IPPOOL_NULL_USHORT;
    if(pIpLink->pusIpLink != NULL)
    {
        osal_free(pIpLink->pusIpLink);
        pIpLink->pusIpLink = NULL;
    }
    return SYS_ERR_OK ;
}

/************************************************************
  Function:      InitIpSectionBitMap
  Description:   Initialize the bitmap used by the address pool
  Calls:
  Called By:
  Table Accessed:
  Table Updated:
  Input:    *pBitmap:Bitmap structure pointer
            bIsAlloc:Indicates whether this structure allocates memory. If it is 1, it means no memory is allocated.
            *pbBitmapMem:Pointer to the memory where the bitmap is located
            wBitmapSize:The size of the bitmap in bytes
            wMinAllocNo:The smallest number that the bitmap can be divided into
            wMaxAllocNo:The maximum number that a bitmap can be divided into
  Output:
  Return:
  Others:
**************************************************************/

STATUS InitIpSectionBitmap ( IpSectionBitMap_S * pRefBitmap , uchar_t bIsAlloc, ulong_t *pbBitmapMem,
                  ushort_t wBitmapSize, ushort_t wMinAllocNo, ushort_t wMaxAllocNo )
{
    IpSectionBitMap_S *pBitmap ;

    if(NULL == pRefBitmap )
       return SYS_ERR_FAILED ;

    pBitmap = pRefBitmap;

    if( wBitmapSize == 0)
        return SYS_ERR_FAILED;
    if( wMinAllocNo > wMaxAllocNo )
       return SYS_ERR_FAILED;//Considering that a bitmap has only one assignable value

    pBitmap->bIsAlloc = bIsAlloc;
    if ( bIsAlloc )
    {
        /*wBitmapSize is the number of bytes*/
        pBitmap->pbBitmapMem = ( ulong_t *) osal_alloc(wBitmapSize);
        if ( pBitmap->pbBitmapMem == NULL )
        {
            /*IpPool_Info_Printf ( "Bitmap mlloc memory and fails.\n" );*/
            DHCP_SERVER_DBG( "Bitmap mlloc memory and fails." );
            return SYS_ERR_FAILED;
        }
        memset((char*)pBitmap->pbBitmapMem,0,(ulong_t)wBitmapSize);
    }
    else
    {
        pBitmap->pbBitmapMem = pbBitmapMem;
    }

    pBitmap->wBitmapSize = wBitmapSize;/*Indicates the memory size of this bitmap in bytes*/
    pBitmap->wMinAllocNo = wMinAllocNo;/*The minimum index value that can be assigned*/
    pBitmap->wMaxAllocNo = wMaxAllocNo;/*The maximum index value that can be assigned*/
    pBitmap->wFreeNum =  wMaxAllocNo - wMinAllocNo + 1;/*The total number of indexes that can be allocated*/

    return SYS_ERR_OK;
}

/************************************************************
  Function:      InitIpSectionIpLink
  Description: Initialize the free chain of the address pool segment and apply for memory for the address pool chain
  Calls:
  Called By:
  Table Accessed:
  Table Updated:
  Input:     Pointer to the free list structure of the address pool segment, length of the address segment
  Output:
  Return:
  Others:
**************************************************************/
STATUS InitIpSectionIpLink(IpPoolLink_S *pIpRefLink,ushort_t usLength)
{
    ushort_t i;
    ulong_t  ulSize;
    IpPoolLink_S *pIpLink;

    if(pIpRefLink == NULL)
        return SYS_ERR_FAILED;

    pIpLink = pIpRefLink;
    if(( usLength == 0 )||(usLength > IPPOOL_MAX_IPPerSection_NUM))
        return SYS_ERR_FAILED;

    ulSize = usLength*sizeof(IpLinkNode_S);

    pIpLink->pusIpLink =(IpLinkNode_S*)osal_alloc(ulSize);
    if(pIpLink->pusIpLink == NULL)
        return SYS_ERR_FAILED ;

    /*The free list is maximum when initialized*/
    pIpLink->usIdleHead = 0;
    //The head pointer of the free list points to 0 of the array, and the tail pointer points to the end of the array
    pIpLink->usIdleTail = usLength -1;
    pIpLink->usUsedHead = IPPOOL_NULL_USHORT ;//Pointing to an illegal value indicates that the address is not in use
    /*0052*/
    pIpLink->usConflictHead = IPPOOL_NULL_USHORT;

    pIpLink->usMaxLength = usLength;
    for( i = 0; i < usLength; i++ )
    {
        //If usLength is 0xFFFF, there are some problems, but fortunately this is not the case in actual use.
        ipPoolClearLeaseCount(pIpLink, i);
        ipPoolSetLeaseFlag(pIpLink, i, LEASE_DISABLE);
        if(i == usLength-1)
            pIpLink->pusIpLink[i].usNext= (ushort_t)-1;
        else
            pIpLink->pusIpLink[i].usNext= i+1;

        if(i == 0)
            pIpLink->pusIpLink[i].usPre= (ushort_t)-1;
        else
            pIpLink->pusIpLink[i].usPre= i-1;
    }
    return SYS_ERR_OK;
}


/************************************************************
  Function:      ipPoolCreateIpSection
  Description:   Create an address segment
  Calls:
  Called By:
  Table Accessed:
  Table Updated:
  Input:         Address pool index, address segment index, start address, end address
  Output:        This function checks that address segments cannot overlap each other.
  Return:       When checking the validity of the address, set the invalid bit in the response bitmap of the invalid address
    ERROR_SECTION_STARTIP_INVALID, The starting IP address is an illegal IP address
    ERROR_SECTION_ENDIP_INVALID,     Terminate IP as an illegal IP address
    ERROR_SECTION_STARTIP_HIGHER,   The starting address is greater than the ending IP
    ERROR_SECTION_SIZE_TOOBIG,        The address within the segment exceeds the limit of 512
    ERROR_SECTION_INDEX_TOOBIG,      The segment ID number can only be 0-7
    ERROR_SECTION_NUM_FULL            The total number of segments exceeds 36
  Others:    Add different return value types to facilitate command line error determination
**************************************************************/
STATUS ipPoolCreateIpSection(ulong_t ulIpPoolIndex,
                    ulong_t ulIpSectionIndex,
                    ulong_t ulStartIp,
                    ulong_t ulEndIp,
                    ulong_t ulMask)
{

    ulong_t ulIp,ulIpCount,ulTemp,ulTemp1;
    AAA_IPSection_S *pIpSection ;
    STATUS ucError,ulSectErr;
    ulIpCount = ulEndIp - ulStartIp+1;
    ulIp =0;
    ucError = SYS_ERR_FAILED;
    ulSectErr  = SYS_ERR_FAILED;

    if(gulIPSectionNum>=IPPOOL_MAX_TOTAL_SECTION_NUM)
        return DHCP_ERROR_SECTION_NUM_FULL;
    pIpSection = &(gIpPool[ulIpPoolIndex].IpSection[ulIpSectionIndex]) ;
    ucError = (char )ipPoolCheckIpAndMaskValid(ulStartIp,  ulMask);
    if(SYS_ERR_OK != ucError)
        return DHCP_ERROR_SECTION_STARTIP_INVALID;

    ucError = (char )ipPoolCheckIpAndMaskValid(ulEndIp,  ulMask);
    if(SYS_ERR_OK != ucError)
        return DHCP_ERROR_SECTION_ENDIP_INVALID;

    if(ulEndIp < ulStartIp)
    {
        return DHCP_ERROR_SECTION_STARTIP_HIGHER;
    }

    if( (ulEndIp-ulStartIp+1)>IPPOOL_MAX_IPPerSection_NUM)
    	return DHCP_ERROR_SECTION_LENGTH_TOOBIG;

    pIpSection = &(gIpPool[ulIpPoolIndex].IpSection[ulIpSectionIndex]) ;

    if((gulTotalIpNum + ulEndIp - ulStartIp+1) > IPPOOL_MAX_TOTAL_IPADDR_NUM)
    {
        return DHCP_ERROR_SECTION_NUM_FULL;
    }

    if((ulIpPoolIndex >= IPPOOL_MAX_POOL_NUM)
         ||(ulIpSectionIndex >= IPPOOL_MAX_IPSection_NUM))
    {
        return DHCP_ERROR_SECTION_INDEX_TOOBIG;
    }

    if(gIpPool[ulIpPoolIndex].ulGatewayIp == 0)
    {
        return DHCP_ERROR_SECTION_NO_GATEWAY;
    }

    if(SYS_ERR_OK != ipPoolCheckIpAndGatewaySame(ulIpPoolIndex,ulStartIp,ulEndIp))
    {
        return DHCP_ERROR_SECTION_IPGATE_NOSAMESUBNET;
    }
    /*Check if the current address segment overlaps with the previous address segment? 
    Does this function take the mask into account?*/
    ulSectErr = ipPoolCheckIpSectionOverlap(ulIpPoolIndex,ulIpSectionIndex,ulStartIp,ulEndIp);
	switch(ulSectErr)
	{
		case Sect_Overlap:
	        DHCP_SERVER_DBG("The address range of the current address segment overlaps that of any other scope");
			return DHCP_ERROR_SECTION_OVERLAP;
		case Sect_InUsed:
	        DHCP_SERVER_DBG("Error! The address in the address section is in use and the scope cannot be modified");
			return DHCP_ERROR_SECTION_IP_INUSED;
		case Sect_Normal:
			break;
		case Sect_Other:
		default:
			return SYS_ERR_FAILED;
	}

    
    pIpSection = &(gIpPool[ulIpPoolIndex].IpSection[ulIpSectionIndex]) ;
    ipPoolInitIpSection(pIpSection);
    pIpSection->ulStartIp = ulStartIp;
    pIpSection->ulEndIp = ulEndIp;
//    pIpSection->ulMask = ulMask;

//(ulIpCount * IPPOOL_BITMAP_LITTLE_STEP) Number of addresses
//Multiplying by 4 bits indicates the required number of bits. After adding modulo 32, if
//the result is 0, it means that the actual number of bits required can be mapped to an integer ulong.
//If it is not zero, it means that there is no corresponding integer ulong and the value needs to be rounded down.
    ulTemp = ((ushort_t)ulIpCount * IPPOOL_BITMAP_LITTLE_STEP)
                    % SC_IP_POOL_BIT_COUNTS_IN_ULONG ;

    if(ulTemp != 0)
        ulTemp1 = ((ushort_t)ulIpCount * IPPOOL_BITMAP_LITTLE_STEP
                       + SC_IP_POOL_BIT_COUNTS_IN_ULONG - (ushort_t)ulTemp )
                             /SC_IP_POOL_BIT_COUNTS_IN_ULONG;
    else
        ulTemp1 = ((ushort_t)ulIpCount * IPPOOL_BITMAP_LITTLE_STEP )
                               /SC_IP_POOL_BIT_COUNTS_IN_ULONG;

//ulTemp1 is the number of ulongs required for ulIpCount IP addresses. 
//This needs to be converted to bytes when allocating //memory.
    ulTemp1 *= SC_IP_POOL_BITMAP_STEP;//SC_IP_POOL_BITMAP_STEP represents the number of bytes in a ulong
    if(SYS_ERR_OK != InitIpSectionBitmap(&(pIpSection->Bitmap), 1,
                  pIpSection->Bitmap.pbBitmapMem,
                  (ushort_t)ulTemp1,
                  SC_MIN_IPPOOL_ALLOCINDEX,
                  (ushort_t)(ulIpCount + SC_MIN_IPPOOL_ALLOCINDEX -1)))//If the initialization bitmap fails to restore 
                                                                       //the address pool to its initial state, a failure is returned.
    {
        ipPoolInitIpSection(pIpSection);
        return SYS_ERR_FAILED ;//Bitmap initialization failed
    }

    if(SYS_ERR_OK != InitIpSectionIpLink(&(pIpSection->stIpLink), (ushort_t)ulIpCount))//Initialize the free list
    {
        ipPoolInitIpSection(pIpSection);/*Need to release the memory allocated in the bitmap*/
            return SYS_ERR_FAILED;//If the function fails, the memory for the linked list must not have been allocated yet.
    }

    for(ulIp = ulStartIp; ulIp <= ulEndIp; ulIp++)//Check the validity of the ip address and modify the bitmap accordingly
    {//(ulIp - ulStartIp + SC_MIN_IPPOOL_ALLOCINDEX)The meaning of this calculation is to get the offset of the address 
        //in the bitmap, add the minimum sequence number, and get the correct bitmap index value.
        if(SYS_ERR_OK != ipPoolCheckIpAndMaskValid(ulIp ,ulMask))//The IP address is not suitable as a host address
        {
            ipPoolChangeIpSectionBitMap(&(pIpSection->Bitmap),
                                   (ushort_t)(ulIp - ulStartIp + SC_MIN_IPPOOL_ALLOCINDEX),
                                   IPPOOL_IPSTATUS_INVALID);
            pIpSection->ulInvalid++ ;
            ipPoolChangeSectionIpIdleLink(&pIpSection->stIpLink,(ushort_t)(ulIp - ulStartIp ), (uchar_t)FALSE,ulIpPoolIndex,ulIpSectionIndex);//Failure handling needs to be considered
            /*IpPool_Info_Printf("\r\nCreateIpSection->Ip %s is invalid",IPAddrToStr( ulIp, (uchar_t *)szTemp));*/
        }
    }
    pIpSection->ulUnUsed = ulIpCount - pIpSection->ulInvalid ;

    gulTotalIpNum += ulIpCount;
    pIpSection->usLength = (ushort_t)ulIpCount;
    gulIPSectionNum ++;
    DHCP_SERVER_DBG("add %d to gulTotalIpNum when creating in poolIndex:%d, Section %d",
    	(pIpSection->usLength), ulIpPoolIndex,ulIpSectionIndex);
    DHCP_SERVER_DBG("gulTotalIpNum = %d,unUsed = %d",gulTotalIpNum,   pIpSection->ulUnUsed);

	pIpSection->ulSectionRowStatus = ROW_ACTIVE;

    return SYS_ERR_OK ;

}


/************************************************************
  Function:      ulong_t ipPoolClearIpSection
  Description:   Initialize the address pool
  Calls:
  Called By:
  Table Accessed:
  Table Updated:
  Input:         ( AAA_IPSection_S *pool )Address pool pointer
  Output:
  Return:
  Others:
**************************************************************/
STATUS ipPoolClearIpSection( AAA_IPSection_S *pIpSect )
{

    if(NULL== pIpSect)
        return SYS_ERR_FAILED ;

    #if 0
    /*The number of addresses in use is not 0*/
    if(pIpSect->ulInUsed != 0)
    {
        /*IpPool_Info_Printf("\r\nscope is in used,can't be deleted");*/
        return ERROR;//The address pool is in use and cannot be deleted. The error message is "failed".
    }
    #endif

    gulTotalIpNum-= (pIpSect->usLength);
    gulIPSectionNum--;
    DHCP_SERVER_DBG("remove %d from gulTotalIpNum when deleting Section ", 	pIpSect->usLength);
    DHCP_SERVER_DBG("gulTotalIpNum = %d,gulIPSectionNum = %d",gulTotalIpNum,  gulIPSectionNum);

    pIpSect->ulStartIp = 0;
    pIpSect->ulEndIp   = 0;
    pIpSect->usLength  = 0;
    pIpSect->ulInUsed  = 0;
    pIpSect->ulConflict =0;
    pIpSect->ulInvalid  =0;
    pIpSect->ulDisable  =0;
    pIpSect->ulUnUsed   =0;
    pIpSect->ulSectionRowStatus = ROW_DESTROY;
    DeInitIpSectionBitmap(&(pIpSect->Bitmap));
    DeInitIpSectionIpLink(&(pIpSect->stIpLink));
    return SYS_ERR_OK;
}



/************************************************************
  Function:      ulong_t ipPoolDeleteIpSection
  Description:   Initialize the address pool
  Calls:
  Called By:
  Table Accessed:
  Table Updated:
  Input:         ( AAA_IPSection_S *pool )Address pool pointer
  Output:
  Return:
  Others:
**************************************************************/
STATUS ipPoolInitIpSection( AAA_IPSection_S *pIpSect )
{

    if(NULL== pIpSect)
        return SYS_ERR_FAILED ;

    /*The number of addresses in use is not 0*/
    if(pIpSect->ulInUsed != 0)
    {
        /*IpPool_Info_Printf("\r\nscope is in used,can't be deleted");*/
        return SYS_ERR_FAILED;//The address pool is in use and cannot be deleted. The error message is "failed".
    }

    pIpSect->ulStartIp = 0;
    pIpSect->ulEndIp   = 0;
    pIpSect->usLength  = 0;
    pIpSect->ulInUsed  = 0;
    pIpSect->ulConflict =0;
    pIpSect->ulInvalid  =0;
    pIpSect->ulDisable  =0;
    pIpSect->ulUnUsed   =0;
    pIpSect->ulSectionRowStatus = ROW_DESTROY;
    DeInitIpSectionBitmap(&(pIpSect->Bitmap));
    DeInitIpSectionIpLink(&(pIpSect->stIpLink));
    return SYS_ERR_OK;
}


/*******************************************************************************
Function: STATUS deleteDisableIp(ulong_t ulPoolIndex, ulong_t ulSectionIndex)
Description: Deletes the disabled address table entry in the MIB.
Input: ulong_t ulPoolIndex: address pool number; 
       ulong_t ulSectionIndex: address section number
Output:
Return: OK on success, ERROR on failure
Other: Provides command-line functionality for deleting address sections and pools.
*******************************************************************************/

STATUS deleteDisableIp(ulong_t ulPoolIndex, ulong_t ulSectionIndex)
{
    ushort usIndex;
    uchar_t ucOldState;
    //ulong_t ulRowStatus,ulSpecialIp,disbleIndex[4];
    STATUS rc = SYS_ERR_OK;
    AAA_IPSection_S * pIpSect;
    pIpSect = &gIpPool[ulPoolIndex].IpSection[ulSectionIndex];
    if( pIpSect->ulDisable==0 )
        return SYS_ERR_OK;
    for( usIndex =0;usIndex<pIpSect->usLength;usIndex++)
    {
        GetIpBitmapState(&pIpSect->Bitmap,
                         usIndex, &ucOldState);
        if(ucOldState != IPPOOL_IPSTATUS_DISABLE)
        {
            continue;
        }
        #if 0
        ulSpecialIp = pIpSect->ulStartIp+usIndex;
        ulRowStatus = 2;
        memset(disbleIndex,0,sizeof(disbleIndex));
        IptoCcdIndex( ulSpecialIp, disbleIndex);
        rc += ccdSet ( "gbnL3IPPool:ipDisableStatus", disbleIndex, 4, &ulRowStatus,sizeof(ulong_t));
        #endif
    }
    return rc;
}
int initIpdisable(void)
{
	ulong_t                 createIndex=0;
	for(createIndex=0;createIndex<IPPOOL_ALL_MAX_DISABLEIP_NUM;createIndex++)
	{
		memset(&gDisableIp[createIndex], 0, sizeof(IPPOOL_IPDisable_S));
	}
	return OK;
}


STATUS InitIpPool(uchar_t ucIndex)
{
    uchar_t i;
    if(ucIndex >= IPPOOL_MAX_POOL_NUM)
    {
        DHCP_SERVER_DBG("InitIpPool->pool index %d is out of range", ucIndex);
        return SYS_ERR_FAILED ;
    }

    memset(gIpPool[ucIndex].szName, 0, sizeof(gIpPool[ucIndex].szName));
    /*Clear the contents of each address segment*/
    for(i=0 ; i<IPPOOL_MAX_IPSection_NUM; i++)
    {
        gIpPool[ucIndex].IpSection[i].ulSectionRowStatus = ROW_DESTROY;
        ipPoolInitIpSection(&(gIpPool[ucIndex].IpSection[i]));
    }
    memset((char *)&gIpPool[ucIndex], 0, sizeof(AAA_IPPOOL_S));
    gIpPool[ucIndex].ulSubnetMask = IPPOOL_DEFAULT_SUBNET_MASK ;
    gIpPool[ucIndex].ulLeaseTime = IPPOOL_DEFAULT_LEASETIME;
    ipPool_RowStatus[ucIndex] = ROW_DESTROY;
    for(i = 0; i < IPPOOL_MAX_DHCPOPTIONS; i++)
    {
        gIpPool[ucIndex].stDHCPOPT[i].ulID = 0;
        gIpPool[ucIndex].stDHCPOPT[i].ulIP = 0;
    }
    return SYS_ERR_OK;
}


STATUS DeleteIpPool(uchar_t ucIndex)
{
    uchar_t i;
    if(ucIndex >= IPPOOL_MAX_POOL_NUM)
    {
        DHCP_SERVER_DBG("InitIpPool->pool index is out of range");
        return SYS_ERR_FAILED ;
    }

    memset(gIpPool[ucIndex].szName,0,sizeof(gIpPool[ucIndex].szName));
    /*Clear the contents of each address segment*/
    for(i=0 ; i<IPPOOL_MAX_IPSection_NUM; i++)
    {
        if(gIpPool[ucIndex].IpSection[i].ulSectionRowStatus == ROW_ACTIVE)
            ipPoolClearIpSection(&(gIpPool[ucIndex].IpSection[i]));
    }
    memset((char *)&gIpPool[ucIndex], 0, sizeof(AAA_IPPOOL_S));
    gIpPool[ucIndex].ulSubnetMask = IPPOOL_DEFAULT_SUBNET_MASK ;
    gIpPool[ucIndex].ulLeaseTime = IPPOOL_DEFAULT_LEASETIME;
    ipPool_RowStatus[ucIndex] = ROW_DESTROY;
    for(i = 0; i < IPPOOL_MAX_DHCPOPTIONS; i++)
    {
        gIpPool[ucIndex].stDHCPOPT[i].ulID = 0;
        gIpPool[ucIndex].stDHCPOPT[i].ulIP = 0;
    }
    return SYS_ERR_OK;
}


/**************************************************************
 Function: InitIpPoolAll
 Description: Initializes all address pool groups
 Input:
 Output:
 Return: SUCCESS: Success;
           Other: Failure
***************************************************************/
STATUS InitIpPoolAll(void)
{
    uchar_t i;
    int ucIndex;
    g_bIpPoolDebug = TRUE;
    for(i=0; i< IPPOOL_MAX_POOL_NUM; i++)
    {
        if(SYS_ERR_OK != InitIpPool((uchar_t)i))
        {
            return SYS_ERR_FAILED;
        }
    }

    gulTotalIpNum = 0;
    gulIPSectionNum = 0;
    for(ucIndex=0; ucIndex< IPPOOL_MAX_POOL_NUM; ucIndex++)
    {
        for(i = 0; i < IPPOOL_MAX_DHCPOPTIONS; i++)
        {
            gIpPool[ucIndex].stDHCPOPT[i].ulID = 0;
            gIpPool[ucIndex].stDHCPOPT[i].ulIP = 0;
        }
    }
    /*Banned Addresses*/
	initIpdisable();

    return SYS_ERR_OK;
}

STATUS ipPoolCreateByName(char * szPoolName)
{
    uchar_t i;
    char str[IPPOOL_MAX_POOL_NAME];

	osal_memset(&str,0, IPPOOL_MAX_POOL_NAME);

	for (i = 0; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		if (!strcmp(gIpPool[i].szName, szPoolName))
		{
			return SYS_ERR_OK;
		}
	}

	for (i = 0; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		if (!strcmp(gIpPool[i].szName, str))
		{
			InitIpPool(i);
			memcpy(gIpPool[i].szName, szPoolName, strlen(szPoolName));
			ipPool_RowStatus[i] = ROW_ACTIVE;
			return SYS_ERR_OK;
		}
	}

  return SYS_ERR_DHCP_IP_POOL_FULL;

}

STATUS ipPoolDeleteByName(char *szPoolName)
{
    uchar_t i;
	for (i = 0; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		if (!strcmp(gIpPool[i].szName, szPoolName))
		{
			DeleteIpPool(i);
			return SYS_ERR_OK;
		}
	}

    return SYS_ERR_OK;
}

int dhcpserverIPPoolCfg(char *szPoolName, BOOL add)
{
	int ret = SYS_ERR_OK;

	if (add)
	{
        ret = ipPoolCreateByName(szPoolName);
    }
	else
	{
        ret = ipPoolDeleteByName(szPoolName);
	}

    return ret;
}

int dhcpserverGatewayCfg(char *szPoolName, sys_ipv4_t addr, sys_ipv4_t mask)
{
    uchar_t i;
	for (i = 0; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		if (!strcmp(gIpPool[i].szName, szPoolName))
		{
			gIpPool[i].ulGatewayIp = addr;
			gIpPool[i].ulSubnetMask = mask;
			return SYS_ERR_OK;
		}
	}

    return SYS_ERR_FAILED;
}

int dhcpserverSectionCfg(char *szPoolName, uint32 id, sys_ipv4_t start_ip, sys_ipv4_t end_ip)
{
    uchar_t i;

	for (i = 0; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		if (!strcmp(gIpPool[i].szName, szPoolName))
		{
			if(start_ip && end_ip)
			{
				return ipPoolCreateIpSection(i, id, start_ip, end_ip, gIpPool[i].ulSubnetMask);
			}
			else 
			{
				return ipPoolClearIpSection(&gIpPool[i].IpSection[id]);
			}
		}
	}

    return SYS_ERR_FAILED;
}

int dhcpserverDnsCfg(char *szPoolName, uint32 dns_num, sys_ipv4_t addr)
{
    uchar_t i;

	if ((dns_num < 0) || (dns_num > IPPOOL_MAX_DNS_NUM))
		return SYS_ERR_FAILED;

	for (i = 0; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		if (!strcmp(gIpPool[i].szName, szPoolName))
		{
			gIpPool[i].ulDNSIp[dns_num - 1] = addr;
			return SYS_ERR_OK;
		}
	}

    return SYS_ERR_FAILED;
}

int dhcpserverDnsSuffixCfg(char *szPoolName, char *dnsSuffixName)
{
    uchar_t i;

	for (i = 0; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		if (!strcmp(gIpPool[i].szName, szPoolName))
		{
			memcpy(gIpPool[i].szDnsSuffixName, dnsSuffixName, strlen(dnsSuffixName));
			return SYS_ERR_OK;
		}
	}

    return SYS_ERR_FAILED;
}

/*******************************************************************************
Function: void ippool_debug_printf(char *exp,.....)
Description: Debug print function for the address pool module
Input: None
Output: None
Return: None
Other: None
*******************************************************************************/
void ippool_debug_printf( char *exp, ...)
{
    if(exp == NULL)
    {
       return ;
    }
    if ( g_bIpPoolDebug == TRUE )
    {
    	osal_printf("%s", exp);
    #if 0
        char     debugStr[256];
        va_list  vl;
        ULONG    ulDebugStrLen;

        memset(debugStr, 0, sizeof(debugStr));
        va_start ( vl, exp );
        ulDebugStrLen = vsprintf ( debugStr, exp, vl );
        va_end ( vl );

        /* If the assertion is false, vsprintf will either write out of bounds or fail. */
        vosAssert(ulDebugStrLen < sizeof(debugStr) && ulDebugStrLen > 0);
        printf("%s", debugStr);
    #endif
    }
}

/*******************************************************************************
Function: STATUS ippool_set_debug_enable( ulong_t ulState )
Description: Sets the address pool debug information on/off state
Input: ulong_t ulState - Address pool debug information on/off state
Output:
Return: OK if successful, ERROR if failed
*******************************************************************************/
STATUS ippool_set_debug_enable( ulong_t ulState )
{
    /*Determine the validity of parameters*/
    if ( (ulState != 1) && (ulState != 2) )
    {
        return SYS_ERR_FAILED;
    }
    
    if ( ulState == 1 )
    {
        g_bIpPoolDebug = TRUE;
    }
    else
    {
        g_bIpPoolDebug = FALSE;
    }
    return OK;
}

int ipPoolSectionUsedGet(ulong_t ulPoolIndex, ulong_t ulSectionIndex,
    ulong_t *pulInUsed, ulong_t *pulUnUsed)
{
    if ( (ulPoolIndex >= IPPOOL_MAX_POOL_NUM) || (ulSectionIndex >= IPPOOL_MAX_IPSection_NUM) )
    {
        return SYS_ERR_FAILED;
    }

    *pulInUsed = gIpPool[ulPoolIndex].IpSection[ulSectionIndex].ulInUsed;

    gIpPool[ulPoolIndex].IpSection[ulSectionIndex].ulUnUsed =
        *pulInUsed - gIpPool[ulPoolIndex].IpSection[ulSectionIndex].ulConflict
        - gIpPool[ulPoolIndex].IpSection[ulSectionIndex].ulDisable
        - gIpPool[ulPoolIndex].IpSection[ulSectionIndex].ulInvalid
        - gIpPool[ulPoolIndex].IpSection[ulSectionIndex].ulInUsed;

    *pulUnUsed = gIpPool[ulPoolIndex].IpSection[ulSectionIndex].ulUnUsed;
    return SYS_ERR_OK;
}

int ipPoolSectionBitmapGet(ulong_t ulPoolIndex, ulong_t ulSectionIndex,
    ushort_t *pwMinAllocNo, uchar_t bitmapBuf[IPPOOL_BITMAP_BUF_LEN])
{
    IpSectionBitMap_S       *pBitmap;

    if ( (ulPoolIndex >= IPPOOL_MAX_POOL_NUM) || (ulSectionIndex >= IPPOOL_MAX_IPSection_NUM) )
    {
        return SYS_ERR_FAILED;
    }

    pBitmap = &(gIpPool[ulPoolIndex].IpSection[ulSectionIndex].Bitmap);
    if ( pBitmap->wBitmapSize > IPPOOL_BITMAP_BUF_LEN )
    {
        return SYS_ERR_FAILED;
    }

    *pwMinAllocNo = pBitmap->wMinAllocNo;
    memcpy(bitmapBuf, pBitmap->pbBitmapMem, pBitmap->wBitmapSize);
    return SYS_ERR_OK;
}

int ipPoolGatewayCheck(ulong_t ulPoolIndex, ulong_t ulGatewayIp,
    ulong_t ulMask, int *piErrNo)
{
    *piErrNo = DHCPRS_E_OK;

    if ( ulPoolIndex >= IPPOOL_MAX_POOL_NUM )
    {
        return SYS_ERR_FAILED;
    }

    if ((ulGatewayIp != gIpPool[ulPoolIndex].ulGatewayIp)&&
          ( OK != ipPoolCheckIsIpPoolNull(ulPoolIndex)))
    {
        *piErrNo = DHCPRS_E_IP;
        return SYS_ERR_FAILED;
    }

    if((ulMask != gIpPool[ulPoolIndex].ulSubnetMask)&&
          ( OK != ipPoolCheckIsIpPoolNull(ulPoolIndex)))
    {
        *piErrNo = DHCPRS_E_MASK;
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

int ipPoolLeaseTimeSet(ulong_t ulPoolIndex, ulong_t ulTime)
{
    if ( ulPoolIndex >= IPPOOL_MAX_POOL_NUM )
    {
        return SYS_ERR_FAILED;
    }

    gIpPool[ulPoolIndex].ulLeaseTime = ulTime;
    return SYS_ERR_OK;
}


int ipPoolLeaseTimeCfg(char *szPoolName, ulong_t ulTime)
{
    uchar_t i;

	for (i = 0; i < IPPOOL_MAX_POOL_NUM; ++i)
	{
		if (!strcmp(gIpPool[i].szName, szPoolName))
		{
			return ipPoolLeaseTimeSet(i, ulTime);
		}
	}

    return SYS_ERR_OK;
}

int ipPoolIprangeCheck(ulong_t ulStartIp, ulong_t ulEndIp)
{
    unsigned int             i;

    for ( i=0; i < IPPOOL_MAX_POOL_NUM; i++ )
    {
        if((gIpPool[i].ulGatewayIp >= ulStartIp)&&
           (gIpPool[i].ulGatewayIp <= ulEndIp))
        {
            return SYS_ERR_FAILED;
        }
    }

    return SYS_ERR_OK;
}
STATUS getDisableIpIndex(ulong_t disable_ip, ulong_t *disableIndex)
{
    ulong_t idx;
    for(idx=0;idx<IPPOOL_ALL_MAX_DISABLEIP_NUM;idx++)
    {
        if (gDisableIp[idx].ulDisableIp== disable_ip)
        {
            *disableIndex = idx;
            return OK;
        }
    }
    return SYS_ERR_FAILED;
} 
int setDisableIp(ulong_t disable_ip, ulong_t enable)
{
	int                      rc;
	ulong_t                  disableIndex = 0;
	ulong_t                  createIndex=0, ulPoolIndex, ulSectIndex;
	ushort_t                 usIndex;
	
	rc = getDisableIpIndex(disable_ip, &disableIndex);
	if(rc != OK)/*The entry does not exist*/
	{
		if(enable)
		{
			for(createIndex=0;createIndex<IPPOOL_ALL_MAX_DISABLEIP_NUM;createIndex++)
			{ 
				if(gDisableIp[createIndex].ulDisableRowStatus == 0)
				{ 
					for(ulPoolIndex =0;ulPoolIndex<IPPOOL_MAX_POOL_NUM;ulPoolIndex++)
					{	 
						rc = FindIpSectionByIp(ulPoolIndex, disable_ip, &ulSectIndex, &usIndex);
						if(rc !=OK )
							rc = ERROR;
						else
						{
							gDisableIp[createIndex].ulPoolIndex= ulPoolIndex+1; 
							gDisableIp[createIndex].ulDisableRowStatus= 1;
							gDisableIp[createIndex].ulDisableIp = disable_ip; 
							rc = ipPoolChangeIpStatusInSection(ulPoolIndex,ulSectIndex,
								  usIndex ,IPPOOL_IPSTATUS_DISABLE,LEASE_DISABLE);
							if(rc != OK)
							{								  
								return rc;
							}
		
							dhcpserver_romovehash_by_ip(disable_ip);
		
							/*No longer search other address pools*/
							return rc;
						}
					}
		
					return rc;
				 }
			}
			/*No available resources found*/
			if ( createIndex >= IPPOOL_ALL_MAX_DISABLEIP_NUM )
			{
				rc = DHCP_ERROR_DISABLE_IP_NUM_FULL;
			}		
		}
	} 
	else /*Already exists*/
	{
		if(enable == 0)
		{
			for(ulPoolIndex =0;ulPoolIndex<IPPOOL_MAX_POOL_NUM;ulPoolIndex++)
			{	 
				rc = FindIpSectionByIp(ulPoolIndex, disable_ip, &ulSectIndex, &usIndex);
				/*Find the address pool and address segment where the address is located*/ 
				if(rc==OK)
				{
					rc = ipPoolChangeIpStatusInSection(ulPoolIndex,ulSectIndex,usIndex,
						IPPOOL_IPSTATUS_UNUSED,LEASE_DISABLE);
					if(rc != OK)
					{								  
						return rc;
					}
					else
						/*No need to continue searching, jump out of the loop*/
						break;
				}
			}
			/*Not found or already activated successfully*/
			/*if( ulPoolIndex > =IPPOOL_MAX_POOL_NUM)*/
			gDisableIp[disableIndex].ulDisableIp = 0;  
			gDisableIp[disableIndex].ulPoolIndex = 0;
			gDisableIp[disableIndex].ulDisableRowStatus = 0;
		}
	}
	return OK;
}

int ipPoolDisableIp(BOOL bDisable, char *szPoolName, ulong_t ulIp, int *piErrNo)
{
    int                      rc;
    ulong_t                  ulPoolIndex,ulSectIndex;
    ulong_t                  ulRowStatus,idx[4];
    char                    *objectName[] = {"gbnL3IPPool:ipDisableStatus"};
    ushort_t                 usIndex;
    uchar_t                  ucOldState;
    ulong_t                  disableIndex = 0;
    ulong_t                  createIndex=0;

    *piErrNo = DHCPRS_E_OK;

    if ( OK != ipPoolGetPoolIndexByName(szPoolName, &ulPoolIndex))
    {
        *piErrNo = DHCPRS_E_POOL;
        return ERROR;
    }

    if ( OK != FindIpSectionByIp(ulPoolIndex,ulIp,&ulSectIndex, &usIndex))
    {
        *piErrNo = DHCPRS_E_IP;
        return ERROR;
    }

    GetIpBitmapState(&gIpPool[ulPoolIndex].IpSection[ulSectIndex].Bitmap,
                     usIndex, &ucOldState);

    if ( TRUE == bDisable )
    {
        if( ucOldState == IPPOOL_IPSTATUS_INVALID )
        {
            *piErrNo = DHCPRS_E_S_INVALID;
            return ERROR;
        }

        if ( ucOldState == IPPOOL_IPSTATUS_DISABLE )
        {
            *piErrNo = DHCPRS_E_S_DISABLED;
            return OK;
        }

        if( gIpPool[ulPoolIndex].IpSection[ulSectIndex].ulDisable>=IPPOOL_SECTION_MAX_DISABLEIP_NUM)
        {
            *piErrNo = DHCPRS_E_MAX;
            return ERROR;
        }
		return setDisableIp(ulIp, bDisable);
 	 
    }
    else
    {
        if((ucOldState != IPPOOL_IPSTATUS_DISABLE)&&(ucOldState != IPPOOL_IPSTATUS_CONFLICT))
        {
            *piErrNo = DHCPRS_E_S_DISABLED;
            return ERROR;
        }

        if(ucOldState == IPPOOL_IPSTATUS_CONFLICT)
        {
            //enable
			rc = setDisableIp(ulIp, TRUE);

            //disable
			rc = setDisableIp(ulIp, FALSE);
            return rc;
        } 
        return setDisableIp(ulIp, FALSE);
    }

    return OK;
}

int ipPoolDisableIpGet(char *szPoolName, ulong_t ulIp[GET_NUM_ONE])
{
    int                      rc;
    ulong_t                  ulPoolIndex,ulSectIndex;
    ushort_t                 usIndex = 0;
    uchar_t                  ucOldState;
    ulong_t                  disableIndex = 0;
    ulong_t                  createIndex=0;
	static ulong_t           indexflag = 0;
    

    if ( OK != ipPoolGetPoolIndexByName(szPoolName, &ulPoolIndex))
    {
        return ERROR;
    }

	for( createIndex = indexflag; createIndex<IPPOOL_ALL_MAX_DISABLEIP_NUM; createIndex++)
	{
		if(gDisableIp[createIndex].ulDisableRowStatus && gDisableIp[createIndex].ulPoolIndex == (ulPoolIndex + 1))
		{
			ulIp[usIndex] = gDisableIp[createIndex].ulDisableIp;
			usIndex ++;
			if(usIndex == GET_NUM_ONE)
			{
				if((createIndex + 1) < IPPOOL_ALL_MAX_DISABLEIP_NUM)
					indexflag = createIndex + 1;
				else
					indexflag = 0;
				return OK;
			}
			else 
			{
				indexflag = 0;
			}
		}
	}

    return OK;
}
/*Check whether the IP address is in the address pool*/
int static_mac_ip_bind_check_pool(ulong_t ulIp, ulong_t* used)
{
    int                      rc;
    ulong_t                  ulPoolIndex,ulSectIndex;
    ushort_t                 usIndex;
    uchar_t                  ucOldState;
    ulong_t                  disableIndex = 0;
    ulong_t                  createIndex=0;
	uchar_t                  szPoolName[IPPOOL_MAX_POOL_NAME];

 	memset(szPoolName, 0, sizeof(szPoolName));

    for (ulPoolIndex = 0; ulPoolIndex < IPPOOL_MAX_POOL_NUM; ++ulPoolIndex)
    {  
		if (memcmp(gIpPool[ulPoolIndex].szName, szPoolName, IPPOOL_MAX_POOL_NAME))
		{
			if ( OK != FindIpSectionByIp(ulPoolIndex, ulIp, &ulSectIndex, &usIndex))
		    {
		        continue;
		    }
		    *used = TRUE;
			return SYS_ERR_OK;
		}
    }
    
	*used = FALSE;

    return SYS_ERR_OK;
}

#endif /*GBN_INCLUDE_IPPOOL*/
