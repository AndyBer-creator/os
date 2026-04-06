/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  File Name::  DhcpSocket.c
  Description: This file defines all functions for the DHCP module's socket receive task module.
  Autor: Kniazkov Andrew
  Other Notes: None.
********************************************************************************/
#include <common/sys_def.h>
#include <pthread.h>
#include "dhcps.h"
#ifdef GBN_INCLUDE_DHCPRELAYSERVER
#define GBN_INCLUDE_DHCPRSOCKET
#endif
#ifdef GBN_INCLUDE_DHCPRSOCKET
#include "dhcpr.h"
#include "dhcpSocket.h"


#define OK SYS_ERR_OK
/*Global variable declaration*/
int g_Dhcp_Sockfd = -1;    /*Declare global socket interface variables*/
pthread_t dhcpser_thread_id = -1;

ulong_t gulDhcpSockMsgLose=0;          /*Count the number of lost messages*/
ulong_t gulDhcpSockMsgSum=0;           /*Total number of statistics messages*/


/*Function definition*/

void dhcp_err_printf(char *exp, ...)
{
    if(exp == NULL)
    {
       return ;
    }
#if 0
    if ( syslogModuleSwitch(MID_DHCP, ERRORS) )
    {
        char     debugStr[256];
        va_list  vl;
        ULONG    ulDebugStrLen;

        memset(debugStr, 0, sizeof(debugStr));

        va_start ( vl, exp );
        ulDebugStrLen = vsprintf ( debugStr, exp, vl );
        va_end ( vl );

        /* ������Դ���vsprintfҪôԽ��д�ڴ�Ҫô���� */
        vosAssert(ulDebugStrLen < sizeof(debugStr) && ulDebugStrLen > 0);
        syslogSendInfo(MID_DHCP, ERRORS, "dhcprelay", debugStr, debugStr, FALSE, FALSE, NO_WAIT);
    }
#endif
}

int dhcp_initsocket ( void)
{
    sal_sockaddr_in          sin;
    int                  sockreturn, on = 1;

    /*Create a global socket object to receive messages*/
    g_Dhcp_Sockfd = socket(SAL_AF_INET, SAL_SOCK_DGRAM, 0);
    if ( g_Dhcp_Sockfd <= 0 )
    {
        DHCP_SERVER_CMD_DBG("Cannot create g_Dhcp_Sockfd socket!");
        return SYS_ERR_FAILED;
    }
    
    on = 1;
    if ( sal_setsockopt(g_Dhcp_Sockfd, SAL_SOL_SOCKET, SAL_SO_REUSEADDR,
            (char *)&on, sizeof(on)) < 0 )
    {
        DHCP_SERVER_CMD_DBG("Can't set SO_REUSEADDR option on dhcp socket");
    }

    if ( sal_setsockopt(g_Dhcp_Sockfd, SAL_SOL_SOCKET, SAL_SO_BROADCAST,
            (char *)&on, sizeof(on)) < 0 )
    {
        DHCP_SERVER_CMD_DBG("Can't set SO_BROADCAST option on dhcp socket:");
    }

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = SAL_AF_INET;
    sin.sin_addr.s_addr = osal_htonl(INADDR_ANY);
    sin.sin_port = htons(DHCPSERVER_PORT);
#ifdef HAVE_SA_LEN
    sin.sin_len = sizeof(sin);
#endif

    sockreturn = sal_bind(g_Dhcp_Sockfd, (sal_sockaddr *)&sin, sizeof(sin));
    if ( sockreturn != SYS_ERR_OK )
    {
        DHCP_SERVER_CMD_DBG("Bind g_Dhcp_Sockfd Error!");
        sal_close(g_Dhcp_Sockfd);
        g_Dhcp_Sockfd = SYS_ERR_FAILED;
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

char *dhcpsock_receivebuffer(ushort_t *pusBufferLen, ulong_t *pIp,
    DHCP_INTERFACE_PARA *ifPara)
{
    static char             *szRecvBuffer = NULL;
    sal_sockaddr_in          RecvSockAddr;
    int                      result;
    int                      solen = sizeof(RecvSockAddr);
    sal_s_ext_stc_t            socketExt;

    if ( NULL == szRecvBuffer )
    {
        szRecvBuffer = (char *)osal_alloc(DHCP_MAX_LEN);
        if ( NULL == szRecvBuffer )
        {
            DHCP_SERVER_RX_DBG("malloc failed.");
            return NULL;
        }
    }

    result = sal_recvfromExt(g_Dhcp_Sockfd, szRecvBuffer, DHCP_MAX_LEN, 0,
        (sal_sockaddr *)&RecvSockAddr, &solen, &socketExt);
    if ( result <= 0 )
    {
        return NULL;
    }

    DHCP_SERVER_RX_DBG("l3intftype=(%d) vlan=(%d) port=(%d)", socketExt.ulSw, socketExt.ulVid, socketExt.ulPort);

    *pIp = osal_ntohl(RecvSockAddr.sin_addr.s_addr);
    ifPara->dhcp_sw    = socketExt.ulSw;
    ifPara->dhcp_dstIp = osal_ntohl(socketExt.destIp.sin.sin_addr.s_addr);
    ifPara->dhcp_vid   = socketExt.ulVid;
    ifPara->dhcp_port  = socketExt.ulPort;

    *pusBufferLen = (ushort_t)result;

    return szRecvBuffer;
}

int dhcpsock_sendto ( int socketfd, char * szBuffer, int iLength,
        int iFlags,  sal_sockaddr_in  *pDestAddr, int iTolen )
{
    return sal_sendto(socketfd, szBuffer, iLength, iFlags,
                                 (sal_sockaddr *)pDestAddr, iTolen);
}

int dhcpsock_sendto2(char *szDHCPBuff, int iDHCPLen, ulong_t ulSrcIp,
    ulong_t ulDstIp, ushort_t usDstUdpPort)
{
    sal_sockaddr_in          destAddr;

    memset(&destAddr, 0, sizeof(destAddr));
    destAddr.sin_family = SAL_AF_INET;
    destAddr.sin_addr.s_addr = osal_htonl(ulDstIp);
    destAddr.sin_port = osal_htons(usDstUdpPort);
#ifdef HAVE_SA_LEN
    destAddr.sin_len = sizeof(destAddr);
#endif
	DHCP_SERVER_TX_DBG("ulSrcIp %x ulDstIp %x usDstUdpPort %d", ulSrcIp, destAddr.sin_addr.s_addr, destAddr.sin_port);

    return dhcpsock_sendto(g_Dhcp_Sockfd, szDHCPBuff, iDHCPLen, 0,
                &destAddr, sizeof(destAddr));
}

int dhcpsock_sendBroadcast(char *szDHCPBuff, int iDHCPLen, uint32 ulSw, ushort_t usDstUdpPort)
{
    sal_sockaddr_in          destAddr;
    struct ifreq             ifr;
    char                     ifName[32];

    sprintf(ifName, "sw%d", ulSw - 1);
    strncpy(ifr.ifr_name, ifName, strlen(ifName));
    if ( -1 == ioctl(g_Dhcp_Sockfd, SIOCGIFBRDADDR, &ifr) )
    {
        return -1;
    }

    memcpy(&destAddr, &ifr.ifr_broadaddr, sizeof(sal_sockaddr_in));
    destAddr.sin_family = SAL_AF_INET;
    destAddr.sin_port = osal_htons(usDstUdpPort);
#ifdef HAVE_SA_LEN
    destAddr.sin_len = sizeof(destAddr);
#endif

    return dhcpsock_sendto(g_Dhcp_Sockfd, szDHCPBuff, iDHCPLen, 0,
                &destAddr, sizeof(destAddr));
}

int dhcpsock_handle(int iSocketFd, void *pArg)
{
    ulong_t                  ulSrcIp = 0;
    ushort_t                 usBufferLen = 0;
    char                    *pBuf = NULL;
    DHCP_INTERFACE_PARA      ifPara;

    pBuf = dhcpsock_receivebuffer(&usBufferLen, &ulSrcIp, &ifPara);
    if ( NULL != pBuf )
    {
        dhcprelayRxSocket(FALSE, (DHCP_HEAD_S *)pBuf, usBufferLen, ulSrcIp, &ifPara);
    }

    return OK;
}
void shut_down(int sock){
    if (sock > 0)
        sal_close(sock);

    return;
}

int msgSockAdd( void *socktId)
{
    int ret=0;
    int failed_time = 0;
	int sock = *((int*)socktId);

    while(1)
    {
        int ret= -1;
        //struct timeval tv;
       	//tv.tv_sec = 0;
        //tv.tv_usec = 0;

        fd_set rd_fd;
        SAL_FD_ZERO(&rd_fd);                          /*Socket collection clear*/
        SAL_FD_SET(sock, &rd_fd);/**/
        ret = sal_select(sock+ 1, &rd_fd, NULL, NULL, NULL);
        switch(ret)
        {
            case -1:
                DHCP_SERVER_RX_DBG("select error");
                break;
            case 0:
            	DHCP_SERVER_RX_DBG("time out");
                break;
            default:
                if(SAL_FD_ISSET(sock, &rd_fd))
                {
                    //osal_printf("yeah, some data come on\n");

                    /* receive the datagram */
					dhcpsock_handle(sock, NULL);
                }
                break;
        }
    }
    DHCP_SERVER_RX_DBG("qiut server task");
    shut_down(sock);
    return 0;
}
// Create a DHCP socket packet receiving task
void create_dhcp_socket_task(void * socktId)
{
	int ret = 0;
	ret = pthread_create(&dhcpser_thread_id, NULL, (void*)msgSockAdd, socktId);
	if (ret)
    {
        DHCP_SERVER_CMD_DBG("Create dhcp socket task  error.\n");
        return ;
    }
}
// Destroy a DHCP socket packet receiving task
void destroy_dhcp_socket_task(void)
{
	if(dhcpser_thread_id != -1)
	{
		pthread_cancel(dhcpser_thread_id);
		dhcpser_thread_id = -1;
	}	
}

int set_socket_status(sys_enable_t enable)
{
    int                      iRet = SYS_ERR_OK;

    if( enable == TRUE)
    {
        iRet = dhcp_initsocket();
        if ( SYS_ERR_OK == iRet )
        {
            create_dhcp_socket_task((void *)&g_Dhcp_Sockfd);
        }
    }
    else
    {
        if ( g_Dhcp_Sockfd > 0 )
        {
            //vosMsgSockDelete(g_Dhcp_Sockfd);
            destroy_dhcp_socket_task();
            sal_close(g_Dhcp_Sockfd);
            g_Dhcp_Sockfd = -1;
        }
    }

    return iRet;
}

#endif /*GBN_INCLUDE_DHCPRSOCKET*/

