
#include <libsal/sal_util.h>
#include <libsal/sal_igmp.h>

const char *profileActStr[] =
{
    "deny",
    "permit",
};

const char *mcastStatCntName[] =
{
    "Total RX",    
    "Valid RX",    
    "Invalid RX",   
    "Other RX",    
    "Leave RX",   
    "Report RX",   
    "General Query RX",   
    "Specail Group Query RX",   
    "Specail Group & Source Query RX",  
    "Leave TX",    
    "Report TX",   
    "General Query TX",  
    "Specail Group Query TX",   
    "Specail Group & Source Query TX",   
};

const char *gUnknownMcastAct[] =
{
    "Drop",
    "Flood",
    "Router-Port",
};

const char *gLookupMode[] =
{
    "mac",
    "src-dst-ip",
    "dip",
};

#ifdef CONFIG_SYS_PROTO_MVR
const char *gMvrPortType[] =
{
    "None",
    "Receiver",
    "Source",
};
#endif

const sys_text_t text_mcast[] =
{
    {   "IGMP" },
    {   "MLD"  },
};

int32 sal_igmp_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_igmp_enable_set(sys_enable_t enable)
{
    sys_enable_t oldEnable, mldEnable = DISABLED;
    igmp_unknMcastAction_t Action;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_igmp_enable_get(&oldEnable));

    if(oldEnable == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(mcast_igmp_enable_set(enable));
    SYS_ERR_CHK(cfg_igmp_enable_set(enable));

    if (DISABLED == enable)
    {
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        SYS_ERR_CHK(sal_mld_enable_get(&mldEnable));
    #endif
        if (DISABLED == mldEnable)
        {
            SYS_ERR_CHK(sal_igmp_unknMcastAction_get(&Action));
            if (IGMP_UNKNOWN_MCAST_ACTION_FLOOD != Action)
            {
                SYS_ERR_CHK(sal_igmp_unknMcastAction_set(IGMP_UNKNOWN_MCAST_ACTION_FLOOD));
            }
        }
    }

    return SYS_ERR_OK;
}

int32 sal_igmp_suppre_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(mcast_igmp_suppre_enable_set(enable));
    SYS_ERR_CHK(cfg_igmp_suppre_enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_igmp_suppre_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_suppre_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_igmp_statistics_clear(void)
{
    SYS_ERR_CHK(mcast_igmp_statistics_clear());

    return SYS_ERR_OK;
}

int32 sal_igmp_info_get(sys_mcast_info_t *pInfo)
{
    SYS_PARAM_CHK((NULL == pInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_igmp_info_get(pInfo));

    return SYS_ERR_OK;
}

int32 sal_igmp_portStat_get(sys_logic_port_t lport, sys_mcast_portStat_t *portStat)
{
    SYS_PARAM_CHK((NULL == portStat), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(lport), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(mcast_igmp_portStat_get(lport, portStat));

    return SYS_ERR_OK;
}

int32 sal_igmp_portStat_clear(sys_logic_port_t lport)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(lport), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(mcast_igmp_portStat_clear(lport));
    return SYS_ERR_OK;
}

int32 sal_igmp_lookupMode_set(igmp_lookup_mode_t mode)
{
    SYS_PARAM_CHK((mode >= IGMP_LOOKUP_MODE_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(mcast_igmp_lookupMode_set(mode));
    SYS_ERR_CHK(cfg_igmp_lookupMode_set(mode));

    return SYS_ERR_OK;
}

int32 sal_igmp_lookupMode_get(igmp_lookup_mode_t *mode)
{
    SYS_PARAM_CHK((NULL == mode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_igmp_lookupMode_get(mode));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER

int32 sal_igmp_querier_set(sys_igmp_querier_t* pQuerier)
{
    SYS_PARAM_CHK((NULL == pQuerier), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_igmp_querier_set(pQuerier));
    SYS_ERR_CHK(cfg_igmp_querier_set(pQuerier));

    return SYS_ERR_OK;
}

int32 sal_igmp_querier_get(sys_igmp_querier_t *pQuerier)
{
    SYS_PARAM_CHK((NULL == pQuerier), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pQuerier->ipType < MULTICAST_TYPE_IPV4 ||pQuerier->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(mcast_igmp_querier_get(pQuerier));

    return SYS_ERR_OK;
}

int32 sal_igmp_querier_vlanmask_get(sys_vlanmask_t *pQuerier)
{
    SYS_PARAM_CHK((NULL == pQuerier), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_querier_get(pQuerier));

    return SYS_ERR_OK;
}
#endif

int32 sal_igmp_operVersion_set(uint8 operVersion)
{
    SYS_ERR_CHK(mcast_igmp_operVersion_set(operVersion));
    SYS_ERR_CHK(cfg_igmp_operVersion_set(operVersion));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL

int32 sal_igmp_groupHead_database_get(sys_igmp_groupHead_db_t *pGrpHeadDb)
{
    SYS_PARAM_CHK((NULL == pGrpHeadDb), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_igmp_groupHead_database_get(pGrpHeadDb));

    return SYS_ERR_OK;
}

int32 sal_igmp_static_filter_group_set(igmp_group_entry_t *pGroup, uint8 fmode)
{
    char portmask_str[CAPA_PORTMASK_STR_LEN];

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pGroup->ipType< MULTICAST_TYPE_IPV4 ||pGroup->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(LPM2STR(&pGroup->mbr, portmask_str));

    SYS_ERR_CHK(mcast_igmpv3_static_filter_group_set(pGroup,fmode));

    return SYS_ERR_OK;

}

int32 sal_igmpv3_static_group_set(sys_vid_t vid, uint32 groupAddr,
         uint32 sourceAddr, sys_logic_portmask_t *pSpmsk,  uint8 fmode)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    SYS_ERR_CHK(mcast_igmpv3_static_groupEntry_set(vid,groupAddr,sourceAddr,pSpmsk,fmode));

    return SYS_ERR_OK;
}
#endif

int32 sal_mcast_table_clear(void)
{
    SYS_ERR_CHK(mcast_table_clear());

    return SYS_ERR_OK;
}

int32 sal_igmp_groupByType_clear(mcast_group_type_t type)
{
    SYS_ERR_CHK(mcast_igmp_group_del_byType(type));

    return SYS_ERR_OK;
}

int32 sal_igmp_databaseSize_get(uint32 *pSize)
{
    SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_igmp_databaseSize_get(pSize));

    return SYS_ERR_OK;
}

int32 sal_mcast_vlan_get(mcast_vlan_entry_t *pMcastVlan)
{
    SYS_PARAM_CHK((NULL == pMcastVlan), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VID_INVALID(pMcastVlan->vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((pMcastVlan->ipType < MULTICAST_TYPE_IPV4 ||pMcastVlan->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(mcast_vlan_get(pMcastVlan));

    return SYS_ERR_OK;
}

int32 sal_mcast_vlan_set(mcast_vlan_entry_t* pMcastVlan)
{
    SYS_PARAM_CHK((NULL == pMcastVlan), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VID_INVALID(pMcastVlan->vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((pMcastVlan->ipType < MULTICAST_TYPE_IPV4 ||pMcastVlan->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(mcast_vlan_set(pMcastVlan));

    return SYS_ERR_OK;
}

int32 sal_mcast_vlan_add(mcast_vlan_entry_t* pMcastVlan)
{
    mcast_vlan_entry_t tmpMcastVlan;

    SYS_PARAM_CHK((NULL == pMcastVlan), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VID_INVALID(pMcastVlan->vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((pMcastVlan->ipType < MULTICAST_TYPE_IPV4 ||pMcastVlan->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(mcast_vlan_add(pMcastVlan));

    osal_memset(&tmpMcastVlan,0,sizeof(mcast_vlan_entry_t));
    tmpMcastVlan.vid = pMcastVlan->vid;
    tmpMcastVlan.ipType = pMcastVlan->ipType;
    SYS_ERR_CHK(mcast_vlan_get(&tmpMcastVlan));

    if (0 == tmpMcastVlan.vid)
    {
        return SYS_ERR_MCAST_VLAN_NOT_EXIST;
    }

    return SYS_ERR_OK;
}

int32 sal_mcast_vlanMskConfig_get(multicast_ipType_t ipType, sys_vlanmask_t *pBitmap)
{
    SYS_PARAM_CHK((NULL == pBitmap), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(mcast_vlanMskConfig_get(ipType,pBitmap));

    return SYS_ERR_OK;
}

int32 sal_mcast_router_get(igmp_router_entry_t *pRouter)
{
    SYS_PARAM_CHK((NULL == pRouter), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VID_INVALID(pRouter->vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((pRouter->ipType < MULTICAST_TYPE_IPV4 ||pRouter->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(mcast_router_get(pRouter));

    return SYS_ERR_OK;
}

int32 sal_mcast_database_get(sys_igmp_db_t *pDb)
{
    SYS_PARAM_CHK((NULL == pDb), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_database_get(pDb));

    return SYS_ERR_OK;
}

int32 sal_mcast_databaseSize_get(uint32 *pSize)
{
    SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_databaseSize_get(pSize));

    return SYS_ERR_OK;
}

int32 sal_mcast_dipOnly_databaseSize_get(uint32 *pSize)
{
    SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_dipOnly_databaseSize_get(pSize));

    return SYS_ERR_OK;
}

int32 sal_igmp_unknMcastAction_get(igmp_unknMcastAction_t *pAction)
{
    igmp_unknMcastAction_t action;

    SYS_PARAM_CHK((NULL == pAction), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_unknMcastAction_get(&action));

    *pAction = action;

    return SYS_ERR_OK;
}

int32 sal_igmp_unknMcastAction_set(igmp_unknMcastAction_t action)
{
    sys_enable_t igmpEnable = DISABLED , mldEnable = DISABLED;

    if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == action)
    {
       SYS_ERR_CHK(sal_igmp_enable_get(&igmpEnable));

        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            SYS_ERR_CHK(sal_mld_enable_get(&mldEnable));
        #endif

        if (DISABLED == igmpEnable && DISABLED == mldEnable)
        {
            return SYS_ERR_IGMP_UNKNOWN_ROUTERPORT_ACT;
        }
    }

    SYS_ERR_CHK(mcast_igmp_unknMcastAction_set(action));
    SYS_ERR_CHK(cfg_igmp_unknMcastAction_set(action));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER

int32 sal_mcast_router_forbid_port_set(sys_vid_t vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(mcast_router_forbid_port_set(vid, ipType, pmsk));

    return SYS_ERR_OK;
}

 int32 sal_mcast_router_static_port_set(sys_vid_t vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(mcast_router_static_port_set(vid, ipType, pmsk));

    return SYS_ERR_OK;
}
#endif

int32 sal_mcast_static_group_clear(igmp_group_entry_t *pGroup)
{
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VID_INVALID(pGroup->vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((pGroup->ipType < MULTICAST_TYPE_IPV4 ||pGroup->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(mcast_static_group_clear(pGroup));

    return SYS_ERR_OK;
}

int32 sal_mcast_static_group_set(igmp_group_entry_t *pGroup, uint8 fmode, uint8 isAdd)
{
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_basic_static_groupEntry_set(pGroup,fmode, isAdd));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

int32 sal_igmp_portGroup_limit_set(sys_logic_port_t port, uint16 maxnum)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(mcast_portGroup_limit_set(MULTICAST_TYPE_IPV4, port,maxnum));

    SYS_ERR_CHK(cfg_igmp_portGroup_limit_set(port, maxnum));

    return SYS_ERR_OK;
}

int32 sal_igmp_portGroup_limit_get(sys_logic_port_t port, uint16 *pMaxnum)
{
    SYS_ERR_CHK(cfg_igmp_portGroup_limit_get(port, pMaxnum));

    return SYS_ERR_OK;
}

int32 sal_igmp_portGroup_excessAct_set(sys_logic_port_t port, igmp_maxGroupAct_t act)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_ERR_CHK(mcast_portGroup_excessAct_set(MULTICAST_TYPE_IPV4, port,act));
    SYS_ERR_CHK(cfg_igmp_portGroup_excessAct_set(port, act));

    return SYS_ERR_OK;
}

int32 sal_igmp_portGroup_excessAct_get(sys_logic_port_t port, igmp_maxGroupAct_t *pAct)
{
    SYS_ERR_CHK(cfg_igmp_portGroup_excessAct_get(port, pAct));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING

int32 sal_igmp_profile_get(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_IGMP_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_profile_get(profileId, pProfile));
    if (0 == pProfile->valid)
    {
        return SYS_ERR_IGMP_PROFILE_NOT_EXIST;
    }

    return SYS_ERR_OK;
}

int32 sal_igmp_profile_getNext(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    sys_igmp_profileId_t index;
    sys_igmp_profile_t profile;

    SYS_PARAM_CHK(profileId > BOND_MCAST_PROFILE_MAX -1, SYS_ERR_IGMP_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    for(index = profileId; index <= BOND_MCAST_PROFILE_MAX; index++)
    {
        osal_memset(&profile, 0, sizeof(sys_igmp_profile_t));
        SYS_ERR_CHK(cfg_igmp_profile_get(index, &profile));
        if ( 1 == profile.valid )
        {
            osal_memcpy(pProfile, &profile, sizeof(sys_igmp_profile_t ));
            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_FAILED;
}

int32 sal_igmp_profile_set(sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_IGMP_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_profile_set(MULTICAST_TYPE_IPV4, profileId, pProfile));
    SYS_ERR_CHK(cfg_igmp_profile_set(profileId, pProfile));

    return SYS_ERR_OK;
}

int32 sal_igmp_port_filter_get(sys_logic_port_t port, sys_igmp_profileId_t *pProfileId)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pProfileId), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_port_filter_get(port, pProfileId));

    return SYS_ERR_OK;
}

int32 sal_igmp_port_filter_set(sys_logic_port_t port, sys_igmp_profileId_t  profileId)
{
    sys_igmp_profile_t profile;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(profileId > BOND_MCAST_PROFILE_MAX, SYS_ERR_IGMP_PROFILE_ID);

    if (INVALID_PROFILE_ID != profileId)
    {
        SYS_MEM_CLEAR(profile);
        SYS_ERR_CHK(sal_igmp_profile_get(profileId, &profile));
        if (MCAST_PROFILE_TYPE_FILTER != profile.profileType)
            return SYS_ERR_IGMP_PROFILE_TYPE;
    }
    SYS_ERR_CHK(mcast_port_filter_set(MULTICAST_TYPE_IPV4,port, profileId));
    SYS_ERR_CHK(cfg_igmp_port_filter_set(port, profileId));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_MVR

int32 sal_igmp_mvr_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_mvr_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_enable_set(sys_enable_t enable)
{
    sys_enable_t oldEn;
    sys_logic_port_t port;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_igmp_mvr_enable_get(&oldEn));
    if (oldEn == enable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(mcast_igmp_mvr_enable_set(enable));
    SYS_ERR_CHK(cfg_igmp_mvr_enable_set(enable));

    if (DISABLED == enable)
    {
        FOR_EACH_LOGIC_PORT(port)
        {
            SYS_ERR_CHK(sal_igmp_mvr_portType_set(port, DFLT_IGMP_MVR_PORT_TYPE));
            SYS_ERR_CHK(sal_igmp_mvr_portImmediate_set(port, DFLT_IGMP_MVR_IMMEDIATE_LEAVE));
        }

        SYS_ERR_CHK(sal_igmp_mvr_mode_set(MVR_MODE_COMPATABLE));
        SYS_ERR_CHK(sal_igmp_mvr_querytime_set(DFLT_IGMP_MVR_QUERYTIME));
        SYS_ERR_CHK(sal_igmp_mvr_vlanId_set(DFLT_IGMP_MVR_VLAN));
        SYS_ERR_CHK(cfg_igmp_mvr_groupRange_set(0,0));
    }

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_mode_get(mvr_mode_t *pMode)
{
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_mvr_mode_get(pMode));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_mode_set(mvr_mode_t mode)
{
    SYS_PARAM_CHK((mode < MVR_MODE_COMPATABLE || mode >= MVR_MODE_END ), SYS_ERR_INPUT);

    SYS_ERR_CHK(mcast_igmp_mvr_mode_set(mode));
    SYS_ERR_CHK(cfg_igmp_mvr_mode_set(mode));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_querytime_get(uint32 *pValue)
{
    SYS_PARAM_CHK((NULL == pValue), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_mvr_querytime_get(pValue));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_querytime_set(uint32 queryTime)
{
    SYS_ERR_CHK(mcast_igmp_mvr_querytime_set(queryTime));
    SYS_ERR_CHK(cfg_igmp_mvr_querytime_set(queryTime));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_vlanId_get(sys_vid_t  *pVid)
{
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_mvr_vlanId_get(pVid));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_vlanId_set(sys_vid_t vid)
{
    sys_vid_t mvrVlan;
    sys_vlan_operEntry_t vlanEntry;
    sys_logic_port_t port;
    mvr_portType_t type;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    if (!IS_STATIC_VLAN_EXIST(vid))
        return SYS_ERR_VLAN_NOT_STATIC;

    SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlan));
    if (mvrVlan == vid)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_vlan_operEntry_get(vid, &vlanEntry));
    FOR_EACH_LOGIC_PORT(port)
    {
        SYS_ERR_CHK(sal_igmp_mvr_portType_get(port, &type));
        if (MVR_TYPE_RECEIVER == type)
        {
            if (IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmMember, port))
            {
                return SYS_ERR_MVR_VLAN_PORT_RECEIVER;

            }
        }
        else if (MVR_TYPE_SOURCE == type)
        {
            if (!IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmMember, port))
            {
                return SYS_ERR_MVR_VLAN_PORT_SOURCE;

            }
        }
    }

    SYS_ERR_CHK(mcast_igmp_mvr_vlanId_set(vid));
    SYS_ERR_CHK(cfg_igmp_mvr_vlanId_set(vid));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_groupRange_get(uint32 *pStart, uint32 *pEnd)
{
    SYS_PARAM_CHK((NULL == pStart), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnd), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_mvr_groupRange_get(pStart, pEnd));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_groupRange_set(uint32 startIp, uint32 endIp)
{
    SYS_PARAM_CHK((!IGMP_IN_MULTICAST(startIp)), SYS_ERR_INPUT);
    SYS_PARAM_CHK((!IGMP_IN_MULTICAST(endIp)), SYS_ERR_INPUT);

    SYS_ERR_CHK(mcast_igmp_mvr_groupRange_set(startIp, endIp));
    SYS_ERR_CHK(cfg_igmp_mvr_groupRange_set(startIp, endIp));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_portType_get(sys_logic_port_t port , mvr_portType_t *pType)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pType), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_mvr_portType_get(port, pType));

    return SYS_ERR_OK;

}

int32 sal_igmp_mvr_portType_set(sys_logic_port_t port , mvr_portType_t type)
{
    sys_vid_t mvrVlan;
    sys_vlan_operEntry_t vlanEntry;

     SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
     SYS_PARAM_CHK((type < MVR_TYPE_NONE) || (type >= MVR_TYPE_END) , SYS_ERR_MVR_TYPE);

    SYS_ERR_CHK(sal_igmp_mvr_vlanId_get(&mvrVlan));
    SYS_ERR_CHK(sal_vlan_operEntry_get(mvrVlan, &vlanEntry));

    if (MVR_TYPE_RECEIVER == type)
    {
        if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_ACCESS)))
            return SYS_ERR_MVR_PORT_MODE;

        if (IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmMember, port))
            return SYS_ERR_MVR_VLAN_PORT_RECEIVER;
    }
    else if (MVR_TYPE_SOURCE == type)
    {
        #if 0
        if ( !(SYS_VLAN_PORT_MODE_GENERAL == portMode || SYS_VLAN_PORT_MODE_TRUNK == portMode))
            return SYS_ERR_MVR_PORT_MODE;
        #endif

        if (!IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmMember, port))
            return SYS_ERR_MVR_VLAN_PORT_TYPE;

        #if 0
        VLANMASK_CLEAR_ALL(vlanExist);
        SYS_ERR_CHK(sal_vlan_operEntryExist_get(&vlanExist));
        FOR_EACH_VID_IN_VLANMASK(vid, vlanExist)
        {
            if (vid == mvrVlan)
                continue;
            SYS_ERR_CHK(sal_vlan_operEntry_get(vid, &vlanEntry));
            if (IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmMember, port))
                return SYS_ERR_MVR_VLAN_PORT_SOURCE;
        }
        #endif
    }

    SYS_ERR_CHK(mcast_igmp_mvr_portType_set(port, type));
    SYS_ERR_CHK(cfg_igmp_mvr_portType_set(port, type));

    return SYS_ERR_OK;

}

int32 sal_igmp_mvr_portImmediate_get(sys_logic_port_t port , sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_igmp_mvr_portImmediate_get(port, pEnable));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvr_portImmediate_set(sys_logic_port_t port , sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(mcast_igmp_mvr_portImmediate_set(port, enable));
    SYS_ERR_CHK(cfg_igmp_mvr_portImmediate_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_igmp_mvrGroupNum_get(uint32 *pSize)
{
    SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_igmp_mvrGroupNum_get(pSize));

    return SYS_ERR_OK;
}

uint8 sal_igmp_mvr_group_ret(sys_igmp_db_t *pGrpDb)
{
    sys_vid_t mvrVlan;
    sys_enable_t mvrEn;
    uint32 startIp, endIp;

    SYS_PARAM_CHK((NULL == pGrpDb), FALSE);

    if (pGrpDb->ipType != MULTICAST_TYPE_IPV4)
        return FALSE;

    sal_igmp_mvr_enable_get(&mvrEn);
    if (DISABLED == mvrEn)
        return FALSE;

    sal_igmp_mvr_vlanId_get(&mvrVlan);
    if (mvrVlan != pGrpDb->vid)
            return FALSE;

    sal_igmp_mvr_groupRange_get(&startIp, &endIp);
    if (MULTICAST_TYPE_IPV4 == pGrpDb->ipType)
    {
         if ((pGrpDb->dip < startIp) || (pGrpDb->dip >endIp))
            return FALSE;
    }

    return TRUE;
}

int32 sal_mvr_groupByType_clear(mcast_group_type_t type)
{
    SYS_ERR_CHK(mcast_mvr_group_del_byType(type));

    return SYS_ERR_OK;
}

#endif  

int32 sal_l2_mcast_static_group_set(l2_mcast_group_t *pGroup, uint8 isAdd)
{
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

    if(isAdd)
    {
        SYS_ERR_CHK(mcast_static_l2_mcast_groupEntry_add(pGroup));
#ifdef CONFIG_SYS_PROTO_GARP
        SYS_ERR_CHK(gmrp_static_l2_mcast_group_set(pGroup, ENABLED));
#endif
    }
    else
    {
        SYS_ERR_CHK(mcast_static_l2_mcast_groupEntry_del(pGroup));
#ifdef CONFIG_SYS_PROTO_GARP
        SYS_ERR_CHK(gmrp_static_l2_mcast_group_set(pGroup, DISABLED));
#endif
    }

    return SYS_ERR_OK;
}

int32 sal_l2_mcast_databaseSize_get(uint32 *pSize)
{
    SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_static_l2_mcast_databaseSize_get(pSize));
    
    return SYS_ERR_OK;
}

int32 sal_l2_mcast_database_get(l2_mcast_group_t *pDb, uint32 index)
{
    SYS_PARAM_CHK((NULL == pDb), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(mcast_static_l2_mcast_database_get(pDb, index));

    return SYS_ERR_OK;
}

int32 sal_l2_mcast_static_group_get(l2_mcast_group_t *pGroup, uint32 codeOp)
{
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

	if(codeOp)
    	SYS_ERR_CHK(mcast_static_l2_mcast_groupEntry_add(pGroup));
    else
    	SYS_ERR_CHK(mcast_static_l2_mcast_groupEntry_del(pGroup));

    return SYS_ERR_OK;
}

