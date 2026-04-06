#include <linux/net/net_svi.h>
#include <authmgr/proto_authmgr.h>
#include <common/sys_pktfmt.h>
#include <rsd/rsd_l2.h>
#include <ksi_authmgr.h>
#define WEBAUTH_TCP_SESSION_TIMEOUT      (2) 
#define WEBAUTH_TCP_AUTING_IDLE_TIME_MAX (600) 

#define WEBAUTH_TCPSESS_NUM_MAX          (256)
extern sys_mac_t g_sysMac;

static sys_ipv4_t g_sysIpv4 = 0;
static sys_ipv4_t g_natIpv4 = 0;
static uint32     g_isArpAdd = FALSE;
static sys_ipv6_t g_sysIpv6;
static sys_ipv6_t g_natIpv6;
static osal_mutex_t webauth_sem;

static webauth_tcpSess_t *g_tcpSessList = NULL;

ROUTINE_INIT(webauthTimer, SYS_TIMER_SEC);
extern int32 webauth_sm_check(webauth_t *pWebauth);
static svi_tx_t _webauth_pkt_tx(sys_nic_pkt_t *pPkt, void *pCookie);

static svi_handler_t webauthTxHandler =
{
    .priority       = RSD_NIC_HANDLER_PRI_AUTHMGR,
    .tx_cbf         = _webauth_pkt_tx,
};
static void _webauth_natArp_add(void)
{
    webauth_arp_t req;

    SYS_MEM_CLEAR(req);

    req.ip = g_natIpv4;
    osal_memcpy(&req.mac, &g_sysMac, ETHER_ADDR_LEN);

    g_isArpAdd = TRUE;

    KSI_NOTIFY_MSG_DATA(KSI_AUTHMGR_WEBAUTH_ARP_ADD, sizeof(webauth_arp_t), &req);
}

static void _webauth_natArp_del(void)
{
    webauth_arp_t req;

    SYS_MEM_CLEAR(req);

    req.ip = g_natIpv4;

    g_isArpAdd = FALSE;

    KSI_NOTIFY_MSG_DATA(KSI_AUTHMGR_WEBAUTH_ARP_DEL, sizeof(webauth_arp_t), &req);
}

static void _webauth_natIpv4_set(void)
{
        sys_ipv4_t mask = 0;
        sys_ipv4_t maskSysIp = 0;
    sys_l3_ipv4_t l3IPv4;

    SYS_MEM_CLEAR(l3IPv4);
    rsd_sys_mgmtIPv4Addr_get(&l3IPv4);

    g_sysIpv4 = l3IPv4.addr;
    mask = IPV4_PFX2MASK(l3IPv4.plen);

    maskSysIp = (g_sysIpv4 & ~mask);
    
    if (maskSysIp == (~mask - 1) || maskSysIp == (~mask - 2))
    {
        g_natIpv4 = g_sysIpv4 - 1;

                    }
    else
    {
        g_natIpv4 = g_sysIpv4 + 1;

                    }

    WEBAUTH_DBG2("Select NAT IP Address %d.%d.%d.%d",
                 (g_natIpv4>>24)&0xFF,
                 (g_natIpv4>>16)&0xFF,
                 (g_natIpv4>>8)&0xFF,
                 (g_natIpv4)&0xFF);
}

static void _webauth_ipv6_init(void)
{
    SYS_MEM_CLEAR(g_sysIpv6);
    SYS_MEM_CLEAR(g_natIpv6);

    g_sysIpv6.hex[15] = g_sysMac.octet[5];
    g_sysIpv6.hex[14] = g_sysMac.octet[4];
    g_sysIpv6.hex[13] = g_sysMac.octet[3];
    g_sysIpv6.hex[12] = 0xfe;
    g_sysIpv6.hex[11] = 0xff;
    g_sysIpv6.hex[10] = g_sysMac.octet[2];
    g_sysIpv6.hex[9] = g_sysMac.octet[1];
    g_sysIpv6.hex[8] = g_sysMac.octet[0];

    if (g_sysIpv6.hex[8] & (1 << 1))
        g_sysIpv6.hex[8] -=0x2;
    else
        g_sysIpv6.hex[8] +=0x2;

    g_sysIpv6.hex[1] = 0x80;
    g_sysIpv6.hex[0] = 0xfe;

    osal_memcpy(&g_natIpv6, &g_sysIpv6, sizeof(sys_ipv6_t));

    if ((g_natIpv6.hex[15] + 1) > 0xff)
    {
        if (0xff == g_natIpv6.hex[14])
            g_natIpv6.hex[13] +=1;

        g_natIpv6.hex[14]+=1;
    }

    g_natIpv6.hex[15] += 1;
}

static int32 _webauth_mgmtIPChgEvent(uint32 event, char *pData)
{
    sys_system_event_t *pSysEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pSysEvent = (sys_system_event_t *)pData;

    _webauth_natIpv4_set();

    return SYS_ERR_OK;
}

static webauth_tcpSess_t *_webauth_tcpSess_add(webauth_tcpSessInfo_t *pSessInfo)
{
    webauth_tcpSess_t *pSessCurr = NULL;
    webauth_tcpSess_t *pSessLast = NULL;
    webauth_tcpSess_t *pSessNew = NULL;
    uint32            sessNum = 0;

    WEBAUTH_SEM_LOCK(webauth_sem);

    if (NULL == g_tcpSessList)
    {
        g_tcpSessList = (webauth_tcpSess_t *) osal_alloc(sizeof(webauth_tcpSess_t));

        if (NULL == g_tcpSessList)
        {
            WEBAUTH_SEM_UNLOCK(webauth_sem);
            return NULL;
        }
        else
        {
            osal_memset(g_tcpSessList, 0, sizeof(webauth_tcpSess_t));

            osal_memcpy(&g_tcpSessList->sessInfo, pSessInfo, sizeof(webauth_tcpSessInfo_t));

            g_tcpSessList->timer = WEBAUTH_TCP_SESSION_TIMEOUT;
            g_tcpSessList->next = NULL;

            #if 0
            WEBAUTH_DBG2("Add first trap session %02x:%02x:%02x:%02x:%02x:%02x SA:%d.%d.%d.%d DA:%d.%d.%d.%d TCP:%d on port %s vlan %d",
                         g_tcpSessList->sessInfo.suppInfo.mac.octet[0],
                         g_tcpSessList->sessInfo.suppInfo.mac.octet[1],
                         g_tcpSessList->sessInfo.suppInfo.mac.octet[2],
                         g_tcpSessList->sessInfo.suppInfo.mac.octet[3],
                         g_tcpSessList->sessInfo.suppInfo.mac.octet[4],
                         g_tcpSessList->sessInfo.suppInfo.mac.octet[5],
                         g_tcpSessList->sessInfo.srcIP >> 24 & 0xFF,
                         g_tcpSessList->sessInfo.srcIP >> 16 & 0xFF,
                         g_tcpSessList->sessInfo.srcIP >> 8 & 0xFF,
                         g_tcpSessList->sessInfo.srcIP >> 0 & 0xFF,
                         g_tcpSessList->sessInfo.dstIP >> 24 & 0xFF,
                         g_tcpSessList->sessInfo.dstIP >> 16 & 0xFF,
                         g_tcpSessList->sessInfo.dstIP >> 8 & 0xFF,
                         g_tcpSessList->sessInfo.dstIP >> 0 & 0xFF,
                         g_tcpSessList->sessInfo.tcpPort,
                         LPSTR(g_tcpSessList->sessInfo.suppInfo.port),
                         g_tcpSessList->sessInfo.suppInfo.vid);
            #endif
        }

        WEBAUTH_SEM_UNLOCK(webauth_sem);

        _webauth_natArp_add();

        return g_tcpSessList;
    }

    pSessCurr = g_tcpSessList;

    while (pSessCurr != NULL)
    {
        
        if (0 == osal_memcmp(pSessInfo, &pSessCurr->sessInfo, sizeof(webauth_tcpSessInfo_t)))
        {
            
            if (pSessCurr->timer < WEBAUTH_TCP_SESSION_TIMEOUT)
                pSessCurr->timer = WEBAUTH_TCP_SESSION_TIMEOUT;

            WEBAUTH_SEM_UNLOCK(webauth_sem);

            return pSessCurr;
        }

        if (pSessInfo->tcpPort == pSessCurr->sessInfo.tcpPort)
        {
            WEBAUTH_SEM_UNLOCK(webauth_sem);

            return NULL;
        }

        sessNum++;

        pSessLast = pSessCurr;
        pSessCurr = pSessCurr->next;
    }

    if (((sessNum + 1) > WEBAUTH_TCPSESS_NUM_MAX) || (NULL == pSessLast))
    {
        WEBAUTH_SEM_UNLOCK(webauth_sem);

        return NULL;
    }

    pSessNew = (webauth_tcpSess_t *) osal_alloc(sizeof(webauth_tcpSess_t));

    if (NULL != pSessNew)
    {
        osal_memset(pSessNew, 0, sizeof(webauth_tcpSess_t));
        osal_memcpy(&pSessNew->sessInfo, pSessInfo, sizeof(webauth_tcpSessInfo_t));

        pSessNew->timer = WEBAUTH_TCP_SESSION_TIMEOUT;
        pSessNew->next = NULL;

        #if 0
        WEBAUTH_DBG2("Add trap session %02x:%02x:%02x:%02x:%02x:%02x SA:%d.%d.%d.%d DA:%d.%d.%d.%d TCP:%d on port %s vlan %d",
                     pSessNew->sessInfo.suppInfo.mac.octet[0],
                     pSessNew->sessInfo.suppInfo.mac.octet[1],
                     pSessNew->sessInfo.suppInfo.mac.octet[2],
                     pSessNew->sessInfo.suppInfo.mac.octet[3],
                     pSessNew->sessInfo.suppInfo.mac.octet[4],
                     pSessNew->sessInfo.suppInfo.mac.octet[5],
                     pSessNew->sessInfo.srcIP >> 24 & 0xFF,
                     pSessNew->sessInfo.srcIP >> 16 & 0xFF,
                     pSessNew->sessInfo.srcIP >> 8 & 0xFF,
                     pSessNew->sessInfo.srcIP >> 0 & 0xFF,
                     pSessNew->sessInfo.dstIP >> 24 & 0xFF,
                     pSessNew->sessInfo.dstIP >> 16 & 0xFF,
                     pSessNew->sessInfo.dstIP >> 8 & 0xFF,
                     pSessNew->sessInfo.dstIP >> 0 & 0xFF,
                     pSessNew->sessInfo.tcpPort,
                     LPSTR(pSessNew->sessInfo.suppInfo.port),
                     pSessNew->sessInfo.suppInfo.vid);
        #endif

        pSessLast->next = pSessNew;
    }

    WEBAUTH_SEM_UNLOCK(webauth_sem);

    return pSessNew;
}

static int32 _webauth_tcpSess_update(uint32 delAll)
{
    webauth_tcpSess_t *pSess = NULL;
    webauth_tcpSess_t *pSessPrev = NULL;
    uint32            sessNum = 0;

    WEBAUTH_SEM_LOCK(webauth_sem);

    pSess = g_tcpSessList;

    while (pSess != NULL)
    {
        DEC(pSess->timer);
        
        if (pSess->timer == 0 || delAll)
        {
            #if 0
            WEBAUTH_DBG2("Delete TCP session %02x:%02x:%02x:%02x:%02x:%02x SA:%d.%d.%d.%d DA:%d.%d.%d.%d TCP:%d on port %s vlan %d",
                         pSess->sessInfo.suppInfo.mac.octet[0],
                         pSess->sessInfo.suppInfo.mac.octet[1],
                         pSess->sessInfo.suppInfo.mac.octet[2],
                         pSess->sessInfo.suppInfo.mac.octet[3],
                         pSess->sessInfo.suppInfo.mac.octet[4],
                         pSess->sessInfo.suppInfo.mac.octet[5],
                         pSess->sessInfo.srcIP >> 24 & 0xFF,
                         pSess->sessInfo.srcIP >> 16 & 0xFF,
                         pSess->sessInfo.srcIP >> 8 & 0xFF,
                         pSess->sessInfo.srcIP >> 0 & 0xFF,
                         pSess->sessInfo.dstIP >> 24 & 0xFF,
                         pSess->sessInfo.dstIP >> 16 & 0xFF,
                         pSess->sessInfo.dstIP >> 8 & 0xFF,
                         pSess->sessInfo.dstIP >> 0 & 0xFF,
                         pSess->sessInfo.tcpPort,
                         LPSTR(pSess->sessInfo.suppInfo.port),
                         pSess->sessInfo.suppInfo.vid);
            #endif

            if (NULL == pSessPrev)
            {
                g_tcpSessList = pSess->next;
                osal_free(pSess);
                pSess = g_tcpSessList;
            }
            else
            {
                pSessPrev->next = pSess->next;
                osal_free(pSess);
                pSess = pSessPrev->next;
            }

            continue;
        }

        sessNum++;

        pSessPrev = pSess;
        pSess = pSess->next;
    }

    if (sessNum == 0 && g_isArpAdd)
        _webauth_natArp_del();
        
    WEBAUTH_SEM_UNLOCK(webauth_sem);

    return SYS_ERR_OK;
}

static webauth_tcpSessInfo_t *_webauth_tcpSessInfoByTCP_ret(uint16 tcpPort)
{
    webauth_tcpSess_t *pSess = NULL;

    WEBAUTH_SEM_LOCK(webauth_sem);

    pSess = g_tcpSessList;

    while (pSess != NULL)
    {
        if ((pSess->sessInfo.tcpPort == tcpPort))
        {
            WEBAUTH_SEM_UNLOCK(webauth_sem);

            return &pSess->sessInfo;
        }

        pSess = pSess->next;
    }

    WEBAUTH_SEM_UNLOCK(webauth_sem);

    return NULL;
}

static webauth_tcpSess_t *_webauth_tcpSessByTCP_ret(uint16 tcpPort)
{
    webauth_tcpSess_t *pSess = NULL;

    WEBAUTH_SEM_LOCK(webauth_sem);

    pSess = g_tcpSessList;

    while (pSess != NULL)
    {
        if ((pSess->sessInfo.tcpPort == tcpPort))
        {
            WEBAUTH_SEM_UNLOCK(webauth_sem);

            return pSess;
        }

        pSess = pSess->next;
    }

    WEBAUTH_SEM_UNLOCK(webauth_sem);

    return NULL;
}

static authmgr_sess_t *_webauth_sessByTCP_ret(uint16 tcpPort)
{
    webauth_tcpSess_t *pSess = NULL;
    authmgr_sess_t *pAuthSess = NULL;

    WEBAUTH_SEM_LOCK(webauth_sem);

    pSess = g_tcpSessList;

    while (pSess != NULL)
    {
        
        if ((pSess->sessInfo.tcpPort == tcpPort))
        {
            pAuthSess = authmgr_sess_getBySuppInfo_ret(&pSess->sessInfo.suppInfo);

            WEBAUTH_SEM_UNLOCK(webauth_sem);

            return pAuthSess;
        }

        pSess = pSess->next;
    }

    WEBAUTH_SEM_UNLOCK(webauth_sem);

    return NULL;
}

static uint32 _webauth_rxNeedProcess_ret(authmgr_suppInfo_t *pSuppInfo)
{
    authmgr_sess_t *pSess = NULL;

    if (NULL != (pSess = authmgr_sess_getBySuppInfo_ret(pSuppInfo)))
    {
        switch (pSess->status.authStatus)
        {
            case AUTHMGR_STATUS_AUTHED:
            case AUTHMGR_STATUS_GUEST:
                return FALSE;

            default:
                return TRUE;
        }
    }

    return FALSE;
}

l2g_act_t webauth_pkt_chk(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_logic_port_t   srcPort;
    sys_mac_t          *pSrcMac = NULL;
    ethhdr_t           *ethhdr = NULL;
    iphdr_t            *iphdr = NULL;
    ipv6hdr_t          *ipv6hdr = NULL;
    tcphdr_t           *tcphdr = NULL;
    sys_vid_t          vid = 0;
    sys_vid_t          currVid = 0;
    authmgr_suppInfo_t suppInfo;

    if (pPkt == NULL)
        return RSD_NIC_HANDLER_ACT_NONE;

    if (DISABLED == g_authCfg.typeEnbl[AUTHMGR_TYPE_WEB])
        return RSD_NIC_HANDLER_ACT_NONE;

    srcPort = pPkt->rx_tag.source_port;

    if (DISABLED == g_authCfg.portCfg[srcPort].typeEnbl[AUTHMGR_TYPE_WEB])
        return RSD_NIC_HANDLER_ACT_NONE;

    ethhdr = (ethhdr_t*)(pPkt->data);

    if (ethhdr->h_proto == 0x806) 
    {
        return RSD_NIC_HANDLER_ACT_COPY;
    }
    else if (ethhdr->h_proto != 0x800 && ethhdr->h_proto != 0x86DD) 
    {
        return RSD_NIC_HANDLER_ACT_NONE;
    }

    pSrcMac = (sys_mac_t *)(pPkt->data + sizeof(sys_mac_t));
    rsd_nic_pktVid_get(pPkt, &vid);

    if (AUTHMGR_MODE_MULTI_HOST == g_authCfg.portCfg[srcPort].mode)
    {
        
        if (IS_PORT_GUEST_VLAN_ENABLE(srcPort) && vid == g_authCfg.guestVlan)
        {
            rsd_vlan_portAdminPvid_get(srcPort, &currVid);

                    }
    }

    SYS_MEM_CLEAR(suppInfo);
    suppInfo.vid = vid;
    suppInfo.port = srcPort;
    osal_memcpy(&suppInfo.mac, pSrcMac, sizeof(sys_mac_t));

    if (ethhdr->h_proto == 0x800)
    {
        iphdr = (iphdr_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));
        tcphdr = (tcphdr_t*)(((uint8*) iphdr) + HDR_DATALEN_TRANS(iphdr->ihl));

        if (iphdr->protocol == 17 && ((tcphdr->dest == 53) || tcphdr->dest == 67))
        {
            return RSD_NIC_HANDLER_ACT_TRAP;
        }
        
        else if (iphdr->protocol == 6 && ((tcphdr->dest == 80) || tcphdr->dest == 443))
        {
            if (_webauth_rxNeedProcess_ret(&suppInfo))
            {
                webauth_tcpSess_t *pTcpSess = NULL;
                webauth_tcpSessInfo_t sessInfo;

                SYS_MEM_CLEAR(sessInfo);
                osal_memcpy(&sessInfo.suppInfo, &suppInfo, sizeof(authmgr_suppInfo_t));
                sessInfo.ipVer = iphdr->version;
                sessInfo.srcIP = iphdr->saddr;
                sessInfo.dstIP = iphdr->daddr;
                sessInfo.tcpPort = tcphdr->source;

                if (NULL == (pTcpSess = _webauth_tcpSess_add(&sessInfo)))
                    return RSD_NIC_HANDLER_ACT_DROP;

                osal_memcpy(ethhdr->h_dest, g_sysMac.octet, ETHER_ADDR_LEN);

                iphdr->saddr = g_natIpv4;
                iphdr->daddr = g_sysIpv4;
                iphdr->check = 0;
                iphdr->check = ipChecksum(iphdr);
                if (443 == tcphdr->dest)
                    tcphdr->dest = WEBAUTH_NAT_TCPPORT_HTTPS;
                else if (80 == tcphdr->dest)
                    tcphdr->dest = WEBAUTH_NAT_TCPPORT_HTTP;
                tcphdr->check = 0;
                tcphdr->check = tcpChecksum(iphdr);

                return RSD_NIC_HANDLER_ACT_NONE;
            }
        }
        
        else if (iphdr->protocol == 1)
        {
            authmgr_sess_t *pSess = NULL;
            if (NULL != (pSess = authmgr_sess_getBySuppInfo_ret(&suppInfo)))
            {
                if (AUTHMGR_STATUS_AUTHED != pSess->status.authStatus)
                    return RSD_NIC_HANDLER_ACT_DROP;
            }
        }
    }
    
    else if (ethhdr->h_proto == 0x86DD)
    {
        ipv6hdr = (ipv6hdr_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));

        if (ipv6hdr->nexthdr == 58)
        {
            icmp6hdr_t *icmpv6hdr = NULL;

            icmpv6hdr = (icmp6hdr_t*) ((uint8*)ipv6hdr + sizeof(ipv6hdr_t));

            if (icmpv6hdr->icmp6_type == 128 || icmpv6hdr->icmp6_type == 129)
            {
                authmgr_sess_t *pSess = NULL;
                if (NULL != (pSess = authmgr_sess_getBySuppInfo_ret(&suppInfo)))
                {
                    if (AUTHMGR_STATUS_AUTHED != pSess->status.authStatus)
                        return RSD_NIC_HANDLER_ACT_DROP;
                }
            }

            return RSD_NIC_HANDLER_ACT_COPY;
        }

        tcphdr = (tcphdr_t*)(((uint8*) ipv6hdr) + sizeof(ipv6hdr_t));

        if (ipv6hdr->nexthdr == 17 && ((tcphdr->dest == 53) || tcphdr->dest == 547))
        {
            return RSD_NIC_HANDLER_ACT_TRAP;
        }
        
        else if (ipv6hdr->nexthdr == 6 && ((tcphdr->dest == 80) || tcphdr->dest == 443))
        {
            if (_webauth_rxNeedProcess_ret(&suppInfo))
            {
                webauth_tcpSess_t *pTcpSess = NULL;
                webauth_tcpSessInfo_t sessInfo;

                SYS_MEM_CLEAR(sessInfo);
                osal_memcpy(&sessInfo.suppInfo, &suppInfo, sizeof(authmgr_suppInfo_t));
                sessInfo.ipVer = ipv6hdr->version;
                osal_memcpy(&sessInfo.srcIPv6, ipv6hdr->saddr, sizeof(sys_ipv6_t));
                osal_memcpy(&sessInfo.dstIPv6, ipv6hdr->daddr, sizeof(sys_ipv6_t));
                sessInfo.tcpPort = tcphdr->source;

                if (NULL == (pTcpSess = _webauth_tcpSess_add(&sessInfo)))
                    return RSD_NIC_HANDLER_ACT_DROP;

                osal_memcpy(ethhdr->h_dest, g_sysMac.octet, ETHER_ADDR_LEN);

                osal_memcpy(ipv6hdr->saddr, &g_natIpv6, sizeof(sys_ipv6_t));
                osal_memcpy(ipv6hdr->daddr, &g_sysIpv6, sizeof(sys_ipv6_t));
                if (443 == tcphdr->dest)
                    tcphdr->dest = WEBAUTH_NAT_TCPPORT_HTTPS;
                else if (80 == tcphdr->dest)
                    tcphdr->dest = WEBAUTH_NAT_TCPPORT_HTTP;
                tcphdr->check = 0;
                tcphdr->check = tcpv6Checksum(ipv6hdr);

                return RSD_NIC_HANDLER_ACT_NONE;
            }
        }
    }

    return RSD_NIC_HANDLER_ACT_NONE;
}

static svi_tx_t _webauth_pkt_tx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    ethhdr_t *ethhdr = NULL;
    iphdr_t  *iphdr = NULL;
    ipv6hdr_t *ipv6hdr = NULL;
    tcphdr_t *tcphdr = NULL;
    sys_vid_t vid = 0;
    webauth_tcpSessInfo_t *pSessInfo = NULL;
    sys_logic_portmask_t toPorts;

    if (DISABLED == g_authCfg.typeEnbl[AUTHMGR_TYPE_WEB])
        return SVI_TX_HANDLED;

    ethhdr = (ethhdr_t*)(pPkt->data);

    if (ethhdr->h_proto != 0x800 && ethhdr->h_proto != 0x86DD)
        return SVI_TX_HANDLED;

    if (ethhdr->h_proto == 0x800)
    {
        iphdr = (iphdr_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));

        if (iphdr->protocol != 6)
            return SVI_TX_HANDLED;

        tcphdr = (tcphdr_t*)(((uint8*) iphdr) + HDR_DATALEN_TRANS(iphdr->ihl));

        if (tcphdr->source != WEBAUTH_NAT_TCPPORT_HTTPS && tcphdr->source != WEBAUTH_NAT_TCPPORT_HTTP)
            return SVI_TX_HANDLED;

        if (iphdr->saddr != g_sysIpv4 || iphdr->daddr != g_natIpv4)
            return SVI_TX_HANDLED;

        if (NULL == (pSessInfo = _webauth_tcpSessInfoByTCP_ret(tcphdr->dest)))
            return SVI_TX_HANDLED;

        #if 0
        SYS_PRINTF("Find session %02x:%02x:%02x:%02x:%02x:%02x SA:%d.%d.%d.%d DA:%d.%d.%d.%d TCP:%d on port %s vlan %d\n",
                   pSessInfo->suppInfo.mac.octet[0],
                   pSessInfo->suppInfo.mac.octet[1],
                   pSessInfo->suppInfo.mac.octet[2],
                   pSessInfo->suppInfo.mac.octet[3],
                   pSessInfo->suppInfo.mac.octet[4],
                   pSessInfo->suppInfo.mac.octet[5],
                   pSessInfo->srcIP >> 24 & 0xFF,
                   pSessInfo->srcIP >> 16 & 0xFF,
                   pSessInfo->srcIP >> 8 & 0xFF,
                   pSessInfo->srcIP >> 0 & 0xFF,
                   pSessInfo->dstIP >> 24 & 0xFF,
                   pSessInfo->dstIP >> 16 & 0xFF,
                   pSessInfo->dstIP >> 8 & 0xFF,
                   pSessInfo->dstIP >> 0 & 0xFF,
                   pSessInfo->tcpPort,
                   LPSTR(pSessInfo->suppInfo.port),
                   pSessInfo->suppInfo.vid);
        #endif

        osal_memcpy(ethhdr->h_dest, pSessInfo->suppInfo.mac.octet, ETHER_ADDR_LEN);
        iphdr->saddr = pSessInfo->dstIP;
        iphdr->daddr = pSessInfo->srcIP;
        iphdr->check = 0;
        iphdr->check = ipChecksum(iphdr);
        if (WEBAUTH_NAT_TCPPORT_HTTPS == tcphdr->source)
            tcphdr->source = 443;
        else if (WEBAUTH_NAT_TCPPORT_HTTP == tcphdr->source)
            tcphdr->source = 80;
        tcphdr->check = 0;
        tcphdr->check = tcpChecksum(iphdr);
    }
    
    else if (ethhdr->h_proto == 0x86DD)
    {
        ipv6hdr = (ipv6hdr_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));

        if (ipv6hdr->nexthdr != 6)
            return SVI_TX_HANDLED;

        tcphdr = (tcphdr_t*)(((uint8*) ipv6hdr) + sizeof(ipv6hdr_t));

        if (tcphdr->source != WEBAUTH_NAT_TCPPORT_HTTPS && tcphdr->source != WEBAUTH_NAT_TCPPORT_HTTP)
            return SVI_TX_HANDLED;

        if (0 != osal_memcmp(ipv6hdr->saddr, &g_sysIpv6, sizeof(sys_ipv6_t))
            || 0 != osal_memcmp(ipv6hdr->daddr, &g_natIpv6, sizeof(sys_ipv6_t)))
            return SVI_TX_HANDLED;

        if (NULL == (pSessInfo = _webauth_tcpSessInfoByTCP_ret(tcphdr->dest)))
            return SVI_TX_HANDLED;

        #if 0
        SYS_PRINTF("Find session %02x:%02x:%02x:%02x:%02x:%02x SA:%02x%02x::%02x%02x:%02x%02x:%02x%02x:%02x%02x DA:%02x%02x::%02x%02x:%02x%02x:%02x%02x:%02x%02x TCP:%d on port %s vlan %d\n",
                   pSessInfo->suppInfo.mac.octet[0],
                   pSessInfo->suppInfo.mac.octet[1],
                   pSessInfo->suppInfo.mac.octet[2],
                   pSessInfo->suppInfo.mac.octet[3],
                   pSessInfo->suppInfo.mac.octet[4],
                   pSessInfo->suppInfo.mac.octet[5],
                   pSessInfo->srcIPv6.hex[0],
                   pSessInfo->srcIPv6.hex[1],
                   pSessInfo->srcIPv6.hex[8],
                   pSessInfo->srcIPv6.hex[9],
                   pSessInfo->srcIPv6.hex[10],
                   pSessInfo->srcIPv6.hex[11],
                   pSessInfo->srcIPv6.hex[12],
                   pSessInfo->srcIPv6.hex[13],
                   pSessInfo->srcIPv6.hex[14],
                   pSessInfo->srcIPv6.hex[15],
                   pSessInfo->dstIPv6.hex[0],
                   pSessInfo->dstIPv6.hex[1],
                   pSessInfo->dstIPv6.hex[8],
                   pSessInfo->dstIPv6.hex[9],
                   pSessInfo->dstIPv6.hex[10],
                   pSessInfo->dstIPv6.hex[11],
                   pSessInfo->dstIPv6.hex[12],
                   pSessInfo->dstIPv6.hex[13],
                   pSessInfo->dstIPv6.hex[14],
                   pSessInfo->dstIPv6.hex[15],
                   pSessInfo->tcpPort,
                   LPSTR(pSessInfo->suppInfo.port),
                   pSessInfo->suppInfo.vid);
        #endif

        osal_memcpy(ethhdr->h_dest, pSessInfo->suppInfo.mac.octet, ETHER_ADDR_LEN);
        osal_memcpy(ipv6hdr->saddr, &pSessInfo->dstIPv6, sizeof(sys_ipv6_t));
        osal_memcpy(ipv6hdr->daddr, &pSessInfo->srcIPv6, sizeof(sys_ipv6_t));
        if (WEBAUTH_NAT_TCPPORT_HTTPS == tcphdr->source)
            tcphdr->source = 443;
        else if (WEBAUTH_NAT_TCPPORT_HTTP == tcphdr->source)
            tcphdr->source = 80;
        tcphdr->check = 0;
        tcphdr->check = tcpv6Checksum(ipv6hdr);
    }

    LOGIC_PORTMASK_CLEAR_ALL(toPorts);
    LOGIC_PORTMASK_SET_PORT(toPorts, pSessInfo->suppInfo.port);

    vid = pSessInfo->suppInfo.vid;

    rsd_nic_pkt_tx(pPkt, &toPorts, SYS_CPU_PORT_PRIORITY_AUTHMGR, vid,
                       (RSD_NIC_TX_CARE_VLAN | RSD_NIC_TX_CARE_STP | RSD_NIC_TX_CARE_TRUNK));

    return SVI_TX_NOT_HANDLED;
}

static int32 _webauth_pkt_fwd(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_vid_t vid = 0;
    sys_logic_port_t srcPort;
    sys_logic_portmask_t toPorts;
    sys_l2_entry_t l2Entry;

    srcPort = pPkt->rx_tag.source_port;

    rsd_nic_pktVid_get(pPkt, &vid);

    if (AUTHMGR_MODE_MULTI_HOST == g_authCfg.portCfg[srcPort].mode)
    {
        
        if (IS_PORT_GUEST_VLAN_ENABLE(srcPort) && vid == g_authCfg.guestVlan)
        {
            rsd_vlan_portAdminPvid_get(srcPort, &vid);
        }
    }

    l2Entry.vid = vid;
    osal_memcpy(&l2Entry.mac, pPkt->data, CAPA_MAC_BYTE_NUM);

    if (SYS_ERR_OK == rsd_l2_mac_search(&l2Entry))
    {
        
        LOGIC_PORTMASK_CLEAR_ALL(toPorts);
        LOGIC_PORTMASK_SET_PORT(toPorts, l2Entry.port);

        rsd_nic_pkt_tx(pPkt, &toPorts, SYS_CPU_PORT_PRIORITY_AUTHMGR, vid,
                           (RSD_NIC_TX_CARE_VLAN | RSD_NIC_TX_CARE_STP | RSD_NIC_TX_CARE_TRUNK));
    }
    else
    {
        
        LOGIC_PORTMASK_SET_ALL(toPorts);
        LOGIC_PORTMASK_CLEAR_PORT(toPorts, srcPort);

        rsd_nic_pkt_tx(pPkt, &toPorts, SYS_CPU_PORT_PRIORITY_AUTHMGR, vid,
                           (RSD_NIC_TX_CARE_VLAN | RSD_NIC_TX_CARE_STP | RSD_NIC_TX_CARE_TRUNK));
    }

    return TRUE;
}

uint32 webauth_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    ethhdr_t  *ethhdr = NULL;
    iphdr_t   *iphdr = NULL;
    ipv6hdr_t *ipv6hdr = NULL;
    tcphdr_t  *tcphdr = NULL;

    if (pPkt == NULL)
        return FALSE;

    ethhdr = (ethhdr_t*)(pPkt->data);
    iphdr = (iphdr_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));
    tcphdr = (tcphdr_t*)(((uint8*) iphdr) + HDR_DATALEN_TRANS(iphdr->ihl));

    if (ethhdr->h_proto == 0x806)
    {
        return _webauth_pkt_fwd(pPkt, pCookie);
    }

    if (ethhdr->h_proto == 0x800)
    {
        iphdr = (iphdr_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));
        tcphdr = (tcphdr_t*)(((uint8*) iphdr) + HDR_DATALEN_TRANS(iphdr->ihl));

        if (iphdr->protocol == 17 && ((tcphdr->dest == 53) || tcphdr->dest == 67))
        {
            return _webauth_pkt_fwd(pPkt, pCookie);
        }
    }
    
    else if (ethhdr->h_proto == 0x86DD)
    {
        ipv6hdr = (ipv6hdr_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));

        if (ipv6hdr->nexthdr == 58)
        {
            return _webauth_pkt_fwd(pPkt, pCookie);
        }

        tcphdr = (tcphdr_t*)(((uint8*) ipv6hdr) + sizeof(ipv6hdr_t));

        if (ipv6hdr->nexthdr == 17 && ((tcphdr->dest == 53) || tcphdr->dest == 547))
        {
            return _webauth_pkt_fwd(pPkt, pCookie);
        }
    }

    return FALSE;
}

static void webauth_sess_refresh(unsigned long id)
{
    _webauth_tcpSess_update(FALSE);
}

int32 webauth_init(void)
{
    
    webauth_sem = osal_sem_mutex_create();

    if (0 == webauth_sem)
    {
        WEBAUTH_DBG2("semaphore create failed");
        return SYS_ERR_FAILED;
    }

    _webauth_ipv6_init();

#ifdef CONFIG_SYS_L3_ROUTE
    SYS_ERR_CHK(sys_notifier_observer_register("webauth", SYS_NOTIFIER_SUBJECT_L3, SYS_NT_L3_INTF_IP_ADD, _webauth_mgmtIPChgEvent, SYS_NOTIFIER_PRI_LOW));
#else
    SYS_ERR_CHK(sys_notifier_observer_register("webauth", SYS_NOTIFIER_SUBJECT_SYSTEM, SYS_NT_SYS_MGMT_IP_CHG, _webauth_mgmtIPChgEvent, SYS_NOTIFIER_PRI_LOW));
#endif

    ROUTINE_START(webauthTimer, "WebAuth", webauth_sess_refresh, SYS_THREAD_PRI_LOW, 32768);

    SYS_ERR_CHK(svi_handler_register(&webauthTxHandler));

    return SYS_ERR_OK;
}

int32 webauth_exit(void)
{
    ROUTINE_STOP(webauthTimer);

    _webauth_tcpSess_update(TRUE);

    osal_sem_mutex_destroy(webauth_sem);

    SYS_ERR_CHK(svi_handler_unregister(&webauthTxHandler));

#ifdef CONFIG_SYS_L3_ROUTE
    SYS_ERR_CHK(sys_notifier_observer_unregister("webauth", SYS_NOTIFIER_SUBJECT_L3, _webauth_mgmtIPChgEvent));
#else
    SYS_ERR_CHK(sys_notifier_observer_unregister("webauth", SYS_NOTIFIER_SUBJECT_SYSTEM, _webauth_mgmtIPChgEvent));
#endif

    return SYS_ERR_OK;
}

webauth_t *webauth_create(sys_logic_port_t port)
{
    webauth_t *pWebauth = NULL;

    pWebauth = (webauth_t *) osal_alloc(sizeof(webauth_t));

    if (NULL != pWebauth)
    {
        osal_memset(pWebauth, 0, sizeof(webauth_t));

        pWebauth->reauthEnbl = g_authCfg.portCfg[port].reauthEnbl;
        pWebauth->initialize = TRUE;
    }

    return pWebauth;
}
void webauth_timer_routine(webauth_t *pWebauth)
{
    webauth_sm_check(pWebauth);

    return;
}

void webauth_timer_chk(webauth_t *pWebauth)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *)pWebauth->pParentSess;

    switch(pSess->status.authStatus)
    {
        case AUTHMGR_STATUS_AUTHED:
            
            if (authmgr_action_activeChk_ret(pSess))
                pSess->status.inactWhile = 0;

            if ((pSess->status.authorInfo.inactTime != 0)
                 && (pSess->status.inactWhile >= pSess->status.authorInfo.inactTime))
            {
                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_TIMEOUT_INACT,
                            pSess->suppInfo.mac.octet[0],
                            pSess->suppInfo.mac.octet[1],
                            pSess->suppInfo.mac.octet[2],
                            pSess->suppInfo.mac.octet[3],
                            pSess->suppInfo.mac.octet[4],
                            pSess->suppInfo.mac.octet[5],
                            LPSTR(pSess->suppInfo.port),
                            pSess->sessId.cnt,
                            pSess->sessId.time);

                pWebauth->inactTimeout = TRUE;
            }
            
            if (pSess->status.authedWhile >= pSess->status.authorInfo.reauthTime)
            {
                if (ENABLED == pSess->pPortCfg->reauthEnbl)
                {
                    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_TIMEOUT_REAUTH,
                                pSess->suppInfo.mac.octet[0],
                                pSess->suppInfo.mac.octet[1],
                                pSess->suppInfo.mac.octet[2],
                                pSess->suppInfo.mac.octet[3],
                                pSess->suppInfo.mac.octet[4],
                                pSess->suppInfo.mac.octet[5],
                                LPSTR(pSess->suppInfo.port),
                                pSess->sessId.cnt,
                                pSess->sessId.time);

                    pWebauth->reauthTimeout = TRUE;
                }
            }
            break;

        case AUTHMGR_STATUS_LOCKED:
            
            if (pSess->status.quietWhile >= pSess->pPortCfg->quietPeriod)
            {
                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_TIMEOUT_QUIET,
                            pSess->suppInfo.mac.octet[0],
                            pSess->suppInfo.mac.octet[1],
                            pSess->suppInfo.mac.octet[2],
                            pSess->suppInfo.mac.octet[3],
                            pSess->suppInfo.mac.octet[4],
                            pSess->suppInfo.mac.octet[5],
                            LPSTR(pSess->suppInfo.port),
                            pSess->sessId.cnt,
                            pSess->sessId.time);

                pWebauth->quietTimeout = TRUE;
            }
            break;

        case AUTHMGR_STATUS_INIT:
            if (pWebauth->inactWhile >= 120)
            {
                pSess->delSess = TRUE;
            }
            else
            {
                INC(pWebauth->inactWhile);
            }
            break;

        case AUTHMGR_STATUS_UNAUTHED:
            break;

        case AUTHMGR_STATUS_GUEST:
            pWebauth->guestChk = TRUE;
            break;

        default:
            break;
    }

    return;
}
int32 authmgr_webauthStatus_set(webauth_authInfo_t *pAuthInfo)
{
    authmgr_sess_t    *pSess = NULL;
    webauth_tcpSess_t *pTcpSess = NULL;
    webauth_t         *pWebauth = NULL;
    int32             ret = SYS_ERR_OK;

    SYS_PARAM_CHK((NULL == pAuthInfo), SYS_ERR_NULL_POINTER);

    if (NULL != (pSess = _webauth_sessByTCP_ret(pAuthInfo->tcpPort)))
    {
        AUTHMGR_SEM_LOCK(authmgr_sem);

        if (NULL != (pWebauth = pSess->pWebauth))
        {
            switch (pAuthInfo->authSts)
            {
                case SYS_AUTHING:
                    
                    if (NULL != (pTcpSess = _webauth_tcpSessByTCP_ret(pAuthInfo->tcpPort)))
                        pTcpSess->timer = WEBAUTH_TCP_AUTING_IDLE_TIME_MAX;
                    break;

                case SYS_AUTH:
                    ret = authmgr_action_authorInfo_set(pSess, &pAuthInfo->author, &pAuthInfo->authorSrc);
                    if (SYS_ERR_OK == ret)
                    {
                        pWebauth->authSuccess = TRUE;
                        webauth_sm_check(pWebauth);
                    }
                    else
                    {
                        if (0 != g_authCfg.portCfg[pSess->suppInfo.port].loginAttempt)
                        {
                            pWebauth->loginFailNum++;
                            if (pWebauth->loginFailNum >= g_authCfg.portCfg[pSess->suppInfo.port].loginAttempt)
                            {
                                pWebauth->authFail = TRUE;
                                webauth_sm_check(pWebauth);
                            }
                        }
                    }
                    if (NULL != (pTcpSess = _webauth_tcpSessByTCP_ret(pAuthInfo->tcpPort)))
                        pTcpSess->timer = WEBAUTH_TCP_SESSION_TIMEOUT;
                    break;

                case SYS_UNAUTH:
                    if (0 != g_authCfg.portCfg[pSess->suppInfo.port].loginAttempt)
                    {
                        pWebauth->loginFailNum++;
                        if (pWebauth->loginFailNum >= g_authCfg.portCfg[pSess->suppInfo.port].loginAttempt)
                        {
                            pWebauth->authFail = TRUE;
                            webauth_sm_check(pWebauth);
                        }
                    }
                    if (NULL != (pTcpSess = _webauth_tcpSessByTCP_ret(pAuthInfo->tcpPort)))
                        pTcpSess->timer = WEBAUTH_TCP_SESSION_TIMEOUT;
                    break;

                default:
                    break;
            }
        }

        AUTHMGR_SEM_UNLOCK(authmgr_sem);
    }

    return SYS_ERR_OK;
}
int32 authmgr_webauthStatusByKey_get(uint32 key, authmgr_status_t *pStatus)
{
    authmgr_sess_t *pSess = NULL;

    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((key > 65535), SYS_ERR_INPUT);

    if (NULL == (pSess = _webauth_sessByTCP_ret(key)))
        return SYS_ERR_NULL_POINTER;

    *pStatus = pSess->status.authStatus;

    return SYS_ERR_OK;
}
int32 authmgr_webauthSuppInfoByKey_get(uint32 key, authmgr_suppInfo_t *pSupp)
{
    webauth_tcpSessInfo_t *pTcpSessInfo = NULL;

    SYS_PARAM_CHK((NULL == pSupp), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((key > 65535), SYS_ERR_INPUT);

    if (NULL == (pTcpSessInfo = _webauth_tcpSessInfoByTCP_ret(key)))
        return SYS_ERR_NULL_POINTER;

    osal_memcpy(pSupp, &pTcpSessInfo->suppInfo, sizeof(authmgr_suppInfo_t));

    return SYS_ERR_OK;
}
