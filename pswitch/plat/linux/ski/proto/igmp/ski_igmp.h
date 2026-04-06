
#ifndef __SKI_IGMP_H__
#define __SKI_IGMP_H__

#include <ski/ski_core.h>
#include <igmp/proto_igmp.h>

typedef enum ski_igmp_cmd_e
{
    
    SKI_IGMP_SET_BASE = (SKI_CORE_MOD_IGMP * SKI_CORE_MODCMD_TOTAL),
    SKI_IGMP_ENABLE_SET,
    SKI_IGMP_SUPPRE_ENABLE_SET,
    SKI_IGMP_LOOKUP_MODE_SET,
    SKI_IGMP_STATISTICS_RESET,
    SKI_IGMP_PORTSTAT_RESET,
    SKI_IGMP_VLAN_SET,
    SKI_IGMP_VLAN_DEL,
    SKI_IGMP_VLAN_ADD,
    SKI_IGMP_TABLE_CLR,
    SKI_IGMP_UNKNOWN_MCAST_ACTION_SET,
    #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    SKI_IGMP_QUERIER_SET,
    #endif
    SKI_IGMP_OPER_VERSION_SET,

    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    SKI_IGMP_PORTGROUP_LIMIT_SET,
    SKI_IGMP_PORTGROUP_MAX_GROUP_ACT_SET,
    #endif

    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
    SKI_IGMP_STATIC_ROUTER_PORT_SET,
    SKI_IGMP_FORBID_ROUTER_PORT_SET,
    #endif

    SKI_IGMP_STATIC_GROUP_CLEAR,
    SKI_IGMP_STATIC_GROUP_ENTRY_SET,
    SKI_IGMP_GROUP_TYPE_CLEAR,
    SKI_IGMP_STATIC_GROUP_L2MCAST_ENTRY_ADD,
	SKI_IGMP_STATIC_GROUP_L2MCAST_ENTRY_DEL,

    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    SKI_IGMPV3_STATIC_GROUP_ENTRY_SET,
    SKI_IGMPV3_STATIC_FILTER_GROUP_ENTRY_SET,
    #endif

    #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    SKI_IGMP_PROFILE_SET,
    SKI_IGMP_FILTER_SET,
    #endif

    #ifdef CONFIG_SYS_PROTO_MVR
    
    SKI_IGMP_MVR_ENABLE_SET,
    SKI_IGMP_MVR_MODE_SET,
    SKI_IGMP_MVR_QUERYTIME_SET,
    SKI_IGMP_MVR_VLANID_SET,
    SKI_IGMP_MVR_GROUPRANGE_SET,
    SKI_IGMP_MVR_PORTTYPE_SET,
    SKI_IGMP_MVR_PORTIMMEDIATE_SET,
    SKI_IGMP_MVR_GROUP_TYPE_CLEAR,
    #endif

    SKI_IGMP_GET_BASE = (SKI_IGMP_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_MCAST_ENTRYMAX_GET,
    SKI_IGMP_ENTRYMAX_GET,
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    SKI_MLD_ENTRYMAX_GET,
#endif
    SKI_IGMP_INFO_GET,
    SKI_IGMP_PORTSTAT_GET,
    SKI_IGMP_LOOKUP_MODE_GET,
    SKI_MCAST_DBSIZE_GET,
    SKI_IGMP_DBSIZE_GET,

    SKI_IGMP_DIPONLY_DBSIZE_GET,
    SKI_IGMP_DB_ENTRY_GET,
    SKI_IGMP_VLAN_GET,
    SKI_IGMP_VLAN_CONFIG_GET,
    SKI_IGMP_ROUTER_GET,

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    SKI_IGMP_GRPHEAD_DB_ENTRY_GET,
#endif

 #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    SKI_IGMP_QUERIER_GET,
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    SKI_IGMP_PROFILE_GET,
#endif

#ifdef CONFIG_SYS_PROTO_MVR
    SKI_IGMP_MVR_DBSIZE_GET,
#endif
	SKI_IGMP_STATIC_GROUP_L2MCAST_DBSIZE_GET,
	SKI_IGMP_STATIC_GROUP_L2MCAST_DB_ENTRY_GET,

    SKI_IGMP_CMD_END
} ski_igmp_cmd_t;

typedef struct mcast_vlan_config_s{
    multicast_ipType_t     ipType;
    sys_vlanmask_t        confVlanMsk;
}mcast_vlan_config_t;

#ifdef CONFIG_SYS_PROTO_MVR
typedef struct mvr_port_config_s{
    sys_logic_port_t port;
    sys_enable_t      enable;
    mvr_portType_t  type;
}mvr_port_config_t;
#endif

typedef union ski_igmp_ioctl_u
{
    uint8                               enable;
    uint8                               operVersion;
    mcast_group_type_t         type;
    igmp_unknMcastAction_t   unknMcastActn;
    uint32                  dbSize;
    sys_logic_port_t    port;
    sys_mcast_info_t         info;
    sys_mcast_portStat_t  portStat;
    mcast_vlan_entry_t    vlan;
    mcast_vlan_config_t   configMsk;
    igmp_router_entry_t     router;
    sys_igmp_db_t           db;

    #ifdef  CONFIG_SYS_PROTO_IGMPV3_FULL
    sys_igmp_groupHead_db_t grpHeadDb;
    #endif
    #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    sys_igmp_querier_t      querier;
    #endif

    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    sys_igmp_portGroup_limit_t  limit;
    sys_igmp_maxGroup_act_t  maxgroup_act;
    #endif
    igmp_lookup_mode_t      lookup_mode;
    igmp_static_group_set_t  group;
    #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    sys_igmp_profile_t          profile;
    sys_igmp_filter_t             filter;
    #endif
    #ifdef CONFIG_SYS_PROTO_MVR
    mvr_config_t                 mvrConfig;
    mvr_port_config_t          mvrPortConfig;
    #endif
    mcast_mac_t                 mcastMac;
    l2_mcast_group_t            l2Mcast;
} ski_igmp_ioctl_t;

extern int32 ski_igmp_init(void);

#endif  

