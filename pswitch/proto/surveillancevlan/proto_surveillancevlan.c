#include <common/sys_def.h>
#include <rsd/rsd_acl.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_qos.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_vlan_surveillance.h>
#include <surveillancevlan/proto_surveillancevlan.h>
static int32 _sv_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);

typedef enum _surveillance_vlan_update_mode_e
{
    SV_UPDATE_PORT_MODE = 0,
    SV_UPDATE_COS,
    SV_UPDATE_END
}_surveillance_vlan_update_mode_t;
osal_mutex_t g_svsem;
static uint32 g_rxPkts = 0;
static uint32 g_aging;
static sys_vid_t g_vid;
static sys_pri_t g_pri;
static sys_qid_t g_qid;
static sys_enable_t g_state;
static sys_enable_t g_remarkEnable;
static sys_logic_portmask_t g_portEnablePM;
static sys_logic_portmask_t g_portCoSSrcPM;
static sys_logic_portmask_t g_portManualPM;
static sys_logic_portmask_t g_copy2CpuPM[CAPA_VLAN_SURVEILLANCE_OUI_NUM];
static surveillance_vlan_oui_t g_oui[CAPA_VLAN_SURVEILLANCE_OUI_NUM];

static volatile int32 g_timer_thread_status = 0;
static uint32 g_portAgingTimer[SYS_LOGICPORT_NUM_MAX];
static sys_enable_t g_timer_update[SYS_LOGICPORT_NUM_MAX];
static osal_timer_list_t oui_timer[CAPA_VLAN_SURVEILLANCE_OUI_NUM][SYS_LOGICPORT_NUM_MAX];

static rsd_nic_handler_t l2gOUIHandler = {
    .priority       = RSD_NIC_HANDLER_PRI_SURVEILLANCE_VLAN_OUI,
    .dmac           = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .dmac_caremask  = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .smac           = { 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0x0 },
    .smac_caremask  = { 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0x0 },
    .ethertype      = 0x0,
    .proto          = 0,
    .sport          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_COPY,
    .rx_cbf         = _sv_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_TRUNK,
    .pName          = "Surveillance VLAN RX Thread",
    .pCookie        = NULL,
};
#define SV_MBR_PIE_TIMER      (30)  
#define SV_SEM_LOCK()    \
do{                             \
    if (SYS_ERR_OK != osal_sem_mutex_take(g_svsem, OSAL_SEM_WAIT_FOREVER)){    \
        SV_DBG("VLAN surveillance Lock Error");     \
        return SYS_ERR_SEM_LOCK;}          \
}while(0)

#define SV_SEM_UNLOCK()    \
do{                     \
    if (SYS_ERR_OK != osal_sem_mutex_give(g_svsem)){    \
        SV_DBG("VLAN surveillance unLock Error");     \
        return SYS_ERR_SEM_UNLOCK;}          \
}while(0)
static int32 _sv_copyEntry_set(uint32 idx,sys_enable_t enable)
{
    sys_logic_port_t lp;
    rsd_sv_entry_t svEntry;

    SYS_MEM_CLEAR(svEntry);

    SV_DBG("_sv_copyEntry_set idx %u state %u oui %x:%x:%x",
            idx, enable, g_oui[idx].oui[0], g_oui[idx].oui[1], g_oui[idx].oui[2]);

    if(DISABLED == enable)
    {
        SYS_ERR_CHK(rsd_vlan_surveillance_chip_copyEntry_del(idx));

        return SYS_ERR_OK;
    }

    LOGIC_PORTMASK_COPY(svEntry.spm, g_copy2CpuPM[idx]);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, svEntry.spm)
    {
        if(IS_LP_TRKMBR(lp))
            LOGIC_PORTMASK_CLEAR_PORT(svEntry.spm, lp);
    }

    if(IS_LOGIC_PORTMASK_CLEAR(svEntry.spm))
        SYS_ERR_CHK(rsd_vlan_surveillance_chip_copyEntry_del(idx));
    else
    {
        svEntry.vid = g_vid;
        osal_memcpy(svEntry.oui, g_oui[idx].oui, SNAPOUI_LEN);
        SYS_ERR_CHK(rsd_vlan_surveillance_chip_copyEntry_set(idx, &svEntry));
    }

    return SYS_ERR_OK;
}

static int32 _sv_cosSrcEntry_set(uint32 idx,sys_enable_t enable)
{
    sys_logic_port_t lp;
    rsd_sv_entry_t svEntry;

    SYS_MEM_CLEAR(svEntry);

    SV_DBG("_sv_cosSrcEntry_set idx %u state %u oui %x:%x:%x",
            idx, enable, g_oui[idx].oui[0], g_oui[idx].oui[1], g_oui[idx].oui[2]);

    if(DISABLED == enable)
    {
        SYS_ERR_CHK(rsd_vlan_surveillance_chip_cosSrcEntry_del(idx));

        return SYS_ERR_OK;
    }

    LOGIC_PORTMASK_AND(svEntry.spm, g_portEnablePM, g_portCoSSrcPM);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, svEntry.spm)
    {
        if(IS_LP_TRKMBR(lp))
            LOGIC_PORTMASK_CLEAR_PORT(svEntry.spm, lp);
    }

    if(IS_LOGIC_PORTMASK_CLEAR(svEntry.spm))
        SYS_ERR_CHK(rsd_vlan_surveillance_chip_cosSrcEntry_del(idx));
    else
    {
        svEntry.vid = g_vid;
        svEntry.qid = g_qid;
        svEntry.pri = g_pri;
        svEntry.remark = g_remarkEnable;
        osal_memcpy(svEntry.oui, g_oui[idx].oui, SNAPOUI_LEN);
        SYS_ERR_CHK(rsd_vlan_surveillance_chip_cosSrcEntry_set(idx, &svEntry));
    }

    return SYS_ERR_OK;
}

static int32 _sv_cosAllEntry_set(sys_enable_t enable)
{
    sys_logic_port_t lp;
    rsd_sv_entry_t svEntry;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    SYS_MEM_CLEAR(svEntry);

    if(DISABLED == enable)
    {
        SYS_ERR_CHK(rsd_vlan_surveillance_chip_cosAllEntry_del());

        return SYS_ERR_OK;
    }

    LOGIC_PORTMASK_ANDNOT(svEntry.spm, g_portEnablePM, g_portCoSSrcPM);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, svEntry.spm)
    {
        if(IS_LP_TRKMBR(lp))
            LOGIC_PORTMASK_CLEAR_PORT(svEntry.spm, lp);
    }

    if(IS_LOGIC_PORTMASK_CLEAR(svEntry.spm))
        SYS_ERR_CHK(rsd_vlan_surveillance_chip_cosAllEntry_del());
    else
    {

        svEntry.vid = g_vid;
        svEntry.qid = g_qid;
        svEntry.pri = g_pri;
        svEntry.remark = g_remarkEnable;
        SYS_ERR_CHK(rsd_vlan_surveillance_chip_cosAllEntry_set(&svEntry));
    }

    return SYS_ERR_OK;
}

static int32 _sv_copyEntry_update(sys_enable_t enable)
{
    uint32 i;

    for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        if (!IS_OUI_CLEAR(g_oui[i].oui))
            SYS_ERR_CHK(_sv_copyEntry_set(i, enable));
    }

    return SYS_ERR_OK;
}

static int32 _sv_cosEntry_update(sys_enable_t enable)
{
    uint32 i;

    for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        if (!IS_OUI_CLEAR(g_oui[i].oui))
            SYS_ERR_CHK(_sv_cosSrcEntry_set(i,enable));
    }

    SYS_ERR_CHK(_sv_cosAllEntry_set(enable));

    return SYS_ERR_OK;
}

static int32 _sv_holdtime_update(void)
{
    sys_logic_port_t lp;

    if (DISABLED == g_state)
        return SYS_ERR_OK;

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (!IS_LOGIC_PORTMASK_PORTSET(g_portEnablePM, lp))
            continue;

        if (IS_LOGIC_PORTMASK_PORTSET(g_portManualPM, lp))
            continue;

        if(DISABLED == g_timer_update[lp])
            continue;

        if (g_portAgingTimer[lp] > 0)
        {
            g_portAgingTimer[lp]--;

            if (0 == g_portAgingTimer[lp])
            {
                SYS_ERR_CHK(rsd_vlan_portSurveillanceVlanActive_set(lp, DISABLED));
                SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_SURVEILLANCE_MBR_DEL, LPSTR(lp), g_vid);
                SV_DBG("Port %s is delete from surveillance vlan %u", LPSTR(lp), g_vid);
            }
        }
    }

    return SYS_ERR_OK;
}

static void _sv_oui_hold_timer_expired(unsigned long data)
{
    uint32 i;
    sys_logic_port_t lp;

    i = data / SYS_LOGICPORT_NUM_MAX;
    lp = data % SYS_LOGICPORT_NUM_MAX;

    SV_DBG("member update entry:%u For Add Port %s", i, LPSTR(lp));

    g_timer_update[lp] = ENABLED;
    LOGIC_PORTMASK_SET_PORT(g_copy2CpuPM[i], lp);
    _sv_copyEntry_set(i, ENABLED);
}

static void _sv_timer_thread(void *pInput)
{
    for (;;)
    {
        osal_time_usleep(1 * 1000 * 1000); 
        _sv_holdtime_update();

        if (g_timer_thread_status)
            break;
    }
    g_timer_thread_status = -1;

    osal_thread_exit(0);
}

static int32 _sv_qosEvent(uint32 event, char *pData)
{
    sys_qos_event_t *pQosEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);
    pQosEvent = (sys_qos_event_t *)pData;

    if (SYS_NT_QOS_COS2QUEUE_CHG == event)
    {
        SV_DBG("Qos event cos %u to queue %u",pQosEvent->cos,pQosEvent->qid);

        if(pQosEvent->cos == g_pri)
        {
            SV_SEM_LOCK();
            SYS_ERR_CHK(rsd_qos_cosQueue_get(g_pri, &g_qid));
            SV_SEM_UNLOCK();
        }
    }

    return SYS_ERR_OK;
}

static int32 _sv_linkEvent(uint32 event, char *pData)
{
    uint32 i;
    sys_enable_t enable;
    sys_port_event_t *pPortEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pPortEvent = (sys_port_event_t *)pData;
    SYS_ERR_CHK(rsd_vlan_portSurveillanceVlanActive_get(pPortEvent->lport,&enable));

    if(DISABLED == g_state)
        return SYS_ERR_OK;

    if((ENABLED == enable) && (PORT_LINKDOWN == pPortEvent->new_linkStatus))
    {
        SYS_ERR_CHK(rsd_vlan_portSurveillanceVlanActive_set(pPortEvent->lport, DISABLED));

        SV_SEM_LOCK();

        g_portAgingTimer[pPortEvent->lport] = 0;

        for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
        {
            osal_timer_stop(&oui_timer[i][pPortEvent->lport]);
            LOGIC_PORTMASK_SET_PORT(g_copy2CpuPM[i], pPortEvent->lport);
        }

        _sv_copyEntry_update(ENABLED);
        SV_SEM_UNLOCK();

        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_SURVEILLANCE_MBR_DEL,
            LPSTR(pPortEvent->lport), g_vid);
        SV_DBG("Port %s is Del from surveillance VLAN with LinkDown",LPSTR(pPortEvent->lport));
    }

    return SYS_ERR_OK;
}

static int32 _sv_trunkEvent(uint32 event, char *pData)
{
    sys_trunk_event_t *pTrunkEvent;
    sys_logic_portmask_t carePm;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pTrunkEvent = (sys_trunk_event_t *)pData;

    if (DISABLED == g_state)
        return SYS_ERR_OK;

    LOGIC_PORTMASK_COPY(carePm, pTrunkEvent->portmask);
    LOGIC_PORTMASK_SET_PORT(carePm, TRK2LP(pTrunkEvent->trunkIdx));
    LOGIC_PORTMASK_AND(carePm, carePm, g_portEnablePM);

    if(!IS_LOGIC_PORTMASK_CLEAR(carePm))
    {
        SYS_ERR_CHK(_sv_cosEntry_update(ENABLED));
        SYS_ERR_CHK(_sv_copyEntry_update(ENABLED));
    }

    return SYS_ERR_OK;
}

static int32 _sv_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    uint8 macStr[CAPA_MAC_STR_LEN];
    uint32 i;
    sys_logic_port_t slp;
    sys_vid_t vid = DFLT_VLAN_DFLT_VLAN;

    SV_DBG("Rx packet count:%u",g_rxPkts++);
    slp = pPkt->rx_tag.source_port;
    SYS_MEM_CLEAR(macStr);
    SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + 6, macStr));
    SYS_ERR_CHK(rsd_nic_pktVid_get(pPkt, &vid));

    if (DISABLED == g_state)
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_SURVEILLANCE_GLOBAL_DROP, LPSTR(slp), macStr);
        goto out;
    }

    if (!IS_LOGIC_PORTMASK_PORTSET(g_portEnablePM, slp))
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_SURVEILLANCE_INTF_DROP, LPSTR(slp), macStr);
        goto out;
    }

    if (vid != g_vid)
        goto out;

    if((IS_TRKLP(slp)) &&(IS_TRKLP_EMPTY(slp)))
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_SURVEILLANCE_INTF_DROP, LPSTR(slp), macStr);
        goto out;
    }

    for (i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        if ((g_oui[i].oui[0] == pPkt->data[6]) &&
            (g_oui[i].oui[1] == pPkt->data[7]) &&
            (g_oui[i].oui[2] == pPkt->data[8]))
            break;
    }

    if (CAPA_VLAN_SURVEILLANCE_OUI_NUM == i)
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_SURVEILLANCE_OUI_DROP, LPSTR(slp), macStr);
        goto out;
    }

    if (g_portAgingTimer[slp] == 0)
    {
        SYS_ERR_CHK(rsd_vlan_portSurveillanceVlanActive_set(slp, ENABLED));
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_SURVEILLANCE_MBR_ADD, LPSTR(slp), g_vid, macStr);
        SV_DBG("Port %s is added to Surveillance VLAN", LPSTR(slp));
    }

    SV_SEM_LOCK();

    LOGIC_PORTMASK_CLEAR_PORT(g_copy2CpuPM[i],slp);
    _sv_copyEntry_set(i, ENABLED);
    g_timer_update[slp] = DISABLED;
    g_portAgingTimer[slp] = g_aging * 60;
    osal_timer_stop(&oui_timer[i][slp]);
    osal_timer_start(&oui_timer[i][slp], 1000 *60 * SV_MBR_PIE_TIMER);

    SV_SEM_UNLOCK();

out:
    if (SYS_ERR_OK != rsd_nic_pkt_free(pPkt))
        SV_DBG("Surveillance VLAN pkt free falied!");

    return SYS_ERR_OK;
}

static int32 _sv_acl_entryInfo_dump(sys_acl_entry_t *entry)
{
    ACLDBG("  idx: %d", entry->key.idx);

    return 0;
}

static int32 _sv_acl_entryPrio_cmp(sys_acl_entry_t *entry, sys_acl_entry_t *cmp)
{
    SYS_PARAM_CHK((NULL == entry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == cmp), SYS_ERR_NULL_POINTER);

    if (entry->key.idx < cmp->key.idx)
        return 1;

    return 0;
}

static int32 _sv_acl_entry2Str_get(sys_acl_entry_t *entry, char *buf, uint32 maxlen)
{
    uint32 ouiIdx = 0;
    surveillance_vlan_oui_t *oui = NULL;

    SYS_PARAM_CHK((NULL == entry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == buf), SYS_ERR_NULL_POINTER);

    if (entry->key.idx < (2 * CAPA_VLAN_SURVEILLANCE_OUI_NUM))
    {
        ouiIdx = entry->key.idx / 2;
        oui = &g_oui[ouiIdx];

        if (ACL_PROTOTYPE_SURVEILLANCE_VL == entry->type)
        {
            if (0 == entry->key.idx % 2)
                osal_sprintf(buf, "Copy member ACL for OuiIdx %u %02x:%02x:%02x", ouiIdx,
                    oui->oui[0], oui->oui[1], oui->oui[2]);
            else
                osal_sprintf(buf, "Cos Source and remark ACL for OuiIdx %u %02x:%02x:%02x", ouiIdx,
                    oui->oui[0], oui->oui[1], oui->oui[2]);
        }
#ifdef CONFIG_SYS_RTL8390
        else if (ACL_PROTOTYPE_SURVEILLANCE_VL_RMK == entry->type)
        {
            osal_sprintf(buf, "90 cos remark ACL for OuiIdx %u %02x:%02x:%02x", ouiIdx,
                oui->oui[0], oui->oui[1], oui->oui[2]);
        }
#endif
    }
    else
    {
        if (ACL_PROTOTYPE_SURVEILLANCE_VL == entry->type)
        {
            osal_sprintf(buf, "Cos All and remark ACL");
        }
#ifdef CONFIG_SYS_RTL8390
        else if (ACL_PROTOTYPE_SURVEILLANCE_VL_RMK == entry->type)
        {
            osal_sprintf(buf, "90 cos All remark ACL");
        }
#endif
    }

    return SYS_ERR_OK;
}
int32 surveillance_vlanVid_set(sys_vid_t vid)
{
    SYS_PARAM_CHK(IS_SYSTEM_VID_INVALID(vid), SYS_ERR_VLAN_ID);

    SV_DBG("Surveillance VLAN ID is set to %u", vid);

    SV_SEM_LOCK();

    g_vid = vid;
    rsd_vlan_surveillanceVid_set(vid);

    if(ENABLED == g_state)
    {
        _sv_cosEntry_update(ENABLED);
        _sv_copyEntry_update(ENABLED);
    }

    SV_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 surveillance_vlanPri_set(sys_pri_t pri)
{
    SYS_PARAM_CHK(IS_PRI_INVALID(pri), SYS_ERR_PRIORITY);

    SV_DBG("Surveillance VLAN CoS is set to %u", pri);

    SV_SEM_LOCK();

    g_pri = pri;
    rsd_qos_cosQueue_get(g_pri, &g_qid);

    if(ENABLED == g_state)
        _sv_cosEntry_update(ENABLED);

    SV_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 surveillance_vlanState_set(sys_enable_t state)
{
    uint32 i = 0;
    sys_logic_port_t lp;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(state), SYS_ERR_INPUT);

    SV_DBG("Surveillance VLAN is set to %s", text_enable[state].text);

    SV_SEM_LOCK();

    g_state = state;

    if (ENABLED == state)
    {
        for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
        {
            if (!IS_OUI_CLEAR(g_oui[i].oui))
            {
                
                osal_memcpy(l2gOUIHandler.smac, g_oui[i].oui, SNAPOUI_LEN);
                osal_memset(l2gOUIHandler.smac_caremask, 0xff, SNAPOUI_LEN);
                osal_sprintf(l2gOUIHandler.pName, "SV RX %u %x:%x:%x", i, g_oui[i].oui[0],
                          g_oui[i].oui[1], g_oui[i].oui[2]);
                rsd_nic_handler_register(&l2gOUIHandler);
            }
        }
    }
    else
    {
        _sv_copyEntry_update(DISABLED);
        _sv_cosEntry_update(DISABLED);

        for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
        {
            FOR_EACH_LOGIC_PORT(lp)
                osal_timer_stop(&oui_timer[i][lp]);

            if (!IS_OUI_CLEAR(g_oui[i].oui))
            {
                osal_memcpy(l2gOUIHandler.smac, g_oui[i].oui, SNAPOUI_LEN);
                osal_memset(l2gOUIHandler.smac_caremask, 0xff, SNAPOUI_LEN);
                osal_sprintf(l2gOUIHandler.pName, "SV RX %u %x:%x:%x", i, g_oui[i].oui[0],
                          g_oui[i].oui[1], g_oui[i].oui[2]);
                rsd_nic_handler_unregister(&l2gOUIHandler);
            }
        }
    }

    SV_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 surveillance_vlanAging_set(uint32 aging)
{
    sys_logic_port_t lp;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(aging, BOND_VLAN_SURVEILLANCE_AGING_MIN, BOND_VLAN_SURVEILLANCE_AGING_MAX), SYS_ERR_OUT_OF_RANGE);

    SV_DBG("Surveillance VLAN aging is set to %u minute(s)", aging);

    SV_SEM_LOCK();

    g_aging = aging;

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (g_portAgingTimer[lp] > 0)
            g_portAgingTimer[lp] = aging * 60;
    }

    SV_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 surveillance_vlanOuiByIdx_set(uint32 idx, surveillance_vlan_oui_t *pOui)
{
    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);

    SV_DBG("Surveillance VLAN set OUI %u %02x:%02x:%02x with name %s", idx,
        pOui->oui[0], pOui->oui[1], pOui->oui[2], pOui->ouiName.str);

    SV_SEM_LOCK();

    if (IS_OUI_CLEAR(pOui->oui))
    {
        _sv_copyEntry_set(idx, DISABLED);
        _sv_cosSrcEntry_set(idx, DISABLED);

        osal_memcpy(l2gOUIHandler.smac, g_oui[idx].oui, SNAPOUI_LEN);
        osal_memset(l2gOUIHandler.smac_caremask, 0xff, SNAPOUI_LEN);
        osal_sprintf(l2gOUIHandler.pName, "SV RX %u %x:%x:%x", idx, g_oui[idx].oui[0],
                          g_oui[idx].oui[1], g_oui[idx].oui[2]);
        rsd_nic_handler_unregister(&l2gOUIHandler);
        SYS_MEM_CLEAR(g_oui[idx]);
    }
    else
    {
        osal_memcpy(&g_oui[idx], pOui, sizeof(surveillance_vlan_oui_t));

        if (ENABLED == g_state)
        {
            
            osal_memcpy(l2gOUIHandler.smac, g_oui[idx].oui, SNAPOUI_LEN);
            osal_memset(l2gOUIHandler.smac_caremask, 0xff, SNAPOUI_LEN);
            osal_sprintf(l2gOUIHandler.pName, "SV RX %u %x:%x:%x", idx, g_oui[idx].oui[0],
                          g_oui[idx].oui[1], g_oui[idx].oui[2]);

            rsd_nic_handler_register(&l2gOUIHandler);

            _sv_copyEntry_set(idx, ENABLED);
            _sv_cosSrcEntry_set(idx, ENABLED);
        }
    }

    SV_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 surveillance_vlan1pRemarkEnable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SV_DBG("Surveillance VLAN 1p remark is set to %s", text_enable[enable].text);

    SV_SEM_LOCK();

    g_remarkEnable = enable;

    if (ENABLED == g_state)
        _sv_cosEntry_update(ENABLED);

    SV_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 surveillance_vlan_portEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    uint32 i = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    SV_DBG("%s surveillance VLAN is set to %s", LPSTR(port), text_enable[enable].text);

    SV_SEM_LOCK();

    if (ENABLED == enable)
    {
        LOGIC_PORTMASK_SET_PORT(g_portEnablePM, port);

        if(!IS_LOGIC_PORTMASK_PORTSET(g_portManualPM, port))
        {
            for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
                LOGIC_PORTMASK_SET_PORT(g_copy2CpuPM[i],port);
        }
    }
    else
    {
        LOGIC_PORTMASK_CLEAR_PORT(g_portEnablePM,port);

        for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
        {
            osal_timer_stop(&oui_timer[i][port]);
            LOGIC_PORTMASK_CLEAR_PORT(g_copy2CpuPM[i], port);
        }

        if ((g_portAgingTimer[port] > 0))
        {
            g_portAgingTimer[port] = 0;
            rsd_vlan_portSurveillanceVlanActive_set(port, DISABLED);
        }
    }

    if (ENABLED == g_state)
    {
        _sv_cosEntry_update(ENABLED);
        _sv_copyEntry_update(ENABLED);
    }

    SV_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 surveillance_vlan_portMode_set(sys_logic_port_t port, surveillance_vlan_port_mode_t mode)
{
    uint32 i = 0;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((mode >= SURVEILLANCE_VLAN_PORT_MODE_END), SYS_ERR_INPUT);

    SV_DBG("%s surveillance VLAN is set to %s", LPSTR(port),
        (SURVEILLANCE_VLAN_PORT_MODE_AUTO == mode)?"Auto":"Manual");

    SV_SEM_LOCK();

    if (SURVEILLANCE_VLAN_PORT_MODE_MANUAL == mode)
    {
        LOGIC_PORTMASK_SET_PORT(g_portManualPM, port);

        for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
        {
            osal_timer_stop(&oui_timer[i][port]);
            LOGIC_PORTMASK_CLEAR_PORT(g_copy2CpuPM[i], port);
        }

        if (g_portAgingTimer[port] > 0)
        {
            rsd_vlan_portSurveillanceVlanActive_set(port,DISABLED);
            g_portAgingTimer[port] = 0;
        }
    }
    else
    {
        LOGIC_PORTMASK_CLEAR_PORT(g_portManualPM, port);

        if(IS_LOGIC_PORTMASK_PORTSET(g_portEnablePM, port))
        {
            for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
                LOGIC_PORTMASK_SET_PORT(g_copy2CpuPM[i], port);
        }
    }

    if(ENABLED == g_state)
    {
        _sv_cosEntry_update(ENABLED);
        _sv_copyEntry_update(ENABLED);
    }

    SV_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 surveillance_vlan_portCosMode_set(sys_logic_port_t port, surveillance_vlan_cos_mode_t cosMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((cosMode>= SURVEILLANCE_VLAN_PORT_COS_END), SYS_ERR_INPUT);

    SV_DBG("sv port %u cos mode set to %u", port, cosMode);

    SV_DBG("%s surveillance VLAN is set to %s", LPSTR(port),
        (SURVEILLANCE_VLAN_PORT_COS_SRC == cosMode)?"Source":"All");

    SV_SEM_LOCK();

    if (SURVEILLANCE_VLAN_PORT_COS_SRC == cosMode)
        LOGIC_PORTMASK_SET_PORT(g_portCoSSrcPM, port);
    else
        LOGIC_PORTMASK_CLEAR_PORT(g_portCoSSrcPM, port);

    if(ENABLED == g_state)
        _sv_cosEntry_update(ENABLED);

    SV_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 surveillance_vlan_init(void)
{
    uint32 i = 0;
    sys_logic_port_t lp;
    sys_acl_cb_t cb_info;

    g_svsem = osal_sem_mutex_create();

    g_aging = DFLT_VLAN_SURVEILLANCE_AGING;
    g_vid = DFLT_VLAN_SURVEILLANCE_VLAN;
    g_pri = DFLT_VLAN_SURVEILLANCE_PRI;
    g_state = DFLT_VLAN_SURVEILLANCE_EBL;
    g_remarkEnable = DFLT_VLAN_SURVEILLANCE_REMARK_EBL;
    SYS_MEM_CLEAR(g_oui);
    SYS_MEM_CLEAR(g_copy2CpuPM);
    SYS_MEM_CLEAR(g_portEnablePM);
    SYS_MEM_CLEAR(g_portCoSSrcPM);
    SYS_MEM_CLEAR(g_portManualPM);
    SYS_MEM_CLEAR(g_portAgingTimer);
    SYS_MEM_CLEAR(g_timer_update);

    FOR_EACH_LOGIC_PORT(lp)
    {
        if (DFLT_VLAN_SURVEILLANCE_PORT_COS_MODE == SURVEILLANCE_VLAN_PORT_COS_SRC)
            LOGIC_PORTMASK_SET_PORT(g_portCoSSrcPM, lp);

        if (DFLT_VLAN_SURVEILLANCE_PORT_EBL == ENABLED)
            LOGIC_PORTMASK_SET_PORT(g_portEnablePM, lp);

        if (DFLT_VLAN_SURVEILLANCE_PORT_MODE == SURVEILLANCE_VLAN_PORT_MODE_MANUAL)
            LOGIC_PORTMASK_SET_PORT(g_portManualPM, lp);
    }

    for(i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        FOR_EACH_LOGIC_PORT(lp)
        {
            SYS_ERR_CHK(osal_timer_init(&oui_timer[i][lp],
                _sv_oui_hold_timer_expired, i * SYS_LOGICPORT_NUM_MAX + lp));
        }
    }

    SYS_ERR_CHK(sys_notifier_observer_register("surveillance vlan", SYS_NOTIFIER_SUBJECT_QOS,
        SYS_NT_QOS_COS2QUEUE_CHG, _sv_qosEvent, SYS_NOTIFIER_PRI_MEDIUM));

    SYS_ERR_CHK(sys_notifier_observer_register("surveillance vlan", SYS_NOTIFIER_SUBJECT_PORT,
        SYS_NT_PORT_LINK_CHG, _sv_linkEvent, SYS_NOTIFIER_PRI_MEDIUM));

    SYS_ERR_CHK(sys_notifier_observer_register("surveillance vlan", SYS_NOTIFIER_SUBJECT_TRUNK,
        (SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE), _sv_trunkEvent, SYS_NOTIFIER_PRI_MEDIUM));

    if ((osal_thread_t)NULL == (osal_thread_create("Surveillance VLAN Timer Thread", 32768,
        DFLT_THREAD_PRI_SURVEVLAN_TIMER, (void *)_sv_timer_thread, NULL)))
    {
        SV_DBG("surveillance vlan create timer thread failed");
        return SYS_ERR_FAILED;
    }

    SYS_MEM_CLEAR(cb_info);
    cb_info.entry_info_dump = _sv_acl_entryInfo_dump;
    cb_info.entry_str_get = _sv_acl_entry2Str_get;
    cb_info.entry_prio_cmp = _sv_acl_entryPrio_cmp;
    SYS_ERR_CHK(rsd_acl_cb_register(ACL_PROTOTYPE_SURVEILLANCE_VL, &cb_info));

#ifdef CONFIG_SYS_RTL8390
    if (IS_CHIP_CYPRESS())
        SYS_ERR_CHK(rsd_acl_cb_register(ACL_PROTOTYPE_SURVEILLANCE_VL_RMK, &cb_info));
#endif

    return SYS_ERR_OK;
}
int32 surveillance_vlan_exit(void)
{
    
    g_timer_thread_status = 1;
    do { } while (g_timer_thread_status >= 0);

    SYS_ERR_CHK(sys_notifier_observer_unregister("surveillance vlan", SYS_NOTIFIER_SUBJECT_QOS,
        _sv_qosEvent));

    SYS_ERR_CHK(sys_notifier_observer_unregister("surveillance vlan", SYS_NOTIFIER_SUBJECT_PORT,
        _sv_linkEvent));

    SYS_ERR_CHK(sys_notifier_observer_unregister("surveillance vlan", SYS_NOTIFIER_SUBJECT_TRUNK,
        _sv_linkEvent));

    SYS_ERR_CHK(surveillance_vlanState_set(DISABLED));

    osal_sem_mutex_destroy(g_svsem);

    return SYS_ERR_OK;
}

