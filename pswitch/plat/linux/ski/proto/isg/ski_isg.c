
#include <ski/proto/isg/ski_isg.h>

int32 ski_isg_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_isg_ioctl_t);
    ski_isg_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_ISG_PORT_ENABLE_SET:
            SYS_ERR_HDL(isg_port_enable_set(pData->port, pData->enable, pData->vt), _exit, ret);
            break;

        case SKI_ISG_BINDING_ENTRY_SET:
            SYS_ERR_HDL(isg_bindingEntry_set(&pData->entry, FALSE), _exit, ret);
            break;

        case SKI_ISG_BINDING_ENTRY_DEL:
            SYS_ERR_HDL(isg_bindingEntry_del(&pData->entry), _exit, ret);
            break;

        case SKI_ISG_PORT_MAX_ENTRY_NUM_SET:
            SYS_ERR_HDL(isg_port_maxEntryNum_set(pData->port, pData->counter), _exit, ret);
            break;

        case SKI_ISG_DATABASE_SET:
            SYS_ERR_HDL(isg_db_set(&pData->db), _exit, ret);
            break;

        case SKI_ISG_DATABASE_TIMER_SET:
            SYS_ERR_HDL(isg_db_timer_set(pData->timer), _exit, ret);
            break;

        case SKI_ISG_BINDING_ENTRY_GET:
            SYS_ERR_HDL(isg_bindingEntryByIndex_get(pData->entryIdx, &pData->entry), _exit, ret);
            break;

        case SKI_ISG_PORT_CURRENT_ENTRY_NUM_GET:
            SYS_ERR_HDL(isg_port_currentEntryNum_get(pData->port, &pData->counter), _exit, ret);
            break;

        case SKI_ISG_MAX_ENTRY_NUM_GET:
            SYS_ERR_HDL(isg_bindingMaxEntryNum_get(&pData->counter), _exit, ret);
            break;

        case SKI_ISG_DATABASE_TIMER_GET:
            SYS_ERR_HDL(isg_db_timer_get(&pData->timer), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if(cmd > SKI_ISG_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_isg_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_ISG, ski_isg_cmd_mux));

    return SYS_ERR_OK;
}

