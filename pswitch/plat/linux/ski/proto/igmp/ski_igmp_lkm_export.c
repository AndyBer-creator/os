
#include <ski/ski_lkm.h>
#include <common/sys_type.h>
#include <igmp/proto_igmp.h>

EXPORT_SYMBOL(mcast_igmp_enable_set);
EXPORT_SYMBOL(mcast_igmp_info_get);
EXPORT_SYMBOL(mcast_igmp_suppre_enable_set);
EXPORT_SYMBOL(mcast_table_clear);
EXPORT_SYMBOL(mcast_igmp_unknMcastAction_set);
EXPORT_SYMBOL(mcast_igmp_statistics_clear);
EXPORT_SYMBOL(mcast_router_get);

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
EXPORT_SYMBOL(mcast_igmp_querier_set);
EXPORT_SYMBOL(mcast_igmp_querier_get);
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
EXPORT_SYMBOL(mcast_router_static_port_set);
EXPORT_SYMBOL(mcast_router_forbid_port_set);
#endif
EXPORT_SYMBOL(mcast_igmp_operVersion_set);
#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
EXPORT_SYMBOL(mcast_portGroup_limit_set);
EXPORT_SYMBOL(mcast_portGroup_limit_get);
#endif

EXPORT_SYMBOL(mcast_vlan_set);
EXPORT_SYMBOL(mcast_vlan_get);
EXPORT_SYMBOL(mcast_vlan_del);
EXPORT_SYMBOL(mcast_vlan_add);

EXPORT_SYMBOL(mcast_igmp_group_del_byType);

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
EXPORT_SYMBOL(mcast_mld_enable_set);
EXPORT_SYMBOL(mcast_mld_suppre_enable_set);
EXPORT_SYMBOL(mcast_mld_statistics_clear);
EXPORT_SYMBOL(mcast_mld_group_del_byType);
EXPORT_SYMBOL(mcast_mld_operVersion_set);
#endif

