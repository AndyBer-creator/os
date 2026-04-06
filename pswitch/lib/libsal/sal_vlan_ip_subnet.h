
#ifndef __SAL_VLAN_IP_SUBNET_H__
#define __SAL_VLAN_IP_SUBNET_H__

#include <rsd/rsd_vlan_ip_subnet.h>

extern int32 sal_vlan_ip_subnet_group_get(uint32 group_idx, sys_ipSubnet_vlan_group_t *pIpSubNetGroup);

extern int32 sal_vlan_ip_subnet_group_set(uint32 group_idx, sys_ipSubnet_vlan_group_t *pSubnetGroup);
extern int32 sal_vlan_ip_subnet_group_del(uint32 group_idx);
extern int32 sal_vlan_port_ipsubnet_vlan_group_bind_set(sys_logic_port_t port, uint32 group_idx, sys_enable_t enable);
extern int32 sal_vlan_port_ipsubnet_vlan_group_bind_get(sys_logic_port_t port, uint32 *pGroup_idx);
extern int32 sal_vlan_port_ipsubnet_vlan_enable(sys_logic_port_t port, sys_enable_t enable);
extern int32 sal_vlan_port_ipsubnet_vlan_status(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 sal_vlan_ip_subnet_group_init(uint32 group_idx, sys_ipSubnet_vlan_group_t *pSubnetGroup);
extern int32 sal_vlan_ip_subnet_port_init(sys_logic_port_t port, sys_enable_t enable);
#endif

