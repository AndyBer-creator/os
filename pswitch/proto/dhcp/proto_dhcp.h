
#ifndef __PROTO_DHCP_H__
#define __PROTO_DHCP_H__
#include <common/sys_def.h>
#define DHCP_RATE_LAST_OVER_SEC       2
#define DHCP_RATE_TICK_PER_SEC        10
#define SYS_DHCP_O82_CID_VLAN_MAX   (64)

#define SYS_DHCP_TRANSATION_MAX     (256)
#define SYS_DHCP_TRANSATION_AGEOUT  (30)
#define SYS_DHCP_O82_MAX_LEN   ((64 * 2) + 4)

typedef enum sys_dhcp_option_e
{
    DHCP_DISCOVER = 1,
    DHCP_OFFER,
    DHCP_REQUEST,
    DHCP_DECLIENT,
    DHCP_ACK,
    DHCP_NACK,
    DHCP_RELEASE,
    DHCP_INFO,
}sys_dhcp_option_t;

typedef enum  sys_dhcp_ipFilter_mode_e
{
    FILTER_MODE_NONE = 0,
    ISG_FILTER_MODE_IP_ONLY,
    ISG_FILTER_MODE_IP_MAC,
    FILTER_MODE_END,
} sys_dhcp_ipFilter_mode_t;

typedef enum  sys_dhcp_option82_act_e
{
    DHCP_OPTION82_KEEP = 0,
    DHCP_OPTION82_DROP,
    DHCP_OPTION82_REBUILD,
    DHCP_OPTION82_END,
} sys_dhcp_option82_act_t;

typedef enum  sys_dhcp_option82_format_e
{
    DHCP_OPTION82_FORMAT_NORMAL = 0,
    DHCP_OPTION82_FORMAT_USERDEFINE,
    DHCP_OPTION82_FORMAT_END,
} sys_dhcp_option82_format_t;
typedef struct ipHdr_s
{
    
    uint8 ip_vhl;
    uint8 ip_tos;     
    uint16 ip_len;     
    uint16 ip_id;      
    uint16 ip_off;     
    uint8 ip_ttl;     
    uint8 ip_p;       
    uint16 ip_sum; 
    uint32 ip_src,ip_dst;	
}__attribute__ ((aligned(1), packed))ipHdr_t;

typedef struct udphdr_s
{
    uint16 uh_sport;
    uint16 uh_dport;
    uint16 uh_ulen;
    uint16 uh_sum;
}__attribute__ ((aligned(1), packed))udphdr_t;

typedef struct dhcpMessage
{
  uint8 op;          
  uint8 htype;    
  uint8 hlen;      
  uint8 hops;     
  int32 xid;           
  uint16 secs;   
  uint16 flags;
  uint32 ciaddr; 
  uint32 yiaddr; 
  uint32 siaddr; 
  uint32 giaddr; 
  uint8 chaddr[16];  
  uint8 sname[64]; 
  uint8 file[128];       
  uint32 mcookie;    
  uint8 options[1300];    
} __attribute__ ((aligned(1), packed)) dhcpMessage;

typedef struct dhcp_option82tlv_normal_s
{
    uint8 opcode;
    uint8 len;
    uint8 suOption1;
    uint8 sub_option1Len;
    uint16 vid;
    uint16 port;
    uint8 suOption2;
    uint8 sub_option2Len;
    sys_mac_t mac;
} __attribute__ ((aligned(1), packed))dhcp_option82tlv_normal_t;

typedef struct sys_dhcp_mib_s
{
    uint32 dhcpForward;                      
    uint32 dhcpChaddrChkDrop;                
    uint32 dhcpUntrustDrop;                  
    uint32 dhcpUntrustWithOption82Drop;      
    uint32 dhcpInvalidDrop;                  
}sys_dhcp_mib_t;

typedef struct sys_dhcp_option82_id_s
{
    char str[SYS_BUF64_LEN];
}sys_dhcp_option82_id_t;

typedef struct sys_dhcp_option82_circuit_id_s
{
    sys_vid_t vid;
    char str[SYS_BUF64_LEN];
}sys_dhcp_option82_circuit_id_t;

typedef struct dhcp_info_s
{
    uint8 opcode;
    uint8 hasOption82;
    sys_vid_t vid;
    sys_logic_port_t port;
    uint32 leaseTime;
}dhcp_info_t;

typedef struct dhcp_request_transation_s
{
    struct dhcp_request_transation_s *prev;
    struct dhcp_request_transation_s *next;
    uint8 timer;
    sys_mac_t mac;
    sys_vid_t vid;
    sys_logic_port_t port;
}dhcp_request_transation_t;
extern int32 dhcp_init(void);
extern int32 dhcp_exit(void);
extern int32 dhcp_option82_enable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 dhcp_option82_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 dhcp_option82_action_set(sys_logic_port_t port, sys_dhcp_option82_act_t action);
extern int32 dhcp_option82_action_get(sys_logic_port_t port, sys_dhcp_option82_act_t *pAction);
extern int32 dhcp_option82_format_set(sys_logic_port_t port, sys_dhcp_option82_format_t format);
extern int32 dhcp_option82_format_get(sys_logic_port_t port, sys_dhcp_option82_format_t *pFormat);
extern int32 dhcp_option82_remoteID_set(sys_dhcp_option82_id_t *pRemoteID);
extern int32 dhcp_option82_remoteID_get(sys_dhcp_option82_id_t *pRemoteID);
extern int32 dhcp_option82_circuitID_set(sys_logic_port_t port, sys_dhcp_option82_circuit_id_t *pCircuitID);
extern int32 dhcp_snooping_enable_set(sys_enable_t enable);
extern int32 dhcp_snooping_enable_get(sys_enable_t *pEnable);
extern int32 dhcp_snooping_vlan_enable_set (sys_vid_t vid, sys_enable_t enable);
extern int32 dhcp_snooping_vlan_enable_get (sys_vid_t vid, sys_enable_t *pEnable);
extern int32 dhcp_snooping_port_trust_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 dhcp_snooping_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 dhcp_snooping_portChaddrChk_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 dhcp_snooping_portChaddrChk_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 dhcp_snooping_portMib_get(sys_logic_port_t port, sys_dhcp_mib_t *pCounter);
extern int32 dhcp_snooping_portMib_clear(sys_logic_port_t port);
extern int32 dhcp_snooping_rateLimit_set(sys_logic_port_t port, uint32 rate);
#endif

