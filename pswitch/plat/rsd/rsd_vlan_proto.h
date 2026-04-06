
#ifndef __RSD_VLAN_PROTO_H__
#define __RSD_VLAN_PROTO_H__

#include <rtk/vlan.h>

typedef rtk_vlan_protoGroup_t sys_vlan_protoGroup_t;
typedef rtk_vlan_protoVlanCfg_t sys_vlan_protoVlanCfg_t;

#define PV_DBG(fmt, args...) SYS_DBG(DBG_VLAN_PROTOCOL, fmt, ##args)

extern int32 rsd_vlan_protoGroup_set(uint32 protoGroup_idx, sys_vlan_protoGroup_t *pProtoGroup);

extern int32 rsd_vlan_portProtoVlan_set(sys_logic_port_t port, uint32 protoGroup_idx, sys_vlan_protoVlanCfg_t *pVlan_cfg);

extern int32 rsd_vlan_portProtoVlan_disable(sys_logic_port_t port, uint32 protoGroup_idx);

extern int32 rsd_vlan_proto_chip_init(void);
#endif

