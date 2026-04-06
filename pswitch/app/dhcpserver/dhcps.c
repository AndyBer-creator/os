/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  File Name:  Dhcps.c
  Autor: Kniazkov Andrew
  Version:1.0.0
  Description: This file defines all function of the DHCP server module,
               completing the DHCP server functionaly
********************************************************************************/
/****************************HEADER FILLES**********************************/
#include <common/sys_def.h>
#include <pthread.h>
#define GBN_INCLUDE_DHCPRELAYSERVER
#define GBN_INCLUDE_IPPOOL
#ifdef GBN_INCLUDE_DHCPRELAYSERVER
#ifdef  GBN_INCLUDE_IPPOOL
#include "ip_pool.h"
#include "dhcps.h"
#include "dhcpr.h"
#include "hashCommon.h"
#include "dhcpHash.h"
#include "dhcpSocket.h"
#include "dhcp.h"

/*********************GLOBAL VARIABLE DECLARATION********************************/
HASH_S g_stDhcpHashCB;    /*DHCP module global HASH table control block*/
ULONG gulIpBindSwitch = FALSE;
BOOL  unboundAssign  = TRUE;
BOOL  g_bDhcpServerSendTrap = FALSE;
uint32 g_dhcpserverEnable = FALSE;
extern int dhcpMaxHops;

/*External variables*/
dhcpOption60Match_t dhcpOption60MatchEntry[DHCP_MAX_INTERFACE_NUM][DHCPINTERFACEMAXNUM];

uchar_t  *dhcp_ipaddr_to_str(ulong_t ulIPAddr, uchar_t * string)
{
    sprintf((char *)string, "%u.%u.%u.%u",
        ulIPAddr >> 24, (ulIPAddr >> 16) & 0xff,
        (ulIPAddr >> 8) & 0xff, ulIPAddr & 0xff);
    return string;
}

/*******************************************************************************
  Function: STATUS dhcp_user_pkt_check
  Description: Check the user's DHCP message
  Input: DHCP_HEAD_S *pDhcp - message header pointer
  Output: uchar_t *pstMac  - variable for recording the user's MAC address
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS  dhcp_user_pkt_check(DHCP_HEAD_S *pDhcp, uchar_t *pstMac)
{
    int   i;
    uchar_t ucCnt;

   if((pDhcp == NULL) || (pstMac == NULL))
   {
        DHCP_SERVER_RX_DBG("dhcp_user_pkt_check: Invalid parameter");
        return ERROR;
   }

    /*DHCP message inspection*/
    /*First determine whether the current message is a DHCPREQUEST or DHCPREPLY*/
    if (pDhcp->ucop != DHCP_REQUIRE && pDhcp->ucop != DHCP_REPLY)
    {
        DHCP_SERVER_RX_DBG("dhcp_user_pkt_check: The packet is not a REQUEST or REPLY");
        /*No,quit*/
        return ERROR;
    }

    /*Determine the number of transfer hops, if too many, give up and do not forward*/
    if (pDhcp->uchops > dhcpMaxHops)
    {
        DHCP_SERVER_RX_DBG("dhcp_user_pkt_check: too many hops, packetHops(%d) > maxHops(%d)", pDhcp->uchops, dhcpMaxHops);
        return ERROR;
    }

    /*Determine whether it is ETHER-2 type hardware type and hardware address length*/
    if ((pDhcp->uchtype !=ETHERNET_HARDWARETYPE)
         || (pDhcp->uchlen != ETHERNET_HARDWARELEN))
    {
        DHCP_SERVER_RX_DBG("dhcp_user_pkt_check: unsupported hardwaretype or hardware address length");
        return ERROR;
    }

    ucCnt = 0;
    /*Record user's MAC adresses*/
    for (i = 0; i < ETHERNET_HARDWARELEN; i++)
    {
        pstMac[i] = pDhcp->szchaddr[i];
        if(pDhcp->szchaddr[i] == 0xff)
            ucCnt++;
    }

    /*Check whether the users MAC address is a broadcast address*/
    if (ucCnt == ETHERNET_HARDWARELEN)
    {
        DHCP_SERVER_RX_DBG("dhcp_user_pkt_check: source MAC is a broadcast MAC");
        return ERROR;
    }

    /*Check whether the users MAC address is a multicast address*/
    if ((pDhcp->szchaddr[0] & 0x01) == 0x01)
    {
        DHCP_SERVER_RX_DBG("dhcp_user_pkt_check: source MAC is a multicast MAC");
        return ERROR;
    }

    /*Check the SNAME field*/
    for (i = DHCP_SNAME_LENGTH - 1; i >= 0; i-- )
    {
        if ((pDhcp->szsname)[i] == '\0')
        {
           break;
        }
    }
    if (i < 0)
    {
        DHCP_SERVER_RX_DBG("dhcp_user_pkt_check: The 'Sname' is not null terminated");
        return ERROR;
    }
    else
    {
        return OK;
    }

}

/*******************************************************************************
  Function: STATUS dhcp_show_packet
  Description: Prints relevant information from the received DHCO message.
               Used for debbuging
  Input: DHCP_HEAD_S *pDhcp - message header pointer
         ulong_t ulDhcpLen - message length
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS  dhcp_show_packet ( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, char *pSyslogHead )
{
    int    i;
    char szOut[300];
    //char szTotalStr[500];
    //DHCP_OPTION_S stDhcpOption;
    //ulong_t ultempIP = 0;
    return  OK;

    if(pDhcp == NULL)
    {
        DHCP_SERVER_RX_DBG("ShowPacket: Invalid parameter");
        return ERROR;
 	}
    //szTotalStr[0]='\0';
    DHCP_SERVER_PKT_DBG("packet contents is");

    DHCP_SERVER_PKT_DBG("\txid:%u", osal_ntohl(pDhcp->ulxid));
    //strcat(szTotalStr,szOut);
    DHCP_SERVER_PKT_DBG("Secs:%d", osal_ntohs(pDhcp->ussecs));
    //strcat(szTotalStr,szOut);
    DHCP_SERVER_PKT_DBG("\top_code:%d", pDhcp->ucop);
    //strcat(szTotalStr,szOut);
    DHCP_SERVER_PKT_DBG("HardType:%d", pDhcp->uchtype);
    //strcat(szTotalStr,szOut);
    DHCP_SERVER_PKT_DBG("HardLen:%d", pDhcp->uchlen);
    //strcat(szTotalStr,szOut);
    DHCP_SERVER_PKT_DBG("\tchaddr:");
    for(i = 0; i < pDhcp->uchlen; i ++)
    {
        DHCP_SERVER_PKT_DBG("%02X", pDhcp->szchaddr[i]);
        //strcat(szTotalStr, szOut);
        if(i != pDhcp->uchlen - 1)
        {
            DHCP_SERVER_PKT_DBG("-");
        }
    }

    DHCP_SERVER_PKT_DBG("Hops:%d", pDhcp->uchops);
    //strcat(szTotalStr,szOut);
    DHCP_SERVER_PKT_DBG("Flag:%u", osal_ntohs(pDhcp->usflag)&0x8000);
    //strcat(szTotalStr,szOut);

    dhcp_ipaddr_to_str(osal_ntohl(pDhcp->ulciaddr), (uchar_t*)szOut);
    DHCP_SERVER_PKT_DBG("Ciaddr:");
    DHCP_SERVER_PKT_DBG("%s",szOut);
    dhcp_ipaddr_to_str(osal_ntohl(pDhcp->ulyiaddr), (uchar_t*)szOut);
    DHCP_SERVER_PKT_DBG("Yiaddr:");
    DHCP_SERVER_PKT_DBG("%s",szOut);
    dhcp_ipaddr_to_str(osal_ntohl(pDhcp->ulsiaddr), (uchar_t*)szOut);
    DHCP_SERVER_PKT_DBG("Siaddr:");
    DHCP_SERVER_PKT_DBG("%s",szOut);
    dhcp_ipaddr_to_str(osal_ntohl(pDhcp->ulgiaddr), (uchar_t*)szOut);
    DHCP_SERVER_PKT_DBG("Giaddr:");
    DHCP_SERVER_PKT_DBG("%s",szOut);

    for (i = DHCP_SNAME_LENGTH - 1; i >= 0; i-- ) /*string length check*/
    {
        if (pDhcp->szsname[i] == '\0')
        {
            break;
        }
    }
    if(i < 0)
    {
        DHCP_SERVER_PKT_DBG("ShowPacket: 'Sname' field is not null terminated");
        return ERROR;
    }
    //syslogSendInfo(MID_DHCP, DEBUGGING, pSyslogHead, szTotalStr, szTotalStr, FALSE, FALSE, NO_WAIT);

    DHCP_SERVER_PKT_DBG("Sname:%s", pDhcp->szsname);
    DHCP_SERVER_PKT_DBG("Options:");
#if 0
    /*Traverse the "options" field of the DHCP message and bring back the location pointer of the relevant "option"*/
    memset ((char *)&stDhcpOption, 0, sizeof( DHCP_OPTION_S ));
    if ( ERROR == dhcp_record_all_opt(pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption) )
    {
        return ERROR;
    }

    osal_printf("\r\n \tMSG TYPE:");
    if (stDhcpOption.pucDHCPMsgType != NULL)
    {
        osal_printf("%d",*(stDhcpOption.pucDHCPMsgType));
    }
    else
    {
        szOut[0] = '\0';
    }

    //strcat(szTotalStr,szOut);

    osal_printf("\tSERVER IP:");
    if (stDhcpOption.pulDHCPServerIp != NULL)
    {
        memcpy(&ultempIP, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
        dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
    }
    else
    {
        szOut[0] = '\0';
    }
    osal_printf("%s",szOut);

    osal_printf("\tREQUEST IP:");
    if (stDhcpOption.pulReqIp != NULL)
    {
        memcpy(&ultempIP, stDhcpOption.pulReqIp, sizeof(ulong_t));
        dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
    }
    else
    {
        szOut[0] = '\0';
    }
    osal_printf("%s",szOut);

    osal_printf("\r\n \tREQUEST PARAM LIST:");
    if (stDhcpOption.ucReqParamListLen != 0)
    {
        for (i = 0; i < (stDhcpOption.ucReqParamListLen); i++)
        {
            osal_sprintf(szOut,"0x%02X ",*((stDhcpOption.pucReqParamList)++));
            osal_printf("%s",szOut);
        }
    }
    else
    {
        szOut[0] = '\0';
        osal_printf("%s",szOut);
    }
    //syslogSendInfo(MID_DHCP, DEBUGGING, pSyslogHead, szTotalStr, szTotalStr, FALSE, FALSE, NO_WAIT);

#define _SHOW_MORE_DHCP_PKT_INFO
#ifdef _SHOW_MORE_DHCP_PKT_INFO
    osal_printf("\r\n \tROUTER IP:");
    if (stDhcpOption.ucRouterIpLen != 0)
    {
        for (i = 0; i < (stDhcpOption.ucRouterIpLen >> 2); i++)
        {
            memcpy(&ultempIP, stDhcpOption.pulRouterIp + i, sizeof(ulong_t));
            dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
            osal_printf("%s",szOut);
            osal_printf("\t");
        }
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

    osal_printf("\r\n \tDNS IP:");
    if (stDhcpOption.ucDNSIpLen != 0)
    {
        for (i = 0; i < (stDhcpOption.ucDNSIpLen >> 2); i++)
        {
            memcpy(&ultempIP, stDhcpOption.pulDNSIp + i, sizeof(ulong_t));
            dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
            osal_printf("%s",szOut);
            osal_printf("\t");
        }
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

    osal_printf("\r\n \tNetbios Name Server IP:");
    if (stDhcpOption.pulNBNSIp != 0)
    {
        for (i = 0; i < (stDhcpOption.ucNBNSIpLen >> 2); i++)
        {
            memcpy(&ultempIP, stDhcpOption.pulNBNSIp + i, sizeof(ulong_t));
            dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
            osal_printf("%s",szOut);
            osal_printf("\t");
        }
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

    osal_printf("\r\n \tHOST NAME:");
    if (stDhcpOption.ucHostNameLen != 0)
    {
        memcpy(szOut, stDhcpOption.pucHostName, stDhcpOption.ucHostNameLen);
        szOut[stDhcpOption.ucHostNameLen] = '\0';
        osal_printf("%s",szOut);
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

    osal_printf(szTotalStr,"\r\n \tDOMAIN NAME:");
    if (stDhcpOption.ucDomainNameLen != 0)
    {
        memcpy(szOut, stDhcpOption.pucDomainName, stDhcpOption.ucDomainNameLen);
        szOut[stDhcpOption.ucDomainNameLen] = '\0';
        osal_printf("%s",szOut);
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

#endif

    osal_printf(szTotalStr,"\r\n \tSUBNETMASK:");
    if (stDhcpOption.pulSubnetMask != NULL)
    {
        memcpy(&ultempIP, stDhcpOption.pulSubnetMask, sizeof(ulong_t));
        dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
    }
    else
    {
        szOut[0] = '\0';
    }
    osal_printf("%s",szOut);
#endif

    //syslogSendInfo(MID_DHCP, DEBUGGING, pSyslogHead, szTotalStr, szTotalStr, FALSE, FALSE, NO_WAIT);

    return  OK;
}

/*Function defifition*/
void macToStr(uchar_t * mac, uchar_t * buf)
{
    osal_sprintf((char *)buf, "%02x:%02x:%02x:%02x:%02x:%02x",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/*Determine if the IP is an interface IP and store all IPs of all interfaces into the array ifIpAddr*/
BOOL dhcpserver_ip_is_ifip(ulong_t gateway, ulong_t sw, ulong_t ifIpAddr[DEFAULT_MAX_IP_SUBNET])
{
    ulong_t  ulIfIp;
    BOOL    isifip = FALSE;
    int i = 1; /*Other secondary IPs are stored starting from 1, and the primary IP is stored at position 0*/
    ulong_t ifPrimIp, ulNum, ulIndex;
    ulong_t ulIpAddr[DEFAULT_MAX_IP_SUBNET];
    //ulong_t ulIpMask[DEFAULT_MAX_IP_SUBNET];

    /*Get the primary IP address corresponding to sw*/
    ifPrimIp = 0;
    //l3ifSwPrimIpGet(sw, &ifPrimIp);

    /*Get all IP adrresses*/
    ulNum = 0;
    //l3ifSwAllIpMaskGet(sw, &ulNum, ulIpAddr, ulIpMask);
    for ( ulIndex = 0; ulIndex < ulNum; ulIndex++ )
    {
        ulIfIp = ulIpAddr[ulIndex];

        if(ulIfIp == gateway)
        {
            isifip = TRUE;
            if(gateway != ifPrimIp)
                ifIpAddr[0]=ifPrimIp; /*position 0 stored the main IP*/
        }
        else
        {
            if(ulIfIp == ifPrimIp)
                ifIpAddr[0]=ulIfIp;
            else
            {
                ifIpAddr[i]=ulIfIp;
                i++;
            }
        }
    }

    return isifip;
}

uint32 dhcp_get_ifmask_by_ipaddr ( ulong_t ulIfIP, ulong_t *pIfMask )
{
    unsigned int             i;
    ulong_t                  ulNum;
    ulong_t                  ulIpAddr[DEFAULT_MAX_IP_SUBNET];
    ulong_t                  ulIpMask[DEFAULT_MAX_IP_SUBNET];

    /*Find the interface mask through the interface IP*/
    //ulSw = 0;
    while ( 1 )
    {
       // if ( SYS_ERR_OK != l3ifSwGetNextValid(&ulSw) )
        {
            break;
        }

        ulNum = 0;
       // if ( SYS_ERR_OK != l3ifSwAllIpMaskGet(ulSw, &ulNum, ulIpAddr, ulIpMask) )
        {
            break;
        }

        for ( i = 0; i < ulNum; i++ )
        {
            if ( ulIfIP == ulIpAddr[i] )
            {
                *pIfMask = ulIpMask[i];
                return SYS_ERR_OK;
            }
        }
    }

    return SYS_ERR_FAILED;
}


MACHASH_S * dhcpserver_HashInsert(HASH_S *pHashCB, MACHASH_S *pHashData)
{
    MACHASH_S *pHashRet = NULL;

    pHashRet = HashInsert(pHashCB, pHashData);

    return pHashRet;
}

STATUS  dhcpserver_HashDelete(HASH_S *pHashCB, MACHASH_S *pHashData)
{
    STATUS iRet;

    iRet =  HashDelete(pHashCB, pHashData);

    return iRet;
}

int dhcpserverHashDataGet(ushort_t usStart, ushort_t *pusNextStart,
    ushort_t *pusBufNum, MACHASH_S hostBuf[DHCPRS_HOST_BUF_MAX])
{
    unsigned int             i = 0, bufIndex = 0;
    ulong_t                  ulHashNum;
    MACHASH_S               *pHashData;

    *pusBufNum = 0;
    *pusNextStart = 0;

    ulHashNum = (g_stDhcpHashCB.ulListNum+g_stDhcpHashCB.ulFreeNum);

    for ( i = usStart; i < ulHashNum; i++)
    {
        pHashData = (MACHASH_S *)g_stDhcpHashCB.pList[i].pData;

        if ( (pHashData != NULL) && (g_stDhcpHashCB.pList[i].ulFlag == HASH_NODE_IN_USED ))
        {
            memcpy(&hostBuf[bufIndex++], pHashData, sizeof(MACHASH_S));
            if ( bufIndex >= DHCPRS_HOST_BUF_MAX )
            {
                break;
            }
        }
    }

    *pusBufNum = bufIndex;
    if ( i < ulHashNum )
    {
        *pusNextStart = i + 1;
    }

    return SYS_ERR_OK;
}

int dhcpseverHashDataGetByKey(MACHASH_S *pDataBuf)
{
    MACHASH_S               *pHashData;

    pHashData = HashSearch(&g_stDhcpHashCB, (void *)pDataBuf);
    if ( NULL == pHashData )
    {
        return ERROR;
    }

    memcpy(pDataBuf, pHashData, sizeof(MACHASH_S));
    return OK;
}

int dhcpseverTrapsCfg(BOOL bTrapSend)
{
    g_bDhcpServerSendTrap = bTrapSend;
    return OK;
}

int dhcpseverHashNumGet(ulong_t *pulNum)
{
    unsigned int             i, uiNum = 0;
    ulong_t                  ulHashNum;
    MACHASH_S               *pHashData;

    ulHashNum = (g_stDhcpHashCB.ulListNum+g_stDhcpHashCB.ulFreeNum);
    for ( i = 0; i < ulHashNum; i++)
    {
        pHashData = (MACHASH_S *)g_stDhcpHashCB.pList[i].pData;

        if ( (pHashData != NULL) && (g_stDhcpHashCB.pList[i].ulFlag == HASH_NODE_IN_USED ))
        {
            uiNum++;
        }
    }

    *pulNum = uiNum;
    return OK;
}


/*******************************************************************************
  Function: void dhcpserver_debug_printf(char *exp,.....)
  Description: Debug print function for the DHCP SERVER module
  Input: None
  Output: None
  Return: None
*******************************************************************************/
void dhcpserver_debug_printf( char *exp, ...)
{

    if(exp == NULL)
    {
       return ;
    }
    osal_printf("%s", exp);
#if 0
    if ( syslogModuleSwitch(MID_DHCP, DEBUGGING) )
    {
        char     debugStr[256];
        va_list  vl;
        ULONG    ulDebugStrLen;

        memset(debugStr, 0, sizeof(debugStr));

        va_start ( vl, exp );
        ulDebugStrLen = vsprintf ( debugStr, exp, vl );
        va_end ( vl );

        /*If the assertion is false, vsprintf will either write out of bounds or fail*/
        vosAssert(ulDebugStrLen < sizeof(debugStr) && ulDebugStrLen > 0);
        syslogSendInfo(MID_DHCP, DEBUGGING, "dhcpserver", debugStr, debugStr, FALSE, FALSE, NO_WAIT);
    }
#endif
}

/*******************************************************************************
  Function: void dhcpserver_init(void)
  Description: Initializes all global variables of the DHCP Server module
               Creates HASH table
               Creates the DHCP Server module's message queue
               Creates and starts a timer task
  Input: None
  Output: None
  Return: None
*******************************************************************************/
int dhcpserver_init(void)
{
    STATUS rc = SYS_ERR_OK;

    /*Initialize the HASH table*/
    rc = HashInit ( &g_stDhcpHashCB, HASH_FACTOR, HASH_COLLISIONLEN,
           hash_malloc_data, hash_free_data, hash_key, hash_compare_key );
    if ( rc != SYS_ERR_OK )
    {
        DHCP_SERVER_DBG ( "DHCP: Hash table initialization failed" );
        return SYS_ERR_FAILED;
    }

    /*Start the IP POOL address pool module*/
    rc = ipPoolStartup( );
    if ( rc != SYS_ERR_OK )
    {
        DHCP_SERVER_DBG( "DHCP SERVER start ip pool failed");
        return SYS_ERR_FAILED;
    }
    return SYS_ERR_OK;
}

STATUS dhcpserverRxPkt(int iType, DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen,
    ulong_t ulVid, ulong_t ulServerIp ,DHCP_INTERFACE_PARA *if_Para)
{
    /*Print message*/
  //  if ( syslogModuleSwitch(MID_DHCP, DEBUGGING) )
    {
        dhcp_show_packet( pDhcp, ulDhcpLen, "dhcpserver" );
    }

    switch ( iType )
    {
        case DHCPSERVER_RX_DISCOVER:
            /*Processing DISCOVER messages*/
            dhcpserver_process_discover(pDhcp, ulDhcpLen, ulVid, ulServerIp, if_Para);
            break;

        case DHCPSERVER_RX_REQUEST:
            /*Processing REQUEST messages*/
            dhcpserver_process_request(pDhcp, ulDhcpLen, ulVid, ulServerIp, if_Para);
            break;

        case DHCPSERVER_RX_DECLINE:
            /*Processing DECLINE messages*/
            dhcpserver_process_decline(pDhcp, ulDhcpLen, ulVid, ulServerIp, if_Para);
            break;

        case DHCPSERVER_RX_RELAESE:
            /*Processing RELEASE messages*/
            dhcpserver_process_release(pDhcp, ulDhcpLen, ulVid, ulServerIp , if_Para);
            break;

        default:
            DHCP_SERVER_RX_DBG( "DHCPSERVER: Receive unkown message " );
            break;

    }

    return OK;
}

/*******************************************************************************
  Function: dhcpserver_ip_is_usedByOthers
  Description: Determine if the current IP is used by another user
  Input: ulong_t ulUserIp - user IP
         uchar_t ucUserMac[6] - user MAC
  Output: None
  Return: Returns TRUE if the IP already used by another user, otherwise returns FALSE
*******************************************************************************/
BOOL dhcpserver_ip_is_usedByOthers(ulong_t ulUserIp, uchar_t ucUserMac[6])
{
    MACHASH_S *pHashData;
    ulong_t i;
    uchar_t ucZeroMac[6] = {0, 0, 0, 0, 0, 0};

    if ((0 == ulUserIp) || (0 == memcmp(ucUserMac, ucZeroMac, 6)))
    {
        return FALSE;
    }

    /* Search the HASH table in order */
    for (i = 0; i < (g_stDhcpHashCB.ulListNum + g_stDhcpHashCB.ulFreeNum); i++)
    {
        if ((g_stDhcpHashCB.pList[i].pData != NULL) && (g_stDhcpHashCB.pList[i].ulFlag == HASH_NODE_IN_USED))
        {
            pHashData = (MACHASH_S *)g_stDhcpHashCB.pList[i].pData;
            if ((pHashData->ulIpAddr == ulUserIp) && (0 != memcmp(pHashData->ucUserMac, ucUserMac, 6)))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*******************************************************************************
  Function: STATUS dhcpserver_process_discover
  Description:  Processes the DHCPDISCOVER message sent from the DHCP RELAY
  Input: DHCP_HEAD_S *pDhcp - message header pointer
         ulong_t ulDhcpLen - message length 
         ulong_t ulVid - message VID
         ulong_t ulServerIp - dhcp server address in the message
		DHCP_INTERFACE_PARA *if_Para - interface information
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/

STATUS dhcpserver_process_discover( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulServerIp ,DHCP_INTERFACE_PARA *if_Para)
{
    MACHASH_S stHashData;
    MACHASH_S *pHashData = NULL;
    uchar_t ucUserMac[6];
    DHCP_OPTION_S stDhcpOption;
    STATUS rc = OK;
    ulong_t ulGetIp = 0;
    ulong_t ulUserIp = 0;
    ulong_t ifIpAddr[DEFAULT_MAX_IP_SUBNET];
    int i = 0;
    uchar_t buf[64];
    BOOL cantBeBound = FALSE;
    ulong_t vid = 0;
    ulong_t ultempIP = 0;

    memset(ifIpAddr,0,sizeof(ifIpAddr));
    DHCP_SERVER_DBG( "DHCP SERVER receive a DHCP DISCOVER packet " );
    memset ( (char *)&stHashData, 0, sizeof(stHashData) );
    /*General message inspection function*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }

    /*Record message option field*/
    dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption );

    /*Since it is a built-in dhcp server, the giaddr field must not be equal to 0*/
    if ( osal_ntohl(pDhcp->ulgiaddr) == 0 )
    {
        DHCP_SERVER_DBG ( "DHCPSERVER: the giaddr value cannot be null " );
        return ERROR;
    }

    /*Check if the customer exist in the HASH table*/
    memcpy ( (char * )&(stHashData.ucUserMac[0]), ucUserMac, 6 );
    pHashData = HashSearch( &g_stDhcpHashCB, (void *)&stHashData );

    if ( pHashData != NULL )
    {
        /*If the gateway address in the message points to an address pool that does not contain
        the currently assigned IP address, this address needs to be released because the user
        may have moved the network range*/
        if ( ((pHashData->ulIpAddr != 0)&&(IPPOOL_CheckSpecialIp(osal_ntohl(pDhcp->ulgiaddr), pHashData->ulIpAddr) != OK))
              || (gulIpBindSwitch == TRUE))/*after binding an IP address, you must reapply 
                                            each time because the binding value may change
                                            or it may not have been before but is now bound*/
        {
            IPPOOL_GetbackIpWithoutGateway( pHashData->ulIpAddr );
            pHashData->ulIpAddr = 0;
        }
    }

    if ( (pHashData == NULL)||((pHashData != NULL)&&(pHashData->ulIpAddr == 0)) )
    {
        if(gulIpBindSwitch == TRUE)
        {
            if(dhcps_fund_ipbind_by_mac(ucUserMac, &ulUserIp,if_Para->dhcp_vid) != SYS_ERR_OK)
            {
                if(unboundAssign)
                {
                    cantBeBound = TRUE;
                    vid = ulVid;
                    goto assign_unbound;
                }
                else
                {
                    //No bound IP found, allocation not allowed
                    macToStr(ucUserMac, buf);
                    DHCP_SERVER_DBG ( "dhcp-client bind is enabled, but no ip address is bound to mac %s", buf);
                    goto get_ip_error;
                }
            }
            else  //Found the bound IP
            {
                /*Apply for a designated IP address*/
                rc = IPPOOL_GetSpecialIp ( ntohl(pDhcp->ulgiaddr), ulUserIp, cantBeBound,vid);
                if ( (rc != IP_SUCCESS) && (rc != IP_IN_USED) )
                {
                    //dhcp_ipaddr_to_str(ulUserIp, buf);
                    //dhcpserver_debug_printf ( "\r\n Get bound ip %s from ip pool failure", buf);
                    //return ERROR;
                    if ( dhcpserver_ip_is_ifip(ntohl(pDhcp->ulgiaddr), if_Para->dhcp_sw , ifIpAddr))
                    {
                        for(i=0; i<DEFAULT_MAX_IP_SUBNET ; i++)
                        {
                            if(ifIpAddr[i])
                            {
                                int j = 0;
                                for(j=0; j < DHCPINTERFACEMAXNUM; j++)
                                {
                                    if(dhcpOption60MatchEntry[if_Para->dhcp_sw-1][j].gateway == ifIpAddr[i])
                                    {
                                        goto continue_outside_for1;/* This gateway has been assigned with another
                                                                    specific option60 address*/
                                    }
                                }
                                //dhcp_ipaddr_to_str(ifIpAddr[i], buf);
                                //dhcpserver_debug_printf ( "\r\n Get bound ip from ip pool(ip %s as gateway)", buf);

                                rc = IPPOOL_GetSpecialIp ( ifIpAddr[i], ulUserIp, cantBeBound, vid);
                                if ( (rc != IP_SUCCESS) && (rc != IP_IN_USED) )
                                {
                                    DHCP_SERVER_DBG ( "Get bound ip from ip pool(ip %s as gateway) failure ", buf );
                                    continue;
                                }
                                else
                                {
                                    if ((rc == IP_IN_USED) && dhcpserver_ip_is_usedByOthers(ulUserIp, ucUserMac))
                                    {
                                        //dhcp_ipaddr_to_str(ulUserIp, buf);
                                        //dhcpserver_debug_printf("\r\n Bound ip(mac %02x:%02x:%02x:%02x:%02x:%02x, ip %s) is already in used by others",
                                        //    ucUserMac[0], ucUserMac[1], ucUserMac[2], ucUserMac[3], ucUserMac[4], ucUserMac[5], buf);
                                        //ulGetIp = 0;
                                    }
                                    else
                                    {
                                        pDhcp->ulgiaddr = osal_htonl(ifIpAddr[i]);
                                        ulGetIp = ulUserIp;
                                    }
                                    break;
                                }
                            }
                            continue_outside_for1:
                                ;/* null statement */
                        }
                        if ( i==DEFAULT_MAX_IP_SUBNET)
                        {
                            DHCP_SERVER_DBG ( "No bound ip can be allocated from this interface vlan %d", if_Para->dhcp_vid);
                            goto get_ip_error;
                        }
                    }
                    else
                    {
                        goto get_ip_error;
                    }
                }
                else
                {
                    if ((rc == IP_IN_USED) && dhcpserver_ip_is_usedByOthers(ulUserIp, ucUserMac))
                    {
                        //dhcp_ipaddr_to_str(ulUserIp, buf);
                        //dhcpserver_debug_printf("\r\n Bound ip(mac %02x:%02x:%02x:%02x:%02x:%02x, ip %s) is already in used by others",
                        //    ucUserMac[0], ucUserMac[1], ucUserMac[2], ucUserMac[3], ucUserMac[4], ucUserMac[5], buf);
                        //ulGetIp = 0;
                    }
                    else
                    {
                        ulGetIp = ulUserIp;
                    }
                }
            }
        }
        else
        {
assign_unbound:
            /*Determine whether the user specifies the application IP*/
            if ( stDhcpOption.pulReqIp != NULL) /*apply for a designated address*/
            {
                memcpy(&ultempIP, stDhcpOption.pulReqIp, sizeof(ulong_t));
                ultempIP = osal_ntohl(ultempIP);
                rc = IPPOOL_GetSpecialIp ( osal_ntohl(pDhcp->ulgiaddr), ultempIP,cantBeBound, vid);
                if ( rc != IP_SUCCESS )
                {
                    //dhcp_ipaddr_to_str(ultempIP, buf);
                    //dhcpserver_debug_printf ( "\r\n Get special ip %s from ip pool failure ", buf);
                }
                else
                {
                    ulGetIp = ultempIP;
                }
            }

            if ( (stDhcpOption.pulReqIp != NULL && rc != IP_SUCCESS) /*application for designated address failed*/
                  ||stDhcpOption.pulReqIp == NULL) /*or no addres is specified*/
            {
                /*Apply for an idle IP address from the address pool*/
                rc = IPPOOL_GetAnyFreeIp ( osal_ntohl(pDhcp->ulgiaddr), &ulGetIp, LEASE_ABLE, cantBeBound, vid);
                if ( rc != IP_SUCCESS )
                {
                    DHCP_SERVER_DBG ( "Get any free ip from ip pool failure " );
                    if (dhcpserver_ip_is_ifip(osal_ntohl(pDhcp->ulgiaddr),if_Para->dhcp_sw,ifIpAddr))
                    {
                        for(i=0; i<DEFAULT_MAX_IP_SUBNET; i++)
                        {
                            if(ifIpAddr[i])
                            {
                                int j = 0;
                                for(j=0; j < DHCPINTERFACEMAXNUM; j++)
                                {
                                    if(dhcpOption60MatchEntry[if_Para->dhcp_sw-1][j].gateway == ifIpAddr[i])
                                    {
                                        goto continue_outside_for2;/*this gateway has been assigned with another option60 address*/
                                    }
                                }
                                //dhcp_ipaddr_to_str(ifIpAddr[i], buf);
                                //dhcpserver_debug_printf ( "\r\n Get any free ip from ip pool(ip %s as gateway)", buf);
                                rc = IPPOOL_GetAnyFreeIp ( ifIpAddr[i], &ulGetIp, LEASE_ABLE, cantBeBound, vid);
                                if ( rc != IP_SUCCESS )
                                {
                                    DHCP_SERVER_DBG("Get any free ip from ip pool(ip %s as gateway) failure ",buf );
                                    continue;
                                }
                                else
                                {
                                    pDhcp->ulgiaddr = osal_htonl(ifIpAddr[i]);
                                    break;
                                }
                            }
                            continue_outside_for2:
                                ;/* null statement */
                        }
                        if ( i==DEFAULT_MAX_IP_SUBNET)
                        {
                            DHCP_SERVER_DBG("No ip can be allocated from this interface vlan %d", if_Para->dhcp_vid);
                            goto get_ip_error;
                        }
                    }
                    else
                    {
                        goto get_ip_error;
                    }
                }
            }
        }
    }
    else if ((pHashData != NULL) && (pHashData->ulIpAddr != 0))
    {
        ulGetIp = pHashData->ulIpAddr;
    }

    /*Apply for IP address successfully*/
    if ( ulGetIp != 0 )
    {
        /*Update data in the HASH table*/
        if ( pHashData == NULL )
        {
            /*Inserting a new entry*/
            stHashData.ulIpAddr = ulGetIp;
            stHashData.usVlanId = (ushort_t)ulVid;
            stHashData.usPort = if_Para->dhcp_port;
            record_hostname(stHashData.hostname, stDhcpOption.pucHostName,stDhcpOption.ucHostNameLen);
            pHashData = dhcpserver_HashInsert(&g_stDhcpHashCB, (void *)&stHashData);
            if ( pHashData == NULL )
            {
                DHCP_SERVER_DBG ( "DHCP Server Insert hash entry error --discover " );
                /*If it tails, th IP address should be released fisrt*/
                IPPOOL_GetbackIp ( osal_ntohl(pDhcp->ulgiaddr), ulGetIp);
                return ERROR;
            }
           	DHCP_SERVER_DBG("userMac=0x%x:%x:%x:%x:%x:%x", pHashData->ucUserMac[0],pHashData->ucUserMac[1], pHashData->ucUserMac[2],
			pHashData->ucUserMac[3], pHashData->ucUserMac[4], pHashData->ucUserMac[5]);

        }
        else
        {
            /*Update table entry*/
            pHashData->ulIpAddr = ulGetIp;
            pHashData->usVlanId = (ushort_t)ulVid;
            pHashData->usPort = if_Para->dhcp_port;
            record_hostname(pHashData->hostname, stDhcpOption.pucHostName,stDhcpOption.ucHostNameLen);
        }

        /*Construct a DHCPOFFER message and send it to the relay module queue*/
        rc = dhcpserver_send_offer_packet(pDhcp, &stDhcpOption, pHashData->ulIpAddr, ulServerIp, if_Para);
        if ( rc != OK )
        {
            DHCP_SERVER_DBG("Dhcp Server send dhcp offer packet failure " );
            /*If the OFFER fails to be sent, the address should be released and the HASH table entry should be deleted*/
            IPPOOL_GetbackIp ( osal_ntohl(pDhcp->ulgiaddr), pHashData->ulIpAddr );
            dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);
            return ERROR;
        }

        /*After successfuly sending a DHCPOFFER, the flag_discover set to 1,
          this flag is set to 0 after receiving a dhcp-request and is used for IP pool calculation.
          This flaf also sets a timeout when the server fails to receive a dhcp-request message*/
        ipPoolSetDiscoverFlag(osal_ntohl(pDhcp->ulgiaddr), pHashData->ulIpAddr, 1);
        return OK;
    }

get_ip_error:
    if((pHashData != NULL)&&(pHashData->ulIpAddr == 0))
    {
        dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);
    }
    return ERROR;
}


/*******************************************************************************
  Function: STATUS dhcpserver_process_request
  Description: Processes the DHCPREQUEST message sent from the DHCP Relay
  Input: DHCP_HEAD_S *pDhcp - message header pointer
         ulong_t ulDhcpLen - message length
         ulong_t ulVid - message VID 
         ulong_t ulServerIp - DHCP server address for message
 		 DHCP_INTERFACE_PARA *if_Para - interface information
  Output: None
  Return: Success OK, Failure ERROR
 *******************************************************************************/
STATUS dhcpserver_process_request( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulServerIp,DHCP_INTERFACE_PARA *if_Para)
{
    MACHASH_S stHashData;
    MACHASH_S *pHashData = NULL;
    uchar_t ucUserMac[6];
    DHCP_OPTION_S stDhcpOption;
    STATUS rc = OK;
    ulong_t ulGetIp = 0;
    uchar_t buf[64];
    ULONG ulUserIp = 0;
    BOOL cantBeBound = FALSE;
    ulong_t vid = 0;
    ulong_t ultempIP = 0;

    DHCP_SERVER_DBG( "DHCP SERVER receive a DHCP REQUESTS packet " );
    memset ( (char *)&stHashData, 0, sizeof(stHashData) );
    /*General message inspection function*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }
    /*Record message option field*/
    dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption );

    /*Since it is a built-in dhcp server, the giaddr field must not be equal to 0*/
    if ( osal_ntohl(pDhcp->ulgiaddr) == 0 )
    {
        DHCP_SERVER_DBG ( "DHCPSERVER: the giaddr value cannot be null " );
        return ERROR;
    }

    /*Check if the customer exist in the HASH table*/
    memcpy ( (char * )&(stHashData.ucUserMac[0]), ucUserMac, 6 );
    pHashData = HashSearch( &g_stDhcpHashCB, (void *)&stHashData );
    if ( pHashData == NULL )
    {
        /*Inserting new entry*/
        pHashData = dhcpserver_HashInsert(&g_stDhcpHashCB, (void *)&stHashData);
        if ( pHashData == NULL )
        {
            DHCP_SERVER_DBG ( "DHCP Server Insert hash entry error -- request " );
            return ERROR;
        }
    }


    /*If there is an assigned IP in the HASH table, set the flag_discover flag to 0*/
    ipPoolSetDiscoverFlag(osal_ntohl(pDhcp->ulgiaddr), pHashData->ulIpAddr, 0);

    /*Check whether the ciaddr field of the message is equal to 0. If it equal to0, it indicates
     that it is an IP renewal request message. Otherwise, it is a lease renewal message*/
    if ( osal_ntohl(pDhcp->ulciaddr) != 0 )
    {
        /*Determine whether the renewed IP address is legal*/
        if (( pHashData->ulIpAddr == 0) || (pHashData->ulIpAddr != osal_ntohl(pDhcp->ulciaddr)))
        {
            rc = ERROR;
        }
        else
        {
            rc = OK;
        }

        //dhcp_ipaddr_to_str( osal_ntohl(pDhcp->ulciaddr), buf );
        if (rc == OK)
        {
            /*Renew IP address*/
            DHCP_SERVER_DBG("dhcpserver_process_request: Renew ip = %s ", buf );
            rc = IPPOOL_RenewIpTime ( osal_ntohl(pDhcp->ulgiaddr), osal_ntohl(pDhcp->ulciaddr) );
        }
        if ( rc == OK )
        {
            /*Lease renewal successfull*/
            DHCP_SERVER_DBG ( "Dhcp Server Renew ip = %s successfully ", buf );
            /*Construct a DHCPACK message and send it to the dhcp relay message queue*/
            rc = dhcpserver_send_ack_packet(pDhcp, &stDhcpOption, osal_ntohl(pDhcp->ulciaddr), ulServerIp ,if_Para);
            if ( rc != OK )
            {
                DHCP_SERVER_DBG( "Dhcp Server send ack failed " );
                return ERROR;
            }
            /*Update the HASH table, because it may be a newly inserted entry (switch restart)*/
            pHashData->ulIpAddr = osal_ntohl(pDhcp->ulciaddr);
            pHashData->usVlanId = (ushort_t)ulVid;
            pHashData->usPort = if_Para->dhcp_port;
            record_hostname(pHashData->hostname, stDhcpOption.pucHostName,stDhcpOption.ucHostNameLen);
        }
        else
        {
            /*Failed to renew lease*/
            DHCP_SERVER_DBG ( "Dhcp Server Renew ip = %s failed ", buf );
            /*Clear records from the HASH table due to lease renewal failure*/
            dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)&stHashData);
            /*Construct a DHCPNAK message and send it to the dhcp relay message queue*/
            rc = dhcpserver_send_nck_packet(pDhcp, &stDhcpOption, 0, ulServerIp, if_Para);
            if ( rc != OK )
            {
                DHCP_SERVER_DBG( "Dhcp Server send nck failed " );
                return ERROR;
            }
        }
    }
    else    /*Failed to renew lease*/
    {
        DHCP_SERVER_DBG ( "dhcpserver_process_request: apply ip " );
        /*Determine whether the user specifies to apply for a centran IP address*/
        if ( stDhcpOption.pulReqIp == NULL )
        {
            /*No specified IP address was applied for*/
            /*Determine whether the IP address in the HASH table has allocated, that is 
            , whether pHashData->ulIpAddr is equal to 0*/
            if ( pHashData->ulIpAddr != 0 )
            {
                /*pHashData->ulIpAddr if it is not 0, it means that the address needs to be renewed*/
                rc = IPPOOL_RenewIpTime ( osal_ntohl(pDhcp->ulgiaddr), pHashData->ulIpAddr );
                //dhcp_ipaddr_to_str( pHashData->ulIpAddr, buf );
                if ( rc == OK )
                {
                    /*Lease renewal succefull*/
                    DHCP_SERVER_DBG ( "Dhcp Server Renew ip = %s successfully ", buf );
                    /*Construct a DHCPACK message and send it to the dhcp relay message queue*/
                    rc = dhcpserver_send_ack_packet(pDhcp, &stDhcpOption, pHashData->ulIpAddr, ulServerIp,if_Para);
                    if ( rc != OK )
                    {
                        DHCP_SERVER_DBG( "Dhcp Server send ack failed " );
                    }
                    return OK;
                }
                else
                {
                    /*Failed to renew lease*/
                    DHCP_SERVER_DBG ( "Dhcp Server Renew ip = %s failed ", buf );
                    /*Construct a DHCPNAK message and send it to the dhcp relay message queue*/
                    rc = dhcpserver_send_nck_packet(pDhcp, &stDhcpOption, 0, ulServerIp,if_Para);
                    if ( rc != OK )
                    {
                        DHCP_SERVER_DBG( "Dhcp Server send nck failed " );
                    }
                    return OK;
                }
            }
            else   /*pHashData->ulIpAddr == 0*/
            {
                /*pHashData->ulIpAddr == 0 means you need to apply for any IP*/

                if(gulIpBindSwitch == TRUE)
                {
                	#if 1
                    if(dhcps_fund_ipbind_by_mac(ucUserMac, &ulUserIp, if_Para->dhcp_vid) == SYS_ERR_OK)
                    {
                        /*Apply for a designated IP address*/
                        //dhcp_ipaddr_to_str( ulUserIp, buf );
                        rc = IPPOOL_GetSpecialIp ( ntohl(pDhcp->ulgiaddr), ulUserIp, cantBeBound, vid );
                        if ( rc != IP_SUCCESS )
                        {
                            DHCP_SERVER_DBG ( "\r\n Dhcp Server apply special ip %s failed", buf );
                            rc = dhcpserver_send_nck_packet(pDhcp, &stDhcpOption, 0, ulServerIp, if_Para);
                            if ( rc != OK )
                            {
                                DHCP_SERVER_DBG( "\r\n Dhcp Server send nck failed " );
                            }
                            /*Delete the newly inserted HASH table entry*/
                            rc = dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);

                            if ( rc != OK )
                            {
                                DHCP_SERVER_DBG ( "\r\n Delete hash entry failed " );
                            }
                            return OK;
                        }
                        else
                        {
                            DHCP_SERVER_DBG ( "\r\n Dhcp Server apply special ip %s successfully ", buf );
                            /*Construct a DHCPACK message and send it to the dhcp relay message queue*/
                            rc = dhcpserver_send_ack_packet(pDhcp, &stDhcpOption, ulUserIp, ulServerIp, if_Para);
                            if ( rc != OK )
                            {
                                DHCP_SERVER_DBG( "\r\n Dhcp Server send ack failed " );
                                /*If the sending fails, the requested IP address should be released and
                                the HASH should be deleted*/
                                IPPOOL_GetbackIp ( ntohl(pDhcp->ulgiaddr), ulUserIp );
                                dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);
                                return ERROR;
                            }
                            /*Update the HASH table*/
                            pHashData->ulIpAddr = ulUserIp;
                            pHashData->usVlanId = (ushort_t)ulVid;
                            pHashData->usPort = if_Para->dhcp_port;
                            record_hostname(pHashData->hostname, stDhcpOption.pucHostName,stDhcpOption.ucHostNameLen);
                            return OK;
                        }
                    }
                    else
                    {
                        if(unboundAssign)
                        {
                            cantBeBound = TRUE;
                            vid = ulVid;
                            goto assign_unbound;
                        }
                        else
                        {
                        //No bound IP found, allocation not allowed
                            rc = dhcpserver_send_nck_packet(pDhcp, &stDhcpOption, 0, ulServerIp,if_Para);
                            if ( rc != OK )
                            {
                                DHCP_SERVER_DBG( "\r\n Dhcp Server send nck failed " );
                            }
                            /*Deleted the newly inserted HASH table entry*/
                            rc = dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);

                            if ( rc != OK )
                            {
                                DHCP_SERVER_DBG ( "\r\n Delete hash entry failed " );
                            }
                            return OK;
                        }
                    }
					#endif
                }
                else
                {
assign_unbound:
                    rc = IPPOOL_GetAnyFreeIp ( osal_ntohl(pDhcp->ulgiaddr), &ulGetIp, LEASE_ABLE, cantBeBound, vid );
                    //dhcp_ipaddr_to_str( ulGetIp, buf );
                    if ( rc == OK )
                    {
                        /*Apply to any IP successfully*/
                        DHCP_SERVER_DBG ( "Dhcp Server apply any ip = %s successfully ", buf );
                        /*Construct a DHCPACK message and send it to the dhcp relay message queue*/
                        rc = dhcpserver_send_ack_packet(pDhcp, &stDhcpOption, ulGetIp, ulServerIp, if_Para);
                        if ( rc != OK )
                        {
                            DHCP_SERVER_DBG( "Dhcp Server send ack failed " );
                            /*If the sending fails, the requested IP address should be released and
                                the HASH should be deleted*/
                            IPPOOL_GetbackIp ( osal_ntohl(pDhcp->ulgiaddr), ulGetIp );
                            HashDelete ( &g_stDhcpHashCB, (void *)pHashData );
                            return ERROR;
                        }
                        /*Update the HASH table*/
                        pHashData->ulIpAddr = ulGetIp;
                        pHashData->usVlanId = (ushort_t)ulVid;
                        pHashData->usPort = if_Para->dhcp_port;
                        record_hostname(pHashData->hostname, stDhcpOption.pucHostName,stDhcpOption.ucHostNameLen);
                        return OK;
                    }
                    else
                    {
                        /*Failed to apply for any IP*/
                        DHCP_SERVER_DBG ( "Dhcp Server apply any ip failed " );
                        /*Construct a DHCPNAK message and send it to the dhcp relay message queue*/
                        rc = dhcpserver_send_nck_packet(pDhcp, &stDhcpOption, 0, ulServerIp, if_Para);
                        if ( rc != OK )
                        {
                            DHCP_SERVER_DBG( "Dhcp Server send nck failed " );
                        }
                        /*Deleted the newly inserted HASH table entry*/
                        rc = dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);

                        if ( rc != OK )
                        {
                            DHCP_SERVER_DBG ( "Delete hash entry failed " );
                        }
                        return OK;
                    }

                }
            }
        }
        else    /*stDhcpOption.pulReqIp != NULL */
        {
            /*The IP address of the server*/
            DHCP_SERVER_DBG ( "dhcpserver_process_request: apply special ip " );
            /*The hash of the requested IP address is returned as pHashData->ulIpAddr.*/
            memcpy(&ultempIP, stDhcpOption.pulReqIp, sizeof(ulong_t));
            ultempIP = osal_ntohl(ultempIP);
            if ( pHashData->ulIpAddr != 0 )
            {
                /*pHashData->ulIpAddr is 0. This is the correct address.
                To return to the correct address, enter the pHash IP address.
                The returned value is 0.*/
                if (ultempIP != pHashData->ulIpAddr )
                {
                    /*The NAK code is not available, the code is not available.*/
                    //dhcp_ipaddr_to_str(ultempIP, buf );
                    DHCP_SERVER_DBG ( "Dhcp Server Renew ip = %s failed ", buf );
                    /*The DHCPNAK error message is displayed. The DHCP relay message is displayed.*/
                    rc = dhcpserver_send_nck_packet(pDhcp, &stDhcpOption, 0, ulServerIp, if_Para);
                    if ( rc != OK )
                    {
                        DHCP_SERVER_DBG( "Dhcp Server send nck failed " );
                    }
                    return OK;
                }

                rc = IPPOOL_RenewIpTime ( osal_ntohl(pDhcp->ulgiaddr), pHashData->ulIpAddr );
               // dhcp_ipaddr_to_str( pHashData->ulIpAddr, buf );
                if ( rc == OK )
                {
                    DHCP_SERVER_DBG ( "Dhcp Server Renew ip = %s successfully ", buf );
                    /*The DHCPACK message is sent to the server and contains the DHCP relay message.*/
                    rc = dhcpserver_send_ack_packet(pDhcp, &stDhcpOption, pHashData->ulIpAddr, ulServerIp, if_Para);
                    if ( rc != OK )
                    {
                        DHCP_SERVER_DBG( "Dhcp Server send ack failed " );
                    }
                    return OK;
                }
                else
                {
                    /*Lost the*/
                    DHCP_SERVER_DBG ( "Dhcp Server Renew ip = %s failed ", buf );
                    /*The DHCPNAK error message is displayed. The DHCP relay message is displayed.*/
                    rc = dhcpserver_send_nck_packet(pDhcp, &stDhcpOption, 0, ulServerIp, if_Para);
                    if ( rc != OK )
                    {
                        DHCP_SERVER_DBG( "Dhcp Server send nck failed " );
                    }
                    return OK;
                }
            }
            else   /*pHashData->ulIpAddr == 0*/
            {
                /*pHashData->ulIpAddr == 0 The following table shows the main points 
                of the instruction:*(stDhcpOption.pulReqIp)*/
                rc = IPPOOL_GetSpecialIp ( osal_ntohl(pDhcp->ulgiaddr), ultempIP, cantBeBound, vid );
                //dhcp_ipaddr_to_str(ultempIP, buf );
                if ( rc == OK )
                {
                    DHCP_SERVER_DBG ( "Dhcp Server apply special ip = %s successfully ", buf );
                    /*The DHCPACK message is sent to the server and contains the DHCP relay message.*/
                    rc = dhcpserver_send_ack_packet(pDhcp, &stDhcpOption, ultempIP, ulServerIp, if_Para);
                    if ( rc != OK )
                    {
                        DHCP_SERVER_DBG( "Dhcp Server send ack failed " );
                        /*If you lose the response, return the URL and delete the hash.*/
                        IPPOOL_GetbackIp ( osal_ntohl(pDhcp->ulgiaddr), ultempIP );
                        dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);
                        return ERROR;
                    }
                    pHashData->ulIpAddr = ultempIP;
                    pHashData->usVlanId = (ushort_t)ulVid;
                    pHashData->usPort = if_Para->dhcp_port;
                    record_hostname(pHashData->hostname, stDhcpOption.pucHostName,stDhcpOption.ucHostNameLen);
                    return OK;
                }
                else
                {
                    /*Requested IP (stDhcpOption.pulReqIp) lost/missing */
                    DHCP_SERVER_DBG ( "Dhcp Server apply special ip = %s failed ", buf );
                    /*Request DHCP NAK or failed to relay DHCP relay message*/
                    rc = dhcpserver_send_nck_packet(pDhcp, &stDhcpOption, 0, ulServerIp, if_Para);
                    if ( rc != OK )
                    {
                        DHCP_SERVER_DBG( "Dhcp Server send nck failed " );
                    }
                    /*Delete the hash information in the request.*/
                    rc = dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);
                    if ( rc != OK )
                    {
                        DHCP_SERVER_DBG ( "Delete hash entry failed " );
                    }
                    return OK;
                }
            }

        }
    }

    return OK;
}


/*******************************************************************************
  Function: STATUS dhcpserver_process_decline
  Description: This function returns a DHCP RELAY message to the server. 
             This function returns a DHCPDECLINE message to the server.
  Input:       DHCP_HEAD_S *pDhcp - message header pointer
               ulong_t ulDhcpLen - message length
               ulong_t ulVid - message vid
               ulong_t ulServerIp - DHCP Server IP address for message
		       DHCP_INTERFACE_PARA *if_Para - interface information
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcpserver_process_decline( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulServerIp,DHCP_INTERFACE_PARA *if_Para)
{
    MACHASH_S stHashData;
    MACHASH_S *pHashData = NULL;
    uchar_t ucUserMac[6];
    STATUS rc = OK;
    uchar_t buf[32];
    DHCP_OPTION_S stDhcpOption;
    //char    szTmpE[128];
    ulong_t ultempIP = 0;

    DHCP_SERVER_DBG ( "DHCP SERVER receive a DHCP DECLINE packet " );
    memset ( (char *)&stHashData, 0, sizeof(stHashData) );
    /*Status code 1 or 7.*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }
    /*Record the selected option*/
    dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption );

    /*The dhcp server giaddr is not set to 0.*/
    if ( osal_ntohl(pDhcp->ulgiaddr) == 0 )
    {
        DHCP_SERVER_DBG ( "DHCPSERVER: the giaddr value cannot be null " );
        return ERROR;
    }

    /*The decline is not a request. The IP address is 0.*/
    memcpy(&ultempIP, stDhcpOption.pulReqIp, sizeof(ulong_t));
    ultempIP = osal_ntohl(ultempIP);
    //dhcp_ipaddr_to_str( ultempIP, buf );
    if (ultempIP == 0)
    {
        DHCP_SERVER_DBG ( "DHCPSERVER: The request ip cannot be 0, because the packet is DHCP DECLINE " );
        return ERROR;
    }

    /*Delete HASH request related error status codes 1 and 7*/
    memcpy ( (char * )&(stHashData.ucUserMac[0]), ucUserMac, 6 );
    pHashData = HashSearch( &g_stDhcpHashCB, (void *)&stHashData );
    if ( pHashData != NULL )
    {
        /*The client rejects the address-related hash request with error status codes 1 and 7. 
        It is necessary to delete the hash key, and the function returns an error status code*/
        if ( pHashData->ulIpAddr == ultempIP )
        {
            rc = dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);
            if ( rc != OK )
            {
                DHCP_SERVER_DBG ( "dhcpserver_process_decline: Delete hash entry failed " );
                return ERROR;
            }
        }
        else
        {
            DHCP_SERVER_DBG ( "The decline ip %s is not equal to the ip in the hash entry", buf);
            return ERROR;
        }
    }

    /*Notification address related request burst*/
    rc = IPPOOL_SetConflictIp ( osal_ntohl(pDhcp->ulgiaddr), ultempIP );
    if ( rc != OK )
    {
        DHCP_SERVER_DBG ( "Set ip = %s to conflict failure ", buf );
        return ERROR;
    }
#if 0
    if (syslogModuleSwitch(MID_DHCP, WARNINGS))
    {
        sprintf(szTmpE, "\r\ndhcp: IP address %s is conflict!", buf);
        syslogSendInfo(MID_DHCP, WARNINGS, "dhcp", szTmpE, szTmpE, FALSE, FALSE, NO_WAIT);
    }
#endif
    DHCP_SERVER_DBG ( "Set ip = %s to conflict successfully ", buf );
    return OK;
}


/*******************************************************************************
Function: STATUS dhcpserver_process_release
Description: Process DHCP RELAY message related to DHCPRELEASE handling
Input: DHCP_HEAD_S *pDhcp ¡ª pointer to DHCP message header
    ulong_t ulDhcpLen ¡ª length of the DHCP message, including header and options
    ulong_t ulVid ¡ª VLAN ID
    ulong_t ulServerIp ¡ª IP address of the DHCP server
    DHCP_INTERFACE_PARA *if_Para ¡ª interface parameters
Output: None
Return: OK for success, ERROR for failure
*******************************************************************************/
STATUS dhcpserver_process_release( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulServerIp  ,DHCP_INTERFACE_PARA *if_Para)
{
    MACHASH_S stHashData;
    MACHASH_S *pHashData = NULL;
    uchar_t ucUserMac[6];
    STATUS rc = OK;
    uchar_t buf[32];
    ulong_t ultempIP = 0;

    DHCP_SERVER_DBG ( "DHCP SERVER receive a DHCP RELEASE packet " );
    memset ( (char *)&stHashData, 0, sizeof(stHashData) );
    /*Communication protocol status codes 1 and 7.*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }

    /*When the DHCP server receives the giaddr field as 0*/
    if ( osal_ntohl(pDhcp->ulgiaddr) == 0 )
    {
        DHCP_SERVER_DBG ( "DHCPSERVER: the giaddr value cannot be null " );
        return ERROR;
    }

    /*In the client release message, the ciaddr field is 0*/
    ultempIP = osal_ntohl(pDhcp->ulciaddr);
    //dhcp_ipaddr_to_str ( ultempIP, buf );
    if ( ultempIP == 0 )
    {
        DHCP_SERVER_DBG ( "DHCPSERVER: The ciaddr cannot be 0, because the packet is DHCP RELEASE " );
        return ERROR;
    }

    /*Client rejects HASH request with error status codes 1 and 7*/
    memcpy ( (char * )&(stHashData.ucUserMac[0]), ucUserMac, 6 );
    pHashData = HashSearch( &g_stDhcpHashCB, (void *)&stHashData );
    if ( pHashData != NULL )
    {
        /*The client rejects the address-related hash request with error status codes 1 and 7. 
        It is necessary to delete the hash key, and the function returns an error status code*/
        if ( pHashData->ulIpAddr == ultempIP )
        {
            rc = dhcpserver_HashDelete(&g_stDhcpHashCB, (void *)pHashData);
            if ( rc != OK )
            {
                DHCP_SERVER_DBG ( "dhcpserver_process_decline: Delete hash entry failed " );
                return ERROR;
            }
        }
        else
        {
            DHCP_SERVER_DBG ( "The release ip %s is not equal to the ip in the hash entry", buf);
            return ERROR;
        }
    }
    else
    {
        /*DHCP hash based on IP does not respond according to the MAC address; it should be based on the MAC address*/
        DHCP_SERVER_DBG ( "Can't find client MAC %x:%x:%x:%x:%x:%x  in dhcp hash table!",
                  ucUserMac[0],ucUserMac[1],ucUserMac[2],ucUserMac[3],ucUserMac[4],ucUserMac[5]);
        return ERROR;
    }

    /*Notify the client about address-related changes or address stripping*/
    IPPOOL_GetbackIp ( osal_ntohl(pDhcp->ulgiaddr), ultempIP );

    DHCP_SERVER_DBG ( "Release ip = %s successfully ", buf );
    return OK;
}

/*******************************************************************************
Function: STATUS dhcpserver_rx_timertick
Description: On each timer tick, the server checks all IP pools and interfaces 
             to find IP addresses whose leases have expired or have abnormalities related 
             to status codes 1 and 7. For such cases, the server deletes the corresponding 
             hash entries associated with those IP addresses.
Parameters: None
Return values: 1 for success, 0 for failure
Status: OK for success, ERROR for failure
*******************************************************************************/
STATUS dhcpserver_rx_timertick(void)
{
    /*Check all IP pools and interfaces for allocation states 1 and 7*/
    HandIpAddrLeaseTimer();
    HandIpAddrConflictTimer();
    return OK;
}


/*******************************************************************************
Function: STATUS dhcpserver_send_offer_packet
Description: This function sends a DHCPOFFER message through the DHCP relay module to a client
Parameters: pDhcp: pointer to the DHCP message header
            pDhcpOption: pointer to the DHCP options structure
            ulIP: the IP address being offered to the client
            ulServerIp: the DHCP server's IP address responding to the client
            DHCP_INTERFACE_PARA *if_Para: interface parameters
Return value: 1 indicates success
              0 indicates failure
Status: OK for success, ERROR for failure
*******************************************************************************/
STATUS dhcpserver_send_offer_packet( DHCP_HEAD_S *pDhcp, DHCP_OPTION_S *pDhcpOption, ulong_t ulIP, ulong_t ulServerIp,DHCP_INTERFACE_PARA *if_Para)
{
    DHCP_HEAD_S    *pDhcpHead;
    STATUS rc = OK;
    int i = 0;
    ulong_t ulGatewayIp = 0;
    ulong_t ulMask= 0;
    ulong_t ulRouterIp = 0;
    ulong_t ulDnsIp[IPPOOL_MAX_DNS_NUM] = {0};
    ulong_t ulPrimaryNBNSIp = 0;
    ulong_t ulSecondNBNSIp = 0;
    ulong_t ulLeaseTime = 0;
    uchar_t *pucDhcpOption = NULL;
    ulong_t *pulMagicCookie = NULL;
    ulong_t ulReqParamFlag = 0;
    ulong_t *pulIp = NULL;
    uchar_t *pucLength = NULL;
    ulong_t ulDhcpLen = 0;
    char *p = NULL;
    ulong_t ulIfMask = 0;
    uchar_t str[20];
    uchar_t str1[20];
    uchar_t szDnsSuffixName[IPPOOL_MAX_DNS_SUFFIX_NAME];
    ulong_t vlen = 0;
    ulong_t ultemp = 0;
    IPPOOL_OPTION43 option43;
    char ucoption66[IPPOOL_OPTION66_MAX_LEN+1], ucoption67[IPPOOL_OPTION67_MAX_LEN+1];


    
    if ( pDhcp == NULL || pDhcpOption == NULL )
    {
        DHCP_SERVER_DBG ( "The pDhcp or pDhcpOption is invalid parameter " );
        return ERROR;
    }
    
    //dhcp_ipaddr_to_str ( ulIP, str );
    #if 1
    if ( dhcprelay_check_ip_valid ( ulIP ) != OK || ulIP == 0)
    {
        dhcpserver_debug_printf ( "\r\n The ip %s assigned by ip pool is invalid ip address ", str );
        return ERROR;
    }
	#endif

    /*Get public parameters of the address pool*/
    ulGatewayIp = osal_ntohl(pDhcp->ulgiaddr);
    //dhcp_ipaddr_to_str ( ulGatewayIp, str1 );
    memset ( szDnsSuffixName, 0, sizeof(szDnsSuffixName) );
    rc = IpPool_GetParameter( ulGatewayIp, &ulMask, ulDnsIp,
                                                &ulPrimaryNBNSIp, &ulSecondNBNSIp, &ulLeaseTime, szDnsSuffixName, &ulRouterIp);
    rc += IPPOOL_GetOpton43(ulGatewayIp, &option43);
    if ( rc != OK )
    {
        DHCP_SERVER_DBG ( "Get ip pool common parameter failed " );
    }

    /*Get the subnet mask of the interface corresponding to the gateway of the address pool. 
    Note that this code only supports the device being used as the first-level relay/server*/
    dhcp_get_ifmask_by_ipaddr( ulGatewayIp, &ulIfMask );

    /*To determine whether the assigned IP address and the gateway address are in the same subnet, 
    you first need to compare the subnet mask lengths of the address pool and the corresponding interface, 
    and use the larger subnet mask length for the judgment*/
    if ( ulIfMask > ulMask )
    {
        ulMask = ulIfMask;
        if (((ulIP&(~ulIfMask)) == (~ulIfMask)) 
            || ((ulIP&(~ulIfMask)) == 0))       
        {
            DHCP_SERVER_DBG ( "The user's Ip %s assigned by ip pool is broadcast or subnet address of the interface ip addrss %s ", str, str1 );
            return ERROR;
        }
    }
    if ( (ulIP & ulMask) != (ulGatewayIp & ulMask) )
    {
        DHCP_SERVER_DBG ( "The user's Ip %s assigned by ip pool not in the same subnet as the interface ip addrss %s ", str, str1 );
        return ERROR;
    }

    /*Start the DHCPOFFER header*/
    /*The DHCP relay model is used to verify the authenticity of the network.*/
    p = (char *)osal_alloc( DHCP_MAX_LEN );
    if ( p == NULL )
    {
        DHCP_SERVER_DBG ( "dhcpserver_send_offer_packet: Malloc memory failure " );
        return ERROR;
    }
    memset ( (void *)p, 0, DHCP_MAX_LEN );

    /*DHCP assigns static IP addresses*/
    pDhcpHead = (DHCP_HEAD_S *)p;
    pDhcpHead->ucop      = DHCP_REPLY;
    pDhcpHead->uchtype   = ETHERNET_HARDWARETYPE;
    pDhcpHead->uchlen    = ETHERNET_HARDWARELEN;
    pDhcpHead->uchops    = 0;
    pDhcpHead->ulxid     = pDhcp->ulxid;  
    pDhcpHead->ussecs    = pDhcp->ussecs;
    pDhcpHead->usflag    = pDhcp->usflag; 
    pDhcpHead->ulciaddr  = pDhcp->ulciaddr;
    pDhcpHead->ulyiaddr  = osal_htonl(ulIP);   
    pDhcpHead->ulsiaddr  = osal_htonl( ulServerIp );
    pDhcpHead->ulgiaddr  = pDhcp->ulgiaddr;   
    memcpy(pDhcpHead->szchaddr, pDhcp->szchaddr, ETHERNET_HARDWARELEN); 

    /* Start setting dhcp option */

    /*Get the required address for the network segment*/
    dhcpserver_record_client_request_param ( pDhcpOption, &ulReqParamFlag );

    /*Start configuring DHCP options*/
    /*magic cookie */
    pulMagicCookie = (ulong_t *)(char *)(pDhcpHead+1);
    ultemp = osal_htonl(MAGIC_COOKIE);
    memcpy(pulMagicCookie, &ultemp, sizeof(ulong_t));

    /*To support BOOTP, you must specify the BOOTP configuration file*/
    if ( pDhcpOption->pucDHCPMsgType == NULL )
    {
        /*The BOOTP command is only used to select the interception site for the BOOTP server*/
        pucDhcpOption = (uchar_t *)(pulMagicCookie+1);
        /*submask option*/
        *pucDhcpOption++ = DHCP_SUBNETMASK_CODE;
        *pucDhcpOption++ = 4;  /*Option Length*/
        pulIp = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(ulMask);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption = (uchar_t *)(pulIp + 1);

        /*gateway ip option*/
        *pucDhcpOption++ = DHCP_ROUTERIP_CODE;
        *pucDhcpOption++ = 4;  /*Option Length*/
        pulIp = (ulong_t *)pucDhcpOption;
        ultemp = (ulRouterIp != 0) ? osal_htonl(ulRouterIp) : osal_htonl(ulGatewayIp);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption = (uchar_t *)(pulIp + 1);

        goto sendoffer;
    }

    /*"DHCP message type" option*/
    pucDhcpOption = (uchar_t *)(pulMagicCookie+1);
    *pucDhcpOption++ = DHCP_MESSAGETYPE_CODE;   
    *pucDhcpOption++ = 1;    
    *pucDhcpOption++ = DHCP_OFFER_TYPE;   

    /*"Ip subnet mask " option*/
    if ( ulReqParamFlag & DHCP_SUBNETMASK_CODE_FLAGBIT )
    {
        *pucDhcpOption++ = DHCP_SUBNETMASK_CODE;
        *pucDhcpOption++ = 4;  
        pulIp = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(ulMask);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption    = (uchar_t *)(pulIp + 1);
    }

    /*"Router Ip " option */
    if (ulReqParamFlag & DHCP_ROUTERIP_CODE_FLAGBIT )
    {
        *pucDhcpOption++ = DHCP_ROUTERIP_CODE;
        *pucDhcpOption++ = 4;  
        pulIp = (ulong_t *)pucDhcpOption;
        ultemp = (ulRouterIp != 0) ? osal_htonl(ulRouterIp) : osal_htonl(ulGatewayIp);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption = (uchar_t *)(pulIp + 1);
    }

    /*"DNS IP" option  currentky consist of two: primary and secondary*/
    if (ulReqParamFlag & DHCP_DNSIP_CODE_FLAGBIT )
    {
        *pucDhcpOption++  = DHCP_DNSIP_CODE;
        pucLength         = pucDhcpOption;
        *pucLength        = 0;
        pucDhcpOption++;
        pulIp    = (ulong_t *)pucDhcpOption;

        for (i = 0; i < IPPOOL_MAX_DNS_NUM; i++)
        {
        	#if 1
            if( (dhcprelay_check_ip_valid(ulDnsIp[i]) == OK) && (ulDnsIp[i] != 0) )
            {
                *pucLength += 4;
                ultemp = osal_htonl(ulDnsIp[i]);
                memcpy(pulIp, &ultemp, sizeof(ulong_t));
                pulIp++;
            }
            #endif
        }

        if(*pucLength == 0)
        {
            /*If the NS IP address is not specified, this option must be selected. 
            If not, set the pucDhcpOption option to the specified value*/
            pucDhcpOption -= 2;
            DHCP_SERVER_DBG("IP pool has no DNS configuration ");
        }
        else
        {
            pucDhcpOption = (uchar_t *)(pulIp);
        }
    }

    /*"Ip lease time" option */
    if (ulReqParamFlag & DHCP_IPLEASETIME_CODE_FLAGBIT )
    {
        *pucDhcpOption++ = DHCP_IPLEASETIME_CODE;
        *pucDhcpOption++ = 4;  
        pulIp = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(ulLeaseTime);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption = (uchar_t *)(pulIp + 1);

        /*"Renew time" option */
        if (ulReqParamFlag & DHCP_RENEWTIME_CODE_FLAGBIT )
        {
            *pucDhcpOption++ = DHCP_RENEWTIME_CODE;
            *pucDhcpOption++ = 4;  
            pulIp = (ulong_t *)pucDhcpOption;
            ultemp = osal_htonl(ulLeaseTime/2);
            memcpy(pulIp, &ultemp, sizeof(ulong_t));
            pucDhcpOption = (uchar_t *)(pulIp + 1);
        }

        /*"Rebind time" option */
        if (ulReqParamFlag & DHCP_REBINDTIME_CODE_FLAGBIT )
        {
            *pucDhcpOption++ = DHCP_REBINDTIME_CODE;
            *pucDhcpOption++ = 4;  
            pulIp = (ulong_t *)pucDhcpOption;
            ultemp = osal_htonl(ulLeaseTime * 7 / 8);
            memcpy(pulIp, &ultemp, sizeof(ulong_t));
            pucDhcpOption = (uchar_t *)(pulIp + 1);
        }
    }
    else  /*Time*/
    {
        /*"Renew time" option */
        *pucDhcpOption++ = DHCP_RENEWTIME_CODE;
        *pucDhcpOption++ = 4; 
        pulIp = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(DHCP_MAX_RENEW_TIME);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption = (uchar_t *)(pulIp + 1);

        /*"Rebind time" option */
        *pucDhcpOption++ = DHCP_REBINDTIME_CODE;
        *pucDhcpOption++ = 4; 
        pulIp  = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(DHCP_MAX_REBIND_TIME);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption = (uchar_t *)(pulIp + 1);

        /*"Ip lease time" option    */
        *pucDhcpOption++ = DHCP_IPLEASETIME_CODE;
        *pucDhcpOption++ = 4; 
        pulIp  = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(DHCP_MAX_LEASE_TIME);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption = (uchar_t *)(pulIp + 1);
    }

    /*"Netbios name server" option  */
    if (ulReqParamFlag & DHCP_NBNS_CODE_FLAGBIT )
    {
        *pucDhcpOption++  = DHCP_NBNS_CODE;
        *pucDhcpOption++  = 8; 
        pulIp = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(ulPrimaryNBNSIp);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pulIp++;
        ultemp = osal_htonl(ulSecondNBNSIp);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption = (uchar_t *)(pulIp + 1);
    }

    /*"Domain name suffix" option  */
    if ((ulReqParamFlag & DHCP_DOMAINNAME_CODE_FLAGBIT) && strlen((char *)szDnsSuffixName) != 0 )
    {
        *pucDhcpOption++  = DHCP_DOMAINNAME_CODE;
        vlen = (strlen ( (char *)szDnsSuffixName )) + 1;
        *pucDhcpOption++  = (uchar_t)vlen;  
        memcpy ( pucDhcpOption, szDnsSuffixName, vlen );
        pucDhcpOption += vlen;
    }

    /*"dhcp server ip" option*/
    *pucDhcpOption++ = DHCP_SERVERIP_CODE;
    *pucDhcpOption++ = 4;  
    pulIp  = (ulong_t *)pucDhcpOption;
    ultemp = osal_htonl(ulServerIp);
    memcpy(pulIp, &ultemp, sizeof(ulong_t));
    pucDhcpOption = (uchar_t *)(pulIp + 1);

    /*option43*/
    if ( (ulReqParamFlag & DHCP_OPTION43_CODE_FLAGBIT) && option43.ucValueLen )
    {
        *pucDhcpOption++  = DHCP_OPTION43_CODE;
        *pucDhcpOption++  = option43.ucValueLen;  
        memcpy(pucDhcpOption, option43.szValue, option43.ucValueLen);
        pucDhcpOption += option43.ucValueLen;
    }

    /*option66: TFTP server name*/
    if (ulReqParamFlag & DHCP_TFTPSERVER_NAME_CODE_FLAGBIT)
    {
        memset(ucoption66, '\0', sizeof(ucoption66));
        if (IP_SUCCESS == IpPool_GetParameter_tftp(ulGatewayIp, (uchar_t*)ucoption66, NULL))
        {
            vlen = strlen(ucoption66);
            if (vlen > 0)
            {
                *pucDhcpOption++ = DHCP_TFTPSERVER_NAME_CODE;
                *pucDhcpOption++  = (uchar_t)vlen;  
                memcpy(pucDhcpOption, ucoption66, vlen);
                pucDhcpOption += vlen;
            }
        }
    }

    /*option67: Bootfile name*/
    if (ulReqParamFlag & DHCP_BOOTFILE_NAME_CODE_FLAGBIT)
    {
        memset(ucoption67, '\0', sizeof(ucoption67));
        if (IP_SUCCESS == IpPool_GetParameter_tftp(ulGatewayIp, NULL, (uchar_t*)ucoption67))
        {
            vlen = strlen((char *)ucoption67);
            if (vlen > 0)
            {
                memcpy(pDhcpHead->szfile, ucoption67, vlen);
                *pucDhcpOption++  = DHCP_BOOTFILE_NAME_CODE;
                *pucDhcpOption++  = (uchar_t)vlen;  
                memcpy(pucDhcpOption, ucoption67, vlen);
                pucDhcpOption += vlen;
            }
        }
    }

    /* option 60 */
    if(pDhcpOption->pucOption60)
    {
        dhcpserver_option60_process(&pucDhcpOption, pDhcpOption, if_Para);
    }
    /*Terminator character*/
    sendoffer:
    *pucDhcpOption++   = ENDOFF_OPTIONS;

    /*configure DNS IP for DHCP.*/
    ulDhcpLen = pucDhcpOption - (uchar_t *)(pDhcpHead);
    /*To support BOOTP, the BOOTP service should be enabled and started,
     and small-scale DHCP service is sufficient to support BOOTP clients*/
    if ( ulDhcpLen < DHCP_MIN_LEN )
    {
        ulDhcpLen = DHCP_MIN_LEN;
    }

    
    if ( OK != dhcprelayRxFromLocalserver(pDhcpHead, ulDhcpLen, if_Para) )
    {
        osal_free(p);
        DHCP_SERVER_DBG ( "Dhcp Server send dhcprelayRxFromLocalserver(DHCPOFFER) msg failed " );
        return ERROR;
    }

    osal_free(p);
    return OK;
}


/*******************************************************************************
  Function: STATUS dhcpserver_record_client_request_param
  Description: Checks which DHCP parameters the user needs and record them into
                into a long integer
  Input: pDhcpOption - Pointer to the option structure of the request message
  Output: pulReqParamFlag - flag recording the DHCP parameters requesteed by the user
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS  dhcpserver_record_client_request_param(DHCP_OPTION_S *pstDhcpOption, ulong_t *pulReqParamFlag )
{
    uchar_t ucReqParamLen;
    uchar_t *pucReqParam;
    uchar_t i;

    if((pstDhcpOption == NULL)||(pulReqParamFlag == NULL))
    {
        DHCP_SERVER_DBG ( "dhcpserver_record_client_request_param: Invalid parameter" );
        return ERROR;
    }
    pucReqParam   = pstDhcpOption->pucReqParamList;
    ucReqParamLen = pstDhcpOption->ucReqParamListLen;

    if ((pucReqParam == NULL) || (ucReqParamLen == 0))
    {
        DHCP_SERVER_DBG( "dhcpserver_record_client_request_param: client didn't request any parameter" );
        *pulReqParamFlag = 0;
        ucReqParamLen = 0;
    }

    /*DHCP client request parameter flag pulReqParamFlag*/
    for (i = 0; i < ucReqParamLen; i++)
    {
        switch (*pucReqParam++)
        {
            case DHCP_SUBNETMASK_CODE :
                *pulReqParamFlag |= DHCP_SUBNETMASK_CODE_FLAGBIT;
                break;

            case DHCP_ROUTERIP_CODE :
                *pulReqParamFlag |= DHCP_ROUTERIP_CODE_FLAGBIT;
                break;

            case DHCP_DNSIP_CODE :
                *pulReqParamFlag |= DHCP_DNSIP_CODE_FLAGBIT;
                break;

            case DHCP_DOMAINNAME_CODE :
                *pulReqParamFlag |= DHCP_DOMAINNAME_CODE_FLAGBIT;
                break;

            case DHCP_NBNS_CODE :
                *pulReqParamFlag |= DHCP_NBNS_CODE_FLAGBIT;
                break;

            case DHCP_NBDS_CODE :
                *pulReqParamFlag |= DHCP_NBDS_CODE_FLAGBIT;
                break;

            case DHCP_NBNT_CODE :
                *pulReqParamFlag |= DHCP_NBNT_CODE_FLAGBIT;
                break;

            case DHCP_NBSCOPE_CODE :
                *pulReqParamFlag |= DHCP_NBSCOPE_CODE_FLAGBIT;
                break;

            case DHCP_IPLEASETIME_CODE :
                *pulReqParamFlag |= DHCP_IPLEASETIME_CODE_FLAGBIT;
                break;

            case DHCP_RENEWTIME_CODE :
                *pulReqParamFlag |= DHCP_RENEWTIME_CODE_FLAGBIT;
                break;

            case DHCP_REBINDTIME_CODE :
                *pulReqParamFlag |= DHCP_REBINDTIME_CODE_FLAGBIT;
                break;

            case DHCP_OPTION43_CODE:
                *pulReqParamFlag |= DHCP_OPTION43_CODE_FLAGBIT;
                break;

            case DHCP_TFTPSERVER_NAME_CODE:
                *pulReqParamFlag |= DHCP_TFTPSERVER_NAME_CODE_FLAGBIT;
                break;

            case DHCP_BOOTFILE_NAME_CODE:
                *pulReqParamFlag |= DHCP_BOOTFILE_NAME_CODE_FLAGBIT;
                break;

            default:
                break;
        }
    }
    /*This is about how Windows DHCP clients send DISCOVER and REQUEST messages, 
    and how flags such as pulReqParamFlag are involved in recording or requesting 
    certain parameters during DHCP negotiation*/
    *pulReqParamFlag |= DHCP_IPLEASETIME_CODE_FLAGBIT;
    *pulReqParamFlag |= DHCP_RENEWTIME_CODE_FLAGBIT;
    *pulReqParamFlag |= DHCP_REBINDTIME_CODE_FLAGBIT;

    /*This is related to how Windows DHCP clients manage and record flags for 
    DISCOVER and REQUEST messages in the DHCP negotiation process*/
    *pulReqParamFlag |= DHCP_SUBNETMASK_CODE_FLAGBIT;
    *pulReqParamFlag |= DHCP_ROUTERIP_CODE_FLAGBIT;

    return OK;
}

/*******************************************************************************
§¡§Ô§ä§ã§Ö§ê§ë§ä: STATUS dhcpserver_send_ack_packet
Description: Sends a DHCPACK packet to inform the client that its request has been approved, considering whether to relay the DHCP message through the DHCP relay module.
Parameters: pDhcp: Pointer to the DHCP packet header
            pDhcpOption: Pointer to the DHCP options structure
            ulIP: Assigned client IP address
            ulServerIp: DHCP server IP address responding to the client
            DHCP_INTERFACE_PARA *if_Para: Interface parameters information
Return: Indicates success (OK) or failure (ERROR)
*******************************************************************************/
STATUS dhcpserver_send_ack_packet( DHCP_HEAD_S *pDhcp, DHCP_OPTION_S *pDhcpOption, ulong_t ulIP, ulong_t ulServerIp, DHCP_INTERFACE_PARA *if_Para)
{
    DHCP_HEAD_S    *pDhcpHead;
    STATUS rc = OK;
    int i = 0;
    ulong_t ulGatewayIp = 0;
    ulong_t ulMask= 0;
    ulong_t ulRouterIp = 0;
    ulong_t ulDnsIp[IPPOOL_MAX_DNS_NUM] = {0};
    ulong_t ulPrimaryNBNSIp = 0;
    ulong_t ulSecondNBNSIp = 0;
    ulong_t ulLeaseTime = 0;
    uchar_t *pucDhcpOption = NULL;
    ulong_t *pulMagicCookie = NULL;
    ulong_t ulReqParamFlag = 0;
    ulong_t *pulIp = NULL;
    uchar_t *pucLength = NULL;
    ulong_t ulDhcpLen = 0;
    char *p = NULL;
    ulong_t ulIfMask = 0;
    uchar_t str[20];
    uchar_t str1[20];
    uchar_t szDnsSuffixName[IPPOOL_MAX_DNS_SUFFIX_NAME];
    ulong_t vlen = 0;
    ulong_t ultemp = 0;
    IPPOOL_OPTION43 option43;
    uchar_t ucoption66[IPPOOL_OPTION66_MAX_LEN+1], ucoption67[IPPOOL_OPTION67_MAX_LEN+1];

    if ( pDhcp == NULL || pDhcpOption == NULL )
    {
        DHCP_SERVER_DBG ( "The pDhcp or pDhcpOption is invalid parameter " );
        return ERROR;
    }
    
    //dhcp_ipaddr_to_str ( ulIP, str );
    #if 1
    if ( dhcprelay_check_ip_valid ( ulIP ) != OK || ulIP == 0 )
    {
        dhcpserver_debug_printf ( "\r\n The ip %s assigned by ip pool is invalid ip address ", str );
        return ERROR;
    }
    #endif

    /*Get address*/
    ulGatewayIp = osal_ntohl(pDhcp->ulgiaddr);
    //dhcp_ipaddr_to_str ( ulGatewayIp, str1 );
    memset ( szDnsSuffixName, 0, sizeof(szDnsSuffixName) );
    rc = IpPool_GetParameter( ulGatewayIp, &ulMask, ulDnsIp,
                                                &ulPrimaryNBNSIp, &ulSecondNBNSIp, &ulLeaseTime, szDnsSuffixName, &ulRouterIp);
    rc += IPPOOL_GetOpton43(ulGatewayIp, &option43);
    if ( rc != OK )
    {
        DHCP_SERVER_DBG ( "Get ip pool common parameter failed " );
    }

    /* Obtaining the address is related to the interface parameters; note this is not fully verified. 
    It supports equipment used as a relay/server */
    dhcp_get_ifmask_by_ipaddr( ulGatewayIp, &ulIfMask );

    /* The address accessed and the subnet address belong to the same subnet, 
    so check the subnet mask length carefully to avoid IP address conflicts and broadcast storms,
     which cause network congestion. */
    if ( ulIfMask > ulMask )
    {
        ulMask = ulIfMask;
        if (((ulIP&(~ulIfMask)) == (~ulIfMask)) 
            || ((ulIP&(~ulIfMask)) == 0))       
        {
            DHCP_SERVER_DBG ( "The user's Ip %s assigned by ip pool is broadcast or subnet address of the interface ip addrss %s ", str, str1 );
            return ERROR;
        }
    }
    if ( (ulIP & ulMask) != (ulGatewayIp & ulMask) )
    {
        DHCP_SERVER_DBG ( "The user's Ip %s assigned by ip pool not in the same subnet as the interface ip addrss %s ", str, str1 );
        return ERROR;
    }


    /* Initialize the DHCP OFFER packet header */
    /* Handle empty message, considering DHCP relay mode */
    p = (char *)osal_alloc( DHCP_MAX_LEN*sizeof(char) );
    if ( p == NULL )
    {
        DHCP_SERVER_DBG ( "dhcpserver_send_ack_packet: Malloc memory failure " );
        return ERROR;
    }
    memset ( (void *)p, 0, DHCP_MAX_LEN*sizeof(char) );

   /* DHCP fixed address assignment */
    pDhcpHead = (DHCP_HEAD_S *)p;
    pDhcpHead->ucop      = DHCP_REPLY;
    pDhcpHead->uchtype   = ETHERNET_HARDWARETYPE;
    pDhcpHead->uchlen    = ETHERNET_HARDWARELEN;
    pDhcpHead->uchops    = 0;
    pDhcpHead->ulxid     = pDhcp->ulxid;  
    pDhcpHead->ussecs    = pDhcp->ussecs;
    pDhcpHead->usflag    = pDhcp->usflag;  
    pDhcpHead->ulciaddr  = pDhcp->ulciaddr; 
    pDhcpHead->ulyiaddr  = osal_htonl(ulIP);   
    pDhcpHead->ulsiaddr  = osal_htonl( ulServerIp );
    pDhcpHead->ulgiaddr  = pDhcp->ulgiaddr;   
    memcpy(pDhcpHead->szchaddr, pDhcp->szchaddr, ETHERNET_HARDWARELEN);  

    /* Initialize dhcp option */

    /* Retrieve necessary request parameters */
    dhcpserver_record_client_request_param ( pDhcpOption, &ulReqParamFlag );

        /*magic cookie */
    pulMagicCookie = (ulong_t *)(char *)(pDhcpHead+1);
    ultemp = osal_htonl(MAGIC_COOKIE);
    memcpy(pulMagicCookie, &ultemp, sizeof(ulong_t));

    /*"DHCP message type" option*/
    pucDhcpOption = (uchar_t *)(pulMagicCookie+1);
    *pucDhcpOption++ = DHCP_MESSAGETYPE_CODE;    
    *pucDhcpOption++ = 1;    
    *pucDhcpOption++ = DHCP_ACK_TYPE;    

    /*"Ip subnet mask " option*/
    if ( ulReqParamFlag & DHCP_SUBNETMASK_CODE_FLAGBIT )
    {
        *pucDhcpOption++ = DHCP_SUBNETMASK_CODE;
        *pucDhcpOption++ = 4; 
        pulIp = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(ulMask);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption    = (uchar_t *)(pulIp + 1);
    }

    /*"Router Ip " option */
    if (ulReqParamFlag & DHCP_ROUTERIP_CODE_FLAGBIT )
    {
        *pucDhcpOption++ = DHCP_ROUTERIP_CODE;
        *pucDhcpOption++ = 4;  
        pulIp   = (ulong_t *)pucDhcpOption;
        ultemp = (ulRouterIp != 0) ? osal_htonl(ulRouterIp) : osal_htonl(ulGatewayIp);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption    = (uchar_t *)(pulIp + 1);
    }

   /* The 'DNS IP' option is currently in use */
    if (ulReqParamFlag & DHCP_DNSIP_CODE_FLAGBIT )
    {
        *pucDhcpOption++  = DHCP_DNSIP_CODE;
        pucLength         = pucDhcpOption;
        *pucLength        = 0;
        pucDhcpOption++;
        pulIp    = (ulong_t *)pucDhcpOption;

        for (i = 0; i < IPPOOL_MAX_DNS_NUM; i++)
        {
        	#if 1
            if( (dhcprelay_check_ip_valid(ulDnsIp[i]) == OK) && (ulDnsIp[i] != 0) )
            {
                *pucLength += 4;
                ultemp = osal_htonl(ulDnsIp[i]);
                memcpy(pulIp, &ultemp, sizeof(ulong_t));
                pulIp++;
            }
            #endif
        }

        if(*pucLength == 0)
        {
            /* When the DNS IP option is not provided, you need to select the appropriate option. At this time, 
            pucDhcpOption points to a NULL pointer, so you must ensure that related operations are modified accordingly */
            pucDhcpOption -= 2;
            DHCP_SERVER_DBG("IP pool has no DNS configuration ");
        }
        else
        {
            pucDhcpOption = (uchar_t *)(pulIp);
        }
    }

    /*"Ip lease time" option */
    if (ulReqParamFlag & DHCP_IPLEASETIME_CODE_FLAGBIT )
    {
        *pucDhcpOption++ = DHCP_IPLEASETIME_CODE;
        *pucDhcpOption++ = 4;  
        pulIp   = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(ulLeaseTime);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption    = (uchar_t *)(pulIp + 1);

        /*"Renew time" option */
        if (ulReqParamFlag & DHCP_RENEWTIME_CODE_FLAGBIT )
        {
            *pucDhcpOption++ = DHCP_RENEWTIME_CODE;
            *pucDhcpOption++ = 4;  
            pulIp   = (ulong_t *)pucDhcpOption;
            ultemp = osal_htonl(ulLeaseTime/2);
            memcpy(pulIp, &ultemp, sizeof(ulong_t));
            pucDhcpOption    = (uchar_t *)(pulIp + 1);
        }

        /*"Rebind time" option */
        if (ulReqParamFlag & DHCP_REBINDTIME_CODE_FLAGBIT )
        {
            *pucDhcpOption++ = DHCP_REBINDTIME_CODE;
            *pucDhcpOption++ = 4;  
            pulIp   = (ulong_t *)pucDhcpOption;
            ultemp = osal_htonl(ulLeaseTime*7/8);
            memcpy(pulIp, &ultemp, sizeof(ulong_t));
            pucDhcpOption    = (uchar_t *)(pulIp + 1);
        }
    }
    else  
    {
        /*"Renew time" option */
        *pucDhcpOption++ = DHCP_RENEWTIME_CODE;
        *pucDhcpOption++ = 4;  
        pulIp   = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(DHCP_MAX_RENEW_TIME);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption    = (uchar_t *)(pulIp + 1);

        /*"Rebind time" option */
        *pucDhcpOption++ = DHCP_REBINDTIME_CODE;
        *pucDhcpOption++ = 4;  
        pulIp   = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(DHCP_MAX_REBIND_TIME);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption    = (uchar_t *)(pulIp + 1);

        /*"Ip lease time" option    */
        *pucDhcpOption++ = DHCP_IPLEASETIME_CODE;
        *pucDhcpOption++ = 4; 
        pulIp   = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(DHCP_MAX_LEASE_TIME);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption    = (uchar_t *)(pulIp + 1);
    }

    /*"Netbios name server" option  */
    if (ulReqParamFlag & DHCP_NBNS_CODE_FLAGBIT )
    {
        *pucDhcpOption++  = DHCP_NBNS_CODE;
        *pucDhcpOption++  = 8;  
        pulIp    = (ulong_t *)pucDhcpOption;
        ultemp = osal_htonl(ulPrimaryNBNSIp);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pulIp++;
        ultemp = osal_htonl(ulSecondNBNSIp);
        memcpy(pulIp, &ultemp, sizeof(ulong_t));
        pucDhcpOption     = (uchar_t *)(pulIp + 1);
    }

    /*"Domain name suffix" option  */
    if ((ulReqParamFlag & DHCP_DOMAINNAME_CODE_FLAGBIT) && strlen((char *)szDnsSuffixName) != 0 )
    {
        *pucDhcpOption++  = DHCP_DOMAINNAME_CODE;
        vlen = (strlen ( (char *)szDnsSuffixName )) +1;
        *pucDhcpOption++  = (uchar_t)vlen;  
        memcpy ( pucDhcpOption, szDnsSuffixName, vlen );
        pucDhcpOption += vlen;
    }

    /*"dhcp server ip" option*/
    *pucDhcpOption++ = DHCP_SERVERIP_CODE;
    *pucDhcpOption++ = 4;  
    pulIp  = (ulong_t *)pucDhcpOption;
    ultemp = osal_htonl(ulServerIp);
    memcpy(pulIp, &ultemp, sizeof(ulong_t));
    pucDhcpOption    = (uchar_t *)(pulIp + 1);

    /*option43*/
    if ( (ulReqParamFlag & DHCP_OPTION43_CODE_FLAGBIT) && option43.ucValueLen )
    {
        *pucDhcpOption++  = DHCP_OPTION43_CODE;
        *pucDhcpOption++  = option43.ucValueLen;
        memcpy(pucDhcpOption, option43.szValue, option43.ucValueLen);
        pucDhcpOption += option43.ucValueLen;
    }

   /*option66: TFTP server name*/
    if (ulReqParamFlag & DHCP_TFTPSERVER_NAME_CODE_FLAGBIT)
    {
        memset(ucoption66, '\0', sizeof(ucoption66));
        if (IP_SUCCESS == IpPool_GetParameter_tftp(ulGatewayIp, (uchar_t*)ucoption66, NULL))
        {
            vlen = strlen((char*)ucoption66);
            if (vlen > 0)
            {
                *pucDhcpOption++ = DHCP_TFTPSERVER_NAME_CODE;
                *pucDhcpOption++  = (uchar_t)vlen; 
                memcpy(pucDhcpOption, ucoption66, vlen);
                pucDhcpOption += vlen;
            }
        }
    }

    /*option67: Bootfile name*/
    if (ulReqParamFlag & DHCP_BOOTFILE_NAME_CODE_FLAGBIT)
    {
        memset(ucoption67, '\0', sizeof(ucoption67));
        if (IP_SUCCESS == IpPool_GetParameter_tftp(ulGatewayIp, NULL, (uchar_t*)ucoption67))
        {
            vlen = strlen((char *)ucoption67);
            if (vlen > 0)
            {
                memcpy(pDhcpHead->szfile, ucoption67, vlen);
                *pucDhcpOption++  = DHCP_BOOTFILE_NAME_CODE;
                *pucDhcpOption++  = (uchar_t)vlen; 
                memcpy(pucDhcpOption, ucoption67, vlen);
                pucDhcpOption += vlen;
            }
        }
    }

    /* option 60 */
    if(pDhcpOption->pucOption60)
    {
        dhcpserver_option60_process(&pucDhcpOption, pDhcpOption,if_Para);
    }
    *pucDhcpOption++   = ENDOFF_OPTIONS;
    ulDhcpLen = pucDhcpOption -(uchar_t *)(pDhcpHead);
    /* The accessed DHCP option is smaller than the minimum DHCP option length,
     indicating this is a malformed or minimal DHCP option */
    if ( ulDhcpLen < DHCP_MIN_LEN )
    {
        ulDhcpLen = DHCP_MIN_LEN;
    }

    /*Sending DHCPACK packet*/
    if ( OK != dhcprelayRxFromLocalserver(pDhcpHead, ulDhcpLen, if_Para) )
    {
        /*DHCP failure*/
        osal_free(p);
        DHCP_SERVER_DBG ( "Dhcp Server send dhcprelayRxFromLocalserver(DHCPACK) msg failed " );
        return ERROR;
    }

    osal_free(p);
    return OK;
}


/*******************************************************************************
 Function: STATUS dhcpserver_send_nck_packet
Description: Sends a DHCPNAK (Negative Acknowledgement) packet. 
             This occurs when the DHCP server rejects a DHCP request due to errors 
             such as invalid requests or when operating in DHCP relay mode.
Input: pDhcp: Pointer to the DHCP packet header structure.
       pDhcpOption: Pointer to the DHCP option structure.
       ulIP: The IP address involved in the request.
       ulServerIp: The IP address of the responding DHCP server.
       *DHCP_INTERFACE_PARA if_Para: Interface parameters information.
Return: Success OK, failure ERROR.
Remarks: Used for handling negative acknowledgments in DHCP communication.
*******************************************************************************/
STATUS dhcpserver_send_nck_packet( DHCP_HEAD_S *pDhcp, DHCP_OPTION_S *pDhcpOption, ulong_t ulIP, ulong_t ulServerIp,DHCP_INTERFACE_PARA *if_Para)
{
    DHCP_HEAD_S    *pDhcpHead;
    STATUS rc = OK;
    ulong_t ulGatewayIp = 0;
    ulong_t ulMask= 0;
    ulong_t ulRouterIp = 0;
    ulong_t ulDnsIp[IPPOOL_MAX_DNS_NUM] = {0};
    ulong_t ulPrimaryNBNSIp = 0;
    ulong_t ulSecondNBNSIp = 0;
    ulong_t ulLeaseTime = 0;
    uchar_t *pucDhcpOption = NULL;
    ulong_t *pulMagicCookie = NULL;
    ulong_t *pulIp = NULL;
    ulong_t ulDhcpLen = 0;
    char *p = NULL;
    uchar_t szDnsSuffixName[IPPOOL_MAX_DNS_SUFFIX_NAME];
    ulong_t ultemp = 0;

    if ( pDhcp == NULL || pDhcpOption == NULL )
    {
        DHCP_SERVER_DBG ( "The pDhcp or pDhcpOption is invalid parameter " );
        return ERROR;
    }
    /* The DHCPNAK packet needs special handling, because the IP address is set to 0 */
    /* Retrieve the address from the interface parameter */
    ulGatewayIp = osal_ntohl(pDhcp->ulgiaddr);
    memset ( szDnsSuffixName, 0, sizeof(szDnsSuffixName) );
    rc = IpPool_GetParameter( ulGatewayIp, &ulMask, ulDnsIp,
                                                &ulPrimaryNBNSIp, &ulSecondNBNSIp, &ulLeaseTime, szDnsSuffixName, &ulRouterIp);
    if ( rc != OK )
    {
        DHCP_SERVER_DBG ( "Get ip pool common parameter failed " );
    }


    /* Initialize DHCPOFFER packet header */
    /* Handle empty message, considering DHCP relay mode */
    p = (char *)osal_alloc( DHCP_MAX_LEN*sizeof(char) );
    if ( p == NULL )
    {
        DHCP_SERVER_DBG ( "dhcpserver_send_nck_packet: Malloc memory failure " );
        return ERROR;
    }
    memset ( (void *)p, 0, DHCP_MAX_LEN*sizeof(char) );

    /*DHCP fixed address assignment*/
    pDhcpHead = (DHCP_HEAD_S *)p;
    pDhcpHead->ucop      = DHCP_REPLY;
    pDhcpHead->uchtype   = ETHERNET_HARDWARETYPE;
    pDhcpHead->uchlen    = ETHERNET_HARDWARELEN;
    pDhcpHead->uchops    = 0;
    pDhcpHead->ulxid     = pDhcp->ulxid; /* Acquire DHCP transaction ID (xid) */
    pDhcpHead->ussecs    = pDhcp->ussecs;
    pDhcpHead->usflag    = pDhcp->usflag;     
    pDhcpHead->ulciaddr  = pDhcp->ulciaddr; 
    pDhcpHead->ulyiaddr  = osal_htonl(0);   
    pDhcpHead->ulsiaddr  = osal_htonl( 0 );
    pDhcpHead->ulgiaddr  = pDhcp->ulgiaddr;   
    memcpy(pDhcpHead->szchaddr, pDhcp->szchaddr, ETHERNET_HARDWARELEN);   

    /* Initialize DHCP option */
    /* DHCPNAK option contains only the server IP address */

    /* Fixed DHCP address assignment */
    /*magic cookie */
    pulMagicCookie = (ulong_t *)(char *)(pDhcpHead+1);
    ultemp = osal_htonl(MAGIC_COOKIE);
    memcpy(pulMagicCookie, &ultemp, sizeof(ulong_t));

    /*"DHCP message type" option*/
    pucDhcpOption = (uchar_t *)(pulMagicCookie+1);
    *pucDhcpOption++ = DHCP_MESSAGETYPE_CODE;    
    *pucDhcpOption++ = 1;   
    *pucDhcpOption++ = DHCP_NAK_TYPE;  

    /*"dhcp server ip" option*/
    *pucDhcpOption++ = DHCP_SERVERIP_CODE;
    *pucDhcpOption++ = 4; 
    pulIp  = (ulong_t *)pucDhcpOption;
    ultemp = osal_htonl(ulServerIp);
    memcpy(pulIp, &ultemp, sizeof(ulong_t));
    pucDhcpOption    = (uchar_t *)(pulIp + 1);

    /* option 60 */
    if(pDhcpOption->pucOption60)
    {
        dhcpserver_option60_process(&pucDhcpOption, pDhcpOption,if_Para);
    }

       *pucDhcpOption++   = ENDOFF_OPTIONS;

    

   /* Initialize DHCP option
    DHCPNAK option contains only server IP address
    Fixed DHCP address assignment*/

    ulDhcpLen = pucDhcpOption -(uchar_t *)(pDhcpHead);
   /* The accessed DHCP option is smaller than the minimum DHCP option length,
    indicating this is a malformed or minimal DHCP option */
    if ( ulDhcpLen < DHCP_MIN_LEN )
    {
        ulDhcpLen = DHCP_MIN_LEN;
    }

    /*Send DHCPNAK packet*/
    if ( OK != dhcprelayRxFromLocalserver(pDhcpHead, ulDhcpLen, if_Para) )
    {
        osal_free(p);
        dhcpserver_debug_printf ( "\r\n Dhcp Server send dhcprelayRxFromLocalserver(DHCPNAK) msg failed " );
        return ERROR;
    }

    osal_free(p);
    return OK;
}


/*******************************************************************************
    Function: void dhcpserver_show_hash
    Description: Displays the current state of the hash table
    Input: None
    Output: None
    Return: OK on success, ERROR on failure
*******************************************************************************/
void dhcpserver_show_hash(void)
{
    ulong_t i = 0;
    ulong_t j = 0;
    MACHASH_S *pHashData;
    uchar_t buf[20];

    /*Sequentially output hash table*/
    DHCP_SERVER_DBG( "User MAC            VID    IP             " );
    for(i=0;i<(g_stDhcpHashCB.ulListNum+g_stDhcpHashCB.ulFreeNum);i++)
    {
        if ( (g_stDhcpHashCB.pList[i].pData != NULL) && (g_stDhcpHashCB.pList[i].ulFlag == HASH_NODE_IN_USED ))
        {
            pHashData = (MACHASH_S *)g_stDhcpHashCB.pList[i].pData;
            /*mac addr */
            macToStr ( (uchar_t *)&(pHashData->ucUserMac[0]), buf );
            DHCP_SERVER_DBG ( "%-20s", buf );
            /*vid */
            DHCP_SERVER_DBG ( "%-7d", pHashData->usVlanId );
            /*IP*/
            //dhcp_ipaddr_to_str ( pHashData->ulIpAddr, buf );
            DHCP_SERVER_DBG ( "%-15s", buf );
            DHCP_SERVER_DBG ( " " );

            j++;
            if ( j %22 == 0 )
            {
                DHCP_SERVER_DBG ( "User MAC            VID    IP             " );
            }
        }
    }
}


/*******************************************************************************
  Function: STATUS dhcpserver_romovehash_by_ip
  Description:This function deletes an IP address from the DHCP IP pool hash table. 
  It scans the hash table elements sequentially to find the element corresponding 
  to the given IP address and removes it from the hash. This deletes the corresponding
  IP entry from the hash table.
  Input: ulong_t ulIP ¡ª The IP address to be removed from the hash table.
  output Status code
  Return : OK if successful, ERROR if failed
*******************************************************************************/
STATUS dhcpserver_romovehash_by_ip ( ulong_t ulIP )
{
    ulong_t i;
    MACHASH_S *pHashData;
    uchar_t mac[20];
    uchar_t strIP[20];


    /*Sequentially output hash table*/
    for(i=0;i<(g_stDhcpHashCB.ulListNum+g_stDhcpHashCB.ulFreeNum);i++)
    {
        if ( g_stDhcpHashCB.pList[i].pData != NULL )
        {
            pHashData = (MACHASH_S *)g_stDhcpHashCB.pList[i].pData;
            if ( pHashData->ulIpAddr == ulIP )
            {
                macToStr ( pHashData->ucUserMac, mac );
                //dhcp_ipaddr_to_str ( ulIP, strIP );
                DHCP_SERVER_DBG ( "Delete hash entry %s-->%s ", mac, strIP );
                HashDelete ( &g_stDhcpHashCB, (void *)pHashData );
                break;
            }
        }
    }

    return OK;
}


/*******************************************************************************
Function: STATUS dhcpserver_romovehash_by_ippool
Description: This function deletes IP addresses from the DHCP IP pool hash table. 
It sequentially scans the hash table entries within the specified IP pool and 
removes those entries matching the IP addresses in the pool, effectively cleaning
up the hash entries associated with that IP pool.
Input: ulong_t ippool_gate: The gateway address of the IP pool.
       ulong_t ippool_mask: The subnet mask of the IP pool.
Output: Status code
Return: OK is successful, ERROR otherwise.
*******************************************************************************/
STATUS dhcpserver_romovehash_by_ippool(ulong_t ippool_gate, ulong_t ippool_mask)
{
    ulong_t i = 0;
    ulong_t j = 0;
    ulong_t ulIp;
    MACHASH_S *pHashData;
    uchar_t mac[20];
    uchar_t strIP[20];

    /*Sequentially output hash table*/
    for(j=0; j < 2; j++)
        for(i=0;i<(g_stDhcpHashCB.ulListNum+g_stDhcpHashCB.ulFreeNum);i++)
        {
            if ( g_stDhcpHashCB.pList[i].pData != NULL )
            {
                pHashData = (MACHASH_S *)g_stDhcpHashCB.pList[i].pData;
                ulIp = pHashData->ulIpAddr;
                /* When removing an IP address from the IP pool, 
                if the IP address is within a subnet, it should simultaneously delete
                 the corresponding hash entry */
                if ( (ulIp & ippool_mask) == (ippool_gate & ippool_mask) )
                {
                    macToStr ( pHashData->ucUserMac, mac );
                    //dhcp_ipaddr_to_str ( ulIp, strIP );
                    DHCP_SERVER_DBG ( "Delete hash entry %s-->%s ", mac, strIP );
                    HashDelete ( &g_stDhcpHashCB, (void *)pHashData );
                }
            }
        }

    return OK;
}


/*******************************************************************************
  Function: STATUS dhcpserver_romovehash_by_ippool_section
  Description: This function deletes IP addresses in a specified IP address range 
               while preventing any running tasks related to these addresses from being affected or corrupted
  Input: ulong_t startIp - the starting IP address
         ulong_t endIp -  the ending IP address
  Output: value
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcpserver_romovehash_by_ippool_section(ulong_t startIp, ulong_t endIp)
{
    ulong_t i = 0;
    ulong_t j = 0;
    MACHASH_S *pHashData;
    uchar_t mac[20];
    uchar_t strIP[20];

    /* Delete hash nodes in order*/
    for(j=0; j < 2; j++)
        for(i=0;i<(g_stDhcpHashCB.ulListNum+g_stDhcpHashCB.ulFreeNum);i++)
        {
            if ( g_stDhcpHashCB.pList[i].pData != NULL )
            {
                pHashData = (MACHASH_S *)g_stDhcpHashCB.pList[i].pData;

                if ( (startIp <= pHashData->ulIpAddr) && (pHashData->ulIpAddr <= endIp) )
                {
                      macToStr ( pHashData->ucUserMac, mac );
                      //dhcp_ipaddr_to_str ( pHashData->ulIpAddr, strIP );
                      DHCP_SERVER_DBG ( "Delete hash entry %s-->%s ", mac, strIP );
                      HashDelete ( &g_stDhcpHashCB, (void *)pHashData );
                }
            }
        }

    return OK;
}

/*******************************************************************************
  Function: STATUS ipPoolStartup(  )
  Description: Initialize IP address pool module
  Input: None
  Output: value
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS ipPoolStartup(void)
{
    STATUS      rc;
    rc = InitIpPoolAll();
    if(rc != SYS_ERR_OK)
        DHCP_SERVER_DBG( "  Init ip pool fail " ) ;

    return( rc );
}

STATUS dhcpserver_option60_process( uchar_t **ppucDhcpOption, DHCP_OPTION_S *pDhcpOption,DHCP_INTERFACE_PARA *if_Para)
{
    int i = 0;
    int matchlen = 0;
    ulong_t ulIfNo = if_Para->dhcp_sw;
    int foundi = -1;

    if (pDhcpOption->pucOption60)
    {
        for (i = 0; i < DHCPINTERFACEMAXNUM; i++)
        {
            if (dhcpOption60MatchEntry[ulIfNo-1][i].matchType == OPTION60_EQUALS)
            {
                if (dhcpOption60MatchEntry[ulIfNo-1][i].contentlen == pDhcpOption->ucOption60Len)
                {
                    if (0 == memcmp(pDhcpOption->pucOption60, dhcpOption60MatchEntry[ulIfNo-1][i].matchContent, dhcpOption60MatchEntry[ulIfNo-1][i].contentlen ))
                    {
                        foundi = i;
                        break;
                    }
                }
            }
            else if (dhcpOption60MatchEntry[ulIfNo-1][i].matchType == OPTION60_STARTS_WITH)
            {
                if (dhcpOption60MatchEntry[ulIfNo-1][i].contentlen <= pDhcpOption->ucOption60Len)
                {
                    if (0 == memcmp(pDhcpOption->pucOption60, dhcpOption60MatchEntry[ulIfNo-1][i].matchContent, dhcpOption60MatchEntry[ulIfNo-1][i].contentlen))
                    {
                        if (dhcpOption60MatchEntry[ulIfNo-1][i].contentlen > matchlen)
                        {
                            foundi = i;
                            matchlen = dhcpOption60MatchEntry[ulIfNo-1][i].contentlen;
                        }
                    }
                }
            }
        }

        /* Handle the client request to obtain option60, i.e., client option60 */
        if ((foundi >= 0) && (dhcpOption60MatchEntry[ulIfNo-1][foundi].replyLen > 0))
        {
            **ppucDhcpOption = DHCP_OPTION60_CODE;
            *ppucDhcpOption += 1;
            **ppucDhcpOption = dhcpOption60MatchEntry[ulIfNo-1][foundi].replyLen;
            *ppucDhcpOption += 1;
            memcpy(*ppucDhcpOption, dhcpOption60MatchEntry[ulIfNo-1][foundi].replyContent, dhcpOption60MatchEntry[ulIfNo-1][foundi].replyLen);
            *ppucDhcpOption += dhcpOption60MatchEntry[ulIfNo-1][foundi].replyLen;
        }
    }

    return OK;
}

STATUS dhcpserver_sendClientTrap(uchar_t *pucUserMac, ulong_t ulUserIp, ushort_t usUserVlan)
{
#if 0
    MACHASH_S stHashData;
    MACHASH_S *pHashData;
    vbList_t *vbList = NULL;
    //ulong_t   trapOid[] = {1, 3, 6, 1, 4, 1, OEMOID_OEM, 1, 2, 5, 5, 8, 2};  /* dhcpRelaySvrClientTrap */
    ulong_t   idxBuf[6];
    STATUS    rc = ERROR;
    int       i;

    if (!g_bDhcpServerSendTrap || (NULL == pucUserMac) || (0 == ulUserIp))
    {
        return OK;
    }

    /* Verify whether the client IP address belongs to the specified server function*/
    memcpy(stHashData.ucUserMac, pucUserMac, 6);
    pHashData = HashSearch(&g_stDhcpHashCB, (void *)&stHashData);
    if ((NULL == pHashData) || (ulUserIp != pHashData->ulIpAddr) || (usUserVlan != pHashData->usVlanId))
    {
        return ERROR;
    }

    /* Allocate memory for varbind list */
    vbList = envoyNotifyVBLAlloc(3);
    if (NULL != vbList)
    {
        for (i = 0; i < 6; i++)
        {
            idxBuf[i] = pucUserMac[i];
        }
        rc  = envoyNotifyVBLVbAdd(vbList, (uchar_t *)"gbnL3DhcpRelay:dhcpRelayUserMac", idxBuf, 6);
        rc |= envoyNotifyVBLVbAdd(vbList, (uchar_t *)"gbnL3DhcpRelay:dhcpRelayVlanId", idxBuf, 6);
        rc |= envoyNotifyVBLVbAdd(vbList, (uchar_t *)"gbnL3DhcpRelay:dhcpRelayIpAddress", idxBuf, 6);
        if (OK != rc)
        {
            vosPrintf("\r\ndhcpserver_sendClientTrap: Failed to add variable.\r\n");
        }
        else
        {
           // rc = envoyNotifySend((uchar_t *)"gbn", trapOid, sizeof(trapOid) / sizeof(ulong_t), vbList);
            if (OK != rc)
            {
                vosPrintf("\r\ndhcpserver_sendClientTrap: Failed to send notify.\r\n");
            }
        }

        /* clean up should be done by sendTtn */
        envoyNotifyVBLClean(&vbList);
    }

    return rc;
#endif
	return OK;

}

int dhcpserverIpBindSwitch(BOOL bEnable)
{
    gulIpBindSwitch = bEnable;
    return OK;
}

int dhcpseverIpBindGet(BOOL *pbEnable, BOOL *pbUnBoundAssign)
{
    *pbEnable = gulIpBindSwitch;
    *pbUnBoundAssign = unboundAssign;
    return OK;
}

int dhcpseverUnBoundAssign(BOOL bEnable)
{
    unboundAssign = bEnable;
    return OK;
}

#endif /* GBN_INCLUDE_IPPOOL */
#endif /*GBN_INCLUDE_DHCPRELAYSERVER*/

