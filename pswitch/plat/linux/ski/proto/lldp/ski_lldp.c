
#include <ski/proto/lldp/ski_lldp.h>
#ifdef CONFIG_SYS_PROTO_LLDP_MED
#include <ski/proto/lldp/ski_lldp_med.h>
#endif

int32 ski_lldp_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_lldp_ioctl_t);
    ski_lldp_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {

        case SKI_LLDP_SET_ENABLE:
            SYS_ERR_HDL(lldp_enable_set(pData->enable.enable), _exit, ret);;
            break;

        case SKI_LLDP_GET_ENABLE:
            SYS_ERR_HDL(lldp_enable_get(&pData->enable.enable), _exit, ret);;
            break;

        case SKI_LLDP_SET_PORT:
            SYS_ERR_HDL(lldp_port_set(pData->lldpContrl.port,pData->lldpContrl.lldpPortSate), _exit, ret);;
            break;

        case SKI_LLDP_GET_PORT:
            SYS_ERR_HDL(lldp_port_get(pData->lldpContrl.port, &pData->lldpContrl.lldpPortSate), _exit, ret);;
            break;

        case SKI_LLDP_SET_HOLDTIME:
            SYS_ERR_HDL(lldp_holdtime_set(pData->holdtime), _exit, ret);;
            break;

        case SKI_LLDP_SET_TX_INTERVAL:
            SYS_ERR_HDL(lldp_tx_interval_set(pData->tx_interval), _exit, ret);;
            break;

        case SKI_LLDP_SET_REINIT_DELAY:
            SYS_ERR_HDL(lldp_reinit_delay_set(pData->reinit_delay), _exit, ret);;
            break;

        case SKI_LLDP_GET_PORT_LOCAL_ENTRYINFO:
            SYS_ERR_HDL(lldp_portLocalEntryInfo_get(pData->localEntryInfo.port, &pData->localEntryInfo.entryInfo), _exit, ret);;
            break;

        case SKI_LLDP_GET_PORT_REMOTE_ENTRYINFO:
            SYS_ERR_HDL(lldp_portRemoteEntryInfo_get(pData->remoteEntryInfo.port, pData->remoteEntryInfo.index, &pData->remoteEntryInfo.entryInfo), _exit, ret);;
            break;

        case SKI_LLDP_SET_ENTRYINFO_CLEAR:
            SYS_ERR_HDL(lldp_portRemoteEntry_clear(pData->remoteEntryInfo.port, pData->remoteEntryInfo.index), _exit, ret);;
            break;

        case SKI_LLDP_GET_PORT_VLAN_NAME_TLV:
            SYS_ERR_HDL(lldp_portRemoteVlanNameTlv_get(pData->lldp_portRemoteVlanTlv.port, pData->lldp_portRemoteVlanTlv.index,
                &pData->lldp_portRemoteVlanTlv.vlanTlv), _exit, ret);;
            break;

        case SKI_LLDP_GET_PORT_OVERLOAD:
            SYS_ERR_HDL(lldp_portTlvOverload_get(pData->lldp_portOverload.port, &pData->lldp_portOverload.overload), _exit, ret);;
            break;

        case SKI_LLDP_GET_STATISTIC:
            SYS_ERR_HDL(lldp_statistic_get(&pData->lldp_portStatistic.lldp_statistic), _exit, ret);;
            break;

        case SKI_LLDP_GET_PORT_STATISTIC:
            SYS_ERR_HDL(lldp_portEntry_get(pData->lldp_portStatistics.portIndex, &pData->lldp_portStatistics.portEntry), _exit, ret);;
            break;

        case SKI_LLDP_GLOBALSTATISTICS_CLEAR:
            SYS_ERR_HDL(lldp_globalStatistics_clear(), _exit, ret);;
            break;

        case SKI_LLDP_PORTSTATISTICS_CLEAR:
            SYS_ERR_HDL(lldp_portStatistics_clear(pData->lldp_portStatistic.port), _exit, ret);;
            break;

        case SKI_LLDP_SET_PORT_TLV_SEL:
            SYS_ERR_HDL(lldp_portTlvSel_set(pData->lldp_portTlvSel.port, pData->lldp_portTlvSel.lldpTlvSel), _exit, ret);;
            break;

        case SKI_LLDP_SET_TX_DELAY:
            SYS_ERR_HDL(lldp_tx_delay_set(pData->txDelay), _exit, ret);;
            break;

        case SKI_LLDP_SET_DISABLE_ACTION:
            SYS_ERR_HDL(lldp_disableAction_set(pData->action), _exit, ret);;
            break;

        case SKI_LLDP_GET_DISABLE_ACTION:
            SYS_ERR_HDL(lldp_disableAction_get(&pData->action), _exit, ret);;
            break;

        case SKI_LLDP_GET_PORT_TLV_SEL:
            SYS_ERR_HDL(lldp_portTlvSel_get(pData->lldp_portTlvSel.port, &pData->lldp_portTlvSel.lldpTlvSel), _exit, ret);;
            break;

        case SKI_LLDP_GET_TX_DELAY:
            SYS_ERR_HDL(lldp_tx_delay_get(&pData->txDelay), _exit, ret);;
            break;

        case SKI_LLDP_SET_PORT_TLV_PVID_ENABLE:
            SYS_ERR_HDL(lldp_portTlvPvidEnable_set(pData->lldp_portTlvPvidEbl.port, pData->lldp_portTlvPvidEbl.pvidEnable), _exit, ret);;
            break;

        case SKI_LLDP_GET_PORT_TLV_PVID_ENABLE:
            SYS_ERR_HDL(lldp_portTlvPvidEnable_get(pData->lldp_portTlvPvidEbl.port, &pData->lldp_portTlvPvidEbl.pvidEnable), _exit, ret);;
            break;

        case SKI_LLDP_SET_PORT_TLV_VLANMASK:
            SYS_ERR_HDL(lldp_portTlvVlanmask_set(pData->lldp_portTlvVlanmask.port, pData->lldp_portTlvVlanmask.vlanmask), _exit, ret);;
            break;

        case SKI_LLDP_GET_PORT_TLV_VLANMASK:
            SYS_ERR_HDL(lldp_portTlvVlanmask_get(pData->lldp_portTlvVlanmask.port, &pData->lldp_portTlvVlanmask.vlanmask), _exit, ret);;
            break;

        case SKI_LLDP_GET_CHANGE_TIME:
            SYS_ERR_HDL(lldp_remoteTableChangeTime_get(&pData->changeTime), _exit, ret);;
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if(cmd > SKI_LLDP_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_lldp_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_LLDP, ski_lldp_cmd_mux));

#ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_ERR_CHK(ski_lldp_med_init());
#endif

    return SYS_ERR_OK;
}

