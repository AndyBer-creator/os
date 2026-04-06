#ifndef __COMMON_SYS_DBG_DESC_H__
#define __COMMON_SYS_DBG_DESC_H__
#include <common/sys_type.h>
typedef enum dbg_cat_e
{
    DBG_AAA,
    DBG_ACLMGR,
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    DBG_AUTHMGR,
#endif
    DBG_CFGMGR,
#ifdef CONFIG_SYS_PROTO_DAI
    DBG_DAI,
#endif
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
    DBG_DHCP_SNOOPING,
#endif
    DBG_IMAGEMGR,
#ifdef CONFIG_SYS_PROTO_ISG
    DBG_IPSG,
#endif
    DBG_L2,
    DBG_L3,
#ifdef CONFIG_SYS_PROTO_LLDP
    DBG_LLDP,
#endif
    DBG_LOGGING,
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    DBG_MCAST,
#endif
#ifdef CONFIG_SYS_MIRROR
    DBG_MIRROR,
#endif
    DBG_PLATFORM,
    DBG_PORT,
#ifdef CONFIG_SYS_PORT_SECURITY
    DBG_PSECURE,
#endif
    DBG_QOSMGR,
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    DBG_RMON,
#endif
#ifdef CONFIG_SYS_UI_SNMP
    DBG_SNMP,
#endif
    DBG_TIME,
#ifdef CONFIG_SYS_PROTO_STP
    DBG_STP,
#endif
    DBG_STORM_CONTROL,
    DBG_SYSTEM,
    DBG_LAG,
#ifdef CONFIG_SYS_PROTO_UDLD
    DBG_UDLD,
#endif
    DBG_VLAN,
#ifdef CONFIG_SYS_PROTO_ERPS
	DBG_ERPS,
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
	DBG_LOOPBACK,
#endif
#ifdef CONFIG_SYS_DHCP_SERVER
	DBG_DHCP_SERVER,
#endif
#ifdef CONFIG_SYS_L3_VRRP
	DBG_L3_VRRP,
#endif
    DBG_CAT_ALL,
} dbg_cat_t;
#define DBG_EXT_MAX         32
#define DBG_EXT_TOTAL       (DBG_EXT_MAX + 1)
#define DBG_EXT_BASE(cat)   (cat * DBG_EXT_TOTAL + DBG_CAT_ALL)
#define DBG_EXT_ALL(cat)    (DBG_EXT_BASE(cat) + DBG_EXT_MAX)

typedef enum dbg_aclmgr_e
{
    DBG_ACLMGR_API = DBG_EXT_BASE(DBG_ACLMGR),
#ifdef CONFIG_SYS_MGMT_ACL
    DBG_ACLMGR_MGMT,
#endif
    DBG_ACLMGR_RSV,
#ifdef CONFIG_SYS_USER_DEFINED_ACL
    DBG_ACLMGR_USER_DEF,
#endif
    DBG_ACLMGR_ALL,
} dbg_aclmgr_t;

#ifdef CONFIG_SYS_PROTO_AUTHMGR
typedef enum dbg_authmgr_e
{
    DBG_AUTHMGR_DOT1X = DBG_EXT_BASE(DBG_AUTHMGR),
    DBG_AUTHMGR_MACAUTH,
    DBG_AUTHMGR_WEBAUTH,
    DBG_AUTHMGR_ALL,
} dbg_authmgr_t;
#endif

typedef enum dbg_l3_e
{
    DBG_L3_INTF = DBG_EXT_BASE(DBG_L3),
    DBG_L3_ROUTE,
    DBG_L3_FIB,
    DBG_L3_FPM,
    DBG_L3_ALL,
} dbg_l3_t;

#ifdef CONFIG_SYS_PROTO_LLDP
typedef enum dbg_lldp_e
{
    DBG_LLDP_COMMON = DBG_EXT_BASE(DBG_LLDP),
    DBG_LLDP_PKT,
    DBG_LLDP_POE,
    DBG_LLDP_RX,
    DBG_LLDP_TX,
    DBG_LLDP_ALL,
} dbg_lldp_t;
#endif
typedef enum dbg_vlan_e
{
    DBG_VLAN_COMMON = DBG_EXT_BASE(DBG_VLAN),
#ifdef CONFIG_SYS_PROTO_GVRP
    DBG_VLAN_GVRP,
#endif
#ifdef CONFIG_SYS_MACVLAN
    DBG_VLAN_MAC,
#endif
#ifdef CONFIG_SYS_PROTOCOL_VLAN
    DBG_VLAN_PROTOCOL,
#endif
#ifdef CONFIG_SYS_SURVEILLANCEVLAN
    DBG_VLAN_SURVEILLANCE,
#endif
#ifdef CONFIG_SYS_VOICEVLAN
    DBG_VLAN_VOICE,
#endif
    DBG_VLAN_ALL,
} dbg_vlan_t;

#ifdef CONFIG_SYS_DHCP_SERVER
typedef enum dbg_dhcp_server_e
{
    DBG_DHCP_SERVER_COMMON = DBG_EXT_BASE(DBG_DHCP_SERVER),
    DBG_DHCP_SERVER_PKT,
    DBG_DHCP_SERVER_CMD,
    DBG_DHCP_SERVER_RX,
    DBG_DHCP_SERVER_TX,
    DBG_DHCP_SERVER_ALL,
} dbg_dhcp_server_t;
#endif

#ifdef CONFIG_SYS_L3_VRRP
typedef enum dbg_l3_vrrp_e
{
    DBG_L3_VRRP_COMMON = DBG_EXT_BASE(DBG_L3_VRRP),
    DBG_L3_VRRP_PKT,
    DBG_L3_VRRP_CMD,
    DBG_L3_VRRP_RX,
    DBG_L3_VRRP_TX,
    DBG_L3_VRRP_ALL,
} dbg_l3_vrrp_t;
#endif
typedef struct sys_dbg_extmask_s
{
    uint32 bits[SYS_BITS_TO_LONGS(DBG_EXT_MAX)];
} sys_dbg_extmask_t;

typedef struct sys_dbg_desc_s
{
    const uint32 extnum;
    const int32  *children;
} sys_dbg_desc_t;
#endif

