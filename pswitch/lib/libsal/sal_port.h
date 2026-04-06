
#ifndef __SAL_PORT_H__
#define __SAL_PORT_H__

#include <rsd/rsd_port.h>

typedef struct sys_port_speed_s
{
    sys_enable_t            autoSpeed;
    sys_port_speedAbility_t ability;
} sys_port_speed_t;

typedef struct sys_port_duplex_s
{
    sys_enable_t             autoDuplex;
    sys_port_duplexAbility_t ability;
} sys_port_duplex_t;

typedef enum sal_port_combo_1g_fiber_media_e
{
    PORT_COMBO_FIBER_1G_MEDIA_1000 = 0,
    PORT_COMBO_FIBER_1G_MEDIA_100,
    PORT_COMBO_FIBER_1G_MEDIA_AUTO,
    PORT_COMBO_FIBER_1G_MEDIA_END
} sal_port_combo_1g_fiber_media_t;

extern const sys_text_t text_port_speed[];
extern const sys_text_t text_port_duplex[];
extern const sys_text_t text_port_errdisable[];

extern int32 sal_port_linkChangeTime_get(sys_logic_port_t port, sys_port_linkChangeTime_t *changeTime);

extern int32 sal_port_linkStatus_get(sys_logic_port_t port, sys_port_linkStatus_t *pLink);

extern int32 sal_port_allLinkStatus_get(sys_port_allLinkStatus_t *pAllLink);

extern int32 sal_port_linkMedia_get(sys_logic_port_t port, sys_port_media_t *pMedia);

extern int32 sal_port_comboMediaPrefer_set(sys_logic_port_t port, sys_port_comboMedia_t media);

extern int32 sal_port_comboMediaPrefer_get(sys_logic_port_t port, sys_port_comboMedia_t *pMedia);

extern int32 sal_port_media_get(sys_logic_port_t port, sys_port_media_t *pMedia);

extern int32 sal_port_enable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_port_enableCfg_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);

#ifdef CONFIG_SYS_PORT_ERR_DISABLED

extern int32 sal_port_errDisable_get(sys_logic_port_t port, sys_port_errDisable_t *pReason);

extern int32 sal_port_errDisableRecovery_set(sys_port_errDisable_t reason, sys_enable_t enable);

extern int32 sal_port_errDisableRecovery_get(sys_port_errDisable_t reason, sys_enable_t *pEnable);

extern int32 sal_port_errDisableTime_set(uint32 time);

extern int32 sal_port_errDisableTime_get(uint32 *pTime);

extern int32 sal_port_errDisableCurrTime_get(sys_logic_port_t port, uint32 *pCurrTime);

extern int32 sal_port_errDisable_recover(sys_logic_port_t port);

#endif 

extern int32 sal_port_rxEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_port_speedDuplex_get(sys_logic_port_t port, sys_port_speedDuplex_t *pSpeedDuplex);

extern int32 sal_port_fiberAutoNegoEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_port_fiberAutoNegoEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_port_autoNegoEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_port_speedDuplex_set(
    sys_logic_port_t  port,
    sys_port_speed_t  speed,
    sys_port_duplex_t duplex,
    sys_port_media_t  media,
    uint32            isTrkMbr);

extern int32 sal_port_speed_set(sys_logic_port_t port, sys_port_speed_t speed);

extern int32 sal_port_speed_get(sys_logic_port_t port, sys_port_speed_t *pSpeed);

extern int32 sal_port_10gMedia_set(sys_logic_port_t port, sys_port_10gMedia_t xgMedia);

extern int32 sal_port_duplex_set(sys_logic_port_t port, sys_port_duplex_t duplex);

extern int32 sal_port_duplex_get(sys_logic_port_t port, sys_port_duplex_t *pDuplex);

extern int32 sal_port_fiberSpeed_set(sys_logic_port_t port, sys_port_speed_t speed);
extern int32 sal_port_combo_fiberSpeed_set(sys_logic_port_t port, sys_port_speed_t speed);

extern int32 sal_port_fiberSpeed_get(sys_logic_port_t port, sys_port_speed_t *pSpeed);

extern int32 sal_port_flowCtrlAuto_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_port_flowCtrlAuto_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_port_flowCtrl_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_port_flowCtrl_get(sys_logic_port_t port, sys_enable_t *pEnable, sys_enable_t *pStatus);

extern int32 sal_port_flowCtrlStatus_get(
    sys_logic_port_t port,
    sys_enable_t *pTxStatus,
    sys_enable_t *pRxStatus);

#ifdef CONFIG_SYS_PORT_ISOLATION

extern int32 sal_port_protected_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_port_protected_get(sys_logic_port_t port, sys_enable_t *pEnable);
#endif 

#ifdef CONFIG_SYS_PORT_DESCRIPTION

extern int32 sal_port_descp_set(sys_logic_port_t port, char *pDescp);

extern int32 sal_port_descp_get(sys_logic_port_t port, char *pDescp);
#endif 

extern int32 sal_port_maxL2Entry_get(uint32 unit, uint32 *pMaxL2Entry);

extern int32 sal_port_isolation_get(sys_logic_port_t port, sys_logic_portmask_t *pIsolationPortmask);

extern int32 sal_port_backpressureEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_port_backpressureEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_port_logic2Physic_get(sys_logic_port_t port, sys_physic_port_t *pPhysicPort);

extern int32 sal_port_physic2Logic_get(sys_physic_port_t physicPort, sys_logic_port_t *pPort);

#endif 
