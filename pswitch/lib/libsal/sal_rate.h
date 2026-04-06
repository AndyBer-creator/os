
#ifndef __SAL_RATE_H__
#define __SAL_RATE_H__

#include <rsd/rsd_rate.h>
#include <libsal/sal_type.h>

extern const sys_text_t text_storm_action[];

#if defined(CONFIG_SYS_STORM_CTRL) || defined(CONFIG_SYS_BW_CTRL)

extern int32 sal_rate_portDflt_init(sys_rate_portDflt_t dfltInfo);
#endif

#ifdef CONFIG_SYS_STORM_CTRL

extern int32 sal_rate_stormControlPortEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_rate_stormControlPortEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_rate_stormControlTypeEnable_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    sys_enable_t            *pEnable);

extern int32 sal_rate_stormControlTypeEnable_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    sys_enable_t            enable);

extern int32
sal_rate_stormControlRate_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    uint32                  *pRate);

extern int32
sal_rate_stormControlRate_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    uint32                  rate);

extern int32 sal_rate_portStormCtrlTypeSel_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t storm_type,
    sys_rate_storm_sel_t *pStorm_sel);

extern int32 sal_rate_portStormCtrlTypeSel_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t  storm_type,
    sys_rate_storm_sel_t storm_sel);

extern int32
sal_rate_stormControlAction_get(sys_logic_port_t port, sys_rate_storm_action_t *pAction);

extern int32
sal_rate_stormControlAction_set(sys_logic_port_t port, sys_rate_storm_action_t action);

extern int32
sal_rate_stormCtrlIncludeIfg_get(sys_enable_t *pIfg_include);

extern int32
sal_rate_stormCtrlIncludeIfg_set(sys_enable_t ifg_include);

extern int32
sal_rate_stormCtrlRefreshMode_get(sys_rate_storm_rateMode_t *pMode);

extern int32
sal_rate_stormCtrlRefreshMode_set(sys_rate_storm_rateMode_t mode);

#endif 

#ifdef CONFIG_SYS_BW_CTRL

extern int32
sal_rate_igrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32
sal_rate_igrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32
sal_rate_igrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRate);

extern int32
sal_rate_igrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate);

extern int32
sal_rate_portIgrBandwidthCtrlBurstSize_get(sys_logic_port_t port, uint32 *pBurst_size);

extern int32
sal_rate_igrBandwidthCtrlBurstSize_get(uint32 *pBurst_size);

extern int32
sal_rate_igrBandwidthCtrlBurstSize_set(uint32 burst_size);

extern int32
sal_rate_portIgrBandwidthCtrlBurstSize_set(sys_logic_port_t port, uint32 burst_size);

extern int32
sal_rate_egrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32
sal_rate_egrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32
sal_rate_egrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRate);

extern int32
sal_rate_egrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate);

extern int32
sal_rate_egrBandwidthCtrlBurstSize_get(uint32 *pBurst_size);

extern int32
sal_rate_egrBandwidthCtrlBurstSize_set(uint32 burst_size);

extern int32
sal_rate_portEgrBandwidthCtrlBurstSize_get(sys_logic_port_t port,uint32 *pBurst_size);

extern int32
sal_rate_portEgrBandwidthCtrlBurstSize_set(sys_logic_port_t port,uint32 burst_size);

extern int32
sal_rate_egrQueueCtrlRate_get(sys_logic_port_t port,uint32 queueId,uint32 *pRate);

extern int32
sal_rate_egrQueueCtrlRate_set(sys_logic_port_t port,uint32 queueId,uint32 rate);

extern int32
sal_rate_egrQueueCtrlEnable_get(sys_logic_port_t port,uint32 queueId,sys_enable_t *pEnable);

extern int32
sal_rate_egrQueueCtrlEnable_set(sys_logic_port_t port,uint32 queueId,sys_enable_t enable);

extern int32
sal_rate_egrSysQueueCtrlBurstSize_get(uint32 *pBurst_size);

extern int32
sal_rate_egrSysQueueCtrlBurstSize_set(uint32 burst_size);

extern int32
sal_rate_portEgrQueueCtrlBurstSize_get(sys_logic_port_t port,uint32 queueId,uint32 *pBurst_size);

extern int32
sal_rate_portEgrQueueCtrlBurstSize_set(sys_logic_port_t port,uint32 queueId,uint32 burst_size);

extern int32
sal_rate_igrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include);

extern int32
sal_rate_igrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include);

extern int32
sal_rate_egrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include);

extern int32
sal_rate_egrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include);

extern int32
sal_rate_flowBurst_get(uint32 *burst);

extern int32
sal_rate_flowBurst_set(uint32 burst);
#endif 

#ifdef CONFIG_SYS_DEFENCE_ENGINE

extern int32
sal_rate_cpuEgrBandwidthCtrl_set(sys_enable_t enable, uint32 rate);

extern int32
sal_rate_cpuEgrQueueCtrlEnable_set(sys_qid_t queueId, sys_enable_t enable);

extern int32
sal_rate_cpuEgrBandwidthCtrlRateMode_get(sys_rate_rateMode_t *pRate_mode);

#endif 
#endif 

