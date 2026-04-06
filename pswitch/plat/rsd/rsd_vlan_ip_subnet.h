
#ifndef __RSD_VLAN_IP_SUBNET_H__
#define __RSD_VLAN_IP_SUBNET_H__

#include <rtk/vlan.h>

typedef rtk_vlan_ipSubnetVlanEntry_t sys_vlan_ipsubnet_VlanCfg_t;

typedef struct sys_ipSubnet_vlan_group_s
{
	sys_l3_ipv4_t        ipaddr;
	sys_vid_t            vid;
	sys_pri_t            priority;
    uint32               groupId;
    sys_logic_port_t     lport;
    uint32               valid;
}sys_ipSubnet_vlan_group_t;

#define PV_DBG(fmt, args...) SYS_DBG(DBG_VLAN_PROTOCOL, fmt, ##args)

extern int32 rsd_vlan_ip_subnet_group_set(uint32 group_idx, sys_ipSubnet_vlan_group_t *pProtoGroup);

extern int32 rsd_vlan_ip_subnet_group_destroy(uint32 group_idx, sys_ipSubnet_vlan_group_t *pSubnetGroup);

extern int32 rsd_vlan_port_ipsubnet_vlan_group_bind(sys_logic_port_t port, uint32 group_idx);

extern int32 rsd_vlan_port_ipsubnet_vlan_group_unbind(sys_logic_port_t port, uint32 protoGroup_idx);

extern int32 rsd_vlan_ip_subnet_chip_init(void);

extern int32 rsd_vlan_port_ipsubnet_vlan_enable(sys_logic_port_t port, sys_enable_t enable);
#endif

