
#ifndef __SKI_L3_H__
#define __SKI_L3_H__

#include <ski/ski_core.h>
#include <rsd/rsd_l3.h>

typedef enum ski_l3_cmd_e
{
    
    SKI_L3_SET_BASE = (SKI_CORE_MOD_L3 * SKI_CORE_MODCMD_TOTAL),
#ifdef CONFIG_SYS_L3_ROUTE
    SKI_L3_SET_ROUTEV4_ADD,
    SKI_L3_SET_ROUTEV4_DEL,
    SKI_L3_SET_ROUTEV6_ADD,
    SKI_L3_SET_ROUTEV6_DEL,
    SKI_L3_SET_FWD_IPV4ENBL,
    SKI_L3_SET_FWD_IPV6ENBL,
#endif
    SKI_L3_SET_INTF_IP_ADD,
    SKI_L3_SET_INTF_IP_DEL,
    SKI_L3_SET_INTF_ADMINENBL,
    SKI_L3_SET_INTF_ADMINIPV6ENBL,
    SKI_L3_SET_INTF_ADMINIPV6DESTUNREACH,
    SKI_L3_SET_INTF_GRATUITOUS_ARP_SEND,
#ifdef CONFIG_SYS_L3_ROUTE
    SKI_L3_SET_NEIGH_AGETIME,
    SKI_L3_SET_NEIGH_RESPONSE,
    SKI_L3_SET_NEIGH_RETRY,
    SKI_L3_SET_NEIGH_RENEW,
    SKI_L3_SET_NEIGH_CACHESIZE,
    SKI_L3_SET_NEIGH_CLEAR,
    SKI_L3_SET_NEIGH_DEL,
#ifdef CONFIG_SYS_L3_ROUTE_RIP
	SKI_L3_RIP_ENBL,
#endif
#ifdef CONFIG_SYS_L3_ROUTE_OSPF
	SKI_L3_OSPF_ENBL,
#endif
#endif
    
    SKI_L3_GET_BASE = (SKI_L3_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_L3_GET_NEIGH_CNT,

    SKI_L3_CMD_END
} ski_l3_cmd_t;

typedef union ski_l3_ioctl_u
{
    sys_l3_intfId_t   intfId;
    sys_l3_routeV4_t  v4Entry;
    sys_l3_routeV6_t  v6Entry;
    sys_enable_t      enable;

    uint32 val;

    struct {
        sys_l3_intfId_t id;
        sys_l3_af_t     af;
        sys_l3_ipType_t type;
        sys_l3_ip_t     ip;
    } intfIp;

    struct {
        sys_l3_intfId_t id;
        sys_enable_t    enable;
    } intfAdminEnbl;

    struct {
        sys_l3_neighType_t neighClearType;
        sys_l3_intfId_t intfId;
        sys_ip_t ip;
        sys_mac_t mac;
    } neigh;
} ski_l3_ioctl_t;

extern int32 ski_l3_init(void);

#endif  

