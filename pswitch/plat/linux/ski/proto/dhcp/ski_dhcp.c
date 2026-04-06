
#include <dhcp/proto_dhcp.h>
#include <ski/proto/dhcp/ski_dhcp.h>

int32 ski_dhcp_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_dhcp_ioctl_t);
    ski_dhcp_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
        case SKI_DHCP_SNOOPING_ENABLE_SET:
            SYS_ERR_HDL(dhcp_snooping_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_DHCP_SNOOPING_VLANENABLE_SET:
            SYS_ERR_HDL(dhcp_snooping_vlan_enable_set(pData->vid, pData->enable), _exit, ret);
            break;

        case SKI_DHCP_SNOOPING_PORTTYPE_SET:
            SYS_ERR_HDL(dhcp_snooping_port_trust_set(pData->port, pData->type), _exit, ret);
            break;

        case SKI_DHCP_SNOOPING_PORTCHADDR_CHECK_SET:
            SYS_ERR_HDL(dhcp_snooping_portChaddrChk_set(pData->port, pData->enable), _exit, ret);
            break;
#endif

        case SKI_DHCP_OPTION82_PORT_ENABLE_SET:
            SYS_ERR_HDL(dhcp_option82_enable_set(pData->port, pData->enable), _exit, ret);
            break;

        case SKI_DHCP_OPTION82_PORT_ACTION_SET:
            SYS_ERR_HDL(dhcp_option82_action_set(pData->port, pData->action), _exit, ret);
            break;

        case SKI_DHCP_OPTION82_PORT_REMOTEID_SET:
            SYS_ERR_HDL(dhcp_option82_remoteID_set(&pData->id), _exit, ret);
            break;

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
        case SKI_DHCP_OPTION82_PORT_FORMAT_SET:
            SYS_ERR_HDL(dhcp_option82_format_set(pData->port, pData->format), _exit, ret);
            break;

        case SKI_DHCP_OPTION82_PORT_CIRCUITID_SET:
            SYS_ERR_HDL(dhcp_option82_circuitID_set(pData->port, &pData->cid), _exit, ret);
            break;
#endif
        case SKI_DHCP_SNOOPING_MIB_CLEAR:
            SYS_ERR_HDL(dhcp_snooping_portMib_clear(pData->port), _exit, ret);
            break;

        case SKI_DHCP_SNOOPING_RL_SET:
            SYS_ERR_HDL(dhcp_snooping_rateLimit_set(pData->port, pData->rate), _exit, ret);
            break;

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING
        case SKI_DHCP_SNOOPING_ENABLE_GET:
            SYS_ERR_HDL(dhcp_snooping_enable_get(&pData->enable), _exit, ret);
            break;

        case SKI_DHCP_SNOOPING_VLANENABLE_GET:
            SYS_ERR_HDL(dhcp_snooping_vlan_enable_get(pData->vid, &pData->enable), _exit, ret);
            break;

        case SKI_DHCP_SNOOPING_PORTTYPE_GET:
            SYS_ERR_HDL(dhcp_snooping_port_trust_get(pData->port, &pData->type), _exit, ret);
            break;

        case SKI_DHCP_SNOOPING_PORTCHADDR_CHECK_GET:
            SYS_ERR_HDL(dhcp_snooping_portChaddrChk_get(pData->port, &pData->enable), _exit, ret);
            break;
#endif

        case SKI_DHCP_OPTION82_PORT_ENABLE_GET:
            SYS_ERR_HDL(dhcp_option82_enable_get(pData->port, &pData->enable), _exit, ret);
            break;

        case SKI_DHCP_OPTION82_PORT_ACTION_GET:
            SYS_ERR_HDL(dhcp_option82_action_get(pData->port, &pData->action), _exit, ret);
            break;
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
        case SKI_DHCP_OPTION82_PORT_FORMAT_GET:
            SYS_ERR_HDL(dhcp_option82_format_get(pData->port, &pData->format), _exit, ret);
            break;
#endif
        case SKI_DHCP_OPTION82_PORT_REMOTEID_GET:
            SYS_ERR_HDL(dhcp_option82_remoteID_get(&pData->id), _exit, ret);
            break;

        case SKI_DHCP_SNOOPING_MIB_GET:
            SYS_ERR_HDL(dhcp_snooping_portMib_get(pData->port, &pData->counter), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if(cmd > SKI_DHCP_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_dhcp_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_DHCP, ski_dhcp_cmd_mux));

    return SYS_ERR_OK;
}

