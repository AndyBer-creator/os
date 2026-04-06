#include <rsd/rsd_ipmcast.h>
#include <rsd/rsd_mcast.h>
#include <rsd/rsd_l2.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_port.h>
#include <igmp/proto_igmp.h>
#include <igmp/proto_igmp_group_db.h>
#include <igmp/proto_igmp_querier.h>
#include <igmp/proto_igmp_rx.h>
#include <igmp/proto_igmp_util.h>
static rsd_nic_handler_t igmpHandler =
{
    .priority       = RSD_NIC_HANDLER_PRI_IGMP,
    .dmac           = { 0x01,0x00,0x5e,0x00,0x00,0x00 },
    .dmac_caremask  = { 0xff,0xff,0xff,0x00,0x00,0x00 },
    .ethertype      = 0x0,
    .proto          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_TRAP,
    .rx_cbf         = mcast_igmp_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_TRUNK,
    .chk_cbf        = mcast_igmp_preCheck,
    .pName          = "IGMP RX Thread",
    .pCookie        = NULL,
};

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
static rsd_nic_handler_t mldHandler =
{
    .priority       = RSD_NIC_HANDLER_PRI_MLD,
    .dmac           = { 0x33,0x33,0x00,0x00,0x00,0x00 },
    .dmac_caremask  = { 0xff,0xff,0x00,0x00,0x00,0x00 },
    .ethertype      = 0x86dd,
    .proto          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_TRAP,
    .rx_cbf         = mcast_mld_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_TRUNK,
    .chk_cbf    =    mcast_mld_preCheck,
    .pName          = "MLD RX Thread",
    .pCookie        = NULL,
};
#endif

static int32 _mcast_hwEntryByVid_del(sys_vlanmask_t *pVlanMask, multicast_ipType_t ipType, mcast_group_type_t type, mcast_app_type_t appType);
static int32 _mcast_vlan_handler(uint32 event, char* pData);
static int32 _mcast_group_timer_dipsip(void);
static int32 _mcast_group_timer_dmacvid(void);
extern void mcast_igmp_snooping_stats_init(int32 boot);
extern igmp_router_db_t router_db[MULTICAST_TYPE_END];
extern uint8            igmp_trapDataPkt_lock;
extern uint8            mcast_group_full;
extern int32            igmp_group_num;
extern int32            mcast_group_num;
extern int32            mld_group_num;

extern int32            igmp_staticMac_num;
extern int32            mcast_staticMac_num;
extern int32            mld_staticMac_num;

extern int32            mcast_mvr_group_num;
extern int32            group_sortedAry_entry_num;
extern uint32           igmp_hw_entry_num;
extern igmp_group_entry_t **group_sortedArray;
extern igmp_group_entry_t *group_db;
extern igmp_group_head_entry_t *groupHead_db;
extern igmp_static_group_t  groupSave;
extern sys_igmp_profile_t  profile_db[MULTICAST_TYPE_END][CAPA_MCAST_PROFILE_NUM];
extern sys_igmp_profileId_t port_profileId[MULTICAST_TYPE_END][SYS_LOGICPORT_NUM_MAX];

extern mcast_vlan_db_t    vlan_db[MULTICAST_TYPE_END];
extern sys_vlanmask_t mcastVlanCfgMsk[MULTICAST_TYPE_END];

volatile int32   igmp_thread_status = 0;
osal_mutex_t            igmp_sem_pkt;
osal_mutex_t            igmp_sem_db;
igmp_cfg_t              igmp_cfg;
igmp_stats_t            igmp_stats[SYS_LOGICPORT_NUM_MAX];
sys_mac_t               sys_mac; 
sys_enable_t            igmp_packet_stop = DISABLED;
sys_enable_t            mcast_timer_stop = DISABLED;
sys_enable_t            mcast_groupAdd_stop = DISABLED;
igmp_unknMcastAction_t                  gUnknown_mcast_igmp_action;
igmp_unknMcastAction_t                  gUnknown_mcast_mld_action;
uint32                  gMcast_filteringEnable;
uint8                   gNetworkEnabled = FALSE;

mld_cfg_t                    mld_cfg;
mld_stats_t                 mld_stats[SYS_LOGICPORT_NUM_MAX];

sys_mcast_event_t     mcastVlanEvent[SYS_VLAN_NUM_MAX];
uint32                        inEvent = 0;
uint32                        outEvent = 0;

const char *gMcastType[] =
{
    "IGMP snooping",
    "MLD snooping"
};

void mcast_igmp_group_head_timer(void);
sys_igmp_func_t         igmp_lookup_mode_func[] =
{

        {
            .igmp_group_timer = _mcast_group_timer_dmacvid,
            .igmp_groupMbrPort_del_wrapper = mcast_groupMbrPort_del_wrapper_dmacvid,
            .igmp_group_mcstData_add_wrapper = mcast_group_mcstData_add_wrapper_dmacvid,
            .igmpv3_group_mcstData_add_wrapper = mcast_group_mcstData_add_wrapper_dmacvid,
        },

        {
            .igmp_group_timer = _mcast_group_timer_dipsip,
            .igmp_groupMbrPort_del_wrapper = mcast_groupMbrPort_del_wrapper_dipsip,
            .igmp_group_mcstData_add_wrapper = mcast_igmp_group_mcstData_add_wrapper_dipsip,
            .igmpv3_group_mcstData_add_wrapper = mcast_igmpv3_group_mcstData_add_wrapper_dipsip,
        },

        {
            .igmp_group_timer = _mcast_group_timer_dipsip,
            .igmp_groupMbrPort_del_wrapper = mcast_groupMbrPort_del_wrapper_dipsip,
            .igmp_group_mcstData_add_wrapper = mcast_igmp_group_mcstData_add_wrapper_dipsip,
            .igmpv3_group_mcstData_add_wrapper = mcast_igmpv3_group_mcstData_add_wrapper_dipsip,
        },

        {
            .igmp_group_timer = _mcast_group_timer_dmacvid,
            .igmp_groupMbrPort_del_wrapper = mcast_groupMbrPort_del_wrapper_dmacvid,
            .igmp_group_mcstData_add_wrapper = mcast_group_mcstData_add_wrapper_dmacvid,
            .igmpv3_group_mcstData_add_wrapper = mcast_group_mcstData_add_wrapper_dmacvid,
        },

        {
            .igmp_group_timer = _mcast_group_timer_dipsip,
            .igmp_groupMbrPort_del_wrapper = mcast_groupMbrPort_del_wrapper_dipsip,
            .igmp_group_mcstData_add_wrapper = mcast_igmp_group_mcstData_add_wrapper_dipsip,
            .igmpv3_group_mcstData_add_wrapper = mcast_igmpv3_group_mcstData_add_wrapper_dipsip,
        },

        {
            .igmp_group_timer = _mcast_group_timer_dipsip,
            .igmp_groupMbrPort_del_wrapper = mcast_groupMbrPort_del_wrapper_dipsip,
            .igmp_group_mcstData_add_wrapper = mcast_igmp_group_mcstData_add_wrapper_dipsip,
            .igmpv3_group_mcstData_add_wrapper = mcast_igmpv3_group_mcstData_add_wrapper_dipsip,
        },
};
static int32 _mcast_allGroup_remove_port(sys_vlanmask_t *pVlanMask, sys_logic_port_t lPort)
{
    int32 i, tmpCnt;
    igmp_group_entry_t *pGroup = NULL;
    sys_vid_t vid;
    multicast_ipType_t ipType;
    igmp_router_entry_t *pRouterEntry;
    sys_logic_portmask_t  portMbr;

    SYS_PARAM_CHK((NULL == pVlanMask), SYS_ERR_NULL_POINTER);

    LOGIC_PORTMASK_CLEAR_ALL(portMbr);
    LOGIC_PORTMASK_SET_PORT(portMbr, lPort);

    mcast_group_rx_timer_stop();

    for (i = 0; i < mcast_group_num; i++)
    {
        pGroup = mcast_group_get_with_index(i);
        if (pGroup == NULL)
            continue;

        if (IGMP_GROUP_ENTRY_STATIC == pGroup->form)
            continue;

        if (!VLANMASK_IS_VIDSET(*pVlanMask, pGroup->vid))
            continue;

        if (IS_LOGIC_PORTMASK_PORTSET(pGroup->mbr, lPort))
        {
            tmpCnt = mcast_group_num;
            mcast_groupPort_remove(pGroup, lPort);
            if (mcast_group_num != tmpCnt)
                i--;
        }
    }
     
     FOR_EACH_VID(vid)
     {
       if (!VLANMASK_IS_VIDSET(*pVlanMask, vid))
            continue;

        for(ipType = MULTICAST_TYPE_IPV4; ipType < MULTICAST_TYPE_END; ipType++)
        {
            mcast_router_db_get(vid , ipType, &pRouterEntry);
            if (NULL != pRouterEntry)
            {
                if (IS_LOGIC_PORTMASK_PORTSET(pRouterEntry->router_pmsk, lPort))
                {
                    if (!IS_LOGIC_PORTMASK_PORTSET(pRouterEntry->router_static_pmsk, lPort))
                    {
                        LOGIC_PORTMASK_CLEAR_PORT(pRouterEntry->router_pmsk, lPort);
                        pRouterEntry->portTimer[lPort] = 0;
                        mcast_routerPortMbr_remove(ipType, pVlanMask, &portMbr);
                    }
                }
            }
        }
     }

   mcast_group_rx_timer_start();
   return SYS_ERR_OK;
}

static int32 _mcast_linkevent_handler(uint32 event, char *pData)
{
    sys_port_event_t    *pPortEvent = NULL;
    sys_vlanmask_t vlanMsk;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pPortEvent = (sys_port_event_t *)pData;

    VLANMASK_SET_ALL(vlanMsk);

   if (PORT_LINKDOWN == pPortEvent->new_linkStatus)
   {
        IGMP_DB_SEM_LOCK();
        _mcast_allGroup_remove_port(&vlanMsk, pPortEvent->lport);
        IGMP_DB_SEM_UNLOCK();
   }

    return SYS_ERR_OK;
}
static int32 _macst_vlanPortMembenChg_groupEntryUp_set(uint16 vid, sys_logic_portmask_t pMask, uint8 isAdd)
{
    int32 i, ret, tmpCnt;
    igmp_group_entry_t *pEntry = NULL;
    sys_ipv46McastEntry_t  ip46Mcst_entry;
    igmp_lookup_mode_t igmpLookupMode, mldLookupMode, lookupMode;
    sys_vlan_operEntry_t vlanEntry;
    multicast_ipType_t ipType;
    igmp_router_entry_t *pRouterEntry;
    sys_vlanmask_t vlanMsk;
    sys_logic_portmask_t  portMbr;
    sys_logic_port_t   lPort;
    uint8 isFwdAll = TRUE;

    if (isAdd)
    {
        mldLookupMode = IGMP_DMAC_VID_MODE;
        rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4, &igmpLookupMode);
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6, &mldLookupMode);
        #endif

        if ((IGMP_DIP_SIP_MODE == igmpLookupMode || IGMP_DIP_SIP_MODE == mldLookupMode) &&
            IGMP_UNKNOWN_MCAST_ACTION_FLOOD == gUnknown_mcast_igmp_action)
        {
            for(i = 0; i < mcast_group_num; i++)
            {
                pEntry = mcast_group_get_with_index(i);
                if (pEntry == NULL)
                    continue;

                if (!IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
                    continue;

                #ifdef CONFIG_SYS_PROTO_MVR
                isFwdAll = mcast_mvr_fwdAll_ret(pEntry);
                #endif
                rsd_mcast_lookupMode_get(pEntry->ipType, &lookupMode);
                if ( IGMP_DIP_SIP_MODE == lookupMode )
                {
                    if (pEntry->sip != 0)
                    {
                        ret = mcast_hw_ipMcastEntry_get(pEntry, &ip46Mcst_entry);
                        if (SYS_ERR_OK == ret)
                        {
                            LOGIC_PORTMASK_OR(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, pMask);
                            SYS_ERR_CHK(mcast_hw_ipMcastEntry_set(pEntry->ipType, &ip46Mcst_entry, FALSE, isFwdAll));
                        }
                    }
                }
            }
        }
    }
    else
    {
       
       for (i = 0; i < mcast_group_num; i++)
       {
            pEntry = mcast_group_get_with_index(i);
            if (pEntry == NULL)
                continue;

            if (vid != pEntry->vid)
                continue;

            if (IGMP_GROUP_ENTRY_STATIC == pEntry->form)
                continue;

            #ifdef CONFIG_SYS_PROTO_MVR
            isFwdAll = mcast_mvr_fwdAll_ret(pEntry);
            #endif

           if (!IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
           {
                LOGIC_PORTMASK_CLEAR_ALL(portMbr);
                LOGIC_PORTMASK_AND(portMbr, pEntry->mbr, pMask);
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(lPort, portMbr)
                {
                    tmpCnt = mcast_group_num;
                    mcast_groupPort_remove(pEntry, lPort);
                    if (mcast_group_num != tmpCnt)
                        i--;
                }
           }
           else
           {
                
                if (IGMP_UNKNOWN_MCAST_ACTION_FLOOD == gUnknown_mcast_igmp_action)
                {
                    rsd_mcast_lookupMode_get(pEntry->ipType, &lookupMode);
                    if ((IGMP_DIP_SIP_MODE == lookupMode) &&  (0 !=  pEntry->sip))
                    {
                         ret = mcast_hw_ipMcastEntry_get(pEntry, &ip46Mcst_entry);
                         if (SYS_ERR_OK == ret)
                         {
                            rsd_vlan_operEntry_get(pEntry->vid, &vlanEntry);
                            LOGIC_PORTMASK_COPY(ip46Mcst_entry.portmask, vlanEntry.lpmMember);
                            SYS_ERR_CHK(mcast_hw_ipMcastEntry_set(pEntry->ipType, &ip46Mcst_entry,TRUE,isFwdAll));
                         }
                    }
                }
            }
        }

        LOGIC_PORTMASK_COPY(portMbr, pMask);
        VLANMASK_CLEAR_ALL(vlanMsk);
        VLANMASK_SET_VID(vlanMsk, vid);

        for(ipType = MULTICAST_TYPE_IPV4; ipType < MULTICAST_TYPE_END; ipType++)
        {
            mcast_router_db_get(vid , ipType, &pRouterEntry);
            if (NULL != pRouterEntry)
            {
                LOGIC_PORTMASK_AND(portMbr, portMbr, pRouterEntry->router_pmsk);
                LOGIC_PORTMASK_ANDNOT(portMbr, portMbr, pRouterEntry->router_static_pmsk);
                if (!IS_LOGIC_PORTMASK_CLEAR(portMbr))
                {
                    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lPort, portMbr)
                    {
                        LOGIC_PORTMASK_CLEAR_PORT(pRouterEntry->router_pmsk, lPort);
                        pRouterEntry->portTimer[lPort] = 0;
                    }

                    mcast_routerPortMbr_remove(ipType, &vlanMsk, &portMbr);
                }
            }
        }

    }

    return SYS_ERR_OK;
}
static void _mcast_vlan_event_init(void)
{
    inEvent = 0;
    outEvent = 0;
}

static int32 _mcast_vlan_event_input(sys_vid_t vid,  sys_logic_portmask_t chgmask, uint32 isAdd)
{
    if (SYS_VLAN_NUM_MAX == inEvent)
        inEvent = 0;

    mcastVlanEvent[inEvent].vid = vid;
    mcastVlanEvent[inEvent].isAdd = isAdd;
    LOGIC_PORTMASK_COPY(mcastVlanEvent[inEvent].chgMbr, chgmask);

    MCASTDBG("input event index = %d, output event index = %d", inEvent, outEvent);
    inEvent++;
    return SYS_ERR_OK;
}

static int32 _mcast_vlan_event_output(void)
{
    while (inEvent != outEvent)
    {
        if (SYS_VLAN_NUM_MAX == outEvent)
            outEvent = 0;

        if (IS_VID_INVALID(mcastVlanEvent[outEvent].vid))
            return SYS_ERR_FAILED;

        _macst_vlanPortMembenChg_groupEntryUp_set(mcastVlanEvent[outEvent].vid, mcastVlanEvent[outEvent].chgMbr, mcastVlanEvent[outEvent].isAdd);
        osal_memset(&mcastVlanEvent[outEvent], 0, sizeof(sys_mcast_event_t));
        outEvent++;
    }

    return SYS_ERR_OK;
}
int32 mcast_Trunk_groupEntry_update(uint32 event, uint8 isCrtDel)
{
    int32 i;
    int32  ret = 0;
    igmp_lookup_mode_t lookup_mode;

    igmp_group_entry_t *pEntry = NULL;
    sys_l2McastEntry_t  l2Mcst_entry;
    sys_ipv46McastEntry_t  ip46Mcst_entry;
    sys_logic_portmask_t aggratePmsk, rtPmsk;
    uint8 isStaticMac, isFwdAll = TRUE;

    IGMP_DB_SEM_LOCK();

    for(i = 0; i < mcast_group_num; i++)
    {
        pEntry = mcast_group_get_with_index(i);
        if (pEntry == NULL)
            continue;

        isStaticMac = FALSE;
        if (!is_mcast_group(pEntry))
            isStaticMac = TRUE;

        #ifdef CONFIG_SYS_PROTO_MVR
        isFwdAll = mcast_mvr_fwdAll_ret(pEntry);
        #endif

        rsd_mcast_lookupMode_get(pEntry->ipType, &lookup_mode);
        if (IGMP_DMAC_VID_MODE == lookup_mode || isStaticMac)
        {
            l2Mcst_entry.vid = pEntry->vid;
            osal_memcpy(l2Mcst_entry.mac, pEntry->mac, CAPA_MAC_BYTE_NUM);

            if (SYS_ERR_OK != mcast_hw_l2McastEntry_get(&l2Mcst_entry))
            {
                if (MULTICAST_TYPE_IPV4 == pEntry->ipType)
                {
                    MCASTDBG("Getting Mcst enrty failed! ret:%d   vid:%d , mac: "MAC_PRINT"",
                            ret,
                            pEntry->vid, MAC_PRINT_ARG(pEntry->mac));
                }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                {
                    MCASTDBG("Getting Mcst enrty failed! ret:%d   vid:%d , mac: "MAC_PRINT"",
                            ret,
                            pEntry->vid, MAC_PRINT_ARG(pEntry->mac));
                }
#endif
            }
            else
            {
                LOGIC_PORTMASK_CLEAR_ALL(l2Mcst_entry.portmask);
                LOGIC_PORTMASK_COPY(l2Mcst_entry.portmask, pEntry->mbr);

                mcast_aggregatePort_get(pEntry, &aggratePmsk);
                LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, aggratePmsk);
                #ifdef CONFIG_SYS_PROTO_MVR
                if (is_mvr_group_ret2(pEntry))
                {
                    LOGIC_PORTMASK_CLEAR_ALL(rtPmsk);
                    if (MVR_MODE_COMPATABLE ==  mcast_mvr_mode_ret(pEntry->ipType))
                    {
                        mcast_mvr_pmsk_get(pEntry->ipType,MVR_TYPE_SOURCE, &rtPmsk);
                    }
                }
                else
                #endif
                {
                    mcast_routerPort_get(pEntry->vid, pEntry->ipType, &rtPmsk);
                }
                LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, rtPmsk);
                if (SYS_NT_TRK_MBR_LEAVE == event && isCrtDel)
                {
                    LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, pEntry->mbr);
                }

                if (SYS_ERR_OK != (ret = mcast_hw_l2McastEntry_set(pEntry->ipType, &l2Mcst_entry, TRUE, isFwdAll)))
                {
                    IGMP_DB_SEM_UNLOCK();
                    return ret;
                }
            }
        }
        else
        {
            if (pEntry->sip != 0 || IGMP_DIP_VID_MODE == lookup_mode)
            {
                ret = mcast_hw_ipMcastEntry_get(pEntry, &ip46Mcst_entry);
                if (SYS_ERR_OK == ret)
                {
                    LOGIC_PORTMASK_CLEAR_ALL(ip46Mcst_entry.portmask);
                    LOGIC_PORTMASK_COPY(ip46Mcst_entry.portmask, pEntry->mbr);

                    #ifdef CONFIG_SYS_PROTO_MVR
                    if (is_mvr_group_ret2(pEntry))
                    {
                        LOGIC_PORTMASK_CLEAR_ALL(rtPmsk);
                        if (MVR_MODE_COMPATABLE ==  mcast_mvr_mode_ret(pEntry->ipType))
                        {
                            mcast_mvr_pmsk_get(pEntry->ipType,MVR_TYPE_SOURCE, &rtPmsk);
                        }
                    }
                    else
                    #endif
                    {
                        mcast_routerPort_get(pEntry->vid, pEntry->ipType, &rtPmsk);
                    }
                    LOGIC_PORTMASK_OR(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, rtPmsk);

                    if (SYS_NT_TRK_MBR_LEAVE == event && isCrtDel)
                    {
                        LOGIC_PORTMASK_OR(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, pEntry->mbr);
                    }

                    if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_set(pEntry->ipType, &ip46Mcst_entry, TRUE, isFwdAll)))
                    {
                        IGMP_DB_SEM_UNLOCK();
                        return ret;
                    }
                }
            }
        }

    }

    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}
static int32 _mcast_trunk_process(uint32 event, char *pData)
{
    sys_logic_portmask_t pmk;  
    sys_logic_portmask_t tmp, trunkMbr;
    sys_trunk_event_t *pTrunkEvent = NULL;
    uint8 isAdd = FALSE, isDel = FALSE;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);
    pTrunkEvent = (sys_trunk_event_t *)pData;

    LOGIC_PORTMASK_CLEAR_ALL(pmk);
    LOGIC_PORTMASK_CLEAR_ALL(trunkMbr);

    rsd_trunk_member_get(pTrunkEvent->trunkIdx, &trunkMbr);
    LOGIC_PORTMASK_ANDNOT(tmp, trunkMbr, pTrunkEvent->portmask);

    if (SYS_NT_TRK_MBR_JOIN == event)
    {
        if (!IS_LOGIC_PORTMASK_CLEAR(tmp))
        {
            LOGIC_PORTMASK_COPY(pmk, pTrunkEvent->portmask);
            isAdd = TRUE; 
        }
    }
    else if (SYS_NT_TRK_MBR_LEAVE == event)
    {
        if (!IS_LOGIC_PORTMASK_CLEAR(trunkMbr))
        {
            isDel = TRUE;
            LOGIC_PORTMASK_COPY(pmk, pTrunkEvent->portmask);
        }
    }
    if (isAdd || isDel)
    {
        
        mcast_Trunk_groupEntry_update(event, FALSE);
        
    }
    else
    {
        mcast_Trunk_groupEntry_update(event, TRUE);
        
    }

    return SYS_ERR_OK;

}

static int _mcast_vlanMask_get(sys_vid_t vid, sys_vlanmask_t *pVlanMask)
{
    SYS_PARAM_CHK((NULL == pVlanMask), SYS_ERR_NULL_POINTER);
    VLANMASK_CLEAR_ALL(*pVlanMask);
    VLANMASK_SET_VID(*pVlanMask, vid);
    return SYS_ERR_OK;
}
int32 mcast_table_clear(void)
{
    mcast_group_del_all();

    return SYS_ERR_OK;
}
igmp_unknMcastAction_t mcast_unknownAct_ret(multicast_ipType_t ipType)
{
    #if 0
    if (MULTICAST_TYPE_IPV4 == ipType)
        return gUnknown_mcast_igmp_action;
    else
        return gUnknown_mcast_mld_action;
    #endif
    return gUnknown_mcast_igmp_action;
}
int32 mcast_igmp_unknMcastAction_set(igmp_unknMcastAction_t action)
{
    igmp_unknMcastAction_t oldAction = gUnknown_mcast_igmp_action;
    igmp_lookup_mode_t lookup_mode;
    uint8 isGroupFull = FALSE;

    if (CAPA_MCAST_GROUP_NUM == igmp_group_num + mld_group_num)
        isGroupFull = TRUE;
    SYS_ERR_CHK(rsd_mcast_igmp_unKnownMcast_action_set(action, oldAction, igmp_cfg.igmpsnp_enable, mld_cfg.mldsnp_enable, isGroupFull));

    gUnknown_mcast_igmp_action = action;

    if (action != oldAction)
    {
        if (action == IGMP_UNKNOWN_MCAST_ACTION_FLOOD || action == IGMP_UNKNOWN_MCAST_ACTION_DROP)
        {
            if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == oldAction)
            {
                mcast_group_learnByData_del(MULTICAST_TYPE_END);
            }
        }

        rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4 , &lookup_mode);
        if ((IGMP_DIP_SIP_MODE == lookup_mode) || (IGMP_DIP_VID_MODE == lookup_mode))
        {
            mcast_group_learnByData_del(MULTICAST_TYPE_IPV4);
        }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6 , &lookup_mode);
        if ((IGMP_DIP_SIP_MODE == lookup_mode) || (IGMP_DIP_VID_MODE == lookup_mode))
        {
            mcast_group_learnByData_del(MULTICAST_TYPE_IPV6);
        }
#endif
    }

    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4 , &lookup_mode);
    if (IGMP_DIP_SIP_MODE == lookup_mode)
    {
        if (igmp_trapDataPkt_lock)
        {
            SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(MULTICAST_TYPE_IPV4, DISABLED, gUnknown_mcast_igmp_action));
        }
    }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING

        rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6 , &lookup_mode);
        if (IGMP_DIP_SIP_MODE == lookup_mode)
        {
            if (igmp_trapDataPkt_lock)
            {
                SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(MULTICAST_TYPE_IPV6, DISABLED, gUnknown_mcast_igmp_action));
            }
        }
#endif

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING

int32 mcast_portGroup_limit_set(multicast_ipType_t ipType, sys_logic_port_t port, uint16 maxnum)
{
    uint16 oldMaxnum;
    uint32 i;
    int32 tmpCnt;
    igmp_group_entry_t *pGroup;
    sys_logic_portmask_t delPmsk;
    uint32 cnt = 0;

    IGMP_DB_SEM_LOCK();

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        oldMaxnum = igmp_cfg.port_entry_limit[port];
        igmp_cfg.port_entry_limit[port] = maxnum;
    }
    else
    {
        oldMaxnum = mld_cfg.port_entry_limit[port];
        mld_cfg.port_entry_limit[port] = maxnum;
    }

    if (oldMaxnum <= maxnum)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
    }

    LOGIC_PORTMASK_CLEAR_ALL(delPmsk);
    LOGIC_PORTMASK_SET_PORT(delPmsk, port);

    mcast_group_rx_timer_stop();
    for (i = 0; i < mcast_group_num; i++)
    {
        pGroup = mcast_group_get_with_index(i);
        if (pGroup == NULL)
            continue;
        
        if (IGMP_GROUP_ENTRY_STATIC == pGroup->form)
            continue;

        if (ipType != pGroup->ipType)
            continue;

        #ifdef CONFIG_SYS_PROTO_MVR
        if (!is_mcast_mvr_maxGroup(pGroup->ipType))
        {
            if (is_mvr_group_ret2(pGroup))
                continue;
        }
        #endif

        if (IS_LOGIC_PORTMASK_PORTSET(pGroup->mbr, port))
        {
            cnt++;
            if (cnt <= maxnum)
                continue;

            tmpCnt = mcast_group_num;
            mcast_groupPort_remove(pGroup, port);
            if (mcast_group_num != tmpCnt)
                i--;
        }
    }

    mcast_group_rx_timer_start();
    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 mcast_portGroup_excessAct_set(multicast_ipType_t ipType, sys_logic_port_t port, igmp_maxGroupAct_t act)
{
    IGMP_DB_SEM_LOCK();
    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        igmp_cfg.max_groups_act[port] = act;
    }
    else
    {
        mld_cfg.max_groups_act[port] = act;
    }
    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}
#endif
int32 mcast_portGroup_excessAct_get(multicast_ipType_t ipType, sys_logic_port_t port, igmp_maxGroupAct_t *pAct)
{
    SYS_PARAM_CHK((NULL == pAct), SYS_ERR_NULL_POINTER);
     if (MULTICAST_TYPE_IPV4 == ipType)
    {
        *pAct = igmp_cfg.max_groups_act[port];
    }
    else
    {
        *pAct = mld_cfg.max_groups_act[port];
    }

    return SYS_ERR_OK;
}
int32 mcast_portGroup_limit_get(multicast_ipType_t ipType, sys_logic_port_t port, uint16 *pMaxnum)
{
    SYS_PARAM_CHK((NULL == pMaxnum), SYS_ERR_NULL_POINTER);

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        *pMaxnum = igmp_cfg.port_entry_limit[port];
    }
    else
    {
        *pMaxnum = mld_cfg.port_entry_limit[port];
    }

    return SYS_ERR_OK;
}
static int32 _mcast_hwEntryByVid_del(sys_vlanmask_t *pVlanMask, multicast_ipType_t ipType,mcast_group_type_t type, mcast_app_type_t appType)
{
    uint32 i = 0;
    uint32 ret = SYS_ERR_OK;
    igmp_group_entry_t *pEntry = NULL;
    uint32 tmpNum;

    SYS_PARAM_CHK((NULL == pVlanMask), SYS_ERR_NULL_POINTER);

    mcast_group_rx_timer_stop();
    for (i = 0 ; i < mcast_group_num ; i++)
    {
        pEntry = mcast_group_get_with_index(i);

        if ( NULL == pEntry)
         {
            continue;
         }
        if (MULTICAST_TYPE_END != ipType)
        {
            if ( ipType != pEntry->ipType)
                continue;
        }

        if (IGMP_GROUP_ENTRY_ALL !=  type)
        {
            if (IGMP_GROUP_ENTRY_STATIC_MAC == type)
            {
                if (is_mcast_group(pEntry))
                    continue;
            }
            else
            {
                 if (IGMP_GROUP_ENTRY_STATIC & type)
                 {
                    if (!is_mcast_group(pEntry))
                        continue;
                 }

                 if ( !(type & pEntry->form))
                    continue;
            }
        }

        #ifdef CONFIG_SYS_PROTO_MVR
        if (MCAST_ENTRY_ALL != appType)
        {
            if (MCAST_ENTRY_SNP == appType)
            {
                if (is_mvr_group_ret2(pEntry))
                    continue;
            }
            else if (MCAST_ENTRY_MVR == appType)
            {
                if (!is_mvr_group_ret2(pEntry))
                    continue;
            }
        }
        #endif

        if (VLANMASK_IS_VIDSET(*pVlanMask,pEntry->vid))
        {
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                MCASTDBG("%s():%d  Delete HW multicast entry with groupId=%X", __FUNCTION__, __LINE__, pEntry->groupId);
            }
#endif

            ret = mcast_hw_mcst_entry_del(pEntry);
            if (SYS_ERR_OK == ret)
            {
                
                mcast_hw_group_num_cal(FALSE);
            }

            tmpNum = mcast_group_num;
            mcast_group_del(pEntry);
            if (tmpNum != mcast_group_num)
                i--;

        }
    }

    mcast_group_rx_timer_start();
    return SYS_ERR_OK;
}
static int32 _mcast_vlan_handler(uint32 event, char* pData)
{
    int32 ret = SYS_ERR_OK;
    sys_vlan_event_t *pVlanEvent = NULL;
    igmp_querier_entry_t *pEntry = NULL;
    sys_vlanmask_t *pVlanMask = NULL;
    multicast_ipType_t ipType;
    sys_vlanmask_t vmExist;
    mcast_vlan_entry_t mcastVlan;
    sys_logic_portmask_t tmpPmsk;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pVlanEvent = (sys_vlan_event_t *)pData;

    IGMP_DB_SEM_LOCK();

    SYS_MEM_ALLOC_HDL(pVlanMask, sizeof(sys_vlanmask_t), _exit, ret);

    if(SYS_NT_VLAN_OPERVLAN_CREATE == event)
    {
        for (ipType = MULTICAST_TYPE_IPV4; ipType < MULTICAST_TYPE_END ; ipType++)
        {
            mcast_querier_db_add(pVlanEvent->vid, ipType);
            osal_memset(&mcastVlan, 0, sizeof(mcast_vlan_entry_t));
            mcastVlan.vid = pVlanEvent->vid;
            mcastVlan.ipType = ipType;
            mcast_vlanMskConfig_get(ipType, &vmExist);
            if (!VLANMASK_IS_VIDSET(vmExist, pVlanEvent->vid))
            {
                mcast_vlan_add(&mcastVlan);
            }
        }
    }
    else if (SYS_NT_VLAN_OPERVLAN_DESTROY == event)
    {
        for (ipType = MULTICAST_TYPE_IPV4; ipType < MULTICAST_TYPE_END; ipType++)
        {
            mcast_querier_db_get(pVlanEvent->vid, ipType, &pEntry);
            if (pEntry)
            {
                mcast_querier_db_del(pVlanEvent->vid, ipType);
            }
            else
            {
                if (MULTICAST_TYPE_IPV4 == ipType)
                    MCASTDBG("Warring! IGMP querier entry for VLAN-%d doesn't exist!", pVlanEvent->vid);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                    MCASTDBG("Warring! MLD querier entry for VLAN-%d doesn't exist!", pVlanEvent->vid);
#endif
            }
        }

        VLANMASK_CLEAR_ALL(*pVlanMask);
        if (!IS_VID_INVALID(pVlanEvent->vid))
            VLANMASK_SET_VID(*pVlanMask, pVlanEvent->vid);
        _mcast_hwEntryByVid_del(pVlanMask, MULTICAST_TYPE_END, IGMP_GROUP_ENTRY_DYNAMIC, MCAST_ENTRY_ALL);
    }
    else if (SYS_NT_VLAN_OPERENTRY_CHG == event)
    {
        LOGIC_PORTMASK_CLEAR_ALL(tmpPmsk);
        LOGIC_PORTMASK_AND(tmpPmsk, pVlanEvent->lpmMember, pVlanEvent->lpmChanged);
        
        if (IS_LOGIC_PORTMASK_CLEAR(tmpPmsk))
        {
            _mcast_vlan_event_input(pVlanEvent->vid, pVlanEvent->lpmChanged, FALSE);
            
        }
        else 
        {
            _mcast_vlan_event_input(pVlanEvent->vid, pVlanEvent->lpmChanged, TRUE);
            
        }
    }

_exit:
    IGMP_DB_SEM_UNLOCK();
    SYS_MEM_FREE(pVlanMask);

    return ret;
}
int32 mcast_router_get(igmp_router_entry_t *pRouter)
{
    igmp_router_entry_t *tmp;

    SYS_PARAM_CHK(NULL == pRouter, SYS_ERR_NULL_POINTER);

    mcast_router_db_get(pRouter->vid, pRouter->ipType, &tmp);

    if (tmp)
    {
        osal_memcpy(pRouter, tmp, sizeof(igmp_router_entry_t));
    }
    else
        osal_memset(pRouter, 0, sizeof(igmp_router_entry_t));

    return SYS_ERR_OK;
}
int32 mcast_router_set(igmp_router_entry_t *pRouter)
{
    return mcast_router_db_set(pRouter);
}
int32 mcast_router_dynPortMbr_del(sys_vlanmask_t vlanMsk, multicast_ipType_t ipType)
{
    sys_vid_t vid;
    igmp_router_entry_t  *pRouterEntry;
    sys_logic_portmask_t dynPmsk;

    sys_logic_port_t port;

    SYS_PARAM_CHK((ipType< MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    FOR_EACH_VID(vid)
    {
        if (!VLANMASK_IS_VIDSET(vlanMsk, vid))
            continue;

        mcast_router_db_get(vid , ipType, &pRouterEntry);
        if (NULL != pRouterEntry)
        {
            LOGIC_PORTMASK_CLEAR_ALL(dynPmsk);
            LOGIC_PORTMASK_ANDNOT(dynPmsk, pRouterEntry->router_pmsk, pRouterEntry->router_static_pmsk);

            if (IS_LOGIC_PORTMASK_CLEAR(pRouterEntry->router_static_pmsk) &&
                IS_LOGIC_PORTMASK_CLEAR(pRouterEntry->router_forbid_pmsk))
            {
                mcast_router_db_del(vid, ipType);
            }

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, dynPmsk)
            {
                LOGIC_PORTMASK_CLEAR_PORT(pRouterEntry->router_pmsk, port);
                pRouterEntry->portTimer[port] = 0;
            }

            if (IS_LOGIC_PORTMASK_CLEAR(dynPmsk))
            {
                mcast_routerPortMbr_remove(ipType, &vlanMsk,  &dynPmsk);
            }
        }
    }

    return SYS_ERR_OK;
}
int32 mcast_database_get(sys_igmp_db_t *pDb)
{
    int life;
    igmp_group_entry_t *pGroup_entry;
    sys_logic_port_t port;

    IGMP_DB_SEM_LOCK();

    pGroup_entry = mcast_group_get_with_index(pDb->indx);

    if (pGroup_entry)
    {
        pDb->dip = pGroup_entry->dip;
        pDb->sip = pGroup_entry->sip;
        pDb->vid = pGroup_entry->vid;
        if ((life = mcast_max_life_get(pGroup_entry)) > 0)
        {
            pDb->life = life;
        }
        else
        {
            pDb->life = pGroup_entry->groupTimer;
        }
        osal_memcpy(pDb->mac, pGroup_entry->mac, CAPA_MAC_BYTE_NUM);
        osal_memcpy(&pDb->portmask, &pGroup_entry->mbr, sizeof(sys_logic_portmask_t));

        pDb->l2TableWriteFlag = pGroup_entry->lookupTableFlag;
        FOR_EACH_LOGIC_PORT(port)
        {
            pDb->v3.fwdFlag[port] = pGroup_entry->v3.mbrFwdFlag[port];
        }

        pDb->form = pGroup_entry->form;
        pDb->ipType = pGroup_entry->ipType;

        if (MULTICAST_TYPE_IPV4 != pGroup_entry->ipType)
        {
            osal_memcpy(pDb->dipv6, pGroup_entry->dipv6.addr, IPV6_ADDR_LEN);
            osal_memcpy(pDb->sipv6, pGroup_entry->sipv6.addr, IPV6_ADDR_LEN);
        }

        osal_memcpy(&pDb->staticMbr, &pGroup_entry->staticMbr,sizeof(sys_logic_portmask_t));
        osal_memcpy(&pDb->staticInMbr, &pGroup_entry->staticInMbr,sizeof(sys_logic_portmask_t));

    }

    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 mcast_databaseSize_get(uint32 *pSize)
{
    SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);
    *pSize = mcast_group_num;

    return SYS_ERR_OK;
}
int32 mcast_igmp_databaseSize_get(uint32 *pSize)
{
     SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);
     *pSize = igmp_group_num;
     return SYS_ERR_OK;
}
int32 mcast_igmp_staticMacNum_get(uint32 *pSize)
{
     SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);
    *pSize = igmp_staticMac_num;
    return SYS_ERR_OK;
}
int32 mcast_dipOnly_databaseSize_get(uint32 *pSize)
{
    *pSize = group_sortedAry_entry_num;

    return SYS_ERR_OK;
}
void mcast_routerPort_timer(void)
{
    uint8                   result;
    uint16                  i, time, flag = FALSE;
    sys_logic_port_t        port;
    sys_logic_portmask_t    delPmsk;
    igmp_router_entry_t     *rt_array;

    multicast_ipType_t ipType;
    sys_vlanmask_t rtVlanMask;

    IGMP_DB_SEM_LOCK();
    for(ipType = MULTICAST_TYPE_IPV4; ipType < MULTICAST_TYPE_END; ipType++)
    {
        rt_array = router_db[ipType].rt_entry;

        for (i = 0; i < router_db[ipType].entry_num; i++)
        {
            flag = FALSE;
            osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, rt_array[i].router_pmsk)
            {
                if(!IS_LOGIC_PORTMASK_PORTSET( rt_array[i].router_static_pmsk, port))
                {
                    time = rt_array[i].portTimer[port];
                    if (time == 0)
                    {
                        if (ipType == MULTICAST_TYPE_IPV4)
                        {
                            MCASTDBG("%s():%d  Warring! an existing mbr whose life is zero", __FUNCTION__, __LINE__);
                        }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                        else
                        {
                            MCASTDBG("%s():%d  Warring! an existing mbr whose life is zero", __FUNCTION__, __LINE__);
                        }
#endif
                    }
                    else
                    {
                        if (time > PASS_SECONDS)
                            rt_array[i].portTimer[port] -= PASS_SECONDS;
                        else
                        {
                            LOGIC_PORTMASK_CLEAR_PORT(rt_array[i].router_pmsk, port);
                            LOGIC_PORTMASK_SET_PORT(delPmsk, port);
                            mcast_log_rtPort(ipType,rt_array[i].vid, port, MCAST_RT_TIMEOUT);

                            if (!IS_LOGIC_PORTMASK_PORTSET(rt_array[i].router_pmsk, LP2TRKLP(port)))
                            {
                                flag = TRUE;
                            }
                        }
                    }
                }
            }
            if (flag)
            {
                _mcast_vlanMask_get(rt_array[i].vid, &rtVlanMask);
                mcast_routerPortMbr_remove(ipType, &rtVlanMask, &delPmsk);

                LOGIC_PORTMASK_IS_CLEAR(rt_array[i].router_pmsk, result);
                if (result)
                {
                    LOGIC_PORTMASK_IS_CLEAR(rt_array[i].router_forbid_pmsk, result);
                    if(result)
                    {
                        mcast_router_db_del(rt_array[i].vid, ipType);
                        i--;        
                    }
                }
            }
        }
    }
    IGMP_DB_SEM_UNLOCK();
}
int32 mcast_group_entry_clear(igmp_group_entry_t *pGroup)
{
    int32 ret;
    igmp_group_entry_t *pEntry;

    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    uint16 sortedIdx;
    igmp_group_entry_t *groupHead;
    #endif

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

    pEntry = mcast_group_get(pGroup);

    if(pEntry == NULL)
    {
        return SYS_ERR_FAILED;
    }

    ret = mcast_hw_mcst_entry_del(pEntry);
    if (ret == SYS_ERR_OK)
    {
        
        mcast_hw_group_num_cal(FALSE);
    }

    mcast_group_del(pGroup);

    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    if(igmp_cfg.oper_version == IGMP_VERSION_V3_FULL)
    {
        mcast_group_sortedArray_search(mcast_group_sortKey_ret(pEntry), &sortedIdx, &groupHead);
        if(groupHead == NULL)
        {
            mcast_igmp_group_head_del(pGroup->dip, pGroup->vid);
        }
    }
    #endif

    return SYS_ERR_OK;
}
uint8  _mcast_asicSet_check(multicast_ipType_t ipType, sys_vid_t vid, uint8 isMvrGroup)
{
    uint8 isAsicSet = TRUE;
    mcast_vlan_entry_t *pMcastVlan = NULL;

    if (isMvrGroup)
        return isAsicSet;

    if (MULTICAST_TYPE_IPV4 == ipType && DISABLED == igmp_cfg.igmpsnp_enable)
    {
        isAsicSet = FALSE;
    }

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    if (MULTICAST_TYPE_IPV6 == ipType && DISABLED == mld_cfg.mldsnp_enable)
    {
       isAsicSet = FALSE;
    }
#endif

    mcast_vlan_db_get(vid , ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        
        isAsicSet = FALSE;
    }
    else
    {
        if (DISABLED == pMcastVlan->enable)
        {
            isAsicSet = FALSE;
        }
    }

    return isAsicSet;
}
int32 mcast_basic_static_groupEntry_set(igmp_group_entry_t *pGroup,  uint8 fmode, uint8 isAdd)
{
    uint16 sortedIdx;
    igmp_group_entry_t      *pEntry, *groupHead;
    sys_ipv46McastEntry_t      ip46McstEntry;
    sys_logic_port_t          port = 0, firstPort =0;

    int32                   ret;
    sys_l2McastEntry_t      l2McstEntry;
    sys_logic_portmask_t    rtPmsk;
    sys_logic_portmask_t    addInMbr, addExMbr, delExMbr, delInMbr;
    uint8 zeroIpv6[IPV6_ADDR_LEN];
    igmp_aggregate_entry_t  *pAggrEntry = NULL;
    mcast_groupAddr_t groupAddr;
    uint8 isMvrGroup = FALSE;
    uint8 isAsicSet = TRUE, isFwdAll = TRUE;
    igmp_lookup_mode_t lookup_mode;
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pGroup->ipType< MULTICAST_TYPE_IPV4 ||pGroup->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    osal_memset(&l2McstEntry, 0, sizeof(sys_l2McastEntry_t));
    rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);

    osal_memset(&groupAddr, 0, sizeof(mcast_groupAddr_t));
    groupAddr.ipv4 = pGroup->dip;
    osal_memcpy(groupAddr.ipv6.addr, pGroup->dipv6.addr,IPV6_ADDR_LEN);

    #ifdef CONFIG_SYS_PROTO_MVR
    if (is_mvr_group_ret1(pGroup->ipType, pGroup->vid,&groupAddr, TRUE))
    {
        isMvrGroup = TRUE;
        if (!is_mcast_mvr_forwardAll(pGroup->ipType))
            isFwdAll = FALSE;
    }
    #endif

    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
    {
        if (!IGMP_IN_MULTICAST(ntohl(pGroup->dip)))
        {
            return SYS_ERR_MCAST_ADDR;
        }

        if ((ntohl(pGroup->dip) >= ntohl(0xE0000000L)) && (ntohl(pGroup->dip) <= ntohl(0xE00000FFL)))
        {
            return SYS_ERR_IGMP_RSV_ADDR;
        }
    }
    else
    {
        osal_memset(zeroIpv6,0, IPV6_ADDR_LEN);
        if (pGroup->dipv6.addr[0] != 0xff)
        {
            return SYS_ERR_MCAST_ADDR;
        }

        if ((pGroup->dipv6.addr[1] == 0x0 || pGroup->dipv6.addr[1] == 0x01) &&
              (osal_memcmp(&pGroup->dipv6.addr[2], &zeroIpv6[2] ,IPV6_ADDR_LEN-2) == 0)  )
        {
            return SYS_ERR_IGMP_RSV_ADDR;
        }

        pGroup->dip = IPV6_TO_IPV4(pGroup->dipv6.addr);
    }

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(pGroup), &sortedIdx, &groupHead);   
    if (groupHead)
    {
        
        if(IGMP_GROUP_ENTRY_DYNAMIC == groupHead->form)
        {
            if (FALSE == isAdd)
            {
                return SYS_ERR_OK;
            }
            else
                mcast_basic_static_group_clear(pGroup);
        }
        else
        {
            IGMP_DB_SEM_LOCK();

            LOGIC_PORTMASK_CLEAR_ALL(addInMbr);
            LOGIC_PORTMASK_CLEAR_ALL(addExMbr);
            LOGIC_PORTMASK_CLEAR_ALL(delExMbr);
            LOGIC_PORTMASK_CLEAR_ALL(delInMbr);

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pGroup->mbr)
            {
                if(TRUE == isAdd)
                {
                    
                    if(FILTER_MODE_INCLUDE == fmode)
                    {
                        if (!IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, port))
                        {
                            LOGIC_PORTMASK_SET_PORT(addInMbr, port);
                        }

                        LOGIC_PORTMASK_SET_PORT(groupHead->staticMbr, port);
                        LOGIC_PORTMASK_SET_PORT(groupHead->mbr, port);
                        LOGIC_PORTMASK_SET_PORT(groupHead->staticInMbr, port);
                    }
                    else
                    {
                        LOGIC_PORTMASK_SET_PORT(groupHead->staticMbr, port);

                        if(igmp_cfg.oper_version <= IGMP_VERSION_V2)
                            LOGIC_PORTMASK_CLEAR_PORT(groupHead->mbr, port);
                        else
                            LOGIC_PORTMASK_SET_PORT(groupHead->mbr, port);

                        LOGIC_PORTMASK_SET_PORT(addExMbr, port);
                    }
                }
                else
                {
                    LOGIC_PORTMASK_CLEAR_PORT(groupHead->staticMbr, port);

                    if(FILTER_MODE_INCLUDE == fmode)
                    {
                        LOGIC_PORTMASK_CLEAR_PORT(groupHead->mbr, port);
                        LOGIC_PORTMASK_CLEAR_PORT(groupHead->staticInMbr, port);
                        LOGIC_PORTMASK_SET_PORT(delInMbr,port);
                    }
                    else
                    {
                        if(igmp_cfg.oper_version > IGMP_VERSION_V2)
                        {
                            LOGIC_PORTMASK_CLEAR_PORT(groupHead->mbr, port);
                        }
                        LOGIC_PORTMASK_SET_PORT(delExMbr, port);
                    }
                }
            }

            if (FALSE == isAdd)
            {
                if (IS_LOGIC_PORTMASK_CLEAR(groupHead->mbr))
                {
                    
                    LOGIC_PORTMASK_OR(groupHead->mbr, groupHead->mbr, delInMbr);

                    mcast_basic_static_group_clear(groupHead);
                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_OK;
                }
            }
            mcast_routerPort_get(pGroup->vid, pGroup->ipType, &rtPmsk);

            if ((IGMP_DMAC_VID_MODE == lookup_mode) ||(IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != pGroup->ipType) )
            {
                mcast_aggregate_db_get(pGroup, &pAggrEntry, TRUE);
                if (pAggrEntry == NULL)
                {
                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_IGMP_GROUP_ENTRY_NOT_EXIST;

                }
                else
                {
                    if(TRUE == isAdd)
                    {
                        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, addInMbr)
                        {
                            pAggrEntry->port_ref_cnt[port]++;
                        }
                    }
                    else
                    {
                        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, delInMbr)
                        {
                            pAggrEntry->port_ref_cnt[port]--;
                        }
                    }
                }

                if (IGMP_DMAC_VID_MODE == lookup_mode)
                {
                    l2McstEntry.vid = pGroup->vid;
                    osal_memcpy(l2McstEntry.mac, groupHead->mac, CAPA_MAC_BYTE_NUM);
                    ret = mcast_hw_l2McastEntry_get(&l2McstEntry);
                }
                else
                {
                    SYS_MEM_CLEAR(ip46McstEntry);
                    ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46McstEntry);
                }

                if (SYS_ERR_OK == ret)
                {
                    if(FALSE == isAdd)
                    {
                        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, delInMbr)
                        {
                            if (0 == pAggrEntry->port_ref_cnt[port])
                            {
                                if (!IS_LOGIC_PORTMASK_PORTSET(rtPmsk, port))
                                {
                                    LOGIC_PORTMASK_CLEAR_PORT(l2McstEntry.portmask, port);
                                    LOGIC_PORTMASK_CLEAR_PORT(ip46McstEntry.portmask, port);
                                }
                            }
                        }
                    }
                    else
                    {
                        LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, addInMbr);
                        LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, addInMbr);
                    }

                     if (IGMP_DMAC_VID_MODE == lookup_mode)
                     {
                         ret = mcast_hw_l2McastEntry_set(pGroup->ipType, &l2McstEntry, FALSE, isFwdAll);
                     }
                     else
                     {
                        ret = mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46McstEntry, FALSE, isFwdAll);
                     }
                }
            }
            else
            {
                
                if( igmp_cfg.oper_version > IGMP_VERSION_V3_BASIC)
                {
                    pEntry = groupHead;
                    while(pEntry)
                    {
                        
                        if (IGMP_GROUP_ENTRY_STATIC == pEntry->form)
                        {
                            if(pEntry != groupHead)
                            {
                                osal_memcpy(&pEntry->mbr, &groupHead->mbr,sizeof(sys_logic_portmask_t));
                                osal_memcpy(&pEntry->staticMbr, &groupHead->staticMbr,sizeof(sys_logic_portmask_t));
                                osal_memcpy(&pEntry->staticInMbr, &groupHead->staticInMbr,sizeof(sys_logic_portmask_t));
                            }

                            if(TRUE == isAdd)
                            {
                                FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, addExMbr)
                                {
                                    pEntry->v3.mbrFwdFlag[port] = IGMPV3_FWD_NOT;
                                }
                            }
                            else
                            {
                                FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, delExMbr)
                                {
                                    if (IGMPV3_FWD_NOT ==  pEntry->v3.mbrFwdFlag[port])
                                       pEntry->v3.mbrFwdFlag[port] = IGMPV3_FWD_NORMAL;
                                }
                            }

                            if (SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pEntry,&ip46McstEntry)))
                            {
                                mcast_routerPort_get(pGroup->vid, pGroup->ipType, &rtPmsk);
                                #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
                                mcast_igmpv3_fwdPmsk_get(&ip46McstEntry.portmask, pEntry);
                                #endif
                                LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, rtPmsk);
                                mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46McstEntry, TRUE, isFwdAll);
                           }
                        }
                       pEntry = pEntry->next_subgroup;
                    }
                }
                else
                {
                    if (groupHead->sip == 0 && IGMP_DIP_SIP_MODE == lookup_mode)     
                    {
                        IGMP_DB_SEM_UNLOCK();
                        return SYS_ERR_OK;      
                    }
                    else                        
                    {
                        pEntry = groupHead;
                        while(pEntry)
                        {
                            if(pEntry != groupHead)
                            {
                                if(IS_LOGIC_PORTMASK_EQUAL(pEntry->mbr, groupHead->mbr))
                                {
                                    pEntry = pEntry->next_subgroup;
                                    continue;
                                }
                                osal_memcpy(&pEntry->mbr, &groupHead->mbr, sizeof(sys_logic_portmask_t));
                            }

                            if (SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pEntry, &ip46McstEntry)))
                            {
                                mcast_routerPort_get(pGroup->vid, pGroup->ipType, &rtPmsk);
                                LOGIC_PORTMASK_OR(ip46McstEntry.portmask, pEntry->mbr, rtPmsk);
                                mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46McstEntry, TRUE, isFwdAll);
                            }
                            pEntry = pEntry->next_subgroup;
                        }
                    }
                }
            }

            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }
     }
     else
     {
           if( FALSE == isAdd)
           {
                if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
                {
                    MCASTDBG("The VLAN %d group ip("IPADDR_PRINT")  is not exist !", pGroup->vid,
                                                                                                  IPADDR_PRINT_ARG(pGroup->dip));
                }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                    MCASTDBG("The VLAN %d group ip("IPADDRV6_PRINT")  is not exist !", pGroup->vid,
                                                                                                  IPADDRV6_PRINT_ARG(pGroup->dipv6.addr));
#endif
                return SYS_ERR_OK;
           }
     }

    IGMP_DB_SEM_LOCK();

    pEntry = mcast_group_freeEntry_get(pGroup->ipType, isMvrGroup);
    if (pEntry != NULL)
    {
        
        pEntry->form = IGMP_GROUP_ENTRY_STATIC;
        pEntry->ipType = pGroup->ipType;
        pEntry->dip = pGroup->dip;
        pEntry->sip = 0;
        pEntry->vid = pGroup->vid;
        pEntry->sortKey =  mcast_group_sortKey_ret(pGroup);

        if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
        {
            pEntry->mac[0] = 0x01;
            pEntry->mac[1] = 0x00;
            pEntry->mac[2] = 0x5e;
            pEntry->mac[3] = (pGroup->dip >> 16) & 0x7f;
            pEntry->mac[4] = (pGroup->dip >> 8) & 0xff;
            pEntry->mac[5] = pGroup->dip & 0xff;
        }
        else
        {
            pEntry->mac[0] = 0x33;
            pEntry->mac[1] = 0x33;
            pEntry->mac[2] = (pGroup->dip >> 24) & 0xff;
            pEntry->mac[3] = (pGroup->dip >> 16) & 0xff;
            pEntry->mac[4] = (pGroup->dip >> 8) & 0xff;
            pEntry->mac[5] = pGroup->dip & 0xff;

            osal_memcpy(pEntry->dipv6.addr, pGroup->dipv6.addr,IPV6_ADDR_LEN);
        }

        osal_memcpy(&pEntry->staticMbr, &pGroup->mbr, sizeof(sys_logic_portmask_t));
        osal_memcpy(&pEntry->mbr, &pGroup->mbr, sizeof(sys_logic_portmask_t));

        if(FILTER_MODE_INCLUDE == fmode)
        {
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port,pGroup->mbr)
            {
                LOGIC_PORTMASK_SET_PORT(pEntry->staticInMbr, port);
            }
            
        }
        else
        {
            if(igmp_cfg.oper_version > IGMP_VERSION_V2)
            {
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pGroup->mbr)
                {
                    pEntry->v3.mbrFwdFlag[port] = IGMPV3_FWD_NOT;
                }
            }
        }

        if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
        {
            mcast_group_entry_release(pEntry);
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_OK;
        }

        isAsicSet = _mcast_asicSet_check(pGroup->ipType, pGroup->vid, isMvrGroup);

        if ((IGMP_DMAC_VID_MODE == lookup_mode) || (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != pGroup->ipType) )
        {
             
            mcast_aggregate_db_get(pGroup, &pAggrEntry, FALSE);

            if (!isMvrGroup)
            {
                if (!isAsicSet)
                {
                    if (pAggrEntry == NULL)
                    {
                        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pEntry->mbr)
                        {
                            firstPort = port;
                            break;
                        }

                        if (SYS_ERR_OK != (ret = mcast_aggregate_db_add(pGroup, firstPort)))
                        {
                            mcast_group_entry_release(pEntry);
                            IGMP_DB_SEM_UNLOCK();
                            return SYS_ERR_FAILED;
                        }

                        mcast_aggregate_db_get(pGroup, &pAggrEntry,TRUE);
                        if (pAggrEntry != NULL)
                        {
                            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pEntry->mbr)
                            {
                                if (firstPort != port)
                                {
                                    pAggrEntry->port_ref_cnt[port]++;
                                }
                            }
                        }
                    }
                    else
                    {
                        pAggrEntry->group_ref_cnt++;
                        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pEntry->mbr)
                        {
                            pAggrEntry->port_ref_cnt[port]++;
                        }
                    }

                    mcast_group_num_cal(pGroup->ipType,FALSE, isMvrGroup,TRUE);
                    mcast_group_sortedArray_ins(sortedIdx, pEntry);
                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_OK;
                }
             }

            if ( SYS_ERR_OK != (ret = mcast_newGroup_aggPortMsk_add(pEntry, pEntry->mbr, isMvrGroup)))
            {
                if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
                {
                    MCASTDBG("Failed add new group port member to ASIC!  ret:%d  vid:%d   mac:"MAC_PRINT"",
                            ret, pGroup->vid, MAC_PRINT_ARG(l2McstEntry.mac));
                }
                #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                {
                    MCASTDBG("Failed writing to ASIC!  ret:%d  vid:%d   dip:"IPADDRV6_PRINT"",
                            ret,pGroup->vid, IPADDRV6_PRINT_ARG(ip46McstEntry.ipv6Mcast.dip.octet));
                }
                #endif
                mcast_group_entry_release(pEntry);
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }
        }
        else
        {
            if (!isMvrGroup)
            {
                if (!isAsicSet)
                {
                    mcast_group_num_cal(pGroup->ipType,FALSE, isMvrGroup,TRUE);
                    mcast_group_sortedArray_ins(sortedIdx, pEntry);
                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_OK;
                }
            }

            SYS_MEM_CLEAR(ip46McstEntry);
            if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
            {
                mcast_ipMcast_asicKey_get(pGroup->vid, pGroup->dip, pGroup->sip, &ip46McstEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                if (IS_CHIP_CSW())
                {
                    ip46McstEntry.ipMcast.groupId = pGroup->groupId;
                }
#endif
            }
            else
            {
                mcast_ipv6Mcast_asicKey_get(pGroup->vid, pGroup->dipv6.addr, pGroup->sipv6.addr, &ip46McstEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                if (IS_CHIP_CSW())
                {
                    ip46McstEntry.ipv6Mcast.groupId = pGroup->groupId;
                }
#endif
            }

           #ifdef CONFIG_SYS_PROTO_MVR
            if (isMvrGroup)
            {
                if (MVR_MODE_COMPATABLE == mcast_mvr_mode_ret(pGroup->ipType))
                    mcast_mvr_pmsk_get(pGroup->ipType, MVR_TYPE_SOURCE, &rtPmsk);
            }
            else
            #endif
            {
                mcast_routerPort_get(pGroup->vid, pGroup->ipType, &rtPmsk);
            }

            LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, rtPmsk);
            LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, pEntry->mbr);

            if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_add(pGroup->ipType, &ip46McstEntry, isFwdAll)))
            {
                mcast_log_asicIP_fail(pGroup->ipType, &ip46McstEntry,MCAST_ADD);
                mcast_group_entry_release(pEntry);
                IGMP_DB_SEM_UNLOCK();
                return SYS_ERR_FAILED;
            }
            else
            {
                pEntry->lookupTableFlag = L2TABLE_WROTE;
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                if (IS_CHIP_CSW())
                {
                    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
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
        }

        mcast_group_num_cal(pGroup->ipType,FALSE, isMvrGroup,TRUE);
        mcast_group_sortedArray_ins(sortedIdx, pEntry);
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
    }
    else
    {
        IGMP_DB_SEM_UNLOCK();
        mcast_log_tablefull(pGroup->ipType);
        return SYS_ERR_MCAST_DATABASE_FULL;
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 mcast_basic_static_group_clear(igmp_group_entry_t *pGroup)
{
    uint16 sortedIdx;
    igmp_group_entry_t  *groupHead = NULL, *pEntry = NULL, *pTmp = NULL;
    igmp_lookup_mode_t lookup_mode;

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(pGroup), &sortedIdx, &groupHead);

    if (groupHead == NULL)
        return SYS_ERR_OK;

    rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);
    if (IGMP_DIP_SIP_MODE == lookup_mode)
    {
        pEntry = groupHead;
        while(pEntry)
        {
            pTmp = pEntry->next_subgroup;
            mcast_group_entry_clear(pEntry);
            pEntry = pTmp;
        }
    }
    else
    {
        mcast_group_entry_clear(groupHead);
    }

    return SYS_ERR_OK;
}
int32 mcast_static_group_clear(igmp_group_entry_t  *pGroup)
{
    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

    IGMP_DB_SEM_LOCK();
    if(pGroup->sip  == 0)
    {
        mcast_basic_static_group_clear(pGroup);
    }
    else
    {
        mcast_group_entry_clear(pGroup);
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 mcast_static_group_hwRestore(sys_vlanmask_t vlanMsk, multicast_ipType_t ipType)
{
    int i;
    igmp_group_entry_t *pGroup = NULL;
    sys_logic_portmask_t  rtPmsk;
    sys_l2McastEntry_t l2McstEntry;
    sys_ipv46McastEntry_t ip46McstEntry;
    int32 ret;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    mcast_groupAddr_t groupAddr;
    igmp_lookup_mode_t lookup_mode;
    uint8 isStaticMac, isFwdAll;
    SYS_PARAM_CHK((ipType > MULTICAST_TYPE_END || ipType < MULTICAST_TYPE_IPV4), SYS_ERR_MCAST_IPTYPE);

    IGMP_DB_SEM_LOCK();

    for (i = 0; i < mcast_group_num; i++)
    {
        pGroup = mcast_group_get_with_index(i);

        if ( NULL == pGroup)
            continue;

        if (ipType != pGroup->ipType)
            continue;

        if (!VLANMASK_IS_VIDSET(vlanMsk, pGroup->vid))
            continue;

        if (IGMP_GROUP_ENTRY_STATIC != pGroup->form)
            continue;

        mcast_vlan_db_get(pGroup->vid, pGroup->ipType, &pMcastVlan);
        if (NULL == pMcastVlan)
        {
            continue;
        }
        else
        {
            if (DISABLED == pMcastVlan->enable)
                continue;
        }

        isStaticMac = FALSE;
        if (!is_mcast_group(pGroup))
            isStaticMac = TRUE;

        mcast_routerPort_get(pGroup->vid, pGroup->ipType, &rtPmsk);

        groupAddr.ipv4 = pGroup->dip;
        if (MULTICAST_TYPE_IPV4 != pGroup->ipType)
            osal_memcpy(groupAddr.ipv6.addr, pGroup->dipv6.addr,IPV6_ADDR_LEN);
        isFwdAll = TRUE;

        rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);
        if(IGMP_DMAC_VID_MODE == lookup_mode || isStaticMac)
        {
            osal_memset(&l2McstEntry, 0, sizeof(sys_l2McastEntry_t));
            l2McstEntry.vid = pGroup->vid;
            osal_memcpy(l2McstEntry.mac, pGroup->mac,CAPA_MAC_BYTE_NUM);

            LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, rtPmsk);
            LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, pGroup->mbr);

            #ifdef CONFIG_SYS_PROTO_MVR
            if (is_mvr_group_ret1(ipType, pGroup->vid, &groupAddr, TRUE))
            {
                l2McstEntry.crossVlan = ENABLED;
                if (!is_mcast_mvr_forwardAll(ipType))
                    isFwdAll = FALSE;
            }
            #endif

            if (SYS_ERR_OK != (ret = mcast_hw_l2McastEntry_add(ipType, &l2McstEntry, isFwdAll)))
            {
                
                if (SYS_ERR_L2_MAC_IS_EXIST == ret)
                {
                    
                    if (SYS_ERR_OK != (ret = mcast_hw_l2McastEntry_set(ipType, &l2McstEntry, TRUE, isFwdAll)))
                    {
                        IGMP_DB_SEM_UNLOCK();
                        return SYS_ERR_FAILED;
                    }
                }
                else if (SYS_ERR_L2_MAC_FULL == ret)
                {
                    mcast_log_asicMac_fail(&l2McstEntry,MCAST_ADD);
                    IGMP_DB_SEM_UNLOCK();
                    return SYS_ERR_OK;
                }
            }
            else
            {

                mcast_hw_group_num_cal(TRUE);
            }
        }
        else
        {
            if ( 0 == pGroup->sip && IGMP_DIP_SIP_MODE == lookup_mode)
                continue;

            if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
            {
                mcast_ipMcast_asicKey_get(pGroup->vid, pGroup->dip, pGroup->sip, &ip46McstEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                if (IS_CHIP_CSW())
                {
                    ip46McstEntry.ipMcast.groupId = pGroup->groupId;
                }
#endif
            }
            else
            {
                mcast_ipv6Mcast_asicKey_get(pGroup->vid, pGroup->dipv6.addr, pGroup->sipv6.addr, &ip46McstEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                if (IS_CHIP_CSW())
                {
                    ip46McstEntry.ipv6Mcast.groupId = pGroup->groupId;
                }
#endif
            }

            if (igmp_cfg.oper_version < IGMP_VERSION_V3_FULL)
            {
                LOGIC_PORTMASK_COPY(ip46McstEntry.portmask, pGroup->mbr);
                LOGIC_PORTMASK_OR(ip46McstEntry.portmask,ip46McstEntry.portmask, rtPmsk);
            }

            #ifdef CONFIG_SYS_PROTO_MVR
            if (is_mvr_group_ret1(ipType, pGroup->vid, &groupAddr, TRUE))
            {
                ip46McstEntry.ipMcast.crossVlan = ENABLED;
                 if (!is_mcast_mvr_forwardAll(ipType))
                    isFwdAll = FALSE;
            }
            #endif

            if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_add(ipType, &ip46McstEntry, isFwdAll)))
            {
                 if (SYS_ERR_L2_MAC_IS_EXIST == ret)
                 {
                     if (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != ipType)
                     {
                         if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_set(ipType, &ip46McstEntry, TRUE, isFwdAll)))
                         {
                            IGMP_DB_SEM_UNLOCK();
                            return SYS_ERR_FAILED;
                        }
                     }
                 }
                 else if (SYS_ERR_L2_MAC_FULL == ret)
                 {
                    mcast_log_asicIP_fail(ipType, &ip46McstEntry,MCAST_ADD);

                    if (IGMP_DIP_SIP_MODE == lookup_mode)
                    {
                        rsd_mcast_ipMcastPkt2CpuEnable_set(ipType, DISABLED, mcast_unknownAct_ret(ipType));
                        igmp_trapDataPkt_lock = TRUE;
                    }
                 }
            }
            else
            {
                pGroup->lookupTableFlag = L2TABLE_WROTE;
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                if (IS_CHIP_CSW())
                {
                    if (MULTICAST_TYPE_IPV4 == ipType)
                    {
                        pGroup->groupId = ip46McstEntry.ipMcast.groupId;
                    }
                    else
                    {
                        pGroup->groupId = ip46McstEntry.ipv6Mcast.groupId;
                    }
                }
#endif
                mcast_hw_group_num_cal(TRUE);
            }
        }
    }

    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_PROTO_IGMP_FILTERING
int32 mcast_profile_get(multicast_ipType_t ipType, sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    SYS_PARAM_CHK((ipType > MULTICAST_TYPE_END || ipType < MULTICAST_TYPE_IPV4), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_IGMP_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    if (0 ==  profile_db[ipType][profileId-1].valid)
    {
        if (MULTICAST_TYPE_IPV4 == ipType)
            return SYS_ERR_IGMP_PROFILE_NOT_EXIST;
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        else
            return SYS_ERR_MLD_PROFILE_NOT_EXIST;
#endif
    }
    osal_memcpy(pProfile, &profile_db[ipType][profileId-1], sizeof(sys_igmp_profile_t));
    return SYS_ERR_OK;
}

int32 mcast_profile_set(multicast_ipType_t ipType, sys_igmp_profileId_t profileId, sys_igmp_profile_t *pProfile)
{
    SYS_PARAM_CHK((ipType > MULTICAST_TYPE_END || ipType < MULTICAST_TYPE_IPV4), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK((profileId > BOND_MCAST_PROFILE_MAX || profileId < BOND_MCAST_PROFILE_MIN), SYS_ERR_IGMP_PROFILE_ID);
    SYS_PARAM_CHK((NULL == pProfile), SYS_ERR_NULL_POINTER);

    osal_memcpy(&profile_db[ipType][profileId-1], pProfile, sizeof(sys_igmp_profile_t));
    return SYS_ERR_OK;
}

int32 mcast_port_filter_set(multicast_ipType_t ipType, sys_logic_port_t port, sys_igmp_profileId_t  profileId)
{
    uint32 i;
    int32  tmpCnt;
    sys_igmp_profile_t profile;
    sys_logic_portmask_t delPmsk;
    igmp_group_entry_t *pGroup = NULL;
    mcast_groupAddr_t   groupAddr;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    if ( profileId == port_profileId[ipType][port])
        return SYS_ERR_OK;

    port_profileId[ipType][port] = profileId;

    if (INVALID_PROFILE_ID == profileId)
        return SYS_ERR_OK;

    osal_memset(&profile, 0, sizeof(sys_igmp_profile_t));
    SYS_ERR_CHK(mcast_profile_get(ipType, profileId, &profile));

    IGMP_DB_SEM_LOCK();

    LOGIC_PORTMASK_CLEAR_ALL(delPmsk);
    LOGIC_PORTMASK_SET_PORT(delPmsk, port);

    mcast_group_rx_timer_stop();
    for (i = 0; i < mcast_group_num; i++)
    {
        pGroup = mcast_group_get_with_index(i);
        if (NULL == pGroup)
            continue;

        if (IGMP_GROUP_ENTRY_STATIC == pGroup->form)
            continue;

        if (ipType != pGroup->ipType)
            continue;
        if (IS_LOGIC_PORTMASK_PORTSET(pGroup->mbr, port))
        {
            osal_memset(&groupAddr, 0, sizeof(groupAddr));
            if ( MULTICAST_TYPE_IPV4 == ipType)
                groupAddr.ipv4 = pGroup->dip;
            else
                osal_memcpy(groupAddr.ipv6.addr, pGroup->dipv6.addr, CAPA_IPV6_BYTE_NUM);

            if (SYS_ERR_OK == mcast_filter_check(ipType, port, &groupAddr))
                continue;

            tmpCnt = mcast_group_num;
            mcast_groupPort_remove(pGroup, port);
            if (mcast_group_num != tmpCnt)
                i--;
        }
    }

    mcast_group_rx_timer_start();
    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}

int32 mcast_port_filter_get(multicast_ipType_t ipType,sys_logic_port_t port, sys_igmp_profileId_t  *pProfileId)
{

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    *pProfileId = port_profileId[ipType][port];
    return SYS_ERR_OK;
}
#endif

static int32 _mcast_group_timer_dmacvid(void)
{
    int32                   ret;
    uint16                  i, time;
    sys_logic_port_t        port;
    igmp_group_entry_t      *pGroup, *pNextgroup;
    sys_logic_portmask_t    delPmsk;
    int32                   tmpNum;
    igmp_aggregate_entry_t  *pAggrEntry;
    igmp_querier_entry_t    *pQuerEntry;
    sys_l2McastEntry_t      l2McstEntry;
    igmp_router_entry_t     *pRouterEntry;

    mcast_vlan_entry_t *pMcastVlan = NULL;
    igmp_lookup_mode_t lookup_mode;
    uint8 isFwdAll = TRUE;

    mcast_groupAdd_stop = ENABLED;
    osal_time_usleep(200);

    _mcast_vlan_event_output();

    osal_memset(&l2McstEntry, 0, sizeof(sys_l2McastEntry_t));

    IGMP_DB_SEM_LOCK();

    for (i = 0; i < group_sortedAry_entry_num; i++)
    {
       if (ENABLED == mcast_timer_stop)
       {
            IGMP_DB_SEM_UNLOCK();
            mcast_groupAdd_stop = DISABLED;
            return SYS_ERR_OK;
       }

        if (group_sortedArray[i] == NULL)
        {
            if (i != (group_sortedAry_entry_num - 1))
                MCASTDBG("Warring! group_sortedArray has hole!");

            break;
        }

        pGroup = group_sortedArray[i];
        rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);
        if (IGMP_DMAC_VID_MODE != lookup_mode)
            continue;

        while (pGroup)
        {
            osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
            pNextgroup = pGroup->next_subgroup;

            if(IGMP_GROUP_ENTRY_STATIC == pGroup->form)
            {
                pGroup = pNextgroup;
                continue;
            }
            if (IS_LOGIC_PORTMASK_CLEAR(pGroup->mbr))
            {
                time = pGroup->groupTimer;
                if (time == 0)
                {
                    if (pGroup->ipType == MULTICAST_TYPE_IPV4)
                        MCASTDBG("%s():%d  Warring! an existing mbr whose life is zero", __FUNCTION__, __LINE__);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                    else
                        MCASTDBG("%s():%d  Warring! an existing mbr whose life is zero", __FUNCTION__, __LINE__);
#endif
                
                #if 0 
                osal_printf("%s():%d  Warring! an existing mbr whose life is zero\n", __FUNCTION__, __LINE__);
                ret = mcast_hw_mcst_entry_del(pGroup);
                if (ret == SYS_ERR_OK)
                        igmp_hw_entry_num--;

                tmpNum = group_sortedAry_entry_num;
                if (SYS_ERR_OK != mcast_group_del(pGroup))
                    osal_printf("group delete failure !\n");
                if (tmpNum != group_sortedAry_entry_num)
                    i--;
                #endif
                }
                else
                {
                    if (time >= IGMP_TEMP_PERIOD)
                    {
                        pGroup->groupTimer -= PASS_SECONDS;
                    }
                    else if (time == 2)
                    {
                        l2McstEntry.vid = pGroup->vid;
                        osal_memcpy(l2McstEntry.mac, pGroup->mac, CAPA_MAC_BYTE_NUM);
                        mcast_hw_l2McastEntry_get(&l2McstEntry);
                        l2McstEntry.crossVlan = ENABLED;
                        rsd_l2McastEntryIncCPU_set(&l2McstEntry);
                        pGroup->groupTimer -= PASS_SECONDS;
                    }
                    else
                    {
                        MCASTDBG("Group Timeout of IP = "IPADDR_PRINT"", IPADDR_PRINT_ARG(pGroup->dip));

                        ret = mcast_hw_mcst_entry_del(pGroup);
                        if (ret == SYS_ERR_OK)
                                mcast_hw_group_num_cal(FALSE);
                        tmpNum = group_sortedAry_entry_num;
                        mcast_group_del(pGroup);
                        if (tmpNum != group_sortedAry_entry_num)
                            i--;
                    }
                }

                pGroup = pNextgroup;
                continue;
            }

            mcast_vlan_db_get(pGroup->vid, pGroup->ipType, &pMcastVlan);
            if (NULL == pMcastVlan)
            {
                pGroup = pNextgroup;
                continue;
            }

            mcast_querier_db_get(pGroup->vid, pGroup->ipType, &pQuerEntry);
            if (pQuerEntry == NULL)
            {
                if (pGroup->ipType == MULTICAST_TYPE_IPV4)
                    MCASTDBG("%s():%d VLAN-%d doesn't have query DB entry!", __FUNCTION__, __LINE__, pGroup->vid);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                    MCASTDBG("%s():%d VLAN-%d doesn't have query DB entry!", __FUNCTION__, __LINE__, pGroup->vid);
#endif
                IGMP_DB_SEM_UNLOCK();
                mcast_groupAdd_stop = DISABLED;
                return SYS_ERR_OK;
            }

            #ifdef CONFIG_SYS_PROTO_MVR
            isFwdAll = mcast_mvr_fwdAll_ret(pGroup);
            #endif

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pGroup->mbr)
            {
                time = pGroup->mbrTimer[port];
                if (time == 0)
                {
                    if (pGroup->ipType == MULTICAST_TYPE_IPV4)
                        MCASTDBG("%s():%d  Warring! an existing mbr whose life is zero", __FUNCTION__, __LINE__);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                    else
                        MCASTDBG("%s():%d  Warring! an existing mbr whose life is zero", __FUNCTION__, __LINE__);
#endif
                }
                else
                {
                    if (time > PASS_SECONDS)
                        pGroup->mbrTimer[port] -= PASS_SECONDS;
                    else
                    {
                        if ((pQuerEntry->status == IGMP_QUERIER && (pGroup->gsQueryCnt[port] >= 1 && pGroup->gsQueryCnt[port] <= pMcastVlan->operLastMmbrQueryCnt))
                            #ifdef CONFIG_SYS_PROTO_MVR
                             || (pGroup->gsQueryCnt[port] ==1 && (is_mvr_group_ret2(pGroup)))
                             #endif
                             )
                        {
                            #ifndef CONFIG_SYS_PROTO_MVR
                            pGroup->mbrTimer[port] = pMcastVlan->operGsqueryIntv;
                            pGroup->gsQueryCnt[port]++;
                            mcast_send_gs_query(pGroup, port);
                            #else
                            if (is_mvr_group_ret2(pGroup) && (MVR_MODE_COMPATABLE == mcast_mvr_mode_ret(pGroup->ipType)))
                            {
                                pGroup->mbrTimer[port] = igmp_cfg.mvrConfig.queryTime;
                                pGroup->gsQueryCnt[port] = 0;
                                mcast_igmp_mvr_send_specific_query(pGroup->vid, pGroup->dip, port);
                            }
                            else
                            {
                                pGroup->mbrTimer[port] = pMcastVlan->operGsqueryIntv;
                                pGroup->gsQueryCnt[port]++;
                                mcast_send_gs_query(pGroup, port);
                            }
                            #endif
                        }
                        else
                        {
                            MCASTDBG("Group membership Timeout on Port-%s for IP = "IPADDR_PRINT"",
                                LPSTR(port) , IPADDR_PRINT_ARG(pGroup->dip));
                            pGroup->mbrTimer[port] = 0;
                            LOGIC_PORTMASK_CLEAR_PORT(pGroup->mbr, port);

                            LOGIC_PORTMASK_CLEAR_ALL(delPmsk);
                            LOGIC_PORTMASK_SET_PORT(delPmsk, port);
                            mcast_hw_mcst_mbr_remove(pGroup, &delPmsk);

                            if (IS_LOGIC_PORTMASK_CLEAR(pGroup->mbr))
                            {
                                if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(pGroup->ipType))
                                {
                                    mcast_aggregate_db_get(pGroup, &pAggrEntry,TRUE);
                                    if (pAggrEntry == NULL)
                                    {
                                         IGMP_DB_SEM_UNLOCK();
                                         mcast_groupAdd_stop = DISABLED;
                                        return SYS_ERR_OK;
                                    }

                                    if (pAggrEntry->group_ref_cnt == 1)
                                    {
                                        pGroup->groupTimer = IGMP_TEMP_PERIOD;

                                        l2McstEntry.vid = pGroup->vid;
                                        osal_memcpy(l2McstEntry.mac, pGroup->mac, CAPA_MAC_BYTE_NUM);
                                        mcast_router_db_get(pGroup->vid, pGroup->ipType, &pRouterEntry);
                                        if (pRouterEntry)
                                            LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, pRouterEntry->router_pmsk);
                                        else
                                            MCASTDBG("Warring! This vlan has no router port!");

                                        ret = mcast_hw_l2McastEntry_set(pGroup->ipType, &l2McstEntry, TRUE, isFwdAll);
                                    }
                                    else
                                    {
                                        pAggrEntry->group_ref_cnt--;
                                        tmpNum = group_sortedAry_entry_num;
                                        mcast_group_del(pGroup);
                                        if (tmpNum != group_sortedAry_entry_num)
                                            i--;        
                                    }
                                }
                                else
                                {
                                    
                                    ret = mcast_hw_mcst_entry_del(pGroup);
                                    if (ret == SYS_ERR_OK)
                                        mcast_hw_group_num_cal(FALSE);
                                   tmpNum =  group_sortedAry_entry_num;
                                   mcast_group_del(pGroup);
                                   if (tmpNum != group_sortedAry_entry_num)
                                        i--;
                                 }
                           }
                    }
                }
            }
        }
        pGroup = pNextgroup;
       }
    }

    IGMP_DB_SEM_UNLOCK();
    mcast_groupAdd_stop = DISABLED;
    return SYS_ERR_OK;
}

static int32 _mcast_group_timer_dipsip(void)
{
    int32                   ret;
    uint16                  i, time, flag = FALSE;
    sys_logic_port_t        port;
    igmp_group_entry_t      *pGroup, *pNextgroup;
    sys_logic_portmask_t    delPmsk;
    int32                   tmpNum;
    uint32                  isClear;
    igmp_querier_entry_t    *pQuerEntry;
    sys_ipv46McastEntry_t      ip46McstEntry;
    igmp_router_entry_t     *pRouterEntry;

    igmp_group_entry_t *pEntry = NULL;
    igmp_group_entry_t  tmpGroup;

    mcast_vlan_entry_t *pMcastVlan = NULL;
    igmp_lookup_mode_t lookup_mode;
    sys_vlan_operEntry_t vlanEntry;
    uint8 isFwdAll = TRUE;

    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    uint16 sortedIdx;
    igmp_group_entry_t  *groupHead;
    #endif

    mcast_groupAdd_stop = ENABLED;
    osal_time_usleep(200);

    IGMP_DB_SEM_LOCK();
    for (i = 0; i < group_sortedAry_entry_num; i++)
    {
       if (ENABLED == mcast_timer_stop)
       {
            IGMP_DB_SEM_UNLOCK();
            mcast_groupAdd_stop = DISABLED;
            return SYS_ERR_OK;
       }

        if (group_sortedArray[i] == NULL)
        {
            if (i != (group_sortedAry_entry_num - 1))
                MCASTDBG("Warring! group_sortedArray has hole!");

            break;
        }

        pGroup = group_sortedArray[i];
        rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);
        if (IGMP_DMAC_VID_MODE == lookup_mode)
            continue;

        while (pGroup)
        {
            flag = FALSE;
            osal_memset(&delPmsk, 0, sizeof(sys_logic_portmask_t));
            pNextgroup = pGroup->next_subgroup;

            LOGIC_PORTMASK_IS_CLEAR(pGroup->mbr, isClear);
            if (isClear)
            {
                if (IGMP_GROUP_ENTRY_STATIC == pGroup->form)
                {
                    pGroup = pNextgroup;
                    continue;
                }
                time = pGroup->groupTimer;
                if (time == 0)
                {
                    if (pGroup->ipType == MULTICAST_TYPE_IPV4)
                        MCASTDBG("%s():%d  Warring! an existing mbr whose life is zero", __FUNCTION__, __LINE__);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                    else
                        MCASTDBG("%s():%d  Warring! an existing mbr whose life is zero", __FUNCTION__, __LINE__);
#endif
                }
                else
                {
                    if (time > PASS_SECONDS + 1)
                    {
                        pGroup->groupTimer -= PASS_SECONDS;
                    }
                    else if (time == 2)
                    {
                        
                        if (SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46McstEntry)))
                        {
                            mcast_hw_ipMcastEntryIncCPU_set(pGroup->ipType, &ip46McstEntry);
                        }
                        pGroup->groupTimer -= PASS_SECONDS;
                    }
                    else
                    {
                        MCASTDBG("Group Timeout of IP = "IPADDR_PRINT"", IPADDR_PRINT_ARG(pGroup->dip));
                        ret = mcast_hw_mcst_entry_del(pGroup);
                        if(IGMP_VERSION_V3_FULL == igmp_cfg.oper_version)
                        {
                            SYS_ERR_CHK(mcast_build_groupEntryKey(pGroup->vid, pGroup->ipType, pGroup->dip, pGroup->dipv6.addr, &tmpGroup));
                            tmpGroup.sip = pGroup->sip;
                        }

                        if (ret == SYS_ERR_OK)
                        {
                           
                            mcast_hw_group_num_cal(FALSE);
                        }

                        tmpNum = group_sortedAry_entry_num;
                        mcast_group_del(pGroup);

                        #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
                        if(IGMP_VERSION_V3_FULL == igmp_cfg.oper_version)
                        {
                            mcast_group_sortedArray_search(mcast_group_sortKey_ret(&tmpGroup), &sortedIdx, &groupHead);
                            if(groupHead == NULL)
                            {
                                mcast_igmp_group_head_del(tmpGroup.dip, tmpGroup.vid);
                            }
                        }
                        #endif

                        if (tmpNum != group_sortedAry_entry_num)
                            i--;
                    }
                }

                pGroup = pNextgroup;
                continue;
            }

            mcast_vlan_db_get(pGroup->vid, pGroup->ipType, &pMcastVlan);
            if (NULL == pMcastVlan)
            {
                pGroup = pNextgroup;
                continue;
            }

            mcast_querier_db_get(pGroup->vid, pGroup->ipType, &pQuerEntry);
            if (pQuerEntry == NULL)
            {
                if (pGroup->ipType == MULTICAST_TYPE_IPV4)
                    MCASTDBG("%s():%d VLAN-%d doesn't have query DB entry!", __FUNCTION__, __LINE__, pGroup->vid);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                else
                    MCASTDBG("%s():%d VLAN-%d doesn't have query DB entry!", __FUNCTION__, __LINE__, pGroup->vid);
#endif

                IGMP_DB_SEM_UNLOCK();
                mcast_groupAdd_stop = DISABLED;
                return SYS_ERR_FAILED;
            }

            #ifdef CONFIG_SYS_PROTO_MVR
            isFwdAll = mcast_mvr_fwdAll_ret(pGroup);
            #endif

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pGroup->mbr)
            {
                
                if(IS_LOGIC_PORTMASK_PORTSET(pGroup->staticMbr, port))
                    continue;
                time = pGroup->mbrTimer[port];
                if (time == 0)
                {
                    if (pGroup->ipType == MULTICAST_TYPE_IPV4)
                        MCASTDBG("Warring! an existing mbr whose life is zero");
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                    else
                        MCASTDBG("Warring! an existing mbr whose life is zero");
#endif
                }
                else
                {
                    if (time > PASS_SECONDS)
                        pGroup->mbrTimer[port] -= PASS_SECONDS;
                    else
                    {
                        
                        if ((pQuerEntry->status == IGMP_QUERIER && (pGroup->gsQueryCnt[port] >= 1 && pGroup->gsQueryCnt[port] <= pMcastVlan->operLastMmbrQueryCnt)) ||
                            (pQuerEntry->status == IGMP_QUERIER && (pGroup->v3.gssQueryCnt[port] >= 1 && pGroup->v3.gssQueryCnt[port] <= pMcastVlan->operLastMmbrQueryCnt))
                            #ifdef CONFIG_SYS_PROTO_MVR
                             || (pGroup->gsQueryCnt[port] ==1 && (is_mvr_group_ret2(pGroup)))
                             #endif
                            )
                        {
                            if((pQuerEntry->status == IGMP_QUERIER && (pGroup->gsQueryCnt[port] >= 1 && pGroup->gsQueryCnt[port] <= pMcastVlan->operLastMmbrQueryCnt))
                            #ifdef CONFIG_SYS_PROTO_MVR
                             || (pGroup->gsQueryCnt[port] ==1 && (is_mvr_group_ret2(pGroup)))
                             #endif
                            )
                            {
                                #ifndef CONFIG_SYS_PROTO_MVR
                                pGroup->mbrTimer[port] = pMcastVlan->operGsqueryIntv;
                                pGroup->gsQueryCnt[port]++;
                                mcast_send_gs_query(pGroup, port);
                                #else
                                if (is_mvr_group_ret2(pGroup) && (MVR_MODE_COMPATABLE == mcast_mvr_mode_ret(pGroup->ipType)))
                                {
                                    pGroup->mbrTimer[port] = igmp_cfg.mvrConfig.queryTime;
                                    pGroup->gsQueryCnt[port] = 0;
                                    mcast_igmp_mvr_send_specific_query(pGroup->vid, pGroup->dip, port);
                                 }
                                else
                                {
                                    pGroup->mbrTimer[port] = pMcastVlan->operGsqueryIntv;
                                    pGroup->gsQueryCnt[port]++;
                                    mcast_send_gs_query(pGroup, port);
                                }
                                #endif
                            }
                            else
                            {
                                if(IGMP_VERSION_V3_FULL == igmp_cfg.oper_version)
                                {
                                    if((pQuerEntry->status == IGMP_QUERIER && (pGroup->v3.gssQueryCnt[port] >= 1 && pGroup->v3.gssQueryCnt[port] <= pMcastVlan->operLastMmbrQueryCnt)))
                                    {
                                        pGroup->mbrTimer[port] = pMcastVlan->operGsqueryIntv;

                                        mcast_igmp_send_grp_src_specific_query(pGroup->vid, pGroup->dip, &pGroup->sip, 1,port);
                                        pEntry->v3.gssQueryCnt[port] ++;
                                    }
                                }
                            }
                        }

                        else
                        {
                            MCASTDBG("Group membership Timeout on Port-%s for IP = "IPADDR_PRINT"",
                                LPSTR(port), IPADDR_PRINT_ARG(pGroup->dip));
                            pGroup->mbrTimer[port] = 0;
                            LOGIC_PORTMASK_CLEAR_PORT(pGroup->mbr, port);

                            if(IGMP_VERSION_V3_FULL == igmp_cfg.oper_version)
                            {
                                if(IS_LOGIC_PORTMASK_PORTSET(pGroup->v3.isGssQueryFlag, port))
                                    LOGIC_PORTMASK_CLEAR_PORT(pGroup->v3.isGssQueryFlag, port);
                            }

                            flag = TRUE;
                            LOGIC_PORTMASK_SET_PORT(delPmsk, port);
                        }

                    }
                }
            }

            if (flag)
            {
                if (SYS_ERR_OK != (ret = mcast_hw_mcst_mbr_remove(pGroup, &delPmsk)))
                {
                    MCASTDBG("Deleting Mcst mbr failed!  ret:%d", ret);
                }

                if (IS_LOGIC_PORTMASK_CLEAR(pGroup->mbr))
                {
                    
                    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
                    {
                        mcast_ipMcast_asicKey_get(pGroup->vid, pGroup->dip, pGroup->sip, &ip46McstEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                        if (IS_CHIP_CSW())
                        {
                            ip46McstEntry.ipMcast.groupId = pGroup->groupId;
                        }
#endif
                    }
                    else
                    {
                        mcast_ipv6Mcast_asicKey_get(pGroup->vid,pGroup->dipv6.addr, pGroup->sipv6.addr,&ip46McstEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
                        if (IS_CHIP_CSW())
                        {
                            ip46McstEntry.ipv6Mcast.groupId = pGroup->groupId;
                        }
#endif
                    }
                    LOGIC_PORTMASK_CLEAR_ALL(ip46McstEntry.portmask);

                    if (IGMP_DIP_SIP_MODE == lookup_mode)
                    {
                        
                        pGroup->groupTimer = IGMP_TEMP_PERIOD;

                        if (IGMP_UNKNOWN_MCAST_ACTION_FLOOD == mcast_unknownAct_ret(pGroup->ipType) )
                        {

                            rsd_vlan_operEntry_get(pGroup->vid, &vlanEntry);
                            LOGIC_PORTMASK_COPY(ip46McstEntry.portmask, vlanEntry.lpmMember);
                        }
                        else if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(pGroup->ipType))
                        {
                            mcast_router_db_get(pGroup->vid, pGroup->ipType, &pRouterEntry);
                            if (pRouterEntry)
                                LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, pRouterEntry->router_pmsk);
                            else
                                MCASTDBG("Warring! This vlan has no router port!");
                        }

                        ret = mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46McstEntry, TRUE,isFwdAll);
                    }
                    else
                    {
                        
                        if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(pGroup->ipType))
                        {
                            pGroup->groupTimer = IGMP_TEMP_PERIOD;
                        }
                        else
                        {
                            
                            ret = mcast_hw_mcst_entry_del(pGroup);
                            if (ret == SYS_ERR_OK)
                            {
                                mcast_hw_group_num_cal(FALSE);
                            }
                            tmpNum = group_sortedAry_entry_num;
                            mcast_group_del(pGroup);
                            if (tmpNum != group_sortedAry_entry_num)
                                i--;
                            pGroup = pNextgroup;
                            continue;
                        }
                    }
                }
            }
            pGroup = pNextgroup;
        }
    }
    IGMP_DB_SEM_UNLOCK();
     mcast_groupAdd_stop = DISABLED;
    return SYS_ERR_OK;
}

void mcast_tick_thread(void *pInput)
{
    igmp_lookup_mode_t igmp_lookup_mode, mld_lookup_mode;
    osal_usecs_t  startTime, endTime;
    int32 interval;

    for (;;)
    {
        startTime = 0;
        endTime = 0;
        interval = 0;

        osal_time_usecs_get(&startTime);
        mcast_igmp_querier_timer();
        mcast_routerPort_timer();

        if(IGMP_VERSION_V3_FULL == igmp_cfg.oper_version)
            mcast_igmp_group_head_timer();

        rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4, &igmp_lookup_mode);

        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6, &mld_lookup_mode);
        #else
            mld_lookup_mode = igmp_lookup_mode;
        #endif

        if (igmp_lookup_mode == mld_lookup_mode)
        {
            igmp_lookup_mode_func[igmp_lookup_mode].igmp_group_timer();
        }
        else
        {
             igmp_lookup_mode_func[igmp_lookup_mode].igmp_group_timer();
            igmp_lookup_mode_func[1 * IGMP_LOOKUP_MODE_END + mld_lookup_mode].igmp_group_timer();
        }
        osal_time_usecs_get(&endTime);
        interval = endTime - startTime;
        interval = PASS_SECONDS * 1000 * 1000 - interval;
        if (interval > 0 )
            osal_time_usleep(interval);  

        if (igmp_thread_status)
        {
            break;
        }
    }

    igmp_thread_status = -1;

    osal_thread_exit(0);
}
int32 mcast_vlan_get(mcast_vlan_entry_t *pEntry)
{
    mcast_vlan_entry_t  *tmp = NULL;

    SYS_PARAM_CHK(IS_VID_INVALID(pEntry->vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((pEntry->ipType < MULTICAST_TYPE_IPV4 ||pEntry->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    mcast_vlan_db_get(pEntry->vid, pEntry->ipType, &tmp);

    if(tmp)
    {
        osal_memcpy(pEntry,tmp,sizeof(mcast_vlan_entry_t));
    }
    else
    {
        osal_memset(pEntry,0,sizeof(mcast_vlan_entry_t));
    }
    return SYS_ERR_OK;

}
int32 mcast_vlan_set(mcast_vlan_entry_t *pEntry)
{
    mcast_vlan_entry_t tmp;
    sys_vlanmask_t vlanMask;

    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
    igmp_group_entry_t *pGroup = NULL;
    sys_l2McastEntry_t  l2Mcst_entry;
    sys_ipv46McastEntry_t ip46Mcast_entry;
    sys_logic_portmask_t  delStaticPmsk, aggratePmk;
    igmp_router_entry_t router;
    int32 i, ret;
    igmp_lookup_mode_t lookup_mode;
    uint8 isFwdAll = TRUE;
    #endif

    SYS_PARAM_CHK(IS_VID_INVALID(pEntry->vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((pEntry->ipType < MULTICAST_TYPE_IPV4 ||pEntry->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    osal_memset(&tmp, 0, sizeof(mcast_vlan_entry_t));
    tmp.vid = pEntry->vid;
    tmp.ipType = pEntry->ipType;
    mcast_vlan_get(&tmp);

    VLANMASK_CLEAR_ALL(vlanMask);
    VLANMASK_SET_VID(vlanMask, pEntry->vid);

    SYS_ERR_CHK(mcast_vlan_db_set(pEntry));

    if (DISABLED == pEntry->enable)
    {
        if ((0 != tmp.vid) && (tmp.enable == ENABLED))
        {
            IGMP_DB_SEM_LOCK();
            mcast_static_group_save(vlanMask, pEntry->ipType);
            _mcast_hwEntryByVid_del(&vlanMask, pEntry->ipType, IGMP_GROUP_ENTRY_ALL, MCAST_ENTRY_SNP);
            IGMP_DB_SEM_UNLOCK();
            SYS_ERR_CHK(mcast_static_group_restore());
            SYS_ERR_CHK(mcast_router_dynPortMbr_del(vlanMask, pEntry->ipType));
        }
    }
    else
    {
        SYS_ERR_CHK(mcast_static_group_hwRestore(vlanMask, pEntry->ipType));
    }

    #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    SYS_ERR_CHK(mcast_querier_enable_set(pEntry->ipType, &vlanMask, pEntry->enable));
    #endif

    VLANMASK_SET_VID(mcastVlanCfgMsk[pEntry->ipType], pEntry->vid);

    #ifdef CONFIG_SYS_PROTO_IGMP_STATIC_FORWARD
    if (0 !=  tmp.vid)
    {
        if (IS_LOGIC_PORTMASK_EQUAL(tmp.staticMbr, pEntry->staticMbr) &&
            IS_LOGIC_PORTMASK_EQUAL(tmp.forbidMbr, pEntry->forbidMbr))
        {
            ;  
        }
        else
        {
            LOGIC_PORTMASK_CLEAR_ALL(delStaticPmsk);

            LOGIC_PORTMASK_ANDNOT(delStaticPmsk, tmp.staticMbr, pEntry->staticMbr);

            IGMP_DB_SEM_LOCK();
            for (i = 0; i < mcast_group_num; i++)
            {
                pGroup = mcast_group_get_with_index(i);
                if (pGroup == NULL)
                    continue;

                if (pEntry->ipType != pGroup->ipType)
                    continue;

                if (pGroup->vid != pEntry->vid)
                    continue;

                if (!is_mcast_group(pGroup))
                    continue;

                osal_memset(&router, 0, sizeof(igmp_router_entry_t));
                router.vid = pEntry->vid;
                router.ipType = pGroup->ipType;
                mcast_router_get(&router);

                #ifdef CONFIG_SYS_PROTO_MVR
                isFwdAll = mcast_mvr_fwdAll_ret(pGroup);
                #endif

                rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);
                if ((IGMP_DMAC_VID_MODE == lookup_mode) || (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != pGroup->ipType))
                {
                    if (IGMP_DMAC_VID_MODE == lookup_mode)
                    {
                        l2Mcst_entry.vid = pEntry->vid;
                        osal_memcpy(l2Mcst_entry.mac, pGroup->mac, CAPA_MAC_BYTE_NUM);
                        ret = mcast_hw_l2McastEntry_get(&l2Mcst_entry);

                        if (SYS_ERR_OK == ret)
                        {
                            if (!IS_LOGIC_PORTMASK_CLEAR(delStaticPmsk))
                            {
                                LOGIC_PORTMASK_ANDNOT(l2Mcst_entry.portmask, l2Mcst_entry.portmask, delStaticPmsk);
                            }

                             mcast_aggregatePort_get(pGroup, &aggratePmk);
                             LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, aggratePmk);
                             LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, router.router_pmsk);
                             LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, pGroup->mbr);

                             ret = mcast_hw_l2McastEntry_set(pGroup->ipType, &l2Mcst_entry, TRUE, isFwdAll);
                         }
                    }
                    else
                    {
                        ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46Mcast_entry);
                        if (SYS_ERR_OK == ret)
                        {
                            if (!IS_LOGIC_PORTMASK_CLEAR(delStaticPmsk))
                            {
                                LOGIC_PORTMASK_ANDNOT(ip46Mcast_entry.portmask, ip46Mcast_entry.portmask, delStaticPmsk);
                            }

                             mcast_aggregatePort_get(pGroup, &aggratePmk);
                             LOGIC_PORTMASK_OR(ip46Mcast_entry.portmask, ip46Mcast_entry.portmask, aggratePmk);
                             LOGIC_PORTMASK_OR(ip46Mcast_entry.portmask, ip46Mcast_entry.portmask, router.router_pmsk);
                             LOGIC_PORTMASK_OR(ip46Mcast_entry.portmask, ip46Mcast_entry.portmask, pGroup->mbr);

                             ret = mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46Mcast_entry, TRUE, isFwdAll);
                        }
                    }

                    if (SYS_ERR_OK != ret)
                    {
                        IGMP_DB_SEM_UNLOCK();
                        return ret;
                    }
                }
                else
                {
                    
                    if (IGMP_DIP_VID_MODE == lookup_mode)
                    {
                        SYS_MEM_CLEAR(ip46Mcast_entry);
                        if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46Mcast_entry)))
                        {
                            MCASTDBG("Getting ip Mcst enrty failed! ret:%d   vid:%d", ret, pEntry->vid);
                        }
                        else
                        {
                            if (!IS_LOGIC_PORTMASK_CLEAR(delStaticPmsk))
                            {
                                LOGIC_PORTMASK_ANDNOT(ip46Mcast_entry.portmask, ip46Mcast_entry.portmask, delStaticPmsk);
                            }

                            LOGIC_PORTMASK_OR(ip46Mcast_entry.portmask, ip46Mcast_entry.portmask, router.router_pmsk);
                            LOGIC_PORTMASK_OR(ip46Mcast_entry.portmask, ip46Mcast_entry.portmask, pGroup->mbr);

                            if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46Mcast_entry,TRUE, isFwdAll)))
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
    #endif

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 mcast_vlan_add(mcast_vlan_entry_t *pEntry)
{
    SYS_PARAM_CHK(IS_VID_INVALID(pEntry->vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((pEntry->ipType < MULTICAST_TYPE_IPV4 ||pEntry->ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

     return mcast_vlan_db_add(pEntry->vid, pEntry->ipType);
}
int32 mcast_vlan_del(sys_vid_t vid, multicast_ipType_t ipType)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    return mcast_vlan_db_del(vid, ipType);
}
int32 mcast_vlanMskConfig_get(multicast_ipType_t ipType, sys_vlanmask_t *pBitmap)
{
    SYS_PARAM_CHK((NULL == pBitmap), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    osal_memcpy(pBitmap,  &mcastVlanCfgMsk[ipType], sizeof(sys_vlanmask_t));

    return SYS_ERR_OK;
}
int32 mcast_igmp_suppre_enable_set(sys_enable_t enable)
{
    igmp_cfg.suppre_enable = enable;
    return SYS_ERR_OK;
}
int32 mcast_igmp_statistics_clear(void)
{
    sys_logic_port_t lport;

    FOR_EACH_LOGIC_PORT(lport)
    {
        SYS_MEM_CLEAR(igmp_stats[lport]);
    }

    return SYS_ERR_OK;
}
int32 mcast_igmp_portStat_clear(sys_logic_port_t lport)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(lport), SYS_ERR_PORT_ID);
    SYS_MEM_CLEAR(igmp_stats[lport]);
    return SYS_ERR_OK;
}
int32 mcast_igmp_operVersion_get(igmp_version_t *pVersion)
{
    SYS_PARAM_CHK((NULL == pVersion), SYS_ERR_NULL_POINTER);
    *pVersion = igmp_cfg.oper_version;
    return SYS_ERR_OK;
}
int32 mcast_igmp_operVersion_set(uint8 operVersion)
{
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    sys_vlanmask_t vlanMsk;
    sys_vid_t vid;
    sys_igmp_querier_t qEntry;

    VLANMASK_CLEAR_ALL(vlanMsk);
#endif

    if (operVersion == igmp_cfg.oper_version)
        return SYS_ERR_OK;

    igmp_cfg.oper_version = operVersion;

   #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    if (IGMP_VERSION_V2 == operVersion)
    {
        rsd_vlan_operEntryExist_get(&vlanMsk);
        FOR_EACH_VID_IN_VLANMASK(vid, vlanMsk)
        {
            osal_memset(&qEntry,0,sizeof(sys_igmp_querier_t));
            qEntry.vid = vid;
            qEntry.ipType = MULTICAST_TYPE_IPV4;
            mcast_igmp_querier_get(&qEntry);
            
            {
                qEntry.version = IGMP_QUERY_V2;
                mcast_igmp_querier_set(&qEntry);
            }
        }
    }
    #endif

    return SYS_ERR_OK;
}
int32 mcast_igmp_info_get(sys_mcast_info_t *pInfo)
{
    sys_logic_port_t lport;
    int32 i;

    SYS_PARAM_CHK(NULL == pInfo, SYS_ERR_NULL_POINTER);

    pInfo->enable                   = igmp_cfg.igmpsnp_enable;
    pInfo->suppreEnable         = igmp_cfg.suppre_enable;
    pInfo->unknMcastActn            = mcast_unknownAct_ret(MULTICAST_TYPE_IPV4);
    pInfo->operVersion             = igmp_cfg.oper_version;

    osal_memset(pInfo->totStat, 0, sizeof(pInfo->totStat));

    FOR_EACH_LOGIC_PORT(lport)
    {
        for (i = 0; i < MCAST_STAT_END; i++)
        {
            pInfo->totStat[i] += igmp_stats[lport].igmpStatCnt[i];
        }
    }

    return SYS_ERR_OK;
}
int32 mcast_igmp_portStat_get(sys_logic_port_t lport, sys_mcast_portStat_t *pPortStat)
{
    SYS_PARAM_CHK(NULL == pPortStat, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(lport), SYS_ERR_PORT_ID);

    osal_memset(pPortStat->totStat,0,sizeof(pPortStat->totStat));

    osal_memcpy(pPortStat->totStat, igmp_stats[lport].igmpStatCnt, sizeof(pPortStat->totStat));

    return SYS_ERR_OK;
}
int32 mcast_igmp_lookupMode_set(igmp_lookup_mode_t mode)
{
    igmp_lookup_mode_t lookup_mode;
    sys_vlanmask_t vlanMsk;

    VLANMASK_SET_ALL(vlanMsk);
    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4, &lookup_mode);

    if(mode != lookup_mode)
    {
        SYS_ERR_CHK(mcast_static_group_save(vlanMsk, MULTICAST_TYPE_IPV4));

        if(ENABLED == igmp_cfg.igmpsnp_enable)
        {
            igmp_packet_stop = ENABLED;

            mcast_ipv4_group_del_byType(IGMP_GROUP_ENTRY_ALL);
            
            rsd_igmp_lookupMode_set(mode);
            igmp_packet_stop = DISABLED;
        }
        else
        {
            
            mcast_ipv4_group_del_byType(IGMP_GROUP_ENTRY_ALL);
            rsd_igmp_lookupMode_set(mode);
        }

        SYS_ERR_CHK(mcast_static_group_restore());
        mcast_igmp_unknMcastAction_set(gUnknown_mcast_igmp_action);
    }

    return SYS_ERR_OK;
}
int32 mcast_igmp_lookupMode_get(igmp_lookup_mode_t *mode)
{
    SYS_PARAM_CHK((NULL == mode), SYS_ERR_NULL_POINTER);

    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4,mode);
    return SYS_ERR_OK;
}
int32 mcast_igmp_group_del_byType( mcast_group_type_t type)
{
    sys_vlanmask_t vlanMask;
    mcast_group_type_t delType = type;

    if (type < IGMP_GROUP_ENTRY_DYNAMIC || type > IGMP_GROUP_ENTRY_ALL)
        return SYS_ERR_MCAST_GROUP_TYPE;

    if (IGMP_GROUP_ENTRY_ALL == type)
        delType = IGMP_GROUP_ENTRY_DYNAMIC | IGMP_GROUP_ENTRY_STATIC;

    VLANMASK_SET_ALL(vlanMask);

    IGMP_DB_SEM_LOCK();
    _mcast_hwEntryByVid_del(&vlanMask, MULTICAST_TYPE_IPV4, delType, MCAST_ENTRY_SNP);
    IGMP_DB_SEM_UNLOCK();
    return  SYS_ERR_OK;
}

int32 mcast_ipv4_group_del_byType(mcast_group_type_t type)
{
    sys_vlanmask_t vlanMask;
    mcast_group_type_t delType = type;

    if (type < IGMP_GROUP_ENTRY_DYNAMIC || type > IGMP_GROUP_ENTRY_ALL)
        return SYS_ERR_MCAST_GROUP_TYPE;

    if (IGMP_GROUP_ENTRY_ALL == type)
        delType = IGMP_GROUP_ENTRY_DYNAMIC | IGMP_GROUP_ENTRY_STATIC;

    VLANMASK_SET_ALL(vlanMask);

    IGMP_DB_SEM_LOCK();
    _mcast_hwEntryByVid_del(&vlanMask, MULTICAST_TYPE_IPV4, delType, MCAST_ENTRY_ALL);
    IGMP_DB_SEM_UNLOCK();
    return  SYS_ERR_OK;
}

void mcast_igmp_snooping_stats_init (int32 boot)
{
    sys_logic_port_t port;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    if (boot)
    {
        osal_memset(&igmp_cfg, 0, sizeof(igmp_cfg_t));

        osal_memset(&profile_db[ipType],0, CAPA_MCAST_PROFILE_NUM * sizeof(sys_igmp_profile_t));

        igmp_cfg.igmpsnp_enable = DISABLED;
        igmp_cfg.suppre_enable = DFLT_IGMP_SUPPRE_EBL;
        igmp_cfg.oper_version = DFLT_IGMP_OPERATION_VER;

        #ifdef CONFIG_SYS_PROTO_MVR
        igmp_cfg.mvrConfig.mvr_enable = DISABLED;
        igmp_cfg.mvrConfig.mvr_vid = DFLT_IGMP_MVR_VLAN;
        igmp_cfg.mvrConfig.isMaxGroup = DISABLED;  
        igmp_cfg.mvrConfig.isFilter = DISABLED;
        igmp_cfg.mvrConfig.isForward = DISABLED;
        #endif

        gUnknown_mcast_igmp_action = DFLT_IGMP_UNKNOWN_MCAST_ACTION;

        FOR_EACH_LOGIC_PORT(port)
        {
            osal_memset(&igmp_stats[port],0,sizeof(igmp_stats_t));
            igmp_cfg.port_entry_limit[port] = CAPA_MCAST_PORT_LIMIT_NUM;
            igmp_cfg.max_groups_act[port] = DFLT_IGMP_MAX_GROUP_ACT;
            port_profileId[ipType][port] = INVALID_PROFILE_ID;

            #ifdef CONFIG_SYS_PROTO_MVR
            igmp_cfg.mvrConfig.type[port] = DFLT_IGMP_MVR_PORT_TYPE;
            #endif
        }

        rsd_igmp_lookupMode_set(DFLT_IGMP_LOOKUP_MODE);
        rsd_mcast_igmp_unKnownMcast_action_set(DFLT_IGMP_UNKNOWN_MCAST_ACTION, DFLT_IGMP_UNKNOWN_MCAST_ACTION, DISABLED, DISABLED, FALSE);
        mcast_igmp_enable_set(DFLT_IGMP_EBL);
    }
    else
    {
        mcast_igmp_statistics_clear();
    }

}
int32 mcast_igmp_enable_set(sys_enable_t enable)
{
    sys_vlanmask_t vlanMsk;
    igmp_lookup_mode_t lookup_mode;
    uint8 isMvrEn = FALSE;

    #ifdef CONFIG_SYS_PROTO_MVR
    isMvrEn = is_mcast_mvr_enable(MULTICAST_TYPE_IPV4);
    #endif

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    VLANMASK_SET_ALL(vlanMsk);

    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4,&lookup_mode);

    if (ENABLED == enable && igmp_cfg.igmpsnp_enable == DISABLED)
    {
        igmp_cfg.igmpsnp_enable = ENABLED;
        MCASTDBG("Snooping = Enabled");

        if (DISABLED == mld_cfg.mldsnp_enable && (!isMvrEn))
        {
            igmp_thread_status = 0;
            if ((osal_thread_t)NULL == (osal_thread_create("IGMP Tick Thread", 32768, DFLT_THREAD_PRI_IGMP_TIMER, (void *)mcast_tick_thread, NULL)))
            {
                MCASTDBG("IGMP Tick Thread create failed");
                return SYS_ERR_FAILED;
            }

            SYS_ERR_CHK(sys_notifier_observer_register("igmp", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG, _mcast_linkevent_handler, SYS_NOTIFIER_PRI_MEDIUM));
            SYS_ERR_CHK(rsd_mcast_l2_addressRsv_set(enable));
        }

        SYS_ERR_CHK(mcast_static_group_hwRestore(vlanMsk, MULTICAST_TYPE_IPV4));

        SYS_ERR_CHK(rsd_igmp_enable_set(enable, lookup_mode));
    }
    else if (DISABLED == enable && igmp_cfg.igmpsnp_enable == ENABLED)
    {
        
        igmp_cfg.igmpsnp_enable = DISABLED;
        MCASTDBG("Snooping = Disabled");
        mcast_igmp_snooping_stats_init(0);

        SYS_ERR_CHK(mcast_static_group_save(vlanMsk, MULTICAST_TYPE_IPV4));
        SYS_ERR_CHK(mcast_igmp_group_del_byType(IGMP_GROUP_ENTRY_ALL));
        SYS_ERR_CHK(mcast_static_group_restore());

        SYS_ERR_CHK(mcast_router_dynPortMbr_del(vlanMsk, MULTICAST_TYPE_IPV4));

        if (DISABLED== mld_cfg.mldsnp_enable && (!isMvrEn))
        {
            _mcast_vlan_event_init();
            SYS_ERR_CHK(sys_notifier_observer_unregister("igmp", SYS_NOTIFIER_SUBJECT_PORT, _mcast_linkevent_handler));
            
            igmp_thread_status = 1;
            do {
                osal_time_usleep(50);
            } while (igmp_thread_status >= 0);

            SYS_ERR_CHK(rsd_mcast_l2_addressRsv_set(enable));

        }

        if (!isMvrEn)
            SYS_ERR_CHK(rsd_igmp_enable_set(enable, lookup_mode));
    }

    #ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER
    SYS_ERR_CHK(mcast_querier_enable_set(MULTICAST_TYPE_IPV4, &vlanMsk, enable));
    #endif

    SYS_ERR_CHK(mcast_igmp_unknMcastAction_set(gUnknown_mcast_igmp_action));

    return SYS_ERR_OK;
}
void mcast_igmp_group_head_timer(void)
{
    int i;
    uint16 port, portid;
    uint16 time;
    uint16 sortedIdx;
    uint16 flag = FALSE;

    igmp_group_entry_t  *pEntry = NULL, *groupHead;
    igmp_group_head_entry_t  *pGrpHead;
    igmp_group_entry_t groupEntry;
    for(i = 0; i < CAPA_MCAST_GROUP_NUM; i++)
    {
        pGrpHead = &groupHead_db[i];

        if(0 == pGrpHead->dip)
            continue;

        mcast_build_groupEntryKey(pGrpHead->vid, pGrpHead->ipType, pGrpHead->dip, NULL, &groupEntry);
        groupEntry.sip = pGrpHead->sip;

        FOR_EACH_LOGIC_PORT(port)
        {
            if( PORT_IS_FILTER_EXCLUDE(pGrpHead->fmode, port))
            {
                time = pGrpHead->mbrTimer[port];
                if (time == 0)
                    MCASTDBG("Warring! an existing mbr whose life is zero");
                else
                {
                    if (time > PASS_SECONDS)
                        pGrpHead->mbrTimer[port] -= PASS_SECONDS;
                    else
                    {
                        mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);
                        pEntry = groupHead;
                        while(pEntry != NULL)
                        {
                            if(pEntry->v3.mbrFwdFlag[port] == IGMPV3_FWD_CONFLICT)
                            {
                                FOR_EACH_LOGIC_PORT(portid)
                                {
                                    if(pEntry->mbrTimer[portid] != 0)
                                    {
                                        PORT_SET_FILTER_INCLUDE(pGrpHead->fmode, port);
                                        flag = TRUE;
                                        break;
                                    }
                                }
                            }
                            else if(pEntry->v3.mbrFwdFlag[port] == IGMPV3_FWD_NORMAL)
                            {
                                flag = TRUE;
                                pEntry->v3.mbrFwdFlag[port] = IGMPV3_FWD_CONFLICT;
                            }
                            pEntry = pEntry->next_subgroup;
                        }

                        if(flag == FALSE)
                        {
                            PORT_SET_FILTER_INCLUDE(pGrpHead->fmode, port);
                        }
                    }
                }
            }
        }
    }
}

#ifdef CONFIG_SYS_PROTO_IGMPV3_FULL

int32 mcast_igmp_groupHead_database_get(sys_igmp_groupHead_db_t *pGrpHeadDb)
{
    uint32 port;

    IGMP_DB_SEM_LOCK();

    if(pGrpHeadDb->indx < 0 || pGrpHeadDb->indx >= CAPA_MCAST_GROUP_NUM)
        return SYS_ERR_FAILED;

    pGrpHeadDb->dip =  groupHead_db[pGrpHeadDb->indx].dip;
    pGrpHeadDb->sip =  groupHead_db[pGrpHeadDb->indx].sip;
    pGrpHeadDb->vid =  groupHead_db[pGrpHeadDb->indx].vid;
    pGrpHeadDb->ipType = groupHead_db[pGrpHeadDb->indx].ipType;

    osal_memcpy(pGrpHeadDb->mac, groupHead_db[pGrpHeadDb->indx].mac, CAPA_MAC_BYTE_NUM);
    FOR_EACH_LOGIC_PORT(port)
    {
        pGrpHeadDb->mbrTimer[port]  =  groupHead_db[pGrpHeadDb->indx].mbrTimer[port];
    }
    osal_memcpy(&pGrpHeadDb->fmode,  &groupHead_db[pGrpHeadDb->indx].fmode, sizeof(sys_logic_portmask_t));

    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 mcast_igmpv3_static_groupEntry_set(sys_vid_t vid, uint32 groupAddr,
                                 uint32 sourceAddr, sys_logic_portmask_t *pSpmsk,  uint8 fmode)
{
    igmp_group_entry_t  *pEntry = NULL, groupEntry;
    igmp_group_entry_t  newEntry;
    igmp_group_head_entry_t *group_head_entry;
    sys_logic_portmask_t     exMbr, delStaticMbr;
    sys_logic_port_t    port;
    int32 ret;
    uint8 isStaticEntryExist = FALSE;
    uint8 isFwdAll = FALSE;

    sys_ipv46McastEntry_t  ip46Mcst_entry;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    IGMP_DB_SEM_LOCK();

    if (!IGMP_IN_MULTICAST(ntohl(groupAddr)))
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_MCAST_ADDR;
    }

    if ((ntohl(groupAddr) >= ntohl(0xE0000000L)) && (ntohl(groupAddr) <= ntohl(0xE00000FFL)))
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_IGMP_RSV_ADDR;
    }

    if((0x0 == sourceAddr) || (0xFFFFFFFF == sourceAddr))
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_IPV4_ADDR;
    }

    mcast_igmp_group_head_get(groupAddr, vid, &group_head_entry);
    if (group_head_entry)
    {
        MCASTDBG("Group head is exist !");
        osal_memset(&groupEntry, 0, sizeof(igmp_group_entry_t));
        groupEntry.vid = vid;
        groupEntry.dip = groupAddr;
        groupEntry.sip = sourceAddr;
        groupEntry.ipType = ipType;

        pEntry = mcast_group_get(&groupEntry);
        if(pEntry != NULL)
        {
            if(IGMP_GROUP_ENTRY_DYNAMIC == pEntry->form)
            {
                IGMP_DB_SEM_UNLOCK();

                mcast_static_group_clear(&groupEntry);
                IGMP_DB_SEM_LOCK();
            }
            else
            {
                if(FILTER_MODE_INCLUDE == fmode)
                {
                    
                    LOGIC_PORTMASK_ANDNOT(exMbr,pEntry->staticMbr, pEntry->staticInMbr);

                    LOGIC_PORTMASK_ANDNOT(delStaticMbr,pEntry->staticInMbr, *pSpmsk);

                    osal_memcpy(&pEntry->staticInMbr, pSpmsk, sizeof(sys_logic_portmask_t));
                    LOGIC_PORTMASK_ANDNOT(pEntry->staticInMbr, pEntry->staticInMbr,delStaticMbr);
                    LOGIC_PORTMASK_ANDNOT(pEntry->staticInMbr, pEntry->staticInMbr, exMbr);
                    LOGIC_PORTMASK_OR(pEntry->staticMbr, pEntry->staticMbr, *pSpmsk);
                    LOGIC_PORTMASK_ANDNOT(pEntry->staticMbr, pEntry->staticMbr, delStaticMbr);

                    LOGIC_PORTMASK_OR(pEntry->mbr, pEntry->mbr, *pSpmsk);
                    LOGIC_PORTMASK_ANDNOT(pEntry->mbr, pEntry->mbr, delStaticMbr);
                }
                else if(FILTER_MODE_EXCLUDE == fmode)
                {

                    LOGIC_PORTMASK_ANDNOT(exMbr,pEntry->staticMbr, pEntry->staticInMbr);
                    LOGIC_PORTMASK_ANDNOT(delStaticMbr,exMbr, *pSpmsk);

                    LOGIC_PORTMASK_OR(pEntry->mbr, pEntry->mbr, *pSpmsk);
                    LOGIC_PORTMASK_ANDNOT(pEntry->mbr, pEntry->mbr, delStaticMbr);

                    LOGIC_PORTMASK_OR(pEntry->staticMbr, pEntry->staticMbr, *pSpmsk);
                    LOGIC_PORTMASK_ANDNOT(pEntry->staticMbr, pEntry->staticMbr, delStaticMbr);

                    LOGIC_PORTMASK_ANDNOT(pEntry->staticInMbr, pEntry->staticInMbr, *pSpmsk);

                    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, pEntry->mbr)
                    {
                        if((!IS_LOGIC_PORTMASK_PORTSET(pEntry->staticInMbr, port)) ||
                            ((IS_LOGIC_PORTMASK_PORTSET(pEntry->mbr, port)) && (!IS_LOGIC_PORTMASK_PORTSET(pEntry->staticMbr, port))))
                        {
                            pEntry->v3.mbrFwdFlag[port] = IGMPV3_FWD_NOT;
                        }
                    }
                }

                isStaticEntryExist = TRUE;
            }
        }
    }

    mcast_igmp_group_head_get(groupAddr, vid, &group_head_entry);
    if (group_head_entry == NULL)
    {
        MCASTDBG("Group head is not exist !");
        group_head_entry = mcast_igmp_free_group_head_get();
        if(group_head_entry != NULL)
        {
            osal_memset(&group_head_entry->fmode, 0, sizeof(sys_logic_portmask_t));
            
            group_head_entry->dip = groupAddr;
            group_head_entry->sip = 0;
            group_head_entry->vid = vid;

            group_head_entry->mac[0] = 0x01;
            group_head_entry->mac[1] = 0x00;
            group_head_entry->mac[2] = 0x5e;
            group_head_entry->mac[3] = (groupAddr >> 16) & 0xff;
            group_head_entry->mac[4] = (groupAddr >> 8) & 0xff;
            group_head_entry->mac[5] = groupAddr & 0xff;
        }
        else
        {
            MCASTDBG("Group head database is full!");
            IGMP_DB_SEM_UNLOCK();
            return SYS_ERR_FAILED;
        }
    }

    osal_memset(&newEntry, 0 , sizeof(igmp_group_entry_t));
    MCASTDBG("Add new static group entry Group is: "IPADDR_PRINT" , sourece  ip is : "IPADDR_PRINT"",
        IPADDR_PRINT_ARG(groupAddr), IPADDR_PRINT_ARG(sourceAddr));
    newEntry.dip = groupAddr;
    newEntry.sip = sourceAddr;
    newEntry.vid = vid;
    newEntry.ipType = ipType;

    newEntry.lookupTableFlag = L2TABLE_WROTE;

    if(isStaticEntryExist)
    {
        osal_memcpy(&newEntry.mbr, &pEntry->mbr,sizeof(sys_logic_portmask_t));
        osal_memcpy(&newEntry.staticMbr, &pEntry->staticMbr, sizeof(sys_logic_portmask_t));
        osal_memcpy(&newEntry.staticInMbr, &pEntry->staticInMbr, sizeof(sys_logic_portmask_t));
    }
    else
    {
        osal_memcpy(&newEntry.mbr, pSpmsk, sizeof(sys_logic_portmask_t));
        osal_memcpy(&newEntry.staticMbr, pSpmsk, sizeof(sys_logic_portmask_t));

        if(FILTER_MODE_INCLUDE == fmode)
        {
            osal_memcpy(&newEntry.staticInMbr, pSpmsk, sizeof(sys_logic_portmask_t));
        }
        else if (FILTER_MODE_EXCLUDE == fmode)
        {
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, newEntry.mbr)
            {
                newEntry.v3.mbrFwdFlag[port] = IGMPV3_FWD_NOT;
            }
        }
    }

    newEntry.form = IGMP_GROUP_ENTRY_STATIC;

    newEntry.mac[0] = 0x01;
    newEntry.mac[1] = 0x00;
    newEntry.mac[2] = 0x5e;
    newEntry.mac[3] = (groupAddr >> 16) & 0xff;
    newEntry.mac[4] = (groupAddr >> 8) & 0xff;
    newEntry.mac[5] = groupAddr & 0xff;

    ret = mcast_group_add(&newEntry);
    if (ret)
    {
        MCASTDBG("Failed add a static group!  ret:%d", ret);
        IGMP_DB_SEM_UNLOCK();
        return ret;
    }

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        mcast_ipMcast_asicKey_get(vid, newEntry.dip, newEntry.sip, &ip46Mcst_entry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
        if (IS_CHIP_CSW())
        {
            ip46Mcst_entry.ipMcast.groupId = pEntry->groupId;
        }
#endif
    }

    if(isStaticEntryExist ||((FILTER_MODE_INCLUDE == fmode)&&(!isStaticEntryExist)) )
    {
        mcast_igmpv3_fwdPmsk_get(&ip46Mcst_entry.portmask, &newEntry);
        
        if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_set(ipType, &ip46Mcst_entry, TRUE, isFwdAll)))
        {
            IGMP_DB_SEM_UNLOCK();
            return ret;
        }
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
int32 mcast_igmpv3_static_filter_group_set(igmp_group_entry_t *pGroup, uint8 fmode)
{
    uint16 sortedIdx;
    int32 ret;
    igmp_group_entry_t *groupHead = NULL, *pEntry = NULL;
    igmp_group_entry_t  newEntry;
    sys_logic_port_t  port;
    sys_ipMcastEntry_t  ipMcst_entry;

    SYS_PARAM_CHK(NULL == pGroup, SYS_ERR_NULL_POINTER);

    IGMP_DB_SEM_LOCK();

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(pGroup), &sortedIdx, &groupHead);
    if(groupHead == NULL)
    {
        MCASTDBG("the Group ip ("IPADDR_PRINT") is not exist", IPADDR_PRINT_ARG(pGroup->dip));
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }
    else
    {
        osal_memset(&ipMcst_entry, 0, sizeof(sys_ipMcastEntry_t));
        ipMcst_entry.vid = pGroup->vid;
        ipMcst_entry.sip = pGroup->sip;
        ipMcst_entry.dip = pGroup->dip;

#if 0
        
        if(groupHead->sip == 0)
        {
            groupHead->sip = pGroup->sip;
            groupHead->lookupTableFlag = L2TABLE_WROTE;

            mcast_igmpv3_fwdPmsk_get(&ipMcst_entry.portmask, groupHead);
        }
        else
#endif
        {
                
            pEntry = mcast_group_get(pGroup);
            if(pEntry != NULL)
            {
                IGMP_DB_SEM_UNLOCK();
                mcast_static_group_clear(pGroup);
                IGMP_DB_SEM_LOCK();
            }

            osal_memset(&newEntry, 0 , sizeof(igmp_group_entry_t));
            MCASTDBG("Add new static group entry Group is: "IPADDR_PRINT" , sourece  ip is : "IPADDR_PRINT"",
                IPADDR_PRINT_ARG(pGroup->dip), IPADDR_PRINT_ARG(pGroup->sip));

            newEntry.dip = pGroup->dip;
            newEntry.sip = pGroup->sip;
            newEntry.vid = pGroup->vid;
            newEntry.ipType = pGroup->ipType;

            newEntry.lookupTableFlag = L2TABLE_NOT_WROTE;

            osal_memcpy(&newEntry.staticInMbr, &groupHead->staticInMbr, sizeof(sys_logic_portmask_t));
            osal_memcpy(&newEntry.staticMbr, &groupHead->staticMbr, sizeof(sys_logic_portmask_t));
            LOGIC_PORTMASK_OR(newEntry.mbr, newEntry.mbr, newEntry.staticMbr);

            if (FILTER_MODE_EXCLUDE == fmode)
            {
                FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, newEntry.staticMbr)
                {
                    if(!IS_LOGIC_PORTMASK_PORTSET(newEntry.staticInMbr, port))
                        newEntry.v3.mbrFwdFlag[port] = IGMPV3_FWD_NOT;
                }
            }

            newEntry.form = IGMP_GROUP_ENTRY_STATIC;

            newEntry.mac[0] = 0x01;
            newEntry.mac[1] = 0x00;
            newEntry.mac[2] = 0x5e;
            newEntry.mac[3] = (pGroup->dip >> 16) & 0xff;
            newEntry.mac[4] = (pGroup->dip >> 8) & 0xff;
            newEntry.mac[5] = pGroup->dip & 0xff;

            ret = mcast_group_add(&newEntry);
            if (ret)
            {
                MCASTDBG("Failed add a static group!  ret:%d", ret);
                IGMP_DB_SEM_UNLOCK();
                return ret;
            }
        }
    }

    pEntry = mcast_group_get(pGroup);
    if(pEntry != NULL)
    {
        mcast_igmpv3_ipMcstEntry_update_dipsip(pGroup->vid, pGroup->dip, pGroup->sip, pEntry);
    }

    IGMP_DB_SEM_UNLOCK();
    return SYS_ERR_OK;
}
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
int32 mcast_mld_suppre_enable_set(sys_enable_t enable)
{
    mld_cfg.suppre_enable = enable;
    return SYS_ERR_OK;
}
int32 mcast_mld_info_get(sys_mcast_info_t *pInfo)
{
    sys_logic_port_t lport;
    int32 i;

    SYS_PARAM_CHK(NULL == pInfo, SYS_ERR_NULL_POINTER);

    pInfo->enable                   = mld_cfg.mldsnp_enable;
    pInfo->suppreEnable         = mld_cfg.suppre_enable;
    pInfo->unknMcastActn            =  mcast_unknownAct_ret(MULTICAST_TYPE_IPV6);
    pInfo->oper_version             = mld_cfg.oper_version;

    osal_memset(pInfo->totStat, 0, sizeof(pInfo->totStat));

    FOR_EACH_LOGIC_PORT(lport)
    {
        for (i = 0; i < MCAST_STAT_END; i++)
        {
            pInfo->totStat[i] += mld_stats[lport].mldStatCnt[i];
        }
    }

    return SYS_ERR_OK;
}
int32 mcast_mld_statistics_clear(void)
{
    sys_logic_port_t lport;
    FOR_EACH_LOGIC_PORT(lport)
    {
        SYS_MEM_CLEAR(mld_stats[lport]);
    }

    return SYS_ERR_OK;
}
int32 mcast_mld_group_del_byType(mcast_group_type_t type)
{
    sys_vlanmask_t vlanMask;
    mcast_group_type_t delType = type;

    if (type < IGMP_GROUP_ENTRY_DYNAMIC || type > IGMP_GROUP_ENTRY_ALL)
        return SYS_ERR_MCAST_GROUP_TYPE;

    if (IGMP_GROUP_ENTRY_ALL == type)
        delType = IGMP_GROUP_ENTRY_DYNAMIC | IGMP_GROUP_ENTRY_STATIC;

    VLANMASK_SET_ALL(vlanMask);

    IGMP_DB_SEM_LOCK();
    _mcast_hwEntryByVid_del(&vlanMask, MULTICAST_TYPE_IPV6, delType, MCAST_ENTRY_SNP);
    IGMP_DB_SEM_UNLOCK();
    return  SYS_ERR_OK;
}

void mcast_mld_snooping_stats_init (int32 boot)
{
    sys_logic_port_t port;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;

    if (boot)
    {
        osal_memset(&mld_cfg, 0, sizeof(mld_cfg_t));

        osal_memset(&profile_db[ipType],0, CAPA_MCAST_PROFILE_NUM * sizeof(sys_igmp_profile_t));

        mld_cfg.mldsnp_enable= DISABLED;
        mld_cfg.suppre_enable = DFLT_IGMP_SUPPRE_EBL;
        mld_cfg.oper_version = DFLT_MLD_OPERATION_VER;

        gUnknown_mcast_mld_action = DFLT_IGMP_UNKNOWN_MCAST_ACTION;

        FOR_EACH_LOGIC_PORT(port)
        {
            osal_memset(&mld_stats[port], 0, sizeof(mld_stats_t));
            mld_cfg.port_entry_limit[port] = CAPA_MCAST_PORT_LIMIT_NUM;
            mld_cfg.max_groups_act[port] = DFLT_IGMP_MAX_GROUP_ACT;
            port_profileId[ipType][port] = INVALID_PROFILE_ID;
        }

        rsd_mld_lookupMode_set(DFLT_MLD_LOOKUP_MODE);
        mcast_mld_enable_set(DFLT_MLD_EBL);
    }
    else
    {
        mcast_mld_statistics_clear();
    }

}
int32 mcast_mld_enable_set(sys_enable_t enable)
{
    sys_vlanmask_t vlanMsk;
    igmp_lookup_mode_t lookup_mode;
    uint8 isMvrEn = FALSE;

    #ifdef CONFIG_SYS_PROTO_MVR
    isMvrEn = is_mcast_mvr_enable(MULTICAST_TYPE_IPV4);
    #endif

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    VLANMASK_SET_ALL(vlanMsk);

    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6,&lookup_mode);

    if (ENABLED == enable && mld_cfg.mldsnp_enable == DISABLED)
    {
        
        MCASTDBG("MLD Snooping = Enabled");
        mld_cfg.mldsnp_enable = ENABLED;

        if (DISABLED == igmp_cfg.igmpsnp_enable && (!isMvrEn))
        {
            igmp_thread_status = 0;
            if ((osal_thread_t)NULL == (osal_thread_create("IGMP Tick Thread", 32768, DFLT_THREAD_PRI_IGMP_TIMER, (void *)mcast_tick_thread, NULL)))
            {
                MCASTDBG("Mcast Tick Thread create failed");
                return SYS_ERR_FAILED;
            }
            SYS_ERR_CHK(sys_notifier_observer_register("igmp", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG, _mcast_linkevent_handler, SYS_NOTIFIER_PRI_MEDIUM));
            SYS_ERR_CHK(rsd_mcast_l2_addressRsv_set(enable));
        }

        SYS_ERR_CHK(mcast_static_group_hwRestore(vlanMsk, MULTICAST_TYPE_IPV6));

    }
    else if (DISABLED == enable && mld_cfg.mldsnp_enable == ENABLED)
    {
        mld_cfg.mldsnp_enable = DISABLED;
        MCASTDBG("MLD Snooping = Disabled");
        mcast_mld_snooping_stats_init(0);

        SYS_ERR_CHK(mcast_static_group_save(vlanMsk, MULTICAST_TYPE_IPV6));
        SYS_ERR_CHK(mcast_mld_group_del_byType(IGMP_GROUP_ENTRY_ALL));
        SYS_ERR_CHK(mcast_static_group_restore());

        SYS_ERR_CHK(mcast_router_dynPortMbr_del(vlanMsk, MULTICAST_TYPE_IPV6));

        if (DISABLED == igmp_cfg.igmpsnp_enable && (!isMvrEn))
        {
            _mcast_vlan_event_init();
            SYS_ERR_CHK(sys_notifier_observer_unregister("mld", SYS_NOTIFIER_SUBJECT_PORT, _mcast_linkevent_handler));
            igmp_thread_status = 1;
            do {
                osal_time_usleep(50);
            } while (igmp_thread_status >= 0);

            SYS_ERR_CHK(rsd_mcast_l2_addressRsv_set(enable));
        }

    }

    SYS_ERR_CHK(rsd_mld_enable_set(enable));
    SYS_ERR_CHK(mcast_igmp_unknMcastAction_set(gUnknown_mcast_igmp_action));

    return SYS_ERR_OK;
}
int32 mcast_mld_operVersion_get(mld_version_t *pVersion)
{
    SYS_PARAM_CHK((NULL == pVersion), SYS_ERR_NULL_POINTER);
    *pVersion = mld_cfg.oper_version;
    return SYS_ERR_OK;
}
int32 mcast_mld_operVersion_set(mld_version_t  operVersion)
{
    if (operVersion == mld_cfg.oper_version)
        return SYS_ERR_OK;

    if(MLD_VERSION_V2_FULL == operVersion)
        return SYS_ERR_FAILED;

    mld_cfg.oper_version = operVersion;

    #if 0
    if (MLD_VERSION_V1 == operVersion)
    {
        rsd_vlan_operEntryExist_get(&vlanMsk);
        FOR_EACH_VID_IN_VLANMASK(vid, vlanMsk)
        {
            osal_memset(&qEntry,0,sizeof(sys_igmp_querier_t));
            qEntry.vid = vid;
            qEntry.ipType = MULTICAST_TYPE_IPV6;
            mcast_mld_querier_get(&qEntry);
            
            {
                qEntry.version = MLD_QUERY_V1;
                mcast_mld_querier_set(&qEntry);
            }
        }
    }
    #endif

    return SYS_ERR_OK;
}
int32 mcast_mld_databaseSize_get(uint32 *pSize)
{
     SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);
    *pSize = mld_group_num;
    return SYS_ERR_OK;
}
int32 mcast_mld_unknMcastAction_set(uint8 action)
{
#if 0
    igmp_unknMcastAction_t oldAction = gUnknown_mcast_mld_action;
    igmp_lookup_mode_t lookup_mode;
    uint8 isGroupFull = FALSE;

    if (CAPA_MCAST_GROUP_NUM == mcast_group_num)
        isGroupFull = TRUE;

    SYS_ERR_CHK(rsd_mcast_mld_unKnownMcast_action_set(action, oldAction, mld_cfg.mldsnp_enable, isGroupFull));

    if (action != oldAction)
    {
        if (action == IGMP_UNKNOWN_MCAST_ACTION_FLOOD || action == IGMP_UNKNOWN_MCAST_ACTION_DROP)
        {
            if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == oldAction)
            {
                mcast_group_learnByData_del(MULTICAST_TYPE_IPV6);
            }
        }

        rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6 , &lookup_mode);
        if (IGMP_DIP_SIP_MODE == lookup_mode)
        {
            mcast_group_learnByData_del(MULTICAST_TYPE_IPV6);
        }
    }
    gUnknown_mcast_mld_action = action;
    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6 , &lookup_mode);
    if (IGMP_DIP_SIP_MODE == lookup_mode)
    {
        if (igmp_trapDataPkt_lock)
        {
            SYS_ERR_CHK(rsd_mcast_ipMcastPkt2CpuEnable_set(MULTICAST_TYPE_IPV6, DISABLED, gUnknown_mcast_mld_action));
        }
    }
    #endif
    return SYS_ERR_OK;

}
int32 mcast_mld_lookupMode_set(igmp_lookup_mode_t mode)
{
    igmp_lookup_mode_t lookup_mode;
    sys_vlanmask_t vlanMsk;

    VLANMASK_SET_ALL(vlanMsk);

    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6,&lookup_mode);

    if(mode != lookup_mode)
    {
        SYS_ERR_CHK(mcast_static_group_save(vlanMsk, MULTICAST_TYPE_IPV6));
        if(ENABLED == mld_cfg.mldsnp_enable)
        {
            igmp_packet_stop = ENABLED;

            mcast_mld_group_del_byType(IGMP_GROUP_ENTRY_ALL);

            rsd_mld_lookupMode_set(mode);
            igmp_packet_stop = DISABLED;
        }
        else
        {

            mcast_mld_group_del_byType(IGMP_GROUP_ENTRY_ALL);
            rsd_mld_lookupMode_set(mode);
        }
       SYS_ERR_CHK(mcast_static_group_restore());
       mcast_igmp_unknMcastAction_set(gUnknown_mcast_igmp_action);
    }

    return SYS_ERR_OK;
}
int32 mcast_mld_lookupMode_get(igmp_lookup_mode_t *mode)
{
    SYS_PARAM_CHK((NULL == mode), SYS_ERR_NULL_POINTER);

    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV6, mode);
    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_MVR

int32 mcast_mvr_pmsk_get(multicast_ipType_t ipType, mvr_portType_t portType, sys_logic_portmask_t *pMsk)
{
    sys_logic_portmask_t tmpPmsk;
    sys_logic_port_t port;

    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK((portType < MVR_TYPE_NONE ||portType > MVR_TYPE_END), SYS_ERR_MVR_TYPE);

    SYS_PARAM_CHK((NULL == pMsk), SYS_ERR_NULL_POINTER);

    LOGIC_PORTMASK_CLEAR_ALL(tmpPmsk);

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        FOR_EACH_LOGIC_PORT(port)
        {
            if (portType == igmp_cfg.mvrConfig.type[port])
                LOGIC_PORTMASK_SET_PORT(tmpPmsk, port);
        }
    }
    else
    {
        FOR_EACH_LOGIC_PORT(port)
        {
            if (portType == mld_cfg.mvrConfig.type[port])
                LOGIC_PORTMASK_SET_PORT(tmpPmsk, port);
        }
    }

    LOGIC_PORTMASK_COPY(*pMsk, tmpPmsk);
    return SYS_ERR_OK;

}

uint8 is_mcast_mvr_enable(multicast_ipType_t ipType)
{
    if (ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END)
        return FALSE;

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        if (DISABLED == igmp_cfg.mvrConfig.mvr_enable)
            return FALSE;
    }
    else
    {
        if (DISABLED == mld_cfg.mvrConfig.mvr_enable)
            return FALSE;
    }

    return TRUE;
}

int32 mcast_mvr_portFastleave_get(multicast_ipType_t ipType,sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        *pEnable = igmp_cfg.mvrConfig.fastLeaveEnable[port];
    }
    else
    {
        *pEnable = mld_cfg.mvrConfig.fastLeaveEnable[port];
    }
    return SYS_ERR_OK;
}
int32 mcast_mvr_vlanId_get(multicast_ipType_t ipType,sys_vid_t *pVid)
{
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        *pVid = igmp_cfg.mvrConfig.mvr_vid;
    }
    else
    {
        *pVid = 0;
    }
    return SYS_ERR_OK;
}
int32 mcast_mvr_portType_get(multicast_ipType_t ipType, sys_logic_port_t port , mvr_portType_t *pType)
{
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK((NULL == pType), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        *pType = igmp_cfg.mvrConfig.type[port];
    }
    else
    {
        *pType = mld_cfg.mvrConfig.type[port];
    }

    return SYS_ERR_OK;
}
int32 mcast_mvr_queryTime_get(multicast_ipType_t ipType, uint32 *pTime)
{
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        *pTime = igmp_cfg.mvrConfig.queryTime;
    }
    else
    {
        *pTime = mld_cfg.mvrConfig.queryTime;
    }

    return SYS_ERR_OK;
}
int32 mcast_igmp_mvr_enable_set(sys_enable_t enable)
{
    sys_vlanmask_t  vlanMsk;
    sys_vid_t mvrVlan;
    igmp_lookup_mode_t lookup_mode;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    if (enable == igmp_cfg.mvrConfig.mvr_enable)
        return SYS_ERR_OK;

    VLANMASK_CLEAR_ALL(vlanMsk);
    mcast_mvr_vlanId_get(MULTICAST_TYPE_IPV4,&mvrVlan);
    VLANMASK_SET_VID(vlanMsk, mvrVlan);

    IGMP_DB_SEM_LOCK();
     _mcast_hwEntryByVid_del(&vlanMsk, MULTICAST_TYPE_IPV4, IGMP_GROUP_ENTRY_ALL, MCAST_ENTRY_ALL);
    IGMP_DB_SEM_UNLOCK();

    rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4, &lookup_mode);

    if (ENABLED == enable)
    {
        if (DISABLED == mld_cfg.mldsnp_enable && DISABLED == igmp_cfg.igmpsnp_enable)
        {
            igmp_thread_status = 0;
            if ((osal_thread_t)NULL == (osal_thread_create("IGMP Tick Thread", 32768, DFLT_THREAD_PRI_IGMP_TIMER, (void *)mcast_tick_thread, NULL)))
            {
                MCASTDBG("IGMP Tick Thread create failed");
                return SYS_ERR_FAILED;
            }

            SYS_ERR_CHK(sys_notifier_observer_register("igmp", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG, _mcast_linkevent_handler, SYS_NOTIFIER_PRI_MEDIUM));
            
        }
        SYS_ERR_CHK(rsd_igmp_enable_set(enable, lookup_mode));
        SYS_ERR_CHK(rsd_mcast_l2_addressRsv_set(enable));
    }
    else if (DISABLED == enable)
    {
        if (DISABLED== mld_cfg.mldsnp_enable && DISABLED == igmp_cfg.igmpsnp_enable)
        {
            _mcast_vlan_event_init();
            SYS_ERR_CHK(sys_notifier_observer_unregister("mld", SYS_NOTIFIER_SUBJECT_PORT, _mcast_linkevent_handler));
            
            igmp_thread_status = 1;
            do {
                osal_time_usleep(50);
                } while (igmp_thread_status >= 0);

            SYS_ERR_CHK(rsd_mcast_l2_addressRsv_set(enable));
        }

        if (DISABLED == igmp_cfg.igmpsnp_enable)
            SYS_ERR_CHK(rsd_igmp_enable_set(enable, lookup_mode));

        igmp_cfg.mvrConfig.groupIp_start.ipv4 = 0;
        igmp_cfg.mvrConfig.groupIp_end.ipv4 = 0;
    }

    SYS_ERR_CHK(rsd_mcast_mcastLeakyEnable_set(enable));
    igmp_cfg.mvrConfig.mvr_enable = enable;

    return SYS_ERR_OK;
}
int32 mcast_igmp_mvr_mode_set(mvr_mode_t mode)
{
    mvr_mode_t oldMode;
    sys_logic_portmask_t sourcePmsk;
    uint8 isAddsrc, isFwdAll;
    igmp_group_entry_t *pGroup;
    uint32 i;
    sys_l2McastEntry_t l2Mcst_entry;
    sys_ipv46McastEntry_t ip46Mcst_entry;
    int32 ret;
    igmp_lookup_mode_t lookup_mode;
    sys_logic_port_t lport;

    SYS_PARAM_CHK((mode < MVR_MODE_COMPATABLE || mode >=  MVR_MODE_END), SYS_ERR_INPUT);

    oldMode = igmp_cfg.mvrConfig.mode;
    if (mode == oldMode)
        return SYS_ERR_OK;

    igmp_cfg.mvrConfig.mode = mode;

    if (0 == mcast_mvr_group_num)
        return SYS_ERR_OK;

    LOGIC_PORTMASK_CLEAR_ALL(sourcePmsk);
    mcast_mvr_pmsk_get(MULTICAST_TYPE_IPV4, MVR_TYPE_SOURCE, &sourcePmsk);

    IGMP_DB_SEM_LOCK();
    
    if (MVR_MODE_COMPATABLE == mode)
    {
        if (SYS_ERR_OK !=  (ret = mcast_mvr_staticGroup_dynMode_chk()))
        {
            igmp_cfg.mvrConfig.mode = oldMode;
            MCASTDBG("MVR static group can't use source port for compatable mode");
            IGMP_DB_SEM_UNLOCK();
            return ret;
        }

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lport, sourcePmsk)
        {
            mcast_mvr_group_remove_port(lport);
        }
    }

    if (IS_LOGIC_PORTMASK_CLEAR(sourcePmsk))
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
    }

    if (MVR_MODE_COMPATABLE == oldMode)
        isAddsrc = FALSE;
    else
    {
        mcast_suppreFlag_clear(igmp_cfg.mvrConfig.mvr_vid,MULTICAST_TYPE_IPV4);
        isAddsrc = TRUE;
    }

    for (i = 0; i < mcast_group_num; i++)
    {
        pGroup = mcast_group_get_with_index(i);
        if ( NULL == pGroup)
            continue;

        rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);

        if (is_mvr_group_ret2(pGroup))
        {
             isFwdAll = TRUE;
            if (!is_mcast_mvr_forwardAll(pGroup->ipType))
            {
                isFwdAll = FALSE;
            }

            if(IGMP_DMAC_VID_MODE == lookup_mode)
            {
                l2Mcst_entry.vid = pGroup->vid;
                osal_memcpy(l2Mcst_entry.mac, pGroup->mac, CAPA_MAC_BYTE_NUM);

                if (SYS_ERR_OK == mcast_hw_l2McastEntry_get(&l2Mcst_entry))
                {
                    if (isAddsrc)
                    {
                        LOGIC_PORTMASK_OR(l2Mcst_entry.portmask, l2Mcst_entry.portmask, sourcePmsk);
                    }
                    else
                    {
                         
                         LOGIC_PORTMASK_ANDNOT(l2Mcst_entry.portmask, l2Mcst_entry.portmask, sourcePmsk);
                    }

                    if (SYS_ERR_OK != (ret = mcast_hw_l2McastEntry_set(pGroup->ipType, &l2Mcst_entry, FALSE, isFwdAll)))
                    {
                        IGMP_DB_SEM_UNLOCK();
                        return ret;
                    }
                }
            }
            else
            {
                    if (SYS_ERR_OK == (ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46Mcst_entry)))
                    {
                        if (isAddsrc)
                        {
                            LOGIC_PORTMASK_OR(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, sourcePmsk);
                        }
                        else
                        {
                            
                            LOGIC_PORTMASK_ANDNOT(ip46Mcst_entry.portmask, ip46Mcst_entry.portmask, sourcePmsk);
                        }

                        if (SYS_ERR_OK != (ret = mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46Mcst_entry, FALSE, isFwdAll)))
                        {
                            IGMP_DB_SEM_UNLOCK();
                            return ret;
                        }

                    }

            }
        }
    }

    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 mcast_igmp_mvr_querytime_set(uint32 queryTime)
{
    igmp_cfg.mvrConfig.queryTime = queryTime;

    return SYS_ERR_OK;
}
int32 mcast_igmp_mvr_vlanId_set(sys_vid_t vid)
{
    sys_vid_t oldMvrVlan;
    sys_vlanmask_t vlanMask;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    oldMvrVlan = igmp_cfg.mvrConfig.mvr_vid;
    if (vid == igmp_cfg.mvrConfig.mvr_vid)
        return SYS_ERR_OK;

    if (ENABLED == igmp_cfg.mvrConfig.mvr_enable)
    {
        VLANMASK_CLEAR_ALL(vlanMask);
        VLANMASK_SET_VID(vlanMask, oldMvrVlan);
        VLANMASK_SET_VID(vlanMask, vid);
        IGMP_DB_SEM_LOCK();
        _mcast_hwEntryByVid_del(&vlanMask, MULTICAST_TYPE_IPV4, IGMP_GROUP_ENTRY_ALL, MCAST_ENTRY_ALL);
        IGMP_DB_SEM_UNLOCK();
    }

    igmp_cfg.mvrConfig.mvr_vid = vid;

    return SYS_ERR_OK;
}
int32 mcast_igmp_mvr_groupRange_set(uint32 startIp, uint32 endIp)
{
    sys_vlanmask_t vlanMask;

    SYS_PARAM_CHK((!IGMP_IN_MULTICAST(startIp)), SYS_ERR_INPUT);
    SYS_PARAM_CHK((!IGMP_IN_MULTICAST(endIp)), SYS_ERR_INPUT);

    if (startIp == igmp_cfg.mvrConfig.groupIp_start.ipv4 &&
        endIp == igmp_cfg.mvrConfig.groupIp_end.ipv4)
        return SYS_ERR_OK;

    if (ENABLED == igmp_cfg.mvrConfig.mvr_enable)
    {
        VLANMASK_CLEAR_ALL(vlanMask);
        VLANMASK_SET_VID(vlanMask, igmp_cfg.mvrConfig.mvr_vid);
        IGMP_DB_SEM_LOCK();
        _mcast_hwEntryByVid_del(&vlanMask, MULTICAST_TYPE_IPV4, IGMP_GROUP_ENTRY_ALL, MCAST_ENTRY_ALL);
        IGMP_DB_SEM_UNLOCK();
    }

    igmp_cfg.mvrConfig.groupIp_start.ipv4 = startIp;
    igmp_cfg.mvrConfig.groupIp_end.ipv4 = endIp;

    return SYS_ERR_OK;
}
int32 mcast_igmp_mvr_portType_set(sys_logic_port_t port , mvr_portType_t type)
{
    mvr_portType_t oldType;
    igmp_group_entry_t *pGroup;
    uint32 i;
    sys_l2McastEntry_t l2Mcst_entry;
    sys_ipv46McastEntry_t ip46Mcst_entry;
    uint8 isAdd = TRUE, isSet, isFwdAll, isUpdate = TRUE;
    int32 ret;
    igmp_lookup_mode_t lookup_mode;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((type < MVR_TYPE_NONE) || (type >= MVR_TYPE_END) , SYS_ERR_MVR_TYPE);

    oldType = igmp_cfg.mvrConfig.type[port];
    if (type ==  igmp_cfg.mvrConfig.type[port])
        return SYS_ERR_OK;

    igmp_cfg.mvrConfig.type[port] = type;

    if (0 == mcast_mvr_group_num)
        return SYS_ERR_OK;

    if (DISABLED == igmp_cfg.mvrConfig.mvr_enable)
        return SYS_ERR_OK;

    IGMP_DB_SEM_LOCK();

    if (MVR_TYPE_NONE == type)
    {
        if ((MVR_TYPE_RECEIVER == oldType) || (MVR_TYPE_SOURCE == oldType && MVR_MODE_DYNAMIC == igmp_cfg.mvrConfig.mode))
        {
            if (SYS_ERR_OK !=  (ret = mcast_mvr_staticGroup_portType_chk(port)))
            {
                 igmp_cfg.mvrConfig.type[port] = oldType;
                MCASTDBG("MVR static group is receive from port %s!",LPSTR(port));
                IGMP_DB_SEM_UNLOCK();
                return ret;
            }

            mcast_mvr_group_remove_port(port);

            if (MVR_TYPE_RECEIVER == oldType)
                isUpdate = FALSE;
        }
    }

   if (MVR_TYPE_SOURCE == type || MVR_TYPE_SOURCE == oldType)
   {
        if (MVR_MODE_DYNAMIC == igmp_cfg.mvrConfig.mode)
        {
            isUpdate = FALSE;
            mcast_suppreFlag_clear(igmp_cfg.mvrConfig.mvr_vid,MULTICAST_TYPE_IPV4);
        }

        if (MVR_TYPE_SOURCE == type)
            isAdd = TRUE;
        else
        {
            if (MVR_TYPE_SOURCE == oldType)
                isAdd = FALSE;
        }
   }

   if (MVR_TYPE_RECEIVER == type)
        isUpdate = FALSE;

   if (!isUpdate)
   {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
   }

    for (i = 0; i < mcast_group_num; i++)
    {
        pGroup = mcast_group_get_with_index(i);
        if ( NULL == pGroup)
            continue;

        isSet = FALSE;

        if (is_mvr_group_ret2(pGroup))
        {
            #ifdef CONFIG_SYS_PROTO_MVR
            isFwdAll = mcast_mvr_fwdAll_ret(pGroup);
            #endif
            rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);
            if(IGMP_DMAC_VID_MODE == lookup_mode)
            {
                l2Mcst_entry.vid = pGroup->vid;
                osal_memcpy(l2Mcst_entry.mac, pGroup->mac, CAPA_MAC_BYTE_NUM);

                if (SYS_ERR_OK == mcast_hw_l2McastEntry_get(&l2Mcst_entry))
                {
                    if (isAdd)
                    {
                        if (!IS_LOGIC_PORTMASK_PORTSET(l2Mcst_entry.portmask, port))
                        {
                            LOGIC_PORTMASK_SET_PORT(l2Mcst_entry.portmask, port);
                            isSet = TRUE;
                        }
                    }
                    else
                    {
                        if (IS_LOGIC_PORTMASK_PORTSET(l2Mcst_entry.portmask, port))
                        {

                            {
                                LOGIC_PORTMASK_CLEAR_PORT(l2Mcst_entry.portmask, port);
                                isSet = TRUE;
                            }
                        }
                    }

                    if (isSet)
                    {
                        if (SYS_ERR_OK != (ret = mcast_hw_l2McastEntry_set(pGroup->ipType, &l2Mcst_entry, FALSE, isFwdAll)))
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
                        if (isAdd)
                        {
                            if (!IS_LOGIC_PORTMASK_PORTSET(ip46Mcst_entry.portmask, port))
                            {
                                LOGIC_PORTMASK_SET_PORT(ip46Mcst_entry.portmask, port);
                                isSet = TRUE;
                            }
                        }
                        else
                        {
                            if (IS_LOGIC_PORTMASK_PORTSET(ip46Mcst_entry.portmask, port))
                            {
                                LOGIC_PORTMASK_CLEAR_PORT(ip46Mcst_entry.portmask, port);
                                isSet = TRUE;
                            }
                        }

                        if (isSet)
                        {
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
int32 mcast_igmp_mvr_portImmediate_set(sys_logic_port_t port , sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    if (enable == igmp_cfg.mvrConfig.fastLeaveEnable[port])
        return SYS_ERR_OK;

    igmp_cfg.mvrConfig.fastLeaveEnable[port] = enable;
    return SYS_ERR_OK;
}
int32 mcast_igmp_mvrGroupNum_get(uint32 *pSize)
{
     SYS_PARAM_CHK((NULL == pSize), SYS_ERR_NULL_POINTER);
    *pSize = mcast_mvr_group_num;
    return SYS_ERR_OK;
}
mvr_mode_t mcast_mvr_mode_ret(multicast_ipType_t ipType)
{
    if (MULTICAST_TYPE_IPV4 == ipType)
        return  igmp_cfg.mvrConfig.mode;
    else
        return  mld_cfg.mvrConfig.mode;
}
int32 mcast_mvr_group_del_byType( mcast_group_type_t type)
{
    sys_vlanmask_t vlanMask;
    mcast_group_type_t delType = type;
    sys_vid_t mvrVid;

    SYS_MEM_CLEAR(vlanMask);

    mcast_mvr_vlanId_get(MULTICAST_TYPE_IPV4 , &mvrVid);

    if (type < IGMP_GROUP_ENTRY_DYNAMIC || type > IGMP_GROUP_ENTRY_ALL)
        return SYS_ERR_MCAST_GROUP_TYPE;

    if (IGMP_GROUP_ENTRY_ALL == type)
        delType = IGMP_GROUP_ENTRY_DYNAMIC | IGMP_GROUP_ENTRY_STATIC;

    VLANMASK_SET_VID(vlanMask, mvrVid);

    IGMP_DB_SEM_LOCK();
    _mcast_hwEntryByVid_del(&vlanMask, MULTICAST_TYPE_IPV4, delType, MCAST_ENTRY_MVR);
    IGMP_DB_SEM_UNLOCK();
    return  SYS_ERR_OK;
}
#endif 

int32 mcast_init(void)
{
    sys_vlan_event_t vlanEvent;

    igmp_sem_pkt = osal_sem_mutex_create();
    igmp_sem_db = osal_sem_mutex_create();

    gMcast_filteringEnable = ENABLED;

    mcast_igmp_snooping_stats_init(1);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    mcast_mld_snooping_stats_init(1);
#endif

    mcast_db_init();
    mcast_querier_init();

    osal_memset(&groupSave,0,sizeof(igmp_static_group_t));

    rsd_sys_macAddr_get(0, &sys_mac);

    SYS_ERR_CHK(rsd_nic_handler_register(&igmpHandler));
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    SYS_ERR_CHK(rsd_nic_handler_register(&mldHandler));
#endif

    SYS_ERR_CHK(sys_notifier_observer_register("igmp", SYS_NOTIFIER_SUBJECT_TRUNK,
        (SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE) , _mcast_trunk_process, SYS_NOTIFIER_PRI_MEDIUM));
    _mcast_vlan_event_init();
    SYS_ERR_CHK(sys_notifier_observer_register("igmp mcast", SYS_NOTIFIER_SUBJECT_VLAN,
        (SYS_NT_VLAN_OPERVLAN_CREATE | SYS_NT_VLAN_OPERVLAN_DESTROY | SYS_NT_VLAN_OPERENTRY_CHG), _mcast_vlan_handler, SYS_NOTIFIER_PRI_MEDIUM));

    vlanEvent.vid = DFLT_VLAN_DFLT_VLAN;
    _mcast_vlan_handler(SYS_NT_VLAN_OPERVLAN_CREATE, (char*)&vlanEvent);

    return SYS_ERR_OK;
}

int32 mcast_exit(void)
{
    osal_sem_mutex_destroy(igmp_sem_pkt);
    osal_sem_mutex_destroy(igmp_sem_db);

    SYS_ERR_CHK(rsd_nic_handler_unregister(&igmpHandler));
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    SYS_ERR_CHK(rsd_nic_handler_unregister(&mldHandler));
#endif
    SYS_ERR_CHK(sys_notifier_observer_unregister("igmp", SYS_NOTIFIER_SUBJECT_TRUNK, _mcast_trunk_process));
   
    SYS_ERR_CHK(sys_notifier_observer_unregister("igmp", SYS_NOTIFIER_SUBJECT_VLAN, _mcast_vlan_handler));

    return SYS_ERR_OK;
}
void mcast_ipStr_get(multicast_ipType_t ipType, uint32 dipv4, char *dipv6, char *pIpStr)
{
    if (ipType >= MULTICAST_TYPE_END || NULL == pIpStr )
        return ;

    if (MULTICAST_TYPE_IPV4 != ipType)
    {
        if (NULL == dipv6)
            return ;
    }

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        osal_sprintf(pIpStr, "%d.%d.%d.%d", IPADDR_PRINT_ARG(dipv4));
    }
    else
    {
        osal_sprintf(pIpStr, "%X:%X:%X:%X:%X:%X:%X:%X", IPADDRV6_PRINT_ARG(dipv6));
    }

}

void mcast_log_tablefull(multicast_ipType_t ipType)
{
    if (ipType >= MULTICAST_TYPE_END)
        return ;

    MCAST_LOG(LOG_MCAST_DB_FULL);
}

void mcast_log_GroupDb_failure(igmp_group_entry_t *pGroup, mcast_operType_t oper)
{
    int32 mcastOperArry[MCAST_OPER_END];
    char dipStr[SYS_BUF64_LEN];

    if (NULL == pGroup || pGroup->ipType >= MULTICAST_TYPE_END || oper >= MCAST_OPER_END)
        return ;

    mcastOperArry[0] = LOG_MCAST_DB_UPDATE_FAIL;
    mcastOperArry[1] = LOG_MCAST_DB_ADD_FAIL;
    mcastOperArry[2] = LOG_MCAST_DB_DEL_FAIL;

    SYS_MEM_CLEAR(dipStr);
    mcast_ipStr_get(pGroup->ipType, pGroup->dip, pGroup->dipv6.addr, dipStr);
    MCAST_LOG(mcastOperArry[oper], pGroup->vid, dipStr);
}

void mcast_log_asicMac_fail(sys_l2McastEntry_t *pEntry, mcast_operType_t oper)
{
    int32 mcastOperArry[MCAST_OPER_END];

    if (NULL == pEntry || oper >= MCAST_OPER_END)
        return ;

    mcastOperArry[0] = LOG_MCAST_ASIC_DMAC_UPDATE_FAIL;
    mcastOperArry[1] = LOG_MCAST_ASIC_DMAC_ADD_FAIL;
    mcastOperArry[2] = LOG_MCAST_ASIC_DMAC_DEL_FAIL;

    MCAST_LOG(mcastOperArry[oper], pEntry->vid, MAC_PRINT_ARG(pEntry->mac));
}

void mcast_log_asicIP_fail(multicast_ipType_t ipType, sys_ipv46McastEntry_t *pEntry,mcast_operType_t oper)
{
    char dipStr[SYS_BUF64_LEN];
    int32 mcastOperArry[MCAST_OPER_END];
    sys_vid_t vid;

    if (ipType >= MULTICAST_TYPE_END || NULL == pEntry || oper >= MCAST_OPER_END)
        return ;

    SYS_MEM_CLEAR(dipStr);
    mcast_ipStr_get(ipType, pEntry->ipMcast.dip, pEntry->ipv6Mcast.dip.octet, dipStr);
    if (MULTICAST_TYPE_IPV4 == ipType)
        vid = pEntry->ipMcast.vid;
    else
        vid = pEntry->ipv6Mcast.vid;

    mcastOperArry[0] = LOG_MCAST_ASIC_DIP_UPDATE_FAIL;
    mcastOperArry[1] = LOG_MCAST_ASIC_DIP_ADD_FAIL;
    mcastOperArry[2] = LOG_MCAST_ASIC_DIP_DEL_FAIL;

    MCAST_LOG(mcastOperArry[oper], vid, dipStr);
}

void mcast_log_portLimit_arv(multicast_ipType_t ipType, sys_logic_port_t lp, uint32 maxNum)
{
    if (ipType >= MULTICAST_TYPE_END)
        return ;

    #ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
    MCAST_LOG(LOG_MCAST_LIMIT_EXCEED, gMcastType[ipType], maxNum, LPSTR(lp));
    #endif
}

void mcast_log_checksum_err(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp)
{
    if (ipType >= MULTICAST_TYPE_END)
        return ;

    MCAST_LOG(LOG_MCAST_PKT_CHKSUM_DROP, gMcastType[ipType], vid, LPSTR(lp));
}

void mcast_log_vlan_disable(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp)
{
    if (ipType >= MULTICAST_TYPE_END)
        return ;

    MCAST_LOG(LOG_MCAST_VLAN_DISABLE, gMcastType[ipType], vid);
    MCAST_LOG(LOG_MCAST_VLAN_DISABLE_FLOOD, gMcastType[ipType], vid, LPSTR(lp));
}

void mcast_log_rtPort(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp, mcast_rt_portType_t rtType)
{
    int32 mcastRtlogArry[MCAST_RT_END];

    if (ipType >= MULTICAST_TYPE_END || rtType >= MCAST_RT_END)
        return ;

    mcastRtlogArry[0] = LOG_MCAST_RTPORT_ADD;
    mcastRtlogArry[1] = LOG_MCAST_RTPORT_PIM_ADD;
    mcastRtlogArry[2] = LOG_MCAST_RTPORT_TIMEOUT;

    MCAST_LOG(mcastRtlogArry[rtType], LPSTR(lp), gMcastType[ipType], vid);
}
void mcast_log_dipCollison(multicast_ipType_t ipType, sys_ipv46McastEntry_t *pEntry)
{
    char dipStr[SYS_BUF64_LEN];
    sys_vid_t vid;

    if (NULL == pEntry || ipType >= MULTICAST_TYPE_END )
        return ;

    SYS_MEM_CLEAR(dipStr);
    mcast_ipStr_get(ipType, pEntry->ipMcast.dip, pEntry->ipv6Mcast.dip.octet, dipStr);
    if (MULTICAST_TYPE_IPV4 == ipType)
        vid = pEntry->ipMcast.vid;
    else
        vid = pEntry->ipv6Mcast.vid;

    MCAST_LOG(LOG_MCAST_DIP_COLLISION, vid, dipStr);
}

void mcast_log_dmacCollison(multicast_ipType_t ipType, sys_l2McastEntry_t *pEntry)
{
    if (NULL == pEntry || ipType >= MULTICAST_TYPE_END )
        return ;

    MCAST_LOG(LOG_MCAST_DMAC_COLLISION, pEntry->vid, MAC_PRINT_ARG(pEntry->mac));
}

void mcast_log_stpblock(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp)
{
    if (ipType >= MULTICAST_TYPE_END)
        return ;

    MCAST_LOG(LOG_MCAST_STP_DROP, vid, LPSTR(lp), gMcastType[ipType]);
}

void mcast_log_invalidGroup(multicast_ipType_t ipType)
{
    if (ipType >= MULTICAST_TYPE_END)
        return ;

    if (MULTICAST_TYPE_IPV4 == ipType)
        MCAST_LOG(LOG_MCAST_GROUP_RANGE_INVALID, "224.0.0.X");
    else
        MCAST_LOG(LOG_MCAST_GROUP_RANGE_INVALID, "FF0X::XXXX");
}

void mcast_log_ver_flood(multicast_ipType_t ipType, sys_vid_t vid)
{
    int32 ver = 3;

    if (ipType >= MULTICAST_TYPE_END)
        return ;

    if (MULTICAST_TYPE_IPV4 == ipType)
        ver = 3;
    else
        ver = 2;

    MCAST_LOG(LOG_MCAST_VER_FLOOD, gMcastType[ipType],ver, vid);
}
void mcast_log_vlanFilter_drop(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp)
{
    if (ipType >= MULTICAST_TYPE_END)
        return ;

    MCAST_LOG(LOG_MCAST_VLAN_DROP, vid, LPSTR(lp), gMcastType[ipType]);
}

void mcast_log_group_notExsit(multicast_ipType_t ipType)
{
    if (ipType >= MULTICAST_TYPE_END)
        return ;

    MCAST_LOG(LOG_MCAST_GROUP_NOTEXSIT_DROP, gMcastType[ipType]);
}
void mcast_dbg_jn_lv_report(multicast_ipType_t ipType, sys_vid_t vid, sys_logic_port_t lp, mcast_groupAddr_t *pAddr, uint8 isJn)
{
     if (ipType >= MULTICAST_TYPE_END || pAddr == NULL)
        return ;

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        if (isJn)
            MCASTDBG("Rceived IGMP Join group "IPADDR_PRINT" VLAN-%d on port %s", IPADDR_PRINT_ARG(pAddr->ipv4), vid, LPSTR(lp));
        else
            MCASTDBG("Rceived IGMP Leave group "IPADDR_PRINT" VLAN-%d on port %s", IPADDR_PRINT_ARG(pAddr->ipv4), vid, LPSTR(lp));
    }
    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else
    {
        if (isJn)
            MCASTDBG("Rceived MLD Join group "IPADDRV6_PRINT" VLAN-%d on port %s", IPADDRV6_PRINT_ARG(pAddr->ipv6.addr), vid, LPSTR(lp));
        else
            MCASTDBG("Rceived MLD Leave group "IPADDRV6_PRINT" VLAN-%d on port %s", IPADDRV6_PRINT_ARG(pAddr->ipv6.addr), vid, LPSTR(lp));
    }
    #endif
}

#if 0

int32 mcast_static_l2_mcast_groupEntry_add(l2_mcast_group_t *pGroup)
{
	uint32 rc = SYS_ERR_OK;

	SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

	if(l2_mcast_is_exist_by_mac_vlan(pGroup->vlanId, pGroup->mac))
	{
		rc = l2_mcast_port_add(pGroup);
	}
	else
	{
		rc = l2_mcast_create(pGroup);
	}
	return  rc;
}
int32 mcast_static_l2_mcast_groupEntry_del(l2_mcast_group_t *pGroup)
{
	uint32 rc = SYS_ERR_OK;

	SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

	if(l2_mcast_is_exist_by_mac_vlan(pGroup->vlanId, pGroup->mac))
	{
		if(IS_LOGIC_PORTMASK_CLEAR(pGroup->staticMbr))
		{
			rc = l2_mcast_delete(pGroup);
		}
		else
		{
			rc = l2_mcast_port_delete(pGroup);
		}
	}

	return  rc;
}
int32 mcast_static_l2_mcast_databaseSize_get(uint32 *pSize)
{
    SYS_PARAM_CHK(NULL == pSize, SYS_ERR_NULL_POINTER);

    *pSize = l2_mcast_entry_used_cnt();
    
    return SYS_ERR_OK;
}
int32 mcast_static_l2_mcast_database_get(l2_mcast_group_t *pDb, uint32 index)
{
	uint32 rc = SYS_ERR_OK;
	l2mcast_vlan_db_t macDB;

    IGMP_DB_SEM_LOCK();

	rc = l2_mcast_entry_by_used_cnt(index, &macDB);
	if(rc == SYS_ERR_OK)
	{
		pDb->vlanId = macDB.vlan_id;
		osal_memcpy(pDb->mac, macDB.mcast_mac, CAPA_MAC_BYTE_NUM);
		pDb->staticMbr = macDB.staticMbr;
	}
	
    IGMP_DB_SEM_UNLOCK();

    return rc;
}
#endif

int32 mcast_static_l2_mcast_groupEntry_add(l2_mcast_group_t *pGroup)
{
	uint32 rc = SYS_ERR_OK;

	SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

	if(rsd_l2_mcast_is_exist_by_mac_vlan(pGroup->vlanId, pGroup->mac))
	{
		rc = rsd_l2_mcast_port_add(pGroup);
	}
	else
	{
		rc = rsd_l2_mcast_create(pGroup);
	}
	return  rc;
}
int32 mcast_static_l2_mcast_groupEntry_del(l2_mcast_group_t *pGroup)
{
	uint32 rc = SYS_ERR_OK;

	SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);

	if(rsd_l2_mcast_is_exist_by_mac_vlan(pGroup->vlanId, pGroup->mac))
	{
		if(IS_LOGIC_PORTMASK_CLEAR(pGroup->staticMbr))
		{
			rc = rsd_l2_mcast_delete(pGroup);
		}
		else
		{
			rc = rsd_l2_mcast_port_delete(pGroup);
		}
	}

	return  rc;
}
int32 mcast_static_l2_mcast_databaseSize_get(uint32 *pSize)
{
    SYS_PARAM_CHK(NULL == pSize, SYS_ERR_NULL_POINTER);

    *pSize = rsd_l2_mcast_entry_used_cnt();
    
    return SYS_ERR_OK;
}
int32 mcast_static_l2_mcast_database_get(l2_mcast_group_t *pDb, uint32 index)
{
	uint32 rc = SYS_ERR_OK;
	l2mcast_vlan_db_t macDB;

    IGMP_DB_SEM_LOCK();

	rc = rsd_l2_mcast_entry_by_used_cnt(index, &macDB);
	if(rc == SYS_ERR_OK)
	{
		pDb->vlanId = macDB.vlan_id;
		osal_memcpy(pDb->mac, macDB.mcast_mac, CAPA_MAC_BYTE_NUM);
		pDb->staticMbr = macDB.staticMbr;
		pDb->gmrpMbr = macDB.gmrpMbr;
	}
	
    IGMP_DB_SEM_UNLOCK();

    return rc;
}

