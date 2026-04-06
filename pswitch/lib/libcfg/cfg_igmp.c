
#include <libcfg/cfg.h>

int32 cfg_igmp_enable_get(sys_enable_t *pEnable)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pEnable = pIgmp->enable;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_enable_set(sys_enable_t enable)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->enable = enable;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_suppre_enable_get(sys_enable_t *pEnable)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pEnable = pIgmp->suppreEnable;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_suppre_enable_set(sys_enable_t enable)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->suppreEnable = enable;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_lookupMode_set(igmp_lookup_mode_t mode)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((mode < IGMP_DMAC_VID_MODE || mode >= IGMP_LOOKUP_MODE_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->mode = mode;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_lookupMode_get(igmp_lookup_mode_t *mode)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == mode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *mode = pIgmp->mode;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_unknMcastAction_get(igmp_unknMcastAction_t *pAction)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pAction), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pAction = pIgmp->unknMcastAction;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_unknMcastAction_set(igmp_unknMcastAction_t action)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((action >= IGMP_UNKNOWN_MCAST_ACTION_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->unknMcastAction = action;

    CFG_CLOSE(CFG_ID_IGMP);
}
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER

int32 cfg_igmp_querier_set(sys_igmp_querier_t *pQuerier)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pQuerier), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    if (ENABLED == pQuerier->enabled)
        VLANMASK_SET_VID(pIgmp->querier, pQuerier->vid);
    else
        VLANMASK_CLEAR_VID(pIgmp->querier, pQuerier->vid);

    pIgmp->querierVersion = pQuerier->version;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_querier_get(sys_vlanmask_t *pQuerier)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pQuerier), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    osal_memcpy(pQuerier, &pIgmp->querier, sizeof(sys_vlanmask_t));

    CFG_CLOSE(CFG_ID_IGMP);
}
#endif

int32 cfg_igmp_operVersion_set(igmp_version_t operVersion)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((operVersion >= IGMP_VERSION_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->operVersion = operVersion;

    CFG_CLOSE(CFG_ID_IGMP);
}

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

int32 cfg_igmp_portGroup_limit_set(sys_logic_port_t port , uint16 maxnum)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->portLimit[port] = maxnum;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_portGroup_limit_get(sys_logic_port_t port, uint16 *pMaxnum)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMaxnum), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pMaxnum = pIgmp->portLimit[port];

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_portGroup_excessAct_set(sys_logic_port_t port, igmp_maxGroupAct_t act)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((act >= IGMP_ACT_END), SYS_ERR_IGMP_MAX_GROUP_ACT);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->portExAct[port] = act;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_portGroup_excessAct_get(sys_logic_port_t port, igmp_maxGroupAct_t *pAct)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pAct), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pAct = pIgmp->portExAct[port];

    CFG_CLOSE(CFG_ID_IGMP);
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

int32 cfg_igmp_profile_get(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_IGMP_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    osal_memcpy(pProfile, &pIgmp->igmpProfile[profileId-1], sizeof(sys_igmp_profile_t));

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_profile_set(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_IGMP_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    osal_memcpy(&pIgmp->igmpProfile[profileId-1], pProfile, sizeof(sys_igmp_profile_t));

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_port_filter_get(sys_logic_port_t port, sys_igmp_profileId_t *pProfileId)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pProfileId), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pProfileId = pIgmp->portFilterProfileId[port];

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_port_filter_set(sys_logic_port_t port, sys_igmp_profileId_t  profileId)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(profileId > BOND_MCAST_PROFILE_MAX, SYS_ERR_IGMP_PROFILE_ID);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->portFilterProfileId[port] = profileId;

    CFG_CLOSE(CFG_ID_IGMP);
}
#endif

#ifdef CONFIG_SYS_PROTO_MVR

int32 cfg_igmp_mvr_enable_get(sys_enable_t *pEnable)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pEnable = pIgmp->mvrConfig.mvr_enable;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_enable_set(sys_enable_t enable)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->mvrConfig.mvr_enable = enable;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_mode_get(mvr_mode_t *pMode)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pMode = pIgmp->mvrConfig.mode;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_mode_set(mvr_mode_t mode)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK( (mode < MVR_MODE_COMPATABLE) ||(mode >= MVR_MODE_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->mvrConfig.mode = mode;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_querytime_get(uint32 *pValue)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pValue), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pValue = pIgmp->mvrConfig.queryTime;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_querytime_set(uint32 queryTime)
{
    cfg_igmp_t *pIgmp = NULL;

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->mvrConfig.queryTime = queryTime;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_vlanId_get(sys_vid_t  *pVid)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pVid = pIgmp->mvrConfig.mvr_vid;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_vlanId_set(sys_vid_t vid)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->mvrConfig.mvr_vid = vid;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_groupRange_get(uint32 *pStart, uint32 *pEnd)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK((NULL == pStart), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnd), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pStart = pIgmp->mvrConfig.groupIp_start.ipv4;
    *pEnd = pIgmp->mvrConfig.groupIp_end.ipv4;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_groupRange_set(uint32 startIp, uint32 endIp)
{
    cfg_igmp_t *pIgmp = NULL;

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->mvrConfig.groupIp_start.ipv4 = startIp;
    pIgmp->mvrConfig.groupIp_end.ipv4 = endIp;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_portType_get(sys_logic_port_t port , mvr_portType_t *pType)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pType), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pType = pIgmp->mvrConfig.type[port];

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_portType_set(sys_logic_port_t port , mvr_portType_t type)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((type < MVR_TYPE_NONE) || (type >= MVR_TYPE_END) , SYS_ERR_MVR_TYPE);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->mvrConfig.type[port] = type;

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_portImmediate_get(sys_logic_port_t port , sys_enable_t *pEnable)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    *pEnable = pIgmp->mvrConfig.fastLeaveEnable[port];

    CFG_CLOSE(CFG_ID_IGMP);
}

int32 cfg_igmp_mvr_portImmediate_set(sys_logic_port_t port , sys_enable_t enable)
{
    cfg_igmp_t *pIgmp = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_IGMP, (cfg_addr)&pIgmp);

    pIgmp->mvrConfig.fastLeaveEnable[port] = enable;

    CFG_CLOSE(CFG_ID_IGMP);
}
#endif 

