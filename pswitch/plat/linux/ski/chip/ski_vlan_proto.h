
#ifndef __SKI_VLAN_PROTO_H__
#define __SKI_VLAN_PROTO_H__

#include <ski/ski_core.h>
#include <rsd/rsd_vlan_proto.h>

typedef enum ski_vlan_proto_cmd_e
{
    
    SKI_VLAN_PROTO_SET_BASE = (SKI_CORE_MOD_VLAN_PROTO * SKI_CORE_MODCMD_TOTAL),
    SKI_VLAN_PROTOGROUP_SET,
    SKI_VLAN_PORTPROTOVLAN_SET,
    SKI_VLAN_PORTPROTOVLAN_DISABLE,

    SKI_VLAN_PROTO_GET_BASE = (SKI_VLAN_PROTO_SET_BASE + SKI_CORE_MODCMD_RANGE),

    SKI_VLAN_PROTO_CMD_END
} ski_vlan_proto_cmd_t;

typedef union ski_vlan_proto_ioctl_u
{
    struct
    {
        uint32   index;
        sys_vlan_protoGroup_t   protoGroup;
    } proGroup;

    struct ski_vlan_portPro_s
    {
        sys_logic_port_t port;
        uint32 protoGroup_idx;
        sys_vlan_protoVlanCfg_t vlan_cfg;
    } portPro;
} ski_vlan_proto_ioctl_t;

extern int32 ski_vlan_proto_init(void);
#endif

