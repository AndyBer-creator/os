
#ifndef __SAL_DHCP_SERVER_H__
#define __SAL_DHCP_SERVER_H__

#include <rsd/rsd_dhcp_server.h>
#include <libipc/vos.h>

#define DHCP_SERVER_PATH "dhcp_server_ipc_"
#define IP_MAX_USER_NAME                (32+1)
#define IPPOOL_MAC_BUF_LEN              32
#define HOSTNAME_LEN                    16
#define DHCPRS_HOST_BUF_MAX             52
#define IPPOOL_MAX_DAY                  999
#define GET_NUM_ONE                     100

#define IP_POOL_OPTION_FORMAT_ASCII 0
#define IP_POOL_OPTION_FORMAT_HEX   1
#define IP_POOL_OPTION_FORMAT_IPV4  2
#define IP_POOL_OPTION43_MAX_LEN 64

typedef struct sys_ippool_option43_s{
	uchar_t 	   ucFormat;
	uchar_t 	   ucValueLen;
	uchar_t 	   szValue[IP_POOL_OPTION43_MAX_LEN + 1];
}sys_ippool_option43_t;

#ifndef DHCP_INTERFACE_MAX_NUM
#define DHCP_INTERFACE_MAX_NUM 8
#endif
#ifndef DHCPINTERFACEMAXNUM
#define DHCPINTERFACEMAXNUM 8
#endif
#define DHCPEMPTYGROUP 0

typedef enum{
    DHCP_ERROR_SECTION_STARTIP_INVALID = 1,
    DHCP_ERROR_SECTION_ENDIP_INVALID,
    DHCP_ERROR_SECTION_STARTIP_HIGHER,
    DHCP_ERROR_SECTION_LENGTH_TOOBIG,
    DHCP_ERROR_SECTION_INDEX_TOOBIG,
    DHCP_ERROR_SECTION_NO_GATEWAY,
    DHCP_ERROR_SECTION_IPGATE_NOSAMESUBNET,
    DHCP_ERROR_SECTION_OVERLAP,
    DHCP_ERROR_SECTION_NUM_FULL,
    DHCP_ERROR_DISABLE_IP_NUM_FULL,
    DHCP_ERROR_SECTION_IP_INUSED,
}DHCP_ERR_CODE;
typedef enum {
    DHCPS_FUNC_START,
    DHCPS_FUNC_ENABLE,
    DHCPS_FUNC_SERVER_IP_SET,
    DHCPS_FUNC_GROUP_SET,
    DHCPS_FUNC_CHECK_GROUPNUM_USED,
    DHCPS_FUNC_IF_GROUP_OP,
    DHCPS_FUNC_IP_POOL_SET,
    DHCPS_FUNC_GATEWAY_SET,
    DHCPS_FUNC_SECTION_SET,
    DHCPS_FUNC_PORT_RELAY_ENABLE,
    DHCPS_FUNC_IP_BIND_SWITCH_SET,
    DHCPS_FUNC_IP_BIND_SWITCH_GET,
    DHCPS_FUNC_IPPOOL_ADDTO_MACLIST,
    DHCPS_FUNC_IPPOOL_DELFROM_MACLIST,
    DHCPS_FUNC_IPPOOL_MACLIST_GET,
    DHCPS_FUNC_UNBIND_ASSIGN_SET,
    DHCPS_FUNC_HASH_DATA_GET,
    DHCPS_FUNC_HASH_DATA_GET_BY_KEY,
    DHCPS_FUNC_HASH_NUM_GET,
	DHCPS_FUNC_OPTION82_CFG,
	DHCPS_FUNC_OPTION82_DEV_CFG,
	DHCPS_FUNC_OPTION82_PORT_CFG,
	DHCPS_FUNC_OPTION82_PORT_STRATEGY_CFG,
	DHCPS_FUNC_OPTION82_FORMAT_CFG,
	DHCPS_FUNC_OPTION82_INFO_FORMAT_CFG,
	DHCPS_FUNC_OPTION82_FORMAT_VERBOSE,
	DHCPS_FUNC_OPTION82_PORT_CIRCUITID,
	DHCPS_FUNC_OPTION82_PORT_REMOTEID,
	DHCPS_FUNC_OPTION82_PORT_SUBOPTION9,
	DHCPS_FUNC_OPTION82_VLANLIST_ENABLE,
	DHCPS_FUNC_OPTION82_VLANLIST_STRATEGY,
	DHCPS_FUNC_OPTION82_VLANLIST_CIRCUITID,
	DHCPS_FUNC_OPTION82_VLANLIST_REMOTEID,
	DHCPS_FUNC_OPTION82_GLOBAL_GET,
	DHCPS_FUNC_OPTION82_PORT_GET,
	DHCPS_FUNC_OPTION82_VLAN_GET,
	DHCPS_FUNC_DNS_IP_SET,
	DHCPS_FUNC_DNS_SUFFIX_SET,
	DHCPS_FUNC_WINS_IP_SET,
	DHCPS_FUNC_IPPOOL_LEASETIME_SET,
    DHCPS_FUNC_SYSTEM_MAC_SET,
    DHCPS_FUNC_SYSTEM_OID_SET,
    DHCPS_FUNC_HOSTNAME_SET,
    DHCPS_FUNC_IPPOOL_DISABLE_IP,
    DHCPS_FUNC_IPPOOL_DISABLE_IP_GET,
    DHCPS_FUNC_IPPOOL_OPTION43_SET_BY_INDEX,
    DHCPS_FUNC_STATIC_IP_CHECK_GET,
    DHCPS_FUNC_MAX
}DHCPS_FUNC_ENT;

typedef struct dhcp_snooping_mac_lst_s{
    struct dhcp_snooping_mac_lst_s *next;
    uint8   mac[6];
    uint16  usVlanID;   
    char    szUserName[IP_MAX_USER_NAME];
    uint32  ulIpAddress;
}dhcp_snooping_mac_lst_t;

typedef struct tagMACTOVLAN_HASHDATA
{
	uint8 ucUserMac[6]; 
	uint16 usVlanId;		 
	uint16 usPort;		 
	uint32 ulIpAddr;		 
	char    hostname[HOSTNAME_LEN];
	uint32  bindFlag;
}MACHASH_S;

extern int32 sal_dhcp_server_enable_set(sys_enable_t enable);

extern int32 sal_dhcp_server_db_init(void);
extern int32 sal_dhcp_server_enable_get(sys_enable_t *pEnable);
extern int32 sal_dhcp_server_group_ip_add(uint32 uiGroupId, sys_ipv4_t pIp);
extern int32 sal_dhcp_server_group_ip_del(uint32 uiGroupId);
extern int32 sal_dhcp_server_group_ip_get(uint32 uiGroupId, sys_ipv4_t *ipv4, uint32 *pIntf);
extern int32 sal_dhcp_server_group_ip_clear(void);
extern int32 sal_dhcp_server_intf_group_ip_add(uint32 uiGroupId, uint32 vid);
extern int32 sal_dhcp_server_intf_group_ip_del(uint32 uiGroupId, uint32 vid);
extern int32 sal_dhcp_server_intf_group_ip_get(DATA_OP_ENT op, uint32* intfId, uint32* pVid, uint32 groupNo[DHCP_INTERFACE_MAX_NUM]);
extern int32 sal_dhcp_server_intf_group_ip_init(void);
extern int32 sal_dhcp_server_intf_group_by_vid_get(uint32 vid, uint32* group);
extern int32 sal_ip_pool_create(char *name);
extern int32 sal_ip_pool_del(char *name);
extern int32 sal_ip_pool_gateway_set(char *name, sys_ipv4_t addr, sys_ipv4_t mask);
extern int32 sal_ip_pool_section_set(char *name, uint32 id, sys_ipv4_t start_ip, sys_ipv4_t end_ip);
extern int32 sal_dhcp_relay_port_enable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 sal_dhcp_relay_port_enable_get(sys_logic_port_t port, sys_enable_t *enable);
extern int32 sal_dhcp_server_IpBind_Switch_enable_set(sys_enable_t enable);
extern int32 sal_dhcp_client_host_mac_ip_add(sys_mac_t* pMac, sys_ipv4_t ipaddr, sys_vid_t vid, char* pUserName);
extern int32 sal_dhcp_client_host_mac_ip_del(sys_mac_t* pMac, sys_vid_t vid);
extern int32 sal_dhcp_client_host_mac_ip_get(uint16 usStart, uint16 *pusNextStart, uint16 *pusBufNum,
		dhcp_snooping_mac_lst_t nodeBuf[IPPOOL_MAC_BUF_LEN]);
extern int32 sal_dhcp_server_IpBind_get(sys_enable_t* enable, sys_enable_t* unBoundAssign);
extern int32 sal_dhcp_client_unknown_client_assign(sys_enable_t enable);

extern int32 sal_dhcp_server_dns_set(char *name, uint32 dnsNum, sys_ipv4_t ip);
extern int32 sal_dhcp_server_dns_get(char *name, uint32 dnsNum, sys_ipv4_t *ip);
extern int32 sal_dhcp_server_dns_suffix_set(char *name, char *dnsSuffixName);
extern int32 sal_dhcp_server_dns_suffix_get(char *name, char *dnsSuffixName);
extern int32 sal_dhcp_server_wins_set(char *name, uint32 winsNum, sys_ipv4_t ip);
extern int32 sal_dhcp_server_wins_get(char *name, uint32 winsNum, sys_ipv4_t *ip);
extern int32 sal_dhcp_server_lease_time_set(char *name, uint32 lease_time);
extern int32 sal_dhcp_server_lease_time_get(char *name, uint32 *lease_time);

extern int32 sal_dhcp_relay_option82_enable_set(sys_enable_t enable);
extern int32 sal_dhcp_relay_option82_enable_get(sys_enable_t *pEnable);
extern int32 sal_dhcp_relay_option82_device_enable_set(sys_enable_t enable);
extern int32 sal_dhcp_relay_option82_device_enable_get(sys_enable_t *pEnable);
extern int32 sal_dhcp_relay_option82_format_set(uint32 format);
extern int32 sal_dhcp_relay_option82_format_get(uint32 *format);
extern int32 sal_dhcp_relay_option82_info_format_set(uint32 iFormat);
extern int32 sal_dhcp_relay_option82_info_format_get(uint32 *iFormat);
extern int32 sal_dhcp_relay_option82_format_verbose_set(char *option82VerboseNodeId);
extern int32 sal_dhcp_relay_option82_format_verbose_get(char *option82VerboseNodeId);
extern int32 sal_dhcp_relay_option82_port_enable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 sal_dhcp_relay_option82_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 sal_dhcp_relay_option82_port_strategy_set(sys_logic_port_t port, uint32 strategy);
extern int32 sal_dhcp_relay_option82_port_strategy_get(sys_logic_port_t port, uint32 *pStrategy);
extern int32 sal_dhcp_relay_option82_port_circuit_id_set(sys_logic_port_t port, char *option82CircuitId);
extern int32 sal_dhcp_relay_option82_port_circuit_id_get(sys_logic_port_t port, char *option82CircuitId);
extern int32 sal_dhcp_relay_option82_port_remote_id_set(sys_logic_port_t port, char *option82RemoteId);
extern int32 sal_dhcp_relay_option82_port_remote_id_get(sys_logic_port_t port, char *option82RemoteId);
extern int32 ipc_dhcp_system_mac_set(sys_mac_t *pMac);
extern int32 ipc_dhcp_system_oid_set(uint32 oid);
extern int32 ipc_dhcp_hostname_set(char *hostname);
extern int32 sal_dhcp_client_hash_get(uint16 usStart, uint16 *pusNextStart,
    uint16 *pusBufNum, MACHASH_S hostBuf[DHCPRS_HOST_BUF_MAX]);
extern int32 sal_ip_pool_disable_ip_set(char *name, uint32 enable, sys_ipv4_t ipv4);
extern int32 sal_ip_pool_disable_ip_get(char *name, sys_ipv4_t ipv4[GET_NUM_ONE]);
extern int32 sal_ip_pool_option43_set(char *name, sys_ippool_option43_t *option43);
extern int32 sal_dhcp_server_check_group_used(uint32 group, uint32* pUsed, uint32* vid);
#endif 

