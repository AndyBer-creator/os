
#ifndef __SKI_RATE_H__
#define __SKI_RATE_H__

#include <ski/ski_core.h>
#include <rsd/rsd_rate.h>
#include <rsd/rsd_qos.h>

typedef enum ski_rate_cmd_e
{
    
    SKI_RATE_SET_BASE = (SKI_CORE_MOD_RATE * SKI_CORE_MODCMD_TOTAL),
#if defined(CONFIG_SYS_STORM_CTRL) || defined(CONFIG_SYS_BW_CTRL)
    SKI_RATE_PORT_DFLTINIT,
#endif
#ifdef CONFIG_SYS_STORM_CTRL
    SKI_RATE_STORM_CONTROL_RATE_SET,
    SKI_RATE_STORM_CTRL_ENABLE_SET,
    SKI_RATE_STORM_CTRL_ACTION_SET,
    SKI_RATE_STORM_CTRL_TYPESEL_SET,
    SKI_RATE_STORM_CTRL_INCLUDE_IFG_SET,
    SKI_RATE_STORM_CTRL_REFRESH_MODE_SET,
#endif
#ifdef CONFIG_SYS_BW_CTRL
    SKI_RATE_IGR_BWCTRL_ENABLE_SET,
    SKI_RATE_IGR_BWCTRL_RATE_SET,
    SKI_RATE_IGR_BWCTRL_BURSTSIZE_SET,
    SKI_RATE_IGR_PORT_BWCTRL_BURSTSIZE_SET,
    SKI_RATE_EGR_BWCTRL_ENABLE_SET,
    SKI_RATE_EGR_BWCTRL_RATE_SET,
    SKI_RATE_EGR_BWCTRL_BURSTSIZE_SET,
    SKI_RATE_EGR_PORT_BWCTRL_BURSTSIZE_SET,
    SKI_RATE_EGR_QUEUE_RATE_SET,
    SKI_RATE_EGR_QUEUE_ENABLE_SET,
    SKI_RATE_EGR_QUEUE_BURSTSIZE_SET,
    SKI_RATE_EGR_PORT_QUEUE_BURSTSIZE_SET,
    SKI_RATE_IGR_BWCTRL_INCLUDE_IFG_SET,
    SKI_RATE_EGR_BWCTRL_INCLUDE_IFG_SET,
#endif
#ifdef CONFIG_SYS_DEFENCE_ENGINE
    SKI_RATE_CPU_EGR_BWCTRL_SET,
    SKI_RATE_CPU_EGR_QUEUE_BWCTRL_SET,
    SKI_RATE_CPU_EGR_BWCTRL_RATE_MODE_SET,

#endif

    SKI_RATE_GET_BASE = (SKI_RATE_SET_BASE + SKI_CORE_MODCMD_RANGE),
#ifdef CONFIG_SYS_STORM_CTRL
    SKI_RATE_STORM_DUMMY1_GET, 
    SKI_RATE_STORM_DUMMY2_GET, 
    SKI_RATE_STORM_CONTROL_RATE_GET,
    SKI_RATE_STORM_CTRL_ENABLE_GET,
    SKI_RATE_STORM_CTRL_ACTION_GET,
    SKI_RATE_STORM_CTRL_TYPESEL_GET,
    SKI_RATE_STORM_CTRL_INCLUDE_IFG_GET,
    SKI_RATE_STORM_CTRL_REFRESH_MODE_GET,
#endif
#ifdef CONFIG_SYS_BW_CTRL
    SKI_RATE_IGR_BWCTRL_ENABLE_GET,
    SKI_RATE_IGR_BWCTRL_RATE_GET,
    SKI_RATE_IGR_BWCTRL_BURSTSIZE_GET,
    SKI_RATE_IGR_PORT_BWCTRL_BURSTSIZE_GET,
    SKI_RATE_EGR_BWCTRL_ENABLE_GET,
    SKI_RATE_EGR_BWCTRL_RATE_GET,
    SKI_RATE_EGR_BWCTRL_BURSTSIZE_GET,
    SKI_RATE_EGR_PORT_BWCTRL_BURSTSIZE_GET,
    SKI_RATE_EGR_QUEUE_RATE_GET,
    SKI_RATE_EGR_QUEUE_ENABLE_GET,
    SKI_RATE_EGR_QUEUE_BURSTSIZE_GET,
    SKI_RATE_EGR_PORT_QUEUE_BURSTSIZE_GET,
    SKI_RATE_IGR_BWCTRL_INCLUDE_IFG_GET,
    SKI_RATE_EGR_BWCTRL_INCLUDE_IFG_GET,
#endif

#ifdef CONFIG_SYS_DEFENCE_ENGINE
    SKI_RATE_CPU_EGR_BWCTRL_RATE_MODE_GET,
#endif

    SKI_RATE_CMD_END
} ski_rate_cmd_t;

#ifdef CONFIG_SYS_STORM_CTRL
typedef struct ski_rate_stormControlRate_s
{
    sys_logic_port_t        port;
    sys_rate_storm_group_t   storm_type;
    sys_rate_storm_sel_t    storm_sel;
    sys_enable_t            enable;
    uint32                  rate;
} ski_rate_stormControlRate_t;

typedef struct ski_rate_stormControlState_s
{
    sys_logic_port_t        port;
    sys_rate_storm_group_t  storm_type;
    sys_enable_t            enable;
} ski_rate_stormControlState_t;

typedef struct ski_rate_stormControlAction_s
{
    sys_logic_port_t        port;
    sys_rate_storm_action_t action;
} ski_rate_stormControlAction_t;

#endif 

#ifdef CONFIG_SYS_BW_CTRL
typedef struct ski_rate_bandwidthControlStatus_s
{
    sys_logic_port_t    port;
    sys_qid_t queueId;
    sys_enable_t        enable;
} ski_rate_bandwidthControlStatus_t;

typedef struct ski_rate_bandwidthControlRate_s
{
    sys_logic_port_t    port;
    sys_qid_t  queueId;
    uint32              rate;
    uint32              burst_size;
} ski_rate_bandwidthControlRate_t;

#endif 

#ifdef CONFIG_SYS_DEFENCE_ENGINE
typedef struct ski_rate_cpuEgrBwCtrl_s
{
    uint32 rate;
    sys_qid_t qid;
    sys_enable_t enable;
    sys_rate_rateMode_t rateMode;
} ski_rate_cpuEgrBwCtrl_t;
#endif

typedef union ski_rate_ioctl_u
{
#if defined(CONFIG_SYS_STORM_CTRL) || defined(CONFIG_SYS_BW_CTRL)
    sys_rate_portDflt_t portDfltInfo;
#endif
#ifdef CONFIG_SYS_STORM_CTRL
    ski_rate_stormControlRate_t stormControlRate;
    ski_rate_stormControlAction_t stormCtrlAction;
    sys_enable_t    stormCtrlIncludeIfg;
    sys_rate_storm_rateMode_t stormRefreshMode;
    ski_rate_stormControlState_t stormCtrlState;

#endif

#ifdef CONFIG_SYS_BW_CTRL
    ski_rate_bandwidthControlStatus_t bandwidthControlStatus;
    ski_rate_bandwidthControlRate_t bandwidthControlRate;
    sys_enable_t    bandwidthCtrlIncludeIfg;
    uint32          burst;
#endif

#ifdef CONFIG_SYS_DEFENCE_ENGINE
    ski_rate_cpuEgrBwCtrl_t cpuEgrBwCtrl;
#endif
} ski_rate_ioctl_t;

int32 ski_rate_init(void);

#endif 

