
#include <libcfg/cfg.h>

int32 cfg_lldp_medFastRepeatCount_set(uint8 fastRepeatCount)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(fastRepeatCount, BOND_LLDP_MED_FAST_REPEAT_COUNT_MIN, BOND_LLDP_MED_FAST_REPEAT_COUNT_MAX), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    pLldp->fastRepeatCount = fastRepeatCount;

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_medFastRepeatCount_get(uint8 *pFastRepeatCount)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK((NULL == pFastRepeatCount), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    *pFastRepeatCount = pLldp->fastRepeatCount;

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_medNetworkPolicy_set(uint32 idx, lldp_med_networkPolicy_t policy)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK((idx >= CAPA_LLDP_MED_NETWORK_POLICY_NUM), SYS_ERR_OUT_OF_RANGE);
    if(TRUE == policy.valid)
    {
        SYS_PARAM_CHK((policy.app >= LLDP_APP_END), SYS_ERR_INPUT);
        SYS_PARAM_CHK(((((policy.vid) < BOND_LLDP_MED_NETWORK_POLICY_VID_MIN) ||
            ((policy.vid) > BOND_LLDP_MED_NETWORK_POLICY_VID_MAX)) ? 1 : 0), SYS_ERR_VLAN_ID);
        SYS_PARAM_CHK((policy.userPriority > BOND_LLDP_MED_USER_PRI_MAX), SYS_ERR_OUT_OF_RANGE);
        SYS_PARAM_CHK((policy.dscpValue > BOND_LLDP_MED_DSCP_PRI_MAX), SYS_ERR_OUT_OF_RANGE);
    }

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    osal_memcpy(&pLldp->policy[idx] , &policy, sizeof(policy));

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_medNetworkPolicy_get(uint32 idx, lldp_med_networkPolicy_t *pPolicy)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK((idx >= CAPA_LLDP_MED_NETWORK_POLICY_NUM), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pPolicy), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    *pPolicy = pLldp->policy[idx];

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_medNetworkPolicyAuto_set(sys_enable_t enable)
{
    cfg_lldp_med_t *pLldp = NULL;

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    pLldp->networkPolicyAuto = enable;

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_medNetworkPolicyAuto_get(sys_enable_t *pEnable)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    *pEnable = pLldp->networkPolicyAuto;

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_portMedTlvSel_set(sys_logic_port_t port, lldp_med_tlv_sel_t lldpMedTlvSel)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    pLldp->lldpMedTlvSel[port] = lldpMedTlvSel;

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_portMedTlvSel_get(sys_logic_port_t port, lldp_med_tlv_sel_t *pLldpMedTlvSel)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLldpMedTlvSel), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    *pLldpMedTlvSel = pLldp->lldpMedTlvSel[port];

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_portMedEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    pLldp->medEnable[port] = enable;

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_portMedEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    *pEnable = pLldp->medEnable[port];

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_portMedNetworkPolicyBind_set(sys_logic_port_t port, lldp_med_networkPolicy_bind_t policyBind)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    pLldp->policyBind[port] = policyBind;

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_portMedNetworkPolicyBind_get(sys_logic_port_t port, lldp_med_networkPolicy_bind_t *pPolicyBind)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPolicyBind), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    *pPolicyBind = pLldp->policyBind[port];

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_portMedLocation_set(sys_logic_port_t port, lldp_med_location_t location)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    osal_memcpy(&pLldp->location[port], &location, sizeof(location));

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

int32 cfg_lldp_portMedLocation_get(sys_logic_port_t port, lldp_med_location_t *pLocation)
{
    cfg_lldp_med_t *pLldp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLocation), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LLDP_MED, (cfg_addr)&pLldp);

    *pLocation = pLldp->location[port];

    CFG_CLOSE(CFG_ID_LLDP_MED);
}

