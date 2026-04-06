/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  FileName:  DhcpHash.c
  Autor: Kniazkov Andrew
  Version:1.0.0        
  Date:2024/04/09
  Description: This file defines all functions in the DHCP RELAY module 
               and implements DHCP RELAY functionaly
  Other notes: None
********************************************************************************/

#define GBN_INCLUDE_DHCPRELAYSERVER
#ifdef GBN_INCLUDE_DHCPRELAYSERVER
#include <common/sys_def.h>
#include "ip_pool.h"
#include "dhcps.h"
#include "dhcpr.h"
#include "hashCommon.h"
#include "dhcpHash.h"
#include "dhcp.h"
#include "dhcprelayserver.h"
#include "dhcpSocket.h"
#include "libsal/sal_socket.h"
#include "libsal/sal_dhcp_server.h"


dhcpOption82Policy_t dhcpOption82Policy =
{
    FALSE,                    /*default ptioon82 disable*/
    FALSE,                    /*By default the non-stacked systems Option82 circuit ID does not include the device*/
    OPTION82STRATEGY_REPLACE, /*The default policy is replace*/
    OPTION82FORMAT_NORMAL,    /*The default format is the standart format*/
    OPTION82INFOFORMAT_HEX,   /*Default format is hex*/
    "",                    /*The detailed format node identifier defaults to the switch mac*/
    "",              /*If default emty, fill with standart format content when populating*/
    ""              /*Defaults to emty; when filling, use the standart format content*/
};

dhcpOption82Policy_port_t dhcpOption82Policy_port[SYS_LOGICPORT_NUM_MAX];
dhcpOption82Policy_vlan_t dhcpOption82Policy_vlan[DHCP_OPTION82_POIICY_VLAN_NUM];
//AVL_NODE *vlanPolicyAVL = NULL;
//LIST vlanPolicyListFree;

dhcpOption60Match_t dhcpOption60MatchEntry[DHCP_MAX_INTERFACE_NUM][DHCPINTERFACEMAXNUM];

ulong_t g_bDhcpRelayEnable = FALSE;

uchar_t g_bDhcpRelaySysMac[6] = {0};
ulong_t g_bDhcpRelaySysOid = 0;
char g_bDhcpRelayHostname[32 + 1] = {0};;

/*Global variable declaration*/

/* Is there a switch in dhcprelay to hide the DHCP server address?
    Default is FALSE, not hidden, clients can see the real server IP.
    When set TRUE, dhcp will use its own interface IP as the server IP.
*/
BOOL g_bDhcpHideServerIp = FALSE;
int dhcpMaxHops = DHCP_MAX_HOPS;
DHCP_SERVER_S    g_dhcpgroup[DHCP_MAX_GROUP_NUM];    /*dhcp server group configuration table empty*/
VLANIfToDHCPS_S  g_dhcpInterface[DHCP_MAX_INTERFACE_NUM]; /*mapping between VLAN interfaces and DHCP server groupes*/

BOOL gbDHCPRelayPortEnable[SYS_LOGICPORT_NUM_MAX] = {0};

/*Function declaration*/
#ifdef GBN_INCLUDE_DHCPSNOOPING
extern int dhcpsnoopingServerAck(uchar_t *mac, ulong_t ulVid, ulong_t ulClientIp, ulong_t LeaseTime);
extern int dhcpsnoopingServerNak(uchar_t *mac, ulong_t ulVid, ulong_t ulxid);
#endif

#define DEBUGIPV4(ipv4) \
do {    					\
		char ipStr[100];    \
		sys_util_ip2Str(ipv4, ipStr);\
		DHCP_SERVER_DBG("ipaddr:%s", ipStr);\
}while(0)

BOOL dhcprelay_checkLocalIp(ulong_t ulIpAddress, ulong_t *pulSw)
{
#if 1
	
#ifndef CONFIG_SYS_L3_ARCHITECTURE
	if (TRUE == sal_l3_Mgmt_IPv4_ip_equal(ulIpAddress))
	{
		*pulSw = SYS_L3_INTFTYPE_MGMTVLAN;
		return TRUE;
	}
#endif
	if (TRUE == sal_l3_intfIPv4_ip_equal(ulIpAddress))
	{
		*pulSw = SYS_L3_INTFTYPE_VLAN;
		return TRUE;
	}

#if 0
    if ( OK == l3ifSwGetSwByIp(ulIpAddress, pulSw) )
    {
        return TRUE;
    }
#endif
    #ifdef GBN_INCLUDE_VRRP
    /*Determine whether an IP address is a local VRRP virtual IP address*/
    if ( TRUE == vrrpCheckDstIsVip(ulIpAddress, pulSw) )
    {
        return TRUE;
    }
    #endif
#endif
    return FALSE;
}

STATUS dhcprelay_get_ifaddr_by_vid(ulong_t ulVid, ulong_t *pulSw, ulong_t *ifAddr)
{
#if 0
    int                      rc;
    long                     sw;
    ulong_t                  ulValue;

    sw = getSWByVLAN(ulVid);
    if ( -1 == sw )
    {
        return ERROR;
    }

    /*Get the main IP address of the switch*/
    rc = l3ifSwPrimIpGet(sw, &ulValue);
    if ( OK != rc )
    {
        return ERROR;
    }

    *pulSw = sw;
    *ifAddr = ulValue;
 #endif
    return OK;
}


int dhcp_split(char **Sour, char *Dest, char flag, int Len)
{
  register char *temp;
  register int i=0;

  temp=*Sour;
  for ( ; ; )
  {
      if (*temp==flag)
      {
          *Sour=temp+1;
          *Dest='\0';
          return 1;
      }
      else
      if (!(*temp))
      {
          *Sour=temp;
          *Dest='\0';
          return 0;
      }
      else
      if (i==Len)
      {
          *Sour=temp;
          *Dest='\0';
          return 2;
      }
      else
      {
          *Dest++=*temp++;
          i++;
      }
  }
}

STATUS dhcp_str_to_ipaddr(char *string, ulong_t *ulRet)
{
    char stemp[300];
    ulong_t a,b,c,d;

    if (dhcp_split(&string,stemp,'.',300)!=1)
        return ERROR;
    if (!*stemp)
        return ERROR;
    a = (ulong_t)atol(stemp);
    if (a>255)
        return ERROR;
    if (dhcp_split(&string,stemp,'.',300)!=1)
        return ERROR;
    if (!*stemp)
        return ERROR;
    b = (ulong_t)atol(stemp);
    if (b>255)
        return ERROR;
    if (dhcp_split(&string,stemp,'.',300)!=1)
        return ERROR;
    if (!*stemp)
        return ERROR;
    c = (ulong_t)atol(stemp);
    if (c>255)
        return ERROR;
    if (dhcp_split(&string,stemp,'\0',300)!=1)
        return ERROR;
    if (!*stemp)
        return ERROR;
    d = (ulong_t)atol(stemp);
    if (d>255)
        return ERROR;

    *ulRet = ((a << 24) | (b << 16) | (c << 8) | d);
    return OK;
}

void dhcprelay_option82_init()
{
    int i = 0;
    for (i = 0; i < SYS_LOGICPORT_NUM_MAX; i++)
    {
        dhcpOption82Policy_port[i].option82Enabled = TRUE;
        dhcpOption82Policy_port[i].opion82Strategy = OPTION82STRATEGY_REPLACE;
        dhcpOption82Policy_port[i].option82CircuitId[0] = 0;
        dhcpOption82Policy_port[i].option82RemoteId[0] = 0;
        dhcpOption82Policy_port[i].option82SubOpt9[0] = 0;
    }
#if 0
    lstInit(&vlanPolicyListFree);
    for (i = 0; i < DHCP_OPTION82_POIICY_VLAN_NUM; i++)
    {
        memset(dhcpOption82Policy_vlan + i,0,sizeof(dhcpOption82Policy_vlan_t));
        lstAdd(&vlanPolicyListFree, (NODE *)&dhcpOption82Policy_vlan[i]);
    }
#endif
}

/*******************************************************************************
  Function: void dhcprelay_init_mib_table(void)
  Description: Initialize the MIB variable table for the DHCP RELAY module
  Input: None
  Output: None
  Return: None
  *******************************************************************************/
void dhcprelay_init_mib_table(void)
{
    int i = 0, j = 0;

    for ( i = 0; i < DHCP_MAX_GROUP_NUM; i++ )
    {
        g_dhcpgroup[i].rowstatus = DHCP_ENABLE;
        g_dhcpgroup[i].groupNo = i+1;
        g_dhcpgroup[i].serverIp = 0;
    }

    for ( i = 0; i < DHCP_MAX_INTERFACE_NUM; i++ )
    {
	    g_dhcpInterface[i].vid = 0;
        for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
        {
            g_dhcpInterface[i].groupNo[j] = DHCPEMPTYGROUP;
        }
    }

    return;
}


/*******************************************************************************
  Function: void dhcprelay_init(void)
  Description: Initialize the DHCP RELAY module
               Create the HASH table
               Create and start the main message processing task for the DHCP Relay module
               Create message queue for the DHCP Relay module
               Create and start the Socket receiving task
               Retrieve MIB variable values related to the DHCP Relay module from NVM
  Input: None
  Output: None
  Return: None
 *******************************************************************************/
int dhcprelay_init(void)
{
    int                      i;

    g_bDhcpRelayEnable = FALSE;    /*��ʼ��ģ�鿪�عر�*/

    dhcprelay_init_mib_table ( );
    dhcprelay_option82_init ();

    for ( i = 0; i < SYS_LOGICPORT_NUM_MAX; i++ )
    {
        gbDHCPRelayPortEnable[i] = FALSE;
    }

    return OK;
}

#ifdef ISF_DHCPSNOOPING_DISTRIBUTE
int dhcprelay_send_to_master_wrap(SMD_MSG *pMsg, SMD_ARG args[], ulong_t ulArgsNum)
{
    ulong_t ulDhcpLen;
    DHCP_MSG stMsg;
    char *szRecvBuffer;

    if ( 4 != ulArgsNum )
    {
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:ulArgsNum %ld error", __func__, ulArgsNum);
        return ERROR;
    }

    ulDhcpLen = *((ulong_t *)args[0].pBuf);
    ulDhcpLen = (ulDhcpLen < DHCP_MAX_LEN) ? ulDhcpLen : DHCP_MAX_LEN;

    szRecvBuffer =(char *) malloc( DHCP_MAX_LEN*sizeof(char) );
    if (szRecvBuffer == NULL)
    {
        return ERROR;
    }

    stMsg.usType = DHCPRELAY_RX_SOCKETDATE;
    stMsg.pData = szRecvBuffer;
    stMsg.ulLen = ulDhcpLen;
    stMsg.ulPara1 = *((ulong_t *)args[2].pBuf);;
    memcpy(&stMsg.ulPara4, args[3].pBuf, sizeof(DHCP_INTERFACE_PARA));
    memcpy(szRecvBuffer, args[1].pBuf, ulDhcpLen);

    if ( msgQSend ( g_ulQue_DHCPRELAY, (char * )&stMsg, sizeof(stMsg), NO_WAIT,MSG_PRI_NORMAL ) != 0 )
    {
        free(szRecvBuffer);
        return ERROR;
    }

    return OK;
}

STATUS dhcprelay_send_to_master( DHCP_HEAD_S *pDhcp, ulong_t ulDhcpLen, ulong_t ulIP,DHCP_INTERFACE_PARA *ifPara)
{
    int iRet;
    SMD_MSG msg;
    SMD_ARG args[4];
    uchar_t ucDevId;

    iRet = isf_stack_master_device_get(&ucDevId);
    if ( OK != iRet )
    {
        return ERROR;
    }

    smdMsgInit(&msg, ucDevId, ISF_DHCPREALY_PKT_TO_LOCALSERVER, SMD_GLOBAL);
    msg.ucNeedRet = SMD_MSG_RET_NOT;

    args[0].pBuf = &ulDhcpLen;
    args[0].usFlags = 0;
    args[0].usLen = sizeof(ulong_t);

    args[1].pBuf = pDhcp;
    args[1].usFlags = SMD_ARG_POINTER;
    args[1].usLen = ulDhcpLen;

    args[2].pBuf = &ulIP;
    args[2].usFlags = 0;
    args[2].usLen = sizeof(ulong_t);

    args[3].pBuf = ifPara;
    args[3].usFlags = SMD_ARG_POINTER;
    args[3].usLen = sizeof(DHCP_INTERFACE_PARA);

    iRet = smdMsgRequest(&msg, args, 4, SMD_TIMEOUT_DEFAULT);
    if ( 0 != iRet )
    {
        iRet = ERROR;
        isf_debug_printf(ISF_DEBUG_ERROR, "%s:request failed iRet = %d", __func__, iRet);
    }

    return iRet;
}
#endif

int dhcprelayRxSocket(BOOL bFromDhcpsnooping, DHCP_HEAD_S *pDhcp,
    ulong_t ulDhcpLen, ulong_t ulIP, DHCP_INTERFACE_PARA *ifPara)
{
    int                      iRet;
    uchar_t tmp[64];

    dhcp_ipaddr_to_str(ulIP, tmp);
    DHCP_SERVER_RX_DBG("DHCPRELAY: Receive message from socket, from ip=%s, from dhcpsnoping %d.",
        tmp, bFromDhcpsnooping);

    iRet = dhcprelay_prcess_socketdate(bFromDhcpsnooping, pDhcp, ulDhcpLen, ulIP, ifPara);

    return iRet;
}

int dhcprelayRxFromLocalserver(DHCP_HEAD_S *pDhcp, ulong_t ulDhcpLen, DHCP_INTERFACE_PARA *if_Para)
{
    DHCP_SERVER_RX_DBG ( "DHCPRELAY: Receive message from local server" );

    return dhcprelay_process_localserverdate(pDhcp, ulDhcpLen , if_Para);
}
#if 0
/*******************************************************************************
  Function: STATUS dhcp_show_packet
  Description: Prints the relevant content of received DHCP packet for debbuging use
  Input: DHCP_HEAD_S *pDhcp - pointer to the packet header
         ulong_t ulDhcpLen Legth of the packet (including header and option data)
  Output: None
  Return: success OK, Failure ERROR
 *******************************************************************************/
STATUS  dhcp_show_packet ( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, char *pSyslogHead )
{
    int    i;
    char szOut[300];
    char szTotalStr[500];
    DHCP_OPTION_S stDhcpOption;
    ulong_t ultempIP = 0;

    if(pDhcp == NULL)
    {
        //syslogSendInfo(MID_DHCP, DEBUGGING, pSyslogHead, "ShowPacket: Invalid parameter", "ShowPacket: Invalid parameter", FALSE, FALSE, NO_WAIT);
        return ERROR;
    }
    szTotalStr[0]='\0';
    strcat(szTotalStr, "\r\n packet contents is");

    sprintf(szOut,"\r\n \txid:%u",ntohl(pDhcp->ulxid));
    strcat(szTotalStr,szOut);
    sprintf(szOut,"\tSecs:%d",ntohs(pDhcp->ussecs));
    strcat(szTotalStr,szOut);
    sprintf(szOut,"\r\n \top_code:%d",pDhcp->ucop);
    strcat(szTotalStr,szOut);
    sprintf(szOut,"\tHardType:%d",pDhcp->uchtype);
    strcat(szTotalStr,szOut);
    sprintf(szOut,"\tHardLen:%d",pDhcp->uchlen);
    strcat(szTotalStr,szOut);
    strcat(szTotalStr, "\r\n \tchaddr:");
    for(i = 0; i < pDhcp->uchlen; i ++)
    {
        sprintf(szOut, "%02X", pDhcp->szchaddr[i]);
        strcat(szTotalStr, szOut);
        if(i != pDhcp->uchlen - 1)
        {
            strcat(szTotalStr, "-");
        }
    }

    sprintf(szOut,"\tHops:%d",pDhcp->uchops);
    strcat(szTotalStr,szOut);
    sprintf(szOut,"\tFlag:%u",ntohs(pDhcp->usflag)&0x8000);
    strcat(szTotalStr,szOut);

    dhcp_ipaddr_to_str(ntohl(pDhcp->ulciaddr), (uchar_t*)szOut);
    strcat(szTotalStr,"\r\n \tCiaddr:");
    strcat(szTotalStr,szOut);
    dhcp_ipaddr_to_str(ntohl(pDhcp->ulyiaddr), (uchar_t*)szOut);
    strcat(szTotalStr,"\tYiaddr:");
    strcat(szTotalStr,szOut);
    dhcp_ipaddr_to_str(ntohl(pDhcp->ulsiaddr), (uchar_t*)szOut);
    strcat(szTotalStr,"\r\n \tSiaddr:");
    strcat(szTotalStr,szOut);
    dhcp_ipaddr_to_str(ntohl(pDhcp->ulgiaddr), (uchar_t*)szOut);
    strcat(szTotalStr,"\tGiaddr:");
    strcat(szTotalStr,szOut);

    for (i = DHCP_SNAME_LENGTH - 1; i >= 0; i-- ) /*�ַ������ȼ��*/
    {
        if (pDhcp->szsname[i] == '\0')
        {
            break;
        }
    }
    if(i < 0)
    {
        vosPrintf("\r\n ShowPacket: 'Sname' field is not null terminated");
        return ERROR;
    }
    syslogSendInfo(MID_DHCP, DEBUGGING, pSyslogHead, szTotalStr, szTotalStr, FALSE, FALSE, NO_WAIT);

    sprintf(szTotalStr, "\r\n \tSname:%s", pDhcp->szsname);
    strcat(szTotalStr,"\r\n \tOptions:");

    /*Travers the "options" field of the DHCP packet and return the pointer to the relevant*/
    memset ((char *)&stDhcpOption, 0, sizeof( DHCP_OPTION_S ));
    if ( ERROR == dhcp_record_all_opt(pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption) )
    {
        return ERROR;
    }

    strcat(szTotalStr,"\r\n \tMSG TYPE:");
    if (stDhcpOption.pucDHCPMsgType != NULL)
    {
        sprintf(szOut,"%d",*(stDhcpOption.pucDHCPMsgType));
    }
    else
    {
        szOut[0] = '\0';
    }

    strcat(szTotalStr,szOut);

    strcat(szTotalStr,"\tSERVER IP:");
    if (stDhcpOption.pulDHCPServerIp != NULL)
    {
        memcpy(&ultempIP, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
        dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
    }
    else
    {
        szOut[0] = '\0';
    }
    strcat(szTotalStr,szOut);

    strcat(szTotalStr,"\tREQUEST IP:");
    if (stDhcpOption.pulReqIp != NULL)
    {
        memcpy(&ultempIP, stDhcpOption.pulReqIp, sizeof(ulong_t));
        dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
    }
    else
    {
        szOut[0] = '\0';
    }
    strcat(szTotalStr,szOut);

    strcat(szTotalStr,"\r\n \tREQUEST PARAM LIST:");
    if (stDhcpOption.ucReqParamListLen != 0)
    {
        for (i = 0; i < (stDhcpOption.ucReqParamListLen); i++)
        {
            sprintf(szOut,"0x%02X ",*((stDhcpOption.pucReqParamList)++));
            strcat(szTotalStr,szOut);
        }
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }
    syslogSendInfo(MID_DHCP, DEBUGGING, pSyslogHead, szTotalStr, szTotalStr, FALSE, FALSE, NO_WAIT);

#define _SHOW_MORE_DHCP_PKT_INFO
#ifdef _SHOW_MORE_DHCP_PKT_INFO
    sprintf(szTotalStr,"\r\n \tROUTER IP:");
    if (stDhcpOption.ucRouterIpLen != 0)
    {
        for (i = 0; i < (stDhcpOption.ucRouterIpLen >> 2); i++)
        {
            memcpy(&ultempIP, stDhcpOption.pulRouterIp + i, sizeof(ulong_t));
            dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
            strcat(szTotalStr,szOut);
            strcat(szTotalStr,"\t");
        }
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

    strcat(szTotalStr,"\r\n \tDNS IP:");
    if (stDhcpOption.ucDNSIpLen != 0)
    {
        for (i = 0; i < (stDhcpOption.ucDNSIpLen >> 2); i++)
        {
            memcpy(&ultempIP, stDhcpOption.pulDNSIp + i, sizeof(ulong_t));
            dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
            strcat(szTotalStr,szOut);
            strcat(szTotalStr,"\t");
        }
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

    strcat(szTotalStr,"\r\n \tNetbios Name Server IP:");
    if (stDhcpOption.pulNBNSIp != 0)
    {
        for (i = 0; i < (stDhcpOption.ucNBNSIpLen >> 2); i++)
        {
            memcpy(&ultempIP, stDhcpOption.pulNBNSIp + i, sizeof(ulong_t));
            dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
            strcat(szTotalStr,szOut);
            strcat(szTotalStr,"\t");
        }
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

    strcat(szTotalStr,"\r\n \tHOST NAME:");
    if (stDhcpOption.ucHostNameLen != 0)
    {
        memcpy(szOut, stDhcpOption.pucHostName, stDhcpOption.ucHostNameLen);
        szOut[stDhcpOption.ucHostNameLen] = '\0';
        strcat(szTotalStr, szOut);
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

    strcat(szTotalStr,"\r\n \tDOMAIN NAME:");
    if (stDhcpOption.ucDomainNameLen != 0)
    {
        memcpy(szOut, stDhcpOption.pucDomainName, stDhcpOption.ucDomainNameLen);
        szOut[stDhcpOption.ucDomainNameLen] = '\0';
        strcat(szTotalStr, szOut);
    }
    else
    {
        szOut[0] = '\0';
        strcat(szTotalStr,szOut);
    }

#endif

    strcat(szTotalStr,"\r\n \tSUBNETMASK:");
    if (stDhcpOption.pulSubnetMask != NULL)
    {
        memcpy(&ultempIP, stDhcpOption.pulSubnetMask, sizeof(ulong_t));
        dhcp_ipaddr_to_str(ntohl(ultempIP), (uchar_t*)szOut);
    }
    else
    {
        szOut[0] = '\0';
    }
    strcat(szTotalStr,szOut);

    syslogSendInfo(MID_DHCP, DEBUGGING, pSyslogHead, szTotalStr, szTotalStr, FALSE, FALSE, NO_WAIT);

    return  OK;
}
#endif

/*******************************************************************************
  Function: STATUS dhcprelay_prcess_socketdate
  Description: Processes messages received from socket tasks,where the messages
               contain DHCP protocol packets. This function handles the received
               protocol packets and calls the corresponding DHCP Relay module
                handling functions based on the packet type
  Input: DHCP_HEAD_S *pDhcp - pointer of the packet header
               ulong_t ulDhcpLen - packet length (including header and option data)
               ulong_t ulIP - source IP address of the packet
               DHCP_INTERFACE_PARA *ifPara - interface information
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcprelay_prcess_socketdate(BOOL bFromDhcpsnooping,
    DHCP_HEAD_S *pDhcp, ulong_t ulDhcpLen, ulong_t ulIP, DHCP_INTERFACE_PARA *ifPara)
{
    STATUS rc = OK;
    DHCP_OPTION_S stDhcpOpt;
    uchar_t  ucUserMac[6];

    /*If the dhcp relay module function disabled, return immediately*/
    if ( g_bDhcpRelayEnable == FALSE )
    {
        return ERROR;
    }

    if ( pDhcp == NULL )
    {
        DHCP_SERVER_RX_DBG ( "dhcprelay_prcess_socketdate: The packet pointer is null " );
        return ERROR;
    }

    /*Print DHCP message*/
    //if ( syslogModuleSwitch(MID_DHCP, DEBUGGING) )
    {
        dhcp_show_packet( pDhcp, ulDhcpLen, "dhcprelay");
    }

    /*ͨGeneral message inspection function*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }
    /*Identify message type*/
    rc = dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOpt );
    if ( rc != OK )
    {
        return ERROR;
    }

    if ( stDhcpOpt.pucDHCPMsgType == NULL )
    {
        DHCP_SERVER_RX_DBG ( "This packet is BOOTP packet " );
        if ( pDhcp->ucop == DHCP_REQUIRE )
        {
            dhcprelay_process_dhcpdiscover ( pDhcp, ulDhcpLen ,ifPara);
        }
        else
        {
            dhcprelay_process_dhcpoffer ( pDhcp, ulDhcpLen ,ifPara);
        }
        return OK;
    }

    if ( dhcpOption82Policy.option82Enabled && !bFromDhcpsnooping )
    {
        rc = dhcp_otion82_process(pDhcp, *(stDhcpOpt.pucDHCPMsgType),ulDhcpLen, &ulDhcpLen, ifPara);
        if( rc != OK )
        {
            return ERROR;
        }
    }

    switch ( *(stDhcpOpt.pucDHCPMsgType) )
    {
        case DHCP_DISCOVER_TYPE:
            DHCP_SERVER_DBG ( "Receive DHCPDISCOVER packet " );
            dhcprelay_process_dhcpdiscover ( pDhcp, ulDhcpLen, ifPara);
            break;

        case DHCP_OFFER_TYPE:
            DHCP_SERVER_DBG ( "Receive DHCPOFFER packet " );
            dhcprelay_process_dhcpoffer ( pDhcp, ulDhcpLen ,ifPara);
            break;

        case DHCP_REQUEST_TYPE:
            DHCP_SERVER_DBG ( "Receive DHCPREQUEST packet " );
            dhcprelay_process_dhcprequest ( pDhcp, ulDhcpLen, ifPara);
            break;

        case DHCP_DECLINE_TYPE:
            DHCP_SERVER_DBG ( "Receive DHCPDECLINE packet " );
            dhcprelay_process_dhcpreleasedecline ( pDhcp, ulDhcpLen,ifPara);
            break;

        case DHCP_ACK_TYPE:
            DHCP_SERVER_DBG ( "Receive DHCPACK packet " );
            dhcprelay_process_dhcpack ( pDhcp, ulDhcpLen,ifPara);
            break;

        case DHCP_NAK_TYPE:
            DHCP_SERVER_DBG ( "Receive DHCPNAK packet " );
            dhcprelay_process_dhcpnak ( pDhcp, ulDhcpLen ,ifPara);
            break;

        case DHCP_RELEASE_TYPE:
            DHCP_SERVER_DBG ( "Receive DHCPRELEASE packet " );
            dhcprelay_process_dhcpreleasedecline ( pDhcp, ulDhcpLen,ifPara);
            break;

        case DHCP_INFORM_TYPE:
            DHCP_SERVER_DBG( "Dhcp relay do not process the DHCPINFORM packet " );
            break;

        default:
            DHCP_SERVER_DBG ( "Unkown dhcp packet " );
            break;
    }
    return OK;
}

STATUS dhcprelay_packet_to_localserver(ulong_t ulType, ulong_t ulDhcpLen,
        ulong_t ulServerIp, ulong_t ulVid, DHCP_INTERFACE_PARA *ifPara, DHCP_HEAD_S *pDhcp)
{
		//DHCP_SERVER_DBG();
    return dhcpserverRxPkt(ulType, pDhcp, ulDhcpLen, ulVid, ulServerIp, ifPara);
}

/*******************************************************************************
  Function: STATUS dhcprelay_process_localserverdate
  Description: Processes messages sent from the DHCP Server task, 
               where the messages contain DHCP protocol packets. This function
               handles the received protocol packets and calls the corresponding 
               DHCP Relay module handling functions based on the packet type
  Input: DHCP_HEAD_S *pDhcp - pointer to the packet header
         ulong_t ulDhcpLen - packet length (including header and option data)
         DHCP_INTERFACE_PARA *ifPara - interface information
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcprelay_process_localserverdate ( DHCP_HEAD_S *pDhcp, ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara)
{
    STATUS rc;
    DHCP_OPTION_S stDhcpOpt;
    uchar_t  ucUserMac[6];

    /*If the dhcp relay module function disabled, return immediately*/
    if ( g_bDhcpRelayEnable == FALSE )
    {
        return ERROR;
    }

    if ( pDhcp == NULL )
    {
        DHCP_SERVER_DBG ( "dhcprelay_prcess_socketdate: The packet pointer is null " );
        return ERROR;
    }

    if ( ulDhcpLen < DHCP_HEAD_LEN )
    {
        DHCP_SERVER_DBG ( "dhcprelay_prcess_socketdate: The packet is too short " );
        return ERROR;
    }

    /*Print massege*/
    //if ( syslogModuleSwitch(MID_DHCP, DEBUGGING) )
    {
        dhcp_show_packet( pDhcp, ulDhcpLen, "dhcprelay" );
    }
    /*ͨGeneral message inspection function*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }

    /*Identify message type*/
    rc = dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOpt );
    if ( rc != OK )
    {
        return ERROR;
    }

    if ( stDhcpOpt.pucDHCPMsgType == NULL )
    {
        DHCP_SERVER_DBG ( "This packet is BOOTP packet " );
        dhcprelay_process_dhcpoffer ( pDhcp, ulDhcpLen,ifPara );
        return OK;
    }

    switch ( *(stDhcpOpt.pucDHCPMsgType) )
    {
        case DHCP_DISCOVER_TYPE:
            break;

        case DHCP_OFFER_TYPE:
            dhcprelay_process_dhcpoffer ( pDhcp, ulDhcpLen,ifPara);
            break;

        case DHCP_REQUEST_TYPE:
            break;

        case DHCP_DECLINE_TYPE:
            break;

        case DHCP_ACK_TYPE:
            dhcprelay_process_dhcpack ( pDhcp, ulDhcpLen ,ifPara);
            break;

        case DHCP_NAK_TYPE:
            dhcprelay_process_dhcpnak ( pDhcp, ulDhcpLen ,ifPara);
            break;

        case DHCP_RELEASE_TYPE:
            break;

        case DHCP_INFORM_TYPE:
            DHCP_SERVER_DBG( "Dhcp relay do not process the DHCPINFORM packet " );
            break;

        default:
            DHCP_SERVER_DBG ( "Unkown dhcp packet " );
            break;
    }

    return OK;
}

/*******************************************************************************
  Function: STATUS dhcprelay_process_dhcpdiscover
  Description: Processes DHCPDISCOVER packets sent from DHCP clients
  Input: DHCP_HEAD_S *pDhcp - pointer to the packet header
        ulong_t ulDhcpLen - packet length (including header and optuin data)
        ulong_t ulIP - source IP address of the packet
        DHCP_INTERFACE_PARA *ifPara - interface information
  Output: None
  Return: success OK, Failure ERROR
*******************************************************************************/
int32 dhcprelay_process_dhcpdiscover( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara)
{
    uchar_t  ucUserMac[6];
    uchar_t buf[128];
    ulong_t ulIfaddr;    /*IP address of the interface*/
    ulong_t ulIfNo;
    ulong_t ulDhcpServerIp[DHCPINTERFACEMAXNUM] = {0};    /*dhcp server address*/
    //ulong_t ulDhcpserverIp = 0;
    ulong_t ulVid;
    ulong_t ulPort;
    ulong_t ulSrcIp = 0;    /*the source IP address used in the IP header corresponding to the relay IP*/
    int32 rc = 0;
    int    i;
    DHCP_OPTION_S stDhcpOption;
    ulong_t dhcpOption60server = 0;

    pDhcp->uchops++;
   /*ͨGeneral message inspection function*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return SYS_ERR_FAILED;
    }
     /*Record message option fields*/
    dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption );

    if (NULL == ifPara)
    {
        return SYS_ERR_FAILED;
    }

    ulIfNo = ifPara->dhcp_sw;
    ulVid  = ifPara->dhcp_vid;
    ulPort = ifPara->dhcp_port;
    if ( (ulPort < SYS_LOGICPORT_NUM_MAX) && (gbDHCPRelayPortEnable[ulPort] == FALSE) )
    {
        DHCP_SERVER_RX_DBG( "dhcprelay_process_dhcpdiscover: port[%d] relay-enable is false.", ulPort);
        return SYS_ERR_FAILED;
    }
    rc = dhcprelay_get_ifaddr(ulIfNo, ulVid, &ulIfaddr);
    //if ( dhcprelay_get_ifaddr(ulIfNo, ulVid, &ulIfaddr) != SYS_ERR_OK)
    if(rc != SYS_ERR_OK)
    {
        DHCP_SERVER_RX_DBG( "dhcprelay_process_dhcpdiscover: The vlan %d has no interiface ip address", ulVid);
        return SYS_ERR_FAILED;
    }

    /*ͨObtain the corresponding DHCP server address through the interface*/
    memset(ulDhcpServerIp, 0, sizeof(ulDhcpServerIp));
    if ( dhcprelay_get_serverip_by_vlan ( ulVid, ulDhcpServerIp ) != OK )
    {
        DHCP_SERVER_RX_DBG ( "dhcprelay_process_dhcpdiscover: The vlan %d has not configed dhcp server ip", ulVid);
        return SYS_ERR_FAILED;
    }

    /*Check if the giaddr field in the DHCP message is zero*/
    if ( osal_ntohl(pDhcp->ulgiaddr) == 0 )
    {
        if (dhcprelay_add_gateway(ulIfaddr, pDhcp, ifPara, &stDhcpOption, &dhcpOption60server) != OK)
        {
            DHCP_SERVER_RX_DBG ( "dhcprelay_add_gateway failed: no interface ip can be gateway, maybe because too many dhcp option 60 configed." );
            return SYS_ERR_FAILED;
        }

        ulSrcIp = ntohl(pDhcp->ulgiaddr);
    }
    else
    {
        ulSrcIp = ulIfaddr;
    }

#if 1
    for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
    {
        if ( ulDhcpServerIp[i] == 0 )
        {
            continue;
        }

        if (dhcpOption60server && dhcpOption60server != ulDhcpServerIp[i])
        {
            continue;
        }

        /*Determine whether the DHCP server corresponding to the current interface is a built-in server*/
        if ( dhcprelay_check_local_server ( ulIfaddr, ulDhcpServerIp[i] ) == OK )
        {
            /*build-in dhcp server*/
            dhcp_ipaddr_to_str( ulDhcpServerIp[i], buf );
            DHCP_SERVER_TX_DBG ( "Send DHCPDISCOVER packet to local server(%s)", buf );
            pDhcp->ulgiaddr = ntohl(ulDhcpServerIp[i]);// add wt 20210417
            /*send the packet to the build-in server message queue via messaging*/
            rc = dhcprelay_packet_to_localserver(DHCPSERVER_RX_DISCOVER, ulDhcpLen, ulDhcpServerIp[i], ulVid, ifPara, pDhcp);
            if ( rc != 0 )
            {
                /*send failure*/
                DHCP_SERVER_TX_DBG ( "Dhcp relay send DHCPSERVER_RX_DISCOVER msg failed " );
            }
        }
        else
        {
            /*external dhcp server*/
            dhcp_ipaddr_to_str( ulDhcpServerIp[i], buf );
            DHCP_SERVER_TX_DBG ( "Send DHCPDISCOVER packet to dhcp server(%s)", buf );
            /*call the socket interface function to send to the corresponding dhcp server*/
            rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulSrcIp, ulDhcpServerIp[i], DHCPSERVER_PORT);
            if ( rc == ERROR )
            {
                DHCP_SERVER_TX_DBG ( "\r\n Send DHCPDISCOVER packet by socket failed " );
            }
        }
    }
#else
	    /*Determine whether the DHCP server corresponding to the current interface is a built-in server*/
        if ( dhcprelay_check_local_server ( ulIfaddr, ulDhcpserverIp ) == SYS_ERR_OK )
        {
            /*build-in dhcp server*/
            dhcp_ipaddr_to_str( ulDhcpserverIp, buf );
            dhcp_debug_printf ( "\r\n Send DHCPDISCOVER packet to local server(%s)", buf );
            /*send the packet to the build-in server message queue via messaging*/
            rc = dhcprelay_packet_to_localserver(DHCPSERVER_RX_DISCOVER, ulDhcpLen,
                    ulDhcpserverIp, ulVid, ifPara, pDhcp);
            if ( rc != 0 )
            {
                /*send failure*/
                dhcp_debug_printf ( "\r\n Dhcp relay send DHCPSERVER_RX_DISCOVER msg failed " );
            }
        }
        else
        {
            /*external dhcp server*/
            dhcp_ipaddr_to_str( ulDhcpserverIp, buf );
            dhcp_debug_printf ( "\r\n Send DHCPDISCOVER packet to dhcp server(%s)", buf );
            /*call the socket interface function to send to the corresponding dhcp server*/
            rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulSrcIp, ulDhcpserverIp, DHCPSERVER_PORT);
            if ( rc == ERROR )
            {
                dhcp_debug_printf ( "\r\n Send DHCPDISCOVER packet by socket failed " );
            }
        }
#endif
    return SYS_ERR_OK;
}

/*******************************************************************************
  Function: STATUS dhcprelay_get_serverip_by_vlanif
  Description: Retrieve the DHCP server IP address corresponding to the interface
  Input: ulong_t ulIfNo - VLAN interface index
  Output: ulong_t *ulDhcpServerIp - DHCP server IP address corresponding to the interface
  Return: success OK, failure ERROR
*******************************************************************************/
int32 dhcprelay_get_serverip_by_vlanif( ulong_t ulIfNo, ulong_t pDhcpServrIp[DHCPINTERFACEMAXNUM] )
{
    ulong_t i = 0;
    ulong_t ulGroup;
    ulong_t ulValidGrupNum = 0;

    if ( ulIfNo > DHCP_MAX_INTERFACE_NUM )
        return ERROR;

    for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
    {
        ulGroup = g_dhcpInterface[ulIfNo-1].groupNo[i];
        pDhcpServrIp[i] = 0;
        if ( ulGroup != DHCPEMPTYGROUP )
        {
            if ( g_dhcpgroup[ulGroup-1].serverIp != 0 )
            {
                pDhcpServrIp[i] = g_dhcpgroup[ulGroup-1].serverIp;
                ulValidGrupNum++;
            }
        }
    }

    if ( ulValidGrupNum )
        return OK;
    else
        return ERROR;
}

/*******************************************************************************
  Function: STATUS dhcprelay_get_serverip_by_vlanif
  Description: Retrieve the DHCP server IP address corresponding to the VLAN interface
  Input: ulong_t ulIfNo - VLAN interface index
  Output: ulong_t *ulDhcpServerIp - DHCP server IP address corresponding to the interface
  Return: Success OK, Failure ERROR
*******************************************************************************/
#if 0
int32 dhcprelay_get_serverip_by_vlan( ulong_t ulvid, ulong_t *pDhcpServrIp )
{
    ulong_t ulGroup;
	SYS_ERR_CHK(sal_dhcp_server_intf_group_by_vid_get(ulvid, &ulGroup));
	SYS_ERR_CHK(sal_dhcp_server_group_ip_get(ulGroup, pDhcpServrIp));

	DEBUGIPV4(*pDhcpServrIp);
	return SYS_ERR_OK;

}
#endif
int32 dhcprelay_get_serverip_by_vlan( ulong_t ulvid, ulong_t pDhcpServrIp[DHCPINTERFACEMAXNUM] )
{
    ulong_t i = 0, j = 0;
    ulong_t ulGroup;
    ulong_t ulValidGrupNum = 0;

    if (( ulvid < 1 ) || ( ulvid > 4094 ))
        return ERROR;

    for ( i = 0; i < DHCP_MAX_INTERFACE_NUM; i++ )
    {
    	if (g_dhcpInterface[i].vid == ulvid)
		{
		    for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
		    {
		        ulGroup = g_dhcpInterface[i].groupNo[j];
		        pDhcpServrIp[j] = 0;
		        if ( ulGroup != DHCPEMPTYGROUP )
		        {
		            if ( g_dhcpgroup[ulGroup-1].serverIp != 0 )
		            {
		                pDhcpServrIp[j] = g_dhcpgroup[ulGroup-1].serverIp;
		                ulValidGrupNum++;
		            }
		        }
		    }
		}
	}

    if ( ulValidGrupNum )
        return OK;
    else
        return ERROR;
}

/*******************************************************************************
  Function: BOOL dhcprelay_ipis_serverip_of_vlanif
  Description: Checks whether the given IP is the DHCP server IP corresponding to the VLAN interface
  Input: ulong_t ulIfNo - VLAN interface index
  Output: ulong_t ulIp - IP address to check
  Return: Success TRUE, Failure FALSE
*******************************************************************************/
BOOL dhcprelay_ipis_serverip_of_vlanif( ulong_t ulIfNo, ulong_t ulIp )
{
    ulong_t i = 0;
    ulong_t ulGroup;

    if ( ulIfNo > DHCP_MAX_INTERFACE_NUM )
        return FALSE;

    for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
    {
        ulGroup = g_dhcpInterface[ulIfNo-1].groupNo[i];
        if ( ulGroup != DHCPEMPTYGROUP )
        {
            if ( (g_dhcpgroup[ulGroup-1].serverIp != 0)
                 && (ulIp == g_dhcpgroup[ulGroup-1].serverIp) )
            {
                 return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL dhcprelay_ipis_serverip_of_vlan( ulong_t ulvid, ulong_t ulIp )
{
    ulong_t i = 0, j = 0;
    ulong_t ulGroup;

    if (( ulvid < 1 ) || ( ulvid > 4094 ))
        return FALSE;

    for ( i = 0; i < DHCP_MAX_INTERFACE_NUM; i++ )
    {
    	if (g_dhcpInterface[i].vid == ulvid)
		{
			for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
			{
				ulGroup = g_dhcpInterface[i].groupNo[j];
				if ( ulGroup != DHCPEMPTYGROUP )
				{
					if ( (g_dhcpgroup[ulGroup-1].serverIp != 0)
						 && (ulIp == g_dhcpgroup[ulGroup-1].serverIp) )
					{
						 return TRUE;
					}
				}
			}
		}
    }

    return FALSE;
}

STATUS dhcprelay_setzero_by_vlanif( ulong_t ulIfNo )
{
    ulong_t i = 0;

    if ( ulIfNo > DHCP_MAX_INTERFACE_NUM )
        return ERROR;

    for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
    {
        g_dhcpInterface[ulIfNo-1].groupNo[i] = DHCPEMPTYGROUP;
    }

    return OK;
}
/*******************************************************************************
  Function: STATUS dhcprelay_check_local_server
  Description: Determine whether the DHCP server corresponding to the current interface
               is a built-in server
  Input: ulong_t ulIfaddr - interface index
         ulong_t ulDhcpServerIp  - DHCP server IP address corresponding to the current
         interface is a built-in DHCP server
  Output: None
  Return: OK if it a built-in DHCP server, otherwise ERROR
*******************************************************************************/
STATUS dhcprelay_check_local_server ( ulong_t ulIfaddr, ulong_t ulDhcpServerIp )
{
    char  localserver[] = "127.0.0.1";
    ulong_t  ulIp, ulSw;

    /*determine whether the DHCP server address corresponding to the current interface is 127.0.0.1 */
    if ( dhcp_str_to_ipaddr ( localserver, &ulIp ) == OK )
    {
        if ( ulDhcpServerIp == ulIp )
        {
            return SYS_ERR_OK;
        }
    }

    /*check if the DHCP server address is an IP address of the switch interface*/
    // add: extended DHCP server address check against all IP addresses of the interface
    if (dhcprelay_checkLocalIp(ulDhcpServerIp, &ulSw))
    {
        return OK;
    }

    return SYS_ERR_FAILED;
}


/*******************************************************************************
  Function: STATUS dhcprelay_process_dhcpdiscover
  Description: Handles the DHCPOFFER message sent from a DHCP client
  Inpur: DHCP_HEAD_S *pDhcp - Pointer to the message header
         ulong_t ulDhcpLen - Message length (including header and option data)
         ulong_t ulIP - Source IP address of the message
         DHCP_INTERFACE_PARA *ifPara Interface information
  Output: None
  Return:  Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcprelay_process_dhcpoffer( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara)
{
    uchar_t ucUserMac[6];
    uchar_t buf[128];
    ulong_t ulIfaddr = 0;
    ulong_t ulIfNo;
    ulong_t ulGatewaySw = 1; /*Start from 1*/
    ulong_t ulVid;
    ulong_t ulPort;
    ulong_t ulGatewayIp = 0;    /*relay ip in dhcp packet*/
    STATUS rc;
    DHCP_OPTION_S stDhcpOption;
    ulong_t ultemp = 0;

    pDhcp->uchops++;
	DHCP_SERVER_DBG("dhcprelay_process_dhcpoffer: uchops %d", pDhcp->uchops);
    /*General packet inspection function*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }
    /*Record packet option fields*/
    dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption );

    if (NULL == ifPara)
    {
        return ERROR;
    }

    ulIfNo = ifPara->dhcp_sw;
    ulVid  = ifPara->dhcp_vid;
    ulPort = ifPara->dhcp_port;

    /*Check if the server IP field is present*/
    if ( stDhcpOption.pulDHCPServerIp != NULL )
    {
        memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
        dhcp_ipaddr_to_str (ntohl(ultemp), buf);
        DHCP_SERVER_DBG ("Dhcp relay receive DHCPOFFER packet from %s ", buf );
    }

    /*Check if the server IP is hidden*/
    if( g_bDhcpHideServerIp )
    {
        if ( dhcprelay_get_ifaddr(ulIfNo, ulVid, &ulIfaddr) != OK)
        {
            DHCP_SERVER_DBG( "dhcprelay_process_dhcpoffer: The vlan %u has no interiface ip address", ulVid);
            return ERROR;
        }

        if ( (stDhcpOption.pulDHCPServerIp != NULL))
        {
            memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
            if ( 0 != ntohl(ultemp))
            {
                ultemp = htonl(ulIfaddr);
                memcpy(stDhcpOption.pulDHCPServerIp, &ultemp, sizeof(ulong_t));
            }
        }

        if ( ntohl(pDhcp->ulsiaddr) != 0 )
        {
            pDhcp->ulsiaddr = htonl(ulIfaddr);
        }
    }

    /*Check whether the giaddr address in the DHCPOFFER packet is the local interface address*/
    ulGatewayIp = ntohl(pDhcp->ulgiaddr);
    if ( dhcprelay_checkLocalIp(ulGatewayIp, &ulGatewaySw) )
    {
        /*Check whether the ciaddr field int the current DHCPOFFER packet indicates unicast
        or broadcast transmission*/
        if ( ntohl(pDhcp->ulciaddr) == 0 )
        {
            /*Broadcast transmission*/
            dhcp_ipaddr_to_str ( INADDR_BROADCAST, buf );
            rc = dhcprelay_send_broadcast_to_client(ulGatewayIp, pDhcp, ulDhcpLen, ulVid, ulPort, ulGatewaySw);
            if ( rc == ERROR )
            {
                DHCP_SERVER_DBG ( "Send DHCPOFFER packet by socket failed to %s", buf );
                return rc;
            }
            DHCP_SERVER_DBG ( "Send DHCPOFFER packet in broadcast by socket to %s", buf );
        }
        else
        {
            /*Unicast transmission*/
            dhcp_ipaddr_to_str ( ntohl(pDhcp->ulciaddr), buf );
            rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulGatewayIp, ntohl(pDhcp->ulciaddr), DHCPCLIENT_PORT);
            if ( rc == ERROR )
            {
                DHCP_SERVER_DBG ( "Send DHCPOFFER packet by socket failed to %s", buf );
                return rc;
            }
            DHCP_SERVER_DBG ( "Send DHCPOFFER packet in unicast by socket to %s", buf );
        }
    }
    else
    {
        /*This situation indicates that the DHCPOFFER message is recieved from the local server and
         there is also a DHCP relay device connected downstream*/

        /*ֱDirectry send to the giaddr address DHCP relay device through the socket interface*/
        if (ulGatewayIp == 0)/*when the relay and the server  or relay are on the same subnet and broadcast
                               OFFER is sent, this situation may occur. Avoid such loops where the message
                               is sent back to itself*/
        {
            DHCP_SERVER_DBG ( "dhcp relay receive DHCPOFFER packet with giaddr = 0, drop packet" );
            return ERROR;
        }

        dhcp_ipaddr_to_str ( ulGatewayIp, buf );
        rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulIfaddr, ulGatewayIp, DHCPSERVER_PORT);
        if ( rc == ERROR )
        {
            DHCP_SERVER_DBG ( "Send DHCPOFFER packet by socket failed to %s", buf );
            return rc;
        }
        DHCP_SERVER_DBG ( "Send DHCPOFFER packet by socket to %s", buf );

    }
    return OK;
}

/*******************************************************************************
  Function: STATUS dhcprelay_process_dhcprequest
  Description:Processes DHCPREQUEST packets recieved from clients
  Input: DHCP_HEAD_S *pDhcp  - pointer to the DHCP message header
         ulong_t ulDhcpLen - length of the DHCP message (including header and options)
         ulong_t ulIP - source IP address of the message
         DHCP_INTERFACE_PARA *ifPara - interface infotmation
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcprelay_process_dhcprequest( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, DHCP_INTERFACE_PARA *ifPara)
{
    uchar_t ucUserMac[6];
    uchar_t buf[128];
    ulong_t ulIfaddr;    /*interface IP addressIP*/
    ulong_t ulIfNo;
    ulong_t ulDhcpServerIp[DHCPINTERFACEMAXNUM] = {0};    /*dhcp server address*/
    ulong_t ulGatewayIp;
    ulong_t ulVid;
    ulong_t ulPort;
    ulong_t ulSrcIp = 0;    /*the source IP address used in the IP header corresponds to the relay IP*/
    STATUS rc;
    int    i;
    DHCP_OPTION_S stDhcpOption;
    BOOL bjust2local = FALSE;
    BOOL bhasSent = FALSE;
    ulong_t dhcpOption60server = 0;
    ulong_t ultemp = 0;

    pDhcp->uchops++;
    /*General m=essage inspection function*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }
    /*Record the option field of the message*/
    dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption );

    if (NULL == ifPara)
    {
        return ERROR;
    }

    ulIfNo = ifPara->dhcp_sw;
    ulVid  = ifPara->dhcp_vid;
    ulPort = ifPara->dhcp_port;

    if ( (ulPort < SYS_LOGICPORT_NUM_MAX) && (gbDHCPRelayPortEnable[ulPort] == FALSE) )
    {
        DHCP_SERVER_DBG( "dhcprelay_process_dhcprequest: port[%d] relay-enable is false.", ulPort);
        return ERROR;
    }
    if ( dhcprelay_get_ifaddr(ulIfNo, ulVid, &ulIfaddr) != OK)
    {
        DHCP_SERVER_DBG( "dhcprelay_process_dhcprequest: The vlan %u has no interiface ip address ", ulVid);
        return ERROR;
    }

    /*ͨObtain the corresponding DHCP server address through the interface*/
    if ( g_bDhcpHideServerIp == FALSE )
    {
        if ( (stDhcpOption.pulDHCPServerIp != NULL))
        {
            memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
            if (dhcprelay_ipis_serverip_of_vlan(ulVid, ntohl(ultemp)))
            {
                ulDhcpServerIp[0] = ntohl(ultemp);
            }
            else
            {
                DHCP_SERVER_DBG ( "dhcprelay_process_dhcprequest: The vlan %u has not configed dhcp server ip of the specified server-identifier of dhcp request packet", ulVid);
                
                goto getAllServers;
            }
        }
        else if (ntohl(pDhcp->ulciaddr) != 0 && ifPara->dhcp_dstIp != 0 && ifPara->dhcp_dstIp != 0xffffffff)
        {
            if (dhcprelay_ipis_serverip_of_vlan(ulVid, ifPara->dhcp_dstIp))
            {
                ulDhcpServerIp[0] = ifPara->dhcp_dstIp;
            }
            else
            {
                DHCP_SERVER_DBG ( "dhcprelay_process_dhcprequest: The vlan %u has not configed dhcp server ip of the unicast dhcp request packet", ulVid);
                bjust2local = TRUE;
                goto getAllServers;
            }
        }
        else
        {
        getAllServers:
            if ( dhcprelay_get_serverip_by_vlan ( ulVid, ulDhcpServerIp ) != OK )
            {
                DHCP_SERVER_DBG ( "dhcprelay_process_dhcprequest: The vlan %u has not configed dhcp server ip", ulVid);
                return ERROR;
            }
        }
    }
    else
    {
        /*When renewing the lease, the ciaddr is not 0. If the server IP is hidden, the destination address
        of the renewwal message cannot be used as the reak server address, At this time, use the IP address
        assigned by the customer to check the local address pool. If the IP is assigned locally, the server
        address is the interface address, otherwise for an external server, use sw to find the address 
        of the subordinate server
        */
        if ( (ntohl(pDhcp->ulciaddr) != 0) && (ipPoolGetGatewayBySpecialIp(ntohl(pDhcp->ulciaddr), &ulGatewayIp) == OK)
            && dhcprelay_ipis_serverip_of_vlan(ulVid, ulIfaddr))
        {
            ulDhcpServerIp[0] = ulIfaddr;
        }
        else
        {
            if ( dhcprelay_get_serverip_by_vlan ( ulVid, ulDhcpServerIp ) != OK )
            {
                DHCP_SERVER_DBG ( "dhcprelay_process_dhcprequest: The vlan %u has not configed dhcp server ip", ulVid);
                return ERROR;
            }
        }
    }

    /*Determine whether the giaddr field of the DHCP packet is 0*/
    if ( ntohl(pDhcp->ulgiaddr) == 0 )
    {
        if (dhcprelay_add_gateway(ulIfaddr, pDhcp, ifPara, &stDhcpOption, &dhcpOption60server) != OK)
        {
            DHCP_SERVER_DBG ( "dhcprelay_add_gateway failed: no interface ip can be gateway, maybe because too many dhcp option 60 configed." );
            return ERROR;
        }

        ulSrcIp = ntohl(pDhcp->ulgiaddr);
    }
    else
    {
        ulSrcIp = ulIfaddr;
    }

    /*Send packets to dhcp server*/
    for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
    {
        if ( ulDhcpServerIp[i] == 0 )
        {
            continue;
        }

        if (dhcpOption60server && dhcpOption60server != ulDhcpServerIp[i])
        {
            continue;
        }

        /*Whether to hide the server ip*/
        if ( g_bDhcpHideServerIp )
        {
            /*You need to yse the subordinate server address in the message to change the server address
               and option server address in the message*/
            if ( (stDhcpOption.pulDHCPServerIp != NULL))
            {
                memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
                if ( 0 != ntohl(ultemp))
                {
                    ultemp = htonl(ulDhcpServerIp[i]);
                    memcpy(stDhcpOption.pulDHCPServerIp, &ultemp, sizeof(ulong_t));
                }
            }

            if( ntohl(pDhcp->ulsiaddr) != 0 )
            {
                pDhcp->ulsiaddr = htonl(ulDhcpServerIp[i]);
            }
        }

        if ( dhcprelay_check_local_server ( ulIfaddr, ulDhcpServerIp[i] ) == OK )
        {
             
             if ( (stDhcpOption.pulDHCPServerIp != NULL))
             {
                 memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
                 if ( ulDhcpServerIp[i] != ntohl(ultemp))
                 {
                    continue;
                 }
             }

             if (bjust2local)
             {
                 if (bhasSent)
                    break;
                 else
                    bhasSent = TRUE;
             }

            /*built-in dhcp server*/
            dhcp_ipaddr_to_str( ulDhcpServerIp[i], buf );
            DHCP_SERVER_DBG ( "Send DHCPREQUEST packet to local server(%s)", buf );
            pDhcp->ulgiaddr = ntohl(ulDhcpServerIp[i]);//add wt 20210417
            /*Send packets to the built-in server message queue via message*/
            rc = dhcprelay_packet_to_localserver(DHCPSERVER_RX_REQUEST, ulDhcpLen,
                    ulDhcpServerIp[i], ulVid, ifPara, pDhcp);
            if ( rc != 0 )
            {
                /*Sending failed*/
                DHCP_SERVER_DBG ( "Dhcp relay send DHCPSERVER_RX_REQUEST msg failed " );
            }
        }
        else
        {
            if (bjust2local)
            {
                continue;
            }

            /*External dhcp server*/
            dhcp_ipaddr_to_str( ulDhcpServerIp[i], buf );
            DHCP_SERVER_DBG ( "Send DHCPREQUEST packet to dhcp server(%s)", buf );
            /*Call the socket interface function and send it to the corresponding dhcp server*/
            rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulSrcIp, ulDhcpServerIp[i], DHCPSERVER_PORT);
            if ( rc == ERROR )
            {
                DHCP_SERVER_DBG ( "Send DHCPREQUEST packet by socket failed " );
            }
        }
    }

    return OK;
}

/*******************************************************************************
  Function: STATUS dhcprelay_process_dhcpack
  Description: Process DHCPACK packets sent from DHCP clients
  Input: DHCP_HEAD_S *pDhcp - header pointer
         ulong_t ulDhcpLen - message length
         ulong_t ulIP - packet source IP address
         DHCP_INTERFACE_PARA *ifPara - interface information
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcprelay_process_dhcpack( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara)
{
    uchar_t ucUserMac[6];
    uchar_t buf[128];
    ulong_t ulIfaddr = 0;
    ulong_t ulIfNo;
    ulong_t ulGatewaySw = 1; 
    ulong_t ulVid;
    ulong_t ulPort;
    ulong_t ulGatewayIp = 0;    
    STATUS rc;
    DHCP_OPTION_S stDhcpOption;
    ulong_t ultemp = 0;
    pDhcp->uchops++;
       if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }
        dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption );

    if (NULL == ifPara)
    {
        return ERROR;
    }

    ulIfNo = ifPara->dhcp_sw;
    ulVid  = ifPara->dhcp_vid;
    ulPort = ifPara->dhcp_port;

#ifdef GBN_INCLUDE_DHCPSNOOPING
    memcpy(&ultemp, stDhcpOption.pulLeaseTime, sizeof(ulong_t));
    ultemp = ntohl(ultemp);

    dhcpsnoopingServerAck(pDhcp->szchaddr, ulVid, ntohl(pDhcp->ulyiaddr), ultemp);
#endif

    if ( stDhcpOption.pulDHCPServerIp != NULL )
    {
        memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
        dhcp_ipaddr_to_str ( ntohl(ultemp), buf );
        DHCP_SERVER_DBG ("Dhcp relay receive DHCPACK packet from %s ", buf );
    }

        if ( g_bDhcpHideServerIp )
    {
        if ( dhcprelay_get_ifaddr(ulIfNo, ulVid, &ulIfaddr) != OK)
        {
            DHCP_SERVER_DBG( "dhcprelay_process_dhcpack: The vlan %u has no interiface ip address", ulVid);
            return ERROR;
        }

        if ( (stDhcpOption.pulDHCPServerIp != NULL))
        {
            memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
            if ( 0 != osal_ntohl(ultemp))
            {
                ultemp = osal_htonl(ulIfaddr);
                memcpy(stDhcpOption.pulDHCPServerIp, &ultemp, sizeof(ulong_t));
            }
        }

        if ( ntohl(pDhcp->ulsiaddr) != 0 )
        {
            pDhcp->ulsiaddr = osal_htonl(ulIfaddr);
        }
    }

    /*Determine whether the giaddr address in the DHCPACK packet is the local interface address*/
    ulGatewayIp = ntohl(pDhcp->ulgiaddr);
    if ( dhcprelay_checkLocalIp(ulGatewayIp, &ulGatewaySw) )
    {
        /*Determine the ciaddr field int the current ack packet to determine whether to send unicast
          or broadcast*/
        if ( ntohl(pDhcp->ulciaddr) == 0 )
        {
            /*Broadcast*/
            dhcp_ipaddr_to_str ( INADDR_BROADCAST, buf );
            rc = dhcprelay_send_broadcast_to_client(ulGatewayIp, pDhcp, ulDhcpLen, ulVid, ulPort, ulGatewaySw);
            if ( rc == ERROR )
            {
                DHCP_SERVER_DBG ( "Send DHCPACK packet by socket failed to %s", buf );
                return rc;
            }
            DHCP_SERVER_DBG ( "Send DHCPACK packet in broadcast by socket to %s", buf );
        }
        else
        {
            /*Unicast*/
            dhcp_ipaddr_to_str ( ntohl(pDhcp->ulciaddr), buf );
            rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulGatewayIp, ntohl(pDhcp->ulciaddr), DHCPCLIENT_PORT);
            if ( rc == ERROR )
            {
                DHCP_SERVER_DBG ( "Send DHCPACK packet by socket failed to %s", buf );
                return rc;
            }
            DHCP_SERVER_DBG ( "Send DHCPACK packet in unicast by socket to %s", buf );
        }
    }
    else
    {
        /*This situation means that it is recieving DHCPACK packets sent by the local server 
          and the DHCP Relay device is also hung up*/

        /*ֱSent directly to the giaddr address DHCP Relay device through the socket interface*/
        if (ulGatewayIp == 0)/*Relay and server or Relay will appear when they are in the same
                               network segment and send a broadcast ACK, so avoid sending
                               it to yourself in this loop*/
        {
            DHCP_SERVER_DBG ( "dhcp relay receive DHCPACK packet with giaddr = 0, drop packet" );
            return ERROR;
        }

        dhcp_ipaddr_to_str ( ulGatewayIp, buf );
        rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulIfaddr, ulGatewayIp, DHCPSERVER_PORT);
        if ( rc == ERROR )
        {
            DHCP_SERVER_DBG ( "Send DHCPACK packet by socket failed to %s", buf );
            return rc;
        }
        DHCP_SERVER_DBG ( "Send DHCPACK packet by socket to %s", buf );
    }

    return OK;
}

/*******************************************************************************
  Function: STATUS dhcprelay_process_dhcpnak
  Description: Process DHCPNAK packets sent from DHCP clients
  Input: DHCP_HEAD_S *pDhcp - heder pointer
         ulong_t ulDhcpLen - message length
         ulong_t ulIP - packet source IP address
         DHCP_INTERFACE_PARA *ifPara - interface information
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcprelay_process_dhcpnak( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara)
{
    uchar_t ucUserMac[6];
    uchar_t buf[128];
    ulong_t ulIfaddr = 0;
    ulong_t ulIfNo;
    ulong_t ulGatewaySw = 1; 
    ulong_t ulVid;
    ulong_t ulPort;
    ulong_t ulGatewayIp = 0;    
    STATUS rc;
    DHCP_OPTION_S stDhcpOption;
    ulong_t ultemp;

    pDhcp->uchops++;
    /*ͨUniversal message checking function*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }
    /*Record the option field of the message*/
    dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption );

    if (NULL == ifPara)
    {
        return ERROR;
    }

    ulIfNo = ifPara->dhcp_sw;
    ulVid  = ifPara->dhcp_vid;
    ulPort = ifPara->dhcp_port;

    if ( stDhcpOption.pulDHCPServerIp != NULL )
    {
        memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
        dhcp_ipaddr_to_str (ntohl(ultemp), buf);
        DHCP_SERVER_DBG ("Dhcp relay receive DHCPNAK packet from %s ", buf );
    }

#ifdef GBN_INCLUDE_DHCPSNOOPING
    /*Delete the user's information, mainly to deal with the built-in DHCP server responce packets*/
    dhcpsnoopingServerNak(pDhcp->szchaddr, ulVid, ntohl(pDhcp->ulxid));
#endif

        if ( g_bDhcpHideServerIp )
    {
        if (dhcprelay_get_ifaddr(ulIfNo, ulVid, &ulIfaddr) != OK)
        {
            DHCP_SERVER_DBG( "dhcprelay_process_dhcpnak: The vlan %u has no interiface ip address", ulVid);
            return ERROR;
        }

        if ( (stDhcpOption.pulDHCPServerIp != NULL))
        {
            memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
            if ( 0 != ntohl(ultemp))
            {
                ultemp = htonl(ulIfaddr);
                memcpy(stDhcpOption.pulDHCPServerIp, &ultemp, sizeof(ulong_t));
            }
        }

        if ( ntohl(pDhcp->ulsiaddr) != 0 )
        {
            pDhcp->ulsiaddr = htonl(ulIfaddr);
        }
    }

    /*Determine whether the giaddr address in the DHCPNAK packet is the local interfave address*/
    ulGatewayIp = ntohl(pDhcp->ulgiaddr);
    if ( dhcprelay_checkLocalIp(ulGatewayIp, &ulGatewaySw))
    {
        /*Determine the ciaddr field in the current nak packet to determine whether it is unicast
          or broadcast transmission*/
        if ( ntohl(pDhcp->ulciaddr) == 0 )
        {
            /*broadcast*/
            rc = dhcprelay_send_broadcast_to_client(ulGatewayIp, pDhcp, ulDhcpLen, ulVid, ulPort, ulGatewaySw);
            dhcp_ipaddr_to_str ( INADDR_BROADCAST, buf );
            if ( rc == ERROR )
            {
                DHCP_SERVER_DBG ( "Send DHCPNAK packet by socket failed to %s", buf );
                return rc;
            }
            DHCP_SERVER_DBG ( "Send DHCPNAK packet in broadcast by socket to %s", buf );
        }
        else
        {
            /*unicast*/
            dhcp_ipaddr_to_str ( ntohl(pDhcp->ulciaddr), buf );
            rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulGatewayIp, ntohl(pDhcp->ulciaddr), DHCPCLIENT_PORT);
            if ( rc == ERROR )
            {
                DHCP_SERVER_DBG ( "Send DHCPNAK packet by socket failed to %s", buf );
                return rc;
            }
            DHCP_SERVER_DBG ( "Send DHCPNAK packet in unicast by socket to %s", buf );
        }
    }
    else
    {
        /*This situation means that it is receiving DHCPNAK packets sent by the local server
           and the DHCP Relay device is also hung up*/

        /*ֱSent directly to the diaddr address DHCP Relay device through the socket interface*/
        if (ulGatewayIp == 0)/*Relay and server or Relay will appear when they are in the same network
                            segment and broadcast nack is sent, so avoid sending it to yourself in this loop*/
        {
            DHCP_SERVER_DBG ( "dhcp relay receive DHCPNAK packet with giaddr = 0, drop packet" );
            return ERROR;
        }

        dhcp_ipaddr_to_str (ulGatewayIp, buf );
        rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulIfaddr, ulGatewayIp, DHCPSERVER_PORT);
        if ( rc == ERROR )
        {
            DHCP_SERVER_DBG ( "Send DHCPNAK packet by socket failed to %s", buf );
            return rc;
        }
        DHCP_SERVER_DBG ( "Send DHCPNAK packet by socket to %s", buf );
    }

    return OK;
}

/*******************************************************************************
  Function: STATUS dhcprelay_process_dhcpreleasedecline
  Description: Process DHCPRELEASE/DHCPDECLINE packets sent from DHCP clients
  Input: DHCP_HEAD_S *pDhcp - header pointer
        ulong_t ulDhcpLen - message length
        ulong_t ulIP - packet source IP address
        DHCP_INTERFACE_PARA *ifPara - interface information
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcprelay_process_dhcpreleasedecline( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara)
{
    uchar_t ucUserMac[6];
    uchar_t buf[128];
    ulong_t ulIfaddr;    
    ulong_t ulIfNo;
    ulong_t ulDhcpServerIp[DHCPINTERFACEMAXNUM] = {0};   
    ulong_t ulGatewayIp;
    ulong_t ulVid;
    ulong_t ulType;
    ulong_t ulSrcIp = 0;    
    STATUS rc;
    int    i;
    DHCP_OPTION_S stDhcpOption;
    BOOL bjust2local = FALSE;
    BOOL bhasSent = FALSE;
    ulong_t dhcpOption60server = 0;
    ulong_t ultemp = 0;

    memset(ulDhcpServerIp, 0, sizeof(ulDhcpServerIp));
    pDhcp->uchops++;
    /*ͨUniversal message checking function*/
    if ( dhcp_user_pkt_check( pDhcp, ucUserMac ) != OK )
    {
        return ERROR;
    }
    /*Record the option field of the message*/
    dhcp_record_all_opt ( pDhcp, ulDhcpLen-DHCP_HEAD_LEN, &stDhcpOption );

    if ( *(stDhcpOption.pucDHCPMsgType) == DHCP_RELEASE_TYPE )
    {
        DHCP_SERVER_DBG ( "dhcprelay_process_dhcpreleasedecline: Receive DHCPRELEASE packet ");
    }
    else
    {
        DHCP_SERVER_DBG ( "dhcprelay_process_dhcpreleasedecline: Receive DHCPDECLINE packet ");
    }

    if (NULL == ifPara)
    {
        return ERROR;
    }

    ulIfNo = ifPara->dhcp_sw;
    ulVid  = ifPara->dhcp_vid;

    if (dhcprelay_get_ifaddr(ulIfNo, ulVid, &ulIfaddr) != OK)
    {
        DHCP_SERVER_DBG( "dhcprelay_process_dhcpreleasedecline: The vlan %u has no interiface ip address", ulVid);
        return ERROR;
    }

    /*ͨObtain the corresponding DHCP server address through the interface*/
    if( g_bDhcpHideServerIp == FALSE )
    {
        if ( *(stDhcpOption.pucDHCPMsgType) == DHCP_RELEASE_TYPE )/*The release message is unicast message*/
        {
            if (ntohl(pDhcp->ulciaddr) != 0)
            {
                if (dhcprelay_ipis_serverip_of_vlan(ulVid, ifPara->dhcp_dstIp))
                {
                    ulDhcpServerIp[0] = ifPara->dhcp_dstIp;
                }
                else
                {
                    DHCP_SERVER_DBG ( "dhcprelay_process_dhcpreleasedecline: The vlan %u has not configed dhcp server ip of the unicast dhcp release packet", ulVid);
                    bjust2local = TRUE;
                    goto getAllServers;
                }
            }
            else
            {
                goto getAllServers;
            }
        }
        else /*decline broadcast message*/
        {
        getAllServers:
            if ( dhcprelay_get_serverip_by_vlan ( ulVid, ulDhcpServerIp ) != OK )
            {
                DHCP_SERVER_DBG ( "dhcprelay_process_dhcpreleasedecline: The vlan %u has not configed dhcp server ip", ulVid);
                return ERROR;
            }
        }
    }
    else
    {
        /*When renewing the lease, the ciaddr is not 0. If the server IP is hidden,
        the destination address of the renewal message cannot be used as the real server address.
        At this time, use the IP address assigned by the customer to check the local address pool.
        If the IP is assigned locally, the server address is the interface address, otherwise
        For external server, use sw to find the address of the subordinate server
        */
        if( (ntohl(pDhcp->ulciaddr) != 0) && (ipPoolGetGatewayBySpecialIp(ntohl(pDhcp->ulciaddr), &ulGatewayIp) == OK)
            && dhcprelay_ipis_serverip_of_vlan(ulVid, ulIfaddr))
        {
            ulDhcpServerIp[0] = ulIfaddr;
        }
        else
        {
            if ( dhcprelay_get_serverip_by_vlan ( ulVid, ulDhcpServerIp ) != OK )
            {
                DHCP_SERVER_DBG ( "dhcprelay_process_dhcpreleasedecline: The vlan %u has not configed dhcp server ip", ulVid);
                return ERROR;
            }
        }
    }

    /*Determine whether the giaddr field of the DHCP packet is 0*/
    if ( ntohl(pDhcp->ulgiaddr) == 0 )
    {
        if (dhcprelay_add_gateway(ulIfaddr, pDhcp, ifPara, &stDhcpOption, &dhcpOption60server) != OK)
        {
            DHCP_SERVER_DBG ( "dhcprelay_process_dhcpreleasedecline failed: no interface ip can be gateway, maybe because too many dhcp option 60 configed." );
            return ERROR;
        }

        ulSrcIp = ntohl(pDhcp->ulgiaddr);
    }
    else
    {
        ulSrcIp = ulIfaddr;
    }

    /*Send release/decline message to dhcp server*/
    for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
    {
        if ( ulDhcpServerIp[i] == 0 )
        {
            continue;
        }

        /*Whether to hide server ip*/
        if ( g_bDhcpHideServerIp )
        {
            if ( (stDhcpOption.pulDHCPServerIp != NULL))
            {
                memcpy(&ultemp, stDhcpOption.pulDHCPServerIp, sizeof(ulong_t));
                if ( 0 != osal_ntohl(ultemp))
                {
                    ultemp = osal_htonl(ulDhcpServerIp[i]);
                    memcpy(stDhcpOption.pulDHCPServerIp, &ultemp, sizeof(ulong_t));
                }
            }

            if ( ntohl(pDhcp->ulsiaddr) != 0 )
            {
                pDhcp->ulsiaddr = osal_htonl(ulDhcpServerIp[i]);
            }
        }

        if ( dhcprelay_check_local_server ( ulIfaddr, ulDhcpServerIp[i] ) == OK )
        {
            if (bjust2local)
            {
                if (bhasSent)
                   break;
                else
                   bhasSent = TRUE;
            }

            /*Built-indhcp server*/
			//osal_printf("\r\n %s %d", __func__, __LINE__);
            dhcp_ipaddr_to_str( ulDhcpServerIp[i], buf );
            if ( *(stDhcpOption.pucDHCPMsgType) == DHCP_RELEASE_TYPE )
            {
                DHCP_SERVER_DBG ( "Send DHCPRELEASE packet to local server(%s)", buf );
                ulType = DHCPSERVER_RX_RELAESE;
            }
            else
            {
                DHCP_SERVER_DBG ( "Send DHCPDECLINE packet to local server(%s)", buf );
                ulType = DHCPSERVER_RX_DECLINE;
            }
            /*The gateway field of the lease renewal message is obtained by ciaddr checking the IP address pool*/
            if( ntohl(pDhcp->ulciaddr) != 0 )
            {
				//osal_printf("\r\n %s %d", __func__, __LINE__);
               if( ipPoolGetGatewayBySpecialIp(ntohl(pDhcp->ulciaddr), &ulGatewayIp)!= OK)
               {
                    DHCP_SERVER_DBG ( "dhcprelay_process_dhcpreleasedecline: Get gateway by client ip failed");
                    continue;
               }
               pDhcp->ulgiaddr = htonl(ulGatewayIp);
            }
			//osal_printf("\r\n %s %d", __func__, __LINE__);
            /*Send packets to the built-in server message queue via message*/
            rc = dhcprelay_packet_to_localserver(ulType, ulDhcpLen, ulDhcpServerIp[i], ulVid, ifPara, pDhcp);
			//osal_printf("\r\n %s %d", __func__, __LINE__);
            if ( rc != 0 )
            {
                /*Sending failed*/
                DHCP_SERVER_DBG ( "Dhcp relay send DHCPSERVER_RX_RELAESE or DHCPSERVER_RX_DECLINE msg failed " );
            }
        }
        else
        {
            if (bjust2local)
            {
                continue;
            }

            /*External dhcp server*/
            dhcp_ipaddr_to_str( ulDhcpServerIp[i], buf );
            if ( *(stDhcpOption.pucDHCPMsgType) == DHCP_RELEASE_TYPE )
            {
                DHCP_SERVER_DBG ( "Send DHCPRELEASE packet to dhcp server(%s)", buf );
            }
            else
            {
                DHCP_SERVER_DBG ( "Send DHCPDECLINE packet to dhcp server(%s)", buf );
            }
            /*Call the socket interface function and send it to the corresponding dhcp server*/
            rc = dhcpsock_sendto2((char *)pDhcp, (int)ulDhcpLen, ulSrcIp, ulDhcpServerIp[i], DHCPSERVER_PORT);
            if ( rc == ERROR )
            {
                DHCP_SERVER_DBG ( "Send DHCPRELEASE or DHCPDECLINE packet by socket failed " );
            }
        }
    }

    return OK;
}

/*******************************************************************************
  Function: STATUS dhcprelay_set_enble_state( ulong_t ulState )
  Description: Set the DHCP Relay module start switch
  Input: ulong_t ulState  - Startswitch value
  Output: None
  Return: Success OK, Failure ERROR
  Other: Provide for use by MIB processing function
*******************************************************************************/
STATUS dhcprelay_set_enble_state( ulong_t ulState )
{
    /*Judge the legitimacy parameters*/
    if ( (ulState != DHCP_ENABLE) && (ulState != DHCP_DISABLE) )
    {
        return ERROR;
    }

    if ( ulState == DHCP_ENABLE )
    {
        /*If the dhcp relay function already turned on, it not will start*/
        if ( g_bDhcpRelayEnable == TRUE )
        {
            return OK;
        }

        #ifdef ISF_DHCPSNOOPING_DISTRIBUTE
        #ifdef GBN_INCLUDE_DHCPSNOOPING
        /*Judge first DHCP Snooping has it been started?*/
        if ( TRUE == isf_stack_enable() )
        {
            if ( bDhcpSnoopingOn == TRUE )
            {
                return ERROR;
            }
        }
        #endif
        #endif

        g_bDhcpRelayEnable = TRUE;

        set_socket_status( enablesocket );

        //smdVDDhcprelayOnOff(VD_ON_OFF_ENABLE);
    }
    else
    {
        if (FALSE == g_bDhcpRelayEnable)
        {
            return OK;
        }

        g_bDhcpRelayEnable = FALSE;

        //smdVDDhcprelayOnOff(VD_ON_OFF_DISABLE);

        set_socket_status(disablesocket);
    }

    return OK;
}


/*******************************************************************************
  Function: STATUS dhcprelay_get_enble_state( ulong_t *pState )
  Description: Set the dhcp relay module start switch
  Input: ulong_t ulState  - start switch value
  Output: None
  Return: Success OK, Failure ERROR
  Other: Provided for use by MIB processing functions
*******************************************************************************/
STATUS dhcprelay_get_enble_state( ulong_t *pState )
{
    if ( g_bDhcpRelayEnable == TRUE)
    {
        *pState = DHCP_ENABLE;
    }
    else
    {
        *pState = DHCP_DISABLE;
    }
    return OK;
}

void dhcprelayIpHeaderCreate(
    int			proto,		/* protocol number	*/
    struct in_addr *	pSrcAddr,	/* source ip address 	*/
    struct in_addr *	pDstAddr,	/* dest ip address	*/
    struct sal_ip *		    pih,		/* internet header 	*/
    int			length		/* datagram size 	*/
    )
{
    					/* fill in the IP header */
    pih->ip_v = IPVERSION;
    pih->ip_hl = (sizeof (struct sal_ip) >> 2) & 0xf;
    pih->ip_len	= osal_htons ((u_short) length);
    pih->ip_id	= 0;//(u_short) (tickGet () & 0xffff);
    pih->ip_ttl	= MAXTTL;
    pih->ip_p	= (u_char) proto;
    pih->ip_src.s_addr	= pSrcAddr->s_addr;
    pih->ip_dst.s_addr	= pDstAddr->s_addr;

    pih->ip_sum	= 0;	/* zero out the checksum while computing it */
    pih->ip_sum = 0;//checksum ((u_short *) pih, (pih->ip_hl << 2));
}

/*******************************************************************************
  Function: STATUS dhcprelay_send_broadcast_to_client
  Description: Send a broadcast meesage to the clientt. This function was added
               because sending broadcast messages using sockets failed
  Input: DHCP_HEAD_S *pDhcp - message header pointer
         ulong_t ulDhcpLen  - message length
         ulong_t ulIfAddr - interface IP address
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcprelay_send_broadcast_to_client ( ulong_t ulIfAddr,
    DHCP_HEAD_S *pDhcp, ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulPort,
    ulong_t ulGatewaySw)
{
#define DHCP_MAX_PKT_LEN 1536
    int                      iBuflen, iRet;
    struct ether_header     *eth;
    char                    *pTmp;
    struct in_addr           dstAddr;
    struct in_addr           srcAddr;
    struct sal_ip            iphead;
    struct udphdr            udphead;
    //SMD_PKT_INFO             pktInfo;
    ushort_t                 szPkt[DHCP_MAX_PKT_LEN/sizeof(ushort_t) + 1];
    char                     destMac[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	int sock;
	sal_sockaddr_in sin;
    int32 rc = 0;

    /*Length judgement*/
    if(ulDhcpLen > DHCP_MAX_LEN)
    {
        DHCP_SERVER_DBG ("DHCP Relay: Too long dhcp packet, length is %u", ulDhcpLen);
        return ERROR;
    }

    iBuflen = ulDhcpLen + DHCP_IP_HEAD_LEN + DHCP_UDP_HEAD_LEN + sizeof(struct ether_header);
    if ( iBuflen > DHCP_MAX_PKT_LEN )
    {
        DHCP_SERVER_DBG ("DHCP Relay: Too long ethernet packet, length is %d", iBuflen);
        return ERROR;
    }

    /*Message structure*/
    bzero((char *)&iphead, sizeof(struct sal_ip));
    bzero((char *)&udphead, sizeof(struct udphdr));

    /*Constructing UDP header*/
    udphead.dest = osal_htons ( DHCPCLIENT_PORT );
    udphead.source = osal_htons ( DHCPSERVER_PORT );
    udphead.len = osal_htons((ushort_t)(DHCP_UDP_HEAD_LEN + ulDhcpLen));
    udphead.check = 0;

    /*Constructing IP header*/
    dstAddr.s_addr = osal_htons ( INADDR_BROADCAST );
    srcAddr.s_addr = osal_htons ( ulIfAddr );

    /*Determine whether the response message should be broadcast or unicast based on flags*/
    if ( (0x8000 & pDhcp->usflag) == 0 )
    {
        /*Unicast message, the destination IP and daetination MAC address
        need to be modified*/
        dstAddr.s_addr = pDhcp->ulyiaddr;
        memcpy(destMac, pDhcp->szchaddr, 6);
    }

    /* Copy the destination and source MAC addresses */
    eth = (struct ether_header *)szPkt;
    memcpy(eth->ether_dhost, destMac, 6);
    //getSysMacAdress(eth->ether_shost);
    eth->ether_type = osal_htons( ETHERTYPE_IP );

    pTmp = (char *)szPkt;
    pTmp += sizeof(struct ether_header);

    dhcprelayIpHeaderCreate(IPPROTO_UDP, &srcAddr, &dstAddr, &iphead, DHCP_IP_HEAD_LEN + DHCP_UDP_HEAD_LEN + ulDhcpLen);
    bcopy ( (char *)&iphead, (char *)pTmp, DHCP_IP_HEAD_LEN );
    bcopy ( (char *)&udphead, (char *)(pTmp + DHCP_IP_HEAD_LEN), DHCP_UDP_HEAD_LEN );
    bcopy ( (char *)pDhcp, (char *)(pTmp + DHCP_IP_HEAD_LEN + DHCP_UDP_HEAD_LEN), ulDhcpLen );
#if 0
    /*Message sending*/
    memset(&pktInfo, 0, sizeof(SMD_PKT_INFO));
    pktInfo.ulFlags = SMD_PKT_UNTAG;

	iRet = smdVDPktSendSw((uchar_t *)szPkt, iBuflen, &pktInfo, ulGatewaySw);
    if ( VD_E_NONE != iRet )
    {
        return ERROR;
    }
#endif
	sock = sal_socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)
		return -1;

	int optval = 1;
	rc = sal_setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&optval,sizeof(int));
	if(rc!=0)
	{
		DHCP_SERVER_DBG ( "Send dhcprelay_send_broadcast_to_client packet by socket failed 1111" );
		sal_close(sock);
		return -1;
	}
	optval = 1;
	rc = sal_setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval,sizeof(int));
	if(rc!=0)
	{
		sal_close(sock);
		return -1;
	}

	osal_memset(&sin, 0, sizeof(sin));
	sin.sin_family = SAL_AF_INET;
	sin.sin_addr.s_addr = osal_htonl(ulIfAddr);
	sin.sin_port = htons(DHCPSERVER_PORT);
#ifdef HAVE_SA_LEN
	sin.sin_len = sizeof(sin);
#endif

	rc = sal_bind(sock, (sal_sockaddr *)&sin, sizeof(sin));
	if ( rc != OK )
	{
		DHCP_SERVER_DBG("Bind g_Dhcp sock Error rc %d!", rc);
		sal_close(sock);
		return -1;
	}

	/* Set destination address. */
	osal_memset(&sin, 0, sizeof(sal_sockaddr_in));
	sin.sin_family = SAL_AF_INET;
#ifdef HAVE_SIN_LEN
	sin.sin_len = sizeof(sal_sockaddr_in);
#endif /* HAVE_SIN_LEN */

	sin.sin_port = osal_htons(DHCPCLIENT_PORT);
	sin.sin_addr.s_addr = osal_htonl(INADDR_BROADCAST);


	rc = dhcpsock_sendto(sock, (void *)pDhcp, ulDhcpLen, 0, &sin, sizeof(sin));
	if ( rc == ERROR )
	{
		DHCP_SERVER_DBG ( "Send dhcprelay_send_broadcast_to_client packet by socket failed " );
	}
	DHCP_SERVER_DBG ( "Send dhcprelay_send_broadcast_to_client packet by socket ok %d", rc);

	sal_close(sock);

    return OK;
}

/*******************************************************************************
  Function: STATUS dhcprelay_check_ip_valid
  Description: Checks the validity of the IP address
  Input: ulong_t ulIpAddr - checks the IP address
  Output: None
  Return: OK if the IP address is valid, ERROR if the IP address invalid
*******************************************************************************/
STATUS dhcprelay_check_ip_valid( ulong_t ulIpAddr )
{
    uchar_t IpFirstByte;

    if ( ulIpAddr == 0 )
    {
        return OK;
    }

    IpFirstByte = (uchar_t)(ulIpAddr>>24);
    if(127 == IpFirstByte)
    {
        return ERROR;
    }
    /*
    Only subnet mask IP addresses are supported
    if( ((IpFirstByte > 0)&&(IpFirstByte < 127)&&(ulMask < 0xFF000000)) 
        ||((IpFirstByte >= 128)&&(IpFirstByte <= 191)&&(ulMask < 0xFFFF0000))
        ||((IpFirstByte >= 192)&&(IpFirstByte <= 223)&&(ulMask < 0xFFFFFF00))
        ||(IpFirstByte >= 224))
    */

    /*Supports subnet mask and supernet IP address checking to determine that the 
    mask length of A.B.C class adresses cannot be less than 8 bits*/
    if(  ((IpFirstByte == 0) ||(IpFirstByte > 223)) /*��A.B.C���ַ*/
          ||(IpFirstByte >= 224))     /*D.E���ַ�Ƿ�*/
    {
        return ERROR;
    }

    return OK;
}

STATUS dhcprelay_option60_process(ulong_t ulIfNo, uchar_t* pucOption60, uchar_t ucOption60Len,ulong_t* pulgateway,ulong_t *pserver)
{
    int i = 0;
    int matchlen = 0;

    for (i = 0; i < DHCPINTERFACEMAXNUM; i++)
    {
        if (dhcpOption60MatchEntry[ulIfNo-1][i].matchType == OPTION60_EQUALS)
        {
            if (dhcpOption60MatchEntry[ulIfNo-1][i].contentlen == ucOption60Len)
            {
                if (0 == memcmp(pucOption60, dhcpOption60MatchEntry[ulIfNo-1][i].matchContent, dhcpOption60MatchEntry[ulIfNo-1][i].contentlen ))
                {
                    *pulgateway = dhcpOption60MatchEntry[ulIfNo-1][i].gateway;
                    matchlen = dhcpOption60MatchEntry[ulIfNo-1][i].contentlen ;


                    if (dhcpOption60MatchEntry[ulIfNo-1][i].servergroup >= 1)
                    {
                        *pserver = g_dhcpgroup[dhcpOption60MatchEntry[ulIfNo-1][i].servergroup-1].serverIp;
                    }

                    break;
                }
            }
        }
        else if (dhcpOption60MatchEntry[ulIfNo-1][i].matchType == OPTION60_STARTS_WITH)
        {
            if (dhcpOption60MatchEntry[ulIfNo-1][i].contentlen <= ucOption60Len)
            {
                if (0 == memcmp(pucOption60, dhcpOption60MatchEntry[ulIfNo-1][i].matchContent, dhcpOption60MatchEntry[ulIfNo-1][i].contentlen ))
                {
                    if (*pulgateway == 0 || dhcpOption60MatchEntry[ulIfNo-1][i].contentlen > matchlen)
                    {
                        *pulgateway = dhcpOption60MatchEntry[ulIfNo-1][i].gateway;
                        matchlen = dhcpOption60MatchEntry[ulIfNo-1][i].contentlen;

                        if (dhcpOption60MatchEntry[ulIfNo-1][i].servergroup >= 1)
                        {
                            *pserver = g_dhcpgroup[dhcpOption60MatchEntry[ulIfNo-1][i].servergroup-1].serverIp;
                        }
                        else
                        {
                            *pserver = 0;
                        }
                    }
                }
            }
        }
    }

    return OK;
}

STATUS dhcprelay_add_gateway(ulong_t ulIfaddr, DHCP_HEAD_S *pDhcp ,DHCP_INTERFACE_PARA *ifPara,DHCP_OPTION_S * pDhcpOption,ulong_t *pserver)
{
    ulong_t ulIfNo = ifPara->dhcp_sw;
    ulong_t gateway = 0;

    /*If the message contains option 60, first use option 60 to find the gateway*/
    if (pDhcpOption->pucOption60)
    {
	    //osal_printf("\r\n %s %d ulIfaddr %x", __func__, __LINE__, ulIfaddr);
        dhcprelay_option60_process(ulIfNo, pDhcpOption->pucOption60, pDhcpOption->ucOption60Len, &gateway, pserver);
    }
	//osal_printf("\r\n %s %d", __func__, __LINE__);

    /*If option 60 is not found, the primary IP address is used as the gateway*/
    if (!gateway)
    {
	    //osal_printf("\r\n %s %d", __func__, __LINE__);
        gateway = ulIfaddr;
    }

    if (!gateway)
    {
	    //osal_printf("\r\n %s %d", __func__, __LINE__);
        return ERROR;
    }
    else
    {
	    //osal_printf("\r\n %s %d", __func__, __LINE__);
        pDhcp->ulgiaddr = htonl(gateway);
        return OK;
    }
}

ulong_t dhcp_index2IpAddr( ulong_t  *ipIndex )
{
    ulong_t ipAddr;

    ipAddr = 0;
    ipAddr |= (ipIndex[ 0 ] & 0xFF) << 24;
    ipAddr |= (ipIndex[ 1 ] & 0xFF) << 16;
    ipAddr |= (ipIndex[ 2 ] & 0xFF) <<  8;
    ipAddr |= (ipIndex[ 3 ] & 0xFF) <<  0;

    return ipAddr;
}

/*******************************************************************************
  Function: void dhcp_debug_printf(char *exp,.....)
  Description: Debug print function for the DHCP module
  Input: None
  Output: None
  Return: None
*******************************************************************************/
#if 0
void dhcp_debug_printf( char *exp, ...)
{
    if(exp == NULL)
    {
       return ;
    }

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
        syslogSendInfo(MID_DHCP, DEBUGGING, "dhcprelay", debugStr, debugStr, FALSE, FALSE, NO_WAIT);
    }

}
#endif

/*******************************************************************************
  Function: STATUS dhcp_record_all_opt
  Descritpion: Retrieves the variable-length option field of a DHCP message
               and records all possible options. This is e very general function
  Input: DHCP_HEAD_S *pDhcp - message header pointer
               lOptionLen - length of the variable-length option fiald in the DHCP message
  Output: pDhcpOpt - pointer of the location where various options are recorded
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcp_record_all_opt(DHCP_HEAD_S *pDhcp, ulong_t lOptionLen,DHCP_OPTION_S *pDhcpOpt)
{
    ulong_t  ulMagicCookie;
    uchar_t  *popt;
    ulong_t   lDhcpOptionLen;
    uchar_t  *poptions;
    int  i;
    uchar_t  a_ucCount[DHCP_OPTION_COUNT];

    if((pDhcp == NULL)||(pDhcpOpt == NULL))
    {
        DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: Invalid paramter");
        return ERROR;
    }

    /*Initialize the DHCP_OPTION_S structure*/
    memset ( (char *)pDhcpOpt, 0, sizeof(DHCP_OPTION_S) );

    poptions     = (uchar_t *)(pDhcp+1);
    lDhcpOptionLen = lOptionLen;

    popt = poptions;

    /*Check if the first four bytes "options" field are "magic cookie": 0x63,0x82,0x53,0x63,
    if not, return directly*/
    memcpy(&ulMagicCookie, popt, 4);
    if( ulMagicCookie != htonl(MAGIC_COOKIE) )
    {
        DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: Unknown magic cookie");
        return ERROR;
    }

    for (i = 0; i < DHCP_OPTION_COUNT; a_ucCount[i] = 0, i++) ;

    poptions     += 4;
    lDhcpOptionLen -= 4;

    /*Whether the end of the"option" field has been reached*/
    while (*poptions!= ENDOFF_OPTIONS)
    {
        uchar_t optionCode, optionLen;

        optionCode    = *poptions++;
        /*Improve the length judgement of the option field*/
        if (optionCode == 0)
        {
            optionLen     = 0;
            lDhcpOptionLen --;
        }
        else
        {
            optionLen     = *poptions++;
            lDhcpOptionLen -= 2;
        }

        /*Determine whether the length of the variable-length field following 
        the DHCP message is sufficient to ensure that the variable-length field
         does not exceed the length of the current DHCP message*/
        if (lDhcpOptionLen <= (long)optionLen)
        {
            /*Rewritten to support BOOTP messages for Ephones: the BOOTP message 
            may not have a terminating character ("0xFF"). When the "DHCP message type"
            option is missing, it is a BOOTP message*/
            if (pDhcpOpt->pucDHCPMsgType == NULL)
            {
                DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: no 'DHCP message type' option");
                goto optionnumchk;
            }

            DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: no END tag in this packet");
            return ERROR;
        }

        /*Note: this options are not required, so it is normal that they are not found*/
        switch (optionCode)
        {
            /*1."Subnet mask" option, code = 0x01*/
            case DHCP_SUBNETMASK_CODE :
                if (optionLen != 4)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Subnet mask' option is not 4 bytes");
                    return ERROR;
                }

                a_ucCount[0]++;
                pDhcpOpt->pulSubnetMask = (ulong_t *)poptions;
                break;

            /*2."Router" option, code = 0x03*/
            case DHCP_ROUTERIP_CODE :
                /*Determine whether the length is a multiple of 4*/
                if ((optionLen & 0x03) != 0)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Router' option is not a multiple of 4 bytes");
                    return ERROR;
                }

                a_ucCount[1]++;
                pDhcpOpt->pulRouterIp   = (ulong_t *)poptions;
                pDhcpOpt->ucRouterIpLen = optionLen;
                break;

            /*3."DNS SERVER" option, code = 0x06*/
            case DHCP_DNSIP_CODE :
                /*Determine whether the length is a multiple of 4*/
                if ((optionLen & 0x03) != 0)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'DNS server' option is not a multiple of 4 bytes");
                    return ERROR;
                }

                a_ucCount[2]++;
                pDhcpOpt->pulDNSIp   = (ulong_t *)poptions;
                pDhcpOpt->ucDNSIpLen = optionLen;
                break;

            /*4."Host name" option, code = 0x0c*/
            case DHCP_HOSTNAME_CODE :
                if (optionLen < 1)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Host name' option is 0");
                    return ERROR;
                }

                a_ucCount[3]++;
                pDhcpOpt->pucHostName   = poptions;
                pDhcpOpt->ucHostNameLen = optionLen;
                break;

            /*5."Domain name" option, code = 0x0f*/
            case DHCP_DOMAINNAME_CODE :
                if (optionLen < 1)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Domain name' option is 0");
                    return ERROR;
                }

                a_ucCount[4]++;
                pDhcpOpt->pucDomainName   = poptions;
                pDhcpOpt->ucDomainNameLen = optionLen;
                break;

            /*6."Netbios name server" option, code = 0x2c*/
            case DHCP_NBNS_CODE :
                /*Determine whether the length is a multiple of 4*/
                if ((optionLen & 0x03) != 0)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'NBNS server' option is not a multiple of 4 bytes");
                    return ERROR;
                }

                a_ucCount[5]++;
                pDhcpOpt->pulNBNSIp   = (ulong_t *)poptions;
                pDhcpOpt->ucNBNSIpLen = optionLen;
                break;

            /*7."Netbios data distribute server" option, code = 0x2d*/
            case DHCP_NBDS_CODE :
                /*Determine whether the length is a multiple of 4*/
                if ((optionLen & 0x03) != 0)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'NBDS server' option is not a multiple of 4 bytes");
                    return ERROR;
                }

                a_ucCount[6]++;
                pDhcpOpt->pulNBDSIp   = (ulong_t *)poptions;
                pDhcpOpt->ucNBDSIpLen = optionLen;
                break;

            /*8."Netbios node type" optrion, code = 0x2e*/
            case DHCP_NBNT_CODE :
                if (optionLen != 1)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'NB node type' option is not 1 byte");
                    return ERROR;
                }

                a_ucCount[7]++;
                pDhcpOpt->pucNBNodeType = poptions;
                break;

            /*9."Netbios scope" option, code = 0x2f*/
            case DHCP_NBSCOPE_CODE :
                if (optionLen < 1)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'NB scope' option is 0");
                    return ERROR;
                }

                a_ucCount[8]++;
                pDhcpOpt->pucNBScope   = poptions;
                pDhcpOpt->ucNBScopeLen = optionLen;
                break;

            /*10."Requested Ip" option, code = 0x32*/
            case DHCP_REQUESTEDIP_CODE :
                if (optionLen != 4)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Requested Ip' option is not 4 bytes");
                    return ERROR;
                }

                a_ucCount[9]++;
                pDhcpOpt->pulReqIp = (ulong_t *)poptions;
                break;

            /*11."Ip address lease time" option, code=0x33*/
            case DHCP_IPLEASETIME_CODE:
                if (optionLen != 4)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Ip Address lease time' option is not 4 bytes");
                    return ERROR;
                }

                a_ucCount[10]++;
                pDhcpOpt->pulLeaseTime = (ulong_t *)poptions;/*IP lifetime in seconds*/
                break;

            /*12."DHCP message type" option, code=0x35*/
            /*When this option is not present, error handling will be performed in the
            function that calls this function*/
            case DHCP_MESSAGETYPE_CODE:
                /*"DHCP Message Type"is the length of the option 1*/
                if (optionLen != 1)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'DHCP message type' option is not 1 byte");
                    return ERROR;
                }

                a_ucCount[11]++;
                pDhcpOpt->pucDHCPMsgType = poptions;
                break;

            /*13."Server Identifier" option, code=0x36*/
            case DHCP_SERVERIP_CODE:
                if (optionLen != 4) /*Is futher processing required?*/
                {
                     DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Server Identifier' option is not 4 bytes");
                     return ERROR;
                }

                a_ucCount[12]++;
                pDhcpOpt->pulDHCPServerIp = (ulong_t *)poptions;
                break;

            /*14."Requested parameter list" option, code = 0x37*/
            case DHCP_REQPARAM_CODE :
                if (optionLen < 1)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Requested parameter list' option is 0");
                    return ERROR;
                }

                a_ucCount[13]++;
                pDhcpOpt->pucReqParamList   = poptions;
                pDhcpOpt->ucReqParamListLen = optionLen;
                break;

            /*15."Renew time" option, code = 0x3a*/
            case DHCP_RENEWTIME_CODE :
                if (optionLen != 4)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Renew time' option is not 4 bytes");
                    return ERROR;
                }

                a_ucCount[14]++;
                pDhcpOpt->pulRenewTime = (ulong_t *)poptions;/*in seconds*/
                break;

            /*16."Rebind time" option, code = 0x3b*/
            case DHCP_REBINDTIME_CODE :
                if (optionLen != 4)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Rebind time' option is not 4 bytes");
                    return ERROR;
                }

                a_ucCount[15]++;
                pDhcpOpt->pulRebindTime = (ulong_t *)poptions; /*in seconds*/
                break;

            /*17."Client Id" option, code = 0x3d*/
            case DHCP_CLIENTID_CODE :
                if (optionLen < 2)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Client Id' option is less than 2 bytes");
                    return ERROR;
                }

                a_ucCount[16]++;
                pDhcpOpt->pucClientId   = poptions;
                pDhcpOpt->ucClientIdLen = optionLen;
                break;

            /*18."Vendor information" option, code = 0x29*/
            case DHCP_VENDORINFO_CODE :
                if (optionLen < 1)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Vendor Information' option is 0");
                    return ERROR;
                }

                a_ucCount[17]++;
                pDhcpOpt->pucVendorOption   = poptions;
                pDhcpOpt->ucVendorOptinLen  = optionLen;
                break;

            /*19."Vendor class identifier" option, code = 60*/
            case DHCP_OPTION60_CODE :
                if (optionLen < 1)
                {
                    DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: The length of 'Vendor class identifier' option is 0");
                    return ERROR;
                }

                a_ucCount[18]++;
                pDhcpOpt->pucOption60   = poptions;
                pDhcpOpt->ucOption60Len  = optionLen;
                break;

          /*Do nothing for other options*/
           default:
                break;
        }

        /*Skip optionLen length bytes and get the next code byte*/
        poptions     += optionLen;
        lDhcpOptionLen -= optionLen;

    }

    if (*poptions == ENDOFF_OPTIONS)
    {
        pDhcpOpt->pucEndOfOption = poptions;
    }
    /*Rewritten to support BOOTP messages for Ephone:BOOTP messages may not have a "0xFF" terminator
    When the "DHCP message type" option is missing, it is a BOOTP message*/
    else if (pDhcpOpt->pucDHCPMsgType == NULL)
    {
        DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: no 'DHCP message type' option(Pkt length correct),perhaps a BOOTP Pkt");
    }
    else
    {
        DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: no END tag");
        return ERROR;
    }

optionnumchk:
    for(i = 0; (i < DHCP_OPTION_COUNT) && (a_ucCount[i] <= 1); i++) ;

    if (i < DHCP_OPTION_COUNT)
    {
        DHCP_SERVER_RX_DBG("RecordDhcpAllOpt: Some options of this DHCP pkt occur more than once");
        return ERROR;
    }

    return OK;
}

/*******************************************************************************
  Function: STATUS dhcprelay_get_ifaddr
  Description: Get the corresponding interface address from interface sw 
  Input: ulong_t sw - interface index
  Output: ulong_t *pIfaddr - interface address
  Return: Success OK, Failure ERROR
*******************************************************************************/
int32 dhcprelay_get_ifaddr(ulong_t sw, ushort_t vid, ulong_t *ifAddr)
{
    int rc = 0;
    ulong_t  idxBuf[5] = {0};
    ulong_t  value;
    ulong_t  value_len;
    #ifdef GBN_INCLUDE_VRRP
    ulong_t VrrpIp, ulMask;
    #endif
	sys_l3_intfId_t intfid;
	sys_l3_ipv4_t   ipv4;
	sys_l3_intfMgmt_t mgmtIp;

    if (ifAddr == NULL)
    {
        return ERROR;
    }

    intfid.id = vid;
    intfid.type = sw;

	if(intfid.type == SYS_L3_INTFTYPE_MGMTVLAN)
	{
		rc = sal_l3_intfMgmt_get(&mgmtIp);
		value = osal_ntohl(mgmtIp.ipv4.addr);
	}
	else 
	{
	    /*Get the primary IP address corresponding to sw+vid*/
	    rc = sal_l3_intfIPv4_get(&intfid, &ipv4);
	    value = osal_ntohl(ipv4.addr);
	}
    if ( rc == SYS_ERR_FAILED )
    {
        DHCP_SERVER_DBG ( "Can't find the interface ip address by sw %d+ vid %d", sw, vid);
        return SYS_ERR_FAILED;
    }

    #ifdef GBN_INCLUDE_VRRP
    miscIpAddr2IpIndex( value,  &idxBuf[1] );
    rc = ccdGet ( "gbnL3Mib:gbnL3IpSubnetMask" , idxBuf , 5 , &ulMask, &value_len);
    if ( rc == ERROR )
    {
        DHCP_SERVER_DBG ( "Can't find the interface ip mask by sw %d", sw);
        return SYS_ERR_FAILED;
    }

    /*Determine whether the primary IP segment of the interface running VRRP.
    If so, replace it with a virtual IP*/
    if ( OK == vrrpDstToVip(idxBuf[0], value, ulMask, &VrrpIp) )
    {
        value = VrrpIp;
    }
    #endif

    *ifAddr = value;

    return SYS_ERR_OK;
}

int dhcprelayEnableCfg(ulong_t enable)
{
	if (g_bDhcpRelayEnable == enable)
		return OK;
	
	set_socket_status(enable);
	g_bDhcpRelayEnable = enable;
    return OK;
}

int dhcprelayServerIPCfg(uint32 uiGroupId, sys_ipv4_t ipv4)
{
	if (uiGroupId > DHCP_MAX_GROUP_NUM)
	{
        return ERROR;
    }

	g_dhcpgroup[uiGroupId - 1].serverIp = ipv4;

    return OK;
}

int dhcprelayIfGroupCfg(ulong_t ulSw, BOOL bAdd, ulong_t ulGroupNum)
{
    unsigned int i, j;
    ulong_t cont = 0;
	//DHCP_SERVER_CMD_DBG("sw=%d group=%d",ulSw, ulGroupNum);

    if ( TRUE == bAdd )
    {
        for ( i = 0; i < DHCP_MAX_INTERFACE_NUM; i++ )
        {
            if ( g_dhcpInterface[i].vid == ulSw )
            {
				for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
				{
					if ( g_dhcpInterface[i].groupNo[j] == ulGroupNum )
		                return OK;
				}

				for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
				{
					if ( g_dhcpInterface[i].groupNo[j] == DHCPEMPTYGROUP )
					{
						g_dhcpInterface[i].groupNo[j] = ulGroupNum;
		                return OK;
					}
				}

				if (j == DHCPINTERFACEMAXNUM)
					return ERROR;
            }
        }

        for ( i = 0; i < DHCP_MAX_INTERFACE_NUM; i++ )
        {
            if ( g_dhcpInterface[i].vid == 0 )
            {
                g_dhcpInterface[i].vid = ulSw;

				for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
				{
					if ( g_dhcpInterface[i].groupNo[j] == DHCPEMPTYGROUP )
					{
						g_dhcpInterface[i].groupNo[j] = ulGroupNum;
		                return OK;
					}
				}

				if (j == DHCPINTERFACEMAXNUM)
					return ERROR;
            }
        }
    }
    else
    {

        for ( i = 0; i < DHCP_MAX_INTERFACE_NUM; i++ )
        {
            if ( g_dhcpInterface[i].vid == ulSw )
            {
            	for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
				{
					if ( g_dhcpInterface[i].groupNo[j] != DHCPEMPTYGROUP )
						cont++;
						
				}
				for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
				{
					if ( g_dhcpInterface[i].groupNo[j] == ulGroupNum )
					{
						g_dhcpInterface[i].groupNo[j] = DHCPEMPTYGROUP;
						if(cont == 1)
						{
							g_dhcpInterface[i].vid = 0;
						}
		                return OK;
					}
				}

				if (j == DHCPINTERFACEMAXNUM)
					return ERROR;
            }
        }
    }

    return ERROR;
}

int dhcprelayIfGroupOp(DATA_OP_ENT op, ulong_t *pulSw, ulong_t* pVid, ulong_t groupNo[DHCPINTERFACEMAXNUM])
{
    unsigned int             i, j;
    ulong_t                  ulSw;

    if ( (DATA_OP_GET != op) && (DATA_OP_GETNEXT != op) )
    {
        return ERROR;
    }

    ulSw = *pulSw;

    if ( DATA_OP_GETNEXT == op )
    {
        for ( i = (ulSw + 1); i < DHCP_MAX_INTERFACE_NUM; i++ )
        {
            for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
            {
                if ( DHCPEMPTYGROUP != g_dhcpInterface[i - 1].groupNo[j] )
                {
                	//DHCP_SERVER_CMD_DBG("sw=%d group1=%d group2=%d group3=%d",i, g_dhcpInterface[i - 1].groupNo[j], g_dhcpInterface[i - 1].groupNo[j+1], g_dhcpInterface[i - 1].groupNo[j+2]);
                    *pulSw = i;
                    *pVid = g_dhcpInterface[i - 1].vid;
                    memcpy(groupNo, g_dhcpInterface[i - 1].groupNo, sizeof(ulong_t) * DHCPINTERFACEMAXNUM);
                    return OK;
                }
            }
        }

        return ERROR;
    }
    else
    {
        for ( i = (ulSw + 1); i < DHCP_MAX_INTERFACE_NUM; i++ )
        {
			if(g_dhcpInterface[i - 1].vid == *pVid)
			{
				//DHCP_SERVER_CMD_DBG("VID=%d", g_dhcpInterface[i - 1].vid);
				memcpy(groupNo, g_dhcpInterface[i - 1].groupNo, sizeof(ulong_t) * DHCPINTERFACEMAXNUM);
			}
        }
 
        return OK;
    }

    return ERROR;
}

int dhcprelayCheckGroupUsed(ulong_t ulGroupNum, ulong_t *pbUsed, ulong_t* vid)
{
    unsigned int             i, j;

    *pbUsed = FALSE;
    
    for(i = 1;i <= DHCP_MAX_INTERFACE_NUM;i++)
    {
        /*Being used by an interface*/
        for (j = 0; j < DHCPINTERFACEMAXNUM; j++)
        {
            if (g_dhcpInterface[i-1].groupNo[j] != 0 && g_dhcpInterface[i-1].groupNo[j] == ulGroupNum)
            {
                *pbUsed = TRUE;
                *vid = g_dhcpInterface[i-1].vid;
                //DHCP_SERVER_CMD_DBG("vid=%d",i, g_dhcpInterface[i-1].vid);
                return OK;
            } 
        }
    }
    
    return OK;
}

int getSysMacAdress(uchar_t *pMac)
{
    memcpy(pMac, g_bDhcpRelaySysMac, 6);
    return OK;
}

int dhcprelaySystemMacCfg(uchar_t *pMac)
{
    memcpy(g_bDhcpRelaySysMac, pMac, 6);

    return OK;
}


int dhcprelaySystemOidCfg(ulong_t oid)
{
    g_bDhcpRelaySysOid = oid;

    return OK;
}

int dhcprelayHostnameGet(uchar_t *pHostname)
{
	memcpy(pHostname, g_bDhcpRelayHostname, strlen(g_bDhcpRelayHostname));
    return OK;
}

int dhcprelayHostnameCfg(uchar_t *pHostname)
{
	memset(g_bDhcpRelayHostname, 0, strlen(g_bDhcpRelayHostname));
	memcpy(g_bDhcpRelayHostname, pHostname, strlen(pHostname));
    return OK;
}

#if 0
int dhcprelayPortlistEnableGet(vd_portlist_t portlistEnabled)
{
    ulong_t                  ulPort;

    VD_PORTLIST_CLEAR(portlistEnabled);
    for ( ulPort = 1; ulPort <= gul_max_port_num; ulPort++ )
    {
        if ( TRUE == gbDHCPRelayPortEnable[ulPort - 1] )
        {
            VD_PORTLIST_PORT_ADD(portlistEnabled, ulPort);
        }
    }

    return OK;
}
#endif
int dhcprelayPortEnableSet(ulong_t ulPort, ulong_t bEnable)
{
    if ( (ulPort < 0) || (ulPort > SYS_LOGICPORT_NUM_MAX) )
    {
        return ERROR;
    }

    gbDHCPRelayPortEnable[ulPort] = bEnable;

    return OK;
}

int dhcpRelayMaxHopsSet(ulong_t ulHops)
{
    dhcpMaxHops = ulHops;
    return OK;
}

int dhcpRelayMaxHopsGet(ulong_t *pulHops)
{
    *pulHops = dhcpMaxHops;
    return OK;
}

STATUS dhcp_otion82_process
(
	DHCP_HEAD_S *pDhcp,
	ulong_t ulMsgType,
	ulong_t ulDhcpLen,
	ulong_t *ulNewDhcpLen,
	DHCP_INTERFACE_PARA *ifPara
)
{
#if 1
#define HOSTNAME_MAXLEN 128
    uchar_t  *pPacket = NULL;
    uchar_t  *pOption82 = NULL;
    uchar_t  *pOption82NextOption = NULL;
    uchar_t  Option82Len = 0;
    uchar_t  SysMac[6] = {0};
    uchar_t  sysOid[4];
    uchar_t  option82[255] = {0}, *p = option82;
    ulong_t  ulIfaddr;
    ulong_t  nSlot = 0, nNumber = 0, nDevice = 0;
    uchar_t  *p1,*p11,*p111,*p12,*p121,*p19,*p191,*p1911; 
    int      new82len = 0;
    int      i = 0;
    int      olddhcplen = ulDhcpLen;
    static char padding[DHCP_MAX_LEN - DHCP_HEAD_LEN] = {0};
    int      oldpadlen = 0;
    ushort_t ustemp = 0;
    BOOL append = FALSE;
    BOOL old2new = FALSE;

    char szHostName[HOSTNAME_MAXLEN + 1] = { 0 };

	dhcprelayHostnameGet(szHostName);

    if (NULL != dhcpoption82_get_avlnode(ifPara->dhcp_vid))
    {
        return dhcp_otion82_vlan_process(pDhcp, ulMsgType, ulDhcpLen, ulNewDhcpLen, ifPara);
    }
    else if (dhcpOption82Policy_port[ifPara->dhcp_port].option82Enabled == FALSE)
    {
        return OK;
    }

    sysOid[0] = (uchar_t)(g_bDhcpRelaySysOid >> 24);
    sysOid[1] = (uchar_t)(g_bDhcpRelaySysOid >> 16);
    sysOid[2] = (uchar_t)(g_bDhcpRelaySysOid >>  8);
    sysOid[3] = (uchar_t)(g_bDhcpRelaySysOid >>  0);

    memset(option82, 0, sizeof(option82));

    #ifdef GBN_INCLUDE_STACK_ISF
    if ( (TRUE == isf_stack_enable()) && (FALSE == isf_stack_is_master()) )
    {
        isf_stack_master_mac(SysMac);
    }
    else
    {
    #endif
    getSysMacAdress((char *)SysMac);
    #ifdef GBN_INCLUDE_STACK_ISF
    }
    #endif

	#if 0
    smdVDPortSnmpToUser(ifPara->dhcp_port + 1, &nDevice, &nSlot, &nNumber);
	#endif
	nNumber = ifPara->dhcp_port + 1;

    *ulNewDhcpLen = ulDhcpLen;

    pPacket = (uchar_t *)(pDhcp+1);
    pPacket += 4; /*point to the first option*/

    /*It is assumed here that the content of the message is normal
    and has been verified in dhcp_record_all_opt*/
    while (*pPacket != ENDOFF_OPTIONS)
    {
        if( *pPacket == DHCP_OPTION82_CODE )
        {
            Option82Len = *(pPacket+1)+2;
            pOption82 = pPacket;
            pOption82NextOption = pOption82+Option82Len;
        }
        if( *pPacket == 0 )
        {
            pPacket++;
        }
        else
        {
            pPacket += (*(pPacket+1))+2;
        }
    }

    /*֮After that, pPacke points to end option*/

    /*Store the padding of the original message, if there is padding*/
    if ( (oldpadlen = (ulDhcpLen-(pPacket-((uchar_t *)pDhcp)+1))) > 0)
    {
        memcpy(padding, pPacket + 1, oldpadlen);
    }

    switch (ulMsgType)
    {
        case DHCP_DISCOVER_TYPE:
        case DHCP_REQUEST_TYPE:
            if ( pOption82 != NULL )
            {
                if (dhcpOption82Policy_port[ifPara->dhcp_port].opion82Strategy == OPTION82STRATEGY_DROP)
                {
                    dhcp_debug_printf ( "\r\n dhcp option 82 strategy = drop, drop dhcp request message " );
                    return ERROR;
                }
                else if (dhcpOption82Policy_port[ifPara->dhcp_port].opion82Strategy == OPTION82STRATEGY_KEEP)
                {
                    return OK;
                }
                else
                {
                    if (dhcpOption82Policy_port[ifPara->dhcp_port].opion82Strategy ==OPTION82STRATEGY_APPEND_HOSTNAME
                    || dhcpOption82Policy_port[ifPara->dhcp_port].opion82Strategy ==OPTION82STRATEGY_APPEND_HOSTNAME_IP)
                    {
                        append = TRUE;
                        if(Option82Len < sizeof(option82))
                        {
                            memcpy(option82,pOption82,Option82Len);
                        }
                        else
                        {
                            dhcp_debug_printf ( "\r\n local option82[255] too small, cannot copy, not appeded " );
                            return OK;
                        }
                    }

                    old2new = TRUE;
                }
            }

            if(append)
            {
                int suboptionlen = Option82Len - 2;
                uchar_t *popt9 = NULL;
                uchar_t * q = p + 2;

                p1 = p+1; 

                while(suboptionlen >= 1 )
                {
                    if( *q == 9 )
                    {
                        popt9 = q;
                    }

                    if( *q == 0 )
                    {
                        q++;
                        suboptionlen--;
                    }
                    else
                    {
                        if(suboptionlen >= 2)
                        {
                            suboptionlen -= ((*(q+1))+2);
                            q += (*(q+1))+2;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                if(suboptionlen != 0) 
                {
                    dhcp_debug_printf ( "\r\n option 82 sub option format not right, not appeded " );
                    return OK;
                }
                else
                {
                    if(popt9)
                    {
                        int N = *(popt9+1);
                        p19=popt9+1;
                        if(N > 4)
                        {
                            p = popt9+2;/*Points to the four-byte enterprise ID */
                            if(p[0]==sysOid[0] && p[1]==sysOid[1] && p[2]==sysOid[2] && p[3]==sysOid[3])
                            {
                                int N1 = *(p+4);
                                int N1_left = N1;
                                int si = 0;
                                p += 4;
                                p191 = p++; 

                                while(N1_left >= 1 )
                                {
                                    if( *p == 0 )
                                    {
                                        p++;
                                        N1_left--;
                                    }
                                    else
                                    {
                                        if(N1_left >= 2)
                                        {
                                            ++si;
                                            N1_left -= ((*(p+1))+2);
                                            p += (*(p+1))+2;
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                }
                                if(N1_left != 0) 
                                {
                                    dhcp_debug_printf ( "\r\n option 82 sub option 9 format not right or not henan format, not appeded " );
                                    return OK;
                                }
                                else 
                                {
                                    uchar_t tmp[200];
                                    int lenleft = Option82Len-(p-option82); 
                                    if(lenleft>0 && lenleft < sizeof(tmp))
                                    {
                                          memcpy(tmp,p,lenleft);
                                    }
                                    else if(lenleft == 0)
                                    {
                                        /*do nothing*/
                                    }
                                    else
                                    {
                                        dhcp_debug_printf ( "\r\n option 82 tmp[200] too small cannot copy, not appeded " );
                                        return OK;
                                    }

                                    *p++ = ++si;
                                    p1911 = p++; 
                                    strcpy((char*)p, szHostName);
                                    p += strlen(szHostName);
                                    *p1911 = strlen(szHostName);
                                    *p191 += 2+strlen(szHostName);
                                    *p19 += 2+strlen(szHostName);
                                    *p1 += 2+strlen(szHostName); 
                                    new82len = Option82Len + 2+strlen(szHostName); 

                                    if(dhcpOption82Policy_port[ifPara->dhcp_port].opion82Strategy==OPTION82STRATEGY_APPEND_HOSTNAME_IP)
                                    {
                                        *p++ = ++si;
                                        p1911 = p++; 
                                        ulIfaddr = 0;
                                        dhcprelay_get_ifaddr(ifPara->dhcp_sw, ifPara->dhcp_vid, &ulIfaddr);
                                        memcpy(p, &ulIfaddr, 4);
                                        p += 4;
                                        *p1911 = 4;
                                        *p191 += 2+4;
                                        *p19 += 2+4;
                                        *p1 += 2+4; 
                                        new82len += 2+4;  
                                     }
                                     if(lenleft)
                                        memcpy(p,tmp,lenleft);
                                }
                            }
                            else
                            {
                                //dhcp_debug_printf ( "\r\n option 82 sub option 9 enterprise ID is not 0x%08x, not henan format, not appeded ", OEMOID_OEM);
                                return OK;
                            }
                        }
                        else
                        {
                            dhcp_debug_printf ( "\r\n option 82 sub option 9 N<=4, not henan format, not appeded " );
                            return OK;
                        }
                    }
                    else 
                    {
                        dhcp_debug_printf ( "\r\n option 82 sub option 9 not exist, not appeded " );
                        return OK;
                    }
                }
            }
            else
            {
                *p++ = 82;
                p1 = p++; 
                if(dhcpOption82Policy.option82Format == OPTION82FORMAT_HENAN)
                {

                    /*Add suboption 1, its content is the receiving port VID+receiving port for a custom string*/

                      *p++ = 1;     
                      p11  = p++;   
                      ustemp = htons(ifPara->dhcp_vid);
                      memcpy(p, &ustemp, sizeof(ustemp));
                      p += 2;
                      if (dhcpOption82Policy.option82DevEnabled == TRUE)
                      {
                          *p++ = (uchar_t)nDevice;
                      }
                      *p++ = (uchar_t)nSlot;
                      *p++ = (uchar_t)nNumber;

                      *p11 = p - p11 - 1;   

                    /*Add suboption 2, whose content is the MAC address of the relay device
                    or the switch name or a custom string*/

                      *p++ = 2;   //sub opt type 2
                      p12 = p ++ ;  //len

                      memcpy(p, SysMac, 6);
                      p += 6;

                      *p12 = p - p12-1;   //���볤��

                    /*Add suboption 9, whose content hostname*/

                    if(dhcpOption82Policy_port[ifPara->dhcp_port].opion82Strategy ==OPTION82STRATEGY_APPEND_HOSTNAME_IP)
                    {
                        *p++ = 9;     
                        p19 = p++;   
                        memcpy(p, sysOid, 4);  
                        p += 4;
                        p191 = p++;  

                        *p++ = 0x01;
                        *p++ = 0x00;

                        *p++ = 0x02;
                        p1911 = p++; 
                        strcpy((char*)p,szHostName);
                        p += strlen(szHostName);
                        *p1911 = p - p1911 -1;

                        *p++ = 0x03;
                        p1911 = p++; 
                        ulIfaddr = 0;
                        dhcprelay_get_ifaddr(ifPara->dhcp_sw, ifPara->dhcp_vid, &ulIfaddr);
                        memcpy(p, &ulIfaddr, 4);
                        p += 4;
                        *p1911 = 4/*p - p1911 -1*/;

                        *p191 = p - p191 -1;
                        *p19 = p - p19 -1;
                    }
                    else
                    {
                         *p++ = 9;     
                         p19 = p++;   
                         memcpy(p, sysOid, 4);  
                         p += 4;
                         p191 = p++;  

                         *p++ = 0x01;
                         p1911 = p++; 
                         strcpy((char*)p,szHostName);
                         p += strlen(szHostName);
                         *p1911 = p - p1911 -1;

                         *p191 = p - p191 -1;
                         *p19 = p - p19 -1;
                    }
                }
                else if(dhcpOption82Policy.option82Format == OPTION82FORMAT_NORMAL)
                {
                  /*Add suboption 1, its content is the receiving port VID + receiving port or a custom string*/

                    *p++ = 1;     //sub opt type 1
                     p11 = p++;   
                     *p++ = 0;    //circuit id type
                     p111 = p++;  

                    if(dhcpOption82Policy_port[ifPara->dhcp_port].option82CircuitId[0] != 0)
                    {
                        strcpy((char*)p,(char*)dhcpOption82Policy_port[ifPara->dhcp_port].option82CircuitId);
                        p += strlen((char*)dhcpOption82Policy_port[ifPara->dhcp_port].option82CircuitId);
                    }
                    else
                    {
                        ustemp = htons(ifPara->dhcp_vid);
                        memcpy(p, &ustemp, sizeof(ustemp));
                        p += 2;
                        if (dhcpOption82Policy.option82DevEnabled == TRUE)
                        {
                            *p++ = (uchar_t)nDevice;
                        }
                        *p++ = (uchar_t)nSlot;
                        *p++ = (uchar_t)nNumber;
                    }

                    *p111 = p - p111 - 1; 
                    *p11 = p - p11 - 1;   

                  /*Add suboption 2, its content is the MAC address of relay device or switch name or a custom string*/

                    *p++ = 2;   //sub opt type 2
                    p12 = p++ ;  //len
                     *p++ = 0;   //remote id type
                    p121 = p++;   //len
                    if(dhcpOption82Policy_port[ifPara->dhcp_port].option82RemoteId[0] != 0)
                    {
                        if(strcmp((char*)dhcpOption82Policy_port[ifPara->dhcp_port].option82RemoteId,OPTION82_VERBOSE_NODEID_HOSTNAME))
                        {
                            strcpy((char*)p, (char*)dhcpOption82Policy_port[ifPara->dhcp_port].option82RemoteId);
                            p += strlen((char*)dhcpOption82Policy_port[ifPara->dhcp_port].option82RemoteId);
                        }
                        else
                        {
                            strcpy((char*)p, szHostName);
                            p += strlen(szHostName);
                        }
                    }
                    else
                    {
                        memcpy(p, SysMac, 6);
                        p += 6;
                    }

                    *p121 = p - p121 - 1; //���볤��
                    *p12 = p - p12 - 1;   //���볤��
                }
                else //OPTION82FORMAT_verbose
                {
                    /*Add suboption 1*/
                    *p++ = 1;     //sub opt type 1
                     p11 = p++;   
                    

                    //node identifier Ϊmac or switch name or custom string
                    if(dhcpOption82Policy.option82VerboseNodeId[0] != 0)
                    {
                        if(strcmp((char*)dhcpOption82Policy.option82VerboseNodeId, OPTION82_VERBOSE_NODEID_HOSTNAME) == 0)
                        {
                            //Use hostname as nodeId,hostname cannot contain spaces
                            if(strchr(szHostName,' '))
                            {
                                dhcp_debug_printf ( "\r\n dhcp option 82 add failed, there is space character in hostname " );
                                return OK;
                            }

                            strcpy((char*)p,szHostName);
                            p += strlen(szHostName);

                        }
                        else
                        {
                            //custom
                            strcpy((char*)p, (char*)dhcpOption82Policy.option82VerboseNodeId);
                            p += strlen((char*)dhcpOption82Policy.option82VerboseNodeId);
                        }

                    }
                    else
                    {
                        //Use mac as nodeId
                        for (i = 0; i < 6; i++ )
                        {
                            sprintf((char*)p, "%02x", SysMac[i]);
                            p += 2;
                        }
                    }

                    if (dhcpOption82Policy.option82DevEnabled == TRUE)
                    {
                        i = sprintf((char*)p, " eth 0/%u/%u/%02u:", nDevice, nSlot, nNumber);
                    }
                    else
                    {
                        i = sprintf((char*)p, " eth 0/%u/0/%02u:",nSlot,nNumber);
                    }
                    p += i; 

                    if (dhcpOption82Policy.opion82InfoFormat == OPTION82INFOFORMAT_HEX)
                    {
                        ustemp = htons(ifPara->dhcp_vid);
                        memcpy(p, &ustemp, sizeof(ustemp));
                        p += 2;
                    }
                    else
                    {
                        sprintf((char*)p, "%04u", ifPara->dhcp_vid);
                        p += 4;
                    }

                    *p11 = p - p11 - 1;  

                   /*Add suboption 2, its content is the MAC address of relay device or switch name or a custom string*/
                    *p++ = 2;   //sub opt type 2
                    p12 = p++;   //len
                    if(dhcpOption82Policy_port[ifPara->dhcp_port].option82RemoteId[0] != 0)
                    {
                        if(strcmp((char*)dhcpOption82Policy_port[ifPara->dhcp_port].option82RemoteId,OPTION82_VERBOSE_NODEID_HOSTNAME))
                        {
                            strcpy((char*)p, (char*)dhcpOption82Policy_port[ifPara->dhcp_port].option82RemoteId);
                            p += strlen((char*)dhcpOption82Policy_port[ifPara->dhcp_port].option82RemoteId);
                        }
                        else
                        {
                            strcpy((char*)p, szHostName);
                            p += strlen(szHostName);
                        }
                    }
                    else
                    {
                        if (dhcpOption82Policy.opion82InfoFormat == OPTION82INFOFORMAT_HEX)
                        {
                            memcpy(p, SysMac, 6);
                            p += 6;
                        }
                        else
                        {
                            for (i = 0; i < 6; i++ )
                            {
                                sprintf((char*)p, "%02x", SysMac[i]);
                                p += 2;
                            }
                        }
                    }

                    *p12 = p - p12 - 1;

                }

                *p1 = p - p1 - 1; 

                new82len = p - option82;  
            }

            /*Add new option 82 content after the message*/
            if ( ((ulDhcpLen - Option82Len) + new82len) > DHCP_MAX_LEN )/*if message length is too long after adding
                                                                        the option, it will not be added*/
            {
                if (((ulDhcpLen - Option82Len) + new82len - oldpadlen) > DHCP_MAX_LEN)
                {
                    dhcp_debug_printf ( "\r\n dhcp option 82 add failed, the packet too long, greater than 1500 - 28 after added " );
                    return OK;
                }
            }

            if (old2new)
            {
                /*Clear the original option82 content*/
                *ulNewDhcpLen = ulDhcpLen - Option82Len;
                pPacket -= Option82Len;
                Option82Len = ulDhcpLen - (pOption82NextOption - (uchar_t *)pDhcp);
                memcpy(pOption82, pOption82NextOption, Option82Len);
            }


            /*Add new option82 content*/
            memcpy(pPacket, option82, new82len);
            pPacket += new82len;
            *pPacket = 0xff;
            *ulNewDhcpLen += new82len;

            if (*ulNewDhcpLen > 548 && *ulNewDhcpLen > olddhcplen )
            {
                int paddinglen = *ulNewDhcpLen-(pPacket-((uchar_t *)pDhcp)+1);
                if (paddinglen > 0)
                {
                    if (paddinglen >= *ulNewDhcpLen-olddhcplen) 
                    {
                        *ulNewDhcpLen = olddhcplen;
                    }
                    else
                    {
                        *ulNewDhcpLen -= paddinglen;
                    }
                }
            }

            /*Copy the padding og tje original message*/
            if (*ulNewDhcpLen - (pPacket - ((uchar_t *)pDhcp) + 1) > 0)
            {
                memcpy(pPacket + 1, padding, *ulNewDhcpLen - (pPacket - ((uchar_t *)pDhcp) + 1));
            }

            break;

        case DHCP_OFFER_TYPE:
        case DHCP_ACK_TYPE:
        case DHCP_NAK_TYPE:
            if ( pOption82 != NULL )
            {
                /*Clear the original option82 content*/
                *ulNewDhcpLen = ulDhcpLen - Option82Len;
                Option82Len = ulDhcpLen - (pOption82NextOption - (uchar_t *)pDhcp);
                memcpy(pOption82, pOption82NextOption, Option82Len);
            }
            break;

        default:
            break;
    }
#endif
    return SYS_ERR_OK;
}

STATUS dhcp_otion82_vlan_process
(
	DHCP_HEAD_S *pDhcp,
	ulong_t ulMsgType,
	ulong_t ulDhcpLen,
	ulong_t *ulNewDhcpLen,
	DHCP_INTERFACE_PARA *ifPara
)
{
#if 0
    uchar_t  *pPacket = NULL;
    uchar_t  *pOption82 = NULL;
    uchar_t  *pOption82NextOption = NULL;
    uchar_t  Option82Len = 0;
    uchar_t  SysMac[6] = {0};
    uchar_t  sysOid[4];
    uchar_t  option82[255] = {0}, *p = option82;
    ulong_t  ulIfaddr;
    ulong_t  nSlot = 0, nNumber = 0, nDevice = 0;
    uchar_t  *p1,*p11,*p111,*p12,*p121,*p19,*p191,*p1911; 
    int      new82len = 0;
    int      i = 0;
    int      olddhcplen = ulDhcpLen;
    static char padding[DHCP_MAX_LEN - DHCP_HEAD_LEN] = {0};
    int      oldpadlen = 0;
    ushort_t ustemp = 0;
    BOOL append = FALSE;
    BOOL old2new = FALSE;
    dhcpOption82Policy_vlan_t *opt82Avl = NULL;

    char szHostName[HOSTNAME_MAXLEN + 1] = { 0 };

    vosHostNameGet(szHostName, HOSTNAME_MAXLEN);

    if ((NULL != (opt82Avl = dhcpoption82_get_avlnode(ifPara->dhcp_vid)))
        && (opt82Avl->option82Enabled == FALSE))
    {
        return OK;
    }

    sysOid[0] = (uchar_t)(OEMOID_OEM >> 24);
    sysOid[1] = (uchar_t)(OEMOID_OEM >> 16);
    sysOid[2] = (uchar_t)(OEMOID_OEM >>  8);
    sysOid[3] = (uchar_t)(OEMOID_OEM >>  0);

    memset(option82, 0, sizeof(option82));

    #ifdef GBN_INCLUDE_STACK_ISF
    if ( (TRUE == isf_stack_enable()) && (FALSE == isf_stack_is_master()) )
    {
        isf_stack_master_mac(SysMac);
    }
    else
    {
    #endif
    getSysMacAdress((char *)SysMac);
    #ifdef GBN_INCLUDE_STACK_ISF
    }
    #endif
    smdVDPortSnmpToUser(ifPara->dhcp_port + 1, &nDevice, &nSlot, &nNumber);

    *ulNewDhcpLen = ulDhcpLen;

    pPacket = (uchar_t *)(pDhcp+1);
    pPacket += 4; 
    /*It is assumed here that the content of the message is normal and has been verified in DHCP_r=RECORD_ALL_OPT*/
      while (*pPacket != ENDOFF_OPTIONS)
    {
        if( *pPacket == DHCP_OPTION82_CODE )
        {
            Option82Len = *(pPacket+1)+2;
            pOption82 = pPacket;
            pOption82NextOption = pOption82+Option82Len;
        }
        if( *pPacket == 0 )
        {
            pPacket++;
        }
        else
        {
            pPacket += (*(pPacket+1))+2;
        }
    }
    //Then pPacket points to the end option
    //Store the padding of the original message, if there is padding
    if ( (oldpadlen = (ulDhcpLen-(pPacket-((uchar_t *)pDhcp)+1))) > 0)
    {
        memcpy(padding, pPacket + 1, oldpadlen);
    }

    switch (ulMsgType)
    {
        case DHCP_DISCOVER:
        case DHCP_REQUEST:
            if ( pOption82 != NULL )
            {
                if (opt82Avl->opion82Strategy == OPTION82STRATEGY_DROP)
                {
                    dhcp_debug_printf ( "\r\n dhcp option 82 strategy = drop, drop dhcp request message " );
                    return ERROR;
                }
                else if (opt82Avl->opion82Strategy == OPTION82STRATEGY_KEEP)
                {
                #if defined(TINETS8600_CONTROL_GPON) || defined(TINETS5600_08GPON)
                #if defined(TINETS8600_CONTROL_GPON)
                    rc = CB_TransPort2EponIndex(ifPara->dhcp_port+1, &slotid, &ponid);
                    if (OK != rc)
                    {
                        return ERROR;
                    }
                    #endif
                #if defined(TINETS5600_08GPON)
                    VD_port_snmp_to_user((ifPara->dhcp_port + 1), &nDevice, &slotid, &ponid);
                #endif
                    if (CB_PortIsPonPort(ifPara->dhcp_port+1))
                    {
                        is_pon = 1;
                        if ( pOption82 != NULL )
                        {
                            ont_id = *(pOption82 + 4);
                            memcpy(sn, (pOption82 + 5), 8);
                            ont_port = *(pOption82 + 13);
                            if ((NULL == sn) || (ERROR == checkOntSn(slotid, ponid, sn)))
                            {
                                return OK;
                            }
                        }
                        is_from_ont = 1;
                    }
                #else
                    return OK;
                #endif
                }
                else
                {
                    if (opt82Avl->opion82Strategy ==OPTION82STRATEGY_APPEND_HOSTNAME
                    || opt82Avl->opion82Strategy ==OPTION82STRATEGY_APPEND_HOSTNAME_IP)
                    {
                        append = TRUE;
                        if(Option82Len < sizeof(option82))
                        {
                            memcpy(option82,pOption82,Option82Len);
                        }
                        else
                        {
                            dhcp_debug_printf ( "\r\n local option82[255] too small, cannot copy, not appeded " );
                            return OK;
                        }
                    }

                    old2new = TRUE;
                }
            }

            if(append)
            {
                int suboptionlen = Option82Len - 2;
                uchar_t *popt9 = NULL;
                uchar_t * q = p + 2;

                p1 = p+1; 

                while(suboptionlen >= 1 )
                {
                    if( *q == 9 )
                    {
                        popt9 = q;
                    }

                    if( *q == 0 )
                    {
                        q++;
                        suboptionlen--;
                    }
                    else
                    {
                        if(suboptionlen >= 2)
                        {
                            suboptionlen -= ((*(q+1))+2);
                            q += (*(q+1))+2;/*jump to next suboption*/
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                if(suboptionlen != 0) /*The message is incorrect or format is incorrect*/
                {
                    dhcp_debug_printf ( "\r\n option 82 sub option format not right, not appeded " );
                    return OK;
                }
                else
                {
                    if(popt9)
                    {
                        int N = *(popt9+1);
                        p19=popt9+1;
                        if(N > 4)
                        {
                            p = popt9+2;
                            if(p[0]==sysOid[0] && p[1]==sysOid[1] && p[2]==sysOid[2] && p[3]==sysOid[3])
                            {
                                int N1 = *(p+4);
                                int N1_left = N1;
                                int si = 0;
                                p += 4;
                                p191 = p++; 

                                while(N1_left >= 1 )
                                {
                                    if( *p == 0 )
                                    {
                                        p++;
                                        N1_left--;
                                    }
                                    else
                                    {
                                        if(N1_left >= 2)
                                        {
                                            ++si;
                                            N1_left -= ((*(p+1))+2);
                                            p += (*(p+1))+2;
                                        }
                                        else
                                        {
                                            break;
                                        }
                                    }
                                }
                                if(N1_left != 0) 
                                {
                                    dhcp_debug_printf ( "\r\n option 82 sub option 9 format not right or not henan format, not appeded " );
                                    return OK;
                                }
                                else 
                                {
                                    uchar_t tmp[200];
                                    int lenleft = Option82Len-(p-option82); 
                                    if(lenleft>0 && lenleft < sizeof(tmp))
                                    {
                                        memcpy(tmp,p,lenleft);
                                    }
                                    else if(lenleft == 0)
                                    {
                                        /*do nothing*/
                                    }
                                    else
                                    {
                                        dhcp_debug_printf ( "\r\n option 82 tmp[200] too small cannot copy, not appeded " );
                                        return OK;
                                    }

                                    *p++ = ++si;
                                    p1911 = p++;
                                    strcpy((char*)p, szHostName);
                                    p += strlen(szHostName);
                                    *p1911 = strlen(szHostName);
                                    *p191 += 2+strlen(szHostName);
                                    *p19 += 2+strlen(szHostName);
                                    *p1 += 2+strlen(szHostName); 
                                    new82len = Option82Len + 2+strlen(szHostName); 

                                    if(opt82Avl->opion82Strategy==OPTION82STRATEGY_APPEND_HOSTNAME_IP)
                                    {
                                        *p++ = ++si;
                                        p1911 = p++; 
                                        ulIfaddr = 0;
                                        dhcprelay_get_ifaddr(ifPara->dhcp_sw, &ulIfaddr);
                                        memcpy(p, &ulIfaddr, 4);
                                        p += 4;
                                        *p1911 = 4/*p - p1911 -1*/;
                                        *p191 += 2+4;
                                        *p19 += 2+4;
                                        *p1 += 2+4; 
                                        new82len += 2+4; 
                                     }
                                     if(lenleft)
                                        memcpy(p,tmp,lenleft);
                                }
                            }
                            else
                            {
                                dhcp_debug_printf ( "\r\n option 82 sub option 9 enterprise ID is not 0x%08x, not henan format, not appeded ", OEMOID_OEM);
                                return OK;
                            }
                        }
                        else
                        {
                            dhcp_debug_printf ( "\r\n option 82 sub option 9 N<=4, not henan format, not appeded " );
                            return OK;
                        }
                    }
                    else /*Suboption 9 does not exist, no append*/
                    {
                        dhcp_debug_printf ( "\r\n option 82 sub option 9 not exist, not appeded " );
                        return OK;
                    }
                }
            }
            else
            {
            #ifdef TINETS8600_CONTROL_EPON
                rc = CB_TransPort2EponTrueIndex(ifPara->dhcp_port+1, &slot, &pon);
                if (OK != rc)
                {
                    return ERROR;
                }
                rc = CB_TransPort2EponIndex(ifPara->dhcp_port+1, &slotid, &ponid);
                if (OK != rc)
                {
                    return ERROR;
                }
                if (CB_PortIsPonPort(ifPara->dhcp_port+1))
                {
                    is_pon = 1;
                    rc = epon_if_get_onu_llid_from_mac(slot, pon, pDhcp->szchaddr, &onu_llid);

                    if (0 != onu_llid)
                    {
                        epon_if_get_onu_mac_from_llid(slot, pon, onu_llid, onu_mac, &onu_id);
                    }

                }
           #endif
            #if defined(TINETS8600_CONTROL_GPON) || defined(TINETS5600_08GPON)
                if (!is_from_ont)
                {
                #if defined(TINETS8600_CONTROL_GPON)
                rc = CB_TransPort2EponIndex(ifPara->dhcp_port+1, &slotid, &ponid);
                if (OK != rc)
                {
                    return ERROR;
                }
                #endif
                #if defined(TINETS5600_08GPON)
                    VD_port_snmp_to_user((ifPara->dhcp_port + 1), &nDevice, &slotid, &ponid);
                #endif
                if (CB_PortIsPonPort(ifPara->dhcp_port+1))
                {
                    is_pon = 1;
                    if ( pOption82 != NULL )/*option82 exist in the packet*/
                    {
                        ont_id = *(pOption82 + 4);
                        memcpy(sn, (pOption82 + 5), 8);
                        ont_port = *(pOption82 + 13);
                    }
                    }
                }
           #endif
                *p++ = 82;
                p1 = p++; // skip the length first and fill it in later
                if(dhcpOption82Policy.option82Format == OPTION82FORMAT_HENAN)
                {

                    /*Add suboption 1, its content is receiving port VID+ receiving port or a custom string*/

                      *p++ = 1;     
                      p11  = p++;   
                      ustemp = htons(ifPara->dhcp_vid);
                      memcpy(p, &ustemp, sizeof(ustemp));
                      p += 2;
                      if (dhcpOption82Policy.option82DevEnabled == TRUE)
                      {
                          *p++ = (uchar_t)nDevice;
                      }
                      *p++ = (uchar_t)nSlot;
                      *p++ = (uchar_t)nNumber;

                      *p11 = p - p11 - 1;   

                    /*Add suboption 2, whose content is the MAC address of the relay device or the switch name
                    or a custom string*/

                      *p++ = 2;   //sub opt type 2
                      p12 = p ++ ;  //len

                      memcpy(p, SysMac, 6);
                      p += 6;

                      *p12 = p - p12-1;   

                    /*Add suboption 9, whose content is hostname*/

                    if(opt82Avl->opion82Strategy ==OPTION82STRATEGY_APPEND_HOSTNAME_IP)
                    {
                        *p++ = 9;     //sub opt type 9
                        p19 = p++;   // len
                        memcpy(p, sysOid, 4);  
                        p += 4;
                        p191 = p++;  

                        *p++ = 0x01;
                        *p++ = 0x00;

                        *p++ = 0x02;
                        p1911 = p++; 
                        strcpy((char*)p,szHostName);
                        p += strlen(szHostName);
                        *p1911 = p - p1911 -1;

                        *p++ = 0x03;
                        p1911 = p++; 
                        ulIfaddr = 0;
                        dhcprelay_get_ifaddr(ifPara->dhcp_sw, &ulIfaddr);
                        memcpy(p, &ulIfaddr, 4);
                        p += 4;
                        *p1911 = 4/*p - p1911 -1*/;

                        *p191 = p - p191 -1;
                        *p19 = p - p19 -1;
                    }
                    else
                    {
                         *p++ = 9;     //sub opt type 9
                         p19 = p++;   
                         memcpy(p, sysOid, 4);  
                         p += 4;
                         p191 = p++;  

                         *p++ = 0x01;
                         p1911 = p++; 
                         strcpy((char*)p,szHostName);
                         p += strlen(szHostName);
                         *p1911 = p - p1911 -1;

                         *p191 = p - p191 -1;
                         *p19 = p - p19 -1;
                    }
                }
                else if(dhcpOption82Policy.option82Format == OPTION82FORMAT_NORMAL)
                {
                  /*Add suboption 1, its content is receiving port VID+ receiving port or a custom string*/

                    *p++ = 1;     //sub opt type 1
                     p11 = p++;   
                    *p++ = 0;    //circuit id type
                     p111 = p++;  

                    if(opt82Avl->option82CircuitId[0] != 0)
                    {
                        strcpy((char*)p,(char*)opt82Avl->option82CircuitId);
                        p += strlen((char*)opt82Avl->option82CircuitId);
                    }
                    else
                    {
                    #ifdef TINETS8600_CONTROL_EPON
                        if (1 == is_pon)
                        {
                            /*Add suboption 1, whose content is the receiving port VID+receiving port+ONU MAC*/

                            ustemp = htons(ifPara->dhcp_vid);
                            memcpy(p, &ustemp, sizeof(ustemp));
                            p += 2;
                            *p++ = (uchar_t)slotid;
                            *p++ = (uchar_t)ponid;
                            *p++ = (uchar_t)onu_id;
                            memcpy(p, onu_mac, 6);
                            p += 6;
                        }
                        else
                        {
                            ustemp = htons(ifPara->dhcp_vid);
                            memcpy(p, &ustemp, sizeof(ustemp));
                            p += 2;
                            *p++ = (uchar_t)slotid;
                            *p++ = (uchar_t)ponid;
                        }
                    #elif defined(TINETS8600_CONTROL_GPON) || defined(TINETS5600_08GPON)
                        if (1 == is_pon)
                        {
                           /*Add suboption 1, whose content is the receiving port VID+receiving port+ONU MAC*/
                            ustemp = htons(ifPara->dhcp_vid);
                            memcpy(p, &ustemp, sizeof(ustemp));
                            p += 2;
                            *p++ = (uchar_t)slotid;
                            *p++ = (uchar_t)ponid;
                            *p++ = (uchar_t)ont_id;
                            memcpy(p, sn, 8);
                            p += 8;
                            *p++ = (uchar_t)ont_port;
                        }
                        else
                        {
                            ustemp = htons(ifPara->dhcp_vid);
                            memcpy(p, &ustemp, sizeof(ustemp));
                            p += 2;
                            *p++ = (uchar_t)slotid;
                            *p++ = (uchar_t)ponid;
                        }
                    #else
                        ustemp = htons(ifPara->dhcp_vid);
                        memcpy(p, &ustemp, sizeof(ustemp));
                        p += 2;
                        if (dhcpOption82Policy.option82DevEnabled == TRUE)
                        {
                            *p++ = (uchar_t)nDevice;
                        }
                        *p++ = (uchar_t)nSlot;
                        *p++ = (uchar_t)nNumber;
                    #endif
                    }

                    *p111 = p - p111 - 1; 
                    *p11 = p - p11 - 1;   

                  /*Add suboption 2, whose content is the MAC address of the relay device or the switch name
                    or a custom string*/
                    *p++ = 2;   //sub opt type 2
                    p12 = p++ ;  //len
                    *p++ = 0;   //remote id type
                    p121 = p++;   //len
                    if(opt82Avl->option82RemoteId[0] != 0)
                    {
                        if(strcmp((char*)opt82Avl->option82RemoteId,OPTION82_VERBOSE_NODEID_HOSTNAME))
                        {
                            strcpy((char*)p, (char*)opt82Avl->option82RemoteId);
                            p += strlen((char*)opt82Avl->option82RemoteId);
                        }
                        else
                        {
                            strcpy((char*)p, szHostName);
                            p += strlen(szHostName);
                        }
                    }
                    else
                    {
                        memcpy(p, SysMac, 6);
                        p += 6;
                    }

                    *p121 = p - p121 - 1; 
                    *p12 = p - p12 - 1;   
                }
                else //OPTION82FORMAT_verbose
                {
                    /*Add suboption 1*/
                    #if 0
                   
                    #endif

                    *p++ = 1;     //sub opt type 1
                     p11 = p++;   

                    //node identifier mac or switch name or customer string
                    if(dhcpOption82Policy.option82VerboseNodeId[0] != 0)
                    {
                        if(strcmp((char*)dhcpOption82Policy.option82VerboseNodeId, OPTION82_VERBOSE_NODEID_HOSTNAME) == 0)
                        {
                            //Use hostname as nodeId,hostname cannot contain spaces
                            if(strchr(szHostName,' '))
                            {
                                dhcp_debug_printf ( "\r\n dhcp option 82 add failed, there is space character in hostname " );
                                return OK;
                            }
                            strcpy((char*)p,szHostName);
                            p += strlen(szHostName);

                        }
                        else
                        {
                            //custom
                            strcpy((char*)p, (char*)dhcpOption82Policy.option82VerboseNodeId);
                            p += strlen((char*)dhcpOption82Policy.option82VerboseNodeId);
                        }

                    }
                    else
                    {
                    #ifdef TINETS8600_CONTROL_EPON
                        if(1 == is_pon)
                        {
                            is_onu = 1;
                            for(i = 0; i < 6; i++ )
                            {
                                sprintf(p,"%02x",onu_mac[i]);
                                p += 2;
                            }
                        }
                        else
                        {
                            //Use mac as nodeId
                            for(i = 0; i < 6; i++ )
                            {
                                sprintf(p,"%02x",SysMac[i]);
                                p += 2;
                            }
                        }
                    #elif defined(TINETS8600_CONTROL_GPON) || defined(TINETS5600_08GPON)
                        if(1 == is_pon)
                        {
                            is_ont = 1;
                            memcpy(p, sn, 4);
                            for (i = 0; i < 4; i++)
                            {
                                if (p[i] == 0)
                                {
                                    p[i] = ' ';
                                }
                            }
                            p += 4;
                            for (i = 4; i < 8; i++ )
                            {
                                sprintf(p, "%02x", sn[i]);
                                p += 2;
                            }
                            *p++ = (uchar_t)ont_id;
                            *p++ = (uchar_t)ont_port;
                        }
                        else
                        {
                            //Use mac as nodeId
                            for (i = 0; i < 6; i++ )
                            {
                                sprintf(p, "%02x", SysMac[i]);
                                p += 2;
                            }
                        }
                    #else
                        //Use mac as nodeId
                        for (i = 0; i < 6; i++ )
                        {
                            sprintf((char*)p, "%02x", SysMac[i]);
                            p += 2;
                        }
                    #endif

                    }

                #ifdef TINETS8600_CONTROL_EPON
                    if(1 == is_pon)
                    {
                        if(1 == is_onu)
                        {
                            i = sprintf(p," onu 0/%02u/0/%02u/0/%02u:",slotid, ponid, onu_id);
                        }
                        else
                        {
                            i = sprintf(p," pon 0/%02u/0/%02u:",slotid, ponid);
                        }
                    }
                    else
                    {
                        i = sprintf(p," eth 0/%02u/0/%02u:",slotid, ponid);
                    }
                #elif defined(TINETS8600_CONTROL_GPON) || defined(TINETS5600_08GPON)
                    if(1 == is_pon)
                    {
                        if(1 == is_ont)
                        {
                            i = sprintf(p," ont 0/%02u/0/%02u/0/%03u:",slotid, ponid, ont_id);
                        }
                        else
                        {
                            i = sprintf(p," pon 0/%02u/0/%02u:",slotid, ponid);
                        }
                    }
                    else
                    {
                        i = sprintf(p," eth 0/%02u/0/%02u:",slotid, ponid);
                    }
                #else
                    if (dhcpOption82Policy.option82DevEnabled == TRUE)
                    {
                        i = sprintf((char*)p, " eth 0/%lu/%lu/%02lu:", nDevice, nSlot, nNumber);
                    }
                    else
                    {
                        i = sprintf((char*)p, " eth 0/%lu/0/%02lu:",nSlot,nNumber);
                    }
                #endif
                    p += i; 

                    if (dhcpOption82Policy.opion82InfoFormat == OPTION82INFOFORMAT_HEX)
                    {
                        ustemp = htons(ifPara->dhcp_vid);
                        memcpy(p, &ustemp, sizeof(ustemp));
                        p += 2;
                    }
                    else
                    {
                        sprintf((char*)p, "%04u", ifPara->dhcp_vid);
                        p += 4;
                    }

                    *p11 = p - p11 - 1;  

                    /*Add suboption 2, whose content is the MAC address of the relay device or the switch name or a custom string*/
                    *p++ = 2;   //sub opt type 2
                    p12 = p++;   //len
                    if(opt82Avl->option82RemoteId[0] != 0)
                    {
                        if(strcmp((char*)opt82Avl->option82RemoteId,OPTION82_VERBOSE_NODEID_HOSTNAME))
                        {
                            strcpy((char*)p, (char*)opt82Avl->option82RemoteId);
                            p += strlen((char*)opt82Avl->option82RemoteId);
                        }
                        else
                        {
                            strcpy((char*)p, szHostName);
                            p += strlen(szHostName);
                        }
                    }
                    else
                    {
                        if (dhcpOption82Policy.opion82InfoFormat == OPTION82INFOFORMAT_HEX)
                        {
                            memcpy(p, SysMac, 6);
                            p += 6;
                        }
                        else
                        {
                            for (i = 0; i < 6; i++ )
                            {
                                sprintf((char*)p, "%02x", SysMac[i]);
                                p += 2;
                            }
                        }
                    }

                    *p12 = p - p12 - 1;

                }

                *p1 = p - p1 - 1; 

                new82len = p - option82;  //�ܳ�
            }

            /*Add new option 82 content after the message*/
            if ( ((ulDhcpLen - Option82Len) + new82len) > DHCP_MAX_LEN )
            {
                if (((ulDhcpLen - Option82Len) + new82len - oldpadlen) > DHCP_MAX_LEN)
                {
                    dhcp_debug_printf ( "\r\n dhcp option 82 add failed, the packet too long, greater than 1500 - 28 after added " );
                    return OK;
                }
            }

            if (old2new)
            {
                /*Clear the original option82 content*/
                *ulNewDhcpLen = ulDhcpLen - Option82Len;
                pPacket -= Option82Len;
                Option82Len = ulDhcpLen - (pOption82NextOption - (uchar_t *)pDhcp);
                memcpy(pOption82, pOption82NextOption, Option82Len);
            }


            /*Add new option82 content*/
            memcpy(pPacket, option82, new82len);
            pPacket += new82len;
            *pPacket = 0xff;
            *ulNewDhcpLen += new82len;

            if (*ulNewDhcpLen > 548 && *ulNewDhcpLen > olddhcplen )
            {
                int paddinglen = *ulNewDhcpLen-(pPacket-((uchar_t *)pDhcp)+1);
                if (paddinglen > 0)//��0 padding
                {
                    if (paddinglen >= *ulNewDhcpLen-olddhcplen) 
                    {
                        *ulNewDhcpLen = olddhcplen;
                    }
                    else
                    {
                        *ulNewDhcpLen -= paddinglen;
                    }
                }
            }

            /*Copy the padding of the original message*/
            if (*ulNewDhcpLen - (pPacket - ((uchar_t *)pDhcp) + 1) > 0)
            {
                memcpy(pPacket + 1, padding, *ulNewDhcpLen - (pPacket - ((uchar_t *)pDhcp) + 1));
            }

            break;

        case DHCP_OFFER:
        case DHCP_ACK:
        case DHCP_NAK:
            if ( pOption82 != NULL )
            {
                *ulNewDhcpLen = ulDhcpLen - Option82Len;
                Option82Len = ulDhcpLen - (pOption82NextOption - (uchar_t *)pDhcp);
                memcpy(pOption82, pOption82NextOption, Option82Len);
            }
            break;

        default:
            break;
    }
#endif
    return SYS_ERR_OK;
}


/*******************************************************************************
  Function: STATUS dhcpoption82_avl_compare
  Description: AVL comparison function
  Input: void * node - node
         GENERIC_ARGUMENT key - coparison parameter
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
int dhcpoption82_avl_compare(void * node, GENERIC_ARGUMENT key)
{
    dhcpOption82Policy_vlan_t *dhcpOption82PolicyVLAN = (dhcpOption82Policy_vlan_t *)node;
    ulong_t ulVlan = (ulong_t)key.u;

    if (ulVlan == dhcpOption82PolicyVLAN->ulVlan)
    {
        return 0;
    }
    else if (ulVlan < dhcpOption82PolicyVLAN->ulVlan)
    {
        return -1;
    }
    else if (ulVlan > dhcpOption82PolicyVLAN->ulVlan)
    {
        return 1;
    }

    return 0;
}

/*******************************************************************************
  Function: dhcpOption82Policy_vlan_t *dhcpoption82_add_avlnode
  Description: Add VLAN node information
  Input: ulong_t ulVlan -the vlan to be added
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
dhcpOption82Policy_vlan_t *dhcpoption82_add_avlnode(ulong_t ulVlan)
{
	#if 0
    STATUS               rc;
    GENERIC_ARGUMENT     key;
    dhcpOption82Policy_vlan_t *dhcpOption82PolicyVLAN = NULL;

    if ((ulVlan < 1) || (ulVlan > 4094))
    {
        return NULL;
    }

    key.u = ulVlan;

    dhcpOption82PolicyVLAN = (dhcpOption82Policy_vlan_t *)avlSearch(vlanPolicyAVL,
                                           key, dhcpoption82_avl_compare );

    if ( dhcpOption82PolicyVLAN == NULL )
    {
        dhcpOption82PolicyVLAN = (dhcpOption82Policy_vlan_t *)lstGet(&vlanPolicyListFree);
        if ( dhcpOption82PolicyVLAN == NULL )
        {
            dhcp_debug_printf("\r\n lstGet() failed, no enough node space.");
            return NULL;
        }

        rc = avlInsert(&vlanPolicyAVL, dhcpOption82PolicyVLAN, key, dhcpoption82_avl_compare);
        if ( rc == OK )
        {
            dhcpOption82PolicyVLAN->ulVlan = ulVlan;
            dhcpOption82PolicyVLAN->opion82Strategy = OPTION82STRATEGY_REPLACE;
        }
        else
        {
            lstAdd(&vlanPolicyListFree, (NODE *)dhcpOption82PolicyVLAN);
            return NULL;
        }
    }

    return dhcpOption82PolicyVLAN;
    #endif
    return NULL;
}

/*******************************************************************************
  Function: STATUS dhcpoption82_delete_avlnode
  Description: Deletes a VLAN node
  Input: ulong_t ulVlan - the VLAN to be deleted
  Output: None
  Return: Success OK, Failure ERROR
*******************************************************************************/
STATUS dhcpoption82_delete_avlnode(ulong_t ulVlan)
{
#if 0

    GENERIC_ARGUMENT     key;
    dhcpOption82Policy_vlan_t *dhcpOption82PolicyVLAN = NULL;

    if ((ulVlan < 1) || (ulVlan > 4094))
    {
        return ERROR;
    }

    key.u = ulVlan;

    dhcpOption82PolicyVLAN = (dhcpOption82Policy_vlan_t *)avlSearch(vlanPolicyAVL,
                                           key, dhcpoption82_avl_compare );
    if ( dhcpOption82PolicyVLAN == NULL )
    {
        return OK;
    }

    dhcpOption82PolicyVLAN = (dhcpOption82Policy_vlan_t *)avlDelete(&vlanPolicyAVL,
                                           key, dhcpoption82_avl_compare );
    if ( dhcpOption82PolicyVLAN == NULL )
    {
        return OK;
    }

    memset(dhcpOption82PolicyVLAN, 0, sizeof(dhcpOption82Policy_vlan_t));
    lstAdd(&vlanPolicyListFree, (NODE *)dhcpOption82PolicyVLAN);
 #endif
    return SYS_ERR_OK;
}

/*******************************************************************************
  Function: dhcpOption82Policy_vlan_t *dhcpoption82_get_avlnode
  Description: Gets information about a VLAN node
  Input: ulong_t ulVlan - the VLAN to be retrieved
  Output: None
  Return: AVL node pointer, or NULL
*******************************************************************************/
dhcpOption82Policy_vlan_t *dhcpoption82_get_avlnode(ulong_t ulVlan)
{
#if 0

    GENERIC_ARGUMENT     key;
    dhcpOption82Policy_vlan_t *dhcpOption82PolicyVLAN = NULL;

    if ((ulVlan < 1) || (ulVlan > 4094))
    {
        return NULL;
    }

    key.u = ulVlan;
    dhcpOption82PolicyVLAN = (dhcpOption82Policy_vlan_t *)avlSearch(vlanPolicyAVL,
                                           key, dhcpoption82_avl_compare );

    return dhcpOption82PolicyVLAN;
    #endif
    return NULL;
}

/*******************************************************************************
  Function: STATUS dhcpoption82_get_next_avlnode
  Description: Get the next vlaan node information for command line display
  Input: ulong_t ulVlan - VLAN address. If 0, the search starts from the beginning
  Output: ulong_t *dhcpOption82PolicyVLAN - the next node
  Return: Success OK, Failure ERROR
*******************************************************************************/
dhcpOption82Policy_vlan_t *dhcpoption82_get_next_avlnode(ulong_t ulVlan)
{
#if 0
    GENERIC_ARGUMENT key;
    dhcpOption82Policy_vlan_t *dhcpOp82PolicyVLAN = NULL;

    if (ulVlan > 4094)
    {
        return NULL;
    }

    if (0 == ulVlan)
    {
        dhcpOp82PolicyVLAN = (dhcpOption82Policy_vlan_t *)avlMinimumGet(vlanPolicyAVL);
    }
    else
    {
        key.u = ulVlan;
        dhcpOp82PolicyVLAN = (dhcpOption82Policy_vlan_t *)avlSuccessorGet(vlanPolicyAVL,
            key, dhcpoption82_avl_compare);
    }

    return dhcpOp82PolicyVLAN;
#endif
	return NULL;
}

int dhcpoption82Cfg(BOOL bEnable)
{
    dhcpOption82Policy.option82Enabled = bEnable;
    return OK;
}

int dhcpoption82DevCfg(BOOL bEnable)
{
    dhcpOption82Policy.option82DevEnabled = bEnable;
    return OK;
}

int dhcpoption82PortCfg(ulong_t ulPort, BOOL bEnable)
{
    if ( (ulPort < 0) || (ulPort > SYS_LOGICPORT_NUM_MAX) )
    {
        return ERROR;
    }

    dhcpOption82Policy_port[ulPort-1].option82Enabled = bEnable;
    return OK;
}

int dhcpoption82PortStrategyCfg(ulong_t ulPort, int iStrategy)
{
    if ( (ulPort < 0) || (ulPort > SYS_LOGICPORT_NUM_MAX) )
    {
        return ERROR;
    }

    dhcpOption82Policy_port[ulPort-1].opion82Strategy = iStrategy;
    return OK;
}

int dhcpoption82FormatCfg(int iFormat)
{
    dhcpOption82Policy.option82Format = iFormat;
    return OK;
}

int dhcpoption82InfoFormatCfg(int iFormat)
{
    dhcpOption82Policy.opion82InfoFormat = iFormat;
    return OK;
}

int dhcpoption82FormatVerbose(uchar_t option82VerboseNodeId[OPTION82_VERBOS_NODEID_LEN + 1])
{
    if ( strlen(option82VerboseNodeId) > OPTION82_VERBOS_NODEID_LEN )
    {
        return ERROR;
    }

    strcpy(dhcpOption82Policy.option82VerboseNodeId, option82VerboseNodeId);
    dhcpOption82Policy.option82Format = OPTION82FORMAT_VERBOSE;
    return OK;
}

int dhcpoption82PortCircuitid(ulong_t ulPort, uchar_t option82CircuitId[OPTION82_CIRCUITID_LEN + 1])
{
    if ( (ulPort < 0) || (ulPort > SYS_LOGICPORT_NUM_MAX) )
    {
        return ERROR;
    }

    if ( strlen(option82CircuitId) > OPTION82_CIRCUITID_LEN )
    {
        return ERROR;
    }

    strcpy((char *)dhcpOption82Policy_port[ulPort].option82CircuitId, option82CircuitId);
    return OK;
}

int dhcpoption82PortRemoteid(ulong_t ulPort, uchar_t option82RemoteId[OPTION82_REMOTEID_LEN + 1])
{
    if ( (ulPort < 0) || (ulPort > SYS_LOGICPORT_NUM_MAX) )
    {
        return ERROR;
    }

    if ( strlen(option82RemoteId) > OPTION82_REMOTEID_LEN )
    {
        return ERROR;
    }

    strcpy((char *)dhcpOption82Policy_port[ulPort].option82RemoteId, option82RemoteId);
    return OK;
}

int dhcpooption82PortSuboption9(ulong_t ulPort, uchar_t option82SubOpt9[OPTION82_SUBOPTION9_LEN + 1])
{
    if ( (ulPort < 0) || (ulPort > SYS_LOGICPORT_NUM_MAX) )
    {
        return ERROR;
    }

    if ( strlen(option82SubOpt9) > OPTION82_SUBOPTION9_LEN )
    {
        return ERROR;
    }

    strcpy((char *)dhcpOption82Policy_port[ulPort].option82SubOpt9, option82SubOpt9);
    return OK;
}

/*When bEnable is TRUE, ucVlanList is also used as an error output. The corresponding VLAN bit is cleared after success*/
int dhcpoption82VlanlistCfg(BOOL bEnable, uchar_t *ucVlanList)
{
    unsigned int             vlanId;
    dhcpOption82Policy_vlan_t *pVlanOption82Avl;

    if ( TRUE == bEnable )
    {
        for (vlanId = 1; vlanId <= 4094; vlanId++)
        {
            if (!(ucVlanList[vlanId / 8] & (0x80 >> (vlanId & 7))))
            {
                continue;
            }

            pVlanOption82Avl = dhcpoption82_get_avlnode(vlanId);
            if ( NULL == pVlanOption82Avl )
            {
                pVlanOption82Avl = dhcpoption82_add_avlnode(vlanId);
                if ( NULL == pVlanOption82Avl )
                {
                    continue;
                }
            }

            ucVlanList[vlanId / 8] &= ~(0x80 >> (vlanId & 7));

            pVlanOption82Avl->ulVlan =  vlanId;
            pVlanOption82Avl->option82Enabled = TRUE;
            pVlanOption82Avl->opion82Strategy = OPTION82STRATEGY_REPLACE;
        }
    }
    else
    {
        for (vlanId = 1; vlanId <= 4094; vlanId++)
        {
            if (!(ucVlanList[vlanId / 8] & (0x80 >> (vlanId & 7))))
            {
                continue;
            }

            dhcpoption82_delete_avlnode(vlanId);
        }
    }

    return OK;
}

/*ucVlanList is also used as an error output. The corresponding VLAN bit is cleared after success*/
int dhcpoption82VlanlistStrategy(int iStrategy, uchar_t *ucVlanList)
{
    unsigned int             vlanId;
    dhcpOption82Policy_vlan_t *pVlanOption82Avl;

    for (vlanId = 1; vlanId <= 4094; vlanId++)
    {
        if (!(ucVlanList[vlanId / 8] & (0x80 >> (vlanId & 7))))
        {
            continue;
        }

        pVlanOption82Avl = dhcpoption82_get_avlnode(vlanId);
        if ( NULL == pVlanOption82Avl )
        {
            continue;
        }

        ucVlanList[vlanId / 8] &= ~(0x80 >> (vlanId & 7));

        pVlanOption82Avl->opion82Strategy = iStrategy;
    }

    return OK;
}

/*ucVlanList is also used as an error output. The corresponding VLAN bit is cleared after success*/
int dhcpoption82VlanlistCircuitid(uchar_t option82CircuitId[OPTION82_CIRCUITID_LEN + 1],
    uchar_t *ucVlanList)
{
    unsigned int             vlanId;
    dhcpOption82Policy_vlan_t *pVlanOption82Avl;

    if ( strlen(option82CircuitId) > OPTION82_CIRCUITID_LEN )
    {
        return ERROR;
    }

    for (vlanId = 1; vlanId <= 4094; vlanId++)
    {
        if (!(ucVlanList[vlanId / 8] & (0x80 >> (vlanId & 7))))
        {
            continue;
        }

        pVlanOption82Avl = dhcpoption82_get_avlnode(vlanId);
        if ( NULL == pVlanOption82Avl )
        {
            continue;
        }

        ucVlanList[vlanId / 8] &= ~(0x80 >> (vlanId & 7));

        strcpy(pVlanOption82Avl->option82CircuitId, option82CircuitId);
    }

    return OK;
}

/*ucVlanList is also used as an error output. The corresponding VLAN bit is cleared after success*/
int dhcpoption82VlanlistRemoteid(uchar_t option82RemoteId[OPTION82_REMOTEID_LEN + 1],
    uchar_t *ucVlanList)
{
    unsigned int             vlanId;
    dhcpOption82Policy_vlan_t *pVlanOption82Avl;

    if ( strlen(option82RemoteId) > OPTION82_REMOTEID_LEN )
    {
        return ERROR;
    }

    for (vlanId = 1; vlanId <= 4094; vlanId++)
    {
        if (!(ucVlanList[vlanId / 8] & (0x80 >> (vlanId & 7))))
        {
            continue;
        }

        pVlanOption82Avl = dhcpoption82_get_avlnode(vlanId);
        if ( NULL == pVlanOption82Avl )
        {
            continue;
        }

        ucVlanList[vlanId / 8] &= ~(0x80 >> (vlanId & 7));

        strcpy(pVlanOption82Avl->option82RemoteId, option82RemoteId);
    }

    return OK;
}

int dhcpoption82GlobalGet(dhcpOption82Policy_t *pGlobalCfg)
{
    memcpy(pGlobalCfg, &dhcpOption82Policy, sizeof(dhcpOption82Policy_t));
    return OK;
}

int dhcpoption82PortGet(ulong_t ulPort, dhcpOption82Policy_port_t *pPortCfg)
{
    if ( (ulPort < 1) || (ulPort > SYS_LOGICPORT_NUM_MAX) )
    {
        return ERROR;
    }

    memcpy(pPortCfg, &dhcpOption82Policy_port[ulPort - 1], sizeof(dhcpOption82Policy_port_t));
    return OK;
}

int dhcpoption82VlanGet(DATA_OP_ENT op, ulong_t *pulVid, dhcpOption82Policy_vlan_t *pVlanCfg)
{
    dhcpOption82Policy_vlan_t *pVlanAvl;

    if ( (DATA_OP_GET != op) && (DATA_OP_GETNEXT != op) )
    {
        return ERROR;
    }

    if ( DATA_OP_GETNEXT == op )
    {
        pVlanAvl = dhcpoption82_get_next_avlnode(*pulVid);
    }
    else
    {
        pVlanAvl = dhcpoption82_get_avlnode(*pulVid);
    }

    if ( NULL == pVlanAvl )
    {
        return ERROR;
    }

    if ( DATA_OP_GETNEXT == op )
    {
        *pulVid = pVlanAvl->ulVlan;
    }
    memcpy(pVlanCfg, pVlanAvl, sizeof(dhcpOption82Policy_vlan_t));
    return OK;
}

/*When bAdd is FALSE, pMatch is NULLto delete all*/
int dhcpoption60Cfg(ulong_t ulSw, BOOL bAdd, dhcpOption60Match_t *pMatch,
    int *piErrNo)
{
    unsigned int             i;

    if ( (ulSw < 1) || (ulSw > DHCP_MAX_INTERFACE_NUM) )
    {
        return ERROR;
    }

    *piErrNo = DHCPRS_E_OK;

    if ( TRUE == bAdd )
    {
        for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
        {
            if ( dhcpOption60MatchEntry[ulSw-1][i].matchType == pMatch->matchType
                 && dhcpOption60MatchEntry[ulSw-1][i].format == pMatch->format
                 && dhcpOption60MatchEntry[ulSw-1][i].contentlen== pMatch->contentlen
                 && 0 == memcmp(dhcpOption60MatchEntry[ulSw-1][i].matchContent,pMatch->matchContent,pMatch->contentlen)
                 )
            {
                memcpy((void*)&dhcpOption60MatchEntry[ulSw-1][i],(void*)pMatch,sizeof(dhcpOption60Match_t));
                return OK;
            }
        }

        for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
        {
            if ( dhcpOption60MatchEntry[ulSw-1][i].contentlen == 0 )
            {
                memcpy((void*)&dhcpOption60MatchEntry[ulSw-1][i],(void*)pMatch,sizeof(dhcpOption60Match_t));
                return OK;
            }
        }
    }
    else
    {
        if ( NULL == pMatch )
        {
            for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
            {
                memset((void*)&dhcpOption60MatchEntry[ulSw-1][i], 0, sizeof(dhcpOption60Match_t));
            }

            return OK;
        }
        else
        {
            for ( i = 0; i < DHCPINTERFACEMAXNUM; i++ )
            {
                if ( dhcpOption60MatchEntry[ulSw-1][i].matchType == pMatch->matchType
                     && dhcpOption60MatchEntry[ulSw-1][i].format == pMatch->format
                     && dhcpOption60MatchEntry[ulSw-1][i].contentlen== pMatch->contentlen
                     && 0 == memcmp(dhcpOption60MatchEntry[ulSw-1][i].matchContent,pMatch->matchContent,pMatch->contentlen)
                     )
                {
                    memset((void*)&dhcpOption60MatchEntry[ulSw-1][i], 0, sizeof(dhcpOption60Match_t));
                    return OK;
                }
            }
        }
    }

    *piErrNo = DHCPRS_E_MAX;
    return ERROR;
}

int dhcpoption60CfgGet(DATA_OP_ENT op, ulong_t *pulSw, dhcpOption60Match_t entryBuf[DHCPINTERFACEMAXNUM])
{
    unsigned int             i, j;

    if ( (DATA_OP_GET != op) && (DATA_OP_GETNEXT != op) )
    {
        return ERROR;
    }

    if ( DATA_OP_GETNEXT == op )
    {
        for ( i = (*pulSw + 1); i < DHCP_MAX_INTERFACE_NUM; i++ )
        {
            for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
            {
                if ( dhcpOption60MatchEntry[i - 1][j].contentlen > 0 )
                {
                    memcpy(entryBuf, &dhcpOption60MatchEntry[i - 1][0],
                        sizeof(dhcpOption60Match_t) * DHCPINTERFACEMAXNUM);

                    *pulSw = i;
                    return OK;
                }
            }
        }
    }
    else
    {
        i = *pulSw;

        if ( (i < 1) || (i >= DHCP_MAX_INTERFACE_NUM) )
        {
            return ERROR;
        }

        for ( j = 0; j < DHCPINTERFACEMAXNUM; j++ )
        {
            if ( dhcpOption60MatchEntry[i - 1][j].contentlen > 0 )
            {
                memcpy(entryBuf, &dhcpOption60MatchEntry[i - 1][0],
                    sizeof(dhcpOption60Match_t) * DHCPINTERFACEMAXNUM);

                return OK;
            }
        }
    }

    return ERROR;
}

#endif /*GBN_INCLUDE_DHCPRELAYSERVER*/

