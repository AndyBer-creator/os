#include <rsd/rsd_l2.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_rate.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_vlan.h>
#include <isg/proto_isg.h>
#include <dhcp/proto_dhcp.h>
#include <dai/proto_dai.h>
#include <common/sys_portview.h>
#define LOG_PRINT_INTERVAL      10

static int32 _arp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
static l2g_act_t _arp_pkt_ratelimit_chk(sys_nic_pkt_t *pPkt, void *pCookie);

static rsd_nic_handler_t l2gARPHandler = {
    .priority       = RSD_NIC_HANDLER_PRI_DAI,
    .dmac           = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .dmac_caremask  = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .ethertype      = 0x806,
    .proto          = 0,
    .sport          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_COPY,
    .chk_cbf        = _arp_pkt_ratelimit_chk,
    .rx_cbf         = _arp_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_TRUNK,
    .pName          = "DAI ARP RX Thread",
    .pCookie        = NULL,
};

static sys_enable_t g_daiEnable = DISABLED;
static sys_vlanmask_t g_daiVlanEnable;
static sys_logic_portmask_t g_trustPM;
static sys_logic_portmask_t g_smacChkPM;
static sys_logic_portmask_t g_dmacChkPM;
static sys_logic_portmask_t g_ipChkPM;
static sys_logic_portmask_t g_ipAllowZeroPM;
static sys_arp_mib_t g_portMib[SYS_LOGICPORT_NUM_MAX];
static sys_arp_mib_t g_vlanMib[SYS_VLAN_NUM_MAX];

static osal_thread_t g_arpThread;
static int32 g_portArpRateLimit[SYS_LOGICPORT_NUM_MAX];
static int32 g_portArpRateLimitCnt[SYS_LOGICPORT_NUM_MAX];

static int32 g_portArpRateLimitTmp[SYS_LOGICPORT_NUM_MAX];
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
static int8 g_portShutdownPortCnt[SYS_LOGICPORT_NUM_MAX];
#endif

static uint32 g_portArpCnt[ARP_RATE_LAST_OVER_SEC][SYS_LOGICPORT_NUM_MAX];
static uint32 g_secondTick = 0; 

#ifdef CONFIG_SYS_PROTO_ISG
static int32 _isg_entry_exist(sys_mac_t *pMac, sys_logic_port_t port, sys_vid_t vid, sys_ipv4_t ip)
{
    sys_isg_entry_t bindEntry;

    osal_memset(&bindEntry, 0, sizeof(sys_isg_entry_t));

    osal_memcpy(&bindEntry.mac, pMac, CAPA_MAC_BYTE_NUM);
    bindEntry.port = port;
    bindEntry.vid = vid;
    bindEntry.ip = ip;

    SYS_ERR_CHK(isg_bindingEntry_exist(&bindEntry, FALSE));

    return SYS_ERR_OK;
}
#endif

static void _dai_timer_thread(void *pInput)
{
    static sys_logic_port_t lp = 0;
    static struct timespec time;
    static uint32 diff = 0;
    static int32 prev_usec = 0;
    static int32 now_usec = 0;
    static uint8 tmp[SYS_LOGICPORT_NUM_MAX] = {1,};

    for (;;)
    {
        if (DISABLED == g_daiEnable)
        {
            
            osal_time_sleep(1);
        }
        else
        {
            
            osal_time_usleep(1 * 1000 * (1000 / ARP_RATE_TICK_PER_SEC));
        }

        diff = 0;
        time = current_kernel_time();

        now_usec = time.tv_nsec / 10000000;

        if (now_usec < prev_usec)
        {
            prev_usec -= 100;

            FOR_EACH_LOGIC_PORT(lp)
            {
                if (g_portArpCnt[g_secondTick % ARP_RATE_LAST_OVER_SEC][lp] != 0)
                    SYS_DBG(DBG_DAI, "idx %u %s %3u %3u", g_secondTick % ARP_RATE_LAST_OVER_SEC, LPSTR(lp+1),
                    g_portArpCnt[0][lp], g_portArpCnt[1][lp]);
            }

            g_secondTick++;
            SYS_MEM_CLEAR(g_portArpCnt[g_secondTick % ARP_RATE_LAST_OVER_SEC]);
        }

        diff = now_usec - prev_usec;

        FOR_EACH_LOGIC_PORT(lp)
        {
            if (BOND_RATE_UNLIMITED != g_portArpRateLimit[lp])
            {
                if (g_portArpRateLimitCnt[lp] < 0)
                    tmp[lp]++;
                else
                    tmp[lp] = 1;

                if (ARP_RATE_TICK_PER_SEC == tmp[lp])
                {
                    tmp[lp] = 1;
                    diff = 0;
                }

                if (g_portArpRateLimitCnt[lp] < g_portArpRateLimit[lp])
                    g_portArpRateLimitCnt[lp] += (diff * g_portArpRateLimit[lp]) / 10;
            }

            if (ARP_RATE_TICK_PER_SEC == g_portShutdownPortCnt[lp])
            {
                SYS_LOG(LOG_CAT_DAI, LOG_DAI_RATE_EXCEED, g_portArpRateLimit[lp]+1, LPSTR(lp));
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
                SYS_LOG(LOG_CAT_DAI, LOG_DAI_ERRDISABLE, LPSTR(lp));
                if (SYS_ERR_FAILED == rsd_port_errDisable_set(lp, SYS_PORT_ERRDISABLE_ARP_RATE_LIMIT))
                    SYS_DBG(DBG_DAI, "errdisabled port %s failed", LPSTR(lp));
#endif
                g_portShutdownPortCnt[lp]--;
            }
            else if (0 != g_portShutdownPortCnt[lp])
            {
                g_portShutdownPortCnt[lp]--;
            }
        }

        prev_usec = now_usec ;
    }

    osal_thread_exit(0);
}

static sys_pkt_action_t _dai_process(sys_nic_pkt_t *pPkt, sys_logic_port_t srcPort, sys_vid_t vid)
{
    uint8 *pArp;
    uint8 ipStr[CAPA_IPV4_STR_LEN];
    uint8 macStr1[CAPA_MAC_STR_LEN];
    uint8 macStr2[CAPA_MAC_STR_LEN];
    sys_dai_arpMessage_t *pArpMsg = NULL;

    pArp = (uint8 *)(pPkt->data + 14);

    if (pPkt->rx_tag.cvid_tagged)
        pArp += VLANID_OFFSET;
    if (pPkt->rx_tag.svid_tagged)
        pArp += VLANID_OFFSET;

    pArpMsg = (sys_dai_arpMessage_t *)pArp;

    if (IS_LOGIC_PORTMASK_PORTSET(g_smacChkPM, srcPort))
    {
        
        if (0 != osal_memcmp(&pArpMsg->sHaddr, (pPkt->data + CAPA_MAC_BYTE_NUM), CAPA_MAC_BYTE_NUM))
        {
            g_portMib[srcPort].arpSmacDrop++;
            g_vlanMib[vid].arpSmacDrop++;

            SYS_MEM_CLEAR(macStr1);
            SYS_MEM_CLEAR(macStr2);
            SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
            SYS_ERR_CHK(sys_util_mac2Str(pArpMsg->sHaddr.octet, macStr2));
            SYS_LOG(LOG_CAT_DAI, LOG_DAI_SMAC_DROP, vid, LPSTR(srcPort),macStr1, macStr2);

            return SYS_PKT_ACTION_DROP;
        }
    }

    if ((ARP_OPERATION_REPLY == pArpMsg->operation) && IS_LOGIC_PORTMASK_PORTSET(g_dmacChkPM, srcPort))
    {
        
        if (0 != osal_memcmp(&pArpMsg->tHaddr, pPkt->data, CAPA_MAC_BYTE_NUM))
        {
            g_portMib[srcPort].arpDmacDrop++;
            g_vlanMib[vid].arpDmacDrop++;

            SYS_MEM_CLEAR(macStr1);
            SYS_MEM_CLEAR(macStr2);
            SYS_ERR_CHK(sys_util_mac2Str(pPkt->data, macStr1));
            SYS_ERR_CHK(sys_util_mac2Str(pArpMsg->tHaddr.octet, macStr2));
            SYS_LOG(LOG_CAT_DAI, LOG_DAI_DMAC_DROP, vid, LPSTR(srcPort),macStr1, macStr2);

            return SYS_PKT_ACTION_DROP;
        }

#if 0

        if (pArpMsg->tHaddr.octet[0] & 0x1)
        {
            g_portMib[srcRepresentPort].arpDmacDrop++;
            g_vlanMib[vid].arpDmacDrop++;
            
            SYS_LOG(LOG_CAT_DAI, LOG_DAI_INVALIDDMAC_DROP);
            goto DROP;
        }

        if (osal_memcmp(&pArpMsg->tHaddr, zero_mac, CAPA_MAC_BYTE_NUM) == 0)
        {
            g_portMib[srcRepresentPort].arpDmacDrop++;
            g_vlanMib[vid].arpDmacDrop++;
            
            SYS_LOG(LOG_CAT_DAI, LOG_DAI_INVALIDDMAC_DROP);
            goto DROP;
        }
#endif
    }

    if (IS_LOGIC_PORTMASK_PORTSET(g_ipChkPM, srcPort))
    {
        
        if ((pArpMsg->sInaddr == 0xFFFFFFFF) || ((pArpMsg->sInaddr & 0xE0000000) == 0xE0000000))
        {
            g_portMib[srcPort].arpSIPDrop++;
            g_vlanMib[vid].arpSIPDrop++;

            SYS_MEM_CLEAR(ipStr);
            SYS_ERR_CHK(sys_util_ip2Str(pArpMsg->sInaddr, ipStr));
            SYS_LOG(LOG_CAT_DAI, LOG_DAI_SIP_DROP, vid, LPSTR(srcPort), ipStr);

            return SYS_PKT_ACTION_DROP;
        }

        if (!IS_LOGIC_PORTMASK_PORTSET(g_ipAllowZeroPM, srcPort) && (pArpMsg->sInaddr == 0))
        {
            g_portMib[srcPort].arpSIPDrop++;
            g_vlanMib[vid].arpSIPDrop++;

            SYS_MEM_CLEAR(ipStr);
            SYS_ERR_CHK(sys_util_ip2Str(pArpMsg->sInaddr, ipStr));
            SYS_LOG(LOG_CAT_DAI, LOG_DAI_SIP_DROP, vid, LPSTR(srcPort), ipStr);

            return SYS_PKT_ACTION_DROP;
        }

        if ((ARP_OPERATION_REPLY == pArpMsg->operation))
        {
            if ((pArpMsg->tInaddr == 0xFFFFFFFF) || ((pArpMsg->tInaddr & 0xE0000000) == 0xE0000000))
            {
                g_portMib[srcPort].arpDIPDrop++;
                g_vlanMib[vid].arpDIPDrop++;

                SYS_MEM_CLEAR(ipStr);
                SYS_ERR_CHK(sys_util_ip2Str(pArpMsg->tInaddr, ipStr));
                SYS_LOG(LOG_CAT_DAI, LOG_DAI_DIP_DROP, vid, LPSTR(srcPort), ipStr);

                return SYS_PKT_ACTION_DROP;
            }

            if (!IS_LOGIC_PORTMASK_PORTSET(g_ipAllowZeroPM, srcPort) && (pArpMsg->tInaddr == 0))
            {
                g_portMib[srcPort].arpDIPDrop++;
                g_vlanMib[vid].arpDIPDrop++;

                SYS_MEM_CLEAR(ipStr);
                SYS_ERR_CHK(sys_util_ip2Str(pArpMsg->tInaddr, ipStr));
                SYS_LOG(LOG_CAT_DAI, LOG_DAI_DIP_DROP, vid, LPSTR(srcPort), ipStr);

                return SYS_PKT_ACTION_DROP;
            }
        }
    }

#ifdef CONFIG_SYS_PROTO_ISG
    if (SYS_ERR_OK == _isg_entry_exist(&(pArpMsg->sHaddr), srcPort, vid, pArpMsg->sInaddr))
    {
        SYS_MEM_CLEAR(ipStr);
        SYS_MEM_CLEAR(macStr1);
        SYS_ERR_CHK(sys_util_ip2Str(pArpMsg->sInaddr, ipStr));
        SYS_ERR_CHK(sys_util_mac2Str(pArpMsg->sHaddr.octet, macStr1));
        SYS_DBG(DBG_DAI, "dai match isg entry of vid %u port %s mac %s sender ip %s",
            vid, LPSTR(srcPort), macStr1, ipStr);

        if ((ARP_OPERATION_REPLY== pArpMsg->operation))
        {
            sys_logic_port_t lp;
            sys_l2_entry_t l2Entry;

            SYS_MEM_CLEAR(l2Entry);
            l2Entry.vid = vid;
            osal_memcpy(&l2Entry.mac, &(pArpMsg->tHaddr), sizeof(sys_mac_t));

            if (SYS_ERR_OK == rsd_l2_mac_search(&l2Entry))
            {
                lp = LP2TRKLP(l2Entry.port);

                if (IS_LOGIC_PORTMASK_PORTSET(g_trustPM, lp))
                    return SYS_PKT_ACTION_FORWARD;
            }

            if (SYS_ERR_OK == _isg_entry_exist(&(pArpMsg->tHaddr), srcPort, vid, pArpMsg->tInaddr))
            {
                SYS_MEM_CLEAR(ipStr);
                SYS_MEM_CLEAR(macStr1);
                SYS_ERR_CHK(sys_util_ip2Str(pArpMsg->tInaddr, ipStr));
                SYS_ERR_CHK(sys_util_mac2Str(pArpMsg->tHaddr.octet, macStr1));
                SYS_DBG(DBG_DAI, "dai match isg entry of vid %u port %s mac %s target ip %s",
                    vid, LPSTR(srcPort), macStr1, ipStr);

                return SYS_PKT_ACTION_FORWARD;
            }
            else
            {
                g_portMib[srcPort].arpIPMACMismatchDrop++;
                g_vlanMib[vid].arpIPMACMismatchDrop++;

                SYS_MEM_CLEAR(ipStr);
                SYS_MEM_CLEAR(macStr1);
                SYS_ERR_CHK(sys_util_ip2Str(pArpMsg->tInaddr, ipStr));
                SYS_ERR_CHK(sys_util_mac2Str(pArpMsg->tHaddr.octet, macStr1));
                SYS_LOG(LOG_CAT_DAI, LOG_DAI_TARGET_IPMAC_BIND_DROP, vid, LPSTR(srcPort), macStr1, ipStr);
            }
            return SYS_PKT_ACTION_DROP;
        }

        return SYS_PKT_ACTION_FORWARD;
    }
    else
    {
        g_portMib[srcPort].arpIPMACMismatchDrop++;
        g_vlanMib[vid].arpIPMACMismatchDrop++;

        SYS_MEM_CLEAR(ipStr);
        SYS_MEM_CLEAR(macStr1);
        SYS_ERR_CHK(sys_util_ip2Str(pArpMsg->sInaddr, ipStr));
        SYS_ERR_CHK(sys_util_mac2Str(pArpMsg->sHaddr.octet, macStr1));
        SYS_LOG(LOG_CAT_DAI, LOG_DAI_SENDER_IPMAC_BIND_DROP, vid, LPSTR(srcPort), macStr1, ipStr);

        return SYS_PKT_ACTION_DROP;
    }
#endif
}
static l2g_act_t _arp_pkt_ratelimit_chk(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_enable_t enable = DISABLED;
    sys_vlan_membership_t membership;
    sys_logic_port_t srcPort;
    sys_vid_t vid = 0;

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);

    srcPort = LP2TRKLP(pPkt->rx_tag.source_port);
    SYS_ERR_CHK(rsd_nic_pktVid_get(pPkt, &vid));
    dai_vlan_enable_get(vid, &enable);

    if (DISABLED == enable)
        return RSD_NIC_HANDLER_ACT_COPY;

    dai_port_trust_get(srcPort, &enable);

        if (ENABLED == enable)
        return RSD_NIC_HANDLER_ACT_COPY;

    SYS_ERR_CHK(rsd_vlan_portOperVlanMembership_get(srcPort,vid, &membership));

    if ((membership != SYS_VLAN_UNTAGGED) && (membership != SYS_VLAN_TAGGED))
    {
        SYS_DBG(DBG_DAI, "free packet rx from %s because vid is %u not in VLAN membership(%u)",
            LPSTR (srcPort), vid, membership);

        return RSD_NIC_HANDLER_ACT_DROP;
    }

    g_portArpCnt[g_secondTick % ARP_RATE_LAST_OVER_SEC][srcPort]++;

    if (BOND_RATE_UNLIMITED != g_portArpRateLimit[srcPort])
    {
        if (g_portArpRateLimitCnt[srcPort] < 0 - g_portArpRateLimitTmp[srcPort])
        {
            if ((g_portArpCnt[0][srcPort] >= g_portArpRateLimit[srcPort])
                && (g_portArpCnt[1][srcPort] >= g_portArpRateLimit[srcPort]))
            {

                if (0 == g_portShutdownPortCnt[srcPort])
                    g_portShutdownPortCnt[srcPort] = ARP_RATE_TICK_PER_SEC;
            }

            return RSD_NIC_HANDLER_ACT_DROP;
        }

        g_portArpRateLimitCnt[srcPort] -= ARP_RATE_TICK_PER_SEC;
    }

    return RSD_NIC_HANDLER_ACT_COPY;
}

static int32 _arp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_vid_t vid = 0;
    sys_logic_port_t srcPort = 0;
    sys_pkt_action_t pkt_action = SYS_PKT_ACTION_FORWARD;
    sys_enable_t vlan_enable = DISABLED;
    sys_enable_t trust = DISABLED;
    sys_dai_arpMessage_t *pArpMsg = NULL;
    uint8 *pArp = NULL;
    uint8 ipStr[CAPA_IPV4_STR_LEN];
    uint8 macStr[CAPA_MAC_STR_LEN];

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);

    pArp = (uint8 *)(pPkt->data + 14);

    if (pPkt->rx_tag.cvid_tagged)
        pArp += VLANID_OFFSET;
    if (pPkt->rx_tag.svid_tagged)
        pArp += VLANID_OFFSET;

    pArpMsg = (sys_dai_arpMessage_t *)pArp;
    srcPort = pPkt->rx_tag.source_port;
    rsd_nic_pktVid_get(pPkt, &vid);
    dai_vlan_enable_get(vid, &vlan_enable);
    dai_port_trust_get(srcPort, &trust);

    SYS_DBG(DBG_DAI, "dai vid %u is %u %s is %u", vid, vlan_enable, LPSTR(srcPort), trust);

#define _GET_MAC_IP_STR(_mac, _ip)  \
do {                                \
    SYS_MEM_CLEAR(ipStr);           \
    SYS_MEM_CLEAR(macStr);          \
    SYS_ERR_CHK(sys_util_ip2Str(_ip, ipStr));       \
    SYS_ERR_CHK(sys_util_mac2Str(_mac, macStr));    \
}while(0);

    if (DISABLED == g_daiEnable)
    {
        _GET_MAC_IP_STR(pPkt->data + CAPA_MAC_BYTE_NUM, pArpMsg->sInaddr);
        SYS_LOG(LOG_CAT_DAI, LOG_DAI_GLOBAL_DROP, vid, LPSTR(srcPort), macStr, ipStr);
        SYS_ERR_CHK(rsd_nic_pkt_free(pPkt));

        return SYS_ERR_OK;
    }

    if (FALSE == rsd_nic_isPortSTPForwad_ret(srcPort, vid))
    {
        _GET_MAC_IP_STR(pPkt->data + CAPA_MAC_BYTE_NUM, pArpMsg->sInaddr);
        SYS_LOG(LOG_CAT_DAI, LOG_DAI_STP_DROP, vid, LPSTR(srcPort), macStr, ipStr);
        SYS_ERR_CHK(rsd_nic_pkt_free(pPkt));

        return SYS_ERR_OK;
    }

    if (TRUE == rsd_nic_isPktMyMac_ret(pPkt))
    {
        SYS_ERR_CHK(rsd_nic_pkt_free(pPkt));

        return SYS_ERR_OK;
    }

    if (DISABLED == vlan_enable)
    {
        _GET_MAC_IP_STR(pPkt->data + CAPA_MAC_BYTE_NUM, pArpMsg->sInaddr);
        SYS_LOG(LOG_CAT_DAI, LOG_DAI_VLAN_DROP, vid, LPSTR(srcPort), macStr, ipStr);
        SYS_ERR_CHK(rsd_nic_pktByDmacBlkPort_tx(pPkt, SYS_CPU_PORT_PRIORITY_ARP, vid, srcPort));

        return SYS_ERR_OK;
    }

    if (DISABLED == trust)
        pkt_action = _dai_process(pPkt, srcPort, vid);

    if (SYS_PKT_ACTION_FORWARD == pkt_action)
    {
        g_portMib[srcPort].arpForward++;
        g_vlanMib[vid].arpForward++;
        SYS_ERR_CHK(rsd_nic_pktByDmacBlkPort_tx(pPkt, SYS_CPU_PORT_PRIORITY_ARP, vid, srcPort));

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(rsd_nic_pkt_free(pPkt));

    return SYS_ERR_OK;
}
int32 dai_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_DBG(DBG_DAI, "dai set to %u", enable);

    if (enable == g_daiEnable)
        return SYS_ERR_OK;

    if (ENABLED == enable)
    {
        
        g_daiEnable = ENABLED;
        SYS_ERR_CHK(rsd_nic_handler_register(&l2gARPHandler));
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_ARP_DAI, ENABLED));
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_ARP_BCAST, DISABLED));
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_ARP_MYMAC, DISABLED));
    }
    else
    {
        
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_ARP_DAI, DISABLED));
        SYS_ERR_CHK(rsd_nic_handler_unregister(&l2gARPHandler));
        g_daiEnable = DISABLED;

        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_ARP_BCAST, ENABLED));
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_ARP_MYMAC, ENABLED));
    }

    return SYS_ERR_OK;
}
int32 dai_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    *pEnable = g_daiEnable;

    return SYS_ERR_OK;
}
int32 dai_vlan_enable_set (sys_vid_t vid, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_DBG(DBG_DAI, "dai vlan %u set to %u", vid, enable);

    if (ENABLED == enable)
        VLANMASK_SET_VID(g_daiVlanEnable, vid);
    else
        VLANMASK_CLEAR_VID(g_daiVlanEnable, vid);

    return SYS_ERR_OK;
}
int32 dai_vlan_enable_get (sys_vid_t vid, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    if (VLANMASK_IS_VIDSET(g_daiVlanEnable, vid))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}
int32 dai_port_trust_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_DBG(DBG_DAI, "dai %s type set to %u", LPSTR(port), enable);

    if (ENABLED == enable)
        LOGIC_PORTMASK_SET_PORT(g_trustPM, port);
    else
        LOGIC_PORTMASK_CLEAR_PORT(g_trustPM, port);

    return SYS_ERR_OK;
}
int32 dai_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    if (IS_LOGIC_PORTMASK_PORTSET(g_trustPM, port))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}
int32 dai_port_smacChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_DBG(DBG_DAI, "dai %s check smac set to %u", LPSTR(port), enable);

    if (ENABLED == enable)
        LOGIC_PORTMASK_SET_PORT(g_smacChkPM, port);
    else
        LOGIC_PORTMASK_CLEAR_PORT(g_smacChkPM, port);

    return SYS_ERR_OK;
}
int32 dai_port_smacChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    if (IS_LOGIC_PORTMASK_PORTSET(g_smacChkPM, port))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}
int32 dai_port_dmacChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_DBG(DBG_DAI, "dai %s check dmac set to %u", LPSTR(port), enable);

    if (ENABLED == enable)
        LOGIC_PORTMASK_SET_PORT(g_dmacChkPM, port);
    else
        LOGIC_PORTMASK_CLEAR_PORT(g_dmacChkPM, port);

    return SYS_ERR_OK;
}
int32 dai_port_dmacChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    if (IS_LOGIC_PORTMASK_PORTSET(g_dmacChkPM, port))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}
int32 dai_port_ipChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_DBG(DBG_DAI, "dai %s check ip set to %u", LPSTR(port), enable);

    if (ENABLED == enable)
        LOGIC_PORTMASK_SET_PORT(g_ipChkPM, port);
    else
        LOGIC_PORTMASK_CLEAR_PORT(g_ipChkPM, port);

    return SYS_ERR_OK;
}
int32 dai_port_ipChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    if (IS_LOGIC_PORTMASK_PORTSET(g_ipChkPM, port))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}
int32 dai_port_ipZeroAllow_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_DBG(DBG_DAI, "dai %s allow zero ip set to %u", LPSTR(port), enable);

    if (ENABLED == enable)
        LOGIC_PORTMASK_SET_PORT(g_ipAllowZeroPM, port);
    else
        LOGIC_PORTMASK_CLEAR_PORT(g_ipAllowZeroPM, port);

    return SYS_ERR_OK;
}
int32 dai_port_ipZeroAllow_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    if (IS_LOGIC_PORTMASK_PORTSET(g_ipAllowZeroPM, port))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}
int32 dai_portMib_get(sys_logic_port_t port, sys_arp_mib_t *pCounter)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCounter), SYS_ERR_NULL_POINTER);

    osal_memcpy(pCounter, &g_portMib[port], sizeof(sys_arp_mib_t));

    return SYS_ERR_OK;
}
int32 dai_portMib_clear(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_MEM_CLEAR(g_portMib[port]);

    return SYS_ERR_OK;
}
int32 dai_vlanMib_get(sys_vid_t vid, sys_arp_mib_t *pCounter)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pCounter), SYS_ERR_NULL_POINTER);

    osal_memcpy(pCounter, &g_vlanMib[vid], sizeof(sys_arp_mib_t));

    return SYS_ERR_OK;
}
int32 dai_vlanMib_clear(sys_vid_t vid)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    SYS_MEM_CLEAR(g_vlanMib[vid]);

    return SYS_ERR_OK;
}
int32 dai_arp_rateLimit_set(sys_logic_port_t port, uint32 rate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    SYS_DBG(DBG_DAI, "dai %s rate limit set to %u pps", LPSTR(port), rate);

    g_portArpRateLimit[port] = rate;
    g_portArpRateLimitCnt[port] = rate;

    if (BOND_RATE_UNLIMITED == rate)
    {
        g_portArpRateLimitTmp[port] = rate;
    }
    else
    {
        if (rate > 100)
            g_portArpRateLimitTmp[port] = (rate * 2)/ARP_RATE_TICK_PER_SEC;
        else if (rate > 50)
            g_portArpRateLimitTmp[port] = (rate * 5)/ARP_RATE_TICK_PER_SEC;
        else
            g_portArpRateLimitTmp[port] = rate;
    }

    return SYS_ERR_OK;
}
int32 dai_init(void)
{
    sys_logic_port_t port;

    VLANMASK_CLEAR_ALL(g_daiVlanEnable);

    LOGIC_PORTMASK_CLEAR_ALL(g_trustPM);
    LOGIC_PORTMASK_CLEAR_ALL(g_smacChkPM);
    LOGIC_PORTMASK_CLEAR_ALL(g_dmacChkPM);
    LOGIC_PORTMASK_CLEAR_ALL(g_ipChkPM);
    LOGIC_PORTMASK_CLEAR_ALL(g_ipAllowZeroPM);
    SYS_MEM_CLEAR(g_portArpCnt);

    SYS_MEM_CLEAR(g_portMib);
    SYS_MEM_CLEAR(g_vlanMib);

    FOR_EACH_LOGIC_PORT(port)
    {
        g_portArpRateLimit[port] = BOND_RATE_UNLIMITED;
        g_portArpRateLimitCnt[port] = 0;
        g_portArpRateLimitTmp[port] = 0;
    }

    g_arpThread = osal_thread_create("ARP RL Thread", 32768, DFLT_THREAD_PRI_ARP_RL_TIMER, (void *)_dai_timer_thread, NULL);

    if ((osal_thread_t)NULL == g_arpThread)
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}
int32 dai_exit(void)
{
    return SYS_ERR_OK;
}

