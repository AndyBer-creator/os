
#include <libsal/sal_util.h>
#include <libsal/sal_vlan_proto.h>

const sys_text_t text_protoVlan_frameType[] =
{
    {              "ethernet_ii" },
    {               "snap_1042" },
    {             "snap_8021h" },
    {             "snap_other" },
    {              "llc_other" },
};

int32 sal_vlan_protoGroup_get(uint32 protoGroup_idx, sys_vlan_protoGroup_t *pProtoGroup)
{
    SYS_PARAM_CHK((protoGroup_idx >= CAPA_VLAN_PROTO_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pProtoGroup), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_protoGroup_get(protoGroup_idx, pProtoGroup));

    return SYS_ERR_OK;
} 

int32 sal_vlan_protoGroup_set(uint32 protoGroup_idx, sys_vlan_protoGroup_t *pProtoGroup)
{
    uint32 index;
    sys_vlan_protoGroup_t protoGroup;

    SYS_PARAM_CHK((protoGroup_idx >= CAPA_VLAN_PROTO_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pProtoGroup), SYS_ERR_NULL_POINTER);

    if (FRAME_TYPE_END != pProtoGroup->frametype)
    {
        for (index = 0; index < CAPA_VLAN_PROTO_GROUP_NUM; index++)
        {
            if (index == protoGroup_idx)
                continue;

            SYS_ERR_CHK(sal_vlan_protoGroup_get(index, &protoGroup));

            if ((protoGroup.frametype == pProtoGroup->frametype) &&
                (protoGroup.framevalue == pProtoGroup->framevalue))
            {
                return SYS_ERR_ENTRY_IS_EXIST;
            }
        }
    }

    SYS_ERR_CHK(rsd_vlan_protoGroup_set(protoGroup_idx, pProtoGroup));
    SYS_ERR_CHK(cfg_vlan_protoGroup_set(protoGroup_idx, pProtoGroup));

    return SYS_ERR_OK;
}

int32 sal_vlan_portProtoVlan_get(sys_logic_port_t port, uint32 protoGroup_idx, sys_vlan_protoVlanCfg_t *pVlan_cfg, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((protoGroup_idx >= CAPA_VLAN_PROTO_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pVlan_cfg), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_portProtoVlan_get(port, protoGroup_idx, pVlan_cfg, pEnable));

    return SYS_ERR_OK;
}

int32 sal_vlan_portProtoVlan_set(sys_logic_port_t port, uint32 protoGroup_idx, sys_vlan_protoVlanCfg_t *pVlan_cfg, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((protoGroup_idx >= CAPA_VLAN_PROTO_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pVlan_cfg), SYS_ERR_NULL_POINTER);

    if(ENABLED == enable)
    {
        if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_GENERAL)))
            return SYS_ERR_VLAN_PROVLAN_PORT_MODE;

        SYS_ERR_TRK_CHK(rsd_vlan_portProtoVlan_set(port, protoGroup_idx, pVlan_cfg));
    }
    else
        SYS_ERR_TRK_CHK(rsd_vlan_portProtoVlan_disable(port, protoGroup_idx));

    SYS_ERR_CHK(cfg_vlan_portProtoVlan_set(port, protoGroup_idx, pVlan_cfg, enable));

    return SYS_ERR_OK;
}

