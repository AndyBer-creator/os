
#include <libcfg/cfg.h>

int32 cfg_mld_enable_get(sys_enable_t *pEnable)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    *pEnable = pMld->enable;

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_enable_set(sys_enable_t enable)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    pMld->enable = enable;

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_lookupMode_set(igmp_lookup_mode_t mode)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK((mode < IGMP_DMAC_VID_MODE || mode >= IGMP_LOOKUP_MODE_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    pMld->mode = mode;

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_lookupMode_get(igmp_lookup_mode_t *mode)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK((NULL == mode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    *mode = pMld->mode;

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_unknMcastAction_get(igmp_unknMcastAction_t *pAction)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK((NULL == pAction), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    *pAction = pMld->unknMcastAction;

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_unknMcastAction_set(igmp_unknMcastAction_t action)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK((action >= IGMP_UNKNOWN_MCAST_ACTION_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    pMld->unknMcastAction = action;

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_suppre_enable_set(sys_enable_t enable)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    pMld->suppreEnable = enable;

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_suppre_enable_get(sys_enable_t *pEnable)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    *pEnable = pMld->suppreEnable;

    CFG_CLOSE(CFG_ID_MLD);
}

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

int32 cfg_mld_portGroup_limit_set(sys_logic_port_t port , uint16 maxnum)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    pMld->portLimit[port] = maxnum;

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_portGroup_limit_get(sys_logic_port_t port, uint16 *pMaxnum)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMaxnum), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    *pMaxnum = pMld->portLimit[port];

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_portGroup_excessAct_set(sys_logic_port_t port, igmp_maxGroupAct_t act)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((act >= IGMP_ACT_END), SYS_ERR_MLD_MAX_GROUP_ACT);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    pMld->portExAct[port] = act;

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_portGroup_excessAct_get(sys_logic_port_t port, igmp_maxGroupAct_t *pAct)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pAct), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    *pAct = pMld->portExAct[port];

    CFG_CLOSE(CFG_ID_MLD);
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

int32 cfg_mld_profile_get(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_MLD_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    osal_memcpy(pProfile, &pMld->igmpProfile[profileId-1], sizeof(sys_igmp_profile_t));

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_profile_set(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_MLD_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    osal_memcpy(&pMld->igmpProfile[profileId-1], pProfile, sizeof(sys_igmp_profile_t));

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_port_filter_get(sys_logic_port_t port, sys_igmp_profileId_t *pProfileId)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pProfileId), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    *pProfileId = pMld->portProfileId[port];

    CFG_CLOSE(CFG_ID_MLD);
}

int32 cfg_mld_port_filter_set(sys_logic_port_t port, sys_igmp_profileId_t  profileId)
{
    cfg_mld_t *pMld = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(profileId > BOND_MCAST_PROFILE_MAX, SYS_ERR_MLD_PROFILE_ID);

    CFG_OPEN(CFG_ID_MLD, (cfg_addr)&pMld);

    pMld->portProfileId[port] = profileId;

    CFG_CLOSE(CFG_ID_MLD);
}
#endif

