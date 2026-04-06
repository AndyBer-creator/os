/*******************************************************************************
  Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
  File Name::  DhcpSocket.h
  Description: This file defines the data structures required for the DHCP module's socket receive task.
  Autor: Kniazkov Andrew
  Other Notes: None.
********************************************************************************/

#ifndef _DHCP_SOCKET_H_
#define	_DHCP_SOCKET_H_
#include <common/sys_def.h>
#include "libsal/sal_socket.h"
#include <net/if.h>
#include <sys/ioctl.h>
#include "dhcps.h"

#ifdef __cplusplus
extern "C"{
#endif


//#define	INADDR_ANY		(uint32)0x00000000

enum {enablesocket,disablesocket};

/*Function declaration*/

void dhcp_err_printf( char *exp, ...)
#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
;

int32 dhcpsock_main ( void );
int dhcpsock_sendto ( int socketfd, char * szBuffer, int iLength, 
                                           int iFlags, 
                                           sal_sockaddr_in  * pDestAddr, 
                                           int iTolen );
int dhcpsock_sendto2(char *szDHCPBuff, int iDHCPLen, ulong_t ulSrcIp, ulong_t ulDstIp, ushort_t usDstUdpPort);
int dhcpsock_sendBroadcast(char *szDHCPBuff, int iDHCPLen, 
    ulong_t ulSw, ushort_t usDstUdpPort);
int dhcp_initsocket (void);
char *dhcpsock_receivebuffer(ushort_t *pusBufferLen, ulong_t *pIp, DHCP_INTERFACE_PARA *ifPara);

int set_socket_status(sys_enable_t enable);

void dhcp_dot1x_hook_init(int flag);

#ifdef __cplusplus
}  
#endif  /* end of __cplusplus */

#endif   /* _DHCP_SOCKET_H */


