
#ifndef __RSD_SYS_H__
#define __RSD_SYS_H__

#include <rtk/switch.h>
#include <rtk/default.h>
#include <board/board.h>
#include <common/sys_def.h>

#include <rsd/rsd_acl_rsv.h>
#include <rsd/rsd_trap.h>

#define SYS_CPU_PORT_PRIORITY_BPDU         7
#define SYS_CPU_PORT_PRIORITY_LACP         7
#ifdef CONFIG_SYS_PROTO_UDLD
#define SYS_CPU_PORT_PRIORITY_UDLD         7
#endif
#ifdef CONFIG_SYS_PROTO_ERPS
#define SYS_CPU_PORT_PRIORITY_ERPS         7
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
#define SYS_CPU_PORT_PRIORITY_LOOPBACK     7
#endif

#define SYS_CPU_PORT_PRIORITY_LLDP         6
#ifdef CONFIG_SYS_PROTO_GVRP
#define SYS_CPU_PORT_PRIORITY_GVRP         6
#endif
#ifdef CONFIG_SYS_PROTO_GARP
#define SYS_CPU_PORT_PRIORITY_GARP         6
#endif

#ifdef CONFIG_SYS_PROTO_AUTHMGR
#define SYS_CPU_PORT_PRIORITY_DOT1X        6
#endif
#define SYS_CPU_PORT_PRIORITY_IGMP         5
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#define SYS_CPU_PORT_PRIORITY_MLD          5
#endif
#ifdef CONFIG_SYS_L3_ROUTE_RIP
#define SYS_CPU_PORT_PRIORITY_RIP          4
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
#define SYS_CPU_PORT_PRIORITY_OSPF         4
#endif
#ifdef CONFIG_SYS_L3_VRRP
#define SYS_CPU_PORT_PRIORITY_VRRP         3
#endif

#define SYS_CPU_PORT_PRIORITY_NBS          4
#define SYS_CPU_PORT_PRIORITY_ICMP         4
#define SYS_CPU_PORT_PRIORITY_SNMP         3
#define SYS_CPU_PORT_PRIORITY_HTTP         3
#define SYS_CPU_PORT_PRIORITY_HTTPS        3
#define SYS_CPU_PORT_PRIORITY_TFTP         3
#define SYS_CPU_PORT_PRIORITY_TELNET       3
#define SYS_CPU_PORT_PRIORITY_MYMAC        3
#ifdef CONFIG_USER_SSH_SSHD
#define SYS_CPU_PORT_PRIORITY_SSH          3
#endif
#define SYS_CPU_PORT_PRIORITY_DHCP         2
#define SYS_CPU_PORT_PRIORITY_DHCP_SERVER  1
#define SYS_CPU_PORT_PRIORITY_ARP          1
#ifdef CONFIG_SYS_PORT_SECURITY
#define SYS_CPU_PORT_PRIORITY_PORT_SEC     0
#endif
#ifdef CONFIG_SYS_PROTO_AUTHMGR
#define SYS_CPU_PORT_PRIORITY_AUTHMGR      0
#endif
#define SYS_CPU_PORT_PRIORITY_DEFAULT      0

#define SYS_CPU_PORT_ACTION_BPDU           SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_LACP           SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_IGMP           SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_DVMRP          SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_OSPF           SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_MOSPF          SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_PIMV2          SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_LLDP           SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_ARP            SYS_PKT_ACTION_COPY2CPU
#define SYS_CPU_PORT_ACTION_NBS            SYS_PKT_ACTION_COPY2CPU
#define SYS_CPU_PORT_ACTION_DHCP           SYS_PKT_ACTION_COPY2CPU
#define SYS_CPU_PORT_ACTION_DS_CLIENT      SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_DS_SERVER      SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_DAI            SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_ICMP           SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_SNMP           SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_HTTP           SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_HTTPS          SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_TFTP           SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_TELNET         SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_ASDPM          SYS_PKT_ACTION_REDIRECT
#define SYS_CPU_PORT_ACTION_MYMAC          SYS_PKT_ACTION_TRAP2CPU
#ifdef CONFIG_SYS_PROTO_GVRP
#define SYS_CPU_PORT_ACTION_GVRP           SYS_PKT_ACTION_TRAP2CPU
#endif
#ifdef CONFIG_SYS_PROTO_GARP
#define SYS_CPU_PORT_ACTION_GARP           SYS_PKT_ACTION_TRAP2CPU
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#define SYS_CPU_PORT_ACTION_MLD            SYS_PKT_ACTION_TRAP2CPU
#endif
#ifdef CONFIG_USER_SSH_SSHD
#define SYS_CPU_PORT_ACTION_SSH            SYS_PKT_ACTION_TRAP2CPU
#endif
#define SYS_CPU_PORT_ACTION_DEFAULT        SYS_PKT_ACTION_TRAP2CPU
#define SYS_CPU_PORT_ACTION_NONE           SYS_PKT_ACTION_FORWARD

typedef rtk_switch_devInfo_t sys_switch_devInfo_t;
typedef ipaddr_t             sys_ipv4_addr_t;
typedef rtk_ipv6_addr_t      sys_ipv6_addr_t;
typedef rtk_snapMode_t       sys_snapMode_t;

typedef enum sys_reset_act_e
{
    SYS_RESET_ACT_NONE = 0,
    SYS_RESET_ACT_REBOOT,
    SYS_RESET_ACT_RESTORE_ACCOUNT,
    SYS_RESET_ACT_RESTORE_FACTORY,
    SYS_RESET_ACT_END,
} sys_reset_act_t;

typedef enum sys_status_e
{
    SYS_STATUS_INITIAL = 0,
    SYS_STATUS_READY,
    SYS_STATUS_END,
} sys_status_t;

typedef struct sys_sysinfo_s
{
    char name[CAPA_SYS_NAME_LEN + 1];
    char contact[CAPA_SYS_CONTACT_LEN + 1];
    char location[CAPA_SYS_LOCATION_LEN + 1];
    char description[CAPA_SYS_DESC_LEN + 1];
    char serialnum[CAPA_SYS_SN_LEN + 1];
    char versionNum[CAPA_SYS_VER_LEN + 1];
    char loginLogoName[CAPA_SYS_NAME_LEN + 1];
    char homeLogoName[CAPA_SYS_NAME_LEN + 1];
    char title[CAPA_SYS_NAME_LEN + 1];
    char manufacturerName[CAPA_SYS_LOCATION_LEN + 1];
	char supportInformation[CAPA_SYS_LOCATION_LEN + 1];
	char telephoneNumber[CAPA_SYS_NAME_LEN + 1];
} sys_sysinfo_t;

typedef struct sys_mgmtProtoAction_s
{
    sys_trap_mgmtType_t mgmtProto;
    sys_action_t        action;
    sys_pri_t           pri;
} sys_mgmtProtoAction_t;

typedef struct sys_system_event_s
{
    union
    {
        uint32 ip;
        char name[CAPA_SYS_NAME_LEN + 1];
        char description[CAPA_SYS_DESC_LEN + 1];

        sys_status_t status;

        struct
        {
            uint8 isAllPort;
            sys_logic_port_t port;
            uint32 size;
        }portMaxFrame;

    };
} sys_system_event_t;

#define IS_RESET_ACT_VALID(action)    \
    ((((action) > SYS_RESET_ACT_NONE) && ((action) < SYS_RESET_ACT_END)) ? 1 : 0)

extern int32 rsd_sys_init(void);

extern int32 rsd_sys_mgmtMac_set(void);

extern int32 rsd_sys_devInfo_get(uint32 unit, sys_switch_devInfo_t *pInfo);

extern int32 rsd_sys_macAddr_get(uint32 unit, sys_mac_t *pMac);

extern int32 rsd_sys_portMacAddr_get(sys_logic_port_t port, sys_mac_t *pMac);

extern int32 rsd_sys_mgmtIPv4Addr_get(sys_l3_ipv4_t *pIp);

#ifdef CONFIG_SYS_IPV6

extern int32 rsd_sys_intfIpv6Addr_get(sys_l3_intfIPv6AddrsInfo_t *pAddrInfo);
#endif

#if !defined(CONFIG_SYS_L3_ROUTE)

extern int32 rsd_sys_ipAddr_set(uint32 ip);
#endif

extern int32 rsd_sys_jiffies_get(uint32 *pJiffies);

extern int32 rsd_sys_hz_get(int32 *pHz);

extern int32 rsd_sys_cpuFreq_get(uint32 *pFreq);

extern int32 rsd_sys_firmwareDate_get(char *pDate);

extern int32 rsd_sys_resetStatus_get(uint32 *pResetStatus);

extern int32 rsd_sys_resetStatus_set(sys_reset_act_t action);

extern int32 rsd_sys_cpuQueueUltz_set(void);

extern int32 rsd_sys_info_set(sys_sysinfo_t *pSysinfo);

extern int32 rsd_sys_info_get(sys_sysinfo_t *pSysinfo);

extern int32 rsd_sys_watchdog_set(sys_enable_t enable);

extern int32 rsd_sys_status_set(sys_status_t status);

extern int32 rsd_sys_status_get(sys_status_t *pStatus);

#endif 

