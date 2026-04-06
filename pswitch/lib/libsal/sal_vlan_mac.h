
#ifndef __SAL_VLAN_MAC_H__
#define __SAL_VLAN_MAC_H__

#include <macvlan/proto_macvlan.h>

extern int32 sal_macvlan_group_get(uint32 idx, sys_mac_vlan_group_t *pmacGroup);

extern int32 sal_macvlan_group_set(sys_mac_vlan_group_t *pmacGroup);

extern int32 sal_macvlan_group_del(sys_mac_vlan_group_t *pmacGroup);

extern int32 sal_macvlan_mapping_get(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping);

extern int32 sal_macvlan_mapping_set(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping);

extern int32 sal_macvlan_mapping_del(sys_logic_port_t port, sys_mac_vlan_mapping_t *mapping);
#endif

