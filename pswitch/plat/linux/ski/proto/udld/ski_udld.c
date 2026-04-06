
#include <udld/proto_udld.h>
#include <ski/proto/udld/ski_udld.h>

int32 ski_udld_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_udld_ioctl_t);
    ski_udld_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_UDLD_FIBER_MODE_SET:
            SYS_ERR_HDL(udld_fiberMode_set(pData->udldFiberMode), _exit, ret);
            break;

        case SKI_UDLD_PORT_MODE_SET:
            SYS_ERR_HDL(udld_portMode_set(pData->udldPortMode.port, pData->udldPortMode.mode, pData->udldPortMode.isFollow), _exit, ret);
            break;

        case SKI_UDLD_MESSAGE_TIME_SET:
            SYS_ERR_HDL(udld_messageTime_set(pData->messageTime), _exit, ret);
            break;

        case SKI_UDLD_FIBER_MODE_GET:
            SYS_ERR_HDL(udld_fiberMode_get(&pData->udldFiberMode), _exit, ret);
            break;

        case SKI_UDLD_PORT_MODE_GET:
            SYS_ERR_HDL(udld_portMode_get(pData->udldPortMode.port, &pData->udldPortMode.mode, &pData->udldPortMode.isFollow), _exit, ret);
            break;

        case SKI_UDLD_MESSAGE_TIME_GET:
            SYS_ERR_HDL(udld_messageTime_get(&pData->messageTime), _exit, ret);
            break;

        case SKI_UDLD_PORT_OPER_MODE_GET:
            SYS_ERR_HDL(udld_portOperMode_get(pData->udldPortOperMode.port, &pData->udldPortOperMode.mode), _exit, ret);
            break;

        case SKI_UDLD_PORT_BIDIR_STATE_GET:
            SYS_ERR_HDL(udld_portBidiretionState_get(pData->udldPortBidirState.port, &pData->udldPortBidirState.bidirState), _exit, ret);
            break;

        case SKI_UDLD_PORT_OPER_STATE_GET:
            SYS_ERR_HDL(udld_portOperState_get(pData->udldPortOperState.port, &pData->udldPortOperState.operational_state), _exit, ret);
            break;

        case SKI_UDLD_PORT_NEIGHBOR_NUMBER_GET:
            SYS_ERR_HDL(udld_portNeighborNumber_get(pData->udldPortNeighborNumber.port, &pData->udldPortNeighborNumber.neighborNum), _exit, ret);
            break;

        case SKI_UDLD_PORT_NEIGHBOR_GET:
            SYS_ERR_HDL(udld_portNeighbor_get(pData->udldPortNeighbor.port, pData->udldPortNeighbor.neighborId, &pData->udldPortNeighbor.nbrInfo), _exit, ret);
            break;

        case SKI_UDLD_PORT_NEIGHBOR_ECHO_GET:
            SYS_ERR_HDL(udld_portNeighborEcho_get(pData->udldPortNeighborEcho.port, pData->udldPortNeighborEcho.neighborId,
                pData->udldPortNeighborEcho.echoId, &pData->udldPortNeighborEcho.echoInfo), _exit, ret);
            break;

        case SKI_UDLD_ERROR_DISABLE_RESET:
            SYS_ERR_HDL(udld_error_disable_reset(), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if (cmd > SKI_UDLD_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_udld_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_UDLD, ski_udld_cmd_mux));

    return SYS_ERR_OK;
}

