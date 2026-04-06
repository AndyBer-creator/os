
#include <libsal/sal_util.h>
#include <libcfg/cfg.h>
#include <libsal/sal_rate.h>

const sys_text_t text_storm_action[] =
{
    {        "Drop"      },
    {    "Shutdown"  },
};

#if defined(CONFIG_SYS_STORM_CTRL) || defined(CONFIG_SYS_BW_CTRL)

int32 sal_rate_portDflt_init(sys_rate_portDflt_t dfltInfo)
{
    sys_logic_port_t lp;
#ifdef CONFIG_SYS_BW_CTRL
    sys_qid_t qid;
#endif

    SYS_ERR_CHK(rsd_rate_portDflt_init(dfltInfo));

    FOR_EACH_LOGIC_PORT(lp)
    {
#ifdef CONFIG_SYS_STORM_CTRL

        SYS_ERR_CHK(cfg_rate_stormControlRate_set(lp, STORM_GROUP_UNICAST, dfltInfo.strmRate));
        SYS_ERR_CHK(cfg_rate_stormControlRate_set(lp, STORM_GROUP_MULTICAST, dfltInfo.strmRate));
        SYS_ERR_CHK(cfg_rate_stormControlRate_set(lp, STORM_GROUP_BROADCAST, dfltInfo.strmRate));
        SYS_ERR_CHK(cfg_rate_stormControlTypeEnable_set(lp, STORM_GROUP_UNICAST, dfltInfo.strmUucastEnbl));
        SYS_ERR_CHK(cfg_rate_stormControlTypeEnable_set(lp, STORM_GROUP_MULTICAST, dfltInfo.strmUmcastEnbl));
        SYS_ERR_CHK(cfg_rate_stormControlTypeEnable_set(lp, STORM_GROUP_BROADCAST, dfltInfo.strmBcastEnbl));

        SYS_ERR_CHK(cfg_rate_stormControlAction_set(lp, dfltInfo.strmAction));
#endif

#ifdef CONFIG_SYS_BW_CTRL
        
        SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlRate_set(lp, dfltInfo.bwIgrsRate));
        SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlEnable_set(lp, dfltInfo.bwIgrsEnbl));

        if (PORT_TYPE_100M == LP2TYPE(lp))
        {
            SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlRate_set(lp, dfltInfo.bwEgrsRateFE));
        }
        else
        {
            SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlRate_set(lp, dfltInfo.bwEgrsRateGE));
        }
        SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlEnable_set(lp, dfltInfo.bwEgrsEnbl));

        FOR_EACH_QID(qid)
        {
            if (PORT_TYPE_100M == LP2TYPE(lp))
            {
                SYS_ERR_CHK(cfg_rate_egrQueueCtrlRate_set(lp, qid, dfltInfo.bwEgrsRateFE));
            }
            else
            {
                SYS_ERR_CHK(cfg_rate_egrQueueCtrlRate_set(lp, qid, dfltInfo.bwEgrsRateGE));
            }
            SYS_ERR_CHK(cfg_rate_egrQueueCtrlEnable_set(lp, qid, dfltInfo.bwEgrsQueueEnbl));
        }
#endif
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_STORM_CTRL

int32 sal_rate_stormControlPortEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_stormControlPortEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_rate_stormControlPortEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    uint32 i;
    uint32 rate;
    sys_enable_t typeEnbl;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    if (ENABLED == enable)
    {
        for (i = 0; i < STORM_GROUP_END; i++)
        {
            SYS_ERR_CHK(cfg_rate_stormControlTypeEnable_get(port, i, &typeEnbl));

            if (ENABLED == typeEnbl)
            {
                SYS_ERR_TRK_CHK(rsd_rate_stormControlEnable_set(port, i, enable));
                SYS_ERR_CHK(cfg_rate_stormControlRate_get(port, i, &rate));
                SYS_ERR_TRK_CHK(rsd_rate_stormControlRate_set(port, i, rate));
            }
        }
    }
    else
    {
        for (i = 0; i < STORM_GROUP_END; i++)
        {
            SYS_ERR_TRK_CHK(rsd_rate_stormControlEnable_set(port, i, enable));
        }
    }

    SYS_ERR_CHK(cfg_rate_stormControlPortEnable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_rate_stormControlTypeEnable_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    sys_enable_t            *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((storm_type >= STORM_GROUP_END), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_stormControlTypeEnable_get(port, storm_type, pEnable));

    return SYS_ERR_OK;
}

int32 sal_rate_stormControlTypeEnable_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    sys_enable_t            enable)
{
    uint32 rate;
    sys_enable_t portEnbl = DISABLED;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((storm_type >= STORM_GROUP_END), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_rate_stormControlPortEnable_get(port, &portEnbl));

    if (ENABLED == portEnbl)
    {
        if (ENABLED == enable)
        {
            SYS_ERR_TRK_CHK(rsd_rate_stormControlEnable_set(port, storm_type, enable));
            SYS_ERR_CHK(cfg_rate_stormControlRate_get(port, storm_type, &rate));
            SYS_ERR_TRK_CHK(rsd_rate_stormControlRate_set(port, storm_type, rate));
        }
        else
        {
            SYS_ERR_TRK_CHK(rsd_rate_stormControlEnable_set(port, storm_type, enable));
        }
    }

    SYS_ERR_CHK(cfg_rate_stormControlTypeEnable_set(port, storm_type, enable));

    return SYS_ERR_OK;
}

int32 sal_rate_stormControlRate_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    uint32                  *pRate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((storm_type >= STORM_GROUP_END), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pRate), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_stormControlRate_get(port, storm_type, pRate));

    return SYS_ERR_OK;
} 

int32 sal_rate_stormControlRate_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t   storm_type,
    uint32                  rate)
{
    sys_enable_t portEnbl;
    sys_enable_t typeEnbl;
    sys_rate_storm_rateMode_t mode;
    uint32 rsd_rate;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((storm_type >= STORM_GROUP_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_rate_stormCtrlRefreshMode_get(&mode));
    if(BASED_ON_PKT == mode)
    {
        SYS_PARAM_CHK((rate < BOND_RATE_STORM_PPS_MIN || rate > BOND_RATE_STORM_PPS_MAX), SYS_ERR_INPUT);
    }
    else
    {
        SYS_PARAM_CHK((rate < BOND_RATE_STORM_BPS_MIN || rate > BOND_RATE_STORM_BPS_MAX), SYS_ERR_INPUT);
    }

    SYS_ERR_CHK(cfg_rate_stormControlPortEnable_get(port, &portEnbl));
    SYS_ERR_CHK(cfg_rate_stormControlTypeEnable_get(port, storm_type, &typeEnbl));

    if (ENABLED == portEnbl && ENABLED == typeEnbl)
    {
        SYS_ERR_TRK_CHK(rsd_rate_stormControlRate_set(port, storm_type, rate));

        SYS_ERR_TRK_CHK(rsd_rate_stormControlRate_get(port, storm_type, &rsd_rate));
        rate = rsd_rate;
    }
    SYS_ERR_CHK(cfg_rate_stormControlRate_set(port, storm_type, rate));

    return SYS_ERR_OK;
} 

int32
sal_rate_portStormCtrlTypeSel_get(
    sys_logic_port_t        port,
    sys_rate_storm_group_t storm_type,
    sys_rate_storm_sel_t *pStorm_sel)
{
    SYS_PARAM_CHK((NULL == pStorm_sel), SYS_ERR_NULL_POINTER);

    SYS_ERR_TRK_CHK(rsd_rate_portStormCtrlTypeSel_get(port, storm_type, pStorm_sel));

    return SYS_ERR_OK;
} 

int32
sal_rate_portStormCtrlTypeSel_set(
    sys_logic_port_t        port,
    sys_rate_storm_group_t  storm_type,
    sys_rate_storm_sel_t storm_sel)
{
    SYS_ERR_TRK_CHK(rsd_rate_portStormCtrlTypeSel_set(port, storm_type, storm_sel));

    return SYS_ERR_OK;
} 

int32 sal_rate_stormControlAction_get(sys_logic_port_t port, sys_rate_storm_action_t *pAction)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pAction), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_stormControlAction_get(port, pAction));

    return SYS_ERR_OK;
} 

int32 sal_rate_stormControlAction_set(sys_logic_port_t port, sys_rate_storm_action_t action)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((action >= STORM_ACTION_END), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_rate_stormControlAction_set(port, action));
    SYS_ERR_CHK(cfg_rate_stormControlAction_set(port, action));

    return SYS_ERR_OK;
} 

int32
sal_rate_stormCtrlIncludeIfg_get(sys_enable_t *pIfg_include)
{
    SYS_PARAM_CHK((NULL == pIfg_include), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_stormCtrlIncludeIfg_get(pIfg_include));

    return SYS_ERR_OK;
} 

int32
sal_rate_stormCtrlIncludeIfg_set(sys_enable_t ifg_include)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(ifg_include), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_rate_stormCtrlIncludeIfg_set(ifg_include));
    SYS_ERR_CHK(cfg_rate_stormCtrlIncludeIfg_set(ifg_include));

    return SYS_ERR_OK;
} 

int32
sal_rate_stormCtrlRefreshMode_get(sys_rate_storm_rateMode_t *pMode)
{
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_stormCtrlRefreshMode_get(pMode));

    return SYS_ERR_OK;
} 

int32
sal_rate_stormCtrlRefreshMode_set(sys_rate_storm_rateMode_t mode)
{
    sys_logic_port_t        port;
    sys_rate_storm_rateMode_t oldMode;
    SYS_PARAM_CHK((mode >= STORM_RATE_MODE_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_rate_stormCtrlRefreshMode_get(&oldMode));

    if (oldMode == mode)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_rate_stormCtrlRefreshMode_set(mode));

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_rate_stormControlRate_set(port,
                                                  STORM_GROUP_UNICAST,
                                                  DFLT_RATE_STORM_CTRL));
        SYS_ERR_CHK(sal_rate_stormControlRate_set(port,
                                                  STORM_GROUP_MULTICAST,
                                                  DFLT_RATE_STORM_CTRL));
        SYS_ERR_CHK(sal_rate_stormControlRate_set(port,
                                                  STORM_GROUP_BROADCAST,
                                                  DFLT_RATE_STORM_CTRL));
    }

    SYS_ERR_CHK(cfg_rate_stormCtrlRefreshMode_set(mode));

    return SYS_ERR_OK;
} 

#endif 

#ifdef CONFIG_SYS_BW_CTRL

int32
sal_rate_igrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlEnable_get(port, pEnable));

    return SYS_ERR_OK;
} 

int32
sal_rate_igrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_rate_igrBandwidthCtrlEnable_set(port, enable));
    SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlEnable_set(port, enable));

    return SYS_ERR_OK;
} 

int32
sal_rate_igrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pRate), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlRate_get(port, pRate));

    return SYS_ERR_OK;
} 

int32
sal_rate_igrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate)
{
    uint32 rsd_rate;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((rate < BOND_RATE_BANDWIDTH_MIN || rate > BOND_RATE_UNLIMITED), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_rate_igrBandwidthCtrlRate_set(port, rate));

    SYS_ERR_TRK_CHK(rsd_rate_igrBandwidthCtrlRate_get(port, &rsd_rate));
    rate = rsd_rate;

    SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlRate_set(port, rate));

    return SYS_ERR_OK;
} 

int32
sal_rate_igrBandwidthCtrlBurstSize_get(uint32 *pBurst_size)
{
    SYS_PARAM_CHK((NULL == pBurst_size), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlBurstSize_get(pBurst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_igrBandwidthCtrlBurstSize_set(uint32 burst_size)
{
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(burst_size, BOND_RATE_BANDWIDTH_BURST_MIN, BOND_RATE_BANDWIDTH_BURST_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_rate_igrBandwidthCtrlBurstSize_set(burst_size));
    SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlBurstSize_set(burst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_portIgrBandwidthCtrlBurstSize_get(sys_logic_port_t port, uint32 *pBurst_size)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pBurst_size), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_portIgrBandwidthCtrlBurstSize_get(port, pBurst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_portIgrBandwidthCtrlBurstSize_set(sys_logic_port_t port, uint32 burst_size)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(burst_size, BOND_RATE_BANDWIDTH_BURST_MIN, BOND_RATE_BANDWIDTH_BURST_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_rate_portIgrBandwidthCtrlBurstSize_set(port, burst_size));
    SYS_ERR_CHK(cfg_rate_portIgrBandwidthCtrlBurstSize_set(port, burst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_egrBandwidthCtrlEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlEnable_get(port, pEnable));

    return SYS_ERR_OK;
} 

int32
sal_rate_egrBandwidthCtrlEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_rate_egrBandwidthCtrlEnable_set(port, enable));
    SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlEnable_set(port, enable));

    return SYS_ERR_OK;
} 

int32
sal_rate_egrBandwidthCtrlRate_get(sys_logic_port_t port, uint32 *pRate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pRate), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlRate_get(port, pRate));

    return SYS_ERR_OK;
} 

int32
sal_rate_egrBandwidthCtrlRate_set(sys_logic_port_t port, uint32 rate)
{
    uint32 rsd_rate;
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((rate < BOND_RATE_BANDWIDTH_MIN || rate > BOND_RATE_UNLIMITED), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_rate_egrBandwidthCtrlRate_set(port, rate));

    SYS_ERR_TRK_CHK(rsd_rate_egrBandwidthCtrlRate_get(port, &rsd_rate));
    rate = rsd_rate;

    SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlRate_set(port, rate));

    return SYS_ERR_OK;
} 

int32
sal_rate_egrBandwidthCtrlBurstSize_get(uint32 *pBurst_size)
{
    SYS_PARAM_CHK((NULL == pBurst_size), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlBurstSize_get(pBurst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_egrBandwidthCtrlBurstSize_set(uint32 burst_size)
{
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(burst_size, BOND_RATE_BANDWIDTH_BURST_MIN, BOND_RATE_BANDWIDTH_BURST_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_rate_egrBandwidthCtrlBurstSize_set(burst_size));
    SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlBurstSize_set(burst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_portEgrBandwidthCtrlBurstSize_get(sys_logic_port_t port,uint32 *pBurst_size)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pBurst_size), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_portEgrBandwidthCtrlBurstSize_get(port,pBurst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_portEgrBandwidthCtrlBurstSize_set(sys_logic_port_t port,uint32 burst_size)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(burst_size, BOND_RATE_BANDWIDTH_BURST_MIN, BOND_RATE_BANDWIDTH_BURST_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_rate_portEgrBandwidthCtrlBurstSize_set(port,burst_size));
    SYS_ERR_CHK(cfg_rate_portEgrBandwidthCtrlBurstSize_set(port,burst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_egrQueueCtrlRate_get(sys_logic_port_t port,sys_qid_t queueId,uint32 *pRate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((queueId >= CAPA_QOS_QUEUE_NUM), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((NULL == pRate), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_egrQueueCtrlRate_get(port,queueId, pRate));

    return SYS_ERR_OK;
}

int32
sal_rate_egrQueueCtrlRate_set(sys_logic_port_t port,sys_qid_t queueId,uint32 rate)
{
    uint32 rsd_rate;
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((queueId >= CAPA_QOS_QUEUE_NUM), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((rate < BOND_RATE_BANDWIDTH_MIN || rate > BOND_RATE_UNLIMITED), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_rate_egrQueueCtrlRate_set(port,queueId,rate));

    SYS_ERR_TRK_CHK(rsd_rate_egrQueueCtrlRate_get(port, queueId, &rsd_rate));
    rate = rsd_rate;

    SYS_ERR_CHK(cfg_rate_egrQueueCtrlRate_set(port,queueId, rate));

    return SYS_ERR_OK;
}

int32
sal_rate_egrQueueCtrlEnable_get(sys_logic_port_t port,uint32 queueId,sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((queueId >= CAPA_QOS_QUEUE_NUM), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_egrQueueCtrlEnable_get(port,queueId,pEnable));

    return SYS_ERR_OK;
}

int32
sal_rate_egrQueueCtrlEnable_set(sys_logic_port_t port,uint32 queueId,sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((queueId >= CAPA_QOS_QUEUE_NUM), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_rate_egrQueueCtrlEnable_set(port,queueId,enable));
    SYS_ERR_CHK(cfg_rate_egrQueueCtrlEnable_set(port,queueId,enable));

    return SYS_ERR_OK;
}

int32
sal_rate_egrSysQueueCtrlBurstSize_get(uint32 *pBurst_size)
{
    SYS_PARAM_CHK((NULL == pBurst_size), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_egrSysQueueCtrlBurstSize_get(pBurst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_egrSysQueueCtrlBurstSize_set(uint32 burst_size)
{
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(burst_size, BOND_RATE_BANDWIDTH_BURST_MIN, BOND_RATE_BANDWIDTH_BURST_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_rate_egrSysQueueCtrlBurstSize_set(burst_size));
    SYS_ERR_CHK(cfg_rate_egrSysQueueCtrlBurstSize_set(burst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_portEgrQueueCtrlBurstSize_get(sys_logic_port_t port,uint32 queueId,uint32 *pBurst_size)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((queueId >= CAPA_QOS_QUEUE_NUM), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((NULL == pBurst_size), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_portEgrQueueCtrlBurstSize_get(port,queueId,pBurst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_portEgrQueueCtrlBurstSize_set(sys_logic_port_t port,uint32 queueId,uint32 burst_size)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((queueId >= CAPA_QOS_QUEUE_NUM), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(burst_size, BOND_RATE_BANDWIDTH_BURST_MIN, BOND_RATE_BANDWIDTH_BURST_MAX), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_TRK_CHK(rsd_rate_portEgrQueueCtrlBurstSize_set(port,queueId,burst_size));
    SYS_ERR_CHK(cfg_rate_portEgrQueueCtrlBurstSize_set(port,queueId,burst_size));

    return SYS_ERR_OK;
}

int32
sal_rate_igrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include)
{
    SYS_PARAM_CHK((NULL == pIfg_include), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlIncludeIfg_get(pIfg_include));

    return SYS_ERR_OK;
} 

int32
sal_rate_igrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(ifg_include), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_rate_igrBandwidthCtrlIncludeIfg_set(ifg_include));
    SYS_ERR_CHK(cfg_rate_igrBandwidthCtrlIncludeIfg_set(ifg_include));

    return SYS_ERR_OK;
} 

int32
sal_rate_egrBandwidthCtrlIncludeIfg_get(sys_enable_t *pIfg_include)
{
    SYS_PARAM_CHK((NULL == pIfg_include), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlIncludeIfg_get(pIfg_include));

    return SYS_ERR_OK;
} 

int32
sal_rate_egrBandwidthCtrlIncludeIfg_set(sys_enable_t ifg_include)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(ifg_include), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_rate_egrBandwidthCtrlIncludeIfg_set(ifg_include));
    SYS_ERR_CHK(cfg_rate_egrBandwidthCtrlIncludeIfg_set(ifg_include));

    return SYS_ERR_OK;
} 
#endif 

#ifdef CONFIG_SYS_DEFENCE_ENGINE

int32
sal_rate_cpuEgrBandwidthCtrl_set(sys_enable_t enable, uint32 rate)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK((rate < BOND_RATE_BANDWIDTH_MIN || rate > BOND_RATE_UNLIMITED), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(rsd_rate_cpuEgrBandwidthCtrl_set(enable, rate));

    return SYS_ERR_OK;
}

int32
sal_rate_cpuEgrQueueCtrlEnable_set(sys_qid_t queueId, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK((queueId >= CAPA_QOS_QUEUE_NUM), SYS_ERR_QOS_QUEUE_INVALID);

    SYS_ERR_CHK(rsd_rate_cpuEgrQueueCtrlEnable_set(queueId, enable));

    return SYS_ERR_OK;
}

int32
sal_rate_cpuEgrBandwidthCtrlRateMode_get(sys_rate_rateMode_t *pRate_mode)
{
    sys_rate_rateMode_t rate_mode;

    SYS_PARAM_CHK((NULL == pRate_mode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_rate_cpuEgrBandwidthCtrlRateMode_get(&rate_mode));
    *pRate_mode = rate_mode;

    return SYS_ERR_OK;
}

#endif 

