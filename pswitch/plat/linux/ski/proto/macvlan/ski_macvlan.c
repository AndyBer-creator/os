
#include <macvlan/proto_macvlan.h>
#include <ski/proto/macvlan/ski_macvlan.h>

int32 ski_macvlan_cmd_mux(unsigned int cmd, unsigned long arg)
{
    int32 ret = SYS_ERR_OK;
    uint32 dataSize = sizeof(ski_macvlan_ioctl_t);
    ski_macvlan_ioctl_t *pData = NULL;

    SYS_MEM_ALLOC_HDL(pData, dataSize, _exit, ret);

    copy_from_user(pData, (int __user*)arg, dataSize);

    switch(cmd)
    {
        case SKI_MACVLAN_GROUP_SET:
            SYS_ERR_HDL(macvlan_group_set(&pData->group.group), _exit, ret);
            break;

        case SKI_MACVLAN_GROUP_DEL:
            SYS_ERR_HDL(macvlan_group_del(&pData->group.group), _exit, ret);
            break;

        case SKI_MACVLAN_MAPPING_SET:
            SYS_ERR_HDL(macvlan_mapping_set(pData->mapping.port, &pData->mapping.mapping[0]), _exit, ret);
            break;

        case SKI_MACVLAN_MAPPING_DEL:
            SYS_ERR_HDL(macvlan_mapping_del(pData->mapping.port, &pData->mapping.mapping[0]), _exit, ret);
            break;

        case SKI_MACVLAN_GROUP_GET:
            SYS_ERR_HDL(macvlan_group_get(pData->group.idx, &pData->group.group), _exit, ret);
            break;

        case SKI_MACVLAN_MAPPING_GET:
            SYS_ERR_HDL(macvlan_mapping_get(pData->mapping.port, pData->mapping.mapping), _exit, ret);
            break;

        default:
            ret = SYS_ERR_FAILED;
            goto _exit;
    }

    if(cmd > SKI_MACVLAN_GET_BASE)
        copy_to_user((int __user*)arg, pData, dataSize);

_exit:
    SYS_MEM_FREE(pData);

    return ret;
}

int32 ski_mac_vlan_init(void)
{
    SYS_ERR_CHK(ski_handler_register(SKI_CORE_MOD_MACVLAN, ski_macvlan_cmd_mux));

    return SYS_ERR_OK;
}

