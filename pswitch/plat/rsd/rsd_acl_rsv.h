
#ifndef __RSD_ACL_RSV_H__
#define __RSD_ACL_RSV_H__

#include <rsd/rsd_acl.h>

typedef enum sys_acl_rsvRuleType_e
{
    ACL_RSV_ARP_MYMAC_DROP,             
    ACL_RSV_ARP_MYMAC,                  
    ACL_RSV_ARP_BCAST,                  
#ifdef CONFIG_SYS_PROTO_DAI
    ACL_RSV_ARP_DAI,                    
#endif
#ifdef CONFIG_SYS_PROTO_STP
    ACL_RSV_BPDU,                       
  #ifdef CONFIG_SYS_RTL8380
    ACL_RSV_BPDU_PORT1,                 
    ACL_RSV_BPDU_PORT2,
    ACL_RSV_BPDU_PORT3,
    ACL_RSV_BPDU_PORT4,
    ACL_RSV_BPDU_PORT5,
    ACL_RSV_BPDU_PORT6,
    ACL_RSV_BPDU_PORT7,
    ACL_RSV_BPDU_PORT8,
    ACL_RSV_BPDU_PORT9,
    ACL_RSV_BPDU_PORT10,
    ACL_RSV_BPDU_PORT11,
    ACL_RSV_BPDU_PORT12,
    ACL_RSV_BPDU_PORT13,
    ACL_RSV_BPDU_PORT14,
    ACL_RSV_BPDU_PORT15,
    ACL_RSV_BPDU_PORT16,
    ACL_RSV_BPDU_PORT17,
    ACL_RSV_BPDU_PORT18,
    ACL_RSV_BPDU_PORT19,
    ACL_RSV_BPDU_PORT20,
    ACL_RSV_BPDU_PORT21,
    ACL_RSV_BPDU_PORT22,
    ACL_RSV_BPDU_PORT23,
    ACL_RSV_BPDU_PORT24,
    ACL_RSV_BPDU_PORT25,
    ACL_RSV_BPDU_PORT26,
    ACL_RSV_BPDU_PORT27,
    ACL_RSV_BPDU_PORT28,
  #endif
#endif
#ifdef CONFIG_SYS_PLAT_CUSMOD
    ACL_RSV_CUSTOM1,                    
    ACL_RSV_CUSTOM2,                    
    ACL_RSV_CUSTOM3,                    
    ACL_RSV_CUSTOM4,                    
#endif
#ifdef CONFIG_SYS_PROTO_DHCP
    ACL_RSV_DHCP_SERVER_BCAST,          
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    ACL_RSV_DHCP_CLIENT_SNOOPING,       
    ACL_RSV_DHCP_SERVER_SNOOPING,       
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    ACL_RSV_EAPOL,
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
    ACL_RSV_GVRP,                       
#endif
#ifdef CONFIG_SYS_PROTO_GARP
    ACL_RSV_GMRP,                       
#endif
#ifdef CONFIG_SYS_APP_HTTPD
    ACL_RSV_HTTP,                       
  #ifdef CONFIG_USER_BOA_WITH_SSL
    ACL_RSV_HTTPS,                      
  #endif
#endif
    ACL_RSV_ICMP,                       
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    ACL_RSV_IGMP,                       
    ACL_RSV_IGMP_FLOOD,                 
    ACL_RSV_IGMP_DVMRP_04,              
    ACL_RSV_IGMP_MOSPF_05,              
    ACL_RSV_IGMP_MOSPF_06,              
    ACL_RSV_IGMP_PIMV2_13,              
    ACL_RSV_IGMP_ALL_IP_224,            
    ACL_RSV_UNKNOWN_IPV4_DROP,          
#endif
#ifdef CONFIG_SYS_PROTO_LACP
    ACL_RSV_LACP,                       
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
    ACL_RSV_LLDP,                       
#endif
    ACL_RSV_MYMAC,                      
#ifdef CONFIG_SYS_L3_ROUTE
    ACL_RSV_L3_DHCPV4,
#endif
#ifdef CONFIG_SYS_IPV6
  #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    ACL_RSV_MLD_FLOOD_DHCPV6,           
    ACL_RSV_UNKNOWN_IPV6_DROP,          
    ACL_RSV_MLD_143,                    
    ACL_RSV_MLD_130,                    
    ACL_RSV_MLD_131,                    
    ACL_RSV_MLD_132,                    
    ACL_RSV_MLD_DVMRP_04,               
    ACL_RSV_MLD_MOSPF_05,               
    ACL_RSV_MLD_MOSPF_06,               
    ACL_RSV_MLD_PIMV2_13,               
  #endif
    ACL_RSV_FF0X_FLOOD,                 
    ACL_RSV_NBS_MYMAC_CPU_PERMIT,       
    ACL_RSV_NBS_MYMAC_DROP,             
    ACL_RSV_NBS,                        
#endif

    ACL_RSV_RMA_0X,
    ACL_RSV_RMA_1X_1,
    ACL_RSV_RMA_1X_2,
    ACL_RSV_RMA_2X,
    ACL_RSV_RMA_3X,
    ACL_RSV_RMA_4_7X,
    ACL_RSV_RMA_8_FX,
    ACL_RSV_RMA_IPV6_102,
    ACL_RSV_RMA_IPV6_16,

#ifdef CONFIG_SYS_SNMP
    ACL_RSV_SNMP,                       
#endif
#ifdef CONFIG_USER_SSH_SSHD
    ACL_RSV_SSH,                        
#endif
#ifdef CONFIG_SYS_APP_TELNETD
    ACL_RSV_TELNET,                     
#endif
    ACL_RSV_TFTP,                       
#ifdef CONFIG_SYS_PROTO_UDLD
    ACL_RSV_UDLD,                       
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
		ACL_RSV_ERPS,						
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
		ACL_RSV_LOOPBACK,						
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	ACL_RSV_DHCP_SERVER,		
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	ACL_RSV_RIP,		               
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	ACL_RSV_OSPF,					   
#endif
#ifdef CONFIG_SYS_L3_VRRP
	ACL_RSV_VRRP,					   
#endif

    ACL_RSV_END
}sys_acl_rsvRuleType_t;

#if 1
typedef enum rsd_acl_rsv_fieldTyep_e
{
    ACL_RSV_FIELD_SPM,
    ACL_RSV_FIELD_CARE_CPU,
    ACL_RSV_FIELD_DMAC,
    ACL_RSV_FIELD_MGMT_VID,
    ACL_RSV_FIELD_DMAC_SWITCH_MAC,
    ACL_RSV_FIELD_SMAC_SWITCH_MAC,
    ACL_RSV_FIELD_IP4,
    ACL_RSV_FIELD_IP6,
    ACL_RSV_FIELD_ETHERTYPE,
    ACL_RSV_FIELD_DIP,
    ACL_RSV_FIELD_IPPROTO,
    ACL_RSV_FIELD_L4DPORT,
    ACL_RSV_FIELD_TCPFLAG,
    ACL_RSV_FIELD_ICMPTYPE,
    ACL_RSV_FIELD_STPDROP,
    ACL_RSV_FIELD_END,
} rsd_acl_rsv_fieldType_t;

#define ACL_RSV_FIELDMASK_SPM               SYS_SHIFT_BIT(ACL_RSV_FIELD_SPM)
#define ACL_RSV_FIELDMASK_CARE_CPU          SYS_SHIFT_BIT(ACL_RSV_FIELD_CARE_CPU)
#define ACL_RSV_FIELDMASK_DMAC              SYS_SHIFT_BIT(ACL_RSV_FIELD_DMAC)
#define ACL_RSV_FIELDMASK_MGMT_VID          SYS_SHIFT_BIT(ACL_RSV_FIELD_MGMT_VID)
#define ACL_RSV_FIELDMASK_DMAC_SWITCH_MAC   SYS_SHIFT_BIT(ACL_RSV_FIELD_DMAC_SWITCH_MAC)
#define ACL_RSV_FIELDMASK_SMAC_SWITCH_MAC   SYS_SHIFT_BIT(ACL_RSV_FIELD_SMAC_SWITCH_MAC)
#define ACL_RSV_FIELDMASK_IP4               SYS_SHIFT_BIT(ACL_RSV_FIELD_IP4)
#define ACL_RSV_FIELDMASK_IP6               SYS_SHIFT_BIT(ACL_RSV_FIELD_IP6)
#define ACL_RSV_FIELDMASK_ETHERTYPE         SYS_SHIFT_BIT(ACL_RSV_FIELD_ETHERTYPE)
#define ACL_RSV_FIELDMASK_DIP               SYS_SHIFT_BIT(ACL_RSV_FIELD_DIP)
#define ACL_RSV_FIELDMASK_IPPROTO           SYS_SHIFT_BIT(ACL_RSV_FIELD_IPPROTO)
#define ACL_RSV_FIELDMASK_L4DPORT           SYS_SHIFT_BIT(ACL_RSV_FIELD_L4DPORT)
#define ACL_RSV_FIELDMASK_TCPFLAG           SYS_SHIFT_BIT(ACL_RSV_FIELD_TCPFLAG)
#define ACL_RSV_FIELDMASK_ICMPTYPE          SYS_SHIFT_BIT(ACL_RSV_FIELD_ICMPTYPE)
#define ACL_RSV_FIELDMASK_STPDROP           SYS_SHIFT_BIT(ACL_RSV_FIELD_STPDROP)

typedef struct rsd_acl_rsvField_s
{
    uint32  fieldMask;
    uint32  dip;
    uint32  care_dip;
    uint16  ethertype;
    uint16  dstPort;
    uint8   ipProto;
    uint8   tcpFlag;
    uint8   tcpFlagMask;
    uint8   icmpType;
    uint8   dmac[CAPA_MAC_BYTE_NUM];
    uint8   care_dmac[CAPA_MAC_BYTE_NUM];
    uint8   smac[CAPA_MAC_BYTE_NUM];
    uint8   care_smac[CAPA_MAC_BYTE_NUM];
    uint8   stpDrop;
    uint8   stpDropMask;
    sys_logic_portmask_t spm;
} rsd_acl_rsv_field_t;
#endif

typedef struct rsd_acl_rsvRateChk_s
{
    uint32 pcnt;
    uint16 counterIdx;
    uint8  doRateChk;
    uint8  isBlocking;
} rsd_acl_rsvRateChk_t;

typedef struct rsd_acl_rsvEntry_s
{
    sys_enable_t            status;
    sys_enable_t            rma;
    sys_acl_rsvRuleType_t   type;
    rsd_acl_rsv_field_t     field;
    sys_acl_action_t        act_ebl;
    sys_acl_action_t        act_dbl;
    rsd_acl_rsvRateChk_t    rateChk;
    char  name[SYS_BUF32_LEN];
} rsd_acl_rsvEntry_t;

extern int32 rsd_acl_rsv_ruleEnable_set(sys_acl_rsvRuleType_t type, sys_enable_t enable);

extern int32 rsd_acl_rsv_rule_update(sys_acl_rsvRuleType_t ruleType,
    rsd_acl_rsv_field_t *pRuleField, sys_acl_action_t *pRuleEnAction,
    sys_acl_action_t *pRuleDisAction);

extern int32 rsd_acl_rsv_init(void);
#endif

