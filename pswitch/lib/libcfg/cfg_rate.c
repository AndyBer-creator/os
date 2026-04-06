
#include <libcfg/cfg.h>

int32 cfg_rate_stormControlPortEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pEnable = pRate->stormCtrlPortEnbl[port];

    CFG_CLOSE(CFG_ID_RATE);
}

int32 cfg_rate_stormControlPortEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->stormCtrlPortEnbl[port] = enable;

    CFG_CLOSE(CFG_ID_RATE);
}

int32 cfg_rate_stormControlTypeEnable_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t  storm_type,
    sys_enable_t            *pEnable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pEnable = pRate->stormCtrlTypeEnbl[port][storm_type];

    CFG_CLOSE(CFG_ID_RATE);
}

int32 cfg_rate_stormControlTypeEnable_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    sys_enable_t            enable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->stormCtrlTypeEnbl[port][storm_type] = enable;

    CFG_CLOSE(CFG_ID_RATE);
}

int32 cfg_rate_stormControlRate_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    uint32                  *pRateValue)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pRateValue = pRate->stormCtrlRate[port][storm_type];

    CFG_CLOSE(CFG_ID_RATE);
} 

int32 cfg_rate_stormControlRate_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    uint32                  rate)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->stormCtrlRate[port][storm_type] = rate;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32 cfg_rate_stormControlAction_get(
    sys_logic_port_t        port,
    sys_rate_storm_action_t *pAction)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pAction = pRate->stormCtrlAction[port];

    CFG_CLOSE(CFG_ID_RATE);
} 

int32 cfg_rate_stormControlAction_set(
    sys_logic_port_t        port,
    sys_rate_storm_action_t action)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->stormCtrlAction[port] = action;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_igrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pEnable = pRate->igrBwCtrlStatus[port];

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_igrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->igrBwCtrlStatus[port] = enable;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_igrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRateValue)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pRateValue = pRate->igrBwCtrlRate[port];

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_igrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->igrBwCtrlRate[port] = rate;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_igrBandwidthCtrlBurstSize_get(uint32 *pBurst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pBurst_size = pRate->igrBwCtrlBurstSize;

    CFG_CLOSE(CFG_ID_RATE);

}

int32
cfg_rate_igrBandwidthCtrlBurstSize_set(uint32 burst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->igrBwCtrlBurstSize = burst_size;

    CFG_CLOSE(CFG_ID_RATE);
}

int32
cfg_rate_portIgrBandwidthCtrlBurstSize_get(sys_logic_port_t port, uint32 *pBurst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pBurst_size = pRate->portIgrBwCtrlBurstSize[port];

    CFG_CLOSE(CFG_ID_RATE);

}

int32
cfg_rate_portIgrBandwidthCtrlBurstSize_set(sys_logic_port_t port, uint32 burst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->portIgrBwCtrlBurstSize[port]= burst_size;

    CFG_CLOSE(CFG_ID_RATE);
}

int32
cfg_rate_egrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pEnable = pRate->egrBwCtrlStatus[port];

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_egrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->egrBwCtrlStatus[port] = enable;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_egrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRateValue)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pRateValue = pRate->egrBwCtrlRate[port];

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_egrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->egrBwCtrlRate[port] = rate;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_egrBandwidthCtrlBurstSize_get(uint32 *pBurst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pBurst_size = pRate->egrBwCtrlBurstSize;

    CFG_CLOSE(CFG_ID_RATE);

}

int32
cfg_rate_egrBandwidthCtrlBurstSize_set(uint32 burst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->egrBwCtrlBurstSize = burst_size;

    CFG_CLOSE(CFG_ID_RATE);
}

int32
cfg_rate_portEgrBandwidthCtrlBurstSize_get(sys_logic_port_t port,uint32 *pBurst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pBurst_size = pRate->portEgrBwCtrlBurstSize[port];

    CFG_CLOSE(CFG_ID_RATE);

}

int32
cfg_rate_portEgrBandwidthCtrlBurstSize_set(sys_logic_port_t port,uint32 burst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->portEgrBwCtrlBurstSize[port]= burst_size;

    CFG_CLOSE(CFG_ID_RATE);

}

int32
cfg_rate_egrQueueCtrlRate_get(sys_logic_port_t port,sys_qid_t queueId,uint32 *pRateValue)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pRateValue = pRate->egr_QueueCtrlRate[port][queueId];

    CFG_CLOSE(CFG_ID_RATE);

}

int32
cfg_rate_egrQueueCtrlRate_set(sys_logic_port_t port,sys_qid_t queueId,uint32 rate)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->egr_QueueCtrlRate[port][queueId] = rate;

    CFG_CLOSE(CFG_ID_RATE);

}

int32
cfg_rate_egrQueueCtrlEnable_get(sys_logic_port_t port,sys_qid_t queueId,sys_enable_t *pEnable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pEnable= pRate->egr_QueueCtrlStatus[port][queueId];

    CFG_CLOSE(CFG_ID_RATE);

}

int32
cfg_rate_egrQueueCtrlEnable_set(sys_logic_port_t port,sys_qid_t queueId,sys_enable_t enable)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->egr_QueueCtrlStatus[port][queueId] = enable;

    CFG_CLOSE(CFG_ID_RATE);
}

int32
cfg_rate_egrSysQueueCtrlBurstSize_get(uint32 *pBurst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pBurst_size = pRate->egrSysQueueCtrlBurstSize;

    CFG_CLOSE(CFG_ID_RATE);
}

int32
cfg_rate_egrSysQueueCtrlBurstSize_set(uint32 burst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->egrSysQueueCtrlBurstSize = burst_size;

    CFG_CLOSE(CFG_ID_RATE);
}

int32
cfg_rate_portEgrQueueCtrlBurstSize_get(sys_logic_port_t port,sys_qid_t queueId,uint32 *pBurst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pBurst_size = pRate->egr_portQueueCtrlBurstSize[port][queueId];

    CFG_CLOSE(CFG_ID_RATE);
}

int32
cfg_rate_portEgrQueueCtrlBurstSize_set(sys_logic_port_t port,sys_qid_t queueId,uint32 burst_size)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->egr_portQueueCtrlBurstSize[port][queueId] = burst_size;

    CFG_CLOSE(CFG_ID_RATE);
}

int32
cfg_rate_igrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pIfg_include = pRate->igrBwCtrlIncludeIFG;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_igrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->igrBwCtrlIncludeIFG = ifg_include;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_egrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pIfg_include = pRate->egrBwCtrlIncludeIFG;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_egrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->egrBwCtrlIncludeIFG = ifg_include;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_stormCtrlIncludeIfg_get(sys_enable_t *pIfg_include)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pIfg_include = pRate->stormCtrlIncludeIFG;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_stormCtrlIncludeIfg_set(sys_enable_t ifg_include)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->stormCtrlIncludeIFG = ifg_include;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_stormCtrlRefreshMode_get(sys_rate_storm_rateMode_t *pMode)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    *pMode = pRate->stormCtrlRefreshMode;

    CFG_CLOSE(CFG_ID_RATE);
} 

int32
cfg_rate_stormCtrlRefreshMode_set(sys_rate_storm_rateMode_t mode)
{
    cfg_rate_t *pRate = NULL;

    CFG_OPEN(CFG_ID_RATE, (cfg_addr)&pRate);

    pRate->stormCtrlRefreshMode = mode;

    CFG_CLOSE(CFG_ID_RATE);
} 

