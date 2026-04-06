
#include <rsd/rsd_stp.h>
#include <stp/proto_stp.h>
#include <ski/proto/stp/ski_stp.h>

int32 ski_stp_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_stp_ioctl_t);
    ski_stp_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_STP_SET_ENABLE:
            SYS_ERR_HDL(stp_enable_set(pData->enable.enable), _exit, ret);
            break;

        case SKI_STP_SET_FORWARD_BPDU:
            SYS_ERR_HDL(stp_forwardBpdu_set(pData->forwardBpdu.enable), _exit, ret);
            break;

        case SKI_STP_GET_ENABLE:
            SYS_ERR_HDL(stp_enable_get(&pData->enable.enable), _exit, ret);
            break;

        case SKI_STP_GET_FORWARD_BPDU:
            SYS_ERR_HDL(stp_forwardBpdu_get(&pData->forwardBpdu.enable), _exit, ret);
            break;

        case SKI_STP_SET_FORCE_VERSION:
            SYS_ERR_HDL(stp_forceVersion_set(pData->forceVersion.forceVersion), _exit, ret);
            break;

        case SKI_STP_GET_FORCE_VERSION:
            SYS_ERR_HDL(stp_forceVersion_get(&pData->forceVersion.forceVersion), _exit, ret);
            break;

        case SKI_STP_SET_HELLO_TIME:
            SYS_ERR_HDL(stp_helloTime_set(pData->helloTime.helloTime), _exit, ret);
            break;

        case SKI_STP_GET_HELLO_TIME:
            SYS_ERR_HDL(stp_helloTime_get(&pData->helloTime.helloTime), _exit, ret);
            break;

        case SKI_STP_SET_MAX_HOPS:
            SYS_ERR_HDL(stp_maxHops_set(pData->maxHops.maxHops), _exit, ret);
            break;

        case SKI_STP_GET_MAX_HOPS:
            SYS_ERR_HDL(stp_maxHops_get(&pData->maxHops.maxHops), _exit, ret);
            break;

        case SKI_STP_SET_FORWARD_DELAY:
            SYS_ERR_HDL(stp_forwardDelay_set(pData->forwardDelay.forwardDelay), _exit, ret);
            break;

        case SKI_STP_GET_FORWARD_DELAY:
            SYS_ERR_HDL(stp_forwardDelay_get(&pData->forwardDelay.forwardDelay), _exit, ret);
            break;

        case SKI_STP_SET_MAXIMUM_AGE:
            SYS_ERR_HDL(stp_maximumAge_set(pData->maximumAge.maximumAge), _exit, ret);
            break;

        case SKI_STP_GET_MAXIMUM_AGE:
            SYS_ERR_HDL(stp_maximumAge_get(&pData->maximumAge.maximumAge), _exit, ret);
            break;

        case SKI_STP_SET_TX_HOLD_COUNT:
            SYS_ERR_HDL(stp_txHoldCount_set(pData->txHoldCount.txHoldCount), _exit, ret);
            break;

        case SKI_STP_GET_TX_HOLD_COUNT:
            SYS_ERR_HDL(stp_txHoldCount_get(&pData->txHoldCount.txHoldCount), _exit, ret);
            break;

        case SKI_STP_SET_PATHCOST_METHOD:
            SYS_ERR_HDL(stp_pathcostMethod_set(pData->pathcostMethod.pathcostMethod), _exit, ret);
            break;

        case SKI_STP_GET_PATHCOST_METHOD:
            SYS_ERR_HDL(stp_pathcostMethod_get(&pData->pathcostMethod.pathcostMethod), _exit, ret);
            break;

        case SKI_STP_SET_PORT_ADMIN_ENABLE:
            SYS_ERR_HDL(stp_portAdminEnable_set(pData->portAdminEnable.port, pData->portAdminEnable.adminEnable), _exit, ret);
            break;

        case SKI_STP_GET_PORT_ADMIN_ENABLE:
            SYS_ERR_HDL(stp_portAdminEnable_get(pData->portAdminEnable.port, &pData->portAdminEnable.adminEnable), _exit, ret);
            break;

        case SKI_STP_SET_PORT_FORWARD_BPDU:
            SYS_ERR_HDL(stp_portForwardBpdu_set(pData->portForwardBpdu.port, pData->portForwardBpdu.forwardBpdu), _exit, ret);
            break;

        case SKI_STP_GET_PORT_FORWARD_BPDU:
            SYS_ERR_HDL(stp_portForwardBpdu_get(pData->portForwardBpdu.port, &pData->portForwardBpdu.forwardBpdu), _exit, ret);
            break;

        case SKI_STP_SET_PORT_PATH_COST:
            SYS_ERR_HDL(stp_portPathCost_set(pData->portPathCost.port, pData->portPathCost.pathCost), _exit, ret);
            break;

        case SKI_STP_GET_PORT_PATH_COST:
            SYS_ERR_HDL(stp_portPathCost_get(pData->portPathCost.port, &pData->portPathCost.pathCost), _exit, ret);
            break;

#if 0
        case SKI_STP_SET_PORT_PRIORITY:
            SYS_ERR_HDL(stp_portPriority_set(pData->portPriority.port, pData->portPriority.priority), _exit, ret);
            break;

        case SKI_STP_GET_PORT_PRIORITY:
            SYS_ERR_HDL(stp_portPriority_get(pData->portPriority.port, &pData->portPriority.priority), _exit, ret);
            break;
#endif

        case SKI_STP_SET_PORT_EDGE_PORT:
            SYS_ERR_HDL(stp_portEdgePort_set(pData->portEdgePort.port, pData->portEdgePort.edgePort), _exit, ret);
            break;

        case SKI_STP_GET_PORT_EDGE_PORT:
            SYS_ERR_HDL(stp_portEdgePort_get(pData->portEdgePort.port, &pData->portEdgePort.edgePort), _exit, ret);
            break;

#ifdef CONFIG_SYS_STP_BPDU_FILTER
        case SKI_STP_SET_PORT_BPDU_FILTER:
            SYS_ERR_HDL(stp_portBpduFilter_set(pData->portBpduFilter.port, \
                                               pData->portBpduFilter.bpduFilter), _exit, ret);
            break;

        case SKI_STP_GET_PORT_BPDU_FILTER:
            SYS_ERR_HDL(stp_portBpduFilter_get(pData->portBpduFilter.port, \
                                               &pData->portBpduFilter.bpduFilter), _exit, ret);
            break;
#endif  

#ifdef CONFIG_SYS_STP_BPDU_GUARD
        case SKI_STP_SET_PORT_BPDU_GUARD:
            SYS_ERR_HDL(stp_portBpduGuard_set(pData->portBpduGuard.port, \
                                  pData->portBpduGuard.bpduGuard), _exit, ret);
            break;

        case SKI_STP_GET_PORT_BPDU_GUARD:
            SYS_ERR_HDL(stp_portBpduGuard_get(pData->portBpduGuard.port, \
                                              &pData->portBpduGuard.bpduGuard), _exit, ret);
            break;
#endif  

        case SKI_STP_SET_PORT_POINT_TO_POINT_MAC:
            SYS_ERR_HDL(stp_portPointToPointMac_set(pData->portPointToPointMac.port, \
                                                    pData->portPointToPointMac.pointToPointMac), _exit, ret);
            break;

        case SKI_STP_GET_PORT_POINT_TO_POINT_MAC:
            SYS_ERR_HDL(stp_portPointToPointMac_get(pData->portPointToPointMac.port, \
                                                    &pData->portPointToPointMac.pointToPointMac), _exit, ret);
            break;

        case SKI_STP_SET_PORT_MCHECK:
            SYS_ERR_HDL(stp_portMcheck_set(pData->portMcheck.port, pData->portMcheck.mcheck), _exit, ret);
            break;

        case SKI_STP_GET_PORT_MCHECK:
            SYS_ERR_HDL(stp_portMcheck_get(pData->portMcheck.port, &pData->portMcheck.mcheck), _exit, ret);
            break;

        case SKI_STP_SET_MST_DFLTINIT:
            SYS_ERR_HDL(stp_mstDflt_init(pData->mstDflt.mstDflt), _exit, ret);
            break;

        case SKI_STP_SET_MST_CONFIG_NAME:
            SYS_ERR_HDL(stp_mstConfigName_set(pData->mstConfigName.name), _exit, ret);
            break;

        case SKI_STP_GET_MST_CONFIG_NAME:
            SYS_ERR_HDL(stp_mstConfigName_get(pData->mstConfigName.name), _exit, ret);
            break;

        case SKI_STP_SET_MST_CONFIG_REVISION:
            SYS_ERR_HDL(stp_mstConfigRevision_set(pData->mstConfigRevision.revision), _exit, ret);
            break;

        case SKI_STP_GET_MST_CONFIG_REVISION:
            SYS_ERR_HDL(stp_mstConfigRevision_get(&pData->mstConfigRevision.revision), _exit, ret);
            break;

        case SKI_STP_SET_MST_VLAN_INSTANCE:
            SYS_ERR_HDL(stp_mstVlanInstance_set(pData->mstVlanInstance.vlanId, \
                                                pData->mstVlanInstance.instance), _exit, ret);
            break;

        case SKI_STP_GET_MST_VLAN_INSTANCE:
            stp_mstVlanInstance_get(pData->mstVlanInstance.vlanId, \
                                    &pData->mstVlanInstance.instance);
            break;

        case SKI_STP_SET_MST_VLANMASK:
            SYS_ERR_HDL(stp_mstVlanmask_set(pData->mstVlanmask.instance, \
                                            pData->mstVlanmask.vlanmask), _exit, ret);
            break;
#if 0
        case SKI_STP_GET_MST_VLANMASK:
            SYS_ERR_HDL(stp_mstVlanmask_get(pData->mstVlanmask.instance, \
                                            &pData->mstVlanmask.vlanmask), _exit, ret);
            break;
#endif

        case SKI_STP_SET_MST_PRIORITY:
            SYS_ERR_HDL(stp_mstPriority_set(pData->mstPriority.instance, \
                                            pData->mstPriority.priority), _exit, ret);
            break;

        case SKI_STP_GET_MST_PRIORITY:
            SYS_ERR_HDL(stp_mstPriority_get(pData->mstPriority.instance, \
                                            &pData->mstPriority.priority), _exit, ret);
            break;

        case SKI_STP_SET_MST_PORT_PATH_COST:
            SYS_ERR_HDL(stp_mstPortPathCost_set(pData->mstPortPathCost.instance, \
                                                pData->mstPortPathCost.port, \
                                                pData->mstPortPathCost.pathCost), _exit, ret);
            break;

        case SKI_STP_GET_MST_PORT_PATH_COST:
            SYS_ERR_HDL(stp_mstPortPathCost_get(pData->mstPortPathCost.instance, \
                                                pData->mstPortPathCost.port, \
                                                &pData->mstPortPathCost.pathCost), _exit, ret);
            break;

        case SKI_STP_SET_MST_PORT_PRIORITY:
            SYS_ERR_HDL(stp_mstPortPriority_set(pData->mstPortPriority.instance, \
                                                pData->mstPortPriority.port, \
                                                pData->mstPortPriority.priority), _exit, ret);
            break;

        case SKI_STP_GET_MST_PORT_PRIORITY:
            SYS_ERR_HDL(stp_mstPortPriority_get(pData->mstPortPriority.instance, \
                                                pData->mstPortPriority.port, \
                                                &pData->mstPortPriority.priority), _exit, ret);
            break;

        case SKI_STP_GET_INFO:
            SYS_ERR_HDL(stp_info_get(&pData->info.info), _exit, ret);
            break;

        case SKI_STP_GET_PORT_INFO:
            SYS_ERR_HDL(stp_portInfo_get(pData->portInfo.port, &pData->portInfo.portInfo), _exit, ret);
            break;

        case SKI_STP_SET_PORT_COUNTER_CLEAR:
            SYS_ERR_HDL(stp_portCounter_clear(pData->portCounter.port), _exit, ret);
            break;

        case SKI_STP_GET_MST_INFO:
            SYS_ERR_HDL(stp_mstInfo_get(pData->mstInfo.instance, &pData->mstInfo.mstInfo), _exit, ret);
            break;

        case SKI_STP_GET_MST_PORT_INFO:
            SYS_ERR_HDL(stp_mstPortInfo_get(pData->mstPortInfo.instance, \
                                            pData->mstPortInfo.port, \
                                            &pData->mstPortInfo.mstPortInfo), _exit, ret);
            break;

        case SKI_STP_GET_MST_PORT_MSG:
            SYS_ERR_HDL(stp_mstPortMsg_get(pData->mstPortMsg.instance, \
                                           pData->mstPortMsg.port, \
                                           &pData->mstPortMsg.mstPortMsg), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if (cmd > SKI_STP_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_stp_init(void)
{
    
    SYS_ERR_CHK(rsd_stp_init());
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_STP, ski_stp_cmd_mux));

    return SYS_ERR_OK;
}

