
#include <ski/proto/lldp/ski_lldp_med.h>

int32 ski_lldp_med_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_lldp_med_ioctl_t);
    ski_lldp_med_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {

        case SKI_LLDP_MED_GET_PORT_MED_LOCAL_ENTRYINFO:
            SYS_ERR_HDL(lldp_portMedLocalEntryInfo_get(pData->localMedEntryInfo.port, &pData->localMedEntryInfo.entryInfo), _exit, ret);
            break;
        case SKI_LLDP_MED_GET_PORT_MED_REMOTE_ENTRYINFO:
            SYS_ERR_HDL(lldp_portMedRemoteEntryInfo_get(pData->remoteMedEntryInfo.port, pData->remoteMedEntryInfo.index, &pData->remoteMedEntryInfo.entryInfo), _exit, ret);
            break;
        case SKI_LLDP_MED_SET_MED_FAST_REPEAT_COUNT:
            SYS_ERR_HDL(lldp_medFastRepeatCount_set(pData->fastRepeatCount), _exit, ret);
            break;
        case SKI_LLDP_MED_SET_MED_NETWORK_POLICY:
            SYS_ERR_HDL(lldp_medNetworkPolicy_set(pData->lldp_medNetworkPolicy.idx, pData->lldp_medNetworkPolicy.policy), _exit, ret);
            break;
        case SKI_LLDP_MED_SET_MED_NETWORK_POLICY_AUTO:
            SYS_ERR_HDL(lldp_medNetworkPolicyAuto_set(pData->networkPolicyAuto), _exit, ret);
            break;
        case SKI_LLDP_MED_SET_PORT_MED_TLV_SEL:
            SYS_ERR_HDL(lldp_portMedTlvSel_set(pData->lldp_portMedTlvSel.port, pData->lldp_portMedTlvSel.lldpMedTlvSel), _exit, ret);
            break;
        case SKI_LLDP_MED_SET_PORT_MED_ENABLE:
            SYS_ERR_HDL(lldp_portMedEnable_set(pData->lldp_portMedEnable.port, pData->lldp_portMedEnable.enable), _exit, ret);
            break;
        case SKI_LLDP_MED_SET_PORT_MED_NETWORK_POLICY_BIND:
            SYS_ERR_HDL(lldp_portMedNetworkPolicyBind_set(pData->lldp_portMedNetworkPolicyBind.port,
                pData->lldp_portMedNetworkPolicyBind.policyBind), _exit, ret);
            break;
        case SKI_LLDP_MED_SET_PORT_MED_LOCATION:
            SYS_ERR_HDL(lldp_portMedLocation_set(pData->lldp_portMedLocation.port, pData->lldp_portMedLocation.location), _exit, ret);
            break;
        case SKI_LLDP_MED_SET_MED_INVENTORY:
            SYS_ERR_HDL(lldp_medInventory_set(pData->lldp_medInventory.inventoryType, pData->lldp_medInventory.strData), _exit, ret);
            break;
        case SKI_LLDP_MED_GET_MED_FAST_REPEAT_COUNT:
            SYS_ERR_HDL(lldp_medFastRepeatCount_get(&pData->fastRepeatCount), _exit, ret);
            break;
        case SKI_LLDP_MED_GET_MED_NETWORK_POLICY:
            SYS_ERR_HDL(lldp_medNetworkPolicy_get(pData->lldp_medNetworkPolicy.idx, &pData->lldp_medNetworkPolicy.policy), _exit, ret);
            break;
        case SKI_LLDP_MED_GET_MED_NETWORK_POLICY_AUTO:
            SYS_ERR_HDL(lldp_medNetworkPolicyAuto_get(&pData->networkPolicyAuto), _exit, ret);
            break;
        case SKI_LLDP_MED_GET_PORT_MED_TLV_SEL:
            SYS_ERR_HDL(lldp_portMedTlvSel_get(pData->lldp_portMedTlvSel.port, &pData->lldp_portMedTlvSel.lldpMedTlvSel), _exit, ret);
            break;
        case SKI_LLDP_MED_GET_PORT_MED_ENABLE:
            SYS_ERR_HDL(lldp_portMedEnable_get(pData->lldp_portMedEnable.port, &pData->lldp_portMedEnable.enable), _exit, ret);
            break;
        case SKI_LLDP_MED_GET_PORT_MED_NETWORK_POLICY_BIND:
            SYS_ERR_HDL(lldp_portMedNetworkPolicyBind_get(pData->lldp_portMedNetworkPolicyBind.port,
                &pData->lldp_portMedNetworkPolicyBind.policyBind), _exit, ret);
            break;
        case SKI_LLDP_MED_GET_PORT_MED_LOCATION:
            SYS_ERR_HDL(lldp_portMedLocation_get(pData->lldp_portMedLocation.port, &pData->lldp_portMedLocation.location), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if(cmd > SKI_LLDP_MED_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_lldp_med_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_LLDP_MED, ski_lldp_med_cmd_mux));

    return SYS_ERR_OK;
}

