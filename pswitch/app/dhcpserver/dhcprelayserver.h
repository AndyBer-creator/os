/*******************************************************************************
 Copyright (C), 2021-2025, Symanitron-electronics, Ltd.
 File Name: dhcprelayserverTask.h
 Autor: Kniazkov Andrew
 Description: Public definition file for the dhcp relay/server
*******************************************************************************/
#ifndef _INCLUDE_DHCPRELAYSERVER_H_
#define _INCLUDE_DHCPRELAYSERVER_H_
/**********************************HEADER FILES**************************************/
#include <common/sys_def.h>
#include "dhcpHash.h"

/**********************************DATA STRUCTURE************************************/

/*Process Name*/
#define DHRS_PROCESS_NAME  GLN_PROC_DIR"dhcpsrs"

/*Decompiled module markers, 0x0001 used by dhcp snooping*/
#define DHRS_FUNC_FLAG_DHCP           (0x0002 << BLDRUN_FLAG_USER_OFF)
#define DHRS_FUNC_FLAG_IPPOOL         (0x0004 << BLDRUN_FLAG_USER_OFF)

#define DHCPINTERFACEMAXNUM 8
#define DHCPEMPTYGROUP 0

#if 0
/*HASH structure to save user information*/
#define HOSTNAME_LEN       16     /*Record up to the first 15 characters of the computer name,
                                    consistent with NetBIOS*/

typedef struct tagMACTOVLAN_HASHDATA
{
    uchar_t  ucUserMac[6]; /*MAC address of the user host*/
    ushort_t usVlanId;       /* User's Vlan ID  */
    ushort_t usPort;         /* User's port number */ 
    ulong_t  ulIpAddr;       /*IP address assigned to the user by the DHCP Server*/
    char     hostname[HOSTNAME_LEN];
}MACHASH_S;
#endif

/*Option82 message processing policy*/
#define OPTION82STRATEGY_DROP      1/*discarding packets*/
#define OPTION82STRATEGY_KEEP      2/*maintain th original option82 content*/
#define OPTION82STRATEGY_REPLACE   3/*Change option82 content*/
#define OPTION82STRATEGY_APPEND_HOSTNAME    4/*append option82 suboption 9 with hostname*/
#define OPTION82STRATEGY_APPEND_HOSTNAME_IP    5/*append option82 suboption 9 with hostname and IP*/

/*Option82 message format*/
#define OPTION82FORMAT_NORMAL      1
#define OPTION82FORMAT_VERBOSE     2
#define OPTION82FORMAT_HENAN       3

/*option82 information formatʽ*/
#define OPTION82INFOFORMAT_HEX     1
#define OPTION82INFOFORMAT_ASCII   2
#if 0
/*Data structure definition*/
#define IP_MAX_USER_NAME  (32+1)

typedef struct dhcp_snooping_mac_lst_s{
    struct dhcp_snooping_mac_lst_s *next;
    uint8  mac[6];
    uint32 usVlanID;   /*interface VID where the DHCP message is sent*/
    uchar_t   szUserName[IP_MAX_USER_NAME];   
    ULONG ulIpAddress;
}dhcp_snooping_mac_lst_t;

typedef enum {
    DATA_OP_ADD     = 0x0,
    DATA_OP_DELETE,
    DATA_OP_GET,
    DATA_OP_GETNEXT
}DATA_OP_ENT;
#endif

struct udphdr
{
  u_int16_t source;
  u_int16_t dest;
  u_int16_t len;
  u_int16_t check;
} __attribute__((packed));

typedef union
    {
    int     i;
    uint32  u;
    void *  p;
    } GENERIC_ARGUMENT;

#if 0
#define IPPOOL_OPTION43_MAX_LEN 64      

typedef struct IPPOOL_OPTION43_tag{
    uchar_t        ucFormat;
    uchar_t        ucValueLen;
    uchar_t        szValue[IPPOOL_OPTION43_MAX_LEN + 1];
}IPPOOL_OPTION43;

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
#endif

#define DHCPRS_HOST_BUF_MAX             52
#define IPPOOL_BITMAP_BUF_LEN           1024
#define IPPOOL_MAC_BUF_LEN              32
#define	IPVERSION	                    4
#define	MAXTTL		                    255	
#define ETHERTYPE_IP		            0x0800

/***********************************FUNCTION***************************************/

STATUS dhcprelay_setzero_by_vlanif( ulong_t ulIfNo );
int dhcprelayCheckGroupUsed(ulong_t ulGroupNum, ulong_t *pbUsed, ulong_t* vid);
int dhcprelayIfGroupCfg(ulong_t ulSw, BOOL bAdd, ulong_t ulGroupNum);
int dhcprelayIfGroupOp(DATA_OP_ENT op, ulong_t *pulSw, ulong_t* pVid, ulong_t groupNo[DHCPINTERFACEMAXNUM]);
//int dhcprelayPortlistEnableGet(vd_portlist_t portlistEnabled);
int dhcprelayPortEnableSet(ulong_t ulPort, ulong_t bEnable);
int dhcpserverHashDataGet(ushort_t usStart, ushort_t *pusNextStart,
    ushort_t *pusBufNum, MACHASH_S hostBuf[DHCPRS_HOST_BUF_MAX]);
int dhcpseverHashNumGet(ulong_t *pulNum);
int dhcpseverHashDataGetByKey(MACHASH_S *pDataBuf);
int dhcpseverTrapsCfg(BOOL bTrapSend);
int dhcpoption82Cfg(BOOL bEnable);
int dhcpoption82DevCfg(BOOL bEnable);
int dhcpoption82PortCfg(ulong_t ulPort, BOOL bEnable);
int dhcpoption82PortStrategyCfg(ulong_t ulPort, int iStrategy);
int dhcpoption82FormatCfg(int iFormat);
int dhcpoption82InfoFormatCfg(int iFormat);
int dhcpoption82FormatVerbose(uchar_t option82VerboseNodeId[OPTION82_VERBOS_NODEID_LEN + 1]);
int dhcpoption82PortCircuitid(ulong_t ulPort, uchar_t option82CircuitId[OPTION82_CIRCUITID_LEN + 1]);
int dhcpoption82PortRemoteid(ulong_t ulPort, uchar_t option82RemoteId[OPTION82_REMOTEID_LEN + 1]);
int dhcpooption82PortSuboption9(ulong_t ulPort, uchar_t option82SubOpt9[OPTION82_SUBOPTION9_LEN + 1]);
/*When bEnable is TRUE, ucVlanList is also used as an error output.If successful, the corresponding vlan bit is cleared*/
int dhcpoption82VlanlistCfg(BOOL bEnable, uchar_t *ucVlanList);
/*ucVlanList is also used as an error output. The corresponding VLAN bit is cleared after success*/
int dhcpoption82VlanlistStrategy(int iStrategy, uchar_t *ucVlanList);
/*ucVlanList is also used as an error output. The corresponding VLAN bit is cleared after success*/
int dhcpoption82VlanlistCircuitid(uchar_t option82CircuitId[OPTION82_CIRCUITID_LEN + 1], 
    uchar_t *ucVlanList);
/*ucVlanList is also used as an error output. The corresponding VLAN bit is cleared after success*/
int dhcpoption82VlanlistRemoteid(uchar_t option82RemoteId[OPTION82_REMOTEID_LEN + 1], 
    uchar_t *ucVlanList);
int dhcpoption82GlobalGet(dhcpOption82Policy_t *pGlobalCfg);
int dhcpoption82PortGet(ulong_t ulPort, dhcpOption82Policy_port_t *pPortCfg);
int dhcpoption82VlanGet(DATA_OP_ENT op, ulong_t *pulVid, dhcpOption82Policy_vlan_t *pVlanCfg);
/*When bAdd is FALSE, pMatch is ULL to delete*/
int dhcpoption60Cfg(ulong_t ulSw, BOOL bAdd, dhcpOption60Match_t *pMatch, 
    int *piErrNo);
int dhcpoption60CfgGet(DATA_OP_ENT op, ulong_t *pulSw, dhcpOption60Match_t entryBuf[DHCPINTERFACEMAXNUM]);
int dhcpRelayMaxHopsSet(ulong_t ulHops);
int dhcpRelayMaxHopsGet(ulong_t *pulHops);
STATUS ipPoolGetPoolIndexByName(char * szPoolName, ulong_t * pPoolIndex);
STATUS ipPoolCheckIsIpPoolInUse(ulong_t ulIndex);
STATUS deleteDisableIp(ulong_t ulPoolIndex, ulong_t ulSectionIndex);
int ipPoolSectionUsedGet(ulong_t ulPoolIndex, ulong_t ulSectionIndex,
    ulong_t *pulInUsed, ulong_t *pulUnUsed);
int ipPoolSectionBitmapGet(ulong_t ulPoolIndex, ulong_t ulSectionIndex, 
    ushort_t *pwMinAllocNo, uchar_t bitmapBuf[IPPOOL_BITMAP_BUF_LEN]);
int ipPoolGatewayCheck(ulong_t ulPoolIndex, ulong_t ulGatewayIp, 
    ulong_t ulMask, int *piErrNo);
int ipPoolLeaseTimeSet(ulong_t ulPoolIndex, ulong_t ulTime);
int ipPoolIprangeCheck(ulong_t ulStartIp, ulong_t ulEndIp);
int ipPoolDisableIp(BOOL bDisable, char *szPoolName, ulong_t ulIp, int *piErrNo);
STATUS ippool_set_debug_enable( ulong_t ulState );
ULONG dhcp_snooping_addTo_macList(uchar_t *mac, ULONG ulIpAddress,USHORT usVlanID,char* pUserName);
int dhcp_snooping_delfrom_macList(uchar_t *mac,USHORT usVlanID);
int dhcp_snooping_clear_macList(void);
int dhcp_snooping_macList_get(ushort_t usStart, 
    ushort_t *pusNextStart, ushort_t *pusBufNum, 
    dhcp_snooping_mac_lst_t nodeBuf[IPPOOL_MAC_BUF_LEN]);
int dhcpserverIpBindSwitch(BOOL bEnable);
int dhcpseverIpBindGet(BOOL *pbEnable, BOOL *pbUnBoundAssign);
int dhcpseverUnBoundAssign(BOOL bEnable);
STATUS IPPOOL_SetOpton43_ByIndex(ulong_t ulPoolIndex, IPPOOL_OPTION43 *pOption43);
STATUS IPPOOL_GetOpton43_ByIndex(ulong_t ulPoolIndex, IPPOOL_OPTION43 *pOption43);
STATUS IPPOOL_SetOpton43_ByName(char *szPoolName, IPPOOL_OPTION43 *pOption43);
int dhcpserverDnsCfg(char *szPoolName, uint32 dns_num, sys_ipv4_t addr);
int dhcpserverDnsSuffixCfg(char *szPoolName, char *dnsSuffixName);
int ipPoolLeaseTimeCfg(char *szPoolName, ulong_t ulTime);
int initIpdisable(void);
int ipPoolDisableIpGet(char *szPoolName, ulong_t ulIp[GET_NUM_ONE]);
int static_mac_ip_bind_check_pool(ulong_t ulIp, ulong_t* used);

#endif /*_INCLUDE_DHCPRELAYSERVERTASK_H_*/

