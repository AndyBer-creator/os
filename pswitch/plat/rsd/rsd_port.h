
#ifndef __RSD_PORT_H__
#define __RSD_PORT_H__

#include <rtk/l2.h>
#include <rtk/port.h>
#include <rtk/flowctrl.h>
#include <common/sys_def.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_sys.h>
#include <rsd/rsd_stp.h>

#define SYS_PORT_SPCONGEST_TIMER      (2)   
#define SYS_PORT_SPCONGEST_CNTR       (4)
#define SYS_SPCONGEST_DRAINOUT_THRD     (0x50)

typedef uint32 sys_port_linkChangeTime_t;
typedef rtk_port_linkStatus_t sys_port_linkStatus_t;
typedef rtk_port_phy_ability_t sys_port_phyAbility_t;
typedef rtk_flowctrl_drop_thresh_t sys_flowctrl_drop_thresh_t;
typedef rtk_port_10gMedia_t sys_port_10gMedia_t;

typedef struct sys_port_event_s
{
    sys_logic_port_t lport;

    union
    {
        sys_port_linkStatus_t new_linkStatus;
        sys_enable_t enable;
    };
} sys_port_event_t;

typedef struct sys_port_speedDuplex_s
{
    rtk_port_speed_t speed;
    rtk_port_duplex_t duplex;
} sys_port_speedDuplex_t;

typedef struct sys_port_flowCtrl_s
{
    uint32 txStatus;
    uint32 rxStatus;
} sys_port_flowCtrl_t;

typedef struct sys_port_allLinkStatus_s
{
    rtk_port_linkStatus_t status[SYS_LOGICPORT_NUM_MAX];
} sys_port_allLinkStatus_t;

typedef enum sys_port_speedAbility_e
{
    SYS_PORT_SPEED_10M = 0,
    SYS_PORT_SPEED_100M,
    SYS_PORT_SPEED_1000M,
    SYS_PORT_SPEED_10M_100M,
    SYS_PORT_SPEED_2500M,
    SYS_PORT_SPEED_10G,
    SYS_PORT_SPEED_10G_DAC_50,
    SYS_PORT_SPEED_10G_DAC_100,
    SYS_PORT_SPEED_10G_DAC_300,
    SYS_PORT_SPEED_10G_DAC_500,
    SYS_PORT_SPEED_ALL,
    SYS_PORT_SPEED_END
} sys_port_speedAbility_t;

typedef enum sys_port_duplexAbility_e
{
    SYS_PORT_DUPLEX_FULL = 0,
    SYS_PORT_DUPLEX_HALF,
    SYS_PORT_DUPLEX_END
} sys_port_duplexAbility_t;

typedef enum sys_port_errDisable_e
{
    SYS_PORT_ERRDISABLE_NONE = 0,
    SYS_PORT_ERRDISABLE_BPDUGUARD,
#ifdef CONFIG_SYS_PROTO_UDLD
    SYS_PORT_ERRDISABLE_UDLD,
#endif
#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
    SYS_PORT_ERRDISABLE_SELFLOOP,
#endif
    SYS_PORT_ERRDISABLE_STORMBCAST,
    SYS_PORT_ERRDISABLE_STORMUNKMCAST,
    SYS_PORT_ERRDISABLE_STORMUNKUCAST,
    SYS_PORT_ERRDISABLE_USER_DEFINED_ACL,
    SYS_PORT_ERRDISABLE_PORT_SECURITY,
    SYS_PORT_ERRDISABLE_DHCP_RATE_LIMIT,
    SYS_PORT_ERRDISABLE_ARP_RATE_LIMIT,
    SYS_PORT_ERRDISABLE_END
} sys_port_errDisable_t;

typedef enum sys_port_stateCtrlProto_e
{
#ifdef CONFIG_SYS_PROTO_STP
    SYS_PORT_STATECTRLPROTO_STP,
#endif
#ifdef CONFIG_SYS_PROTO_LACP
    SYS_PORT_STATECTRLPROTO_LACP,
#endif
    SYS_PORT_STATECTRLPROTO_CHIP,
    SYS_PORT_STATECTRLPROTO_END
} sys_port_stateCtrlProto_t;

typedef enum sys_port_comboMedia_e
{
    PORT_COMBO_MEDIA_AUTO = 0,
    PORT_COMBO_MEDIA_FORCE_COPPER,
    PORT_COMBO_MEDIA_FORCE_FIBER,
    PORT_COMBO_END
} sys_port_comboMedia_t;

typedef struct sys_port_stateCtrl_s
{
    sys_port_stateCtrlProto_t proto;
    sys_logic_port_t          port;
    sys_stp_state_t           state;
    uint32                    msti;
} sys_port_stateCtrl_t;

extern const sys_text_t text_port_log_speed[];
extern const sys_text_t text_port_log_duplex[];
extern const sys_text_t text_port_log_media[];

#define IS_ERRDISABLE_REASON_IN_MASK(mask, reason) ((mask >> reason) & 0x1)

extern int32 rsd_port_init(void);

extern int32 rsd_port_specialCongest_init(void);

extern int32 rsd_port_monitor_init(void);

extern int32 rsd_port_linkChangeTime_get(sys_logic_port_t port, uint32 *changeTime);

extern int32 rsd_port_linkStatus_set(sys_logic_port_t port, sys_port_linkStatus_t link);

extern int32 rsd_port_linkStatus_get(sys_logic_port_t port, sys_port_linkStatus_t *pLink);

extern int32 rsd_port_allLinkStatus_get(sys_port_allLinkStatus_t *pAllLink);

extern int32 rsd_port_linkMedia_get(sys_logic_port_t port, sys_port_media_t *pMedia);

extern int32 rsd_port_linkMediaEvent_handler(sys_logic_port_t port, sys_port_media_t media);

extern int32 rsd_port_comboMediaPrefer_set(sys_logic_port_t port, sys_port_comboMedia_t Media);

extern int32 rsd_port_comboMediaPrefer_get(sys_logic_port_t port, sys_port_comboMedia_t *pMedia);

extern int32 rsd_port_media_set(sys_logic_port_t port, sys_port_media_t media);

extern int32 rsd_port_media_get(sys_logic_port_t port, sys_port_media_t *pMedia);

extern int32 rsd_port_adminEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_enable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_enableForce_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);

#ifdef CONFIG_SYS_PORT_ERR_DISABLED

extern int32 rsd_port_errDisable_init(void);

extern int32 rsd_port_errDisable_set(sys_logic_port_t port, sys_port_errDisable_t reason);

extern int32 rsd_port_errDisable_get(sys_logic_port_t port, sys_port_errDisable_t *pReason);

extern int32 rsd_port_errDisableRecovery_set(sys_port_errDisable_t reason, sys_enable_t enable);

extern int32 rsd_port_errDisableRecovery_get(sys_port_errDisable_t reason, sys_enable_t *pEnable);

extern int32 rsd_port_errDisableTime_set(uint32 time);

extern int32 rsd_port_errDisableTime_get(uint32 *pTime);

extern int32 rsd_port_errDisableCurrTime_set(sys_logic_port_t port, uint32 currTime);

extern int32 rsd_port_errDisableCurrTime_get(sys_logic_port_t port, uint32 *pCurrTime);

extern int32 rsd_port_errDisable_clear(sys_logic_port_t port);

extern int32 rsd_port_errDisable_recover(sys_logic_port_t port);
#endif 

extern int32 rsd_port_rxEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_fiberRxEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_txEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_speedDuplex_get(sys_logic_port_t port, sys_port_speedDuplex_t *pSpeedDuplex);

extern int32 rsd_port_flowCtrl_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_flowCtrl_get(sys_logic_port_t port, sys_enable_t *pFlowCtrl);

extern int32 rsd_port_flowCtrlStatus_get(
    sys_logic_port_t port,
    sys_enable_t *pTxStatus,
    sys_enable_t *pRxStatus);

extern int32 rsd_port_autoNegoEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_autoNegoEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 rsd_port_autoNegoAbility_set(sys_logic_port_t port, sys_port_phyAbility_t *pAbility);

extern int32 rsd_port_autoNegoAbility_get(sys_logic_port_t port, sys_port_phyAbility_t *pAbility);

extern int32 rsd_port_forceModeAbility_set(
    sys_logic_port_t       port,
    sys_port_speedDuplex_t *pSpeedDuplex,
    sys_enable_t           fc);

extern int32 rsd_port_forceModeAbility_get(
    sys_logic_port_t       port,
    sys_port_speedDuplex_t *pSpeedDuplex,
    sys_enable_t           *pFc);

extern int32 rsd_port_10gMedia_set(sys_logic_port_t port, sys_port_10gMedia_t xgMedia);

extern int32 rsd_port_flowCtrlAuto_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_flowCtrlAuto_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 rsd_port_combo_1g_fiber_speed_set(sys_logic_port_t port, rtk_port_fiber_media_t gMedia);

extern int32 rsd_port_combo_1g_fiber_speed_set_new(sys_logic_port_t port, rtk_port_fiber_media_t gMedia);

extern int32 rsd_port_fiber_force_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 rsd_port_fiber_force_get(sys_logic_port_t port, sys_enable_t *pEnable);
#ifdef CONFIG_SYS_PORT_ISOLATION

extern int32 rsd_port_protected_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_protected_get(sys_logic_port_t port, sys_enable_t *pEnable);
#endif 

#ifdef CONFIG_SYS_PORT_DESCRIPTION

extern int32 rsd_port_descp_set(sys_logic_port_t port, char *pDescp);

extern int32 rsd_port_descp_get(sys_logic_port_t port, char *pDescp);
#endif 

extern int32 rsd_port_state_set(sys_port_stateCtrl_t stateCtrl);

extern int32 rsd_port_state_get(sys_port_stateCtrl_t *pStateCtrl);

#ifdef CONFIG_SYS_PROTO_LLDP

extern int32 rsd_port_macPhyInfo_get(sys_logic_port_t port, uint8 *pAutoNego, uint16 *pCapa, uint16 *pOper);
#endif 

extern int32
rsd_port_backpressureEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32
rsd_port_backpressureEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_port_logic2Physic_get(sys_logic_port_t port, sys_physic_port_t *pPhysicPort);

extern int32 rsd_port_physic2Logic_get(sys_physic_port_t physicPort, sys_logic_port_t *pPort);

extern int32 rsd_port_led_entityswCtrl_enable_get(sys_logic_port_t port, uint32 entity, sys_enable_t *pEnable);

extern int32 rsd_port_led_entityswCtrl_enable_set(sys_logic_port_t port, uint32                   entity, sys_enable_t enable);

extern int32 rsd_port_led_entityswCtrl_mode_get(sys_logic_port_t port, uint32                   entity, sys_port_media_t media, sys_led_swCtrl_mode_t *pMode);

extern int32 rsd_port_led_entityswCtrl_mode_set(sys_logic_port_t port, uint32                   entity, sys_port_media_t media, sys_led_swCtrl_mode_t mode);

extern int32 rsd_led_swCtrl_start(uint32 unit);

#endif 

