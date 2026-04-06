
#ifndef __CFG_DHCP_SERVER_H__
#define __CFG_DHCP_SERVER_H__

#include <common/sys_type.h>
#include <libsal/sal_dhcp_server.h>
#include <libipc/vos.h>

#define DHCP_SERVER_GROUP_MAX_NUM  8

#define L3INTFNUM          32
#define DHCP_GROUP_MAX_NUM  256
#define IP_POOL_MAX_NAME    (32 + 1)     
#define IP_POOL_ERR_MSG_NUM    35
#define IP_POOL_MAX_DNS_NUM     4        
#define IP_POOL_MAX_DNS_SUFFIX_NAME    (32+1) 
#define IP_POOL_MAX_NUM   (128)

#define IP_POOL_DEFAULT_SUBNET_MASK  0xFFFF0000 
#define IP_POOL_DEFAULT_LEASETIME (1*24*60*60)
#define IP_POOL_MAX_IPSECTION_NUM   (8)
#define IPPOOL_DEFAULT_SUBNET_MASK  0xFFFF0000 
#define IPPOOL_DEFAULT_LEASETIME (1*24*60*60)

#define DHCP_GLOBAL_CHANGE_FLAG              (0x1 << 0)
#define DHCP_INTF_CHANGE_FLAG                (0x1 << 1)
#define DHCP_GROUP_CHANGE_FLAG               (0x1 << 2)
#define DHCP_IPPOOL_CHANGE_FLAG              (0x1 << 3)
#define DHCP_PORT_CHANGE_FLAG                (0x1 << 4)

#define OPTION82STRATEGY_DROP      1
#define OPTION82STRATEGY_KEEP      2
#define OPTION82STRATEGY_REPLACE   3
#define OPTION82STRATEGY_APPEND_HOSTNAME    4
#define OPTION82STRATEGY_APPEND_HOSTNAME_IP    5

#define OPTION82FORMAT_NORMAL      1
#define OPTION82FORMAT_VERBOSE     2

#define OPTION82INFOFORMAT_HEX     1
#define OPTION82INFOFORMAT_ASCII   2

#define OPTION82_VERBOS_NODEID_LEN 60
#define OPTION82_CIRCUITID_LEN 80
#define OPTION82_REMOTEID_LEN 80

#define OPTION82_VERBOSE_NODEID_MAC          "\"mac\""
#define OPTION82_VERBOSE_NODEID_HOSTNAME     "\"hostname\""

#define IP_POOL_PRIMARY_DNS_NUM    1
#define IP_POOL_SECOND_DNS_NUM     2
#define IP_POOL_THIRD_DNS_NUM      3
#define IP_POOL_PFOURTH_DNS_NUM    4

#define IP_POOL_PRIMARY_WINS_NUM    1
#define IP_POOL_SECOND_WINS_NUM     2

#define IP_MAX_USER_NAME  (32+1)
#define  IPPOOL_MAX_DNS_SUFFIX_NAME    (32+1) 

typedef struct sys_dhcp_group_s
{
	uint32  flag;       					 
	uint32	group;	                        
} sys_dhcp_group_t;

typedef struct sys_intf_dhcp_s
{
	uint32  vid;        					 
	
    uint32 groupNo[DHCP_INTERFACE_MAX_NUM]; 
}sys_intf_dhcp_t;

typedef struct sys_dhcp_server_group_s
{
    
    sys_ipv4_t serverIp;    
    uint32  intf;           
} sys_dhcp_server_group_t;

typedef struct sys_ippool_section_S
{
    ulong_t   ulStartIp;          
    ulong_t   ulEndIp;            
    
}sys_ippool_section_t;

typedef struct sys_aaa_ippool
{
    char     szName[IP_POOL_MAX_NAME];
    uint32   IpPoolMsgNum[IP_POOL_ERR_MSG_NUM];
    uint32   ulSubnetMask;             
    uint32   ulGatewayIp;               
    uint32   ulRouterIp;                  
    uint32   ulDNSIp[IP_POOL_MAX_DNS_NUM];           
    char     szDnsSuffixName[IP_POOL_MAX_DNS_SUFFIX_NAME];    
  																
    uint32   ulPrimaryNBNSIp;           
    uint32   ulSecondNBNSIp;            

    uint32   ulLeaseTime;                

    sys_ippool_section_t   IpSection[IP_POOL_MAX_IPSECTION_NUM];
    sys_ippool_option43_t  option43;

}ippool_t;

typedef struct dhcpOption82_port_s
{
	BOOL  option82Enabled; 
	int  opion82Strategy;  

	char option82CircuitId[OPTION82_CIRCUITID_LEN + 1];	  
	char option82RemoteId[OPTION82_REMOTEID_LEN + 1];	 
	
}dhcpOption82_port_t;

typedef struct cfg_dhcp_server_s
{
	uint32                     dhcpConfigSync;
    sys_enable_t               enable;
    sys_enable_t               portRelayEnable[SYS_LOGICPORT_NUM_MAX];
    sys_intf_dhcp_t            intfDhcp[L3INTFNUM];
    sys_dhcp_server_group_t    dhcpGroup[DHCP_GROUP_MAX_NUM];
    sys_enable_t               option82Enable;
	sys_enable_t               option82DevEnabled;
    uint32                     opion82Strategy;  
    uint32                     option82Format;   
    uint32                     opion82InfoFormat; 
    char                       option82VerboseNodeId[OPTION82_VERBOS_NODEID_LEN + 1]; 

	dhcpOption82_port_t        portOption82[SYS_LOGICPORT_NUM_MAX];
	ippool_t                   gIpPool[IP_POOL_MAX_NUM];
	sys_enable_t               ipbindEnable;
	sys_enable_t               ipUnbindAssignEnable;
} cfg_dhcp_server_t;

extern int32 cfg_dhcp_server_enable_set(sys_enable_t enable);

extern int32 cfg_dhcp_server_db_init(void);
extern int32 cfg_dhcp_server_get(cfg_dhcp_server_t *pDhcpServer);
extern int32 cfg_dhcp_server_sync_clean(void);
extern int32 cfg_dhcp_server_enable_set(sys_enable_t enable);
extern int32 cfg_dhcp_server_enable_get(sys_enable_t *pEnable);
extern int32 cfg_dhcp_server_group_ip_set(uint32 group, sys_ipv4_t ipv4);
extern int32 cfg_dhcp_server_group_ip_get(uint32 group, sys_ipv4_t* ipv4, uint32* pIntf);
extern int32 cfg_dhcp_server_intf_group_ip_add(uint32 group, uint32 vid);
extern int32 cfg_dhcp_server_intf_group_ip_del(uint32 group, uint32 vid);
extern int32 cfg_dhcp_server_intf_group_ip_get(uint32 intf, sys_intf_dhcp_t* data);
extern int32 cfg_dhcp_server_intf_group_ip_init(uint32 intf);
extern int32 cfg_ip_pool_create(char *name);
extern int32 cfg_ip_pool_del(char *name);
extern int32 cfg_ip_pool_gateway_set(char *name, sys_ipv4_t addr, sys_ipv4_t mask);
extern int32 cfg_ip_pool_section_set(char *name, uint32 section, sys_ipv4_t start_ip, sys_ipv4_t end_ip);
extern int32 cfg_ip_pool_option43_set(char *name, sys_ippool_option43_t *option43);
extern int32 cfg_ip_pool_option43_get(char *name, sys_ippool_option43_t *option43);
extern int32 cfg_dhcp_relay_port_enable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 cfg_dhcp_relay_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 cfg_dhcp_server_IpBind_Switch_enable_set(sys_enable_t enable);
extern int32 cfg_dhcp_client_unknown_client_assign(sys_enable_t enable);
extern int32 cfg_dhcp_server_IpBind_get(sys_enable_t* enable, sys_enable_t* unBoundAssign);

extern int32 cfg_dhcp_relay_option82_enable_set(sys_enable_t enable);
extern int32 cfg_dhcp_relay_option82_enable_get(sys_enable_t *pEnable);
extern int32 cfg_dhcp_relay_option82_device_enable_set(sys_enable_t enable);
extern int32 cfg_dhcp_relay_option82_device_enable_get(sys_enable_t *pEnable);
extern int32 cfg_dhcp_relay_option82_format_set(uint32 format);
extern int32 cfg_dhcp_relay_option82_format_get(uint32 *format);
extern int32 cfg_dhcp_relay_option82_info_format_set(uint32 iFormat);
extern int32 cfg_dhcp_relay_option82_info_format_get(uint32 *iFormat);
extern int32 cfg_dhcp_relay_option82_format_verbose_set(char *option82VerboseNodeId);
extern int32 cfg_dhcp_relay_option82_format_verbose_get(char *option82VerboseNodeId);

extern int32 cfg_dhcp_relay_option82_port_enable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 cfg_dhcp_relay_option82_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 cfg_dhcp_relay_option82_port_strategy_set(sys_logic_port_t port, uint32 strategy);
extern int32 cfg_dhcp_relay_option82_port_strategy_get(sys_logic_port_t port, sys_enable_t *pStrategy);
extern int32 cfg_dhcp_relay_option82_port_circuit_id_set(sys_logic_port_t port, char *option82CircuitId);
extern int32 cfg_dhcp_relay_option82_port_circuit_id_get(sys_logic_port_t port, char *option82CircuitId);
extern int32 cfg_dhcp_relay_option82_port_remote_id_set(sys_logic_port_t port, char *option82RemoteId);
extern int32 cfg_dhcp_relay_option82_port_remote_id_get(sys_logic_port_t port, char *option82RemoteId);

extern int32 cfg_dhcp_server_dns_set(char *name, uint32 dnsNum, sys_ipv4_t ip);
extern int32 cfg_dhcp_server_dns_get(char *name, uint32 dnsNum, sys_ipv4_t *pIp);
extern int32 cfg_dhcp_server_dns_suffix_set(char *name, char *dnsSuffixName);
extern int32 cfg_dhcp_server_dns_suffix_get(char *name, char *dnsSuffixName);
extern int32 cfg_dhcp_server_wins_set(char *name, uint32 winsNum, sys_ipv4_t ip);
extern int32 cfg_dhcp_server_wins_get(char *name, uint32 winsNum, sys_ipv4_t *pIp);
extern int32 cfg_dhcp_server_lease_time_set(char *name, uint32 lease_time);
extern int32 cfg_dhcp_server_lease_time_get(char *name, uint32 *pLease_time);
extern int32 cfg_dhcp_server_init(void);
extern int32 cfg_ip_pool_get(char *name, ippool_t *pPool);
extern int32 cfg_dhcp_server_group_use_stat_set(uint32 group, uint32 vid);

#endif 

