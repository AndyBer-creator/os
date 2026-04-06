
#ifndef __SAL_VLAN_PROTO_H__
#define __SAL_VLAN_PROTO_H__

#include <rsd/rsd_vlan_proto.h>

extern int32 sal_vlan_protoGroup_get(uint32 protoGroup_idx, sys_vlan_protoGroup_t *pProtoGroup);

extern int32 sal_vlan_protoGroup_set(uint32 protoGroup_idx, sys_vlan_protoGroup_t *pProtoGroup);

extern int32 sal_vlan_portProtoVlan_get(sys_logic_port_t port, uint32 protoGroup_idx, sys_vlan_protoVlanCfg_t *pVlan_cfg, sys_enable_t *pEnable);

extern int32 sal_vlan_portProtoVlan_set(sys_logic_port_t port, uint32 protoGroup_idx, sys_vlan_protoVlanCfg_t *pVlan_cfg, sys_enable_t enable);
#endif

