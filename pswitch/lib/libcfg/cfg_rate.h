
#ifndef __CFG_RATE_H__
#define __CFG_RATE_H__

#include <libsal/sal_rate.h>

typedef struct cfg_rate_s
{
    
    sys_enable_t stormCtrlPortEnbl[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t stormCtrlTypeEnbl[SYS_LOGICPORT_NUM_MAX][STORM_GROUP_END];
    uint32 stormCtrlRate[SYS_LOGICPORT_NUM_MAX][STORM_GROUP_END]; 
    sys_rate_storm_action_t stormCtrlAction[SYS_LOGICPORT_NUM_MAX];

    sys_enable_t igrBwCtrlStatus[SYS_LOGICPORT_NUM_MAX];
    uint32  igrBwCtrlRate[SYS_LOGICPORT_NUM_MAX];
    uint32  igrBwCtrlBurstSize;

    sys_enable_t egrBwCtrlStatus[SYS_LOGICPORT_NUM_MAX];
    uint32  egrBwCtrlRate[SYS_LOGICPORT_NUM_MAX];
    uint32  portIgrBwCtrlBurstSize[SYS_LOGICPORT_NUM_MAX];
    uint32  portEgrBwCtrlBurstSize[SYS_LOGICPORT_NUM_MAX];
    uint32  egr_QueueCtrlStatus[SYS_LOGICPORT_NUM_MAX][CAPA_QOS_QUEUE_NUM];
    uint32  egr_QueueCtrlRate[SYS_LOGICPORT_NUM_MAX][CAPA_QOS_QUEUE_NUM];
    uint32  egr_portQueueCtrlBurstSize[SYS_LOGICPORT_NUM_MAX][CAPA_QOS_QUEUE_NUM];
    uint32  egrBwCtrlBurstSize;
    uint32  egrSysQueueCtrlBurstSize;
    sys_enable_t igrBwCtrlIncludeIFG;
    sys_enable_t egrBwCtrlIncludeIFG;
    sys_enable_t stormCtrlIncludeIFG;
    sys_rate_storm_rateMode_t stormCtrlRefreshMode;
} cfg_rate_t;

extern int32 cfg_rate_stormControlPortEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_rate_stormControlPortEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_rate_stormControlTypeEnable_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    sys_enable_t            *pEnable);

extern int32 cfg_rate_stormControlTypeEnable_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t  storm_type,
    sys_enable_t            enable);

extern int32
cfg_rate_stormControlRate_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    uint32                  *pRateValue);

extern int32
cfg_rate_stormControlRate_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    uint32                  rate);

extern int32 cfg_rate_stormControlAction_get(
    sys_logic_port_t        port,
    sys_rate_storm_action_t *pAction);

extern int32 cfg_rate_stormControlAction_set(
    sys_logic_port_t        port,
    sys_rate_storm_action_t action);

extern int32
cfg_rate_igrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32
cfg_rate_igrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32
cfg_rate_igrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRateValue);

extern int32
cfg_rate_igrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate);

extern int32
cfg_rate_igrBandwidthCtrlBurstSize_get(uint32 *pBurst_size);

extern int32
cfg_rate_igrBandwidthCtrlBurstSize_set(uint32 burst_size);

extern int32
cfg_rate_portIgrBandwidthCtrlBurstSize_get(sys_logic_port_t port, uint32 *pBurst_size);

extern int32
cfg_rate_portIgrBandwidthCtrlBurstSize_set(sys_logic_port_t port, uint32 burst_size);

extern int32
cfg_rate_flowBurst_get(uint32 *burst);

extern int32
cfg_rate_flowBurst_set(uint32 burst);

extern int32
cfg_rate_egrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32
cfg_rate_egrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32
cfg_rate_egrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRateValue);

extern int32
cfg_rate_egrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate);

extern int32
cfg_rate_egrBandwidthCtrlBurstSize_get(uint32 *pBurst_size);

extern int32
cfg_rate_egrBandwidthCtrlBurstSize_set(uint32 burst_size);

extern int32
cfg_rate_portEgrBandwidthCtrlBurstSize_get(sys_logic_port_t port,uint32 *pBurst_size);

extern int32
cfg_rate_portEgrBandwidthCtrlBurstSize_set(sys_logic_port_t port,uint32 burst_size);

extern int32
cfg_rate_egrQueueCtrlRate_get(sys_logic_port_t port,sys_qid_t queueId,uint32 *pRateValue);

extern int32
cfg_rate_egrQueueCtrlRate_set(sys_logic_port_t port,sys_qid_t queueId,uint32 rate);

extern int32
cfg_rate_egrQueueCtrlEnable_get(sys_logic_port_t port,sys_qid_t queueId,sys_enable_t *pEnable);

extern int32
cfg_rate_egrQueueCtrlEnable_set(sys_logic_port_t port,sys_qid_t queueId,sys_enable_t enable);

extern int32
cfg_rate_egrSysQueueCtrlBurstSize_get(uint32 *pBurst_size);

extern int32
cfg_rate_egrSysQueueCtrlBurstSize_set(uint32 burst_size);

extern int32
cfg_rate_portEgrQueueCtrlBurstSize_get(sys_logic_port_t port,sys_qid_t queueId,uint32 *pBurst_size);

extern int32
cfg_rate_portEgrQueueCtrlBurstSize_set(sys_logic_port_t port,sys_qid_t queueId,uint32 burst_size);

extern int32
cfg_rate_igrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include);

extern int32
cfg_rate_igrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include);

extern int32
cfg_rate_egrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include);

extern int32
cfg_rate_egrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include);

extern int32
cfg_rate_stormCtrlIncludeIfg_get(sys_enable_t *pIfg_include);

extern int32
cfg_rate_stormCtrlIncludeIfg_set(sys_enable_t ifg_include);

extern int32
cfg_rate_stormCtrlRefreshMode_get(sys_rate_storm_rateMode_t *pMode);

extern int32
cfg_rate_stormCtrlRefreshMode_set(sys_rate_storm_rateMode_t mode);

#endif 

