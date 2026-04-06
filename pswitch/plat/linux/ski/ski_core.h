
#ifndef __SKI_CORE_H__
#define __SKI_CORE_H__

#include <common/sys_def.h>
#include <ski/ski_lkm.h>

#define SKI_CORE_DEV_NAME     "/dev/ski"
#define SKI_CORE_PARAM_FILE_NAME  "/mnt/ski_param"

#define SKI_CORE_IOCTLARG_MAX (4)
#define SKI_CORE_PORTARG_MAX  (SKI_CORE_IOCTLARG_MAX - 1)

#define SKI_CORE_MODCMD_RANGE (64)                          
#define SKI_CORE_MODCMD_TOTAL (SKI_CORE_MODCMD_RANGE * 2)   

typedef enum ski_core_mod_e
{
    SKI_CORE_MOD_SYS = 0,
    SKI_CORE_MOD_BOARD,

#ifdef CONFIG_SYS_USER_DEFINED_ACL
    SKI_CORE_MOD_ACL,
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SKI_CORE_MOD_AUTHMGR,
#endif

#ifdef CONFIG_SYS_CABLE_DIAG
    SKI_CORE_MOD_CABLE_DIAG,
#endif

#ifdef CONFIG_SYS_LIB_CUSTOM
    SKI_CORE_MOD_CUSTOM,
#endif

#ifdef CONFIG_SYS_PROTO_DAI
    SKI_CORE_MOD_DAI,
#endif

    SKI_CORE_MOD_DBG,

#ifdef CONFIG_SYS_PROTO_DHCP
    SKI_CORE_MOD_DHCP,
#endif

#ifdef CONFIG_SYS_DOS_PROTECTION
    SKI_CORE_MOD_DOS,
#endif

#ifdef CONFIG_SYS_EEE
    SKI_CORE_MOD_EEE,
#endif

    SKI_CORE_MOD_FIBER,
    SKI_CORE_MOD_GPIO,

#ifdef CONFIG_SYS_PROTO_GVRP
    SKI_CORE_MOD_GVRP,
#endif

#ifdef CONFIG_SYS_PROTO_GARP
    SKI_CORE_MOD_GARP,
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    SKI_CORE_MOD_IGMP,
#endif

    SKI_CORE_MOD_IPMCAST,

#ifdef CONFIG_SYS_PROTO_ISG
    SKI_CORE_MOD_ISG,
#endif

    SKI_CORE_MOD_L2,
    SKI_CORE_MOD_L3,

#ifdef CONFIG_SYS_PROTO_LACP
    SKI_CORE_MOD_LACP,
#endif

    SKI_CORE_MOD_LED,

#ifdef CONFIG_SYS_PROTO_LLDP
    SKI_CORE_MOD_LLDP,
#endif

#if defined(CONFIG_SYS_PROTO_LLDP) && defined(CONFIG_SYS_PROTO_LLDP_MED)
    SKI_CORE_MOD_LLDP_MED,
#endif

#ifdef CONFIG_SYS_SYSLOG
    SKI_CORE_MOD_LOG,
#endif

#ifdef CONFIG_SYS_MGMT_ACL
    SKI_CORE_MOD_MGMTACL,
#endif

    SKI_CORE_MOD_MIB,

#ifdef CONFIG_SYS_MIRROR
    SKI_CORE_MOD_MIRROR,
#endif

#if defined(CONFIG_SYS_PROTO_IGMP_SNOOPING) && defined(CONFIG_SYS_PROTO_MLD_SNOOPING)
    SKI_CORE_MOD_MLD,
#endif

    SKI_CORE_MOD_OPTICAL,
    SKI_CORE_MOD_PORT,
#ifdef CONFIG_SYS_PORT_SECURITY
    SKI_CORE_MOD_PSECURE,
#endif
    SKI_CORE_MOD_QOS,
    SKI_CORE_MOD_RATE,

#ifdef CONFIG_SYS_PROTO_STP
    SKI_CORE_MOD_STP,
#endif
    SKI_CORE_MOD_SWITCH,
    SKI_CORE_MOD_TRAP,
    SKI_CORE_MOD_TRUNK,
#ifdef CONFIG_SYS_PROTO_UDLD
    SKI_CORE_MOD_UDLD,
#endif
    SKI_CORE_MOD_VLAN,

#ifdef CONFIG_SYS_VOICEVLAN
    SKI_CORE_MOD_VOICEVLAN,
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    SKI_CORE_MOD_VLAN_PROTO,
#endif
#ifdef CONFIG_SYS_IP_SUBNET
	SKI_CORE_MOD_VLAN_IP_SUBNET,
#endif
#ifdef CONFIG_SYS_MACVLAN
    SKI_CORE_MOD_MACVLAN,
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    SKI_CORE_MOD_SURVEILLANCEVLAN,
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
    SKI_CORE_MOD_ERPS,
#endif
#ifdef CONFIG_SYS_POE
	SKI_CORE_MOD_POE,
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	SKI_CORE_MOD_DHCP_SERVER,
#endif
#ifdef CONFIG_SYS_UART1
	SKI_CORE_MOD_SERIAL,
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
		SKI_CORE_MOD_LOOPBACK,
#endif
#ifdef CONFIG_SYS_BOARD_RTC
	SKI_CORE_MOD_RTC,
#endif
	SKI_CORE_MOD_BOARD_DEV,

    SKI_CORE_MOD_END
} ski_core_mod_t;

typedef struct ski_mapper_s
{
    int32 no;
    int32 (*fCallback)(unsigned int cmd, unsigned long arg);
} ski_mapper_t;

typedef struct ski_portconf_s
{
    sys_logic_port_t port;
    uint32 cfg[SKI_CORE_PORTARG_MAX];
} ski_portconf_t;

extern int32 ski_handler_register(int modId, int (*fCallback)(unsigned int cmd, unsigned long arg));

#endif  

