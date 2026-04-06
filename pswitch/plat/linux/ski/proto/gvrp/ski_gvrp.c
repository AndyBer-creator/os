
#include <gvrp/proto_gvrp.h>
#include <ski/proto/gvrp/ski_gvrp.h>
#include <rsd/rsd_gvrp.h>

int32 ski_gvrp_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_gvrp_ioctl_t);
    ski_gvrp_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_GVRP_ENABLE_SET:
                SYS_ERR_HDL(gvrp_enable_set(pData->enable), _exit, ret);
                break;

        case SKI_GVRP_PORT_ENABLE_SET:
                SYS_ERR_HDL(gvrp_port_enable_set(pData->portEn.port, pData->portEn.enable), _exit, ret);
                break;

        case SKI_GVRP_PORT_REGMODE_SET:
                SYS_ERR_HDL(gvrp_port_regmode_set(pData->regmode.port, pData->regmode.regval), _exit, ret);
                break;

        case SKI_GVRP_PORT_FORBIDVLAN_SET:
                SYS_ERR_HDL(gvrp_port_forbidVlan_set(pData->forbidVlanEn.port, pData->forbidVlanEn.enable), _exit, ret);
                break;

        case SKI_GVRP_VLAN_SET:
                SYS_ERR_HDL(gvrp_vlan_db_set(pData->vlandb.vid, &pData->vlandb), _exit, ret);
                break;

        case SKI_GVRP_TIME_SET:
                SYS_ERR_HDL(gvrp_timer_set(pData->timer.type, pData->timer.val), _exit, ret);
                break;

        case SKI_GVRP_STAT_CLEAR:
                SYS_ERR_HDL(gvrp_statistics_clear(pData->stat.port), _exit, ret);
                break;

        case SKI_GVRP_ERROR_STAT_CLEAR:
                SYS_ERR_HDL(gvrp_error_statistics_clear(pData->errorStat.port), _exit, ret);
                break;

        case SKI_GVRP_INFO_GET:
                SYS_ERR_HDL(gvrp_info_get(&pData->info), _exit, ret);
                break;

        case SKI_GVRP_VLAN_GET:
                SYS_ERR_HDL(gvrp_vlan_db_get(pData->vlandb.vid, &pData->vlandb), _exit, ret);
                break;

        case SKI_GVRP_PORT_STAT_GET:
                SYS_ERR_HDL(gvrp_port_statistics_get(pData->stat.port, &pData->stat.statistic), _exit, ret);
                break;

        case SKI_GVRP_PORT_ERROR_STAT_GET:
                SYS_ERR_HDL(gvrp_port_error_statistics_get(pData->errorStat.port, &pData->errorStat.statistic), _exit, ret);
                break;

        case SKI_GVRP_VLAN_MASK_GET:
                SYS_ERR_HDL(gvrp_vlanMaskExist_get(&pData->vlanMask), _exit, ret);
                break;

        case SKI_GVRP_PORT_VLAN_MACHINE_GET:
                SYS_ERR_HDL(gvrp_gidtt_machine_get(pData->status.port, pData->status.vid, &pData->status.machine), _exit, ret);
                break;

        case SKI_GVRP_VLAN_DB_SIZE_GET:
                SYS_ERR_HDL(gvrp_vlan_db_size_get(&pData->dbsize), _exit, ret);
                break;

        default:
            SYS_PRINTF("Unknown GVRP command option !!\n");
            ret = SYS_ERR_FAILED;
            goto _exit;
        break;
    }

    if(cmd > SKI_GVRP_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_gvrp_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_GVRP, ski_gvrp_cmd_mux));

    return SYS_ERR_OK;
}

