/*******************************************************************************
Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
File Name: Dhcps.h
Description: The header files corresponding to Dhcps.c
Autor: Kniazkov Andrew
Version:1.0.0
*******************************************************************************/
#ifndef _DHCPS_H_
#define _DHCPS_H_
/**********************************HEADER FILE**************************************/
#include <common/sys_def.h>
#include "dhcp.h"
#include <libsal/sal_dhcp_server.h>

#define GBN_INCLUDE_DHCPRELAYSERVER
#define GBN_INCLUDE_IPPOOL
/**********************************DATA STRUCTURE************************************/

#define DEFAULT_MAX_IP_SUBNET 32

#define DHCP_MAX_LEASE_TIME    (8*24*3600)    
#define DHCP_MAX_RENEW_TIME   (4*24*3600)   
#define DHCP_MAX_REBIND_TIME  (7*24*3600)   
#define DHCP_HALF_HOURE    1800

#define DHCPRS_HOST_BUF_MAX             52
#define IPPOOL_BITMAP_BUF_LEN           1024
//#define IPPOOL_MAC_BUF_LEN              32

enum
{
   DHCP_DISCOVER_TYPE=1,
   DHCP_OFFER_TYPE,
   DHCP_REQUEST_TYPE,
   DHCP_DECLINE_TYPE,
   DHCP_ACK_TYPE,
   DHCP_NAK_TYPE,
   DHCP_RELEASE_TYPE,
   DHCP_INFORM_TYPE
};

/*DHCP SERVER deletes hash table entries through the address pool*/
enum
{   
   REMOVE_BY_IPPOOL,             
   REMOVE_BY_IPPOOL_SECTION,    
   REMOVE_BY_IPPOOL_SECTION_IP   
};

/*Used to obtain interface information in DHCP UDP packets*/
typedef struct dhcp_Interface_para
{
    ulong_t dhcp_sw; 
    ulong_t dhcp_dstIp; 
    ushort_t dhcp_vid;   
    ushort_t dhcp_port;
}DHCP_INTERFACE_PARA;

/*Message types used by DHCP SERVER module*/
enum 
{
    DHCPSERVER_RX_DISCOVER,
    DHCPSERVER_RX_REQUEST,
    DHCPSERVER_RX_DECLINE,
    DHCPSERVER_RX_RELAESE,
    DHCPSERVER_RX_INFORM,
};

typedef enum {
    DHCPRS_E_OK = 0,
    DHCPRS_E_FAILED,
    DHCPRS_E_MAX,
    DHCPRS_E_IP,
    DHCPRS_E_MASK,
    DHCPRS_E_POOL,
    DHCPRS_E_S_INVALID,
    DHCPRS_E_S_DISABLED,
    DHCPRS_E_RESOURCE,
    DHCPRS_E_END,
}DHCPRS_E_ENT;

/************************************FUNCTIONS**************************************/

void dhcpserver_debug_printf( char *exp, ...)
#ifdef __GNUC__
__attribute__ ((format (printf, 1, 2)))
#endif
;

int dhcpserver_init(void);
STATUS dhcpserverRxPkt(int iType, DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, 
    ulong_t ulVid, ulong_t ulServerIp ,DHCP_INTERFACE_PARA *if_Para);
STATUS dhcpserver_process_discover( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulServerIp,DHCP_INTERFACE_PARA *if_Para);
STATUS dhcpserver_process_request( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulServerIp ,DHCP_INTERFACE_PARA *if_Para);
STATUS dhcpserver_process_decline( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulServerIp ,DHCP_INTERFACE_PARA *if_Para);
STATUS dhcpserver_process_release( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulServerIp ,DHCP_INTERFACE_PARA *if_Para);
STATUS dhcpserver_rx_timertick(void);
STATUS dhcpserver_send_offer_packet( DHCP_HEAD_S *pDhcp, DHCP_OPTION_S *pDhcpOption, ulong_t ulIP, ulong_t ulServerIp,DHCP_INTERFACE_PARA *if_Para);
STATUS  dhcpserver_record_client_request_param(DHCP_OPTION_S *pstDhcpOption, ulong_t *pulReqParamFlag );
STATUS dhcpserver_send_ack_packet( DHCP_HEAD_S *pDhcp, DHCP_OPTION_S *pDhcpOption, ulong_t ulIP, ulong_t ulServerIp,DHCP_INTERFACE_PARA *if_Para);
STATUS dhcpserver_send_nck_packet( DHCP_HEAD_S *pDhcp, DHCP_OPTION_S *pDhcpOption, ulong_t ulIP, ulong_t ulServerIp ,DHCP_INTERFACE_PARA *if_Para);
void dhcpserver_show_hash (void );
STATUS dhcpserver_romovehash_by_ip ( ulong_t ulIP );
STATUS dhcpserver_romovehash_by_ippool(ulong_t ippool_gate, ulong_t ippool_mask);
STATUS dhcpserver_romovehash_by_ippool_section(ulong_t startIp, ulong_t endIp);
STATUS dhcpserver_set_debug_enable( ulong_t ulState );
STATUS dhcpserver_get_debug_enable( ulong_t *pState );

STATUS ipPoolCheckNameStr(char *szName);
STATUS ipPoolCheckIsIpPoolNull(ulong_t ulIndex);
STATUS IPPOOL_CheckIfPoolValid(ulong_t ulPoolIndex);
STATUS DeleteIpPool(uchar_t ucIndex);
STATUS ipPoolStartup(void);
ULONG dhcps_fund_ipbind_by_mac(uchar_t mac[6],ULONG *pulIpAddress,USHORT usVlanID);
STATUS dhcpserver_option60_process( uchar_t **ppucDhcpOption, DHCP_OPTION_S *pDhcpOption, DHCP_INTERFACE_PARA *if_Para);
STATUS  dhcp_show_packet ( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, char *pSyslogHead );
int dhcpserverIpBindSwitch(BOOL bEnable);
int dhcpseverIpBindGet(BOOL *pbEnable, BOOL *pbUnBoundAssign);
ULONG dhcp_snooping_addTo_macList(uchar_t *mac, ULONG ulIpAddress,USHORT usVlanID,char* pUserName);;
int dhcp_snooping_delfrom_macList(uchar_t *mac,USHORT usVlanID);
int dhcp_snooping_macList_get(ushort_t usStart, ushort_t *pusNextStart, ushort_t *pusBufNum, 
    dhcp_snooping_mac_lst_t nodeBuf[IPPOOL_MAC_BUF_LEN]);
int dhcpseverUnBoundAssign(BOOL bEnable);


#endif /*_INCLUDE_DHCPS_H_*/

