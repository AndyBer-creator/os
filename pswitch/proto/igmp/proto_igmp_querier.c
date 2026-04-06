#include <rsd/rsd_sys.h>
#include <rsd/rsd_nic.h>
#ifdef CONFIG_SYS_L3_ROUTE
#include <rsd/rsd_l3.h>
#endif

#include <igmp/proto_igmp.h>
#include <igmp/proto_igmp_querier.h>
#include <igmp/proto_igmp_tx.h>
#define IGMP_QUERY_PDU_LEN  46 
#define IGMP_QUERY_V3_PDU_LEN  50 

#define MLD_QUERY_V1_PDU_LEN  90
#define MLD_QUERY_V2_PDU_LEN  94

extern osal_mutex_t     igmp_sem_db;
extern sys_mac_t        sys_mac;
extern igmp_stats_t     igmp_stats[SYS_LOGICPORT_NUM_MAX];
extern mld_stats_t      mld_stats[SYS_LOGICPORT_NUM_MAX];
extern igmp_cfg_t       igmp_cfg;
extern mld_cfg_t         mld_cfg;
extern int8             gNetworkEnabled;

sys_ipv4_t              g_querier_ip;
sys_ipv6_t              g_querier_ip6;

igmp_querier_db_t       querier_db[MULTICAST_TYPE_END];
igmp_querier_entry_t    tmp_querier_array[MAX_ROUTER_VLAN - 1];

#ifdef CONFIG_SYS_L3_ROUTE
static int32 _mcast_querier_intfEvent_handler(uint32 event, char *pData)
{
    int32 i = 0;

    sys_ip_t emptyAddr;
    sys_l3_event_t *pEvent = NULL;
    sys_l3_ipv4Db_t *pIpv4Db = NULL;
    sys_l3_ipv6Db_t *pIpv6Db = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(emptyAddr);

    pEvent = (sys_l3_event_t *)pData;

    switch (event)
    {
        case SYS_NT_L3_INTF_IP_ADD:
        case SYS_NT_L3_INTF_IP_DEL:
        {
            FOR_EACH_L3_IPV4(i, pIpv4Db)
            {
                if (osal_memcmp(&pIpv4Db->ip.addr, &emptyAddr.v4, sizeof(sys_ipv4_t)))
                {
                    osal_memcpy(&g_querier_ip, &pIpv4Db->ip.addr, sizeof(sys_ipv4_t));
                    break;
                }
            }

            FOR_EACH_L3_IPV6(i, pIpv6Db)
            {
                if (osal_memcmp(&pIpv6Db->ip.addr, &emptyAddr.v6, sizeof(sys_ipv6_t)))
                {
                    osal_memcpy(&g_querier_ip6, &pIpv6Db->ip.addr, sizeof(sys_ipv6_t));
                    break;
                }
            }

            break;
        }

        default:
            return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
#else
static int32 _mcast_querier_sysEvent_handler(uint32 event, char *pData)
{
    sys_l3_ipv4_t ip;
    sys_ipv4_t emptyAddr;
    sys_system_event_t *pSysEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(ip);
    SYS_MEM_CLEAR(emptyAddr);

    pSysEvent = (sys_system_event_t *)pData;

    if (SYS_NT_SYS_MGMT_IP_CHG == event)
    {
        if (osal_memcmp(&pSysEvent->ip, &emptyAddr, sizeof(sys_ipv4_t)))
        {
            osal_memcpy(&g_querier_ip, &pSysEvent->ip, sizeof(sys_ipv4_t));
        }
        else
        {
            SYS_ERR_CHK(rsd_sys_mgmtIPv4Addr_get(&ip));
        }
    }

    return SYS_ERR_OK;
}
#endif

void mcast_code_convert(uint32 code,  uint8 *pResult)
{
    uint8 tmp;

    if (NULL == pResult )
        return ;

    if(code < 128)
    {
        *pResult = code;
    }
    else
    {
        for(tmp =15; tmp>7; tmp--)
            if((code) & (0x1<<tmp))
                break;
        *pResult = (((code)>>(tmp-4)) & 0xf) 
            |(((tmp-4-3)<<4) & 0x70 ) 
            | 0x80;    
     }
}

int32 mcast_querier_array_search(uint16 search, multicast_ipType_t ipType, uint16 *idx, igmp_querier_entry_t **entry)
{
    int low = 0;
    int mid;
    int high;
    igmp_querier_entry_t *sortedArray = NULL;

    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 || ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    if (idx == NULL || entry == NULL)
        return SYS_ERR_FAILED;

    high = querier_db[ipType].entry_num -1;
    sortedArray = querier_db[ipType].querier_entry;

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

int32 mcast_querier_array_ins(uint16 sortedIdx, multicast_ipType_t ipType, igmp_querier_entry_t *entry)
{
    if (entry == NULL)
        return SYS_ERR_FAILED;

    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 || ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    if (sortedIdx > querier_db[ipType].entry_num)
    {
        MCASTDBG("Error: sortedIdx > querier_db.entry_num");
        return SYS_ERR_FAILED;
    }
    if (querier_db[ipType].entry_num >= MAX_ROUTER_VLAN)
        return SYS_ERR_FAILED;

    osal_memcpy(tmp_querier_array, querier_db[ipType].querier_entry + sortedIdx, sizeof(igmp_querier_entry_t) * (querier_db[ipType].entry_num - sortedIdx));
    osal_memcpy(querier_db[ipType].querier_entry + sortedIdx, entry, sizeof(igmp_querier_entry_t));
    osal_memcpy(querier_db[ipType].querier_entry + sortedIdx + 1, tmp_querier_array, sizeof(igmp_querier_entry_t) * (querier_db[ipType].entry_num - sortedIdx));
    querier_db[ipType].entry_num++;

    return SYS_ERR_OK;
}

int32 mcast_querier_array_remove(uint16 sortedIdx, multicast_ipType_t ipType)
{

    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 || ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);

    if (sortedIdx >= querier_db[ipType].entry_num)
    {
        MCASTDBG("Error: sortedIdx >= querier_db.entry_num");
        return SYS_ERR_FAILED;
    }

    osal_memcpy(tmp_querier_array, querier_db[ipType].querier_entry + sortedIdx + 1, sizeof(igmp_querier_entry_t) * (querier_db[ipType].entry_num - sortedIdx));
    osal_memcpy(querier_db[ipType].querier_entry + sortedIdx, tmp_querier_array, sizeof(igmp_querier_entry_t) * (querier_db[ipType].entry_num - sortedIdx));
    querier_db[ipType].entry_num--;
    osal_memset(querier_db[ipType].querier_entry + querier_db[ipType].entry_num, 0, sizeof(igmp_querier_entry_t));

    return SYS_ERR_OK;
}

int32 mcast_querier_db_init(void)
{
    multicast_ipType_t ipType;

    for(ipType = MULTICAST_TYPE_IPV4 ; ipType < MULTICAST_TYPE_END; ipType++)
    {
        osal_memset(&querier_db[ipType], 0, sizeof(querier_db[ipType]));
    }
    return SYS_ERR_OK;
}

int32 mcast_querier_db_add(uint16 vid, multicast_ipType_t ipType)
{
    uint16              sortedIdx;
    igmp_querier_entry_t *entry = NULL;
    igmp_querier_entry_t newEntry;

    mcast_querier_array_search(vid, ipType, &sortedIdx, &entry);
    if (entry)
    {
        
    }
    else  
    {
        osal_memset(&newEntry, 0, sizeof(igmp_querier_entry_t));
        newEntry.vid = vid;
        newEntry.ipType =  ipType;
        return mcast_querier_array_ins(sortedIdx, ipType, &newEntry);
    }

    return SYS_ERR_OK;
}

int32 mcast_querier_db_get(uint16 vid,  multicast_ipType_t ipType, igmp_querier_entry_t **ppEntry)
{
    uint16              sortedIdx;

    SYS_PARAM_CHK(NULL == ppEntry, SYS_ERR_NULL_POINTER);

    mcast_querier_array_search(vid, ipType, &sortedIdx, ppEntry);

    return SYS_ERR_OK;
}

int32 mcast_querier_db_del(uint16 vid, multicast_ipType_t ipType)
{
    uint16          sortedIdx;
    igmp_querier_entry_t   *entry;

    mcast_querier_array_search(vid, ipType, &sortedIdx, &entry);   
    if (entry)    
    {
        return mcast_querier_array_remove(sortedIdx,ipType);
    }

    return SYS_ERR_FAILED;
}

void mcast_igmp_build_general_query (sys_nic_pkt_t * pkt, sys_vid_t vid, uint16 pktLen, uint8 version, uint32 queryEnable,uint32 gdaddr, uint8 query_type)
{
    uint8 igmpv3hd[IGMPV3_HEAD_LEN];  
    uint8 qqic;

    uint8 dst_mac[6]    = {0x01, 0x00, 0x5e, 0x00, 0x00, 0x01};
    uint16 ctag         = htons(0x8100);
    uint16 cvid;
    uint16 type         = htons(0x0800);

    uint8 lv            = htons(0x45);
    uint8 tos           = htons(0x00);
    uint16 id           = htons(0x0000);
    uint16 frag_off     = htons(0x0000);
    uint8 ttl           = htons(0x01);
    uint8 protocol      = htons(2);     
    uint16 csum         = htons(0x00);

    uint32 daddr        = htonl(0xE0000001L);

    uint8 itype         = htons(IGMP_HOST_MEMBERSHIP_QUERY);
        uint32 gda           = htonl(0x00000000L);
    iphdr_t *iph;
    igmp_hdr_t *igmph;
    uint32 saddr;
    uint16 tot_len;
    uint8 code;
    uint16 posite, igmpHdrCsm;

    uint8  options[4] = {0x94, 0x04, 0x00, 0x00};

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;

    if (IGMP_GENERAL_QUERY == query_type)
    {
        daddr =  htonl(0xE0000001L);
        gda    = htonl(0x00000000L);
    }
    else if(IGMP_SPECIFIC_GROUP_QUERY == query_type)
    {
        daddr = htonl(gdaddr);
        gda    = htonl(gdaddr);
        dst_mac[3] = (gdaddr >> 16) & 0x7f;
        dst_mac[4] = (gdaddr >> 8) & 0xff;
        dst_mac[5] =  gdaddr & 0xff;
    }

    mcast_vlan_db_get(vid,ipType, &pMcastVlan);
    if (NULL == pMcastVlan )
        return;

    cvid = (uint16)htons(vid);

    if(version == IGMP_QUERY_V3)
    {
        mcast_code_convert(pMcastVlan->responseTime * 10, &code);

        code = htons(code);
                        tot_len =  htons ((sizeof(iphdr_t))+ sizeof(igmp_hdr_t));
    }
    else
    {
        code = pMcastVlan->responseTime * 10;

        tot_len =  htons ((sizeof(iphdr_t)) + sizeof(igmp_hdr_t) - sizeof(iph->v3) - sizeof(igmph->v3));
    }

    if(ENABLED == queryEnable)
        saddr = htonl(g_querier_ip);
    else
        saddr = htonl(0x00);

    IGMP_FILL_QUERY(pkt, &dst_mac, 0, 6);
    IGMP_FILL_QUERY(pkt, &sys_mac, 6, 6);
    IGMP_FILL_QUERY(pkt, &ctag, 12, 2);
    IGMP_FILL_QUERY(pkt, &cvid, 14, 2);
    IGMP_FILL_QUERY(pkt, &type, 16, 2);

    iph = (iphdr_t *)(&pkt->data[18]);

    if(version == IGMP_QUERY_V2)
    {
        IGMP_FILL_QUERY(pkt, &lv, 18, 1);
    }
    else
    {
        lv = htons(0x46);
        IGMP_FILL_QUERY(pkt, &lv, 18, 1);
    }

    IGMP_FILL_QUERY(pkt, &tos, 19, 1);
    IGMP_FILL_QUERY(pkt, &tot_len, 20, 2);
    IGMP_FILL_QUERY(pkt, &id, 22, 2);

    if(version == IGMP_QUERY_V2)
    {
        IGMP_FILL_QUERY(pkt, &frag_off, 24, 2);
    }
    else
    {
        frag_off = htons(0x4000);
        IGMP_FILL_QUERY(pkt, &frag_off, 24, 2);
    }

    IGMP_FILL_QUERY(pkt, &ttl, 26, 1);
    IGMP_FILL_QUERY(pkt, &protocol, 27, 1);
    IGMP_FILL_QUERY(pkt, &csum, 28, 2);
    IGMP_FILL_QUERY(pkt, &saddr, 30, 4);
    IGMP_FILL_QUERY(pkt, &daddr, 34, 4);

    posite = 38;
    if (IGMP_QUERY_V2  == version)
    {
        
        rsd_nic_checksum_get((uint16 *)iph, sizeof(iphdr_t) - sizeof(iph->v3), &csum);
    }
    else
    {
        IGMP_FILL_QUERY(pkt, options, posite, 4);
        posite += 4;
        rsd_nic_checksum_get((uint16 *)iph, sizeof(iphdr_t), &csum);
    }

    csum = htons(csum);
    IGMP_FILL_QUERY(pkt, &csum, 28, 2);

    csum = htons(0x0000);
    igmph = (igmp_hdr_t *)(&pkt->data[posite]);

    IGMP_FILL_QUERY(pkt, &itype, posite, 1);
    posite += 1;
    IGMP_FILL_QUERY(pkt, &code, posite, 1);
    posite += 1;
    igmpHdrCsm = posite;
    IGMP_FILL_QUERY(pkt, &csum, posite, 2);
    posite += 2;
    IGMP_FILL_QUERY(pkt, &gda, posite, 4);
    posite += 4;

    if(IGMP_QUERY_V3 == version)
    {
        if (IGMP_GENERAL_QUERY == query_type)
        {
            mcast_code_convert(pMcastVlan->queryIntv, &qqic);
            igmpv3hd[0] = htons(pMcastVlan->robustnessVar);
        }
        else
        {
            mcast_code_convert(pMcastVlan->grpSpecificQueryIntv, &qqic);
            igmpv3hd[0] = htons(pMcastVlan->lastMmbrQueryCnt);
        }

        igmpv3hd[1] = htons(qqic);
        igmpv3hd[2] = htons(0x0);
        igmpv3hd[3] = htons(0x0);

        IGMP_FILL_QUERY(pkt, igmpv3hd, posite, 4);
    }

    if(IGMP_QUERY_V3 == version)
        tot_len = sizeof(igmp_hdr_t) ;
    else
        tot_len = sizeof(igmp_hdr_t)  - sizeof(igmph->v3);

    rsd_nic_checksum_get((uint16 *)igmph, tot_len, &csum);
    csum = htons(csum);

    IGMP_FILL_QUERY(pkt, &csum, igmpHdrCsm, 2);

    pkt->as_txtag = TRUE;
    pkt->length = pktLen;
    pkt->tail += pktLen;
}

void mcast_igmp_build_gs_query(sys_nic_pkt_t * pkt, sys_vid_t vid, uint16 pktLen, uint32 gdaddr, uint16 version)
{
    uint8 dst_mac[6]    = {0x01, 0x00, 0x5e, 0x00, 0x00, 0x01}; 
    uint16 ctag        = htons(0x8100);
    uint16 cvid;
    uint16 type         = htons(0x0800);

    uint8 lv            = htons(0x46);
    uint8 tos           = htons(0x00);
    uint16 tot_len ;

    uint16 id           = htons(0x0000);
    uint16 frag_off     = htons(0x0000);
    uint8 ttl           = htons(0x01);
    uint8 protocol      = htons(2);     
    uint16 csum         = htons(0x0000);

    uint32 daddr        = htonl(gdaddr);

    uint8 itype         = htons(IGMP_HOST_MEMBERSHIP_QUERY);

    uint8 code ;

    uint32 gda          = htonl(gdaddr);
    uint8  options[4] = {0x94, 0x04, 0x00, 0x00};
    uint8 igmpv3hd[IGMPV3_HEAD_LEN];  
    uint8 qqic;

    iphdr_t *iph;
    igmp_hdr_t *igmph;
    uint32 saddr;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    uint8  isMvrPacket = FALSE;

    #ifdef CONFIG_SYS_PROTO_MVR
    uint32 qryTime;
    mcast_groupAddr_t grpAddr;
    grpAddr.ipv4 = gdaddr;
    if (is_mvr_group_ret1(ipType, vid, &grpAddr, TRUE))
    {
        isMvrPacket = TRUE;
        mcast_mvr_queryTime_get(ipType, &qryTime);
        code = (uint8)htons(qryTime * 10);
        pkt->rx_tag.cvid_tagged = TRUE;
    }
    #endif

    if (!isMvrPacket
    #ifdef CONFIG_SYS_PROTO_MVR
        || (MVR_MODE_DYNAMIC == mcast_mvr_mode_ret(ipType))
    #endif
     )
    {
        mcast_vlan_db_get(vid, ipType, &pMcastVlan);
        if (NULL == pMcastVlan)
            return;
        #ifdef CONFIG_SYS_PROTO_MVR
        if (MVR_MODE_DYNAMIC == mcast_mvr_mode_ret(ipType))
            pkt->rx_tag.cvid_tagged = TRUE;
        #endif
    }

    cvid = (uint16)htons(vid);

    if(IGMP_QUERY_V3 == version)
    {
        mcast_code_convert(pMcastVlan->grpSpecificQueryIntv * 10, &code);
    }
    else
    {
        if (!isMvrPacket
        #ifdef CONFIG_SYS_PROTO_MVR
         || (MVR_MODE_DYNAMIC == mcast_mvr_mode_ret(ipType))
        #endif
         )
            code   = (uint8)htons(pMcastVlan->grpSpecificQueryIntv * 10);
    }

    tot_len = (uint16)htons(pktLen-18);  

    saddr = htonl(g_querier_ip);

    dst_mac[3] = (gdaddr >> 16) & 0x7f;
    dst_mac[4] = (gdaddr >> 8) & 0xff;
    dst_mac[5] =  gdaddr & 0xff;

    IGMP_FILL_QUERY(pkt, &dst_mac, 0, 6);
    IGMP_FILL_QUERY(pkt, &sys_mac, 6, 6);
    IGMP_FILL_QUERY(pkt, &ctag, 12, 2);
    IGMP_FILL_QUERY(pkt, &cvid, 14, 2);
    IGMP_FILL_QUERY(pkt, &type, 16, 2);

    iph = (iphdr_t *)(&pkt->data[18]);
    IGMP_FILL_QUERY(pkt, &lv, 18, 1);
    IGMP_FILL_QUERY(pkt, &tos, 19, 1);
    IGMP_FILL_QUERY(pkt, &tot_len, 20, 2);
    IGMP_FILL_QUERY(pkt, &id, 22, 2);
    IGMP_FILL_QUERY(pkt, &frag_off, 24, 2);
    IGMP_FILL_QUERY(pkt, &ttl, 26, 1);
    IGMP_FILL_QUERY(pkt, &protocol, 27, 1);
    IGMP_FILL_QUERY(pkt, &csum, 28, 2);
    IGMP_FILL_QUERY(pkt, &saddr, 30, 4);
    IGMP_FILL_QUERY(pkt, &daddr, 34, 4);
    IGMP_FILL_QUERY(pkt, &options, 38, 4);
        rsd_nic_checksum_get((uint16 *)iph, sizeof(iphdr_t), &csum);

    csum = htons(csum);
    IGMP_FILL_QUERY(pkt, &csum, 28, 2);

    igmph = (igmp_hdr_t *)(&pkt->data[42]);
    csum = 0;

    IGMP_FILL_QUERY(pkt, &itype, 42, 1);
    IGMP_FILL_QUERY(pkt, &code, 43, 1);
    IGMP_FILL_QUERY(pkt, &csum, 44, 2);
    IGMP_FILL_QUERY(pkt, &gda, 46, 4);

    if(IGMP_QUERY_V3 == version)
    {

        mcast_code_convert(pMcastVlan->grpSpecificQueryIntv, &qqic);
        igmpv3hd[0] = htons(pMcastVlan->lastMmbrQueryCnt);

        igmpv3hd[1] = htons(qqic);
        igmpv3hd[2] = htons(0x0);
        igmpv3hd[3] = htons(0x0);

        IGMP_FILL_QUERY(pkt, igmpv3hd, 50, 4);

        rsd_nic_checksum_get((uint16 *)igmph, sizeof(igmp_hdr_t), &csum);
    }
    else
    {
        rsd_nic_checksum_get((uint16 *)igmph, sizeof(igmp_hdr_t)-sizeof(igmph->v3), &csum);
    }
    csum = htons(csum);

    IGMP_FILL_QUERY(pkt, &csum, 44, 2);
    pkt->as_txtag = TRUE;
    pkt->length = pktLen;
    pkt->tail += pktLen;
}
void mcast_igmp_build_gss_query(sys_nic_pkt_t * pkt, sys_vid_t vid, uint32 gdaddr, uint32 *pSip, uint16 numSrc, uint16 pktlen)
{
    uint8 dst_mac[6] ; 
    uint16 ctag        = htons(0x8100);
    uint16 cvid;
    uint16 type         = htons(0x0800);
    uint8 lv            = htons(0x45);
    uint8 tos           = htons(0x00);
    uint16 tot_len      = htons (pktlen - 14);
    uint16 id           = htons(0x0000);
    uint16 frag_off     = htons(0x0000);
    uint8 ttl           = htons(0x01);
    uint8 protocol      = htons(2);     
    uint16 csum         = htons(0x0000);
    uint32 daddr        = htonl(gdaddr);
    uint8 itype         = htons(IGMP_HOST_MEMBERSHIP_QUERY);
    uint32 gda          = htonl(gdaddr);
    uint8 rsv               = htons(0);
    uint8 qqic               = htons(DFLT_IGMP_QUERY_INTV);
    uint16 srcNum         = htons(numSrc);
    iphdr_t *iph;
    igmp_hdr_t *igmph;
    uint32 saddr;
    uint32 sip;
    uint16 i;
    uint8 code;
    uint32 tmp=0;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;

    osal_memset(dst_mac, 0, sizeof(dst_mac));

    dst_mac[0] = 0x01;
    dst_mac[1] = 0x00;
    dst_mac[2] = 0x5e;
    dst_mac[3] = (gdaddr >>16 & 0x07f);
    dst_mac[4] = (gdaddr >>8 & 0x0ff);
    dst_mac[5] = (gdaddr & 0x0ff);

    saddr = htonl(g_querier_ip);

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);
    if (NULL == pMcastVlan)
        return;

    cvid = (uint16)htons(vid);

    if(pMcastVlan->grpSpecificQueryIntv * 10 < 128)
    {
        code = pMcastVlan->grpSpecificQueryIntv * 10;
    }
    else
    {
        for(tmp =15; tmp>7; tmp--)
            if((pMcastVlan->grpSpecificQueryIntv * 10) & (0x1<<tmp))
                break;
        code = (((pMcastVlan->grpSpecificQueryIntv * 10)>>(tmp-4)) & 0xf) 
            |(((tmp-4-3)<<4) & 0x70 ) 
            | 0x80;    
    }

    code = htons(code);

    IGMP_FILL_QUERY(pkt, &dst_mac, 0, 6);
    IGMP_FILL_QUERY(pkt, &sys_mac, 6, 6);
    IGMP_FILL_QUERY(pkt, &ctag, 12, 2);
    IGMP_FILL_QUERY(pkt, &cvid, 14, 2);
    IGMP_FILL_QUERY(pkt, &type, 16, 2);

    iph = (iphdr_t *)(&pkt->data[18]);
    IGMP_FILL_QUERY(pkt, &lv, 18, 1);
    IGMP_FILL_QUERY(pkt, &tos, 19, 1);
    IGMP_FILL_QUERY(pkt, &tot_len, 20, 2);
    IGMP_FILL_QUERY(pkt, &id, 22, 2);
    IGMP_FILL_QUERY(pkt, &frag_off, 24, 2);
    IGMP_FILL_QUERY(pkt, &ttl, 26, 1);
    IGMP_FILL_QUERY(pkt, &protocol, 27, 1);
    IGMP_FILL_QUERY(pkt, &csum, 28, 2);
    IGMP_FILL_QUERY(pkt, &saddr, 30, 4);
    IGMP_FILL_QUERY(pkt, &daddr, 34, 4);

    rsd_nic_checksum_get((uint16*)iph, sizeof(iphdr_t), &csum);
    csum = htons(csum);
    IGMP_FILL_QUERY(pkt, &csum, 28, 2);

    igmph = (igmp_hdr_t *)(&pkt->data[38]);
    csum = 0;

    IGMP_FILL_QUERY(pkt, &itype, 38, 1);
    IGMP_FILL_QUERY(pkt, &code, 39, 1);
    IGMP_FILL_QUERY(pkt, &csum, 40, 2);
    IGMP_FILL_QUERY(pkt, &gda, 42, 4);

    IGMP_FILL_QUERY(pkt, &rsv, 46, 1);
    IGMP_FILL_QUERY(pkt, &qqic, 47, 1);
    IGMP_FILL_QUERY(pkt, &srcNum, 48, 2);

    for(i = 0; i < numSrc; i++)
    {
        sip = ntohl(*(pSip + i));
        IGMP_FILL_QUERY(pkt, &sip, 50+4*i, 4);
    }

    rsd_nic_checksum_get((uint16*)igmph, pktlen - 38, &csum);
    csum = htons(csum);

    IGMP_FILL_QUERY(pkt, &csum, 40, 2);

    pkt->as_txtag = TRUE;
    pkt->length = pktlen;
    pkt->tail += pktlen;
}

void mcast_igmp_send_general_query(igmp_querier_entry_t* qryPtr, uint8 igmp_query_version)
{
    sys_nic_pkt_t *pkt;
    sys_logic_portmask_t zeroPmsk;
    int32 queryPduLen;
    int32 ret = -1;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t mcastVlanEntry;

    if (DISABLED == igmp_cfg.igmpsnp_enable)
        return;

    if(igmp_query_version == IGMP_QUERY_V3)
        queryPduLen =  IGMP_QUERY_PDU_LEN + IGMPV3_HEAD_LEN + IGMPV3_OPTIONS_LEN ;
    else
        queryPduLen =  IGMP_QUERY_PDU_LEN;

    osal_memset(&mcastVlanEntry, 0, sizeof(mcastVlanEntry));
    mcastVlanEntry.vid = qryPtr->vid;
    mcastVlanEntry.ipType = ipType;
    mcast_vlan_get(&mcastVlanEntry);
    if (qryPtr->vid != mcastVlanEntry.vid)
    {
        return;
    }

    osal_memset(&zeroPmsk, 0, sizeof(sys_logic_portmask_t));
    if ((ENABLED == mcastVlanEntry.enable) && (ENABLED == qryPtr->enabled))
    {
        #ifdef CONFIG_SYS_PROTO_MVR
        sys_vid_t mvrVid;
        sys_logic_portmask_t rPmsk;
        if (is_mcast_mvr_enable(ipType))
        {
            mcast_mvr_vlanId_get(ipType, &mvrVid);
            if (mvrVid != qryPtr->vid)
            {
                mcast_mvr_pmsk_get(ipType, MVR_TYPE_RECEIVER, &rPmsk);
                LOGIC_PORTMASK_OR(zeroPmsk, zeroPmsk, rPmsk);
            }
        }
        #endif
        if (SYS_ERR_OK == rsd_nic_pkt_alloc(queryPduLen, &pkt))
        {
            mcast_igmp_build_general_query(pkt, qryPtr->vid,queryPduLen, igmp_query_version, ENABLED,0,IGMP_GENERAL_QUERY);

            MCASTDBG("Send General Query in VLAN-%d", qryPtr->vid);

            #ifdef CONFIG_SYS_PROTO_MVR
            if (is_mcast_mvr_enable(ipType))
            {
                if (mvrVid == qryPtr->vid)
                {
                    mcast_mvr_pmsk_get(ipType, MVR_TYPE_RECEIVER, &rPmsk);
                    ret = mcast_mvr_tx(pkt, qryPtr->vid, pkt->length, &rPmsk);
                    if (SYS_ERR_OK == ret)
                        mcast_portMskStat_inc(ipType, qryPtr->vid, rPmsk, MCAST_STAT_GENQRY_TX);
                    ret = rsd_nic_pktBlkPM_flood(pkt, qryPtr->vid, zeroPmsk, SYS_CPU_PORT_PRIORITY_IGMP);
                }
                else
                {
                    ret = rsd_nic_pktBlkPM_flood(pkt, qryPtr->vid, zeroPmsk, SYS_CPU_PORT_PRIORITY_IGMP);
                }
            }
            else
            {
                ret = rsd_nic_pktBlkPM_flood(pkt, qryPtr->vid, zeroPmsk, SYS_CPU_PORT_PRIORITY_IGMP);
            }
            #else
            ret = rsd_nic_pktBlkPM_flood(pkt, qryPtr->vid, zeroPmsk, SYS_CPU_PORT_PRIORITY_IGMP);
           #endif
           if (SYS_ERR_OK == ret)
           {
                mcast_blockPortMskStat_inc(ipType, qryPtr->vid, zeroPmsk, MCAST_STAT_GENQRY_TX);

                mcastVlanEntry.operRespTime = mcastVlanEntry.responseTime;
                mcastVlanEntry.operQueryIntv = mcastVlanEntry.queryIntv;
                mcastVlanEntry.operRobNessVar = mcastVlanEntry.robustnessVar;
               
                mcast_vlan_set(&mcastVlanEntry);
           }
        }
        else
        {
            MCASTDBG("igmp_snooping_send_general_query: dev_alloc_skb() failed");
            return;
        }
    }
}

void mcast_igmp_send_grp_specific_query(uint16 vid, uint32 dip, sys_logic_port_t port)
{
    int32   ret = -1;
    sys_nic_pkt_t *pkt;
    sys_logic_portmask_t txPmsk;
    uint16 pktlen = 0;
    igmp_querier_entry_t *pEntry = NULL;
    uint16 sortedIdx;
    igmp_group_entry_t *groupHead = NULL,  groupEntry;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t mcastVlanEntry;
    #ifdef CONFIG_SYS_PROTO_MVR
    sys_logic_portmask_t rPmsk;
    #endif

    LOGIC_PORTMASK_CLEAR_ALL(txPmsk);
    LOGIC_PORTMASK_SET_PORT(txPmsk, port);

    mcast_querier_db_get(vid, ipType, &pEntry);
    if (NULL == pEntry)
        return;

    if (IGMP_QUERY_V2 == pEntry->version)
        pktlen = IGMP_QUERY_PDU_LEN + IGMPV3_OPTIONS_LEN;
    else
        pktlen = IGMP_QUERY_V3_PDU_LEN + IGMPV3_OPTIONS_LEN;

    mcast_build_groupEntryKey(vid, ipType, dip, NULL, &groupEntry);
    mcast_group_sortedArray_search(mcast_group_sortKey_ret(&groupEntry), &sortedIdx, &groupHead);   
    if (NULL == groupHead)
    {
        return ;
    }

    if (!IS_LOGIC_PORTMASK_PORTSET(groupHead->mbr, port))
        return;

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(pktlen, &pkt))
    {
        mcast_igmp_build_gs_query(pkt, vid,pktlen, dip, pEntry->version);
        #ifdef CONFIG_SYS_PROTO_MVR
        if (is_mvr_group_ret2(&groupEntry))
        {
            mcast_mvr_pmsk_get(ipType, MVR_TYPE_RECEIVER, &rPmsk);
            if (IS_LOGIC_PORTMASK_PORTSET(rPmsk, port))
            {
                ret = mcast_mvr_tx(pkt, vid, pktlen, &txPmsk);
            }
            else
            {
                ret = mcast_snooping_tx(pkt, vid, pktlen, &txPmsk);
            }
        }
        else
        {
            ret = mcast_snooping_tx(pkt, vid, pktlen, &txPmsk);
        }
        #else
            ret = mcast_snooping_tx(pkt, vid, pktlen, &txPmsk);
        #endif

        if (ret)
            MCASTDBG("mcast_snooping_tx() failed!  ret:%d", ret);
        else
        {
            mcast_portMskStat_inc(ipType, vid,txPmsk, MCAST_STAT_SPEC_GROUP_QRY_TX);

            MCASTDBG("Send Group Specific Query ("IPADDR_PRINT") in VLAN-%d", IPADDR_PRINT_ARG(dip), vid);

            osal_memset(&mcastVlanEntry, 0, sizeof(mcastVlanEntry));
            mcastVlanEntry.vid = vid;
            mcastVlanEntry.ipType = ipType;
            mcast_vlan_get(&mcastVlanEntry);
            if (vid == mcastVlanEntry.vid)
            {
                mcastVlanEntry.operGsqueryIntv = mcastVlanEntry.grpSpecificQueryIntv;
                mcastVlanEntry.operLastMmbrQueryCnt = mcastVlanEntry.lastMmbrQueryCnt;
                mcast_vlan_set(&mcastVlanEntry);
            }
        }

        drv_nic_pkt_free(0, pkt);
    }
    else
    {
        MCASTDBG("igmp_snooping_send_general_query: dev_alloc_skb() failed");
        return;
    }
}

#ifdef CONFIG_SYS_PROTO_MVR
void mcast_igmp_mvr_send_specific_query(uint16 vid, uint32 dip, sys_logic_port_t port)
{
    int32   ret;
    sys_nic_pkt_t *pkt;
    sys_logic_portmask_t txPmsk;
    uint16 pktlen = 0;

    LOGIC_PORTMASK_CLEAR_ALL(txPmsk);
    LOGIC_PORTMASK_SET_PORT(txPmsk, port);

    pktlen = IGMP_QUERY_PDU_LEN + IGMPV3_OPTIONS_LEN;

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(pktlen, &pkt))
    {
        mcast_igmp_build_gs_query(pkt, vid,pktlen, dip, IGMP_QUERY_V2);

        ret = mcast_mvr_tx(pkt, vid, pktlen, &txPmsk);

        if (ret)
            MCASTDBG("mcast_mvr_tx() failed!  ret:%d", ret);
        else
        {
                        mcast_portMskStat_inc(MULTICAST_TYPE_IPV4,vid, txPmsk, MCAST_STAT_SPEC_GROUP_QRY_TX);
            MCASTDBG("MVR Send Group Specific Query ("IPADDR_PRINT") in VLAN-%d", IPADDR_PRINT_ARG(dip), vid);
        }

        drv_nic_pkt_free(0, pkt);
    }
    else
    {
        MCASTDBG("rsd_nic_pkt_alloc failed");
    }
}
#endif

void mcast_igmp_send_grp_src_specific_query(uint16 vid, uint32 dip, uint32 *sip, uint16 numSrc, sys_logic_port_t port)
{
    int ret;
    sys_nic_pkt_t *pkt;
    sys_logic_portmask_t txPmsk;
    uint16 pktLen = 0;
    int i;

    igmp_querier_entry_t *pEntry = NULL;
    igmp_router_entry_t *pRouterEntry = NULL;
    igmp_group_entry_t *pGroup = NULL, groupEntry;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t mcastVlanEntry;

    if (DISABLED == igmp_cfg.igmpsnp_enable)
        return;

    LOGIC_PORTMASK_CLEAR_ALL(txPmsk);

    mcast_querier_db_get(vid, ipType, &pEntry);
    if (NULL == pEntry)
        return;

    mcast_router_db_get(vid, ipType, &pRouterEntry);
    if (NULL != pRouterEntry)
    {
        LOGIC_PORTMASK_OR(txPmsk, txPmsk, pRouterEntry->router_pmsk);
    }

    mcast_build_groupEntryKey(vid, ipType, dip, NULL, &groupEntry);

    for(i = 0; i < numSrc; i++)
    {
        groupEntry.sip = sip[i];
        pGroup = mcast_group_get(&groupEntry);
        if (NULL != pGroup)
        {
            LOGIC_PORTMASK_OR(txPmsk, txPmsk, pGroup->mbr);
        }
    }

    if (IS_LOGIC_PORTMASK_CLEAR(txPmsk))
        return;

    pktLen =  IGMP_QUERY_PDU_LEN + 1 + 1  + 2 + numSrc * 4; 

    if(SYS_ERR_OK == rsd_nic_pkt_alloc(pktLen, &pkt))
    {
        mcast_igmp_build_gss_query(pkt, vid, dip, sip, numSrc, pktLen);

        ret = mcast_snooping_tx(pkt, vid, pktLen, &txPmsk);

        if (ret)
            MCASTDBG("mcast_snooping_tx() failed!  ret:%d", ret);
        else
        {
            
            mcast_portMskStat_inc(ipType, vid,txPmsk, MCAST_STAT_SPEC_GROUP_SRC_QRY_TX);
            MCASTDBG("Send Group Specific Source Specific Query ("IPADDR_PRINT") in VLAN-%d", IPADDR_PRINT_ARG(dip), vid);

            osal_memset(&mcastVlanEntry, 0, sizeof(mcastVlanEntry));
            mcastVlanEntry.vid = vid;
            mcastVlanEntry.ipType = ipType;
            mcast_vlan_get(&mcastVlanEntry);
            if (vid == mcastVlanEntry.vid)
            {
                mcastVlanEntry.operGsqueryIntv = mcastVlanEntry.grpSpecificQueryIntv;
                mcast_vlan_set(&mcastVlanEntry);
            }
        }

        drv_nic_pkt_free(0, pkt);
    }
    else
    {
        MCASTDBG("mcast_igmp_send_grp_src_specific_query: dev_alloc_skb() failed");
        return;
    }

}

int32 mcast_igmp_querier_check(uint16 vid, uint32 sip, uint8 qver)
{
    igmp_querier_entry_t *pEntry;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;
    mcast_vlan_entry_t *pMcastVlan = NULL;

    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    IGMP_DB_SEM_LOCK();

    mcast_vlan_db_get(vid, ipType, &pMcastVlan);

    if (NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    mcast_querier_db_get(vid, ipType, &pEntry);

    if (!pEntry)
    {
        IGMP_DB_SEM_UNLOCK();
        MCASTDBG("%s():%d  Warring! Querier for VLAN-%d doesn't exist!", __FUNCTION__, __LINE__, vid);
        return SYS_ERR_OK;
    }

    if (pEntry->enabled)
    {
        if ((pEntry->ip == 0 && g_querier_ip < sip)
            || (g_querier_ip < sip && g_querier_ip <= pEntry->ip))
        {
            
            pEntry->ip = g_querier_ip;
            pEntry->status = IGMP_QUERIER;
        }
        else
        {
            
            if ((pEntry->ip == 0 && sip <= g_querier_ip) || (sip < pEntry->ip))
                pEntry->ip = sip;

            pEntry->timer = pMcastVlan->otherQuerierPresentInterval;
            pEntry->status = IGMP_NON_QUERIER;
            pEntry->version = qver;
        }
    }

    return SYS_ERR_OK;
}

void mcast_igmp_querier_timer(void)
{
    uint16                  i, time;
    igmp_querier_entry_t    *sortedArray ;
    static uint32           cnt;

    multicast_ipType_t ipType;
    mcast_vlan_entry_t *pMcstVlan = NULL;

    if (++cnt == 30)
    {
        cnt = 0;
    }

    IGMP_DB_SEM_LOCK();
    for (ipType = MULTICAST_TYPE_IPV4; ipType < MULTICAST_TYPE_END; ipType++)
    {
        sortedArray = querier_db[ipType].querier_entry;

        for (i = 0; i < querier_db[ipType].entry_num; i++)
        {
            if (sortedArray[i].enabled)
            {
                mcast_vlan_db_get(sortedArray[i].vid, ipType, &pMcstVlan);
                if (NULL == pMcstVlan)
                    continue;

                if (gNetworkEnabled && sortedArray[i].startupQueryCnt <= pMcstVlan->robustnessVar)
                {
                    
                                        mcast_send_general_query(&sortedArray[i], sortedArray[i].version);
                    sortedArray[i].startupQueryCnt++;
                }

                time = sortedArray[i].timer;
                if (time != 0)
                {
                    if (time > PASS_SECONDS)
                    {
                        if (time > pMcstVlan->otherQuerierPresentInterval)      
                            sortedArray[i].timer = pMcstVlan->otherQuerierPresentInterval;

                        sortedArray[i].timer -= PASS_SECONDS;
                    }
                    else
                    {
                        
                                                mcast_send_general_query(&sortedArray[i], sortedArray[i].version);

                        sortedArray[i].ip = g_querier_ip;
                        sortedArray[i].timer = pMcstVlan->queryIntv;
                        sortedArray[i].status = IGMP_QUERIER;

                        osal_time_usleep(10 * 1000); 
                    }
                }

            }
        }
    }
    IGMP_DB_SEM_UNLOCK();
}

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
void mcast_mld_build_general_query (sys_nic_pkt_t * pkt, sys_vid_t vid, uint16 pktLen, uint8 version)
{
    uint8 dst_mac[6]    = {0x33, 0x33, 0x00, 0x00, 0x00, 0x01};
    uint16 ctag         = htons(0x8100);
    uint16 cvid;
    uint16 type         = htons(0x86dd);
    uint16 vtf           =  htons(0x6000);
    uint16 flowlabel   = htons(0x0000);
    uint16 tot_len;
    uint8  ipv6_nextHrd = htons(0x00);
    uint8  hopLimit           = htons(0x01);
    uint8  saddr[CAPA_IPV6_BYTE_NUM];
    uint8  daddr[CAPA_IPV6_BYTE_NUM] = {0xff,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
    uint8  hop_nextHrd =  htons(0x3a);
    uint8  hop_len         =    htons(0x0);
    uint8 routerAlert[4]    =  {0x05, 0x02, 0x00, 0x00};
    uint16 padn = htons(0x0100);
    uint8  reportType = htons(0x82);
    uint8  code = htons(0x0);
    uint16 mldHdrCsm = 0;
    uint16 max_response;
    uint16 rev = htons(0x0000);
    uint8  sflag_rob = 0x02;
    uint8  qqi = 0x14;
    uint16 numSrc = htons(0x0000);

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    uint8 bufcsm[32+4+4+28]; 
    uint16 offset = 0;
    uint16 mld_offset = 0;

#if 0
#ifdef CONFIG_SYS_IPV6
    sys_ipv6_addr_t ipv6Addr;
#endif
#endif

    mcast_vlan_db_get(vid,ipType, &pMcastVlan);
    if (NULL == pMcastVlan )
        return;

    if (MLD_QUERY_V1 == version)
    {
        tot_len = 32;
    }
    else
    {
        tot_len = 36;
    }

    tot_len = htons(tot_len);
    cvid = (uint16)htons(vid);
    max_response = pMcastVlan->responseTime * 1000;
    max_response = htons(max_response);
    osal_memset(bufcsm,0,sizeof(bufcsm));

    IGMP_FILL_QUERY(pkt, &dst_mac, offset, 6);
    offset += 6;
    IGMP_FILL_QUERY(pkt, &sys_mac, offset, 6);
    offset += 6;
    IGMP_FILL_QUERY(pkt, &ctag, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &cvid, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &type, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &vtf, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &flowlabel, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &tot_len, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &ipv6_nextHrd, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, &hopLimit, offset, 1);
    offset += 1;

    osal_memset(saddr,0,sizeof(saddr));
#if 0
#ifdef CONFIG_SYS_IPV6
    rsd_sys_ipv6Addr_get(&ipv6Addr);
    osal_memcpy(saddr, ipv6Addr.octet,IPV6_ADDR_LEN);
#endif
#endif
    IGMP_FILL_QUERY(pkt, saddr, offset, 16);
    offset += 16;
    IGMP_FILL_QUERY(pkt, daddr, offset, 16);
    offset += 16;
    IGMP_FILL_QUERY(pkt, &hop_nextHrd, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, &hop_len, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, routerAlert, offset, 4);
    offset += 4;
    IGMP_FILL_QUERY(pkt, &padn, offset, 2);
    offset += 2;
    mld_offset = offset;
    IGMP_FILL_QUERY(pkt, &reportType, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, &code, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, &mldHdrCsm, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &max_response, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &rev, offset, 2);
    offset += 2;

    osal_memset(saddr,0,sizeof(saddr));
    IGMP_FILL_QUERY(pkt, saddr, offset, 16);
    offset += 16;

    osal_memcpy(bufcsm, &pkt->data[26], 32);
    if (MLD_QUERY_V2 == version)
    {
        sflag_rob = (pMcastVlan->robustnessVar) & 0x7;
        mcast_code_convert(pMcastVlan->queryIntv,&qqi);

        IGMP_FILL_QUERY(pkt, &sflag_rob, offset, 1);
        offset += 1;
        IGMP_FILL_QUERY(pkt, &qqi, offset, 1);
        offset += 1;
        IGMP_FILL_QUERY(pkt, &numSrc, offset, 2);
        offset += 2;

        bufcsm[35] = 28;
        bufcsm[39] = 0x3a;
        osal_memcpy(&bufcsm[40], &pkt->data[mld_offset], 28);
        rsd_nic_checksum_get((uint16 *)bufcsm,sizeof(bufcsm),&mldHdrCsm);
    }
    else
    {
        bufcsm[35] = 24;
        bufcsm[39] = 0x3a;
        osal_memcpy(&bufcsm[40], &pkt->data[mld_offset], 24);
        rsd_nic_checksum_get((uint16 *)bufcsm,sizeof(bufcsm)-4,&mldHdrCsm);
    }

    pkt->as_txtag = TRUE;
    pkt->length = pktLen;
    pkt->tail += pktLen;
}
void mcast_mld_build_gs_query(sys_nic_pkt_t * pkt, sys_vid_t vid, uint16 pktLen, uint8 *pIpv6Addr, uint16 version)
{
    uint8 dst_mac[6]    = {0x33, 0x33, 0x00, 0x00, 0x00, 0x00};
    uint16 ctag         = htons(0x8100);
    uint16 cvid;
    uint16 type         = htons(0x86dd);
    uint16 vtf           =  htons(0x6000);
    uint16 flowlabel   = htons(0x0000);
    uint16 tot_len;
    uint8  ipv6_nextHrd = htons(0x00);
    uint8  hopLimit           = htons(0x01);
    uint8  saddr[CAPA_IPV6_BYTE_NUM];
    uint8  daddr[CAPA_IPV6_BYTE_NUM];
    uint8  hop_nextHrd =  htons(0x3a);
    uint8  hop_len         =    htons(0x0);
    uint8 routerAlert[4]    =  {0x05, 0x02, 0x00, 0x00};
    uint16 padn = htons(0x0100);
    uint8  reportType = htons(0x82);
    uint8  code = htons(0x0);
    uint16 mldHdrCsm = 0;
    uint16 max_response;
    uint16 rev = htons(0x0000);
    uint8  sflag_rob = 0x02;
    uint8  qqi = 0x14;
    uint16 numSrc = htons(0x0000);

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;
    mcast_vlan_entry_t *pMcastVlan = NULL;
    uint8 bufcsm[32+4+4+28]; 
    uint16 offset = 0;
    uint16 mld_offset = 0;
#if 0
#ifdef CONFIG_SYS_IPV6
    sys_ipv6_addr_t ipv6Addr;
#endif
#endif

    mcast_vlan_db_get(vid,ipType, &pMcastVlan);
    if (NULL == pMcastVlan )
        return;

    if (MLD_QUERY_V1 == version)
    {
        tot_len = 32;
    }
    else
    {
        tot_len = 36;
    }

    tot_len = htons(tot_len);
    cvid = (uint16)htons(vid);
    max_response = pMcastVlan->responseTime * 1000;
    max_response = htons(max_response);
    osal_memset(bufcsm,0,sizeof(bufcsm));

    osal_memcpy(dst_mac + 2, pIpv6Addr+12, 4);
    osal_memcpy(daddr, pIpv6Addr, IPV6_ADDR_LEN);

    IGMP_FILL_QUERY(pkt, &dst_mac, offset, 6);
    offset += 6;
    IGMP_FILL_QUERY(pkt, &sys_mac, offset, 6);
    offset += 6;
    IGMP_FILL_QUERY(pkt, &ctag, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &cvid, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &type, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &vtf, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &flowlabel, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &tot_len, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &ipv6_nextHrd, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, &hopLimit, offset, 1);
    offset += 1;

    osal_memset(saddr,0,sizeof(saddr));
#if 0
#ifdef CONFIG_SYS_IPV6
    rsd_sys_ipv6Addr_get(&ipv6Addr);
    osal_memcpy(saddr, ipv6Addr.octet,IPV6_ADDR_LEN);
#endif
#endif
    IGMP_FILL_QUERY(pkt, saddr, offset, 16);
    offset += 16;
    IGMP_FILL_QUERY(pkt, daddr, offset, 16);
    offset += 16;
    IGMP_FILL_QUERY(pkt, &hop_nextHrd, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, &hop_len, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, routerAlert, offset, 4);
    offset += 4;
    IGMP_FILL_QUERY(pkt, &padn, offset, 2);
    offset += 2;
    mld_offset = offset;
    IGMP_FILL_QUERY(pkt, &reportType, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, &code, offset, 1);
    offset += 1;
    IGMP_FILL_QUERY(pkt, &mldHdrCsm, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &max_response, offset, 2);
    offset += 2;
    IGMP_FILL_QUERY(pkt, &rev, offset, 2);
    offset += 2;

    osal_memset(saddr,0,sizeof(saddr));
    IGMP_FILL_QUERY(pkt, saddr, offset, 16);
    offset += 16;

    osal_memcpy(bufcsm, &pkt->data[26], 32);
    if (MLD_QUERY_V2 == version)
    {
        sflag_rob = (pMcastVlan->lastMmbrQueryCnt) & 0x7;
        mcast_code_convert(pMcastVlan->grpSpecificQueryIntv,&qqi);

        IGMP_FILL_QUERY(pkt, &sflag_rob, offset, 1);
        offset += 1;
        IGMP_FILL_QUERY(pkt, &qqi, offset, 1);
        offset += 1;
        IGMP_FILL_QUERY(pkt, &numSrc, offset, 2);
        offset += 2;

        bufcsm[35] = 28;
        bufcsm[39] = 0x3a;
        osal_memcpy(&bufcsm[40], &pkt->data[mld_offset], 28);
        rsd_nic_checksum_get((uint16 *)bufcsm,sizeof(bufcsm),&mldHdrCsm);
    }
    else
    {
        bufcsm[35] = 24;
        bufcsm[39] = 0x3a;
        osal_memcpy(&bufcsm[40], &pkt->data[mld_offset], 24);
        rsd_nic_checksum_get((uint16 *)bufcsm,sizeof(bufcsm)-4,&mldHdrCsm);
    }

    pkt->as_txtag = TRUE;
    pkt->length = pktLen;
    pkt->tail += pktLen;
}

void mcast_mld_send_general_query(igmp_querier_entry_t* qryPtr, uint8 query_version)
{
    sys_nic_pkt_t *pkt;
    sys_logic_portmask_t zeroPmsk;
    int32 queryPduLen;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;
    mcast_vlan_entry_t mcastVlanEntry;

    if (DISABLED == mld_cfg.mldsnp_enable)
        return;

    if(query_version == MLD_QUERY_V1)
        queryPduLen =  MLD_QUERY_V1_PDU_LEN;
    else
        queryPduLen =  MLD_QUERY_V2_PDU_LEN;

    osal_memset(&mcastVlanEntry, 0, sizeof(mcastVlanEntry));
    mcastVlanEntry.vid = qryPtr->vid;
    mcastVlanEntry.ipType = ipType;
    mcast_vlan_get(&mcastVlanEntry);
    if (qryPtr->vid != mcastVlanEntry.vid)
    {
        return;
    }

    osal_memset(&zeroPmsk, 0, sizeof(sys_logic_portmask_t));
    if ((ENABLED == mcastVlanEntry.enable) && (ENABLED == qryPtr->enabled))
    {
        if (SYS_ERR_OK == rsd_nic_pkt_alloc(queryPduLen, &pkt))
        {
            mcast_mld_build_general_query(pkt, qryPtr->vid,queryPduLen, query_version);

            MCASTDBG("Send MLD General Query in VLAN-%d", qryPtr->vid);

            if (SYS_ERR_OK == rsd_nic_pktBlkPM_flood(pkt, qryPtr->vid, zeroPmsk, SYS_CPU_PORT_PRIORITY_MLD))
            {
                              mcast_blockPortMskStat_inc(ipType, qryPtr->vid, zeroPmsk, MCAST_STAT_GENQRY_TX);

                mcastVlanEntry.operRespTime = mcastVlanEntry.responseTime;
                mcastVlanEntry.operQueryIntv = mcastVlanEntry.queryIntv;
                mcastVlanEntry.operRobNessVar = mcastVlanEntry.robustnessVar;
                mcastVlanEntry.operLastMmbrQueryCnt = mcastVlanEntry.robustnessVar;
                mcast_vlan_set(&mcastVlanEntry);
           }
        }
        else
        {
            MCASTDBG("mcast_mld_send_general_query: dev_alloc_skb() failed");
            return;
        }
    }
}

void mcast_mld_send_grp_specific_query(igmp_group_entry_t *pGroup, sys_logic_port_t port)
{
    int32   ret;
    sys_nic_pkt_t *pkt;
    sys_logic_portmask_t txPmsk;
    uint16 pktlen = 0;
    igmp_querier_entry_t *pEntry = NULL;
        uint16 sortedIdx;
    igmp_group_entry_t *groupHead = NULL;
    igmp_router_entry_t *pRouterEntry = NULL;

    multicast_ipType_t ipType = MULTICAST_TYPE_IPV6;
    mcast_vlan_entry_t mcastVlanEntry;

    LOGIC_PORTMASK_CLEAR_ALL(txPmsk);

    mcast_querier_db_get(pGroup->vid, ipType, &pEntry);
    if (NULL == pEntry)
        return;

    if (MLD_QUERY_V1 == pEntry->version)
        pktlen = MLD_QUERY_V1_PDU_LEN;
    else
        pktlen = MLD_QUERY_V2_PDU_LEN;

    mcast_router_db_get(pGroup->vid, ipType, &pRouterEntry);
    if (NULL != pRouterEntry)
    {
        LOGIC_PORTMASK_OR(txPmsk,txPmsk,pRouterEntry->router_pmsk);
    }

    mcast_group_sortedArray_search(mcast_group_sortKey_ret(pGroup), &sortedIdx, &groupHead);   
    if (NULL != groupHead)
    {
        LOGIC_PORTMASK_OR(txPmsk, txPmsk, groupHead->mbr);
    }

#if 0
    mcast_igmp_operVersion_get(&version);

    if (IGMP_VERSION_V3_FULL == version)
    {
        pGroupEntry = groupHead;
        while(pGroupEntry)
        {
            LOGIC_PORTMASK_OR(txPmsk,txPmsk,pGroupEntry->mbr);
            pGroupEntry = pGroupEntry->next_subgroup;
        }
    }
#endif

    if (IS_LOGIC_PORTMASK_CLEAR(txPmsk))
        return;

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(pktlen, &pkt))
    {
        mcast_mld_build_gs_query(pkt, pGroup->vid,pktlen, pGroup->dipv6.addr, pEntry->version);

        ret = mcast_snooping_tx(pkt, pGroup->vid, pktlen, &txPmsk);

        if (ret)
            MCASTDBG("mcast_snooping_tx() failed!  ret:%d", ret);
        else
        {
                        mcast_portMskStat_inc(ipType, pGroup->vid,txPmsk, MCAST_STAT_SPEC_GROUP_QRY_TX);

            MCASTDBG("Send Group Specific MLD Query ("IPADDR_PRINT") in VLAN-%d", IPADDR_PRINT_ARG(pGroup->dip), pGroup->vid);

            osal_memset(&mcastVlanEntry, 0, sizeof(mcastVlanEntry));
            mcastVlanEntry.vid = pGroup->vid;
            mcastVlanEntry.ipType = ipType;
            mcast_vlan_get(&mcastVlanEntry);
            if (pGroup->vid == mcastVlanEntry.vid)
            {
                mcastVlanEntry.operGsqueryIntv = mcastVlanEntry.grpSpecificQueryIntv;
                mcast_vlan_set(&mcastVlanEntry);
            }
        }

        drv_nic_pkt_free(0, pkt);
    }
    else
    {
        MCASTDBG("igmp_snooping_send_general_query: dev_alloc_skb() failed");
        return;
    }
}
#endif

void mcast_send_general_query( igmp_querier_entry_t *qryPtr, uint8 version)
{
    if (NULL == qryPtr)
        return ;

    if (MULTICAST_TYPE_IPV4 == qryPtr->ipType)
        mcast_igmp_send_general_query(qryPtr, version);
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else
        mcast_mld_send_general_query(qryPtr, version);
#endif

    mcast_suppreFlag_clear(qryPtr->vid, qryPtr->ipType);
}

void mcast_send_gs_query(igmp_group_entry_t *pGroup, sys_logic_port_t lport)
{
    if (NULL == pGroup)
        return;

    if (MULTICAST_TYPE_IPV4 == pGroup->ipType)
    {
        if (DISABLED == igmp_cfg.igmpsnp_enable)
            return;
        mcast_igmp_send_grp_specific_query(pGroup->vid, pGroup->dip, lport);
    }
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else
     {
        if (DISABLED == mld_cfg.mldsnp_enable)
            return;
        mcast_mld_send_grp_specific_query(pGroup, lport);
     }
#endif

    }

#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING_QUERIER

int32 mcast_igmp_querier_set(sys_igmp_querier_t *pQuerier)
{
    igmp_querier_entry_t *pEntry;

    mcast_vlan_entry_t *pMcastVlan = NULL;
    multicast_ipType_t ipType = MULTICAST_TYPE_IPV4;

    SYS_PARAM_CHK(NULL == pQuerier, SYS_ERR_NULL_POINTER);

    IGMP_DB_SEM_LOCK();

    mcast_vlan_db_get(pQuerier->vid, ipType, &pMcastVlan);

    if (NULL == pMcastVlan)
    {
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_FAILED;
    }

    mcast_querier_db_get(pQuerier->vid, ipType, &pEntry);

    if (!pEntry)
    {
        MCASTDBG("Warring! querier entry for VLAN-%d doesn't exist!", pQuerier->vid);
        IGMP_DB_SEM_UNLOCK();
        return SYS_ERR_OK;
    }

    if (pEntry->enabled == pQuerier->enabled)
    {
        if (ENABLED == pEntry->enabled)
        {
            if (IGMP_NON_QUERIER == pEntry->status)
            {
                 if (pEntry->version != pQuerier->version)
                 {
                     if ((ENABLED == pMcastVlan->enable) && (ENABLED == igmp_cfg.igmpsnp_enable))
                     {
                        IGMP_DB_SEM_UNLOCK();
                        return SYS_ERR_IGMP_QUERIER_CHANGE;
                     }
                     else
                        pEntry->version = pQuerier->version;
                 }
            }
            else
            {
                pEntry->version = pQuerier->version;
            }
        }

        IGMP_DB_SEM_UNLOCK();

        return SYS_ERR_OK;
    }

    pEntry->enabled = pQuerier->enabled;
    pEntry->version = pQuerier->version;

    if (pEntry->enabled)
    {
        if (ENABLED == pMcastVlan->enable)
            pEntry->status = IGMP_QUERIER;
        else
            pEntry->status = IGMP_NON_QUERIER;

        pEntry->timer = pMcastVlan->operQueryIntv / 4;
    }
    else
    {
        pEntry->status = IGMP_NON_QUERIER;
        pEntry->ip = 0;
    }

    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 mcast_igmp_querier_get(sys_igmp_querier_t *pQuerier)
{
    igmp_querier_entry_t *pEntry;

    IGMP_DB_SEM_LOCK();

    mcast_querier_db_get(pQuerier->vid, pQuerier->ipType, &pEntry);

    if (!pEntry)
    {
        pQuerier->enabled = 0;
        pQuerier->status = 0;

        MCASTDBG("Warring! querier entry for VLAN-%d doesn't exist!", pQuerier->vid);
        IGMP_DB_SEM_UNLOCK();

        return SYS_ERR_OK;
    }

    pQuerier->enabled = pEntry->enabled;

    if (pEntry->enabled)
        pQuerier->status = pEntry->status;
    else
        pQuerier->status = 0;

    if (pEntry->ip == 0)
        pQuerier->ipAddr = g_querier_ip;
    else
        pQuerier->ipAddr = pEntry->ip;

    pQuerier->version = pEntry->version;

    IGMP_DB_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 mcast_querier_enable_set(multicast_ipType_t ipType, sys_vlanmask_t *pVlanMsk, sys_enable_t enable)
{
    igmp_querier_entry_t *pEntry;
    mcast_vlan_entry_t  *pMcastVlan;
    sys_vid_t vid;

    SYS_PARAM_CHK((ipType < MULTICAST_TYPE_IPV4 || ipType > MULTICAST_TYPE_END), SYS_ERR_MCAST_IPTYPE);
    SYS_PARAM_CHK(NULL == pVlanMsk, SYS_ERR_NULL_POINTER);

    if (MULTICAST_TYPE_IPV4 != ipType)
        return SYS_ERR_OK;

    FOR_EACH_VID_IN_VLANMASK(vid, *pVlanMsk)
    {
       mcast_querier_db_get(vid, ipType, &pEntry);

       if (!pEntry)
            continue;

        if (ENABLED == pEntry->enabled)
        {
            if (DISABLED == enable)
            {
                pEntry->status = IGMP_NON_QUERIER;
                pEntry->ip = 0;
            }
            else
            {
                mcast_vlan_db_get(vid, ipType, &pMcastVlan);

                if (NULL != pMcastVlan)
                {
                    if ((ENABLED == pMcastVlan->enable) && (ENABLED == igmp_cfg.igmpsnp_enable))
                    {
                        pEntry->ip = g_querier_ip;
                        pEntry->timer = pMcastVlan->operQueryIntv /4;
                        pEntry->status = IGMP_QUERIER;
                    }
                    else
                    {
                        pEntry->status = IGMP_NON_QUERIER;
                        pEntry->ip = 0;
                    }
                }
            }
        }
    }

    return SYS_ERR_OK;
}
#endif
int32 mcast_querier_init(void)
{
#ifdef CONFIG_SYS_L3_ROUTE
    int32 i = 0;
    sys_ip_t emptyAddr;
    sys_l3_ipv4Db_t *pIpv4Db = NULL;
    sys_l3_ipv6Db_t *pIpv6Db = NULL;
#else
    sys_l3_ipv4_t ip;
    sys_ipv4_t emptyAddr;
#endif

    SYS_ERR_CHK(mcast_querier_db_init());

    SYS_MEM_CLEAR(g_querier_ip);
    SYS_MEM_CLEAR(emptyAddr);

#ifdef CONFIG_SYS_L3_ROUTE
    FOR_EACH_L3_IPV4(i, pIpv4Db)
    {
        if (osal_memcmp(&pIpv4Db->ip.addr, &emptyAddr.v4, sizeof(sys_ipv4_t)))
        {
            osal_memcpy(&g_querier_ip, &pIpv4Db->ip.addr, sizeof(sys_ipv4_t));
            break;
        }
    }

    FOR_EACH_L3_IPV6(i, pIpv6Db)
    {
        if (osal_memcmp(&pIpv6Db->ip.addr, &emptyAddr.v6, sizeof(sys_ipv6_t)))
        {
            osal_memcpy(&g_querier_ip6, &pIpv6Db->ip.addr, sizeof(sys_ipv6_t));
            break;
        }
    }

    SYS_ERR_CHK(sys_notifier_observer_register("igmp_querier", SYS_NOTIFIER_SUBJECT_L3,
                    SYS_NT_L3_INTF_IP_ADD | SYS_NT_L3_INTF_IP_DEL,
                    _mcast_querier_intfEvent_handler, SYS_NOTIFIER_PRI_MEDIUM));
#else
    rsd_sys_mgmtIPv4Addr_get(&ip);
    osal_memcpy(&g_querier_ip, &ip.addr, sizeof(sys_ipv4_t));

    SYS_ERR_CHK(sys_notifier_observer_register("igmp_querier", SYS_NOTIFIER_SUBJECT_SYSTEM,
                    SYS_NT_SYS_MGMT_IP_CHG, _mcast_querier_sysEvent_handler, SYS_NOTIFIER_PRI_MEDIUM));
#endif

    return SYS_ERR_OK;
}

