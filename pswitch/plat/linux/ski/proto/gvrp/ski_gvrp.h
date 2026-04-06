
#ifndef __SKI_GVRP_H__
#define __SKI_GVRP_H__

#include <ski/ski_core.h>
#include <gvrp/proto_gvrp.h>

typedef enum ski_igmp_cmd_e
{
    
    SKI_GVRP_SET_BASE = (SKI_CORE_MOD_GVRP * SKI_CORE_MODCMD_TOTAL),
    SKI_GVRP_ENABLE_SET,
    SKI_GVRP_PORT_ENABLE_SET,
    SKI_GVRP_PORT_REGMODE_SET,
    SKI_GVRP_PORT_FORBIDVLAN_SET,
    SKI_GVRP_VLAN_SET,
    SKI_GVRP_TIME_SET,
    SKI_GVRP_STAT_CLEAR,
    SKI_GVRP_ERROR_STAT_CLEAR,

   SKI_GVRP_GET_BASE = (SKI_GVRP_SET_BASE + SKI_CORE_MODCMD_RANGE),
   SKI_GVRP_INFO_GET,
   SKI_GVRP_VLAN_GET,
   SKI_GVRP_PORT_STAT_GET,
   SKI_GVRP_PORT_ERROR_STAT_GET,
   SKI_GVRP_VLAN_MASK_GET,
   SKI_GVRP_PORT_VLAN_MACHINE_GET,
   SKI_GVRP_VLAN_DB_SIZE_GET,

    SKI_GVRP_CMD_END
} ski_gvrp_cmd_t;

typedef struct port_status_s
{
    sys_logic_port_t    port;
    sys_enable_t        enable;
}port_status_t;

typedef struct port_regmode_s
{
    sys_logic_port_t    port;
    register_mode_t    regval;
}port_regmode_t;

typedef struct gvrp_vlan_s
{
    sys_vid_t  vid;
    sys_logic_portmask_t stPmsk;
}gvrp_vlan_t;

typedef struct gvrp_time_s
{
    gvrp_timer_type_t type;
    uint32                   val;
}gvrp_time_t;

typedef struct gvrp_stat_s
{
    sys_logic_port_t port;
    gvrp_statistics_t         statistic;
}gvrp_stat_t;

typedef struct gvrp_error_stat_s
{
    sys_logic_port_t port;
    gvrp_error_statistics_t   statistic;
}gvrp_error_stat_t;

typedef struct gvrp_machine_s
{
    sys_logic_port_t port;
    sys_vid_t  vid;
    Gid_machine machine;
}gvrp_machine_t;

typedef union ski_gvrp_ioctl_u
{
    uint8                   enable;
    uint8                   debug;
    sys_logic_port_t    port;
    port_status_t        portEn;
    port_status_t        forbidVlanEn;
    sys_gvrp_info_t         info;
    gvrp_stat_t                 stat;
    gvrp_error_stat_t        errorStat;
    port_regmode_t        regmode;
    gvrp_vlan_t              vlan;
    Gvd                         vlandb;
    gvrp_time_t             timer;
    uint32                     dbsize;
    sys_vlanmask_t       vlanMask;
    gvrp_machine_t       status;

} ski_gvrp_ioctl_t;

extern int32 ski_gvrp_init(void);

#endif  

