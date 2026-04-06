
#include <libcfg/cfg.h>

int32 cfg_vlan_protoGroup_get( uint32 protoGroup_idx, sys_vlan_protoGroup_t *pProtoGroup)
{
    cfg_vlan_proto_t *pVlan = NULL;

    SYS_PARAM_CHK((protoGroup_idx >= CAPA_VLAN_PROTO_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pProtoGroup), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_PROTO, (cfg_addr)&pVlan);

    osal_memcpy(pProtoGroup, &pVlan->groups[protoGroup_idx], sizeof(sys_vlan_protoGroup_t));

    CFG_CLOSE(CFG_ID_VLAN_PROTO);
}

int32 cfg_vlan_protoGroup_set( uint32 protoGroup_idx, sys_vlan_protoGroup_t *pProtoGroup)
{
    cfg_vlan_proto_t *pVlan = NULL;

    SYS_PARAM_CHK((protoGroup_idx >= CAPA_VLAN_PROTO_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pProtoGroup), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_PROTO, (cfg_addr)&pVlan);

    osal_memcpy(&pVlan->groups[protoGroup_idx], pProtoGroup, sizeof(sys_vlan_protoGroup_t));

    CFG_CLOSE(CFG_ID_VLAN_PROTO);
}

int32 cfg_vlan_portProtoVlan_get(sys_logic_port_t port, uint32 protoGroup_idx, sys_vlan_protoVlanCfg_t *pVlan_cfg, sys_enable_t *pEnable)
{
    cfg_vlan_proto_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((protoGroup_idx >= CAPA_VLAN_PROTO_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pVlan_cfg), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_PROTO, (cfg_addr)&pVlan);

    osal_memcpy(pVlan_cfg, &pVlan->pvlanPortCfg[port][protoGroup_idx].vlan_cfg, sizeof(sys_vlan_protoVlanCfg_t));
    *pEnable = pVlan->pvlanPortCfg[port][protoGroup_idx].enable;

    CFG_CLOSE(CFG_ID_VLAN_PROTO);
}

int32 cfg_vlan_portProtoVlan_set(sys_logic_port_t port, uint32 protoGroup_idx, sys_vlan_protoVlanCfg_t *pVlan_cfg, sys_enable_t enable)
{
    cfg_vlan_proto_t *pVlan = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((protoGroup_idx >= CAPA_VLAN_PROTO_GROUP_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pVlan_cfg), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_VLAN_PROTO, (cfg_addr)&pVlan);

    osal_memcpy(&pVlan->pvlanPortCfg[port][protoGroup_idx].vlan_cfg, pVlan_cfg, sizeof(sys_vlan_protoVlanCfg_t));
    pVlan->pvlanPortCfg[port][protoGroup_idx].enable = enable;

    CFG_CLOSE(CFG_ID_VLAN_PROTO);
}

