
#include <rsd/rsd_ipmcast.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_l2.h>
#include <rsd/rsd_mcast.h>

#include <igmp/proto_igmp.h>
#include <igmp/proto_igmp_util.h>
extern int32  mcast_group_num;
extern osal_mutex_t     igmp_sem_db;
int32 _mcast_hw_ip6DipvidMcastEntry_get(igmp_group_entry_t *pGroup, sys_ipv46McastEntry_t *pEntry)
{
    int32 ret = -1, i;
    igmp_group_entry_t *pGrpTmp;
    int64 agKey = 0;

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
        return ret;

    agKey = mcast_aggregate_sortKey_ret(pGroup);

    for (i = 0; i < mcast_group_num; i++)
    {
        pGrpTmp = mcast_group_get_with_index(i);
        if ( NULL == pGrpTmp)
            continue;

        if (agKey != mcast_aggregate_sortKey_ret(pGrpTmp))
            continue;

        if (pGrpTmp->lookupTableFlag)
        {
            mcast_ipv6Mcast_asicKey_get(pGrpTmp->vid, pGrpTmp->dipv6.addr, pGrpTmp->sipv6.addr, pEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                pEntry->ipv6Mcast.groupId = pGroup->groupId;
            }
#endif
            ret = rsd_ip6McastEntry_get(&pEntry->ipv6Mcast);

            if (SYS_ERR_OK == ret)
                return SYS_ERR_OK;
            else
                break;
        }
    }

    mcast_ipv6Mcast_asicKey_get(pGroup->vid, pGroup->dipv6.addr, pGroup->sipv6.addr, pEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                pEntry->ipv6Mcast.groupId = pGroup->groupId;
            }
#endif

    return ret;
}

igmp_group_entry_t *mcast_ip6DipvidMcast_MaxLifeEntry_get(igmp_group_entry_t *pGroup, sys_ipv46McastEntry_t *pIp6Mcast)
{
    int32  i;
    igmp_group_entry_t *pGrpTmp = NULL , *pMaxLifeGrp = NULL;
    int64 agKey = 0;
    uint16 maxLife = 0, life;

    if (NULL == pGroup || NULL == pIp6Mcast)
        return pGrpTmp;

    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
        return pGrpTmp;

    agKey = mcast_aggregate_sortKey_ret(pGroup);

    for (i = 0; i < mcast_group_num; i++)
    {
        pGrpTmp = mcast_group_get_with_index(i);
        if ( NULL == pGrpTmp)
            continue;

        if (agKey != mcast_aggregate_sortKey_ret(pGrpTmp))
            continue;

        if (0 == osal_memcmp(pGroup->dipv6.addr, pGrpTmp->dipv6.addr, IPV6_ADDR_LEN))
            continue;

        if (IGMP_GROUP_ENTRY_STATIC == pGrpTmp->form)
        {
            mcast_ipv6Mcast_asicKey_get(pGrpTmp->vid, pGrpTmp->dipv6.addr, pGrpTmp->sipv6.addr, pIp6Mcast);
            return pGrpTmp;
        }

        if ((life = mcast_max_life_get(pGrpTmp)) == 0)
        {
            life = pGrpTmp->groupTimer;
        }

        if (life > maxLife)
        {
            mcast_ipv6Mcast_asicKey_get(pGrpTmp->vid, pGrpTmp->dipv6.addr, pGrpTmp->sipv6.addr, pIp6Mcast);
            pMaxLifeGrp = pGrpTmp;
        }
    }

    return pMaxLifeGrp;
}

int32 mcast_hwLgMask2swLgMask_get(sys_logic_portmask_t *pHwMask)
{
    uint32 trunkId = 0;
    sys_logic_portmask_t portMsk, trunkMbr;

    SYS_PARAM_CHK(NULL == pHwMask, SYS_ERR_NULL_POINTER);

    FOR_EACH_TRUNK(trunkId)
    {
        SYS_MEM_CLEAR(trunkMbr);
        SYS_MEM_CLEAR(portMsk);
        TRK2MBRLPM(trunkId, &trunkMbr);
        if (IS_LOGIC_PORTMASK_CLEAR(trunkMbr))
            continue;
        LOGIC_PORTMASK_AND(portMsk, *pHwMask, trunkMbr);
        if (IS_LOGIC_PORTMASK_EQUAL(portMsk, trunkMbr))
        {
            LOGIC_PORTMASK_ANDNOT(*pHwMask, *pHwMask, trunkMbr);
            LOGIC_PORTMASK_SET_PORT(*pHwMask, TRK2LP(trunkId));
        }
    }
    return SYS_ERR_OK;
}

int32 mcast_ipMcast_asicKey_get(sys_vid_t vid, uint32 dip, uint32 sip, sys_ipv46McastEntry_t *pEntry)
{
    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_FAILED);

    osal_memset(pEntry, 0, sizeof(sys_ipv46McastEntry_t));

    pEntry->ipMcast.vid = vid;
    pEntry->ipMcast.dip = dip;
    pEntry->ipMcast.sip = sip;
    return SYS_ERR_OK;
}

int32 mcast_ipv6Mcast_asicKey_get(sys_vid_t vid, char *ipv6_dip, char *ipv6_sip, sys_ipv46McastEntry_t *pEntry)
{
    SYS_PARAM_CHK(NULL == ipv6_dip, SYS_ERR_FAILED);
    SYS_PARAM_CHK(NULL == ipv6_sip, SYS_ERR_FAILED);
    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_FAILED);

    osal_memset(pEntry, 0, sizeof(sys_ipv46McastEntry_t));

    pEntry->ipv6Mcast.vid = vid;
    osal_memcpy(pEntry->ipv6Mcast.dip.octet, ipv6_dip, IPV6_ADDR_LEN);
    osal_memcpy(pEntry->ipv6Mcast.sip.octet, ipv6_sip, IPV6_ADDR_LEN);
    return SYS_ERR_OK;
}
int32 mcast_max_life_get(igmp_group_entry_t *group_entry)
{
    int max = 0;
    sys_logic_port_t port;

    SYS_PARAM_CHK(NULL == group_entry, SYS_ERR_FAILED);     

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, group_entry->mbr)
    {
        if (max < group_entry->mbrTimer[port])
            max = group_entry->mbrTimer[port];
    }

    return max;
}

int32 mcast_hw_mcst_mbr_remove(igmp_group_entry_t *pGroup, sys_logic_portmask_t *delPmsk)
{
    int32                   ret;
    sys_ipv46McastEntry_t      ip46McstEntry;
    sys_l2McastEntry_t      l2McstEntry;
    uint16 trunkId;
    sys_logic_port_t         normalPort;
    sys_logic_portmask_t trunkMbr, routPmsk, removePmsk;
    igmp_aggregate_entry_t *pAggrEntry = NULL;
    sys_logic_port_t port;
    igmp_group_entry_t group_entry;
    igmp_lookup_mode_t lookup_mode;
    uint8 isFwdAll = TRUE;

    SYS_PARAM_CHK(NULL == pGroup, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == delPmsk, SYS_ERR_NULL_POINTER);

    osal_memset(&group_entry, 0, sizeof(igmp_group_entry_t));
    osal_memcpy(&group_entry, pGroup, sizeof(igmp_group_entry_t));

    LOGIC_PORTMASK_CLEAR_ALL(routPmsk);
    LOGIC_PORTMASK_CLEAR_ALL(removePmsk);
    LOGIC_PORTMASK_COPY(removePmsk, *delPmsk);

    mcast_routerPort_get(group_entry.vid, group_entry.ipType, &routPmsk);
    rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);

    #ifdef CONFIG_SYS_PROTO_MVR
    isFwdAll = mcast_mvr_fwdAll_ret(pGroup);
    #endif

    if ((IGMP_DMAC_VID_MODE == lookup_mode) ||(IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != pGroup->ipType) )
    {
        mcast_aggregate_db_get(pGroup, &pAggrEntry,TRUE);
        if (pAggrEntry == NULL)
        {
            return SYS_ERR_FAILED;
        }

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, removePmsk)
        {
            pAggrEntry->port_ref_cnt[port]--;
            if ( 0 != pAggrEntry->port_ref_cnt[port])
            {
                LOGIC_PORTMASK_CLEAR_PORT(removePmsk, port);
            }
        }

        LOGIC_PORTMASK_ANDNOT(removePmsk, removePmsk, routPmsk);

        if (IS_LOGIC_PORTMASK_CLEAR(removePmsk))
            return SYS_ERR_OK;

        if (IGMP_DMAC_VID_MODE == lookup_mode)
        {
            l2McstEntry.vid = group_entry.vid;
            osal_memcpy(l2McstEntry.mac, group_entry.mac, CAPA_MAC_BYTE_NUM);
            ret = mcast_hw_l2McastEntry_get(&l2McstEntry);
        }
        else
        {
            ret = mcast_hw_ipMcastEntry_get(pGroup, &ip46McstEntry);
        }

        if (SYS_ERR_OK == ret)
        {
            FOR_EACH_TRUNK(trunkId)
            {
                if (IS_LOGIC_PORTMASK_PORTSET(removePmsk, TRK2LP(trunkId)))
                {
                    TRK2MBRLPM(trunkId, &trunkMbr);
                    if (IGMP_DMAC_VID_MODE == lookup_mode)
                    {
                        LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, group_entry.mbr);
                        LOGIC_PORTMASK_OR(l2McstEntry.portmask, l2McstEntry.portmask, routPmsk);
                        LOGIC_PORTMASK_ANDNOT(l2McstEntry.portmask, l2McstEntry.portmask, trunkMbr);
                    }
                    else
                    {
                        LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, group_entry.mbr);
                        LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, routPmsk);
                        LOGIC_PORTMASK_ANDNOT(ip46McstEntry.portmask, ip46McstEntry.portmask, trunkMbr);
                    }
                }

                if (IS_LOGIC_PORTMASK_PORTSET(pGroup->mbr, TRK2LP(trunkId)))
                {
                    if (IGMP_DMAC_VID_MODE == lookup_mode)
                        LOGIC_PORTMASK_SET_PORT(l2McstEntry.portmask, TRK2LP(trunkId));
                    else
                        LOGIC_PORTMASK_SET_PORT(ip46McstEntry.portmask, TRK2LP(trunkId));
                }
            }

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(normalPort, removePmsk)
            {
                if (IGMP_DMAC_VID_MODE == lookup_mode)
                    LOGIC_PORTMASK_CLEAR_PORT(l2McstEntry.portmask, normalPort);
                else
                    LOGIC_PORTMASK_CLEAR_PORT(ip46McstEntry.portmask, normalPort);
            }

            if (IGMP_DMAC_VID_MODE == lookup_mode)
                ret = mcast_hw_l2McastEntry_set(group_entry.ipType, &l2McstEntry, TRUE, isFwdAll);
            else
                ret = mcast_hw_ipMcastEntry_set(group_entry.ipType, &ip46McstEntry, TRUE, isFwdAll);
        }
        else
        {
            MCASTDBG("Getting Mcst enrty failed! ret:%d  dip:"IPADDR_PRINT"  vid:%d",
            ret, IPADDR_PRINT_ARG(group_entry.dip), group_entry.vid);
        }

        return ret;
    }
    else
    {
       LOGIC_PORTMASK_ANDNOT(removePmsk, removePmsk, routPmsk);

       if (IS_LOGIC_PORTMASK_CLEAR(removePmsk))
        return SYS_ERR_OK;

        ret = mcast_hw_ipMcastEntry_get(&group_entry, &ip46McstEntry);
        if (SYS_ERR_OK == ret)
        {
            FOR_EACH_TRUNK(trunkId)
            {
                if (IS_LOGIC_PORTMASK_PORTSET(removePmsk, TRK2LP(trunkId)))
                {
                    TRK2MBRLPM(trunkId, &trunkMbr);
                    LOGIC_PORTMASK_ANDNOT(ip46McstEntry.portmask, ip46McstEntry.portmask, trunkMbr);
                    LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, group_entry.mbr);
                    LOGIC_PORTMASK_OR(ip46McstEntry.portmask, ip46McstEntry.portmask, routPmsk);
                }

                if (IS_LOGIC_PORTMASK_PORTSET(pGroup->mbr, TRK2LP(trunkId)))
                {
                    LOGIC_PORTMASK_SET_PORT(ip46McstEntry.portmask, TRK2LP(trunkId));
                }
            }

            FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(normalPort, removePmsk)
            {
                LOGIC_PORTMASK_CLEAR_PORT(ip46McstEntry.portmask, normalPort);
            }

            ret = mcast_hw_ipMcastEntry_set(pGroup->ipType, &ip46McstEntry, TRUE, isFwdAll);

            return ret;
        }
    }

    return SYS_ERR_OK;
}

int32 mcast_hw_mcst_entry_del(igmp_group_entry_t *group_entry)
{
    sys_ipv46McastEntry_t   ip46McstEntry, chgIp6McastEntry;
    sys_l2McastEntry_t      l2McstEntry;
    igmp_aggregate_entry_t *pAggrEntry = NULL;
    sys_logic_portmask_t delPmsk;
    int32 ret = SYS_ERR_OK;
    igmp_lookup_mode_t lookup_mode;
    uint8 isStaticMac = FALSE;
    igmp_group_entry_t *pMaxLifeGrp = NULL;

    SYS_PARAM_CHK(NULL == group_entry, SYS_ERR_NULL_POINTER);

    if (!is_mcast_group(group_entry))
        isStaticMac = TRUE;

    rsd_mcast_lookupMode_get(group_entry->ipType, &lookup_mode);
    if ((IGMP_DMAC_VID_MODE == lookup_mode) || (isStaticMac) || (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != group_entry->ipType))
    {
        if ((IGMP_DMAC_VID_MODE == lookup_mode) || (isStaticMac))
        {
            l2McstEntry.vid = group_entry->vid;
            osal_memcpy(l2McstEntry.mac, group_entry->mac, CAPA_MAC_BYTE_NUM);
        }
        else
        {
            SYS_MEM_CLEAR(ip46McstEntry);
            ip46McstEntry.ipv6Mcast.vid = group_entry->vid;
            osal_memcpy(ip46McstEntry.ipv6Mcast.dip.octet, group_entry->dipv6.addr, CAPA_IPV6_BYTE_NUM);
            osal_memcpy(ip46McstEntry.ipv6Mcast.sip.octet, group_entry->sipv6.addr, CAPA_IPV6_BYTE_NUM);
        }

         mcast_aggregate_db_get(group_entry, &pAggrEntry,TRUE);
         if (pAggrEntry == NULL)
         {
            if ((IGMP_DMAC_VID_MODE == lookup_mode) || (isStaticMac))
            {
                ret = mcast_hw_l2McastEntry_del(&l2McstEntry);
            }
            else
            {
               ret =  mcast_hw_ipMcastEntry_del(group_entry->ipType, &ip46McstEntry);
            }

            mcast_log_GroupDb_failure(group_entry, MCAST_DEL);
            return SYS_ERR_FAILED;
         }
                  if (1 == pAggrEntry->group_ref_cnt)
         {
            pAggrEntry->group_ref_cnt--;
            mcast_aggregate_db_del(group_entry);

            if ((IGMP_DMAC_VID_MODE == lookup_mode) || (isStaticMac))
            {
                ret = mcast_hw_l2McastEntry_get(&l2McstEntry);
                if (SYS_ERR_OK == ret)
                {
                    ret = mcast_hw_l2McastEntry_del(&l2McstEntry);
                }
            }
            else
            {
                ret = mcast_hw_ipMcastEntry_get(group_entry, &ip46McstEntry);
                if (SYS_ERR_OK == ret)
                {
                    ret = mcast_hw_ipMcastEntry_del(group_entry->ipType, &ip46McstEntry);
                }
            }
            return ret;
        }
        else if (pAggrEntry->group_ref_cnt > 1)
        {
            LOGIC_PORTMASK_CLEAR_ALL(delPmsk);
            LOGIC_PORTMASK_COPY(delPmsk, group_entry->mbr);
            pAggrEntry->group_ref_cnt--;
            if (!IS_LOGIC_PORTMASK_CLEAR(delPmsk))
            {
                LOGIC_PORTMASK_CLEAR_ALL(group_entry->mbr);
               ret = mcast_hw_mcst_mbr_remove(group_entry, &delPmsk);
               if (SYS_ERR_OK == ret)
                    ret = SYS_ERR_IGMP_REMOVE_PORT_OK;
            }

                        if (IGMP_DIP_VID_MODE == lookup_mode && MULTICAST_TYPE_IPV4 != group_entry->ipType)
            {
                if (SYS_ERR_OK == _mcast_hw_ip6DipvidMcastEntry_get(group_entry, &ip46McstEntry))
                {
                    if (0 == osal_memcmp(group_entry->dipv6.addr, ip46McstEntry.ipv6Mcast.dip.octet, IPV6_ADDR_LEN))
                    {
                        pMaxLifeGrp = mcast_ip6DipvidMcast_MaxLifeEntry_get(group_entry, &chgIp6McastEntry);
                        if (NULL == pMaxLifeGrp)
                            return SYS_ERR_FAILED;

                        LOGIC_PORTMASK_COPY(chgIp6McastEntry.ipv6Mcast.portmask, ip46McstEntry.ipv6Mcast.portmask);
                        if (SYS_ERR_OK == rsd_ip6McastEntry_del(&ip46McstEntry.ipv6Mcast))
                        {
                            if (SYS_ERR_OK != rsd_ip6McastEntry_add(&chgIp6McastEntry.ipv6Mcast))
                            {
                                #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                                MCASTDBG("Change ASIC ipv6 multicast address dip"IPADDRV6_PRINT" failure!",IPADDRV6_PRINT_ARG(chgIp6McastEntry.ipv6Mcast.dip.octet));
                                #endif
                            }
                            else
                            {
                                pMaxLifeGrp->lookupTableFlag = L2TABLE_WROTE;
                            }
                        }
                        else
                        {
                            #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
                            MCASTDBG("Delete ASIC ipv6 multicast address dip"IPADDRV6_PRINT" failure!",IPADDRV6_PRINT_ARG(group_entry->dipv6.addr));
                            #endif
                        }
                    }
                }
            }

            return ret;
        }
    }
    else
    {
        if (SYS_ERR_OK == mcast_hw_ipMcastEntry_get(group_entry, &ip46McstEntry))
        {
            ret = mcast_hw_ipMcastEntry_del(group_entry->ipType, &ip46McstEntry);
            return ret;
        }
        else
        {
            return SYS_ERR_ENTRY_NOT_FOUND;
        }
    }

    return SYS_ERR_FAILED;
}
int32 mcast_hw_l2McastEntry_add(multicast_ipType_t ipType, sys_l2McastEntry_t *pEntry, uint8 isFwdAll)
{
    mcast_vlan_entry_t    mcastVlan;
    sys_logic_portmask_t logicMbr;
    int32 ret = SYS_ERR_OK;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    LOGIC_PORTMASK_CLEAR_ALL(logicMbr);
    LOGIC_PORTMASK_OR(logicMbr, logicMbr, pEntry->portmask);

    if (isFwdAll)
    {
        osal_memset(&mcastVlan, 0, sizeof(mcastVlan));
        mcastVlan.vid = pEntry->vid;
        mcastVlan.ipType = ipType;
        SYS_ERR_CHK(mcast_vlan_get(&mcastVlan));
        if (0 != mcastVlan.vid)
        {
            LOGIC_PORTMASK_OR(logicMbr, logicMbr, mcastVlan.staticMbr);
            LOGIC_PORTMASK_ANDNOT(logicMbr, logicMbr, mcastVlan.forbidMbr);
        }
    }

    SYS_ERR_CHK(mcast_hw_l2Mcast_validMbr_get(&logicMbr, &pEntry->portmask));
    ret = rsd_l2McastEntry_add(pEntry);
    if (SYS_ERR_L2_MAC_FULL == ret)
    {
        mcast_log_dmacCollison(ipType, pEntry);
    }

    return ret;
}

int32 mcast_hw_l2McastEntry_set(multicast_ipType_t ipType, sys_l2McastEntry_t *pEntry, uint8 isMoveTrunkMbr, uint8 isFwdAll)
{
    mcast_vlan_entry_t    mcastVlan;
    sys_logic_portmask_t logicMbr, setLogMbr;
    int32 ret = SYS_ERR_OK;
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((ipType > MULTICAST_TYPE_END) || (ipType < MULTICAST_TYPE_IPV4), SYS_ERR_MCAST_IPTYPE);

    LOGIC_PORTMASK_CLEAR_ALL(logicMbr);
    LOGIC_PORTMASK_CLEAR_ALL(setLogMbr);
    LOGIC_PORTMASK_OR(logicMbr, logicMbr, pEntry->portmask);

    if (isFwdAll)
    {
        osal_memset(&mcastVlan, 0, sizeof(mcastVlan));
        mcastVlan.vid = pEntry->vid;
        mcastVlan.ipType = ipType;
        SYS_ERR_CHK(mcast_vlan_get(&mcastVlan));
        if (0 != mcastVlan.vid)
        {
            LOGIC_PORTMASK_OR(logicMbr, logicMbr, mcastVlan.staticMbr);
            LOGIC_PORTMASK_ANDNOT(logicMbr, logicMbr, mcastVlan.forbidMbr);
        }
    }

    if (isMoveTrunkMbr)
        SYS_ERR_CHK(mcast_hw_l2Mcast_validMbr_get(&logicMbr, &setLogMbr));
    else
        LOGIC_PORTMASK_COPY(setLogMbr,logicMbr);

    LOGIC_PORTMASK_COPY(pEntry->portmask, setLogMbr);

    ret = rsd_l2McastEntry_set(pEntry);
    if (SYS_ERR_OK != ret)
    {
        mcast_log_asicMac_fail(pEntry, MCAST_UPDATE);
    }

    return ret;
}
int32 mcast_hw_l2McastEntry_get(sys_l2McastEntry_t *pEntry)
{
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);
    SYS_ERR_CHK(rsd_l2McastEntry_get(pEntry));

    mcast_hwLgMask2swLgMask_get(&pEntry->portmask);

    return SYS_ERR_OK;
}

int32 mcast_hw_l2McastEntry_del(sys_l2McastEntry_t *pEntry)
{
    int32 ret;
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    ret = rsd_l2McastEntry_del(pEntry);

    if (SYS_ERR_OK != ret)
    {
        mcast_log_asicMac_fail(pEntry,MCAST_DEL);
    }

    return ret;
}
int32 mcast_hw_ipMcastEntry_add(multicast_ipType_t ipType, sys_ipv46McastEntry_t *pEntry, uint8 isFwdAll)
{
    mcast_vlan_entry_t    mcastVlan;
    sys_logic_portmask_t logicMbr,setLogMbr;
    igmp_lookup_mode_t lookup_mode = IGMP_DIP_SIP_MODE;
    int32 ret;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((ipType > MULTICAST_TYPE_END) || (ipType < MULTICAST_TYPE_IPV4), SYS_ERR_MCAST_IPTYPE);

    LOGIC_PORTMASK_CLEAR_ALL(logicMbr);
    LOGIC_PORTMASK_OR(logicMbr, logicMbr, pEntry->portmask);

    #ifdef CONFIG_SYS_PROTO_MVR
    if (isFwdAll)
    #endif
    {
        rsd_mcast_lookupMode_get(ipType, &lookup_mode);
        if (IGMP_DIP_VID_MODE == lookup_mode)
        {
            osal_memset(&mcastVlan, 0, sizeof(mcastVlan));
            mcastVlan.ipType = ipType;
            if (MULTICAST_TYPE_IPV4 == ipType)
                mcastVlan.vid = pEntry->ipMcast.vid;
            else
                mcastVlan.vid = pEntry->ipv6Mcast.vid;
            SYS_ERR_CHK(mcast_vlan_get(&mcastVlan));
            if (0 != mcastVlan.vid)
            {
                LOGIC_PORTMASK_OR(logicMbr, logicMbr, mcastVlan.staticMbr);
                LOGIC_PORTMASK_ANDNOT(logicMbr, logicMbr, mcastVlan.forbidMbr);
            }
        }
    }

    SYS_ERR_CHK(mcast_hw_l2Mcast_validMbr_get(&logicMbr, &setLogMbr));

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        LOGIC_PORTMASK_COPY(pEntry->ipMcast.portmask, setLogMbr);
        ret = rsd_ipMcastEntry_add(&pEntry->ipMcast);
        if (SYS_ERR_L2_MAC_FULL == ret)
        {
            mcast_log_dipCollison(ipType, pEntry);
        }
        return ret;
    }
    else
    {
        LOGIC_PORTMASK_COPY(pEntry->ipv6Mcast.portmask, setLogMbr);
        ret = rsd_ip6McastEntry_add(&pEntry->ipv6Mcast);
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        if (SYS_ERR_L2_MAC_FULL == ret)
        {
            mcast_log_dipCollison(ipType, pEntry);
        }
        #endif
        return ret;
    }

    return SYS_ERR_FAILED;
}

int32 mcast_hw_ipMcastEntry_set(multicast_ipType_t ipType, sys_ipv46McastEntry_t *pEntry, uint8 isMoveTrunkMbr, uint8 isFwdAll)
{
    mcast_vlan_entry_t    mcastVlan;
    sys_logic_portmask_t logicMbr, setLogMbr;
    igmp_lookup_mode_t lookup_mode = IGMP_DIP_SIP_MODE;
    int32 ret = SYS_ERR_FAILED;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((ipType > MULTICAST_TYPE_END) || (ipType < MULTICAST_TYPE_IPV4), SYS_ERR_MCAST_IPTYPE);

    LOGIC_PORTMASK_CLEAR_ALL(logicMbr);
    LOGIC_PORTMASK_CLEAR_ALL(setLogMbr);
    LOGIC_PORTMASK_OR(logicMbr, logicMbr, pEntry->portmask);

    if (isFwdAll)
    {
        rsd_mcast_lookupMode_get(ipType, &lookup_mode);
        if (IGMP_DIP_VID_MODE == lookup_mode)
        {
            osal_memset(&mcastVlan, 0, sizeof(mcastVlan));
            mcastVlan.ipType = ipType;
            if (MULTICAST_TYPE_IPV4 == ipType)
                mcastVlan.vid = pEntry->ipMcast.vid;
            else
                mcastVlan.vid = pEntry->ipv6Mcast.vid;
            SYS_ERR_CHK(mcast_vlan_get(&mcastVlan));
            if (0 != mcastVlan.vid)
            {
                LOGIC_PORTMASK_OR(logicMbr, logicMbr, mcastVlan.staticMbr);
                LOGIC_PORTMASK_ANDNOT(logicMbr, logicMbr, mcastVlan.forbidMbr);
            }
        }
    }

    if (isMoveTrunkMbr)
    {
        SYS_ERR_CHK(mcast_hw_l2Mcast_validMbr_get(&logicMbr, &setLogMbr));
    }
    else
    {
        LOGIC_PORTMASK_COPY(setLogMbr, logicMbr);
    }

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        LOGIC_PORTMASK_COPY(pEntry->ipMcast.portmask, setLogMbr);
        ret = rsd_ipMcastEntry_set(&pEntry->ipMcast);
        if (SYS_ERR_OK != ret)
        {
            mcast_log_asicIP_fail(ipType, pEntry, MCAST_UPDATE);
        }
    }

    #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else
    {
        LOGIC_PORTMASK_COPY(pEntry->ipv6Mcast.portmask, setLogMbr);
        ret = rsd_ip6McastEntry_set(&pEntry->ipv6Mcast);

        if (SYS_ERR_OK != ret)
        {
            mcast_log_asicIP_fail(ipType, pEntry, MCAST_UPDATE);
        }
    }
    #endif

    return ret;

}
int32 mcast_hw_ipMcastEntry_get(igmp_group_entry_t *pGroup, sys_ipv46McastEntry_t *pEntry)
{
    int32 ret = 0;
    igmp_lookup_mode_t lookup_mode = IGMP_DMAC_VID_MODE;

    SYS_PARAM_CHK((NULL == pGroup), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
    {
        mcast_ipMcast_asicKey_get(pGroup->vid, pGroup->dip, pGroup->sip, pEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
        if (IS_CHIP_CSW())
        {
            pEntry->ipMcast.groupId = pGroup->groupId;
        }
#endif
        ret = rsd_ipMcastEntry_get(&pEntry->ipMcast);
        LOGIC_PORTMASK_COPY(pEntry->portmask, pEntry->ipMcast.portmask);
    }
    else
    {
        rsd_mcast_lookupMode_get(pGroup->ipType, &lookup_mode);
        if (IGMP_DIP_VID_MODE == lookup_mode)
        {
            ret = _mcast_hw_ip6DipvidMcastEntry_get(pGroup, pEntry);
        }
        else
        {
            mcast_ipv6Mcast_asicKey_get(pGroup->vid, pGroup->dipv6.addr, pGroup->sipv6.addr, pEntry);
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
            if (IS_CHIP_CSW())
            {
                pEntry->ipv6Mcast.groupId = pGroup->groupId;
            }
#endif
            ret = rsd_ip6McastEntry_get(&pEntry->ipv6Mcast);
        }
        LOGIC_PORTMASK_COPY(pEntry->portmask, pEntry->ipv6Mcast.portmask);
    }

    mcast_hwLgMask2swLgMask_get(&pEntry->portmask);

    return ret;
}

int32 mcast_hw_ipMcastEntry_del(multicast_ipType_t ipType,sys_ipv46McastEntry_t *pEntry)
{
    int ret = 0;
    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        ret = rsd_ipMcastEntry_del(&pEntry->ipMcast);
        if (SYS_ERR_OK != ret)
        {
            mcast_log_asicIP_fail(ipType, pEntry, MCAST_DEL);
         }
    }
    else
    {
        ret = rsd_ip6McastEntry_del(&pEntry->ipv6Mcast);
        #ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
        if (SYS_ERR_OK != ret)
        {
            mcast_log_asicIP_fail(ipType, pEntry, MCAST_DEL);
        }
        #endif
    }

    return ret;
}

int32 mcast_hw_ipMcastEntryIncCPU_set(multicast_ipType_t ipType,sys_ipv46McastEntry_t *pEntry)
{
    sys_ipv46McastEntry_t ipMcastEntry;

    SYS_PARAM_CHK((NULL == pEntry), SYS_ERR_NULL_POINTER);

    osal_memcpy(&ipMcastEntry,pEntry,sizeof(sys_ipv46McastEntry_t));

    if (MULTICAST_TYPE_IPV4 == ipType)
    {
        SYS_ERR_CHK(mcast_hw_l2Mcast_validMbr_get(&pEntry->ipMcast.portmask, &ipMcastEntry.ipMcast.portmask));
        SYS_ERR_CHK(rsd_ipMcastEntryIncCPU_set(&ipMcastEntry.ipMcast));
    }
    else
    {
        SYS_ERR_CHK(mcast_hw_l2Mcast_validMbr_get(&pEntry->ipv6Mcast.portmask, &ipMcastEntry.ipv6Mcast.portmask));
        SYS_ERR_CHK(rsd_ip6McastEntryIncCPU_set(&ipMcastEntry.ipv6Mcast));
    }

    return SYS_ERR_OK;
}
int32 mcast_hw_l2Mcast_validMbr_get(sys_logic_portmask_t *pPmsk, sys_logic_portmask_t *pResultPmsk)
{
    sys_logic_port_t port;

    SYS_PARAM_CHK((NULL == pPmsk), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pResultPmsk), SYS_ERR_NULL_POINTER);

    LOGIC_PORTMASK_COPY(*pResultPmsk, *pPmsk);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, *pPmsk)
    {
        if (IS_LP_TRKMBR(port))
        {
            LOGIC_PORTMASK_CLEAR_PORT(*pResultPmsk, port);
        }
    }
    return SYS_ERR_OK;
}
