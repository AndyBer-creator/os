
#ifndef __CMD_CLI_H__
#define __CMD_CLI_H__

#include <libclicmd/cmd_ext.h>
#include <libclicmd/cmd_common.h>
#include <libclicmd/cmd_sys.h>
#include <libclicmd/cmd_time.h>
#include <libclicmd/cmd_sys_fs.h>
#include <libclicmd/cmd_vlan.h>
#include <libclicmd/cmd_port.h>
#include <libclicmd/cmd_trunk.h>
#include <libclicmd/cmd_switch.h>
#include <libclicmd/cmd_eee.h>
#ifdef CONFIG_SYS_MIRROR
#include <libclicmd/cmd_mirror.h>
#endif
#include <libclicmd/cmd_l2.h>
#ifdef CONFIG_SYS_L3_ROUTE
#include <libclicmd/cmd_l3.h>
#endif
#include <libclicmd/cmd_stp.h>
#include <libclicmd/cmd_qos.h>
#ifdef CONFIG_SYS_SNMP
#include <libclicmd/cmd_snmp.h>
#endif
#include <libclicmd/cmd_rate.h>
#include <libclicmd/cmd_syslog.h>
#include <libclicmd/cmd_dos.h>
#include <libclicmd/cmd_util.h>
#ifdef CONFIG_SYS_PLAT_DBGMOD
#include <libclicmd/cmd_debug.h>
#endif
#ifdef CONFIG_SYS_PORT_SECURITY
#include <libclicmd/cmd_psecure.h>
#endif
#ifdef CONFIG_SYS_USER_DEFINED_ACL
#include <libclicmd/cmd_acl_usr.h>
#endif
#ifdef CONFIG_SYS_AAA
#include <libclicmd/cmd_aaa.h>
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
#include <libclicmd/cmd_authmgr.h>
#endif
#ifdef CONFIG_SYS_LIB_CUSTOM
#include <libcustom/cmd_custom.h>
#endif
#ifdef CONFIG_SYS_PROTO_LACP
#include <libclicmd/cmd_lacp.h>
#endif
#ifdef CONFIG_SYS_PROTO_LLDP
#include <libclicmd/cmd_lldp.h>
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
#include <libclicmd/cmd_lldp_med.h>
  #endif
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
#include <libclicmd/cmd_igmp.h>
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#include <libclicmd/cmd_mld.h>
#endif
#ifdef CONFIG_SYS_PROTO_ISG
#include <libclicmd/cmd_isg.h>
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
#include <libclicmd/cmd_dhcp_snooping.h>
#endif
#ifdef CONFIG_SYS_PROTO_DAI
#include <libclicmd/cmd_dai.h>
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
#include <libclicmd/cmd_radius.h>
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
#include <libclicmd/cmd_tacplus.h>
#endif
#ifdef CONFIG_SYS_PROTO_GVRP
#include <libclicmd/cmd_gvrp.h>
#endif
#ifdef CONFIG_SYS_PROTO_GARP
#include <libclicmd/cmd_garp.h>
#endif
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
#include <libclicmd/cmd_rmon.h>
#endif
#ifdef CONFIG_SYS_MGMT_ACL
#include <libclicmd/cmd_acl_mgmt.h>
#endif
#ifdef CONFIG_SYS_CABLE_DIAG
#include <libclicmd/cmd_cablediag.h>
#endif
#ifdef CONFIG_SYS_APP_SSH
#include <libclicmd/cmd_ssh.h>
#endif
#ifdef CONFIG_SYS_VOICEVLAN
#include <libclicmd/cmd_vlan_voice.h>
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
#include <libclicmd/cmd_vlan_proto.h>
#endif
#ifdef CONFIG_SYS_PROTO_UDLD
#include <libclicmd/cmd_udld.h>
#endif
#ifdef CONFIG_SYS_MACVLAN
#include <libclicmd/cmd_vlan_mac.h>
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
#include <libclicmd/cmd_vlan_surveillance.h>
#endif
#ifdef CONFIG_SYS_TECH_SUPPORT
#include <libclicmd/cmd_sys_tech.h>
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
#include <libclicmd/cmd_erps.h>
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
#include <libclicmd/cmd_loopback.h>
#endif
#ifdef CONFIG_SYS_POE
#include <libclicmd/cmd_poe.h>
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
#include <libclicmd/cmd_dhcp_server.h>
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
#include <libclicmd/cmd_l3_rip.h>
#endif
#ifdef CONFIG_SYS_L3_ROUTE_ZEBRA
#include <libclicmd/cmd_l3_zebra.h>
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
#include <libclicmd/cmd_l3_ospf.h>
#endif
#ifdef CONFIG_SYS_L3_ROUTE_BGP
#include <libclicmd/cmd_l3_bgp.h>
#endif
#ifdef CONFIG_SYS_APP_EASYCWMP
#include <libclicmd/cmd_easycwmp.h>
#endif
#ifdef	CONFIG_SYS_L3_VRRP
#include <libclicmd/cmd_l3_vrrp.h>
#endif
#ifdef CONFIG_SYS_IP_SUBNET
#include <libclicmd/cmd_vlan_ip_subnet.h>
#endif

#endif 

