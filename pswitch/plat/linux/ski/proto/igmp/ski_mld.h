
#ifndef __SKI_MLD_H__
#define __SKI_MLD_H__

#include <ski/ski_core.h>
#include <igmp/proto_igmp.h>

typedef enum ski_mld_cmd_e
{
    
    SKI_MLD_SET_BASE = (SKI_CORE_MOD_MLD* SKI_CORE_MODCMD_TOTAL),
    SKI_MLD_ENABLE_SET,
    SKI_MLD_SUPPRE_ENABLE_SET,
    SKI_MLD_STATISTICS_RESET,
    SKI_MLD_LOOKUP_MODE_SET,
    SKI_MLD_GROUP_TYPE_CLEAR,
    SKI_MLD_VERSION_SET,
    SKI_MLD_UNKNOWN_MCAST_ACTION_SET,

    SKI_MLD_GET_BASE = (SKI_MLD_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_MLD_INFO_GET,
    SKI_MLD_LOOKUP_MODE_GET,
    SKI_MLD_DBSIZE_GET,

    SKI_MLD_CMD_END
} ski_mld_cmd_t;

typedef union ski_mld_ioctl_u
{
    uint8                   enable;
    uint8                   unknMcastActn;
    uint8                   operVersion;
    uint32                  dbSize;

    mcast_group_type_t  type;
    igmp_lookup_mode_t      lookup_mode;

    sys_mcast_info_t         info;
    sys_logic_port_t    port;

    igmp_static_group_set_t  group;

} ski_mld_ioctl_t;

extern int32 ski_mld_init(void);

#endif  

