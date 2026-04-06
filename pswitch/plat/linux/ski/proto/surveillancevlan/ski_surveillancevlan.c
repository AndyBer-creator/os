
#include <surveillancevlan/proto_surveillancevlan.h>
#include <ski/proto/surveillancevlan/ski_surveillancevlan.h>

int32 ski_surveillance_vlan_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_surveillance_vlan_ioctl_t);
    ski_surveillance_vlan_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_SV_VID_SET:
            SYS_ERR_HDL(surveillance_vlanVid_set(pData->vid), _exit, ret);
            break;

        case SKI_SV_PRI_SET:
            SYS_ERR_HDL(surveillance_vlanPri_set(pData->pri), _exit, ret);
            break;

        case SKI_SV_STATE_SET:
            SYS_ERR_HDL(surveillance_vlanState_set(pData->state), _exit, ret);
            break;

        case SKI_SV_AGING_SET:
            SYS_ERR_HDL(surveillance_vlanAging_set(pData->aging), _exit, ret);
            break;

        case SKI_SV_OUI_SET:
            SYS_ERR_HDL(surveillance_vlanOuiByIdx_set(pData->idx, &pData->oui), _exit, ret);
            break;

        case SKI_SV_REMARK_ENABLE_SET:
            SYS_ERR_HDL(surveillance_vlan1pRemarkEnable_set(pData->state), _exit, ret);
            break;

        case SKI_SV_PORT_ENABLE_SET:
            SYS_ERR_HDL(surveillance_vlan_portEnable_set(pData->lp, pData->state), _exit, ret);
            break;

        case SKI_SV_PORT_MODE_SET:
            SYS_ERR_HDL(surveillance_vlan_portMode_set(pData->lp, pData->portMode), _exit, ret);
            break;

        case SKI_SV_PORT_COS_MODE_SET:
            SYS_ERR_HDL(surveillance_vlan_portCosMode_set(pData->lp, pData->portCosMode), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if (cmd > SKI_SV_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_surveillance_vlan_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_SURVEILLANCEVLAN, ski_surveillance_vlan_cmd_mux));

    return SYS_ERR_OK;
}

