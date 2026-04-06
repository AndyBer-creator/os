
#ifndef __SKI_SYS_H__
#define __SKI_SYS_H__

#include <ski/ski_core.h>
#include <rsd/rsd_sys.h>

#ifdef CONFIG_SYS_ADVANCE_DNS
#include <rsd/rsd_dns.h>
#endif

typedef enum ski_sys_cmd_e
{
    
    SKI_SYS_SET_BASE = (SKI_CORE_MOD_SYS * SKI_CORE_MODCMD_TOTAL),
#if !defined(CONFIG_SYS_L3_ROUTE)
    SKI_SYS_SET_IP_ADDR,
#endif
    SKI_SYS_SET_SYSINFO,
    SKI_SYS_SET_WATCHDOG,
    SKI_SYS_SET_STATUS,
#ifdef CONFIG_SYS_ADVANCE_DNS
    SKI_SYS_DNS_SET,
    SKI_SYS_DNS_CLEAR,
#endif

    SKI_SYS_GET_BASE = (SKI_SYS_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_SYS_GET_MAC_ADDR,
    SKI_SYS_GET_PORT_MAC_ADDR,
    SKI_SYS_GET_JIFFIES,
    SKI_SYS_GET_HZ,
    SKI_SYS_GET_CPU_FREQ,
    SKI_SYS_GET_RESET_STATUS,
    SKI_SYS_GET_STATUS,

    SKI_SYS_CMD_END
} ski_sys_cmd_t;

typedef struct ski_sys_macAddr_s
{
    uint32    dev;
    sys_mac_t mac;
} ski_sys_macAddr_t;

typedef struct ski_port_macAddr_s
{
    sys_logic_port_t    lport;
    sys_mac_t mac;
} ski_port_macAddr_t;

typedef struct ski_sys_ipAddr_s
{
    char            devName[16];
    sys_ipv4_addr_t ip;
    sys_ipv4_addr_t netmask;
} ski_sys_ipAddr_t;

typedef struct ski_sys_boardPortConf_s
{
    sys_logic_port_t port;
    sys_port_conf_t portConf;
} ski_sys_boardPortConf_t;

typedef union ski_sys_ioctl_u
{
    ski_sys_macAddr_t macAddr;
    ski_sys_ipAddr_t  ipAddr;
    uint32            jiffies;
    int32             hz;
    uint32            cpuFreq;
    uint32            resetStatus;
#ifdef CONFIG_SYS_IPV6
    sys_ipv6_addr_t   ipv6Addr;
#endif
    sys_sysinfo_t sysinfo;
    ski_port_macAddr_t  portMac;
    sys_enable_t      watchdogEnbl;
    sys_status_t      status;
#ifdef CONFIG_SYS_ADVANCE_DNS
    sys_enable_t      dnsEnable;
#endif
} ski_sys_ioctl_t;

extern int32 ski_sys_init(void);

#endif  

