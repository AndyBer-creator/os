#include <common/sys_def.h>
#include <osal/time.h>

#include <rsd/rsd_sys.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_l2.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_qos.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_vlan_voice.h>

#include <voice_vlan/proto_voice_vlan.h>
#include <lldp/proto_lldp.h>
#define VOICEVLAN_MBR_PIE_TIMER      (30)  
#define VOICEVLAN_RMK_ENTRY_NUM      (1)   
osal_mutex_t                         voicevlan_sem;

uint32                               packcnt = 0;
static osal_timer_list_t             oui_timer[CAPA_VLAN_VOICE_OUI_NUM][SYS_LOGICPORT_NUM_MAX];
static sys_enable_t                  voice_vlan_timer_update[SYS_LOGICPORT_NUM_MAX];
static volatile int32                voice_vlan_timer_thread_status = 0;
static voice_vlan_state_t            voiceVlanState = VOICE_VLAN_DISABLED;
static sys_enable_t                  voiceVlan1pRemarkEnable = DISABLED;
static uint32                        voiceVlanAging = 1440;
static sys_vlan_voiceQos_t           voiceVlanQos;
static sys_vlan_voiceVlan_oui_t      voiceVlanOui[CAPA_VLAN_VOICE_OUI_NUM];
static sys_logic_portmask_t          voiceVlanPortEnableMask;
static sys_logic_portmask_t          voiceVlanPortCosSrcMask;
static sys_logic_portmask_t          voiceVlanPortManualMask;
static sys_logic_portmask_t          voiceVlanLldpMemberMask;
static sys_logic_portmask_t          voiceVlanCdpMemberMask;
static sys_logic_portmask_t          voiceVlanPortCpuMask[CAPA_VLAN_VOICE_OUI_NUM];
static uint32                        voiceVlanPortAging[SYS_LOGICPORT_NUM_MAX];

#ifdef CONFIG_SYS_PROTO_LLDP_MED
static int32 voice_vlan_lldp_voipEvent(uint32 event, char *pData);
#endif
static int32 voice_vlan_idChgEvent(uint32 event, char *pData);
static void  voice_vlan_timer_thread(void *pInput);
static void  voice_vlan_oui_hold_timer_expired(unsigned long data);
static int32 voice_vlan_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
static int32 voice_vlan_qosEvent(uint32 event, char *pData);
static int32 voice_vlan_linkEvent(uint32 event, char *pData);
static int32 voice_vlan_trunkEvent(uint32 event, char *pData);
static int32 voice_vlan_oui_index_get(sys_vlan_voiceVlan_oui_t Oui,uint32 *pIndex);
static int32 voice_vlan_holdtime_update(void);
static int32 _voice_vlan_mbrEntry_set(uint32 entryIdx,sys_enable_t enable);
static int32 _voice_vlan_mbrEntry_update(sys_enable_t enable);
static int32 _voice_vlan_cosSrcEntry_set(uint32 entryIdx, sys_enable_t enable);
static int32 _voice_vlan_cosAllEntry_set(sys_enable_t enable);
static int32 _voice_vlan_cosEntry_update(sys_enable_t enable);
static rsd_nic_handler_t l2gOUIHandler = {
    .priority       = RSD_NIC_HANDLER_PRI_VOICE_VLAN_OUI,
    .dmac           = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .dmac_caremask  = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 },
    .smac           = { 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0x0 },
    .smac_caremask  = { 0xFF, 0xFF, 0xFF, 0x0, 0x0, 0x0 },
    .ethertype      = 0x0,
    .proto          = 0,
    .sport          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_COPY,
    .rx_cbf         = voice_vlan_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_TRUNK,
    .pName          = "Voice VLAN RX Thread",
    .pCookie        = NULL,
};
#define VOICEVLAN_SEM_LOCK()    \
do{                             \
    if (SYS_ERR_OK != osal_sem_mutex_take(voicevlan_sem, OSAL_SEM_WAIT_FOREVER)){    \
        VV_DBG("VLAN voice Lock Error");     \
        return SYS_ERR_SEM_LOCK;}          \
}while(0)

#define VOICEVLAN_SEM_UNLOCK()    \
do{                     \
    if (SYS_ERR_OK != osal_sem_mutex_give(voicevlan_sem)){    \
        VV_DBG("VLAN voice unLock Error");     \
        return SYS_ERR_SEM_UNLOCK;}          \
}while(0)
static int32 voice_vlan_oui_index_get(sys_vlan_voiceVlan_oui_t Oui,uint32 *pIndex)
{
    uint32 entryIndex;
    uint32 foundIdx = 0xff;
    sys_vlan_voiceVlan_oui_t ouiData;

    FOR_EACH_VOICE_OUI_ENTRY(entryIndex)
    {
        osal_memset(&ouiData, 0, sizeof(sys_vlan_voiceVlan_oui_t));
        SYS_ERR_CHK(voice_vlan_oui_get(entryIndex, &ouiData));

        if((ENABLED == ouiData.enable) &&
            (Oui.oui[0] == ouiData.oui[0]) &&
             (Oui.oui[1] == ouiData.oui[1]) &&
              (Oui.oui[2] == ouiData.oui[2]))
        {
             foundIdx = entryIndex;
             break;
        }
    }
    if(0xff == foundIdx)
        return SYS_ERR_FAILED;
    else
        *pIndex = foundIdx;

    return SYS_ERR_OK;

}

#ifdef CONFIG_SYS_PROTO_LLDP_MED
static int32 voice_vlan_lldp_voipEvent(uint32 event, char *pData)
{
    voice_vlan_state_t state;
    sys_vid_t voiceVid;
    sys_vlan_portMode_t portMode;
    sys_lldp_voice_vlan_event_t *pVoipEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);
    pVoipEvent = (sys_lldp_voice_vlan_event_t *)pData;

    SYS_ERR_CHK(voice_vlan_state_get(&state));
    SYS_ERR_CHK(rsd_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(rsd_vlan_portAdminMode_get(pVoipEvent->port,&portMode));

    if(SYS_NT_LLDP_VOICE_VLAN_PORT_ADD == pVoipEvent->event)
    {
        VV_DBG("Voice member Port %s Add Event Happened By LLDP", LPSTR(pVoipEvent->port));

        if(VOICE_VLAN_AUTO_ENABLED != state)
            return SYS_ERR_OK;
        VOICEVLAN_SEM_LOCK();
        LOGIC_PORTMASK_SET_PORT(voiceVlanLldpMemberMask,pVoipEvent->port);
        VOICEVLAN_SEM_UNLOCK();

        if((SYS_VLAN_PORT_MODE_TRUNK != portMode) && (SYS_VLAN_PORT_MODE_GENERAL != portMode))
        {
            VV_DBG("By LLDP Trigger,Voice VLAN Add Port %s to Voice VLAN Failed with port "
                "not in General or Trunk Mode", LPSTR(pVoipEvent->port));
            return SYS_ERR_OK;
        }
        SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_set(pVoipEvent->port,ENABLED));
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_VOICE_MBR_LLDP_ADD,
            LPSTR(pVoipEvent->port), voiceVid);
        VV_DBG("Port %s is Add to voice VLAN %u By LLDP Trigger", LPSTR(pVoipEvent->port), voiceVid);
    }

    if(SYS_NT_LLDP_VOICE_VLAN_PORT_DEL == pVoipEvent->event)
    {
        VV_DBG("Voice member Port %s Del Event Happened By LLDP", LPSTR(pVoipEvent->port));

        VOICEVLAN_SEM_LOCK();
        LOGIC_PORTMASK_CLEAR_PORT(voiceVlanLldpMemberMask,pVoipEvent->port);
        VOICEVLAN_SEM_UNLOCK();

        if(IS_LOGIC_PORTMASK_PORTSET(voiceVlanCdpMemberMask, pVoipEvent->port))
            return SYS_ERR_OK;
        if(VOICE_VLAN_AUTO_ENABLED != state)
            return SYS_ERR_OK;
        SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_set(pVoipEvent->port,DISABLED));

        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_VOICE_MBR_LLDP_DEL,
            LPSTR(pVoipEvent->port), voiceVid);
        VV_DBG("Port %s is Del from voice VLAN %u By LLDP Trigger", LPSTR(pVoipEvent->port), voiceVid);
    }

    return SYS_ERR_OK;
}
#endif

static int32 voice_vlan_idChgEvent(uint32 event, char *pData)
{
    voice_vlan_state_t state;
    sys_vlan_event_t *pVidEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);
    pVidEvent = (sys_vlan_event_t *)pData;

    SYS_ERR_CHK(voice_vlan_state_get(&state));
    if(VOICE_VLAN_DISABLED == state)
        return SYS_ERR_OK;

    if(SYS_NT_VLAN_VOICE_VID_CHG == event)
    {
        VV_DBG("voice vlan id change to %u",pVidEvent->vid);

        if(VOICE_VLAN_OUI_ENABLED == state)
        {
            SYS_ERR_CHK(_voice_vlan_cosEntry_update(ENABLED));
            SYS_ERR_CHK(_voice_vlan_mbrEntry_update(ENABLED));
        }
    }

    return SYS_ERR_OK;
}

static int32 voice_vlan_qosEvent(uint32 event, char *pData)
{
    voice_vlan_state_t state;
    sys_qos_event_t *pQosEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);
    pQosEvent = (sys_qos_event_t *)pData;

    SYS_ERR_CHK(voice_vlan_state_get(&state));
    if(VOICE_VLAN_OUI_ENABLED != state)
        return SYS_ERR_OK;

    if (SYS_NT_QOS_COS2QUEUE_CHG == event)
    {
        VV_DBG("Qos event cos %u to queue %u",pQosEvent->cos,pQosEvent->qid);

        if(pQosEvent->cos == voiceVlanQos.pri)
            SYS_ERR_CHK(_voice_vlan_cosEntry_update(ENABLED));
    }

    return SYS_ERR_OK;
}

static int32 voice_vlan_linkEvent(uint32 event, char *pData)
{
    uint32 entryIdx;
    voice_vlan_state_t state;
    sys_vid_t voiceVid;
    sys_enable_t enable;
    sys_port_event_t *pPortEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pPortEvent = (sys_port_event_t *)pData;
    SYS_ERR_CHK(voice_vlan_state_get(&state));
    SYS_ERR_CHK(rsd_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_get(pPortEvent->lport,&enable));

    if(VOICE_VLAN_OUI_ENABLED != state)
        return SYS_ERR_OK;

    if((ENABLED == enable)&&(PORT_LINKDOWN == pPortEvent->new_linkStatus))
    {
        SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_set(pPortEvent->lport,DISABLED));
        VOICEVLAN_SEM_LOCK();
        voiceVlanPortAging[pPortEvent->lport] = 0;
        
        FOR_EACH_VOICE_OUI_ENTRY(entryIdx)
        {
            osal_timer_stop(&oui_timer[entryIdx][pPortEvent->lport]);
            LOGIC_PORTMASK_SET_PORT(voiceVlanPortCpuMask[entryIdx],pPortEvent->lport);
        }
        VOICEVLAN_SEM_UNLOCK();
        SYS_ERR_CHK(_voice_vlan_mbrEntry_update(ENABLED));

        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_VOICE_MBR_DEL, LPSTR(pPortEvent->lport), voiceVid);
        VV_DBG("Port %s is Del from voice VLAN with LinkDown",LPSTR(pPortEvent->lport));
    }

    return SYS_ERR_OK;
}

static int32 voice_vlan_trunkEvent(uint32 event, char *pData)
{
    sys_trunk_event_t *pTrunkEvent;
    voice_vlan_state_t state;
    sys_logic_portmask_t carePm;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pTrunkEvent = (sys_trunk_event_t *)pData;
    SYS_ERR_CHK(voice_vlan_state_get(&state));

    if (VOICE_VLAN_OUI_ENABLED != state)
        return SYS_ERR_OK;

    LOGIC_PORTMASK_COPY(carePm,pTrunkEvent->portmask);
    LOGIC_PORTMASK_SET_PORT(carePm,TRK2LP(pTrunkEvent->trunkIdx));
    LOGIC_PORTMASK_AND(carePm,carePm,voiceVlanPortEnableMask);

    if(!IS_LOGIC_PORTMASK_CLEAR(carePm))
    {
        SYS_ERR_CHK(_voice_vlan_cosEntry_update(ENABLED));
        SYS_ERR_CHK(_voice_vlan_mbrEntry_update(ENABLED));
    }

    return SYS_ERR_OK;
}
static int32 voice_vlan_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_logic_port_t slp;
    sys_enable_t portEnable = DISABLED;
    sys_vid_t voiceVid = DFLT_VLAN_DFLT_VLAN;
    sys_vid_t vid = DFLT_VLAN_DFLT_VLAN;
    voice_vlan_state_t state;
    voice_vlan_port_mode_t portMode;
    sys_vlan_voiceVlan_oui_t Oui;
    uint32 entryIndex;
    uint8 macStr[CAPA_MAC_STR_LEN];

    VV_DBG("rx packet count:%u",packcnt++);
    slp = pPkt->rx_tag.source_port;

    SYS_MEM_CLEAR(Oui);
    SYS_MEM_CLEAR(macStr);
    SYS_ERR_CHK(sys_util_mac2Str(pPkt->data + 6, macStr));
    SYS_ERR_CHK(rsd_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(voice_vlan_state_get(&state));
    SYS_ERR_CHK(voice_vlan_port_enable_get(slp, &portEnable));
    SYS_ERR_CHK(voice_vlan_port_mode_get(slp, &portMode));
    SYS_ERR_CHK(rsd_nic_pktVid_get(pPkt, &vid));

    if (VOICE_VLAN_OUI_ENABLED != state)
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_VOICE_GLOBAL_DROP, LPSTR(slp), macStr);
        goto out;
    }

    if ((ENABLED != portEnable) && (VOICE_VLAN_MODE_AUTO != portMode))
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_VOICE_INTF_DROP, LPSTR(slp), macStr);
        goto out;
    }

    if (vid != voiceVid)
        goto out;

    if((IS_TRKLP(slp)) &&(IS_TRKLP_EMPTY(slp)))
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_VOICE_INTF_DROP, LPSTR(slp), macStr);
        goto out;
    }

    osal_memcpy(&(Oui.oui),pPkt->data + 6, SNAPOUI_LEN);

    if(SYS_ERR_OK != (voice_vlan_oui_index_get(Oui,&entryIndex)))
    {
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_VOICE_OUI_DROP, LPSTR(slp), macStr);
        goto out;
    }

    if (voiceVlanPortAging[slp] == 0)
    {
        SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_set(slp,ENABLED));
        SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_VOICE_MBR_ADD, LPSTR(slp), voiceVid, macStr);
        VV_DBG("Port %s is added to voice VLAN",LPSTR(slp));
    }

    voice_vlan_timer_update[slp] = DISABLED;
    LOGIC_PORTMASK_CLEAR_PORT(voiceVlanPortCpuMask[entryIndex],slp);
    SYS_ERR_CHK(_voice_vlan_mbrEntry_set(entryIndex,ENABLED));
    SYS_ERR_CHK(osal_timer_stop(&oui_timer[entryIndex][slp]));
    SYS_ERR_CHK(osal_timer_start(&oui_timer[entryIndex][slp],1000*60*VOICEVLAN_MBR_PIE_TIMER));

    VOICEVLAN_SEM_LOCK();
    voiceVlanPortAging[slp] = voiceVlanAging*60;
    VOICEVLAN_SEM_UNLOCK();

    goto out;

out:
    if (SYS_ERR_OK != rsd_nic_pkt_free(pPkt))
        VV_DBG("Voice VLAN pkt free falied!");

    return SYS_ERR_OK;
}

static int32 _voice_vlan_acl_entryInfo_dump(sys_acl_entry_t *entry)
{
    ACLDBG("  idx: %d", entry->key.idx);

    return 0;
}

static int32 _voice_vlan_acl_entryPrio_cmp(sys_acl_entry_t *entry, sys_acl_entry_t *cmp)
{
    SYS_PARAM_CHK((NULL == entry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == cmp), SYS_ERR_NULL_POINTER);

    if (entry->key.idx < cmp->key.idx)
        return 1;

    return 0;
}

static int32 _voice_vlan_acl_entry2Str_get(sys_acl_entry_t *entry, char *buf, uint32 maxlen)
{
    uint32 ouiIdx = 0;
    sys_vlan_voiceVlan_oui_t *oui = NULL;

    SYS_PARAM_CHK((NULL == entry), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == buf), SYS_ERR_NULL_POINTER);

    if (entry->key.idx < (2 * CAPA_VLAN_VOICE_OUI_NUM))
    {
        ouiIdx = entry->key.idx / 2;
        oui = &voiceVlanOui[ouiIdx];

        if (ACL_PROTOTYPE_VOICE_VL == entry->type)
        {
            if (0 == entry->key.idx % 2)
                osal_sprintf(buf, "Copy member ACL for OuiIdx %u %02x:%02x:%02x", ouiIdx,
                    oui->oui[0], oui->oui[1], oui->oui[2]);
            else
                osal_sprintf(buf, "Cos Source and remark ACL for OuiIdx %u %02x:%02x:%02x", ouiIdx,
                    oui->oui[0], oui->oui[1], oui->oui[2]);
        }
 #ifdef CONFIG_SYS_RTL8390
        else if (ACL_PROTOTYPE_VOICE_VL_RMK == entry->type)
        {
            osal_sprintf(buf, "90 cos remark ACL for OuiIdx %u %02x:%02x:%02x", ouiIdx,
                oui->oui[0], oui->oui[1], oui->oui[2]);
        }
#endif
    }
    else
    {
        if (ACL_PROTOTYPE_VOICE_VL == entry->type)
        {
            osal_sprintf(buf, "Cos All and remark ACL");
        }
 #ifdef CONFIG_SYS_RTL8390
        else if (ACL_PROTOTYPE_VOICE_VL_RMK == entry->type)
        {
            osal_sprintf(buf, "90 cos All remark ACL");
        }
#endif
    }

    return SYS_ERR_OK;
}
int32 voice_vlan_init(void)
{
    sys_logic_port_t logicPortId;
    uint32 entryIdx;
    sys_acl_cb_t cb_info;

    voicevlan_sem = osal_sem_mutex_create();

    LOGIC_PORTMASK_CLEAR_ALL(voiceVlanPortEnableMask);
    LOGIC_PORTMASK_CLEAR_ALL(voiceVlanPortManualMask);
    LOGIC_PORTMASK_CLEAR_ALL(voiceVlanPortCosSrcMask);
    LOGIC_PORTMASK_CLEAR_ALL(voiceVlanLldpMemberMask);
    LOGIC_PORTMASK_CLEAR_ALL(voiceVlanCdpMemberMask);

    FOR_EACH_LOGIC_PORT(logicPortId)
    {
        voiceVlanPortAging[logicPortId] = 0;
        voice_vlan_timer_update[logicPortId] = DISABLED;
    }

    osal_memset(voiceVlanOui, 0, sizeof(sys_vlan_voiceVlan_oui_t)*CAPA_VLAN_VOICE_OUI_NUM);

    FOR_EACH_VOICE_OUI_ENTRY(entryIdx)
    {
        LOGIC_PORTMASK_CLEAR_ALL(voiceVlanPortCpuMask[entryIdx]);
        FOR_EACH_LOGIC_PORT(logicPortId)
        {
            SYS_ERR_CHK(osal_timer_init(&oui_timer[entryIdx][logicPortId],
                voice_vlan_oui_hold_timer_expired,entryIdx*SYS_LOGICPORT_NUM_MAX +logicPortId));
        }
    }

#ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_ERR_CHK(sys_notifier_observer_register("voce vlan", SYS_NOTIFIER_SUBJECT_LLDP,
        (SYS_NT_LLDP_VOICE_VLAN_PORT_ADD | SYS_NT_LLDP_VOICE_VLAN_PORT_DEL),
        voice_vlan_lldp_voipEvent, SYS_NOTIFIER_PRI_MEDIUM));
#endif
    SYS_ERR_CHK(sys_notifier_observer_register("voce vlan", SYS_NOTIFIER_SUBJECT_VLAN, SYS_NT_VLAN_VOICE_VID_CHG,
        voice_vlan_idChgEvent, SYS_NOTIFIER_PRI_MEDIUM));

    SYS_ERR_CHK(sys_notifier_observer_register("voce vlan", SYS_NOTIFIER_SUBJECT_QOS, SYS_NT_QOS_COS2QUEUE_CHG,
        voice_vlan_qosEvent, SYS_NOTIFIER_PRI_MEDIUM));

    SYS_ERR_CHK(sys_notifier_observer_register("voce vlan", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG,
        voice_vlan_linkEvent, SYS_NOTIFIER_PRI_MEDIUM));

    SYS_ERR_CHK(sys_notifier_observer_register("voce vlan", SYS_NOTIFIER_SUBJECT_TRUNK,(SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE),
        voice_vlan_trunkEvent, SYS_NOTIFIER_PRI_MEDIUM));

    if ((osal_thread_t)NULL == (osal_thread_create("Voice VLAN Timer Thread", 32768,
        DFLT_THREAD_PRI_VOICEVLAN_TIMER, (void *)voice_vlan_timer_thread, NULL)))
    {
        VV_DBG("voice vlan timer thread create failed");
        return SYS_ERR_FAILED;
    }

    SYS_MEM_CLEAR(cb_info);
    cb_info.entry_info_dump = _voice_vlan_acl_entryInfo_dump;
    cb_info.entry_str_get = _voice_vlan_acl_entry2Str_get;
    cb_info.entry_prio_cmp = _voice_vlan_acl_entryPrio_cmp;
    SYS_ERR_CHK(rsd_acl_cb_register(ACL_PROTOTYPE_VOICE_VL, &cb_info));

#ifdef CONFIG_SYS_RTL8390
    if (IS_CHIP_CYPRESS())
        SYS_ERR_CHK(rsd_acl_cb_register(ACL_PROTOTYPE_VOICE_VL_RMK, &cb_info));
#endif

    return SYS_ERR_OK;
}
int32 voice_vlan_exit(void)
{
    
    voice_vlan_timer_thread_status = 1;
    do { } while (voice_vlan_timer_thread_status >= 0);

#ifdef CONFIG_SYS_PROTO_LLDP_MED
    SYS_ERR_CHK(sys_notifier_observer_unregister("voice vlan", SYS_NOTIFIER_SUBJECT_LLDP,voice_vlan_lldp_voipEvent));
#endif
    SYS_ERR_CHK(sys_notifier_observer_unregister("voice vlan", SYS_NOTIFIER_SUBJECT_VLAN,voice_vlan_idChgEvent));

    SYS_ERR_CHK(sys_notifier_observer_unregister("voice vlan", SYS_NOTIFIER_SUBJECT_QOS,voice_vlan_qosEvent));

    SYS_ERR_CHK(sys_notifier_observer_unregister("voice vlan", SYS_NOTIFIER_SUBJECT_PORT,voice_vlan_linkEvent));

    SYS_ERR_CHK(voice_vlan_state_set(VOICE_VLAN_DISABLED));

    osal_sem_mutex_destroy(voicevlan_sem);

    return SYS_ERR_OK;
} 
int32 voice_vlan_state_set(voice_vlan_state_t state)
{
    int32 entryIndex;
    sys_vid_t defaultVid;
    voice_vlan_state_t orgState;
    sys_vlan_voiceVlan_oui_t ouiData;
    sys_vid_t voiceVid;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t autoMbr;

    SYS_PARAM_CHK(state >= VOICE_VLAN_STATE_END, SYS_ERR_OUT_OF_RANGE);

    if (state == voiceVlanState)
        return SYS_ERR_OK;

    VOICEVLAN_SEM_LOCK();
    orgState = voiceVlanState;
    voiceVlanState = state;
    VOICEVLAN_SEM_UNLOCK();

    SYS_ERR_CHK(rsd_vlan_defaultVid_get(&defaultVid));
    SYS_ERR_CHK(rsd_vlan_voiceVid_get(&voiceVid));
    if (VOICE_VLAN_OUI_ENABLED == state)
    {
        
        FOR_EACH_VOICE_OUI_ENTRY(entryIndex)
        {
            osal_memset(&ouiData, 0, sizeof(sys_vlan_voiceVlan_oui_t));
            SYS_ERR_CHK(voice_vlan_oui_get(entryIndex, &ouiData));
            
            if (DISABLED == ouiData.enable)
                continue;
            
            osal_memcpy(l2gOUIHandler.smac_caremask, ouiData.ouiMask, SNAPOUI_LEN);
            osal_memcpy(l2gOUIHandler.smac, ouiData.oui, SNAPOUI_LEN);
            osal_sprintf(l2gOUIHandler.pName, "Voice VLAN RX Thread %u", entryIndex);
            SYS_ERR_CHK(rsd_nic_handler_register(&l2gOUIHandler));
        }
    }
    else if( VOICE_VLAN_AUTO_ENABLED == state)
    {
        if(defaultVid != voiceVid)
        {
            LOGIC_PORTMASK_OR(autoMbr,voiceVlanLldpMemberMask,voiceVlanCdpMemberMask);
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort,autoMbr)
            {
                SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_set(logicPort,ENABLED));
            }
        }
    }
    else
    {
        if(VOICE_VLAN_AUTO_ENABLED == orgState)
        {
            LOGIC_PORTMASK_OR(autoMbr,voiceVlanLldpMemberMask,voiceVlanCdpMemberMask);
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort,autoMbr)
            {
                SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_set(logicPort,DISABLED));
            }
        }
        if(VOICE_VLAN_OUI_ENABLED == orgState)
        {
            SYS_ERR_CHK(_voice_vlan_mbrEntry_update(DISABLED));
            SYS_ERR_CHK(_voice_vlan_cosEntry_update(DISABLED));

            FOR_EACH_VOICE_OUI_ENTRY(entryIndex)
            {
                FOR_EACH_LOGIC_PORT(logicPort)
                    SYS_ERR_CHK(osal_timer_stop(&oui_timer[entryIndex][logicPort]));

                osal_memset(&ouiData, 0, sizeof(sys_vlan_voiceVlan_oui_t));
                SYS_ERR_CHK(voice_vlan_oui_get(entryIndex, &ouiData));
                
                if (DISABLED == ouiData.enable)
                    continue;

                osal_memcpy(l2gOUIHandler.smac_caremask, ouiData.ouiMask, SNAPOUI_LEN);
                osal_memcpy(l2gOUIHandler.smac, ouiData.oui, SNAPOUI_LEN);
                osal_sprintf(l2gOUIHandler.pName, "Voice VLAN RX Thread %u", entryIndex);
                rsd_nic_handler_unregister(&l2gOUIHandler);
            }
        }
    }

    return SYS_ERR_OK;
} 
int32 voice_vlan_state_get(voice_vlan_state_t *pState)
{
    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    VOICEVLAN_SEM_LOCK();
    *pState = voiceVlanState;
    VOICEVLAN_SEM_UNLOCK();

    return SYS_ERR_OK;
} 
int32 voice_vlan_aging_set(uint32 aging)
{
    sys_logic_port_t logicPortId;

    if (voiceVlanAging == aging)
        return SYS_ERR_OK;

    VOICEVLAN_SEM_LOCK();
    FOR_EACH_LOGIC_PORT(logicPortId)
    {
        if (voiceVlanPortAging[logicPortId] > 0)
        {
            voiceVlanPortAging[logicPortId] = aging*60;
        }
    }
    voiceVlanAging = aging;
    VOICEVLAN_SEM_UNLOCK();

    return SYS_ERR_OK;
} 
int32 voice_vlan_aging_get(uint32 *pAging)
{
    SYS_PARAM_CHK((NULL == pAging), SYS_ERR_NULL_POINTER);

    VOICEVLAN_SEM_LOCK();
    *pAging = voiceVlanAging;
    VOICEVLAN_SEM_UNLOCK();

    return SYS_ERR_OK;
} 
int32
voice_vlan_oui_get(uint32 entryIdx, sys_vlan_voiceVlan_oui_t *pOui)
{
    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);

    VOICEVLAN_SEM_LOCK();
    osal_memcpy(pOui, &voiceVlanOui[entryIdx], sizeof(sys_vlan_voiceVlan_oui_t));
    VOICEVLAN_SEM_UNLOCK();

    return SYS_ERR_OK;
} 
int32 voice_vlan_oui_set(uint32 entryIdx, sys_vlan_voiceVlan_oui_t *pOui)
{
    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);

    VOICEVLAN_SEM_LOCK();
    osal_memcpy(&voiceVlanOui[entryIdx], pOui, sizeof(sys_vlan_voiceVlan_oui_t));
    VOICEVLAN_SEM_UNLOCK();

    if ((VOICE_VLAN_OUI_ENABLED == voiceVlanState)&&(ENABLED == pOui->enable))
    {
        
        osal_memcpy(l2gOUIHandler.smac_caremask, pOui->ouiMask, SNAPOUI_LEN);
        osal_memcpy(l2gOUIHandler.smac, pOui->oui, SNAPOUI_LEN);
        osal_sprintf(l2gOUIHandler.pName, "Voice VLAN RX Thread %u", entryIdx);
        SYS_ERR_CHK(rsd_nic_handler_register(&l2gOUIHandler));

        SYS_ERR_CHK(_voice_vlan_mbrEntry_set(entryIdx,ENABLED));
        SYS_ERR_CHK(_voice_vlan_cosSrcEntry_set(entryIdx,ENABLED));
    }
    else if(DISABLED == pOui->enable)
    {
        
        SYS_ERR_CHK(_voice_vlan_mbrEntry_set(entryIdx,DISABLED));
        SYS_ERR_CHK(_voice_vlan_cosSrcEntry_set(entryIdx,DISABLED));

        osal_memcpy(l2gOUIHandler.smac_caremask, voiceVlanOui[entryIdx].ouiMask, SNAPOUI_LEN);
        osal_memcpy(l2gOUIHandler.smac, voiceVlanOui[entryIdx].oui, SNAPOUI_LEN);
        osal_sprintf(l2gOUIHandler.pName, "Voice VLAN RX Thread %u", entryIdx);
        rsd_nic_handler_unregister(&l2gOUIHandler);
     }

    return SYS_ERR_OK;
} 
int32 voice_vlan_port_enable_set(sys_logic_port_t portId, sys_enable_t enable)
{
    uint32 entryIdx;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    if (IS_LOGIC_PORTMASK_PORTSET(voiceVlanPortEnableMask, portId) &&
        (ENABLED == enable))
        return SYS_ERR_OK;

    if (!IS_LOGIC_PORTMASK_PORTSET(voiceVlanPortEnableMask, portId) &&
        (DISABLED == enable))
        return SYS_ERR_OK;

    if (ENABLED == enable)
    {
        VOICEVLAN_SEM_LOCK();
        LOGIC_PORTMASK_SET_PORT(voiceVlanPortEnableMask, portId);
        if(!IS_LOGIC_PORTMASK_PORTSET(voiceVlanPortManualMask,portId))
        {
            FOR_EACH_VOICE_OUI_ENTRY(entryIdx)
            {
                LOGIC_PORTMASK_SET_PORT(voiceVlanPortCpuMask[entryIdx],portId);
            }
        }
        VOICEVLAN_SEM_UNLOCK();
    }
    else
    {
        VOICEVLAN_SEM_LOCK();
        LOGIC_PORTMASK_CLEAR_PORT(voiceVlanPortEnableMask, portId);
        
        FOR_EACH_VOICE_OUI_ENTRY(entryIdx)
        {
            osal_timer_stop(&oui_timer[entryIdx][portId]);
            LOGIC_PORTMASK_CLEAR_PORT(voiceVlanPortCpuMask[entryIdx],portId);
        }
        VOICEVLAN_SEM_UNLOCK();
        if ((voiceVlanPortAging[portId] > 0))
        {
            voiceVlanPortAging[portId] = 0;
            SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_set(portId,DISABLED));
        }
    }
    
    if (VOICE_VLAN_OUI_ENABLED == voiceVlanState)
    {
        SYS_ERR_CHK(_voice_vlan_cosEntry_update(ENABLED));
        SYS_ERR_CHK(_voice_vlan_mbrEntry_update(ENABLED));
    }

    return SYS_ERR_OK;
} 
int32 voice_vlan_port_enable_get(sys_logic_port_t portId, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    VOICEVLAN_SEM_LOCK();
    if (IS_LOGIC_PORTMASK_PORTSET(voiceVlanPortEnableMask, portId))
        *pEnable = ENABLED;
    else
        *pEnable = DISABLED;
    VOICEVLAN_SEM_UNLOCK();

    return SYS_ERR_OK;
} 
int32 voice_vlan_port_mode_set(sys_logic_port_t portId, voice_vlan_port_mode_t mode)
{
    uint32 entryIdx;
    voice_vlan_state_t state;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((mode >= VOICE_VLAN_MODE_END), SYS_ERR_INPUT);

    voice_vlan_state_get(&state);
    if (VOICE_VLAN_MODE_MANUAL == mode)
    {
        VOICEVLAN_SEM_LOCK();
        LOGIC_PORTMASK_SET_PORT(voiceVlanPortManualMask, portId);
        
        FOR_EACH_VOICE_OUI_ENTRY(entryIdx)
        {
            osal_timer_stop(&oui_timer[entryIdx][portId]);
            LOGIC_PORTMASK_CLEAR_PORT(voiceVlanPortCpuMask[entryIdx],portId);
        }
        VOICEVLAN_SEM_UNLOCK();

        if (voiceVlanPortAging[portId] > 0)
        {
            SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_set(portId,DISABLED));
            voiceVlanPortAging[portId] = 0;
        }
    }
    else
    {
        VOICEVLAN_SEM_LOCK();
        LOGIC_PORTMASK_CLEAR_PORT(voiceVlanPortManualMask, portId);
        if(IS_LOGIC_PORTMASK_PORTSET(voiceVlanPortEnableMask, portId))
        {
            FOR_EACH_VOICE_OUI_ENTRY(entryIdx)
            {
                LOGIC_PORTMASK_SET_PORT(voiceVlanPortCpuMask[entryIdx],portId);
            }
        }
        VOICEVLAN_SEM_UNLOCK();
    }

    if(VOICE_VLAN_OUI_ENABLED == state)
    {
        SYS_ERR_CHK(_voice_vlan_cosEntry_update(ENABLED));
        SYS_ERR_CHK(_voice_vlan_mbrEntry_update(ENABLED));
    }
    return SYS_ERR_OK;
} 
int32 voice_vlan_port_mode_get(sys_logic_port_t portId, voice_vlan_port_mode_t *pMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    VOICEVLAN_SEM_LOCK();
    if (IS_LOGIC_PORTMASK_PORTSET(voiceVlanPortManualMask, portId))
        *pMode = VOICE_VLAN_MODE_MANUAL;
    else
        *pMode = VOICE_VLAN_MODE_AUTO;
    VOICEVLAN_SEM_UNLOCK();

    return SYS_ERR_OK;
} 
int32 voice_vlan_cos_mode_set(sys_logic_port_t portId,voice_vlan_cos_mode_t cosMode)
{

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((cosMode >= VOICE_VLAN_COS_END), SYS_ERR_INPUT);

    VOICEVLAN_SEM_LOCK();
    if (VOICE_VLAN_COS_SRC == cosMode)
        LOGIC_PORTMASK_SET_PORT(voiceVlanPortCosSrcMask, portId);
    else
        LOGIC_PORTMASK_CLEAR_PORT(voiceVlanPortCosSrcMask, portId);
    VOICEVLAN_SEM_UNLOCK();

    if(VOICE_VLAN_OUI_ENABLED == voiceVlanState)
        SYS_ERR_CHK(_voice_vlan_cosEntry_update(ENABLED));

    return SYS_ERR_OK;
}

int32 voice_vlan_cos_mode_get(sys_logic_port_t portId,voice_vlan_cos_mode_t *pCosMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCosMode), SYS_ERR_NULL_POINTER);

    VOICEVLAN_SEM_LOCK();
    if (IS_LOGIC_PORTMASK_PORTSET(voiceVlanPortCosSrcMask, portId))
        *pCosMode = VOICE_VLAN_COS_SRC;
    else
        *pCosMode = VOICE_VLAN_COS_ALL;
    VOICEVLAN_SEM_UNLOCK();

    return SYS_ERR_OK;
}

int32 voice_vlan_1pRemark_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    VOICEVLAN_SEM_LOCK();
    *pEnable = voiceVlan1pRemarkEnable;
    VOICEVLAN_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 voice_vlan_1pRemark_enable_set(sys_enable_t enable)
{
    voice_vlan_state_t state;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    VOICEVLAN_SEM_LOCK();
    voiceVlan1pRemarkEnable = enable;
    VOICEVLAN_SEM_UNLOCK();

    SYS_ERR_CHK(voice_vlan_state_get(&state));

    if(VOICE_VLAN_OUI_ENABLED == state)
        SYS_ERR_CHK(_voice_vlan_cosEntry_update(ENABLED));

    return SYS_ERR_OK;
}
int32 voice_vlan_qos_get(sys_vlan_voiceQos_t *pVoiceQos)
{
    SYS_PARAM_CHK((NULL == pVoiceQos), SYS_ERR_NULL_POINTER);

    VOICEVLAN_SEM_LOCK();
    osal_memcpy(pVoiceQos, &voiceVlanQos, sizeof(sys_vlan_voiceQos_t));
    VOICEVLAN_SEM_UNLOCK();

    return SYS_ERR_OK;
}
int32 voice_vlan_qos_set(sys_vlan_voiceQos_t voiceQos)
{
    voice_vlan_state_t state;
    sys_vlan_event_t event;
    sys_vlan_voiceQos_t orgvoiceQos;

    SYS_PARAM_CHK(IS_PRI_INVALID(voiceQos.pri), SYS_ERR_PRIORITY);
    SYS_PARAM_CHK(IS_PRI_INVALID(voiceQos.vpt), SYS_ERR_PRIORITY);
    SYS_PARAM_CHK(IS_DSCP_INVALID(voiceQos.dscp), SYS_ERR_DSCP_ID);

    SYS_ERR_CHK(voice_vlan_state_get(&state));
    SYS_ERR_CHK(voice_vlan_qos_get(&orgvoiceQos));

    VOICEVLAN_SEM_LOCK();
    osal_memcpy(&voiceVlanQos,&voiceQos, sizeof(sys_vlan_voiceQos_t));
    VOICEVLAN_SEM_UNLOCK();

    if((VOICE_VLAN_OUI_ENABLED == state)&&(voiceQos.pri != orgvoiceQos.pri))
    {
        SYS_ERR_CHK(_voice_vlan_cosEntry_update(ENABLED));
    }

    if((voiceQos.vpt != orgvoiceQos.vpt)||(voiceQos.dscp != orgvoiceQos.dscp))
    {
        osal_memcpy(&event.voiceQos,&voiceQos, sizeof(sys_vlan_voiceQos_t));
        SYS_ERR_CHK(sys_notifier_event_dispatcher(SYS_NOTIFIER_SUBJECT_VLAN,
            SYS_NT_VLAN_VOICE_QOS_CHG, (char*)&event));
    }

    return SYS_ERR_OK;
}
static int32
_vlan_voice_chip_mbrEntry_set(uint32 entryIdx,sys_vlan_voice_entry_t voiceEntry)
{
    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(rsd_vlan_voice_chip_mbrEntry_set(entryIdx, &voiceEntry));

    VV_DBG("update voice vlan member For entry %u", entryIdx);

    return SYS_ERR_OK;
}

static int32 _vlan_voice_chip_mbrEntry_del(uint32 entryIdx)
{
    SYS_ERR_CHK(rsd_vlan_voice_entry_del(entryIdx));

    return SYS_ERR_OK;
}
static int32
_vlan_voice_chip_cosSrcEntry_set(uint32 entryIdx,sys_vlan_voice_entry_t voiceEntry)
{
    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(rsd_vlan_voice_chip_cosSrcEntry_set(entryIdx, &voiceEntry));

    VV_DBG("update cos mode src entry voice id %u", voiceEntry.voiceVid);

    return SYS_ERR_OK;
}

static int32 _vlan_voice_chip_cosSrcEntry_del(uint32 entryIdx)
{

    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(rsd_vlan_voice_chip_cosSrcEntry_del((entryIdx)));

    return SYS_ERR_OK;
}

static int32 _vlan_voice_chip_cosAllEntry_set(sys_vlan_voice_entry_t voiceEntry)
{
    SYS_ERR_CHK(rsd_vlan_voice_chip_cosAllEntry_set(&voiceEntry));

    VV_DBG("update cos mode all entry voice id %u", voiceEntry.voiceVid);

    return SYS_ERR_OK;
}

static int32 _vlan_voice_chip_cosAllEntry_del(void)
{
    SYS_ERR_CHK(rsd_vlan_voice_chip_cosAllEntry_del());

    return SYS_ERR_OK;
}

static int32 _voice_vlan_mbrEntry_set(uint32 entryIdx,sys_enable_t enable)
{
    sys_vid_t voiceVid;
    sys_logic_port_t    lp;
    sys_logic_portmask_t copyCpuSpm;
    sys_vlan_voiceVlan_oui_t ouiData;
    sys_vlan_voice_entry_t voiceEntry;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(rsd_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(voice_vlan_oui_get(entryIdx, &ouiData));

    if(DISABLED == enable)
    {
        _vlan_voice_chip_mbrEntry_del(entryIdx);
        return SYS_ERR_OK;

    }

    LOGIC_PORTMASK_COPY(copyCpuSpm,voiceVlanPortCpuMask[entryIdx]);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, copyCpuSpm)
    {
        if(IS_LP_TRKMBR(lp))
            LOGIC_PORTMASK_CLEAR_PORT(copyCpuSpm, lp);
    }

    if(IS_LOGIC_PORTMASK_CLEAR(copyCpuSpm))
        _vlan_voice_chip_mbrEntry_del(entryIdx);
    else
    {
        voiceEntry.voiceVid = voiceVid;
        LOGIC_PORTMASK_COPY(voiceEntry.copyCpuSpm,copyCpuSpm);
        osal_memcpy(voiceEntry.ouiMask,ouiData.ouiMask,SNAPOUI_LEN);
        osal_memcpy( voiceEntry.oui,ouiData.oui,SNAPOUI_LEN);

        _vlan_voice_chip_mbrEntry_set(entryIdx,voiceEntry);
    }

    return SYS_ERR_OK;
}

static int32 _voice_vlan_mbrEntry_update(sys_enable_t enable)
{
    uint32 entryIdx;
    sys_vlan_voiceVlan_oui_t ouiData;

    FOR_EACH_VOICE_OUI_ENTRY(entryIdx)
    {
        osal_memset(&ouiData, 0, sizeof(sys_vlan_voiceVlan_oui_t));
        SYS_ERR_CHK(voice_vlan_oui_get(entryIdx, &ouiData));

        if(DISABLED == ouiData.enable)
            continue;
        _voice_vlan_mbrEntry_set(entryIdx,enable);
    }

    return SYS_ERR_OK;
}

static int32 _voice_vlan_cosSrcEntry_set(uint32 entryIdx,sys_enable_t enable)
{
    sys_qid_t qid;
    sys_vid_t voiceVid;
    sys_logic_port_t lp;
    sys_logic_portmask_t cosSrcPm;
    sys_vlan_voiceQos_t voiceQos;
    sys_vlan_voiceVlan_oui_t ouiData;
    sys_vlan_voice_entry_t voiceEntry;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);

    SYS_MEM_CLEAR(voiceEntry);
    SYS_ERR_CHK(rsd_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(voice_vlan_oui_get(entryIdx, &ouiData));
    SYS_ERR_CHK(voice_vlan_qos_get(&voiceQos));
    SYS_ERR_CHK(rsd_qos_cosQueue_get(voiceQos.pri,&qid));

    if(DISABLED == enable)
    {
        _vlan_voice_chip_cosSrcEntry_del(entryIdx);
        return SYS_ERR_OK;
    }

    LOGIC_PORTMASK_AND(cosSrcPm,voiceVlanPortEnableMask,voiceVlanPortCosSrcMask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, cosSrcPm)
    {
        if(IS_LP_TRKMBR(lp))
            LOGIC_PORTMASK_CLEAR_PORT(cosSrcPm, lp);
    }

    if(IS_LOGIC_PORTMASK_CLEAR(cosSrcPm))
        _vlan_voice_chip_cosSrcEntry_del(entryIdx);
    else
    {
        voiceEntry.voiceVid = voiceVid;
        voiceEntry.qid = qid;
        voiceEntry.remark = voiceVlan1pRemarkEnable;
        voiceEntry.pri = voiceQos.pri;
        LOGIC_PORTMASK_COPY(voiceEntry.cosSrcPm,cosSrcPm);
        osal_memcpy(voiceEntry.ouiMask,ouiData.ouiMask,SNAPOUI_LEN);
        osal_memcpy(voiceEntry.oui,ouiData.oui,SNAPOUI_LEN);

        _vlan_voice_chip_cosSrcEntry_set(entryIdx,voiceEntry);
    }

    return SYS_ERR_OK;

}

static int32 _voice_vlan_cosAllEntry_set(sys_enable_t enable)
{
    sys_qid_t qid;
    sys_vid_t voiceVid;
    sys_logic_port_t lp;
    sys_logic_portmask_t cosAllPm;
    sys_vlan_voiceQos_t  voiceQos;
    sys_vlan_voice_entry_t voiceEntry;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    SYS_MEM_CLEAR(voiceEntry);
    SYS_ERR_CHK(rsd_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(voice_vlan_qos_get(&voiceQos));
    SYS_ERR_CHK(rsd_qos_cosQueue_get(voiceQos.pri,&qid));

    if(DISABLED == enable)
    {
        _vlan_voice_chip_cosAllEntry_del();
        return SYS_ERR_OK;
    }

    LOGIC_PORTMASK_ANDNOT(cosAllPm,voiceVlanPortEnableMask,voiceVlanPortCosSrcMask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, cosAllPm)
    {
        if(IS_LP_TRKMBR(lp))
            LOGIC_PORTMASK_CLEAR_PORT(cosAllPm, lp);
    }

    if(IS_LOGIC_PORTMASK_CLEAR(cosAllPm))
        _vlan_voice_chip_cosAllEntry_del();
    else
    {

        voiceEntry.voiceVid = voiceVid;
        voiceEntry.qid = qid;
        voiceEntry.remark = voiceVlan1pRemarkEnable;
        voiceEntry.pri = voiceQos.pri;
        LOGIC_PORTMASK_COPY(voiceEntry.cosAllPm,cosAllPm);

        _vlan_voice_chip_cosAllEntry_set(voiceEntry);
    }

    return SYS_ERR_OK;

}

static int32 _voice_vlan_cosEntry_update(sys_enable_t enable)
{
    uint32 entryIdx;
    sys_vlan_voiceVlan_oui_t ouiData;

    FOR_EACH_VOICE_OUI_ENTRY(entryIdx)
    {
        osal_memset(&ouiData, 0, sizeof(sys_vlan_voiceVlan_oui_t));
        SYS_ERR_CHK(voice_vlan_oui_get(entryIdx, &ouiData));

        if(DISABLED == ouiData.enable)
            continue;
        _voice_vlan_cosSrcEntry_set(entryIdx,enable);
    }

    _voice_vlan_cosAllEntry_set(enable);

    return SYS_ERR_OK;
}
static void voice_vlan_timer_thread(void *pInput)
{
    
    for (;;)
    {
        osal_time_usleep(1 * 1000 * 1000); 
        voice_vlan_holdtime_update();
        if (voice_vlan_timer_thread_status)
            break;
    }
    voice_vlan_timer_thread_status = -1;

    osal_thread_exit(0);
} 
static int32 voice_vlan_holdtime_update(void)
{
    sys_vid_t voiceVid;
    sys_logic_port_t lp;
    sys_enable_t portEnable;
    voice_vlan_state_t state;
    voice_vlan_port_mode_t portMode;

    SYS_ERR_CHK(voice_vlan_state_get(&state));
    SYS_ERR_CHK(rsd_vlan_voiceVid_get(&voiceVid));

    if (VOICE_VLAN_OUI_ENABLED == state)
    {
        FOR_EACH_LOGIC_PORT(lp)
        {
            SYS_ERR_CHK(voice_vlan_port_enable_get(lp, &portEnable));
            if (ENABLED != portEnable)
                continue;

            SYS_ERR_CHK(voice_vlan_port_mode_get(lp, &portMode));
            if (portMode != VOICE_VLAN_MODE_AUTO)
                continue;

            if(DISABLED == voice_vlan_timer_update[lp])
                continue;

            if (voiceVlanPortAging[lp] > 0)
            {
                voiceVlanPortAging[lp]--;
                if (voiceVlanPortAging[lp] == 0)
                {
                    SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_set(lp,DISABLED));
                    SYS_LOG(LOG_CAT_VLAN, LOG_VLAN_VOICE_MBR_DEL, LPSTR(lp), voiceVid);
                    VV_DBG("Port %s is delete from voice vlan %u",LPSTR(lp),voiceVid);
                }
            }
        }
    }

    return SYS_ERR_OK;

} 

static void voice_vlan_oui_hold_timer_expired(unsigned long data)
{
    uint32 entryIndex;
    sys_logic_port_t logicPortId;
    char port_str[16];

    entryIndex = data/SYS_LOGICPORT_NUM_MAX;
    logicPortId = data % SYS_LOGICPORT_NUM_MAX;
    LP2STR(logicPortId, port_str);

    VV_DBG("member update entry:%u For Add Port %s",entryIndex,port_str);

    voice_vlan_timer_update[logicPortId] = ENABLED;
    LOGIC_PORTMASK_SET_PORT(voiceVlanPortCpuMask[entryIndex],logicPortId);
    _voice_vlan_mbrEntry_set(entryIndex,ENABLED);
}

