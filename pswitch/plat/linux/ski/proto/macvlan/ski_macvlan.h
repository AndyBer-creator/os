
#ifndef __SKI_MACVLAN_H__
#define __SKI_MACVLAN_H__

#include <ski/ski_core.h>
#include <macvlan/proto_macvlan.h>

typedef enum ski_mac_vlan_cmd_e
{
    
    SKI_MACVLAN_SET_BASE = (SKI_CORE_MOD_MACVLAN * SKI_CORE_MODCMD_TOTAL),
    SKI_MACVLAN_GROUP_SET,
    SKI_MACVLAN_GROUP_DEL,
    SKI_MACVLAN_MAPPING_SET,
    SKI_MACVLAN_MAPPING_DEL,

    SKI_MACVLAN_GET_BASE = (SKI_MACVLAN_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_MACVLAN_GROUP_GET,
    SKI_MACVLAN_MAPPING_GET,

    SKI_MACVLAN_CMD_END
} ski_mac_vlan_cmd_t;

typedef struct ski_macvlan_group_s
{
    uint32   idx;
    sys_mac_vlan_group_t group;
}ski_macvlan_group_t;

typedef struct ski_macvlan_mapping_s
{
    sys_logic_port_t port;
    sys_mac_vlan_mapping_t mapping[CAPA_VLAN_MAC_GROUP_NUM_MAX];
}ski_macvlan_mapping_t;

typedef union ski_macvlan_ioctl_u
{
    ski_macvlan_group_t group;
    ski_macvlan_mapping_t mapping;
} ski_macvlan_ioctl_t;

extern int32 ski_mac_vlan_init(void);
#endif

