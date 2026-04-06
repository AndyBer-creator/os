
#include <common/sys_util.h>
#include <libcfg/cfg.h>
#include <libsal/sal_util.h>
#include <libsal/sal_qos.h>

const sys_text_t text_qos_mode[] =
{
    {   "disable" },
    {    "basic"  },
    {  "advanced" },
};

const sys_text_t text_qos_trust[] =
{
    {      "untrusted" },
    {      "cos"  },
    {      "dscp" },
    {  "cos-dscp" },
    {       "ip-precedence" },
};

const sys_text_t text_qos_schedAlgo[] =
{
    {     "wfq" },
    {    "wrr"  },
    {      "strict-priority" },
    {  "sp-wfq" },
    {  "sp-wrr" },
};

#ifdef CONFIG_SYS_BW_CTRL
static int32 _sal_qos_rateLimitCfg_clear(void)
{
    uint32 dflt_rate;
    sys_qid_t qid = 0;
    sys_logic_port_t port;

    FOR_EACH_LOGIC_PORT(port)
    {
        
        SYS_ERR_CHK(sal_rate_igrBandwidthCtrlRate_set(port, DFLT_RATE_PORT_IGR));
        SYS_ERR_CHK(sal_rate_igrBandwidthCtrlEnable_set(port, DFLT_RATE_PORT_IGR_EBL));

        if (PORT_TYPE_100M == LP2TYPE(port))
             dflt_rate = DFLT_RATE_PORT_EGR_FE;
        else
             dflt_rate = DFLT_RATE_PORT_EGR_GE;
        SYS_ERR_CHK(sal_rate_egrBandwidthCtrlRate_set(port, dflt_rate));
        SYS_ERR_CHK(sal_rate_egrBandwidthCtrlEnable_set(port, DFLT_RATE_PORT_EGR_EBL));

        FOR_EACH_QID(qid)
        {
            SYS_ERR_CHK(sal_rate_egrQueueCtrlRate_set(port, qid, dflt_rate));
            SYS_ERR_CHK(sal_rate_egrQueueCtrlEnable_set(port, qid, DFLT_RATE_PORT_EGR_EBL));
            SYS_ERR_CHK(sal_rate_portEgrQueueCtrlBurstSize_set(port, qid, DFLT_RATE_QUEUE_EGR_BURST_THRESH));
        }
    }
    
    SYS_ERR_CHK(sal_rate_egrBandwidthCtrlBurstSize_set(DFLT_RATE_PORT_EGR_BURST_THRESH));
    SYS_ERR_CHK(sal_rate_egrSysQueueCtrlBurstSize_set(DFLT_RATE_QUEUE_EGR_BURST_THRESH));

    return SYS_ERR_OK;
}
#endif 

int32 sal_qos_mode_set(sys_qos_mode_t mode)
{
    SYS_ERR_CHK(rsd_qos_mode_set(mode));

    SYS_ERR_CHK(cfg_qos_mode_set(mode));

#ifdef CONFIG_SYS_BW_CTRL
    if (SYS_QOS_MODE_DISABLE == mode)
        _sal_qos_rateLimitCfg_clear();
#endif

    return SYS_ERR_OK;
}

int32 sal_qos_mode_get(sys_qos_mode_t *pMode)
{
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_mode_get(pMode));

    return SYS_ERR_OK;
}

int32 sal_qos_mode_check(sys_qos_mode_t mode)
{
    sys_qos_mode_t currMode;

    SYS_ERR_CHK(cfg_qos_mode_get(&currMode));

    if ((SYS_QOS_MODE == mode) && (SYS_QOS_MODE_DISABLE == currMode))
        return SYS_ERR_QOS_MODE;

    if (mode != currMode)
    {
        if (mode == SYS_QOS_MODE_BASIC)
            return SYS_ERR_QOS_MODE_BASIC;
    }

    return SYS_ERR_OK;
}

int32 sal_qos_portCos_set(sys_logic_port_t port, sys_qos_cos_t cos)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((cos >= CAPA_QOS_COS_NUM), SYS_ERR_QOS_COS_INVALID);

    SYS_ERR_TRK_CHK(rsd_qos_portCos_set(port, cos));

    SYS_ERR_CHK(cfg_qos_portCos_set(port, cos));

    return SYS_ERR_OK;
}

int32 sal_qos_portCos_get(sys_logic_port_t port, sys_qos_cos_t *pCos)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCos), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_portCos_get(port, pCos));

    return SYS_ERR_OK;
}

int32 sal_qos_strictPriQueueNum_set(uint32 spqNum)
{
    int32 tmpQid;
    sys_qid_t qid;
    sys_logic_port_t logicPort;
    sys_qos_queueWeight_t qWeight;
    sys_qos_queueWeight_t oldQWeight;

    SYS_MEM_CLEAR(qWeight);
    SYS_MEM_CLEAR(oldQWeight);

    SYS_ERR_CHK(cfg_qos_queueWeight_get(&oldQWeight));

    if (spqNum > 0)
    {
        for (tmpQid = (CAPA_QOS_QUEUE_NUM - spqNum - 1); tmpQid > -1; tmpQid--)
        {
            qWeight.weights[tmpQid] = oldQWeight.weights[tmpQid];
        }
    }
    else
    {
        for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
        {
            qWeight.weights[qid] = oldQWeight.weights[qid];
        }
    }

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        SYS_ERR_TRK_CHK(rsd_qos_portQueueWeight_set(logicPort, &qWeight));
    }

    SYS_ERR_CHK(cfg_qos_strictPriQueueNum_set(spqNum));

    SYS_LOG(LOG_CAT_QOSMGR, LOG_QOSMGR_STRICT_PRI_QUEUE,
            spqNum);

    return SYS_ERR_OK;
}

int32 sal_qos_strictPriQueueNum_get(uint32 *pSpqNum)
{
    SYS_PARAM_CHK((NULL == pSpqNum), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_strictPriQueueNum_get(pSpqNum));

    return SYS_ERR_OK;
}

int32 sal_qos_queueWeight_set(sys_qos_queueWeight_t weight)
{
    uint32 i = 0;
    uint32 spqNum = 0;
    sys_logic_port_t logicPort;
    sys_qos_queueWeight_t cfgQWeight;
    sys_qos_queueWeight_t cfgQWeightOld;

    SYS_MEM_CLEAR(cfgQWeight);

    SYS_ERR_CHK(cfg_qos_strictPriQueueNum_get(&spqNum));
    for (i = 0; i < (CAPA_QOS_QUEUE_NUM - spqNum); i++)
    {
        cfgQWeight.weights[i] = weight.weights[i];
    }
    FOR_EACH_LOGIC_PORT(logicPort)
    {
        SYS_ERR_TRK_CHK(rsd_qos_portQueueWeight_set(logicPort, &cfgQWeight));
    }
    
    SYS_ERR_CHK(cfg_qos_queueWeight_get(&cfgQWeightOld));
    
    for (i = (CAPA_QOS_QUEUE_NUM - spqNum); i < CAPA_QOS_QUEUE_NUM; i++)
    {
        cfgQWeight.weights[i] = cfgQWeightOld.weights[i];
    }
    SYS_ERR_CHK(cfg_qos_queueWeight_set(cfgQWeight));

    SYS_LOG(LOG_CAT_QOSMGR, LOG_QOSMGR_QUEUE_WEIGHT,
            cfgQWeight.weights[0],
            cfgQWeight.weights[1],
            cfgQWeight.weights[2],
            cfgQWeight.weights[3],
            cfgQWeight.weights[4],
            cfgQWeight.weights[5],
            cfgQWeight.weights[6],
            cfgQWeight.weights[7]);

    return SYS_ERR_OK;
}

int32 sal_qos_queueWeight_get(sys_qos_queueWeight_t *pWeight)
{
    SYS_PARAM_CHK((NULL == pWeight), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_queueWeight_get(pWeight));

    return SYS_ERR_OK;
}

int32 sal_qos_cosQueue_set(sys_qos_cos_t cos, sys_qid_t qid)
{
    SYS_PARAM_CHK(IS_COS_INVALID(cos), SYS_ERR_QOS_COS_INVALID);
    SYS_PARAM_CHK(IS_QID_INVALID(qid), SYS_ERR_QOS_QUEUE_INVALID);

    SYS_ERR_TRK_CHK(rsd_qos_cosQueue_set(cos, qid));

    SYS_ERR_CHK(cfg_qos_cosQueue_set(cos, qid));

    return SYS_ERR_OK;
}

int32 sal_qos_cosQueue_get(sys_qos_cos_t cos, sys_qid_t *pQid)
{
    SYS_PARAM_CHK(IS_COS_INVALID(cos), SYS_ERR_QOS_COS_INVALID);
    SYS_PARAM_CHK((NULL == pQid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_cosQueue_get(cos, pQid));

    return SYS_ERR_OK;
}

int32 sal_qos_dscpQueue_set(sys_qos_dscp_t dscp, sys_qid_t qid)
{
    SYS_PARAM_CHK(IS_DSCP_INVALID(dscp), SYS_ERR_QOS_DSCP_INVALID);
    SYS_PARAM_CHK(IS_QID_INVALID(qid), SYS_ERR_QOS_QUEUE_INVALID);

    SYS_ERR_CHK(rsd_qos_dscpQueue_set(dscp, qid));

    SYS_ERR_CHK(cfg_qos_dscpQueue_set(dscp, qid));

    return SYS_ERR_OK;
}

int32 sal_qos_dscpQueue_get(sys_qos_dscp_t dscp, sys_qid_t *pQid)
{
    SYS_PARAM_CHK(IS_DSCP_INVALID(dscp), SYS_ERR_QOS_DSCP_INVALID);
    SYS_PARAM_CHK((NULL == pQid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_dscpQueue_get(dscp, pQid));

    return SYS_ERR_OK;
}

int32 sal_qos_ippQueue_set(sys_qos_ipp_t ipp, sys_qid_t qid)
{
    SYS_PARAM_CHK(IS_PRECEDENCE_INVALID(ipp), SYS_ERR_QOS_IPP_INVALID);
    SYS_PARAM_CHK(IS_QID_INVALID(qid), SYS_ERR_QOS_QUEUE_INVALID);

    SYS_ERR_CHK(rsd_qos_ippQueue_set(ipp, qid));

    SYS_ERR_CHK(cfg_qos_ippQueue_set(ipp, qid));

    return SYS_ERR_OK;
}

int32 sal_qos_ippQueue_get(sys_qos_ipp_t ipp, sys_qid_t *pQid)
{
    SYS_PARAM_CHK(IS_PRECEDENCE_INVALID(ipp), SYS_ERR_QOS_IPP_INVALID);
    SYS_PARAM_CHK((NULL == pQid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_ippQueue_get(ipp, pQid));

    return SYS_ERR_OK;
}

int32 sal_qos_trustType_set(sys_qos_trustType_t trustType)
{
    SYS_PARAM_CHK((trustType >= SYS_QOS_TRUST_END), SYS_ERR_INPUT);

    if (IS_SYSTEM_READY())
        SYS_ERR_CHK(sal_qos_mode_check(SYS_QOS_MODE_BASIC));

    SYS_ERR_CHK(rsd_qos_trustType_set(trustType));

    SYS_ERR_CHK(cfg_qos_trustType_set(trustType));

    return SYS_ERR_OK;
}

int32 sal_qos_trustType_get(sys_qos_trustType_t *pTrustType)
{
    SYS_PARAM_CHK((NULL == pTrustType), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_trustType_get(pTrustType));

    return SYS_ERR_OK;
}

int32 sal_qos_portTrustState_set(sys_logic_port_t port, sys_enable_t enable)
{
    sys_qos_trustType_t trustType;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    if (IS_SYSTEM_READY())
        SYS_ERR_CHK(sal_qos_mode_check(SYS_QOS_MODE_BASIC));

    SYS_ERR_CHK(cfg_qos_trustType_get(&trustType));

    if (DISABLED == enable)
        trustType = SYS_QOS_TRUST_NONE;

    SYS_ERR_TRK_CHK(rsd_qos_portTrustType_set(port, trustType));

    SYS_ERR_CHK(cfg_qos_portTrustType_set(port, trustType));

    return SYS_ERR_OK;
}

int32 sal_qos_portTrustState_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    sys_qos_trustType_t trustType;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_portTrustType_get(port, &trustType));

    if (SYS_QOS_TRUST_NONE == trustType)
        *pEnable = DISABLED;
    else
        *pEnable = ENABLED;

    return SYS_ERR_OK;
}

int32 sal_qos_queueCos_set(sys_qid_t qid, sys_qos_cos_t cos)
{
    SYS_PARAM_CHK((qid >= QOS_QUEUE_MAX), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((cos >= CAPA_QOS_COS_NUM), SYS_ERR_QOS_COS_INVALID);

    SYS_ERR_CHK(rsd_qos_queueCos_set(qid, cos));

    SYS_ERR_CHK(cfg_qos_queueCos_set(qid, cos));

    return SYS_ERR_OK;
}

int32 sal_qos_queueCos_get(sys_qid_t qid, sys_qos_cos_t *pCos)
{
    SYS_PARAM_CHK((qid >= QOS_QUEUE_MAX), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((NULL == pCos), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_queueCos_get(qid, pCos));

    return SYS_ERR_OK;
}

int32 sal_qos_queueDscp_set(sys_qid_t qid, sys_qos_dscp_t dscp)
{
    SYS_PARAM_CHK((qid >= QOS_QUEUE_MAX), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((dscp >= CAPA_QOS_DSCP_NUM), SYS_ERR_QOS_DSCP_INVALID);

    SYS_ERR_CHK(rsd_qos_queueDscp_set(qid, dscp));

    SYS_ERR_CHK(cfg_qos_queueDscp_set(qid, dscp));

    return SYS_ERR_OK;
}

int32 sal_qos_queueDscp_get(sys_qid_t qid, sys_qos_dscp_t *pDscp)
{
    SYS_PARAM_CHK((qid >= QOS_QUEUE_MAX), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((NULL == pDscp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_queueDscp_get(qid, pDscp));

    return SYS_ERR_OK;
}

int32 sal_qos_queueIpp_set(sys_qid_t qid, sys_qos_ipp_t ipp)
{
    SYS_PARAM_CHK((qid >= QOS_QUEUE_MAX), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((ipp >= CAPA_QOS_IPP_NUM), SYS_ERR_QOS_IPP_INVALID);

    SYS_ERR_CHK(rsd_qos_queueIpp_set(qid, ipp));

    SYS_ERR_CHK(cfg_qos_queueIpp_set(qid, ipp));

    return SYS_ERR_OK;
}

int32 sal_qos_queueIpp_get(sys_qid_t qid, sys_qos_ipp_t *pIpp)
{
    SYS_PARAM_CHK((qid >= QOS_QUEUE_MAX), SYS_ERR_QOS_QUEUE_INVALID);
    SYS_PARAM_CHK((NULL == pIpp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_queueIpp_get(qid, pIpp));

    return SYS_ERR_OK;
}

int32 sal_qos_portRemarkCos_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_TRK_CHK(rsd_qos_portRemarkCos_set(port, enable));

    SYS_ERR_CHK(cfg_qos_portRemarkCos_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_qos_portRemarkCos_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_portRemarkCos_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_qos_portRemarkDscp_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    if (ENABLED == enable)
    {
        sys_enable_t ippRemark = DISABLED;

        cfg_qos_portRemarkIpp_get(port, &ippRemark);

        if (ENABLED == ippRemark)
            return SYS_ERR_QOS_REMARK_CONFLICT;
    }

    SYS_ERR_TRK_CHK(rsd_qos_portRemarkDscp_set(port, enable));

    SYS_ERR_CHK(cfg_qos_portRemarkDscp_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_qos_portRemarkDscp_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_portRemarkDscp_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_qos_portRemarkIpp_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    if (ENABLED == enable)
    {
        sys_enable_t dscpRemark = DISABLED;

        cfg_qos_portRemarkDscp_get(port, &dscpRemark);

        if (ENABLED == dscpRemark)
            return SYS_ERR_QOS_REMARK_CONFLICT;
    }

    SYS_ERR_TRK_CHK(rsd_qos_portRemarkIpp_set(port, enable));

    SYS_ERR_CHK(cfg_qos_portRemarkIpp_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_qos_portRemarkIpp_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_portRemarkIpp_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_qos_ipRemarkAbility_check(sys_logic_portmask_t ippRmkLpm,
                                                sys_logic_portmask_t dscpRmkLpm)
{
    SYS_ERR_CHK(rsd_qos_ipRemarkAbility_check(ippRmkLpm, dscpRmkLpm));

    return SYS_ERR_OK;
}

int32 sal_qos_ippRemarkPortMask_get(sys_logic_portmask_t* pIppRmkLpm)
{
    sys_logic_port_t logicPort;
    sys_enable_t enable;

    SYS_PARAM_CHK((NULL == pIppRmkLpm), SYS_ERR_NULL_POINTER);

    LOGIC_PORTMASK_CLEAR_ALL(*pIppRmkLpm);
    FOR_EACH_LOGIC_PORT(logicPort)
    {
        SYS_ERR_CHK(sal_qos_portRemarkIpp_get(logicPort, &enable));
        if (enable == ENABLED)
        {
            LOGIC_PORTMASK_SET_PORT(*pIppRmkLpm, logicPort);
        }
    }

    return SYS_ERR_OK;
}

int32 sal_qos_dscpRemarkPortMask_get(sys_logic_portmask_t* pDscpRmkLpm)
{
    sys_logic_port_t logicPort;
    sys_enable_t enable;

    SYS_PARAM_CHK((NULL == pDscpRmkLpm), SYS_ERR_NULL_POINTER);

    LOGIC_PORTMASK_CLEAR_ALL(*pDscpRmkLpm);
    FOR_EACH_LOGIC_PORT(logicPort)
    {
        SYS_ERR_CHK(sal_qos_portRemarkDscp_get(logicPort, &enable));
        if (enable == ENABLED)
        {
            LOGIC_PORTMASK_SET_PORT(*pDscpRmkLpm, logicPort);
        }
    }

    return SYS_ERR_OK;
}

int32 sal_qos_mapDefault_restore(sys_qos_mapType_t mapType)
{
    static const sys_qid_t dfltCosQueueMap[] =
    {
         DFLT_QOS_COS0_QUEUE,
         DFLT_QOS_COS1_QUEUE,
         DFLT_QOS_COS2_QUEUE,
         DFLT_QOS_COS3_QUEUE,
         DFLT_QOS_COS4_QUEUE,
         DFLT_QOS_COS5_QUEUE,
         DFLT_QOS_COS6_QUEUE,
         DFLT_QOS_COS7_QUEUE,
    };

    static const  sys_qid_t dfltIppQueueMap[] =
    {
         DFLT_QOS_IPP0_QUEUE,
         DFLT_QOS_IPP1_QUEUE,
         DFLT_QOS_IPP2_QUEUE,
         DFLT_QOS_IPP3_QUEUE,
         DFLT_QOS_IPP4_QUEUE,
         DFLT_QOS_IPP5_QUEUE,
         DFLT_QOS_IPP6_QUEUE,
         DFLT_QOS_IPP7_QUEUE,
    };

    static const  sys_qid_t dfltDscpQueueMap[] =
    {
         DFLT_QOS_DSCP0_QUEUE,
         DFLT_QOS_DSCP1_QUEUE,
         DFLT_QOS_DSCP2_QUEUE,
         DFLT_QOS_DSCP3_QUEUE,
         DFLT_QOS_DSCP4_QUEUE,
         DFLT_QOS_DSCP5_QUEUE,
         DFLT_QOS_DSCP6_QUEUE,
         DFLT_QOS_DSCP7_QUEUE,
         DFLT_QOS_DSCP8_QUEUE,
         DFLT_QOS_DSCP9_QUEUE,
         DFLT_QOS_DSCP10_QUEUE,
         DFLT_QOS_DSCP11_QUEUE,
         DFLT_QOS_DSCP12_QUEUE,
         DFLT_QOS_DSCP13_QUEUE,
         DFLT_QOS_DSCP14_QUEUE,
         DFLT_QOS_DSCP15_QUEUE,
         DFLT_QOS_DSCP16_QUEUE,
         DFLT_QOS_DSCP17_QUEUE,
         DFLT_QOS_DSCP18_QUEUE,
         DFLT_QOS_DSCP19_QUEUE,
         DFLT_QOS_DSCP20_QUEUE,
         DFLT_QOS_DSCP21_QUEUE,
         DFLT_QOS_DSCP22_QUEUE,
         DFLT_QOS_DSCP23_QUEUE,
         DFLT_QOS_DSCP24_QUEUE,
         DFLT_QOS_DSCP25_QUEUE,
         DFLT_QOS_DSCP26_QUEUE,
         DFLT_QOS_DSCP27_QUEUE,
         DFLT_QOS_DSCP28_QUEUE,
         DFLT_QOS_DSCP29_QUEUE,
         DFLT_QOS_DSCP30_QUEUE,
         DFLT_QOS_DSCP31_QUEUE,
         DFLT_QOS_DSCP32_QUEUE,
         DFLT_QOS_DSCP33_QUEUE,
         DFLT_QOS_DSCP34_QUEUE,
         DFLT_QOS_DSCP35_QUEUE,
         DFLT_QOS_DSCP36_QUEUE,
         DFLT_QOS_DSCP37_QUEUE,
         DFLT_QOS_DSCP38_QUEUE,
         DFLT_QOS_DSCP39_QUEUE,
         DFLT_QOS_DSCP40_QUEUE,
         DFLT_QOS_DSCP41_QUEUE,
         DFLT_QOS_DSCP42_QUEUE,
         DFLT_QOS_DSCP43_QUEUE,
         DFLT_QOS_DSCP44_QUEUE,
         DFLT_QOS_DSCP45_QUEUE,
         DFLT_QOS_DSCP46_QUEUE,
         DFLT_QOS_DSCP47_QUEUE,
         DFLT_QOS_DSCP48_QUEUE,
         DFLT_QOS_DSCP49_QUEUE,
         DFLT_QOS_DSCP50_QUEUE,
         DFLT_QOS_DSCP51_QUEUE,
         DFLT_QOS_DSCP52_QUEUE,
         DFLT_QOS_DSCP53_QUEUE,
         DFLT_QOS_DSCP54_QUEUE,
         DFLT_QOS_DSCP55_QUEUE,
         DFLT_QOS_DSCP56_QUEUE,
         DFLT_QOS_DSCP57_QUEUE,
         DFLT_QOS_DSCP58_QUEUE,
         DFLT_QOS_DSCP59_QUEUE,
         DFLT_QOS_DSCP60_QUEUE,
         DFLT_QOS_DSCP61_QUEUE,
         DFLT_QOS_DSCP62_QUEUE,
         DFLT_QOS_DSCP63_QUEUE,
    };

    static const  sys_qos_cos_t dfltQueueCosMap[] =
    {
         DFLT_QOS_QUEUE0_COS,
         DFLT_QOS_QUEUE1_COS,
         DFLT_QOS_QUEUE2_COS,
         DFLT_QOS_QUEUE3_COS,
         DFLT_QOS_QUEUE4_COS,
         DFLT_QOS_QUEUE5_COS,
         DFLT_QOS_QUEUE6_COS,
         DFLT_QOS_QUEUE7_COS,
    };

    static const  sys_qos_dscp_t dfltQueueDscpMap[] =
    {
         DFLT_QOS_QUEUE0_DSCP,
         DFLT_QOS_QUEUE1_DSCP,
         DFLT_QOS_QUEUE2_DSCP,
         DFLT_QOS_QUEUE3_DSCP,
         DFLT_QOS_QUEUE4_DSCP,
         DFLT_QOS_QUEUE5_DSCP,
         DFLT_QOS_QUEUE6_DSCP,
         DFLT_QOS_QUEUE7_DSCP,
    };

    static const  sys_qos_cos_t dfltQueueIppMap[] =
    {
         DFLT_QOS_QUEUE0_IPP,
         DFLT_QOS_QUEUE1_IPP,
         DFLT_QOS_QUEUE2_IPP,
         DFLT_QOS_QUEUE3_IPP,
         DFLT_QOS_QUEUE4_IPP,
         DFLT_QOS_QUEUE5_IPP,
         DFLT_QOS_QUEUE6_IPP,
         DFLT_QOS_QUEUE7_IPP,
    };

    sys_qid_t qid;
    sys_qos_cos_t cos;
    sys_qos_dscp_t dscp;
    sys_qos_ipp_t ipp;

    switch (mapType)
    {
        case SYS_QOS_MAP_COS2QUEUE:
            for (cos = 0; cos < CAPA_QOS_COS_NUM; cos++)
            {
                SYS_ERR_CONTINUE(sal_qos_cosQueue_set(cos, dfltCosQueueMap[cos]));
            }
            break;

        case SYS_QOS_MAP_DSCP2QUEUE:
            for (dscp = 0; dscp < CAPA_QOS_DSCP_NUM; dscp++)
            {
                SYS_ERR_CONTINUE(sal_qos_dscpQueue_set(dscp, dfltDscpQueueMap[dscp]));
            }
            break;

        case SYS_QOS_MAP_IPP2QUEUE:
            for (ipp = 0; ipp < CAPA_QOS_IPP_NUM; ipp++)
            {
                SYS_ERR_CONTINUE(sal_qos_ippQueue_set(ipp, dfltIppQueueMap[ipp]));
            }
            break;

        case SYS_QOS_MAP_QUEUE2COS:
            for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
            {
                SYS_ERR_CONTINUE(sal_qos_queueCos_set(qid, dfltQueueCosMap[qid]));
            }
            break;

        case SYS_QOS_MAP_QUEUE2DSCP:
            for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
            {
                SYS_ERR_CONTINUE(sal_qos_queueDscp_set(qid, dfltQueueDscpMap[qid]));
            }
            break;

        case SYS_QOS_MAP_QUEUE2IPP:
            for (qid = 0; qid < CAPA_QOS_QUEUE_NUM; qid++)
            {
                SYS_ERR_CONTINUE(sal_qos_queueIpp_set(qid, dfltQueueIppMap[qid]));
            }
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}

int32 sal_qos_congAvoid_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_qos_congAvoid_set(enable));

    return SYS_ERR_OK;
}

int32 sal_qos_congAvoid_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_qos_congAvoid_get(pEnable));

    return SYS_ERR_OK;
}

