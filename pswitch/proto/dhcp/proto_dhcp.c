#include <common/sys_def.h>
#include <rsd/rsd_rate.h>
#include <rsd/rsd_l2.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_vlan.h>
#include <isg/proto_isg.h>
#include <dhcp/proto_dhcp.h>
#define DHCP_SNOOPING_LOCK()    \
do{                             \
    if (SYS_ERR_OK != osal_sem_mutex_take(g_dhcpTransationSem, OSAL_SEM_WAIT_FOREVER)){    \
        DSDBG("DS Lock Error");     \
        return SYS_ERR_SEM_LOCK;}          \
}while(0);
#define DHCP_SNOOPING_UNLOCK()    \
do{                     \
    if (SYS_ERR_OK != osal_sem_mutex_give(g_dhcpTransationSem)){    \
        DSDBG("DS unLock Error");     \
        return SYS_ERR_SEM_UNLOCK;}          \
}while(0);

#if 1
#define DSDBG(string, args...)         SYS_DBG(DBG_DHCP_SNOOPING, string, ##args)
#else
#define DSDBG(string, args...)         printk("%s(%u) "string"\n", __FUNCTION__, __LINE__, ##args)
#endif
static l2g_act_t _dhcp_pkt_ratelimit_chk(sys_nic_pkt_t *pPkt, void *pCookie);
static int32 _dhcp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);

static rsd_nic_handler_t l2gDHCPOfferACKHandler = {
    .priority       = RSD_NIC_HANDLER_PRI_DHCP_SERVER,
    .dmac           = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .dmac_caremask  = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .ethertype      = 0x800,
    .proto          = 17,
    .sport          = 0,
    .dport          = 68,
    .action         = RSD_NIC_HANDLER_ACT_COPY,
    .chk_cbf        = _dhcp_pkt_ratelimit_chk,
    .rx_cbf         = _dhcp_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_TRUNK,
    .pName          = "DHCP Snooping RX Server",
    .pCookie        = NULL,
};

static rsd_nic_handler_t l2gDHCPReleaseHandler = {
    .priority       = RSD_NIC_HANDLER_PRI_DHCP_CLIENT,
    .dmac           = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .dmac_caremask  = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .ethertype      = 0x800,
    .proto          = 17,
    .sport          = 0,
    .dport          = 67,
    .action         = RSD_NIC_HANDLER_ACT_COPY,
    .chk_cbf        = _dhcp_pkt_ratelimit_chk,
    .rx_cbf         = _dhcp_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_TRUNK,
    .pName          = "DHCP Snooping RX Client",
    .pCookie        = NULL,
};

static sys_mac_t g_sysMac;
static sys_enable_t g_port82Enable[SYS_LOGICPORT_NUM_MAX];
static sys_dhcp_option82_act_t g_port82Action[SYS_LOGICPORT_NUM_MAX];
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
static sys_dhcp_option82_format_t g_port82Format[SYS_LOGICPORT_NUM_MAX];
static sys_dhcp_option82_circuit_id_t g_port82VlanCircuitID[SYS_LOGICPORT_NUM_MAX][SYS_DHCP_O82_CID_VLAN_MAX+1];
#endif
static sys_dhcp_option82_id_t g_82RemoteID;

static sys_enable_t g_dsEnable = DISABLED;
static sys_dhcp_mib_t g_portMib[SYS_LOGICPORT_NUM_MAX];
static sys_vlanmask_t g_dsVlanEnabled;
static sys_logic_portmask_t g_trustPM;
static sys_logic_portmask_t g_chaddrChkPM;

static osal_thread_t g_dhcpThread;
static int32 g_portDhcpRateLimit[SYS_LOGICPORT_NUM_MAX];
static int32 g_portDhcpRateLimitCnt[SYS_LOGICPORT_NUM_MAX];

static int32 g_portDhcpRateLimitTmp[SYS_LOGICPORT_NUM_MAX];
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
static int8 g_portShutdownPortCnt[SYS_LOGICPORT_NUM_MAX];
#endif

static uint32 g_portDhcpCnt[DHCP_RATE_LAST_OVER_SEC][SYS_LOGICPORT_NUM_MAX];
static uint32 g_secondTick = 0; 
static osal_mutex_t  g_dhcpTransationSem;
static dhcp_request_transation_t g_dhcpTransationPool[SYS_DHCP_TRANSATION_MAX];
static dhcp_request_transation_t *g_dhcpTransationHead = NULL;
static uint16 ipChecksum(ipHdr_t * pip)
{
    uint32 sum = 0, oddbyte = 0;
    uint16 *ptr = (uint16 *) pip;
    uint32 nbytes = ((pip->ip_vhl & 0xf) << 2);

    while (nbytes > 1)
    {
        sum += (*ptr++);
        nbytes -= 2;
    }
    if (nbytes == 1)
    {
        oddbyte = (*ptr & 0xff00);
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);

    return (~sum);
}

static uint16 tcpChecksum(ipHdr_t * pip)
{
    int32 sum, nbytes, nhdr, i;
    uint16 *sip, *dip, *ptr;
    uint8 backup; 

    nhdr = ((pip->ip_vhl & 0xf) << 2);
    nbytes = osal_ntohs(pip->ip_len) - nhdr;
    ptr = (uint16 *) ((char *) pip + nhdr);
    sum = 0;

    backup = *(((uint8 *) pip) + nhdr + nbytes);
    *(((uint8 *) pip) + nhdr + nbytes) = (uint8) 0;
    for (i = 0; i < nbytes; i = i + 2)
    {
        sum += (unsigned long) osal_ntohs(*ptr);
        ptr++;
    }

    dip = (uint16 *) & pip->ip_dst;
    sum += osal_ntohs(*dip);
    dip++;
    sum += osal_ntohs(*dip);
    sip = (uint16 *) & pip->ip_src;
    sum += osal_ntohs(*sip);
    sip++;
    sum += osal_ntohs(*sip);

    sum += nbytes;
    sum += ((uint16) pip->ip_p);

    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    *(((uint8 *) pip) + nhdr + nbytes) = backup;

    sum = ~sum;
    return osal_htons((uint16) sum);
}

#ifdef CONFIG_SYS_PROTO_ISG
static int32 _isg_entry_set(sys_logic_port_t rxPort, dhcpMessage *pDhcpMsg, dhcp_info_t *pDhcpinfo)
{
    sys_err_code_t ret = SYS_ERR_OK;
    sys_isg_entry_t bindEntry;
    sys_l2_entry_t l2_ucast_entry;
    rtk_l2_ucastAddr_t ucastAddr;
	sys_logic_port_t lp;
	sys_physic_port_t pp;

    osal_memset(&bindEntry, 0, sizeof(sys_isg_entry_t));

    osal_memset(&l2_ucast_entry, 0, sizeof(sys_l2_entry_t));
    l2_ucast_entry.vid = pDhcpinfo->vid;
    osal_memcpy(&l2_ucast_entry.mac, pDhcpMsg->chaddr, CAPA_MAC_BYTE_NUM);

    SYS_MEM_CLEAR(ucastAddr);
    osal_memcpy(&ucastAddr.mac, pDhcpMsg->chaddr, sizeof(sys_mac_t));
    ucastAddr.vid = pDhcpinfo->vid;
    SYS_ERR_XLATE_CHK(rtk_l2_addr_get(SYS_DEV_ID_DEFAULT, &ucastAddr));
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
	if (IS_CHIP_CSW() && (ucastAddr.flags & RTK_L2_UCAST_FLAG_TRUNK_PORT))
	{
		bindEntry.port = TRK2LP(ucastAddr.trk_gid);
	}
	else
#endif
	{
		pp.dev = SYS_DEV_ID_DEFAULT;
		pp.port = ucastAddr.port;
		SYS_ERR_CHK(sys_physic_to_logic_port(pp, &lp));

		bindEntry.port = LP2TRKLP(lp);
	}

    osal_memcpy(&bindEntry.mac, pDhcpMsg->chaddr, CAPA_MAC_BYTE_NUM);
    bindEntry.port = rxPort;
    bindEntry.ip = pDhcpMsg->yiaddr;
    bindEntry.mask = 0xFFFFFFFF;
    bindEntry.vid = pDhcpinfo->vid;
    bindEntry.isCareMac = TRUE;
    bindEntry.type = ISG_ENTRY_DYNAMIC;
    bindEntry.leaseTime = pDhcpinfo->leaseTime;

    DSDBG("DHCP Snooping insert entry, Src Port %s, VLAN %u,IP %u.%u.%u.%u, MAC %2x:%2x:%2x:%2x:%2x:%2x",
            LPSTR(bindEntry.port), bindEntry.vid,(bindEntry.ip >> 24) & 0xff,
            (bindEntry.ip >> 16) & 0xff,(bindEntry.ip >> 8) & 0xff, (bindEntry.ip >> 0) & 0xff,
            bindEntry.mac.octet[0]& 0xff, bindEntry.mac.octet[1]& 0xff,bindEntry.mac.octet[2]& 0xff,
            bindEntry.mac.octet[3]& 0xff, bindEntry.mac.octet[4]& 0xff, bindEntry.mac.octet[5]& 0xff);

    ret = isg_bindingEntry_set(&bindEntry, TRUE);

    if (SYS_ERR_OK != ret)
        DSDBG("DHCP Snooping insert entry failed err %u", ret);

    return ret;
}

static int32 _isg_entry_act(sys_logic_port_t port, sys_vid_t vid, dhcpMessage *pDhcpMsg, int8 isDel)
{
    sys_isg_entry_t bindEntry;

    osal_memset(&bindEntry, 0, sizeof(sys_isg_entry_t));

    osal_memcpy(&bindEntry.mac, pDhcpMsg->chaddr, CAPA_MAC_BYTE_NUM);
    bindEntry.ip = pDhcpMsg->ciaddr;
    bindEntry.vid = vid;
    bindEntry.port = port;
    bindEntry.type = ISG_ENTRY_DYNAMIC;

    if (!isDel)
        SYS_ERR_CHK(isg_bindingEntry_exist(&bindEntry, TRUE));
    else
        SYS_ERR_CHK(isg_bindingEntry_del(&bindEntry));

    return SYS_ERR_OK;
}

static int32 _dhcp_transation_chk(dhcpMessage *pDhcpMsg, dhcp_info_t *pDhcpinfo, sys_logic_port_t *clientPort)
{
    uint8 macStr[CAPA_MAC_STR_LEN];
    dhcp_request_transation_t *transation = NULL;

    DHCP_SNOOPING_LOCK();

    if (NULL == g_dhcpTransationHead)
    {
        DHCP_SNOOPING_UNLOCK();

        return SYS_ERR_ENTRY_NOT_FOUND;
    }

    transation = g_dhcpTransationHead;

    do
    {
        
        if ((pDhcpinfo->vid == transation->vid) && (0 == osal_memcmp(pDhcpMsg->chaddr, &transation->mac, CAPA_MAC_BYTE_NUM)))
        {
            DSDBG("dhcp trans del! vid = %u %s mac %x:%x:%x:%x:%x:%x",
                transation->vid, LPSTR(transation->port), transation->mac.octet[0], transation->mac.octet[1],
                transation->mac.octet[2], transation->mac.octet[3], transation->mac.octet[4],
                transation->mac.octet[5]);

            *clientPort = transation->port;

            if (g_dhcpTransationHead == g_dhcpTransationHead->next)
            {
                g_dhcpTransationHead = NULL;
            }
            else
            {
                if (g_dhcpTransationHead == transation)
                    g_dhcpTransationHead = transation->next;

                transation->next->prev = transation->prev;
                transation->prev->next = transation->next;
            }

            osal_memset(transation, 0 , sizeof(dhcp_request_transation_t));

            DHCP_SNOOPING_UNLOCK();

            return SYS_ERR_OK;
        }

        transation = transation->next;
    }while ((NULL != g_dhcpTransationHead) && (transation != g_dhcpTransationHead));

    DHCP_SNOOPING_UNLOCK();

    SYS_MEM_CLEAR(macStr);
    SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr));
    SYS_LOG(LOG_CAT_DHCP_SNOOPING,  LOG_DHCP_SNOOPING_TRANSACTION_FAIL, pDhcpinfo->vid, LPSTR(pDhcpinfo->port), macStr);

    return SYS_ERR_ENTRY_NOT_FOUND;
}
#endif

static void _dhcp_timer_thread(void *pInput)
{
    uint8 cnt = 0;
    dhcp_request_transation_t *transation = NULL;
    dhcp_request_transation_t *transation_next = NULL;

    static sys_logic_port_t lp = 0;
    static struct timespec time;
    static uint32 diff = 0;
    static int32 prev_usec = 0;
    static int32 now_usec = 0;
    static uint8 tmp[SYS_LOGICPORT_NUM_MAX] = {1,};

    for (;;)
    {
        if (DISABLED == g_dsEnable)
        {
            
            osal_time_sleep(1);
        }
        else
        {
            
            osal_time_usleep(1 * 1000 * (1000 / DHCP_RATE_TICK_PER_SEC));
        }

        diff = 0;
        time = current_kernel_time();

        now_usec = time.tv_nsec / 10000000;

        if (now_usec < prev_usec)
        {
            prev_usec -= 100;

            FOR_EACH_LOGIC_PORT(lp)
            {
                if (g_portDhcpCnt[g_secondTick % DHCP_RATE_LAST_OVER_SEC][lp] != 0)
                    DSDBG("idx %u %s %3u %3u", g_secondTick % DHCP_RATE_LAST_OVER_SEC, LPSTR(lp+1),
                    g_portDhcpCnt[0][lp], g_portDhcpCnt[1][lp]);
            }

            g_secondTick++;
            SYS_MEM_CLEAR(g_portDhcpCnt[g_secondTick % DHCP_RATE_LAST_OVER_SEC]);
        }

        diff = now_usec - prev_usec;

        FOR_EACH_LOGIC_PORT(lp)
        {
            if (BOND_RATE_UNLIMITED != g_portDhcpRateLimit[lp])
            {
                if (g_portDhcpRateLimitCnt[lp] < 0)
                    tmp[lp]++;
                else
                    tmp[lp] = 1;

                if (DHCP_RATE_TICK_PER_SEC == tmp[lp])
                {
                    tmp[lp] = 1;
                    diff = 0;
                }

                if (g_portDhcpRateLimitCnt[lp] < g_portDhcpRateLimit[lp])
                    g_portDhcpRateLimitCnt[lp] += (diff * g_portDhcpRateLimit[lp]) / 10;
            }
            if (DHCP_RATE_TICK_PER_SEC == g_portShutdownPortCnt[lp])
            {
                SYS_LOG(LOG_CAT_DHCP_SNOOPING,  LOG_DHCP_SNOOPING_RATE_EXCEED, g_portDhcpRateLimit[lp]+1, LPSTR(lp));
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
                SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_ERRDISABLE, LPSTR(lp));
                if (SYS_ERR_FAILED == rsd_port_errDisable_set(lp, SYS_PORT_ERRDISABLE_DHCP_RATE_LIMIT))
                    DSDBG("errdisabled port %s failed", LPSTR(lp));
#endif
                g_portShutdownPortCnt[lp]--;
            }
            else if (0 != g_portShutdownPortCnt[lp])
            {
                g_portShutdownPortCnt[lp]--;
            }
        }

        prev_usec = now_usec ;

        cnt++;
        if (0 != (cnt%DHCP_RATE_TICK_PER_SEC))
            continue;

        if (SYS_ERR_OK != osal_sem_mutex_take(g_dhcpTransationSem, OSAL_SEM_WAIT_FOREVER))
        {
            DSDBG("dhcp snooping Lock Error");
            continue;
        }

        if (NULL == g_dhcpTransationHead)
        {
            if (SYS_ERR_OK != osal_sem_mutex_give(g_dhcpTransationSem))
                DSDBG("dhcp snooping unLock Error");

            continue;
        }

        transation_next = g_dhcpTransationHead;

    	do
    	{
    	    transation = transation_next;
    	    transation_next = transation->next;
    	    transation->timer--;

    	    if (0 == transation->timer)
    	    {
    	        DSDBG("dhcp trans ageout! vid = %u %s mac %x:%x:%x:%x:%x:%x",
    	            transation->vid, LPSTR(transation->port), transation->mac.octet[0], transation->mac.octet[1],
    	            transation->mac.octet[2], transation->mac.octet[3], transation->mac.octet[4],
    	            transation->mac.octet[5]);

    	        if (g_dhcpTransationHead == g_dhcpTransationHead->next)
    	        {
    	            g_dhcpTransationHead = NULL;
    	        }
    	        else
    	        {
    	            if (g_dhcpTransationHead == transation)
    	                g_dhcpTransationHead = transation->next;

    	            transation->next->prev = transation->prev;
    	            transation->prev->next = transation->next;
    	        }

    	        osal_memset(transation, 0 , sizeof(dhcp_request_transation_t));
    	    }
    	}while((NULL != g_dhcpTransationHead) && (transation_next != g_dhcpTransationHead));

    	if (SYS_ERR_OK != osal_sem_mutex_give(g_dhcpTransationSem))
    	{
    	    DSDBG("dhcp snooping unLock Error");
    	    continue;
    	}

    	transation = NULL;
    	transation_next = NULL;

    }

    osal_thread_exit(0);

    return;
}
static int32 _dhcp_transation_add(dhcpMessage *pDhcpMsg, dhcp_info_t *pDhcpinfo)
{
    uint32 i = 0;
    dhcp_request_transation_t *transation = NULL;

    DHCP_SNOOPING_LOCK();

    for (i = 0; i< SYS_DHCP_TRANSATION_MAX; i++)
    {
        if (0 == g_dhcpTransationPool[i].timer)
            break;
    }

    if (SYS_DHCP_TRANSATION_MAX == i)
    {
        DHCP_SNOOPING_UNLOCK();

        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_TRANSACTION_FULL);

        return SYS_ERR_ENTRY_FULL;
    }

    transation = &g_dhcpTransationPool[i];

    osal_memcpy(&transation->mac, pDhcpMsg->chaddr, CAPA_MAC_BYTE_NUM);
    transation->vid = pDhcpinfo->vid;
    transation->port = pDhcpinfo->port;
    transation->timer = SYS_DHCP_TRANSATION_AGEOUT;

    DSDBG("dhcp trans add! vid = %u %s mac %x:%x:%x:%x:%x:%x",
                transation->vid, LPSTR(transation->port), transation->mac.octet[0], transation->mac.octet[1],
                transation->mac.octet[2], transation->mac.octet[3], transation->mac.octet[4],
                transation->mac.octet[5]);

    if (NULL == g_dhcpTransationHead)
    {
        g_dhcpTransationHead = transation;
        transation->next = g_dhcpTransationHead;
        transation->prev = g_dhcpTransationHead;
    }
    else
    {
        transation->next = g_dhcpTransationHead;
        transation->prev = g_dhcpTransationHead->prev;
        g_dhcpTransationHead->prev->next = transation;
        g_dhcpTransationHead->prev = transation;
    }

    DHCP_SNOOPING_UNLOCK();

    return SYS_ERR_OK;
}

static int32 _option82_process(sys_nic_pkt_t *pPkt, dhcpMessage *pDhcpMsg, dhcp_info_t *pDhcpinfo)
{
    char *option82_user = NULL;
    uint8 i = 0;
    uint16 option = 0;
    int16 max_option_len = 0;
    uint32 offset = 0;
    uint32 option82Size = 0;
    uint32 option82OldSize = 0;
    ipHdr_t *pIpHdr = NULL;
    udphdr_t *pUdpHdr = NULL;
    dhcp_option82tlv_normal_t *option82 = NULL;
    sys_enable_t vlanEnable = DISABLED;
    sys_enable_t option82Enable = DISABLED;
    sys_enable_t trust = DISABLED;
    sys_dhcp_option82_act_t option82Action = DHCP_OPTION82_KEEP;
    sys_dhcp_option82_format_t option82Format = DHCP_OPTION82_FORMAT_NORMAL;
    uint8 macStr1[CAPA_MAC_STR_LEN];
    uint8 macStr2[CAPA_MAC_STR_LEN];

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == pDhcpinfo, SYS_ERR_NULL_POINTER);

    dhcp_snooping_vlan_enable_get(pDhcpinfo->vid, &vlanEnable);
    dhcp_option82_enable_get(pDhcpinfo->port, &option82Enable);
    dhcp_snooping_port_trust_get(pDhcpinfo->port, &trust);

    if (DISABLED == option82Enable)
        return SYS_PKT_ACTION_FORWARD;

    if (DISABLED == vlanEnable)
        return SYS_PKT_ACTION_FORWARD;

    if (pPkt->rx_tag.cvid_tagged)
        offset += VLANID_OFFSET;

    if (pPkt->rx_tag.svid_tagged)
        offset += VLANID_OFFSET;

    i = 0;
    pIpHdr = (ipHdr_t *)(pPkt->data + offset + 14);
    pUdpHdr = (udphdr_t *)(pPkt->data + offset + 34);

    max_option_len = pDhcpMsg->options - pPkt->data;

    if (max_option_len <= 0)
    {
        g_portMib[pDhcpinfo->port].dhcpInvalidDrop++;
        SYS_MEM_CLEAR(macStr1);
        SYS_MEM_CLEAR(macStr2);
        SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
        SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, pDhcpinfo->vid,
            LPSTR(pDhcpinfo->port), macStr1, macStr2);

        return SYS_ERR_OK;
    }

    offset = 0;

    if ((DHCP_OFFER == pDhcpinfo->opcode) || (DHCP_ACK == pDhcpinfo->opcode) || (DHCP_NACK == pDhcpinfo->opcode))
    {

        offset = 0;

        while(1)
        {
            if (offset > (pPkt->length - max_option_len))
            {
                DSDBG("Drop1 invalid packet offset %u over max option len %u!", (offset + *(pDhcpMsg->options + offset + 1)), (pPkt->length - max_option_len));
                g_portMib[pDhcpinfo->port].dhcpInvalidDrop++;
                SYS_MEM_CLEAR(macStr1);
                SYS_MEM_CLEAR(macStr2);
                SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
                SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
                SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, pDhcpinfo->vid,
                    LPSTR(pDhcpinfo->port), macStr1, macStr2);

                return SYS_PKT_ACTION_DROP;
            }

            if(0x00 == *(pDhcpMsg->options + offset))
            {
                offset++;
                continue;
            }

            if (*(pDhcpMsg->options + offset) == 82) 
            {
                DSDBG("Remove option82 %s", LPSTR(pDhcpinfo->port));

                option82 = (dhcp_option82tlv_normal_t *)(pDhcpMsg->options + offset);

                option82Size = option82->len + 2;

                osal_memset((pDhcpMsg->options + offset + 2), 0, option82Size);
                option82->opcode = 0xFF;
                option82->len = 0;

                pIpHdr->ip_len = osal_htons(osal_ntohs(pIpHdr->ip_len) - option82Size);
                pUdpHdr->uh_ulen = osal_htons(osal_ntohs(pUdpHdr->uh_ulen) - option82Size);
                pIpHdr->ip_sum = 0;
                pIpHdr->ip_sum = ipChecksum(pIpHdr);
                pUdpHdr->uh_sum = 0;
                pUdpHdr->uh_sum = tcpChecksum(pIpHdr);
                pPkt->tail -= option82Size;
                pPkt->length -= option82Size;

                break;
            }
            else if (*(pDhcpMsg->options + offset) == 0xFF) 
            {
                if (offset > pPkt->length)
                {
                    DSDBG("Drop1 invalid packet offset %u over pkt len %u!", offset,pPkt->length);
                    g_portMib[pDhcpinfo->port].dhcpInvalidDrop++;
                    SYS_MEM_CLEAR(macStr1);
                    SYS_MEM_CLEAR(macStr2);
                    SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
                    SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
                    SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, pDhcpinfo->vid,
                        LPSTR(pDhcpinfo->port), macStr1, macStr2);

                    return SYS_PKT_ACTION_DROP;
                }
                else
                {
                    break;
                }
            }

            if ((offset + *(pDhcpMsg->options + offset + 1)) > (pPkt->length - max_option_len))
            {
                DSDBG("Drop1 invalid packet offset %u over max option len %u!", (offset + *(pDhcpMsg->options + offset + 1)), (pPkt->length - max_option_len));
                g_portMib[pDhcpinfo->port].dhcpInvalidDrop++;
                SYS_MEM_CLEAR(macStr1);
                SYS_MEM_CLEAR(macStr2);
                SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
                SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
                SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, pDhcpinfo->vid,
                    LPSTR(pDhcpinfo->port), macStr1, macStr2);

                return SYS_PKT_ACTION_DROP;
            }

            offset += *(pDhcpMsg->options + offset + 1);
            offset += 2;
        }
    }
    else  
    {
        dhcp_option82_action_get(pDhcpinfo->port, &option82Action);

        DSDBG("%s 82 is %u act %u format %u", LPSTR(pDhcpinfo->port),
                option82Enable, option82Action, option82Format);

        if (ENABLED == trust)
            return SYS_PKT_ACTION_FORWARD;

        while(1)
        {
            if (offset > (pPkt->length - max_option_len))
            {
                DSDBG("Drop1 invalid packet offset %u over max option len %u!", (offset + *(pDhcpMsg->options + offset + 1)), (pPkt->length - max_option_len));
                g_portMib[pDhcpinfo->port].dhcpInvalidDrop++;
                SYS_MEM_CLEAR(macStr1);
                SYS_MEM_CLEAR(macStr2);
                SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
                SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
                SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, pDhcpinfo->vid,
                LPSTR(pDhcpinfo->port), macStr1, macStr2);

                return SYS_PKT_ACTION_DROP;
            }

            option = *(pDhcpMsg->options + offset);

            if(0x00 == option)
            {
                offset++;
                continue;
            }

            if (82 == option)
            {
                if (DHCP_OPTION82_DROP == option82Action)
                {
                    DSDBG("Drop pkt because of un trust port %s not allow option82", LPSTR(pDhcpinfo->port));

                    return SYS_PKT_ACTION_DROP;
                }
                else if (DHCP_OPTION82_KEEP == option82Action)
                {
                    DSDBG("Froward pkt because of un trust port %s action is keep", LPSTR(pDhcpinfo->port));

                    return SYS_PKT_ACTION_FORWARD;
                }
                else if (DHCP_OPTION82_REBUILD == option82Action)
                {
                    DSDBG("Rebuild pkt because of un trust port %s action is rebuild", LPSTR(pDhcpinfo->port));

                    option82 = (dhcp_option82tlv_normal_t *)(pDhcpMsg->options + offset);
                    option82OldSize = option82->len + 2;
                }
                break;
            }
            else if (*(pDhcpMsg->options + offset) == 0xFF) 
            {
                if (offset > pPkt->length)
                {
                    DSDBG("Drop1 invalid packet offset %u over pkt len %u!", offset,pPkt->length);
                    g_portMib[pDhcpinfo->port].dhcpInvalidDrop++;
                    SYS_MEM_CLEAR(macStr1);
                    SYS_MEM_CLEAR(macStr2);
                    SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
                    SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
                    SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, pDhcpinfo->vid,
                        LPSTR(pDhcpinfo->port), macStr1, macStr2);

                    return SYS_PKT_ACTION_DROP;
                }
                else
                {
                    option82 = (dhcp_option82tlv_normal_t *)(pDhcpMsg->options + offset);
                    option82->opcode = 82;
                    break;
                }
            }

            if (offset > (pPkt->length - max_option_len))
            {
                DSDBG("Drop1 invalid packet offset %u over max option len %u!", (offset + *(pDhcpMsg->options + offset + 1)), (pPkt->length - max_option_len));
                g_portMib[pDhcpinfo->port].dhcpInvalidDrop++;
                SYS_MEM_CLEAR(macStr1);
                SYS_MEM_CLEAR(macStr2);
                SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
                SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
                SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, pDhcpinfo->vid,
                    LPSTR(pDhcpinfo->port), macStr1, macStr2);

                return SYS_PKT_ACTION_DROP;
            }

            offset += *(pDhcpMsg->options + offset + 1);
            offset += 2;
        }

        option82Size = 0;
        option82_user = pDhcpMsg->options + offset;

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
        
        for (i = 0; i < SYS_DHCP_O82_CID_VLAN_MAX ; i++)
        {
            if (g_port82VlanCircuitID[pDhcpinfo->port][i].vid == pDhcpinfo->vid)
            {
                option82Format = DHCP_OPTION82_FORMAT_USERDEFINE;
                DSDBG("O82 %s vlan %u Cid found %s", LPSTR(pDhcpinfo->port), pDhcpinfo->vid,
                    g_port82VlanCircuitID[pDhcpinfo->port][i].str);

                break;
            }
        }

        if ((SYS_DHCP_O82_CID_VLAN_MAX == i) && (0 == osal_strlen(g_port82VlanCircuitID[pDhcpinfo->port][i].str)))
        {
            option82Format = DHCP_OPTION82_FORMAT_NORMAL;
            DSDBG("O82 Cid not found, use default format");
        }
        else
        {
            option82Format = DHCP_OPTION82_FORMAT_USERDEFINE;
            DSDBG("O82 %s vlan %u Cid found %s", LPSTR(pDhcpinfo->port), pDhcpinfo->vid,
                    g_port82VlanCircuitID[pDhcpinfo->port][i].str);
        }
#else
        option82Format = DHCP_OPTION82_FORMAT_NORMAL;
        DSDBG("O82 Cid not found, use default format");
#endif

        if (DHCP_OPTION82_FORMAT_NORMAL == option82Format)
        {
            
            option82_user += 2;
            *option82_user = 1;

            option82_user++;
            *option82_user = 2 + 4;

            option82_user ++;
            *option82_user = 0;

            option82_user ++;
            *option82_user = 4;

            option82_user ++;
            *(uint16 *)option82_user = osal_htons(pDhcpinfo->vid);
            option82_user += 2;
            *(uint16 *)option82_user = osal_htons(LP2UP(pDhcpinfo->port));
            option82_user += 2;
            option82Size += 4;
        }
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
        else if (DHCP_OPTION82_FORMAT_USERDEFINE == option82Format)
        {
            
            option82_user += 2;
            *option82_user = 1;

            option82_user++;
            *option82_user = 2 + osal_strlen(g_port82VlanCircuitID[pDhcpinfo->port][i].str);

            option82_user ++;
            *option82_user = 1;

            option82_user ++;
            *option82_user = osal_strlen(g_port82VlanCircuitID[pDhcpinfo->port][i].str);

            option82_user ++;
            osal_strcpy(option82_user, g_port82VlanCircuitID[pDhcpinfo->port][i].str);
            option82_user += osal_strlen(g_port82VlanCircuitID[pDhcpinfo->port][i].str);
            option82Size += osal_strlen(g_port82VlanCircuitID[pDhcpinfo->port][i].str);
        }
#endif
        
        option82Size +=4;

        *option82_user = 2;

        if (0 == osal_strlen(g_82RemoteID.str))
        {
            
            option82_user ++;
            *option82_user = 2 + 6;

            option82_user ++;
            *option82_user = 0;

            option82_user ++;
            *option82_user = 6;

            option82_user ++;
            osal_memcpy(option82_user, g_sysMac.octet, 6);
            option82_user += 6;

            option82Size += 6;
        }
        else
        {
            
            option82_user ++;
            *option82_user = 2 + osal_strlen(g_82RemoteID.str);

            option82_user ++;
            *option82_user = 1;

            option82_user ++;
            *option82_user = osal_strlen(g_82RemoteID.str);

            option82_user ++;
            osal_strcpy(option82_user, g_82RemoteID.str);
            option82_user += osal_strlen(g_82RemoteID.str);

            option82Size += osal_strlen(g_82RemoteID.str);
        }

        option82Size += 4;

        *(option82_user) = 0xFF;

        option82->len = option82Size;

        option82Size+=2;

        pIpHdr->ip_len = osal_htons(osal_ntohs(pIpHdr->ip_len) + option82Size- option82OldSize);
        pUdpHdr->uh_ulen = osal_htons(osal_ntohs(pUdpHdr->uh_ulen) + option82Size- option82OldSize);
        pIpHdr->ip_sum = 0;
        pIpHdr->ip_sum = ipChecksum(pIpHdr);
        pUdpHdr->uh_sum = 0;
        pUdpHdr->uh_sum = tcpChecksum(pIpHdr);
        pPkt->tail += option82Size - option82OldSize;
        pPkt->length = pPkt->length + option82Size - option82OldSize;
    }

    return SYS_PKT_ACTION_FORWARD;
}

static sys_pkt_action_t _snooping_process(sys_nic_pkt_t *pPkt, dhcpMessage *pDhcpMsg, dhcp_info_t *pDhcpinfo)
{
    sys_enable_t chaddrChkEnable = DISABLED;
    sys_enable_t trust = DISABLED;
#ifdef CONFIG_SYS_PROTO_ISG
    sys_enable_t clientTrust = ENABLED;
    sys_logic_port_t clientPort = 0;
    sys_err_code_t ret = SYS_ERR_OK;
#endif
    uint8 macStr1[CAPA_MAC_STR_LEN];
    uint8 macStr2[CAPA_MAC_STR_LEN];

    dhcp_snooping_port_trust_get(pDhcpinfo->port, &trust);

    if (ENABLED == trust)
    {
        if (DHCP_ACK == pDhcpinfo->opcode)
        {
#ifdef CONFIG_SYS_PROTO_ISG
            if (SYS_ERR_OK == _dhcp_transation_chk(pDhcpMsg, pDhcpinfo, &clientPort))
            {
                dhcp_snooping_port_trust_get(clientPort, &clientTrust);

                if (DISABLED == clientTrust)
                {
                    if (SYS_ERR_ISG_MAX_ENTRY_REACH == _isg_entry_set(clientPort, pDhcpMsg, pDhcpinfo))
                        DSDBG("insert isg entry failed");
                }
            }
#endif
        }
    }
    else
    {
        
        if ((DHCP_OFFER == pDhcpinfo->opcode) || (DHCP_ACK == pDhcpinfo->opcode) || (DHCP_NACK == pDhcpinfo->opcode))
        {
            g_portMib[pDhcpinfo->port].dhcpUntrustDrop++;

            SYS_MEM_CLEAR(macStr1);
            SYS_MEM_CLEAR(macStr2);
            SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr1));
            if (DHCP_OFFER == pDhcpinfo->opcode)
                osal_sprintf(macStr2, "DHCP_OFFER");
            else if (DHCP_ACK == pDhcpinfo->opcode)
                osal_sprintf(macStr2, "DHCP_ACK");
            else if (DHCP_NACK == pDhcpinfo->opcode)
                osal_sprintf(macStr2, "DHCP_NACK");

            SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_UNTRUSTED_DROP,pDhcpinfo->vid,
                LPSTR(pDhcpinfo->port), macStr1, macStr2);

            return SYS_PKT_ACTION_DROP;
        }

        dhcp_snooping_portChaddrChk_get(pDhcpinfo->port, &chaddrChkEnable);

        if (ENABLED == chaddrChkEnable)
        {
            
            if (osal_memcmp(pDhcpMsg->chaddr, pPkt->data + CAPA_MAC_BYTE_NUM, CAPA_MAC_BYTE_NUM) != 0)
            {
                
                g_portMib[pDhcpinfo->port].dhcpChaddrChkDrop++;

                SYS_MEM_CLEAR(macStr1);
                SYS_MEM_CLEAR(macStr2);
                SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr1));
                SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr2));
                SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_CHADDR_DROP, pDhcpinfo->vid,
                    LPSTR(pDhcpinfo->port), macStr1, macStr2);

                return SYS_PKT_ACTION_DROP;
            }
        }

        if (DHCP_REQUEST == pDhcpinfo->opcode)
            _dhcp_transation_add(pDhcpMsg, pDhcpinfo);

#ifdef CONFIG_SYS_PROTO_ISG
        if ((DHCP_RELEASE == pDhcpinfo->opcode) || (DHCP_DECLIENT == pDhcpinfo->opcode))
        {
            ret = _isg_entry_act(pDhcpinfo->port, pDhcpinfo->vid, pDhcpMsg, FALSE);

            if (SYS_ERR_OK != ret)
            {
                DSDBG("DHCP Release or decilent from different port of request");
                g_portMib[pDhcpinfo->port].dhcpInvalidDrop++;
                SYS_MEM_CLEAR(macStr1);
                SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr1));
                SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_RELEASE_INVALID, pDhcpinfo->vid,
                    LPSTR(pDhcpinfo->port), macStr1);

                return SYS_PKT_ACTION_DROP;
            }
            else if (SYS_ERR_OK == ret)
            {
                if (SYS_ERR_OK != _isg_entry_act(pDhcpinfo->port, pDhcpinfo->vid, pDhcpMsg, TRUE))
                    DSDBG("Delete DHCP Snooping binding entry Failed");
            }
        }
#endif
    }

    return SYS_PKT_ACTION_FORWARD;
}
static l2g_act_t _dhcp_pkt_ratelimit_chk(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_enable_t enable = DISABLED;
    sys_vlan_membership_t membership;
    sys_logic_port_t srcPort;
    sys_vid_t vid = 0;

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);

    srcPort = LP2TRKLP(pPkt->rx_tag.source_port);
    SYS_ERR_CHK(rsd_nic_pktVid_get(pPkt, &vid));
    dhcp_snooping_vlan_enable_get(vid, &enable);

    if (DISABLED == enable)
        return RSD_NIC_HANDLER_ACT_COPY;

    dhcp_snooping_port_trust_get(srcPort, &enable);

        if (ENABLED == enable)
        return RSD_NIC_HANDLER_ACT_COPY;
    SYS_ERR_CHK(rsd_vlan_portOperVlanMembership_get(srcPort,vid, &membership));

    if ((membership != SYS_VLAN_UNTAGGED) && (membership != SYS_VLAN_TAGGED))
    {
        DSDBG("free packet rx from port %s because vid is %u not in VLAN membership(%u)",
            LPSTR (srcPort), vid, membership);

        return RSD_NIC_HANDLER_ACT_DROP;
    }

    g_portDhcpCnt[g_secondTick % DHCP_RATE_LAST_OVER_SEC][srcPort]++;

    if (BOND_RATE_UNLIMITED != g_portDhcpRateLimit[srcPort])
    {
        if (g_portDhcpRateLimitCnt[srcPort] < 0 - g_portDhcpRateLimitTmp[srcPort])
        {
            if ((g_portDhcpCnt[0][srcPort] >= g_portDhcpRateLimit[srcPort])
                && (g_portDhcpCnt[1][srcPort] >= g_portDhcpRateLimit[srcPort]))
            {

                if (0 == g_portShutdownPortCnt[srcPort])
                    g_portShutdownPortCnt[srcPort] = DHCP_RATE_TICK_PER_SEC;
            }

            return RSD_NIC_HANDLER_ACT_DROP;
        }

        g_portDhcpRateLimitCnt[srcPort] -= DHCP_RATE_TICK_PER_SEC;
    }

    return RSD_NIC_HANDLER_ACT_COPY;
}

static int32 _dhcp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    uint32 offset = 0;
    uint16 option = 0;
    int16 max_option_len = 0;
    uint8 isOp53Found = FALSE;
    char buf[CAPA_PORTMASK_STR_LEN];
    char *pdhcp = NULL;
    dhcpMessage *pDhcpMsg = NULL;
    dhcp_info_t dhcpinfo;

    sys_enable_t vlan_enable = DISABLED;
    sys_enable_t trust = DISABLED;
    sys_vid_t vid = 0;
    sys_logic_port_t srcPort = 0;
    sys_logic_portmask_t blkpm;
    sys_pkt_action_t pkt_action ;
    sys_dhcp_option82_act_t option82Action = DHCP_OPTION82_END;
    sys_nic_pkt_t *pNewPkt;
    uint8 macStr1[CAPA_MAC_STR_LEN];
    uint8 macStr2[CAPA_MAC_STR_LEN];

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(dhcpinfo);

    srcPort = pPkt->rx_tag.source_port;
    SYS_ERR_CHK(rsd_nic_pktVid_get(pPkt, &vid));

    dhcp_snooping_vlan_enable_get(vid, &vlan_enable);
    dhcp_snooping_port_trust_get(srcPort, &trust);

    pdhcp = (uint8 *)(pPkt->data + 42);

    if (pPkt->rx_tag.cvid_tagged)
        pdhcp += VLANID_OFFSET;

    if (pPkt->rx_tag.svid_tagged)
        pdhcp += VLANID_OFFSET;

    pDhcpMsg = (dhcpMessage *)pdhcp;

    DSDBG("vlan %u is %u, %s trust is %u", vid, vlan_enable, LPSTR(srcPort), trust);

    if (DISABLED == g_dsEnable)
    {
        g_portMib[srcPort].dhcpInvalidDrop++;
        SYS_MEM_CLEAR(macStr1);
        SYS_MEM_CLEAR(macStr2);
        SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
        SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_GLOBAL_DROP, vid, LPSTR(srcPort), macStr1, macStr2);
        SYS_ERR_CHK(rsd_nic_pkt_free(pPkt));

        return SYS_ERR_OK;
    }

    if (FALSE == rsd_nic_isPortSTPForwad_ret(srcPort, vid))
    {
        SYS_MEM_CLEAR(macStr1);
        SYS_MEM_CLEAR(macStr2);
        SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
        SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_STP_DROP, vid, LPSTR(srcPort), macStr1, macStr2);
        SYS_ERR_CHK(rsd_nic_pkt_free(pPkt));

        return SYS_ERR_OK;
    }

    if (osal_memcmp(pPkt->data, &g_sysMac, CAPA_MAC_BYTE_NUM) == 0)
    {
        rsd_nic_pkt_free(pPkt);

        return SYS_ERR_OK;
    }

    if (DISABLED == vlan_enable)
    {
        SYS_MEM_CLEAR(macStr1);
        SYS_MEM_CLEAR(macStr2);
        SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
        SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_VLAN_FORWARD, vid, LPSTR(srcPort), macStr1, macStr2);
        SYS_ERR_CHK(rsd_nic_pktByDmacBlkPort_tx(pPkt, SYS_CPU_PORT_PRIORITY_DHCP, vid, pPkt->rx_tag.source_port));

        return SYS_ERR_OK;
    }

    if ((pPkt->length + SYS_DHCP_O82_MAX_LEN) > 1522)
    {
        g_portMib[srcPort].dhcpInvalidDrop++;
        SYS_MEM_CLEAR(macStr1);
        SYS_MEM_CLEAR(macStr2);
        SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
        SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, vid, LPSTR(srcPort), macStr1, macStr2);
        rsd_nic_pkt_free(pPkt);

        return SYS_ERR_OK;
    }

    if (SYS_ERR_OK != rsd_nic_pkt_alloc(pPkt->length + SYS_DHCP_O82_MAX_LEN, &pNewPkt))
    {
        g_portMib[srcPort].dhcpInvalidDrop++;
        SYS_MEM_CLEAR(macStr1);
        SYS_MEM_CLEAR(macStr2);
        SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
        SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_ALLOCATE_FAIL, vid, LPSTR(srcPort), macStr1, macStr2);
        rsd_nic_pkt_free(pPkt);

        return SYS_ERR_OK;
    }

    osal_memcpy(pNewPkt->data, pPkt->data, pPkt->length);
    pNewPkt->rx_tag = pPkt->rx_tag;
    pNewPkt->tail += pPkt->length;
    pNewPkt->length = pPkt->length;

    rsd_nic_pkt_free(pPkt);

    pdhcp = (uint8 *)(pNewPkt->data + 42);

    if (pNewPkt->rx_tag.cvid_tagged)
        pdhcp += VLANID_OFFSET;

    if (pNewPkt->rx_tag.svid_tagged)
        pdhcp += VLANID_OFFSET;

    pDhcpMsg = (dhcpMessage *)pdhcp;

    offset = 0;
    max_option_len = pDhcpMsg->options - pNewPkt->data;

    if (max_option_len <= 0)
    {
        g_portMib[srcPort].dhcpInvalidDrop++;
        SYS_MEM_CLEAR(macStr1);
        SYS_MEM_CLEAR(macStr2);
        SYS_ERR_CHK(sys_util_mac2Str(pNewPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
        SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, vid, LPSTR(srcPort), macStr1, macStr2);
        rsd_nic_pkt_free(pNewPkt);

        return SYS_ERR_OK;
    }

    while(1)
    {
        if (offset > (pNewPkt->length - max_option_len))
        {
            DSDBG("Drop1 invalid packet offset %u over max option len %u!", offset, (pNewPkt->length - max_option_len));
            g_portMib[srcPort].dhcpInvalidDrop++;
            SYS_MEM_CLEAR(macStr1);
            SYS_MEM_CLEAR(macStr2);
            SYS_ERR_CHK(sys_util_mac2Str(pNewPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
            SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
            SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, vid, LPSTR(srcPort), macStr1, macStr2);
            rsd_nic_pkt_free(pNewPkt);

            return SYS_ERR_OK;
        }

        option = *(pDhcpMsg->options + offset);

        if(0x00 == option)
        {
            offset++;
            continue;
        }

        if (53 == option)
        {
            dhcpinfo.opcode = *(pDhcpMsg->options + offset + 2);
            isOp53Found = TRUE;
        }
        else if (51 == option) 
        {
            dhcpinfo.leaseTime = osal_ntohl(*(uint32*)(pDhcpMsg->options + offset + 2));
        }
        else if (82 == option)
        {
            if (!IS_LOGIC_PORTMASK_PORTSET(g_trustPM, srcPort))
            {
                dhcp_option82_action_get(srcPort, &option82Action);

                if (DHCP_OPTION82_DROP == option82Action)
                {
                    g_portMib[srcPort].dhcpUntrustWithOption82Drop++;
                    SYS_MEM_CLEAR(macStr1);
                    SYS_MEM_CLEAR(macStr2);
                    SYS_ERR_CHK(sys_util_mac2Str(pNewPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
                    SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
                    SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION82_DROP, vid, LPSTR(srcPort), macStr1, macStr2);
                    rsd_nic_pkt_free(pNewPkt);

                    return SYS_ERR_OK;
                }
            }
        }
        else if (*(pDhcpMsg->options + offset) == 0xFF) 
        {
            if (offset > pNewPkt->length)
            {
                g_portMib[srcPort].dhcpInvalidDrop++;
                SYS_MEM_CLEAR(macStr1);
                SYS_MEM_CLEAR(macStr2);
                SYS_ERR_CHK(sys_util_mac2Str(pNewPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
                SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
                SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, vid, LPSTR(srcPort), macStr1, macStr2);
                rsd_nic_pkt_free(pNewPkt);

                return SYS_ERR_OK;
            }
            else
            {
                break;
            }
        }

        if ((offset + *(pDhcpMsg->options + offset + 1)) > (pNewPkt->length - max_option_len))
        {
            DSDBG("Drop1 invalid packet offset %u over max option len %u!", (offset + *(pDhcpMsg->options + offset + 1)), (pNewPkt->length - max_option_len));
            g_portMib[srcPort].dhcpInvalidDrop++;
            SYS_MEM_CLEAR(macStr1);
            SYS_MEM_CLEAR(macStr2);
            SYS_ERR_CHK(sys_util_mac2Str(pNewPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
            SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
            SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, vid, LPSTR(srcPort), macStr1, macStr2);
            rsd_nic_pkt_free(pNewPkt);

            return SYS_ERR_OK;
        }

        offset += *(pDhcpMsg->options + offset + 1);
        offset += 2;
    }

    dhcpinfo.port = srcPort;
    dhcpinfo.vid = vid;

    if (FALSE == isOp53Found)
    {
        DSDBG("DHCP pkt %s vid %u op 53 not found drop", LPSTR(dhcpinfo.port), dhcpinfo.vid);
        g_portMib[srcPort].dhcpInvalidDrop++;
        SYS_MEM_CLEAR(macStr1);
        SYS_MEM_CLEAR(macStr2);
        SYS_ERR_CHK(sys_util_mac2Str(pNewPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
        SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, vid, LPSTR(srcPort), macStr1, macStr2);
        rsd_nic_pkt_free(pNewPkt);

        return SYS_ERR_OK;
    }

    if (dhcpinfo.opcode > DHCP_INFO)
    {
        DSDBG("DHCP pkt %s vid %u op %u is invalid value", LPSTR(dhcpinfo.port), dhcpinfo.vid, dhcpinfo.opcode);
        g_portMib[srcPort].dhcpInvalidDrop++;
        SYS_MEM_CLEAR(macStr1);
        SYS_MEM_CLEAR(macStr2);
        SYS_ERR_CHK(sys_util_mac2Str(pNewPkt->data + CAPA_MAC_BYTE_NUM, macStr1));
        SYS_ERR_CHK(sys_util_mac2Str(pDhcpMsg->chaddr, macStr2));
        SYS_LOG(LOG_CAT_DHCP_SNOOPING, LOG_DHCP_SNOOPING_OPTION_INVALID, vid, LPSTR(srcPort), macStr1, macStr2);
        rsd_nic_pkt_free(pNewPkt);

        return SYS_ERR_OK;
    }

    DSDBG("DHCP pkt %s vid %u op %u", LPSTR(dhcpinfo.port), dhcpinfo.vid, dhcpinfo.opcode);

    pkt_action = _snooping_process(pNewPkt, pDhcpMsg, &dhcpinfo);

    if (SYS_PKT_ACTION_FORWARD == pkt_action)
        pkt_action = _option82_process(pNewPkt, pDhcpMsg, &dhcpinfo);

    if (SYS_PKT_ACTION_FORWARD == pkt_action)
    {
        g_portMib[srcPort].dhcpForward++;

        LOGIC_PORTMASK_CLEAR_ALL(blkpm);

        if ((DHCP_OFFER != dhcpinfo.opcode) && (DHCP_ACK != dhcpinfo.opcode) && (DHCP_NACK != dhcpinfo.opcode))
        {
            LOGIC_PORTMASK_SET_ALL(blkpm);
            LOGIC_PORTMASK_XOR(blkpm, blkpm, g_trustPM);
        }

        LOGIC_PORTMASK_SET_PORT(blkpm, srcPort);
        LPM2STR(&blkpm, buf);
        DSDBG("Packet Block Ports = %s", buf);

        SYS_ERR_CHK(rsd_nic_pktByDmacBlkPM_tx(pNewPkt, SYS_CPU_PORT_PRIORITY_DHCP, vid, blkpm));

        return SYS_ERR_OK;
    }

    rsd_nic_pkt_free(pNewPkt);

    return SYS_ERR_OK;
}
int32 dhcp_init(void)
{
    sys_logic_port_t port;

    SYS_MEM_CLEAR(g_portMib);
    SYS_MEM_CLEAR(g_port82Enable);
    SYS_MEM_CLEAR(g_port82Action);
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    SYS_MEM_CLEAR(g_port82Format);
    SYS_MEM_CLEAR(g_port82VlanCircuitID);
#endif
    SYS_MEM_CLEAR(g_82RemoteID);
    SYS_MEM_CLEAR(g_portDhcpRateLimitCnt);
    SYS_MEM_CLEAR(g_dhcpTransationPool);
    SYS_MEM_CLEAR(g_portDhcpCnt);

    VLANMASK_CLEAR_ALL(g_dsVlanEnabled);
    LOGIC_PORTMASK_CLEAR_ALL(g_trustPM);
    LOGIC_PORTMASK_CLEAR_ALL(g_chaddrChkPM);

    SYS_ERR_CHK(rsd_sys_macAddr_get(0, &g_sysMac));

    FOR_EACH_LOGIC_PORT(port)
    {
        g_portDhcpRateLimit[port] = BOND_RATE_UNLIMITED;
        g_portDhcpRateLimitCnt[port] = 0;
        g_portDhcpRateLimitTmp[port] = 0;
    }

    g_dhcpTransationSem = osal_sem_mutex_create();
    g_dhcpThread = osal_thread_create("DHCP RL Thread", 32768, DFLT_THREAD_PRI_DHCP_RL_TIMER, (void *)_dhcp_timer_thread, NULL);

    if ((osal_thread_t)NULL == g_dhcpThread)
        return SYS_ERR_FAILED;

    return SYS_ERR_OK;
}
int32 dhcp_exit(void)
{
    return SYS_ERR_OK;
}
int32 dhcp_option82_enable_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    DSDBG("dhcp snooping %s option82 set to %u", LPSTR(port), enable);

    g_port82Enable[port] = enable;

    return SYS_ERR_OK;
}
int32 dhcp_option82_enable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    *pEnable = g_port82Enable[port];

    return SYS_ERR_OK;
}
int32 dhcp_option82_action_set(sys_logic_port_t port, sys_dhcp_option82_act_t action)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((action >= DHCP_OPTION82_END), SYS_ERR_INPUT);

    DSDBG("dhcp snooping %s option82 allow untrust set to %u", LPSTR(port), action);

    g_port82Action[port] = action;

    return SYS_ERR_OK;
}
int32 dhcp_option82_action_get(sys_logic_port_t port, sys_dhcp_option82_act_t *pAction)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pAction), SYS_ERR_NULL_POINTER);

    *pAction = g_port82Action[port];

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID

int32 dhcp_option82_format_set(sys_logic_port_t port, sys_dhcp_option82_format_t format)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((format >= DHCP_OPTION82_FORMAT_END), SYS_ERR_INPUT);

    DSDBG("dhcp snooping %s option82 allow untrust set to %u", LPSTR(port), format);

    g_port82Format[port] = format;

    return SYS_ERR_OK;
}
int32 dhcp_option82_format_get(sys_logic_port_t port, sys_dhcp_option82_format_t *pFormat)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pFormat), SYS_ERR_NULL_POINTER);

    *pFormat = g_port82Format[port];

    return SYS_ERR_OK;
}
#endif
int32 dhcp_option82_remoteID_set(sys_dhcp_option82_id_t *pRemoteID)
{
    SYS_PARAM_CHK((NULL == pRemoteID), SYS_ERR_NULL_POINTER);

    DSDBG("dhcp snooping option82 remote id set to %s", pRemoteID->str);

    osal_memcpy(&g_82RemoteID, pRemoteID, sizeof(sys_dhcp_option82_id_t));

    return SYS_ERR_OK;
}
int32 dhcp_option82_remoteID_get(sys_dhcp_option82_id_t *pRemoteID)
{
    SYS_PARAM_CHK((NULL == pRemoteID), SYS_ERR_NULL_POINTER);

    osal_memcpy(pRemoteID, &g_82RemoteID, sizeof(sys_dhcp_option82_id_t));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID

int32 dhcp_option82_circuitID_set(sys_logic_port_t port, sys_dhcp_option82_circuit_id_t *pCircuitID)
{
    uint32 i = 0;
    int32 emptyIndex = -1;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCircuitID), SYS_ERR_NULL_POINTER);

    DSDBG("dhcp snooping %s vlan %u option82 circuit id set to %s",
            LPSTR(port), pCircuitID->vid, pCircuitID->str);

    if (0 == pCircuitID->vid)
    {
        if (0 == osal_strlen(pCircuitID->str))
            SYS_MEM_CLEAR(g_port82VlanCircuitID[port][SYS_DHCP_O82_CID_VLAN_MAX]);
        else
            osal_memcpy(&g_port82VlanCircuitID[port][SYS_DHCP_O82_CID_VLAN_MAX], pCircuitID, sizeof(sys_dhcp_option82_circuit_id_t));
    }
    else
    {
        if (0 == osal_strlen(pCircuitID->str))
        {
            for (i = 0 ; i < SYS_DHCP_O82_CID_VLAN_MAX ; i++)
            {
                if (g_port82VlanCircuitID[port][i].vid == pCircuitID->vid)
                {
                    SYS_MEM_CLEAR(g_port82VlanCircuitID[port][i]);

                    return SYS_ERR_OK;
                }
            }
        }
        else
        {
            for (i = 0 ; i < SYS_DHCP_O82_CID_VLAN_MAX ; i++)
            {
                if ((-1 == emptyIndex) &&  (0 == g_port82VlanCircuitID[port][i].vid))
                    emptyIndex = i;

                if (g_port82VlanCircuitID[port][i].vid == pCircuitID->vid)
                {
                    osal_memcpy(&g_port82VlanCircuitID[port][i], pCircuitID, sizeof(sys_dhcp_option82_circuit_id_t));
                    break;
                }
            }

            if (i == SYS_DHCP_O82_CID_VLAN_MAX)
            {
                if (-1 == emptyIndex)
                    return SYS_ERR_ENTRY_FULL;

                osal_memcpy(&g_port82VlanCircuitID[port][emptyIndex], pCircuitID, sizeof(sys_dhcp_option82_circuit_id_t));
            }
        }
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING

int32 dhcp_snooping_enable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    DSDBG("dhcp snooping set to %u", enable);

    if (enable == g_dsEnable)
        return SYS_ERR_OK;

    if (ENABLED == enable)
    {
        
        g_dsEnable = ENABLED;
        SYS_ERR_CHK(rsd_nic_handler_register(&l2gDHCPOfferACKHandler));
        SYS_ERR_CHK(rsd_nic_handler_register(&l2gDHCPReleaseHandler));
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_DHCP_CLIENT_SNOOPING, ENABLED));
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_DHCP_SERVER_SNOOPING, ENABLED));

        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_DHCP_SERVER_BCAST, DISABLED));
    }
    else
    {
        
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_DHCP_CLIENT_SNOOPING, DISABLED));
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_DHCP_SERVER_SNOOPING, DISABLED));
        SYS_ERR_CHK(rsd_nic_handler_unregister(&l2gDHCPOfferACKHandler));
        SYS_ERR_CHK(rsd_nic_handler_unregister(&l2gDHCPReleaseHandler));

        g_dsEnable = DISABLED;

#ifdef CONFIG_SYS_PROTO_ISG
        isg_bindingEntryDhcp_del(ISG_ENTRY_DEL_ALL, 0);
#endif

        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_DHCP_SERVER_BCAST, ENABLED));
    }

    return SYS_ERR_OK;
}
int32 dhcp_snooping_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);
    *pEnable = g_dsEnable;
    return SYS_ERR_OK;
} 
int32 dhcp_snooping_vlan_enable_set (sys_vid_t vid, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    DSDBG("dhcp snooping vlan %u set to %u", vid, enable);

    if (ENABLED == enable)
        VLANMASK_SET_VID(g_dsVlanEnabled, vid);
    else
    {
        VLANMASK_CLEAR_VID(g_dsVlanEnabled, vid);
#ifdef CONFIG_SYS_PROTO_ISG
        isg_bindingEntryDhcp_del(ISG_ENTRY_DEL_VID, vid);
#endif
    }

    return SYS_ERR_OK;
} 
int32 dhcp_snooping_vlan_enable_get (sys_vid_t vid, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    if (VLANMASK_IS_VIDSET(g_dsVlanEnabled, vid))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
} 
int32 dhcp_snooping_port_trust_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    DSDBG("dhcp snooping %s type set to %u", LPSTR(port), enable);

    if (ENABLED == enable)
        LOGIC_PORTMASK_SET_PORT(g_trustPM, port);
    else
        LOGIC_PORTMASK_CLEAR_PORT(g_trustPM, port);

    return SYS_ERR_OK;
}
int32 dhcp_snooping_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    if (IS_LOGIC_PORTMASK_PORTSET(g_trustPM, port))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}
int32 dhcp_snooping_portChaddrChk_set(sys_logic_port_t port, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    DSDBG("dhcp snooping %s chaddr check set to %u", LPSTR(port), enable);

    if (ENABLED == enable)
        LOGIC_PORTMASK_SET_PORT(g_chaddrChkPM, port);
    else
        LOGIC_PORTMASK_CLEAR_PORT(g_chaddrChkPM, port);

    return SYS_ERR_OK;
}
int32 dhcp_snooping_portChaddrChk_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    if (IS_LOGIC_PORTMASK_PORTSET(g_chaddrChkPM, port))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;

    return SYS_ERR_OK;
}
int32 dhcp_snooping_portMib_get(sys_logic_port_t port, sys_dhcp_mib_t *pCounter)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCounter), SYS_ERR_NULL_POINTER);

    osal_memcpy(pCounter, &g_portMib[port], sizeof(sys_dhcp_mib_t));

    return SYS_ERR_OK;
}
int32 dhcp_snooping_portMib_clear(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    osal_memset(&g_portMib[port], 0, sizeof(sys_dhcp_mib_t));

    return SYS_ERR_OK;
}
int32 dhcp_snooping_rateLimit_set(sys_logic_port_t port, uint32 rate)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    DSDBG("dhcp snooping %s rate limit set to %u pps", LPSTR(port), rate);

    g_portDhcpRateLimit[port] = rate;
    g_portDhcpRateLimitCnt[port] = rate;

    if (BOND_RATE_UNLIMITED == rate)
    {
        g_portDhcpRateLimitTmp[port] = rate;
    }
    else
    {
        if (rate > 100)
            g_portDhcpRateLimitTmp[port] = (rate * 3)/DHCP_RATE_TICK_PER_SEC;
        else if (rate > 50)
            g_portDhcpRateLimitTmp[port] = (rate * 5)/DHCP_RATE_TICK_PER_SEC;
        else
            g_portDhcpRateLimitTmp[port] = rate;
    }

    return SYS_ERR_OK;
}
#endif

