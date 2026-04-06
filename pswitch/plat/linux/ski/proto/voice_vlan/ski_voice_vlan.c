
#include <voice_vlan/proto_voice_vlan.h>
#include <ski/proto/voice_vlan/ski_voice_vlan.h>

int32 ski_voice_vlan_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_voice_vlan_ioctl_t);
    ski_voice_vlan_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_VOICE_VLAN_STATE_SET:
            SYS_ERR_HDL(voice_vlan_state_set(pData->state), _exit, ret);
            break;

        case SKI_VOICE_VLAN_STATE_GET:
            SYS_ERR_HDL(voice_vlan_state_get(&pData->state), _exit, ret);
            break;

        case SKI_VOICE_VLAN_PORT_ENABLE_SET:
            SYS_ERR_HDL(voice_vlan_port_enable_set(pData->portEnable.port, pData->portEnable.enable), _exit, ret);
            break;

        case SKI_VOICE_VLAN_PORT_ENABLE_GET:
            SYS_ERR_HDL(voice_vlan_port_enable_get(pData->portEnable.port, &pData->portEnable.enable), _exit, ret);
            break;

        case SKI_VOICE_VLAN_PORT_MODE_SET:
            SYS_ERR_HDL(voice_vlan_port_mode_set(pData->portMode.port, pData->portMode.mode), _exit, ret);
            break;

        case SKI_VOICE_VLAN_PORT_MODE_GET:
            SYS_ERR_HDL(voice_vlan_port_mode_get(pData->portMode.port, &pData->portMode.mode), _exit, ret);
            break;

        case SKI_VOICE_VLAN_COS_MODE_SET:
            SYS_ERR_HDL(voice_vlan_cos_mode_set(pData->cosMode.port, pData->cosMode.mode), _exit, ret);
            break;

        case SKI_VOICE_VLAN_COS_MODE_GET:
            SYS_ERR_HDL(voice_vlan_cos_mode_get(pData->cosMode.port, &pData->cosMode.mode), _exit, ret);
            break;

        case SKI_VOICE_VLAN_REMARK_ENABLE_SET:
            SYS_ERR_HDL(voice_vlan_1pRemark_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_VOICE_VLAN_REMARK_ENABLE_GET:
            SYS_ERR_HDL(voice_vlan_1pRemark_enable_get(&pData->enable), _exit, ret);
            break;

        case SKI_VOICE_VLAN_QOS_SET:
            SYS_ERR_HDL(voice_vlan_qos_set(pData->voiceQos), _exit, ret);
            break;

        case SKI_VOICE_VLAN_QOS_GET:
            SYS_ERR_HDL(voice_vlan_qos_get(&pData->voiceQos), _exit, ret);
            break;

        case SKI_VOICE_VLAN_AGING_SET:
            SYS_ERR_HDL(voice_vlan_aging_set(pData->aging), _exit, ret);
            break;

        case SKI_VOICE_VLAN_AGING_GET:
            SYS_ERR_HDL(voice_vlan_aging_get(&pData->aging), _exit, ret);
            break;
#if 0
        case SKI_VOICE_VLAN_SECURITY_ENABLE_SET:
            SYS_ERR_HDL(voice_vlan_security_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_VOICE_VLAN_SECURITY_ENABLE_GET:
            SYS_ERR_HDL(voice_vlan_security_enable_get(&pData->enable), _exit, ret);
            break;
#endif
        case SKI_VOICE_VLAN_OUI_SET:
            SYS_ERR_HDL(voice_vlan_oui_set(pData->ouiCfg.index, &pData->ouiCfg.oui), _exit, ret);
            break;

        case SKI_VOICE_VLAN_OUI_GET:
            SYS_ERR_HDL(voice_vlan_oui_get(pData->ouiCfg.index, &pData->ouiCfg.oui), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if (cmd > SKI_VOICEVLAN_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_voice_vlan_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_VOICEVLAN, ski_voice_vlan_cmd_mux));

    return SYS_ERR_OK;
}

