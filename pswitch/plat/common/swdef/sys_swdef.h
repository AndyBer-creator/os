
#ifndef __SYS_SWDEF_H__
#define __SYS_SWDEF_H__
#include <common/sys_autoconf.h>
#ifdef CONFIG_SYS_AAA
#include <common/swdef/sys_swdef_aaa.h>
#endif
#include <common/swdef/sys_swdef_acl.h>
#ifdef CONFIG_SYS_PROTO_AUTHMGR
#include <common/swdef/sys_swdef_authmgr.h>
#endif
#include <common/swdef/sys_swdef_common.h>
#include <common/swdef/sys_swdef_cli.h>
#ifdef CONFIG_SYS_PROTO_DAI
#include <common/swdef/sys_swdef_dai.h>
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
#include <common/swdef/sys_swdef_dhcp.h>
#endif
#ifdef CONFIG_SYS_DOS_PROTECTION
#include <common/swdef/sys_swdef_dos.h>
#endif
#ifdef CONFIG_SYS_PROTO_ISG
#include <common/swdef/sys_swdef_isg.h>
#endif
#include <common/swdef/sys_swdef_l2.h>
#include <common/swdef/sys_swdef_l3.h>
#ifdef CONFIG_SYS_PROTO_LLDP
#include <common/swdef/sys_swdef_lldp.h>
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
#include <common/swdef/sys_swdef_mcast.h>
#endif
#ifdef CONFIG_SYS_MIRROR
#include <common/swdef/sys_swdef_mirror.h>
#endif
#include <common/swdef/sys_swdef_port.h>
#include <common/swdef/sys_swdef_qos.h>
#include <common/swdef/sys_swdef_rate.h>
#ifdef CONFIG_SYS_MIB_RMON
#include <common/swdef/sys_swdef_mibrmon.h>
#endif
#include <common/swdef/sys_swdef_security.h>
#ifdef CONFIG_SYS_UI_SNMP
#include <common/swdef/sys_swdef_snmp.h>
#endif
#ifdef CONFIG_SYS_PROTO_STP
#include <common/swdef/sys_swdef_stp.h>
#endif
#include <common/swdef/sys_swdef_sys.h>
#ifdef CONFIG_SYS_SYSLOG
#include <common/swdef/sys_swdef_syslog.h>
#endif
#include <common/swdef/sys_swdef_time.h>
#include <common/swdef/sys_swdef_lag.h>
#ifdef CONFIG_SYS_PROTO_UDLD
#include <common/swdef/sys_swdef_udld.h>
#endif
#include <common/swdef/sys_swdef_vlan.h>
#ifdef CONFIG_SYS_PROTO_ERPS
#include <common/swdef/sys_swdef_erps.h>
#endif
#ifdef CONFIG_SYS_POE
#include <common/swdef/sys_swdef_poe.h>
#endif
#endif 

