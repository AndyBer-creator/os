
#include <rsd/rsd_erps.h>
#include <erps/proto_erps.h>
#include <ski/proto/erps/ski_erps.h>

int32 ski_erps_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_erps_ioctl_t);
    ski_erps_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_ERPS_SET_ENABLE:
            SYS_ERR_HDL(erps_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_ERPS_INST_CREATE:
            SYS_ERR_HDL(erps_inst_create(pData->instId), _exit, ret);
            break;

        case SKI_ERPS_INST_DEL:
            SYS_ERR_HDL(erps_inst_delete(pData->instId), _exit, ret);
            break;

        case SKI_ERPS_INST_CTRLVLAN:
            SYS_ERR_HDL(erps_ctrlVlan_set(pData->instId, pData->ctrlVlanId), _exit, ret);
            break;

        case SKI_ERPS_INST_WTR_TIMER:        
            SYS_ERR_HDL(erps_timer_set(ERPS_WTR_TIME, pData->instId, pData->WTRTime), _exit, ret);
            break;

        case SKI_ERPS_INST_GUARD_TIMER:
            SYS_ERR_HDL(erps_timer_set(ERPS_GUARD_TIME, pData->instId, pData->guardTime), _exit, ret);
            break;

        case SKI_ERPS_INST_WORKMODE:
            SYS_ERR_HDL(erps_workMode_set(pData->instId, pData->workMode), _exit, ret);
            break;

        case SKI_ERPS_INST_RINGID:
            SYS_ERR_HDL(erps_ringId_set(pData->instId, pData->ringId), _exit, ret);
            break;

        case SKI_ERPS_INST_RINGLEVEL:
            SYS_ERR_HDL(erps_ringLevel_set(pData->instId, pData->ringLevel), _exit, ret);
            break;

        case SKI_ERPS_INST_RINGPORT_ADD:
			if (pData->isPort0 != 0)
			{
	            SYS_ERR_HDL(erps_ringPort_set(pData->instId, pData->port0, pData->port0Role, TRUE, TRUE), _exit, ret);
			}
			else
			{
	            SYS_ERR_HDL(erps_ringPort_set(pData->instId, pData->port1, pData->port1Role, FALSE, TRUE), _exit, ret);
			}
			break;

        case SKI_ERPS_INST_RINGPORT_DEL:
			if (pData->isPort0 != 0)
			{
	            SYS_ERR_HDL(erps_ringPort_set(pData->instId, pData->port0, pData->port0Role, TRUE, FALSE), _exit, ret);
			}
			else
			{
	            SYS_ERR_HDL(erps_ringPort_set(pData->instId, pData->port1, pData->port1Role, FALSE, FALSE), _exit, ret);
			}
            break;

        case SKI_ERPS_INST_RING_PGINST_ADD:
            SYS_ERR_HDL(erps_inst_pgInst_set(pData->instId, pData->pg_inst, Erps_Protected_Inst_Add), _exit, ret);
            break;

        case SKI_ERPS_INST_RING_PGINST_DEL:
            SYS_ERR_HDL(erps_inst_pgInst_set(pData->instId, pData->pg_inst, Erps_Protected_Inst_Del), _exit, ret);
            break;

        case SKI_ERPS_INST_MEL:
            SYS_ERR_HDL(erps_mel_set(pData->instId, pData->mel), _exit, ret);
            break;

        case SKI_ERPS_INST_RINGENABLE:
            SYS_ERR_HDL(erps_ringState_set(pData->instId, pData->ringEnable), _exit, ret); 
            break;
		case SKI_ERPS_GET_RING_STATE_STATUS:
            SYS_ERR_HDL(erps_get_inst_state_status(pData->instId, &(pData->state)), _exit, ret);
            break;

        case SKI_ERPS_GET_RING_PORT_STATE_STATUS:
            SYS_ERR_HDL(erps_get_inst_port_state_status(pData->portId, &(pData->portState)), _exit, ret);
            break;    
            
        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if (cmd > SKI_ERPS_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_erps_init(void)
{

    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_ERPS, ski_erps_cmd_mux));

    return SYS_ERR_OK;
}

