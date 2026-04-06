
#include <libsal/sal_util.h>
#include <libsal/sal_igmp.h>
#include <libsal/sal_mld.h>

int32 sal_mld_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_mld_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_mld_enable_set(sys_enable_t enable)
{
    igmp_unknMcastAction_t action;
    sys_enable_t oldEnable, igmpEnable;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_mld_enable_get(&oldEnable));

    if(oldEnable == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_igmp_enable_get(&igmpEnable));

    SYS_ERR_CHK(mcast_mld_enable_set(enable));
    SYS_ERR_CHK(cfg_mld_enable_set(enable));

    if (DISABLED == enable)
    {
        if (DISABLED == igmpEnable)
        {
            SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&action));
            if (IGMP_UNKNOWN_MCAST_ACTION_FLOOD != action)
            {
                SYS_ERR_CHK(sal_igmp_unknMcastAction_set(IGMP_UNKNOWN_MCAST_ACTION_FLOOD));
            }
        }
    }

    return SYS_ERR_OK;
}

int32 sal_mld_lookupMode_set(igmp_lookup_mode_t mode)
{
    SYS_PARAM_CHK((mode >= IGMP_LOOKUP_MODE_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(mcast_mld_lookupMode_set(mode));
    SYS_ERR_CHK(cfg_mld_lookupMode_set(mode));

    return SYS_ERR_OK;
}

int32 sal_mld_lookupMode_get(igmp_lookup_mode_t *mode)
{
    SYS_PARAM_CHK((NULL == mode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_mld_lookupMode_get(mode));

    return SYS_ERR_OK;
}

int32 sal_mld_unknMcastAction_get(igmp_unknMcastAction_t *pAction)
{
    igmp_unknMcastAction_t action;

    SYS_PARAM_CHK((NULL == pAction), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_mld_unknMcastAction_get(&action));

    *pAction = action;

    return SYS_ERR_OK;
}

int32 sal_mld_unknMcastAction_set(igmp_unknMcastAction_t action)
{
    igmp_unknMcastAction_t oldAction;

    SYS_ERR_CHK(sal_mld_unknMcastAction_get(&oldAction));
    if (oldAction == action)
        return SYS_ERR_OK;

    SYS_ERR_CHK(mcast_mld_unknMcastAction_set(action));
    SYS_ERR_CHK(cfg_mld_unknMcastAction_set(action));

    return SYS_ERR_OK;
}

int32 sal_mld_suppre_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(mcast_mld_suppre_enable_set(enable));
    SYS_ERR_CHK(cfg_mld_suppre_enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_mld_suppre_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_mld_suppre_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_mld_operVersion_set(uint8 operVersion)
{
    SYS_ERR_CHK(mcast_mld_operVersion_set(operVersion));

    return SYS_ERR_OK;
}

int32 sal_mld_info_get(sys_mcast_info_t *pInfo)
{
    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_mld_info_get(pInfo));

    return SYS_ERR_OK;
}

int32 sal_mld_static_group_set(igmp_group_entry_t *pGroup, uint8 fmode, uint8 isAdd)
{
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_ERR_CHK(mcast_basic_static_groupEntry_set(pGroup,fmode, isAdd));

    return SYS_ERR_OK;
}

int32 sal_mld_statistics_clear(void)
{
    SYS_ERR_CHK(mcast_mld_statistics_clear());

    return SYS_ERR_OK;
}

int32 sal_mld_groupByType_clear(mcast_group_type_t type)
{
    SYS_ERR_CHK(mcast_mld_group_del_byType(type));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

int32 sal_mld_portGroup_limit_set(sys_logic_port_t port, uint16 maxnum)
{
    SYS_ERR_CHK(mcast_portGroup_limit_set(MULTICAST_TYPE_IPV6, port,maxnum));

    SYS_ERR_CHK(cfg_mld_portGroup_limit_set(port, maxnum));

    return SYS_ERR_OK;
}

int32 sal_mld_portGroup_limit_get(sys_logic_port_t port, uint16 *pMaxnum)
{
    SYS_ERR_CHK(cfg_mld_portGroup_limit_get(port, pMaxnum));

    return SYS_ERR_OK;
}

int32 sal_mld_portGroup_excessAct_set(sys_logic_port_t port, igmp_maxGroupAct_t act)
{
    SYS_ERR_CHK(mcast_portGroup_excessAct_set(MULTICAST_TYPE_IPV6, port,act));
    SYS_ERR_CHK(cfg_mld_portGroup_excessAct_set(port, act));

    return SYS_ERR_OK;
}

int32 sal_mld_portGroup_excessAct_get(sys_logic_port_t port, igmp_maxGroupAct_t *pAct)
{
    SYS_ERR_CHK(cfg_mld_portGroup_excessAct_get(port, pAct));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

int32 sal_mld_profile_get(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_MLD_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_mld_profile_get(profileId, pProfile));
    if (0 == pProfile->valid)
    {
        return SYS_ERR_MLD_PROFILE_NOT_EXIST;
    }

    return SYS_ERR_OK;
}

int32 sal_mld_profile_getNext(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    sys_igmp_profileId_t index;
    sys_igmp_profile_t profile;

    SYS_PARAM_CHK(profileId > BOND_MCAST_PROFILE_MAX -1, SYS_ERR_MLD_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    for(index = profileId; index <= BOND_MCAST_PROFILE_MAX; index++)
    {
        osal_memset(&profile, 0, sizeof(sys_igmp_profile_t));
        SYS_ERR_CHK(cfg_mld_profile_get(index, &profile));
        if ( 1 == profile.valid )
        {
            osal_memcpy(pProfile, &profile, sizeof(sys_igmp_profile_t ));
            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_FAILED;
}

int32 sal_mld_profile_set(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_MLD_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_profile_set(MULTICAST_TYPE_IPV6, profileId, pProfile));
    SYS_ERR_CHK(cfg_mld_profile_set(profileId, pProfile));

    return SYS_ERR_OK;
}

int32 sal_mld_port_filter_get(sys_logic_port_t port, sys_igmp_profileId_t *pProfileId)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pProfileId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_mld_port_filter_get(port, pProfileId));

    return SYS_ERR_OK;
}

int32 sal_mld_port_filter_set(sys_logic_port_t port, sys_igmp_profileId_t  profileId)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(profileId > BOND_MCAST_PROFILE_MAX, SYS_ERR_MLD_PROFILE_ID);

    SYS_ERR_CHK(mcast_port_filter_set(MULTICAST_TYPE_IPV6,port, profileId));
    SYS_ERR_CHK(cfg_mld_port_filter_set(port, profileId));

    return SYS_ERR_OK;
}
#endif

int32 sal_mld_databaseSize_get(uint32 *pSize)
{
    SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_mld_databaseSize_get(pSize));

    return SYS_ERR_OK;
}

