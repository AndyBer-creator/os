#ifndef __PROTO_MACVLAN_H__
#define __PROTO_MACVLAN_H__
#include <common/sys_type.h>
typedef struct sys_mac_vlan_group_s
{
    sys_mac_t mac;
    uint8 mask;
    uint32 groupId;
}sys_mac_vlan_group_t;

typedef struct sys_mac_vlan_mapping_s
{
    sys_vid_t vid;
    uint32 groupId;
}sys_mac_vlan_mapping_t;
#define MV_DBG(fmt, args...) SYS_DBG(DBG_VLAN_MAC, fmt, ##args)
extern int32 macvlan_group_get(uint32 idx, sys_mac_vlan_group_t *pmacGroup);
extern int32 macvlan_group_set(sys_mac_vlan_group_t *pmacGroup);
extern int32 macvlan_group_del(sys_mac_vlan_group_t *pmacGroup);
extern int32 macvlan_mapping_get(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping);
extern int32 macvlan_mapping_set(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping);
extern int32 macvlan_mapping_del(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping);
extern int32 mac_vlan_init(void);
#endif

