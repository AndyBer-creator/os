
#ifndef __SKI_PORT_H__
#define __SKI_PORT_H__

#include <ski/ski_core.h>
#include <rsd/rsd_port.h>

typedef enum ski_port_cmd_e
{
    
    SKI_PORT_SET_BASE = (SKI_CORE_MOD_PORT * SKI_CORE_MODCMD_TOTAL),
    SKI_PORT_SET_ENABLE,
    SKI_PORT_SET_ENABLE_FORCE,
    SKI_PORT_SET_ADMIN_ENABLE,
    SKI_PORT_SET_RX_ENABLE,
    SKI_PORT_SET_COMBO_MEDIA_PREFER,
    SKI_PORT_SET_MEDIA,
    SKI_PORT_SET_PORT_PROTECTED,
    SKI_PORT_SET_AUTO_NEGO_ENABLE,
    SKI_PORT_SET_ABILITY_AUTO_NEGO,
    SKI_PORT_SET_ABILITY_FORCE_MODE,
    SKI_PORT_SET_10G_MEDIA,
    SKI_PORT_SET_FLOW_CTRL,
    SKI_PORT_SET_DESCRIPTION,
    SKI_PORT_SET_ERR_DISABLE_RECOVERY,
    SKI_PORT_SET_ERR_DISABLE_TIME,
    SKI_PORT_SET_ERR_DISABLE_CURRTIME,
    SKI_PORT_SET_ERR_DISABLE_DO_RECOVERY,
    SKI_PORT_SET_BACK_PRESSURE_ENABLE,
    SKI_PORT_SET_FLOW_CTRL_AUTO,
    SKI_PORT_SET_COMBO_1G_FIBER_SPEED,
    SKI_PORT_SET_FIBER_FORCE_ENABLE,
    
    SKI_PORT_GET_BASE = (SKI_PORT_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_PORT_GET_ENABLE,
    SKI_PORT_GET_LINK,
    SKI_PORT_GET_ALL_LINK,
    SKI_PORT_GET_COMBO_MEDIA_PREFER,
    SKI_PORT_GET_LINK_MEDIA,
    SKI_PORT_GET_MEDIA,
    SKI_PORT_GET_PORT_PROTECTED,
    SKI_PORT_GET_SPEED_DUPLEX,
    SKI_PORT_GET_FLOW_CTRL,
    SKI_PORT_GET_FLOW_CTRL_STATUS,
    SKI_PORT_GET_AUTO_NEGO_ENABLE,
    SKI_PORT_GET_ABILITY_AUTO_NEGO,
    SKI_PORT_GET_ABILITY_FORCE_MODE,
    SKI_PORT_GET_DESCRIPTION,
    SKI_PORT_GET_ERR_DISABLE,
    SKI_PORT_GET_ERR_DISABLE_RECOVERY,
    SKI_PORT_GET_ERR_DISABLE_TIME,
    SKI_PORT_GET_ERR_DISABLE_CURRTIME,
    SKI_PORT_GET_LINK_CHANGETIME,
    SKI_PORT_GET_STATE_CONTROL,
    SKI_PORT_GET_BACK_PRESSURE_ENABLE,
    SKI_PORT_GET_FLOW_CTRL_AUTO,
    SKI_PORT_GET_LOGIC_TO_PHYSIC,
    SKI_PORT_GET_PHYSIC_TO_LOGIC,

    SKI_PORT_CMD_END
} ski_port_cmd_t;

typedef struct ski_port_enable_s
{
    sys_logic_port_t port;
    sys_enable_t enable;
} ski_port_enable_t;

typedef struct ski_port_linkStatus_s
{
    sys_logic_port_t port;
    sys_port_linkStatus_t status;
    sys_port_linkChangeTime_t time;
} ski_port_linkStatus_t;

typedef struct ski_port_comboMedia_s
{
    sys_logic_port_t port;
    sys_port_comboMedia_t media;
} ski_port_comboMedia_t;

typedef struct ski_port_media_s
{
    sys_logic_port_t port;
    sys_port_media_t media;
} ski_port_media_t;

typedef struct ski_port_xgMedia_s
{
    sys_logic_port_t port;
    sys_port_10gMedia_t xgMedia;
} ski_port_xgMedia_t;

typedef struct ski_port_isolation_s
{
    sys_logic_port_t port;
    sys_logic_portmask_t portmask;
} ski_port_isolation_t;

typedef struct ski_port_speedDuplex_s
{
    sys_logic_port_t port;
    sys_port_speedDuplex_t speedDuplex;
} ski_port_speedDuplex_t;

typedef struct ski_port_ability_s
{
    sys_logic_port_t port;
    sys_port_phyAbility_t autoAbility;
    sys_port_speedDuplex_t fmSpeedDuplex;
    sys_enable_t fmFlowCtrl;
} ski_port_ability_t;

typedef struct ski_port_speed_s
{
    sys_logic_port_t        port;
    sys_port_speedAbility_t speed;
} ski_port_speed_t;

typedef struct ski_port_descp_s
{
    sys_logic_port_t port;
    char descp[CAPA_PORTDESC_STR_LEN + 1];
} ski_port_descp_t;

typedef struct ski_port_flowCtrlStatus_s
{
    sys_logic_port_t port;
    sys_enable_t     txStatus;
    sys_enable_t     rxStatus;
} ski_port_flowCtrlStatus_t;

typedef struct ski_port_errDisable_s
{
    union
    {
        sys_logic_port_t port;
        sys_enable_t     enable;
    };

    union
    {
        sys_port_errDisable_t reason;
        uint32                time;
    };
} ski_port_errDisable_t;

typedef struct ski_port_backPressure_s
{
    sys_logic_port_t port;
    sys_enable_t     enable;
} ski_port_backPressure_t;

typedef struct ski_port_flowCtrlAuto_s
{
    sys_logic_port_t port;
    sys_enable_t     enable;
} ski_port_flowCtrlAuto_t;

typedef struct ski_port_translate_s
{
    sys_logic_port_t  logicPort;
    sys_physic_port_t physicPort;
} ski_port_translate_t;

typedef struct ski_port_combo_1g_fiber_s
{
    sys_logic_port_t  logicPort;
    rtk_port_fiber_media_t gMedia;
} ski_port_combo_1g_fiber_t;

typedef union ski_port_ioctl_u
{
    ski_port_enable_t         portEnable;
    ski_port_linkStatus_t     portStatus;
    sys_port_allLinkStatus_t  portAllStatus;
    ski_port_comboMedia_t     portComboMedia;
    ski_port_media_t          portMedia;
    ski_port_xgMedia_t        xgPortMedia;
    ski_port_isolation_t      portIsolation;
    ski_port_speedDuplex_t    portSpeedDuplex;
    ski_port_ability_t        portAbility;
    ski_port_speed_t          portSpeed;
    ski_portconf_t            portCfg;
    sys_physic_portInof_t     physicPort;
    ski_port_descp_t          portDescp;
    sys_port_stateCtrl_t      portstateCtrl;
    ski_port_flowCtrlStatus_t portFlowCtrlStatus;
    ski_port_errDisable_t     portErrDisable;
    ski_port_backPressure_t   portBackPressure;
    ski_port_flowCtrlAuto_t   portFlowCtrlAuto;
    ski_port_translate_t      portTranslate;
    ski_port_combo_1g_fiber_t port_combo_lg_fiber;
} ski_port_ioctl_t;

extern int32 ski_port_init(void);

#endif  

