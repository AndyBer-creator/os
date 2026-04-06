
#ifndef __PROTO_DAI_H__
#define __PROTO_DAI_H__
#include <common/sys_def.h>
#define ARP_RATE_LAST_OVER_SEC       2
#define ARP_RATE_TICK_PER_SEC        10
#define ARP_OPERATION_REQUEST         1
#define ARP_OPERATION_REPLY           2
typedef struct sys_dai_arpMessage_s
{
    uint16 htype;               
    uint16 ptype;               
    uint8 hlen;                 
    uint8 plen;                 
    uint16 operation;           
    sys_mac_t sHaddr;           
    sys_ipv4_t sInaddr;         
    sys_mac_t tHaddr;           
    sys_ipv4_t tInaddr;         
} __attribute__ ((aligned(1), packed)) sys_dai_arpMessage_t;

typedef struct sys_arp_mib_s
{
    uint32 arpForward;      
    uint32 arpSmacDrop;     
    uint32 arpDmacDrop;     
    uint32 arpSIPDrop;      
    uint32 arpDIPDrop;      
    uint32 arpIPMACMismatchDrop;  
}sys_arp_mib_t;
extern int32 dai_enable_set(sys_enable_t enable);
extern int32 dai_enable_get(sys_enable_t *pEnable);
extern int32 dai_port_trust_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 dai_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 dai_vlan_enable_set (sys_vid_t vid, sys_enable_t enable);
extern int32 dai_vlan_enable_get (sys_vid_t vid, sys_enable_t *pEnable);
extern int32 dai_port_smacChk_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 dai_port_smacChk_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 dai_port_dmacChk_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 dai_port_dmacChk_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 dai_port_ipChk_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 dai_port_ipChk_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 dai_port_ipZeroAllow_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 dai_port_ipZeroAllow_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 dai_portMib_get(sys_logic_port_t port, sys_arp_mib_t *pCounter);
extern int32 dai_portMib_clear(sys_logic_port_t port);
extern int32 dai_vlanMib_get(sys_vid_t vid, sys_arp_mib_t *pCounter);
extern int32 dai_vlanMib_clear(sys_vid_t vid);
extern int32 dai_arp_rateLimit_set(sys_logic_port_t port, uint32 rate);
extern int32 dai_init(void);
extern int32 dai_exit(void);
#endif

