
#include <lacp/proto_lacp.h>
#include <ski/proto/lacp/ski_lacp.h>

int32 ski_lacp_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_lacp_ioctl_t);
    ski_lacp_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_LACP_SET_EBL:
            SYS_ERR_HDL(lacp_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_LACP_GET_EBL:
            SYS_ERR_HDL(lacp_enable_get(&pData->enable), _exit, ret);
            break;

        case SKI_LACP_SET_SYSTEM_PRI:
            SYS_ERR_HDL(lacp_system_priority_set(pData->system_priority), _exit, ret);
            break;

        case SKI_LACP_GET_SYSTEM_PRI:
            SYS_ERR_HDL(lacp_system_priority_get(&pData->system_priority), _exit, ret);
            break;

        case SKI_LACP_SET_GROUP:
            SYS_ERR_HDL(lacp_group_set(pData->lacp_group.trunkId, pData->lacp_group.pMemberPm, pData->lacp_group.pActPm), _exit, ret);
            break;

        case SKI_LACP_SET_GROUP_DEL:
            SYS_ERR_HDL(lacp_group_del(pData->lacp_group.trunkId), _exit, ret);
            break;

        case SKI_LACP_GET_PORT_STATE:
            SYS_ERR_HDL(lacp_port_state_get(pData->portState.port, pData->portState.pPortState), _exit, ret);
            break;

        case SKI_LACP_SET_PORT_ACTIVE:
            SYS_ERR_HDL(lacp_portActivity_set(pData->portActive.port, pData->portActive.active), _exit, ret);
            break;

        case SKI_LACP_SET_PORT_PRI:
            SYS_ERR_HDL(lacp_portPriority_set(pData->portPri.port, pData->portPri.pri), _exit, ret);
            break;

        case SKI_LACP_SET_PORT_TIMEOUT:
            SYS_ERR_HDL(lacp_portTimeout_set(pData->portTimeout.port, pData->portTimeout.timeout), _exit, ret);
            break;

        case SKI_LACP_GET_BACKUP_MEMBER:
            SYS_ERR_HDL(lacp_backupMember_get(pData->lacp_group.trunkId, pData->lacp_group.pMemberPm), _exit, ret);
            break;

        case SKI_LACP_GET_PORT_COUNTER:
            SYS_ERR_HDL(lacp_portCounter_get(pData->portCounter.port, &pData->portCounter.counter), _exit, ret);
            break;

        case SKI_LACP_RESET_PORT_COUNTER:
            SYS_ERR_HDL(lacp_portCounter_reset(pData->portCounter.port), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
            break;
    }

    if(cmd > SKI_LACP_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_lacp_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_LACP, ski_lacp_cmd_mux));

    return SYS_ERR_OK;
}

