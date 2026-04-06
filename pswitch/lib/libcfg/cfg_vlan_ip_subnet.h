
#ifndef __CFG_VLAN_IP_SUBNET_H__
#define __CFG_VLAN_IP_SUBNET_H__

#include <common/sys_list.h>
#include <libsal/sal_vlan_ip_subnet.h>

typedef struct cfg_vlan_ipsubnet_portCfg_s
{
    sys_enable_t enable;
    uint32      groupId;
}cfg_vlan_ipsubnet_portCfg_t;

typedef struct cfg_vlan_ipsubnet_s
{
    sys_ipSubnet_vlan_group_t groups[CAPA_VLAN_IP_SUBNET_GROUP_NUM];
    sys_enable_t              pvlanPortCfg[SYS_LOGICPORT_NUM_MAX];
} cfg_vlan_ipsubnet_t;

extern int32 cfg_vlan_ipsubnet_group_get( uint32 group_idx, sys_ipSubnet_vlan_group_t *pIpsubGroup);

extern int32 cfg_vlan_ipsubnet_group_set( uint32 protoGroup_idx, sys_ipSubnet_vlan_group_t *pProtoGroup);
extern int32 cfg_vlan_ipsubnet_port_group_unbind_set(sys_logic_port_t port, uint32 group_idx);
extern int32 cfg_vlan_ipsubnet_port_group_bind_set(sys_logic_port_t port, uint32 group_idx);

extern int32 cfg_vlan_ipsubnet_port_enable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 cfg_vlan_ipsubnet_port_enable_get(sys_logic_port_t port, sys_enable_t* pEnable);
#endif

