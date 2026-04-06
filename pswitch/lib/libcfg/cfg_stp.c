
#include <libcfg/cfg.h>

int32 cfg_stp_init(void)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->timerSeq[0] = STP_FORWARD_DELAY;
    pStp->timerSeq[1] = STP_MAX_AGE;
    pStp->timerSeq[2] = STP_HELLO_TIME;

    CFG_CLOSE(CFG_ID_STP);

    return SYS_ERR_OK;
}

int32 cfg_stp_enable_set(sys_enable_t enable)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->enable = enable;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_enable_get(sys_enable_t *pEnable)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pEnable = pStp->enable;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_forwardBpdu_set(sys_enable_t enable)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->forwardBpdu = enable;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_forwardBpdu_get(sys_enable_t *pEnable)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pEnable = pStp->forwardBpdu;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_forceVersion_set(uint32 forceVersion)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->forceVersion = forceVersion;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_forceVersion_get(uint32 *pForceVersion)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pForceVersion, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pForceVersion = pStp->forceVersion;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_helloTime_set(uint32 helloTime)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->helloTime = helloTime;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_helloTime_get(uint32 *pHelloTime)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pHelloTime, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pHelloTime = pStp->helloTime;

    CFG_CLOSE(CFG_ID_STP);
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP

int32 cfg_stp_maxHops_set(uint32 maxHops)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->maxHops = maxHops;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_maxHops_get(uint32 *pMaxHops)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pMaxHops, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pMaxHops = pStp->maxHops;

    CFG_CLOSE(CFG_ID_STP);
}
#endif

int32 cfg_stp_forwardDelay_set(uint32 forwardDelay)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->forwardDelay = forwardDelay;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_forwardDelay_get(uint32 *pForwardDelay)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pForwardDelay, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pForwardDelay = pStp->forwardDelay;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_maximumAge_set(uint32 maximumAge)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->maximumAge = maximumAge;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_maximumAge_get(uint32 *pMaximumAge)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pMaximumAge, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pMaximumAge = pStp->maximumAge;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_timer_sequence_set(uint32 seqIdx,uint32 timerSeq)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->timerSeq[seqIdx] = timerSeq;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_timer_sequence_get(uint32 seqIdx,uint32 *pTimerSeq)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pTimerSeq, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pTimerSeq = pStp->timerSeq[seqIdx];

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_txHoldCount_set(uint32 txHoldCount)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->txHoldCount = txHoldCount;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_txHoldCount_get(uint32 *pTxHoldCount)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pTxHoldCount, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pTxHoldCount = pStp->txHoldCount;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_pathcostMethod_set(uint32 method)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->pathcostMethod = method;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_pathcostMethod_get(uint32 *pMethod)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(NULL == pMethod, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pMethod = pStp->pathcostMethod;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_priority_set(uint32 priority)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    pStp->mst[0].priority = priority;
#else
    pStp->priority = priority;
#endif

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_priority_get(uint32 *pPriority)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK((NULL == pPriority), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    *pPriority = pStp->mst[0].priority;
#else
    *pPriority = pStp->priority;
#endif

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portAdminEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->port[port].adminEnable = enable;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portAdminEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pEnable = pStp->port[port].adminEnable;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portForwardBpdu_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->port[port].forwardBpdu = enable;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portForwardBpdu_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pEnable = pStp->port[port].forwardBpdu;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portPathCost_set(sys_logic_port_t port, uint32 pathCost)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->port[port].confPathCost = pathCost;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portPathCost_get(sys_logic_port_t port, uint32 *pPathCost)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPathCost), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pPathCost = pStp->port[port].confPathCost;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portEdgePort_set(sys_logic_port_t port, uint32 conf)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->port[port].confEdgePort = conf;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portEdgePort_get(sys_logic_port_t port, uint32 *pConf)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pConf), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pConf = pStp->port[port].confEdgePort;

    CFG_CLOSE(CFG_ID_STP);
}

#ifdef CONFIG_SYS_STP_BPDU_FILTER

int32 cfg_stp_portBpduFilter_set(sys_logic_port_t port, uint32 conf)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->port[port].confBpduFilter = conf;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portBpduFilter_get(sys_logic_port_t port, uint32 *pConf)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pConf), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pConf = pStp->port[port].confBpduFilter;

    CFG_CLOSE(CFG_ID_STP);
}
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD

int32 cfg_stp_portBpduGuard_set(sys_logic_port_t port, uint32 conf)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->port[port].confBpduGuard = conf;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portBpduGuard_get(sys_logic_port_t port, uint32 *pConf)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pConf), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pConf = pStp->port[port].confBpduGuard;

    CFG_CLOSE(CFG_ID_STP);
}
#endif

int32 cfg_stp_portPointToPointMac_set(sys_logic_port_t port, uint32 conf)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->port[port].confPointToPointMac = conf;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portPointToPointMac_get(sys_logic_port_t port, uint32 *pConf)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pConf), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pConf = pStp->port[port].confPointToPointMac;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portPriority_set(sys_logic_port_t port, uint32 priority)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    pStp->mst[0].mstPort[port].priority = priority;
#else
    pStp->port[port].priority = priority;
#endif

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_portPriority_get(sys_logic_port_t port, uint32 *pPriority)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPriority), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    *pPriority = pStp->mst[0].mstPort[port].priority;
#else
    *pPriority = pStp->port[port].priority;
#endif

    CFG_CLOSE(CFG_ID_STP);
}

#ifdef CONFIG_SYS_PROTO_STP_MSTP

int32 cfg_stp_mstConfigName_set(char *name)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    osal_memcpy(pStp->mst_config.name, name, sizeof(pStp->mst_config.name));

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstConfigName_get(char *name)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK((NULL == name), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    osal_memcpy(name, pStp->mst_config.name, sizeof(pStp->mst_config.name));

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstConfigRevision_set(uint32 revision)
{
    cfg_stp_t *pStp = NULL;

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->mst_config.revision = revision;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstConfigRevision_get(uint32 *pRevision)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK((NULL == pRevision), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pRevision = pStp->mst_config.revision;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstVlanInstance_set(sys_vid_t vlanId, uint32 instance)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_VID_INVALID(vlanId), SYS_ERR_VLAN_ID);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    VLANMASK_CLEAR_VID(pStp->mst[pStp->mstVlanInstance[vlanId]].vlanmask, vlanId);
    VLANMASK_SET_VID(pStp->mst[instance].vlanmask, vlanId);

    pStp->mstVlanInstance[vlanId % SYS_VLAN_NUM_MAX] = instance;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstVlanInstance_get(sys_vid_t vlanId, uint32 *pInstance)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_VID_INVALID(vlanId), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pInstance), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pInstance = pStp->mstVlanInstance[vlanId % SYS_VLAN_NUM_MAX];

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstVlanmask_set(uint32 instance, sys_vlanmask_t vlanmask)
{
    cfg_stp_t *pStp = NULL;
    uint32 vlanId;

    SYS_PARAM_CHK(MSTP_INSTANCE_NUM <= instance, SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        
        VLANMASK_CLEAR_VID(pStp->mst[pStp->mstVlanInstance[vlanId]].vlanmask, vlanId);
        VLANMASK_SET_VID(pStp->mst[instance].vlanmask, vlanId);

        pStp->mstVlanInstance[vlanId % SYS_VLAN_NUM_MAX] = instance;
    }

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstVlanmask_get(uint32 instance, sys_vlanmask_t *pVlanmask)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK((NULL == pVlanmask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pVlanmask = pStp->mst[instance].vlanmask;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstPriority_set(uint32 instance, uint32 priority)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(MSTP_INSTANCE_NUM <= instance, SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->mst[instance].priority = priority;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstPriority_get(uint32 instance, uint32 *pPriority)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK((NULL == pPriority), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pPriority = pStp->mst[instance].priority;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstPortPathCost_set(uint32 instance, sys_logic_port_t port, uint32 pathCost)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->mst[instance].mstPort[port].internalPathCost = pathCost;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstPortPathCost_get(uint32 instance, sys_logic_port_t port, uint32 *pPathCost)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPathCost), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pPathCost = pStp->mst[instance].mstPort[port].internalPathCost;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstPortPriority_set(uint32 instance, sys_logic_port_t port, uint32 priority)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(MSTP_INSTANCE_NUM <= instance, SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    pStp->mst[instance].mstPort[port].priority = priority;

    CFG_CLOSE(CFG_ID_STP);
}

int32 cfg_stp_mstPortPriority_get(uint32 instance, sys_logic_port_t port, uint32 *pPriority)
{
    cfg_stp_t *pStp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPriority), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_STP, (cfg_addr)&pStp);

    *pPriority = pStp->mst[instance].mstPort[port].priority;

    CFG_CLOSE(CFG_ID_STP);
}
#endif
