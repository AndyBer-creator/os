#include <common/sys_def.h>
#include <common/sys_portview.h>
#include <board/board.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_qos.h>
#include <rsd/rsd_sys.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_switch.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_trap.h>
#include <lldp/proto_lldp.h>
#ifdef CONFIG_SYS_VOICEVLAN
#include <voice_vlan/proto_voice_vlan.h>
#endif
#ifdef CONFIG_SYS_MIRROR
#include <rsd/rsd_mirror.h>
#endif
#ifdef CONFIG_SYS_PROTO_LLDP_MED
#include <board/include/lldp.h>
#endif
static int32 lldp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
static rsd_nic_handler_t        l2gLLDPHandler =
{
    .priority       = RSD_NIC_HANDLER_PRI_LLDP,
    .dmac           = { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x0E },
    .dmac_caremask  = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
    .ethertype      = 0,
    .proto          = 0,
    .sport          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_TRAP,
    .rx_cbf         = lldp_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_NONE,
    .pName          = "LLDP RX Thread",
    .pCookie        = NULL,
};
static volatile int32       lldp_tx_thread_status = 0;
static volatile int32       lldp_timer_thread_status = 0;
static lldp_database_t      lldp_db;
static lldp_port_table_t    port_table;
static lldp_statistic_t     lldp_statistic;
static sys_enable_t         lldp_admin_enabled;
static sys_enable_t         lldp_acl_enabled;
static lldp_disable_action_t    lldp_disable_action;
static sys_mac_t            sys_mac;
#ifdef CONFIG_SYS_PROTO_LLDP_MED
static lldp_med_networkPolicy_t networkPolicy[CAPA_LLDP_MED_NETWORK_POLICY_NUM];
#endif
static uint32               LLDP_TTL;
static uint32               tx_interval;
static uint16               tx_delay;
static uint8                holdtime;
static uint8                reinit_delay;
#ifdef CONFIG_SYS_PROTO_LLDP_MED
static uint8                med_fast_repeat_count;
static sys_enable_t         networkPolicyAuto;
static char                 inventory[LLDP_INVENTORY_TYPE_END][SYS_BUF64_LEN];
#endif 
static uint8                tlvChanged[SYS_LOGICPORT_NUM_MAX];
#ifdef CONFIG_SYS_MIRROR
static sys_logic_port_t     mirroringPort[CAPA_MIRROR_SESSION_NUM_MAX];
#endif
static uint8                replaceWhenFull = TRUE; 
static int32 changeTime; 
osal_mutex_t    lldp_debugPrint_sem;
osal_mutex_t    lldp_vlanTlv_sem[SYS_LOGICPORT_NUM_MAX];
#define LLDP_DEBUG_PRINT_SEM_LOCK()   \
    do {} while (osal_sem_mutex_take(lldp_debugPrint_sem, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)
#define LLDP_DEBUG_PRINT_SEM_UNLOCK() \
    do {} while (osal_sem_mutex_give(lldp_debugPrint_sem) != SYS_ERR_OK)
#define LLDP_PORT_VLAN_TLV_SEM_LOCK(port)   \
    do {} while (osal_sem_mutex_take(lldp_vlanTlv_sem[port], OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)
#define LLDP_PORT_VLAN_TLV_SEM_UNLOCK(port) \
    do {} while (osal_sem_mutex_give(lldp_vlanTlv_sem[port]) != SYS_ERR_OK)
static int32 lldp_linkevent_handler(uint32 event, char *pData);
static int32 lldp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
static int32 lldp_pkt_tx(uint32 logicPort, uint8 *pData, int32 len);
static void lldp_pkt_tx_thread(void *pInput);
static void lldp_timer_thread(void *pInput);
static int32 update_database(neighbor_entry_t *pEntry);
static void update_txdelay_time(sys_logic_port_t logicPort);
static void update_reinitdelay_time(sys_logic_port_t logicPort);
static void update_txtime(sys_logic_port_t logicPort);
static void update_holdtime(void);
static void send_lldp_flush_packet(uint32 port);
static int32 send_lldp_packet(uint32 port);
static int32 lldp_portPktTxAvoid_get(sys_logic_port_t port, uint8 *pAvoid);
int32 display_chassis(display_entry_t *pEntry, lldp_tlv_t tlv);
int32 display_portid(display_entry_t *pEntry,lldp_tlv_t tlv);
int32 display_cap(display_entry_t *pEntry, lldp_tlv_t tlv);
uint8 check_tlv_len(uint16 remainLen, uint16 len ,uint8 type);
int32 lldp_dump_packet(uint8 *pktData, uint32 pktLen, uint8 isTx, sys_logic_port_t port);
int32 display_macPhy(display_entry_t *pEntry, lldp_macPhy_tlv_t *macPhy_tlv);
int32 display_8023power(display_entry_t *pEntry, lldp_powerViaMdi_tlv_t *pPowerViaMdi_tlv);
#ifdef CONFIG_SYS_PROTO_LLDP_MED
int32 display_power(display_med_entry_t *pEntry, lldp_med_poe_tlv_t *poe_tlv);
#endif
int32 display_trunk(display_entry_t *pEntry, lldp_trunk_tlv_t *trunk_tlv);
int32 display_pvid(display_entry_t *pEntry, lldp_pvid_tlv_t *pvid_tlv);
#ifdef CONFIG_SYS_PROTO_LLDP_MED
int32 display_medCapa(display_med_entry_t *pEntry, lldp_med_media_capa_tlv_t *media_capa_tlv);
int32 display_networkPolicy(display_med_entry_t *pEntry, lldp_med_networkPolicy_tlv_t *policy_tlv);
int32 display_inventory(display_med_entry_t *pEntry, neighbor_entry_t *neigoborEntry);
int32 display_location(display_med_entry_t *pEntry, lldp_med_location_tlv_t *location_tlv);
#endif 

static const char* lldpCapabilityName[] =
{
    ", Other",
    ", Repeater",
    ", Bridge",
    ", WLAN",
    ", Router",
    ", Telephone",
    ", DOCSIS cable device",
    ", Station Only"
};

static uint16 lldpCapability[] =
{
    1,      
    2,      
    4,      
    8,      
    16,     
    32,     
    64,     
    128     
};
#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)

static int32 lldp_aclRule_update(sys_enable_t bEnable, lldp_disable_action_t disAct)
{
    rsd_acl_rsvEntry_t lldpAclRule =
    {
        .status     =   DISABLED,
        .type       =   ACL_RSV_LLDP,
        .name       =   "ACL_RSV_LLDP",
        .field      =   {   .fieldMask      = ACL_RSV_FIELDMASK_DMAC | ACL_RSV_FIELDMASK_STPDROP,
                            .dmac           = { 0x01, 0x80, 0xc2, 0x00, 0x00, 0x0e},
                            .care_dmac      = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
                            .stpDrop        = 0 },
        .act_ebl    =   {   .actionMask     = ACL_ACTIONMASK_TRAP | ACL_ACTIONMASK_BYPASS_STP | ACL_ACTIONMASK_PRIO,
                            .prio           = SYS_CPU_PORT_PRIORITY_LLDP,},
        .act_dbl    =   {   .actionMask     = 0,},
    };

    if (bEnable == ENABLED)
    {
        switch (disAct)
        {
            case LLDP_DISABLE_ACTION_FILTER:
            case LLDP_DISABLE_ACTION_FLOOD_TO_VLAN:
            case LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS:
                
                lldpAclRule.field.stpDrop     = 0;
                lldpAclRule.field.stpDropMask = 0;
                SYS_ERR_CHK(rsd_acl_rsv_rule_update(ACL_RSV_LLDP, &lldpAclRule.field, NULL, NULL));
                break;
            default:
                SYS_DBG(DBG_LLDP, "Invalid LLDP disable action: %d", disAct);
                return SYS_ERR_FAILED;
        } 
    }
    else if (bEnable == DISABLED)
    {
        switch (disAct)
        {
            case LLDP_DISABLE_ACTION_FILTER:
                
                lldpAclRule.field.stpDrop     = 0;
                lldpAclRule.field.stpDropMask = 0;
                SYS_ERR_CHK(rsd_acl_rsv_rule_update(ACL_RSV_LLDP, &lldpAclRule.field, NULL, NULL));
                break;
            case LLDP_DISABLE_ACTION_FLOOD_TO_VLAN:
            case LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS:
                
                lldpAclRule.field.stpDrop     = 0;
                lldpAclRule.field.stpDropMask = 1;
                SYS_ERR_CHK(rsd_acl_rsv_rule_update(ACL_RSV_LLDP, &lldpAclRule.field, NULL, NULL));
                break;
            default:
                SYS_DBG(DBG_LLDP, "Invalid LLDP disable action: %d", disAct);
                return SYS_ERR_FAILED;
        } 
    }
    else
    {
        
        SYS_DBG(DBG_LLDP, "Invalid LLDP admin status: %d", bEnable);
        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}
#endif 
int32 lldp_globalStatistics_clear(void)
{
    lldp_statistic.insertion = 0;
    lldp_statistic.deletion = 0;
    lldp_statistic.drop = 0;
    lldp_statistic.age_out = 0;

    return SYS_ERR_OK;
}
int32 lldp_portStatistics_clear(sys_logic_port_t port)
{
    osal_memset(&lldp_statistic.port_statistics[port], 0, sizeof(lldp_statistic.port_statistics[port]));
    return SYS_ERR_OK;
}
int32 lldp_statistic_get(lldp_statistic_t *pLldp_statistic)
{
    SYS_PARAM_CHK((NULL == pLldp_statistic), SYS_ERR_NULL_POINTER);

    *pLldp_statistic = lldp_statistic;

    return SYS_ERR_OK;
}
int32 lldp_portEntry_get(sys_logic_port_t logicPort ,lldp_port_entry_t *pEntryInfo)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(logicPort), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEntryInfo), SYS_ERR_NULL_POINTER);

    *pEntryInfo = port_table.port_entry[logicPort];

    return SYS_ERR_OK;
}
int32 lldp_enable_set(sys_enable_t enable)
{
    uint8 avoid;
    uint32 i;
    sys_logic_port_t port;
    sys_enable_t enableTmp;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

  #if 0 
    if(ENABLED == enable)
    {
        if(lldp_acl_enabled != ENABLED)
        {
            SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_LLDP, ENABLED));
            lldp_acl_enabled = ENABLED;
        }
    }
    else
    {
        uint8 lldp_da[CAPA_MAC_BYTE_NUM] = {0x01,0x80,0xC2,0x00,0x00,0x0e};
        sys_mac_t rma_frame;
        sys_trap_rma_action_t rma_action;

        osal_memcpy(&rma_frame.octet[0], &lldp_da[0], CAPA_MAC_BYTE_NUM);

        SYS_ERR_CHK(rsd_trap_rmaAction_get(&rma_frame, &rma_action));
        if(ACTION_TRAP2CPU == rma_action)
        {
            if(lldp_acl_enabled != ENABLED)
            {
                SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_LLDP, ENABLED));
                lldp_acl_enabled = ENABLED;
            }
        }
        else
        {
            if(lldp_acl_enabled != DISABLED)
            {
                SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_LLDP, DISABLED));
                lldp_acl_enabled = DISABLED;
            }
        }
    }
  #else
    if(lldp_acl_enabled != ENABLED)
    {
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_LLDP, ENABLED));
        lldp_acl_enabled = ENABLED;
    }
  #endif

    enableTmp = lldp_admin_enabled;
    lldp_admin_enabled = enable;

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
    if (IS_CHIP_CSW())
    {
        
        SYS_ERR_CHK(lldp_aclRule_update(lldp_admin_enabled, lldp_disable_action));
    }
#endif

    if (enableTmp == ENABLED && enable == DISABLED)
    {
        FOR_EACH_NORMAL_LOGIC_PORT(port)
        {
            if ((ENABLED == port_table.port_entry[port].tx_enabled))
            {
                if (LLDP_LINKUP == port_table.port_entry[port].link_status)
                {
                    lldp_portPktTxAvoid_get(port, &avoid);
                    if(FALSE == avoid)
                        send_lldp_flush_packet(port);
                }
            }

            port_table.port_entry[port].reinitdelay_timer = reinit_delay;

            for (i = 0 ; i < LLDP_MAX_ENTRY_PER_PORT; i++ )
            {
                if (0 != lldp_db.entry[port][i].chasis_id_tlv.tlv_type)
                {
                    if (lldp_db.entry[port][i].TTL > 0)
                    {
                        lldp_db.entry[port][i].time_tlv_value = 0; 
                        lldp_db.entry[port][i].TTL = 0; 
                    }
                }
            }
        }
    }

    return SYS_ERR_OK;
}
int32 lldp_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    *pEnable = lldp_admin_enabled;

    return SYS_ERR_OK;
}
int32 lldp_port_set(sys_logic_port_t port, uint8 lldpPortState)
{
    uint8 avoid;
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    switch (lldpPortState)
    {
        case LLDP_PORT_DISABLE:
            if ((ENABLED == port_table.port_entry[port].tx_enabled))
            {
                if (LLDP_LINKUP == port_table.port_entry[port].link_status)
                {
                    lldp_portPktTxAvoid_get(port, &avoid);
                    if(FALSE == avoid)
                        send_lldp_flush_packet(port);
                }
            }
            port_table.port_entry[port].tx_enabled = DISABLED;
            port_table.port_entry[port].rx_enabled = DISABLED;
            break;

        case LLDP_PORT_RXONLY:
            if ((ENABLED == port_table.port_entry[port].tx_enabled))
            {
                if (LLDP_LINKUP == port_table.port_entry[port].link_status)
                {
                    lldp_portPktTxAvoid_get(port, &avoid);
                    if(FALSE == avoid)
                        send_lldp_flush_packet(port);
                }
            }
            port_table.port_entry[port].tx_enabled = DISABLED;
            port_table.port_entry[port].rx_enabled = ENABLED;
            break;

        case LLDP_PORT_TXONLY:
            port_table.port_entry[port].tx_enabled = ENABLED;
            port_table.port_entry[port].rx_enabled = DISABLED;
            break;

        case LLDP_PORT_TXRX:
            port_table.port_entry[port].tx_enabled = ENABLED;
            port_table.port_entry[port].rx_enabled = ENABLED;
            break;
    }

    return SYS_ERR_OK;
}
int32 lldp_port_get(sys_logic_port_t port, uint8 *pLldpPortState)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLldpPortState), SYS_ERR_NULL_POINTER);

    if (ENABLED == port_table.port_entry[port].tx_enabled)
    {
        if (ENABLED == port_table.port_entry[port].rx_enabled)
            *pLldpPortState = LLDP_PORT_TXRX;
        else
            *pLldpPortState = LLDP_PORT_TXONLY;
    }
    else
    {
        if (ENABLED == port_table.port_entry[port].rx_enabled)
            *pLldpPortState = LLDP_PORT_RXONLY;
        else
            *pLldpPortState = LLDP_PORT_DISABLE;
    }

    return SYS_ERR_OK;
}
int32 lldp_tx_interval_set(uint32 my_tx_interval)
{
    sys_logic_port_t logicPort;

    LLDP_TTL = my_tx_interval * holdtime;
    tx_interval = my_tx_interval;
    if(LLDP_TTL > 65535)
        LLDP_TTL = 65535;

    LLDP_DBG("Tx interval = %d, TTL =%d", tx_interval, LLDP_TTL);
    FOR_EACH_LOGIC_PORT(logicPort)
    {
        port_table.port_entry[logicPort].tx_timer = 0;
    }

    return SYS_ERR_OK;
}
int32 lldp_holdtime_set(uint8 my_holdtime)
{
    sys_logic_port_t logicPort;

    holdtime = my_holdtime;
    LLDP_TTL = tx_interval * holdtime;
    if(LLDP_TTL > 65535)
        LLDP_TTL = 65535;

    LLDP_DBG("holdtime = %d, TTL =%d", holdtime, LLDP_TTL);

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        port_table.port_entry[logicPort].tx_timer = 0;
    }

    return SYS_ERR_OK;
}
int32 lldp_reinit_delay_set(uint8 my_reinit_delay)
{
    reinit_delay = my_reinit_delay;

    LLDP_DBG("reinit_delay = %d", reinit_delay);

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED

int32 lldp_medInventory_set(lldp_inventory_type_t inventoryType, char *strData)
{
    sys_lldp_tlv_change_event_t tlvEvent;
    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    osal_strcpy(inventory[inventoryType], strData);

    tlvEvent.isAllPort = TRUE;
    tlvEvent.type = LLDP_TLV_NOTIFY_INVENTORY;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
int32 lldp_medInventory_get(lldp_inventory_type_t inventoryType, char *strData)
{
    osal_strcpy(strData, inventory[inventoryType]);

    return SYS_ERR_OK;
}
#endif 
int32 lldp_portLocalEntryInfo_get(sys_logic_port_t port, display_entry_t *pEntry)
{
    int32 ret = SYS_ERR_OK;
    sys_l3_ipv4_t ip;
    uint32 maxSize;
    sys_sysinfo_t *pSysinfo = NULL;
    sys_l3_intfIPv6AddrsInfo_t ipv6Addr;
    uint32 i;
    char macStr[CAPA_MAC_STR_LEN];

    osal_memset(pEntry, 0, sizeof(*pEntry));

    pEntry->logicPort = port;

    pEntry->lldp_ttl = LLDP_TTL;

    pEntry->chasis_id.iSubtype = CHASSIS_ID_MAC_ADDRESS;
    osal_strcpy(pEntry->chasis_id.subtype, "MAC address");
    osal_memcpy(pEntry->chasis_id.iId, sys_mac.octet, sizeof(sys_mac.octet));
    sys_util_mac2Str(sys_mac.octet, macStr);
    osal_sprintf(pEntry->chasis_id.id, "%s", macStr);

    pEntry->port_id.iSubtype = PORT_ID_LOCALLY_ASSIGNED; 
    LP2STR(port, pEntry->port_id.id);
#ifdef CONFIG_SYS_L3_ROUTE
    pEntry->system_capabilities.iSupport = (SYSTEM_CAPABILITY_BRIDGE + SYSTEM_CAPABILITY_ROUTER);
    osal_strcpy(pEntry->system_capabilities.support, "Bridge, Router");
    pEntry->system_capabilities.iEnable = (SYSTEM_CAPABILITY_BRIDGE + SYSTEM_CAPABILITY_ROUTER);
    osal_strcpy(pEntry->system_capabilities.enable, "Bridge, Router");
#else
    pEntry->system_capabilities.iSupport = (SYSTEM_CAPABILITY_BRIDGE);
    osal_strcpy(pEntry->system_capabilities.support, "Bridge");
    pEntry->system_capabilities.iEnable = (SYSTEM_CAPABILITY_BRIDGE);
    osal_strcpy(pEntry->system_capabilities.enable, "Bridge");
#endif

    SYS_MEM_ALLOC_HDL(pSysinfo, sizeof(sys_sysinfo_t), _exit, ret);
    rsd_sys_info_get(pSysinfo);
    
    osal_strcpy(pEntry->system_name , pSysinfo->name);
    
    osal_strcpy(pEntry->system_description , pSysinfo->description);
    
    rsd_port_descp_get(port, pEntry->port_description);

    if(TRUE == port_table.port_entry[port].lldpTlvSel.managementAddrSel)
    {
        pEntry->manage_address[0].valid = TRUE;
        pEntry->manage_address[0].len = 5; 
        pEntry->manage_address[0].iSubtype = 1; 
        rsd_sys_mgmtIPv4Addr_get(&ip);
        osal_memcpy(pEntry->manage_address[0].addr, &ip.addr, 4);
        pEntry->manage_address[0].interfaceSubtype = 3; 
        pEntry->manage_address[0].interfaceNum = 0; 

        SYS_MEM_CLEAR(ipv6Addr);
        rsd_sys_intfIpv6Addr_get(&ipv6Addr);

        if(ipv6Addr.cnt > 0)
        {
            for(i = 0; i < ipv6Addr.cnt; i ++)
            {
                if((i + 1) >= MAX_LLDP_MANAGEMENT_ADDR_NUM)
                    break;
                pEntry->manage_address[i+1].valid = TRUE;
                pEntry->manage_address[i+1].len = 17; 
                pEntry->manage_address[i+1].iSubtype = 2; 
                osal_memcpy(pEntry->manage_address[i+1].addr, &ipv6Addr.addr[i], 16);
                pEntry->manage_address[i+1].interfaceSubtype = 3; 
                pEntry->manage_address[i+1].interfaceNum = 0; 
            }
        }
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.maxFrameSizeSel)
    {
        pEntry->maxFrameSize.valid = TRUE;
        rsd_switch_maxFrameSize_get(&maxSize);
        pEntry->maxFrameSize.maxFrameSize = maxSize;
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.macPhySel)
    {
        uint8 autoNego;
        uint16 capa;
        uint16 oper;
        lldp_macPhy_tlv_t   macPhy_tlv;

        rsd_port_macPhyInfo_get(port, &autoNego, &capa, &oper);
        macPhy_tlv.valid = TRUE;
        macPhy_tlv.capa = capa;
        macPhy_tlv.autoNego = autoNego;
        macPhy_tlv.oper = oper;
        display_macPhy(pEntry, &macPhy_tlv);
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.linkAggregationSel)
    {
        lldp_trunk_tlv_t trunk_tlv;

        trunk_tlv.valid = TRUE;

        rsd_trunk_port2Trunk_get(port, &trunk_tlv.aggId);
        if(0xffff != trunk_tlv.aggId)
        {
            trunk_tlv.aggInfo = 0x3; 
            trunk_tlv.aggId += 1;
        }
        else
        {
            trunk_tlv.aggInfo = 0x1; 
            trunk_tlv.aggId = 0;
        }

        display_trunk(pEntry, &trunk_tlv);
    }

    if(ENABLED == port_table.port_entry[port].tlvPvidEbl)
    {
        sys_vid_t pvid;
        lldp_pvid_tlv_t pvid_tlv;

        rsd_vlan_portOperPvid_get(port, &pvid);
        pvid_tlv.valid = TRUE;
        pvid_tlv.pvid = pvid;
        display_pvid(pEntry, &pvid_tlv);
    }

_exit:
    SYS_MEM_FREE(pSysinfo);
    return ret;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED

int32 lldp_portMedLocalEntryInfo_get(sys_logic_port_t port, display_med_entry_t *pEntry)
{
    lldp_med_media_capa_tlv_t media_capa_tlv;
    lldp_med_tlv_sel_t lldpMedTlvSel;

    osal_memset(pEntry, 0, sizeof(*pEntry));

    pEntry->logicPort = port;

    if(ENABLED == port_table.port_entry[port].medEnable)
    {
        media_capa_tlv.valid = TRUE;

        lldp_portMedTlvSel_get(port, &lldpMedTlvSel);
        media_capa_tlv.capa = (lldpMedTlvSel.inventorySel << 5) | (lldpMedTlvSel.poePseSel << 3) |
            (lldpMedTlvSel.locationSel << 2) | (lldpMedTlvSel.networkPolicySel << 1) | 1;
        media_capa_tlv.classType = LLDP_CLASS_TYPE_NETWORK_CONN;

        display_medCapa(pEntry, &media_capa_tlv);

        if (TRUE == port_table.port_entry[port].lldpMedTlvSel.poePseSel)
        {
            lldp_med_poe_tlv_t  poe_tlv;
            lldp_poe_portL2Data_t pse;

            osal_memset(&poe_tlv, 0, sizeof(poe_tlv));
            osal_memset(&pse, 0, sizeof(pse));

            board_lldp_poePortL2Data_get(port, &pse);

            poe_tlv.valid = TRUE;

            poe_tlv.powerInfo.type = pse.pwrType;
            poe_tlv.powerInfo.source = pse.pwrSrc;
            poe_tlv.powerInfo.pri = pse.pri;

            poe_tlv.powerVal = pse.power;

            display_power(pEntry, &poe_tlv);
        }

        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.networkPolicySel)
        {
            lldp_med_networkPolicy_tlv_t policy_tlv[MAX_LLDP_MED_NETWORK_POLICY_RX];
            uint8 policyIdx;
            uint8 i = 0;
          #ifdef CONFIG_SYS_VOICEVLAN
            sys_vid_t voiceVid;
            sys_vlan_voiceQos_t voiceQos;
          #endif

            osal_memset(policy_tlv, 0, sizeof(policy_tlv));

          #ifdef CONFIG_SYS_VOICEVLAN
            if(ENABLED == networkPolicyAuto)
            {
                voice_vlan_state_t state;
                sys_vid_t defaultVid;

                voice_vlan_state_get(&state);
                rsd_vlan_voiceVid_get(&voiceVid);
                voice_vlan_qos_get(&voiceQos);
                rsd_vlan_defaultVid_get(&defaultVid);

                if(voiceVid != defaultVid && VOICE_VLAN_AUTO_ENABLED == state) 
                {
                    policy_tlv[0].valid = TRUE;
                    policy_tlv[0].app = LLDP_APP_VOICE;
                    policy_tlv[0].policyInfo.unknown = 0;
                    policy_tlv[0].policyInfo.tag = TRUE;
                    policy_tlv[0].policyInfo.vlanId = voiceVid;
                    policy_tlv[0].policyInfo.l2Pri = voiceQos.vpt;
                    policy_tlv[0].policyInfo.dscpPri = voiceQos.dscp;
                    i ++;
                }
            }
          #endif

            for(policyIdx = 0; policyIdx < 32; policyIdx ++)
            {
                if(0 != (port_table.port_entry[port].policyBind.bindMask & (1 << policyIdx)))
                {
                    if(TRUE == networkPolicy[policyIdx].valid)
                    {
                        policy_tlv[i].valid = TRUE;
                        policy_tlv[i].app = networkPolicy[policyIdx].app;
                        policy_tlv[i].policyInfo.unknown = 0;
                        policy_tlv[i].policyInfo.tag = networkPolicy[policyIdx].vlanTagged;
                        policy_tlv[i].policyInfo.vlanId = networkPolicy[policyIdx].vid;
                        policy_tlv[i].policyInfo.l2Pri = networkPolicy[policyIdx].userPriority;
                        policy_tlv[i].policyInfo.dscpPri = networkPolicy[policyIdx].dscpValue;
                        i ++;
                    }
                }
            }
            display_networkPolicy(pEntry, policy_tlv);
        }

        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.inventorySel)
        {
            char strData[MAX_INVENTORY_STRING_LEN];

            lldp_medInventory_get(LLDP_INVENTORY_TYPE_HARDWARE_REVISION, strData);
            osal_strcpy(pEntry->hardware_v.strInventory, strData);
            lldp_medInventory_get(LLDP_INVENTORY_TYPE_FIRMWARE_REVISION, strData);
            osal_strcpy(pEntry->firmware_v.strInventory, strData);
            lldp_medInventory_get(LLDP_INVENTORY_TYPE_SOFTWARE_REVISION, strData);
            osal_strcpy(pEntry->software_v.strInventory, strData);
            lldp_medInventory_get(LLDP_INVENTORY_TYPE_SERIA_NUMBER, strData);
            osal_strcpy(pEntry->serial_num.strInventory, strData);
            lldp_medInventory_get(LLDP_INVENTORY_TYPE_MANUFACTURER, strData);
            osal_strcpy(pEntry->manufacturer.strInventory, strData);
            lldp_medInventory_get(LLDP_INVENTORY_TYPE_MODEL_NAME, strData);
            osal_strcpy(pEntry->model_name.strInventory, strData);
            lldp_medInventory_get(LLDP_INVENTORY_TYPE_ASSET_ID, strData);
            osal_strcpy(pEntry->asset.strInventory, strData);

            pEntry->hardware_v.valid = pEntry->firmware_v.valid = pEntry->software_v.valid = pEntry->serial_num.valid =
                pEntry->manufacturer.valid = pEntry->model_name.valid = pEntry->asset.valid = TRUE;
        }

        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.locationSel)
        {
            lldp_med_location_tlv_t location_tlv;
            osal_memset(&location_tlv, 0, sizeof(location_tlv));
            location_tlv.valid = TRUE;
            lldp_portMedLocation_get(port, &location_tlv.location);
            display_location(pEntry, &location_tlv);
        }
    }

    return SYS_ERR_OK;
}
#endif 
int32 lldp_portRemoteEntryInfo_get(sys_logic_port_t port, uint8 index ,display_entry_t *pEntry)
{
    uint8 i;

    osal_memset(pEntry, 0, sizeof(*pEntry));

    if (0 != lldp_db.entry[port][index].chasis_id_tlv.tlv_type)
    {
        LLDP_DBG("display_entry_t size = %d", sizeof(display_entry_t));

        pEntry->logicPort = lldp_db.entry[port][index].port_index;

        pEntry->lldp_ttl = lldp_db.entry[port][index].TTL;

        display_chassis(pEntry, lldp_db.entry[port][index].chasis_id_tlv);

        display_portid(pEntry, lldp_db.entry[port][index].port_id_tlv);

        display_cap(pEntry,lldp_db.entry[port][index].sys_capa_tlv);

        osal_strncpy(pEntry->port_description , lldp_db.entry[port][index].port_description, MAX_TLV_STRING_LEN);
        osal_strncpy(pEntry->system_name , lldp_db.entry[port][index].system_name, MAX_TLV_STRING_LEN);
        osal_strncpy(pEntry->system_description , lldp_db.entry[port][index].system_description, MAX_TLV_STRING_LEN);

        for(i = 0; i < MAX_LLDP_MANAGEMENT_ADDR_NUM; i ++)
        {
            if (TRUE == lldp_db.entry[port][index].manage_address[i].valid)
            {
                pEntry->manage_address[i].valid = TRUE;
                pEntry->manage_address[i].len = lldp_db.entry[port][index].manage_address[i].len;
                pEntry->manage_address[i].iSubtype = lldp_db.entry[port][index].manage_address[i].iSubtype;
                pEntry->manage_address[i].interfaceSubtype = lldp_db.entry[port][index].manage_address[i].interfaceSubtype;
                pEntry->manage_address[i].interfaceNum = lldp_db.entry[port][index].manage_address[i].interfaceNum;
                osal_memcpy(pEntry->manage_address[i].addr, lldp_db.entry[port][index].manage_address[i].addr, sizeof(pEntry->manage_address[i].addr));
            }
        }

        pEntry->maxFrameSize = lldp_db.entry[port][index].maxFrameSize_tlv;

        display_macPhy(pEntry, &lldp_db.entry[port][index].macPhy_tlv);

        display_8023power(pEntry, &lldp_db.entry[port][index].powerViaMdi_tlv);

        display_trunk(pEntry, &lldp_db.entry[port][index].trunk_tlv);

        display_pvid(pEntry, &lldp_db.entry[port][index].pvid_tlv);

        pEntry->vlanNum = lldp_db.entry[port][index].vlanNum;

        pEntry->powerAllocBy = lldp_db.entry[port][index].powerAllocBy;
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED

int32 lldp_portMedRemoteEntryInfo_get(sys_logic_port_t port, uint8 index ,display_med_entry_t *pEntry)
{
    osal_memset(pEntry, 0, sizeof(*pEntry));

    if (0 != lldp_db.entry[port][index].chasis_id_tlv.tlv_type)
    {
        
        pEntry->logicPort = lldp_db.entry[port][index].port_index;

        display_power(pEntry, &lldp_db.entry[port][index].poe_tlv);

        display_medCapa(pEntry, &lldp_db.entry[port][index].media_capa_tlv);

        display_networkPolicy(pEntry, lldp_db.entry[port][index].policy_tlv);

        display_inventory(pEntry, &lldp_db.entry[port][index]);

        display_location(pEntry, &lldp_db.entry[port][index].location_tlv);
    }

    return SYS_ERR_OK;
}
#endif 
int32 lldp_portRemoteVlanNameTlv_get(sys_logic_port_t port, uint16 index, lldp_vlanName_tlv_t  *pVlanTlv)
{
    int i;
    lldp_vlanName_tlv_t *pTlv;

    SYS_PARAM_CHK((NULL == pVlanTlv), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    LLDP_PORT_VLAN_TLV_SEM_LOCK(port);

    pTlv = &lldp_db.entry[port][0].vlanName_tlv;

    osal_memset(pVlanTlv, 0, sizeof(pVlanTlv));

    for(i = 1; i <= index; i ++)
    {
        if(NULL != pTlv->pNext)
            pTlv = pTlv->pNext;
        else
            break;
    }

    if(NULL != pTlv)
    {
        osal_memcpy(pVlanTlv, pTlv, sizeof(*pVlanTlv));
    }
    else
        pVlanTlv->valid = FALSE;
    LLDP_PORT_VLAN_TLV_SEM_UNLOCK(port);

    return SYS_ERR_OK;
}
int32 lldp_portRemoteEntry_clear(sys_logic_port_t port, uint8 index)
{
    
    lldp_db.entry[port][index].time_tlv_value = 0; 
    lldp_db.entry[port][index].TTL = 0; 

    return SYS_ERR_OK;
}   
int32 lldp_portTlvOverload_get(sys_logic_port_t port, lldp_overLoading_t *pOverload)
{
    int32 ret = SYS_ERR_OK;
    char portDescriStr[SYS_BUF128_LEN], portStr[SYS_BUF32_LEN];
    uint32 maxSize;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    lldp_med_location_t *pLocation = NULL;
  #endif
    sys_vid_t vid;
    sys_vlan_membership_t membership;
    sys_sysinfo_t *pSysinfo = NULL;
    sys_l3_intfIPv6AddrsInfo_t *pIpv6Addr = NULL;
    uint32 i;

    rsd_switch_maxFrameSize_get(&maxSize);
    osal_memset(pOverload, 0, sizeof(*pOverload));

    if(maxSize > 1518) 
        maxSize = 1518 - 30; 
    else
        maxSize = maxSize - 30; 
    pOverload->maxSize = maxSize;

    pOverload->mandatory.byte += TLVH_OFFSET + 7;

    LP2STR(port, portStr);
    pOverload->mandatory.byte += TLVH_OFFSET + SUB_TYPE_LEN + osal_strlen(portStr);

    pOverload->mandatory.byte += TLVH_OFFSET + TTL_TLV_LEN;

    pOverload->mandatory.byte += TLVH_OFFSET;

    pOverload->totalSize += pOverload->mandatory.byte;
    if(pOverload->totalSize > pOverload->maxSize)
    {
        pOverload->mandatory.isOverload = TRUE;
        pOverload->isOverload = TRUE;
    }
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_MEM_ALLOC_HDL(pLocation, sizeof(lldp_med_location_t), _exit, ret);
    if(ENABLED == port_table.port_entry[port].medEnable)
    {
        
        pOverload->medCapability.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + MEDIA_CAPA_LEN + CLASS_TYPE_LEN;
        pOverload->totalSize += pOverload->medCapability.byte;
        if(pOverload->totalSize > pOverload->maxSize)
        {
            pOverload->medCapability.isOverload = TRUE;
            pOverload->isOverload = TRUE;
        }

        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.locationSel)
        {
            
            lldp_portMedLocation_get(port, pLocation);
            if(0 != pLocation->coorLen)
            {
                pOverload->medLocation.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + LOCATION_DATA_FMT_LEN + pLocation->coorLen;
            }
            
            if(0 != pLocation->civicLen)
            {
                pOverload->medLocation.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + LOCATION_DATA_FMT_LEN + pLocation->civicLen;
            }
            
            if(0 != pLocation->ecsLen)
            {
                pOverload->medLocation.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + LOCATION_DATA_FMT_LEN + pLocation->ecsLen;
            }

            pOverload->totalSize += pOverload->medLocation.byte;
            if(pOverload->totalSize > pOverload->maxSize)
            {
                pOverload->medLocation.isOverload = TRUE;
                pOverload->isOverload = TRUE;
            }
        }
        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.networkPolicySel)
        {
            if(ENABLED == networkPolicyAuto)
            {
                pOverload->medPolicy.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + APP_TYPE_LEN + POLICY_LEN;
            }
            else
            {
                uint8 policyIdx;
                for(policyIdx = 0; policyIdx < 32; policyIdx ++)
                {
                    if(0 != (port_table.port_entry[port].policyBind.bindMask & (1 << policyIdx)))
                    {
                        if(TRUE == networkPolicy[policyIdx].valid)
                        {
                            pOverload->medPolicy.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + APP_TYPE_LEN + POLICY_LEN;
                        }
                    }
                }
            }
            pOverload->totalSize += pOverload->medPolicy.byte;
            if(pOverload->totalSize > pOverload->maxSize)
            {
                pOverload->medLocation.isOverload = TRUE;
                pOverload->isOverload = TRUE;
            }
        }

        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.poePseSel)
        {
            pOverload->medPoe.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + POWER_INFO_LEN + POWER_VAL_LEN;
            pOverload->totalSize += pOverload->medPoe.byte;
            if(pOverload->totalSize > pOverload->maxSize)
            {
                pOverload->medPoe.isOverload = TRUE;
                pOverload->isOverload = TRUE;
            }
        }
    }
  #endif 

    if(TRUE == port_table.port_entry[port].lldpTlvSel.macPhySel)
    {
        pOverload->tlv802dot3.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + AUTO_NEGO_STATS_LEN + AUTO_NEGO_CAP_LEN + OPER_TYPE_LEN;
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.linkAggregationSel)
    {
        pOverload->tlv802dot3.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + AGG_STATS_LEN + AGG_ID_LEN;
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.maxFrameSizeSel)
    {
        pOverload->tlv802dot3.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + MAX_FRAME_SIZE_LEN;
    }

    pOverload->totalSize += pOverload->tlv802dot3.byte;
    if(pOverload->totalSize > pOverload->maxSize)
    {
        pOverload->tlv802dot3.isOverload = TRUE;
        pOverload->isOverload = TRUE;
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.portDesciptionSel)
    {
        rsd_port_descp_get(port,portDescriStr);
        pOverload->optional.byte += TLVH_OFFSET + osal_strlen(portDescriStr);
    }

    SYS_MEM_ALLOC_HDL(pSysinfo, sizeof(sys_sysinfo_t), _exit, ret);
    rsd_sys_info_get(pSysinfo);
    
    if(TRUE == port_table.port_entry[port].lldpTlvSel.sysNameSel)
    {
        pOverload->optional.byte += TLVH_OFFSET + osal_strlen(pSysinfo->name);
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.sysDescriptionSel)
    {
        pOverload->optional.byte += TLVH_OFFSET + osal_strlen(pSysinfo->description);
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.sysCapabilitySel)
        pOverload->optional.byte += TLVH_OFFSET + CAPA_TLV_OFFSET + CAPA_TLV_OFFSET;

    if(TRUE == port_table.port_entry[port].lldpTlvSel.managementAddrSel)
    {
        pOverload->optional.byte += TLVH_OFFSET + MGMT_ADDR_STR_LEN + MGMT_ADDR_SUBTYPE + IPV4_LEN + IF_NUM_SUBTYPE + IF_NUM + OID + OBJ_IDENTIFIER ;

        SYS_MEM_ALLOC_HDL(pIpv6Addr, sizeof(sys_l3_intfIPv6AddrsInfo_t), _exit, ret);
        rsd_sys_intfIpv6Addr_get(pIpv6Addr);
        if(pIpv6Addr->cnt > 0)
        {
            for(i = 0; i < pIpv6Addr->cnt; i ++)
            {
                if((i + 1) >= MAX_LLDP_MANAGEMENT_ADDR_NUM)
                    break;

                pOverload->optional.byte += TLVH_OFFSET + MGMT_ADDR_STR_LEN + MGMT_ADDR_SUBTYPE + IPV6_LEN + IF_NUM_SUBTYPE + IF_NUM + OID + OBJ_IDENTIFIER ;
            }
        }
    }

    pOverload->totalSize += pOverload->optional.byte;
    if(pOverload->totalSize > pOverload->maxSize)
    {
        pOverload->optional.isOverload = TRUE;
        pOverload->isOverload = TRUE;
    }

  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    if(ENABLED == port_table.port_entry[port].medEnable)
    {
        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.inventorySel)
        {
            lldp_inventory_type_t inventoryType;
            char strData[128];

            for(inventoryType = LLDP_INVENTORY_TYPE_HARDWARE_REVISION; inventoryType < LLDP_INVENTORY_TYPE_END; inventoryType ++)
            {
                lldp_medInventory_get(inventoryType, strData);
                pOverload->medInventory.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + osal_strlen(strData);
            }
        }
        pOverload->totalSize += pOverload->medInventory.byte;
        if(pOverload->totalSize > pOverload->maxSize)
        {
            pOverload->medInventory.isOverload = TRUE;
            pOverload->isOverload = TRUE;
        }
    }
  #endif 

    pOverload->tlv802dot1.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + PORT_VLAN_ID_LEN;

    FOR_EACH_VID_IN_VLANMASK(vid, port_table.port_entry[port].tlvVlanmask)
    {
        rsd_vlan_portOperVlanMembership_get(port, vid, &membership);

        if(SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
        {
            uint8 len;
            sys_namestr_t name;

            rsd_vlan_name_get(vid, &name);
            len = osal_strlen(name.str);
            pOverload->tlv802dot1.byte += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + VLAN_ID_LEN + VLAN_NAME_LEN_LEN + len;
        }
    }
    pOverload->totalSize += pOverload->tlv802dot1.byte;

    if(pOverload->totalSize > pOverload->maxSize)
    {
        pOverload->tlv802dot1.isOverload = TRUE;
        pOverload->isOverload = TRUE;
    }

    pOverload->leftSize = pOverload->maxSize - pOverload->totalSize;

_exit:
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_MEM_FREE(pLocation);
  #endif
    SYS_MEM_FREE(pIpv6Addr);
    SYS_MEM_FREE(pSysinfo);
    return ret;
}
int32 lldp_disableAction_set(lldp_disable_action_t action)
{
    lldp_disable_action = action;

#if defined(CONFIG_SYS_RTL9300) || defined(CONFIG_SYS_RTL9310)
    if (IS_CHIP_CSW())
    {
        
        SYS_ERR_CHK(lldp_aclRule_update(lldp_admin_enabled, lldp_disable_action));
    }
#endif

    return SYS_ERR_OK;
}
int32 lldp_disableAction_get(lldp_disable_action_t *pAction)
{
    SYS_PARAM_CHK((NULL == pAction), SYS_ERR_NULL_POINTER);

    *pAction = lldp_disable_action;

    return SYS_ERR_OK;
}
int32 lldp_portTlvPvidEnable_set(sys_logic_port_t port, sys_enable_t pvidEnable)
{
    sys_lldp_tlv_change_event_t tlvEvent;
    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    port_table.port_entry[port].tlvPvidEbl = pvidEnable;

    tlvEvent.isAllPort = FALSE;
    tlvEvent.port = port;
    tlvEvent.type = LLDP_TLV_NOTIFY_TLV_ADD_RMV;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
int32 lldp_portTlvPvidEnable_get(sys_logic_port_t port, sys_enable_t *pPvidEnable)
{
    *pPvidEnable = port_table.port_entry[port].tlvPvidEbl;

    return SYS_ERR_OK;
}
int32 lldp_portTlvVlanmask_set(sys_logic_port_t port, sys_vlanmask_t  vlanmask)
{
    sys_vid_t vid;
    sys_vlan_membership_t membership;
    sys_lldp_tlv_change_event_t tlvEvent;
    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    FOR_EACH_VID_IN_VLANMASK(vid, vlanmask)
    {
        SYS_ERR_CHK(rsd_vlan_portOperVlanMembership_get(port, vid, &membership));
        if((SYS_VLAN_EXCLUDED == membership) || (SYS_VLAN_FORBIDDEN == membership))
            return SYS_ERR_VLAN_MEMBERSHIP;
    }

    port_table.port_entry[port].tlvVlanmask = vlanmask;

    tlvEvent.isAllPort = FALSE;
    tlvEvent.port = port;
    tlvEvent.type = LLDP_TLV_NOTIFY_TLV_ADD_RMV;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);
    return SYS_ERR_OK;
}
int32 lldp_portTlvVlanmask_get(sys_logic_port_t port, sys_vlanmask_t *pVlanmask)
{
    *pVlanmask = port_table.port_entry[port].tlvVlanmask;

    return SYS_ERR_OK;
}
int32 lldp_portTlvSel_set(sys_logic_port_t port, lldp_tlv_sel_t lldpTlvSel)
{
    sys_lldp_tlv_change_event_t tlvEvent;
    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    port_table.port_entry[port].lldpTlvSel = lldpTlvSel;

    tlvEvent.isAllPort = FALSE;
    tlvEvent.port = port;
    tlvEvent.type = LLDP_TLV_NOTIFY_TLV_ADD_RMV;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
int32 lldp_portTlvSel_get(sys_logic_port_t port, lldp_tlv_sel_t *pLldpTlvSel)
{
    *pLldpTlvSel = port_table.port_entry[port].lldpTlvSel;

    return SYS_ERR_OK;
}
int32 lldp_tx_delay_set(uint16 txDelay)
{
    sys_logic_port_t logicPort;

    tx_delay = txDelay;

    FOR_EACH_LOGIC_PORT(logicPort)
    {
        if(port_table.port_entry[logicPort].txdelay_timer > txDelay)
            port_table.port_entry[logicPort].txdelay_timer = txDelay;
    }

    return SYS_ERR_OK;
}
int32 lldp_tx_delay_get(uint16 *pTxDelay)
{
    SYS_PARAM_CHK((NULL == pTxDelay), SYS_ERR_NULL_POINTER);

    *pTxDelay = tx_delay;

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED

int32 lldp_medFastRepeatCount_set(uint8 fastRepeatCount)
{
    med_fast_repeat_count = fastRepeatCount;

    return SYS_ERR_OK;
}
int32 lldp_medFastRepeatCount_get(uint8 *pFastRepeatCount)
{
    SYS_PARAM_CHK((NULL == pFastRepeatCount), SYS_ERR_NULL_POINTER);

    *pFastRepeatCount = med_fast_repeat_count;

    return SYS_ERR_OK;
}
int32 lldp_medNetworkPolicy_set(uint32 idx, lldp_med_networkPolicy_t policy)
{
    osal_memcpy(&networkPolicy[idx], &policy, sizeof(networkPolicy[idx]));

    return SYS_ERR_OK;
}
int32 lldp_medNetworkPolicy_get(uint32 idx, lldp_med_networkPolicy_t *pPolicy)
{
    osal_memcpy(pPolicy ,&networkPolicy[idx], sizeof(*pPolicy));

    return SYS_ERR_OK;
}
int32 lldp_medNetworkPolicyAuto_set(sys_enable_t enable)
{
    sys_lldp_tlv_change_event_t tlvEvent;
    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    networkPolicyAuto = enable;

    tlvEvent.isAllPort = TRUE;
    tlvEvent.port = 0;
    tlvEvent.type = LLDP_TLV_NOTIFY_VOICE_AUTO_EBL;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
int32 lldp_medNetworkPolicyAuto_get(sys_enable_t *pEnable)
{
    *pEnable = networkPolicyAuto;

    return SYS_ERR_OK;
}
int32 lldp_portMedTlvSel_set(sys_logic_port_t port, lldp_med_tlv_sel_t lldpMedTlvSel)
{
    sys_lldp_tlv_change_event_t tlvEvent;
    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    if (!IS_POE_PORT(port))
        lldpMedTlvSel.poePseSel = FALSE;

    port_table.port_entry[port].lldpMedTlvSel = lldpMedTlvSel;

    tlvEvent.isAllPort = FALSE;
    tlvEvent.port = port;
    tlvEvent.type = LLDP_TLV_NOTIFY_MED_TLV_ADD_RMV;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
int32 lldp_portMedTlvSel_get(sys_logic_port_t port, lldp_med_tlv_sel_t *pLldpMedTlvSel)
{
    *pLldpMedTlvSel = port_table.port_entry[port].lldpMedTlvSel;

    return SYS_ERR_OK;
}
int32 lldp_portMedEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    sys_lldp_tlv_change_event_t tlvEvent;
    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    port_table.port_entry[port].medEnable = enable;

    tlvEvent.isAllPort = FALSE;
    tlvEvent.port = port;
    tlvEvent.type = LLDP_TLV_NOTIFY_MED_TLV_ADD_RMV;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
int32 lldp_portMedEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    *pEnable = port_table.port_entry[port].medEnable;

    return SYS_ERR_OK;
}
int32 lldp_portMedNetworkPolicyBind_set(sys_logic_port_t port, lldp_med_networkPolicy_bind_t policyBind)
{
    uint32 idx;
    uint32 appTypeMask = 0;
    sys_lldp_tlv_change_event_t tlvEvent;

    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    for(idx = 0; idx < CAPA_LLDP_MED_NETWORK_POLICY_NUM; idx ++)
    {
        if(policyBind.bindMask & (1 << idx))
        {
            if(FALSE == networkPolicy[idx].valid)
            {
                return SYS_ERR_LLDP_NETWORK_POLICY_NOT_EXIST;
            }

            if(appTypeMask & (1 << networkPolicy[idx].app))
                return SYS_ERR_LLDP_NETWORK_POLICY_DUPLICATE;
            else
                appTypeMask |= (1 << networkPolicy[idx].app);
        }
    }

    port_table.port_entry[port].policyBind = policyBind;

    tlvEvent.isAllPort = FALSE;
    tlvEvent.port = port;
    tlvEvent.type = LLDP_TLV_NOTIFY_NETWORK_POLICY;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
int32 lldp_portMedNetworkPolicyBind_get(sys_logic_port_t port, lldp_med_networkPolicy_bind_t *pPolicyBind)
{
    *pPolicyBind = port_table.port_entry[port].policyBind;

    return SYS_ERR_OK;
}
int32 lldp_portMedLocation_set(sys_logic_port_t port, lldp_med_location_t location)
{
    sys_lldp_tlv_change_event_t tlvEvent;
    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    osal_memcpy(&port_table.port_entry[port].location, &location, sizeof(location));

    tlvEvent.isAllPort = FALSE;
    tlvEvent.port = port;
    tlvEvent.type = LLDP_TLV_NOTIFY_LOCATION;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
int32 lldp_portMedLocation_get(sys_logic_port_t port, lldp_med_location_t *pLocation)
{
    *pLocation = port_table.port_entry[port].location;

    return SYS_ERR_OK;
}
#endif 

int32 lldp_remoteTableChangeTime_refresh(void)
{
    uint32 jiffie, hz;

    SYS_ERR_CHK(rsd_sys_jiffies_get(&jiffie));
    SYS_ERR_CHK(rsd_sys_hz_get(&hz));

    changeTime = (jiffie * 100) / hz;

    return SYS_ERR_OK;
}
int32 lldp_remoteTableChangeTime_get(uint32 *pChangeTime)
{
    *pChangeTime = changeTime;

    return SYS_ERR_OK;
}
static int32 lldp_linkevent_handler(uint32 event, char *pData)
{
    uint8 i;
    sys_port_event_t *pPortEvent = NULL;
    sys_lldp_tlv_change_event_t tlvEvent;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pPortEvent = (sys_port_event_t *)pData;

    if (SYS_NT_PORT_LINK_CHG == event)
    {
        port_table.port_entry[pPortEvent->lport].link_status = pPortEvent->new_linkStatus;

        switch (port_table.port_entry[pPortEvent->lport].link_status)
        {
            case LLDP_LINKUP:
                LLDP_DBG("Port[%s]:LINK UP",LPSTR(pPortEvent->lport));
              #ifdef CONFIG_SYS_PROTO_LLDP_MED
                if(ENABLED == port_table.port_entry[pPortEvent->lport].medEnable)
                    port_table.port_entry[pPortEvent->lport].fast_repeat_remain = med_fast_repeat_count;
              #endif
                break;

            case LLDP_LINKDOWN:
                LLDP_DBG("Port[%s]:LINK DWON", LPSTR(pPortEvent->lport));
                port_table.port_entry[pPortEvent->lport].tx_timer = 0;
                port_table.port_entry[pPortEvent->lport].reinitdelay_timer = reinit_delay;

                if (!IS_TRKLP(pPortEvent->lport))
                {
                    for (i = 0 ; i < LLDP_MAX_ENTRY_PER_PORT; i++ ) 
                    {
                        lldp_db.entry[pPortEvent->lport][i].time_tlv_value = 0; 
                        lldp_db.entry[pPortEvent->lport][i].TTL = 0;
                    }
                }

                break;

            default:
                break;
        }
    }
    else if (SYS_NT_PORT_DESCRIPTION_CHG == event)
    {
        osal_memset(&tlvEvent, 0, sizeof(tlvEvent));
        tlvEvent.isAllPort = FALSE;
        tlvEvent.port = pPortEvent->lport;
        tlvEvent.type = LLDP_TLV_NOTIFY_PORT_DESCRIPTION;
        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);
    }

    return SYS_ERR_OK;
}
static int32 lldp_trunkEvent_handler(uint32 event, char *pData)
{
    sys_trunk_event_t *pTrunkEvent;
    sys_lldp_tlv_change_event_t tlvEvent;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pTrunkEvent = (sys_trunk_event_t *)pData;

    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));
    tlvEvent.isAllPort = TRUE;
    tlvEvent.type = LLDP_TLV_NOTIFY_TRUNK;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
static int32 lldp_vlanEvent_handler(uint32 event, char *pData)
{
    sys_vlan_event_t *pVlanEvent;
    sys_lldp_tlv_change_event_t tlvEvent;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pVlanEvent = (sys_vlan_event_t *)pData;

    if(SYS_NT_VLAN_OPERPVID_CHG == event)
    {
        osal_memset(&tlvEvent, 0, sizeof(tlvEvent));
        tlvEvent.isAllPort = FALSE;
        tlvEvent.port = pVlanEvent->port;
        tlvEvent.type = LLDP_TLV_NOTIFY_PVID;
        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);
    }

    return SYS_ERR_OK;
}
static int32 lldp_systemEvent_handler(uint32 event, char *pData)
{
    sys_lldp_tlv_change_event_t tlvEvent;
    sys_system_event_t *sysEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));

    if (SYS_NT_SYS_NAME_CHG == event)
    {
        tlvEvent.isAllPort = TRUE;
        tlvEvent.type = LLDP_TLV_NOTIFY_SYSTEM_NAME;
        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);
    }
    else if (SYS_NT_SYS_DESCRIPTION_CHG == event)
    {
        tlvEvent.isAllPort = TRUE;
        tlvEvent.type = LLDP_TLV_NOTIFY_SYSTEM_DESCRIPTION;
        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);
    }
    else if (SYS_NT_SYS_MGMT_IP_CHG == event)
    {
        tlvEvent.isAllPort = TRUE;
        tlvEvent.port = 0;
        tlvEvent.type = LLDP_TLV_NOTIFY_MANAGEMENT_ADDR;
        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);
    }
    else if (SYS_NT_SYS_PORT_MAX_FRAME_CHG == event)
    {
        if (NULL != pData)
            sysEvent = (sys_system_event_t *)pData;
        else
            return SYS_ERR_NULL_POINTER;

        tlvEvent.isAllPort = FALSE;
        tlvEvent.port = sysEvent->portMaxFrame.port;
        tlvEvent.type = LLDP_TLV_NOTIFY_MAX_FRAME_SIZE;
        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);
    }

    return SYS_ERR_OK;
}
static int32 lldp_tlvChangeEvent_handler(uint32 event, char *pData)
{
    sys_lldp_tlv_change_event_t *pTlvEvent = NULL;
    sys_logic_port_t lport;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pTlvEvent = (sys_lldp_tlv_change_event_t *)pData;

    if(pTlvEvent->isAllPort == TRUE)
    {
        FOR_EACH_NORMAL_LOGIC_PORT(lport)
            tlvChanged[lport] = TRUE;
    }
    else
    {
        if(FALSE == tlvChanged[pTlvEvent->port])
            switch(pTlvEvent->type)
            {
                case LLDP_TLV_NOTIFY_SYSTEM_NAME:
                    if(TRUE == port_table.port_entry[pTlvEvent->port].lldpTlvSel.sysNameSel)
                        tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_SYSTEM_DESCRIPTION:
                    if(TRUE == port_table.port_entry[pTlvEvent->port].lldpTlvSel.sysDescriptionSel)
                        tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_PORT_DESCRIPTION:
                    if(TRUE == port_table.port_entry[pTlvEvent->port].lldpTlvSel.portDesciptionSel)
                        tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_MANAGEMENT_ADDR:
                    if(TRUE == port_table.port_entry[pTlvEvent->port].lldpTlvSel.managementAddrSel)
                        tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_TRUNK:
                    if(TRUE == port_table.port_entry[pTlvEvent->port].lldpTlvSel.linkAggregationSel)
                        tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_MAX_FRAME_SIZE:
                    if(TRUE == port_table.port_entry[pTlvEvent->port].lldpTlvSel.maxFrameSizeSel)
                        tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_PVID:
                    if(TRUE == port_table.port_entry[pTlvEvent->port].tlvPvidEbl)
                        tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_VLAN_NAME:
                    tlvChanged[pTlvEvent->port] = TRUE;
                    break;
              #ifdef CONFIG_SYS_PROTO_LLDP_MED
                case LLDP_TLV_NOTIFY_EXTEND_POE:
                    if(ENABLED == port_table.port_entry[pTlvEvent->port].medEnable)
                        if(TRUE == port_table.port_entry[pTlvEvent->port].lldpMedTlvSel.poePseSel)
                            tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_NETWORK_POLICY:
                    if(ENABLED == port_table.port_entry[pTlvEvent->port].medEnable)
                        if(TRUE == port_table.port_entry[pTlvEvent->port].lldpMedTlvSel.networkPolicySel)
                            tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_INVENTORY:
                    if(ENABLED == port_table.port_entry[pTlvEvent->port].medEnable)
                        if(TRUE == port_table.port_entry[pTlvEvent->port].lldpMedTlvSel.inventorySel)
                            tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_LOCATION:
                    if(ENABLED == port_table.port_entry[pTlvEvent->port].medEnable)
                        if(TRUE == port_table.port_entry[pTlvEvent->port].lldpMedTlvSel.locationSel)
                            tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_VOICE_AUTO_EBL:
                    if(ENABLED == port_table.port_entry[pTlvEvent->port].medEnable)
                        if(TRUE == port_table.port_entry[pTlvEvent->port].lldpMedTlvSel.networkPolicySel)
                            tlvChanged[pTlvEvent->port] = TRUE;
                    break;
                case LLDP_TLV_NOTIFY_VOICE_AUTO_CONTENT:
                    if(ENABLED == port_table.port_entry[pTlvEvent->port].medEnable)
                        if((TRUE == port_table.port_entry[pTlvEvent->port].lldpMedTlvSel.networkPolicySel) && (TRUE == networkPolicyAuto))
                            tlvChanged[pTlvEvent->port] = TRUE;
                    break;
              #endif 
                case LLDP_TLV_NOTIFY_TLV_ADD_RMV:
                    tlvChanged[pTlvEvent->port] = TRUE;
                    break;
              #ifdef CONFIG_SYS_PROTO_LLDP_MED
                case LLDP_TLV_NOTIFY_MED_TLV_ADD_RMV:
                    tlvChanged[pTlvEvent->port] = TRUE;
                    break;
              #endif 
                default:
                    break;
            }
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED
  #ifdef CONFIG_SYS_VOICEVLAN

static int32 lldp_voiceVlanEvent_handler(uint32 event, char *pData)
{
    sys_voice_vlan_event_t  *pEvent = NULL;
    sys_lldp_tlv_change_event_t tlvEvent;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pEvent = (sys_voice_vlan_event_t *)pData;

    osal_memset(&tlvEvent, 0, sizeof(tlvEvent));
    tlvEvent.isAllPort = TRUE;
    tlvEvent.type = LLDP_TLV_NOTIFY_VOICE_AUTO_CONTENT;
    sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, (char*)&tlvEvent);

    return SYS_ERR_OK;
}
  #endif  
#endif 
#ifdef CONFIG_SYS_MIRROR
static int32 lldp_mirrorEvent_handler(uint32 event, char *pData)
{
    sys_mirror_event_t *pTlvEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pTlvEvent = (sys_mirror_event_t *)pData;

    if(TRUE == pTlvEvent->mirrorEntry.mirroring_port_iso)
    {
        mirroringPort[pTlvEvent->gid] = pTlvEvent->mirrorEntry.mirroring_port;
    }
    else
    {
        mirroringPort[pTlvEvent->gid] = SYS_LOGIC_PORT_NUM;
    }

    return SYS_ERR_OK;
}
#endif
static int32 lldp_802dot3Tlv_handle(uint8 *pLldp, lldp_tlv_t  *pPkt_tlv, neighbor_entry_t *pRx_entry)
{
    uint8 subType;

    subType = HTONS(*pLldp);
    pLldp = (uint8 *)(pLldp + SUB_TYPE_LEN);

    switch(subType)
    {
        case SUB_TYPE_MAC_PHY:
            pRx_entry->macPhy_tlv.autoNego = *pLldp;
            pLldp = (uint8 *)(pLldp + AUTO_NEGO_STATS_LEN);

            pRx_entry->macPhy_tlv.capa = HTONS(READ16(pLldp));
            pLldp = (uint8 *)(pLldp + AUTO_NEGO_CAP_LEN);

            pRx_entry->macPhy_tlv.oper = HTONS(READ16(pLldp));
            pLldp = (uint8 *)(pLldp + OPER_TYPE_LEN);

            pRx_entry->macPhy_tlv.valid = TRUE;
            break;

        case SUB_TYPE_AGG:
            pRx_entry->trunk_tlv.aggInfo = *pLldp;
            pLldp = (uint8 *)(pLldp + AGG_STATS_LEN);

            pRx_entry->trunk_tlv.aggId = HTONS(READ32(pLldp));
            pLldp = (uint8 *)(pLldp + AGG_ID_LEN);

            pRx_entry->trunk_tlv.valid = TRUE;
            break;

        case SUB_TYPE_MAX_FRAME_SIZE:
            pRx_entry->maxFrameSize_tlv.maxFrameSize = HTONS(READ16(pLldp));
            pLldp = (uint8 *)(pLldp + MAX_FRAME_SIZE_LEN);

            pRx_entry->maxFrameSize_tlv.valid = TRUE;
            break;
        case SUB_TYPE_802_3_POWER_VIA_MDI:
            osal_memcpy(&pRx_entry->powerViaMdi_tlv.support, pLldp, MDI_POWER_SUPPORT_LEN);
            pLldp = (uint8 *)(pLldp + MDI_POWER_SUPPORT_LEN);
            pRx_entry->powerViaMdi_tlv.portPair = *pLldp;
            pLldp = (uint8 *)(pLldp + PSE_POWER_PAIR_LEN);
            pRx_entry->powerViaMdi_tlv.powerClass = *pLldp;
            pLldp = (uint8 *)(pLldp + POWER_CLASS_LEN);
            pRx_entry->powerViaMdi_tlv.valid = TRUE;
            if(pPkt_tlv->tlv_info_len > 7)
            {
                pRx_entry->powerViaMdi_tlv.extValid = TRUE;
                osal_memcpy(&pRx_entry->powerViaMdi_tlv.powerInfo, pLldp, POWER_INFO_LEN);
                pLldp = (uint8 *)(pLldp + POWER_INFO_LEN);

                pRx_entry->powerViaMdi_tlv.requestPower = HTONS(READ16(pLldp));
                pLldp = (uint8 *)(pLldp + POWER_VAL_LEN);

                pRx_entry->powerViaMdi_tlv.allocatePower = HTONS(READ16(pLldp));
                pLldp = (uint8 *)(pLldp + POWER_VAL_LEN);
                pRx_entry->powerAllocBy = 1;
            }
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}
static int32 lldp_vlanNameTlv_clear(lldp_vlanName_tlv_t *pVlanNameTlv)
{
    lldp_vlanName_tlv_t *pNow = NULL, *pNext = NULL;

    pVlanNameTlv->valid = FALSE;

    pNext = pVlanNameTlv->pNext;

    while(pNext != NULL)
    {
        pNow = pNext;
        pNext = pNext->pNext;
        osal_free(pNow);
    }

    return SYS_ERR_OK;
}

static int32 lldp_vlanNameTlv_add(lldp_vlanName_tlv_t *pVlanNameTlv, uint16 vid, uint8 len, char *name)
{
    lldp_vlanName_tlv_t *pNow = NULL, *pNext = NULL;

    if(FALSE == pVlanNameTlv->valid)
    {
        pVlanNameTlv->valid = TRUE;
        pVlanNameTlv->vid = vid;
        pVlanNameTlv->len = len;
        osal_strcpy(pVlanNameTlv->name.str, name);
    }
    else
    {
        pNow =  pVlanNameTlv;
        pNext = pVlanNameTlv->pNext;
        while(pNext != NULL && pNow->valid == TRUE)
        {
            pNow = pNext;
            pNext = pNext->pNext;
        }
        if(pNow->valid == FALSE)
        {
            pNow->valid = TRUE;
            pNow->vid = vid;
            pNow->len = len;
            osal_strcpy(pNow->name.str, name);
        }
        else if(pNext == NULL)
        {
            pNow->pNext = osal_alloc(sizeof(lldp_vlanName_tlv_t));
            pNow = pNow->pNext;
            pNow->valid = TRUE;
            pNow->vid = vid;
            pNow->len = len;
            osal_strcpy(pNow->name.str, name);
            pNow->pNext = NULL;
        }
    }
    return SYS_ERR_OK;
}
static int32 lldp_802dot1Tlv_handle(uint8 *pLldp, lldp_tlv_t  *pPkt_tlv, neighbor_entry_t *pRx_entry)
{
    uint8 subType;
    uint16 vid;
    uint8 len;
    char vlanName[CAPA_NAME_STR_LEN + 1];

    subType = HTONS(*pLldp);
    pLldp = (uint8 *)(pLldp + SUB_TYPE_LEN);

    switch(subType)
    {
        case SUB_TYPE_PORT_VLAN_ID:
            pRx_entry->pvid_tlv.pvid = HTONS(READ16(pLldp));
            pLldp = (uint8 *)(pLldp + PORT_VLAN_ID_LEN);

            pRx_entry->pvid_tlv.valid = TRUE;
            break;
        case SUB_TYPE_PORT_VLAN_NAME:
            vid = HTONS(READ16(pLldp));
            pLldp = (uint8 *)(pLldp + VLAN_ID_LEN);
            len = *pLldp;
            pLldp = (uint8 *)(pLldp + VLAN_NAME_LEN_LEN);
            osal_memset(vlanName, 0, sizeof(vlanName));
            if(len < CAPA_NAME_STR_LEN + 1)
                osal_memcpy(vlanName, pLldp, len);
            else
                osal_memcpy(vlanName, pLldp, CAPA_NAME_STR_LEN);
            SYS_ERR_CHK(lldp_vlanNameTlv_add(&pRx_entry->vlanName_tlv, vid, len, vlanName));
            pRx_entry->vlanNum ++;
            break;
        default:
            break;
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED

static int32 lldp_tiaTlv_handle(uint8 *pLldp, lldp_tlv_t  *pPkt_tlv, neighbor_entry_t *pRx_entry)
{
    uint8 subType;
    uint8 locationFmt;
    uint8 idx;
  #ifdef CONFIG_SYS_VOICEVLAN
    voice_vlan_state_t state;
    sys_vid_t voiceVid;
    sys_vid_t defaultVid;
  #endif

    subType = HTONS(*pLldp);
    pLldp = (uint8 *)(pLldp + SUB_TYPE_LEN);

    switch(subType)
    {
        case SUB_TYPE_MEDIA_CAPA:
            pRx_entry->media_capa_tlv.capa = HTONS(READ16(pLldp));
            pLldp = (uint8 *)(pLldp + MEDIA_CAPA_LEN);

            pRx_entry->media_capa_tlv.classType = *pLldp;
            pLldp = (uint8 *)(pLldp + CLASS_TYPE_LEN);

            pRx_entry->media_capa_tlv.valid = TRUE;
            break;

        case SUB_TYPE_LOCATION:
            locationFmt = *pLldp;
            pLldp = (uint8 *)(pLldp + LOCATION_DATA_FMT_LEN);

            if(LOCATION_DATA_FMT_COORDINATE == locationFmt)
            {
                pRx_entry->location_tlv.location.coorLen = pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUB_TYPE_LEN - LOCATION_DATA_FMT_LEN;

                osal_memcpy(pRx_entry->location_tlv.location.coordinate, pLldp, pRx_entry->location_tlv.location.coorLen);

                pLldp = (uint8 *)(pLldp + pRx_entry->location_tlv.location.coorLen);
            }
            else if(LOCATION_DATA_FMT_CIVIC_ADDR == locationFmt)
            {
                pRx_entry->location_tlv.location.civicLen = pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUB_TYPE_LEN - LOCATION_DATA_FMT_LEN;

                osal_memcpy(pRx_entry->location_tlv.location.civicAddress, pLldp, pRx_entry->location_tlv.location.civicLen);

                pLldp = (uint8 *)(pLldp + pRx_entry->location_tlv.location.civicLen);
            }
            else if(LOCATION_DATA_FMT_ECS_ELIN == locationFmt)
            {
                pRx_entry->location_tlv.location.ecsLen = pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUB_TYPE_LEN - LOCATION_DATA_FMT_LEN;

                osal_memcpy(pRx_entry->location_tlv.location.ecsElin, pLldp, pRx_entry->location_tlv.location.ecsLen);

                pLldp = (uint8 *)(pLldp + pRx_entry->location_tlv.location.ecsLen);
            }
            pRx_entry->location_tlv.valid = TRUE;
            break;

        case SUB_TYPE_NETWORK_POLICY:
          #ifdef CONFIG_SYS_VOICEVLAN
            voice_vlan_state_get(&state);
            rsd_vlan_voiceVid_get(&voiceVid);
            rsd_vlan_defaultVid_get(&defaultVid);
          #endif

            for(idx = 0; idx < MAX_LLDP_MED_NETWORK_POLICY_RX; idx ++)
            {
                if(FALSE == pRx_entry->policy_tlv[idx].valid)
                {
                    pRx_entry->policy_tlv[idx].app = *pLldp;
                    pLldp = (uint8 *)(pLldp + APP_TYPE_LEN);

                    osal_memcpy(&pRx_entry->policy_tlv[idx].policyInfo, pLldp, POLICY_LEN);
                    pLldp = (uint8 *)(pLldp + POLICY_LEN);

                    pRx_entry->policy_tlv[idx].valid = TRUE;
                  #ifdef CONFIG_SYS_VOICEVLAN
                    if((LLDP_APP_VOICE == pRx_entry->policy_tlv[idx].app) &&
                        (pRx_entry->policy_tlv[idx].policyInfo.vlanId == voiceVid) &&
                        (voiceVid != defaultVid) &&
                        (VOICE_VLAN_AUTO_ENABLED == state))
                        pRx_entry->isVoiceVidMatch = TRUE;
                  #endif
                    break;
                }
            }
            break;
        case SUB_TYPE_EXT_POWER_VIA_MDI:
            osal_memcpy(&pRx_entry->poe_tlv.powerInfo, pLldp, POWER_INFO_LEN);
            pLldp = (uint8 *)(pLldp + POWER_INFO_LEN);

            pRx_entry->poe_tlv.powerVal = HTONS(READ16(pLldp));
            pLldp = (uint8 *)(pLldp + POWER_VAL_LEN);

            pRx_entry->powerAllocBy = 2;

            pRx_entry->poe_tlv.valid = TRUE;

            break;

        case LLDP_INVENTORY_TYPE_HARDWARE_REVISION:
            osal_memcpy(pRx_entry->hardware_v_tlv.strInventory, pLldp,
                (pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET));
            pRx_entry->hardware_v_tlv.strInventory[pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET] = '\0';

            pRx_entry->hardware_v_tlv.valid = TRUE;
            break;
        case LLDP_INVENTORY_TYPE_FIRMWARE_REVISION:
            osal_memcpy(pRx_entry->firmware_v_tlv.strInventory, pLldp,
                (pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET));
            pRx_entry->firmware_v_tlv.strInventory[pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET] = '\0';

            pRx_entry->firmware_v_tlv.valid = TRUE;
            break;
        case LLDP_INVENTORY_TYPE_SOFTWARE_REVISION:
            osal_memcpy(pRx_entry->software_v_tlv.strInventory, pLldp,
                (pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET));
            pRx_entry->software_v_tlv.strInventory[pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET] = '\0';

            pRx_entry->software_v_tlv.valid = TRUE;
            break;
        case LLDP_INVENTORY_TYPE_SERIA_NUMBER:
            osal_memcpy(pRx_entry->serial_num_tlv.strInventory, pLldp,
                (pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET));
            pRx_entry->serial_num_tlv.strInventory[pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET] = '\0';

            pRx_entry->serial_num_tlv.valid = TRUE;
            break;
        case LLDP_INVENTORY_TYPE_MANUFACTURER:
            osal_memcpy(pRx_entry->manufacturer_tlv.strInventory, pLldp,
                (pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET));
            pRx_entry->manufacturer_tlv.strInventory[pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET] = '\0';

            pRx_entry->manufacturer_tlv.valid = TRUE;
            break;
        case LLDP_INVENTORY_TYPE_MODEL_NAME:
            osal_memcpy(pRx_entry->model_name_tlv.strInventory, pLldp,
                (pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET));
            pRx_entry->model_name_tlv.strInventory[pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET] = '\0';

            pRx_entry->model_name_tlv.valid = TRUE;
            break;
        case LLDP_INVENTORY_TYPE_ASSET_ID:
            osal_memcpy(pRx_entry->asset_tlv.strInventory, pLldp,
                (pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET));
            pRx_entry->asset_tlv.strInventory[pPkt_tlv->tlv_info_len - ORG_UNIQUE_CODE_LEN - SUBTYPE_OFFSET] = '\0';

            pRx_entry->asset_tlv.valid = TRUE;
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}
#endif 
static int32 lldp_orgSpecificTlv_handle(uint8 *pLldp, lldp_tlv_t  *pPkt_tlv, neighbor_entry_t *pRx_entry)
{
        uint32 orgCode;

        orgCode = HTONS(READ24(pLldp));
        pLldp = (uint8 *)(pLldp + ORG_UNIQUE_CODE_LEN);

        switch(orgCode)
        {
            case ORG_UNIQUE_CODE_802_3:
                SYS_ERR_CHK(lldp_802dot3Tlv_handle(pLldp, pPkt_tlv, pRx_entry));
                break;
            case ORG_UNIQUE_CODE_802_1:
                SYS_ERR_CHK(lldp_802dot1Tlv_handle(pLldp, pPkt_tlv, pRx_entry));
                break;
            case ORG_UNIQUE_CODE_TIA:
              #ifdef CONFIG_SYS_PROTO_LLDP_MED
                SYS_ERR_CHK(lldp_tiaTlv_handle(pLldp, pPkt_tlv, pRx_entry));
              #endif
                break;
            default:
                break;
        }

        return SYS_ERR_OK;
}

static int32 lldp_portPktTxAvoid_get(sys_logic_port_t port, uint8 *pAvoid)
{
  #ifdef CONFIG_SYS_MIRROR
    uint32              mirror_id;
  #endif

    *pAvoid = FALSE;

  #ifdef CONFIG_SYS_MIRROR
    for (mirror_id = 0; mirror_id < CAPA_MIRROR_SESSION_NUM; ++mirror_id)
    {
        if (port == mirroringPort[mirror_id])
        {
            *pAvoid = TRUE;
            return SYS_ERR_OK;
        }
    }
  #endif

    return SYS_ERR_OK;
}
static int32 lldp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    int32 ret = SYS_ERR_OK;
    uint8 *pLldp = NULL;
    uint32 tlvCounter = 0, j = 0;
    uint8 badframe = 0, badButAcceptFrame = 0;
    uint8 finish_flag = 0;
    uint8 port;
    uint16 ether_type;
    uint16 tlv_header = 0;
    neighbor_entry_t *pRx_entry = NULL;
    lldp_tlv_t  pkt_tlv;
    uint16 remainLen;
    uint8 ipIndex = 0;
    uint8 *pLldpTmp = NULL;
    char ipStr[CAPA_IPV4_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];

    osal_memset(ipStr, 0, sizeof(ipStr));
    osal_memset(macStr, 0, sizeof(macStr));

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);

    SYS_MEM_ALLOC_HDL(pRx_entry, sizeof(neighbor_entry_t), _exit, ret);

    if(ENABLED == lldp_admin_enabled)
    {
        osal_memset(pRx_entry,0,sizeof(neighbor_entry_t));
        port = pPkt->rx_tag.source_port;
        pRx_entry->port_index = port;

        remainLen = pPkt->length;

        LLDP_RX_DBG("Receive LLDP PDU from port %s pakect length:%d", LPSTR(pRx_entry->port_index), remainLen);

        if ((DISABLED == port_table.port_entry[port].rx_enabled) || (DISABLED == lldp_admin_enabled))
        {
            SYS_ERR_HDL(rsd_nic_pkt_free(pPkt), _exit, ret);

            ret = SYS_ERR_OK;

            goto _exit;
        }

        lldp_dump_packet(pPkt->data, pPkt->length, FALSE, port);

        pLldp = (uint8 *)(pPkt->data + MAC_HEADER_LEN);
        remainLen -= MAC_HEADER_LEN;

        if (pPkt->rx_tag.cvid_tagged)
        {
            pLldp = (pLldp + VLANID_OFFSET);
            remainLen -= VLANID_OFFSET;
        }

        if (pPkt->rx_tag.svid_tagged)
        {
            pLldp = (pLldp + VLANID_OFFSET);
            remainLen -= VLANID_OFFSET;
        }

        ether_type = HTONS(READ16(pLldp));

        if (LLDP_ETHERTYPE != ether_type)
        {
            LLDP_RX_DBG("Eror: wrong ETHERNET TYPE IN LLDP!");
            SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_ETHERTYPE_INVALID, LPSTR(pRx_entry->port_index));

            SYS_ERR_HDL(rsd_nic_pkt_free(pPkt), _exit, ret);

            ret = SYS_ERR_OK;
            goto _exit;
        }

        lldp_statistic.port_statistics[port].rx_frame ++;

        pLldp = (uint8 *)(pLldp + ETHERTYPE_OFFSET);
        remainLen -= ETHERTYPE_OFFSET;

        pLldpTmp = pLldp;

        while (remainLen > 0)
        {
            osal_memset(&pkt_tlv, 0, sizeof(pkt_tlv));
            tlv_header = HTONS(READ16(pLldp));

            pkt_tlv.tlv_info_len = tlv_header & 0x01FF;

            pkt_tlv.tlv_type = tlv_header >> 9;

            if (check_tlv_len(remainLen, pkt_tlv.tlv_info_len, pkt_tlv.tlv_type) == XEINVALIDTLV)
            {
                LLDP_RX_DBG("tlvCounter =%d tlv_type = %d tlv_info_len ERROR , badframe",tlvCounter ,pkt_tlv.tlv_type);
                SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_LEN_INVALID, LPSTR(pRx_entry->port_index));
                badframe = 1;
                lldp_statistic.port_statistics[port].rx_discard_tlv ++;
                SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TLV_DROP, LPSTR(pRx_entry->port_index));
                break;
            }

            remainLen -= pkt_tlv.tlv_info_len + TLVH_OFFSET;

            switch (pkt_tlv.tlv_type)
            {
                case CHASSIS_ID_TLV: 
                    pLldp = (uint8 *)(pLldp + TLVH_OFFSET);
                    pkt_tlv.sub_type = *pLldp;

                    pLldp = (uint8 *)(pLldp + SUBTYPE_OFFSET);

                    if (CHASSIS_ID_MAC_ADDRESS == pkt_tlv.sub_type)
                    {
                        osal_memcpy(pkt_tlv.tlv_mac, pLldp, CAPA_MAC_BYTE_NUM);
                        sys_util_mac2Str(pkt_tlv.tlv_mac, macStr);
                        osal_sprintf(pkt_tlv.tlv_string, "%s", macStr);
                    }
                    else if (CHASSIS_ID_NETWORK_ADDRESS == pkt_tlv.sub_type)
                    {
                        pkt_tlv.tlv_ip = HTONS(READ32(pLldp + 1));
                        sys_util_ip2Str(pkt_tlv.tlv_ip, ipStr);
                        osal_sprintf(pkt_tlv.tlv_string, "%s", ipStr);
                    }
                    else
                    {
                        osal_memcpy(pkt_tlv.tlv_string, pLldp, (pkt_tlv.tlv_info_len - SUBTYPE_OFFSET));
                        pkt_tlv.tlv_string[pkt_tlv.tlv_info_len -1] = '\0';
                    }

                    pLldp = (uint8 *)(pLldp + (pkt_tlv.tlv_info_len - SUBTYPE_OFFSET));

                    if (0 != tlvCounter)
                    {
                        badframe = 1;
                        lldp_statistic.port_statistics[port].rx_discard_tlv ++;
                        SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TLV_DROP, LPSTR(pRx_entry->port_index));
                    }
                    else
                    {
                        
                        osal_memcpy(&pRx_entry->chasis_id_tlv, &pkt_tlv, sizeof(lldp_tlv_t));
                    }

                    break;

               case PORT_ID_TLV:    
                    pLldp = (uint8 *)(pLldp + TLVH_OFFSET);
                    pkt_tlv.sub_type = *pLldp;
                    pLldp = (uint8 *)(pLldp + SUBTYPE_OFFSET);

                    if (PORT_ID_MAC_ADDRESS == pkt_tlv.sub_type)
                    {
                        osal_memcpy(pkt_tlv.tlv_mac, pLldp, CAPA_MAC_BYTE_NUM);
                        sys_util_mac2Str(pkt_tlv.tlv_mac, macStr);
                        osal_sprintf(pkt_tlv.tlv_string, "%s", macStr);
                    }
                    else if (PORT_ID_NETWORK_ADDRESS == pkt_tlv.sub_type)
                    {
                        pkt_tlv.tlv_ip = HTONS(READ32(pLldp + 1));
                        sys_util_ip2Str(pkt_tlv.tlv_ip, ipStr);
                        osal_sprintf(pkt_tlv.tlv_string, "%s", ipStr);
                    }
                    else
                    {
                        osal_memcpy(pkt_tlv.tlv_string, pLldp, (pkt_tlv.tlv_info_len - SUBTYPE_OFFSET));
                        pkt_tlv.tlv_string[pkt_tlv.tlv_info_len -1] = '\0';
                    }

                    pLldp = (uint8 *)(pLldp + (pkt_tlv.tlv_info_len - SUBTYPE_OFFSET));

                    if (1 != tlvCounter)
                    {
                        badframe = 1;
                        lldp_statistic.port_statistics[port].rx_discard_tlv ++;
                        SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TLV_DROP, LPSTR(pRx_entry->port_index));
                    }
                    else
                    {
                        osal_memcpy(&pRx_entry->port_id_tlv, &pkt_tlv, sizeof(lldp_tlv_t));
                    }

                    break;

               case TIME_TO_LIVE_TLV: 
                    pLldp = (uint8 *)(pLldp + TLVH_OFFSET);
                    pkt_tlv.tlv_value = HTONS(READ16(pLldp));

                    pLldp = (uint8 *)(pLldp + TTL_TLV_LEN);

                    if ((TIME_TO_LIVE_TLV == pkt_tlv.tlv_type) && (2 != tlvCounter))
                    {
                        badframe = 1;
                        lldp_statistic.port_statistics[port].rx_discard_tlv ++;
                        SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TLV_DROP, LPSTR(pRx_entry->port_index));
                    }

                    if (TIME_TO_LIVE_TLV == pkt_tlv.tlv_type)
                    {
                        pRx_entry->TTL = pkt_tlv.tlv_value;
                        pRx_entry->time_tlv_value = pkt_tlv.tlv_value;
                    }

                    break;

               case PORT_DESCRIPTION_TLV:   
               case SYSTEM_NAME_TLV:        
               case SYSTEM_DESCRIPTION_TLV: 
                    pLldp = (uint8 *)(pLldp + TLVH_OFFSET);
                    osal_memcpy(pkt_tlv.tlv_string, pLldp, pkt_tlv.tlv_info_len);
                    pkt_tlv.tlv_string[pkt_tlv.tlv_info_len] = '\0';

                    pLldp = (uint8 *)(pLldp + pkt_tlv.tlv_info_len);

                    if (PORT_DESCRIPTION_TLV == pkt_tlv.tlv_type)
                    {
                       osal_strcpy(pRx_entry->port_description , pkt_tlv.tlv_string);
                    }

                    if (SYSTEM_NAME_TLV == pkt_tlv.tlv_type)
                    {
                       osal_memcpy(&pRx_entry->system_name , &pkt_tlv.tlv_string ,osal_strlen(pkt_tlv.tlv_string));
                    }

                    if (SYSTEM_DESCRIPTION_TLV == pkt_tlv.tlv_type)
                    {
                       osal_memcpy(&pRx_entry->system_description , &pkt_tlv.tlv_string ,osal_strlen(pkt_tlv.tlv_string));
                    }

                    break;

                case SYSTEM_CAPABILITIES_TLV: 
                    pLldp = (uint8 *)(pLldp + TLVH_OFFSET);
                    pkt_tlv.tlv_value = HTONS(READ16(pLldp));
                    pLldp = (uint8 *)(pLldp + CAPA_TLV_OFFSET);
                    pkt_tlv.sub_value = HTONS(READ16(pLldp));

                    pLldp = (uint8 *)(pLldp + CAPA_TLV_OFFSET);

                    if (0 != (pkt_tlv.sub_value & ~(pkt_tlv.tlv_value))) 
                    {
                        badButAcceptFrame = 1;
                        lldp_statistic.port_statistics[port].rx_discard_tlv ++;
                        SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TLV_DROP, LPSTR(pRx_entry->port_index));
                        pkt_tlv.tlv_value = 1;
                        pkt_tlv.sub_value = 1; 
                    }

                    osal_memcpy(&pRx_entry->sys_capa_tlv, &pkt_tlv, sizeof(lldp_tlv_t));
                    break;

                case MANAGEMENT_ADDRESS_TLV: 
                    ipIndex = 0xff;

                    for(j = 0; j < MAX_LLDP_MANAGEMENT_ADDR_NUM; j ++)
                    {
                        if(FALSE == pRx_entry->manage_address[j].valid)
                        {
                            ipIndex = j;
                            break;
                        }
                    }

                    if(0xff != ipIndex)
                    {
                        uint8 oidSkipLen = 0;

                        pLldp = (uint8 *)(pLldp + TLVH_OFFSET);
                        pRx_entry->manage_address[ipIndex].len = *pLldp;
                        pLldp = (uint8 *)(pLldp + 1);
                        pRx_entry->manage_address[ipIndex].iSubtype = pkt_tlv.sub_type = *pLldp;
                        if(1 != pkt_tlv.sub_type && 2 != pkt_tlv.sub_type) 
                        {
                            lldp_statistic.port_statistics[port].rx_unRecognize_tlv ++;
                            SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TLV_UNRECOGNIZE, LPSTR(pRx_entry->port_index));
                            pLldp = (uint8 *)(pLldp + pkt_tlv.tlv_info_len - 1);
                            break;
                        }

                        pLldp = (uint8 *)(pLldp + 1);

                        if(pkt_tlv.sub_type == 1)
                        {
                            osal_memcpy(pRx_entry->manage_address[ipIndex].addr, pLldp, 4);
                            pLldp = (uint8 *)(pLldp + 4);
                            oidSkipLen = 11;
                        }
                        else if(pkt_tlv.sub_type == 2)
                        {
                            osal_memcpy(pRx_entry->manage_address[ipIndex].addr, pLldp, 16);
                            pLldp = (uint8 *)(pLldp + 16);
                            oidSkipLen = 23;
                        }

                        pRx_entry->manage_address[ipIndex].interfaceSubtype = *pLldp;
                        pLldp = (uint8 *)(pLldp + 1);
                        pRx_entry->manage_address[ipIndex].interfaceNum = READ32(pLldp);
                        pLldp = (uint8 *)(pLldp + 4);
                        pLldp = (uint8 *)(pLldp + pkt_tlv.tlv_info_len - oidSkipLen); 
                        pRx_entry->manage_address[ipIndex].valid = TRUE;
                    }
                    else
                    {
                        pLldp = (uint8 *)(pLldp + TLVH_OFFSET);
                        pLldp = (uint8 *)(pLldp + pkt_tlv.tlv_info_len);
                    }

                    break;

                case ORG_SPECIFIC_TLV: 
                    pLldp = (uint8 *)(pLldp + TLVH_OFFSET);
                    lldp_orgSpecificTlv_handle(pLldp, &pkt_tlv, pRx_entry);

                    pLldp = (uint8 *)(pLldp + pkt_tlv.tlv_info_len);
                    break;

                case END_OF_LLDPDU_TLV: 
                    finish_flag = 1;
                    break;

                default:
                    pLldp = (uint8 *)(pLldp + TLVH_OFFSET);
                    pLldp = (uint8 *)(pLldp + pkt_tlv.tlv_info_len);
                    lldp_statistic.port_statistics[port].rx_unRecognize_tlv ++;
                    SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TLV_UNRECOGNIZE, LPSTR(pRx_entry->port_index));
                    badframe = 1;
                    lldp_statistic.port_statistics[port].rx_discard_tlv ++;
                    SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_TLV_DROP, LPSTR(pRx_entry->port_index));
                    break;
            }

            if (finish_flag || badframe)
                break;

            if (tlvCounter > 512)
                break;

            tlvCounter++;
        }
        rsd_nic_pkt_free(pPkt);

        if (badframe)
        {
            lldp_statistic.port_statistics[port].rx_discard_frame ++;
            lldp_statistic.port_statistics[port].rx_error_frame ++;
            LLDP_RX_DBG("bad frame");
            
            lldp_vlanNameTlv_clear(&pRx_entry->vlanName_tlv); 
            SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_FRAME_DROP, LPSTR(pRx_entry->port_index));
            SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_FRAME_ERROR, LPSTR(pRx_entry->port_index));
        }
        else if(0 == finish_flag)
        {
            lldp_statistic.port_statistics[port].rx_discard_frame ++;
            LLDP_RX_DBG("discard frame");
            
            lldp_vlanNameTlv_clear(&pRx_entry->vlanName_tlv); 
            SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_FRAME_DROP, LPSTR(pRx_entry->port_index));
        }
        else
        {
            if(badButAcceptFrame)
            {
                lldp_statistic.port_statistics[port].rx_error_frame ++;
                SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_FRAME_ERROR, LPSTR(pRx_entry->port_index));
            }

            update_database(pRx_entry);

            if (0 == pRx_entry->TTL)
            {
                
                lldp_statistic.port_statistics[port].rx_discard_frame ++;
                SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_FRAME_DROP, LPSTR(pRx_entry->port_index));
            }
        }

    }
    else
    {
        sys_logic_portmask_t portmask;
        uint32 trunkId;
        sys_vid_t vid = DFLT_VLAN_DFLT_VLAN;

        rsd_nic_pktVid_get(pPkt, &vid);

        if (FALSE == rsd_nic_isPortSTPForwad_ret(pPkt->rx_tag.source_port, vid))
        {
            rsd_nic_pkt_free(pPkt);

           goto  _exit;
        }

        if(LLDP_DISABLE_ACTION_FILTER == lldp_disable_action)
        {
            rsd_nic_pkt_free(pPkt);
            SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_DISABLE, "Filtering");
        }
        else if(LLDP_DISABLE_ACTION_FLOOD_TO_VLAN == lldp_disable_action)
        {

            LOGIC_PORTMASK_SET_ALL(portmask);
            LOGIC_PORTMASK_CLEAR_PORT(portmask, pPkt->rx_tag.source_port);

            rsd_trunk_port2Trunk_get(pPkt->rx_tag.source_port, &trunkId);
            if (trunkId < CAPA_LAG_NUM)
            {
                LOGIC_PORTMASK_CLEAR_PORT(portmask, TRK2LP(trunkId));
            }

            if(0 == pPkt->rx_tag.outer_vid)
            {
                rsd_vlan_portOperPvid_get(pPkt->rx_tag.source_port, &vid);
            }
            else
                vid = pPkt->rx_tag.outer_vid;

            SYS_ERR_HDL(rsd_nic_pkt_tx(pPkt, &portmask , SYS_CPU_PORT_PRIORITY_LLDP, vid, RSD_NIC_TX_CARE_VLAN | RSD_NIC_TX_CARE_DOT1X | RSD_NIC_TX_CARE_STP | RSD_NIC_TX_CARE_TRUNK), _exit, ret);
            SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_DISABLE, "Bridging");
        }
        else if(LLDP_DISABLE_ACTION_FLOOD_TO_ALL_PORTS == lldp_disable_action)
        {
            LOGIC_PORTMASK_SET_ALL(portmask);
            LOGIC_PORTMASK_CLEAR_PORT(portmask, pPkt->rx_tag.source_port);

            rsd_trunk_port2Trunk_get(pPkt->rx_tag.source_port, &trunkId);
            if (trunkId < CAPA_LAG_NUM)
            {
                LOGIC_PORTMASK_CLEAR_PORT(portmask, TRK2LP(trunkId));
            }

            SYS_ERR_HDL(rsd_nic_pkt_tx(pPkt, &portmask , SYS_CPU_PORT_PRIORITY_LLDP, 0, RSD_NIC_TX_CARE_DOT1X | RSD_NIC_TX_CARE_STP | RSD_NIC_TX_CARE_TRUNK), _exit, ret);
            SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_DISABLE, "Flooding");
        }
    }

_exit:
    SYS_MEM_FREE(pRx_entry);
    return ret;
}
static int32 lldp_pkt_tx(uint32 logicPort, uint8 *pData, int32 len)
{
    sys_nic_pkt_t *pPkt;
    sys_logic_portmask_t logic_portmask;

    LOGIC_PORTMASK_CLEAR_ALL(logic_portmask);
    LOGIC_PORTMASK_SET_PORT(logic_portmask, logicPort);

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(len, &pPkt))
    {
        osal_memcpy(pPkt->data, pData, len);
        pPkt->tail += len;
        pPkt->length = len;

        lldp_dump_packet(pPkt->data, pPkt->length, TRUE, logicPort);

        if (SYS_ERR_OK == rsd_nic_pkt_tx(pPkt, &logic_portmask, SYS_CPU_PORT_PRIORITY_LLDP, 0 , (RSD_NIC_TX_CARE_DOT1X | RSD_NIC_TX_CARE_STP | RSD_NIC_TX_CARE_TRUNK)))
        {
            
            lldp_statistic.port_statistics[logicPort].tx_frame++;
            LLDP_TX_DBG("Port %s sends out LLDP PDU successfully", LPSTR(logicPort));
        }
    }

    return SYS_ERR_OK;
}

static void lldp_pkt_tx_thread(void *pInput)
{
    sys_logic_port_t logicPort;
    uint8 avoid;

    LLDP_DBG("Tx pkt process thread start running!");

    for (;;)
    {
        FOR_EACH_LOGIC_PORT(logicPort)
        {
            if ((ENABLED == port_table.port_entry[logicPort].tx_enabled) && (ENABLED == lldp_admin_enabled))
            {
                if (LLDP_LINKUP == port_table.port_entry[logicPort].link_status)
                {
                  #ifdef CONFIG_SYS_PROTO_LLDP_MED
                    if ((((0 == port_table.port_entry[logicPort].tx_timer || TRUE == tlvChanged[logicPort])
                         && (0 == port_table.port_entry[logicPort].txdelay_timer))
                        || (port_table.port_entry[logicPort].fast_repeat_remain > 0)) &&
                        (0 == port_table.port_entry[logicPort].reinitdelay_timer)
                        )
                  #else
                    if ((((0 == port_table.port_entry[logicPort].tx_timer || TRUE == tlvChanged[logicPort])
                         && (0 == port_table.port_entry[logicPort].txdelay_timer))) &&
                        (0 == port_table.port_entry[logicPort].reinitdelay_timer)
                        )
                  #endif
                    {
                        tlvChanged[logicPort] = FALSE;

                        lldp_portPktTxAvoid_get(logicPort, &avoid);
                        if(FALSE == avoid)
                            send_lldp_packet(logicPort);
                        port_table.port_entry[logicPort].tx_timer = tx_interval;
                      #ifdef CONFIG_SYS_PROTO_LLDP_MED
                        if(port_table.port_entry[logicPort].fast_repeat_remain > 0)
                            port_table.port_entry[logicPort].fast_repeat_remain --;
                      #endif
                    }
                }
            }
        }

        if (lldp_tx_thread_status)
            break;

        osal_time_usleep(1 * 1000 * 1000); 
    }

    lldp_tx_thread_status = -1;
    osal_thread_exit(0);

    return;
} 

static void lldp_timer_thread(void *pInput)
{
    sys_logic_port_t        logicPort;

    LLDP_DBG("Timer process thread start running!");

    for (;;)
    {
        if (lldp_timer_thread_status)
            break;

        osal_time_usleep(1 * 1000 * 1000); 

        FOR_EACH_LOGIC_PORT(logicPort)
        {
            update_txdelay_time(logicPort);
            update_reinitdelay_time(logicPort);
            update_txtime(logicPort);
        }

        update_holdtime();
    }

    lldp_timer_thread_status = -1;
    osal_thread_exit(0);

    return;
} 
int32 lldp_init(void)
{
    sys_logic_port_t port;

    holdtime = DEFAULT_HOLDTIME_MUL;
    tx_delay = DEFAULT_TX_DELAY;
    tx_interval = DEFAULT_TX_INTERVAL;
    reinit_delay = DEFAULT_REINIT_DELAY;
    LLDP_TTL = DEFAULT_TX_INTERVAL * DEFAULT_HOLDTIME_MUL;

    lldp_debugPrint_sem = osal_sem_mutex_create();
    if (0 == lldp_debugPrint_sem)
    {
        return SYS_ERR_FAILED;
    }

    FOR_EACH_LOGIC_PORT(port)
    {
        lldp_vlanTlv_sem[port] = osal_sem_mutex_create();
        if (0 == lldp_vlanTlv_sem[port])
        {
            return SYS_ERR_FAILED;
        }

        port_table.port_entry[port].rx_enabled = DISABLED;
        port_table.port_entry[port].tx_enabled = DISABLED;
        port_table.port_entry[port].tx_timer = 0;
        port_table.port_entry[port].reinitdelay_timer = 0;
        port_table.port_entry[port].txdelay_timer = 0;
      #ifdef CONFIG_SYS_PROTO_LLDP_MED
        port_table.port_entry[port].fast_repeat_remain = med_fast_repeat_count;
      #endif
    }

    SYS_ERR_CHK(rsd_nic_handler_register(&l2gLLDPHandler));
    SYS_ERR_CHK(sys_notifier_observer_register("lldp", SYS_NOTIFIER_SUBJECT_PORT,
        (SYS_NT_PORT_LINK_CHG | SYS_NT_PORT_DESCRIPTION_CHG), lldp_linkevent_handler, SYS_NOTIFIER_PRI_MEDIUM));
    SYS_ERR_CHK(sys_notifier_observer_register("lldp", SYS_NOTIFIER_SUBJECT_TRUNK, (SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE), lldp_trunkEvent_handler, SYS_NOTIFIER_PRI_MEDIUM));
    SYS_ERR_CHK(sys_notifier_observer_register("lldp", SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_TLV_CHG, lldp_tlvChangeEvent_handler, SYS_NOTIFIER_PRI_MEDIUM));
    SYS_ERR_CHK(sys_notifier_observer_register("lldp", SYS_NOTIFIER_SUBJECT_VLAN, SYS_NT_VLAN_OPERPVID_CHG, lldp_vlanEvent_handler, SYS_NOTIFIER_PRI_MEDIUM));
    SYS_ERR_CHK(sys_notifier_observer_register("lldp", SYS_NOTIFIER_SUBJECT_SYSTEM,
        (SYS_NT_SYS_NAME_CHG | SYS_NT_SYS_DESCRIPTION_CHG | SYS_NT_SYS_MGMT_IP_CHG | SYS_NT_SYS_PORT_MAX_FRAME_CHG),
        lldp_systemEvent_handler, SYS_NOTIFIER_PRI_MEDIUM));
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
   #ifdef CONFIG_SYS_VOICEVLAN
    SYS_ERR_CHK(sys_notifier_observer_register("lldp", SYS_NOTIFIER_SUBJECT_VLAN, SYS_NT_VLAN_VOICE_VID_CHG | SYS_NT_VLAN_VOICE_QOS_CHG, lldp_voiceVlanEvent_handler, SYS_NOTIFIER_PRI_MEDIUM));
   #endif  
  #endif 
  #ifdef CONFIG_SYS_MIRROR
  {
    rsd_mirror_entry_t  mirror_entry;
    uint32              mirror_id;

    for (mirror_id = 0; mirror_id < CAPA_MIRROR_SESSION_NUM; ++mirror_id)
    {
        SYS_ERR_CHK(rsd_mirror_group_get(mirror_id, &mirror_entry));

        mirroringPort[mirror_id] = mirror_entry.mirroring_port;
    }
    SYS_ERR_CHK(sys_notifier_observer_register("lldp", SYS_NOTIFIER_SUBJECT_MIRROR, SYS_NT_MIRROR_GROUP_CHG, lldp_mirrorEvent_handler, SYS_NOTIFIER_PRI_MEDIUM));
  }
  #endif  
    SYS_ERR_CHK(rsd_sys_macAddr_get(0, &sys_mac));

    if ((osal_thread_t)NULL == (osal_thread_create("TX Thread", 32768, DFLT_THREAD_PRI_LLDP_TX, (void *)lldp_pkt_tx_thread, NULL)))
    {
        LLDP_DBG("TX thread create failed");

        return SYS_ERR_FAILED;
    }

    if ((osal_thread_t)NULL == (osal_thread_create("Timer Thread", 32768, DFLT_THREAD_PRI_LLDP_TIMER, (void *)lldp_timer_thread, NULL)))
    {
        LLDP_DBG("Timer thread create failed");

        return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}
int32 lldp_exit(void)
{
    sys_logic_port_t port;

    lldp_tx_thread_status = 1;
    do { } while (lldp_tx_thread_status >= 0);

    lldp_timer_thread_status = 1;
    do { } while (lldp_timer_thread_status >= 0);

    SYS_ERR_CHK(rsd_nic_handler_unregister(&l2gLLDPHandler));
    SYS_ERR_CHK(sys_notifier_observer_unregister("lldp", SYS_NOTIFIER_SUBJECT_PORT, lldp_linkevent_handler));
    SYS_ERR_CHK(sys_notifier_observer_unregister("lldp", SYS_NOTIFIER_SUBJECT_TRUNK, lldp_trunkEvent_handler));
    SYS_ERR_CHK(sys_notifier_observer_unregister("lldp", SYS_NOTIFIER_SUBJECT_LLDP, lldp_tlvChangeEvent_handler));
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
   #ifdef CONFIG_SYS_VOICEVLAN
    SYS_ERR_CHK(sys_notifier_observer_unregister("lldp", SYS_NOTIFIER_SUBJECT_VLAN, lldp_voiceVlanEvent_handler));
   #endif  
  #endif 

    osal_sem_mutex_destroy(lldp_debugPrint_sem);
    FOR_EACH_LOGIC_PORT(port)
    {
        osal_sem_mutex_destroy(lldp_vlanTlv_sem[port]);
    }

    return SYS_ERR_OK;
}

static void send_lldp_flush_packet(uint32 port)
{
    uint8 *pktTxData = NULL;
    tx_tlvh_t lldp_tlvh;
    sys_mac_t port_mac;
    uint8 lldp_da[CAPA_MAC_BYTE_NUM] = {0x01,0x80,0xC2,0x00,0x00,0x0e};
    uint32 Plldp;
    char port_id[SYS_BUF32_LEN];

    if (NULL == (pktTxData = osal_alloc(1518)))
        return;

    osal_memset(pktTxData, 0, 1518);
    osal_memcpy( &pktTxData[0], &lldp_da[0] ,CAPA_MAC_BYTE_NUM);

    rsd_sys_portMacAddr_get(port,&port_mac);
    osal_memcpy(&pktTxData[SA_OFFSET],&port_mac.octet,CAPA_MAC_BYTE_NUM);
    WRITE16(&pktTxData[MAC_HEADER_LEN],LLDP_ETHERTYPE);
    Plldp = MAC_HEADER_LEN + ETHERTYPE_OFFSET ;

    lldp_tlvh.type = CHASSIS_ID_TLV;
    lldp_tlvh.tlv_length = 7;
    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));
    Plldp = Plldp + TLVH_OFFSET ;
    pktTxData[Plldp] = CHASSIS_ID_MAC_ADDRESS ;
    Plldp = Plldp + SUBTYPE_OFFSET;
    osal_memcpy(&pktTxData[Plldp],&sys_mac.octet,CAPA_MAC_BYTE_NUM);
    Plldp = Plldp + CAPA_MAC_BYTE_NUM ;

    osal_memset(port_id, 0, sizeof(port_id));
    LP2STR(port, port_id);
    
    lldp_tlvh.type = PORT_ID_TLV;
    lldp_tlvh.tlv_length = SUBTYPE_OFFSET + osal_strlen(port_id);
    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));
    Plldp = Plldp + TLVH_OFFSET ;
    pktTxData[Plldp] = PORT_ID_LOCALLY_ASSIGNED ;
    Plldp = Plldp + SUBTYPE_OFFSET;
    osal_memcpy(&pktTxData[Plldp],port_id, osal_strlen(port_id));
    Plldp = Plldp + osal_strlen(port_id);
    
    lldp_tlvh.type = TIME_TO_LIVE_TLV;
    lldp_tlvh.tlv_length = TTL_TLV_LEN;
    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));
    Plldp = Plldp + TLVH_OFFSET ;
    WRITE16(&pktTxData[Plldp], 0);
    Plldp = Plldp + TTL_TLV_LEN ;

    WRITE16(&pktTxData[Plldp], 0);
    Plldp = Plldp + TLVH_OFFSET ;

    if(port_table.port_entry[port].link_status == LLDP_LINKUP )
        lldp_pkt_tx(port , pktTxData, Plldp);

    port_table.port_entry[port].txdelay_timer = tx_delay;

    SYS_MEM_FREE(pktTxData);
}

static int32 send_lldp_packet(uint32 port)
{
    int32 ret = SYS_ERR_OK;
    uint8 *pktTxData = NULL;
    tx_tlvh_t lldp_tlvh;
    sys_mac_t port_mac;
    uint8 lldp_da[CAPA_MAC_BYTE_NUM] = {0x01,0x80,0xC2,0x00,0x00,0x0e};
    uint32 Plldp;
    char port_id[SYS_BUF32_LEN];
    char portDescriStr[SYS_BUF128_LEN];
    sys_l3_ipv4_t system_ip;
    uint32 trunkId;
    uint32 maxSize;
    sys_vid_t pvid;
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    lldp_med_tlv_sel_t lldpMedTlvSel;
    uint16  medCapability;
    lldp_med_location_t *pLocation = NULL;

  #ifdef CONFIG_SYS_VOICEVLAN
    voice_vlan_state_t state;
    sys_vid_t voiceVid;
    sys_vlan_voiceQos_t voiceQos;
  #endif 
  #endif 
    uint16 vid;
    sys_vlan_membership_t membership;
    lldp_overLoading_t overload;
    sys_sysinfo_t *pSysinfo = NULL;
    sys_l3_intfIPv6AddrsInfo_t ipv6Addr;
    uint32 i;

    LLDP_TX_DBG("Port %s starts composing LLDP PDU", LPSTR(port));

    SYS_MEM_ALLOC_HDL(pktTxData, 1518, _exit, ret);
    SYS_MEM_ALLOC_HDL(pLocation, sizeof(lldp_med_location_t), _exit, ret);

    rsd_switch_maxFrameSize_get(&maxSize);
    osal_memset(&overload, 0, sizeof(overload));
    if(maxSize > 1518) 
        overload.maxSize = 1518 - 30; 
    else
        overload.maxSize = maxSize - 30; 

    rsd_sys_mgmtIPv4Addr_get(&system_ip);

    osal_memset(pktTxData,0,sizeof(pktTxData));
    osal_memcpy( &pktTxData[0], &lldp_da[0] ,CAPA_MAC_BYTE_NUM);

    rsd_sys_portMacAddr_get(port,&port_mac);
    osal_memcpy(&pktTxData[SA_OFFSET],&port_mac.octet,CAPA_MAC_BYTE_NUM);
    WRITE16(&pktTxData[MAC_HEADER_LEN],LLDP_ETHERTYPE);
    Plldp = MAC_HEADER_LEN + ETHERTYPE_OFFSET ;

    overload.mandatory.byte += TLVH_OFFSET + 7;
    
    lldp_tlvh.type = CHASSIS_ID_TLV;
    lldp_tlvh.tlv_length = 7;
    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));
    Plldp = Plldp + TLVH_OFFSET ;
    pktTxData[Plldp] = CHASSIS_ID_MAC_ADDRESS ;
    Plldp = Plldp + SUBTYPE_OFFSET;
    osal_memcpy(&pktTxData[Plldp],&sys_mac.octet,CAPA_MAC_BYTE_NUM);
    Plldp = Plldp + CAPA_MAC_BYTE_NUM ;

    osal_memset(port_id, 0, sizeof(port_id));
    LP2STR(port, port_id);
    overload.mandatory.byte += TLVH_OFFSET + SUB_TYPE_LEN + osal_strlen(port_id);
    
    lldp_tlvh.type = PORT_ID_TLV;
    lldp_tlvh.tlv_length = SUBTYPE_OFFSET + osal_strlen(port_id);
    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));
    Plldp = Plldp + TLVH_OFFSET ;
    pktTxData[Plldp] = PORT_ID_LOCALLY_ASSIGNED ;
    Plldp = Plldp + SUBTYPE_OFFSET;
    osal_memcpy(&pktTxData[Plldp],port_id, osal_strlen(port_id));
    Plldp = Plldp + osal_strlen(port_id);

    overload.mandatory.byte += TLVH_OFFSET + TTL_TLV_LEN;
    
    lldp_tlvh.type = TIME_TO_LIVE_TLV;
    lldp_tlvh.tlv_length = TTL_TLV_LEN;
    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));
    Plldp = Plldp + TLVH_OFFSET ;
    WRITE16(&pktTxData[Plldp],LLDP_TTL);
    Plldp = Plldp + TTL_TLV_LEN ;

    overload.mandatory.byte += TLVH_OFFSET;
    overload.totalSize += overload.mandatory.byte;

  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    if(ENABLED == port_table.port_entry[port].medEnable)
    {
        
        overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + MEDIA_CAPA_LEN + CLASS_TYPE_LEN;
        if(overload.totalSize <= overload.maxSize)
        {
            
            lldp_tlvh.type = ORG_SPECIFIC_TLV;
            lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + MEDIA_CAPA_LEN + CLASS_TYPE_LEN;
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;

            WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_TIA); 
            Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

            pktTxData[Plldp] = SUB_TYPE_MEDIA_CAPA;
            Plldp = Plldp + SUB_TYPE_LEN ;

            lldp_portMedTlvSel_get(port, &lldpMedTlvSel);
            medCapability = (lldpMedTlvSel.inventorySel << 5) | (lldpMedTlvSel.poePseSel << 3) |
                (lldpMedTlvSel.locationSel << 2) | (lldpMedTlvSel.networkPolicySel << 1) | 1;
            WRITE16(&pktTxData[Plldp], medCapability);
            Plldp = Plldp + MEDIA_CAPA_LEN ;

            pktTxData[Plldp] = LLDP_CLASS_TYPE_NETWORK_CONN;
            Plldp = Plldp + CLASS_TYPE_LEN ;
        }
        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.locationSel)
        {
            
            lldp_portMedLocation_get(port, pLocation);
            if(0 != pLocation->coorLen)
            {
                overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + LOCATION_DATA_FMT_LEN + pLocation->coorLen;
                if(overload.totalSize <= overload.maxSize)
                {
                    lldp_tlvh.type = ORG_SPECIFIC_TLV;
                    lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + LOCATION_DATA_FMT_LEN + pLocation->coorLen;
                    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

                    Plldp = Plldp + TLVH_OFFSET ;

                    WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_TIA); 
                    Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

                    pktTxData[Plldp] = SUB_TYPE_LOCATION;
                    Plldp = Plldp + SUB_TYPE_LEN ;

                    pktTxData[Plldp] = LOCATION_DATA_FMT_COORDINATE;
                    Plldp = Plldp + LOCATION_DATA_FMT_LEN;

                    osal_memcpy(&pktTxData[Plldp],&pLocation->coordinate[0],pLocation->coorLen);
                    Plldp = Plldp + pLocation->coorLen ;
                }
            }
            
            if(0 != pLocation->civicLen)
            {
                overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + LOCATION_DATA_FMT_LEN + pLocation->civicLen;
                if(overload.totalSize <= overload.maxSize)
                {
                    lldp_tlvh.type = ORG_SPECIFIC_TLV;
                    lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + LOCATION_DATA_FMT_LEN + pLocation->civicLen;
                    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

                    Plldp = Plldp + TLVH_OFFSET ;

                    WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_TIA); 
                    Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

                    pktTxData[Plldp] = SUB_TYPE_LOCATION;
                    Plldp = Plldp + SUB_TYPE_LEN ;

                    pktTxData[Plldp] = LOCATION_DATA_FMT_CIVIC_ADDR;
                    Plldp = Plldp + LOCATION_DATA_FMT_LEN;

                    osal_memcpy(&pktTxData[Plldp],&pLocation->civicAddress[0],pLocation->civicLen);
                    Plldp = Plldp + pLocation->civicLen ;
                }
            }
            
            if(0 != pLocation->ecsLen)
            {
                overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + LOCATION_DATA_FMT_LEN + pLocation->ecsLen;
                if(overload.totalSize <= overload.maxSize)
                {
                    lldp_tlvh.type = ORG_SPECIFIC_TLV;
                    lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + LOCATION_DATA_FMT_LEN + pLocation->ecsLen;
                    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

                    Plldp = Plldp + TLVH_OFFSET ;

                    WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_TIA); 
                    Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

                    pktTxData[Plldp] = SUB_TYPE_LOCATION;
                    Plldp = Plldp + SUB_TYPE_LEN ;

                    pktTxData[Plldp] = LOCATION_DATA_FMT_ECS_ELIN;
                    Plldp = Plldp + LOCATION_DATA_FMT_LEN;

                    osal_memcpy(&pktTxData[Plldp],&pLocation->ecsElin[0],pLocation->ecsLen);
                    Plldp = Plldp + pLocation->ecsLen ;
                }
            }
        }
        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.networkPolicySel)
        {
            policy_t policyContent;
            uint8 policyIdx;

          #ifdef CONFIG_SYS_VOICEVLAN
            if(ENABLED == networkPolicyAuto)
            {
                sys_vid_t defaultVid;
                overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + APP_TYPE_LEN + POLICY_LEN;
                if(overload.totalSize <= overload.maxSize)
                {
                    voice_vlan_state_get(&state);
                    rsd_vlan_voiceVid_get(&voiceVid);
                    voice_vlan_qos_get(&voiceQos);
                    rsd_vlan_defaultVid_get(&defaultVid);

                    if(voiceVid != defaultVid && VOICE_VLAN_AUTO_ENABLED == state) 
                    {
                        lldp_tlvh.type = ORG_SPECIFIC_TLV;
                        lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + APP_TYPE_LEN + POLICY_LEN;
                        osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));
                        Plldp = Plldp + TLVH_OFFSET ;

                        WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_TIA); 
                        Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

                        pktTxData[Plldp] = SUB_TYPE_NETWORK_POLICY;
                        Plldp = Plldp + SUB_TYPE_LEN ;

                        pktTxData[Plldp] = LLDP_APP_VOICE;
                        Plldp = Plldp + APP_TYPE_LEN ;

                        osal_memset(&policyContent, 0, sizeof(policyContent));
                        policyContent.unknown = 0;
                        policyContent.tag = TRUE;
                        policyContent.vlanId = voiceVid;
                        policyContent.l2Pri = voiceQos.vpt;
                        policyContent.dscpPri = voiceQos.dscp;
                        osal_memcpy(&pktTxData[Plldp],&policyContent, sizeof(policyContent));
                        Plldp = Plldp + POLICY_LEN;
                    }
                }
            }
          #endif

            for(policyIdx = 0; policyIdx < 32; policyIdx ++)
            {
                if(0 != (port_table.port_entry[port].policyBind.bindMask & (1 << policyIdx)))
                {
                    if(TRUE == networkPolicy[policyIdx].valid)
                    {
                        overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + APP_TYPE_LEN + POLICY_LEN;
                        if(overload.totalSize <= overload.maxSize)
                        {
                            lldp_tlvh.type = ORG_SPECIFIC_TLV;
                            lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + APP_TYPE_LEN + POLICY_LEN;
                            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));
                            Plldp = Plldp + TLVH_OFFSET ;

                            WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_TIA); 
                            Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

                            pktTxData[Plldp] = SUB_TYPE_NETWORK_POLICY;
                            Plldp = Plldp + SUB_TYPE_LEN ;

                            pktTxData[Plldp] = networkPolicy[policyIdx].app;
                            Plldp = Plldp + APP_TYPE_LEN ;

                            SYS_MEM_CLEAR(policyContent);
                            policyContent.unknown = 0;
                            policyContent.tag = networkPolicy[policyIdx].vlanTagged;
                            policyContent.vlanId = networkPolicy[policyIdx].vid;
                            policyContent.l2Pri = networkPolicy[policyIdx].userPriority;
                            policyContent.dscpPri = networkPolicy[policyIdx].dscpValue;
                            osal_memcpy(&pktTxData[Plldp],&policyContent, sizeof(policyContent));
                            Plldp = Plldp + POLICY_LEN;
                        }
                    }
                }
            }
        }
        if (TRUE == port_table.port_entry[port].lldpMedTlvSel.poePseSel)
        {
            overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + POWER_INFO_LEN + POWER_VAL_LEN;
            if (overload.totalSize <= overload.maxSize)
            {
                power_info_t powerInfo;
                lldp_poe_portL2Data_t pse;

                osal_memset(&pse, 0, sizeof(pse));

                board_lldp_poePortL2Data_get(port, &pse);

                lldp_tlvh.type = ORG_SPECIFIC_TLV;
                lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + POWER_INFO_LEN + POWER_VAL_LEN;
                osal_memcpy(&pktTxData[Plldp], &lldp_tlvh, sizeof(tx_tlvh_t));
                Plldp = Plldp + TLVH_OFFSET;

                WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_TIA); 
                Plldp = Plldp + ORG_UNIQUE_CODE_LEN;

                pktTxData[Plldp] = SUB_TYPE_EXT_POWER_VIA_MDI;
                Plldp = Plldp + SUB_TYPE_LEN;

                powerInfo.type = pse.pwrType;
                powerInfo.source = pse.pwrSrc;
                powerInfo.pri = pse.pri;

                osal_memcpy(&pktTxData[Plldp],&powerInfo, sizeof(powerInfo));
                Plldp = Plldp + POWER_INFO_LEN;

                WRITE16(&pktTxData[Plldp], (uint16)pse.power);
                Plldp = Plldp + POWER_VAL_LEN;
            }
        }
    }
  #endif 
    if(TRUE == port_table.port_entry[port].lldpTlvSel.macPhySel)
    {
        overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + AUTO_NEGO_STATS_LEN + AUTO_NEGO_CAP_LEN + OPER_TYPE_LEN;
        if(overload.totalSize <= overload.maxSize)
        {
            uint8 autoNego;
            uint16 capa;
            uint16 oper;

            lldp_tlvh.type = ORG_SPECIFIC_TLV;
            lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + AUTO_NEGO_STATS_LEN + AUTO_NEGO_CAP_LEN + OPER_TYPE_LEN;
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;

            WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_802_3); 
            Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

            pktTxData[Plldp] = SUB_TYPE_MAC_PHY;
            Plldp = Plldp + SUB_TYPE_LEN ;

            rsd_port_macPhyInfo_get(port, &autoNego, &capa, &oper);
            pktTxData[Plldp] = autoNego;
            Plldp = Plldp + AUTO_NEGO_STATS_LEN;

            WRITE16(&pktTxData[Plldp], capa);
            Plldp = Plldp + AUTO_NEGO_CAP_LEN;

            WRITE16(&pktTxData[Plldp], oper);
            Plldp = Plldp + OPER_TYPE_LEN;
        }
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.linkAggregationSel)
    {
        overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + AGG_STATS_LEN + AGG_ID_LEN;
        if(overload.totalSize <= overload.maxSize)
        {
            lldp_tlvh.type = ORG_SPECIFIC_TLV;
            lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + AGG_STATS_LEN + AGG_ID_LEN;
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;

            WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_802_3); 
            Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

            pktTxData[Plldp] = SUB_TYPE_AGG;
            Plldp = Plldp + SUB_TYPE_LEN ;

            rsd_trunk_port2Trunk_get(port, &trunkId);
            if(0xffff != trunkId)
            {
                pktTxData[Plldp] = 0x3; 
                Plldp = Plldp + AGG_STATS_LEN ;

                WRITE32(&pktTxData[Plldp], trunkId + 1);
            }
            else
            {
                pktTxData[Plldp] = 0x1; 
                Plldp = Plldp + AGG_STATS_LEN ;

                WRITE32(&pktTxData[Plldp], 0);
            }

            Plldp = Plldp + AGG_ID_LEN;
        }
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.maxFrameSizeSel)
    {
        overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + MAX_FRAME_SIZE_LEN;
        if(overload.totalSize <= overload.maxSize)
        {
            lldp_tlvh.type = ORG_SPECIFIC_TLV;
            lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + MAX_FRAME_SIZE_LEN;
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;

            WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_802_3); 
            Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

            pktTxData[Plldp] = SUB_TYPE_MAX_FRAME_SIZE;
            Plldp = Plldp + SUB_TYPE_LEN ;
            rsd_switch_maxFrameSize_get(&maxSize);
            WRITE16(&pktTxData[Plldp], maxSize);
            Plldp = Plldp + MAX_FRAME_SIZE_LEN ;
        }
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.portDesciptionSel)
    {
        rsd_port_descp_get(port,portDescriStr);
        overload.totalSize += TLVH_OFFSET + osal_strlen(portDescriStr);
        if(overload.totalSize <= overload.maxSize)
        {
            lldp_tlvh.type = PORT_DESCRIPTION_TLV;
            lldp_tlvh.tlv_length = osal_strlen(portDescriStr);
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;
            osal_memcpy(&pktTxData[Plldp],&portDescriStr,osal_strlen(portDescriStr));

            Plldp = Plldp + osal_strlen(portDescriStr) ;
        }
    }

    SYS_MEM_ALLOC_HDL(pSysinfo, sizeof(sys_sysinfo_t), _exit, ret);
    rsd_sys_info_get(pSysinfo);
    
    if(TRUE == port_table.port_entry[port].lldpTlvSel.sysNameSel)
    {
        overload.totalSize += TLVH_OFFSET + osal_strlen(pSysinfo->name);
        if(overload.totalSize <= overload.maxSize)
        {
            lldp_tlvh.type = SYSTEM_NAME_TLV;
            lldp_tlvh.tlv_length = osal_strlen(pSysinfo->name);
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;
            osal_memcpy(&pktTxData[Plldp],&pSysinfo->name, osal_strlen(pSysinfo->name));

            Plldp = Plldp + osal_strlen(pSysinfo->name) ;
        }
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.sysDescriptionSel)
    {
        overload.totalSize += TLVH_OFFSET + osal_strlen(pSysinfo->description);
        if(overload.totalSize <= overload.maxSize)
        {
            lldp_tlvh.type = SYSTEM_DESCRIPTION_TLV;
            lldp_tlvh.tlv_length = osal_strlen(pSysinfo->description);
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;
            osal_memcpy(&pktTxData[Plldp],&pSysinfo->description,osal_strlen(pSysinfo->description));

            Plldp = Plldp + osal_strlen(pSysinfo->description) ;
        }
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.sysCapabilitySel)
    {
        overload.totalSize += TLVH_OFFSET + CAPA_TLV_OFFSET + CAPA_TLV_OFFSET;
        if(overload.totalSize <= overload.maxSize)
        {
            lldp_tlvh.type = SYSTEM_CAPABILITIES_TLV;
            lldp_tlvh.tlv_length = CAPA_TLV_LEN;
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;

#ifdef CONFIG_SYS_L3_ROUTE
            WRITE16(&pktTxData[Plldp], (SYSTEM_CAPABILITY_BRIDGE + SYSTEM_CAPABILITY_ROUTER));
            Plldp = Plldp + CAPA_TLV_OFFSET ;

            WRITE16(&pktTxData[Plldp], (SYSTEM_CAPABILITY_BRIDGE + SYSTEM_CAPABILITY_ROUTER));
            Plldp = Plldp + CAPA_TLV_OFFSET ;
#else
            WRITE16(&pktTxData[Plldp], (SYSTEM_CAPABILITY_BRIDGE));
            Plldp = Plldp + CAPA_TLV_OFFSET ;

            WRITE16(&pktTxData[Plldp], (SYSTEM_CAPABILITY_BRIDGE));
            Plldp = Plldp + CAPA_TLV_OFFSET ;
#endif
        }
    }

    if(TRUE == port_table.port_entry[port].lldpTlvSel.managementAddrSel)
    {
        overload.totalSize += TLVH_OFFSET + MGMT_ADDR_STR_LEN + MGMT_ADDR_SUBTYPE + IPV4_LEN + IF_NUM_SUBTYPE + IF_NUM + OID + OBJ_IDENTIFIER ;
        if(overload.totalSize <= overload.maxSize)
        {
            lldp_tlvh.type = MANAGEMENT_ADDRESS_TLV;
            lldp_tlvh.tlv_length = MGMT_ADDR_STR_LEN + MGMT_ADDR_SUBTYPE + IPV4_LEN + IF_NUM_SUBTYPE + IF_NUM + OID + OBJ_IDENTIFIER ;
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;

            pktTxData[Plldp] = 5;
            Plldp = Plldp + MGMT_ADDR_STR_LEN ;

            pktTxData[Plldp] = 1;
            Plldp = Plldp + MGMT_ADDR_SUBTYPE ;

            WRITE32(&pktTxData[Plldp],system_ip.addr);
            Plldp = Plldp + IPV4_LEN ;

            pktTxData[Plldp] = 3;
            Plldp = Plldp + IF_NUM_SUBTYPE ;
            Plldp = Plldp + IF_NUM ;
            pktTxData[Plldp] = 0;
            Plldp = Plldp + OID ;
        }

        SYS_MEM_CLEAR(ipv6Addr);
        rsd_sys_intfIpv6Addr_get(&ipv6Addr);
        if(ipv6Addr.cnt > 0)
        {
            for(i = 0; i < ipv6Addr.cnt; i ++)
            {
                if((i + 1) >= MAX_LLDP_MANAGEMENT_ADDR_NUM)
                    break;

                overload.totalSize += TLVH_OFFSET + MGMT_ADDR_STR_LEN + MGMT_ADDR_SUBTYPE + IPV6_LEN + IF_NUM_SUBTYPE + IF_NUM + OID + OBJ_IDENTIFIER ;
                if(overload.totalSize <= overload.maxSize)
                {
                    lldp_tlvh.type = MANAGEMENT_ADDRESS_TLV;
                    lldp_tlvh.tlv_length = MGMT_ADDR_STR_LEN + MGMT_ADDR_SUBTYPE + IPV6_LEN + IF_NUM_SUBTYPE + IF_NUM + OID + OBJ_IDENTIFIER ;
                    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

                    Plldp = Plldp + TLVH_OFFSET ;

                    pktTxData[Plldp] = 17;
                    Plldp = Plldp + MGMT_ADDR_STR_LEN ;

                    pktTxData[Plldp] = 2;
                    Plldp = Plldp + MGMT_ADDR_SUBTYPE ;

                    osal_memcpy(&pktTxData[Plldp], &ipv6Addr.addr[i], 16);
                    Plldp = Plldp + IPV6_LEN ;

                    pktTxData[Plldp] = 3;
                    Plldp = Plldp + IF_NUM_SUBTYPE ;
                    Plldp = Plldp + IF_NUM ;
                    pktTxData[Plldp] = 0;
                    Plldp = Plldp + OID ;
                }
            }
        }
    }

  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    if(ENABLED == port_table.port_entry[port].medEnable)
    {
        if(TRUE == port_table.port_entry[port].lldpMedTlvSel.inventorySel)
        {
            lldp_inventory_type_t inventoryType;
            char strData[128];

            for(inventoryType = LLDP_INVENTORY_TYPE_HARDWARE_REVISION; inventoryType < LLDP_INVENTORY_TYPE_END; inventoryType ++)
            {
                lldp_medInventory_get(inventoryType, strData);
                overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + osal_strlen(strData);
                if(overload.totalSize <= overload.maxSize)
                {
                    lldp_tlvh.type = ORG_SPECIFIC_TLV;
                    lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + osal_strlen(strData);
                    osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));
                    Plldp = Plldp + TLVH_OFFSET ;

                    WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_TIA); 
                    Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

                    pktTxData[Plldp] = inventoryType;
                    Plldp = Plldp + SUB_TYPE_LEN ;

                    osal_memcpy(&pktTxData[Plldp],strData, osal_strlen(strData));
                    Plldp = Plldp + osal_strlen(strData);
                }
            }
        }

    }
  #endif 
    
    if(ENABLED == port_table.port_entry[port].tlvPvidEbl)
    {
        overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + PORT_VLAN_ID_LEN;
        if(overload.totalSize <= overload.maxSize)
        {
            lldp_tlvh.type = ORG_SPECIFIC_TLV;
            lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + PORT_VLAN_ID_LEN;
            osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

            Plldp = Plldp + TLVH_OFFSET ;

            WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_802_1); 
            Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

            pktTxData[Plldp] = SUB_TYPE_PORT_VLAN_ID;
            Plldp = Plldp + SUB_TYPE_LEN ;

            rsd_vlan_portOperPvid_get(port, &pvid);
            WRITE16(&pktTxData[Plldp], pvid);
            Plldp = Plldp + PORT_VLAN_ID_LEN ;
        }
    }
    FOR_EACH_VID_IN_VLANMASK(vid, port_table.port_entry[port].tlvVlanmask)
    {
        rsd_vlan_portOperVlanMembership_get(port, vid, &membership);

        if(SYS_VLAN_TAGGED == membership || SYS_VLAN_UNTAGGED == membership)
        {
            uint8 len;
            sys_namestr_t name;

            rsd_vlan_name_get(vid, &name);
            len = osal_strlen(name.str);

            overload.totalSize += TLVH_OFFSET + ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + VLAN_ID_LEN + VLAN_NAME_LEN_LEN + len;

            if(overload.totalSize <= overload.maxSize)
            {
                lldp_tlvh.type = ORG_SPECIFIC_TLV;
                lldp_tlvh.tlv_length = ORG_UNIQUE_CODE_LEN + SUB_TYPE_LEN + VLAN_ID_LEN + VLAN_NAME_LEN_LEN + len;
                osal_memcpy(&pktTxData[Plldp],&lldp_tlvh,sizeof(tx_tlvh_t));

                Plldp = Plldp + TLVH_OFFSET ;

                WRITE24(&pktTxData[Plldp], ORG_UNIQUE_CODE_802_1); 
                Plldp = Plldp + ORG_UNIQUE_CODE_LEN ;

                pktTxData[Plldp] = SUB_TYPE_PORT_VLAN_NAME;
                Plldp = Plldp + SUB_TYPE_LEN ;

                WRITE16(&pktTxData[Plldp], vid);
                Plldp = Plldp + VLAN_ID_LEN ;

                pktTxData[Plldp] = len;
                Plldp = Plldp + VLAN_NAME_LEN_LEN ;

                if(len > 0)
                {
                    osal_memcpy(&pktTxData[Plldp], name.str, len);
                    Plldp = Plldp + len;
                }
            }
        }
    }

    Plldp = Plldp + TLVH_OFFSET ;

    if(port_table.port_entry[port].link_status == LLDP_LINKUP )
        lldp_pkt_tx(port , pktTxData, Plldp);

    port_table.port_entry[port].txdelay_timer = tx_delay;

_exit:
    SYS_MEM_FREE(pktTxData);
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_MEM_FREE(pLocation);
  
#endif
    SYS_MEM_FREE(pSysinfo);
    return ret;
}

void _lldp_remoteEntry_remove(sys_logic_port_t port, uint8 index)
{
    char macStr[CAPA_MAC_STR_LEN];
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    uint8 idx, medRcv, isVoiceVlanPort;
    sys_lldp_voice_vlan_event_t lldpVoiceVlan_event;
  #endif 

    sys_util_mac2Str(lldp_db.entry[port][index].chasis_id_tlv.tlv_mac, macStr);

    if (0 != lldp_db.entry[port][index].time_tlv_value)
    {
        lldp_statistic.age_out ++;
        lldp_statistic.port_statistics[port].rx_ageOut ++;
        SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_NEIGHBOR_TIMEOUT, LPSTR(port), macStr,
            lldp_db.entry[port][index].port_id_tlv.tlv_string);
    }

    lldp_statistic.deletion++;
    LLDP_DBG("clear lldp db %s entry %d", LPSTR(port), index);
    SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_NEIGHBOR_DEL, LPSTR(port), macStr,
            lldp_db.entry[port][index].port_id_tlv.tlv_string);

    lldp_vlanNameTlv_clear(&lldp_db.entry[port][index].vlanName_tlv); 
    osal_memset(&lldp_db.entry[port][index],0,sizeof(neighbor_entry_t));

  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    medRcv = FALSE;
    isVoiceVlanPort = FALSE;
    for (idx = 0 ; idx < LLDP_MAX_ENTRY_PER_PORT; idx++)
    {
        if(lldp_db.entry[port][idx].media_capa_tlv.valid == TRUE)
        {
            medRcv = TRUE;
        }

        if(lldp_db.entry[port][idx].isVoiceVidMatch == TRUE &&
            (LLDP_CLASS_TYPE_ENDPOINT_CLASS1 == lldp_db.entry[port][idx].media_capa_tlv.classType ||
            LLDP_CLASS_TYPE_ENDPOINT_CLASS2 == lldp_db.entry[port][idx].media_capa_tlv.classType ||
            LLDP_CLASS_TYPE_ENDPOINT_CLASS3 == lldp_db.entry[port][idx].media_capa_tlv.classType))
        {
            isVoiceVlanPort = TRUE;
        }
    }
    port_table.port_entry[port].medReceive = medRcv;
    if(TRUE == port_table.port_entry[port].isVoiceVlanPort && FALSE == isVoiceVlanPort)
    {
        lldpVoiceVlan_event.port = port;
        lldpVoiceVlan_event.event = SYS_NT_LLDP_VOICE_VLAN_PORT_DEL;
        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_VOICE_VLAN_PORT_DEL,
                (char*)&lldpVoiceVlan_event);
    }
    port_table.port_entry[port].isVoiceVlanPort = isVoiceVlanPort;
  #endif 
    lldp_remoteTableChangeTime_refresh();
}

void _lldp_remoteEntry_insert(sys_logic_port_t port, uint8 index, neighbor_entry_t *pEntry)
{
  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    uint8 idx, medRcv, isVoiceVlanPort;
    sys_lldp_voice_vlan_event_t lldpVoiceVlan_event;
  #endif 

    LLDP_PORT_VLAN_TLV_SEM_LOCK(port);

    lldp_vlanNameTlv_clear(&lldp_db.entry[port][index].vlanName_tlv); 
    osal_memcpy(&lldp_db.entry[port][index].vlanName_tlv , &pEntry->vlanName_tlv, sizeof(pEntry->vlanName_tlv));
    if(0 != osal_memcmp(&lldp_db.entry[port][index], pEntry, sizeof(*pEntry)))
    {
        lldp_remoteTableChangeTime_refresh();
    }

  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    if (pEntry->powerViaMdi_tlv.extValid == TRUE &&
        osal_memcmp(&lldp_db.entry[port][index].powerViaMdi_tlv, &pEntry->powerViaMdi_tlv,
                    sizeof(pEntry->powerViaMdi_tlv)))
    {
        if (IS_POE_PORT(port) &&
            (pEntry->powerViaMdi_tlv.powerInfo.type == LLDP_PWR_TYPE_1_PD ||
             pEntry->powerViaMdi_tlv.powerInfo.type == LLDP_PWR_TYPE_2_PD))
        {
            if (1 == pEntry->powerAllocBy)
            {
                lldp_poe_portL2Data_t pd;

                osal_memset(&pd, 0, sizeof(pd));

                pd.power = pEntry->powerViaMdi_tlv.requestPower;
                pd.pwrType = pEntry->powerViaMdi_tlv.powerInfo.source;
                pd.pwrSrc = pEntry->powerViaMdi_tlv.powerInfo.type;
                pd.pri = pEntry->powerViaMdi_tlv.powerInfo.pri;

                board_lldp_poePortL2Data_set(port, pd);

                LLDP_POE_DBG("LLDP-PoE power allocation to port %s by dot3tlv power %d type %d src %d pri %d",
                             LPSTR(port),
                             pd.power, pd.pwrType, pd.pwrSrc, pd.pri);
            }
            else if (2 == pEntry->powerAllocBy)
            {
                lldp_poe_portL2Data_t pd;

                osal_memset(&pd, 0, sizeof(pd));

                pd.power = pEntry->powerViaMdi_tlv.requestPower;
                pd.pwrType = pEntry->powerViaMdi_tlv.powerInfo.source;
                pd.pwrSrc = pEntry->powerViaMdi_tlv.powerInfo.type;
                pd.pri = pEntry->powerViaMdi_tlv.powerInfo.pri;

                board_lldp_poePortL2Data_set(port, pd);

                LLDP_POE_DBG("LLDP-PoE power allocation to port %s by medTlv power %d type %d src %d pri %d",
                             LPSTR(port),
                             pd.power, pd.pwrType, pd.pwrSrc, pd.pri);
            }
        }
    }
  #endif

    osal_memcpy(&lldp_db.entry[port][index] , pEntry , sizeof(neighbor_entry_t));

  #ifdef CONFIG_SYS_PROTO_LLDP_MED
    medRcv = FALSE;
    isVoiceVlanPort = FALSE;
    for (idx = 0 ; idx < LLDP_MAX_ENTRY_PER_PORT; idx++ )
    {
        if(lldp_db.entry[port][idx].media_capa_tlv.valid == TRUE)
        {
            medRcv = TRUE;
        }

        if(lldp_db.entry[port][idx].isVoiceVidMatch == TRUE &&
            (LLDP_CLASS_TYPE_ENDPOINT_CLASS1 == pEntry->media_capa_tlv.classType ||
            LLDP_CLASS_TYPE_ENDPOINT_CLASS2 == pEntry->media_capa_tlv.classType ||
            LLDP_CLASS_TYPE_ENDPOINT_CLASS3 == pEntry->media_capa_tlv.classType))
        {
            isVoiceVlanPort = TRUE;
        }
    }

    port_table.port_entry[port].medReceive = medRcv;
    if(TRUE == port_table.port_entry[port].isVoiceVlanPort && FALSE == isVoiceVlanPort)
    {
        lldpVoiceVlan_event.port = port;
        lldpVoiceVlan_event.event = SYS_NT_LLDP_VOICE_VLAN_PORT_DEL;
        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_VOICE_VLAN_PORT_DEL,
                (char*)&lldpVoiceVlan_event);
    }
    if(FALSE == port_table.port_entry[port].isVoiceVlanPort && TRUE == isVoiceVlanPort)
    {
        lldpVoiceVlan_event.port = port;
        lldpVoiceVlan_event.event = SYS_NT_LLDP_VOICE_VLAN_PORT_ADD;
        sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_LLDP, SYS_NT_LLDP_VOICE_VLAN_PORT_ADD,
                (char*)&lldpVoiceVlan_event);
    }
    port_table.port_entry[port].isVoiceVlanPort = isVoiceVlanPort;
  #endif 

    LLDP_PORT_VLAN_TLV_SEM_UNLOCK(port);
}

static int32 update_database(neighbor_entry_t *pEntry)
{
    uint8 i, minIdx = 0;
    uint16 minTTL = 0xffff;
    sys_logic_port_t    port;
    char macStr[CAPA_MAC_STR_LEN];

    port = pEntry->port_index;

    for (i = 0 ; i < LLDP_MAX_ENTRY_PER_PORT; i++ )
    {
        if ((0 == osal_memcmp(lldp_db.entry[port][i].chasis_id_tlv.tlv_string, pEntry->chasis_id_tlv.tlv_string, lldp_db.entry[port][i].chasis_id_tlv.tlv_info_len - 1))
        && (0 == osal_memcmp(lldp_db.entry[port][i].port_id_tlv.tlv_string, pEntry->port_id_tlv.tlv_string, lldp_db.entry[port][i].port_id_tlv.tlv_info_len - 1))
        && (0 != lldp_db.entry[port][i].chasis_id_tlv.tlv_type)
        && (0 != lldp_db.entry[port][i].port_id_tlv.tlv_type)
        && (lldp_db.entry[port][i].port_index == pEntry->port_index))
        {
            LLDP_RX_DBG("SAME NEIBGHOR! update port=%s i=%d", LPSTR(port), i);

            _lldp_remoteEntry_insert(port, i, pEntry);
            return SYS_ERR_OK;
        }
    }

    sys_util_mac2Str(pEntry->chasis_id_tlv.tlv_mac, macStr);
    SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_NEIGHBOR_DISCOVER, LPSTR(port), macStr,
            pEntry->port_id_tlv.tlv_string);

    for (i = 0 ; i < LLDP_MAX_ENTRY_PER_PORT ; i++ )
    {
        if ((0 == lldp_db.entry[port][i].chasis_id_tlv.tlv_type))
        {
            LLDP_RX_DBG("ADD NEIBGHOR! create port=%s i=%d", LPSTR(port), i);
            lldp_statistic.insertion ++;
            _lldp_remoteEntry_insert(port, i, pEntry);

            return SYS_ERR_OK;
        }
        else
        {
            if(minTTL > lldp_db.entry[port][i].TTL)
            {
                minTTL = lldp_db.entry[port][i].TTL;
                minIdx = i;
            }
        }
    }

    if(TRUE == replaceWhenFull)
    {
        if(pEntry->TTL >= minTTL)
        {
            _lldp_remoteEntry_insert(port, minIdx, pEntry);
        }
        else
        {
            lldp_statistic.drop++;
            LLDP_RX_DBG("lldp_db is full,no entry could be recorded");
            SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_NEIGHBOR_LIMIT, LLDP_MAX_ENTRY_PER_PORT, LPSTR(port));
        }
    }
    else
    {
        lldp_statistic.drop++;
        LLDP_RX_DBG("lldp_db is full,no entry could be recorded");
        SYS_LOG(LOG_CAT_LLDP, LOG_LLDP_NEIGHBOR_LIMIT, LLDP_MAX_ENTRY_PER_PORT, LPSTR(port));
    }
    return SYS_ERR_OK;
}

static void update_reinitdelay_time(sys_logic_port_t logicPort)
{
    if (port_table.port_entry[logicPort].reinitdelay_timer > 0)
    {
        port_table.port_entry[logicPort].reinitdelay_timer--;
    }
}

static void update_txdelay_time(sys_logic_port_t logicPort)
{

    if (port_table.port_entry[logicPort].txdelay_timer > 0)
    {
        port_table.port_entry[logicPort].txdelay_timer--;
    }
}

static void update_txtime(sys_logic_port_t logicPort)
{
    if (port_table.port_entry[logicPort].tx_timer > 0)
        port_table.port_entry[logicPort].tx_timer--;
}

static void update_holdtime(void)
{
    uint8 i;
    sys_logic_port_t    port;

    FOR_EACH_NORMAL_LOGIC_PORT(port)
    {
        for (i = 0 ; i < LLDP_MAX_ENTRY_PER_PORT; i++ )
        {
            if (0 != lldp_db.entry[port][i].chasis_id_tlv.tlv_type)
            {
                if (lldp_db.entry[port][i].TTL > 0)
                {
                    lldp_db.entry[port][i].TTL --;
                }
                else if(0 == lldp_db.entry[port][i].TTL)
                {
                    _lldp_remoteEntry_remove(port, i);
                }
            }
        }
    }
}
int32 display_chassis(display_entry_t *pEntry, lldp_tlv_t tlv)
{
    char ipStr[CAPA_IPV4_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];

    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);

    osal_memset(ipStr, 0, sizeof(ipStr));
    osal_memset(macStr, 0, sizeof(macStr));

    pEntry->chasis_id.iSubtype = tlv.sub_type;

    switch (tlv.sub_type)
    {
        case CHASSIS_ID_INTERFACE_ALIAS:
            osal_strcpy(pEntry->chasis_id.subtype, "Interface alias");
            osal_sprintf(pEntry->chasis_id.id, "%s", tlv.tlv_string);
            break;

        case CHASSIS_ID_MAC_ADDRESS:
            osal_strcpy(pEntry->chasis_id.subtype, "MAC address");
            osal_memcpy(pEntry->chasis_id.iId, tlv.tlv_mac, CAPA_MAC_BYTE_NUM);
            sys_util_mac2Str(tlv.tlv_mac, macStr);
            osal_sprintf(pEntry->chasis_id.id, "%s", macStr);
            break;

        case CHASSIS_ID_NETWORK_ADDRESS:
            osal_strcpy(pEntry->chasis_id.subtype, "Network address");
            sys_util_ip2Str(tlv.tlv_ip, ipStr);
            osal_sprintf(pEntry->chasis_id.id, "%s", ipStr);
            break;

        case CHASSIS_ID_INTERFACE_NAME:
            osal_strcpy(pEntry->chasis_id.subtype, "Interface name");
            osal_sprintf(pEntry->chasis_id.id, "%s", tlv.tlv_string);
            break;

        case CHASSIS_ID_LOCALLY_ASSIGNED:
            osal_strcpy(pEntry->chasis_id.subtype, "Locally assigned");
            osal_sprintf(pEntry->chasis_id.id, "%s", tlv.tlv_string);
            break;

        case CHASSIS_ID_PORT_COMPONENT:
            osal_strcpy(pEntry->chasis_id.subtype, "Port component");
            osal_sprintf(pEntry->chasis_id.id, "%s)", tlv.tlv_string);
            break;

        case CHASSIS_ID_CHASSIS_COMPONENT:
            osal_strcpy(pEntry->chasis_id.subtype, "Unknown");
            osal_sprintf(pEntry->chasis_id.id, "%s)", tlv.tlv_string);
            break;
        default:
            osal_strcpy(pEntry->chasis_id.subtype, "Unknown");
            osal_sprintf(pEntry->chasis_id.id, "%s)", tlv.tlv_string);
            break;
    }

    return SYS_ERR_OK;
}

int32 display_portid(display_entry_t *pEntry,lldp_tlv_t tlv)
{
    char ipStr[CAPA_IPV4_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];

    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);

    osal_memset(ipStr, 0, sizeof(ipStr));
    osal_memset(macStr, 0, sizeof(macStr));

    pEntry->port_id.iSubtype = tlv.sub_type;

    switch (tlv.sub_type)
    {
        case PORT_ID_INTERFACE_ALIAS:
            osal_strcpy(pEntry->port_id.subtype, "Interface alias");
            osal_sprintf(pEntry->port_id.id, "%s", tlv.tlv_string);
            break;

        case PORT_ID_MAC_ADDRESS:
            osal_strcpy(pEntry->port_id.subtype, "MAC address");
            sys_util_mac2Str(tlv.tlv_mac, macStr);
            osal_sprintf(pEntry->port_id.id, "%s", macStr);
            break;

        case PORT_ID_NETWORK_ADDRESS:
            osal_strcpy(pEntry->port_id.subtype, "Network address");
            sys_util_ip2Str(tlv.tlv_ip, ipStr);
            osal_sprintf(pEntry->port_id.id, "%s", ipStr);
            break;

        case PORT_ID_INTERFACE_NAME:
            osal_strcpy(pEntry->port_id.subtype, "Interface name");
            osal_sprintf(pEntry->port_id.id, "%s", tlv.tlv_string);
            break;

        case PORT_ID_LOCALLY_ASSIGNED:
            osal_strcpy(pEntry->port_id.subtype, "Locally assigned");
            osal_sprintf(pEntry->port_id.id, "%s", tlv.tlv_string);
            break;

        case PORT_ID_PORT_COMPONENT:
            osal_strcpy(pEntry->port_id.subtype, "Port component");
            osal_sprintf(pEntry->port_id.id, "%s", tlv.tlv_string);
            break;

        case PORT_ID_AGENT_CIRCUIT_ID:
            osal_strcpy(pEntry->port_id.subtype, "Circuit ID");
            osal_sprintf(pEntry->port_id.id, "%s", tlv.tlv_string);
            break;

        default:
            osal_strcpy(pEntry->port_id.subtype, "Unknown");
            osal_sprintf(pEntry->port_id.id, "%s", tlv.tlv_string);
            break;
    }

    return SYS_ERR_OK;
}

int32 display_cap(display_entry_t *pEntry, lldp_tlv_t tlv)
{
    char tmp[SYS_BUF256_LEN];
    int32 i = 0;

    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);

    osal_memset(tmp, 0, sizeof(tmp));

    for (i = 0; i < (sizeof(lldpCapability)/sizeof(lldpCapability[0])); i++)
    {
        if ((tlv.tlv_value & lldpCapability[i])
            && ((osal_strlen(tmp) + osal_strlen(lldpCapabilityName[i])) <= SYS_BUF64_LEN))
            osal_strcat(tmp, lldpCapabilityName[i]);
    }

    pEntry->system_capabilities.iSupport = tlv.tlv_value;

    if(osal_strlen(tmp) > 3)
        osal_strcpy(pEntry->system_capabilities.support, &tmp[2]);

    osal_memset(tmp, 0, sizeof(tmp));

    for (i = 0; i < (sizeof(lldpCapability)/sizeof(lldpCapability[0])); i++)
    {
        if ((tlv.sub_value & lldpCapability[i])
            && ((osal_strlen(tmp) + osal_strlen(lldpCapabilityName[i])) <= SYS_BUF64_LEN))
            osal_strcat(tmp, lldpCapabilityName[i]);
    }

    pEntry->system_capabilities.iEnable = tlv.sub_value;

    if(osal_strlen(tmp) > 3)
        osal_strcpy(pEntry->system_capabilities.enable, &tmp[2]);

    return SYS_ERR_OK;
}

int32 display_macPhy(display_entry_t *pEntry, lldp_macPhy_tlv_t *pMacPhy_tlv)
{
    char tmp[SYS_BUF512_LEN];

    pEntry->macPhy.valid = pMacPhy_tlv->valid;

    if(FALSE == pMacPhy_tlv->valid)
    {
        return SYS_ERR_OK;
    }

    if(pMacPhy_tlv->autoNego & 1)
        pEntry->macPhy.support = TRUE;
    else
        pEntry->macPhy.support = FALSE;

    if(pMacPhy_tlv->autoNego & (1 << 1))
        pEntry->macPhy.enable = TRUE;
    else
        pEntry->macPhy.enable = FALSE;

    pEntry->macPhy.iCapa = pMacPhy_tlv->capa;
    osal_memset(tmp, 0, sizeof(tmp));
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_OTHER))
        osal_strcat(tmp, ", Other or unknown");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_10BASET))
        osal_strcat(tmp, ", 10BASE-T  half duplex");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_10BASETFD))
        osal_strcat(tmp, ", 10BASE-T  full duplex");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_100BASET4))
        osal_strcat(tmp, ", 100BASE-T4");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_100BASETX))
        osal_strcat(tmp, ", 100BASE-TX half duplex");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_100BASETXFD))
        osal_strcat(tmp, ", 100BASE-TX full duplex");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_100BASET2))
        osal_strcat(tmp, ", 100BASE-T2 half duplex");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_100BASET2FD))
        osal_strcat(tmp, ", 100BASE-T2 full duplex");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_FDXPAUSE))
        osal_strcat(tmp, ", PAUSE for full-duplex links");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_FDXAPAUSE))
        osal_strcat(tmp, ", Asymmetric PAUSE for full-duplex links");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_FDXSPAUSE))
        osal_strcat(tmp, ", Symmetric PAUSE for full-duplex links");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_FDXBPAUSE))
        osal_strcat(tmp, ", Asymmetric and Symmetric PAUSE for full-duplex links");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_1000BASEX))
        osal_strcat(tmp, ", 1000BASE-X, -LX, -SX, -CX half duplex");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_1000BASEXFD))
        osal_strcat(tmp, ", 1000BASE-X, -LX, -SX, -CX full duplex");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_1000BASET))
        osal_strcat(tmp, ", 1000BASE-T half duplex");
    if(pMacPhy_tlv->capa & (1 << LLDP_PHY_CAPA_1000BASETFD))
        osal_strcat(tmp, ", 1000BASE-T full duplex");

    if(strlen(tmp) > 0)
        osal_strcpy(pEntry->macPhy.capa, &tmp[2]);

    pEntry->macPhy.iOper = pMacPhy_tlv->oper;
    switch(pMacPhy_tlv->oper)
    {
        case LLDP_PHY_OPER_OTHER:
            osal_strcpy(pEntry->macPhy.oper, "Other or unknown");
            break;
        case LLDP_PHY_OPER_AUI:
            osal_strcpy(pEntry->macPhy.oper, "AUI");
            break;
        case LLDP_PHY_OPER_10BASE5:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-5");
            break;
        case LLDP_PHY_OPER_FOIRL:
            osal_strcpy(pEntry->macPhy.oper, "FOIRL");
            break;
        case LLDP_PHY_OPER_10BASE2:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-2");
            break;
        case LLDP_PHY_OPER_10BASET:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-T duplex mode unknown");
            break;
        case LLDP_PHY_OPER_10BASEFP:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-FP");
            break;
        case LLDP_PHY_OPER_10BASEFB:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-FB");
            break;
        case LLDP_PHY_OPER_10BASEFL:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-FL duplex mode unknown");
            break;
        case LLDP_PHY_OPER_10BROAD36:
            osal_strcpy(pEntry->macPhy.oper, "10BROAD36");
            break;
        case LLDP_PHY_OPER_10BASETHD:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-T  half duplex mode");
            break;
        case LLDP_PHY_OPER_10BASETFD:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-T  full duplex mode");
            break;
        case LLDP_PHY_OPER_10BASEFLHD:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-FL half duplex mode");
            break;
        case LLDP_PHY_OPER_10BASEFLFD:
            osal_strcpy(pEntry->macPhy.oper, "10BASE-FL full duplex mode");
            break;
        case LLDP_PHY_OPER_100BASET4:
            osal_strcpy(pEntry->macPhy.oper, "100BASE-T4");
            break;
        case LLDP_PHY_OPER_100BASETXHD:
            osal_strcpy(pEntry->macPhy.oper, "100BASE-TX half duplex mode");
            break;
        case LLDP_PHY_OPER_100BASETXFD:
            osal_strcpy(pEntry->macPhy.oper, "100BASE-TX full duplex mode");
            break;
        case LLDP_PHY_OPER_100BASEFXHD:
            osal_strcpy(pEntry->macPhy.oper, "100BASE-FX half duplex mode");
            break;
        case LLDP_PHY_OPER_100BASEFXFD:
            osal_strcpy(pEntry->macPhy.oper, "100BASE-FX full duplex mode");
            break;
        case LLDP_PHY_OPER_100BASET2HD:
            osal_strcpy(pEntry->macPhy.oper, "100BASE-T2 half duplex mode");
            break;
        case LLDP_PHY_OPER_100BASET2FD:
            osal_strcpy(pEntry->macPhy.oper, "100BASE-T2 full duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASEXHD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-X half duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASEXFD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-X full duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASELXHD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-LX half duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASELXFD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-LX full duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASESXHD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-SX half duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASESXFD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-SX full duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASECXHD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-CX half duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASECXFD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-CX full duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASETHD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-T half duplex mode");
            break;
        case LLDP_PHY_OPER_1000BASETFD:
            osal_strcpy(pEntry->macPhy.oper, "1000BASE-T full duplex mode");
            break;
        case LLDP_PHY_OPER_10GBASEX:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-X");
            break;
        case LLDP_PHY_OPER_10GBASELX4:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-LX4");
            break;
        case LLDP_PHY_OPER_10GBASER:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-R");
            break;
        case LLDP_PHY_OPER_10GBASEER:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-ER");
            break;
        case LLDP_PHY_OPER_10GBASELR:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-LR");
            break;
        case LLDP_PHY_OPER_10GBASESR:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-SR");
            break;
        case LLDP_PHY_OPER_10GBASEW:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-W");
            break;
        case LLDP_PHY_OPER_10GBASEEW:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-EW");
            break;
        case LLDP_PHY_OPER_10GBASELW:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-LW");
            break;
        case LLDP_PHY_OPER_10GBASESW:
            osal_strcpy(pEntry->macPhy.oper, "10GBASE-SW");
            break;
    }

    return SYS_ERR_OK;
}

int32 display_8023power(display_entry_t *pEntry, lldp_powerViaMdi_tlv_t *pPowerViaMdi_tlv)
{
    pEntry->dot3power.valid = pPowerViaMdi_tlv->valid;
    pEntry->dot3power.extValid = pPowerViaMdi_tlv->extValid;

    if(FALSE == pPowerViaMdi_tlv->valid)
    {
        return SYS_ERR_OK;
    }
    else
    {
        pEntry->dot3power.iPortClass = pPowerViaMdi_tlv->support.portClass;
        if (pPowerViaMdi_tlv->support.portClass)
            osal_strcat(pEntry->dot3power.portClass, "PSE");
        else
            osal_strcat(pEntry->dot3power.portClass, "PD");

        pEntry->dot3power.iPowerSupport = pPowerViaMdi_tlv->support.powerSupport;
        if(TRUE == pPowerViaMdi_tlv->support.powerSupport)
            osal_strcpy(pEntry->dot3power.powerSupport, "Supported");
        else
            osal_strcpy(pEntry->dot3power.powerSupport, "Not Supported");

        pEntry->dot3power.iPowerState = pPowerViaMdi_tlv->support.powerState;
        if(ENABLED == pPowerViaMdi_tlv->support.powerState)
            osal_strcpy(pEntry->dot3power.powerState, "Enabled");
        else
            osal_strcpy(pEntry->dot3power.powerState, "Not Enabled");

        pEntry->dot3power.iPairControlAbility = pPowerViaMdi_tlv->support.pairsControl;
        if(TRUE == pPowerViaMdi_tlv->support.pairsControl)
            osal_strcpy(pEntry->dot3power.pairControlAbility, "Supported");
        else
            osal_strcpy(pEntry->dot3power.pairControlAbility, "Not Supported");

        pEntry->dot3power.iPowerPair = pPowerViaMdi_tlv->portPair;
        osal_sprintf(pEntry->dot3power.powerPair, "%d", pPowerViaMdi_tlv->portPair);

        pEntry->dot3power.iPowerClass = pPowerViaMdi_tlv->powerClass;
        osal_sprintf(pEntry->dot3power.powerClass, "class%d", pPowerViaMdi_tlv->powerClass - 1);

        if(TRUE == pEntry->dot3power.extValid)
        {
            pEntry->dot3power.iDevType = pPowerViaMdi_tlv->powerInfo.type;
            pEntry->dot3power.iSrc = pPowerViaMdi_tlv->powerInfo.source;
            pEntry->dot3power.iPri = pPowerViaMdi_tlv->powerInfo.pri;

            switch(pPowerViaMdi_tlv->powerInfo.type)
            {
                case LLDP_PWR_TYPE_2_PSE:
                case LLDP_PWR_TYPE_1_PSE:
                    osal_strcat(pEntry->dot3power.devType, "Power Sourcing Entity");
                    switch(pPowerViaMdi_tlv->powerInfo.source)
                    {
                        case LLDP_PWR_SRC_UNKNOWN:
                            osal_strcat(pEntry->dot3power.src, "Unknown");
                            break;
                        case LLDP_PWR_SRC_PRIMARY:
                            osal_strcat(pEntry->dot3power.src, "Primary Power Source");
                            break;
                        case LLDP_PWR_SRC_BACKUP:
                            osal_strcat(pEntry->dot3power.src, "Backup Power Source");
                            break;
                        default:
                            osal_strcat(pEntry->dot3power.src, "Reserved");
                            break;
                    }
                    break;
                case LLDP_PWR_TYPE_1_PD:
                case LLDP_PWR_TYPE_2_PD:
                     osal_strcat(pEntry->dot3power.devType, "Power Device");
                    switch(pPowerViaMdi_tlv->powerInfo.source)
                    {
                        case LLDP_PWR_SRC_UNKNOWN:
                            osal_strcat(pEntry->dot3power.src, "Unknown");
                            break;
                        case LLDP_PWR_SRC_PRIMARY:
                            osal_strcat(pEntry->dot3power.src, "PSE");
                            break;
                        case LLDP_PWR_SRC_BACKUP:
                            osal_strcat(pEntry->dot3power.src, "Local");
                            break;
                        case LLDP_PWR_SRC_ALL:
                            osal_strcat(pEntry->dot3power.src, "PSE and Local");
                            break;
                    }
                    break;
                default:
                    osal_strcat(pEntry->dot3power.devType, "Unknown");
                    osal_strcat(pEntry->dot3power.src, "Unknown");
                    break;
            }

            switch(pPowerViaMdi_tlv->powerInfo.pri)
            {
                case LLDP_PWR_PRI_UNKNOWN:
                    osal_strcpy(pEntry->dot3power.pri, "Unknown");
                    break;
                case LLDP_PWR_PRI_CRITICAL:
                    osal_strcpy(pEntry->dot3power.pri, "Critical");
                    break;
                case LLDP_PWR_PRI_HIGH:
                    osal_strcpy(pEntry->dot3power.pri, "High");
                    break;
                case LLDP_PWR_PRI_LOW:
                    osal_strcpy(pEntry->dot3power.pri, "Low");
                    break;
            }

            pEntry->dot3power.requestPower = pPowerViaMdi_tlv->requestPower;
            pEntry->dot3power.allocatePower = pPowerViaMdi_tlv->allocatePower;
        }
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED
int32 display_power(display_med_entry_t *pEntry, lldp_med_poe_tlv_t *pPoe_tlv)
{
    pEntry->power.valid = pPoe_tlv->valid;
    pEntry->power.iDevType = pPoe_tlv->powerInfo.type;
    pEntry->power.iSrc = pPoe_tlv->powerInfo.source;
    pEntry->power.iPri = pPoe_tlv->powerInfo.pri;

    if(FALSE == pPoe_tlv->valid)
    {
        return SYS_ERR_OK;
    }
    else
    {
        switch(pPoe_tlv->powerInfo.type)
        {
            case LLDP_PWR_TYPE_2_PSE:
            case LLDP_PWR_TYPE_1_PSE:
                pEntry->medCapa.iCapaCurrent |= 1 << 3;
                osal_strcat(pEntry->power.devType, "Power Sourcing Entity");
                switch(pPoe_tlv->powerInfo.source)
                {
                    case LLDP_PWR_SRC_UNKNOWN:
                        osal_strcat(pEntry->power.src, "Unknown");
                        break;
                    case LLDP_PWR_SRC_PRIMARY:
                        osal_strcat(pEntry->power.src, "Primary Power Source");
                        break;
                    case LLDP_PWR_SRC_BACKUP:
                        osal_strcat(pEntry->power.src, "Backup Power Source");
                        break;
                    default:
                        osal_strcat(pEntry->power.src, "Reserved");
                        break;
                }
                break;
            case LLDP_PWR_TYPE_1_PD:
            case LLDP_PWR_TYPE_2_PD:
                pEntry->medCapa.iCapaCurrent |= 1 << 4;
                osal_strcat(pEntry->power.devType, "Power Device");
                switch(pPoe_tlv->powerInfo.source)
                {
                    case LLDP_PWR_SRC_UNKNOWN:
                        osal_strcat(pEntry->power.src, "Unknown");
                        break;
                    case LLDP_PWR_SRC_PRIMARY:
                        osal_strcat(pEntry->power.src, "PSE");
                        break;
                    case LLDP_PWR_SRC_BACKUP:
                        osal_strcat(pEntry->power.src, "Local");
                        break;
                    case LLDP_PWR_SRC_ALL:
                        osal_strcat(pEntry->power.src, "PSE and Local");
                        break;
                }
                break;
            default:
                osal_strcat(pEntry->power.devType, "Unknown");
                osal_strcat(pEntry->power.src, "Unknown");
                break;
        }

        switch(pPoe_tlv->powerInfo.pri)
        {
            case LLDP_PWR_PRI_UNKNOWN:
                osal_strcpy(pEntry->power.pri, "Unknown");
                break;
            case LLDP_PWR_PRI_CRITICAL:
                osal_strcpy(pEntry->power.pri, "Critical");
                break;
            case LLDP_PWR_PRI_HIGH:
                osal_strcpy(pEntry->power.pri, "High");
                break;
            case LLDP_PWR_PRI_LOW:
                osal_strcpy(pEntry->power.pri, "Low");
                break;
        }

        pEntry->power.powerVal = pPoe_tlv->powerVal;
    }

    return SYS_ERR_OK;
}
#endif 

int32 display_trunk(display_entry_t *pEntry, lldp_trunk_tlv_t *pTrunk_tlv)
{
    pEntry->trunk.valid = pTrunk_tlv->valid;

    if(FALSE == pTrunk_tlv->valid)
    {
        return SYS_ERR_OK;
    }

    if(pTrunk_tlv->aggInfo & 0x1)
    {
        osal_strcat(pEntry->trunk.capa, "Capable of being aggregated");
        pEntry->trunk.iCapa = TRUE;
    }
    else
    {
        osal_strcat(pEntry->trunk.capa, "Not capable of being aggregated");
        pEntry->trunk.iCapa = FALSE;
    }

    if(pTrunk_tlv->aggInfo & 0x2)
    {
        osal_strcat(pEntry->trunk.state, "Currently in aggregation");
        pEntry->trunk.iState = TRUE;
    }
    else
    {
        osal_strcat(pEntry->trunk.state, "Not currently in aggregation");
        pEntry->trunk.iState = FALSE;
    }

    pEntry->trunk.id = pTrunk_tlv->aggId;

    return SYS_ERR_OK;
}

int32 display_pvid(display_entry_t *pEntry, lldp_pvid_tlv_t *pPvid_tlv)
{
    pEntry->pvid.valid = pPvid_tlv->valid;

    if(FALSE == pPvid_tlv->valid)
    {
        return SYS_ERR_OK;
    }

    pEntry->pvid.pvid = pPvid_tlv->pvid;

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_LLDP_MED
int32 display_medCapa(display_med_entry_t *pEntry, lldp_med_media_capa_tlv_t *pMedia_capa_tlv)
{
    char tmp[SYS_BUF256_LEN];

    pEntry->medCapa.valid = pMedia_capa_tlv->valid;
    pEntry->medCapa.iCapa = pMedia_capa_tlv->capa;
    pEntry->medCapa.iDevType = pMedia_capa_tlv->classType;

    if(FALSE == pMedia_capa_tlv->valid)
    {
        return SYS_ERR_OK;
    }
    else
        pEntry->medCapa.iCapaCurrent |= 1 << 0;

    osal_memset(tmp, 0, sizeof(tmp));

    if(pMedia_capa_tlv->capa & 0x1)
        osal_strcat(tmp, ", Capabilities");
    if(pMedia_capa_tlv->capa & 0x2)
        osal_strcat(tmp, ", Network Policy");
    if(pMedia_capa_tlv->capa & 0x4)
        osal_strcat(tmp, ", Location");
    if(pMedia_capa_tlv->capa & 0x8)
        osal_strcat(tmp, ", Extended PSE");
    if(pMedia_capa_tlv->capa & 0x10)
        osal_strcat(tmp, ", Extended PD");
    if(pMedia_capa_tlv->capa & 0x20)
        osal_strcat(tmp, ", Inventory");

    if(strlen(tmp) > 0)
        osal_strcat(pEntry->medCapa.capa, &tmp[2]);

    switch(pMedia_capa_tlv->classType)
    {
        case LLDP_CLASS_TYPE_UNDEF:
            osal_strcpy(pEntry->medCapa.devType, "Type Not Defined");
            break;
        case LLDP_CLASS_TYPE_ENDPOINT_CLASS1:
            osal_strcpy(pEntry->medCapa.devType, "Endpoint Class 1");
            break;
        case LLDP_CLASS_TYPE_ENDPOINT_CLASS2:
            osal_strcpy(pEntry->medCapa.devType, "Endpoint Class 2");
            break;
        case LLDP_CLASS_TYPE_ENDPOINT_CLASS3:
            osal_strcpy(pEntry->medCapa.devType, "Endpoint Class 3");
            break;
        case LLDP_CLASS_TYPE_NETWORK_CONN:
            osal_strcpy(pEntry->medCapa.devType, "Network Connectivity");
            break;
        default:
            osal_strcpy(pEntry->medCapa.devType, "Reserved");
            break;
    }

    return SYS_ERR_OK;
}

int32 display_networkPolicy(display_med_entry_t *pEntry, lldp_med_networkPolicy_tlv_t *policy_tlv)
{
    uint8 idx;

    for(idx = 0; idx < MAX_LLDP_MED_NETWORK_POLICY_RX; idx ++)
    {
        pEntry->networkPolicy[idx].valid = policy_tlv[idx].valid;

        if(FALSE == policy_tlv[idx].valid)
        {
            continue;
        }
        else
            pEntry->medCapa.iCapaCurrent |= 1 << 1;

        pEntry->networkPolicy[idx].iType = policy_tlv[idx].app;

        switch(policy_tlv[idx].app)
        {
            case LLDP_APP_VOICE:
                osal_strcpy(pEntry->networkPolicy[idx].type, "Voice");
                break;
            case LLDP_APP_VOICE_SIGNALING:
                osal_strcpy(pEntry->networkPolicy[idx].type, "Voice Signaling");
                break;
            case LLDP_APP_GUEST_VOICE:
                osal_strcpy(pEntry->networkPolicy[idx].type, "Guest Voice");
                break;
            case LLDP_APP_GUEST_VOICE_SIGNALING:
                osal_strcpy(pEntry->networkPolicy[idx].type, "Guest Voice Signaling");
                break;
            case LLDP_APP_SOFTPHONE_VOICE:
                osal_strcpy(pEntry->networkPolicy[idx].type, "Softphone Voice");
                break;
            case LLDP_APP_VIDEO_CONFERENCING:
                osal_strcpy(pEntry->networkPolicy[idx].type, "Video Conferencing");
                break;
            case LLDP_APP_STREAMING_VIDEO:
                osal_strcpy(pEntry->networkPolicy[idx].type, "Streaming Video");
                break;
            case LLDP_APP_VIDEO_SIGNALING:
                osal_strcpy(pEntry->networkPolicy[idx].type, "Video Signaling");
                break;
            default:
                break;
        }

        pEntry->networkPolicy[idx].isUnknown = policy_tlv[idx].policyInfo.unknown;

        if(policy_tlv[idx].policyInfo.unknown == 0)
            osal_strcpy(pEntry->networkPolicy[idx].flag, "Defined");
        else
            osal_strcpy(pEntry->networkPolicy[idx].flag, "Unknown Policy");

        pEntry->networkPolicy[idx].isTagged = policy_tlv[idx].policyInfo.tag;
        if(policy_tlv[idx].policyInfo.tag == 1)
            osal_strcpy(pEntry->networkPolicy[idx].vlanTag, "Tagged VLAN");
        else
            osal_strcpy(pEntry->networkPolicy[idx].vlanTag, "Untagged VLAN");

        pEntry->networkPolicy[idx].vid = policy_tlv[idx].policyInfo.vlanId;
        pEntry->networkPolicy[idx].l2Pri = policy_tlv[idx].policyInfo.l2Pri;
        pEntry->networkPolicy[idx].dscpPri = policy_tlv[idx].policyInfo.dscpPri;

    }

    return SYS_ERR_OK;
}

int32 display_inventory(display_med_entry_t *pEntry, neighbor_entry_t *pNeigoborEntry)
{
    if(pNeigoborEntry->hardware_v_tlv.valid || pNeigoborEntry->firmware_v_tlv.valid || pNeigoborEntry->software_v_tlv.valid ||
        pNeigoborEntry->serial_num_tlv.valid || pNeigoborEntry->manufacturer_tlv.valid || pNeigoborEntry->model_name_tlv.valid ||
        pNeigoborEntry->asset_tlv.valid)
        pEntry->medCapa.iCapaCurrent |= 1 << 5;
    osal_memcpy(&pEntry->hardware_v, &pNeigoborEntry->hardware_v_tlv, sizeof(pNeigoborEntry->hardware_v_tlv));
    osal_memcpy(&pEntry->firmware_v, &pNeigoborEntry->firmware_v_tlv, sizeof(pNeigoborEntry->firmware_v_tlv));
    osal_memcpy(&pEntry->software_v, &pNeigoborEntry->software_v_tlv, sizeof(pNeigoborEntry->software_v_tlv));
    osal_memcpy(&pEntry->serial_num, &pNeigoborEntry->serial_num_tlv, sizeof(pNeigoborEntry->serial_num_tlv));
    osal_memcpy(&pEntry->manufacturer, &pNeigoborEntry->manufacturer_tlv, sizeof(pNeigoborEntry->manufacturer_tlv));
    osal_memcpy(&pEntry->model_name, &pNeigoborEntry->model_name_tlv, sizeof(pNeigoborEntry->model_name_tlv));
    osal_memcpy(&pEntry->asset, &pNeigoborEntry->asset_tlv, sizeof(pNeigoborEntry->asset_tlv));

    return SYS_ERR_OK;
}

int32 display_location(display_med_entry_t *pEntry, lldp_med_location_tlv_t *pLocation_tlv)
{
    int32 i;
    char tmp[16];

    pEntry->locationCoor.valid = FALSE;
    pEntry->locationCivic.valid = FALSE;
    pEntry->locationEcs.valid = FALSE;

    if(FALSE == pLocation_tlv->valid)
    {
        return SYS_ERR_OK;
    }
    else
        pEntry->medCapa.iCapaCurrent |= 1 << 2;

    if(pLocation_tlv->location.coorLen > 0)
    {
        pEntry->locationCoor.valid = TRUE;
        osal_memcpy(pEntry->locationCoor.iData, pLocation_tlv->location.coordinate, sizeof(pLocation_tlv->location.coordinate));
        osal_sprintf(tmp, "%02X", pLocation_tlv->location.coordinate[0]);
        osal_strcat(pEntry->locationCoor.data, tmp);

        for(i = 1; i < pLocation_tlv->location.coorLen; i ++)
        {
            osal_sprintf(tmp, ":%02X", pLocation_tlv->location.coordinate[i]);
            osal_strcat(pEntry->locationCoor.data, tmp);
        }
    }

    if(pLocation_tlv->location.civicLen > 0)
    {
        pEntry->locationCivic.valid = TRUE;
        osal_memcpy(pEntry->locationCivic.iData, pLocation_tlv->location.civicAddress, sizeof(pLocation_tlv->location.civicAddress));
        osal_sprintf(tmp, "%02X", pLocation_tlv->location.civicAddress[0]);
        osal_strcat(pEntry->locationCivic.data, tmp);

        for(i = 1; i < pLocation_tlv->location.civicLen; i ++)
        {
            osal_sprintf(tmp, ":%02X", pLocation_tlv->location.civicAddress[i]);
            osal_strcat(pEntry->locationCivic.data, tmp);
        }
    }

    if(pLocation_tlv->location.ecsLen > 0)
    {
        pEntry->locationEcs.valid = TRUE;
        osal_memcpy(pEntry->locationEcs.iData, pLocation_tlv->location.ecsElin, sizeof(pLocation_tlv->location.ecsElin));
        osal_sprintf(tmp, "%02X", pLocation_tlv->location.ecsElin[0]);
        osal_strcat(pEntry->locationEcs.data, tmp);

        for(i = 1; i < pLocation_tlv->location.ecsLen; i ++)
        {
            osal_sprintf(tmp, ":%02X", pLocation_tlv->location.ecsElin[i]);
            osal_strcat(pEntry->locationEcs.data, tmp);
        }
    }

    return SYS_ERR_OK;
}
 #endif 

uint8 check_tlv_len(uint16 remainLen, uint16 len ,uint8 type)
{

    if(len + TLVH_OFFSET > remainLen)
        return XEINVALIDTLV;

    switch(type)
    {
        case CHASSIS_ID_TLV:            
        case PORT_ID_TLV:               
            if(1 <= len && len <= 255)
                return XVALIDTLV;
            else
                return XEINVALIDTLV;

        case TIME_TO_LIVE_TLV:          
            if(TTL_TLV_LEN == len)
                return XVALIDTLV;
            else
                return XEINVALIDTLV;

        case PORT_DESCRIPTION_TLV:      
            if(len > MAX_TLV_STRING_LEN)
                return XEINVALIDTLV;
            else
                return XVALIDTLV;

        case SYSTEM_NAME_TLV:           
            if(len > CAPA_SYS_NAME_LEN)
                return XEINVALIDTLV;
            else
                return XVALIDTLV;

        case SYSTEM_DESCRIPTION_TLV:    
            if(len > CAPA_SYS_DESC_LEN)
                return XEINVALIDTLV;
            else
                return XVALIDTLV;

        case SYSTEM_CAPABILITIES_TLV:   
            if (len == CAPA_TLV_LEN)
                return XVALIDTLV;
            else
                return XEINVALIDTLV;

        case MANAGEMENT_ADDRESS_TLV:    
            if (9 <= len && len <= 167)
                return XVALIDTLV;
            else
                return XEINVALIDTLV;

        case ORG_SPECIFIC_TLV:          
            if (4 <= len && len <= 511)
                return XVALIDTLV;
            else
                return XEINVALIDTLV;

        case END_OF_LLDPDU_TLV:         
            if (END_TLV_LEN == len)
                return XVALIDTLV;
            else
                return XEINVALIDTLV;

        default:
            return XVALIDTLV;
    }

    return XVALIDTLV;
}

int32 lldp_dump_packet(uint8 *pktData, uint32 pktLen, uint8 isTx, sys_logic_port_t port)
{
    uint32  i, j;
    uint8   len = 10;
    char    buf[50];

    if (IS_DBG_SET(DBG_LLDP_PKT))
    {
        LLDP_DEBUG_PRINT_SEM_LOCK();
        LLDP_PKT_DBG("========== LLDP dump pkt %s port %s ============", (1 == isTx)?"TX to":"RX from", LPSTR(port));

        for (i = 0; i < pktLen; i += len)
        {
            if (pktLen < i + len)
                len = pktLen - i;

            osal_memset(buf, 0, 50);
            for (j = 0; j < len; ++j)
                sprintf(&buf[j * 3], "%02x ", *(pktData + i + j));

            LLDP_PKT_DBG("%s", buf);
        }

        LLDP_PKT_DBG("========== LLDP dump pkt end ============");
        LLDP_DEBUG_PRINT_SEM_UNLOCK();
    }

    return SYS_ERR_OK;
}
