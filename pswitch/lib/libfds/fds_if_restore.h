
#ifndef __FDS_IF_RESTORE_H__
#define __FDS_IF_RESTORE_H__

#include <common/sys_def.h>

extern int32 fds_port_restore(sys_logic_port_t port);

extern int32 fds_port_port_restore(sys_logic_port_t port);

extern int32 fds_switch_port_restore(sys_logic_port_t port);

#ifdef CONFIG_SYS_PORT_SECURITY

extern int32 fds_psecure_port_restore(sys_logic_port_t port);
#endif

#ifdef CONFIG_SYS_EEE

extern int32 fds_eee_port_restore(sys_logic_port_t port);
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR

extern int32 fds_authmgr_port_restore(sys_logic_port_t port);
#endif

extern int32 fds_vlan_port_restore(sys_logic_port_t port);

#ifdef CONFIG_SYS_PROTOCOL_VLAN

extern int32 fds_vlan_protoVlan_port_restore(sys_logic_port_t port);
#endif
#ifdef CONFIG_SYS_VOICEVLAN

extern int32 fds_vlan_voiceVlan_port_restore(sys_logic_port_t port);
#endif

#ifdef CONFIG_SYS_SURVEILLANCEVLAN

extern int32 fds_surveillance_vlan_port_restore(sys_logic_port_t port);
#endif

extern int32 fds_mirror_port_restore(sys_logic_port_t port);

extern int32 fds_trunk_port_restore(sys_logic_port_t port);

#ifdef CONFIG_SYS_PROTO_LACP

extern int32 fds_lacp_port_restore(sys_logic_port_t port);
#endif

extern int32 fds_qos_port_restore(sys_logic_port_t port);

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING

extern int32 fds_igmp_port_restore(sys_logic_port_t port);
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING

extern int32 fds_mld_port_restore(sys_logic_port_t port);
#endif

extern int32 fds_rate_port_restore(sys_logic_port_t port);

#ifdef CONFIG_SYS_PROTO_STP

extern int32 fds_stp_port_restore(sys_logic_port_t port);
#endif

#ifdef CONFIG_SYS_PROTO_LLDP

extern int32 fds_lldp_port_restore(sys_logic_port_t port);
#endif
#ifdef CONFIG_SYS_PROTO_ISG

extern int32 fds_isg_port_restore(sys_logic_port_t port);
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

extern int32 fds_dhcp_snooping_port_restore(sys_logic_port_t port);
#endif
#ifdef CONFIG_SYS_PROTO_DAI

extern int32 fds_dai_port_restore(sys_logic_port_t port);
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION

extern int32 fds_dos_port_restore(sys_logic_port_t port);
#endif
#ifdef CONFIG_SYS_PROTO_GVRP

extern int32 fds_gvrp_port_restore(sys_logic_port_t port);
#endif
#ifdef CONFIG_SYS_PROTO_GARP

extern int32 fds_garp_port_restore(sys_logic_port_t port);
#endif

#ifdef CONFIG_SYS_USER_DEFINED_ACL

extern int32 fds_acl_port_restore(sys_logic_port_t port);
#endif

#ifdef CONFIG_SYS_PROTO_UDLD

extern int32 fds_udld_port_restore(sys_logic_port_t port);
#endif
#ifdef CONFIG_SYS_POE
extern int32 fds_poe_port_restore(sys_logic_port_t port);
#endif
#endif 

