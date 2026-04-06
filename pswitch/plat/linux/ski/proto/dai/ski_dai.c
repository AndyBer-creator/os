
#include <dai/proto_dai.h>
#include <ski/proto/dai/ski_dai.h>

int32 ski_dai_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_dai_ioctl_t);
    ski_dai_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_DAI_ENABLE_SET:
            SYS_ERR_HDL(dai_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_DAI_VLANENABLE_SET:
            SYS_ERR_HDL(dai_vlan_enable_set(pData->vid, pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_TRUST_SET:
            SYS_ERR_HDL(dai_port_trust_set(pData->port, pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_SMACCHK_SET:
            SYS_ERR_HDL(dai_port_smacChk_set(pData->port, pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_DMACCHK_SET:
            SYS_ERR_HDL(dai_port_dmacChk_set(pData->port, pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_IPCHK_SET:
            SYS_ERR_HDL(dai_port_ipChk_set(pData->port, pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_IPZEROALLOW_SET:
            SYS_ERR_HDL(dai_port_ipZeroAllow_set(pData->port, pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_MIB_CLEAR:
            SYS_ERR_HDL(dai_portMib_clear(pData->port), _exit, ret);
            break;

        case SKI_DAI_VLAN_MIB_CLEAR:
            SYS_ERR_HDL(dai_vlanMib_clear(pData->vid), _exit, ret);
            break;

        case SKI_DAI_ARP_RL_SET:
            SYS_ERR_HDL(dai_arp_rateLimit_set(pData->port, pData->rate), _exit, ret);
            break;

        case SKI_DAI_ENABLE_GET:
            SYS_ERR_HDL(dai_enable_get(&pData->enable), _exit, ret);
            break;

        case SKI_DAI_VLANENABLE_GET:
            SYS_ERR_HDL(dai_vlan_enable_get(pData->vid, &pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_TRUST_GET:
            SYS_ERR_HDL(dai_port_trust_get(pData->port, &pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_SMACCHK_GET:
            SYS_ERR_HDL(dai_port_smacChk_get(pData->port, &pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_DMACCHK_GET:
            SYS_ERR_HDL(dai_port_dmacChk_get(pData->port, &pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_IPCHK_GET:
            SYS_ERR_HDL(dai_port_ipChk_get(pData->port, &pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_IPZEROALLOW_GET:
            SYS_ERR_HDL(dai_port_ipZeroAllow_get(pData->port, &pData->enable), _exit, ret);
            break;

        case SKI_DAI_PORT_MIB_GET:
            SYS_ERR_HDL(dai_portMib_get(pData->port, &pData->counter), _exit, ret);
            break;

        case SKI_DAI_VLAN_MIB_GET:
            SYS_ERR_HDL(dai_vlanMib_get(pData->vid, &pData->counter), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if(cmd > SKI_DAI_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_dai_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_DAI, ski_dai_cmd_mux));

    return SYS_ERR_OK;
}

