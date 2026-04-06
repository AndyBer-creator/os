
#ifndef __SKI_VLAN_IP_SUBNET_H__
#define __SKI_VLAN_IP_SUBNET_H__

#include <ski/ski_core.h>
#include <rsd/rsd_vlan_ip_subnet.h>

typedef enum ski_vlan_ip_subnet_cmd_e
{
    
    SKI_VLAN_IP_SUBNET_SET_BASE = (SKI_CORE_MOD_VLAN_IP_SUBNET * SKI_CORE_MODCMD_TOTAL),
    SKI_VLAN_IP_SUBNET_GROUP_SET,
    SKI_VLAN_IP_SUBNET_GROUP_DESTROY,
    SKI_VLAN_IP_SUBNET_GROUP_PORT_BIND,
	SKI_VLAN_IP_SUBNET_GROUP_PORT_UNBIND,
    SKI_VLAN_IP_SUBNET_GROUP_PORT_ENABLE,
	SKI_VLAN_IP_SUBNET_GROUP_PORT_DISABLE,

    SKI_VLAN_IP_SUBNET_GET_BASE = (SKI_VLAN_IP_SUBNET_SET_BASE + SKI_CORE_MODCMD_RANGE),

    SKI_VLAN_IP_SUBNET_CMD_END
} ski_vlan_ip_subnet_cmd_t;

typedef union ski_vlan_ip_subnet_ioctl_u
{
    struct
    {
        uint32   index;
        sys_ipSubnet_vlan_group_t   subNetGroup;
    } proGroup;

    struct ski_vlan_port_subnet_s
    {
        sys_logic_port_t port;
        uint32 subNetGroup_idx;
        sys_enable_t     enable;
    } portSubnet;
} ski_vlan_ip_subnet_ioctl_t;

extern int32 ski_vlan_ip_subnet_init(void);
#endif

