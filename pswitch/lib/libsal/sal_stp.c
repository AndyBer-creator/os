
#include <libsal/sal_util.h>
#include <libsal/sal_stp.h>
#ifdef CONFIG_SYS_PROTO_LLDP
#include <libsal/sal_lldp.h>
#endif

const sys_text_t text_stp_forceversion[] =
{
    {     "STP-Compatible"    },
    {     ""    },
    {     "RSTP-Compatible"    },
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    {     "MSTP-Operation"    },
#endif
};

const sys_text_t text_stp_conf[] =
{
    {        "False"     },
    {         "True"      },
    {         "Auto"      },
};

int32 sal_stp_enable_set(sys_enable_t enable)
{
    sys_enable_t ori_enable = DISABLED;
    sys_enable_t fwd_enable = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_stp_enable_get(&ori_enable));

    if (ori_enable != enable)
    {
        SYS_ERR_CHK(stp_enable_set(enable));

        if (ENABLED == enable)
        {
            SYS_LOG(LOG_CAT_STP, LOG_STP_ENABLE);
        }
        else
        {
            SYS_ERR_CHK(sal_stp_forwardBpdu_get(&fwd_enable));
            SYS_LOG(LOG_CAT_STP, LOG_STP_DISABLE, ENABLED == fwd_enable ? "Flooding" : "Filtering");
        }
    }

    return SYS_ERR_OK;
}

int32 sal_stp_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_stp_forwardBpdu_set(sys_enable_t enable)
{
    sys_enable_t stp_enable;
    sys_enable_t ori_enable;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_stp_enable_get(&stp_enable));
    SYS_ERR_CHK(sal_stp_forwardBpdu_get(&ori_enable));

    SYS_ERR_CHK(stp_forwardBpdu_set(enable));

    if (DISABLED == stp_enable && ori_enable != enable)
        SYS_LOG(LOG_CAT_STP, LOG_STP_DISABLE, ENABLED == enable ? "Flooding" : "Filtering");

    return SYS_ERR_OK;
}

int32 sal_stp_forwardBpdu_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_forwardBpdu_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_stp_forceVersion_set(uint32 forceVersion)
{
#if defined(CONFIG_SYS_PROTO_LLDP) && defined(CONFIG_SYS_PROTO_STP_MSTP)
    lldp_disable_action_t action;
    sys_enable_t enable;

    SYS_ERR_CHK(sal_lldp_enable_get(&enable));
    SYS_ERR_CHK(sal_lldp_disableAction_get(&action));

    if (DISABLED == enable && MSTP_BPDU_VERSION_MSTP == forceVersion && LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS == action)
        return SYS_ERR_STP_MSTP_LLDP_FLOODING;
#endif

    SYS_ERR_CHK(stp_forceVersion_set(forceVersion));

    SYS_ERR_CHK(cfg_stp_forceVersion_set(forceVersion));

    switch (forceVersion)
    {
        case MSTP_BPDU_VERSION_STP:
            SYS_LOG(LOG_CAT_STP, LOG_STP_STP);
            break;

        case MSTP_BPDU_VERSION_RSTP:
            SYS_LOG(LOG_CAT_STP, LOG_STP_RSTP);
            break;

#ifdef CONFIG_SYS_PROTO_STP_MSTP
        case MSTP_BPDU_VERSION_MSTP:
            SYS_LOG(LOG_CAT_STP, LOG_STP_MSTP);
            break;
#endif

        default:
            break;
    }

    return SYS_ERR_OK;
}

int32 sal_stp_forceVersion_get(uint32 *pForceVersion)
{
    SYS_PARAM_CHK((NULL == pForceVersion), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_forceVersion_get(pForceVersion));

    return SYS_ERR_OK;
}

int32 sal_stp_helloTime_set(uint32 helloTime)
{
    SYS_ERR_CHK(stp_helloTime_set(helloTime));
    SYS_ERR_CHK(cfg_stp_helloTime_set(helloTime));

    return SYS_ERR_OK;
}

int32 sal_stp_helloTime_get(uint32 *pHelloTime)
{
    SYS_PARAM_CHK((NULL == pHelloTime), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_helloTime_get(pHelloTime));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP

int32 sal_stp_maxHops_set(uint32 maxHops)
{
    SYS_ERR_CHK(stp_maxHops_set(maxHops));
    SYS_ERR_CHK(cfg_stp_maxHops_set(maxHops));

    return SYS_ERR_OK;
}

int32 sal_stp_maxHops_get(uint32 *pMaxHops)
{
    SYS_PARAM_CHK((NULL == pMaxHops), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_maxHops_get(pMaxHops));

    return SYS_ERR_OK;
}
#endif

int32 sal_stp_forwardDelay_set(uint32 forwardDelay)
{
    SYS_ERR_CHK(stp_forwardDelay_set(forwardDelay));
    SYS_ERR_CHK(cfg_stp_forwardDelay_set(forwardDelay));

    return SYS_ERR_OK;
}

int32 sal_stp_forwardDelay_get(uint32 *pForwardDelay)
{
    SYS_PARAM_CHK((NULL == pForwardDelay), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_forwardDelay_get(pForwardDelay));

    return SYS_ERR_OK;
}

int32 sal_stp_maximumAge_set(uint32 maximumAge)
{
    SYS_ERR_CHK(stp_maximumAge_set(maximumAge));
    SYS_ERR_CHK(cfg_stp_maximumAge_set(maximumAge));

    return SYS_ERR_OK;
}

int32 sal_stp_maximumAge_get(uint32 *pMaximumAge)
{
    SYS_PARAM_CHK((NULL == pMaximumAge), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_maximumAge_get(pMaximumAge));

    return SYS_ERR_OK;
}

int32 sal_stp_timer_set(uint32 helloTime,uint32 forwardDelay,int32 maxAge)
{
    if(!((2*(forwardDelay -1) >= maxAge) && \
        (maxAge >= 2*(helloTime + 1))))
        return SYS_ERR_STP_TIMER_RELATIONSHIPS;

    SYS_ERR_CHK(stp_helloTime_set(helloTime));
    SYS_ERR_CHK(cfg_stp_helloTime_set(helloTime));

    SYS_ERR_CHK(stp_forwardDelay_set(forwardDelay));
    SYS_ERR_CHK(cfg_stp_forwardDelay_set(forwardDelay));

    SYS_ERR_CHK(stp_maximumAge_set(maxAge));
    SYS_ERR_CHK(cfg_stp_maximumAge_set(maxAge));

    return SYS_ERR_OK;
}

static int32 _sal_stp_timer_sequence_generate(uint32 forwardDelay,uint32 maxAge,uint32 helloTime,uint32 seqFlag,uint32 seq[STP_TIMER_SEQUENCE_NUM])
{
    uint32 curHelloTime = 0;
    uint32 curMaxAge = 0;
    uint32 curForwardDelay = 0;
    uint32 a = 0,b= 0,c =0;
    uint32 x = 0,y= 0,z= 0;

    if(0 == seqFlag)
    {
        SYS_ERR_CHK(sal_stp_forwardDelay_get(&curForwardDelay));
        SYS_ERR_CHK(sal_stp_maximumAge_get(&curMaxAge));
        SYS_ERR_CHK(sal_stp_helloTime_get(&curHelloTime));
        a = 2*(curForwardDelay - 1);
        b = curMaxAge;
        c = 2*(curHelloTime +1);
    }
    else
    {
        a = 2*(DFLT_STP_FORWARD_DELAY - 1);
        b = DFLT_STP_MAX_AGE;
        c = 2*(DFLT_STP_HELLO_TIME +1);
    }

    x = 2*(forwardDelay - 1);
    y = maxAge;
    z = 2*(helloTime +1);

    if (x >= b)
    {
        if (y >= c)
        {
            seq[1] = STP_MAX_AGE;
            seq[2] = STP_HELLO_TIME;
        }
        else if (b >= z)
        {
            seq[1] = STP_HELLO_TIME;
            seq[2] = STP_MAX_AGE;
        }
        else
        {
            return SYS_ERR_FAILED;
        }

        seq[0] = STP_FORWARD_DELAY;
    }
    else if ((a>=y) && (y>=c))
    {
        seq[0] = STP_MAX_AGE;
        seq[1] = STP_FORWARD_DELAY;
        seq[2] = STP_HELLO_TIME;
    }
    else if((a>=b) && (b>=z))
    {
        if (a >= y)
        {
            seq[1] = STP_MAX_AGE;
            seq[2] = STP_FORWARD_DELAY;
        }
        else if ( x >= b)
        {
            seq[1] = STP_FORWARD_DELAY;
            seq[2] = STP_MAX_AGE;
        }
        else
        {
            return SYS_ERR_FAILED;
        }
        seq[0] = STP_HELLO_TIME;
    }
    else
    {
        return  SYS_ERR_FAILED;
    }

    return  SYS_ERR_OK;
}

int32 sal_stp_timer_sequence_generate(uint32 forwardDelay,uint32 maxAge,uint32 helloTime,uint32 sequence[STP_TIMER_SEQUENCE_NUM])
{
    uint32 i;
    int32 ret = -1;
    uint32 saveSeq[STP_TIMER_SEQUENCE_NUM];

    ret = _sal_stp_timer_sequence_generate(forwardDelay,maxAge,helloTime,0,sequence);

    if(SYS_ERR_OK != ret)
        return SYS_ERR_FAILED;

    _sal_stp_timer_sequence_generate(forwardDelay,maxAge,helloTime,1,saveSeq);

    for(i = 0; i < STP_TIMER_SEQUENCE_NUM;i++)
        cfg_stp_timer_sequence_set(i,saveSeq[i]);

    return SYS_ERR_OK;
}

int32 sal_stp_timer_sequence_get(uint32 sequence[STP_TIMER_SEQUENCE_NUM])
{
    uint32 i = 0;
    uint32 timerSeq;

    for(i = 0; i < STP_TIMER_SEQUENCE_NUM;i++)
    {
        cfg_stp_timer_sequence_get(i,&timerSeq);
        sequence[i] = timerSeq;
    }

    return SYS_ERR_OK;
}

int32 sal_stp_txHoldCount_set(uint32 txHoldCount)
{
    SYS_ERR_CHK(stp_txHoldCount_set(txHoldCount));
    SYS_ERR_CHK(cfg_stp_txHoldCount_set(txHoldCount));

    return SYS_ERR_OK;
}

int32 sal_stp_txHoldCount_get(uint32 *pTxHoldCount)
{
    SYS_PARAM_CHK((NULL == pTxHoldCount), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_txHoldCount_get(pTxHoldCount));

    return SYS_ERR_OK;
}

int32 sal_stp_pathcostMethod_set(uint32 method)
{
    uint32 origMethod;
    uint32 msti;
    sys_logic_port_t port;

    SYS_ERR_CHK(stp_pathcostMethod_get(&origMethod));
    if (origMethod == method)
    {
        return SYS_ERR_OK;
    }

    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CONTINUE(stp_portPathCost_set(port, 0));
    }

    for (msti = 1; msti < MSTP_INSTANCE_NUM; msti++)
    {
        FOR_EACH_LOGIC_PORT(port)
        {
            SYS_ERR_CONTINUE(stp_mstPortPathCost_set(msti, port, 0));
        }
    }

    SYS_ERR_CHK(stp_pathcostMethod_set(method));
    SYS_ERR_CHK(cfg_stp_pathcostMethod_set(method));

    return SYS_ERR_OK;
}

int32 sal_stp_pathcostMethod_get(uint32 *pMethod)
{
    SYS_PARAM_CHK((NULL == pMethod), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_pathcostMethod_get(pMethod));

    return SYS_ERR_OK;
}

int32 sal_stp_portAdminEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    sys_enable_t fwd_enable = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_portAdminEnable_set(port, enable));
    SYS_ERR_CHK(cfg_stp_portAdminEnable_set(port, enable));

    if (ENABLED == enable)
    {
        SYS_LOG(LOG_CAT_STP, LOG_STP_PORT_ENABLE, LPSTR(port));
    }
    else
    {
        SYS_ERR_CHK(sal_stp_portForwardBpdu_get(port, &fwd_enable));
        SYS_LOG(LOG_CAT_STP, LOG_STP_PORT_DISABLE,
                LPSTR(port), ENABLED == fwd_enable ? "Flooding" : "Filtering");
    }

    return SYS_ERR_OK;
}

int32 sal_stp_priority_set(uint32 priority)
{
    SYS_ERR_CHK(stp_mstPriority_set(0, priority));
    SYS_ERR_CHK(cfg_stp_priority_set(priority));

    return SYS_ERR_OK;
}

int32 sal_stp_priority_get(uint32 *pPriority)
{
    SYS_PARAM_CHK((NULL == pPriority), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_priority_get(pPriority));

    return SYS_ERR_OK;
}

int32 sal_stp_portAdminEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_portAdminEnable_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_stp_portForwardBpdu_set(sys_logic_port_t port, sys_enable_t enable)
{
    sys_enable_t stp_enable;
    sys_enable_t ori_enable;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(sal_stp_portAdminEnable_get(port, &stp_enable));
    SYS_ERR_CHK(sal_stp_portForwardBpdu_get(port, &ori_enable));

    SYS_ERR_CHK(stp_portForwardBpdu_set(port, enable));
    SYS_ERR_CHK(cfg_stp_portForwardBpdu_set(port, enable));

    if (DISABLED == stp_enable && ori_enable != enable)
        SYS_LOG(LOG_CAT_STP, LOG_STP_PORT_DISABLE, ENABLED == enable ? "Flooding" : "Filtering");

    return SYS_ERR_OK;
}

int32 sal_stp_portForwardBpdu_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_portForwardBpdu_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_stp_portPriority_set(sys_logic_port_t port, uint32 priority)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_mstPortPriority_set(0, port, priority));
    SYS_ERR_CHK(cfg_stp_portPriority_set(port, priority));

    return SYS_ERR_OK;
}

int32 sal_stp_portPriority_get(sys_logic_port_t port, uint32 *pPriority)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPriority), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_portPriority_get(port, pPriority));

    return SYS_ERR_OK;
}

int32 sal_stp_portPathCost_set(sys_logic_port_t port, uint32 pathCost)
{
    uint32 method;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_pathcostMethod_get(&method));
    if ((STP_PATHCOST_METHOD_SHORT == method) && (pathCost > 65535))
    {
        return SYS_ERR_STP_COST_SHORT_RANGE;
    }

    SYS_ERR_CHK(stp_portPathCost_set(port, pathCost));
    SYS_ERR_CHK(cfg_stp_portPathCost_set(port, pathCost));

    SYS_ERR_CHK(stp_mstPortPathCost_set(0, port, pathCost));
#ifdef CONFIG_SYS_PROTO_STP_MSTP
    SYS_ERR_CHK(cfg_stp_mstPortPathCost_set(0, port, pathCost));
#endif

    return SYS_ERR_OK;
}

int32 sal_stp_portPathCost_get(sys_logic_port_t port, uint32 *pPathCost)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPathCost), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_portPathCost_get(port, pPathCost));

    return SYS_ERR_OK;
}

int32 sal_stp_portEdgePort_set(sys_logic_port_t port, uint32 conf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_portEdgePort_set(port, conf));
    SYS_ERR_CHK(cfg_stp_portEdgePort_set(port, conf));

    return SYS_ERR_OK;
}

int32 sal_stp_portEdgePort_get(sys_logic_port_t port, uint32 *pConf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pConf), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_portEdgePort_get(port, pConf));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_STP_BPDU_FILTER

int32 sal_stp_portBpduFilter_set(sys_logic_port_t port, uint32 conf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_portBpduFilter_set(port, conf));
    SYS_ERR_CHK(cfg_stp_portBpduFilter_set(port, conf));

    return SYS_ERR_OK;
}

int32 sal_stp_portBpduFilter_get(sys_logic_port_t port, uint32 *pConf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pConf), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_portBpduFilter_get(port, pConf));

    return SYS_ERR_OK;
}
#endif  

#ifdef CONFIG_SYS_STP_BPDU_GUARD

int32 sal_stp_portBpduGuard_set(sys_logic_port_t port, uint32 conf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_portBpduGuard_set(port, conf));
    SYS_ERR_CHK(cfg_stp_portBpduGuard_set(port, conf));

    return SYS_ERR_OK;
}

int32 sal_stp_portBpduGuard_get(sys_logic_port_t port, uint32 *pConf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pConf), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_portBpduGuard_get(port, pConf));

    return SYS_ERR_OK;
}
#endif  

int32 sal_stp_portPointToPointMac_set(sys_logic_port_t port, uint32 conf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_TRK_CHK(stp_portPointToPointMac_set(port, conf));
    SYS_ERR_CHK(cfg_stp_portPointToPointMac_set(port, conf));

    return SYS_ERR_OK;
}

int32 sal_stp_portPointToPointMac_get(sys_logic_port_t port, uint32 *pConf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pConf, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_portPointToPointMac_get(port, pConf));

    return SYS_ERR_OK;
}

int32 sal_stp_portMcheck_set(sys_logic_port_t port, uint32 conf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_portMcheck_set(port, conf));

    SYS_LOG(LOG_CAT_STP, LOG_STP_MIGRATION, LPSTR(port));

    return SYS_ERR_OK;
}

int32 sal_stp_portMcheck_get(sys_logic_port_t port, uint32 *pConf)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pConf, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_portMcheck_get(port, pConf));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP

int32 sal_stp_mstDflt_init(sys_stp_mst_dflt_t dfltInfo)
{
    uint32 msti;
    sys_logic_port_t lp;

    SYS_ERR_CHK(stp_mstDflt_init(dfltInfo));

    for (msti = 0; msti < MSTP_INSTANCE_NUM; msti++)
    {
        SYS_ERR_CHK(cfg_stp_mstPriority_set(msti, dfltInfo.priority));

        FOR_EACH_LOGIC_PORT(lp)
        {
            SYS_ERR_CHK(cfg_stp_mstPortPathCost_set(msti, lp, dfltInfo.portPathCost));
            SYS_ERR_CHK(cfg_stp_mstPortPriority_set(msti, lp, dfltInfo.portPriority));
        }
    }

    return SYS_ERR_OK;
}

int32 sal_stp_mstConfigName_set(char *name)
{
    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_mstConfigName_set(name));
    SYS_ERR_CHK(cfg_stp_mstConfigName_set(name));

    return SYS_ERR_OK;
}

int32 sal_stp_mstConfigName_get(char *name)
{
    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_mstConfigName_get(name));

    return SYS_ERR_OK;
}

int32 sal_stp_mstConfigRevision_set(uint32 revision)
{
    SYS_ERR_CHK(stp_mstConfigRevision_set(revision));
    SYS_ERR_CHK(cfg_stp_mstConfigRevision_set(revision));

    return SYS_ERR_OK;
}

int32 sal_stp_mstConfigRevision_get(uint32 *pRevision)
{
    SYS_PARAM_CHK((NULL == pRevision), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_mstConfigRevision_get(pRevision));

    return SYS_ERR_OK;
}

int32 sal_stp_mstVlanInstance_set(uint32 vlanId, uint32 instance)
{
    uint32 i = 0;
    char strVlanList[CAPA_VLANLIST_STR_LEN];

    SYS_PARAM_CHK(IS_VID_INVALID(vlanId), SYS_ERR_VLAN_ID);

    osal_memset(strVlanList, 0, sizeof(strVlanList));

    SYS_ERR_CHK(sal_stp_mstVlanInstance_get(vlanId, &i));
    snprintf(strVlanList, sizeof(strVlanList), "%u", vlanId);

    SYS_ERR_CHK(stp_mstVlanInstance_set(vlanId, instance));
    SYS_ERR_CHK(cfg_stp_mstVlanInstance_set(vlanId, instance));

    if (0 != instance)
        SYS_LOG(LOG_CAT_STP, LOG_STP_MSTI_VLAN_ADD, strVlanList, instance);
    else
        SYS_LOG(LOG_CAT_STP, LOG_STP_MSTI_VLAN_DEL, strVlanList, i);

    return SYS_ERR_OK;
}

int32 sal_stp_mstVlanInstance_get(uint32 vlanId, uint32 *pInstance)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vlanId), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pInstance), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_mstVlanInstance_get(vlanId, pInstance));

    return SYS_ERR_OK;
}

int32 sal_stp_mstVlanmask_set(uint32 instance, sys_vlanmask_t vlanmask)
{
    char strVlanList[CAPA_VLANLIST_STR_LEN];

    SYS_PARAM_CHK(MSTP_INSTANCE_NUM <= instance, SYS_ERR_INPUT);

    osal_memset(strVlanList, 0, sizeof(strVlanList));

    SYS_ERR_CHK(VMSK2STR(&vlanmask, strVlanList));

    SYS_ERR_CHK(stp_mstVlanmask_set(instance, vlanmask));
    SYS_ERR_CHK(cfg_stp_mstVlanmask_set(instance, vlanmask));

    SYS_LOG(LOG_CAT_STP, LOG_STP_MSTI_VLAN_ADD, strVlanList, instance);

    return SYS_ERR_OK;
}

int32 sal_stp_mstVlanmask_get(uint32 instance, sys_vlanmask_t *pVlanmask)
{
    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_mstVlanmask_get(instance, pVlanmask));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPriority_set(uint32 instance, uint32 priority)
{
    SYS_PARAM_CHK(MSTP_INSTANCE_NUM <= instance, SYS_ERR_INPUT);

    SYS_ERR_CHK(stp_mstPriority_set(instance, priority));
    SYS_ERR_CHK(cfg_stp_mstPriority_set(instance, priority));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPriority_get(uint32 instance, uint32 *pPriority)
{
    SYS_PARAM_CHK((NULL == pPriority), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_mstPriority_get(instance, pPriority));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPortPathCost_set(uint32 instance, sys_logic_port_t port, uint32 pathCost)
{
    uint32 method;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_pathcostMethod_get(&method));
    if ((STP_PATHCOST_METHOD_SHORT == method) && (pathCost > 65535))
    {
        return SYS_ERR_STP_COST_SHORT_RANGE;
    }

    if (0 == instance)
    {
        SYS_ERR_CHK(stp_portPathCost_set(port, pathCost));
        SYS_ERR_CHK(cfg_stp_portPathCost_set(port, pathCost));
    }

    SYS_ERR_CHK(stp_mstPortPathCost_set(instance, port, pathCost));
    SYS_ERR_CHK(cfg_stp_mstPortPathCost_set(instance, port, pathCost));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPortPathCost_get(uint32 instance, sys_logic_port_t port, uint32 *pPathCost)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPathCost), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_mstPortPathCost_get(instance, port, pPathCost));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPortPriority_set(uint32 instance, sys_logic_port_t port, uint32 priority)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_mstPortPriority_set(instance, port, priority));
    SYS_ERR_CHK(cfg_stp_mstPortPriority_set(instance, port, priority));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPortPriority_get(uint32 instance, sys_logic_port_t port, uint32 *pPriority)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPriority), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_stp_mstPortPriority_get(instance, port, pPriority));

    return SYS_ERR_OK;
}
#endif

int32 sal_stp_info_get(sys_stp_info_t *pStpInfo)
{
    SYS_PARAM_CHK((NULL == pStpInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_info_get(pStpInfo));

    return SYS_ERR_OK;
}

int32 sal_stp_portInfo_get(sys_logic_port_t port, sys_stp_port_info_t *pStpPortInfo)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pStpPortInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_portInfo_get(port, pStpPortInfo));

    return SYS_ERR_OK;
}

int32 sal_stp_portCounter_clear(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(stp_portCounter_clear(port));

    return SYS_ERR_OK;
}

int32 sal_stp_mstInfo_get(uint32 instance, sys_stp_mst_info_t *pMstInfo)
{
    SYS_PARAM_CHK((NULL == pMstInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_mstInfo_get(instance, pMstInfo));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPortInfo_get(uint32 instance, sys_logic_port_t port, sys_stp_mst_port_info_t *pMstPortInfo)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pMstPortInfo, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_mstPortInfo_get(instance, port, pMstPortInfo));

    return SYS_ERR_OK;
}

int32 sal_stp_mstPortMsg_get(uint32 instance, sys_logic_port_t port, sys_stp_mstPortMsg_t *pMstPortMsg)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(NULL == pMstPortMsg, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(stp_mstPortMsg_get(instance, port, pMstPortMsg));

    return SYS_ERR_OK;
}

