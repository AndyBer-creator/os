
#ifndef __CFG_VLAN_PROTO_H__
#define __CFG_VLAN_PROTO_H__

#include <common/sys_list.h>
#include <libsal/sal_vlan_proto.h>

typedef struct cfg_pvlan_portCfg_s
{
    sys_enable_t enable;
    sys_vlan_protoVlanCfg_t vlan_cfg;
}cfg_pvlan_portCfg_t;

typedef struct cfg_vlan_proto_s
{
    sys_vlan_protoGroup_t groups[CAPA_VLAN_PROTO_GROUP_NUM];
    cfg_pvlan_portCfg_t pvlanPortCfg[SYS_LOGICPORT_NUM_MAX][CAPA_VLAN_PROTO_GROUP_NUM];
} cfg_vlan_proto_t;

extern int32 cfg_vlan_protoGroup_get( uint32 protoGroup_idx, sys_vlan_protoGroup_t *pProtoGroup);

extern int32 cfg_vlan_protoGroup_set( uint32 protoGroup_idx, sys_vlan_protoGroup_t *pProtoGroup);

extern int32 cfg_vlan_portProtoVlan_get(sys_logic_port_t port, uint32 protoGroup_idx, sys_vlan_protoVlanCfg_t *pVlan_cfg, sys_enable_t *pEnable);

extern int32 cfg_vlan_portProtoVlan_set(sys_logic_port_t port, uint32 protoGroup_idx, sys_vlan_protoVlanCfg_t *pVlan_cfg, sys_enable_t enable);
#endif

