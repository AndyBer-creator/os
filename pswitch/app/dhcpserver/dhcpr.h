/*******************************************************************************
Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
File Name: Dhcpr.h
Autor: Kniazkov Andrew
Description: Header the corresponding to Dhcpr
*******************************************************************************/
#ifndef _INCLUDE_DHCPR_H_
#define _INCLUDE_DHCPR_H_
/**********************************HEADER FILES**************************************/
#include <common/sys_def.h>
#include <libsal/sal_l3_intf.h>

#define dhcp_debug_printf osal_printf
/**********************************DATA STRUCTURE************************************/

#define DHCPINTERFACEMAXNUM 8
typedef struct tag_dhcp_ServerGroup
{
    uint32 groupNo; /*DHCP Server groupe number*/
    uint32 serverIp; /*DHCP Server primary IP address*/
    uchar_t rowstatus; /*Row status*/
}DHCP_SERVER_S;

typedef struct tag_dhcp_InterfaceGroup
{
	uint32  vid;        					 //VLAN corresponding to the interface
    uint32 groupNo[DHCPINTERFACEMAXNUM]; /* DHCP server configuration item index number*/
}VLANIfToDHCPS_S;
struct	ether_header {
	uint8	ether_dhost[ETHER_ADDR_LEN];
	uint8	ether_shost[ETHER_ADDR_LEN];
	uint16	ether_type;
} PACKED ;

#ifndef DHCP_ENABLE
#define DHCP_ENABLE    1
#endif
#ifndef DHCP_DISABLE
#define DHCP_DISABLE    2
#endif
//#define	INADDR_BROADCAST	(u_long)0xffffffff
//#define ETHERTYPE_IP		0x0800
//#define IPPROTO_UDP          17  /* User Datagram Protocol */
#ifndef bcmp
#define bcmp(s1, s2, n) memcmp ((s1), (s2), (n))
#endif
#ifndef bcopy
#define bcopy(s, d, n)  memcpy ((d), (s), (n))
#endif
#ifndef bzero
#define bzero(s, n)     memset ((s), 0, (n))
#endif

/************************************FUNCTION**************************************/

//void dhcp_debug_printf( char *exp, ...)

int dhcprelay_init(void);
int dhcprelayRxSocket(BOOL bFromDhcpsnooping, DHCP_HEAD_S *pDhcp,
    ulong_t ulDhcpLen, ulong_t ulIP, DHCP_INTERFACE_PARA *ifPara);
int dhcprelayRxFromLocalserver(DHCP_HEAD_S *pDhcp, ulong_t ulDhcpLen, DHCP_INTERFACE_PARA *if_Para);
STATUS dhcprelay_prcess_socketdate(BOOL bFromDhcpsnooping,
    DHCP_HEAD_S *pDhcp, ulong_t ulDhcpLen, ulong_t ulIP, DHCP_INTERFACE_PARA *ifPara);
STATUS dhcprelay_process_localserverdate ( DHCP_HEAD_S *pDhcp, ulong_t ulDhcpLen,DHCP_INTERFACE_PARA *ifPara);
STATUS dhcprelay_process_dhcpdiscover( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara);
STATUS dhcprelay_get_serverip_by_vlan( ulong_t ulIfNo, ulong_t pDhcpServrIp[DHCPINTERFACEMAXNUM] );
BOOL dhcprelay_ipis_serverip_of_vlan( ulong_t ulvid, ulong_t ulIp );
STATUS dhcprelay_check_local_server ( ulong_t ulIfaddr, ulong_t ulDhcpServerIp );
STATUS dhcprelay_process_dhcpoffer( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, DHCP_INTERFACE_PARA *ifPara);
STATUS dhcprelay_process_dhcprequest( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, DHCP_INTERFACE_PARA *ifPara);
STATUS dhcprelay_process_dhcpack( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara);
STATUS dhcprelay_process_dhcpnak( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara);
STATUS dhcprelay_process_dhcpreleasedecline( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen ,DHCP_INTERFACE_PARA *ifPara);
STATUS dhcprelay_send_broadcast_to_client ( ulong_t ulIfAddr,
    DHCP_HEAD_S *pDhcp, ulong_t ulDhcpLen, ulong_t ulVid, ulong_t ulPort,
    ulong_t ulGatewaySw);
STATUS dhcprelay_set_enble_state( ulong_t ulState );
STATUS dhcprelay_get_enble_state( ulong_t *pState );
STATUS dhcprelay_check_ip_valid( ulong_t ulIpAddr );
STATUS dhcp_otion82_process(DHCP_HEAD_S *pDhcp, ulong_t ulMsgType, ulong_t ulDhcpLen, ulong_t *ulNewDhcpLen, DHCP_INTERFACE_PARA *ifPara);
STATUS dhcp_otion82_vlan_process(DHCP_HEAD_S *pDhcp, ulong_t ulMsgType, ulong_t ulDhcpLen, ulong_t *ulNewDhcpLen, DHCP_INTERFACE_PARA *ifPara);
ulong_t dhcp_index2IpAddr( ulong_t  *ipIndex );

//int dhcpoption82_avl_compare(void * node, GENERIC_ARGUMENT key);
dhcpOption82Policy_vlan_t *dhcpoption82_add_avlnode(ulong_t ulVlan);
STATUS dhcpoption82_delete_avlnode(ulong_t ulVlan);
dhcpOption82Policy_vlan_t *dhcpoption82_get_avlnode(ulong_t ulVlan);
dhcpOption82Policy_vlan_t *dhcpoption82_get_next_avlnode(ulong_t ulVlan);
STATUS  dhcp_show_packet ( DHCP_HEAD_S *pDhcp , ulong_t ulDhcpLen, char *pSyslogHead );
STATUS  dhcp_user_pkt_check(DHCP_HEAD_S *pDhcp, uchar_t *pstMac);
STATUS dhcp_record_all_opt(DHCP_HEAD_S *pDhcp, ulong_t lOptionLen,DHCP_OPTION_S *pDhcpOpt);
uchar_t  *dhcp_ipaddr_to_str(ulong_t ulIPAddr, uchar_t * string);
STATUS dhcp_str_to_ipaddr(char *string, ulong_t *ulRet);
int32 dhcprelay_get_ifaddr(ulong_t sw, ushort_t vid, ulong_t *ifAddr);
STATUS dhcprelay_add_gateway(ulong_t ulIfaddr, DHCP_HEAD_S *pDhcp ,DHCP_INTERFACE_PARA *ifPara,DHCP_OPTION_S * pDhcpOption,ulong_t *pserver);
STATUS dhcprelay_get_ifaddr_by_vid(ulong_t ulVid, ulong_t *pulSw, ulong_t *ifAddr);
int dhcpRelayMaxHopsGet(ulong_t *pulHops);
int dhcprelayEnableCfg(ulong_t enable);
int dhcprelayServerIPCfg(uint32 uiGroupId, sys_ipv4_t ipv4);
int dhcprelayIfGroupCfg(ulong_t ulSw, BOOL bAdd, ulong_t ulGroupNum);
int dhcpserverIPPoolCfg(char *szPoolName, BOOL add);
int dhcprelayPortEnableSet(ulong_t ulPort, ulong_t bEnable);
int dhcprelaySystemMacCfg(uchar_t *pMac);
int dhcprelaySystemOidCfg(ulong_t oid);
int dhcprelayHostnameCfg(uchar_t *pHostname);

#endif /*_INCLUDE_DHCPR_H_*/

