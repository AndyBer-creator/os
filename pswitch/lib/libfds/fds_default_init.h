
#ifndef __FDS_DEFAULT_INIT_H__
#define __FDS_DEFAULT_INIT_H__

#include <common/sys_def.h>

extern int32 fds_sys_localuser_dfltEntry_init(void);

extern int32 fds_sys_default_init(void);

extern int32 fds_port_default_init(void);

#ifdef CONFIG_SYS_PORT_SECURITY

extern int32 fds_psecure_default_init(void);
#endif

extern int32 fds_switch_default_init(void);

#ifdef CONFIG_SYS_EEE

extern int32 fds_eee_default_init(void);
#endif 

extern int32 fds_vlan_default_init(void);

extern int32 fds_vlan_dfltVlan_init(void);

extern int32 fds_vlan_dfltVlan_dfltEntry_init(void);

#ifdef CONFIG_SYS_PROTOCOL_VLAN

extern int32 fds_vlan_protocolVlan_init(void);
#endif 
#ifdef CONFIG_SYS_IP_SUBNET
extern int32 fds_vlan_ip_subnet_vlan_init(void);
#endif
#ifdef CONFIG_SYS_VOICEVLAN

extern int32 fds_vlan_voiceVlanOui_dfltEntry_init(void);

extern int32 fds_vlan_voiceVlan_init(void);
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN

extern int32 fds_surveillance_vlan_dfltEntry_init(void);

extern int32 fds_surveillance_vlan_init(void);
#endif

extern int32 fds_mirror_default_init(void);

extern int32 fds_l2_default_init(void);

#ifdef CONFIG_SYS_L3_ROUTE

extern int32 fds_l3_default_init(void);
#endif

extern int32 fds_trunk_default_init(void);

#ifdef CONFIG_SYS_PROTO_LACP

extern int32 fds_lacp_default_init(void);
#endif 

extern int32 fds_acl_template_dfltEntry_init(void);

extern int32 fds_qos_default_init(void);

#ifdef CONFIG_SYS_UI_SNMP

extern int32 fds_snmp_default_init(void);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING

extern int32 fds_igmp_default_init(void);

#ifdef CONFIG_SYS_PROTO_MVR

extern int32 fds_mvr_default_init(void);
#endif

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING

extern int32 fds_mld_default_init(void);
#endif

#endif 

extern int32 fds_rate_default_init(void);

#ifdef CONFIG_SYS_PROTO_STP

extern int32 fds_stp_default_init(void);
#endif 

#ifdef CONFIG_SYS_PROTO_LLDP

extern int32 fds_lldp_default_init(void);
#endif 

#ifdef CONFIG_SYS_SYSLOG

extern int32 fds_log_default_init(void);
#endif

#ifdef CONFIG_SYS_AAA

extern int32 fds_aaa_default_init(void);
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

extern int32 fds_authmgr_default_init(void);
#endif

#ifdef CONFIG_SYS_LIB_RADIUS

extern int32 fds_radius_default_init(void);
#endif

#ifdef CONFIG_SYS_PROTO_ISG

extern int32 fds_isg_default_init(void);
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

extern int32 fds_dhcp_snooping_default_init(void);
#endif 

#ifdef CONFIG_SYS_PROTO_DAI

extern int32 fds_dai_default_init(void);
#endif

#ifdef CONFIG_SYS_LIB_CUSTOM

extern int32 fds_custom_default_init(void);
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION

extern int32 fds_dos_default_init(void);
#endif 

#ifdef CONFIG_SYS_PROTO_GVRP

extern int32 fds_gvrp_default_init(void);
#endif 

#ifdef CONFIG_SYS_PROTO_GARP

extern int32 fds_garp_default_init(void);
#endif 

#ifdef CONFIG_SYS_MGMT_ACL

extern int32 fds_acl_mgmt_dfltEntry_init(void);
#endif

#ifdef CONFIG_SYS_PROTO_UDLD

extern int32 fds_udld_default_init(void);
#endif

#ifdef CONFIG_SYS_PROTO_ERPS

extern int32 fds_erps_default_init(void);
#endif

#ifdef CONFIG_SYS_PROTO_LOOPBACK

extern int32 fds_loopback_default_init(void);
#endif

extern int32 fds_sys_post_init(void);

extern int32 fds_port_post_init(void);

#ifdef CONFIG_SYS_POE
extern int32 fds_poe_default_init(void);
extern uint32 fds_poe_Support_check(void);
#endif
#ifdef CONFIG_SYS_DHCP_SERVER

extern int32 fds_dhcp_server_default_init(void);
#endif 
#ifdef CONFIG_SYS_APP_EASYCWMP

extern int32 fds_easyCwmp_default_init(void);
#endif 

extern int32 fds_oem_config_init(void);
#endif 

