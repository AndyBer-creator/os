
#include <ski/proto/igmp/ski_mld.h>

int32 ski_mld_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_mld_ioctl_t);
    ski_mld_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_MLD_ENABLE_SET:
            SYS_ERR_HDL(mcast_mld_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_MLD_SUPPRE_ENABLE_SET:
            SYS_ERR_HDL(mcast_mld_suppre_enable_set(pData->enable), _exit, ret);
            break;

        case SKI_MLD_STATISTICS_RESET:
            SYS_ERR_HDL(mcast_mld_statistics_clear(), _exit, ret);
            break;

        case SKI_MLD_LOOKUP_MODE_SET:
            SYS_ERR_HDL(mcast_mld_lookupMode_set(pData->lookup_mode), _exit, ret);
            break;

        case SKI_MLD_GROUP_TYPE_CLEAR:
            SYS_ERR_HDL(mcast_mld_group_del_byType(pData->type), _exit, ret);
            break;

        case SKI_MLD_VERSION_SET:
            SYS_ERR_HDL(mcast_mld_operVersion_set(pData->operVersion), _exit, ret);
            break;

        case SKI_MLD_UNKNOWN_MCAST_ACTION_SET:
            SYS_ERR_HDL(mcast_mld_unknMcastAction_set(pData->unknMcastActn), _exit, ret);
            break;

        case SKI_MLD_INFO_GET:
            SYS_ERR_HDL(mcast_mld_info_get(&pData->info), _exit, ret);
            break;

        case SKI_MLD_LOOKUP_MODE_GET:
            SYS_ERR_HDL(mcast_mld_lookupMode_get(&pData->lookup_mode), _exit, ret);
            break;

        case SKI_MLD_DBSIZE_GET:
            SYS_ERR_HDL(mcast_mld_databaseSize_get(&pData->dbSize), _exit, ret);
            break;

        default:
            SYS_PRINTF("Unknown MLD command option !!\n");
            ret = SYS_ERR_FAILED;
            goto _exit;
        break;
    }

    if(cmd > SKI_MLD_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_mld_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_MLD, ski_mld_cmd_mux));

    return SYS_ERR_OK;
}

