
#ifndef __RSD_RATE_H__
#define __RSD_RATE_H__

#include <rtk/rate.h>
#include <common/sys_def.h>
#include <rsd/rsd_qos.h>

#define SYS_RATE_STORM_BURST        1700
#define DFLT_FWD_CPU_EGR_PPS_RATE   1024  
#define SYS_RATE_PORT_EGR_FE        (0x186a * 16) 
#define SYS_RATE_PORT_EGR_GE        (0xF424 * 16) 

#define SYS_RATE_STORM_PROTO        10

#ifdef CONFIG_SYS_STORM_CTRL
#define STORM_PROC_NAME    "storm"
#define STORM_PROC_SEQNAME storm
#define STORM_PROC_BUFLEN  4096
#endif

typedef enum sys_rate_storm_action_e
{
    STORM_ACTION_NONE,
    STORM_ACTION_SHUTDOWN,
    STORM_ACTION_END
} sys_rate_storm_action_t;

typedef rtk_rate_storm_group_t sys_rate_storm_group_t;
typedef rtk_rate_storm_sel_t sys_rate_storm_sel_t;
typedef rtk_rate_storm_rateMode_t sys_rate_storm_rateMode_t;
typedef rtk_rate_rateMode_t sys_rate_rateMode_t;
typedef rtk_rate_storm_proto_group_t sys_rate_proto_type_t;

#if defined(CONFIG_SYS_STORM_CTRL) || defined(CONFIG_SYS_BW_CTRL)
typedef struct sys_rate_portDflt_s
{
#ifdef CONFIG_SYS_STORM_CTRL
    uint32                  strmRate;
    sys_enable_t            strmEnbl;
    sys_enable_t            strmUucastEnbl;
    sys_enable_t            strmUmcastEnbl;
    sys_enable_t            strmMcastEnbl;
    sys_enable_t            strmBcastEnbl;
    sys_rate_storm_action_t strmAction;
#endif
#ifdef CONFIG_SYS_BW_CTRL
    uint32       bwIgrsRate;
    sys_enable_t bwIgrsEnbl;
    uint32       bwEgrsRateFE;
    uint32       bwEgrsRateGE;
    sys_enable_t bwEgrsEnbl;
    sys_enable_t bwEgrsQueueEnbl;
#endif
} sys_rate_portDflt_t;
#endif

#define RATE_ROUND16(rate)  ((((rate) > 0) && ((rate) < 16)) ? (1) : ((((rate) % 16) > 7) ? ((rate) / 16 + 1) : ((rate) / 16)))

extern int32 rsd_rate_init(void);

#ifdef CONFIG_SYS_STORM_CTRL

extern int32 rsd_rate_stormProc_dump(char *buf, int32 buflen, int32 *pLen);
#endif

#if defined(CONFIG_SYS_STORM_CTRL) || defined(CONFIG_SYS_BW_CTRL)

extern int32 rsd_rate_portDflt_init(sys_rate_portDflt_t dfltInfo);
#endif

#ifdef CONFIG_SYS_STORM_CTRL

extern int32 rsd_rate_stormControlEnable_get(sys_logic_port_t port, sys_rate_storm_group_t storm_type, sys_enable_t *pEnable);

extern int32 rsd_rate_stormControlEnable_set(sys_logic_port_t port, sys_rate_storm_group_t storm_type, sys_enable_t enable);

extern int32 rsd_rate_portStormCtrlTypeSel_get(sys_logic_port_t port, sys_rate_storm_group_t storm_type, sys_rate_storm_sel_t *pStorm_sel);

extern int32 rsd_rate_portStormCtrlTypeSel_set(sys_logic_port_t port, sys_rate_storm_group_t storm_type, sys_rate_storm_sel_t storm_sel);

extern int32 rsd_rate_stormControlRate_get(sys_logic_port_t port, sys_rate_storm_group_t storm_type, uint32 *pRate);

extern int32 rsd_rate_stormControlRate_set(sys_logic_port_t port, sys_rate_storm_group_t storm_type, uint32 rate);

extern int32 rsd_rate_stormControlAction_get(sys_logic_port_t port, sys_rate_storm_action_t *pAction);

extern int32 rsd_rate_stormControlAction_set(sys_logic_port_t port, sys_rate_storm_action_t action);

extern int32 rsd_rate_stormCtrlIncludeIfg_get(sys_enable_t *pIfg_include);

extern int32 rsd_rate_stormCtrlIncludeIfg_set(sys_enable_t ifg_include);

extern int32 rsd_rate_stormCtrlRefreshMode_get(sys_rate_storm_rateMode_t *pMode);

extern int32 rsd_rate_stormCtrlRefreshMode_set(sys_rate_storm_rateMode_t mode);

#endif 

#ifdef CONFIG_SYS_BW_CTRL

extern int32
rsd_rate_igrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32
rsd_rate_igrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32
rsd_rate_igrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRate);

extern int32
rsd_rate_igrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate);

extern int32
rsd_rate_igrBandwidthCtrlBurstSize_get(uint32 *pBurstSize);

extern int32
rsd_rate_igrBandwidthCtrlBurstSize_set(uint32 burstSize);

extern int32
rsd_rate_portIgrBandwidthCtrlBurstSize_get(sys_logic_port_t port,uint32 *pBurstSize);

extern int32
rsd_rate_portIgrBandwidthCtrlBurstSize_set(sys_logic_port_t port,uint32 burstSize);

extern int32
rsd_rate_egrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32
rsd_rate_egrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32
rsd_rate_egrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRate);

extern int32
rsd_rate_egrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate);

extern int32
rsd_rate_egrBandwidthCtrlBurstSize_get(uint32 *pBurstSize);

extern int32
rsd_rate_egrBandwidthCtrlBurstSize_set(uint32 burstSize);

extern int32
rsd_rate_portEgrBandwidthCtrlBurstSize_get(sys_logic_port_t port,uint32 *pBurstSize);

extern int32
rsd_rate_portEgrBandwidthCtrlBurstSize_set(sys_logic_port_t port,uint32 burstSize);

extern int32
rsd_rate_egrQueueCtrlRate_get(sys_logic_port_t port,sys_qid_t queueId,uint32 *pRate);

extern int32
rsd_rate_egrQueueCtrlRate_set(sys_logic_port_t port,sys_qid_t queueId,uint32 rate);

extern int32
rsd_rate_egrQueueCtrlEnable_get(sys_logic_port_t port,sys_qid_t queueId,sys_enable_t *pEnable);

extern int32
rsd_rate_egrQueueCtrlEnable_set(sys_logic_port_t port,sys_qid_t queueId,sys_enable_t enable);

extern int32
rsd_rate_egrSysQueueCtrlBurstSize_get(uint32 *pBurstSize);

extern int32
rsd_rate_egrSysQueueCtrlBurstSize_set(uint32 burstSize);

extern int32
rsd_rate_portEgrQueueCtrlBurstSize_get(sys_logic_port_t port,sys_qid_t queueId,uint32 *pBurstSize);

extern int32
rsd_rate_portEgrQueueCtrlBurstSize_set(sys_logic_port_t port,sys_qid_t queueId,uint32 burstSize);

extern int32
rsd_rate_igrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include);

extern int32
rsd_rate_igrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include);

extern int32
rsd_rate_egrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include);

extern int32
rsd_rate_egrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include);

extern int32
rsd_rate_flowBurst_set(uint32 burst);
#endif

#ifdef CONFIG_SYS_DEFENCE_ENGINE

extern int32
rsd_rate_cpuEgrBandwidthCtrl_set(sys_enable_t enable, uint32 rate);

extern int32
rsd_rate_cpuEgrQueueCtrlEnable_set(sys_qid_t queueId, sys_enable_t enable);

extern int32
rsd_rate_cpuEgrBandwidthCtrlRateMode_get(sys_rate_rateMode_t *pRate_mode);

extern int32
rsd_rate_cpuEgrBandwidthCtrlRateMode_set(sys_rate_rateMode_t rate_mode);

#endif 
#endif 

