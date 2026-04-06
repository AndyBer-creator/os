
#include <rsd/rsd_mcast.h>
#include <rsd/rsd_l2.h>
#include <rsd/rsd_ipmcast.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_port.h>
#include <igmp/proto_igmp.h>
#include <igmp/proto_igmp_group_db.h>
#include <igmp/proto_igmp_querier.h>
#include <igmp/proto_igmp_rx.h>
#include <igmp/proto_igmp_util.h>

extern osal_mutex_t     igmp_sem_db;
extern igmp_unknMcastAction_t           gUnknown_mcast_igmp_action;
extern igmp_unknMcastAction_t           gUnknown_mcast_mld_action;
extern igmp_stats_t     igmp_stats[SYS_LOGICPORT_NUM_MAX];
extern igmp_cfg_t        igmp_cfg;
extern sys_enable_t     igmp_packet_stop;
extern sys_enable_t     mcast_timer_stop;
extern sys_enable_t           mcast_groupAdd_stop;
extern sys_igmp_func_t  igmp_lookup_mode_func[];

igmp_router_db_t    router_db[MULTICAST_TYPE_END];
igmp_router_entry_t tmp_router_array[MAX_ROUTER_VLAN - 1];
igmp_aggregate_db_t aggregate_db;
igmp_aggregate_entry_t *tmp_aggr_array;

uint8               igmp_trapDataPkt_lock;
uint8               mcast_group_full;

int32               mcast_group_num;
int32               igmp_group_num;
int32               mld_group_num;

int32               mcast_staticMac_num;
int32               igmp_staticMac_num;
int32               mld_staticMac_num;
int32               group_sortedAry_entry_num;
uint32              igmp_hw_entry_num;                               

uint32             mcast_mvr_group_num;
igmp_group_entry_t *group_db;
igmp_group_entry_t **group_sortedArray;
igmp_group_entry_t **tmp_group_sortedArray;
igmp_group_head_t   igmpGroupFreeHead;
igmp_group_head_entry_t *groupHead_db;

igmp_static_group_t  groupSave;

sys_igmp_profile_t     profile_db[MULTICAST_TYPE_END][CAPA_MCAST_PROFILE_NUM];
sys_igmp_profileId_t port_profileId[MULTICAST_TYPE_END][SYS_LOGICPORT_NUM_MAX];

mcast_vlan_db_t    vlan_db[MULTICAST_TYPE_END];
mcast_vlan_entry_t tmp_vlan_array[MAX_ROUTER_VLAN - 1];
sys_vlanmask_t mcastVlanCfgMsk[MULTICAST_TYPE_END];

extern mld_stats_t     mld_stats[SYS_LOGICPORT_NUM_MAX];
extern mld_cfg_t mld_cfg;

static int L2_MCAST_USED_CNT = 0;
l2mcast_vlan_db_t *gl2Mcast_vlan_db;

void mcast_group_rx_timer_stop(void)
{
    mcast_timer_stop = ENABLED;
    igmp_packet_stop = ENABLED;
    osal_time_usleep(20*1000);
}

void mcast_group_rx_timer_start(void)
{
    mcast_timer_stop = DISABLED;
    igmp_packet_stop = DISABLED;
}
int32 mcast_igmp_subgroup_sip_search(igmp_group_entry_t *gipHead, uint32 key, igmp_group_entry_t **subgroup, igmp_group_entry_t **previous);

void mcast_group_num_cal(multicast_ipType_t  ipType, uint8 isStaticMac, uint8 isMvrGroup, uint8 isInc)
{
    #ifdef CONFIG_SYS_PROTO_MVR
    uint32 mcast_max_group = CAPA_MCAST_GROUP_NUM + CAPA_IGMP_MVR_GROUP_NUM;
    #endif

    igmp_lookup_mode_t lookup_mode;
    uint8 isStopUnkown = FALSE;

    if (ipType < MULTICAST_TYPE_IPV4 || ipType > MULTICAST_TYPE_END)
        return ;

    if (TRUE == isInc)
    {
         mcast_group_num++;
         if (!isMvrGroup)
         {
            if (MULTICAST_TYPE_IPV4 == ipType)
            {
                igmp_group_num++;
                if (isStaticMac)
                {
                    mcast_staticMac_num++;
                    igmp_staticMac_num++;
                }
            }
            else
            {
                mld_group_num++;
                if (isStaticMac)
                {
                    mcast_staticMac_num++;
                    mld_staticMac_num++;
                }
            }
         }
        else
        {
            mcast_mvr_group_num++;
        }

        #ifdef CONFIG_SYS_PROTO_MVR
        if (ENABLED == igmp_cfg.mvrConfig.mvr_enable)
        {
            if (mcast_max_group == igmp_group_num + mld_group_num + mcast_mvr_group_num)
                isStopUnkown = TRUE;
        }
        else
        #endif
        {
            if (CAPA_MCAST_GROUP_NUM == igmp_group_num + mld_group_num)
                isStopUnkown = TRUE;
        }

        if (isStopUnkown)
        {
                rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4, &lookup_mode);
                if (IGMP_DMAC_VID_MODE == lookup_mode || IGMP_DIP_VID_MODE == lookup_mode)
                {
                    #ifdef CONFIG_SYS_PROTO_MVR
                    if ((IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(MULTICAST_TYPE_IPV4)) &&
                         ((ENABLED == igmp_cfg.igmpsnp_enable) ||(ENABLED == igmp_cfg.mvrConfig.mvr_enable)))
                    #else
                    if ((IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(MULTICAST_TYPE_IPV4)) &&
                         (ENABLED == igmp_cfg.igmpsnp_enable))
                    #endif
                    {
                       rsd_mcast_unKnown_drop_set(MULTICAST_TYPE_IPV4, ENABLED);
                    }
                }

                #if 0  
                else  if (IGMP_DIP_SIP_MODE == lookup_mode)
                {
                    rsd_mcast_ipMcastPkt2CpuEnable_set(MULTICAST_TYPE_IPV4, DISABLED, gUnknown_mcast_igmp_action);
                }
                #endif

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
              rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6, &lookup_mode);
              if (IGMP_DMAC_VID_MODE == lookup_mode ||  IGMP_DIP_VID_MODE == lookup_mode)
              {
                    if ((IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(MULTICAST_TYPE_IPV6)) &&
                         (ENABLED == mld_cfg.mldsnp_enable))
                    {
                        rsd_mcast_unKnown_drop_set(MULTICAST_TYPE_IPV6, ENABLED);
                    }
              }
    #endif
                mcast_group_full = TRUE;
         }
    }
    else
    {
        mcast_group_num--;
        if (!isMvrGroup)
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
            {
                igmp_group_num--;
            }
            else
            {
                mld_group_num--;
            }
        }
        else
        {
            mcast_mvr_group_num--;
        }
        if (mcast_group_full)
        {
            mcast_group_full = FALSE;
            isStopUnkown = TRUE;
             #ifdef CONFIG_SYS_PROTO_MVR
            if (ENABLED == igmp_cfg.mvrConfig.mvr_enable)
            {
                if (igmp_group_num + mld_group_num + mcast_mvr_group_num < mcast_max_group)
                    isStopUnkown = FALSE;
            }
            else
            #endif
            {
                if (igmp_group_num + mld_group_num < CAPA_MCAST_GROUP_NUM)
                    isStopUnkown = FALSE;
            }

            if (FALSE == isStopUnkown)
            {
                rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4, &lookup_mode);
                if (IGMP_DMAC_VID_MODE == lookup_mode  || IGMP_DIP_VID_MODE == lookup_mode)
                {
                    if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(MULTICAST_TYPE_IPV4))
                    {
                        if (ENABLED == igmp_cfg.igmpsnp_enable)
                            rsd_mcast_unKnown_drop_set(MULTICAST_TYPE_IPV4, DISABLED);
                    }
                 }

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6, &lookup_mode);
                if (IGMP_DMAC_VID_MODE == lookup_mode || IGMP_DIP_VID_MODE == lookup_mode)
                {
                    if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(MULTICAST_TYPE_IPV6))
                    {
                        if (ENABLED == mld_cfg.mldsnp_enable)
                            rsd_mcast_unKnown_drop_set(MULTICAST_TYPE_IPV6, DISABLED);
                    }
                 }
    #endif
            }
        }
    }
}
int32 mcast_hw_group_num_cal(uint8 isInc)
{
    igmp_lookup_mode_t ipv4LookMode;
    igmp_lookup_mode_t ipv6LookMode;

    ipv4LookMode = IGMP_DMAC_VID_MODE;
    ipv6LookMode = IGMP_DMAC_VID_MODE;

    SYS_ERR_CHK(rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4, &ipv4LookMode));

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    SYS_ERR_CHK(rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6, &ipv6LookMode));
#endif

    if (TRUE == isInc)
    {
       igmp_hw_entry_num++;

       if (CAPA_MCAST_GROUP_NUM == igmp_hw_entry_num)
       {
            if (IGMP_DIP_SIP_MODE == ipv4LookMode)
                SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(MULTICAST_TYPE_IPV4, DISABLED, mcast_unknownAct_ret(MULTICAST_TYPE_IPV4)));
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
             if (IGMP_DIP_SIP_MODE == ipv6LookMode)
                SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(MULTICAST_TYPE_IPV6, DISABLED, mcast_unknownAct_ret(MULTICAST_TYPE_IPV6)));
#endif
            igmp_trapDataPkt_lock = TRUE;
       }
    }
    else
    {
        igmp_hw_entry_num--;
        if (TRUE == igmp_trapDataPkt_lock)
        {
            igmp_trapDataPkt_lock = FALSE;
            if (IGMP_DIP_SIP_MODE == ipv4LookMode)
                SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(MULTICAST_TYPE_IPV4, ENABLED, mcast_unknownAct_ret(MULTICAST_TYPE_IPV4)));
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
             if (IGMP_DIP_SIP_MODE == ipv6LookMode)
                SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(MULTICAST_TYPE_IPV6, ENABLED, mcast_unknownAct_ret(MULTICAST_TYPE_IPV6)));
#endif
        }
    }

    {
        sys_l2_event_t l2Event;

        l2Event.mcastEntryNum = igmp_hw_entry_num;

        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_L2, SYS_NT_L2_MCAST_NUM_CHG, (char*)&l2Event);
    }

    return SYS_ERR_OK;
}
int32 mcast_vlan_array_search(uint16 search, multicast_ipType_t ipType,uint16 *idx, mcast_vlan_entry_t **entry)
{
    int low = 0;
    int mid;
    int high = vlan_db[ipType].entry_num - 1;
    mcast_vlan_entry_t *sortedArray = vlan_db[ipType].vlan_entry;

    if (idx == NULL || entry == NULL)
        return SYS_ERR_FAILED;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (sortedArray[mid].vid == 0)   
        {
            *entry = NULL;
            *idx = mid;
            return SYS_ERR_OK;
        }

        if (sortedArray[mid].vid == search)
        {
            *entry = &sortedArray[mid];
            *idx = mid;
            return SYS_ERR_OK;
        }
        else if (sortedArray[mid].vid > search)
        {
            high = mid - 1;
        }
        else if (sortedArray[mid].vid < search)
        {
            low = mid + 1;
        }
    }

    *entry = NULL;
    *idx = low;
    return SYS_ERR_OK;
}

int32 mcast_vlan_array_ins(uint16 sortedIdx, multicast_ipType_t ipType, mcast_vlan_entry_t *entry)
{
    if (entry == NULL)
        return SYS_ERR_FAILED;
    if (sortedIdx > vlan_db[ipType].entry_num)
    {
        MCASTDBG("Error: sortedIdx > vlan_db[%d].entry_num",ipType);
        return SYS_ERR_FAILED;
    }
    if (vlan_db[ipType].entry_num >= MAX_ROUTER_VLAN)
        return SYS_ERR_FAILED;

    osal_memcpy(tmp_vlan_array, vlan_db[ipType].vlan_entry + sortedIdx, sizeof(mcast_vlan_entry_t) * (vlan_db[ipType].entry_num - sortedIdx));
    osal_memcpy(vlan_db[ipType].vlan_entry + sortedIdx, entry, sizeof(mcast_vlan_entry_t));
    osal_memcpy(vlan_db[ipType].vlan_entry + sortedIdx + 1, tmp_vlan_array, sizeof(mcast_vlan_entry_t) * (vlan_db[ipType].entry_num - sortedIdx));
    vlan_db[ipType].entry_num++;

    return SYS_ERR_OK;
}

int32 mcast_vlan_array_remove(uint16 sortedIdx, multicast_ipType_t ipType)
{
    if (sortedIdx >= vlan_db[ipType].entry_num)
    {
        MCASTDBG("Error: sortedIdx >= vlan_db[%d].entry_num", ipType);
        return SYS_ERR_FAILED;
    }

    osal_memcpy(tmp_vlan_array, vlan_db[ipType].vlan_entry + sortedIdx + 1, sizeof(mcast_vlan_entry_t) * (vlan_db[ipType].entry_num - sortedIdx - 1));
    osal_memcpy(vlan_db[ipType].vlan_entry+ sortedIdx, tmp_vlan_array, sizeof(mcast_vlan_entry_t) * (vlan_db[ipType].entry_num - sortedIdx - 1));
    vlan_db[ipType].entry_num--;
    osal_memset(vlan_db[ipType].vlan_entry + vlan_db[ipType].entry_num, 0, sizeof(mcast_vlan_entry_t));

    return SYS_ERR_OK;
}
int32 mcast_vlan_db_init(void)
{
    uint8 i;
    for(i = 0; i < MULTICAST_TYPE_END; i++)
    {
        osal_memset(&vlan_db[i], 0, sizeof(vlan_db[i]));
        osal_memset(&mcastVlanCfgMsk[i], 0, sizeof(mcastVlanCfgMsk[i]));
    }
    return SYS_ERR_OK;
}
int32 mcast_vlan_db_add(uint16 vid, multicast_ipType_t ipType)
{
    uint16              sortedIdx;
    mcast_vlan_entry_t *entry = NULL;
    mcast_vlan_entry_t newEntry;

    mcast_vlan_array_search(vid,ipType, &sortedIdx, &entry);
    if (entry)
    {
        
        return SYS_ERR_OK;
    }
    else  
    {
        osal_memset(&newEntry, 0, sizeof(mcast_vlan_entry_t));

        newEntry.vid = vid;
        newEntry.ipType = ipType;
        LOGIC_PORTMASK_CLEAR_ALL(newEntry.staticMbr);
        LOGIC_PORTMASK_CLEAR_ALL(newEntry.forbidMbr);

        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            newEntry.enable = DFLT_IGMP_VLAN_EBL;
            newEntry.fastleave_enable = DFLT_IGMP_IMMEDIATE_LEAVE;
            newEntry.pimDvmrpLearn = DFLT_IGMP_ROUTER_PIMDVMRP_LEARN;

            newEntry.router_port_timeout =  DFLT_IGMP_ROUTER_TIMEOUT;
            newEntry.grpMembershipIntv = DFLT_IGMP_GROUP_MEMBERSHIP_INTV;
            newEntry.robustnessVar = DFLT_IGMP_ROBUSTNESS_VAR;
            newEntry.operRobNessVar = newEntry.robustnessVar;
            newEntry.queryIntv = DFLT_IGMP_QUERY_INTV;
            newEntry.operQueryIntv = newEntry.queryIntv;
            newEntry.lastMmbrQueryCnt = DFLT_IGMP_ROBUSTNESS_VAR;
            newEntry.operLastMmbrQueryCnt = newEntry.lastMmbrQueryCnt;
            newEntry.grpSpecificQueryIntv = DFLT_IGMP_GSQUERY_RESPONSE_INTV ;
            newEntry.operGsqueryIntv =  newEntry.grpSpecificQueryIntv;
            newEntry.responseTime = DFLT_IGMP_QUERY_RESPONSE_INTV;
            newEntry.operRespTime = newEntry.responseTime;

            newEntry.otherQuerierPresentInterval = (DFLT_IGMP_ROBUSTNESS_VAR * DFLT_IGMP_QUERY_INTV) + DFLT_IGMP_QUERY_RESPONSE_INTV/2;
        }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
        {
            newEntry.enable = DFLT_MLD_VLAN_EBL;
            newEntry.fastleave_enable = DFLT_MLD_IMMEDIATE_LEAVE;
            newEntry.pimDvmrpLearn = DFLT_MLD_ROUTER_PIMDVMRP_LEARN;

            newEntry.router_port_timeout =  DFLT_MLD_ROUTER_TIMEOUT;
            newEntry.grpMembershipIntv = DFLT_MLD_GROUP_MEMBERSHIP_INTV;
            newEntry.robustnessVar = DFLT_MLD_ROBUSTNESS_VAR;
            newEntry.operRobNessVar = newEntry.robustnessVar;
            newEntry.queryIntv = DFLT_MLD_QUERY_INTV;
            newEntry.operQueryIntv = newEntry.queryIntv;
            newEntry.lastMmbrQueryCnt = DFLT_MLD_ROBUSTNESS_VAR;
            newEntry.operLastMmbrQueryCnt = newEntry.lastMmbrQueryCnt;
            newEntry.grpSpecificQueryIntv = DFLT_MLD_GS_QUERY_RESP_INTV;
            newEntry.operGsqueryIntv =  newEntry.grpSpecificQueryIntv;
            newEntry.responseTime = DFLT_MLD_QUERY_RESPONSE_INTV;
            newEntry.operRespTime = newEntry.responseTime;

            newEntry.otherQuerierPresentInterval = (DFLT_MLD_ROBUSTNESS_VAR * DFLT_MLD_QUERY_INTV) + DFLT_MLD_QUERY_RESPONSE_INTV/2;
        }
#endif
        return mcast_vlan_array_ins(sortedIdx, ipType,&newEntry);
    }

    return SYS_ERR_OK;
}

int32 mcast_vlan_db_get(uint16 vid, multicast_ipType_t ipType, mcast_vlan_entry_t **ppEntry)
{
    uint16              sortedIdx;

    SYS_PARAM_CHK(NULL == ppEntry, SYS_ERR_NULL_POINTER);

    mcast_vlan_array_search(vid, ipType, &sortedIdx, ppEntry);

    return SYS_ERR_OK;
}

int32 mcast_vlan_db_set(mcast_vlan_entry_t *pEntry)
{
    uint16              sortedIdx;
    mcast_vlan_entry_t *entry = NULL;

    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);

    mcast_vlan_array_search(pEntry->vid, pEntry->ipType, &sortedIdx, &entry);
    if(entry != NULL)
    {
        entry->enable = pEntry->enable;
        entry->fastleave_enable = pEntry->fastleave_enable;
        entry->pimDvmrpLearn = pEntry->pimDvmrpLearn;
        LOGIC_PORTMASK_COPY(entry->staticMbr, pEntry->staticMbr);
        LOGIC_PORTMASK_COPY(entry->forbidMbr, pEntry->forbidMbr);

        entry->robustnessVar = pEntry->robustnessVar;
        entry->operRobNessVar = pEntry->operRobNessVar;
        entry->queryIntv = pEntry->queryIntv;
        entry->operQueryIntv = pEntry->operQueryIntv;
        entry->responseTime = pEntry->responseTime;
        entry->operRespTime = pEntry->operRespTime;
        entry->lastMmbrQueryCnt = pEntry->lastMmbrQueryCnt;
        entry->operLastMmbrQueryCnt = pEntry->operLastMmbrQueryCnt;
        entry->grpSpecificQueryIntv = pEntry->grpSpecificQueryIntv;
        entry->operGsqueryIntv =  pEntry->operGsqueryIntv;

        entry->otherQuerierPresentInterval = (pEntry->operRobNessVar * pEntry->operQueryIntv) + pEntry->operRespTime/2;
        entry->router_port_timeout = (pEntry->operRobNessVar * pEntry->operQueryIntv) + pEntry->operRespTime;
        entry->grpMembershipIntv = (pEntry->operRobNessVar * pEntry->operQueryIntv) + pEntry->operRespTime;
    }
    else
    {
        SYS_PARAM_CHK(IS_VID_INVALID(pEntry->vid), SYS_ERR_VLAN_ID);
        SYS_PARAM_CHK((pEntry->ipType < MULTICAST_TYPE_IPV4 ||pEntry->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

        mcast_vlan_db_add(pEntry->vid, pEntry->ipType);
    }

    return SYS_ERR_OK;
}
int32 mcast_vlan_db_del(uint16 vid, multicast_ipType_t  ipType)
{
    uint16          sortedIdx;
    mcast_vlan_entry_t   *entry = NULL;

    mcast_vlan_array_search(vid, ipType,&sortedIdx, &entry);
    if (entry)
    {
        return mcast_vlan_array_remove(sortedIdx, ipType);
    }

    return SYS_ERR_FAILED;
}
int32 mcast_router_array_search(uint16 search, multicast_ipType_t ipType, uint16 *idx, igmp_router_entry_t **entry)
{
    int low = 0;
    int mid;
    int high = router_db[ipType].entry_num - 1;
    igmp_router_entry_t *sortedArray = router_db[ipType].rt_entry;

    if (idx == NULL || entry == NULL)
        return SYS_ERR_FAILED;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (sortedArray[mid].vid == 0)   
        {
            *entry = NULL;
            *idx = mid;
            return SYS_ERR_OK;
        }

        if (sortedArray[mid].vid == search)
        {
            *entry = &sortedArray[mid];
            *idx = mid;
            return SYS_ERR_OK;
        }
        else if (sortedArray[mid].vid > search)
        {
            high = mid - 1;
        }
        else if (sortedArray[mid].vid < search)
        {
            low = mid + 1;
        }
    }

    *entry = NULL;
    *idx = low;
    return SYS_ERR_OK;
}

int32 mcast_router_array_ins(uint16 sortedIdx, multicast_ipType_t ipType, igmp_router_entry_t *entry)
{
    if (entry == NULL)
        return SYS_ERR_FAILED;
    if (sortedIdx > router_db[ipType].entry_num)
    {
        MCASTDBG("Error: sortedIdx > router_db[%d].entry_num", ipType);
        return SYS_ERR_FAILED;
    }
    if (router_db[ipType].entry_num >= MAX_ROUTER_VLAN)
        return SYS_ERR_FAILED;

    osal_memcpy(tmp_router_array, router_db[ipType].rt_entry + sortedIdx, sizeof(igmp_router_entry_t) * (router_db[ipType].entry_num - sortedIdx));
    osal_memcpy(router_db[ipType].rt_entry + sortedIdx, entry, sizeof(igmp_router_entry_t));
    osal_memcpy(router_db[ipType].rt_entry + sortedIdx + 1, tmp_router_array, sizeof(igmp_router_entry_t) * (router_db[ipType].entry_num - sortedIdx));
    router_db[ipType].entry_num++;

    return SYS_ERR_OK;
}

int32 mcast_router_array_remove(uint16 sortedIdx, multicast_ipType_t ipType)
{
    if (sortedIdx >= router_db[ipType].entry_num)
    {
        MCASTDBG("Error: sortedIdx >= router_db[%d].entry_num", ipType);
        return SYS_ERR_FAILED;
    }

    osal_memcpy(tmp_router_array, router_db[ipType].rt_entry + sortedIdx + 1, sizeof(igmp_router_entry_t) * (router_db[ipType].entry_num - sortedIdx - 1));
    osal_memcpy(router_db[ipType].rt_entry + sortedIdx, tmp_router_array, sizeof(igmp_router_entry_t) * (router_db[ipType].entry_num - sortedIdx - 1));
    router_db[ipType].entry_num--;
    osal_memset(router_db[ipType].rt_entry + router_db[ipType].entry_num, 0, sizeof(igmp_router_entry_t));

    return SYS_ERR_OK;
}

int32 mcast_db_init(void)
{
    tmp_aggr_array = osal_alloc(sizeof(igmp_aggregate_entry_t) * (CAPA_MCAST_DB_NUM - 1));
    group_db = osal_alloc(sizeof(igmp_group_entry_t) * CAPA_MCAST_DB_NUM);
    group_sortedArray = osal_alloc(sizeof(igmp_group_entry_t *) * CAPA_MCAST_DB_NUM);
    tmp_group_sortedArray = osal_alloc(sizeof(igmp_group_entry_t *) * (CAPA_MCAST_DB_NUM - 1));
    groupHead_db = osal_alloc(sizeof(igmp_group_head_entry_t) * CAPA_MCAST_DB_NUM);

    mcast_vlan_db_init();
    mcast_router_db_init();
    mcast_aggregate_db_init();
    mcast_group_db_init();

    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    mcast_igmp_group_head_db_init();
    #endif
    return SYS_ERR_OK;
}
int32 mcast_router_db_init(void)
{
    multicast_ipType_t ipType;

    for(ipType = MULTICAST_TYPE_IPV4; ipType < MULTICAST_TYPE_END; ipType++)
    {
        osal_memset(&router_db[ipType], 0, sizeof(router_db[ipType]));
    }

    return SYS_ERR_OK;
}

int32 mcast_router_db_add(uint16 vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk, igmp_router_port_type_t portType)
{
    uint16              i, sortedIdx;
    igmp_router_entry_t *entry = NULL;
    igmp_router_entry_t newEntry;

    mcast_vlan_entry_t *pMcastVlan = NULL;

    if (IGMP_ROUTER_PORT_DYNAMIC == portType)
    {
        mcast_vlan_db_get(vid, ipType, &pMcastVlan);
        if (NULL == pMcastVlan)
        {
            return SYS_ERR_FAILED;
        }
    }

    mcast_router_array_search(vid, ipType,&sortedIdx, &entry);
    if (entry)
    {
        switch(portType)
        {
        case IGMP_ROUTER_PORT_STATIC:
            osal_memcpy(&entry->router_static_pmsk, &pmsk, sizeof(sys_logic_portmask_t));
            break;

        case IGMP_ROUTER_PORT_FORBID:
            osal_memcpy(&entry->router_forbid_pmsk, &pmsk, sizeof(sys_logic_portmask_t));
            break;

        case IGMP_ROUTER_PORT_DYNAMIC:
        default:
            osal_memcpy(&entry->router_pmsk, &pmsk, sizeof(sys_logic_portmask_t));
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(i, pmsk)
            {
                entry->portTimer[i] = pMcastVlan->router_port_timeout;
            }
            break;
        }
    }
    else  
    {
        osal_memset(&newEntry, 0, sizeof(igmp_router_entry_t));
        newEntry.vid = vid;

        switch(portType)
        {
        case IGMP_ROUTER_PORT_STATIC:
            osal_memcpy(&newEntry.router_static_pmsk, &pmsk, sizeof(sys_logic_portmask_t));
            break;

        case IGMP_ROUTER_PORT_FORBID:
            osal_memcpy(&newEntry.router_forbid_pmsk, &pmsk, sizeof(sys_logic_portmask_t));
            break;

        case IGMP_ROUTER_PORT_DYNAMIC:
        default:
            osal_memcpy(&newEntry.router_pmsk, &pmsk, sizeof(sys_logic_portmask_t));
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(i, pmsk)
            {
                newEntry.portTimer[i] = pMcastVlan->router_port_timeout;
            }
            break;
        }

        return mcast_router_array_ins(sortedIdx, ipType, &newEntry);
    }

    return SYS_ERR_OK;
}

int32 mcast_router_db_get(uint16 vid,multicast_ipType_t ipType, igmp_router_entry_t **ppEntry)
{
    uint16              sortedIdx;

    SYS_PARAM_CHK(NULL == ppEntry, SYS_ERR_NULL_POINTER);

    mcast_router_array_search(vid, ipType, &sortedIdx, ppEntry);

    return SYS_ERR_OK;
}

int32 mcast_router_db_set(igmp_router_entry_t *pEntry)
{
    uint16              sortedIdx;
    igmp_router_entry_t *entry = NULL;
    sys_logic_port_t port;

    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);

    mcast_router_array_search(pEntry->vid, pEntry->ipType, &sortedIdx, &entry);
    if(entry != NULL)
    {
        FOR_EACH_LOGIC_PORT(port)
        {
            entry->portTimer[port] = pEntry->portTimer[port];
        }
        osal_memcpy(  &entry->router_pmsk, &pEntry->router_pmsk, sizeof(sys_logic_portmask_t));
        osal_memcpy(  &entry->router_static_pmsk, &pEntry->router_static_pmsk, sizeof(sys_logic_portmask_t));
        osal_memcpy(  &entry->router_forbid_pmsk, &pEntry->router_forbid_pmsk, sizeof(sys_logic_portmask_t));
    }
    return SYS_ERR_OK;
}

int32 mcast_router_db_del(uint16 vid, multicast_ipType_t ipType)
{
    uint16          sortedIdx;
    igmp_router_entry_t   *entry;

    mcast_router_array_search(vid, ipType, &sortedIdx, &entry);
    if (entry)
    {
        return mcast_router_array_remove(sortedIdx, ipType);
    }

    return SYS_ERR_FAILED;
}

#ifdef CONFIG_SYS_PROTO_IGMP_STATIC_ROUTER
int32 mcast_router_static_port_set(uint16 vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk)
{
    int32               port;
    uint8               isDelPort = FALSE;
    
    sys_logic_portmask_t tmpPmsk, oldPmsk,delPmsk,trunkMbr;
    igmp_router_entry_t *pEntry = NULL;
    sys_vlanmask_t vlanMsk;

    LOGIC_PORTMASK_CLEAR_ALL(tmpPmsk);
    LOGIC_PORTMASK_CLEAR_ALL(oldPmsk);
    LOGIC_PORTMASK_CLEAR_ALL(delPmsk);
    LOGIC_PORTMASK_CLEAR_ALL(trunkMbr);
    IGMP_DB_SEM_LOCK();

    mcast_router_db_get(vid, ipType, &pEntry);

    if (pEntry)
    {
        if(IS_LOGIC_PORTMASK_EQUAL(pEntry->router_static_pmsk, pmsk))
        {
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        LOGIC_PORTMASK_COPY(oldPmsk, pEntry->router_static_pmsk);
        LOGIC_PORTMASK_CLEAR_ALL(pEntry->router_static_pmsk);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pmsk)
        {
            if(!IS_LOGIC_PORTMASK_PORTSET(pEntry->router_forbid_pmsk, port))
            {
                LOGIC_PORTMASK_SET_PORT(pEntry->router_pmsk, port);
                LOGIC_PORTMASK_SET_PORT(pEntry->router_static_pmsk, port);
                
            }
        }

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, oldPmsk)
        {
            if(!IS_LOGIC_PORTMASK_PORTSET(pEntry->router_static_pmsk,port))
            {
                LOGIC_PORTMASK_CLEAR_PORT(pEntry->router_pmsk, port);
                LOGIC_PORTMASK_SET_PORT(delPmsk, port);
                isDelPort = TRUE;
               
            }
        }
        LOGIC_PORTMASK_COPY(tmpPmsk, pEntry->router_pmsk);
    }
    else
    {
        LOGIC_PORTMASK_COPY(tmpPmsk, pmsk);
        mcast_router_db_add(vid, ipType,tmpPmsk, IGMP_ROUTER_PORT_STATIC);
        mcast_router_db_get(vid, ipType, &pEntry);
        if (NULL != pEntry)
            LOGIC_PORTMASK_OR(pEntry->router_pmsk, pEntry->router_pmsk, tmpPmsk);

    }

    if (!isDelPort)
    {
        
        mcast_suppreFlag_clear(vid, ipType);
    }

    #ifdef CONFIG_SYS_PROTO_MVR
    if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT != mcast_unknownAct_ret(ipType))
    {
        
        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            if (ENABLED == igmp_cfg.mvrConfig.mvr_enable && vid == igmp_cfg.mvrConfig.mvr_vid)
            {
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_OK;
            }
        }
    }
    #endif

    VLANMASK_CLEAR_ALL(vlanMsk);
    VLANMASK_SET_VID(vlanMsk, vid);
    mcast_routerPortMbr_remove(ipType, &vlanMsk, &delPmsk);
    mcast_routerPortMbr_add(ipType, vid,  &tmpPmsk);

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 mcast_router_forbid_port_set(uint16 vid, multicast_ipType_t ipType, sys_logic_portmask_t pmsk)
{
    int32               port;
    igmp_router_entry_t *pEntry = NULL;
    sys_logic_portmask_t tmpPmsk, delPmsk,oldPmsk;

    uint8               flag = TRUE;
    sys_vlanmask_t vlanMsk;

    LOGIC_PORTMASK_CLEAR_ALL(tmpPmsk);
    LOGIC_PORTMASK_CLEAR_ALL(delPmsk);

    IGMP_DB_SEM_LOCK();

    mcast_router_db_get(vid, ipType, &pEntry);

    if(pEntry)
    {
        if(IS_LOGIC_PORTMASK_EQUAL(pEntry->router_forbid_pmsk, pmsk))
        {
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        LOGIC_PORTMASK_COPY(oldPmsk, pEntry->router_forbid_pmsk);
        LOGIC_PORTMASK_CLEAR_ALL(pEntry->router_forbid_pmsk);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pmsk)
        {
            
            if(!IS_LOGIC_PORTMASK_PORTSET(pEntry->router_forbid_pmsk, port))
            {

                LOGIC_PORTMASK_SET_PORT(pEntry->router_forbid_pmsk, port);

                if (IS_LOGIC_PORTMASK_PORTSET(pEntry->router_pmsk, port))
                {
                    LOGIC_PORTMASK_CLEAR_PORT(pEntry->router_pmsk, port);
                    LOGIC_PORTMASK_CLEAR_PORT(pEntry->router_static_pmsk, port);
                    LOGIC_PORTMASK_SET_PORT(delPmsk, port);
                }
            }
        }

        if(IS_LOGIC_PORTMASK_CLEAR(delPmsk))
            flag = FALSE;

    }
    else
    {
        mcast_router_db_add(vid, ipType, pmsk, IGMP_ROUTER_PORT_FORBID);
        flag = FALSE;
    }

    #ifdef CONFIG_SYS_PROTO_MVR
    if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT != mcast_unknownAct_ret(ipType))
    {
        
        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            if (ENABLED == igmp_cfg.mvrConfig.mvr_enable && vid == igmp_cfg.mvrConfig.mvr_vid)
            {
                flag = FALSE;
            }
        }
    }
    #endif

    if(flag == TRUE)
    {
        VLANMASK_CLEAR_ALL(vlanMsk);
        VLANMASK_SET_VID(vlanMsk, vid);
        mcast_routerPortMbr_remove(ipType, &vlanMsk, &delPmsk);
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
#endif

int32 mcast_routerPort_add_wrapper(uint16 vid, multicast_ipType_t ipType, sys_logic_port_t port, uint8 isProt)
{
    int32 i;
    int32 ret;
    igmp_router_entry_t *pEntry = NULL;
    igmp_group_entry_t *pGroup = NULL;
    sys_l2McastEntry_t  l2Mcst_entry;
    sys_ipv46McastEntry_t  ip46Mcst_entry;

    mcast_vlan_entry_t *pMcastVlan = NULL;
    igmp_lookup_mode_t lookup_mode;
    sys_logic_portmask_t rtMask;
    uint8 isFwdAll = TRUE;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    if(IS_LOGIC_PORT_INVALID(port))
        return SYS_ERR_PORT_ID;

    LOGIC_PORTMASK_CLEAR_ALL(rtMask);
    LOGIC_PORTMASK_SET_PORT(rtMask, port);

    IGMP_DB_SEM_LOCK();

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    if (DISABLED == pMcastVlan->pimDvmrpLearn)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    if (DISABLED == pMcastVlan->enable)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
    }

    mcast_router_db_get(vid, ipType, &pEntry);
    if (pEntry)
    {
        if(IS_LOGIC_PORTMASK_PORTSET(pEntry->router_forbid_pmsk, port))
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
            {
                MCASTDBG("vid:%d port:%s is forbidden router port", vid, LPSTR(port));
            }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
            {
                MCASTDBG("vid:%d port:%s is forbidden router port", vid, LPSTR(port));
            }
#endif
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        if(IS_LOGIC_PORTMASK_PORTSET(pEntry->router_static_pmsk, port))
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
                MCASTDBG("vid:%d port:%s is static router port", vid, LPSTR(port));
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
                MCASTDBG("vid:%d port:%s is static router port", vid, LPSTR(port));
#endif
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        pEntry->portTimer[port] = pMcastVlan->router_port_timeout;

        if (IS_LOGIC_PORTMASK_PORTSET(pEntry->router_pmsk, port))
        {
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        LOGIC_PORTMASK_SET_PORT(pEntry->router_pmsk, port);
    }
    else
    {
        mcast_router_db_add(vid, ipType, rtMask,IGMP_ROUTER_PORT_DYNAMIC);
    }

    mcast_log_rtPort(ipType,vid, port, isProt ? MCAST_RT_LRN_BY_PROTO : MCAST_RT_LRN_BY_PIM);

    for (i = 0; i < mcast_group_num; i++)
    {
        pGroup = mcast_group_get_with_index(i);
        if ( NULL == pGroup)
            continue;

        if (!is_mcast_group(pGroup))
            continue;

        if (pGroup->vid == vid && pGroup->ipType == ipType)
        {
            rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);

            if(IGMP_DMAC_VID_MODE == lookup_mode)
            {
                l2Mcst_entry.vid = vid;
                osal_memcpy(l2Mcst_entry.mac, pGroup->mac, CAPA_MAC_BYTE_NUM);
                if (SYS_ERR_OK == mcast_hw_l2McastEntry_get(&l2Mcst_entry))
                {
                    if (!IS_LOGIC_PORTMASK_PORTSET(l2Mcst_entry.portmask, port))
                    {
                        LOGIC_PORTMASK_SET_PORT(l2Mcst_entry.portmask, port);
                        LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, pGroup->mbr);
                        if (SYS_ERR_OK != (ret = mcast_hw_l2McastEntry_set(pGroup->ipType, &l2Mcst_entry, TRUE, isFwdAll)))
                        {
                            IGMP_DB_SEM_UNLOCK();
                            return ret;
                        }
                    }
                }
            }
            else
            {
                if (pGroup->sip != 0 || IGMP_DIP_VID_MODE == lookup_mode)
                {
                    if (SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46Mcst_entry)))
                    {
                        
                        if (IS_LOGIC_PORTMASK_CLEAR(pGroup->mbr))
                        {
                            if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT != mcast_unknownAct_ret(pGroup->ipType))
                            continue;
                        }

                        if (!IS_LOGIC_PORTMASK_PORTSET(ip46Mcst_entry.portmask, port))
                        {
                            LOGIC_PORTMASK_SET_PORT(ip46Mcst_entry.portmask, port);
                            if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46Mcst_entry, FALSE, isFwdAll)))
                            {
                                IGMP_DB_SEM_UNLOCK();
                                return ret;
                            }
                        }
                    }
                }
            }
        }
    }

    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}

int32 mcast_routerPort_get(uint16 vid, multicast_ipType_t ipType, sys_logic_portmask_t *portmask)
{
    igmp_router_entry_t *rt_enrty;

    mcast_router_db_get(vid, ipType, &rt_enrty);
    if (rt_enrty)
        osal_memcpy(portmask, &rt_enrty->router_pmsk, sizeof(sys_logic_portmask_t));
    else
    {
        osal_memset(portmask, 0, sizeof(sys_logic_portmask_t));
    }

    return SYS_ERR_OK;
}

int32 mcast_aggregate_array_search(uint64 search, uint16 *idx, igmp_aggregate_entry_t **entry)
{
    int low = 0;
    int mid;
    int high = aggregate_db.entry_num - 1;
    igmp_aggregate_entry_t *sortedArray = aggregate_db.aggregate_entry;

    if (idx == NULL || entry == NULL)
        return SYS_ERR_FAILED;

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (sortedArray[mid].sortKey == 0)   
        {
            *entry = NULL;
            *idx = mid;
            return SYS_ERR_OK;
        }

        if (sortedArray[mid].sortKey == search)
        {
            *entry = &sortedArray[mid];
            *idx = mid;
            return SYS_ERR_OK;
        }
        else if (sortedArray[mid].sortKey > search)
        {
            high = mid - 1;
        }
        else if (sortedArray[mid].sortKey < search)
        {
            low = mid + 1;
        }
    }

    *entry = NULL;
    *idx = low;
    return SYS_ERR_OK;
}

int32 mcast_aggregate_array_ins(uint16 sortedIdx, igmp_aggregate_entry_t *entry)
{
    if (entry == NULL)
        return SYS_ERR_FAILED;
    if (sortedIdx > aggregate_db.entry_num)
    {
        MCASTDBG("Error: sortedIdx > aggregate_db.entry_num");
        return SYS_ERR_FAILED;
    }
    if (aggregate_db.entry_num >= CAPA_MCAST_GROUP_NUM + CAPA_IGMP_MVR_GROUP_NUM)
    {
        return SYS_ERR_FAILED;
    }

    if (sortedIdx ==  aggregate_db.entry_num)
    {
        osal_memcpy(aggregate_db.aggregate_entry + sortedIdx, entry, sizeof(igmp_aggregate_entry_t));
    }
    else
    {
        osal_memcpy(tmp_aggr_array, aggregate_db.aggregate_entry + sortedIdx, sizeof(igmp_aggregate_entry_t) * (aggregate_db.entry_num - sortedIdx));
        osal_memcpy(aggregate_db.aggregate_entry + sortedIdx, entry, sizeof(igmp_aggregate_entry_t));
        osal_memcpy(aggregate_db.aggregate_entry + sortedIdx + 1, tmp_aggr_array, sizeof(igmp_aggregate_entry_t) * (aggregate_db.entry_num - sortedIdx));
    }
    aggregate_db.entry_num++;

    return SYS_ERR_OK;
}

int32 mcast_aggregate_array_remove(uint16 sortedIdx)
{
    if (sortedIdx >= aggregate_db.entry_num)
    {
        MCASTDBG("Error: sortedIdx >= aggregate_db.entry_num");
        return SYS_ERR_FAILED;
    }

    if (aggregate_db.entry_num - sortedIdx > 1)
    {
        osal_memcpy(tmp_aggr_array, aggregate_db.aggregate_entry + sortedIdx + 1, sizeof(igmp_aggregate_entry_t) * (aggregate_db.entry_num - sortedIdx - 1));
        osal_memcpy(aggregate_db.aggregate_entry + sortedIdx, tmp_aggr_array, sizeof(igmp_aggregate_entry_t) * (aggregate_db.entry_num - sortedIdx - 1));
    }
    aggregate_db.entry_num--;
    osal_memset(aggregate_db.aggregate_entry + aggregate_db.entry_num, 0, sizeof(igmp_aggregate_entry_t));

    return SYS_ERR_OK;
}

int32 mcast_aggregate_db_init(void)
{
    osal_memset(&aggregate_db, 0, sizeof(aggregate_db));

    aggregate_db.aggregate_entry = osal_alloc(sizeof(igmp_aggregate_entry_t) * CAPA_MCAST_DB_NUM);

    osal_memset(aggregate_db.aggregate_entry, 0, sizeof(igmp_aggregate_entry_t) * CAPA_MCAST_DB_NUM);

    return SYS_ERR_OK;
}

int32 mcast_aggregate_db_add(igmp_group_entry_t *pGroup, sys_logic_port_t port)
{
    uint16                  sortedIdx;
    igmp_aggregate_entry_t  *entry = NULL;
    igmp_aggregate_entry_t  newEntry;
    uint64 sortKey;
    int32 ret = SYS_ERR_FAILED;

    SYS_PARAM_CHK(NULL == pGroup, SYS_ERR_NULL_POINTER);

    sortKey = mcast_aggregate_sortKey_ret(pGroup);

    mcast_aggregate_array_search(sortKey, &sortedIdx, &entry);
    if (NULL ==entry && sortKey != 0)
    {
        osal_memset(&newEntry, 0, sizeof(igmp_aggregate_entry_t));
        newEntry.sortKey = sortKey;
        newEntry.port_ref_cnt[port] = 1;
        newEntry.group_ref_cnt = 1;
        ret =  mcast_aggregate_array_ins(sortedIdx, &newEntry);
    }

    if (SYS_ERR_OK != ret)
    {
        mcast_log_GroupDb_failure(pGroup, MCAST_ADD);

          if (pGroup->ipType == MULTICAST_TYPE_IPV4)
                MCASTDBG("%s():%d  Failed to add a aggregate record  ret:%d", __FUNCTION__, __LINE__, ret);
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
           else
                MCASTDBG("%s():%d  Failed to add a aggregate record  ret:%d", __FUNCTION__, __LINE__, ret);
        #endif
    }

    return ret;
}

int32 mcast_aggregate_db_get(igmp_group_entry_t *pGroup, igmp_aggregate_entry_t **ppEntry, uint8 isNullErr)
{
    uint16              sortedIdx;

    SYS_PARAM_CHK(NULL == ppEntry, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == pGroup, SYS_ERR_NULL_POINTER);

    mcast_aggregate_array_search(mcast_aggregate_sortKey_ret(pGroup), &sortedIdx, ppEntry);
    if (NULL == *ppEntry)
    {
        if (isNullErr)
        {
            mcast_log_GroupDb_failure(pGroup,MCAST_UPDATE);
        }
    }

    return SYS_ERR_OK;
}

int32 mcast_aggregate_db_del(igmp_group_entry_t *pGroup)
{
    uint16          sortedIdx;
    igmp_aggregate_entry_t   *entry;
    uint64 sortKey;
    int32 ret;

    SYS_PARAM_CHK(NULL == pGroup, SYS_ERR_NULL_POINTER);
    sortKey =  mcast_aggregate_sortKey_ret(pGroup);
    mcast_aggregate_array_search(sortKey, &sortedIdx, &entry);
    if (entry)
    {
        ret = mcast_aggregate_array_remove(sortedIdx);
        if (SYS_ERR_OK != ret)
            mcast_log_GroupDb_failure(pGroup, MCAST_DEL);
    }

    return SYS_ERR_FAILED;
}

int32 mcast_aggregatePort_get(igmp_group_entry_t *pGroup, sys_logic_portmask_t *portmask)
{
    igmp_aggregate_entry_t  *pAggrEntry = NULL;
    sys_logic_port_t port;

    SYS_PARAM_CHK(NULL == pGroup, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == portmask, SYS_ERR_NULL_POINTER);

    LOGIC_PORTMASK_CLEAR_ALL(*portmask);

    mcast_aggregate_db_get(pGroup, &pAggrEntry,TRUE);
    if (NULL != pAggrEntry)
    {
        FOR_EACH_LOGIC_PORT(port)
        {
            if (0 != pAggrEntry->port_ref_cnt[port])
            {
                LOGIC_PORTMASK_SET_PORT(*portmask, port);
            }
        }
    }
    else
    {
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

int32 mcast_group_aggregatePort_add(igmp_group_entry_t *pGroup, sys_logic_port_t lport, igmp_lookup_mode_t lookup_mode)
{
     igmp_aggregate_entry_t  *pAggrEntry = NULL;
     sys_ipv46McastEntry_t      ip46McstEntry;
     sys_l2McastEntry_t     l2McstEntry;
     sys_logic_portmask_t  rtPmsk;
     uint8 isFwdAll = TRUE;

     SYS_PARAM_CHK(NULL == pGroup, SYS_ERR_NULL_POINTER);

     if (IGMP_DIP_VID_MODE == lookup_mode)
     {
        if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
            return SYS_ERR_OK;
     }
     else if (IGMP_DIP_SIP_MODE == lookup_mode)
        return SYS_ERR_OK;

     mcast_aggregate_db_get(pGroup, &pAggrEntry,TRUE);
     if (pAggrEntry == NULL)
     {
        return SYS_ERR_FAILED;
     }

    pGroup->groupTimer = 0;

    if (!IS_LOGIC_PORTMASK_PORTSET(pGroup->mbr, lport))
    {
        #ifdef CONFIG_SYS_PROTO_MVR
        isFwdAll = mcast_mvr_fwdAll_ret(pGroup);
        #endif

        LOGIC_PORTMASK_SET_PORT(pGroup->mbr, lport);
        if (0 == pAggrEntry->port_ref_cnt[lport])
        {
            if (IGMP_DMAC_VID_MODE == lookup_mode)
            {
                SYS_MEM_CLEAR(l2McstEntry);
                l2McstEntry.vid = pGroup->vid;
                osal_memcpy(l2McstEntry.mac, pGroup->mac, CAPA_MAC_BYTE_NUM);
                SYS_ERR_CHK(mcast_hw_l2McastEntry_get(&l2McstEntry));
                #ifdef CONFIG_SYS_PROTO_MVR
                if (is_mvr_group_ret2(pGroup))
                {
                    LOGIC_PORTMASK_CLEAR_ALL(rtPmsk);
                    if (MVR_MODE_COMPATABLE == mcast_mvr_mode_ret(pGroup->ipType))
                    {
                        mcast_mvr_pmsk_get(pGroup->ipType, MVR_TYPE_SOURCE, &rtPmsk);
                    }
                    LOGIC_PORTMASK_OR(l2McstEntry.portmask, pGroup->mbr, rtPmsk);
                }
                else
                #endif
                {
                
                    LOGIC_PORTMASK_OR(l2McstEntry.portmask, pGroup->mbr, l2McstEntry.portmask);
                }
                SYS_ERR_CHK(mcast_hw_l2McastEntry_set(pGroup->ipType, &l2McstEntry, FALSE, isFwdAll));
            }
            else if (IGMP_DIP_VID_MODE == lookup_mode)   
            {
                SYS_MEM_CLEAR(ip46McstEntry);
                SYS_ERR_CHK(mcast_hw_ipMcastEntry_get(pGroup, &ip46McstEntry));
                mcast_routerPort_get(pGroup->vid, pGroup->ipType, &rtPmsk);
                #ifdef CONFIG_SYS_PROTO_MVR
                LOGIC_PORTMASK_CLEAR_ALL(rtPmsk);
                if (MVR_MODE_COMPATABLE == mcast_mvr_mode_ret(pGroup->ipType))
                {
                    mcast_mvr_pmsk_get(pGroup->ipType, MVR_TYPE_SOURCE, &rtPmsk);
                }
                #endif
                LOGIC_PORTMASK_OR(ip46McstEntry.portmask, pGroup->mbr, rtPmsk);
                SYS_ERR_CHK(mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46McstEntry,TRUE, isFwdAll));
            }
        }
        pAggrEntry->port_ref_cnt[lport]++;

    }

    return SYS_ERR_OK;
}
int32 mcast_newGroup_aggPortMsk_add(igmp_group_entry_t *pEntry, sys_logic_portmask_t pmsk, uint8 isMvrPackt)
{
    igmp_aggregate_entry_t  *pAggrEntry = NULL;
    sys_ipv46McastEntry_t      ip46McstEntry;
    sys_l2McastEntry_t     l2McstEntry;
    sys_logic_port_t       port, firstPort = 0;
    sys_logic_portmask_t rtPmsk;
    int32 ret = -1;
    igmp_lookup_mode_t lookup_mode = IGMP_DMAC_VID_MODE;
    uint8 isFwdAll = TRUE;

    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
    if (IS_CHIP_CSW())
    {
        MCASTDBG("%s():%d  New group add with vid=%d, dip=%X, groupId=%X",
                    __FUNCTION__, __LINE__, pEntry->vid, pEntry->dip, pEntry->groupId);
    }
#endif

    #ifdef CONFIG_SYS_PROTO_MVR
    if (isMvrPackt)
    {
        if (!is_mcast_mvr_forwardAll(pEntry->ipType))
            isFwdAll = FALSE;
    }
    #endif

    rsd_mcast_lookupMode_get(pEntry->ipType, &lookup_mode);

    if (IGMP_DMAC_VID_MODE == lookup_mode)
    {
        l2McstEntry.vid = pEntry->vid;
        osal_memcpy(l2McstEntry.mac, pEntry->mac, CAPA_MAC_BYTE_NUM);
        ret = mcast_hw_l2McastEntry_get(&l2McstEntry);
        LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, pmsk);
    }
    else
    {
         SYS_MEM_CLEAR(ip46McstEntry);
         ret = mcast_hw_ipMcastEntry_get(pEntry, &ip46McstEntry);
         LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, pmsk);
    }

    mcast_aggregate_db_get(pEntry, &pAggrEntry, (SYS_ERR_OK == ret) ? TRUE: FALSE);
    if (SYS_ERR_OK == ret)
    {
        if (pAggrEntry == NULL)
        {
            mcast_log_GroupDb_failure(pEntry,MCAST_ADD);
            return SYS_ERR_FAILED;
        }

        if (IGMP_DMAC_VID_MODE == lookup_mode)
            SYS_ERR_CHK(mcast_hw_l2McastEntry_set(pEntry->ipType, &l2McstEntry, TRUE, isFwdAll));
        else
            SYS_ERR_CHK(mcast_hw_ipMcastEntry_set(pEntry->ipType, &ip46McstEntry, TRUE, isFwdAll));

        pAggrEntry->group_ref_cnt++;
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pEntry->mbr)
        {
           pAggrEntry->port_ref_cnt[port]++;
        }
    }
    else
    {
        if (pAggrEntry != NULL)
        {
            mcast_log_GroupDb_failure(pEntry,MCAST_ADD);
            return SYS_ERR_FAILED;
        }

        LOGIC_PORTMASK_CLEAR_ALL(rtPmsk);
        #ifdef CONFIG_SYS_PROTO_MVR
        if (isMvrPackt)
        {
            l2McstEntry.crossVlan = ENABLED;
            if (MVR_MODE_COMPATABLE == mcast_mvr_mode_ret(pEntry->ipType))
                mcast_mvr_pmsk_get(pEntry->ipType, MVR_TYPE_SOURCE, &rtPmsk);
        }
        else
        #endif
        {
            mcast_routerPort_get(pEntry->vid, pEntry->ipType, &rtPmsk);
        }

        if (IGMP_DMAC_VID_MODE == lookup_mode)
        {
            LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, rtPmsk);
            ret = mcast_hw_l2McastEntry_add(pEntry->ipType, &l2McstEntry, isFwdAll);
            if (SYS_ERR_OK != ret)
                mcast_log_asicMac_fail(&l2McstEntry,MCAST_ADD);
        }
        else
        {
            LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, rtPmsk);
            ret = mcast_hw_ipMcastEntry_add(pEntry->ipType, &ip46McstEntry, isFwdAll);
            if (SYS_ERR_OK != ret)
               mcast_log_asicIP_fail(pEntry->ipType, &ip46McstEntry,MCAST_ADD);
            else
            {
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                if (IS_CHIP_CSW())
                {
                    if (MULTICAST_TYPE_IPV4 == pEntry->ipType)
                    {
                        pEntry->groupId = ip46McstEntry.ipMcast.groupId;
                    }
                    else
                    {
                        pEntry->groupId = ip46McstEntry.ipv6Mcast.groupId;
                    }
                }
#endif
            }
        }

        if (SYS_ERR_OK != ret)
        {
            return ret;
        }
        else
        {
            
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pmsk)
            {
                firstPort = port;
                break;
            }
            mcast_aggregate_db_add(pEntry, firstPort);
            mcast_aggregate_db_get(pEntry, &pAggrEntry,TRUE);
            if (pAggrEntry == NULL)
            {
                mcast_log_GroupDb_failure(pEntry,MCAST_ADD);
                return SYS_ERR_FAILED;
            }
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pmsk)
            {
                if (firstPort != port)
                {
                    pAggrEntry->port_ref_cnt[port]++;
                }
            }
            pEntry->lookupTableFlag = L2TABLE_WROTE;
            mcast_hw_group_num_cal(TRUE);
        }
    }

    return SYS_ERR_OK;
}
int32 mcastData_newGroup_add(igmp_group_entry_t *pEntry, uint8 isMvrPackt, uint16 sortedIdx)
{
    igmp_aggregate_entry_t  *pAggrEntry;
    sys_l2McastEntry_t      l2McstEntry;
    sys_ipv46McastEntry_t ip46McstEntry;
    sys_logic_portmask_t    rtPmsk;
    int32 ret = SYS_ERR_FAILED;
    sys_logic_port_t port = 0;
    igmp_lookup_mode_t lookup_mode = IGMP_DMAC_VID_MODE;
    sys_vlan_operEntry_t    vlanEntry;
    uint8 isFwdAll = TRUE;

    rsd_mcast_lookupMode_get(pEntry->ipType, &lookup_mode);

    if  ((IGMP_DMAC_VID_MODE == lookup_mode) || (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != pEntry->ipType))
    {
        
        mcast_aggregate_db_get(pEntry, &pAggrEntry,FALSE);
        if (pAggrEntry == NULL)
        {
            if (SYS_ERR_OK != (ret = mcast_aggregate_db_add(pEntry, port)))
            {
                mcast_log_GroupDb_failure(pEntry,MCAST_ADD);
                return SYS_ERR_FAILED;
            }

            mcast_aggregate_db_get(pEntry, &pAggrEntry,TRUE);
            if (NULL != pAggrEntry)
            {
                pAggrEntry->port_ref_cnt[port]--;
            }
            else
            {
                mcast_log_GroupDb_failure(pEntry,MCAST_ADD);
                return SYS_ERR_FAILED;
            }
        }
        else
        {

            mcast_log_GroupDb_failure(pEntry,MCAST_ADD);
            return SYS_ERR_FAILED;
        }
    }

    #ifdef CONFIG_SYS_PROTO_MVR
    
    if (isMvrPackt)
    {
        if (!is_mcast_mvr_forwardAll(pEntry->ipType))
            isFwdAll = FALSE;

        if (MVR_MODE_COMPATABLE == mcast_mvr_mode_ret(pEntry->ipType))
            mcast_mvr_pmsk_get(pEntry->ipType, MVR_TYPE_SOURCE, &rtPmsk);
    }
    else
    #endif
    {
        mcast_routerPort_get(pEntry->vid, pEntry->ipType, &rtPmsk);
    }

    if (IGMP_DMAC_VID_MODE == lookup_mode)
    {
        SYS_MEM_CLEAR(l2McstEntry);
        l2McstEntry.vid = pEntry->vid;
        osal_memcpy(l2McstEntry.mac, pEntry->mac, CAPA_MAC_BYTE_NUM);
        LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, rtPmsk);
        ret = mcast_hw_l2McastEntry_add(pEntry->ipType, &l2McstEntry, isFwdAll);
        if (SYS_ERR_OK != ret)
            mcast_log_asicMac_fail(&l2McstEntry,MCAST_ADD);
    }
    else
    {
         SYS_MEM_CLEAR(ip46McstEntry);
         if (MULTICAST_TYPE_IPV4 == pEntry->ipType)
         {
             mcast_ipMcast_asicKey_get(pEntry->vid, pEntry->dip, pEntry->sip, &ip46McstEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                ip46McstEntry.ipMcast.groupId = pEntry->groupId;
            }
#endif
         }
         else
         {
             mcast_ipv6Mcast_asicKey_get(pEntry->vid, pEntry->dipv6.addr, pEntry->sipv6.addr, &ip46McstEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                ip46McstEntry.ipv6Mcast.groupId = pEntry->groupId;
            }
#endif
         }

        if (IGMP_UNKNOWN_MCAST_ACTION_FLOOD == mcast_unknownAct_ret(pEntry->ipType))
        {
            SYS_MEM_CLEAR(vlanEntry);
            rsd_vlan_operEntry_get(pEntry->vid, &vlanEntry);
            LOGIC_PORTMASK_COPY(ip46McstEntry.portmask, vlanEntry.lpmMember);
        }
        else if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(pEntry->ipType))
        {
            LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, rtPmsk);
        }

        ret = mcast_hw_ipMcastEntry_add(pEntry->ipType, &ip46McstEntry, isFwdAll);
        if (SYS_ERR_OK != ret)
            mcast_log_asicIP_fail(pEntry->ipType, &ip46McstEntry, MCAST_ADD);
        else
        {
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                    if (IS_CHIP_CSW())
                    {
                        if (MULTICAST_TYPE_IPV4 == pEntry->ipType)
                        {
                            if (IS_LOGIC_PORTMASK_CLEAR(ip46McstEntry.ipMcast.portmask))
                            {
                                mcast_hw_ipMcastEntryIncCPU_set(pEntry->ipType, &ip46McstEntry);
                            }
                        }
                        else
                        {
                            if (IS_LOGIC_PORTMASK_CLEAR(ip46McstEntry.ipv6Mcast.portmask))
                                mcast_hw_ipMcastEntryIncCPU_set(pEntry->ipType, &ip46McstEntry);
                        }
                    }
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                if (MULTICAST_TYPE_IPV4 == pEntry->ipType)
                {
                    pEntry->groupId = ip46McstEntry.ipMcast.groupId;
                }
                else
                {
                    pEntry->groupId = ip46McstEntry.ipv6Mcast.groupId;
                }
            }
#endif
        }

    }

    if (SYS_ERR_OK != ret)
    {
        if  ((IGMP_DMAC_VID_MODE == lookup_mode) || (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != pEntry->ipType))
            mcast_aggregate_db_del(pEntry);
        return SYS_ERR_FAILED;
    }
    else
    {
        pEntry->lookupTableFlag = L2TABLE_WROTE;
        mcast_hw_group_num_cal(TRUE);
        mcast_group_sortedArray_ins(sortedIdx, pEntry);
        mcast_group_num_cal(pEntry->ipType,FALSE, isMvrPackt,TRUE);
    }

    return SYS_ERR_OK;
}

int32 mcast_newGroup_port_add(igmp_group_entry_t *pEntry, sys_logic_port_t port, uint8 isMvrPackt, igmp_lookup_mode_t lookup_mode, uint16 sortedIdx)
{
    sys_ipv46McastEntry_t      ip46McstEntry;
    sys_l2McastEntry_t     l2McstEntry;
    sys_logic_portmask_t  rtPmsk;
    multicast_ipType_t ipType;
    int32 ret = -1;
    uint8 isFwdAll = TRUE;

    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);
    ipType = pEntry->ipType;

    SYS_MEM_CLEAR(l2McstEntry);
    SYS_MEM_CLEAR(ip46McstEntry);

    LOGIC_PORTMASK_SET_PORT(l2McstEntry.portmask, port);
    LOGIC_PORTMASK_SET_PORT(ip46McstEntry.portmask, port);

    #ifdef CONFIG_SYS_PROTO_MVR
    if (isMvrPackt)
    {
        if (!is_mcast_mvr_forwardAll(pEntry->ipType))
            isFwdAll = FALSE;

        if (MVR_MODE_COMPATABLE == mcast_mvr_mode_ret(ipType))
        {
            mcast_mvr_pmsk_get(ipType, MVR_TYPE_SOURCE, &rtPmsk);
            LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, rtPmsk);
            LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, rtPmsk);
        }
    }
    else
    #endif
    {
        
        mcast_routerPort_get(pEntry->vid, ipType, &rtPmsk);
        LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, rtPmsk);
        LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, rtPmsk);
    }

    l2McstEntry.vid = pEntry->vid;
    osal_memcpy(l2McstEntry.mac, pEntry->mac, CAPA_MAC_BYTE_NUM);
    if (isMvrPackt)
        l2McstEntry.crossVlan = ENABLED;

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        ip46McstEntry.ipMcast.dip = pEntry->dip;
        ip46McstEntry.ipMcast.vid = pEntry->vid;
     }
     else
     {
        ip46McstEntry.ipv6Mcast.vid = pEntry->vid;
        osal_memcpy(ip46McstEntry.ipv6Mcast.dip.octet, pEntry->dipv6.addr, IPV6_ADDR_LEN);
     }

    if ((IGMP_DMAC_VID_MODE == lookup_mode) || (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != ipType ))
    {
        
        if ( SYS_ERR_OK != (ret = mcast_newGroup_aggPortMsk_add(pEntry, pEntry->mbr, isMvrPackt)))
        {
            if (MULTICAST_TYPE_IPV4 == pEntry->ipType)
            {
                MCASTDBG("Failed add new group port member to ASIC!  ret:%d  vid:%d   mac:"MAC_PRINT"",
                        ret, pEntry->vid, MAC_PRINT_ARG(l2McstEntry.mac));
            }
            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
            {
                MCASTDBG("Failed writing to ASIC!  ret:%d  vid:%d   dip:"IPADDRV6_PRINT"",
                        ret,pEntry->vid, IPADDRV6_PRINT_ARG(ip46McstEntry.ipv6Mcast.dip.octet));
            }
            #endif
            return ret;
        }
    }
    else
    {
        ret = mcast_hw_ipMcastEntry_add(ipType, &ip46McstEntry, isFwdAll);
        if (SYS_ERR_OK == ret)
        {
            pEntry->lookupTableFlag = L2TABLE_WROTE;
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                if (MULTICAST_TYPE_IPV4 == ipType)
                {
                    pEntry->groupId = ip46McstEntry.ipMcast.groupId;
                }
                else
                {
                    pEntry->groupId = ip46McstEntry.ipv6Mcast.groupId;
                }
            }
#endif
            mcast_hw_group_num_cal(TRUE);
        }
        else
        {
            mcast_log_asicIP_fail(ipType, &ip46McstEntry, MCAST_ADD);
            return ret;
        }
    }

    return SYS_ERR_OK;
}
int32 mcast_group_db_init(void)
{
    int i;
    igmp_group_entry_t  *iter;

    if (CAPA_MCAST_GROUP_NUM + CAPA_IGMP_MVR_GROUP_NUM > CAPA_MCAST_DB_NUM)
    {
        MCASTDBG("define Multicast max group and MVR max group is over max group entry!");
        return SYS_ERR_FAILED;
    }

    for (i = 0; i < CAPA_MCAST_GROUP_NUM + CAPA_IGMP_MVR_GROUP_NUM; i++)
    {
        group_sortedArray[i] = NULL;
        osal_memset(&group_db[i], 0, sizeof(igmp_group_entry_t));
    }

    igmpGroupFreeHead.freeListNumber = CAPA_MCAST_GROUP_NUM + CAPA_IGMP_MVR_GROUP_NUM;
    iter = igmpGroupFreeHead.item = &group_db[0];
    for (i = 1; i < CAPA_MCAST_GROUP_NUM + CAPA_IGMP_MVR_GROUP_NUM; iter = iter->next_free, i++)
    {
        iter->next_free = &group_db[i];
    }
    iter->next_free = NULL;

    group_sortedAry_entry_num = 0;
    igmp_group_num = 0;
    mld_group_num = 0;
    mcast_group_num = 0;
    mcast_mvr_group_num = 0;

    mcast_staticMac_num = 0;
    igmp_staticMac_num = 0;
    mld_staticMac_num = 0;
    mcast_group_full = FALSE;

    igmp_hw_entry_num = 0;
    igmp_trapDataPkt_lock = FALSE;

    return SYS_ERR_OK;
}

int32 mcast_group_sortedArray_search(uint64 search, uint16 *idx, igmp_group_entry_t **groupHead)
{
    int low = 0;
    int mid;
    int high = group_sortedAry_entry_num - 1;

    SYS_PARAM_CHK(NULL == idx, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == groupHead, SYS_ERR_NULL_POINTER);

    while (low <= high)
    {
        mid = (low + high) / 2;

        if (group_sortedArray[mid] == NULL)   
        {
            *groupHead = NULL;
            *idx = mid;
            return SYS_ERR_OK;
        }

        if (group_sortedArray[mid]->sortKey == search)
        {
            *groupHead = group_sortedArray[mid];
            *idx = mid;
            return SYS_ERR_OK;
        }
        else if (group_sortedArray[mid]->sortKey > search)
        {
            high = mid - 1;
        }
        else if (group_sortedArray[mid]->sortKey < search)
        {
            low = mid + 1;
        }
    }

    *groupHead = NULL;
    *idx = low;
    return SYS_ERR_OK;

}
int32 mcast_group_sortedArray_ins(uint16 sortedIdx, igmp_group_entry_t *entry)
{
    if (entry == NULL)
        return SYS_ERR_FAILED;
    if (sortedIdx > group_sortedAry_entry_num)
    {
        MCASTDBG("Error: sortedIdx:%d > group_sortedAry_entry_num:%d", sortedIdx, group_sortedAry_entry_num);
        mcast_log_GroupDb_failure(entry, MCAST_ADD);
        return SYS_ERR_FAILED;
    }

    if (group_sortedAry_entry_num == sortedIdx)
    {
        group_sortedArray[sortedIdx] = entry;
    }
    else
    {
        osal_memcpy(tmp_group_sortedArray, group_sortedArray + sortedIdx, sizeof(igmp_group_entry_t*) * (group_sortedAry_entry_num - sortedIdx));
        group_sortedArray[sortedIdx] = entry;
        osal_memcpy(group_sortedArray + sortedIdx + 1, tmp_group_sortedArray, sizeof(igmp_group_entry_t*) * (group_sortedAry_entry_num - sortedIdx));
    }
    group_sortedAry_entry_num++;

    return SYS_ERR_OK;
}

int32 mcast_group_sortedArray_del(uint16 sortedIdx)
{
    if (sortedIdx >= group_sortedAry_entry_num)
    {
        MCASTDBG("Error: sortedIdx:%d >= group_sortedAry_entry_num:%d", sortedIdx, group_sortedAry_entry_num);
        return SYS_ERR_FAILED;
    }

    if (group_sortedAry_entry_num - sortedIdx  > 1)
    {
        osal_memcpy(tmp_group_sortedArray, group_sortedArray + sortedIdx + 1, sizeof(igmp_group_entry_t*) * (group_sortedAry_entry_num - sortedIdx - 1));
        osal_memcpy(group_sortedArray + sortedIdx, tmp_group_sortedArray, sizeof(igmp_group_entry_t*) * (group_sortedAry_entry_num - sortedIdx - 1));
    }
    group_sortedAry_entry_num--;
    group_sortedArray[group_sortedAry_entry_num] = NULL;

    return SYS_ERR_OK;
}

igmp_group_entry_t* mcast_group_freeEntry_get(multicast_ipType_t ipType, uint8 isMvrGroup)
{
    igmp_group_entry_t *entry;
    uint32 igmp_max_group = CAPA_IGMP_GROUP_NUM;
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    uint32 mld_max_group = CAPA_MLD_GROUP_NUM;
#else
    uint32 mld_max_group = 0;
#endif

    if (igmpGroupFreeHead.freeListNumber == 0)
        return NULL;

    if (isMvrGroup)
    {
        if (mcast_mvr_group_num >= CAPA_IGMP_MVR_GROUP_NUM)
            return NULL;
    }
    else
    {
        if ( igmp_group_num + mld_group_num >= CAPA_MCAST_GROUP_NUM)
            return NULL;

        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            if (igmp_group_num >= igmp_max_group)
                return NULL;
        }
        else
        {
            if (mld_group_num >= mld_max_group)
                return NULL;
        }
    }

    entry = igmpGroupFreeHead.item;
    if (NULL == entry)
    {
        return NULL;
    }
    igmpGroupFreeHead.item = igmpGroupFreeHead.item->next_free;
    igmpGroupFreeHead.freeListNumber--;
    return entry;
}

int32 mcast_group_entry_release(igmp_group_entry_t *entry)
{
    multicast_ipType_t ipType;

    if (entry == NULL)
    {
        MCASTDBG("Error: return NULL pointer!");
        return SYS_ERR_FAILED;
    }

    ipType = entry->ipType;
    osal_memset(entry, 0, sizeof(igmp_group_entry_t));
    entry->next_subgroup = NULL;
    entry->next_free = igmpGroupFreeHead.item;
    igmpGroupFreeHead.item = entry;
    igmpGroupFreeHead.freeListNumber++;

    return SYS_ERR_OK;
}

int32 mcast_build_groupEntryKey(sys_vid_t vid, multicast_ipType_t ipType, uint32 dip, char *ipv6Addr,  igmp_group_entry_t *pGroup)
{
    SYS_PARAM_CHK(NULL == pGroup, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK((0 == dip), SYS_ERR_IPV4_ADDR);

    osal_memset(pGroup, 0, sizeof(igmp_group_entry_t));
    pGroup->vid = vid;
    pGroup->ipType = ipType;
    pGroup->dip = dip;

    if (MULTICAST_TYPE_IPV4 != ipType)
    {
        SYS_PARAM_CHK(NULL == ipv6Addr, SYS_ERR_NULL_POINTER);
        osal_memcpy(pGroup->dipv6.addr, ipv6Addr, IPV6_ADDR_LEN);
    }

    return SYS_ERR_OK;
}

uint64 mcast_group_sortKey_ret(igmp_group_entry_t *pGroup)
{
    uint64  ret = 0;
    igmp_lookup_mode_t lookup_mode;
    if (NULL == pGroup)
        return ret;

    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
    {
        ret = ((uint64)((pGroup->dip) & 0xffffffff) << 12)  | ((uint64)(pGroup->vid & 0xfff));
        
    }
    else
    {
        rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);

        ret = ( ((uint64)(pGroup->dip) & 0xffffffff) << 12)  | (pGroup->vid & 0xfff);
        if (IGMP_DMAC_VID_MODE == lookup_mode || IGMP_DIP_VID_MODE == lookup_mode)
        {
            ret = ((uint64)((pGroup->dipv6.addr[1]) & 0xff) << 44) | ret;
            ret = ((uint64)((pGroup->dipv6.addr[0]) & 0xff) << 52) | ret;
        }
        else
        {
            if (pGroup->dipv6.addr[0] != 0xff)
            {
                ret = ((uint64)((pGroup->dipv6.addr[1]) & 0xff) << 44) | ret;
                ret = ((uint64)((pGroup->dipv6.addr[0]) & 0xff) << 52) | ret;
            }
        }
        ret = ((uint64)0x01 << 60) | ret;

    }
   return ret;
}
uint64 mcast_aggregate_sortKey_ret(igmp_group_entry_t *pGroup)
{
    uint64  ret = 0;
    uint32 groupIp = 0;
    igmp_lookup_mode_t lookup_mode;

     if (NULL == pGroup)
        return ret;

    if (pGroup->ipType < MULTICAST_TYPE_IPV4 ||pGroup->ipType >= MULTICAST_TYPE_END)
        return ret;

    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
    {
        groupIp =  (pGroup->dip & 0x7fffff) | 0x5e000000;
        ret = ((uint64)(groupIp & 0xffffffff) << 12) | ((uint64)(pGroup->vid & 0xfff));
        ret = ((uint64)0x0100 << 44)  | ret;
    }
    else
    {
        rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);
        if (IGMP_DIP_VID_MODE == lookup_mode)
        {
            ret = ( ((uint64)(pGroup->dip) & 0xffffffff) << 12)  | (pGroup->vid & 0xfff);
            ret = ((uint64)0x01 << 60) | ret;
        }
        else
        {
            ret = ((uint64)(pGroup->dip & 0xffffffff) << 12) | ((uint64)(pGroup->vid & 0xfff));
            ret = ((uint64)0x3333 << 44) | ret;
            ret = ((uint64)0x01 << 60) | ret;
        }
    }

   return ret;
}

int32 mcast_group_add(igmp_group_entry_t *pNewEntry)
{
    uint16              i, sortedIdx;
    uint16              vid;
    uint32              dip, sip;
    igmp_group_entry_t  *tmpEntry;
    igmp_group_entry_t   *subgroup;

    igmp_group_entry_t   *entry, *groupHead;

    mcast_vlan_entry_t *pMcastVlan = NULL;
    multicast_ipType_t ipType;
    igmp_lookup_mode_t lookup_mode;
    SYS_PARAM_CHK(NULL == pNewEntry, SYS_ERR_NULL_POINTER);
    vid = pNewEntry->vid;
    dip = pNewEntry->dip;
    sip = pNewEntry->sip;
    ipType = pNewEntry->ipType;
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        return SYS_ERR_FAILED;
    }

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(pNewEntry), &sortedIdx, &groupHead);   
    if (groupHead)    
    {
        rsd_mcast_lookupMode_get(groupHead->ipType, &lookup_mode);

        if (IGMP_DMAC_VID_MODE == lookup_mode)
        {
            osal_memcpy(groupHead->mac, pNewEntry->mac, CAPA_MAC_BYTE_NUM);
            osal_memcpy(&groupHead->mbr, &pNewEntry->mbr, sizeof(sys_logic_portmask_t));
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(i, pNewEntry->mbr)
            {
                groupHead->mbrTimer[i] = pMcastVlan->grpMembershipIntv;
            }
        }
        else
        {
            mcast_igmp_subgroup_sip_search(groupHead, sip, &subgroup, NULL);
            if (subgroup == NULL)
            {
                entry = mcast_group_freeEntry_get(ipType, FALSE);
                if (entry != NULL)
                {
                    entry->dip = dip;
                    entry->sip = sip;
                    entry->vid = vid;
                    entry->ipType = ipType;

                    entry->sortKey = mcast_group_sortKey_ret(pNewEntry);
                    entry->groupTimer = pNewEntry->groupTimer;
                    osal_memcpy(entry->mac, pNewEntry->mac, CAPA_MAC_BYTE_NUM);
                    osal_memcpy(&entry->mbr, &pNewEntry->mbr, sizeof(sys_logic_portmask_t));

                    if (MULTICAST_TYPE_IPV4 != ipType)
                    {
                        osal_memcpy(entry->dipv6.addr, pNewEntry->dipv6.addr, IPV6_ADDR_LEN);
                        osal_memcpy(entry->sipv6.addr, pNewEntry->sipv6.addr, IPV6_ADDR_LEN);
                    }

                    entry->form = pNewEntry->form;
                    entry->lookupTableFlag = pNewEntry->lookupTableFlag;
                    osal_memcpy(&entry->staticMbr, &pNewEntry->staticMbr,sizeof(sys_logic_portmask_t));
                    osal_memcpy(&entry->staticInMbr, &pNewEntry->staticInMbr,sizeof(sys_logic_portmask_t));

                    FOR_EACH_PORT_IN_LOGIC_PORTMASK(i, pNewEntry->mbr)
                    {
                        if(IGMP_GROUP_ENTRY_DYNAMIC == pNewEntry->form)
                        {
                            entry->mbrTimer[i] = pMcastVlan->grpMembershipIntv;
                         }

                        entry->v3.mbrFwdFlag[i] = pNewEntry->v3.mbrFwdFlag[i] ;
                    }

                    mcast_group_num_cal(ipType,FALSE, FALSE,TRUE);
                    tmpEntry = group_sortedArray[sortedIdx]->next_subgroup;
                    group_sortedArray[sortedIdx]->next_subgroup = entry;
                    entry->next_subgroup = tmpEntry;
                    MCASTDBG("subgroup add  successfull !");
                    return SYS_ERR_OK;
                }
                else
                {
                    
                    mcast_log_tablefull(ipType);
                    return SYS_ERR_MCAST_DATABASE_FULL;
                }
            }
            else
            {
                MCASTDBG("subgroup has existed!");
                
                osal_memcpy(&subgroup->mbr, &pNewEntry->mbr, sizeof(sys_logic_portmask_t));
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(i, pNewEntry->mbr)
                {
                    subgroup->mbrTimer[i] = pMcastVlan->grpMembershipIntv;
                }
            }
        }
    }
    else    
    {
        entry = mcast_group_freeEntry_get(ipType, FALSE);
        if (entry != NULL)
        {
            osal_memset(entry, 0, sizeof(igmp_group_entry_t));

            entry->dip = dip;
            entry->sip = sip;
            entry->vid = vid;
            entry->sortKey = mcast_group_sortKey_ret(pNewEntry);

            entry->form = pNewEntry->form;
            entry->lookupTableFlag = pNewEntry->lookupTableFlag;
            osal_memcpy(&entry->staticMbr, &pNewEntry->staticMbr,sizeof(sys_logic_portmask_t));
            osal_memcpy(&entry->staticInMbr, &pNewEntry->staticInMbr,sizeof(sys_logic_portmask_t));

            osal_memcpy(entry->mac, pNewEntry->mac, CAPA_MAC_BYTE_NUM);
            osal_memcpy(entry->dipv6.addr, pNewEntry->dipv6.addr, IPV6_ADDR_LEN);
            osal_memcpy(entry->sipv6.addr, pNewEntry->sipv6.addr, IPV6_ADDR_LEN);
            osal_memcpy(&entry->mbr, &pNewEntry->mbr, sizeof(sys_logic_portmask_t));

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(i, pNewEntry->mbr)
            {
                entry->mbrTimer[i] = pMcastVlan->grpMembershipIntv;
                entry->v3.mbrFwdFlag[i] = pNewEntry->v3.mbrFwdFlag[i] ;
            }

            mcast_group_num_cal(ipType,FALSE, FALSE, TRUE);
            mcast_group_sortedArray_ins(sortedIdx, entry);
            return SYS_ERR_OK;
        }
        else
        {
            mcast_log_tablefull(ipType);
            return SYS_ERR_MCAST_DATABASE_FULL;
        }
    }

    return SYS_ERR_FAILED;
}
igmp_group_entry_t* mcast_group_get(igmp_group_entry_t *pGroup)
{
    uint16              sortedIdx;
    igmp_group_entry_t  *groupHead;
    igmp_group_entry_t  *subgroup;
    igmp_lookup_mode_t lookup_mode;

    if (NULL == pGroup)
        return NULL;

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(pGroup), &sortedIdx, &groupHead);   
    if (groupHead)    
    {
        rsd_mcast_lookupMode_get(groupHead->ipType, &lookup_mode);
        if(IGMP_DMAC_VID_MODE == lookup_mode)
        {
            return groupHead;
        }
        else
        {
            mcast_igmp_subgroup_sip_search(groupHead, pGroup->sip, &subgroup, NULL);
            if (subgroup)
                return subgroup;
            else
                return NULL;
        }
    }

    return NULL;
}

igmp_group_entry_t* mcast_group_get_with_index(uint32 index)
{
    uint16              i, j = 0;
    igmp_group_entry_t  *subgroup;
    for (i = 0; i < group_sortedAry_entry_num; i++)
    {
        if (group_sortedArray[i] == NULL)
            break;

        subgroup = group_sortedArray[i];
        while (subgroup)
        {
            if (j == index)
                return subgroup;

            subgroup = subgroup->next_subgroup;
            j++;
        }
    }

    return NULL;
}
igmp_group_entry_t* mcast_dip_group_get_with_index(uint32 index)
{
    if (index >= group_sortedAry_entry_num)
        return NULL;

    return group_sortedArray[index];
}
int32 mcast_group_del(igmp_group_entry_t *pGroup)
{
    uint16          sortedIdx;
    igmp_group_entry_t  *prevNode;
    igmp_group_entry_t  *subgroup;
    igmp_group_entry_t  *groupHead;
    igmp_group_entry_t   delGroup;
    mcast_groupAddr_t groupAddr;
    uint8 isMvrGroup = FALSE;
    uint8 isStaticMac = FALSE;
    igmp_lookup_mode_t lookup_mode;
    int32 ret1 = SYS_ERR_OK, ret2 = SYS_ERR_OK;

    SYS_PARAM_CHK(NULL == pGroup, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VID_INVALID(pGroup->vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((pGroup->ipType < MULTICAST_TYPE_IPV4 ||pGroup->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    osal_memset(&delGroup, 0, sizeof(igmp_group_entry_t));
    osal_memcpy(&delGroup, pGroup, sizeof(igmp_group_entry_t));

    groupAddr.ipv4 = delGroup.dip;
    osal_memcpy(groupAddr.ipv6.addr, delGroup.dipv6.addr,IPV6_ADDR_LEN);

    #ifdef CONFIG_SYS_PROTO_MVR
    if (is_mvr_group_ret1(delGroup.ipType, delGroup.vid, &groupAddr, TRUE))
        isMvrGroup = TRUE;
    #endif

    if (!is_mcast_group(pGroup))
        isStaticMac = TRUE;

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(pGroup), &sortedIdx, &groupHead);   
    if (groupHead)    
    {
        rsd_mcast_lookupMode_get(groupHead->ipType, &lookup_mode);
        if(IGMP_DMAC_VID_MODE == lookup_mode || isStaticMac)
        {
            if (SYS_ERR_OK == (ret1 = mcast_group_sortedArray_del(sortedIdx)))
            {
                mcast_group_num_cal(delGroup.ipType, isStaticMac, isMvrGroup, FALSE);
                ret2 = mcast_group_entry_release(groupHead);
                if (SYS_ERR_OK != ret2)
                {
                    mcast_log_GroupDb_failure(&delGroup,MCAST_DEL);
                }

                return SYS_ERR_OK;
            }
        }
        else
        {
            prevNode = NULL;
            mcast_igmp_subgroup_sip_search(groupHead, delGroup.sip, &subgroup, &prevNode);
            if (subgroup)
            {
                if (subgroup == group_sortedArray[sortedIdx])   
                {
                    group_sortedArray[sortedIdx] = subgroup->next_subgroup;
                    if (group_sortedArray[sortedIdx] == NULL)
                        ret1 = mcast_group_sortedArray_del(sortedIdx);
                    ret2 = mcast_group_entry_release(subgroup);
                    mcast_group_num_cal(delGroup.ipType,isStaticMac, isMvrGroup,FALSE);
                }
                else
                {
                    prevNode->next_subgroup = subgroup->next_subgroup;
                    ret1 = mcast_group_entry_release(subgroup);
                    mcast_group_num_cal(delGroup.ipType, isStaticMac, isMvrGroup,FALSE);
                }

                if (SYS_ERR_OK != ret1 || SYS_ERR_OK != ret2)
                {
                    mcast_log_GroupDb_failure(&delGroup, MCAST_DEL);
                }

                return SYS_ERR_OK;
            }
        }
    }

    mcast_log_GroupDb_failure(&delGroup,MCAST_DEL);
    return SYS_ERR_FAILED;
}

int32 mcast_group_subgroup_hw_del_all(igmp_group_entry_t  *entry)
{
    igmp_group_entry_t  *subgroup;
    sys_l2McastEntry_t  l2Mcst_entry;
    sys_ipv46McastEntry_t  ip46Mcst_entry;
    igmp_lookup_mode_t lookup_mode;

    subgroup = entry;
    while (subgroup)
    {
        rsd_mcast_lookupMode_get(subgroup->ipType, &lookup_mode);
        if (IGMP_DMAC_VID_MODE == lookup_mode)
        {
            l2Mcst_entry.vid = subgroup->vid;
            osal_memcpy(l2Mcst_entry.mac, subgroup->mac, CAPA_MAC_BYTE_NUM);
            mcast_hw_l2McastEntry_del(&l2Mcst_entry);
        }
        else
        {
            if(0 != subgroup->sip || IGMP_DIP_VID_MODE == lookup_mode)
            {
                if (SYS_ERR_OK == mcast_hw_ipMcastEntry_get(entry, &ip46Mcst_entry))
                {
                    mcast_hw_ipMcastEntry_del(entry->ipType, &ip46Mcst_entry);
                }
            }
        }
        subgroup = subgroup->next_subgroup;
    }
    return SYS_ERR_OK;
}

int32 mcast_group_del_all(void)
{
    int i;
    sys_vlanmask_t  vlanMsk;
    igmp_lookup_mode_t lookup_mode;

    igmp_packet_stop = ENABLED;

   VLANMASK_SET_ALL(vlanMsk);
   mcast_static_group_save(vlanMsk, MULTICAST_TYPE_END);

    for (i = 0; i < CAPA_MCAST_DB_NUM; i++)
    {
        if (group_sortedArray[i] == NULL)
            break;

        mcast_group_subgroup_hw_del_all(group_sortedArray[i]);
    }

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    mcast_igmp_group_head_db_init();
#endif
    mcast_group_db_init();

    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4, &lookup_mode);

    mcast_aggregate_db_init();
    mcast_static_group_restore();

    igmp_packet_stop = DISABLED;

    return SYS_ERR_OK;
}
int32 mcast_suppre_flag_check(igmp_group_entry_t *pEntry, uint8 *pSendFlag, uint8 *isSetFlag)
{
    SYS_PARAM_CHK((NULL == pSendFlag), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == isSetFlag), SYS_ERR_NULL_POINTER);

    *isSetFlag = 0;

    if (DISABLED == igmp_cfg.suppre_enable)
    {
        *pSendFlag = 1;
        return SYS_ERR_OK;
    }

    if (pEntry)
    {
        if (pEntry->suppreFlag)
        {
            *pSendFlag = 0;
        }
        else
        {
            pEntry->suppreFlag = 1;
            *pSendFlag = 1;
            *isSetFlag = 1;
        }
    }
    else
        *pSendFlag = 1;

    return SYS_ERR_OK;
}
int32 mcast_groupMbrPort_add_wrapper(multicast_ipType_t ipType, uint16 vid, sys_nic_pkt_t *pBuf,
     mcast_groupAddr_t *pAddr, sys_logic_port_t port,  uint8 *pSendFlag, uint8 isMvrPackt)
{
    uint16                  sortedIdx = 0;
    uint32                  dip = 0;
    igmp_group_entry_t      groupKeyEntry;
    igmp_group_entry_t      *pEntry = NULL;
    igmp_group_entry_t      *groupHead = NULL;
    sys_ipv46McastEntry_t      ip46McstEntry;
    int32                   ret = SYS_ERR_OK;
    sys_l2McastEntry_t      l2McstEntry;
    sys_logic_portmask_t    rtPmsk;
    uint32                  portEntryNum = 0;
    uint16                  limitNum = CAPA_MCAST_GROUP_NUM;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    igmp_lookup_mode_t lookup_mode;
    uint32 portTime = 0;
    uint8 isFwdAll = TRUE;
    uint8 isSetSupFlag = 0;

    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    igmp_group_head_entry_t *pGroup_head_entry = NULL;
    #endif

    osal_memset(&l2McstEntry, 0, sizeof(l2McstEntry));
    LOGIC_PORTMASK_CLEAR_ALL(rtPmsk);

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        dip = pAddr->ipv4;
    }
    else
    {
        
        dip = IPV6_TO_IPV4(pAddr->ipv6.addr);
    }

    mcast_dbg_jn_lv_report(ipType,vid, port, pAddr,TRUE);

    SYS_ERR_CHK(mcast_filter_check(ipType, port, pAddr));
    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, dip,pAddr->ipv6.addr, &groupKeyEntry));
    rsd_mcast_lookupMode_get(ipType, &lookup_mode);

    #ifdef CONFIG_SYS_PROTO_MVR
    isFwdAll = mcast_mvr_fwdAll_ret(&groupKeyEntry);
    #endif

    IGMP_DB_SEM_LOCK();

    mcast_port_entry_get( ipType, port, &portEntryNum);
    mcast_portGroup_limit_get(ipType, port, &limitNum);

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupKeyEntry), &sortedIdx, &groupHead);   
    mcast_suppre_flag_check(groupHead, pSendFlag,&isSetSupFlag);

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);

    if (NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }
    portTime = pMcastVlan->grpMembershipIntv;

    if (groupHead)
    {
        if (IGMP_GROUP_ENTRY_STATIC == groupHead->form)
        {
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        if(!IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, port))
        {
            if(portEntryNum >= limitNum)
            {
                mcast_log_portLimit_arv(ipType, port,limitNum);
                if(SYS_ERR_FAILED == mcast_maxGroupAct_process(groupHead->ipType, groupHead->vid, groupHead->dip,0, port))
                {
                     if (isSetSupFlag)
                        groupHead->suppreFlag = 0;

                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_MCAST_GROUP_MAX;
                }
            }
            groupHead->gsQueryCnt[port] = 0;
            groupHead->mbrTimer[port] = portTime;
        }
        else
        {
            groupHead->gsQueryCnt[port] = 0;
            groupHead->mbrTimer[port] = portTime;
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        if ((IGMP_DMAC_VID_MODE == lookup_mode) || (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != ipType ))
        {
            if (SYS_ERR_OK != (ret = mcast_group_aggregatePort_add(groupHead, port, lookup_mode)))
            {
                if (ipType == MULTICAST_TYPE_IPV4)
                    MCASTDBG("Add exsit group port member failure, ret = %d !", ret);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                    MCASTDBG("Add exsit group port member failure, ret = %d !", ret);
#endif
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }
        }
        else
        {
            if (groupHead->sip == 0 && IGMP_DIP_SIP_MODE == lookup_mode)     
            {
                LOGIC_PORTMASK_SET_PORT(groupHead->mbr, port);
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_OK;      
            }
            else                        
            {
                pEntry = groupHead;
                while(pEntry)
                {
                    
                    pEntry->suppreFlag = groupHead->suppreFlag;
                    pEntry->gsQueryCnt[port] = 0;
                    pEntry->mbrTimer[port] = portTime;

                    if (!IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, port))
                    {
                        LOGIC_PORTMASK_SET_PORT(pEntry->mbr, port);
                        if (SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pEntry, &ip46McstEntry)))
                        {
                            mcast_routerPort_get(vid, ipType, &rtPmsk);
                            #ifdef CONFIG_SYS_PROTO_MVR
                            LOGIC_PORTMASK_CLEAR_ALL(rtPmsk);
                            if (MVR_MODE_COMPATABLE == mcast_mvr_mode_ret(pEntry->ipType))
                            {
                                mcast_mvr_pmsk_get(pEntry->ipType, MVR_TYPE_SOURCE, &rtPmsk);
                            }
                            #endif
                            LOGIC_PORTMASK_OR(ip46McstEntry.portmask, pEntry->mbr, rtPmsk);
                            mcast_hw_ipMcastEntry_set(ipType, &ip46McstEntry,TRUE, isFwdAll);
                        }
                    }

                    pEntry = pEntry->next_subgroup;

                }
            }
        }
    }
    else
    {
        if (igmp_packet_stop)
        {
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        if(portEntryNum >= limitNum)
        {
            mcast_log_portLimit_arv(ipType, port, limitNum);
            if(SYS_ERR_FAILED == mcast_maxGroupAct_process(ipType, vid, dip,0, port))
            {
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_MCAST_GROUP_MAX;
            }
            else 
            {
                mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupKeyEntry), &sortedIdx, &groupHead);
            }
        }

        pEntry = mcast_group_freeEntry_get(ipType, isMvrPackt);
        if (NULL == pEntry)
        {
            mcast_log_tablefull(ipType);
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_MCAST_DATABASE_FULL;
        }

        pEntry->ipType = ipType;
        pEntry->form = IGMP_GROUP_ENTRY_DYNAMIC;
        pEntry->dip = dip;
        pEntry->sip = 0;
        pEntry->vid = vid;
        pEntry->sortKey = mcast_group_sortKey_ret(&groupKeyEntry);
        pEntry->mbrTimer[port] = portTime;

        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            pEntry->mac[0] = 0x01;
            pEntry->mac[1] = 0x00;
            pEntry->mac[2] = 0x5e;
            pEntry->mac[3] = (uint8)((dip >> 16) & 0x7f);
            pEntry->mac[4] = (uint8)((dip >> 8) & 0xff);
            pEntry->mac[5] = (uint8)(dip & 0xff);
        }
        else
        {
            osal_memcpy(pEntry->dipv6.addr, pAddr->ipv6.addr, IPV6_ADDR_LEN);
            pEntry->mac[0] = 0x33;
            pEntry->mac[1] = 0x33;
            pEntry->mac[2] = (uint8)((dip >> 24) & 0xff);
            pEntry->mac[3] = (uint8)((dip >> 16) & 0xff);
            pEntry->mac[4] = (uint8)((dip >> 8) & 0xff);
            pEntry->mac[5] = (uint8)(dip & 0xff);
        }

        LOGIC_PORTMASK_CLEAR_ALL(pEntry->mbr);
        LOGIC_PORTMASK_SET_PORT(pEntry->mbr, port);
         
        pEntry->suppreFlag = 1;

        if (SYS_ERR_OK !=  (ret = mcast_newGroup_port_add(pEntry, port, isMvrPackt, lookup_mode,  sortedIdx)))
        {
              if (ipType == MULTICAST_TYPE_IPV4)
                    MCASTDBG("Add new group port member failure, ret = %d !", ret);
            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                    MCASTDBG("Add new group port member failure, ret = %d !", ret);
            #endif
                mcast_group_entry_release(pEntry);
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
        }
        else
        {
            mcast_group_num_cal(pEntry->ipType,FALSE, isMvrPackt,TRUE);
            mcast_group_sortedArray_ins(sortedIdx, pEntry);
        }

      #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
       if (IGMP_VERSION_V3_FULL == igmp_cfg.oper_version)
           mcast_igmp_group_head_ins(dip, vid, port, MODE_IS_INCLUDE,  &pGroup_head_entry);
       #endif
    }
    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}

int32 mcast_groupMbrPort_del_wrapper_dipsip(multicast_ipType_t ipType, uint16 vlanid, sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pAddr,  sys_logic_port_t port, uint8 isMvrPacket)
{
    uint16                  sortedIdx;
    uint32                  dip;
    igmp_group_entry_t      *groupHead;
    igmp_group_entry_t      *pEntry;
    igmp_group_entry_t      groupEntry;
    sys_logic_portmask_t    delPmsk;
    igmp_querier_entry_t    *pQuerEntry;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    sys_vid_t vid = vlanid;
    sys_enable_t  fastLeave = DISABLED;
    igmp_lookup_mode_t lookup_mode = IGMP_DIP_SIP_MODE;
    int32 ret = 0;
    #ifdef CONFIG_SYS_PROTO_MVR
    sys_vid_t mvrVlan;
    #endif

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if(NULL == pMcastVlan)
        return SYS_ERR_FAILED;

    #ifdef CONFIG_SYS_PROTO_MVR
    if (isMvrPacket)
    {
        mcast_mvr_vlanId_get(ipType, &mvrVlan);
        vid = mvrVlan;
        mcast_mvr_portFastleave_get(ipType, port, &fastLeave);
    }
    else
    #endif
    {
        fastLeave = pMcastVlan->fastleave_enable;
    }

    mcast_dbg_jn_lv_report(ipType,vid, port, pAddr,FALSE);
    mcast_querier_db_get(vid, ipType, &pQuerEntry);
    if (pQuerEntry == NULL)
    {
        MCASTDBG("[IGMP]%s():%d VLAN-%d doesn't have query DB entry!", __FUNCTION__, __LINE__, vid);
        return SYS_ERR_FAILED;
    }

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        dip = pAddr->ipv4;
    }
    else
    {
        dip = IPV6_TO_IPV4(pAddr->ipv6.addr);
    }

    rsd_mcast_lookupMode_get(ipType, &lookup_mode);
    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, dip,pAddr->ipv6.addr, &groupEntry));

    IGMP_DB_SEM_LOCK();

    if (fastLeave)
    {
        mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);   
        if (groupHead)
        {
            if (!isMvrPacket)
            {
                if (IGMP_QUERIER == pQuerEntry->status)
                {
                    mcast_send_gs_query(groupHead, port);
                    mcast_suppreFlag_clear(groupHead->vid, groupHead->ipType);
                }
            }

            if (IGMP_GROUP_ENTRY_STATIC == groupHead->form)
            {
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_OK;
            }
            if (groupHead->sip == 0)     
            {
                groupHead->mbrTimer[port] = 0;
                LOGIC_PORTMASK_CLEAR_PORT(groupHead->mbr, port);
                osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
                LOGIC_PORTMASK_SET_PORT(delPmsk, port);
                mcast_hw_mcst_mbr_remove(groupHead, &delPmsk);

                if (IS_LOGIC_PORTMASK_CLEAR(groupHead->mbr))     
                {
                    if (IGMP_DIP_VID_MODE == lookup_mode)
                    {
                         ret = mcast_hw_mcst_entry_del(groupHead);
                         if (ret == SYS_ERR_OK)
                         {
                            mcast_hw_group_num_cal(FALSE);
                         }
                    }
                    mcast_group_del(groupHead);
                }
            }
            else                        
            {
                pEntry = groupHead;
                while(pEntry)
                {
                    pEntry->mbrTimer[port] = 0;
                    LOGIC_PORTMASK_CLEAR_PORT(pEntry->mbr, port);
                    osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
                    LOGIC_PORTMASK_SET_PORT(delPmsk, port);
                    mcast_hw_mcst_mbr_remove(pEntry, &delPmsk);

                    if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
                    {
                        pEntry->groupTimer = IGMP_TEMP_PERIOD;
                    }

                    pEntry = pEntry->next_subgroup;
                }
            }
        }
    }
    else
    {
        mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);   
        if (groupHead)
        {
            if (pQuerEntry->status == IGMP_QUERIER)
            {
                if (!isMvrPacket)
                    mcast_suppreFlag_clear(groupHead->vid, groupHead->ipType);
            }

            #ifdef CONFIG_SYS_PROTO_MVR
            if (IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, port))
            {
                if (isMvrPacket)
                {
                    if (MULTICAST_TYPE_IPV4 == groupHead->ipType)
                    {
                        if (MVR_MODE_COMPATABLE == igmp_cfg.mvrConfig.mode)
                        {
                            groupHead->mbrTimer[port] = PASS_SECONDS;
                            groupHead->gsQueryCnt[port]++;
                        }
                     }
                }
            }
            #endif

            if (groupHead->sip == 0)     
            {
                if (IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, port) && groupHead->gsQueryCnt[port] == 0)
                {
                    if (pQuerEntry->status == IGMP_QUERIER)
                    {
                        if (IGMP_GROUP_ENTRY_STATIC != groupHead->form)
                        {
                            groupHead->mbrTimer[port] = PASS_SECONDS;
                            groupHead->gsQueryCnt[port]++;
                        }
                    }
                }
            }
            else
            {
                pEntry = groupHead;
                while(pEntry)
                {
                    if (IGMP_GROUP_ENTRY_STATIC == pEntry->form)
                    {
                        pEntry = pEntry->next_subgroup;
                        continue;
                    }

                    if (IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, port) && pEntry->gsQueryCnt[port] == 0)
                    {
                        if (pQuerEntry->status == IGMP_QUERIER)
                        {
                            pEntry->mbrTimer[port] = PASS_SECONDS;
                            pEntry->gsQueryCnt[port]++;
                        }
                    }
                    pEntry = pEntry->next_subgroup;
                }
            }
        }
    }
    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}

int32 mcast_groupMbrPort_del_wrapper_dmacvid(multicast_ipType_t ipType, uint16 vlanid, sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pAddr,  sys_logic_port_t port, uint8 isMvrPackt)
{
    int32                   ret;
    uint16                  sortedIdx;
    uint32                  dip;
    igmp_group_entry_t      *groupHead;
    igmp_aggregate_entry_t  *pAggrEntry;
    sys_logic_portmask_t    delPmsk;
    igmp_querier_entry_t    *pQuerEntry;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    igmp_group_entry_t  groupEntry;
    sys_l2McastEntry_t      l2McstEntry;
    sys_vid_t vid = vlanid;
    sys_enable_t  fastLeave = DISABLED;

    #ifdef CONFIG_SYS_PROTO_MVR
    sys_vid_t mvrVlan;
    if (isMvrPackt)
    {
        mcast_mvr_vlanId_get(ipType, &mvrVlan);
        vid = mvrVlan;
        mcast_mvr_portFastleave_get(ipType, port, &fastLeave);
    }
    else
    #endif
    {
        mcast_vlan_db_get(vid, ipType, &pMcastVlan);
        if(NULL == pMcastVlan)
            return SYS_ERR_FAILED;

        fastLeave = pMcastVlan->fastleave_enable;
    }

    mcast_dbg_jn_lv_report(ipType,vid, port, pAddr,FALSE);

    mcast_querier_db_get(vid, ipType, &pQuerEntry);
    if (pQuerEntry == NULL)
    {
        MCASTDBG("[IGMP]%s():%d VLAN-%d doesn't have query DB entry!", __FUNCTION__, __LINE__, vid);
        return SYS_ERR_FAILED;
    }

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        dip = pAddr->ipv4;
    }
    else
    {
        dip = IPV6_TO_IPV4(pAddr->ipv6.addr);
    }

    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, dip, pAddr->ipv6.addr, &groupEntry));

    IGMP_DB_SEM_LOCK();

    if (fastLeave)
    {
        mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);   
        if (groupHead)
        {
            LOGIC_PORTMASK_CLEAR_ALL(delPmsk);
            LOGIC_PORTMASK_SET_PORT(delPmsk, port);

            mcast_aggregate_db_get(groupHead, &pAggrEntry,TRUE);
            if (pAggrEntry == NULL)
            {
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }

            if (IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, port))
            {
                if (!isMvrPackt)
                 {
                    if (IGMP_QUERIER == pQuerEntry->status)
                    {
                        mcast_send_gs_query(groupHead, port);
                        mcast_suppreFlag_clear(groupHead->vid, groupHead->ipType);
                    }
                }

                if (IGMP_GROUP_ENTRY_STATIC == groupHead->form)
                {
                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_OK;
                }

                groupHead->mbrTimer[port] = 0;
                LOGIC_PORTMASK_CLEAR_PORT(groupHead->mbr, port);
                if (IS_LOGIC_PORTMASK_CLEAR(groupHead->mbr))
                {
                    if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT  != mcast_unknownAct_ret(groupHead->ipType))
                    {
                        pAggrEntry->group_ref_cnt--;
                                        
                        if (pAggrEntry->group_ref_cnt == 0)
                        {
                            osal_memset(&l2McstEntry, 0, sizeof(sys_l2McastEntry_t));
                            l2McstEntry.vid = vid;
                            osal_memcpy(l2McstEntry.mac, groupHead->mac, CAPA_MAC_BYTE_NUM);

                            if (SYS_ERR_OK == (ret = mcast_hw_l2McastEntry_del(&l2McstEntry)))
                            {
                                mcast_hw_group_num_cal(FALSE);
                                mcast_aggregate_db_del(groupHead);
                                mcast_group_del(groupHead);
                            }
                        }
                        else
                        {
                            mcast_hw_mcst_mbr_remove(groupHead, &delPmsk);
                            mcast_group_del(groupHead);
                        }
                    }
                    else
                    {
                        groupHead->groupTimer = IGMP_TEMP_PERIOD;
                        mcast_hw_mcst_mbr_remove(groupHead, &delPmsk);
                    }
                }
                else
                {
                    mcast_hw_mcst_mbr_remove(groupHead, &delPmsk);
                }
            }
         }
    }
    else
    {
        mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);   
        if (groupHead)
        {
            mcast_aggregate_db_get(groupHead, &pAggrEntry,TRUE);
            if (pAggrEntry == NULL)
            {
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }
            if (IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, port))
            {
                if (!isMvrPackt)
                {
                    if (groupHead->gsQueryCnt[port] == 0)
                    {
                        if (pQuerEntry->status == IGMP_QUERIER)
                        {
                            if (IGMP_GROUP_ENTRY_STATIC != groupHead->form)
                            {
                                    groupHead->mbrTimer[port] = PASS_SECONDS;
                                    groupHead->gsQueryCnt[port]++;
                            }
                            mcast_suppreFlag_clear(groupHead->vid, groupHead->ipType);
                        }
                    }
                }
                #ifdef CONFIG_SYS_PROTO_MVR
                else
                {
                    if (MULTICAST_TYPE_IPV4 == groupHead->ipType)
                    {
                        if (MVR_MODE_COMPATABLE == igmp_cfg.mvrConfig.mode)
                        {
                            groupHead->mbrTimer[port] = PASS_SECONDS;
                            groupHead->gsQueryCnt[port]++;
                        }
                        else
                        {
                            if (pQuerEntry->status == IGMP_QUERIER)
                            {
                                if (IGMP_GROUP_ENTRY_STATIC != groupHead->form)
                                {
                                    
                                        groupHead->mbrTimer[port] = PASS_SECONDS;
                                        groupHead->gsQueryCnt[port]++;
                                }
                                
                            }
                        }
                    }
                }
                #endif 
            }
        }
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}

int32 mcast_port_entry_get(multicast_ipType_t ipType, sys_logic_port_t port, uint32 *pEntryNum)
{
    int32 i;
    igmp_group_entry_t  *pGroup_entry;
    uint32 entryCnt = 0;
    uint32 lastGroup = 0, lastVid = 0;
    uint16 lastIpv6Pre16 = 0;

    SYS_PARAM_CHK(NULL == pEntryNum, SYS_ERR_NULL_POINTER);

    for(i = 0; i < mcast_group_num; i++)
    {
        pGroup_entry = mcast_group_get_with_index(i);

        if (pGroup_entry)
        {
            if (ipType != pGroup_entry->ipType)
                continue;

            if (igmp_cfg.oper_version < IGMP_VERSION_V3_FULL)
            {
                if (MULTICAST_TYPE_IPV4 == ipType)
                {
                    if (pGroup_entry->dip == lastGroup && lastVid == pGroup_entry->vid)
                        continue;
                }
                else
                {
                    if (pGroup_entry->dip == lastGroup && lastVid == pGroup_entry->vid &&
                         (lastIpv6Pre16 == *(uint16 *)(&pGroup_entry->dipv6.addr[0])))
                         continue;
                }
            }

            lastGroup = pGroup_entry->dip;
            lastVid = pGroup_entry->vid;
            if (MULTICAST_TYPE_IPV4 != ipType)
            {
                lastIpv6Pre16 = *(uint16 *)(&pGroup_entry->dipv6.addr[0]);
            }

            if (IGMP_GROUP_ENTRY_STATIC == pGroup_entry->form)
                continue;

            #ifdef CONFIG_SYS_PROTO_MVR
            if (!is_mcast_mvr_maxGroup(ipType))
            {
                if (is_mvr_group_ret2(pGroup_entry))
                    continue;
            }
            #endif

            if(IS_LOGIC_PORTMASK_PORTSET(pGroup_entry->mbr, port) &&
                pGroup_entry->v3.mbrFwdFlag[port] != IGMPV3_FWD_NOT)
            {
                entryCnt++;
            }
        }
    }

    *pEntryNum = entryCnt;

    return SYS_ERR_OK;

}

int32 mcast_suppreFlag_clear(sys_vid_t vid, multicast_ipType_t ipType)
{
    int32 i;
    igmp_group_entry_t  *pGroup_entry;

    if (DISABLED == igmp_cfg.suppre_enable)
        return SYS_ERR_OK;

    for(i = 0; i < mcast_group_num; i++)
    {
        pGroup_entry = mcast_group_get_with_index(i);

        if (pGroup_entry)
        {
            if (ipType == pGroup_entry->ipType && vid == pGroup_entry->vid)
                pGroup_entry->suppreFlag = 0;
        }
    }
    return SYS_ERR_OK;
}

int32 mcast_group_portTime_update(multicast_ipType_t ipType , sys_vid_t vid, mcast_groupAddr_t *pAddr)
{
    sys_logic_port_t port;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    igmp_group_entry_t  *pGroupHead = NULL, *pEntry = NULL,  groupEntry;
    uint16 sortedIdx;
    uint32 portTime = 0;
    igmp_lookup_mode_t lookup_mode;

    SYS_PARAM_CHK((NULL == pAddr), SYS_ERR_NULL_POINTER);

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
        return SYS_ERR_OK;

    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, pAddr->ipv4, pAddr->ipv6.addr, &groupEntry));
    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &pGroupHead);

    if (NULL == pGroupHead)
        return SYS_ERR_OK;

    {
        portTime = pMcastVlan->operLastMmbrQueryCnt * pMcastVlan->operGsqueryIntv ;
    }

    rsd_mcast_lookupMode_get(ipType, &lookup_mode);
    if (IGMP_DIP_SIP_MODE == lookup_mode )
    {
        pEntry = pGroupHead;
        while(pEntry)
        {
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pEntry->mbr)
            {
                pEntry->mbrTimer[port] =  portTime;
            }
            pEntry = pEntry->next_subgroup;
        }
    }
    else
    {
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pGroupHead->mbr)
        {
            pGroupHead->mbrTimer[port] =  portTime;
        }
    }

    return SYS_ERR_OK;
}

int32 mcast_groupPort_remove(igmp_group_entry_t *pGroup, sys_logic_port_t port)
{
    int32 ret;
    sys_logic_portmask_t delPmsk, rtPmsk;
    igmp_group_entry_t tmpGroup;

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    LOGIC_PORTMASK_CLEAR_ALL(delPmsk);
    LOGIC_PORTMASK_SET_PORT(delPmsk, port);
    osal_memset(&tmpGroup, 0, sizeof(tmpGroup));
    osal_memcpy(&tmpGroup, pGroup, sizeof(igmp_group_entry_t));

    LOGIC_PORTMASK_CLEAR_PORT(pGroup->mbr, port);
    pGroup->mbrTimer[port] = 0;

    if(IS_LOGIC_PORTMASK_CLEAR(pGroup->mbr))
    {
        ret = mcast_hw_mcst_entry_del(&tmpGroup);

        if (ret == SYS_ERR_OK)
        {
            
            mcast_hw_group_num_cal(FALSE);
        }

        mcast_group_del(pGroup);
    }
    else
    {
        mcast_routerPort_get(pGroup->vid, pGroup->ipType, &rtPmsk);
        if (IS_LOGIC_PORTMASK_PORTSET(rtPmsk, port))
        {
            LOGIC_PORTMASK_CLEAR_PORT(delPmsk, port);
        }

        if (SYS_ERR_OK != (ret = mcast_hw_mcst_mbr_remove(pGroup, &delPmsk)))
        {
            MCASTDBG("Deleting Mcst mbr failed!  ret:%d", ret);
        }
    }

    return SYS_ERR_OK;
}
int32 mcast_routerPortMbr_remove(multicast_ipType_t ipType, sys_vlanmask_t *pVlanMsk, sys_logic_portmask_t *pdelMbr)
{
    int32 i, ret;
    igmp_group_entry_t *pGroup = NULL;
    sys_l2McastEntry_t  l2Mcst_entry;
    sys_ipv46McastEntry_t  ip46Mcst_entry;
    sys_logic_portmask_t  removePmsk;
    igmp_aggregate_entry_t *pAggrEntry = NULL;
    sys_logic_port_t port;
    igmp_lookup_mode_t lookup_mode;
    uint8 isFwdAll = TRUE;

    if (IS_LOGIC_PORTMASK_CLEAR((*pdelMbr)))
        return SYS_ERR_OK;

    for (i = 0; i < mcast_group_num; i++)
    {
        pGroup = mcast_group_get_with_index(i);
        if ( NULL == pGroup)
            continue;

        if (!VLANMASK_IS_VIDSET(*pVlanMsk, pGroup->vid))
            continue;

        if (MULTICAST_TYPE_END != ipType)
        {
            if ( ipType != pGroup->ipType)
                continue;
        }

        if(!is_mcast_group(pGroup))
            continue;

        #ifdef CONFIG_SYS_PROTO_MVR
        if (is_mvr_group_ret2(pGroup))
            continue;
        #endif
        rsd_mcast_lookupMode_get(ipType, &lookup_mode);
        if ((IGMP_DMAC_VID_MODE == lookup_mode) || (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != ipType))
        {
            mcast_aggregate_db_get(pGroup, &pAggrEntry,TRUE);
            if (pAggrEntry == NULL)
            {
                return SYS_ERR_FAILED;
            }

            LOGIC_PORTMASK_CLEAR_ALL(removePmsk);
            LOGIC_PORTMASK_COPY(removePmsk, *pdelMbr);
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, removePmsk)
            {
                if ( 0 != pAggrEntry->port_ref_cnt[port])
                {
                    LOGIC_PORTMASK_CLEAR_PORT(removePmsk, port);
                }
            }
            if (IS_LOGIC_PORTMASK_CLEAR(removePmsk))
                continue;

            if (IGMP_DMAC_VID_MODE == lookup_mode)
            {
                l2Mcst_entry.vid = pGroup->vid;
                osal_memcpy(l2Mcst_entry.mac, pGroup->mac, CAPA_MAC_BYTE_NUM);
                ret = mcast_hw_l2McastEntry_get(&l2Mcst_entry);
                LOGIC_PORTMASK_ANDNOT(l2Mcst_entry.portmask, l2Mcst_entry.portmask, removePmsk);
                LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, pGroup->mbr);   
            }
            else
            {
                ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46Mcst_entry);
                LOGIC_PORTMASK_ANDNOT(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, removePmsk);
                LOGIC_PORTMASK_OR(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, pGroup->mbr);   
            }

            if (SYS_ERR_OK == ret)
            {
                if (IGMP_DMAC_VID_MODE == lookup_mode)
                    SYS_ERR_CHK(mcast_hw_l2McastEntry_set(pGroup->ipType, &l2Mcst_entry, FALSE, isFwdAll));
                else
                    SYS_ERR_CHK(mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46Mcst_entry, FALSE, isFwdAll));
            }
        }
        else
        {
            if ((pGroup->sip != 0) || (IGMP_DIP_VID_MODE == lookup_mode) )
            {
                if (SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46Mcst_entry)))
                {
                    if (IS_LOGIC_PORTMASK_CLEAR(pGroup->mbr))
                    {
                        if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT != mcast_unknownAct_ret(pGroup->ipType))
                        continue;
                    }

                    LOGIC_PORTMASK_ANDNOT(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, *pdelMbr);
                    LOGIC_PORTMASK_OR(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, pGroup->mbr);  
                    SYS_ERR_CHK(mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46Mcst_entry, FALSE, isFwdAll));
                }
            }
        }

    }

    return SYS_ERR_OK;
}

int32 mcast_group_learnByData_del(multicast_ipType_t ipType)
{
    uint32 i = 0;
    uint32 ret = SYS_ERR_OK;
    igmp_group_entry_t *pEntry = NULL;
    uint32 tmpNum, dbSize;

    IGMP_DB_SEM_LOCK();
    dbSize = mcast_group_num;

    if (0 == dbSize)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
    }

    mcast_group_rx_timer_stop();
    for (i = dbSize ; i > 0 ; i--)
    {
        pEntry = mcast_group_get_with_index(i-1);

        if ( NULL == pEntry)
            continue;

        if (MULTICAST_TYPE_END != ipType)
        {
            if (ipType != pEntry->ipType)
                continue;
        }

        if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
        {
            ret = mcast_hw_mcst_entry_del(pEntry);
            if (SYS_ERR_OK == ret)
            {
                
                mcast_hw_group_num_cal(FALSE);
            }
            tmpNum = mcast_group_num;
            mcast_group_del(pEntry);
            if (tmpNum != mcast_group_num)
                i++;

        }
    }

    mcast_group_rx_timer_start();
    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
  }

int32 mcast_routerPortMbr_add(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_portmask_t *pAddMbr)
{
    int32 i, ret;
    igmp_group_entry_t *pGroup = NULL;
    sys_l2McastEntry_t  l2Mcst_entry;
    sys_ipv46McastEntry_t  ip46Mcst_entry;
    igmp_lookup_mode_t lookup_mode;
    uint8 isFwdAll = TRUE;

    if (IS_LOGIC_PORTMASK_CLEAR((*pAddMbr)))
        return SYS_ERR_OK;

    for (i = 0; i < mcast_group_num; i++)
    {
        pGroup = mcast_group_get_with_index(i);
        if ( NULL == pGroup)
            continue;

        if (vid != pGroup->vid)
            continue;

        if (MULTICAST_TYPE_END != ipType)
        {
            if ( ipType != pGroup->ipType)
                continue;
        }

        if(!is_mcast_group(pGroup))
            continue;

        #ifdef CONFIG_SYS_PROTO_MVR
        if (is_mvr_group_ret2(pGroup))
            continue;
        #endif
        rsd_mcast_lookupMode_get(ipType, &lookup_mode);

        if(IGMP_DMAC_VID_MODE == lookup_mode)
        {
            l2Mcst_entry.vid = pGroup->vid;
            osal_memcpy(l2Mcst_entry.mac, pGroup->mac, CAPA_MAC_BYTE_NUM);
            if (SYS_ERR_OK == mcast_hw_l2McastEntry_get(&l2Mcst_entry))
            {
                LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, *pAddMbr);
                LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, pGroup->mbr);
                SYS_ERR_CHK(mcast_hw_l2McastEntry_set(pGroup->ipType, &l2Mcst_entry, TRUE, isFwdAll));
            }
        }
        else
        {
            if ((pGroup->sip != 0)  || (IGMP_DIP_VID_MODE == lookup_mode ))
            {
                if (SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46Mcst_entry)))
                {
                    if (IS_LOGIC_PORTMASK_CLEAR(pGroup->mbr))
                    {
                        if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT != mcast_unknownAct_ret(pGroup->ipType))
                        continue;
                    }

                    LOGIC_PORTMASK_OR(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, *pAddMbr);
                    LOGIC_PORTMASK_OR(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, pGroup->mbr);  
                    SYS_ERR_CHK(mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46Mcst_entry, TRUE, isFwdAll));
                }
            }
        }
    }

    return SYS_ERR_OK;
}

int32 mcast_maxGroupAct_process(multicast_ipType_t ipType, sys_vid_t vid, uint32 dip, uint32 sip,sys_logic_port_t port)
{
#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    int32 i;
    igmp_group_entry_t  *pEntry;
    igmp_maxGroupAct_t act;
    uint16  limitNum;
    
    mcast_portGroup_excessAct_get(ipType, port, &act);

    if (IGMP_ACT_DENY == act)
    {
        if (MULTICAST_TYPE_IPV4 == ipType)
            MCASTDBG("The Port %s group is arrived max-group number", LPSTR(port));
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
            MCASTDBG("The Port %s group is arrived max-group number", LPSTR(port));
#endif

        return SYS_ERR_FAILED;
    }

    mcast_portGroup_limit_get(ipType,port, &limitNum);

    if (0 == limitNum)
        return SYS_ERR_FAILED;

    for(i = 0; i < mcast_group_num; i++)
    {
        pEntry = mcast_group_get_with_index(i);
        if (pEntry)
        {
            if (IGMP_GROUP_ENTRY_STATIC == pEntry->form)
                continue;

            if (ipType != pEntry->ipType)
                continue;

            if (pEntry->vid == vid && pEntry->dip == dip)
                continue;

            #ifdef CONFIG_SYS_PROTO_MVR
            if (!is_mcast_mvr_maxGroup(pEntry->ipType))
            {
                if (is_mvr_group_ret2(pEntry))
                    continue;
            }
           #endif

            if (igmp_cfg.oper_version < IGMP_VERSION_V3_FULL)
            {
                if (IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, port))
                {
                    mcast_groupPort_remove(pEntry, port);
                    break;
                }
            }
        }
    }
#endif

    return SYS_ERR_OK;

}
int32 mcast_igmp_subgroup_sip_search(igmp_group_entry_t *gipHead, uint32 key, igmp_group_entry_t **subgroup, igmp_group_entry_t **previous)
{
    igmp_group_entry_t  *entry;
    if (subgroup == NULL)
        return SYS_ERR_FAILED;

    entry = gipHead;
    while (entry)
    {
        if (entry->sip == key)
        {
            *subgroup = entry;
            return SYS_ERR_OK;
        }

        if (previous != NULL)
            *previous = entry;
        entry = entry->next_subgroup;
    }
    *subgroup = NULL;

    return SYS_ERR_OK;
}

int32 mcast_igmp_group_mcstData_add_wrapper_dipsip(multicast_ipType_t ipType, uint16 vid, sys_nic_pkt_t *pBuf,
       mcast_groupAddr_t *pGroupDip, mcast_groupAddr_t * pSip, sys_logic_port_t port, uint8 isMvrPackt)
{
    int32                   ret;
    uint16                  sortedIdx;
    igmp_group_entry_t      *pEntry, *groupHead, *pNewEntry, groupEntry;
    igmp_router_entry_t     *pRouterEntry;
    sys_ipv46McastEntry_t      ip46McstEntry;
    uint32 portEntryNum;
    uint16 limitNum;
    uint32 dip, sip;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    igmp_lookup_mode_t lookup_mode;
    sys_logic_port_t lport;
    uint8 isFwdAll = TRUE;

    dip = pGroupDip->ipv4;
    sip = pSip->ipv4;

    rsd_mcast_lookupMode_get(ipType, &lookup_mode);
    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, dip, pGroupDip->ipv6.addr, &groupEntry));

    #ifdef CONFIG_SYS_PROTO_MVR
    if (isMvrPackt)
    {
        if (is_mcast_mvr_forwardAll(ipType))
            isFwdAll = FALSE;
    }
    #endif

    IGMP_DB_SEM_LOCK();

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        mcast_ipMcast_asicKey_get(vid, dip, sip, &ip46McstEntry);
    }
    else
    {
        mcast_ipv6Mcast_asicKey_get(vid, pGroupDip->ipv6.addr, pSip->ipv6.addr, &ip46McstEntry);
    }

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);   
    if (groupHead)
    {
        if (groupHead->sip == 0 && IGMP_DIP_SIP_MODE == lookup_mode)     
        {
            groupHead->sip = sip;

            if (MULTICAST_TYPE_IPV4 != ipType)
            {
                osal_memcpy(groupHead->sipv6.addr, pSip->ipv6.addr, IPV6_ADDR_LEN);
            }

            mcast_router_db_get(vid, ipType, &pRouterEntry);
            if (pRouterEntry)
            {
                LOGIC_PORTMASK_OR(ip46McstEntry.portmask, pRouterEntry->router_pmsk, groupHead->mbr);
            }
            else
            {
                if (MULTICAST_TYPE_IPV4 == ipType)
                    MCASTDBG("Warring! This vlan has no router port!");
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                    MCASTDBG("Warring! This vlan has no router port!");
#endif
                LOGIC_PORTMASK_COPY(ip46McstEntry.portmask, groupHead->mbr);
            }

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                if (MULTICAST_TYPE_IPV4 == ipType)
                {
                    ip46McstEntry.ipMcast.groupId = groupHead->groupId;
                }
                else
                {
                    ip46McstEntry.ipv6Mcast.groupId = groupHead->groupId;
                }
            }
#endif

            if (isMvrPackt)
                ip46McstEntry.ipMcast.crossVlan = ENABLED;

            if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_add(ipType, &ip46McstEntry, isFwdAll)))
            {
                mcast_log_asicIP_fail(ipType, &ip46McstEntry, MCAST_ADD);
                
                if (SYS_ERR_L2_MAC_FULL == ret)
                {
                    SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType)));
                    igmp_trapDataPkt_lock = TRUE;
                }
            }
            else
            {
                groupHead->lookupTableFlag = L2TABLE_WROTE;
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                if (IS_CHIP_CSW())
                {
                    if (MULTICAST_TYPE_IPV4 == ipType)
                    {
                        groupHead->groupId = ip46McstEntry.ipMcast.groupId;
                    }
                    else
                    {
                        groupHead->groupId = ip46McstEntry.ipv6Mcast.groupId;
                    }
                }
#endif
                mcast_hw_group_num_cal(TRUE);
            }
        }
        else if (IGMP_DIP_VID_MODE == lookup_mode)
        {
#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
            if (IS_CHIP_MSW())
            {
                
                mcast_build_groupEntryKey(vid, ipType, dip,  pGroupDip->ipv6.addr, &groupEntry);
                groupEntry.sip = sip;
                pEntry = mcast_group_get(&groupEntry);

                if (pEntry == NULL)
                {
                    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lport, groupHead->mbr)
                    {
                        mcast_port_entry_get(ipType, lport, &portEntryNum);
                        mcast_portGroup_limit_get(ipType, lport, &limitNum);

                        if(portEntryNum >= limitNum)
                        {
                            mcast_log_portLimit_arv(ipType,port,limitNum);
                            if(SYS_ERR_FAILED == mcast_maxGroupAct_process(ipType, vid, dip,0, lport))
                            {
                                continue;
                            }
                        }
                    }

                    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);

                    pNewEntry = mcast_group_freeEntry_get(ipType, isMvrPackt);
                    if (NULL == pNewEntry)
                    {
                        mcast_log_tablefull(ipType);
                        IGMP_DB_SEM_UNLOCK();
                        return SYS_ERR_MCAST_DATABASE_FULL;
                    }

                    pNewEntry->ipType = ipType;
                    pNewEntry->form = IGMP_GROUP_ENTRY_DYNAMIC;
                    pNewEntry->dip = dip;
                    pNewEntry->sip = sip;
                    pNewEntry->vid = vid;
                    pNewEntry->sortKey = mcast_group_sortKey_ret(&groupEntry);
                    pNewEntry->groupTimer = pMcastVlan->grpMembershipIntv;

                    if (MULTICAST_TYPE_IPV4 !=  ipType)
                    {
                        osal_memcpy(pNewEntry->dipv6.addr, groupHead->dipv6.addr, IPV6_ADDR_LEN);
                        osal_memcpy(pNewEntry->sipv6.addr, pSip->ipv6.addr, IPV6_ADDR_LEN);
                    }

                    if( IGMP_GROUP_ENTRY_STATIC == groupHead->form)
                    {
                        pNewEntry->form = IGMP_GROUP_ENTRY_STATIC;
                        osal_memcpy(&pNewEntry->staticInMbr, &groupHead->staticInMbr, sizeof(sys_logic_portmask_t));
                    }
                    else
                        pNewEntry->form = IGMP_GROUP_ENTRY_DYNAMIC;

                    if (IS_LOGIC_PORTMASK_CLEAR(groupHead->mbr))
                        pNewEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                    osal_memcpy(&pNewEntry->mbr, &groupHead->mbr, sizeof(sys_logic_portmask_t));

                    if (SYS_ERR_OK == mcast_group_sortedArray_ins(pNewEntry->sortKey, pNewEntry))
                    {
                        if (isMvrPackt)
                            ip46McstEntry.ipMcast.crossVlan = ENABLED;
                        osal_memcpy(&ip46McstEntry.portmask, &groupHead->mbr, sizeof(sys_logic_portmask_t));

                        if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_add(ipType, &ip46McstEntry, isFwdAll)))
                        {
                            mcast_log_asicIP_fail(ipType, &ip46McstEntry,MCAST_ADD);
                            if (SYS_ERR_L2_MAC_FULL == ret)
                            {
                                mcast_group_entry_release(pNewEntry);
                                rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType));
                                igmp_trapDataPkt_lock = TRUE;
                                IGMP_DB_SEM_UNLOCK();
                                return SYS_ERR_OK;
                            }
                        }
                        else
                        {
                            pEntry->lookupTableFlag = L2TABLE_WROTE;
                        }
                    }
                    else
                    {
                        mcast_group_entry_release(pNewEntry);
                    }
                }
                else
                {
                    
                    if ( SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pEntry, &ip46McstEntry)))
                    {
                        mcast_hw_ipMcastEntry_set(pEntry->ipType, &ip46McstEntry, FALSE, isFwdAll);
                    }
                    pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                }

            }
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                
                mcast_router_db_get(vid, ipType, &pRouterEntry);
                if (pRouterEntry)
                {
                    LOGIC_PORTMASK_OR(ip46McstEntry.portmask, pRouterEntry->router_pmsk, groupHead->mbr);
                }
                else
                {
                    if (MULTICAST_TYPE_IPV4 == ipType)
                        MCASTDBG("Warring! This vlan has no router port!");
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                    else
                        MCASTDBG("Warring! This vlan has no router port!");
#endif
                    LOGIC_PORTMASK_COPY(ip46McstEntry.portmask, groupHead->mbr);
                }

                if (isMvrPackt)
                    ip46McstEntry.ipMcast.crossVlan = ENABLED;

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                if (MULTICAST_TYPE_IPV4 == ipType)
                {
                    ip46McstEntry.ipMcast.groupId = groupHead->groupId;
                }
                else
                {
                    ip46McstEntry.ipv6Mcast.groupId = groupHead->groupId;
                }

                if ((0 != groupHead->groupId) && IS_LOGIC_PORTMASK_CLEAR(groupHead->mbr))
                {
                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_OK;
                }
            }
#endif

                if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_add(ipType, &ip46McstEntry, isFwdAll)))
                {
                    mcast_log_asicIP_fail(ipType, &ip46McstEntry, MCAST_ADD);
                    
                    if (SYS_ERR_L2_MAC_FULL == ret)
                    {
                        SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType)));
                        igmp_trapDataPkt_lock = TRUE;
                    }
                }
                else
                {
#if 1
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                    if (IS_CHIP_CSW())
                    {
                        if (MULTICAST_TYPE_IPV4 == ipType)
                        {
                            if (IS_LOGIC_PORTMASK_CLEAR(ip46McstEntry.ipMcast.portmask))
                            {
                                mcast_hw_ipMcastEntryIncCPU_set(groupHead->ipType, &ip46McstEntry);
                            }
                        }
                        else
                        {
                            if (IS_LOGIC_PORTMASK_CLEAR(ip46McstEntry.ipv6Mcast.portmask))
                                mcast_hw_ipMcastEntryIncCPU_set(groupHead->ipType, &ip46McstEntry);
                        }
                    }
#endif
#endif
                    groupHead->lookupTableFlag = L2TABLE_WROTE;
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                    if (IS_CHIP_CSW())
                    {
                        if (MULTICAST_TYPE_IPV4 == ipType)
                        {
                            groupHead->groupId = ip46McstEntry.ipMcast.groupId;
                        }
                        else
                        {
                            groupHead->groupId = ip46McstEntry.ipv6Mcast.groupId;
                        }
                    }
#endif
                    mcast_hw_group_num_cal(TRUE);
                }

            }
#endif

        }
        else                        
        {
            
            mcast_build_groupEntryKey(vid, ipType, dip,  pGroupDip->ipv6.addr, &groupEntry);
            groupEntry.sip = sip;
            pEntry = mcast_group_get(&groupEntry);

            if (pEntry == NULL)
            {
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(lport, groupHead->mbr)
                {
                    mcast_port_entry_get(ipType, lport, &portEntryNum);
                    mcast_portGroup_limit_get(ipType, lport, &limitNum);

                    if(portEntryNum >= limitNum)
                    {
                        mcast_log_portLimit_arv(ipType,port,limitNum);
                        if(SYS_ERR_FAILED == mcast_maxGroupAct_process(ipType, vid, dip,0, lport))
                        {
                            continue;
                        }
                    }
                }

                mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);

                pNewEntry = mcast_group_freeEntry_get(ipType, isMvrPackt);
                if (NULL == pNewEntry)
                {
                    mcast_log_tablefull(ipType);
                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_MCAST_DATABASE_FULL;
                }

                pNewEntry->ipType = ipType;
                pNewEntry->form = IGMP_GROUP_ENTRY_DYNAMIC;
                pNewEntry->dip = dip;
                pNewEntry->sip = sip;
                pNewEntry->vid = vid;
                pNewEntry->sortKey = mcast_group_sortKey_ret(&groupEntry);
                pNewEntry->groupTimer = pMcastVlan->grpMembershipIntv;

                if (MULTICAST_TYPE_IPV4 !=  ipType)
                {
                    osal_memcpy(pNewEntry->dipv6.addr, groupHead->dipv6.addr, IPV6_ADDR_LEN);
                    osal_memcpy(pNewEntry->sipv6.addr, pSip->ipv6.addr, IPV6_ADDR_LEN);
                }

                if( IGMP_GROUP_ENTRY_STATIC == groupHead->form)
                {
                    pNewEntry->form = IGMP_GROUP_ENTRY_STATIC;
                    osal_memcpy(&pNewEntry->staticInMbr, &groupHead->staticInMbr, sizeof(sys_logic_portmask_t));
                }
                else
                    pNewEntry->form = IGMP_GROUP_ENTRY_DYNAMIC;

                if (IS_LOGIC_PORTMASK_CLEAR(groupHead->mbr))
                    pNewEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                osal_memcpy(&pNewEntry->mbr, &groupHead->mbr, sizeof(sys_logic_portmask_t));

                if (SYS_ERR_OK == mcast_group_sortedArray_ins(pNewEntry->sortKey, pNewEntry))
                {
                    if (isMvrPackt)
                        ip46McstEntry.ipMcast.crossVlan = ENABLED;
                    osal_memcpy(&ip46McstEntry.portmask, &groupHead->mbr, sizeof(sys_logic_portmask_t));

                    if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_add(ipType, &ip46McstEntry, isFwdAll)))
                    {
                        mcast_log_asicIP_fail(ipType, &ip46McstEntry,MCAST_ADD);
                        if (SYS_ERR_L2_MAC_FULL == ret)
                        {
                            mcast_group_entry_release(pNewEntry);
                            rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType));
                            igmp_trapDataPkt_lock = TRUE;
                            IGMP_DB_SEM_UNLOCK();
                            return SYS_ERR_OK;
                        }
                    }
                    else
                    {
                        pEntry->lookupTableFlag = L2TABLE_WROTE;
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                        if (IS_CHIP_CSW())
                        {
                            if (MULTICAST_TYPE_IPV4 == ipType)
                            {
                                pEntry->groupId = ip46McstEntry.ipMcast.groupId;
                            }
                            else
                            {
                                pEntry->groupId = ip46McstEntry.ipv6Mcast.groupId;
                            }
                        }
#endif
                    }
                }
                else
                {
                    mcast_group_entry_release(pNewEntry);
                }
            }
            else
            {
                
                if ( SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pEntry, &ip46McstEntry)))
                {
                    mcast_hw_ipMcastEntry_set(pEntry->ipType, &ip46McstEntry, FALSE, isFwdAll);
                }
                pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
            }
        }
    }
    else
    {
        pEntry = mcast_group_freeEntry_get(ipType, isMvrPackt);
        if (pEntry != NULL)
        {
            pEntry->ipType = ipType;
            pEntry->form = IGMP_GROUP_ENTRY_DYNAMIC;
            pEntry->dip = dip;
#if defined(CONFIG_SYS_RTL8380) || defined(CONFIG_SYS_RTL8390)
            if (IS_CHIP_MSW())
            {
                pEntry->sip = sip;
            }
#endif

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                if (IGMP_DIP_SIP_MODE == lookup_mode)
                    pEntry->sip = sip;
            }
#endif
            pEntry->vid = vid;
            pEntry->sortKey = mcast_group_sortKey_ret(&groupEntry);
            pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                pEntry->groupId = groupEntry.groupId;
            }
#endif

            if (MULTICAST_TYPE_IPV4 == ipType)
            {
                pEntry->mac[0] = 0x01;
                pEntry->mac[1] = 0x00;
                pEntry->mac[2] = 0x5e;
                pEntry->mac[3] = (dip >> 16) & 0x7f;
                pEntry->mac[4] = (dip >> 8) & 0xff;
                pEntry->mac[5] = dip & 0xff;
            }
            else
            {
                pEntry->mac[0] = 0x33;
                pEntry->mac[1] = 0x33;
                pEntry->mac[2] = (dip >> 24) & 0xff;
                pEntry->mac[3] = (dip >> 16) & 0xff;
                pEntry->mac[4] = (dip >> 8) & 0xff;
                pEntry->mac[5] = dip & 0xff;
                osal_memcpy(pEntry->dipv6.addr, pGroupDip->ipv6.addr, IPV6_ADDR_LEN);
                osal_memcpy(pEntry->sipv6.addr, pSip->ipv6.addr, IPV6_ADDR_LEN);
            }

            if (SYS_ERR_OK != (ret = mcastData_newGroup_add(pEntry, isMvrPackt, sortedIdx)))
            {
                mcast_group_entry_release(pEntry);
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }
        }
        else
        {
            mcast_log_tablefull(ipType);
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_MCAST_DATABASE_FULL;
        }
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}

int32 mcast_group_mcstData_add_wrapper_dmacvid(multicast_ipType_t ipType,uint16 vid, sys_nic_pkt_t *pBuf,
    mcast_groupAddr_t *pGroupDip, mcast_groupAddr_t *pSip, sys_logic_port_t port, uint8 isMvrPackt)
{
    uint16                  sortedIdx;
    int32                   ret;
    igmp_group_entry_t      *pEntry, *groupHead, groupEntry;
    sys_l2McastEntry_t      l2McstEntry;
    uint32 dip;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    uint8 isFwdAll = TRUE;

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        dip = pGroupDip->ipv4;
    }
    else
    {
        dip = IPV6_TO_IPV4(pGroupDip->ipv6.addr);
    }

    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, dip, pGroupDip->ipv6.addr, &groupEntry));

    IGMP_DB_SEM_LOCK();

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    #ifdef CONFIG_SYS_PROTO_MVR
    isFwdAll = mcast_mvr_fwdAll_ret(&groupEntry);
    #endif

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);   
    if (groupHead)
    {
        if (IGMP_GROUP_ENTRY_STATIC == groupHead->form)
        {
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }
        l2McstEntry.vid = groupHead->vid;
        osal_memcpy(l2McstEntry.mac, groupHead->mac, CAPA_MAC_BYTE_NUM);

        if (SYS_ERR_OK == mcast_hw_l2McastEntry_get(&l2McstEntry))
            mcast_hw_l2McastEntry_set(ipType, &l2McstEntry, TRUE, isFwdAll);
        groupHead->groupTimer = pMcastVlan->grpMembershipIntv;
    }
    else
    {
        pEntry = mcast_group_freeEntry_get(ipType, isMvrPackt);
        if (pEntry != NULL)
        {
            pEntry->ipType = ipType;
            pEntry->form = IGMP_GROUP_ENTRY_DYNAMIC;
            pEntry->dip = dip;
            pEntry->sip = 0;
            pEntry->vid = vid;
            pEntry->sortKey = mcast_group_sortKey_ret(&groupEntry);
            pEntry->groupTimer = pMcastVlan->grpMembershipIntv;

            if (MULTICAST_TYPE_IPV4 == ipType)
            {
                pEntry->mac[0] = 0x01;
                pEntry->mac[1] = 0x00;
                pEntry->mac[2] = 0x5e;
                pEntry->mac[3] = (dip >> 16) & 0x7f;
                pEntry->mac[4] = (dip >> 8) & 0xff;
                pEntry->mac[5] = dip & 0xff;
            }
            else
            {
                pEntry->mac[0] = 0x33;
                pEntry->mac[1] = 0x33;
                pEntry->mac[2] = (dip >> 24) & 0xff;
                pEntry->mac[3] = (dip >> 16) & 0xff;
                pEntry->mac[4] = (dip >> 8) & 0xff;
                pEntry->mac[5] = dip & 0xff;
                osal_memcpy(pEntry->dipv6.addr, pGroupDip->ipv6.addr, IPV6_ADDR_LEN);
            }

           if (SYS_ERR_OK !=  (ret = mcastData_newGroup_add(pEntry,isMvrPackt, sortedIdx)))
           {
             if (MULTICAST_TYPE_IPV4 == ipType)
                MCASTDBG("Mcast data add new group failure!");
            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
                MCASTDBG("Mcast data add new group failure!");
            #endif
              goto release;
           }
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }
        else
        {
            mcast_log_tablefull(ipType);
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_MCAST_DATABASE_FULL;
        }
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;

release:
    mcast_group_entry_release(pEntry);
    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_FAILED;
}
#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
int32 mcast_igmpv3_group_mcstData_add_wrapper_dipsip(multicast_ipType_t ipType, uint16 vid, sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pGroupDip,
    mcast_groupAddr_t * pSip, sys_logic_port_t port, uint8 isMvrGroup)
{
    int32                   ret;
    uint16                  sortedIdx;
    uint32                  freeCount;
    igmp_group_entry_t      *pEntry = NULL, *groupHead, newEntry, groupEntry;
    igmp_router_entry_t     *pRouterEntry;
    sys_ipMcastEntry_t      ipMcstEntry;
    sys_vlan_operEntry_t   vlanEntry;

    igmp_group_head_entry_t *group_head_entry;
    sys_logic_port_t portid;
    uint32 portEntryNum;
    uint16 limitNum;
    uint32 dip, sip;

    mcast_vlan_entry_t *pMcastVlan = NULL;

    osal_memset(&ipMcstEntry, 0, sizeof(sys_ipMcastEntry_t));
    osal_memset(&newEntry,0,sizeof(igmp_group_entry_t));

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        dip = pGroupDip->ipv4;
        sip = pSip->ipv4;
    }
    else
    {
        
        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, dip,pGroupDip->ipv6.addr, &groupEntry));
    groupEntry.sip = sip;

    IGMP_DB_SEM_LOCK();

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);   
    
    if (groupHead)
    {
        if (groupHead->sip == 0)     
        {
            mcast_router_db_get(vid, ipType, &pRouterEntry);
            if (pRouterEntry)
            {
                LOGIC_PORTMASK_OR(ipMcstEntry.portmask, pRouterEntry->router_pmsk, ipMcstEntry.portmask);
            }
            else
            {
                MCASTDBG("Warring! This vlan has no router port!");
            }

            if(IGMP_GROUP_ENTRY_DYNAMIC == groupHead->form)
            {
                groupHead->sip = sip;
                mcast_igmpv3_fwdPmsk_get(&ipMcstEntry.portmask,groupHead);
                            
            }
            else
            {
                pEntry = mcast_group_get(&groupEntry);
                if(pEntry == NULL)
                {
                    newEntry.ipType = ipType;
                    newEntry.form = IGMP_GROUP_ENTRY_STATIC;
                    newEntry.dip = dip;
                    newEntry.sip = sip;
                    newEntry.vid = vid;
                    newEntry.ipType = ipType;
                    newEntry.groupTimer = pMcastVlan->grpMembershipIntv;
                    osal_memcpy(newEntry.mac, groupHead->mac, CAPA_MAC_BYTE_NUM);
                    osal_memcpy(&newEntry.mbr, &groupHead->mbr,sizeof(sys_logic_portmask_t));
                    mcast_group_add(&newEntry);
                    LOGIC_PORTMASK_OR(ipMcstEntry.portmask,ipMcstEntry.portmask,groupHead->mbr);
                }
            }

            ipMcstEntry.vid = vid;
            ipMcstEntry.dip = dip;
            ipMcstEntry.sip = sip;
            if (isMvrGroup)
                ipMcstEntry.crossVlan = ENABLED;
            if (SYS_ERR_OK != (ret = rsd_ipMcastEntry_add(&ipMcstEntry)))
            {
                
                rsd_mcast_fwdIndexFreeCount_get(&freeCount);
                if (freeCount == 0)
                {
                    SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType)));
                    igmp_trapDataPkt_lock = TRUE;
                }

                MCASTDBG("Failed writing to ASIC!  ret:%d", ret);
            }
            else
            {
                if(IGMP_GROUP_ENTRY_DYNAMIC == groupHead->form)
                {
                    groupHead->lookupTableFlag = L2TABLE_WROTE;
                }
                else
                {
                    pEntry = mcast_group_get(&groupEntry);
                    if(pEntry != NULL)
                        pEntry->lookupTableFlag = L2TABLE_WROTE;
                }
                mcast_hw_group_num_cal(TRUE);
            }
        }
        else                        
        {
            
            pEntry = mcast_group_get(&groupEntry);

            if (IGMP_VERSION_V3_BASIC == igmp_cfg.oper_version)
            {
                if (pEntry == NULL)
                {
                    mcast_port_entry_get(ipType, port, &portEntryNum);
                    mcast_portGroup_limit_get(ipType, port, &limitNum);
                    if(portEntryNum >= limitNum)
                    {
                        mcast_log_portLimit_arv(ipType, port, limitNum);
                        if(SYS_ERR_FAILED == mcast_maxGroupAct_process(ipType, vid, dip,0, port))
                        {
                            IGMP_DB_SEM_UNLOCK();
                            return SYS_ERR_OK;
                        }
                    }

                    newEntry.dip = dip;
                    newEntry.sip = sip;
                    newEntry.vid = vid;
                    newEntry.ipType = ipType;
                    if(IGMP_GROUP_ENTRY_STATIC == groupHead->form)
                    {
                        newEntry.form = IGMP_GROUP_ENTRY_STATIC;
                        osal_memcpy(&newEntry.staticInMbr, &groupHead->staticInMbr, sizeof(sys_logic_portmask_t));
                        osal_memcpy(&newEntry.staticMbr, &groupHead->staticMbr, sizeof(sys_logic_portmask_t));
                    }
                    else
                    {
                        newEntry.form = IGMP_GROUP_ENTRY_DYNAMIC;
                        newEntry.groupTimer = pMcastVlan->grpMembershipIntv;
                    }

                    osal_memcpy(&newEntry.mbr, &groupHead->mbr, sizeof(sys_logic_portmask_t));

                    osal_memcpy(newEntry.mac, groupHead->mac, CAPA_MAC_BYTE_NUM);

                    ret = mcast_group_add(&newEntry);
                    if (ret)
                    {
                        MCASTDBG("Failed add a group!  ret:%d", ret);
                    }

                    ipMcstEntry.vid = vid;
                    ipMcstEntry.dip = dip;
                    ipMcstEntry.sip = sip;
                    if (isMvrGroup)
                        ipMcstEntry.crossVlan = ENABLED;

                    osal_memcpy(&ipMcstEntry.portmask, &groupHead->mbr, sizeof(sys_logic_portmask_t));

                    if (SYS_ERR_OK != (ret = rsd_ipMcastEntry_add(&ipMcstEntry)))
                    {
                        
                        rsd_mcast_fwdIndexFreeCount_get(&freeCount);
                        if (freeCount == 0)
                        {
                            SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType)));
                            igmp_trapDataPkt_lock = TRUE;
                        }

                        MCASTDBG("Failed writing to ASIC!  ret:%d", ret);
                    }
                    else
                    {
                        pEntry = mcast_group_get(&groupEntry);
                        if(pEntry != NULL)
                            pEntry->lookupTableFlag = L2TABLE_WROTE;

                        mcast_hw_group_num_cal(TRUE);
                    }
                }
                else    
                {
                    MCASTDBG("The entry is not null!");

                }
            }
            else
            {
                mcast_igmp_group_head_get(dip, vid, &group_head_entry);
                if(group_head_entry != NULL)
                {
                    if(pEntry == NULL)
                    {
                        
                            newEntry.ipType = ipType;
                            newEntry.dip = dip;
                            newEntry.sip = sip;
                            newEntry.vid = vid;
                            newEntry.form = IGMP_GROUP_ENTRY_DYNAMIC;

                            newEntry.groupTimer = pMcastVlan->grpMembershipIntv;
                            osal_memcpy(newEntry.mac, groupHead->mac, CAPA_MAC_BYTE_NUM);

                            if(IS_LOGIC_PORTMASK_CLEAR(group_head_entry->fmode))
                            {
                                LOGIC_PORTMASK_CLEAR_ALL(ipMcstEntry.portmask);
                            }
                            else
                            {
                                
                                osal_memcpy(&ipMcstEntry.portmask, &group_head_entry->fmode, sizeof(sys_logic_portmask_t));

                                FOR_EACH_PORT_IN_LOGIC_PORTMASK(portid, group_head_entry->fmode)
                                {
                                    LOGIC_PORTMASK_SET_PORT(newEntry.mbr, portid);
                                    newEntry.v3.mbrFwdFlag[portid] = IGMPV3_FWD_NORMAL;
                                }
                            }

                            ret = mcast_group_add(&newEntry);
                            if (ret)
                            {
                                MCASTDBG("Failed add a group!  ret:%d", ret);
                            }

                            ipMcstEntry.vid = vid;
                            ipMcstEntry.dip = dip;
                            ipMcstEntry.sip = sip;
                            if (isMvrGroup)
                                    ipMcstEntry.crossVlan = ENABLED;

                            if (SYS_ERR_OK != (ret = rsd_ipMcastEntry_add(&ipMcstEntry)))
                            {
                                
                                rsd_mcast_fwdIndexFreeCount_get(&freeCount);
                                if (freeCount == 0)
                                {
                                    SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType)));
                                    igmp_trapDataPkt_lock = TRUE;
                                }

                                MCASTDBG("Failed writing to ASIC!  ret:%d", ret);
                            }
                            else
                            {
                                pEntry = mcast_group_get(&groupEntry);
                                if(pEntry != NULL)
                                    pEntry->lookupTableFlag = L2TABLE_WROTE;

                                mcast_hw_group_num_cal(TRUE);
                                #if 0
                                igmp_hw_entry_num++;
                                if (igmp_hw_entry_num == CAPA_MCAST_GROUP_NUM)
                                {
                                    SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED,gUnknown_mcast_igmp_action));
                                    igmp_trapDataPkt_lock = TRUE;
                                }
                                #endif
                            }
                    }
                    else
                    {
                        mcast_igmpv3_ipMcstEntry_update_dipsip(vid,dip,sip,pEntry);
                    }
                }

            }
        }
    }
    else
    {
        pEntry = mcast_group_freeEntry_get(ipType, FALSE);
        if (pEntry != NULL)
        {
            pEntry->ipType = ipType;
            pEntry->form = IGMP_GROUP_ENTRY_DYNAMIC;
            pEntry->dip = dip;
            pEntry->sip = sip;
            pEntry->vid = vid;
            pEntry->sortKey = mcast_group_sortKey_ret(&groupEntry);
            pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
            pEntry->mac[0] = 0x01;
            pEntry->mac[1] = 0x00;
            pEntry->mac[2] = 0x5e;
            pEntry->mac[3] = (dip >> 16) & 0x7f;
            pEntry->mac[4] = (dip >> 8) & 0xff;
            pEntry->mac[5] = dip & 0xff;

            ipMcstEntry.vid = vid;
            ipMcstEntry.dip = dip;
            ipMcstEntry.sip = sip;

            LOGIC_PORTMASK_CLEAR_ALL(ipMcstEntry.portmask);

            if(IGMP_VERSION_V3_BASIC == igmp_cfg.oper_version)
            {
                if (IGMP_UNKNOWN_MCAST_ACTION_FLOOD == mcast_unknownAct_ret(ipType))
                {
                    
                    rsd_vlan_operEntry_get(vid, &vlanEntry);
                    LOGIC_PORTMASK_COPY(ipMcstEntry.portmask, vlanEntry.lpmMember);
                }
                else if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(ipType))
                {
                    mcast_router_db_get(vid, ipType, &pRouterEntry);
                    if (pRouterEntry)
                        LOGIC_PORTMASK_OR(ipMcstEntry.portmask, ipMcstEntry.portmask, pRouterEntry->router_pmsk);
                    else
                        LOGIC_PORTMASK_COPY(ipMcstEntry.portmask, ipMcstEntry.portmask);
                }
            }
            #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
            else if(IGMP_VERSION_V3_FULL == igmp_cfg.oper_version)
            {
                mcast_igmp_group_head_ins(dip, vid, port, MODE_IS_INCLUDE,  &group_head_entry);
            }
            #endif

            if (isMvrGroup)
                ipMcstEntry.crossVlan = ENABLED;
            if (SYS_ERR_OK != (ret = rsd_ipMcastEntry_add(&ipMcstEntry)))
            {
                
                rsd_mcast_fwdIndexFreeCount_get(&freeCount);
                if (freeCount == 0)
                {
                    SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType)));
                    igmp_trapDataPkt_lock = TRUE;
                    mcast_group_entry_release(pEntry);
                }
                MCASTDBG("Failed writing to ASIC!  ret:%d", ret);
            }
            else
            {
                pEntry->lookupTableFlag = L2TABLE_WROTE;
                mcast_group_num_cal(ipType,FALSE, isMvrGroup,TRUE);
                mcast_group_sortedArray_ins(sortedIdx, pEntry);

                mcast_hw_group_num_cal(TRUE);
                #if 0
                igmp_hw_entry_num++;
                if (igmp_hw_entry_num == CAPA_MCAST_GROUP_NUM)
                {
                    SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType,DISABLED,gUnknown_mcast_igmp_action));
                    igmp_trapDataPkt_lock = TRUE;
                }
                #endif
            }
        }
        else
        {
            IGMP_DB_SEM_UNLOCK();
            mcast_log_tablefull(ipType);
            return SYS_ERR_MCAST_DATABASE_FULL;
        }
    }

    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 mcast_igmpv3_ipMcstEntry_update_dipsip(uint16 vid, uint32 dip, uint32 sip, igmp_group_entry_t *pEntry)
{
    int32 ret;
    uint32                  freeCount;
    igmp_router_entry_t     *pRouterEntry;
    sys_ipMcastEntry_t      ipMcstEntry;
    sys_logic_portmask_t   setMsk;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    mcast_igmpv3_fwdPmsk_get(&setMsk, pEntry);

    mcast_router_db_get(vid, ipType, &pRouterEntry);
    if (pRouterEntry)
    {
        LOGIC_PORTMASK_OR(setMsk, pRouterEntry->router_pmsk, setMsk);
    }

     if(SYS_ERR_OK != (ret = rsd_ipMcastEntry_get(&ipMcstEntry)))
     {
        LOGIC_PORTMASK_OR(ipMcstEntry.portmask, ipMcstEntry.portmask, setMsk);

        if (SYS_ERR_OK != (ret = rsd_ipMcastEntry_add(&ipMcstEntry)))
        {
            
            rsd_mcast_fwdIndexFreeCount_get(&freeCount);
            if (freeCount == 0)
            {
                SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType)));
                igmp_trapDataPkt_lock = TRUE;
            }

            MCASTDBG("Failed writing to ASIC!  ret:%d", ret);
        }
        else
        {
            pEntry->lookupTableFlag = L2TABLE_WROTE;
            mcast_hw_group_num_cal(TRUE);
            #if 0
            igmp_hw_entry_num++;
            if (igmp_hw_entry_num == CAPA_MCAST_GROUP_NUM)
            {
                SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED,gUnknown_mcast_igmp_action));
                igmp_trapDataPkt_lock = TRUE;
            }
            #endif
        }
    }
    else
    {
        if (SYS_ERR_OK != (ret = rsd_ipMcastEntry_set(&ipMcstEntry)))
        {
            MCASTDBG("Failed writing to ASIC!  ret:%d", ret);
        }
    }

    return SYS_ERR_OK;
}
int32 mcast_igmp_group_head_db_init(void)
{
    osal_memset(&groupHead_db, 0, sizeof(groupHead_db));
    return SYS_ERR_OK;
}
int32 mcast_igmp_group_head_ins(uint32 groupAddr, uint16 vid, uint16 portid, uint16 rcdType, igmp_group_head_entry_t **ppGrpHead)
{
    igmp_group_head_entry_t *pEntry = NULL;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        return SYS_ERR_FAILED;
    }

    mcast_igmp_group_head_get(groupAddr, vid, &pEntry);
    if(pEntry != NULL)
        return SYS_ERR_OK;

    pEntry = mcast_igmp_free_group_head_get();
    if(pEntry != NULL)
    {
        osal_memset(&pEntry->fmode, 0, sizeof(sys_logic_portmask_t));
        
        if(rcdType == CHANGE_TO_EXCLUDE_MODE || rcdType == MODE_IS_EXCLUDE)
            PORT_SET_FILTER_EXCLUDE(pEntry->fmode, portid);
        else if(rcdType == CHANGE_TO_INCLUDE_MODE || rcdType == MODE_IS_INCLUDE)
            PORT_SET_FILTER_INCLUDE(pEntry->fmode, portid);

        pEntry->ipType = ipType;
        pEntry->dip = groupAddr;
        pEntry->sip = 0;
        pEntry->vid = vid;

        pEntry->mac[0] = 0x01;
        pEntry->mac[1] = 0x00;
        pEntry->mac[2] = 0x5e;
        pEntry->mac[3] = (groupAddr >> 16) & 0xff;
        pEntry->mac[4] = (groupAddr >> 8) & 0xff;
        pEntry->mac[5] = groupAddr & 0xff;

        pEntry->mbrTimer[portid] = pMcastVlan->grpMembershipIntv;

        *ppGrpHead = pEntry;
    }
    else
    {
        MCASTDBG("Group head database is full!");
        *ppGrpHead = NULL;
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

igmp_group_head_entry_t *mcast_igmp_group_head_get(uint32 groupAddr, uint16 vid, igmp_group_head_entry_t **pGrpHead)
{
    int i;
    igmp_group_head_entry_t  *pEntry = NULL;

    for(i = 0; i < CAPA_MCAST_GROUP_NUM; i++)
    {
        if(groupHead_db[i].dip == groupAddr && groupHead_db[i].vid == vid)
        {
            pEntry = &groupHead_db[i];
            break;
        }
    }
    *pGrpHead = pEntry;
    return pEntry;
}
int32 mcast_igmp_group_head_del(uint32 groupAddr,uint16 vid)
{
    int i;

    for(i = 0; i < CAPA_MCAST_GROUP_NUM; i++)
    {
        if(groupHead_db[i].dip == groupAddr && groupHead_db[i].vid == vid)
        {
            groupHead_db[i].dip = 0;
            groupHead_db[i].vid = 0;
            break;
        }
    }

    return SYS_ERR_OK;
}

igmp_group_head_entry_t *mcast_igmp_free_group_head_get(void)
{
    int i;
    igmp_group_head_entry_t  *pEntry = NULL;

    for(i = 0; i < CAPA_MCAST_GROUP_NUM; i++)
    {
        if(groupHead_db[i].dip == 0)
        {
            pEntry = &groupHead_db[i];
            break;
        }
    }

    return pEntry;
}

int32 mcast_igmpv3_source_add_wrapper(uint32 groupAddr, uint32 sourceAddr, uint16 vid, uint16 portid,
                                uint32 timer, uint16 fwdFlag)
{
    uint32 ret;

    sys_ipMcastEntry_t      ipMcstEntry;
    igmp_group_entry_t    newEntry, *pEntry = NULL;
    uint8  portFlag = FALSE;
    uint32 portEntryNum;
    uint16 limitNum;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    mcast_port_entry_get( ipType, portid, &portEntryNum);
    mcast_portGroup_limit_get(ipType, portid, &limitNum);

    osal_memset(&newEntry, 0 , sizeof(igmp_group_entry_t));
    newEntry.dip = groupAddr;
    newEntry.sip = sourceAddr;
    newEntry.vid = vid;
    newEntry.ipType = ipType;

    pEntry = mcast_group_get(&newEntry);
    if (pEntry == NULL)
    {
        if(portEntryNum >= limitNum)
        {
            mcast_log_portLimit_arv(ipType,portid,limitNum);
            return SYS_ERR_OK;
        }

        osal_memset(&newEntry, 0 , sizeof(igmp_group_entry_t));
        MCASTDBG("add new group entry  ip is : "IPADDR_PRINT"",  IPADDR_PRINT_ARG(sourceAddr));
        newEntry.dip = groupAddr;
        newEntry.sip = sourceAddr;
        newEntry.vid = vid;
        newEntry.ipType = ipType;

        newEntry.lookupTableFlag = L2TABLE_NOT_WROTE;

        LOGIC_PORTMASK_SET_PORT(newEntry.mbr, portid);

        newEntry.mac[0] = 0x01;
        newEntry.mac[1] = 0x00;
        newEntry.mac[2] = 0x5e;
        newEntry.mac[3] = (groupAddr >> 16) & 0xff;
        newEntry.mac[4] = (groupAddr >> 8) & 0xff;
        newEntry.mac[5] = groupAddr & 0xff;
        newEntry.v3.mbrFwdFlag[portid] = fwdFlag;
        newEntry.form = IGMP_GROUP_ENTRY_DYNAMIC;
        ret = mcast_group_add(&newEntry);
        if (ret)
        {
            MCASTDBG("Failed add a group!  ret:%d", ret);
            return ret;
        }
    }
    else 
    {
        MCASTDBG("the group entry is exist !");

        if(IS_LOGIC_PORTMASK_PORTSET(pEntry->staticMbr, portid))
        {
            return SYS_ERR_OK;
        }

        if(!IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, portid))
        {
            if(portEntryNum >= limitNum)
            {
                MCASTDBG("The Port %s group entry is full", LPSTR(portid));
                return SYS_ERR_OK;
            }
            LOGIC_PORTMASK_SET_PORT(pEntry->mbr, portid);
            portFlag = TRUE;
        }

        pEntry->mbrTimer[portid] =  timer;

        if(PORT_IS_SRC_QUERY(pEntry->v3.isGssQueryFlag,portid))
        {
            PORT_SET_SRC_NOT_QUERY(pEntry->v3.isGssQueryFlag,portid);
        }

        pEntry->v3.mbrFwdFlag[portid] =  fwdFlag;
        if(pEntry->lookupTableFlag == L2TABLE_WROTE && portFlag)
        {
            ipMcstEntry.vid = vid;
            ipMcstEntry.sip = pEntry->sip;
            ipMcstEntry.dip = pEntry->dip;

            mcast_igmpv3_fwdPmsk_get(&ipMcstEntry.portmask, pEntry);
            if(SYS_ERR_OK != (ret = rsd_ipMcastEntry_set(&ipMcstEntry)))
            {
                MCASTDBG("Failed get from ASIC!  ret:%d", ret);
                return SYS_ERR_FAILED;
            }
        }

    }

    return SYS_ERR_OK;
}
int32 mcast_igmpv3_source_is_inPkt(igmp_group_entry_t *pEntry, uint16 portid, uint32 *pSrcArray, uint32 srcNum)
{
    uint32 i;
    uint32 *ptr = pSrcArray;

    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == pSrcArray, SYS_ERR_NULL_POINTER);

    for(i = 0; i < srcNum; i++)
    {
        if((pEntry->sip == *ptr)  &&
            (IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, portid)))
            return TRUE;
        ptr++;
    }

    return FALSE;
}

int32 mcast_igmpv3_grp_src_spec_get(uint32 groupAddr, uint16 vid, uint16 portid, uint32 *pNumSrc, uint32 *pSip)
{
    igmp_group_entry_t *groupHead = NULL, *pEntry = NULL, groupEntry;
    uint16 sortedIdx;
    uint32 tmpCnt = 0;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, groupAddr, NULL, &groupEntry));

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);
    if(groupHead == NULL)
    {
        MCASTDBG("the Group ip is : "IPADDR_PRINT" is not exist", IPADDR_PRINT_ARG(groupAddr));
        return SYS_ERR_FAILED;
    }

    pEntry = groupHead;
    while(NULL != pEntry)
    {
        if(PORT_IS_SRC_QUERY(pEntry->v3.isGssQueryFlag, portid))
        {
            pSip[tmpCnt++] = pEntry->sip;
        }
        pEntry = pEntry->next_subgroup;
    }

    *pNumSrc = tmpCnt;

    return SYS_ERR_OK;

}
int32 mcast_igmpv3_fwdPmsk_get(sys_logic_portmask_t *pPortMsk,  igmp_group_entry_t *group)
{
    sys_logic_portmask_t pmsk;
    uint16 port;

    osal_memset(&pmsk, 0, sizeof(sys_logic_portmask_t));

    if(pPortMsk == NULL || group == NULL)
        return SYS_ERR_FAILED;

    FOR_EACH_LOGIC_PORT(port)
    {
        if( IS_LOGIC_PORTMASK_PORTSET(group->mbr, port) &&
            group->v3.mbrFwdFlag[port] != IGMPV3_FWD_NOT)
        {
            LOGIC_PORTMASK_SET_PORT(pmsk, port);
        }
    }

    osal_memcpy(pPortMsk, &pmsk, sizeof(sys_logic_portmask_t));
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_groupMbrPort_isIn_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    uint32 groupAddr = 0, dip;
    uint16 numSrc = 0, i;
    uint32 *sourceAddr = NULL;

    igmp_group_head_entry_t *group_head_entry;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    groupAddr = ntohl(pRcd->grec_mca);
    numSrc = ntohl(pRcd->grec_nsrcs);
    sourceAddr =  pRcd->grec_src;
    dip = groupAddr;

    IGMP_DB_SEM_LOCK();
    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    if(numSrc != 0)
    {
        mcast_igmp_group_head_get(groupAddr, vid, &group_head_entry);

        if (group_head_entry)
        {
            if(!(PORT_IS_FILTER_EXCLUDE(group_head_entry->fmode, portid)))
            {
                
                for(i = 0; i < numSrc; i++)
                {
                    mcast_igmpv3_source_add_wrapper(dip,*sourceAddr, vid,portid,
                        pMcastVlan->grpMembershipIntv,
                        IGMPV3_FWD_CONFLICT);
                    sourceAddr++;
                }
            }
            else
            {
                
                MCASTDBG("proc X+A, and A = GMI");

                group_head_entry->mbrTimer[portid] = pMcastVlan->grpMembershipIntv;

                for(i = 0; i < numSrc; i++)
                {
                    mcast_igmpv3_source_add_wrapper(dip,*sourceAddr, vid,portid,
                        pMcastVlan->grpMembershipIntv,
                        IGMPV3_FWD_CONFLICT);
                    sourceAddr++;
                }
            }
        }
        else  
        {
            MCASTDBG("Group head is not exist !");
            mcast_igmp_group_head_ins(groupAddr, vid, portid, MODE_IS_INCLUDE, &group_head_entry);

            if(group_head_entry != NULL)
            {
                for(i = 0; i < numSrc; i++)
                {
                    MCASTDBG("the source ip is : "IPADDR_PRINT"", IPADDR_PRINT_ARG(*sourceAddr));
                    mcast_igmpv3_source_add_wrapper(groupAddr, *sourceAddr, vid, portid,
                        pMcastVlan->grpMembershipIntv,
                        IGMPV3_FWD_CONFLICT);
                    sourceAddr++;
                }
            }
        }
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_groupMbrPort_isEx_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid, uint16 rcdType)
{
    uint32 grpAddr = 0;
    uint16 srcNum = 0, i;
    uint16 sortedIdx;
    uint32  *pSourceAddr = NULL, *sourceArray = NULL;
    uint32  sipArray[32], numSip = 0;

    igmp_group_entry_t  *pEntry = NULL, *groupHead, groupEntry;
    igmp_querier_entry_t *pQueryEntry = NULL;
    igmp_group_head_entry_t  *group_head_entry = NULL;

    sys_logic_portmask_t delPmsk;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    grpAddr = ntohl(pRcd->grec_mca);
    srcNum = ntohl(pRcd->grec_nsrcs);
    pSourceAddr =  pRcd->grec_src;
    sourceArray = pSourceAddr;

    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, grpAddr, NULL, &groupEntry));

    IGMP_DB_SEM_LOCK();

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if(NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    osal_memset(sipArray,0,sizeof(sipArray));

    mcast_igmp_group_head_get(grpAddr,vid, &group_head_entry);
    if(srcNum == 0)
    {
        if(CHANGE_TO_EXCLUDE_MODE == rcdType)
        {
            MCASTDBG("no pkt src, message join");

            if(SYS_ERR_OK != mcast_igmp_group_head_ins(grpAddr, vid, portid, MODE_IS_INCLUDE, &group_head_entry))
            {
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }
            if(group_head_entry != NULL)
            {
                mcast_igmpv3_source_add_wrapper(grpAddr, 0, vid, portid,
                    pMcastVlan->grpMembershipIntv,
                    IGMPV3_FWD_CONFLICT);
            }
        }
    }
    else
    {
        if(group_head_entry == NULL)
        {
            MCASTDBG("the group head not exist");

            if(SYS_ERR_OK != mcast_igmp_group_head_ins(grpAddr, vid, portid, MODE_IS_EXCLUDE, &group_head_entry))
            {
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }

            if(group_head_entry != NULL)
            {
                for(i = 0; i < srcNum; i++)
                {
                    mcast_igmpv3_source_add_wrapper(grpAddr, *pSourceAddr, vid, portid,
                        pMcastVlan->grpMembershipIntv,
                        IGMPV3_FWD_NOT);
                    pSourceAddr++;
                }
            }
        }
        else
        {
            MCASTDBG("the group head exist");

            group_head_entry->mbrTimer[portid] = pMcastVlan->grpMembershipIntv;

            mcast_querier_db_get(vid, ipType, &pQueryEntry);

            mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);

            if(!PORT_IS_FILTER_EXCLUDE(group_head_entry->fmode, portid))
            {
                
                MCASTDBG("group is include mode");
                
                PORT_SET_FILTER_EXCLUDE(group_head_entry->fmode, portid);
                MCASTDBG("handle src in database, not pkt  --(A-B)");

                pEntry = groupHead;
                while(pEntry != NULL)
                {
                    if(IS_LOGIC_PORTMASK_PORTSET(pEntry->staticMbr, portid))
                    {
                        pEntry = pEntry->next_subgroup;
                        continue;
                    }
                    
                    if(mcast_igmpv3_source_is_inPkt(pEntry, portid, sourceArray, srcNum) == FALSE)
                    {
                        if((pEntry->v3.mbrFwdFlag[portid] != IGMPV3_FWD_NORMAL ) &&
                            (IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, portid)))
                        {
                            pEntry->mbrTimer[portid] = pMcastVlan->grpMembershipIntv;
                            pEntry->v3.mbrFwdFlag[portid] = IGMPV3_FWD_NORMAL;
                        }
                    }
                    else
                    {
                        pEntry->mbrTimer[portid] = pMcastVlan->grpMembershipIntv;
                        pEntry->v3.mbrFwdFlag[portid] = IGMPV3_FWD_CONFLICT;
                        
                        if(rcdType == CHANGE_TO_EXCLUDE_MODE)
                        {
                            if(pMcastVlan->fastleave_enable)
                            {
                                pEntry->mbrTimer[portid] = 0;
                                LOGIC_PORTMASK_CLEAR_PORT(pEntry->mbr, portid);

                                if(pEntry->lookupTableFlag == L2TABLE_WROTE)
                                {
                                    osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
                                    LOGIC_PORTMASK_SET_PORT(delPmsk, portid);
                                    mcast_hw_mcst_mbr_remove(pEntry, &delPmsk);
                                }

                                if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
                                {
                                    pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                                }
                            }
                            else
                            {
                                sipArray[numSip++] = pEntry->sip;
                                PORT_SET_SRC_QUERY(pEntry->v3.isGssQueryFlag, portid);
                            }
                        }
                    }
                    pEntry = pEntry->next_subgroup;
                }
                MCASTDBG("handle pkt src, not database --(B-A)");

                for(i = 0; i < srcNum; i++)
                {
                    groupEntry.sip = *pSourceAddr;
                    pEntry = mcast_group_get(&groupEntry);
                    if(pEntry == NULL)
                        mcast_igmpv3_source_add_wrapper(groupHead->dip, *pSourceAddr,vid,portid,
                        pMcastVlan->grpMembershipIntv, 
                        IGMPV3_FWD_NOT);
                    else
                    {
                        if(!IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, portid))
                            mcast_igmpv3_source_add_wrapper(groupHead->dip, *pSourceAddr,vid,portid,
                            pMcastVlan->grpMembershipIntv, 
                            IGMPV3_FWD_NOT);

                        if(pEntry->v3.mbrFwdFlag[portid] == IGMPV3_FWD_NORMAL)
                            pEntry->v3.mbrFwdFlag[portid] = IGMPV3_FWD_CONFLICT;
                    }

                    pSourceAddr++;
                }
            }
            else
            {
                
                MCASTDBG("group is exclude mode");

                group_head_entry->mbrTimer[portid] =  pMcastVlan->grpMembershipIntv;

                for(i = 0; i < srcNum; i++)
                {
                    groupEntry.sip = *pSourceAddr;
                    pEntry = mcast_group_get(&groupEntry);
                    if(pEntry == NULL)
                    {
                        
                        mcast_igmpv3_source_add_wrapper(grpAddr, *pSourceAddr,vid, portid,
                            pMcastVlan->grpMembershipIntv,
                            IGMPV3_FWD_CONFLICT);

                        if(rcdType == CHANGE_TO_EXCLUDE_MODE)
                        {
                            pEntry = mcast_group_get(&groupEntry);
                            if(pEntry != NULL)
                            {
                                if(IS_LOGIC_PORTMASK_PORTSET(pEntry->staticMbr, portid))
                                    continue;

                                if(pMcastVlan->fastleave_enable)
                                {
                                    pEntry->mbrTimer[portid] = 0;
                                    LOGIC_PORTMASK_CLEAR_PORT(pEntry->mbr, portid);

                                    if(pEntry->lookupTableFlag == L2TABLE_WROTE)
                                    {
                                        osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
                                        LOGIC_PORTMASK_SET_PORT(delPmsk, portid);
                                        mcast_hw_mcst_mbr_remove(pEntry, &delPmsk);
                                    }

                                    if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
                                    {
                                        pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                                    }
                                }
                                else
                                {
                                    sipArray[numSip++] = *pSourceAddr;
                                    PORT_SET_SRC_QUERY(pEntry->v3.isGssQueryFlag, portid);
                                }
                            }
                        }
                    }
                    else
                    {
                        if(IS_LOGIC_PORTMASK_PORTSET(pEntry->staticMbr, portid))
                            continue;

                        if(!IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, portid))
                            mcast_igmpv3_source_add_wrapper(grpAddr, *pSourceAddr,vid, portid,
                            pMcastVlan->grpMembershipIntv,
                            IGMPV3_FWD_NOT);

                        if(pEntry->v3.mbrFwdFlag[portid] == IGMPV3_FWD_CONFLICT)
                        {
                            if(rcdType == CHANGE_TO_EXCLUDE_MODE)
                            {
                                if(pMcastVlan->fastleave_enable)
                                {
                                    pEntry->mbrTimer[portid] = 0;
                                    LOGIC_PORTMASK_CLEAR_PORT(pEntry->mbr, portid);

                                    if(pEntry->lookupTableFlag == L2TABLE_WROTE)
                                    {
                                        osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
                                        LOGIC_PORTMASK_SET_PORT(delPmsk, portid);
                                        mcast_hw_mcst_mbr_remove(pEntry, &delPmsk);
                                    }

                                    if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
                                    {
                                        pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                                    }
                                }
                                else
                                {
                                    sipArray[numSip++] = *pSourceAddr;
                                    PORT_SET_SRC_QUERY(pEntry->v3.isGssQueryFlag, portid);
                                }
                            }

                        }

                    }
                    pSourceAddr++;
                }

                pEntry = groupHead;
                while(pEntry != NULL)
                {
                    if(mcast_igmpv3_source_is_inPkt(pEntry, portid, sourceArray,srcNum) == FALSE)
                    {
                        if((pEntry->v3.mbrFwdFlag[portid] != IGMPV3_FWD_NORMAL ) &&
                            (IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, portid)))
                        {
                            MCASTDBG("Group Address of IP = "IPADDR_PRINT", Src IP = "IPADDR_PRINT"",
                                IPADDR_PRINT_ARG(pEntry->dip),
                                IPADDR_PRINT_ARG(pEntry->sip));

                            pEntry->mbrTimer[portid] = pMcastVlan->grpMembershipIntv;
                            pEntry->v3.mbrFwdFlag[portid] = IGMPV3_FWD_NORMAL;
                        }
                    }
                    pEntry = pEntry->next_subgroup;
                }
            }

            if( (rcdType == CHANGE_TO_EXCLUDE_MODE) && (!pMcastVlan->fastleave_enable))
            {
                if (pQueryEntry == NULL)
                {
                    MCASTDBG("[IGMP]VLAN-%d doesn't have query DB entry!", vid);
                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_FAILED;
                }

                if(pQueryEntry->status == IGMP_QUERIER)
                {
                    if(numSip > 0)
                    {

                        for(i = 0; i < numSip; i++)
                        {
                            groupEntry.sip = sipArray[i];
                            pEntry = mcast_group_get(&groupEntry);
                            if(pEntry != NULL)
                            {
                                pEntry->v3.gssQueryCnt[portid]++;
                                mcast_igmp_send_grp_src_specific_query(vid,grpAddr, &sipArray[i],1, portid);
                            }
                        }
                    }
                }

            }
        }
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 mcast_igmpv3_groupMbrPort_toIn_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    uint32 grpAddr = 0;
    uint16 srcNum = 0, i;
    uint16 sortedIdx;
    uint32  *pSourceAddr = NULL;
    uint32  sipArray[32], numSip = 0;

    igmp_group_entry_t  *pEntry,  *groupHead = NULL, groupEntry;
    igmp_group_head_entry_t   *group_head_entry = NULL;
    igmp_querier_entry_t *pQueryEntry = NULL;

    sys_logic_portmask_t delPmsk;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    grpAddr = ntohl(pRcd->grec_mca);
    srcNum = ntohl(pRcd->grec_nsrcs);
    pSourceAddr =  pRcd->grec_src;

    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, grpAddr, NULL, &groupEntry));

    IGMP_DB_SEM_LOCK();

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if(NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }
    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);

    mcast_igmp_group_head_get( grpAddr, vid, &group_head_entry);

    if(srcNum == 0)  
    {
        if(groupHead == NULL)
        {
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }
        else
        {
            MCASTDBG("no pkt src, message leave");
            while(groupHead)
            {
                groupHead->mbrTimer[portid] = 1;     
                groupHead = groupHead->next_subgroup;
            }
        }
    }
    else
    {
        if(group_head_entry == NULL)
        {
            MCASTDBG("Group not exist !");

            if(SYS_ERR_OK != mcast_igmp_group_head_ins(grpAddr, vid, portid, MODE_IS_INCLUDE, &group_head_entry))
            {
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }

            if(group_head_entry != NULL)
            {
                for(i = 0; i < srcNum; i++)
                {
                    mcast_igmpv3_source_add_wrapper(grpAddr, *pSourceAddr, vid, portid,
                        pMcastVlan->grpMembershipIntv,
                        IGMPV3_FWD_CONFLICT);
                    pSourceAddr++;
                }
            }
        }
        else
        {
            MCASTDBG("Group  exist !");
            if( !PORT_IS_FILTER_EXCLUDE(group_head_entry->fmode, portid))
            {
                PORT_SET_FILTER_EXCLUDE(group_head_entry->fmode, portid);
                group_head_entry->mbrTimer[portid] = pMcastVlan->grpMembershipIntv;

                pEntry = groupHead;
                while(pEntry != NULL)
                {
                    if(IS_LOGIC_PORTMASK_PORTSET(pEntry->staticMbr, portid))
                    {
                        pEntry = pEntry->next_subgroup;
                        continue;
                    }

                    if(mcast_igmpv3_source_is_inPkt(pEntry, portid, pSourceAddr, srcNum) == FALSE)
                    {
                        if(pMcastVlan->fastleave_enable)
                        {
                            pEntry->mbrTimer[portid] = 0;
                            LOGIC_PORTMASK_CLEAR_PORT(pEntry->mbr, portid);

                            if(pEntry->lookupTableFlag == L2TABLE_WROTE)
                            {
                                osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
                                LOGIC_PORTMASK_SET_PORT(delPmsk, portid);
                                mcast_hw_mcst_mbr_remove(pEntry, &delPmsk);
                            }

                            if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
                            {
                                pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                            }
                        }
                        else
                        {
                            sipArray[numSip++] = pEntry->sip;
                            PORT_SET_SRC_QUERY(pEntry->v3.isGssQueryFlag, portid);
                        }
                    }
                    pEntry = pEntry->next_subgroup;
                }
                for(i =0; i < srcNum; i++)
                {
                    if(pMcastVlan->fastleave_enable)
                    {
                        groupEntry.sip = *pSourceAddr;
                        pEntry = mcast_group_get(&groupEntry);
                        if(pEntry != NULL)
                            if(mcast_igmpv3_source_is_inPkt(pEntry, portid, sipArray, numSip) == FALSE)
                            {
                                mcast_igmpv3_source_add_wrapper(group_head_entry->dip,*pSourceAddr,group_head_entry->vid, portid,
                                    pMcastVlan->grpMembershipIntv,
                                    IGMPV3_FWD_CONFLICT);
                            }
                    }
                    else
                        mcast_igmpv3_source_add_wrapper(group_head_entry->dip,*pSourceAddr,group_head_entry->vid, portid,
                        pMcastVlan->grpMembershipIntv,
                        IGMPV3_FWD_CONFLICT);
                    pSourceAddr++;
                }

                if(!pMcastVlan->fastleave_enable)
                {
                    mcast_querier_db_get(vid, ipType, &pQueryEntry);
                    if (pQueryEntry == NULL)
                    {
                        MCASTDBG("[IGMP] VLAN-%d doesn't have query DB entry!", vid);
                        IGMP_DB_SEM_UNLOCK();
                        return SYS_ERR_FAILED;
                    }

                    if(pQueryEntry->status == IGMP_QUERIER)
                    {
                        for(i = 0; i < numSip; i++)
                        {
                            groupEntry.sip = sipArray[i];
                            pEntry = mcast_group_get(&groupEntry);
                            if(pEntry != NULL)
                            {
                                pEntry->v3.gssQueryCnt[portid]++;
                                mcast_igmp_send_grp_src_specific_query(vid,grpAddr, &sipArray[i],1, portid);
                            }
                        }
                    }

                }
            }
            else
            {

                MCASTDBG("proc X+A, and A = GMI");

                group_head_entry->mbrTimer[portid] = pMcastVlan->grpMembershipIntv;
                osal_memset(sipArray,0,sizeof(sipArray));

                pEntry = groupHead;
                while(pEntry != NULL)
                {
                    if(IS_LOGIC_PORTMASK_PORTSET(pEntry->staticMbr, portid))
                    {
                        pEntry = pEntry->next_subgroup;
                        continue;
                    }

                    if(pEntry->v3.mbrFwdFlag[portid] == IGMPV3_FWD_CONFLICT)
                    {
                        if(mcast_igmpv3_source_is_inPkt(pEntry,portid, pSourceAddr, srcNum) == FALSE)
                        {
                            if(pMcastVlan->fastleave_enable)
                            {
                                pEntry->mbrTimer[portid] = 0;
                                LOGIC_PORTMASK_CLEAR_PORT(pEntry->mbr, portid);

                                if(pEntry->lookupTableFlag == L2TABLE_WROTE)
                                {
                                    osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
                                    LOGIC_PORTMASK_SET_PORT(delPmsk, portid);
                                    mcast_hw_mcst_mbr_remove(pEntry, &delPmsk);
                                }

                                if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
                                {
                                    pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                                }
                            }
                            else
                            {
                                sipArray[numSip++] = pEntry->sip;
                                PORT_SET_SRC_QUERY(pEntry->v3.isGssQueryFlag, portid);
                            }
                        }
                    }
                    pEntry = pEntry->next_subgroup;
                }

                for(i = 0; i < srcNum; i++)
                {
                    mcast_igmpv3_source_add_wrapper(grpAddr,*pSourceAddr, vid,portid,
                        pMcastVlan->grpMembershipIntv,
                        IGMPV3_FWD_CONFLICT);
                    pSourceAddr++;
                }

                if(!pMcastVlan->fastleave_enable)
                {
                    mcast_querier_db_get(vid,  ipType, &pQueryEntry);
                    if (pQueryEntry == NULL)
                    {
                        MCASTDBG("[IGMP]VLAN-%d doesn't have query DB entry!", vid);
                        IGMP_DB_SEM_UNLOCK();
                        return SYS_ERR_FAILED;
                    }

                    if(pQueryEntry->status == IGMP_QUERIER)
                    {
                        if(numSip > 0)
                        {
                            for(i = 0; i < numSip; i++)
                            {
                                groupEntry.sip = sipArray[i];
                                pEntry = mcast_group_get(&groupEntry);
                                if(pEntry != NULL)
                                {
                                    pEntry->v3.gssQueryCnt[portid]++;
                                    mcast_igmp_send_grp_src_specific_query(vid,grpAddr, &sipArray[i],1, portid);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_groupMbrPort_block_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    uint32 grpAddr = 0;
    uint16 srcNum = 0, i;
    uint16 sortedIdx;
    uint32  *pSourceAddr = NULL;
    uint32  sipArray[32];  
    uint16  numSip = 0;

    igmp_group_entry_t  *pEntry,  *groupHead = NULL, groupEntry;
    igmp_group_head_entry_t  *group_head_entry = NULL;

    sys_logic_portmask_t  delPmsk;
    igmp_querier_entry_t  *pQueryEntry = NULL;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    grpAddr = ntohl(pRcd->grec_mca);
    srcNum = ntohl(pRcd->grec_nsrcs);
    pSourceAddr =  pRcd->grec_src;

    if(srcNum == 0)
    {
        MCASTDBG("no pkt src in the group");
        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(mcast_build_groupEntryKey(vid, ipType, grpAddr, NULL, &groupEntry));

    IGMP_DB_SEM_LOCK();

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if(NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);

    mcast_igmp_group_head_get(grpAddr, vid,  &group_head_entry);

    if(group_head_entry == NULL)
    {
        MCASTDBG("the group not exist");
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
    }

    if( !(PORT_IS_FILTER_EXCLUDE(group_head_entry->fmode, portid)))
    {
        
        if (pMcastVlan->fastleave_enable)
        {
            if (groupHead->sip == 0)
            {
                if(groupHead->next_subgroup == NULL)
                    mcast_group_del(groupHead);
            }
            else                        
            {
                for(i = 0; i < srcNum; i++)
                {
                    groupEntry.sip = *pSourceAddr;
                    pEntry = mcast_group_get(&groupEntry);
                    if(pEntry != NULL)
                    {
                        if(IS_LOGIC_PORTMASK_PORTSET(pEntry->staticMbr, portid))
                            continue;

                        if(IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, portid))
                        {
                            pEntry->mbrTimer[portid] = 0;
                            LOGIC_PORTMASK_CLEAR_PORT(pEntry->mbr, portid);

                            if(pEntry->lookupTableFlag == L2TABLE_WROTE)
                            {
                                osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
                                LOGIC_PORTMASK_SET_PORT(delPmsk, portid);
                                mcast_hw_mcst_mbr_remove(pEntry, &delPmsk);
                            }

                            if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
                            {
                                pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                            }
                        }
                    }
                    pSourceAddr++;
                }
            }

        }
        else
        {
            mcast_querier_db_get(vid, ipType, &pQueryEntry);
            if (pQueryEntry == NULL)
            {
                MCASTDBG("[IGMP]VLAN-%d doesn't have query DB entry!", vid);
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }

            if (groupHead->sip == 0)     
            {
                if (IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, portid) && groupHead->gsQueryCnt[portid] == 0)
                {
                    if (pQueryEntry->status == IGMP_NON_QUERIER)
                    {
                        ;
                    }
                    else
                    {
                        groupHead->mbrTimer[portid] = pMcastVlan->grpSpecificQueryIntv;
                        groupHead->gsQueryCnt[portid]++;
                        
                        mcast_send_gs_query(groupHead, portid);
                    }
                }
            }
            else
            {
                osal_memset(&sipArray,0,sizeof(sipArray));

                for(i = 0; i < srcNum; i++)
                {
                    groupEntry.sip =  *pSourceAddr;
                    pEntry = mcast_group_get(&groupEntry);
                    if(pEntry != NULL)
                    {
                        if (IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, portid) && pEntry->v3.gssQueryCnt[portid] == 0)
                        {
                            if (pQueryEntry->status == IGMP_NON_QUERIER)
                            {
                                ;
                            }
                            else
                            {
                                pEntry->mbrTimer[portid] = pMcastVlan->grpSpecificQueryIntv;
                                sipArray[numSip++] = *pSourceAddr;
                                PORT_SET_SRC_QUERY(pEntry->v3.isGssQueryFlag, portid);
                            }
                        }
                    }
                    pSourceAddr++;
                }

                if(numSip > 0)
                {
                    for(i = 0; i < numSip; i++)
                    {
                        groupEntry.sip = sipArray[i];
                        pEntry = mcast_group_get(&groupEntry);
                        if(pEntry != NULL)
                        {
                            pEntry->v3.gssQueryCnt[portid]++;
                            mcast_igmp_send_grp_src_specific_query(vid,grpAddr, &sipArray[i],1, portid);
                            MCASTDBG("send gss query sip : ("IPADDR_PRINT") , dip : ("IPADDR_PRINT")", IPADDR_PRINT_ARG(sipArray[i]),
                                IPADDR_PRINT_ARG(grpAddr));
                        }
                    }
                }
            }
        }
    }
    else
    {

        osal_memset(sipArray, 0, sizeof(sipArray));

        for(i = 0; i < srcNum; i++)
        {
            groupEntry.sip = *pSourceAddr;
            pEntry = mcast_group_get(&groupEntry);
            if(pEntry != NULL )  
            {
                if(IS_LOGIC_PORTMASK_PORTSET(pEntry->staticMbr, portid))
                    continue;

                if(pEntry->v3.mbrFwdFlag[portid] == IGMPV3_FWD_CONFLICT)
                {
                    if(pMcastVlan->fastleave_enable)
                    {
                        pEntry->mbrTimer[portid] = 0;
                        LOGIC_PORTMASK_CLEAR_PORT(pEntry->mbr, portid);

                        if(pEntry->lookupTableFlag == L2TABLE_WROTE)
                        {
                            osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
                            LOGIC_PORTMASK_SET_PORT(delPmsk, portid);
                            mcast_hw_mcst_mbr_remove(pEntry, &delPmsk);
                        }

                        if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
                        {
                            pEntry->groupTimer = pMcastVlan->grpMembershipIntv;
                        }
                    }
                    else
                    {
                        sipArray[numSip++] = *pSourceAddr;
                        PORT_SET_SRC_QUERY(pEntry->v3.isGssQueryFlag, portid);
                    }
                }
                else if(pEntry->v3.mbrFwdFlag[portid] == IGMPV3_FWD_NORMAL)
                {
                    sipArray[numSip++] = *pSourceAddr;
                    PORT_SET_SRC_QUERY(pEntry->v3.isGssQueryFlag, portid);
                }
            }
            pSourceAddr++;
        }

        if(!pMcastVlan->fastleave_enable)
        {
            mcast_querier_db_get(vid, ipType, &pQueryEntry);
            if (pQueryEntry == NULL)
            {
                MCASTDBG("[IGMP]VLAN-%d doesn't have query DB entry!", vid);
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }

            if (pQueryEntry->status == IGMP_QUERIER)
            {
                if(numSip > 0)
                {
                    for(i = 0; i < numSip; i++)
                    {
                        groupEntry.sip = sipArray[i];
                        pEntry = mcast_group_get(&groupEntry);
                        if(pEntry != NULL)
                        {
                            pEntry->v3.gssQueryCnt[portid]++;
                            mcast_igmp_send_grp_src_specific_query(vid,grpAddr, &sipArray[i],1, portid);
                            MCASTDBG("send gss query sip : ("IPADDR_PRINT") , dip : ("IPADDR_PRINT")", IPADDR_PRINT_ARG(sipArray[i]),
                                IPADDR_PRINT_ARG(grpAddr));
                        }
                    }
                }
            }

        }
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
#else
 int32 mcast_igmpv3_group_mcstData_add_wrapper_dipsip(multicast_ipType_t ipType, uint16 vid, sys_nic_pkt_t *pBuf, mcast_groupAddr_t *pGroupDip,
    mcast_groupAddr_t * pSip, sys_logic_port_t port, uint8 isMvrGroup)
{
    return SYS_ERR_OK;
}
#endif

int32 mcast_igmpv3_basic_groupMbrPort_isIn_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    uint32 dip;
    uint16 numSrc = 0;
    mcast_groupAddr_t groupAddr;
    uint8  sendFlag = 0;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    dip = ntohl(pRcd->grec_mca);
    numSrc = ntohl(pRcd->grec_nsrcs);

    groupAddr.ipv4 = dip;

    mcast_groupMbrPort_add_wrapper(ipType, vid, NULL, &groupAddr, portid, &sendFlag, FALSE);

    return SYS_ERR_OK;
}

int32 mcast_igmpv3_basic_groupMbrPort_isEx_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    mcast_igmpv3_basic_groupMbrPort_isIn_proc(pRcd,vid,portid);
    return SYS_ERR_OK;
}
int32 mcast_igmpv3_basic_groupMbrPort_toEx_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    mcast_igmpv3_basic_groupMbrPort_isIn_proc(pRcd,vid,portid);
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_basic_groupMbrPort_toIn_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    uint32 dip;
    uint16 numSrc = 0;
    mcast_groupAddr_t groupAddr;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    igmp_lookup_mode_t lookup_mode;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    dip = ntohl(pRcd->grec_mca);
    numSrc = ntohl(pRcd->grec_nsrcs);

    groupAddr.ipv4 = dip;
    rsd_mcast_lookupMode_get(ipType, &lookup_mode);

    if(0 == numSrc)
    {
        if (IGMP_DIP_SIP_MODE == lookup_mode || IGMP_DIP_VID_MODE == lookup_mode)
        {
             mcast_groupMbrPort_del_wrapper_dipsip(ipType, vid,NULL,&groupAddr, portid, FALSE);
        }
        else if (IGMP_DMAC_VID_MODE == lookup_mode)
        {
             mcast_groupMbrPort_del_wrapper_dmacvid(ipType, vid,NULL,&groupAddr,portid, FALSE);
        }
    }
    else
    {
         mcast_igmpv3_basic_groupMbrPort_isIn_proc(pRcd,vid,portid);
    }
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_basic_groupMbrPort_block_proc(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
int32 mcast_mldv2_basic_groupMbrPort_isIn_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    uint16 numSrc = 0;
    mcast_groupAddr_t groupAddr;
    uint8   sendFlag = 0;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    numSrc = ntohl(pRcd->grec_nsrcs);

    osal_memcpy(groupAddr.ipv6.addr, pRcd->grec_ipv6Mca, IPV6_ADDR_LEN);

    mcast_groupMbrPort_add_wrapper(ipType, vid, NULL, &groupAddr, portid, &sendFlag, FALSE);

    return SYS_ERR_OK;
}

int32 mcast_mldv2_basic_groupMbrPort_isEx_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    mcast_mldv2_basic_groupMbrPort_isIn_proc(pRcd,vid,portid);
    return SYS_ERR_OK;
}
int32 mcast_mldv2_basic_groupMbrPort_toEx_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    mcast_mldv2_basic_groupMbrPort_isIn_proc(pRcd,vid,portid);
    return SYS_ERR_OK;
}

int32 mcast_mldv2_basic_groupMbrPort_toIn_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    uint16 numSrc = 0;
    mcast_groupAddr_t groupAddr;
    uint8 sendFlag = 0;
    igmp_lookup_mode_t lookup_mode;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    numSrc = ntohl(pRcd->grec_nsrcs);
    osal_memcpy(groupAddr.ipv6.addr, pRcd->grec_ipv6Mca, IPV6_ADDR_LEN);

    rsd_mcast_lookupMode_get(ipType, &lookup_mode);
    if(0 == numSrc)
    {
        if (IGMP_DMAC_VID_MODE == lookup_mode || IGMP_DIP_VID_MODE == lookup_mode)
        {
             mcast_groupMbrPort_del_wrapper_dmacvid(ipType, vid,NULL,&groupAddr,portid,FALSE);
        }
    }
    else
    {
        mcast_groupMbrPort_add_wrapper(ipType, vid, NULL, &groupAddr, portid, &sendFlag, FALSE);
    }

    return SYS_ERR_OK;
}

int32 mcast_mldv2_basic_groupMbrPort_block_proc(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_MVR

int32 mcast_mvr_group_remove_port(sys_logic_port_t lPort)
{
    uint32 i;
    igmp_group_entry_t *pGroup;
    int32 tmpCnt = 0;

    if (0 == mcast_mvr_group_num)
        return SYS_ERR_OK;

    mcast_group_rx_timer_stop();
    for (i = 0; i < mcast_group_num; i++)
    {
         
        pGroup = mcast_group_get_with_index(i);
        if ( NULL == pGroup)
            continue;

        if (MULTICAST_TYPE_IPV4 != pGroup->ipType)
            continue;

        if (IGMP_GROUP_ENTRY_STATIC == pGroup->form)
            continue;

        if (is_mvr_group_ret2(pGroup))
        {
            if (IS_LOGIC_PORTMASK_PORTSET(pGroup->mbr, lPort))
            {
                tmpCnt = mcast_group_num;
                mcast_groupPort_remove(pGroup, lPort);
                if (mcast_group_num != tmpCnt)
                    i--;
            }
        }
    }
    mcast_group_rx_timer_start();

    return SYS_ERR_OK;
}

int32 mcast_mvr_staticGroup_portType_chk(sys_logic_port_t lPort)
{
    uint32 i;
    igmp_group_entry_t *pGroup;
    sys_vid_t mvrVid = 0;

    mcast_mvr_vlanId_get(MULTICAST_TYPE_IPV4, &mvrVid);

    for (i = 0; i < mcast_group_num; i++)
    {
        
        pGroup = mcast_group_get_with_index(i);
        if ( NULL == pGroup)
            continue;

        if (MULTICAST_TYPE_IPV4 != pGroup->ipType)
            continue;

        if (IGMP_GROUP_ENTRY_STATIC != pGroup->form)
            continue;

         if (is_mvr_group_ret2(pGroup))
         {
             if (IS_LOGIC_PORTMASK_PORTSET(pGroup->staticInMbr, lPort))
                return SYS_ERR_MVR_STATIC_GROUP_PORT_TYPE;
         }
    }

    return SYS_ERR_OK;
}

int32 mcast_mvr_staticGroup_dynMode_chk(void)
{
    uint32 i;
    igmp_group_entry_t *pGroup;
    sys_vid_t mvrVid = 0;
    sys_logic_port_t lport;
    mvr_portType_t pType;

    mcast_mvr_vlanId_get(MULTICAST_TYPE_IPV4, &mvrVid);

    for (i = 0; i < mcast_group_num; i++)
    {
        
        pGroup = mcast_group_get_with_index(i);
        if ( NULL == pGroup)
            continue;

        if (MULTICAST_TYPE_IPV4 != pGroup->ipType)
            continue;

        if (IGMP_GROUP_ENTRY_STATIC != pGroup->form)
            continue;

        if (is_mvr_group_ret2(pGroup))
        {
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(lport, pGroup->staticInMbr)
            {
                mcast_mvr_portType_get(pGroup->ipType, lport, &pType);
                if (MVR_TYPE_SOURCE == pType)
                    return SYS_ERR_MVR_STATIC_GROUP_PORT_TYPE;
            }
        }
    }
    return SYS_ERR_OK;
}

int32 mcast_mvr_groupSet_chk(multicast_ipType_t ipType, mcast_groupAddr_t *pStar, mcast_groupAddr_t *pEnd)
{
    uint32 i;
    igmp_group_entry_t *pGroup;
    sys_vid_t mvrVid = 0;

    SYS_PARAM_CHK((NULL == pStar), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEnd), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    mcast_mvr_vlanId_get(ipType, &mvrVid);

    for (i = 0; i < mcast_group_num; i++)
    {
        
        pGroup = mcast_group_get_with_index(i);
        if ( NULL == pGroup)
            continue;

        if ( pGroup->ipType == ipType && mvrVid == pGroup->vid)
        {
            if (IGMP_GROUP_ENTRY_STATIC == pGroup->form)
            {
                if (MULTICAST_TYPE_IPV4 == ipType)
                {
                    if (pGroup->dip < pStar->ipv4 || pGroup->dip > pEnd->ipv4)
                    {
                        return SYS_ERR_MVR_STATIC_GROUP_CONFLICT;
                    }
                }
                else
                {
                    if ( (osal_memcmp(pGroup->dipv6.addr, pStar->ipv6.addr,  IPV6_ADDR_LEN) < 0) ||
                          (osal_memcmp(pGroup->dipv6.addr, pEnd->ipv6.addr,  IPV6_ADDR_LEN) > 0))
                     {
                        return SYS_ERR_MVR_STATIC_GROUP_CONFLICT;
                     }
                }
            }
        }
    }

    return SYS_ERR_OK;
}

uint8 is_mvr_group_ret1(multicast_ipType_t ipType, sys_vid_t vlanid, mcast_groupAddr_t *pGroupAddr, uint8 isChkVlan)
{
    sys_vid_t mvrVlan;

    if (NULL == pGroupAddr)
        return FALSE;

    if (ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END)
        return FALSE;

    if (isChkVlan)
    {
        if (IS_VID_INVALID(vlanid))
            return FALSE;
    }

    mcast_mvr_vlanId_get(ipType, &mvrVlan);
    if (isChkVlan)
    {
        if (mvrVlan != vlanid)
            return FALSE;
    }

    if (!is_mcast_mvr_enable(ipType))
        return FALSE;

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
         if ((pGroupAddr->ipv4 < igmp_cfg.mvrConfig.groupIp_start.ipv4) || (pGroupAddr->ipv4 > igmp_cfg.mvrConfig.groupIp_end.ipv4))
            return FALSE;
    }
    else
    {
        if ( (osal_memcmp(pGroupAddr->ipv6.addr, mld_cfg.mvrConfig.groupIp_start.ipv6.addr,  IPV6_ADDR_LEN) < 0) ||
              (osal_memcmp(pGroupAddr->ipv6.addr, mld_cfg.mvrConfig.groupIp_end.ipv6.addr,  IPV6_ADDR_LEN) > 0))
              return FALSE;
    }

    return TRUE;
}
uint8 is_mvr_group_ret2(igmp_group_entry_t *pGroup)
{
    sys_vid_t mvrVlan;

    if (NULL == pGroup)
        return FALSE;

    if (pGroup->ipType < MULTICAST_TYPE_IPV4 || pGroup->ipType > MULTICAST_TYPE_END)
        return FALSE;

    if (!is_mcast_mvr_enable(pGroup->ipType))
        return FALSE;

    mcast_mvr_vlanId_get(pGroup->ipType, &mvrVlan);
    if (mvrVlan != pGroup->vid)
            return FALSE;

    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
    {
         if ((pGroup->dip < igmp_cfg.mvrConfig.groupIp_start.ipv4) || (pGroup->dip > igmp_cfg.mvrConfig.groupIp_end.ipv4))
            return FALSE;
    }
    else
    {
        if ( (osal_memcmp(pGroup->dipv6.addr, mld_cfg.mvrConfig.groupIp_start.ipv6.addr,  IPV6_ADDR_LEN) < 0) ||
              (osal_memcmp(pGroup->dipv6.addr, mld_cfg.mvrConfig.groupIp_end.ipv6.addr,  IPV6_ADDR_LEN) > 0))
              return FALSE;
    }

    return TRUE;
}

uint8 is_mcast_mvr_maxGroup(multicast_ipType_t ipType)
{
    if (ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END)
        return FALSE;

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        if (DISABLED == igmp_cfg.mvrConfig.isMaxGroup)
            return FALSE;
    }
    else
    {
        if (DISABLED == mld_cfg.mvrConfig.isMaxGroup)
            return FALSE;
    }

    return TRUE;
}

uint8 is_mcast_mvr_portFilter(multicast_ipType_t ipType)
{
    if (ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END)
        return FALSE;

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        if (DISABLED == igmp_cfg.mvrConfig.isFilter)
            return FALSE;
    }
    else
    {
        if (DISABLED == mld_cfg.mvrConfig.isFilter)
            return FALSE;
    }

    return TRUE;
}

uint8 is_mcast_mvr_forwardAll(multicast_ipType_t ipType)
{
    if (ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END)
        return FALSE;

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        if (DISABLED == igmp_cfg.mvrConfig.isForward)
            return FALSE;
    }
    else
    {
        if (DISABLED == mld_cfg.mvrConfig.isForward)
            return FALSE;
    }

    return TRUE;
}

uint8 mcast_mvr_fwdAll_ret(igmp_group_entry_t *pGroup)
{
    uint8  isFwdAll = TRUE;

    if (NULL == pGroup)
        return isFwdAll;

    if (!is_mcast_mvr_forwardAll(pGroup->ipType))
    {
        if (is_mvr_group_ret2(pGroup))
            isFwdAll = FALSE;
    }

    return isFwdAll;
}
#endif

int32 mcast_static_group_save(sys_vlanmask_t vlanMsk, multicast_ipType_t ipType)
{
    uint32 i;
    igmp_group_entry_t  *pGroup_entry;

    osal_memset(&groupSave,0,sizeof(igmp_static_group_t));

    if(mcast_group_num == 0)
        return SYS_ERR_OK;

    groupSave.pGroup = (igmp_group_entry_t *)osal_alloc(sizeof(igmp_group_entry_t) * mcast_group_num);

    if(groupSave.pGroup == NULL)
    {
        MCASTDBG("Alloc memory failure !");
        return SYS_ERR_FAILED;
    }

    for (i = 0; i < mcast_group_num; i++)
        osal_memset(&groupSave.pGroup[i], 0, sizeof(igmp_group_entry_t));

    for(i = 0; i < mcast_group_num; i++)
    {
        pGroup_entry = mcast_group_get_with_index(i);
        if (pGroup_entry)
        {
            if (IGMP_GROUP_ENTRY_STATIC != pGroup_entry->form)
                continue;

            if (MULTICAST_TYPE_END != ipType)
            {
                if (ipType != pGroup_entry->ipType)
                    continue;
            }

            if (!VLANMASK_IS_VIDSET(vlanMsk, pGroup_entry->vid))
                continue;

            if(pGroup_entry->vid != 0)
            {
                groupSave.pGroup[groupSave.groupCnt].vid = pGroup_entry->vid;
                groupSave.pGroup[groupSave.groupCnt].dip = pGroup_entry->dip;
                groupSave.pGroup[groupSave.groupCnt].form = pGroup_entry->form;
                groupSave.pGroup[groupSave.groupCnt].ipType = pGroup_entry->ipType;
                osal_memcpy(groupSave.pGroup[groupSave.groupCnt].mac, pGroup_entry->mac, CAPA_MAC_BYTE_NUM);
                if (MULTICAST_TYPE_IPV4 != pGroup_entry->ipType)
                {
                    osal_memcpy(groupSave.pGroup[groupSave.groupCnt].dipv6.addr, pGroup_entry->dipv6.addr, IPV6_ADDR_LEN);
                }
                osal_memcpy(&(groupSave.pGroup[groupSave.groupCnt].staticMbr), &pGroup_entry->staticMbr, sizeof(sys_logic_portmask_t));
                osal_memcpy(&(groupSave.pGroup[groupSave.groupCnt].staticInMbr), &pGroup_entry->staticInMbr, sizeof(sys_logic_portmask_t));
                groupSave.groupCnt++;
            }
        }
    }

    return SYS_ERR_OK;
}

int32 mcast_static_group_restore(void)
{
    uint32 i;
    igmp_group_entry_t  group_entry;

    for(i = 0; i < groupSave.groupCnt; i++)
    {
        osal_memset(&group_entry, 0, sizeof(igmp_group_entry_t));
        osal_memcpy(&group_entry, &(groupSave.pGroup[i]), sizeof(igmp_group_entry_t));

        osal_memcpy(&group_entry.mbr, &(groupSave.pGroup[i].staticInMbr), sizeof(sys_logic_portmask_t));
        if (is_mcast_group(&group_entry))
        {
            mcast_basic_static_groupEntry_set(&group_entry, FILTER_MODE_INCLUDE,TRUE);
        }
    }

    if (NULL != groupSave.pGroup)
    {
        osal_free(groupSave.pGroup);
        groupSave.pGroup = NULL;
    }

    return SYS_ERR_OK;
}
int32 mcast_filter_check(multicast_ipType_t ipType, sys_logic_port_t port,  mcast_groupAddr_t  *pGroupAddr)
{
   #ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
    sys_igmp_profile_t profile;
    sys_igmp_profileId_t proIdx;
    int32 ret1 = 0, ret2 = 0;

    SYS_PARAM_CHK((NULL == pGroupAddr), SYS_ERR_NULL_POINTER);

    osal_memset(&profile, 0,sizeof(sys_igmp_profile_t));

    #ifdef CONFIG_SYS_PROTO_MVR
    if (!is_mcast_mvr_portFilter(ipType))
    {
        
        if (is_mvr_group_ret1(ipType, 0, pGroupAddr, FALSE))
            return SYS_ERR_OK;
    }
    #endif

    SYS_ERR_CHK(mcast_port_filter_get(ipType, port, &proIdx));
    if (INVALID_PROFILE_ID ==  proIdx )
        return SYS_ERR_OK;

    mcast_profile_get(ipType, proIdx, &profile);
    if (0 == profile.valid)
    {
        return SYS_ERR_OK;
    }

    if (ipType != profile.ipType)
    {
        if (MULTICAST_TYPE_IPV4 == ipType)
            MCASTDBG("The ip type is not match!");
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
            MCASTDBG("The ip type is not match!");
#endif
        return SYS_ERR_FAILED;
    }

    if (MULTICAST_TYPE_IPV4 != ipType)
    {
        ret1 = osal_memcmp(pGroupAddr->ipv6.addr, profile.ipv6Low, CAPA_IPV6_BYTE_NUM);
        ret2 = osal_memcmp(pGroupAddr->ipv6.addr, profile.ipv6High, CAPA_IPV6_BYTE_NUM);
    }

    if (MCAST_PROFILE_ACT_PERMIT == profile.act)
    {
        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            if (pGroupAddr->ipv4 <= profile.highIp && pGroupAddr->ipv4 >= profile.lowIp)
                return SYS_ERR_OK;
        }
        else
        {
            if (ret2 <= 0 && ret1 >= 0)
                return SYS_ERR_OK;
        }
    }
    else if (MCAST_PROFILE_ACT_DENY == profile.act)
    {
        if (MULTICAST_TYPE_IPV4 == ipType)
        {
            if (pGroupAddr->ipv4 < profile.lowIp || pGroupAddr->ipv4 > profile.highIp)
                return SYS_ERR_OK;
        }
        else
        {
            if (ret1 < 0 || ret2 > 0)
                return SYS_ERR_OK;
        }
    }

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        MCASTDBG("The group ip filter by port filter!");
    }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else
    {
        MCASTDBG("The group ip filter by port filter!");
    }
#endif

  #else
    return SYS_ERR_OK;
  #endif  

    return SYS_ERR_FAILED;
}
uint8 is_mcast_group(igmp_group_entry_t *pGroup)
{
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_FAILED);

    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
    {
        if (IGMP_IN_MULTICAST(pGroup->dip))
            return TRUE;
    }
    else
    {
        if (MLD_IN_MULTICAST(pGroup->dipv6.addr))
            return TRUE;
    }

    return FALSE;
}
int32 mcast_portStat_inc(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lport, mcast_statType_t  statType)
{
    sys_vlan_adminEntry_t vlanEntry;
    sys_port_linkStatus_t linkStat;

    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(lport), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((statType >= MCAST_STAT_END), SYS_ERR_INPUT );
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    if (IS_LP_TRKMBR(lport))
    {
        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(rsd_port_linkStatus_get(lport, &linkStat));
    if (PORT_LINKDOWN == linkStat)
        return SYS_ERR_OK;

    SYS_ERR_CHK(rsd_vlan_adminEntry_get(vid, &vlanEntry));
    if (!IS_LOGIC_PORTMASK_PORTSET(vlanEntry.lpmMember,lport))
        return SYS_ERR_OK;

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        igmp_stats[lport].igmpStatCnt[statType]++;
    }
    else
    {
        mld_stats[lport].mldStatCnt[statType]++;
    }

    return SYS_ERR_OK;
}

int32 mcast_portMskStat_inc(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_portmask_t portMsk, mcast_statType_t  statType)
{
    sys_logic_port_t lport;

    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK((statType >= MCAST_STAT_END), SYS_ERR_INPUT );
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lport, portMsk)
    {
       mcast_portStat_inc(ipType, vid,lport, statType);
    }

    return SYS_ERR_OK;
}

int32 mcast_blockPortMskStat_inc(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_portmask_t blockPortMsk, mcast_statType_t  statType)
{
    sys_logic_port_t lport;

    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK((statType >= MCAST_STAT_END), SYS_ERR_INPUT );
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    FOR_EACH_LOGIC_PORT(lport)
    {
       if (!IS_LOGIC_PORTMASK_PORTSET(blockPortMsk, lport))
       {
            mcast_portStat_inc(ipType, vid, lport, statType);
       }
    }
    return SYS_ERR_OK;
}

int32 l2_mcast_group_db_init(void)
{
    gl2Mcast_vlan_db = (l2mcast_vlan_db_t*)osal_alloc(sizeof(l2mcast_vlan_db_t)*(CAPA_L2_MCAST_GROUP_NUM+1));
    if(NULL == gl2Mcast_vlan_db)
    {
        MCASTDBG("Failed to alloc space for multicast table!");
        return SYS_ERR_FAILED;
    }

    osal_memset(gl2Mcast_vlan_db, 0, sizeof(l2mcast_vlan_db_t)*(CAPA_L2_MCAST_GROUP_NUM+1));

    return SYS_ERR_OK;
}
int vlan_used_by_l2mcast(uint32 vlanId)
{
    int i;

    for(i = 0; i < L2_MCAST_USED_CNT; i++)
    {
        
        if(gl2Mcast_vlan_db[i].vlan_id > vlanId)
        {
            return FALSE;
        }
        if(gl2Mcast_vlan_db[i].vlan_id == vlanId)
        {
            return TRUE;
        }
    }
    return FALSE;
}
int l2_mcast_is_exist_by_mac_vlan(uint32 vlanId, uint8 *mac)
{
    uint32 found;
    uint32 slot = 0xeeee;

    l2_mcast_lookup(vlanId, mac, &found, &slot);

    if(0xeeee == slot)
    {
    	MCASTDBG("");
        return FALSE;
    }
    else
    {
		if(found == FALSE)
			return FALSE;
		else
			return TRUE;
    }

    return FALSE;
}
int l2_mcast_entry_used_cnt(void)
{
    return L2_MCAST_USED_CNT;
}
int l2_mcast_entry_by_used_cnt(uint32 index, l2mcast_vlan_db_t  *mcast)
{
	MCASTDBG("L2_MCAST_USED_CNT:%d", L2_MCAST_USED_CNT);
	if(index < L2_MCAST_USED_CNT)
	{
		if(gl2Mcast_vlan_db[index].vlan_id && gl2Mcast_vlan_db[index].rowStatus)
		{
			MCASTDBG("vlan_id:%d", gl2Mcast_vlan_db[index].vlan_id);
			osal_memcpy(mcast, &gl2Mcast_vlan_db[index], sizeof(l2mcast_vlan_db_t));
			return SYS_ERR_OK;
		}
	}
    return SYS_ERR_FAILED;
}
int32 l2_mcast_lookup(uint32 vlanId, uint8 *mac, uint32 *found, uint32 *freeSlot)
{
    int    i;
    int    result;

    if((NULL == mac) || (NULL == found) || (NULL == freeSlot))
    {
        return SYS_ERR_FAILED;
    }
    
    *found = FALSE;
    *freeSlot = 0xeeee;
    
    for(i = 0; i < L2_MCAST_USED_CNT; i++)
    {
        if(vlanId < gl2Mcast_vlan_db[i].vlan_id)
        {
            break;
        }
        
        if(gl2Mcast_vlan_db[i].vlan_id == vlanId)
        {
            result = osal_memcmp(gl2Mcast_vlan_db[i].mcast_mac, mac, 6);
            if(0 == result)
            {
                *found = TRUE;
                break;
            }
            if(result > 0)
            {
                break;
            }
        }
    }
    
    if(i < CAPA_L2_MCAST_GROUP_NUM)
    {
        *freeSlot = i; 
    }
    
    return SYS_ERR_OK;
}
static int  l2_mcast_entry_alloc(int freeSlot)
{
    int   i = 0;

    if((freeSlot >= CAPA_L2_MCAST_GROUP_NUM) ||(L2_MCAST_USED_CNT >= CAPA_L2_MCAST_GROUP_NUM))
    {
        return SYS_ERR_FAILED;
    }
    if(freeSlot < L2_MCAST_USED_CNT)
    {
        for(i = L2_MCAST_USED_CNT; i > freeSlot; i--)
        {
            memcpy(&gl2Mcast_vlan_db[i], &gl2Mcast_vlan_db[i-1], sizeof(l2mcast_vlan_db_t));
        }
    }
    L2_MCAST_USED_CNT++;
    memset(&gl2Mcast_vlan_db[freeSlot], 0, sizeof(l2mcast_vlan_db_t));
    return freeSlot;
}
static void l2_mcast_entry_delete(int slot)
{
    int    i = 0;

    for(i = slot; i < (L2_MCAST_USED_CNT-1); i++)
    {
        osal_memcpy(&gl2Mcast_vlan_db[i], &gl2Mcast_vlan_db[i+1], sizeof(l2mcast_vlan_db_t));
    }
    if(L2_MCAST_USED_CNT > 0)
    {
        L2_MCAST_USED_CNT--;
    }
    osal_memset(&gl2Mcast_vlan_db[L2_MCAST_USED_CNT], 0, sizeof(l2mcast_vlan_db_t));
}
uint32 l2_mcast_create(l2_mcast_group_t   * mcast)
{
    uint32 found;
    uint32 slot = 0xeeee;
    uint32 rc = SYS_ERR_OK;
	sys_l2McastEntry_t l2McstEntry;

    l2_mcast_lookup(mcast->vlanId, mcast->mac, &found, &slot);

    if(0xeeee == slot)
    {
    	MCASTDBG("");
        rc = SYS_ERR_FAILED;
    }
    else
    {
        if(found == FALSE)
        {
            if(SYS_ERR_FAILED == l2_mcast_entry_alloc(slot))
            {
            	MCASTDBG("");
                rc = SYS_ERR_FAILED;
            }
            else
            {
				
				osal_memset(&l2McstEntry, 0, sizeof(sys_l2McastEntry_t));
				l2McstEntry.vid = mcast->vlanId;
				osal_memcpy(l2McstEntry.mac, mcast->mac,CAPA_MAC_BYTE_NUM);
				LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, mcast->staticMbr);
				rc = rsd_l2McastEntry_add(&l2McstEntry);
			    if (SYS_ERR_L2_MAC_FULL == rc)
			    {
			        
			        osal_printf("\r\n entry full!!!\n");
			    }
                if (SYS_ERR_OK != rc)
                {
                	MCASTDBG("");
                    l2_mcast_entry_delete(slot);
                }
            }
        }        
        
        if(SYS_ERR_OK == rc)
        {
            gl2Mcast_vlan_db[slot].flag = 1;
            gl2Mcast_vlan_db[slot].rowStatus = 1;
            gl2Mcast_vlan_db[slot].vlan_id = mcast->vlanId;
            gl2Mcast_vlan_db[slot].staticMbr = mcast->staticMbr;  
            memcpy(gl2Mcast_vlan_db[slot].mcast_mac, mcast->mac, 6);
        }
    }
    
    return rc;
}
uint32 l2_mcast_delete(l2_mcast_group_t  *mcast)
{
    uint32   slot = 0xeeee;
    uint32   ulFound =FALSE;
    uint32    rc = SYS_ERR_OK;
    sys_l2McastEntry_t l2McstEntryOld;

    l2_mcast_lookup(mcast->vlanId, mcast->mac, &ulFound, &slot);
    if(ulFound == FALSE)
    {
        return SYS_ERR_OK;
    }

    {
        
        osal_memset(&l2McstEntryOld, 0, sizeof(sys_l2McastEntry_t));
        l2McstEntryOld.vid = mcast->vlanId;
        memcpy(l2McstEntryOld.mac, mcast->mac, 6);
        rc = rsd_l2McastEntry_get(&l2McstEntryOld);
        if (SYS_ERR_OK != rc)
        {
			
			MCASTDBG("");
        }
        MCASTDBG("");
        rc = rsd_l2McastEntry_del(&l2McstEntryOld);
        if(SYS_ERR_OK == rc)
        {
            l2_mcast_entry_delete(slot);
        }
    }
    return rc;
}
uint32 l2_mcast_port_add(l2_mcast_group_t  *mcast)
{
    sys_logic_portmask_t     temp1;
    uint32     ulVlanId;
    uint32     freeSlot = 0xeeee;
    uint32     ulFound = FALSE;
    uint32      rc = SYS_ERR_OK;
    sys_l2McastEntry_t l2McstEntry;
    sys_l2McastEntry_t l2McstEntryOld;

    ulVlanId = mcast->vlanId;
    ulFound = FALSE;

    l2_mcast_lookup(mcast->vlanId, mcast->mac, &ulFound, &freeSlot);
    if (ulFound == FALSE)
    {
    	MCASTDBG("");
        
        if ((0xeeee == freeSlot) || (SYS_ERR_FAILED == l2_mcast_entry_alloc(freeSlot)))
        {
        	MCASTDBG("");
            return SYS_ERR_FAILED;
        }
        else
        {
        	MCASTDBG("");
            gl2Mcast_vlan_db[freeSlot].flag = 1;
            gl2Mcast_vlan_db[freeSlot].rowStatus = 1;
            gl2Mcast_vlan_db[freeSlot].vlan_id = mcast->vlanId;
            memcpy(gl2Mcast_vlan_db[freeSlot].mcast_mac, mcast->mac, 6);
        }
    }
    else
    {
        temp1 = gl2Mcast_vlan_db[freeSlot].staticMbr;
        osal_memset(&l2McstEntry, 0, sizeof(sys_l2McastEntry_t));
        osal_memset(&l2McstEntryOld, 0, sizeof(sys_l2McastEntry_t));
        l2McstEntryOld.vid = mcast->vlanId;
        memcpy(l2McstEntryOld.mac, mcast->mac, 6);
        rc = rsd_l2McastEntry_get(&l2McstEntryOld);
        if (SYS_ERR_OK != rc)
        {
			
			MCASTDBG("");
        }
        LOGIC_PORTMASK_OR(l2McstEntryOld.portmask, l2McstEntryOld.portmask, mcast->staticMbr);
        rc = rsd_l2McastEntry_set(&l2McstEntryOld);
	    if (SYS_ERR_OK != rc)
	    {
	        
	        MCASTDBG("");
	    }
	    gl2Mcast_vlan_db[freeSlot].staticMbr = l2McstEntryOld.portmask;  
    }
    return rc;
}
uint32 l2_mcast_port_delete(l2_mcast_group_t  *mcast)
{
    sys_logic_portmask_t     temp1;
    uint32     ulVlanId;
    uint32     freeSlot = 0xeeee;
    uint32     ulFound = FALSE;
    uint32      rc = SYS_ERR_OK;
    sys_l2McastEntry_t l2McstEntry;
    sys_l2McastEntry_t l2McstEntryOld;

    ulVlanId = mcast->vlanId;
    ulFound = FALSE;

    l2_mcast_lookup(mcast->vlanId, mcast->mac, &ulFound, &freeSlot);
    if (ulFound == FALSE)
    {
        
        if ((0xeeee == freeSlot) || (SYS_ERR_FAILED == l2_mcast_entry_alloc(freeSlot)))
        {
            return SYS_ERR_FAILED;
        }
        else
        {
            gl2Mcast_vlan_db[freeSlot].flag = 1;
            gl2Mcast_vlan_db[freeSlot].rowStatus = 1;
            gl2Mcast_vlan_db[freeSlot].vlan_id = mcast->vlanId;
            memcpy(gl2Mcast_vlan_db[freeSlot].mcast_mac, mcast->mac, 6);
        }
    }
    else
    {
        temp1 = gl2Mcast_vlan_db[freeSlot].staticMbr;
        osal_memset(&l2McstEntry, 0, sizeof(sys_l2McastEntry_t));
        osal_memset(&l2McstEntryOld, 0, sizeof(sys_l2McastEntry_t));
        l2McstEntryOld.vid = mcast->vlanId;
        memcpy(l2McstEntryOld.mac, mcast->mac, 6);
        rc = rsd_l2McastEntry_get(&l2McstEntryOld);
        if (SYS_ERR_OK != rc)
        {
			
        }
        LOGIC_PORTMASK_ANDNOT(l2McstEntryOld.portmask, l2McstEntryOld.portmask, mcast->staticMbr);
        rc = rsd_l2McastEntry_set(&l2McstEntryOld);
	    if (SYS_ERR_OK != rc)
	    {
	        
	    }
	    gl2Mcast_vlan_db[freeSlot].staticMbr = l2McstEntryOld.portmask;  
    }
    return rc;
}
