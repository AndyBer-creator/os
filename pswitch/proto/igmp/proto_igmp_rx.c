
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_mcast.h>

#include <igmp/proto_igmp.h>
#include <igmp/proto_igmp_querier.h>
#include <igmp/proto_igmp_rx.h>
#include <igmp/proto_igmp_tx.h>

const ip_addr_t dip_query = {{224, 0, 0, 1}};
const ip_addr_t proxy_query = {{0, 0, 0, 0}};
const ip_addr_t dvmrp_routing = {{224, 0, 0, 4}};
const ip_addr_t ospf_all_routing = {{224, 0, 0, 5}};
const ip_addr_t ospf_designated_routing = {{224, 0, 0, 6}};
const ip_addr_t pimv2_routing = {{224, 0, 0, 13}};

extern igmp_unknMcastAction_t           gUnknown_mcast_igmp_action;
extern igmp_unknMcastAction_t           gUnknown_mcast_mld_action;
extern uint32           gMcast_filteringEnable;
extern igmp_stats_t     igmp_stats[SYS_LOGICPORT_NUM_MAX];
extern igmp_cfg_t        igmp_cfg;
extern sys_igmp_func_t  igmp_lookup_mode_func[];
extern sys_enable_t     igmp_packet_stop;
extern sys_enable_t     mcast_groupAdd_stop;

extern mld_stats_t  mld_stats[SYS_LOGICPORT_NUM_MAX];
extern mld_cfg_t  mld_cfg;

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
const ipv6_addr_t ipv6_dip_query1 = {{0xff, 0x01, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x01}};
const ipv6_addr_t ipv6_dip_query2 = {{0xff, 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x01}};
const ipv6_addr_t ipv6_dip_leave1 = {{0xff, 0x01, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x02}};
const ipv6_addr_t ipv6_dip_leave2 = {{0xff, 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x02}};
const ipv6_addr_t ipv6_dip_leave3 = {{0xff, 0x05, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x02}};

const ipv6_addr_t ipv6_dvmrp_routing =      {{0xff, 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x04}};
const ipv6_addr_t ipv6_ospf_all_routing =   {{0xff, 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x05}};
const ipv6_addr_t ipv6_ospf_designated_routing = {{0xff, 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x06}};
const ipv6_addr_t ipv6_pimv2_routing =      {{0xff, 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0d}};

const ipv6_addr_t ipv6_mldv2_report = {{0xff, 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x16}};
const ipv6_addr_t ipv6_proxy_query = {{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}};
#endif

void mcast_process_delay(void)
{
    if (CAPA_MCAST_GROUP_NUM >= 512)
    {
        if (mcast_groupAdd_stop)
            osal_time_usleep(50 * 1000);
        else
            osal_time_usleep(20 * 1000);
    }
    else
        osal_time_usleep(20 * 1000);
}

int32 mcast_vlan_portIgrFilter_check(sys_vid_t vid, sys_logic_port_t port, multicast_ipType_t ipType)
{
   sys_enable_t igrFilterEnable = DISABLED;
   sys_vlan_operEntry_t operEntry;
   
   sys_vlanmask_t vlanExist;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 ||ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    SYS_ERR_CHK(rsd_vlan_portAdminIgrFilterEnable_get(port, &igrFilterEnable));

    if(igrFilterEnable == ENABLED)
    {
        SYS_ERR_CHK(rsd_vlan_operEntryExist_get(&vlanExist));
        if (!VLANMASK_IS_VIDSET(vlanExist, vid))
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
            {
                MCASTDBG("VLAN %u not exist !", vid);
            }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
            {
                MCASTDBG("VLAN %u not exist !", vid);
            }
#endif
            return SYS_ERR_FAILED;
        }

        osal_memset(&operEntry, 0 , sizeof(operEntry));
        SYS_ERR_CHK(rsd_vlan_operEntry_get(vid, &operEntry));
        if (SYS_VLAN_ENTRY_TYPE_DYNAMIC == operEntry.type)
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
                MCASTDBG("VLAN %d is dynamic !", vid);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
                MCASTDBG("VLAN %d is dynamic !", vid);
#endif
            return SYS_ERR_FAILED;
        }

        if (!IS_LOGIC_PORTMASK_PORTSET(operEntry.lpmMember, port))
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
                MCASTDBG("PORT-%s is not VLAN %d member !", LPSTR(port), vid);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
            else
                MCASTDBG("PORT-%s is not VLAN %d member !", LPSTR(port), vid);
#endif
            return SYS_ERR_FAILED;
        }

        #if 0
        LOGIC_PORTMASK_CLEAR_ALL(trunkPmbr);
        LOGIC_PORTMASK_CLEAR_ALL(tmp);
        TRK2MBRLPM(LP2TRKID(port), &trunkPmbr);
        LOGIC_PORTMASK_AND(tmp, trunkPmbr, operEntry.lpmMember);
        if (!IS_LOGIC_PORTMASK_EQUAL(tmp, trunkPmbr))
        {
            if (MULTICAST_TYPE_IPV4 == ipType)
                MCASTDBG("Logic port %u which got from received packet is not a member port of "
                            "vid %u, discarding control packets", port, vid);
            else
                MCASTDBG("Logic port %u which got from received packet is not a member port of "
                            "vid %u, discarding control packets", port, vid);
            return SYS_ERR_FAILED;
        }
       #endif
    }

    return SYS_ERR_OK;
}

static uint8  mcast_igmp_dmac_check(uint8 *pMac, uint32 ipAddr)
{
    uint32 dip;

    if (NULL == pMac)
        return FALSE;

    dip = *((uint32 *)(pMac + 2));

    if ( (dip & 0x7fffff) == (ipAddr & 0x7fffff))
        return TRUE;
    else
        return FALSE;
}

static int32 mcast_igmp_check_header(uint8 *pMac, iphdr_t *pIpHdr, sys_vid_t vid, sys_logic_port_t lp)
{
    uint16          i, size1, size2, igmplen;
    uint32          group;
    igmp_hdr_t      *pIgmpHdr;
    igmpv3_report_t *report;
    igmpv3_grec_t   *grec, *ptr;
    uint16 csum = 0;

    mcast_groupAddr_t groupAdr;

    osal_memset(&groupAdr, 0, sizeof(mcast_groupAddr_t));
    SYS_PARAM_CHK((NULL == pMac), SYS_ERR_NULL_POINTER);

    if (pIpHdr->ihl < 5 || pIpHdr->version != 4)
    {
        MCASTDBG("IP Header Error: Version or Length Incorrect!");
        return ERRONEOUS_PACKET;
    }

    if (!IGMP_IN_MULTICAST(ntohl(pIpHdr->dip)))
    {
        MCASTDBG("IP Header Error: Dst IP("IPADDR_PRINT") is not in "
            "Multicast range(224.0.0.0 ~ 239.255.255.255)", IPADDR_PRINT_ARG(pIpHdr->dip));
        return ERRONEOUS_PACKET;
    }

    if (!mcast_igmp_dmac_check(pMac, pIpHdr->dip))
    {
        MCASTDBG("IP Header Error: Dst IP("IPADDR_PRINT") is not match "
            "The Mac address "MAC_PRINT"", IPADDR_PRINT_ARG(pIpHdr->dip), MAC_PRINT_ARG(pMac));
        return ERRONEOUS_PACKET;
    }

#if 0
    if (IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, dvmrp_routing.addr) ||
        IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, ospf_all_routing.addr) ||
        IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, ospf_designated_routing.addr) ||
        IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, pimv2_routing.addr))
    {
        return ROUTER_ROUTING_PACKET;
    }
#endif

    if (IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, dvmrp_routing.addr))
    {
        if (DVMRP_PROTO_ID == pIpHdr->protocol)
            return ROUTER_ROUTING_PACKET;
        else
            return ERRONEOUS_PACKET;
    }

    if (IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, ospf_all_routing.addr) ||
        IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, ospf_designated_routing.addr) )
    {
        if (OSPF_PROTO_ID == pIpHdr->protocol)
            return ROUTER_ROUTING_PACKET;
        else
            return ERRONEOUS_PACKET;
    }

    if (IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, pimv2_routing.addr))
    {
        if (PIM_PROTO_ID == pIpHdr->protocol)
            return ROUTER_ROUTING_PACKET;
        else
            return ERRONEOUS_PACKET;
    }
    if (pIpHdr->protocol != 2  )
    {

        groupAdr.ipv4 = ntohl(pIpHdr->dip);
        #ifdef CONFIG_SYS_PROTO_MVR
        if (is_mvr_group_ret1(MULTICAST_TYPE_IPV4, vid, &groupAdr, TRUE))
        {
            MCASTDBG("Ipv4 Multicast data is belong to MVR vlan");
            return MULTICAST_MVR_DATA_PACKET;
        }
        #endif
        if (pIpHdr->protocol == 255    )   
        {
            return MULTICAST_DATA_PACKET;
        }

        return VALID_OTHER_PACKET;
    }

    pIgmpHdr = (igmp_hdr_t *)((char *)pIpHdr + pIpHdr->ihl * 4);

    if(pIgmpHdr->type == IGMP_PIMV1_ROUTING_PKT)
        return ROUTER_ROUTING_PACKET;

    if (pIgmpHdr->type != IGMP_HOST_MEMBERSHIP_QUERY &&
        pIgmpHdr->type != IGMP_HOST_MEMBERSHIP_REPORT &&
        pIgmpHdr->type != IGMP_HOST_NEW_MEMBERSHIP_REPORT &&
        pIgmpHdr->type != IGMP_HOST_LEAVE_MESSAGE &&
        pIgmpHdr->type != IGMPv3_HOST_MEMBERSHIP_REPORT)
    {
        MCASTDBG("IGMP Header Error: Unsupported IGMP version, type = 0x%X!", pIgmpHdr->type);
        return UNSUPPORTED_IGMP_CONTROL_PACKET;
    }

    igmplen = pIpHdr->tot_len - (pIpHdr->ihl * 4);

    if (pIgmpHdr->type == IGMPv3_HOST_MEMBERSHIP_REPORT)
    {
        if (ntohl(0xE0000016L) != ntohl(pIpHdr->dip))
        {
            MCASTDBG("Dst IP("IPADDR_PRINT") in IGMPv3 Report is not 224.0.0.22",
                IPADDR_PRINT_ARG(pIpHdr->dip));
            return ERRONEOUS_PACKET;
        }

        report = (igmpv3_report_t*)((char*)pIpHdr + pIpHdr->ihl * 4);

        rsd_nic_checksum_get((uint16 *)report, igmplen, &csum);
        if (csum != 0)
        {
            mcast_log_checksum_err(MULTICAST_TYPE_IPV4,vid, lp);
            return ERRONEOUS_PACKET;
        }

        if (report->ngrec == 0)
        {
            return SUPPORTED_IGMP_CONTROL_PACKET;
        }

        grec = report->grec;

        size1 = sizeof(igmpv3_grec_t);

        for (i = 0; i < report->ngrec; i++)
        {
            group = ntohl(grec->grec_mca);
            size2 = grec->grec_nsrcs * 4;
            ptr = (igmpv3_grec_t*)((char*) grec + size1 + size2);
            grec = ptr;
            if (!IGMP_IN_MULTICAST(ntohl(group)))
            {
                MCASTDBG("IGMPv3 Header Error: Group Address("IPADDR_PRINT") is not in "
                    "Multicast range(224.0.0.0~239.255.255.255)", IPADDR_PRINT_ARG(group));
                return ERRONEOUS_PACKET;
            }

            if ((ntohl(group) >= ntohl(0xE0000000L)) && (ntohl(group) <= ntohl(0xE00000FFL)))
            {
                MCASTDBG("IGMPv3 Header Error: Group Address("IPADDR_PRINT") belongs to "
                    "Reserved Multicast range(224.0.0.0~224.0.0.255)!", IPADDR_PRINT_ARG(group));

                mcast_log_invalidGroup(MULTICAST_TYPE_IPV4);
                return ERRONEOUS_PACKET;
            }
        }

        return SUPPORTED_IGMP_CONTROL_PACKET;
    }

    rsd_nic_checksum_get((uint16 *)pIgmpHdr, igmplen, &csum);
    if (csum != 0)
    {
        mcast_log_checksum_err(MULTICAST_TYPE_IPV4, vid, lp);
        return ERRONEOUS_PACKET;
    }

    if ((pIgmpHdr->type == IGMP_HOST_MEMBERSHIP_QUERY) && (0 == pIgmpHdr->groupAddr))
    {
        ;
    }
    else
    {
        if (!IGMP_IN_MULTICAST(ntohl(pIgmpHdr->groupAddr)))
        {
            MCASTDBG("IGMP Header Error: Group Address("IPADDR_PRINT") is not in "
                "Multicast range(224.0.0.0~239.255.255.255)", IPADDR_PRINT_ARG(pIgmpHdr->groupAddr));
            return ERRONEOUS_PACKET;
        }

        if ((ntohl(pIgmpHdr->groupAddr) >= ntohl(0xE0000000L)) && (ntohl(pIgmpHdr->groupAddr) <= ntohl(0xE00000FFL)))
        {
            MCASTDBG("IGMP Header Error: Group Address("IPADDR_PRINT") belongs to "
                "Reserved Multicast range(224.0.0.0~224.0.0.255)!", IPADDR_PRINT_ARG(pIgmpHdr->groupAddr));
            mcast_log_invalidGroup(MULTICAST_TYPE_IPV4);
            return ERRONEOUS_PACKET;
        }
    }

    if (pIgmpHdr->type == IGMP_HOST_MEMBERSHIP_QUERY)
    {
        if (0 == pIgmpHdr->groupAddr)
        {
            
            if (ntohl(0xE0000001L) != ntohl(pIpHdr->dip))
            {
                MCASTDBG("IP Header Error: Dst IP("IPADDR_PRINT") of IGMP GENERAL-QUERY "
                        "packet is incorrect!", IPADDR_PRINT_ARG(pIpHdr->dip));
                return ERRONEOUS_PACKET;
            }
        }
        else
        {
            
            if ((ntohl(0xE0000001L) != ntohl(pIpHdr->dip)) && (pIgmpHdr->groupAddr != pIpHdr->dip))
            {
                MCASTDBG("IGMP Header Error: Group Address("IPADDR_PRINT") is not same "
                        "with the Dst IP("IPADDR_PRINT") of IGMP GS-Specific QUERY packet!",
                        IPADDR_PRINT_ARG(pIgmpHdr->groupAddr), IPADDR_PRINT_ARG(pIpHdr->dip));
                return ERRONEOUS_PACKET;
            }
        }
    }

    if (((pIgmpHdr->type == IGMP_HOST_MEMBERSHIP_REPORT) ||
        (pIgmpHdr->type == IGMP_HOST_NEW_MEMBERSHIP_REPORT)) &&
        (pIgmpHdr->groupAddr != pIpHdr->dip))
    {
        MCASTDBG("IGMP Header Error: Group Address("IPADDR_PRINT") is not same "
            "with the Dst IP("IPADDR_PRINT") of IGMP REPORT packet!",
            IPADDR_PRINT_ARG(pIgmpHdr->groupAddr), IPADDR_PRINT_ARG(pIpHdr->dip));
        return ERRONEOUS_PACKET;
    }

    if ((pIgmpHdr->type == IGMP_HOST_LEAVE_MESSAGE) && (ntohl(0xE0000002L) != ntohl(pIpHdr->dip)))
    {
        MCASTDBG("IP Header Error: Dst IP("IPADDR_PRINT") of IGMP LEAVE packet is incorrect!", IPADDR_PRINT_ARG(pIpHdr->dip));
        return ERRONEOUS_PACKET;
    }

    #ifdef CONFIG_SYS_PROTO_MVR
    if (IGMP_HOST_MEMBERSHIP_REPORT == pIgmpHdr->type ||
        IGMP_HOST_NEW_MEMBERSHIP_REPORT == pIgmpHdr->type ||
        IGMP_HOST_LEAVE_MESSAGE == pIgmpHdr->type)
    {
        groupAdr.ipv4 = pIgmpHdr->groupAddr;
        if (is_mvr_group_ret1(MULTICAST_TYPE_IPV4, 0, &groupAdr, FALSE))
        {
            MCASTDBG("Dst IP("IPADDR_PRINT") is belong to MVR group",IPADDR_PRINT_ARG(pIgmpHdr->groupAddr));
            return MULTICAST_MVR_PACKET;
        }
    }

    if (IGMP_HOST_MEMBERSHIP_QUERY == pIgmpHdr->type)
    {
        
        if (0 != pIgmpHdr->groupAddr)
        {
            groupAdr.ipv4 = pIgmpHdr->groupAddr;
            if (is_mvr_group_ret1(MULTICAST_TYPE_IPV4, vid, &groupAdr, TRUE))
            {
                MCASTDBG("Speciel Query Dst IP("IPADDR_PRINT") is belong to MVR group",IPADDR_PRINT_ARG(pIgmpHdr->groupAddr));
                return MULTICAST_MVR_PACKET;
            }
        }
        else 
        {
            if (vid == igmp_cfg.mvrConfig.mvr_vid && ENABLED == igmp_cfg.mvrConfig.mvr_enable)
            {
                MCASTDBG("General Query is belong to MVR");
                return MULTICAST_MVR_PACKET;
            }
        }
    }
    #endif

    return SUPPORTED_IGMP_CONTROL_PACKET;
}

int32 mcast_igmp_up_handle_membership_query(sys_nic_pkt_t *pBuf, uint16 vid, iphdr_t *pIpHdr, igmp_hdr_t *pIgmpHdr, uint8 isMvrPackt)
{
    int32 ret = SYS_ERR_OK;
    uint32                   t, qqic = 0;
    uint8                    robussVar = 0;
    uint32                  group;
    uint8                   query_version = IGMP_QUERY_V2;
    sys_logic_portmask_t    blockPm, txPm, tmpPmsk;
    uint16                 i;
    uint16                 srcNum = 0;
    uint8                  *srcList = NULL;
    uint8                  *pSrcList = NULL;
    uint32                 srcListSize = SYS_BUF512_LEN;
    uint8                  isSpecQuery = TRUE;
    uint32                lastIntvl = 0;

    igmp_router_entry_t *pEntry = NULL;
    sys_logic_port_t port;
    igmp_group_entry_t *pGroup = NULL, groupEntry;
    uint16 sortedIdx;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    mcast_vlan_entry_t *pMcastVlanEntry = NULL;
    mcast_groupAddr_t  *pGroupAddr = NULL;
    char *portStr = NULL;
    sys_nic_pkt_t *pNewPkt;
    sys_logic_port_t  lport = pBuf->rx_tag.source_port;

    SYS_MEM_ALLOC_HDL(srcList, srcListSize, _exit, ret);
    pSrcList = srcList;

    SYS_MEM_ALLOC_HDL(pMcastVlanEntry, sizeof(mcast_vlan_entry_t), _exit, ret);
    SYS_MEM_ALLOC_HDL(pGroupAddr, sizeof(mcast_groupAddr_t), _exit, ret);
    SYS_MEM_ALLOC_HDL(portStr, CAPA_PORTMASK_STR_LEN, _exit, ret);

    LOGIC_PORTMASK_CLEAR_ALL(blockPm);
    LOGIC_PORTMASK_SET_PORT(blockPm, pBuf->rx_tag.source_port);

    LOGIC_PORTMASK_CLEAR_ALL(txPm);
    LOGIC_PORTMASK_CLEAR_ALL(tmpPmsk);

    group = ntohl (pIgmpHdr->groupAddr);
    pGroupAddr->ipv4 = group;

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        ret = SYS_ERR_FAILED;
        goto _exit;
    }

    mcast_suppreFlag_clear(vid, ipType);

    if ( (pIpHdr->tot_len - 4 * pIpHdr->ihl ) > 8)
    {
        query_version = IGMP_QUERY_V3;
    }

    if((query_version == IGMP_QUERY_V3) && (igmp_cfg.oper_version < IGMP_VERSION_V3_BASIC))
    {
        srcNum = ntohl(pIgmpHdr->v3.numOfSrc);
        if (SYS_ERR_OK == rsd_nic_pkt_clone(pBuf->length, &pNewPkt, pBuf))
        {
            rsd_nic_pktBlkPM_flood(pNewPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_IGMP);
            if (0 == group)
                mcast_blockPortMskStat_inc(ipType,vid, blockPm, MCAST_STAT_GENQRY_RX);
            else
            {
                if (0 == srcNum)
                    mcast_blockPortMskStat_inc(ipType,vid, blockPm, MCAST_STAT_SPEC_GROUP_QRY_RX);
                else
                    mcast_blockPortMskStat_inc(ipType,vid, blockPm, MCAST_STAT_SPEC_GROUP_SRC_QRY_RX);
            }
        }

        goto _exit;
    }

    if(query_version != IGMP_QUERY_V3)
    {
        if (group == 0)     
        {
            t = pIgmpHdr->maxRespTime/10 ? pIgmpHdr->maxRespTime/10 : pMcastVlan->responseTime;
            mcast_portStat_inc(ipType, vid, lport, MCAST_STAT_GENQRY_RX);
            isSpecQuery = FALSE;
            MCASTDBG("A IGMP General QUERY received on Vid = %d, Port-%s from "IPADDR_PRINT". (Interval:%d s)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDR_PRINT_ARG(pIpHdr->sip), t);
        }
        else                
        {
            t = pIgmpHdr->maxRespTime/10 ? pIgmpHdr->maxRespTime/10 : pMcastVlan->responseTime;
            if (pIgmpHdr->maxRespTime/10)
                lastIntvl = pIgmpHdr->maxRespTime/10;
            mcast_portStat_inc(ipType, vid, lport,  MCAST_STAT_SPEC_GROUP_QRY_RX);
            MCASTDBG("A IGMP Group-Specific QUERY received on Vid = %d, Port-%s from "IPADDR_PRINT". (Group:"IPADDR_PRINT" Interval:%d s)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDR_PRINT_ARG(pIpHdr->sip), IPADDR_PRINT_ARG(group), t);
        }
    }
    else
    {
        srcNum = ntohl(pIgmpHdr->v3.numOfSrc);

        if (pIgmpHdr->maxRespTime & 0x80)
        {
            t = ((pIgmpHdr->maxRespTime & 0x0F) | 0x10) << (((pIgmpHdr->maxRespTime & 0x70) >> 4) +3);
            t = t/10;
        }
        else
        {
            t = pIgmpHdr->maxRespTime/10;
        }

       if (t)
          lastIntvl = t;

        if (pIgmpHdr->v3.qqic & 0x80)
        {
            qqic = ((pIgmpHdr->v3.qqic & 0x0F) | 0x10) << (((pIgmpHdr->v3.qqic & 0x70) >> 4)+3);
        }
        else
        {
            qqic = pIgmpHdr->v3.qqic;
        }

        robussVar =  pIgmpHdr->v3.rsq & 0x07;

        if(group == 0)
        {
            t = t  ?  t : pMcastVlan->responseTime;
            
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_GENQRY_RX);
            isSpecQuery = FALSE;
            MCASTDBG("A IGMP General QUERY v3 received on Vid = %d, Port-%s from "IPADDR_PRINT". (Interval:%d sec)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDR_PRINT_ARG(pIpHdr->sip), t );
        }
        else if(srcNum == 0) 
        {
            t = t ? t : pMcastVlan->grpSpecificQueryIntv;
            
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_SPEC_GROUP_QRY_RX);
            MCASTDBG("A IGMP v3 Group-Specific QUERY received on Vid = %d, Port-%s from "IPADDR_PRINT". (Group:"IPADDR_PRINT" Interval:%d sec)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDR_PRINT_ARG(pIpHdr->sip), IPADDR_PRINT_ARG(group), t);
        }
        else  
        {
            osal_memset(srcList,0,srcListSize);
            for(i = 0; i < srcNum; i++)
            {
                pSrcList += sprintf(pSrcList, IPADDR_PRINT, IPADDR_PRINT_ARG( (uint32)(*(&(pIgmpHdr->v3.srcList) + i))));
                pSrcList += sprintf(pSrcList, "  ");

                if (i == 20 )
                    break;
            }

            t = t ? t : pMcastVlan->grpSpecificQueryIntv;
           
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_SPEC_GROUP_SRC_QRY_RX);
            MCASTDBG("A IGMP Group-Specific QUERY received on Vid = %d, Port-%s from "IPADDR_PRINT". (Group:"IPADDR_PRINT" Source List : %s, Interval:%d sec)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDR_PRINT_ARG(pIpHdr->sip), IPADDR_PRINT_ARG(group), srcList, t);
        }
    }

    if (!isMvrPackt)
    {
        if (!IS_IPADDR_EQUAL_UINT32(pIpHdr->sip, proxy_query.addr))
        {
            
            SYS_ERR_HDL(mcast_routerPort_add_wrapper(vid, ipType, lport, TRUE), _exit, ret);
        }
    }

    pMcastVlanEntry->vid = vid;
    pMcastVlanEntry->ipType = ipType;
    SYS_ERR_HDL(mcast_vlan_get(pMcastVlanEntry), _exit, ret);
    if (isSpecQuery)
    {
        pMcastVlanEntry->operGsqueryIntv = lastIntvl ?lastIntvl : pMcastVlanEntry->grpSpecificQueryIntv;

        if (IGMP_QUERY_V3 == query_version)
        {
            pMcastVlanEntry->operLastMmbrQueryCnt = robussVar ? robussVar :pMcastVlanEntry->operRobNessVar;
            pMcastVlanEntry->operRobNessVar = robussVar ? robussVar : pMcastVlanEntry->robustnessVar;
        }
        else
        {
             
            pMcastVlanEntry->operLastMmbrQueryCnt = pMcastVlanEntry->operRobNessVar;
            pMcastVlanEntry->operRobNessVar = pMcastVlanEntry->robustnessVar;
        }
    }
    else
    {
        pMcastVlanEntry->operRespTime = t;

        if (IGMP_QUERY_V3 == query_version)
        {
            pMcastVlanEntry->operQueryIntv = qqic ? qqic : pMcastVlanEntry->queryIntv;
            pMcastVlanEntry->operRobNessVar = robussVar ? robussVar : pMcastVlanEntry->robustnessVar;
        }
        else
        {
            pMcastVlanEntry->operQueryIntv = pMcastVlanEntry->queryIntv;
            pMcastVlanEntry->operRobNessVar = pMcastVlanEntry->robustnessVar;
        }
    }

    SYS_ERR_HDL(mcast_vlan_set(pMcastVlanEntry), _exit, ret);

    if (isSpecQuery)
        SYS_ERR_HDL(mcast_group_portTime_update(ipType, vid, pGroupAddr), _exit, ret);

    mcast_igmp_querier_check(vid, pIpHdr->sip, query_version);

    if (!isMvrPackt)
    {
        mcast_router_db_get(vid, ipType, &pEntry);

        if(pEntry != NULL)
        {
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, blockPm)
            {
                if(IS_LOGIC_PORTMASK_PORTSET(pEntry->router_forbid_pmsk, port))
                {
                    MCASTDBG("port %s is forbidden router port", LPSTR(port));
                    ret = SYS_ERR_OK;
                    goto _exit;
                }
            }

           LOGIC_PORTMASK_OR(txPm, txPm, pEntry->router_pmsk);
        }
    }
    if (group == 0)
    {
        if (!isMvrPackt)
        {
            if (SYS_ERR_OK == rsd_nic_pkt_clone(pBuf->length, &pNewPkt, pBuf))
            {
                rsd_nic_pktBlkPM_flood(pNewPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_IGMP);
                mcast_blockPortMskStat_inc(ipType,vid, blockPm, MCAST_STAT_GENQRY_TX);
            }
        }
        #ifdef CONFIG_SYS_PROTO_MVR
        else
        {
            if (MVR_MODE_DYNAMIC == mcast_mvr_mode_ret(ipType))
            {
                mcast_mvr_pmsk_get(ipType, MVR_TYPE_SOURCE, &tmpPmsk);
                LOGIC_PORTMASK_CLEAR_PORT(tmpPmsk, pBuf->rx_tag.source_port);
                if (!IS_LOGIC_PORTMASK_CLEAR(tmpPmsk))
                {
                    LPM2STR(&tmpPmsk, portStr);
                    MCASTDBG("Send MVR VLAN General query to Source ports %s", portStr);
                    mcast_snooping_tx(pBuf, vid, pBuf->length, &tmpPmsk);
                    mcast_portMskStat_inc(ipType, vid,tmpPmsk, MCAST_STAT_GENQRY_TX);
                }
            }

            LOGIC_PORTMASK_CLEAR_ALL(tmpPmsk);
            mcast_mvr_pmsk_get(ipType, MVR_TYPE_RECEIVER, &tmpPmsk);
            LOGIC_PORTMASK_OR(txPm, txPm, tmpPmsk);
            LOGIC_PORTMASK_CLEAR_PORT(txPm, pBuf->rx_tag.source_port);
            LPM2STR(&txPm, portStr);
            MCASTDBG("Send MVR VLAN General query to Receiver Ports %s", portStr);
            
            mcast_mvr_tx(pBuf, vid, pBuf->length, &txPm);
            mcast_portMskStat_inc(ipType, vid,txPm, MCAST_STAT_GENQRY_TX);
        }
        #endif
    }
    else
    {
        mcast_build_groupEntryKey(vid, ipType, group, NULL, &groupEntry);

        if (IGMP_VERSION_V3_FULL == igmp_cfg.oper_version)
        {
            for(i = 0; i < srcNum; i++)
            {
                groupEntry.sip = (uint32)(*(&(pIgmpHdr->v3.srcList) + i));
                pGroup = mcast_group_get(&groupEntry);
                if (NULL != pGroup)
                {
                    LOGIC_PORTMASK_OR(txPm, txPm, pGroup->mbr);
                }
            }
        }
        else
        {
            mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &pGroup);   
            if (NULL != pGroup)
            {
                LOGIC_PORTMASK_OR(txPm, txPm,pGroup->mbr);
            }
        }

        LOGIC_PORTMASK_CLEAR_PORT(txPm, pBuf->rx_tag.source_port);

        if (!isMvrPackt)
        {
            if (!IS_LOGIC_PORTMASK_CLEAR(txPm))
            {
                LPM2STR(&txPm, portStr);
                MCASTDBG("Send IGMP Special query to PORTS %s", portStr);
                mcast_snooping_tx(pBuf, vid, pBuf->length, &txPm);
                if(srcNum == 0)
                   mcast_portMskStat_inc(ipType,  vid, txPm, MCAST_STAT_SPEC_GROUP_QRY_TX);
                else
                    mcast_portMskStat_inc(ipType, vid, txPm, MCAST_STAT_SPEC_GROUP_SRC_QRY_TX);
            }
            else
            {
                ret = SYS_ERR_OK;
                goto _exit;
            }
        }
        #ifdef CONFIG_SYS_PROTO_MVR
        else
        {
            
            if (MVR_MODE_DYNAMIC == mcast_mvr_mode_ret(ipType))
            {
                mcast_mvr_pmsk_get(MULTICAST_TYPE_IPV4, MVR_TYPE_SOURCE, &tmpPmsk);
                LOGIC_PORTMASK_CLEAR_PORT(tmpPmsk, pBuf->rx_tag.source_port);
                if (!IS_LOGIC_PORTMASK_CLEAR(tmpPmsk))
                {
                    LPM2STR(&tmpPmsk, portStr);
                    MCASTDBG("Send MVR VLAN Special query to Source ports %s", portStr);
                    mcast_snooping_tx(pBuf, vid, pBuf->length, &tmpPmsk);
                    if(srcNum == 0)
                        mcast_portMskStat_inc(ipType,  vid,tmpPmsk, MCAST_STAT_SPEC_GROUP_QRY_TX);
                    else
                        mcast_portMskStat_inc(ipType, vid,tmpPmsk, MCAST_STAT_SPEC_GROUP_SRC_QRY_TX);
                }
            }

            LOGIC_PORTMASK_CLEAR_ALL(tmpPmsk);
            mcast_mvr_pmsk_get(MULTICAST_TYPE_IPV4, MVR_TYPE_RECEIVER, &tmpPmsk);
            LOGIC_PORTMASK_AND(tmpPmsk, tmpPmsk, txPm);
            if (!IS_LOGIC_PORTMASK_CLEAR(tmpPmsk))
            {
                LPM2STR(&tmpPmsk, portStr);
                MCASTDBG("Send MVR VLAN Special query to Receive ports %s", portStr);
                mcast_mvr_tx(pBuf, vid, pBuf->length, &tmpPmsk);
                if(srcNum == 0)
                     mcast_portMskStat_inc(ipType, vid, tmpPmsk, MCAST_STAT_SPEC_GROUP_QRY_TX);
                else
                    mcast_portMskStat_inc(ipType, vid,tmpPmsk, MCAST_STAT_SPEC_GROUP_SRC_QRY_TX);
            }
        }
        #endif
    }

_exit:
    SYS_MEM_FREE(srcList);
    SYS_MEM_FREE(pMcastVlanEntry);
    SYS_MEM_FREE(pGroupAddr);
    SYS_MEM_FREE(portStr);

    return ret;
}

int32 mcast_igmpv12_up_handle_membership_report(sys_nic_pkt_t *pBuf, uint16 vid, iphdr_t *pIpHdr, igmp_hdr_t *pIgmpHdr, uint8 isMvrPackt)
{
    uint8  port;
    sys_logic_portmask_t rt_pmsk;
    uint8 sendFlag = 0;
    int32 ret = 0;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_groupAddr_t groupAddr;
    sys_vid_t vlanId;
    char portMaskStr[CAPA_PORTMASK_STR_LEN];

    vlanId = vid;
    port = pBuf->rx_tag.source_port;
    groupAddr.ipv4 = pIgmpHdr->groupAddr;

    #ifdef CONFIG_SYS_PROTO_MVR
    if (isMvrPackt)
    {
        MCASTDBG("A IGMPv1/2 MVR REPORT received on Vid = %d, Port-%s", vid, LPSTR(pBuf->rx_tag.source_port));
        vlanId = igmp_cfg.mvrConfig.mvr_vid;
    }
    else
    #endif
    {
        MCASTDBG("A IGMPv1/2 REPORT received on Vid = %d, Port-%s", vid, LPSTR(pBuf->rx_tag.source_port));
    }

    ret = mcast_groupMbrPort_add_wrapper(ipType, vlanId, pBuf, &groupAddr, port, &sendFlag, isMvrPackt);

    if (SYS_ERR_MCAST_GROUP_MAX == ret)
        return SYS_ERR_OK;

    if (ENABLED == igmp_cfg.suppre_enable && !sendFlag)
        return SYS_ERR_OK;

    LOGIC_PORTMASK_CLEAR_ALL(rt_pmsk);

    #ifdef CONFIG_SYS_PROTO_MVR
    if (isMvrPackt)
    {
        if (MVR_MODE_DYNAMIC == mcast_mvr_mode_ret(ipType))
        {
            mcast_mvr_pmsk_get(ipType, MVR_TYPE_SOURCE, &rt_pmsk);
            LOGIC_PORTMASK_CLEAR_PORT(rt_pmsk, port);
        }
    }
    else
    #endif
    {
        mcast_routerPort_get(vlanId, ipType, &rt_pmsk);
        
        LOGIC_PORTMASK_CLEAR_PORT(rt_pmsk, port);
    }

    if(!IS_LOGIC_PORTMASK_CLEAR(rt_pmsk))
    {
        osal_memset(portMaskStr, 0, sizeof(portMaskStr));
        LPM2STR(&rt_pmsk, portMaskStr);
        MCASTDBG("IGMP v2 report send to VLAN %d, router PORTS: %s", vlanId,  portMaskStr);
        mcast_snooping_tx(pBuf, vlanId, pBuf->length, &rt_pmsk);
        
        mcast_portMskStat_inc(ipType,vlanId,rt_pmsk, MCAST_STAT_REPORT_TX);
    }

    return SYS_ERR_OK;
}

int32 mcast_igmpv3_handle_isIn(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (IGMP_VERSION_V3_BASIC == igmp_cfg.oper_version)
    {
        mcast_igmpv3_basic_groupMbrPort_isIn_proc(pRcd,vid,portid);
    }
   #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    else
    {
        mcast_igmpv3_groupMbrPort_isIn_proc(pRcd,vid,portid);
    }
   #endif
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_handle_isEx(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (IGMP_VERSION_V3_BASIC == igmp_cfg.oper_version)
    {
        mcast_igmpv3_basic_groupMbrPort_isEx_proc(pRcd,vid,portid);
    }
    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    else
    {
        mcast_igmpv3_groupMbrPort_isEx_proc(pRcd,vid,portid, MODE_IS_EXCLUDE);
    }
    #endif
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_handle_toIn(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (IGMP_VERSION_V3_BASIC == igmp_cfg.oper_version)
    {
        mcast_igmpv3_basic_groupMbrPort_toIn_proc(pRcd,vid,portid);
    }
    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    else
    {
        mcast_igmpv3_groupMbrPort_toIn_proc(pRcd,vid,portid);
    }
    #endif
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_handle_toEx(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (IGMP_VERSION_V3_BASIC == igmp_cfg.oper_version)
    {
        mcast_igmpv3_basic_groupMbrPort_toEx_proc(pRcd,vid,portid);
    }
    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    else
    {
        mcast_igmpv3_groupMbrPort_isEx_proc(pRcd,vid,portid, CHANGE_TO_EXCLUDE_MODE);
    }
   #endif
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_handle_allow(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    mcast_igmpv3_handle_isIn(pRcd, vid, portid);

    return SYS_ERR_OK;
}

int32 mcast_igmpv3_handle_block(igmpv3_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (IGMP_VERSION_V3_BASIC == igmp_cfg.oper_version)
    {
        mcast_igmpv3_basic_groupMbrPort_block_proc(pRcd,vid,portid);
    }
    #ifdef CONFIG_SYS_PROTO_IGMPV3_FULL
    else
    {
        mcast_igmpv3_groupMbrPort_block_proc(pRcd,vid,portid);
    }
   #endif
    return SYS_ERR_OK;
}

int32 mcast_igmpv3_up_handle_membership_report(sys_nic_pkt_t *pBuf, uint16 vid, iphdr_t *pIpHdr,  igmpv3_report_t *pIgmpRpt)
{
    uint16 numofRcds = 0;
    uint8 *groupRcds = NULL;
    uint16  i = 0;
    uint8   rcdType = 0xff;
    uint16 port;
    uint16 numofsrc;
    uint8 auxlen;
    sys_logic_portmask_t rt_pmsk;

    multicast_ipType_t  ipType = MULTICAST_TYPE_IPV4;
    mcast_groupAddr_t groupAddr;
    char portMaskStr[CAPA_PORTMASK_STR_LEN];

    numofRcds = ntohs(pIgmpRpt->ngrec);

    port = pBuf->rx_tag.source_port;

    MCASTDBG("Num of records: %d", numofRcds);

    if(numofRcds != 0)
    {
        groupRcds = (uint8 *)(&(pIgmpRpt->grec));
    }
    for(i = 0; i < numofRcds; i++)
    {
        
        osal_memset(&groupAddr, 0 , sizeof(groupAddr));
        groupAddr.ipv4 = ntohl(((igmpv3_grec_t *)groupRcds)->grec_mca);
        rcdType = ((igmpv3_grec_t *)groupRcds)->grec_type;
        switch(rcdType)
        {
        case MODE_IS_INCLUDE:
            mcast_igmpv3_handle_isIn((igmpv3_grec_t *)groupRcds, vid, port);
            break;
        case MODE_IS_EXCLUDE:
            mcast_igmpv3_handle_isEx((igmpv3_grec_t *)groupRcds, vid, port);
            break;
        case CHANGE_TO_INCLUDE_MODE:
            mcast_igmpv3_handle_toIn((igmpv3_grec_t *)groupRcds, vid, port);
            break;
        case CHANGE_TO_EXCLUDE_MODE:
            mcast_igmpv3_handle_toEx((igmpv3_grec_t *)groupRcds, vid, port);
            break;
        case ALLOW_NEW_SOURCES:
            mcast_igmpv3_handle_allow((igmpv3_grec_t *)groupRcds, vid, port);
            break;
        case BLOCK_OLD_SOURCES:
            mcast_igmpv3_handle_block((igmpv3_grec_t *)groupRcds, vid, port);
            break;
        default:
            break;
        }

        numofsrc = ((igmpv3_grec_t *)groupRcds)->grec_nsrcs;
        auxlen = ((igmpv3_grec_t *)groupRcds)->grec_auxwords;
        groupRcds = groupRcds + 8 + numofsrc*4 + auxlen*4;
    }

    mcast_routerPort_get(vid, ipType, &rt_pmsk);

    LOGIC_PORTMASK_CLEAR_PORT(rt_pmsk, port);

    if(!IS_LOGIC_PORTMASK_CLEAR(rt_pmsk))
    {
        LPM2STR(&rt_pmsk, portMaskStr);
        MCASTDBG("IGMP v3 report send to router PORTS: %s", portMaskStr);
        mcast_snooping_tx(pBuf, vid, pBuf->tail - pBuf->data, &rt_pmsk);
        
        mcast_portMskStat_inc(ipType, vid,rt_pmsk, MCAST_STAT_REPORT_TX);
    }

    return SYS_ERR_OK;
}

int32 mcast_igmpv2_up_handle_membership_leave(sys_nic_pkt_t *pBuf, uint16 vid, iphdr_t *pIpHdr, igmp_hdr_t *pIgmpHdr, uint8 isMvrPackt)
{
    int32 ret = SYS_ERR_OK;
    uint8 port;
    sys_logic_portmask_t rt_pmsk;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_groupAddr_t groupAddr;
    mcast_vlan_entry_t mcastVlanEntry;
    igmp_querier_entry_t  *pQuerier = NULL;
    uint16 sortedIdx;
    igmp_group_entry_t *groupHead = NULL, groupEntry;
    uint8 isExistGroup = FALSE;
    sys_vid_t vlanId;
    igmp_lookup_mode_t lookup_mode;
    char *portMaskStr = NULL;

    SYS_MEM_ALLOC_HDL(portMaskStr, CAPA_PORTMASK_STR_LEN, _exit, ret);

    vlanId = vid;
    port = pBuf->rx_tag.source_port;
    groupAddr.ipv4 = pIgmpHdr->groupAddr;

    if (!isMvrPackt)
    {
        MCASTDBG("IGMP Leave("IPADDR_PRINT") received on port = %s", IPADDR_PRINT_ARG(pIgmpHdr->groupAddr), LPSTR(port));
        mcast_querier_db_get(vid, ipType, &pQuerier);
        if (NULL != pQuerier)
        {
            if (ENABLED == pQuerier->status)
            {
                osal_memset(&mcastVlanEntry, 0, sizeof(mcastVlanEntry));
                mcastVlanEntry.vid = vid;
                mcastVlanEntry.ipType = ipType;
                SYS_ERR_HDL(mcast_vlan_get(&mcastVlanEntry), _exit, ret);
                if (vid == mcastVlanEntry.vid)
                {
                    mcastVlanEntry.operLastMmbrQueryCnt = mcastVlanEntry.lastMmbrQueryCnt;
                    SYS_ERR_HDL(mcast_vlan_set(&mcastVlanEntry), _exit, ret);
                }
            }
        }
    }
    #ifdef CONFIG_SYS_PROTO_MVR
    else
    {
        MCASTDBG("MVR IGMP Leave("IPADDR_PRINT") received on port = %s", IPADDR_PRINT_ARG(pIgmpHdr->groupAddr), LPSTR(port));
        SYS_ERR_HDL(mcast_mvr_vlanId_get(ipType, &vlanId), _exit, ret);
    }
    #endif

    SYS_ERR_HDL(mcast_build_groupEntryKey(vlanId, ipType, groupAddr.ipv4, NULL, &groupEntry), _exit, ret);
    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);

    if (NULL == groupHead)
    {
        MCASTDBG("Leave Group("IPADDR_PRINT") not exsit", IPADDR_PRINT_ARG(pIgmpHdr->groupAddr));
        mcast_log_group_notExsit(ipType);
        ret = SYS_ERR_OK;
        goto _exit;
    }
    else
    {
        isExistGroup = TRUE;
        if (IGMP_GROUP_ENTRY_DYNAMIC == groupHead->form)
        {
            if ( !IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, port))
            {
                MCASTDBG("port %s is not Group("IPADDR_PRINT") member",  LPSTR(port),IPADDR_PRINT_ARG(pIgmpHdr->groupAddr));
                ret = SYS_ERR_OK;
                goto _exit;
            }
        }
    }

    rsd_mcast_lookupMode_get(ipType, &lookup_mode);
    igmp_lookup_mode_func[ipType * IGMP_LOOKUP_MODE_END + lookup_mode].igmp_groupMbrPort_del_wrapper(ipType, vid, pBuf, &groupAddr, port, isMvrPackt);

    if (isExistGroup)
    {
        LOGIC_PORTMASK_CLEAR_ALL(rt_pmsk);
        if (!isMvrPackt)
        {
            mcast_routerPort_get(vlanId, ipType,  &rt_pmsk);
            LOGIC_PORTMASK_CLEAR_PORT(rt_pmsk, port);
        }
        #ifdef CONFIG_SYS_PROTO_MVR
        else
        {
            if ( MVR_MODE_DYNAMIC == mcast_mvr_mode_ret(ipType))
            {
                mcast_mvr_pmsk_get(ipType, MVR_TYPE_SOURCE, &rt_pmsk);
            }
        }
        #endif
        if(!IS_LOGIC_PORTMASK_CLEAR(rt_pmsk))
        {
            LPM2STR(&rt_pmsk, portMaskStr);
            MCASTDBG("IGMP v2 leave send to router PORTS: %s", portMaskStr);
            mcast_snooping_tx(pBuf, vlanId, pBuf->length, &rt_pmsk);
            
            mcast_portMskStat_inc(ipType, vlanId,rt_pmsk, MCAST_STAT_LEAVE_TX);
        }
    }

_exit:
    SYS_MEM_FREE(portMaskStr);
    return ret;
}

int32 mcast_igmp_up_handle_mcst_data(sys_nic_pkt_t *pBuf, uint16 vid, iphdr_t *pIpHdr, uint8 isMvrPackt)
{
    int32 ret = SYS_ERR_OK;
    uint8                   port;
    sys_logic_portmask_t    rt_pmsk;
    igmp_group_entry_t      *pEntry, groupEntry;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_groupAddr_t groupAddr;
    mcast_groupAddr_t sipAddr;
    sys_vid_t vlanId;
    igmp_lookup_mode_t lookup_mode;
    char *portMaskStr = NULL;

    SYS_MEM_ALLOC_HDL(portMaskStr, CAPA_PORTMASK_STR_LEN, _exit, ret);

    vlanId = vid;
    LOGIC_PORTMASK_CLEAR_ALL(rt_pmsk);
    MCASTDBG("A mcst data packet received VLAN %d.  dip:"IPADDR_PRINT"  SA:"IPADDR_PRINT"",
                         vid, IPADDR_PRINT_ARG(pIpHdr->dip), IPADDR_PRINT_ARG(pIpHdr->sip));

    port = pBuf->rx_tag.source_port;

    groupAddr.ipv4 = pIpHdr->dip;
    sipAddr.ipv4 = pIpHdr->sip;

    rsd_mcast_lookupMode_get(ipType,&lookup_mode);

    if(igmp_cfg.oper_version > IGMP_VERSION_V3_BASIC)
        igmp_lookup_mode_func[ipType * IGMP_LOOKUP_MODE_END + lookup_mode].igmpv3_group_mcstData_add_wrapper(ipType, vlanId, pBuf, &groupAddr, &sipAddr, port, isMvrPackt);
    else
        igmp_lookup_mode_func[ipType * IGMP_LOOKUP_MODE_END + lookup_mode].igmp_group_mcstData_add_wrapper(ipType, vlanId, pBuf, &groupAddr, &sipAddr, port, isMvrPackt);

    SYS_ERR_HDL(mcast_build_groupEntryKey(vlanId, ipType, pIpHdr->dip,NULL, &groupEntry), _exit, ret);
    groupEntry.sip = pIpHdr->sip;

    pEntry = mcast_group_get(&groupEntry);
    if (pEntry)
    {
        if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
        {
            
            if(IGMP_VERSION_V3_FULL != igmp_cfg.oper_version)
            {
                if (IGMP_UNKNOWN_MCAST_ACTION_FLOOD == mcast_unknownAct_ret(ipType))
                    LOGIC_PORTMASK_SET_ALL(rt_pmsk);
                else if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(ipType))
                {
                    #ifdef CONFIG_SYS_PROTO_MVR
                    if (isMvrPackt)
                    {
                        mcast_mvr_pmsk_get(ipType, MVR_TYPE_SOURCE, &rt_pmsk);
                    }
                    else
                    #endif
                    {
                        mcast_routerPort_get(vid, ipType, &rt_pmsk);
                    }
                }
                else
                    LOGIC_PORTMASK_CLEAR_ALL(rt_pmsk);
            }
        }
        else        
        {
            LOGIC_PORTMASK_COPY(rt_pmsk, pEntry->mbr);
        }
    }

    LOGIC_PORTMASK_CLEAR_PORT(rt_pmsk, port);

    LPM2STR(&rt_pmsk, portMaskStr);
    MCASTDBG("IGMP send multicast data to PORTS: %s", portMaskStr);

    mcast_snooping_tx(pBuf, vlanId, pBuf->tail - pBuf->data, &rt_pmsk);

_exit:
    SYS_MEM_FREE(portMaskStr);
    return ret;
}
l2g_act_t  mcast_igmp_preCheck(sys_nic_pkt_t *pPkt, void *pCookie)
{
    iphdr_t *pIpHdr = NULL;
    uint16  tagLen = 0;  
    igmp_lookup_mode_t lookup_mode;

    if (TRUE == pPkt->rx_tag.cvid_tagged)
        tagLen = tagLen + 4;

    if (TRUE == pPkt->rx_tag.svid_tagged)
        tagLen = tagLen + 4;

    pIpHdr = (iphdr_t*)(&pPkt->data[6 + 6 + 2 + tagLen]);

    if (0x2 == pIpHdr->protocol)
    {
        return RSD_NIC_HANDLER_ACT_TRAP;
    }
    
    else if(0x59 == pIpHdr->protocol)
    {	
    	if(IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, ospf_all_routing.addr) || 
    		IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, ospf_designated_routing.addr))
    	{
			return RSD_NIC_HANDLER_ACT_KERNEL;
    	}
    }
    else
    {
        if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(MULTICAST_TYPE_IPV4))
        {
            return RSD_NIC_HANDLER_ACT_TRAP;
        }
        else
        {
            if (IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, dvmrp_routing.addr) ||
                IS_IPADDR_EQUAL_UINT32(pIpHdr->dip, pimv2_routing.addr))
                {
                    return RSD_NIC_HANDLER_ACT_TRAP;
                }

            rsd_mcast_lookupMode_get(MULTICAST_TYPE_IPV4, &lookup_mode);
            if (IGMP_DIP_SIP_MODE == lookup_mode)
                return RSD_NIC_HANDLER_ACT_TRAP;
            
            return RSD_NIC_HANDLER_ACT_NONE;
        }
    }

    return RSD_NIC_HANDLER_ACT_TRAP;
}
int32 mcast_igmp_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_vid_t               vid = 0;
    uint16                  pl_len = 0;  
    uint16                  tagLen = 0;  
    int32                   result = 0;
    iphdr_t                 *pIpHdr;
    igmp_hdr_t              *pIgmpHdr;
    sys_logic_portmask_t    blockPm;
    igmpv3_report_t       *pIgmpv3Rpt;
    multicast_ipType_t     ipType = MULTICAST_TYPE_IPV4;
    sys_nic_pkt_t *pNewPkt;

    mcast_vlan_entry_t *pMcastVlan = NULL;
    uint8 dmac[CAPA_MAC_BYTE_NUM];
    uint8  isMvrPackt = FALSE, isMvrEn = FALSE;
    igmp_lookup_mode_t lookup_mode;
    sys_logic_port_t lport = pPkt->rx_tag.source_port;

    #ifdef CONFIG_SYS_PROTO_MVR
    mvr_portType_t portType;
    #endif

    MCASTDBG("Received IGMP packet  DA:"MAC_PRINT"  SA:"MAC_PRINT"", MAC_PRINT_ARG(pPkt->data), MAC_PRINT_ARG(pPkt->data + 6));

    if (DISABLED == gMcast_filteringEnable)
    {
        MCASTDBG("Free IGMP packet becasue multicast filtering is disabled");
        goto igmp_up_exit;
    }

    if (TRUE == pPkt->rx_tag.cvid_tagged)
        tagLen = tagLen + 4;

    if (TRUE == pPkt->rx_tag.svid_tagged)
        tagLen = tagLen + 4;

    if (SYS_ERR_OK != rsd_nic_pktVid_get(pPkt, &vid))
    {
        MCASTDBG("Error: Could not retrieve VID from the frame received !!");
    }

    if (FALSE == rsd_nic_isPortSTPForwad_ret(lport, vid))
    {
        MCASTDBG("Drop packet, Port-%s, VLAN-%d is not in STP Forwarding State", LPSTR(lport), vid);
        mcast_log_stpblock(ipType, vid, lport);
        goto igmp_up_exit;
    }

    LOGIC_PORTMASK_CLEAR_ALL(blockPm);
    LOGIC_PORTMASK_SET_PORT(blockPm, lport);

    #ifdef CONFIG_SYS_PROTO_MVR
    isMvrEn = is_mcast_mvr_enable(ipType);
    #endif

    if (DISABLED == igmp_cfg.igmpsnp_enable && (!isMvrEn))
    {
        MCASTDBG("Flood IGMP packet becasue IGMP & MVR is disabled");
        rsd_nic_pktBlkPM_flood(pPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_IGMP);
        return SYS_ERR_OK;
    }

    if (ENABLED == igmp_packet_stop)
    {
        MCASTDBG("Free IGMP packet becasue packet stop sign is enabled");
        goto igmp_up_exit;
    }

    pIpHdr = (iphdr_t*)(&pPkt->data[6 + 6 + 2 + tagLen]);

    osal_memcpy(dmac, pPkt->data, CAPA_MAC_BYTE_NUM);
    pIgmpHdr = (igmp_hdr_t*)((char*)pIpHdr + pIpHdr->ihl*4);

    if(224 == ((pIgmpHdr->groupAddr >> 24) & 0xff) && 0 == ((pIgmpHdr->groupAddr >> 16) & 0xff) && 0 == ((pIgmpHdr->groupAddr >> 8) & 0xff))
    {
        MCASTDBG("Drop packet, Group is reserved!");
        mcast_log_invalidGroup(MULTICAST_TYPE_IPV4);
        goto igmp_up_exit;
    }

    if (SYS_ERR_OK != mcast_vlan_portIgrFilter_check(vid, lport,ipType))
    {
        MCASTDBG("Drop packet, Port-%s is not in VLAN-%d", LPSTR(lport), vid);
        mcast_log_vlanFilter_drop(ipType, vid, lport);
        goto igmp_up_exit;
    }
    mcast_portStat_inc(ipType, vid,pPkt->rx_tag.source_port, MCAST_STAT_TOTAL_RX);
    result = mcast_igmp_check_header(dmac, pIpHdr,vid, lport);

#if 1 
    if (SUPPORTED_IGMP_CONTROL_PACKET == result ||
        MULTICAST_MVR_PACKET == result ||
        ROUTER_ROUTING_PACKET == result)
    {
        if (1 != pIpHdr->ttl)
        {
            MCASTDBG("IP Header Error: TTL is not equal to 1!");
            result = ERRONEOUS_PACKET;
        }
    }
#endif
    switch (result)
    {
        case SUPPORTED_IGMP_CONTROL_PACKET:
        case  MULTICAST_MVR_PACKET:
            break;

        case UNSUPPORTED_IGMP_CONTROL_PACKET:
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_VALID_RX);
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_OTHER_RX);
            MCASTDBG("Unsupported IGMP Pkt: Port = %s, Vid = %d", LPSTR(lport), vid);
            
            goto igmp_up_exit;

        case ERRONEOUS_PACKET:
            MCASTDBG("ERRONEOUS_PACKET drop");
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_INVALID_RX);
            goto igmp_up_exit;

        case VALID_OTHER_PACKET:
        case MULTICAST_DATA_PACKET:
        case MULTICAST_MVR_DATA_PACKET:
            if (MULTICAST_MVR_DATA_PACKET == result)
                isMvrPackt = TRUE;
            rsd_mcast_lookupMode_get(ipType, &lookup_mode);
            mcast_process_delay();
            if(IGMP_DMAC_VID_MODE == lookup_mode || IGMP_DIP_VID_MODE == lookup_mode)
            {
                if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(ipType))
                {
                    mcast_igmp_up_handle_mcst_data(pPkt, vid, pIpHdr, isMvrPackt);
                 }
            }
            else
            {
                mcast_igmp_up_handle_mcst_data(pPkt, vid, pIpHdr, isMvrPackt);
            }
            goto igmp_up_exit;

        case ROUTER_ROUTING_PACKET:
            MCASTDBG("A Routing Pkt: Port = %s, Vid = %d", LPSTR(pPkt->rx_tag.source_port), vid);

            mcast_routerPort_add_wrapper(vid, ipType, lport,FALSE);

            goto igmp_up_exit;

        default:
            MCASTDBG("An unrecognized packet received");
            goto igmp_up_exit;
    }

    mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_VALID_RX);
    if (MULTICAST_MVR_PACKET != result)
    {
        if (DISABLED == igmp_cfg.igmpsnp_enable)
        {
            mcast_log_vlan_disable(MULTICAST_TYPE_IPV4, vid, lport);
            rsd_nic_pktBlkPM_flood(pPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_IGMP);
            return SYS_ERR_OK;
        }

        #ifdef CONFIG_SYS_PROTO_MVR
        
        if (ENABLED == igmp_cfg.mvrConfig.mvr_enable && vid == igmp_cfg.mvrConfig.mvr_vid)
        {
            MCASTDBG("Flood IGMP packet becasue it is belong to MVR vlan report");
            MCAST_LOG(LOG_MCAST_MVR_VLAN_FLOOD, vid);
            rsd_nic_pktBlkPM_flood(pPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_IGMP);
            return SYS_ERR_OK;
        }
        #endif

        mcast_vlan_db_get(vid, ipType, &pMcastVlan);
        if (NULL == pMcastVlan)
        {
            goto igmp_up_exit;
        }

        if (ENABLED != pMcastVlan->enable)
        {
            mcast_log_vlan_disable(MULTICAST_TYPE_IPV4, vid, lport);
            rsd_nic_pktBlkPM_flood(pPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_IGMP);
            return SYS_ERR_OK;
        }
    }
    #ifdef CONFIG_SYS_PROTO_MVR
    else
    {
        
        if (FALSE == rsd_nic_isPortSTPForwad_ret(lport, igmp_cfg.mvrConfig.mvr_vid))
        {
            MCASTDBG("Drop packet, Port-%s, MVR VLAN-%d is not in STP Forwarding State", LPSTR(pPkt->rx_tag.source_port), igmp_cfg.mvrConfig.mvr_vid);
            goto igmp_up_exit;
        }

        mcast_mvr_portType_get(MULTICAST_TYPE_IPV4, pPkt->rx_tag.source_port, &portType);
        if (IGMP_HOST_MEMBERSHIP_QUERY != pIgmpHdr->type)
        {
            if (MVR_TYPE_NONE == portType)
            {
                MCASTDBG("Drop packet, Port-%s MVR Type is none", LPSTR(pPkt->rx_tag.source_port));
                MCAST_LOG(LOG_MCAST_MVR_TYPE_DROP, vid, LPSTR(lport));
                goto igmp_up_exit;
            }
            if (MVR_MODE_COMPATABLE == igmp_cfg.mvrConfig.mode)
            {
                if (MVR_TYPE_RECEIVER != portType)
                {
                    MCASTDBG("Drop packet, MVR mode is compatible, but Port-%s MVR Type is not receiver", LPSTR(pPkt->rx_tag.source_port));
                    MCAST_LOG(LOG_MCAST_MVR_RECEIVER_DROP,vid, LPSTR(lport));
                    goto igmp_up_exit;
                }
            }
        }
        else
        {
            if (MVR_TYPE_SOURCE != portType)
            {
                MCASTDBG("Drop packet, MVR Special group query, but Port-%s MVR Type is source", LPSTR(pPkt->rx_tag.source_port));
                MCAST_LOG(LOG_MCAST_MVR_NOTSRC_DROP,vid, LPSTR(lport));
                goto igmp_up_exit;
            }
        }

        isMvrPackt = TRUE;
    }
    #endif

    mcast_process_delay();
    switch (pIgmpHdr->type)
    {
    case IGMP_TYPE_MEMBERSHIP_QUERY:
        pl_len = pIpHdr->tot_len - (pIpHdr->ihl*4);
        mcast_igmp_up_handle_membership_query(pPkt, vid, pIpHdr, pIgmpHdr, isMvrPackt);
        break;

    case IGMPV1_TYPE_MEMBERSHIP_REPORT:
    case IGMPV2_TYPE_MEMBERSHIP_REPORT:
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_REPORT_RX);
            mcast_igmpv12_up_handle_membership_report(pPkt, vid, pIpHdr, pIgmpHdr,isMvrPackt);
        break;

    case IGMPV2_TYPE_MEMBERSHIP_LEAVE:
        mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_LEAVE_RX);
        if(igmp_cfg.oper_version != IGMP_VERSION_V3_FULL)
        {
            mcast_igmpv2_up_handle_membership_leave(pPkt, vid, pIpHdr, pIgmpHdr,isMvrPackt);
        }
        break;

    case IGMPV3_TYPE_MEMBERSHIP_REPORT:
        mcast_portStat_inc(ipType,vid, lport, MCAST_STAT_REPORT_RX);
        if(igmp_cfg.oper_version >= IGMP_VERSION_V3_BASIC)
        {
            pIgmpv3Rpt = (igmpv3_report_t*)((char*)pIpHdr + pIpHdr->ihl*4);
            mcast_igmpv3_up_handle_membership_report(pPkt, vid, pIpHdr, pIgmpv3Rpt);
        }
        else
        {
            if (SYS_ERR_OK == rsd_nic_pkt_clone(pPkt->length, &pNewPkt, pPkt))
            {
                rsd_nic_pktBlkPM_flood(pNewPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_IGMP);
                mcast_blockPortMskStat_inc(ipType,vid,blockPm, MCAST_STAT_REPORT_TX);
                mcast_log_ver_flood(ipType, vid);
            }
        }
        break;

    default:
        MCASTDBG("An unrecognized IGMP message received");
        mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_OTHER_RX);
        break;

    }

igmp_up_exit:
    drv_nic_pkt_free(0, pPkt);
    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING

int32 mcast_mldv2_handle_isIn(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (MLD_VERSION_V2_BASIC == mld_cfg.oper_version)
    {
        mcast_mldv2_basic_groupMbrPort_isIn_proc(pRcd,vid,portid);
    }
    else
    {
        
        ;
    }

    return SYS_ERR_OK;
}

int32 mcast_mldv2_handle_isEx(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (MLD_VERSION_V2_BASIC == mld_cfg.oper_version)
    {
        mcast_mldv2_basic_groupMbrPort_isEx_proc(pRcd,vid,portid);
    }
    else
    {
        
        ;
    }

    return SYS_ERR_OK;
}

int32 mcast_mldv2_handle_toIn(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (MLD_VERSION_V2_BASIC == mld_cfg.oper_version)
    {
        mcast_mldv2_basic_groupMbrPort_toIn_proc(pRcd,vid,portid);
    }
    else
    {
        
        ;
    }

    return SYS_ERR_OK;
}

int32 mcast_mldv2_handle_toEx(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (MLD_VERSION_V2_BASIC == mld_cfg.oper_version)
    {
        mcast_mldv2_basic_groupMbrPort_toEx_proc(pRcd,vid,portid);
    }
    else
    {
        
        ;
    }

    return SYS_ERR_OK;
}

int32 mcast_mldv2_handle_allow(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    mcast_mldv2_handle_isIn(pRcd, vid, portid);

    return SYS_ERR_OK;
}

int32 mcast_mldv2_handle_block(mldv2_grec_t *pRcd , uint16 vid,  uint16 portid)
{
    if (MLD_VERSION_V2_BASIC == mld_cfg.oper_version)
    {
        mcast_mldv2_basic_groupMbrPort_block_proc(pRcd,vid,portid);
    }
    else
    {
        
        ;
    }

    return SYS_ERR_OK;
}
static uint8  mcast_mld_dmac_check(uint8 *pMac, uint32 ipAddr)
{
    uint32 dip;

    if (NULL == pMac)
        return FALSE;

    dip = *((uint32 *)(pMac + 2));

    if (dip != ipAddr)
        return FALSE;

    return TRUE;
}
uint8 is_ipv6Addr_equel(const uint8 *pSrc1Addr, const uint8 *pSrc2Addr)
{
    if (pSrc1Addr == NULL || pSrc2Addr == NULL)
        return FALSE;

    if (osal_memcmp(pSrc1Addr, pSrc2Addr, IPV6_ADDR_LEN) == 0)
        return TRUE;

    return FALSE;
}

static int32 mcast_mld_check_header(uint8 *pMac, ipv6hdr_t *pIpv6Hdr, uint32 optionLen)
{
    uint16          i, size1, size2, mldLen;

    mld_hdr_t       *pMldHdr;
    mldv2_report_t  *report;
    mldv2_grec_t   *grec, *ptr;

    uint8  *pGroupIpv6 = NULL;

    uint16 csum = 0;
    uint32  dip;

    if (pIpv6Hdr->version != 6)
    {
        MCASTDBG("IP Header Error: Version  Incorrect!");
        return ERRONEOUS_PACKET;
    }

    dip = IPV6_TO_IPV4(pIpv6Hdr->dipv6);
    if (!mcast_mld_dmac_check(pMac, dip))
    {
         MCASTDBG("IPv6 Header Error: Dst IPv6("IPADDRV6_PRINT") is not match "
            "The Mac address "MAC_PRINT"", IPADDRV6_PRINT_ARG(pIpv6Hdr->dipv6), MAC_PRINT_ARG(pMac));
        return ERRONEOUS_PACKET;
    }

#if 0
    if ((osal_memcmp(pIpv6Hdr->dipv6, ipv6_dvmrp_routing.addr, IPV6_ADDR_LEN) == 0) ||
        (osal_memcmp(pIpv6Hdr->dipv6, ipv6_ospf_all_routing.addr, IPV6_ADDR_LEN) == 0) ||
        (osal_memcmp(pIpv6Hdr->dipv6, ipv6_ospf_designated_routing.addr, IPV6_ADDR_LEN) == 0) ||
        (osal_memcmp(pIpv6Hdr->dipv6, ipv6_pimv2_routing.addr, IPV6_ADDR_LEN) == 0))
#endif
    if (is_ipv6Addr_equel(pIpv6Hdr->dipv6, ipv6_dvmrp_routing.addr))
    {
            return ROUTER_ROUTING_PACKET;
    }

    if ((is_ipv6Addr_equel(pIpv6Hdr->dipv6, ipv6_ospf_all_routing.addr)) ||
        (is_ipv6Addr_equel(pIpv6Hdr->dipv6, ipv6_ospf_designated_routing.addr)))
    {
            return ROUTER_ROUTING_PACKET;
    }

    if (is_ipv6Addr_equel(pIpv6Hdr->dipv6, ipv6_pimv2_routing.addr))
    {
            return ROUTER_ROUTING_PACKET;
    }

    if (pIpv6Hdr->nextHead == NO_NEXT_HEADER  )
    {
            return MULTICAST_DATA_PACKET;
    }

    pMldHdr = (mld_hdr_t *)((uint8 *)pIpv6Hdr + optionLen + MLD_IPV6_HDR_LEN);

    if(pMldHdr->type >= MLD_ROUTER_SOLICITATION && pMldHdr->type <= MLD_REDIRECT)
        return ROUTER_ROUTING_PACKET;

    if (pMldHdr->type == MLD_TYPE_MEMBERSHIP_QUERY ||
        pMldHdr->type == MLD_TYPE_MEMBERSHIP_REPORT ||
        pMldHdr->type == MLD_TYPE_MEMBERSHIP_DONE ||
        pMldHdr->type == MLDV2_TYPE_MEMBERSHIP_REPORT)
    {
        MCASTDBG("MLD type = 0x%X!", pMldHdr->type);
    }
    else
    {
        MCASTDBG("Unsupported mld type, type = 0x%X, may is multicast data !", pMldHdr->type);
        return VALID_OTHER_PACKET;
    }

    mldLen = pIpv6Hdr->tot_len - optionLen;

    if (pMldHdr->type == MLDV2_TYPE_MEMBERSHIP_REPORT)
    {
        if (!is_ipv6Addr_equel ( pIpv6Hdr->dipv6, ipv6_mldv2_report.addr))
        {
            MCASTDBG("Dst IPv6("IPADDRV6_PRINT") in MLDv2 Report is not  FF02::16",
            IPADDRV6_PRINT_ARG(pMldHdr->groupIpv6));
            return ERRONEOUS_PACKET;
        }

        report = (mldv2_report_t*)((char*)pMldHdr);

        csum = csum;
#if 0
        rsd_nic_checksum_get((uint16 *)report, mldLen, &csum);
        if (csum != 0)
        {
            MCASTDBG("MLDv2 Header Error: Inorrect Checksum!");
            osal_printf("%s: %d: %s\n",__FILE__,__LINE__,__FUNCTION__);
            return ERRONEOUS_PACKET;
        }
#endif

        if (0 == report->numRcd )
        {
            return SUPPORTED_IGMP_CONTROL_PACKET;
        }

        grec = report->grec;

        size1 = sizeof(mldv2_grec_t);

        for (i = 0; i < report->numRcd; i++)
        {
            pGroupIpv6 = grec->grec_ipv6Mca;
            size2 = grec->grec_nsrcs * IPV6_ADDR_LEN;
            ptr = (mldv2_grec_t*)((char*) grec + size1 + size2);
            grec = ptr;
            if (pGroupIpv6[0] != 0xff)
            {
                MCASTDBG("MLD Header Error: Group Address("IPADDRV6_PRINT") is not  "
                    "Multicast  address", IPADDRV6_PRINT_ARG(pGroupIpv6));
                return ERRONEOUS_PACKET;
            }

            if ((pGroupIpv6[0] == 0xff) && ( (pGroupIpv6[1]  & 0xf0) == 0x0) &&
                  (osal_memcmp(&pGroupIpv6[2], &ipv6_proxy_query.addr[2] ,IPV6_ADDR_LEN-2) == 0)  )
            {
                MCASTDBG("MLD Header Error: Group Address("IPADDRV6_PRINT") is "
                    "invalid Multicast group address !", IPADDRV6_PRINT_ARG(pGroupIpv6));
                mcast_log_invalidGroup(MULTICAST_TYPE_IPV6);
                return ERRONEOUS_PACKET;
            }
        }

        return SUPPORTED_IGMP_CONTROL_PACKET;
    }

#if 0
    tmp = (uint8 *)pMldHdr;
    for(i = 0; i < 24; i++)
    {
        osal_printf("%2x:", *(tmp+i));
    }
    osal_printf("\n\n");

    rsd_nic_checksum_get((uint16 *)pMldHdr, mldLen, &csum);
    osal_printf("chechk sum = %x \n", csum);
    if (csum != 0)
    {
        MCASTDBG("MLD Header Error: Inorrect Checksum!\n");
        osal_printf("%s: %d: %s",__FILE__,__LINE__,__FUNCTION__);
        return ERRONEOUS_PACKET;
    }
#endif

    if ((pMldHdr->type == MLD_TYPE_MEMBERSHIP_QUERY) &&
        (is_ipv6Addr_equel(pMldHdr->groupIpv6, ipv6_proxy_query.addr)))
    {
        ;
    }
    else
    {
            if (pMldHdr->groupIpv6[0] != 0xff)
            {
                MCASTDBG("MLD Header Error: Group Address("IPADDRV6_PRINT") is not  "
                    "Multicast  address", IPADDRV6_PRINT_ARG(pMldHdr->groupIpv6));
                    return ERRONEOUS_PACKET;
            }
            if ((pMldHdr->groupIpv6[0] == 0xff) && ((pMldHdr->groupIpv6[1] & 0xf0) == 0x0) &&
                  (osal_memcmp(&pMldHdr->groupIpv6[2], &ipv6_proxy_query.addr[2], IPV6_ADDR_LEN -2) == 0)  )
            {
                MCASTDBG("MLD Header Error: Group Address("IPADDRV6_PRINT") is "
                    "invalid Multicast group address !", IPADDRV6_PRINT_ARG(pMldHdr->groupIpv6));
                mcast_log_invalidGroup(MULTICAST_TYPE_IPV6);
                return ERRONEOUS_PACKET;
            }

    }

    if (pMldHdr->type == MLD_TYPE_MEMBERSHIP_QUERY)
    {
         if (is_ipv6Addr_equel(pMldHdr->groupIpv6, ipv6_proxy_query.addr))
        {
             
            if ( (!is_ipv6Addr_equel(pIpv6Hdr->dipv6, ipv6_dip_query1.addr)) &&
                  (!is_ipv6Addr_equel(pIpv6Hdr->dipv6, ipv6_dip_query2.addr)))
            {
                MCASTDBG("IP Header Error: Dst IP("IPADDRV6_PRINT") of MLD GENERAL-QUERY "
                    "packet is incorrect!", IPADDRV6_PRINT_ARG(pIpv6Hdr->dipv6));
                return ERRONEOUS_PACKET;
            }
        }
        else
        {
             
            if ( (is_ipv6Addr_equel(pIpv6Hdr->dipv6, ipv6_dip_query1.addr)) ||
                 (is_ipv6Addr_equel(pIpv6Hdr->dipv6, ipv6_dip_query2.addr)))
            {
                MCASTDBG("Mld Header Error: group IP("IPADDRV6_PRINT") of MLD GENERAL-QUERY "
                    "packet is incorrect!", IPADDRV6_PRINT_ARG(pMldHdr->groupIpv6));
                return ERRONEOUS_PACKET;
            }

            if (!is_ipv6Addr_equel(pMldHdr->groupIpv6, pIpv6Hdr->dipv6))
            {
                MCASTDBG("MLD Header Error: Group Address("IPADDRV6_PRINT") is not same "
                    "with the Dst IPV6("IPADDRV6_PRINT") of MLD GS-Specific QUERY packet!",
                    IPADDRV6_PRINT_ARG(pMldHdr->groupIpv6), IPADDRV6_PRINT_ARG(pIpv6Hdr->dipv6));
                return ERRONEOUS_PACKET;
            }
        }
    }

    if (pMldHdr->type == MLD_TYPE_MEMBERSHIP_REPORT)
    {
        if  (!is_ipv6Addr_equel(pMldHdr->groupIpv6, pIpv6Hdr->dipv6))
        {
            MCASTDBG("MLD Header Error: Group Address("IPADDRV6_PRINT") is not same "
                "with the Dst IPV6("IPADDRV6_PRINT") of MLD REPORT packet!",
                IPADDRV6_PRINT_ARG(pMldHdr->groupIpv6), IPADDRV6_PRINT_ARG(pIpv6Hdr->dipv6));
            return ERRONEOUS_PACKET;
        }
    }

    if (pMldHdr->type == MLD_TYPE_MEMBERSHIP_DONE)
    {
        if (!(is_ipv6Addr_equel (pIpv6Hdr->dipv6, ipv6_dip_leave1.addr) ||
              is_ipv6Addr_equel (pIpv6Hdr->dipv6, ipv6_dip_leave2.addr) ||
              is_ipv6Addr_equel (pIpv6Hdr->dipv6, ipv6_dip_leave3.addr)))
       {
            MCASTDBG("IP Header Error: Dst IP("IPADDRV6_PRINT") of MLD LEAVE packet is incorrect!", IPADDRV6_PRINT_ARG(pIpv6Hdr->dipv6));
            return ERRONEOUS_PACKET;
        }
    }

    return SUPPORTED_IGMP_CONTROL_PACKET;
}
int32 mcast_mld_up_handle_membership_query(sys_nic_pkt_t *pBuf, uint16 vid,
    ipv6hdr_t *pIpv6Hdr, uint16 optionLen)
{
    int32 ret = SYS_ERR_OK;
    uint32                   t, qqic = 0;
    uint8                    robussVar = 0;
    uint32                  group;
    sys_ipv6_addr_t    groupIpv6;
    uint8                   query_version = MLD_QUERY_V1;
    sys_logic_portmask_t    blockPm, txPm;
    uint16                 i;
    uint16                 srcNum = 0;
    uint8                  *srcList = NULL;
    uint32                 srcListSize = SYS_BUF512_LEN;
    uint8                  *pSrcList = NULL;
    uint8                  isSpecQuery = TRUE;
    uint32                lastIntvl = 0;

    igmp_router_entry_t *pEntry = NULL;
    sys_logic_port_t port;
    igmp_group_entry_t *pGroup = NULL, groupEntry;
    uint16 sortedIdx;

    mld_hdr_t *pMldHdr = NULL;
    mldv2_qryhdr_t *pMldv2Hdr = NULL;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    mcast_vlan_entry_t *pMcastVlanEntry = NULL;
    mcast_groupAddr_t  *pGroupAddr = NULL;
    sys_nic_pkt_t *pNewPkt;
    char *portMaskStr = NULL;
    sys_logic_port_t lport = pBuf->rx_tag.source_port;

    SYS_MEM_ALLOC_HDL(portMaskStr, CAPA_PORTMASK_STR_LEN, _exit, ret);
    SYS_MEM_ALLOC_HDL(pMcastVlanEntry, sizeof(mcast_vlan_entry_t), _exit, ret);
    SYS_MEM_ALLOC_HDL(pGroupAddr, sizeof(mcast_groupAddr_t), _exit, ret);
    SYS_MEM_ALLOC_HDL(srcList, srcListSize, _exit, ret);

    LOGIC_PORTMASK_CLEAR_ALL(blockPm);
    LOGIC_PORTMASK_SET_PORT(blockPm, pBuf->rx_tag.source_port);

    LOGIC_PORTMASK_CLEAR_ALL(txPm);

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        ret = SYS_ERR_FAILED;
        goto _exit;
    }

    mcast_suppreFlag_clear(vid, ipType);

    if (pIpv6Hdr->tot_len - optionLen - sizeof(mld_hdr_t) > 0)
    {
        query_version = MLD_QUERY_V2;
        pMldv2Hdr = (mldv2_qryhdr_t *)((uint8 *)pIpv6Hdr + optionLen + MLD_IPV6_HDR_LEN);
        osal_memcpy(groupIpv6.octet, pMldv2Hdr->groupIpv6, IPV6_ADDR_LEN);
    }
    else
    {
        pMldHdr = (mld_hdr_t *)((uint8 *)pIpv6Hdr + optionLen + MLD_IPV6_HDR_LEN);
        osal_memcpy(groupIpv6.octet, pMldHdr->groupIpv6, IPV6_ADDR_LEN);
    }

    group = IPV6_TO_IPV4(groupIpv6.octet);
    pGroupAddr->ipv4 = group;
    osal_memcpy(pGroupAddr->ipv6.addr, groupIpv6.octet, IPV6_ADDR_LEN);

    if((query_version == MLD_QUERY_V2) && (mld_cfg.oper_version < MLD_VERSION_V2_BASIC))
    {
        srcNum = ntohl(pMldv2Hdr->numOfSrc);
        if (SYS_ERR_OK == rsd_nic_pkt_clone(pBuf->length, &pNewPkt, pBuf))
        {
            rsd_nic_pktBlkPM_flood(pNewPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_MLD);
            if (osal_memcmp(groupIpv6.octet, ipv6_proxy_query.addr,IPV6_ADDR_LEN)== 0)
            {
                mcast_blockPortMskStat_inc(ipType, vid,blockPm, MCAST_STAT_GENQRY_TX);
            }
            else
            {
                if (0 == srcNum)
                    mcast_blockPortMskStat_inc(ipType, vid,blockPm, MCAST_STAT_SPEC_GROUP_QRY_TX);
                else
                    mcast_blockPortMskStat_inc(ipType, vid,blockPm, MCAST_STAT_SPEC_GROUP_SRC_QRY_TX);
            }
            
        }

        ret = SYS_ERR_OK;
        goto _exit;
    }
    if(query_version != MLD_QUERY_V2)
    {
        if (osal_memcmp(groupIpv6.octet, ipv6_proxy_query.addr,IPV6_ADDR_LEN)== 0)     
        {
            t = pMldHdr->responseDelay/1000 ? pMldHdr->responseDelay/1000 : pMcastVlan->responseTime;
            mcast_portStat_inc(ipType,vid, lport, MCAST_STAT_GENQRY_RX);
            isSpecQuery = FALSE;
            MCASTDBG("A MLD General QUERY received on Vid = %d, Port-%s from "IPADDRV6_PRINT". (Interval:%d s)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDRV6_PRINT_ARG(pIpv6Hdr->sipv6), t);
        }
        else                
        {
            t = pMldHdr->responseDelay/1000 ? pMldHdr->responseDelay/1000 : pMcastVlan->responseTime;
            if (pMldHdr->responseDelay/1000)
                lastIntvl = pMldHdr->responseDelay/1000;
            
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_SPEC_GROUP_QRY_RX);
            MCASTDBG("A MLD Group-Specific QUERY received on Vid = %d, Port-%s from "IPADDRV6_PRINT". (Group:"IPADDRV6_PRINT" Interval:%d s)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDRV6_PRINT_ARG(pIpv6Hdr->sipv6), IPADDRV6_PRINT_ARG(groupIpv6.octet), t);
        }
    }
    else
    {
        i = i;
        pSrcList = srcList;

        srcNum = ntohl(pMldv2Hdr->numOfSrc);
        
        t = pMldv2Hdr->responseDelay/1000;
        if (t)
          lastIntvl = pMldv2Hdr->responseDelay/1000;

        if (pMldv2Hdr->qqic & 0x80)
        {
            qqic = ((pMldv2Hdr->qqic & 0x0F) | 0x10) << (((pMldv2Hdr->qqic & 0x70) >> 4) +3);
        }
        else
        {
            qqic = pMldv2Hdr->qqic;
        }

        robussVar =  pMldv2Hdr->rsq & 0x07;

        if (osal_memcmp(groupIpv6.octet, ipv6_proxy_query.addr, IPV6_ADDR_LEN)== 0)     
        {
            t = t  ?  t : pMcastVlan->responseTime;
            
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_GENQRY_RX);
            isSpecQuery = FALSE;
            MCASTDBG("A MLDv2 General QUERY  received on Vid = %d, Port-%s from "IPADDRV6_PRINT". (Interval:%d s)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDRV6_PRINT_ARG(pIpv6Hdr->sipv6), t);
        }
        else if(srcNum == 0) 
        {
            t = t ? t : pMcastVlan->grpSpecificQueryIntv;
            
            mcast_portStat_inc(ipType, vid,lport, MCAST_STAT_SPEC_GROUP_QRY_RX);
            MCASTDBG("A MLDv2 Group-Specific QUERY received on Vid = %d, Port-%s from "IPADDRV6_PRINT". (Group:"IPADDRV6_PRINT" Interval:%d s)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDRV6_PRINT_ARG(pIpv6Hdr->sipv6), IPADDRV6_PRINT_ARG(groupIpv6.octet), t);
        }
        else  
        {
            osal_memset(srcList,0, srcListSize);
            for(i = 0; i < srcNum; i++)
            {
                pSrcList += sprintf(pSrcList, IPADDRV6_PRINT, IPADDRV6_PRINT_ARG(pMldv2Hdr->srcList[i].addr));
                pSrcList += sprintf(pSrcList, "  ");

                if (i == 10)
                    break;
            }

            t = t ? t : pMcastVlan->grpSpecificQueryIntv;
            
            mcast_portStat_inc(ipType,vid, lport, MCAST_STAT_SPEC_GROUP_SRC_QRY_RX);
            MCASTDBG("A MLD Group-Specific QUERY received on Vid = %d, Port-%s from "IPADDRV6_PRINT". (Group:"IPADDRV6_PRINT" Source List : %s, Interval:%d s)",
                vid, LPSTR(pBuf->rx_tag.source_port), IPADDRV6_PRINT_ARG(pIpv6Hdr->sipv6), IPADDRV6_PRINT_ARG(groupIpv6.octet), srcList, t);
        }

    }

    if (osal_memcmp(pIpv6Hdr->sipv6, ipv6_proxy_query.addr,IPV6_ADDR_LEN))
    {
        
        SYS_ERR_HDL(mcast_routerPort_add_wrapper(vid, ipType, lport,TRUE), _exit, ret);
    }

    pMcastVlanEntry->vid = vid;
    pMcastVlanEntry->ipType = ipType;
    SYS_ERR_HDL(mcast_vlan_get(pMcastVlanEntry), _exit, ret);
    if (isSpecQuery)
    {
        pMcastVlanEntry->operGsqueryIntv = lastIntvl ? lastIntvl : pMcastVlanEntry->grpSpecificQueryIntv;
        if (MLD_QUERY_V2 == query_version)
        {
            pMcastVlanEntry->operLastMmbrQueryCnt = robussVar ? robussVar :pMcastVlanEntry->lastMmbrQueryCnt;
        }
        else
        {
             
            pMcastVlanEntry->operLastMmbrQueryCnt = pMcastVlanEntry->operRobNessVar;
            pMcastVlanEntry->operRobNessVar = pMcastVlanEntry->robustnessVar;
        }
    }
    else
    {
        pMcastVlanEntry->operRespTime = t;

        if (MLD_QUERY_V2 == query_version)
        {
            pMcastVlanEntry->operQueryIntv = qqic ? qqic : pMcastVlanEntry->queryIntv;
            pMcastVlanEntry->operRobNessVar = robussVar ? robussVar : pMcastVlanEntry->robustnessVar;
        }
        else
        {
            pMcastVlanEntry->operQueryIntv = pMcastVlanEntry->queryIntv;
            pMcastVlanEntry->operRobNessVar = pMcastVlanEntry->robustnessVar;
        }
    }

    SYS_ERR_HDL(mcast_vlan_set(pMcastVlanEntry), _exit, ret);

    if (isSpecQuery)
        SYS_ERR_HDL(mcast_group_portTime_update(ipType, vid, pGroupAddr), _exit, ret);

    mcast_router_db_get(vid, ipType, &pEntry);

    if(pEntry != NULL)
    {
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, blockPm)
        {
            if(IS_LOGIC_PORTMASK_PORTSET(pEntry->router_forbid_pmsk, port))
            {
                MCASTDBG("port %s is forbidden router port", LPSTR(port));
                ret = SYS_ERR_OK;
                goto _exit;
            }
        }

       LOGIC_PORTMASK_OR(txPm, txPm, pEntry->router_pmsk);
    }

    if (osal_memcmp(groupIpv6.octet, ipv6_proxy_query.addr,IPV6_ADDR_LEN)== 0)
    {
        if (SYS_ERR_OK == rsd_nic_pkt_clone(pBuf->length, &pNewPkt, pBuf))
        {
            rsd_nic_pktBlkPM_flood(pNewPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_MLD);
            
            mcast_blockPortMskStat_inc(ipType, vid,blockPm, MCAST_STAT_GENQRY_TX);
        }
    }
    else
    {
        if (MLD_VERSION_V2_FULL == mld_cfg.oper_version)
        {
           
            ;
        }
        else
        {
            SYS_ERR_HDL(mcast_build_groupEntryKey(vid, ipType, group,groupIpv6.octet, &groupEntry), _exit, ret);
            mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &pGroup);   
            if (NULL != pGroup)
            {
                LOGIC_PORTMASK_OR(txPm, txPm,pGroup->mbr);
            }

        }

        LOGIC_PORTMASK_CLEAR_PORT(txPm, pBuf->rx_tag.source_port);

        if (!IS_LOGIC_PORTMASK_CLEAR(txPm))
        {
            LPM2STR(&txPm, portMaskStr);
            MCASTDBG("Send MLD special query to PORTS %s", portMaskStr);
            mcast_snooping_tx(pBuf, vid, pBuf->length, &txPm);

            if(query_version == MLD_QUERY_V2)
            {
                if(srcNum == 0)
                    
                    mcast_portMskStat_inc(ipType, vid,txPm, MCAST_STAT_SPEC_GROUP_QRY_TX);
                else
                    
                    mcast_portMskStat_inc(ipType, vid,txPm, MCAST_STAT_SPEC_GROUP_SRC_QRY_TX);
            }
            else
            {

                mcast_portMskStat_inc(ipType, vid,txPm, MCAST_STAT_SPEC_GROUP_QRY_TX);
            }
        }
    }

_exit:
    SYS_MEM_FREE(portMaskStr);
    SYS_MEM_FREE(pMcastVlanEntry);
    SYS_MEM_FREE(pGroupAddr);
    SYS_MEM_FREE(srcList);
    return ret;
}
int32 mcast_mld_up_handle_membership_report(sys_nic_pkt_t *pBuf, uint16 vid,
    ipv6hdr_t *pIpv6Hdr, mld_hdr_t *pMldHdr)
{

    uint8  port;
    sys_logic_portmask_t rt_pmsk;
    uint8 sendFlag = 0;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;
    mcast_groupAddr_t groupAddr;
    char portMaskStr[CAPA_PORTMASK_STR_LEN];
    int32 ret;
    port = pBuf->rx_tag.source_port;

    MCASTDBG("A MLD REPORT received on Vid = %d, Port-%s", vid, LPSTR(pBuf->rx_tag.source_port));

    osal_memcpy(groupAddr.ipv6.addr, pMldHdr->groupIpv6, IPV6_ADDR_LEN);
    osal_memcpy(groupAddr.sipv6.addr, pIpv6Hdr->sipv6, IPV6_ADDR_LEN);
    ret = mcast_groupMbrPort_add_wrapper(ipType, vid, pBuf, &groupAddr, port, &sendFlag, FALSE);

    if (SYS_ERR_MCAST_GROUP_MAX == ret)
        return SYS_ERR_OK;

    if (ENABLED == mld_cfg.suppre_enable && !sendFlag)
        return SYS_ERR_OK;

    mcast_routerPort_get(vid, ipType, &rt_pmsk);
    
    LOGIC_PORTMASK_CLEAR_PORT(rt_pmsk, port);

    if(!IS_LOGIC_PORTMASK_CLEAR(rt_pmsk))
    {
        LPM2STR(&rt_pmsk, portMaskStr);
        MCASTDBG("Send MLD report to router PORTS %s", portMaskStr);
        mcast_snooping_tx(pBuf, vid, pBuf->tail - pBuf->data, &rt_pmsk);
        
        mcast_portMskStat_inc(ipType, vid,rt_pmsk, MCAST_STAT_REPORT_TX);
    }
    return SYS_ERR_OK;
}
int32 mcast_mldv2_up_handle_membership_report(sys_nic_pkt_t *pBuf, uint16 vid,
    ipv6hdr_t *pIpv6Hdr,  mldv2_report_t *pMldRpt)
{
    uint16 numofRcds = 0;
    uint8 *groupRcds = NULL;
    uint16  i = 0;
    uint8   rcdType = 0xff;

    uint16 port;
    uint16 numofsrc;
    uint8 auxlen;
    sys_logic_portmask_t rt_pmsk;

    multicast_ipType_t  ipType = MULTICAST_TYPE_IPV6;
    mcast_groupAddr_t groupAddr;
    char portMaskStr[CAPA_PORTMASK_STR_LEN];

    numofRcds = ntohs(pMldRpt->numRcd);
    
    port = pBuf->rx_tag.source_port;

    MCASTDBG("Num of records: %d", numofRcds);

    if(numofRcds != 0)
    {
        groupRcds = (uint8 *)(&(pMldRpt->grec));
    }
    for(i = 0; i < numofRcds; i++)
    {
        
        osal_memset(&groupAddr, 0, sizeof(groupAddr));
        osal_memcpy(groupAddr.ipv6.addr, ((mldv2_grec_t *)groupRcds)->grec_ipv6Mca, CAPA_IPV6_BYTE_NUM);
        rcdType = ((mldv2_grec_t *)groupRcds)->grec_type;
        switch(rcdType)
        {
        case MODE_IS_INCLUDE:
            mcast_mldv2_handle_isIn((mldv2_grec_t *)groupRcds, vid, port);
            break;
        case MODE_IS_EXCLUDE:
            mcast_mldv2_handle_isEx((mldv2_grec_t *)groupRcds, vid, port);
            break;
        case CHANGE_TO_INCLUDE_MODE:
            mcast_mldv2_handle_toIn((mldv2_grec_t *)groupRcds, vid, port);
            break;
        case CHANGE_TO_EXCLUDE_MODE:
            mcast_mldv2_handle_toEx((mldv2_grec_t *)groupRcds, vid, port);
            break;
        case ALLOW_NEW_SOURCES:
            mcast_mldv2_handle_allow((mldv2_grec_t *)groupRcds, vid, port);
            break;
        case BLOCK_OLD_SOURCES:
            mcast_mldv2_handle_block((mldv2_grec_t *)groupRcds, vid, port);
            break;
        default:
            break;
        }

        numofsrc = ((mldv2_grec_t *)groupRcds)->grec_nsrcs;
        
        auxlen = ((mldv2_grec_t *)groupRcds)->grec_auxwords;
        groupRcds = groupRcds + sizeof(mldv2_grec_t)  + numofsrc* IPV6_ADDR_LEN + auxlen*4;
    }

    mcast_routerPort_get(vid, ipType, &rt_pmsk);

    LOGIC_PORTMASK_CLEAR_PORT(rt_pmsk, port);

    if(!IS_LOGIC_PORTMASK_CLEAR(rt_pmsk))
    {
        LPM2STR(&rt_pmsk, portMaskStr);
        MCASTDBG("Send MLDv2 report to router PORTS %s", portMaskStr);
        mcast_snooping_tx(pBuf, vid, pBuf->tail - pBuf->data, &rt_pmsk);
        
        mcast_portMskStat_inc(ipType, vid,rt_pmsk, MCAST_STAT_REPORT_TX);
    }

    return SYS_ERR_OK;
}
int32 mcast_mld_up_handle_membership_leave(sys_nic_pkt_t *pBuf, uint16 vid,
    ipv6hdr_t *pIpv6Hdr, mld_hdr_t *pMldHdr)
{
    int32 ret = SYS_ERR_OK;
    uint8 port;
    sys_logic_portmask_t rt_pmsk;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;
    mcast_groupAddr_t groupAddr;
    mcast_vlan_entry_t mcastVlanEntry;
    igmp_querier_entry_t *pQuerier = NULL;
    uint16 sortedIdx;
    igmp_group_entry_t *groupHead = NULL, groupEntry;
    uint8 isGroupExist = FALSE;
    igmp_lookup_mode_t lookup_mode;
    char *portMaskStr = NULL;

    port = pBuf->rx_tag.source_port;

    MCASTDBG("MLD Leave("IPADDRV6_PRINT") received on port = %s", IPADDRV6_PRINT_ARG(pMldHdr->groupIpv6), LPSTR(port));

    SYS_MEM_ALLOC_HDL(portMaskStr, CAPA_PORTMASK_STR_LEN, _exit, ret);

    mcast_querier_db_get(vid, ipType, &pQuerier);
    if (NULL != pQuerier)
    {
        if (ENABLED == pQuerier->status)
        {
            osal_memset(&mcastVlanEntry, 0, sizeof(mcastVlanEntry));
            mcastVlanEntry.vid = vid;
            mcastVlanEntry.ipType = ipType;
            SYS_ERR_HDL(mcast_vlan_get(&mcastVlanEntry), _exit, ret);
            if (vid == mcastVlanEntry.vid)
            {
                mcastVlanEntry.operLastMmbrQueryCnt = mcastVlanEntry.lastMmbrQueryCnt;
                SYS_ERR_HDL(mcast_vlan_set(&mcastVlanEntry), _exit, ret);
            }
        }
    }

    osal_memcpy(groupAddr.ipv6.addr, pMldHdr->groupIpv6, IPV6_ADDR_LEN);
    groupAddr.ipv4 = IPV6_TO_IPV4(groupAddr.ipv6.addr);

    SYS_ERR_HDL(mcast_build_groupEntryKey(vid, ipType, groupAddr.ipv4, groupAddr.ipv6.addr, &groupEntry), _exit, ret);
    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);

    if (NULL == groupHead)
    {
        MCASTDBG("Leave Group("IPADDRV6_PRINT")  is not exist",IPADDRV6_PRINT_ARG(pMldHdr->groupIpv6));
        mcast_log_group_notExsit(ipType);
        ret = SYS_ERR_OK;
        goto _exit;
    }
    else
    {
        isGroupExist = TRUE;
        if ( !IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, port))
        {
            MCASTDBG("port %s is not ipv6 Group("IPADDRV6_PRINT") member",  LPSTR(port),IPADDRV6_PRINT_ARG(pMldHdr->groupIpv6));
            ret = SYS_ERR_OK;
            goto _exit;
        }
    }

    rsd_mcast_lookupMode_get(ipType,&lookup_mode);
    igmp_lookup_mode_func[ipType * IGMP_LOOKUP_MODE_END + lookup_mode].igmp_groupMbrPort_del_wrapper(ipType, vid, pBuf, &groupAddr, port, FALSE);

    if (isGroupExist)
    {
        mcast_routerPort_get(vid, ipType,  &rt_pmsk);
        LOGIC_PORTMASK_CLEAR_PORT(rt_pmsk, port);

        if(!IS_LOGIC_PORTMASK_CLEAR(rt_pmsk))
        {
            LPM2STR(&rt_pmsk, portMaskStr);
            MCASTDBG("Send MLD leave to router PORTS %s", portMaskStr);
            mcast_snooping_tx(pBuf, vid, pBuf->tail - pBuf->data, &rt_pmsk);
           
           mcast_portMskStat_inc(ipType,vid, rt_pmsk, MCAST_STAT_LEAVE_TX);
        }
    }

_exit:
    SYS_MEM_FREE(portMaskStr);

    return ret;
}
int32 mcast_mld_up_handle_mcst_data(sys_nic_pkt_t *pBuf, uint16 vid, ipv6hdr_t *pIpv6Hdr)
{
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t           port;
    sys_logic_portmask_t    rt_pmsk;
    igmp_group_entry_t      *pEntry, *pGroupEntry = NULL;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;
    mcast_groupAddr_t groupAddr;
    mcast_groupAddr_t  sipAddr;
    igmp_lookup_mode_t lookup_mode;
    char portMaskStr[CAPA_PORTMASK_STR_LEN];

    LOGIC_PORTMASK_CLEAR_ALL(rt_pmsk);
    MCASTDBG("A mcst data packet received.  dipv6:"IPADDRV6_PRINT"  SA:"IPADDRV6_PRINT"",
                                              IPADDRV6_PRINT_ARG(pIpv6Hdr->dipv6), IPADDRV6_PRINT_ARG(pIpv6Hdr->sipv6));

    SYS_MEM_ALLOC_HDL(pGroupEntry, sizeof(igmp_group_entry_t), _exit, ret);

    port = pBuf->rx_tag.source_port;

    groupAddr.ipv4 = IPV6_TO_IPV4(pIpv6Hdr->dipv6);
    sipAddr.ipv4 = IPV6_SIP_TO_IPV4(pIpv6Hdr->sipv6);
    osal_memcpy(groupAddr.ipv6.addr, pIpv6Hdr->dipv6, IPV6_ADDR_LEN);
    osal_memcpy(sipAddr.ipv6.addr, pIpv6Hdr->sipv6, IPV6_ADDR_LEN);

    rsd_mcast_lookupMode_get(ipType, &lookup_mode);

    if(mld_cfg.oper_version <= MLD_VERSION_V2_BASIC)
        igmp_lookup_mode_func[ipType * IGMP_LOOKUP_MODE_END + lookup_mode].igmp_group_mcstData_add_wrapper(ipType, vid, pBuf, &groupAddr, &sipAddr, port, FALSE);

    pGroupEntry->vid = vid;
    pGroupEntry->ipType = ipType;
    pGroupEntry->dip = groupAddr.ipv4;
    pGroupEntry->sip = sipAddr.ipv4;
    pEntry = mcast_group_get(pGroupEntry);
    if (pEntry)
    {
        if (IS_LOGIC_PORTMASK_CLEAR(pEntry->mbr))
        {
            
            if(MLD_VERSION_V2_FULL != mld_cfg.oper_version)
            {
                if (IGMP_UNKNOWN_MCAST_ACTION_FLOOD == mcast_unknownAct_ret(ipType))
                    LOGIC_PORTMASK_SET_ALL(rt_pmsk);
                else if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(ipType))
                    mcast_routerPort_get(vid, ipType, &rt_pmsk);
                else
                    LOGIC_PORTMASK_CLEAR_ALL(rt_pmsk);
            }
        }
        else        
        {
            mcast_routerPort_get(vid, ipType, &rt_pmsk);
            LOGIC_PORTMASK_OR(rt_pmsk, rt_pmsk, pEntry->mbr);
        }
    }

    LOGIC_PORTMASK_CLEAR_PORT(rt_pmsk, port);

    LPM2STR(&rt_pmsk, portMaskStr);
    MCASTDBG("Send Ipv6 multicast data  to PORTS %s", portMaskStr);
    mcast_snooping_tx(pBuf, vid, pBuf->tail - pBuf->data, &rt_pmsk);

_exit:
    SYS_MEM_FREE(pGroupEntry);

    return SYS_ERR_OK;
}
l2g_act_t  mcast_mld_preCheck(sys_nic_pkt_t *pPkt, void *pCookie)
{
    uint16                  tagLen = 0;  
    ipv6hdr_t                 *pIpv6Hdr = NULL;
    uint8        nextHeader = NO_NEXT_HEADER;
    uint8        *ptr;
    uint16      extLen;
    mld_hdr_t              *pMldHdr = NULL;
    uint8       isPotocal = FALSE;

    MCASTDBG("Received MLD packet  DA:"MAC_PRINT"  SA:"MAC_PRINT"", MAC_PRINT_ARG(pPkt->data), MAC_PRINT_ARG(pPkt->data + 6));
    
    if (DISABLED == mld_cfg.mldsnp_enable)
        return RSD_NIC_HANDLER_ACT_NONE;

    if (TRUE == pPkt->rx_tag.cvid_tagged)
        tagLen = tagLen + 4;

    if (TRUE == pPkt->rx_tag.svid_tagged)
        tagLen = tagLen + 4;
    pIpv6Hdr = (ipv6hdr_t*)(&pPkt->data[6 + 6 + 2 + tagLen]);
    nextHeader = pIpv6Hdr->nextHead;

    if ((osal_memcmp(pIpv6Hdr->dipv6, ipv6_dvmrp_routing.addr, IPV6_ADDR_LEN) == 0) ||
        (osal_memcmp(pIpv6Hdr->dipv6, ipv6_ospf_all_routing.addr, IPV6_ADDR_LEN) == 0) ||
        (osal_memcmp(pIpv6Hdr->dipv6, ipv6_ospf_designated_routing.addr, IPV6_ADDR_LEN) == 0)||
        (osal_memcmp(pIpv6Hdr->dipv6, ipv6_pimv2_routing.addr, IPV6_ADDR_LEN) == 0))
    {
        if (ENABLED == mld_cfg.mldsnp_enable)
            return RSD_NIC_HANDLER_ACT_TRAP;
    }
    if (pIpv6Hdr->nextHead == NO_NEXT_HEADER  )
    {
         return RSD_NIC_HANDLER_ACT_TRAP;
    }

    if ((pIpv6Hdr->dipv6[1] == 0x01) || (pIpv6Hdr->dipv6[1] == 0x02) || (pIpv6Hdr->dipv6[1] == 0x05) )
        isPotocal = TRUE;

    ptr = (uint8 *)pIpv6Hdr;
    ptr += MLD_IPV6_HDR_LEN;

    while (NO_NEXT_HEADER != nextHeader)
    {
        switch(nextHeader)
        {
            case HOP_BY_HOP_OPTIONS_HEADER:
                nextHeader = ptr[0];
                extLen = (((uint16)ptr[1]) + 1) * 8;
                ptr += extLen;
                break;

            case ROUTING_HEADER:
                nextHeader = ptr[0];
                extLen = (((uint16)ptr[1]) + 1) * 8;
                ptr += extLen;
                break;

            case FRAGMENT_HEADER:
                nextHeader = ptr[0];
                ptr += 8;
                break;

            case AUTHENTICATION_HEADER:
                nextHeader = ptr[0];
                extLen = (((uint16)ptr[1]) + 2) * 4;
                ptr+= extLen;
                break;

            case DESTINATION_OPTION_HEADER:
                nextHeader = ptr[0];
                extLen = (((uint16)ptr[1]) + 1) * 8;
                ptr += extLen;
                break;

            case MLD_NH_ICMPV6:
                nextHeader = NO_NEXT_HEADER;
                break;
            case NO_NEXT_HEADER:
                return RSD_NIC_HANDLER_ACT_TRAP;
                break;

            default:
                if (!isPotocal)
                    return RSD_NIC_HANDLER_ACT_TRAP;
                else
                {
                    return RSD_NIC_HANDLER_ACT_NONE;
                }
                break;
        }
    }

    pMldHdr = (mld_hdr_t *)ptr;

    if (MLD_TYPE_MEMBERSHIP_QUERY == pMldHdr->type ||
        MLD_TYPE_MEMBERSHIP_REPORT  == pMldHdr->type ||
        MLD_TYPE_MEMBERSHIP_DONE  == pMldHdr->type ||
        MLDV2_TYPE_MEMBERSHIP_REPORT == pMldHdr->type )
    {
        return RSD_NIC_HANDLER_ACT_TRAP;
    }

    return RSD_NIC_HANDLER_ACT_NONE;
}
int32 mcast_mld_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_vid_t               vid = 0;
    uint16                  tagLen = 0;  
    int32                   result = 0;
    ipv6hdr_t                 *pIpv6Hdr = NULL;
    multicast_ipType_t     ipType = MULTICAST_TYPE_IPV6;

    mld_hdr_t              *pMldHdr = NULL;
    mldv2_report_t       *pMldv2Rpt;
    uint8        *ptr;
    uint8        nextHeader = NO_NEXT_HEADER;
    uint16      extLen;
    uint8        isData = TRUE;
    uint8        dmac[CAPA_MAC_BYTE_NUM];
    sys_nic_pkt_t *pNewPkt;

    uint32     optionLen = 0;

    sys_logic_portmask_t    blockPm;

    mcast_vlan_entry_t *pMcastVlan = NULL;
    igmp_lookup_mode_t lookup_mode;
    sys_logic_port_t lrxPort = pPkt->rx_tag.source_port;
    MCASTDBG("Received MLD packet  DA:"MAC_PRINT"  SA:"MAC_PRINT"", MAC_PRINT_ARG(pPkt->data), MAC_PRINT_ARG(pPkt->data + 6));

    if (DISABLED == gMcast_filteringEnable)
    {
        MCASTDBG("Free MLD packet becasue multicast filtering is disabled");
        goto mld_up_exit;
    }

    if (TRUE == pPkt->rx_tag.cvid_tagged)
        tagLen = tagLen + 4;

    if (TRUE == pPkt->rx_tag.svid_tagged)
        tagLen = tagLen + 4;
    if (SYS_ERR_OK != rsd_nic_pktVid_get(pPkt, &vid))
    {
        MCASTDBG("Error: Could not retrieve VID from the frame received !!");
    }

    if (FALSE == rsd_nic_isPortSTPForwad_ret(lrxPort, vid))
    {
        MCASTDBG("Drop packet, Port-%s is not in STP Forwarding State", LPSTR(lrxPort));
        mcast_log_stpblock(ipType, vid, lrxPort);
        goto mld_up_exit;
    }

    LOGIC_PORTMASK_CLEAR_ALL(blockPm);
    LOGIC_PORTMASK_SET_PORT(blockPm, lrxPort);

    if (ENABLED == igmp_packet_stop)
    {
        MCASTDBG("Free MLD packet becasue packet stop sign is enabled");
        goto mld_up_exit;
    }
    
    if (SYS_ERR_OK != mcast_vlan_portIgrFilter_check(vid, lrxPort,ipType))
    {
        mcast_log_vlanFilter_drop(ipType, vid, lrxPort);
        goto mld_up_exit;
    }

    mcast_portStat_inc(ipType, vid,lrxPort, MCAST_STAT_TOTAL_RX);

    osal_memcpy(dmac, pPkt->data, CAPA_MAC_BYTE_NUM);
    pIpv6Hdr = (ipv6hdr_t*)(&pPkt->data[6 + 6 + 2 + tagLen]);
    nextHeader = pIpv6Hdr->nextHead;

    ptr = (uint8 *)pIpv6Hdr;
    ptr += MLD_IPV6_HDR_LEN;

    while (NO_NEXT_HEADER != nextHeader)
    {
        switch(nextHeader)
        {
            case HOP_BY_HOP_OPTIONS_HEADER:
                nextHeader = ptr[0];
                extLen = (((uint16)ptr[1]) + 1) * 8;
                ptr += extLen;
                break;

            case ROUTING_HEADER:
                nextHeader = ptr[0];
                extLen = (((uint16)ptr[1]) + 1) * 8;
                ptr += extLen;
                break;

            case FRAGMENT_HEADER:
                nextHeader = ptr[0];
                ptr += 8;
                break;

            case AUTHENTICATION_HEADER:
                nextHeader = ptr[0];
                extLen = (((uint16)ptr[1]) + 2) * 4;
                ptr+= extLen;
                break;

            case DESTINATION_OPTION_HEADER:
                nextHeader = ptr[0];
                extLen = (((uint16)ptr[1]) + 1) * 8;
                ptr += extLen;
                break;

            case MLD_NH_ICMPV6:
                nextHeader = NO_NEXT_HEADER;
                isData = FALSE;
                break;

            default:
                isData = FALSE;
                break;
        }

        if (!isData)
            break;
    }

    optionLen = ptr - (uint8 *)pIpv6Hdr - MLD_IPV6_HDR_LEN;

    pMldHdr = (mld_hdr_t *)ptr;
    result = mcast_mld_check_header(dmac, pIpv6Hdr, optionLen);
    if (ROUTER_ROUTING_PACKET != result)
    {
        if (isData)
            result = MULTICAST_DATA_PACKET;
    }

    if (SUPPORTED_IGMP_CONTROL_PACKET == result ||
        ROUTER_ROUTING_PACKET == result)
    {
        if (1 != pIpv6Hdr->hopLimit)
        {
            MCASTDBG("IPv6 Header Error: HOPLIMIT is not equal to 1!");
            result =  ERRONEOUS_PACKET;
        }
    }

    switch (result)
    {
        case SUPPORTED_IGMP_CONTROL_PACKET:
            break;

        case UNSUPPORTED_IGMP_CONTROL_PACKET:

            mcast_portStat_inc(ipType, vid,lrxPort, MCAST_STAT_VALID_RX);
            mcast_portStat_inc(ipType,vid, lrxPort, MCAST_STAT_OTHER_RX);
            MCASTDBG("Unsupported MLD Pkt: Port = %s, Vid = %d", LPSTR(lrxPort), vid);
            goto mld_up_exit;

        case ERRONEOUS_PACKET:
            MCASTDBG("ERRONEOUS_PACKET drop");
            
            mcast_portStat_inc(ipType, vid,lrxPort, MCAST_STAT_INVALID_RX);
            goto mld_up_exit;

        case VALID_OTHER_PACKET:
        case MULTICAST_DATA_PACKET:
            rsd_mcast_lookupMode_get(ipType, &lookup_mode);
            mcast_process_delay();
            if (IGMP_DMAC_VID_MODE == lookup_mode || IGMP_DIP_VID_MODE == lookup_mode)
            {
                if (IGMP_UNKNOWN_MCAST_ACTION_ROUTER_PORT == mcast_unknownAct_ret(ipType))
                {
                    mcast_mld_up_handle_mcst_data(pPkt, vid, pIpv6Hdr);
                 }
            }
            else
            {
                mcast_mld_up_handle_mcst_data(pPkt, vid, pIpv6Hdr);
            }
            goto mld_up_exit;

        case ROUTER_ROUTING_PACKET:
            MCASTDBG("A Routing Pkt: Port = %s, Vid = %d", LPSTR(lrxPort), vid);

            mcast_routerPort_add_wrapper(vid, ipType, lrxPort,FALSE);
            
            goto mld_up_exit;

        default:
            MCASTDBG("An unrecognized packet received");
            goto mld_up_exit;
    }

    mcast_portStat_inc(ipType, vid,lrxPort, MCAST_STAT_VALID_RX);

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
    {
        MCASTDBG("Error: Could not get MLD vlan information!!");
        goto mld_up_exit;
    }
    
    if (ENABLED != pMcastVlan->enable)
    {
        mcast_log_vlan_disable(MULTICAST_TYPE_IPV6, vid, lrxPort);
        rsd_nic_pktBlkPM_flood(pPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_MLD);
        return SYS_ERR_OK;
    }

    mcast_process_delay();
    switch (pMldHdr->type)
    {
        case MLD_TYPE_MEMBERSHIP_QUERY:
            mcast_mld_up_handle_membership_query(pPkt, vid, pIpv6Hdr, optionLen);
            break;

        case MLD_TYPE_MEMBERSHIP_REPORT:
            
            mcast_portStat_inc(ipType, vid,lrxPort, MCAST_STAT_REPORT_RX);
            mcast_mld_up_handle_membership_report(pPkt, vid, pIpv6Hdr, pMldHdr);
            break;

        case MLD_TYPE_MEMBERSHIP_DONE:
            mcast_portStat_inc(ipType, vid,lrxPort, MCAST_STAT_LEAVE_RX);
            if(mld_cfg.oper_version != MLD_VERSION_V2_FULL)
            {
                
                mcast_mld_up_handle_membership_leave(pPkt, vid, pIpv6Hdr, pMldHdr);
            }
            break;

        case MLDV2_TYPE_MEMBERSHIP_REPORT:
            
            mcast_portStat_inc(ipType, vid,lrxPort, MCAST_STAT_REPORT_RX);
            if(mld_cfg.oper_version >= MLD_VERSION_V2_BASIC)
            {
                pMldv2Rpt = (mldv2_report_t*)((char *)pMldHdr);
                mcast_mldv2_up_handle_membership_report(pPkt, vid, pIpv6Hdr, pMldv2Rpt);
            }
            else
            {
                if (SYS_ERR_OK == rsd_nic_pkt_clone(pPkt->length, &pNewPkt, pPkt))
                {
                    rsd_nic_pktBlkPM_flood(pNewPkt, vid, blockPm, SYS_CPU_PORT_PRIORITY_MLD);
                    mcast_blockPortMskStat_inc(ipType, vid,blockPm, MCAST_STAT_REPORT_TX);
                    mcast_log_ver_flood(MULTICAST_TYPE_IPV6, vid);
                }
            }
            break;

        default:
            MCASTDBG("An unrecognized MLD message received");
            
            mcast_portStat_inc(ipType, vid,lrxPort, MCAST_STAT_OTHER_RX);
            break;

    }
mld_up_exit:
    drv_nic_pkt_free(0,pPkt);

    return SYS_ERR_OK;
}
#endif

