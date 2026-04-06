#define _erps_task_c_

#include <linux/jiffies.h>
#include <linux/random.h>

#include <common/sys_portview.h>
#include <common/sys_def.h>
#include <osal/memory.h>
#include <osal/print.h>
#include <osal/sem.h>
#include <osal/lib.h>
#include <osal/thread.h>
#include <osal/time.h>

#include <rsd/rsd_nic.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_qos.h>
#include <rsd/rsd_erps.h>
#include <rsd/rsd_sys.h>
#include <rsd/rsd_trap.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_l2.h>
#include <rsd/rsd_erps.h>
#include <erps/proto_sys_erps.h>
#include <erps/proto_erps.h>

erps_bridge_info_t erps_bridge;

#define ERPS_SEM_LOCK(sem)   \
    do { } while (osal_sem_mutex_take(sem, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)
#define ERPS_SEM_UNLOCK(sem) \
    do { } while (osal_sem_mutex_give(sem) != SYS_ERR_OK)
#define SAL_MAC_LEARNING_RESET_DEFAULT 0x3FFF

static osal_mutex_t erpsMuxSem;
static sys_logic_portmask_t erps_pmLinkup;
static volatile int32 erps_tick_thread_status = 0;
unsigned int g_erps_ring_port_state[SYS_NORMALPORT_NUM_MAX+1] = {0};

uint8 erps_stdMac[] = {0x01, 0x19, 0xa7, 0x00, 0x00, 0x00};

static int32 erps_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
void erps_tx_raps_sf_timer_stop(erps_inst_t *inst);
void erps_tx_raps_sf_timer_start(erps_inst_t *inst);
void erps_tx_raps_sf_timer_expired(unsigned long instId);
void erps_set_each_inst_stp(void);

int32 sal_trap_set_rma_port_rma_lldp_learn_disable_enable(rtk_enable_t enable);
static rsd_nic_handler_t l2gHandler = {
	.priority		= RSD_NIC_HANDLER_PRI_ERPS,
	.dmac			= { 0x01, 0x19, 0xa7, 0x00, 0x00, 0x00 },
	.dmac_caremask	= { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00 },
	.ethertype		= ERPS_ETHER_TYPE,
	.proto			= 0,
	.sport			= 0,
	.dport			= 0,
	.action 		= RSD_NIC_HANDLER_ACT_TRAP,
	.rx_cbf 		= erps_pkt_rx,
	.rx_careMask	= RSD_NIC_RX_CARE_TRUNK,
	.pName			= "ERPS RX Thread",
	.pCookie		= NULL,
};
static int nodeIdCmp(uint8 *nodeId1, uint8 *nodeId2)
{

    if (nodeId1 == NULL || nodeId2 == NULL)
    {
        return 0;
    }
    return memcmp(nodeId1, nodeId2,  6*sizeof(uint8));
    #if 0
    if (cmp > 0)
    {
        return -1;
    }
    else if (cmp == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
    #endif
}

#if 0
void sal_port_limint_learning_reset_default(sys_logic_port_t portId)
{
    rtk_l2_macCnt_t limitCnt;
    rtk_l2_macCnt_t limitNum;
    rtk_l2_macLimitAction_t limitAct;
    sys_physic_port_t phyPort; 
    uint32  port;

    SYS_MEM_CLEAR(limitAct);
	SYS_MEM_CLEAR(limitNum);
	SYS_MEM_CLEAR(limitCnt);

	sys_logic_to_physic_port(portId, &phyPort);
	port = phyPort.port;
	
	limitCnt.portTrkCnt.id = port;
	limitNum.portTrkCnt.id = port;
	limitAct.portTrkAct.id = port;

#if defined (CONFIG_SDK_DRIVER_RTK_LEGACY_API)
    if (IS_BACKWARD_COMPATIBLE)
    {
        rtk_l2_portLearningCnt_get(0, port, &limitCnt.portTrkCnt.cnt);
    }
    else
#endif
    {
        rtk_l2_macLearningCnt_get(0, L2_MAC_LIMIT_PORT, &limitCnt);
    }
#if defined (CONFIG_SDK_DRIVER_RTK_LEGACY_API)
    if (IS_BACKWARD_COMPATIBLE)
    {
        rtk_l2_portLimitLearningCnt_get(0, port, &limitNum.portTrkCnt.cnt);
    }
    else
#endif
    {
        rtk_l2_limitLearningNum_get(0, L2_MAC_LIMIT_PORT, &limitNum);
    }
#if defined (CONFIG_SDK_DRIVER_RTK_LEGACY_API)
    if (IS_BACKWARD_COMPATIBLE)
    {
        rtk_l2_limitLearnCntAction_t act;
        rtk_l2_portLimitLearningCntAction_get(0, port, &act);
        limitAct.portTrkAct.act = (rtk_action_t)act;
    }
    else
#endif
    {
        rtk_l2_limitLearningAction_get(0, L2_MAC_LIMIT_PORT, &limitAct);
    }
    if (limitNum.portTrkCnt.cnt != L2_MAC_CST_DISABLE)
    {
    	sal_port_limit_learning_set(port, ACTION_FORWARD, SAL_MAC_LEARNING_RESET_DEFAULT);
    }
    if (limitAct.portTrkAct.act != ACTION_FORWARD)
    {
    	sal_port_limit_learning_set(port, ACTION_FORWARD, SAL_MAC_LEARNING_RESET_DEFAULT);
    }

}
void sal_port_limit_learning_set(rtk_port_t               port, rtk_action_t act, uint32  counnt)
{

	rtk_l2_macLimitAction_t limitAct;
	rtk_l2_macCnt_t limitNum;
	
	SYS_MEM_CLEAR(limitAct);
	SYS_MEM_CLEAR(limitNum);

	limitAct.portTrkAct.act = act;
	limitAct.portTrkAct.id = port;
	limitNum.portTrkCnt.cnt = counnt;
	limitNum.portTrkCnt.id = port;
#if defined (CONFIG_SDK_DRIVER_RTK_LEGACY_API)
	if (IS_BACKWARD_COMPATIBLE)
	{
		rtk_l2_portLimitLearningCnt_set(0, port, limitNum.portTrkCnt.cnt);
	}
	else
#endif
	{
		rtk_l2_limitLearningNum_set(0, L2_MAC_LIMIT_PORT, &limitNum);
	}
	
#if defined (CONFIG_SDK_DRIVER_RTK_LEGACY_API)
	if (IS_BACKWARD_COMPATIBLE)
	{
		rtk_l2_limitLearnCntAction_t act;

		act = (rtk_l2_limitLearnCntAction_t)limitAct.portTrkAct.act;
		rtk_l2_portLimitLearningCntAction_set(0, port, act);
	}
	else
#endif
	{
		rtk_l2_limitLearningAction_set(0, L2_MAC_LIMIT_PORT, &limitAct);
	}
	return;

}

#endif

#if 1
int32 sal_trap_set_rma_port_rma_lldp_learn_disable_enable(rtk_enable_t enable)
{
    rtk_trap_mgmtType_t type;
    
    type = MGMT_TYPE_LLDP;
    rtk_trap_mgmtFrameLearningEnable_set(SYS_DEV_ID_DEFAULT, type, enable);

    return SYS_ERR_OK;
}
#endif
static int32 erps_linkEvent(uint32 event, char *pData)
{
    sys_port_event_t *pPortEvent = NULL;
    erps_inst_t *inst   = NULL;
    erps_bool_t action = FALSE;
    erps_event_t tmpEvent;
    
    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pPortEvent = (sys_port_event_t *)pData;
    tmpEvent = pPortEvent->new_linkStatus ? ERPS_EVENT_CLEAR_SF : ERPS_EVENT_LOCAL_SF;

    SYS_DBG(DBG_ERPS, "link event port:%s up %u",  LPSTR(pPortEvent->lport), pPortEvent->new_linkStatus);
    if (PORT_LINKUP == pPortEvent->new_linkStatus)
    {
        LOGIC_PORTMASK_SET_PORT(erps_pmLinkup, pPortEvent->lport);
    }
    else
    {
        LOGIC_PORTMASK_CLEAR_PORT(erps_pmLinkup, pPortEvent->lport);
    }
	#if 0
    if (!IS_LP_TRKMBR(pPortEvent->lport))
    {
        ERPS_SEM_LOCK(erpsMuxSem);
        ERPS_SEM_UNLOCK(erpsMuxSem);
    }
    else
    {
        ERPS_SEM_LOCK(erpsMuxSem);
        ERPS_SEM_UNLOCK(erpsMuxSem);
    }
    #endif
    
    for (inst = erps_bridge.instInfo.next; NULL != inst; inst = inst->next)
    {
    	action = FALSE;
    	erps_get_port_enable(inst->ring.port0.portId, &inst->ring.port0.portEnable);
    	erps_get_port_enable(inst->ring.port1.portId, &inst->ring.port1.portEnable);
    	#if 1
    	sal_trap_set_rma_port_rma_lldp_learn_disable_enable(DISABLED);
    	#endif
        if (pPortEvent->lport == inst->ring.port0.portId)
        {
            action = TRUE;
        }
        else if (pPortEvent->lport == inst->ring.port1.portId) 
        {
            action = TRUE;
        }
        if(action && (erps_bridge.bErpsEnable) && (inst->ring.ringEnable))
        {
        	erps_event_stm(inst, &inst->ring, tmpEvent, pPortEvent->lport);
        	if(tmpEvent == ERPS_EVENT_LOCAL_SF)
        	{
        		erps_tx_raps_sf_timer_start(inst);
        	}
        	else if(tmpEvent == ERPS_EVENT_CLEAR_SF)
        	{
        		
        		if((inst->ring.port0.portEnable == TRUE) && (inst->ring.port1.portEnable == TRUE))
        		{
        			erps_tx_raps_sf_timer_stop(inst);
        		}
        	}
        }
        else if (action)
        {
            erps_set_port_state(inst, &inst->ring, pPortEvent->lport, pPortEvent->new_linkStatus ? ERPS_Forwarding : ERPS_Disabled);
        }
    }

    return SYS_ERR_OK;
}

static int32 erps_trunkEvent_update(uint32 event, char *pData)
{
    return SYS_ERR_OK;
}

static int32 erps_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    uint16 etherType = 0;
    int32 ret = SYS_ERR_OK;
    uint16 tagLen = 0;  
    sys_vid_t vid = 0;
    sys_logic_port_t port = pPkt->rx_tag.source_port;

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);
    if (!erps_bridge.bErpsEnable)
    {
        osal_printf("Free ERPS packet becasue ERPS is disabled");
        rsd_nic_pkt_free(pPkt);
        return SYS_ERR_OK;
	}

    if (TRUE == pPkt->rx_tag.cvid_tagged)
        tagLen = tagLen + 4;

    if (TRUE == pPkt->rx_tag.svid_tagged)
        tagLen = tagLen + 4;

    if (SYS_ERR_OK != rsd_nic_pktVid_get(pPkt, &vid))
    {
        osal_printf("Error: Could not retrieve VID from the frame received !!");
        rsd_nic_pkt_free(pPkt);
        return SYS_ERR_OK;
    }

    etherType = *(uint16*) (pPkt->data + 12 + tagLen);
    if (ERPS_ETHER_TYPE != etherType)
    {
        rsd_nic_pkt_free(pPkt);
        osal_printf("stp rx port %s packet ethertype is %x\n", LPSTR(port), etherType);

        return SYS_ERR_OK;
    }

    if (!IS_LOGIC_PORTMASK_PORTSET(erps_pmLinkup, port))
    {
        rsd_nic_pkt_free(pPkt);
        osal_printf("erpsrx port %s is not linkup\n", LPSTR(port));

        return SYS_ERR_OK;
    }

    ERPS_SEM_LOCK(erpsMuxSem);

    SYS_PKT_DUMP(DBG_ERPS, pPkt->data, pPkt->length);
    ret = erps_stm_rcv((erps_raps_pdu_t *)pPkt->data, port, vid);
    ERPS_SEM_UNLOCK(erpsMuxSem);

    rsd_nic_pkt_free(pPkt);

	return ret;
}

static int32 erps_pkt_tx(uint32 portId, uint32 vid, erps_raps_pdu_t *pData, int32 len)
{
    sys_nic_pkt_t *pPkt;
    sys_logic_portmask_t logic_portmask;

    if (!erps_bridge.bErpsEnable)
        return SYS_ERR_OK;

    LOGIC_PORTMASK_CLEAR_ALL(logic_portmask);

    if (!IS_LOGIC_PORTMASK_PORTSET(erps_pmLinkup, portId))
    {
        SYS_DBG(DBG_ERPS, "erps tx port %u is not linkup\n", LPSTR(portId));
        return SYS_ERR_OK;
    }
    LOGIC_PORTMASK_SET_PORT(logic_portmask, portId);
    if (SYS_ERR_OK == rsd_nic_pkt_alloc(len, &pPkt))
    {
        osal_memcpy(pPkt->data, pData, len);
        pPkt->tail += len;
        pPkt->length = len;
        SYS_ERR_CHK(rsd_nic_pkt_tx(pPkt, &logic_portmask, SYS_CPU_PORT_PRIORITY_BPDU, vid, RSD_NIC_TX_CARE_TRUNK | RSD_NIC_TX_CARE_DOT1X));
    }

    return SYS_ERR_OK;
}

static int32 erps_fdb_flush(uint32 instance, uint32 portId)
{
    int32 ret;

    ret = rsd_l2_mac_clearByPort(portId);
    SYS_DBG(DBG_ERPS, "instance: %d, port: %s, ret = %d", instance, LPSTR(portId), ret);

    return SYS_ERR_OK;
}

void erps_flush_mac(erps_inst_t *inst)
{
    int stg = 0;

    for (stg = 0; stg < CAPA_STP_MST_INSTANCE_NUM; stg++)
    {
        if (inst->bStgMap[stg] == TRUE)
    	{
    	    if (inst->ring.port0.portId != ERPS_RING_PORT_ID_DFT)
			{
			    erps_fdb_flush(stg, inst->ring.port0.portId);
			}
			if (inst->ring.port1.portId != ERPS_RING_PORT_ID_DFT)
			{
			    erps_fdb_flush(stg, inst->ring.port1.portId);
			}
    	}
	}

}

void erps_start_timer(void (*expiry_fn) (unsigned long), unsigned long instId,
	uint32 timeout, struct timer_list *tEntry)
{
    osal_timer_init(tEntry, expiry_fn, instId);
    
    osal_timer_start(tEntry, timeout);
}

erps_bool_t erps_stop_timer(struct timer_list *tEntry)
{
    if(tEntry->expires!=0)
    {
        osal_timer_stop(tEntry);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
int32 erps_init(void)
{
    sys_mac_t macAddr;
    sys_logic_port_t lp;
    
	FOR_EACH_LOGIC_PORT(lp)
    {
        g_erps_ring_port_state[lp] = ERPS_Forwarding;
    }

    erpsMuxSem = osal_sem_mutex_create();
    if (0 == erpsMuxSem)
    {
        osal_printf("%s():%d - semaphore create failed\n", __FUNCTION__, __LINE__);
        return SYS_ERR_FAILED;
    }

    SYS_ERR_CHK(rsd_nic_handler_register(&l2gHandler));

    memset(&erps_bridge, 0, sizeof(erps_bridge));
    SYS_ERR_CHK(rsd_sys_macAddr_get(0, &macAddr));
    osal_memcpy(erps_bridge.bridgeId, macAddr.octet, ETHER_ADDR_LEN);

    LOGIC_PORTMASK_CLEAR_ALL(erps_pmLinkup);

    SYS_DBG(DBG_ERPS, "erps port num %u", LOGIC_PORT_END);

    SYS_ERR_CHK(sys_notifier_observer_register("erps", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG, erps_linkEvent, SYS_NOTIFIER_PRI_HIGH));

    SYS_ERR_CHK(sys_notifier_observer_register("erps", SYS_NOTIFIER_SUBJECT_TRUNK,
            (SYS_NT_TRK_ACT_MBR_JOIN | SYS_NT_TRK_ACT_MBR_LEAVE | SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE),
            erps_trunkEvent_update, SYS_NOTIFIER_PRI_MEDIUM));

    return SYS_ERR_OK;
}
int32 erps_exit(void)
{

    erps_tick_thread_status = 1;
    do { } while (erps_tick_thread_status >= 0);

    SYS_ERR_CHK(rsd_nic_handler_unregister(&l2gHandler));

    SYS_ERR_CHK(sys_notifier_observer_unregister("erps", SYS_NOTIFIER_SUBJECT_PORT, erps_linkEvent));

    SYS_ERR_CHK(sys_notifier_observer_unregister("erps", SYS_NOTIFIER_SUBJECT_TRUNK, erps_trunkEvent_update));

    osal_sem_mutex_destroy(erpsMuxSem);

    return SYS_ERR_OK;
}
int32 erps_vlan_nameStr_get(sys_vid_t vid, char *pPrefix, sys_namestr_t *pName)
{
    SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
    SYS_PARAM_CHK((NULL == pPrefix), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    osal_sprintf(pName->str, "%s%04d", pPrefix, vid);

    return SYS_ERR_OK;
}
int erps_vlan_create(erps_inst_t *inst, int ringId)
{
    uint16 vlanId;
    sys_physic_port_t phyPort;
     uint16 instanceId = (uint16)(inst->instId & (CAPA_STP_MST_INSTANCE_NUM - 1));

    if ((NULL == inst) || (ERPS_INVALID_CTRLVLAN_ID == inst->ring.ctrlVlanId))
    {
        return SYS_ERR_FAILED;
    }
    
    vlanId = (uint16)(inst->ring.ctrlVlanId & 0xFFF);
	
	SYS_ERR_CHK(rtk_vlan_stg_set(SYS_DEV_ID_DEFAULT, vlanId, (sys_stg_t)instanceId));
	SYS_MEM_CLEAR(phyPort);
	SYS_ERR_CHK(sys_logic_to_physic_port(inst->ring.port0.portId, &phyPort));
   	SYS_ERR_CHK(rtk_stp_mstpState_set(phyPort.dev,  (sys_stg_t)instanceId,
					    phyPort.port,STP_STATE_FORWARDING));
	SYS_MEM_CLEAR(phyPort);
	SYS_ERR_CHK(sys_logic_to_physic_port(inst->ring.port1.portId, &phyPort));
	SYS_ERR_CHK(rtk_stp_mstpState_set(phyPort.dev,  (sys_stg_t)instanceId,
					    phyPort.port,STP_STATE_FORWARDING));
    return SYS_ERR_OK;
}

int erps_vlan_delete(erps_inst_t *inst,  erps_ring_t *ring)
{
    uint16 vlanId;

    if ((NULL == inst) || (NULL == ring) || (ERPS_INVALID_CTRLVLAN_ID == ring->ctrlVlanId))
    {
        return SYS_ERR_FAILED;
    }

    vlanId = (uint16)ring->ctrlVlanId;
    
	SYS_ERR_XLATE_CHK(rtk_vlan_stg_set(SYS_DEV_ID_DEFAULT, vlanId, 0));

    return SYS_ERR_OK;
}
int erps_vlan_port_add(erps_inst_t *inst, erps_ring_t *ring, int portId)
{  
    sys_physic_port_t phyPort;    
    uint16 vlanId;

    if ((NULL == inst) || (NULL == ring) || (ERPS_RING_PORT_ID_DFT == portId)
        || (ERPS_INVALID_CTRLVLAN_ID == ring->ctrlVlanId)
        || ((ring->port0.portId != portId) && (ring->port1.portId != portId)))
    {
        return SYS_ERR_FAILED;
    }
    
    SYS_ERR_CHK(sys_logic_to_physic_port(portId, &phyPort));
    vlanId = (uint16)ring->ctrlVlanId;
    SYS_ERR_CHK(rtk_vlan_port_add(phyPort.dev, vlanId, phyPort.port, 0));
	
	SYS_ERR_CHK(rtk_stp_mstpState_set(phyPort.dev, inst->instId, phyPort.port, STP_STATE_FORWARDING));

    return SYS_ERR_OK;
}
int erps_vlan_port_remove(erps_inst_t *inst, erps_ring_t *ring, int portId, erps_bool_t forceRemove)
{
    erps_inst_t *instTmp;
    sys_physic_port_t phyPort;
    uint16 vlanId;

    if ((NULL == inst) || (NULL == ring) || (ERPS_RING_PORT_ID_DFT == portId)
        || (ERPS_INVALID_CTRLVLAN_ID == ring->ctrlVlanId)
        || ((ring->port0.portId != portId) && (ring->port1.portId != portId)))
    {
        return SYS_ERR_FAILED;
    }

    if (!forceRemove)   
    {
        for (instTmp = erps_bridge.instInfo.next; NULL != instTmp; instTmp = instTmp->next)
        {
            
            if ((instTmp->instId != inst->instId)&& (instTmp->ring.ctrlVlanId == ring->ctrlVlanId)
                && ((instTmp->ring.port0.portId == portId) || (instTmp->ring.port1.portId == portId)))
            {
                return SYS_ERR_FAILED;
            }
        }
    }

    SYS_MEM_CLEAR(phyPort);
    SYS_ERR_CHK(sys_logic_to_physic_port(portId, &phyPort));
    vlanId = (uint16)ring->ctrlVlanId;
    SYS_ERR_XLATE_CHK(rtk_vlan_port_del(phyPort.dev, vlanId, phyPort.port));

    return SYS_ERR_OK;
}

int erps_inst_create(uint32 instId)
{
    int i;
    erps_inst_t *inst    = NULL;
    erps_inst_t *instTmp = NULL;

    inst = erps_find_inst(instId);
    if (NULL != inst)
    {
        return SYS_ERR_OK;
    }
    instTmp = (erps_inst_t *)osal_alloc(sizeof(erps_inst_t));
    if (NULL == instTmp)
    {
        return SYS_ERR_FAILED;
    }
    instTmp->instId = instId;
    instTmp->mel = 0;                                         
    
    instTmp->workMode      = ERPS_MODE_REVERTIVE;             
    instTmp->TxRAPSTime    = ERPS_DFT_TxRAPS_TIMEOUT;
    instTmp->WTRTime       = ERPS_DFT_WTR_TIMEOUT;
    instTmp->WTBTime       = ERPS_DFT_WTB_TIMEOUT;
    instTmp->guardTime     = ERPS_DFT_GUARD_TIMEOUT;
    instTmp->holdoffTime   = ERPS_DFT_HOLDOFF_TIMEOUT;
    for (i = 0; i < (CAPA_STP_MST_INSTANCE_NUM + 1); i++)
    {
        instTmp->bStgMap[i] = FALSE;
    }

    erps_ring_create(instTmp, ERPS_DFT_RING_ID);
    
    for (inst = &erps_bridge.instInfo; NULL != inst->next; inst = inst->next)
    {
        if (inst->next->instId > instId)
        {
            break;
        }
    }
    instTmp->next = inst->next;
    inst->next    = instTmp;
    return SYS_ERR_OK;
}

int erps_inst_delete(uint32 instId)
{
    erps_inst_t *inst    = NULL;
    erps_inst_t *instTmp = NULL;
    int stg = 0;

    inst = &erps_bridge.instInfo;
    for (instTmp = erps_bridge.instInfo.next; NULL != instTmp; instTmp = instTmp->next)
    {
        if (instTmp->instId == instId)
        {
            break;
        }
        inst = instTmp;
    }

    if (NULL == instTmp)
    {
        return SYS_ERR_OK;
    }

    if (ERPS_INVALID_CTRLVLAN_ID != instTmp->ring.ctrlVlanId)
    {
        erps_vlan_delete(instTmp, &instTmp->ring);
    }
    rsd_erps_port_set(instTmp->ring.port0.portId,0);
    rsd_erps_port_set(instTmp->ring.port1.portId,0);
	for (stg = 0; stg < CAPA_STP_MST_INSTANCE_NUM; stg++)
    {
    	if(instTmp->bStgMap[stg] == TRUE)
    	{
			
    		rsd_erps_ints_cfg_notice(stg, instTmp->ring.port0.portId, SAL_PORT_STATE_VALUE_ZERO);
    		rsd_erps_ints_cfg_notice(stg, instTmp->ring.port1.portId, SAL_PORT_STATE_VALUE_ZERO);
		}
    }
    inst->next    = instTmp->next;

	erps_tx_raps_sf_timer_stop(instTmp);
	erps_wtr_timer_stop(instTmp);
	erps_tx_raps_timer_stop(instTmp);

    erps_ring_inactive(instTmp,instTmp->instId);
    erps_ring_destroy(instTmp,instTmp->ring.ringId);
    
    osal_free(instTmp);    
    instTmp = NULL;
    erps_set_each_inst_stp();
    return SYS_ERR_OK;
}

int erps_ring_create(erps_inst_t *inst, int ringId)
{
    erps_ring_t *ringNew  = NULL;

    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }
    ringNew = (erps_ring_t *)osal_alloc(sizeof(erps_ring_t));
    if (NULL == ringNew)
    {
        return SYS_ERR_FAILED;
    }
	osal_memset(ringNew, 0x0, sizeof(erps_ring_t));
    
    ringNew->ringId             = ringId;
    ringNew->ringLevel          = 0;                        
    ringNew->ctrlVlanId         = ERPS_INVALID_CTRLVLAN_ID;
    ringNew->ringEnable         = FALSE;
    ringNew->role               = ERPS_RPL_Node;
    ringNew->port0.portId = ERPS_RING_PORT_ID_DFT;
    ringNew->port1.portId = ERPS_RING_PORT_ID_DFT;

    ringNew->stmStatus          = ERPS_STATUS_INIT;
    ringNew->sendTimes         = 0;
    ringNew->port0.BPR          = 0;
    memset(ringNew->port0.nodeId, 0, sizeof(6));
    ringNew->port0.sendBPR      = 0;
    ringNew->port1.BPR          = 0;
    memset(ringNew->port1.nodeId, 0, sizeof(6));
    ringNew->port1.sendBPR      = 1;
    ringNew->rxDNF              = FALSE;
    ringNew->sendDNF            = FALSE;
    memcpy(&inst->ring, ringNew, sizeof(erps_ring_t));
    osal_free(ringNew);
    ringNew=NULL;
    return SYS_ERR_OK;
}

int erps_ring_active(erps_inst_t *inst, int ringId)
{
    int rc = SYS_ERR_FAILED;

    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }

    if((inst->ring.role == ERPS_RPL_NONE)
        ||((inst->ring.ringLevel == 0) && ((inst->ring.port0.portId == ERPS_RING_PORT_ID_DFT) || (inst->ring.port1.portId == ERPS_RING_PORT_ID_DFT)))
        ||((inst->ring.ringLevel == 1) && ((inst->ring.port0.portId == ERPS_RING_PORT_ID_DFT) && (inst->ring.port1.portId == ERPS_RING_PORT_ID_DFT)))
        ||(inst->ring.ctrlVlanId == ERPS_INVALID_CTRLVLAN_ID))
    {
        return SYS_ERR_FAILED;
    }

    erps_get_port_enable(inst->ring.port0.portId, &inst->ring.port0.portEnable);
    erps_get_port_enable(inst->ring.port1.portId, &inst->ring.port1.portEnable);
    inst->ring.ringEnable = TRUE;
	inst->ring.stmStatus = ERPS_STATUS_INIT;

    if (erps_bridge.bErpsEnable)
    {    
        rc = erps_event_stm(inst, &inst->ring, ERPS_EVENT_NONE, inst->ring.port0.portId);
        
    }
    else
    {
        rc = SYS_ERR_OK;
    }

    return rc;
}
int erps_ring_inactive(erps_inst_t *inst, int ringId)
{
    erps_stm_state_t preNodeState;

    if (inst == NULL)
    {
        return SYS_ERR_FAILED;
    }
    if (inst->ring.port0.portEnable)
    {
        erps_set_port_state(inst, &inst->ring, inst->ring.port0.portId, ERPS_Forwarding);
    }
    if (inst->ring.port1.portEnable)
    {
        erps_set_port_state(inst, &inst->ring, inst->ring.port1.portId, ERPS_Forwarding);
    }

    preNodeState = inst->ring.stmStatus;
    inst->ring.stmStatus = ERPS_STATUS_INIT;
    if(inst->ring.ringEnable)
    {
    	inst->ring.ringEnable = FALSE;
    }
    return SYS_ERR_OK;
}
int erps_ring_destroy(erps_inst_t *inst, int ringId)
{
    if (NULL == inst)
    {
        return SYS_ERR_OK;
    }

    if (inst->ring.port0.portEnable)
    {
        erps_set_port_state(inst, &inst->ring, inst->ring.port0.portId, ERPS_Forwarding);
    }
    if (inst->ring.port1.portEnable)
    {
        erps_set_port_state(inst, &inst->ring, inst->ring.port1.portId, ERPS_Forwarding);
    }

    erps_vlan_port_remove(inst, &inst->ring, inst->ring.port0.portId, FALSE);
    erps_vlan_port_remove(inst, &inst->ring, inst->ring.port1.portId, FALSE);

    memset(&inst->ring, 0, sizeof(erps_ring_t));

    return SYS_ERR_OK;
}
int erps_msgDeal_pktRcv(erps_raps_pdu_t* pkt, uint32 portId, uint32 ulVlanId)
{
    int rc;

    rc = erps_stm_rcv(pkt, portId, ulVlanId);

    return rc;
}
void erps_msgDeal_portEnable(int portId, erps_bool_t enable)
{
    erps_inst_t *inst = NULL;
    int event = 0;
    erps_bool_t action = FALSE;

    event = enable ? ERPS_EVENT_CLEAR_SF : ERPS_EVENT_LOCAL_SF;

    for (inst = erps_bridge.instInfo.next; NULL != inst; inst = inst->next)
    {
        action = FALSE;
        if ((portId == inst->ring.port0.portId) && (inst->ring.port0.portEnable != enable))
        {
            inst->ring.port0.portEnable = enable;
            action = TRUE;
        }
        else if ((portId == inst->ring.port1.portId) && (inst->ring.port1.portEnable != enable))
        {
            inst->ring.port1.portEnable = enable;
            action = TRUE;
        }

        if (action && (erps_bridge.bErpsEnable) && (inst->ring.ringEnable))
        {
            
            if (event == ERPS_EVENT_CLEAR_SF)
            {
            	#if 0
                extern erps_bool_t cfm_check_port_cc_defect(uint8 portId, uint16 vlanId, uint8 levelId);
                if (cfm_check_port_cc_defect((uint8)portId, inst->ring.ctrlVlanId, (uint8)inst->mel))
                {
                    erps_set_port_state(inst, &inst->ring, portId, ERPS_Blocking);
                    return;
                }
				#endif
            }
            erps_event_stm(inst, &inst->ring, event, portId);
        }
        else if (action)
        {
            erps_set_port_state(inst, &inst->ring, portId, enable ? ERPS_Forwarding : ERPS_Disabled);
        }
    }
}

void erps_msgDeal_portLinkEnable(int portId, erps_bool_t enable)
{
    erps_inst_t *inst = NULL;
    int event = 0;
    erps_bool_t action = FALSE;

    event = enable ?  ERPS_EVENT_LOCAL_SF : ERPS_EVENT_CLEAR_SF;

    for (inst = erps_bridge.instInfo.next; NULL != inst; inst = inst->next)
    {
        action = FALSE;
        if (portId == inst->ring.port0.portId)
        {
            action = TRUE;
        }
        else if (portId == inst->ring.port1.portId)
        {
            action = TRUE;
        }

        if (action && (erps_bridge.bErpsEnable) && (inst->ring.ringEnable))
        {
            erps_event_stm(inst, &inst->ring, event, portId);
        }
    }
}
int erps_get_inst_state_status(uint32 instId, int32 *state)
{
	erps_inst_t *inst = NULL;
	int rc = SYS_ERR_OK;

	inst = erps_find_inst(instId);
	if (NULL == inst)
	{
		return SYS_ERR_FAILED;
	}
	*state = inst->ring.stmStatus;
	return rc;
}
int erps_enable_set(uint32 ulEnable)
{
    int rc = SYS_ERR_OK;
    erps_inst_t *inst;
    if (ulEnable == erps_bridge.bErpsEnable)
    {
        return SYS_ERR_OK;
    }
	rtk_stp_mstpInstance_create(SYS_DEV_ID_DEFAULT, ERPS_MST_INST_ID);
    erps_bridge.bErpsEnable = ulEnable;
	rsd_erps_enable_set(ulEnable);

    for (inst = erps_bridge.instInfo.next; NULL != inst; inst = inst->next)
    {
        if (!inst->ring.ringEnable)
        {
            continue;
        }

        if (FALSE == ulEnable) 
        {
            if (inst->ring.port0.portEnable)
            {
                erps_set_port_state(inst, &inst->ring, inst->ring.port0.portId, ERPS_Forwarding);
            }
            if (inst->ring.port1.portEnable)
            {
                erps_set_port_state(inst, &inst->ring, inst->ring.port1.portId, ERPS_Forwarding);
            }
            inst->ring.stmStatus = ERPS_STATUS_INIT;
        }
        else if (inst->ring.ringEnable)
        {
            rc = erps_ring_active(inst, inst->ring.ringId);
        }
    }

    return rc;
}

int erps_timer_set(int code, uint32 instId, uint32 value)
{
    int rc = SYS_ERR_OK;
    erps_inst_t *inst = NULL;

    inst = erps_find_inst(instId);
    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }

    switch (code)
    {
        case ERPS_WTB_TIME:
            inst->WTBTime = value;
            break;
        case ERPS_WTR_TIME:
            inst->WTRTime = value;          
            break;
        case ERPS_GUARD_TIME:
            inst->guardTime = value;
            break;
        case ERPS_HOLDOFF_TIME:
            inst->holdoffTime = value;
            break;
        default:
            rc = SYS_ERR_FAILED;
            break;
    }

    return rc;
}

int erps_ringStatus(erps_inst_t *inst, int status)
{
    int rc = SYS_ERR_FAILED;

    switch (status)
    {
        case Erps_Ring_Create:
            rc = erps_ring_create(inst,inst->ring.ringId);
            break;
        case Erps_Ring_Active:
            rc = erps_ring_active(inst, inst->ring.ringId);
            break;
        case Erps_Ring_Destroy:
            rc = erps_ring_destroy(inst, inst->ring.ringId);
            break;
        case Erps_Ring_Inactive:
            rc = erps_ring_inactive(inst, inst->ring.ringId);
        default:
            break;
    }
    return rc;
}

int32 erps_ringState_set(uint32 instId, sys_enable_t state)
{
    erps_inst_t *inst = NULL;
    int rc = SYS_ERR_OK;

    inst = erps_find_inst(instId);
    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }

	if (state == TRUE)
	{
		rc = erps_ringStatus(inst, Erps_Ring_Active);
	}
	else
	{
		rc = erps_ringStatus(inst, Erps_Ring_Inactive);
	}

	return rc;
}

int erps_ringPort_set(uint32 instId, int portId, int portRole, uint32 isPort0, uint32 isPortAdd)
{
    erps_inst_t *inst = NULL;
    erps_port_t *ringPort = NULL;
    erps_port_t *partnPort = NULL;
    int rc = SYS_ERR_OK;
    int stg = 0;
    int port1;
    int port0;
    int j = 0;

    inst = erps_find_inst(instId);
    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }

    if (isPort0)
    {
        ringPort  = &inst->ring.port0;
        partnPort = &inst->ring.port1;
        port0 = ringPort->portId;
    	port1 = partnPort->portId;
    }
    else
    {
        ringPort  = &inst->ring.port1;
        partnPort = &inst->ring.port0;
        port0 = ringPort->portId;
    	port1 = partnPort->portId;
    }
    if (partnPort->portId == portId)
    {
        return SYS_ERR_FAILED;
    }

    if(isPortAdd)
    {
    	for(j = 0; j < CAPA_STP_MST_INSTANCE_NUM; j++)
    	{
    		if(inst->bStgMap[j] == TRUE)
    		{
    			rsd_erps_ints_cfg_notice(j, ringPort->portId, SAL_PORT_STATE_VALUE_ZERO);
    		}
    	}
    }
    if (isPortAdd)
    {
        
        if ((partnPort->portRole != ERPS_RPL_PORT) && (portRole != ERPS_RPL_PORT))
        {
            return SYS_ERR_FAILED;
        }

        if ((ringPort->portId == portId) && (ringPort->portRole == portRole))
        {
            return SYS_ERR_OK;
        }

        if (portId != ringPort->portId)
        {
            
            if ((ringPort->portEnable))
            {
                erps_set_port_state(inst, &inst->ring, ringPort->portId, ERPS_Forwarding);
            }
            
            if ( (ringPort->portId != partnPort->portId))
            {
                erps_vlan_port_remove(inst, &inst->ring, ringPort->portId, FALSE);
            }
            
            erps_get_port_enable(portId, &ringPort->portEnable);
            ringPort->portId = portId;
            rsd_erps_port_set(ringPort->portId,1);

            erps_vlan_port_add(inst, &inst->ring, ringPort->portId);
            erps_get_port_enable(inst->ring.port0.portId, &inst->ring.port0.portEnable);
    		erps_get_port_enable(inst->ring.port1.portId, &inst->ring.port1.portEnable);
            if (!(erps_bridge.bErpsEnable) || !(inst->ring.ringEnable))
            {
                erps_set_port_state(inst, &inst->ring, portId, ringPort->portEnable ? ERPS_Forwarding : ERPS_Disabled);
            }
            
        }

        if (portRole != ringPort->portRole)
        {
            ringPort->portRole = portRole;
            
            if ((ringPort->portRole == ERPS_RPL_Owner_PORT) || (partnPort->portRole == ERPS_RPL_Owner_PORT))
            {
               inst->ring.role = ERPS_RPL_Owner_Node;
            }
            else if ((ringPort->portRole == ERPS_RPL_Neighbour_PORT) || (partnPort->portRole == ERPS_RPL_Neighbour_PORT))
            {
               inst->ring.role = ERPS_RPL_Neighbour_Node;
            }
            else if ((ringPort->portRole == ERPS_RPL_Next_Neighbour_PORT) || (partnPort->portRole == ERPS_RPL_Next_Neighbour_PORT))
            {
               inst->ring.role = ERPS_RPL_Next_Neighbour_Node;
            }
            else
            {
               inst->ring.role = ERPS_RPL_Node;
            }
        }
    }
    else 
    {
        
        if ( (ringPort->portEnable))
        {
            erps_set_port_state(inst, &inst->ring, ringPort->portId, ERPS_Forwarding);
        }
        
        if ( (ringPort->portId != partnPort->portId))
        {
            erps_vlan_port_remove(inst, &inst->ring, ringPort->portId, FALSE);
        }
        rsd_erps_port_set(ringPort->portId,0);
       
	    {
	    	for (stg = 0; stg < CAPA_STP_MST_INSTANCE_NUM; stg++)
		    {
		    	if(inst->bStgMap[stg] == TRUE)
		    	{
					
		    		rsd_erps_ints_cfg_notice(stg, ringPort->portId, SAL_PORT_STATE_VALUE_ZERO);
		    		
				}
		    }
	    }
        ringPort->portId = ERPS_RING_PORT_ID_DFT;
        erps_set_each_inst_stp();
        if ((ringPort->portRole == ERPS_RPL_Owner_PORT)
            || (ringPort->portRole == ERPS_RPL_Neighbour_PORT)
            || (ringPort->portRole == ERPS_RPL_Next_Neighbour_PORT))
        {
            inst->ring.role = ERPS_RPL_Node;
        }
        ringPort->portRole = ERPS_RPL_PORT;
    }

    ringPort->BPR    = 0;
    memset(ringPort->nodeId, 0, sizeof(6));
    
    erps_clear_port(ringPort);
    
    if (inst->ring.ringEnable)
    {
        rc = erps_ring_active(inst, inst->ring.ringId);
    }
    if(isPortAdd)
    {
		for (stg = 0; stg < CAPA_STP_MST_INSTANCE_NUM; stg++)
	    {
	    	if(inst->bStgMap[stg] == TRUE)
	    	{
				
	    		rsd_erps_ints_cfg_notice(stg, portId, SAL_PORT_STATE_VALUE_ONE);
			}
	    }
    }
    return rc;
}

int erps_ringId_set(uint32 instId, uint32 ringId)
{
    erps_inst_t *inst = NULL;

    inst = erps_find_inst(instId);
    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }

    inst->ring.ringId = ringId;

    return SYS_ERR_OK;
}

int erps_ctrlVlan_set(uint32 instId, uint32 vlanId)
{
    erps_inst_t *inst = NULL;

    inst = erps_find_inst(instId);
    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }

    if (ERPS_INVALID_CTRLVLAN_ID != inst->ring.ctrlVlanId)
    {

        erps_vlan_port_remove(inst, &inst->ring, inst->ring.port0.portId, TRUE);
        erps_vlan_port_remove(inst, &inst->ring, inst->ring.port1.portId, TRUE);

        erps_vlan_delete(inst, &inst->ring);
    }
    inst->ring.ctrlVlanId = vlanId;
    if (ERPS_INVALID_CTRLVLAN_ID != inst->ring.ctrlVlanId)
    {
        erps_vlan_create(inst, 0);

        erps_vlan_port_add(inst, &inst->ring, inst->ring.port0.portId);
        erps_vlan_port_add(inst, &inst->ring, inst->ring.port1.portId);
    }

    if (inst->ring.ringEnable)
    {
        erps_ring_active(inst, inst->ring.ringId);
    }

    return SYS_ERR_OK;
}
int erps_ringLevel_set(uint32 instId, uint32 level)
{
    erps_inst_t *inst = NULL;

    inst = erps_find_inst(instId);
    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }

    if (level == inst->ring.ringLevel)
    {
        return SYS_ERR_OK;
    }

    inst->ring.ringLevel = level;

    if (inst->ring.ringEnable)
    {
        erps_ring_active(inst, inst->ring.ringId);
    }

    return SYS_ERR_OK;
}

int erps_msgDeal_inst(uint32 instId, int status)
{
    int rc = SYS_ERR_FAILED;

    switch(status)
    {
        case Erps_Inst_Create:    
            rc = erps_inst_create(instId);
            break;
        case Erps_Inst_Delete:    
            rc = erps_inst_delete(instId);
            break;
        default:
            break;
    }

    return rc;
}

int erps_mel_set(uint32 instId, uint32 mel)
{
    erps_inst_t *inst = NULL;

    inst = erps_find_inst(instId);
    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }

    if (mel == inst->mel)
    {
        return SYS_ERR_OK;
    }
    
    inst->mel = mel;

    return SYS_ERR_OK;
}

int erps_workMode_set(uint32 instId, uint32 mode)
{
    erps_inst_t *inst = NULL;

    inst = erps_find_inst(instId);
    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }

    if (mode == inst->workMode)
    {
        return SYS_ERR_OK;
    }
    inst->workMode = mode;

    return SYS_ERR_OK;
}
void erps_set_each_inst_stp(void)
{
    erps_inst_t *inst   = erps_bridge.instInfo.next;
    int j;

    for (; NULL != inst; inst = inst->next)
    {
        for(j = 0; j < CAPA_STP_MST_INSTANCE_NUM; j++)
    	{
    		if(inst->bStgMap[j] == TRUE)
    		{
    			if(inst->ring.port0.portId != ERPS_RING_PORT_ID_DFT)
    			{
    				rsd_erps_ints_cfg_notice(j, inst->ring.port0.portId, SAL_PORT_STATE_VALUE_ONE);
    			}
    			if(inst->ring.port1.portId != ERPS_RING_PORT_ID_DFT)
    			{
    				rsd_erps_ints_cfg_notice(j, inst->ring.port1.portId, SAL_PORT_STATE_VALUE_ONE);
    			}
    		}
    	}

    }

    return;
}

int erps_inst_pgInst_set(uint32 instId, uint32 instList, uint32 status)
{
    int i;
    erps_inst_t *inst = NULL;
    int j;

	if ((status != Erps_Protected_Inst_Add) && (status != Erps_Protected_Inst_Del))
    {
        return SYS_ERR_FAILED;
    }

    inst = erps_find_inst(instId);
    if (NULL == inst)
    {
        return SYS_ERR_FAILED;
    }
    if(status == Erps_Protected_Inst_Add)
    {
    	for(j = 0; j < CAPA_STP_MST_INSTANCE_NUM; j++)
    	{
    		if(inst->bStgMap[j] == TRUE)
    		{
    			inst->bStgMap[j] = FALSE;
    			rsd_erps_ints_cfg_notice(j, inst->ring.port0.portId, SAL_PORT_STATE_VALUE_ZERO);
    			rsd_erps_ints_cfg_notice(j, inst->ring.port1.portId, SAL_PORT_STATE_VALUE_ZERO);
    			erps_set_each_inst_stp();
    		}
    	}
    }
    for(i = 0; i < CAPA_STP_MST_INSTANCE_NUM; i++)
    {
		if (instList == i)
        {
            if ((status == Erps_Protected_Inst_Add)
                && (inst->bStgMap[i] == FALSE))
            {
                inst->bStgMap[i] = TRUE;
                erps_set_port_state(inst, &inst->ring, inst->ring.port0.portId, inst->ring.port0.state);
                erps_set_port_state(inst, &inst->ring, inst->ring.port1.portId, inst->ring.port1.state);
                
    			rsd_erps_ints_cfg_notice(i, inst->ring.port0.portId, SAL_PORT_STATE_VALUE_ONE);
    			rsd_erps_ints_cfg_notice(i, inst->ring.port1.portId, SAL_PORT_STATE_VALUE_ONE);
            }
            else if ((status == Erps_Protected_Inst_Del)
                    && (inst->bStgMap[i] == TRUE))
            {
                inst->bStgMap[i] = FALSE;
                erps_set_port_state(inst, &inst->ring, inst->ring.port0.portId, inst->ring.port0.portEnable ? ERPS_Forwarding : ERPS_Blocking);
                erps_set_port_state(inst, &inst->ring, inst->ring.port1.portId, inst->ring.port0.portEnable ? ERPS_Forwarding : ERPS_Blocking);
                rsd_erps_ints_cfg_notice(i, inst->ring.port0.portId, SAL_PORT_STATE_VALUE_ZERO);
    			rsd_erps_ints_cfg_notice(i, inst->ring.port1.portId, SAL_PORT_STATE_VALUE_ZERO);
    			erps_set_each_inst_stp();
            }
        }
    }
    return SYS_ERR_OK;
}

int erps_msgDeal_updt_port_state(int instance_id)
{
    erps_inst_t *inst = NULL;

    for (inst = erps_bridge.instInfo.next; NULL != inst; inst = inst->next)
    {
        if (inst->bStgMap[instance_id] == TRUE)
	    {
	        erps_set_port_state(inst, &inst->ring, inst->ring.port0.portId, inst->ring.port0.state);
	        erps_set_port_state(inst, &inst->ring, inst->ring.port1.portId, inst->ring.port1.state);
	    }
        else
        {
            erps_set_port_state(inst, &inst->ring, inst->ring.port0.portId, inst->ring.port0.portEnable ? ERPS_Forwarding : ERPS_Blocking);
            erps_set_port_state(inst, &inst->ring, inst->ring.port1.portId, inst->ring.port0.portEnable ? ERPS_Forwarding : ERPS_Blocking);
        }
    }
    return TRUE;
}
erps_bool_t erps_port_reside(int port_no, uint32 *instId, int *ringId)
{
    erps_inst_t *inst = NULL;

    if ((port_no < LOGIC_PORT_START) || (port_no > LOGIC_PORT_END ))
    {
        return FALSE;
    }

    for (inst = erps_bridge.instInfo.next; NULL != inst; inst = inst->next)
    {
        if ((port_no == inst->ring.port0.portId) || (port_no == inst->ring.port1.portId))
        {
            if (NULL != instId)
            {
                *instId = inst->instId;
            }
            if (NULL != ringId)
            {
                *ringId = inst->ring.ringId;
            }
            return TRUE;
        }
    }

    return FALSE;
}
erps_bool_t erps_control_vlan(uint16 vlanID)
{
    erps_inst_t *inst = erps_bridge.instInfo.next;

    for (; NULL != inst; inst = inst->next)
    {
        if ((ERPS_INVALID_CTRLVLAN_ID != inst->ring.ctrlVlanId)
            && (vlanID == inst->ring.ctrlVlanId))
        {
            return TRUE;
        }
    }

    return FALSE;
}

void erps_tx_raps_sf_timer_start(erps_inst_t *inst)
{
    if (inst->ring.TxRAPSSFTimer.active != TRUE)
    {
		erps_start_timer(erps_tx_raps_sf_timer_expired, (unsigned long)inst->instId, 5 * SYS_TIMER_SEC, &inst->ring.TxRAPSSFTimer.value);
        inst->ring.TxRAPSSFTimer.active = TRUE;
    }
}

void erps_tx_raps_sf_timer_expired(unsigned long instId)
{
    erps_raps_pdu_t pkt;
	erps_inst_t *inst = NULL;
    
	inst = erps_find_inst((uint32)instId);
	if(NULL == inst)
	{
		return;
	}
	erps_tx_raps_sf_timer_stop(inst);
	erps_send_pkt(&pkt, inst, &inst->ring, inst->ring.port0.portId, FALSE);
	erps_send_pkt(&pkt, inst, &inst->ring, inst->ring.port1.portId, FALSE);
	
	{
		
		erps_tx_raps_sf_timer_start(inst);
	}
}
void erps_tx_raps_sf_timer_stop(erps_inst_t *inst)
{
	erps_stop_timer(&inst->ring.TxRAPSSFTimer.value);
    inst->ring.TxRAPSSFTimer.active = FALSE;
}
void erps_tx_raps_timer_start(erps_inst_t *inst)
{
    if (inst->ring.TxRAPSTimer.active != TRUE)
    {
		
		erps_start_timer(erps_tx_raps_timer_expired, (unsigned long)inst->instId, inst->TxRAPSTime * SYS_TIMER_SEC, &inst->ring.TxRAPSTimer.value);
        inst->ring.TxRAPSTimer.active = TRUE;
    }
}

void erps_tx_raps_timer_expired(unsigned long instId)
{
    erps_raps_pdu_t pkt;
	erps_inst_t *inst = NULL;
    
	inst = erps_find_inst((uint32)instId);
	if(NULL == inst)
	{
		return;
	}
	erps_tx_raps_timer_stop(inst);
	erps_send_pkt(&pkt, inst, &inst->ring, inst->ring.port0.portId, FALSE);
	erps_send_pkt(&pkt, inst, &inst->ring, inst->ring.port1.portId, FALSE);
	if (inst->ring.sendTimes > 0)
	{
		inst->ring.sendTimes--;
		erps_tx_raps_timer_start(inst);
	}
}
void erps_tx_raps_timer_stop(erps_inst_t *inst)
{
	erps_stop_timer(&inst->ring.TxRAPSTimer.value);
    inst->ring.TxRAPSTimer.active = FALSE;
}
void erps_wtr_timer_start(erps_inst_t *inst)
{
    if (inst->ring.WTRTimer.active != TRUE)
    {
		erps_start_timer(erps_wtr_timer_expired, (unsigned long)inst->instId, inst->WTRTime * 60 * SYS_TIMER_SEC, &inst->ring.WTRTimer.value);
        inst->ring.WTRTimer.active = TRUE;
    }
}

void erps_wtr_timer_expired(unsigned long instId)
{
	erps_inst_t *inst = NULL;
	inst = erps_find_inst((uint32)instId);
	if(NULL == inst)
	{
		return;
	}
	erps_wtr_timer_stop(inst);
	erps_event_stm(inst, &inst->ring, ERPS_EVENT_WTR_EXPIRES, 0);
}

void erps_wtr_timer_stop(erps_inst_t *inst)
{
	erps_stop_timer(&inst->ring.WTRTimer.value);
    inst->ring.WTRTimer.active = FALSE;
}

void erps_wtb_timer_start(erps_inst_t *inst)
{
    if (inst->ring.WTBTimer.active != TRUE)
    {
		erps_start_timer(erps_wtb_timer_expired, (unsigned long)inst->instId, inst->WTBTime * SYS_TIMER_SEC, &inst->ring.WTBTimer.value);
        inst->ring.WTBTimer.active = TRUE;
    }
}

void erps_wtb_timer_expired(unsigned long instId)
{
	erps_inst_t *inst = NULL;
    
	inst = erps_find_inst((uint32)instId);
	if(NULL == inst)
	{
		return;
	}
	erps_wtb_timer_stop(inst);
	erps_event_stm(inst, &inst->ring, ERPS_EVENT_WTB_EXPIRES, 0);
}

void erps_wtb_timer_stop(erps_inst_t *inst)
{
	erps_stop_timer(&inst->ring.WTBTimer.value);
    inst->ring.WTBTimer.active = FALSE;
}
void erps_guard_timer_start(erps_inst_t *inst)
{
    if (inst->ring.guardTimer.active != TRUE)
    {
		erps_start_timer(erps_guard_timer_expired, (unsigned long)inst->instId, inst->guardTime, &inst->ring.guardTimer.value);
        inst->ring.guardTimer.active = TRUE;
    }
}

void erps_guard_timer_expired(unsigned long instId)
{
	erps_inst_t *inst = NULL;
    
	inst = erps_find_inst((uint32)instId);
	if(NULL == inst)
	{
		return;
	}
	erps_guard_timer_stop(inst);
	erps_event_stm(inst, &inst->ring, ERPS_EVENT_GUARDTIME_EXPIRES, 0);
}

void erps_guard_timer_stop(erps_inst_t *inst)
{
	erps_stop_timer(&inst->ring.guardTimer.value);
    inst->ring.guardTimer.active = FALSE;
}
void erps_holdoff_timer_start(erps_inst_t *inst)
{
    if (inst->ring.holdoffTimer.active != TRUE)
    {
		erps_start_timer(erps_holdoff_timer_expired, (unsigned long)inst->instId, inst->holdoffTime * SYS_TIMER_SEC, &inst->ring.holdoffTimer.value);
        inst->ring.holdoffTimer.active = TRUE;
    }
}

void erps_holdoff_timer_expired(unsigned long instId)
{
	erps_inst_t *inst = NULL;
    
	inst = erps_find_inst((uint32)instId);
	if(NULL == inst)
	{
		return;
	}
	erps_holdoff_timer_stop(inst);
	if (inst->ring.port0.portEnable == FALSE)
	{
		erps_event_stm(inst, &inst->ring, ERPS_EVENT_HOLDOFF_EXPIRES, inst->ring.port0.portId);
	}
	if (inst->ring.port1.portEnable == FALSE)
	{
		erps_event_stm(inst, &inst->ring, ERPS_EVENT_HOLDOFF_EXPIRES, inst->ring.port1.portId);
	}
}

void erps_holdoff_timer_stop(erps_inst_t *inst)
{
	erps_stop_timer(&inst->ring.holdoffTimer.value);
    inst->ring.holdoffTimer.active = FALSE;
}
int erps_set_port_state(erps_inst_t *inst, erps_ring_t *ring, int portId, int state)
{
    erps_port_t *port = NULL;
    sys_physic_port_t phyPort;
    int stg = 0;
    uint32 port_state;

    if (portId == ERPS_RING_PORT_ID_DFT)
    {
        return SYS_ERR_FAILED;
    }
    else if (portId == ring->port0.portId)
    {
        port = &ring->port0;
    }
    else if (portId == ring->port1.portId)
    {
        port = &ring->port1;
    }

    if ((NULL == port) || ((!port->portEnable) && (ERPS_Disabled != state)))
    {
        return SYS_ERR_FAILED;
    }

    switch(state)
    {
        case ERPS_Disabled:
            port_state = STP_STATE_DISABLED;
            break;
        case ERPS_Blocking:
            port_state = STP_STATE_BLOCKING;
            break;
        case ERPS_Learning:
            port_state = STP_STATE_LEARNING;
            break;
        case ERPS_Forwarding:
            port_state = STP_STATE_FORWARDING;
            break;
        default:
            port_state = STP_STATE_END;
            break;
    }

    for (stg = 0; stg < CAPA_STP_MST_INSTANCE_NUM; stg++)
    {
    	if(inst->bStgMap[stg] == TRUE)
    	{
			SYS_MEM_CLEAR(phyPort);
			sys_logic_to_physic_port(portId, &phyPort);
			rtk_stp_mstpState_set(phyPort.dev, stg,
								phyPort.port, port_state);
			SYS_DBG(DBG_ERPS, "inst:%d,portId:%d,state:%u",stg,portId,port_state);
		}
    } 
    port->state = state;
    SYS_DBG(DBG_ERPS, " port:%d,state:%d\n",portId,state);
    g_erps_ring_port_state[portId] = state;
    return SYS_ERR_OK;
}

int erps_get_inst_port_state_status(uint32 portId, int32 *state)
{
	*state = g_erps_ring_port_state[portId];
	return SYS_ERR_OK;
}

int erps_get_port_enable(int portId, int *enable)
{
    int rc;
	sys_port_linkStatus_t link = PORT_LINKDOWN;

    if((portId < LOGIC_PORT_START) || (portId > LOGIC_PORT_END ) || (NULL == enable))
    {
        return SYS_ERR_FAILED;
    }
    
    rsd_port_linkStatus_get(portId, &link);

    if(PORT_LINKUP == link)
    {
        *enable = TRUE;
    }
    else
    {
        *enable = FALSE;
    }

    return rc;
}

uint8 erps_get_node_status(erps_ring_t *ring)
{
    return ring->stmStatus;
}
erps_inst_t * erps_find_inst(uint32 instId)
{
    erps_inst_t *inst   = erps_bridge.instInfo.next;
    erps_inst_t *rcInst = NULL;

    if ((instId > ERPS_MAX_RING_INS) || (instId < 0))
    {
        return NULL;
    }

    for (; NULL != inst; inst = inst->next)
    {
        if (inst->instId == instId)
        {
            rcInst = inst;
            break;
        }
    }

    return rcInst;
}
erps_ring_t * erps_find_ring(erps_inst_t **inst, int ringId, int port_id, uint32 ulVlanId)
{
    erps_inst_t   *rcInst = NULL;
    erps_ring_t   *ring   = NULL;

    for (rcInst = erps_bridge.instInfo.next; NULL != rcInst; rcInst = rcInst->next)
    {
        if (((rcInst->ring.ringId == ringId) || (rcInst->ring.ctrlVlanId == ulVlanId))
            &&((rcInst->ring.port0.portId == port_id) || (rcInst->ring.port1.portId == port_id)))
        {
            *inst = rcInst;
            ring = &rcInst->ring;
            break;
        }
    }

    return ring;
}
erps_bool_t erps_protected_inst_used_by_port(uint32 instId, int port_id, int stg)
{
    erps_inst_t *inst   = erps_bridge.instInfo.next;

    if (port_id == ERPS_RING_PORT_ID_DFT)
    {
        return FALSE;
    }

    for (; NULL != inst; inst = inst->next)
    {
        if (inst->instId == instId)
        {
            continue;
        }

        if (((inst->ring.port0.portId == port_id)
            || (inst->ring.port1.portId == port_id))
            && (inst->bStgMap[stg] == TRUE))
        {
            return TRUE;
        }
    }

    return FALSE;
}
int erps_edge_stm_init(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    erps_port_t *eventPort = NULL;
    erps_raps_pdu_t pkt;

    if (ring->port0.portId != ERPS_RING_PORT_ID_DFT)
    {
        eventPort = &ring->port0;
    }
    else
    {
        eventPort = &ring->port1;
    }

    erps_guard_timer_stop(inst);
    erps_wtr_timer_stop(inst);
    
    if (ring->ringLevel != 0) 
    {
        if (eventPort->portEnable)
        {
            eventPort->faultState = FALSE;
            erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
            erps_tx_raps_timer_start(inst);
            ring->sendDNF  = FALSE;
            if (ring->role == ERPS_RPL_Owner_Node)
            {
                ring->sendType = ERPS_PKT_NRRB;
                if(port_id == ring->port0.portId)
                {
                    eventPort->sendBPR = 0;
                }
                else
                {
                    eventPort->sendBPR = 1;
                }
            }
            else
            {
                ring->sendType = ERPS_PKT_NR;
            }

            ring->sendTimes = 3;
            erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE);
            ring->stmStatus = ERPS_STATUS_PENDING;
        }
        else
        {
            eventPort->faultState = TRUE;
            ring->stmStatus = ERPS_STATUS_PROTECTION;
        }
    }
    return SYS_ERR_OK;
}

int erps_stm_init(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    erps_raps_pdu_t pkt;

    if ((ring->port0.portId == ERPS_RING_PORT_ID_DFT) || (ring->port1.portId == ERPS_RING_PORT_ID_DFT))
    {
        return erps_edge_stm_init(inst, ring, event, port_id);
    }

    erps_guard_timer_stop(inst);
    erps_wtr_timer_stop(inst);
    
    if ((ring->port0.portEnable) && ((ring->port1.portEnable)))
    {
        
        if ((ring->role == ERPS_RPL_Owner_Node) || (ring->role == ERPS_RPL_Neighbour_Node))
        {
            if ((ring->port0.portRole == ERPS_RPL_Owner_PORT)
                || (ring->port0.portRole == ERPS_RPL_Neighbour_PORT))
            {
                erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Blocking);
                erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Forwarding);
            }

            if ((ring->port1.portRole == ERPS_RPL_Owner_PORT)
                || (ring->port1.portRole == ERPS_RPL_Neighbour_PORT))
            {
                erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Blocking);
                erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Forwarding);
            }

            if ((ring->role == ERPS_RPL_Owner_Node) && (inst->workMode == ERPS_MODE_REVERTIVE))
            {
                erps_wtr_timer_start(inst);
            }
        }
        
        else if ((ring->role == ERPS_RPL_Node) || (ring->role == ERPS_RPL_Next_Neighbour_Node))
        {
            erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Blocking);
            erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Forwarding);
        }
        else
        {
            return SYS_ERR_FAILED;
        }
        erps_tx_raps_timer_start(inst);
        ring->sendDNF  = FALSE;
        ring->port0.faultState = FALSE;
        ring->port1.faultState = FALSE;
        if (ring->role == ERPS_RPL_Owner_Node)
        {
            ring->sendType = ERPS_PKT_NRRB;
            if(port_id == ring->port0.portId)
            {
                ring->port0.sendBPR = 0;
                ring->port1.sendBPR = 0;
            }
            else
            {
                ring->port0.sendBPR = 1;
                ring->port1.sendBPR = 1;
            }
        }
        else
        {
            ring->sendType = ERPS_PKT_NR;
        }

        ring->sendTimes = 3;
        erps_send_pkt(&pkt, inst, ring, ring->port0.portId, FALSE);
        erps_send_pkt(&pkt, inst, ring, ring->port1.portId, FALSE);
        ring->stmStatus = ERPS_STATUS_PENDING;
    }
    else
    {
        if (ring->port0.portEnable)
        {
            ring->port0.faultState = FALSE;
            erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Forwarding);
            ring->port1.faultState = TRUE;
            erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Disabled);
            ring->port0.sendBPR = 1;
            ring->port1.sendBPR = 1;
            
        }
        else if (ring->port1.portEnable)
        {

            ring->port0.faultState = TRUE;
            erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Disabled);
            ring->port1.faultState = FALSE;
            erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Forwarding);
            ring->port0.sendBPR = 0;
            ring->port1.sendBPR = 0;
            
        }
        else
        {
            ring->port0.faultState = TRUE;
            erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Disabled);
            ring->port1.faultState = TRUE;
            erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Disabled);
            ring->port0.sendBPR = 0;
            ring->port1.sendBPR = 1;

        }
        
        erps_tx_raps_timer_start(inst);
        ring->sendDNF  = FALSE;
        ring->sendType = ERPS_PKT_SF;
        ring->sendTimes = 3;
        erps_send_pkt(&pkt, inst, ring, ring->port0.portId, FALSE); 
        erps_send_pkt(&pkt, inst, ring, ring->port1.portId, FALSE); 
        erps_tx_raps_sf_timer_start(inst);
        ring->stmStatus = ERPS_STATUS_PROTECTION;

    }
    return SYS_ERR_OK;
}

int erps_edge_stm_idle(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    erps_port_t *eventPort = NULL;
    erps_raps_pdu_t pkt;
    int32 portState;

    if (port_id == ring->port0.portId)
    {
        eventPort = &ring->port0;
    }
    else
    {
        eventPort = &ring->port1;
    }

    switch (event)
    {
        case ERPS_EVENT_LOCAL_SF:
            erps_tx_raps_timer_start(inst);
            erps_guard_timer_start(inst);
            if (((eventPort->portEnable) && (eventPort->state == ERPS_Blocking))
                 || ((!eventPort->portEnable) && (eventPort->state == ERPS_Disabled)))
            {
                ring->sendDNF  = TRUE;
                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    eventPort->sendBPR = 0;
                }
                else
                {
                    eventPort->sendBPR = 1;
                }

                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
            }
            else
            {
                if (!eventPort->portEnable)
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Disabled);
                }
                else
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                }

                if ((eventPort->portRole == ERPS_RPL_Owner_PORT)
                    || (eventPort->portRole == ERPS_RPL_Neighbour_PORT))
                {
                    ring->sendDNF  = TRUE;
                }
                else
                {
                    ring->sendDNF  = FALSE;
                }

                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    eventPort->sendBPR = 0;
                }
                else
                {
                    eventPort->sendBPR = 1;
                }
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
                erps_flush_mac(inst);
            }

            ring->stmStatus = ERPS_STATUS_PROTECTION;
            break;
        case ERPS_EVENT_SF:
            if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
            {
                erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
            }
            erps_tx_raps_timer_stop(inst);
            if (ring->rxDNF == FALSE)
            {
                erps_flush_mac(inst);
            }
            ring->stmStatus = ERPS_STATUS_PROTECTION;
            break;
        case ERPS_EVENT_NRRB:
            if (ring->role == ERPS_RPL_Neighbour_Node)
            {
            	SYS_DBG(DBG_ERPS, "ERPS_EVENT_NRRB ");
            	#if 0
                if ((eventPort->portRole == ERPS_RPL_Neighbour_PORT)
                    && (eventPort->state == ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                }
                if ((partnPort->portRole == ERPS_RPL_Neighbour_PORT)
                    && (partnPort->state == ERPS_Forwarding))

                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Blocking);
                }
                #endif
                
                if(eventPort->portRole == ERPS_RPL_Neighbour_PORT)
                {
                	erps_get_inst_port_state_status(eventPort->portId, &portState);
                	if(portState == ERPS_Forwarding)
                	{
                		SYS_DBG(DBG_ERPS, "ERPS_EVENT_NRRB port:%d,status:blocking",eventPort->portId);
                		erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                	}
                	              	
                }
            }
            #if 0
            {
                if ((eventPort->portRole == ERPS_RPL_Neighbour_PORT)
                    && (eventPort->state == ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                }
            }
            #endif
            if (ring->role != ERPS_RPL_Owner_Node)
            {
                erps_tx_raps_timer_stop(inst);
                if (ring->rxDNF == FALSE)
                {
                    erps_flush_mac(inst);
                }
            }
            #if 1 
            sal_trap_set_rma_port_rma_lldp_learn_disable_enable(ENABLED);
			#endif
            break;
        case ERPS_EVENT_NR: 
        	#if 0
            if ((ring->role == ERPS_RPL_Node) && ((ring->role == ERPS_RPL_Next_Neighbour_Node)))
            {
                if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
                }
                erps_tx_raps_timer_stop(inst);
            }
			#endif
            if(nodeIdCmp(inst->bridgeId, erps_bridge.bridgeId) > 0)
        	{
	            if ((ring->role == ERPS_RPL_Node) || ((ring->role == ERPS_RPL_Next_Neighbour_Node)))
	            {
	                if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
	                {
	                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
	                }

	                erps_tx_raps_timer_stop(inst);
	            }
            }
            break;
        default:
            break;
    }
    return SYS_ERR_OK;
}

int erps_stm_idle(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    erps_port_t *eventPort = NULL;
    erps_port_t *partnPort = NULL;
    erps_raps_pdu_t pkt;
    int32 portState;

    if ((ring->port0.portId == ERPS_RING_PORT_ID_DFT) || (ring->port1.portId == ERPS_RING_PORT_ID_DFT))
    {
        return erps_edge_stm_idle(inst, ring, event, port_id);
    }

    if (port_id == ring->port0.portId)
    {
        eventPort = &ring->port0;
        partnPort = &ring->port1;
    }
    else
    {
        partnPort = &ring->port0;
        eventPort = &ring->port1;
    }

    switch (event)
    {
        case ERPS_EVENT_LOCAL_SF:
            erps_tx_raps_timer_start(inst);
            erps_guard_timer_start(inst);
            if (((eventPort->portEnable) && (eventPort->state == ERPS_Blocking))
                 || ((!eventPort->portEnable) && (eventPort->state == ERPS_Disabled)))
            {
                if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                }
                ring->sendDNF  = TRUE;
                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    ring->port0.sendBPR = 0;
                    ring->port1.sendBPR = 0;
                }
                else
                {
                    ring->port0.sendBPR = 1;
                    ring->port1.sendBPR = 1;
                }

                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 

                if (((eventPort->portRole == ERPS_RPL_Next_Neighbour_PORT) && (!eventPort->portEnable))
                     || (eventPort->portRole != ERPS_RPL_Next_Neighbour_PORT))
                {
                    erps_send_pkt(&pkt, inst, ring, partnPort->portId, FALSE); 
                }
            }
            else
            {
                if (!eventPort->portEnable)
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Disabled);
                }
                else
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                }
                if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                }

                if ((eventPort->portRole == ERPS_RPL_Owner_PORT)
                    || (eventPort->portRole == ERPS_RPL_Neighbour_PORT))
                {
                    ring->sendDNF  = TRUE;
                }
                else
                {
                    ring->sendDNF  = FALSE;
                }

                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    ring->port0.sendBPR = 0;
                    ring->port1.sendBPR = 0;
                }
                else
                {
                    ring->port0.sendBPR = 1;
                    ring->port1.sendBPR = 1;
                }

                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 

                if (((eventPort->portRole == ERPS_RPL_Next_Neighbour_PORT) && (!eventPort->portEnable))
                     || (eventPort->portRole != ERPS_RPL_Next_Neighbour_PORT))
                {
                    erps_send_pkt(&pkt, inst, ring, partnPort->portId, FALSE); 
                }
                erps_flush_mac(inst);
            }

            ring->stmStatus = ERPS_STATUS_PROTECTION;
            break;
        case ERPS_EVENT_SF:
            if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
            {
                erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
            }
            if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
            {
                erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
            }
            erps_tx_raps_timer_stop(inst);
            if (ring->rxDNF == FALSE)
            {
                erps_flush_mac(inst);
            }
            ring->stmStatus = ERPS_STATUS_PROTECTION;
            break;
        case ERPS_EVENT_NRRB:
            if (ring->role == ERPS_RPL_Neighbour_Node)
            {
            	SYS_DBG(DBG_ERPS, "ERPS_EVENT_NRRB ");
            	#if 0
                if ((eventPort->portRole == ERPS_RPL_Neighbour_PORT)
                    && (eventPort->state == ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                }
                if ((partnPort->portRole == ERPS_RPL_Neighbour_PORT)
                    && (partnPort->state == ERPS_Forwarding))

                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Blocking);
                }
                #endif
                
                if(eventPort->portRole == ERPS_RPL_Neighbour_PORT)
                {
                	erps_get_inst_port_state_status(eventPort->portId, &portState);
                	if(portState == ERPS_Forwarding)
                	{
                		SYS_DBG(DBG_ERPS, "ERPS_EVENT_NRRB port:%d,status:blocking",eventPort->portId);
                		erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                	}
                	
					erps_get_inst_port_state_status(partnPort->portId, &portState);
					if(portState == ERPS_Blocking)
					{
						SYS_DBG(DBG_ERPS, "ERPS_EVENT_NRRB portid:%d,status:forwarding",partnPort->portId);
						erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
					}                	
                }
                if (partnPort->portRole == ERPS_RPL_Neighbour_PORT)
                {
                	erps_get_inst_port_state_status(partnPort->portId, &portState);
                	if(portState == ERPS_Forwarding)
                	{
                		SYS_DBG(DBG_ERPS, "ERPS_EVENT_NRRB port:%d,status:blocking",partnPort->portId);
                		erps_set_port_state(inst, ring, partnPort->portId, ERPS_Blocking);
                	}
                	erps_get_inst_port_state_status(eventPort->portId, &portState);
                	if(portState == ERPS_Blocking)
                	{
                		SYS_DBG(DBG_ERPS, "ERPS_EVENT_NRRB portid:%d,status:forwarding",eventPort->portId);
                		erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
                	}
                }
            }
            if (ring->role != ERPS_RPL_Owner_Node)
            {
                erps_tx_raps_timer_stop(inst);
                if (ring->rxDNF == FALSE)
                {
                    erps_flush_mac(inst);
                }
            }
            #if 1 
            sal_trap_set_rma_port_rma_lldp_learn_disable_enable(ENABLED);
			#endif
            break;
        case ERPS_EVENT_NR: 
        	if(nodeIdCmp(inst->bridgeId, erps_bridge.bridgeId) > 0)
        	{
	            if ((ring->role == ERPS_RPL_Node) || ((ring->role == ERPS_RPL_Next_Neighbour_Node)))
	            {
	                if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
	                {
	                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
	                }
	                if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
	                {
	                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
	                }
	                erps_tx_raps_timer_stop(inst);
	            }
            }

            if (ring->role == ERPS_RPL_Owner_Node)
            {
                ring->sendDNF  = TRUE;
                ring->sendType = ERPS_PKT_NRRB;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
            }

            break;
        default:
            break;
    }
    return SYS_ERR_OK;
}

int erps_edge_stm_protection(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    erps_port_t *eventPort = NULL;
    erps_raps_pdu_t pkt;

    if (port_id == ring->port0.portId)
    {
        eventPort = &ring->port0;
    }
    else
    {
        eventPort = &ring->port1;
    }

    switch (event)
    {
        case ERPS_EVENT_LOCAL_SF:
            erps_tx_raps_timer_start(inst);
            erps_guard_timer_start(inst);
            if (((eventPort->portEnable) && (eventPort->state == ERPS_Blocking))
                || ((!eventPort->portEnable) && (eventPort->state == ERPS_Disabled)))
            {
                ring->sendDNF  = TRUE;
                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    eventPort->sendBPR = 0;
                }
                else
                {
                    eventPort->sendBPR = 1;
                }
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
            }
            else
            {
                if (!eventPort->portEnable)
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Disabled);
                }
                else
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                }

                if ((eventPort->portRole == ERPS_RPL_Owner_PORT)
                    || (eventPort->portRole == ERPS_RPL_Neighbour_PORT))
                {
                    ring->sendDNF  = TRUE;
                }
                else
                {
                    ring->sendDNF  = FALSE;
                }
                ring->sendType = ERPS_PKT_SF;

                if(port_id == ring->port0.portId)
                {
                    eventPort->sendBPR = 0;
                }
                else
                {
                    eventPort->sendBPR = 1;
                }
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
                erps_flush_mac(inst);
            }
            break;
        case ERPS_EVENT_CLEAR_SF:
            erps_tx_raps_timer_start(inst);

            if (eventPort->portEnable)
            {
                erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
            }
            erps_guard_timer_start(inst);
            ring->sendDNF  = FALSE;
            ring->sendType = ERPS_PKT_NR;
            ring->sendTimes = 3;
            erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
            if ((ring->role == ERPS_RPL_Owner_Node) && (inst->workMode == ERPS_MODE_REVERTIVE))
            {
                erps_wtr_timer_start(inst); 
            }
            ring->stmStatus = ERPS_STATUS_PENDING;
            break;
        case ERPS_EVENT_NR:
            if (eventPort->faultState == FALSE)
            {
                if ((ring->role == ERPS_RPL_Owner_Node) && (inst->workMode == ERPS_MODE_REVERTIVE))
                {
                    erps_wtr_timer_start(inst); 
                }
                ring->stmStatus = ERPS_STATUS_PENDING;
            }
            break;
        case ERPS_EVENT_NRRB:
            ring->stmStatus = ERPS_STATUS_PENDING;
            break;
        default:
            break;
    }
    return SYS_ERR_OK;
}

int erps_stm_protection(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    erps_port_t *eventPort = NULL;
    erps_port_t *partnPort = NULL;
    erps_raps_pdu_t pkt;

    if ((ring->port0.portId == ERPS_RING_PORT_ID_DFT) || (ring->port1.portId == ERPS_RING_PORT_ID_DFT))
    {
        return erps_edge_stm_protection(inst, ring, event, port_id);
    }
    if (event != ERPS_EVENT_GUARDTIME_EXPIRES)
    {
        if (port_id == ring->port0.portId)
        {
            eventPort = &ring->port0;
            partnPort = &ring->port1;
        }
        else
        {
            partnPort = &ring->port0;
            eventPort = &ring->port1;
        }
    }

    switch (event)
    {
        case ERPS_EVENT_LOCAL_SF:
            erps_tx_raps_timer_start(inst);
            erps_guard_timer_start(inst);
            if (((eventPort->portEnable) && (eventPort->state == ERPS_Blocking))
                || ((!eventPort->portEnable) && (eventPort->state == ERPS_Disabled)))
            {
                if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                }
                ring->sendDNF  = TRUE;
                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    ring->port0.sendBPR = 0;
                    ring->port1.sendBPR = 0;
                }
                else
                {
                    ring->port0.sendBPR = 1;
                    ring->port1.sendBPR = 1;
                }
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
                erps_send_pkt(&pkt, inst, ring, partnPort->portId, FALSE); 
            }
            else
            {
                if (!eventPort->portEnable)
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Disabled);
                }
                else
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                }
                if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                }

                if ((eventPort->portRole == ERPS_RPL_Owner_PORT)
                    || (eventPort->portRole == ERPS_RPL_Neighbour_PORT))
                {
                    ring->sendDNF  = TRUE;
                }
                else
                {
                    ring->sendDNF  = FALSE;
                }
                ring->sendType = ERPS_PKT_SF;

                if(port_id == ring->port0.portId)
                {
                    ring->port0.sendBPR = 0;
                    ring->port1.sendBPR = 0;
                }
                else
                {
                    ring->port0.sendBPR = 1;
                    ring->port1.sendBPR = 1;
                }
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
                erps_send_pkt(&pkt, inst, ring, partnPort->portId, FALSE); 
                erps_flush_mac(inst);
            }
            break;
        case ERPS_EVENT_CLEAR_SF:
            erps_tx_raps_timer_start(inst);

            if (eventPort->portEnable)
            {
                erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
            }

            erps_guard_timer_start(inst);

            if (partnPort->faultState == FALSE)
            {
                if (partnPort->state != ERPS_Forwarding)
                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                }
                ring->sendDNF  = FALSE;
                ring->sendType = ERPS_PKT_NR;
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, ring->port0.portId, FALSE); 
                erps_send_pkt(&pkt, inst, ring, ring->port1.portId, FALSE); 
                if ((ring->role == ERPS_RPL_Owner_Node) && (inst->workMode == ERPS_MODE_REVERTIVE))
                {
                    erps_wtr_timer_start(inst); 
                }
                ring->stmStatus = ERPS_STATUS_PENDING;
            }
            break;
        case ERPS_EVENT_NR:
            if ( (eventPort->faultState == FALSE) && (partnPort->faultState == FALSE))
            {
                if ((ring->role == ERPS_RPL_Owner_Node) && (inst->workMode == ERPS_MODE_REVERTIVE))
                {
                    erps_wtr_timer_start(inst); 
                }
                ring->stmStatus = ERPS_STATUS_PENDING;
            }
            break;
        case ERPS_EVENT_NRRB:
            ring->stmStatus = ERPS_STATUS_PENDING;
            break;
        case ERPS_EVENT_GUARDTIME_EXPIRES:
            if ((ring->port0.faultState == FALSE) && ((ring->port0.state == ERPS_Blocking))
                 && (ring->port1.faultState == TRUE))
            {
                erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Forwarding);
            }
            else if ((ring->port1.faultState == FALSE) && ((ring->port1.state == ERPS_Blocking))
                 && (ring->port0.faultState == TRUE))
            {
                erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Forwarding);
            }
            break;
        default:
            break;
    }
    return SYS_ERR_OK;
}

int erps_stm_ms(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    return SYS_ERR_OK;
}
int erps_stm_fs(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    return SYS_ERR_OK;
}
int erps_edge_stm_pending(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    erps_port_t *eventPort = NULL;
    erps_raps_pdu_t pkt;

    if (event != ERPS_EVENT_WTR_EXPIRES)
    {
        if (port_id == ring->port0.portId)
        {
            eventPort = &ring->port0;
        }
        else
        {
            eventPort = &ring->port1;
        }
    }
    else
    {
        if (ring->port0.portId != ERPS_RING_PORT_ID_DFT)
        {
            eventPort = &ring->port0;
        }
        else
        {
            eventPort = &ring->port1;
        }
    }
    switch(event)
    {
        case ERPS_EVENT_LOCAL_SF:
            erps_tx_raps_timer_start(inst);
            erps_guard_timer_start(inst);

            if (((eventPort->portEnable) && (eventPort->state == ERPS_Blocking))
                || ((!eventPort->portEnable) && (eventPort->state == ERPS_Disabled)))
            {
                ring->sendDNF  = TRUE;
                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    eventPort->sendBPR = 0;
                }
                else
                {
                    eventPort->sendBPR = 1;
                }
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
            }
            else
            {
                if (!eventPort->portEnable)
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Disabled);
                }
                else
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                }

                if ((eventPort->portRole == ERPS_RPL_Owner_PORT)
                    || (eventPort->portRole == ERPS_RPL_Neighbour_PORT))
                {
                    ring->sendDNF  = TRUE;
                }
                else
                {
                    ring->sendDNF  = FALSE;
                }
                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    eventPort->sendBPR = 0;
                }
                else
                {
                    eventPort->sendBPR = 1;
                }
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
                erps_flush_mac(inst);
            }
            if (ring->role == ERPS_RPL_Owner_Node)
            {
                erps_wtr_timer_stop(inst);
                
            }
            ring->stmStatus = ERPS_STATUS_PROTECTION;
            break;
        case ERPS_EVENT_SF:
            if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
            {
                erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
            }

            erps_tx_raps_timer_stop(inst);

            if (ring->role == ERPS_RPL_Owner_Node)
            {
                erps_wtr_timer_stop(inst);
                
            }

            if (ring->rxDNF == FALSE)
            {
                erps_flush_mac(inst);
            }
            ring->stmStatus = ERPS_STATUS_PROTECTION;
            break;
        case ERPS_EVENT_WTR_EXPIRES:
            if (ring->role == ERPS_RPL_Owner_Node)
            {
                erps_tx_raps_timer_start(inst);
                erps_wtr_timer_stop(inst);
                if ((ring->port0.portId!= ERPS_RING_PORT_ID_DFT) && (ring->port0.portRole == ERPS_RPL_Owner_PORT))
                {
                    eventPort = &ring->port0;
                    eventPort->sendBPR = 0;
                }
                else if((ring->port1.portId != ERPS_RING_PORT_ID_DFT) && (ring->port1.portRole == ERPS_RPL_Owner_PORT))
                {
                    eventPort = &ring->port1;
                    eventPort->sendBPR = 1;
                }

                ring->sendTimes = 3;

                if (eventPort->state == ERPS_Blocking)
                {
                    ring->sendDNF  = TRUE;
                    ring->sendType = ERPS_PKT_NRRB;
                    erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
                }
                else
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                    ring->sendDNF  = FALSE;
                    ring->sendType = ERPS_PKT_NRRB;
                    erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
                    erps_flush_mac(inst);
                }
            }
            #if 1
            sal_trap_set_rma_port_rma_lldp_learn_disable_enable(ENABLED);
			#endif
            ring->stmStatus = ERPS_STATUS_IDLE;
            break;
        case ERPS_EVENT_NRRB:
            if (ring->role == ERPS_RPL_Owner_Node)
            {
                erps_wtr_timer_stop(inst);
                
            }
            else if (ring->role == ERPS_RPL_Neighbour_Node)
            {
                erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                erps_tx_raps_timer_stop(inst);

                if (ring->rxDNF == FALSE)
                {
                    erps_flush_mac(inst);
                }
            }
            else
            {
                if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
                }
                erps_tx_raps_timer_stop(inst);

                if (ring->rxDNF == FALSE)
                {
                    erps_flush_mac(inst);
                }
            }
            #if 1 
            sal_trap_set_rma_port_rma_lldp_learn_disable_enable(ENABLED);
			#endif
            ring->stmStatus = ERPS_STATUS_IDLE;
            break;
        case ERPS_EVENT_NR:
        	#if 0
            if (ring->role != ERPS_RPL_Owner_Node)
            {
                if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
                }
                erps_tx_raps_timer_stop(inst);
            }
            #endif
            if(nodeIdCmp(inst->bridgeId, erps_bridge.bridgeId) > 0)
        	{            	
                if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
                {	
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
                    
                }

                erps_tx_raps_timer_stop(inst);
	            
            }
            break;
        default:
            break;
    }
    return SYS_ERR_OK;
}
int erps_stm_pending(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    erps_port_t *eventPort = NULL;
    erps_port_t *partnPort = NULL;
    erps_raps_pdu_t pkt;

    if ((ring->port0.portId == ERPS_RING_PORT_ID_DFT) || (ring->port1.portId == ERPS_RING_PORT_ID_DFT))
    {
        return erps_edge_stm_pending(inst, ring, event, port_id);
    }

    if (event != ERPS_EVENT_WTR_EXPIRES)
    {
        if (port_id == ring->port0.portId)
        {
            eventPort = &ring->port0;
            partnPort = &ring->port1;
        }
        else
        {
            partnPort = &ring->port0;
            eventPort = &ring->port1;
        }
    }

    switch(event)
    {
        case ERPS_EVENT_LOCAL_SF:
            erps_tx_raps_timer_start(inst);
            erps_guard_timer_start(inst);

            if (((eventPort->portEnable) && (eventPort->state == ERPS_Blocking))
                || ((!eventPort->portEnable) && (eventPort->state == ERPS_Disabled)))
            {
                if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                }
                ring->sendDNF  = TRUE;
                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    ring->port0.sendBPR = 0;
                    ring->port1.sendBPR = 0;
                }
                else
                {
                    ring->port0.sendBPR = 1;
                    ring->port1.sendBPR = 1;
                }
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
                erps_send_pkt(&pkt, inst, ring, partnPort->portId, FALSE); 
            }
            else
            {
                if (!eventPort->portEnable)
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Disabled);
                }
                else
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                }

                if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                }

                    ring->sendDNF  = FALSE;

                ring->sendType = ERPS_PKT_SF;
                if(port_id == ring->port0.portId)
                {
                    ring->port0.sendBPR = 0;
                    ring->port1.sendBPR = 0;
                }
                else
                {
                    ring->port0.sendBPR = 1;
                    ring->port1.sendBPR = 1;
                }
                ring->sendTimes = 3;
                erps_send_pkt(&pkt, inst, ring, eventPort->portId, FALSE); 
                erps_send_pkt(&pkt, inst, ring, partnPort->portId, FALSE); 
                erps_flush_mac(inst);
            }
            if (ring->role == ERPS_RPL_Owner_Node)
            {
                erps_wtr_timer_stop(inst);
                
            }
            ring->stmStatus = ERPS_STATUS_PROTECTION;
            break;
        case ERPS_EVENT_SF:
            if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
            {
                erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
            }

            if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
            {
                erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
            }

            erps_tx_raps_timer_stop(inst);

            if (ring->role == ERPS_RPL_Owner_Node)
            {
                erps_wtr_timer_stop(inst);
                
            }

            if (ring->rxDNF == FALSE)
            {
                erps_flush_mac(inst);
            }
            ring->stmStatus = ERPS_STATUS_PROTECTION;
            break;
        case ERPS_EVENT_WTR_EXPIRES:
            if (ring->role == ERPS_RPL_Owner_Node)
            {
                erps_tx_raps_timer_start(inst);
                erps_wtr_timer_stop(inst);
                if (ring->port0.portRole == ERPS_RPL_Owner_PORT)
                {
                    ring->port0.sendBPR = 0;
                    ring->port1.sendBPR = 0;
                    ring->sendTimes = 3;
                    if (ring->port0.state == ERPS_Blocking)
                    {
                        erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Forwarding);
                        ring->sendDNF  = TRUE;
                        ring->sendType = ERPS_PKT_NRRB;
                        erps_send_pkt(&pkt, inst, ring, ring->port0.portId, FALSE); 
                        erps_send_pkt(&pkt, inst, ring, ring->port1.portId, FALSE); 
                    }
                    else
                    {
                        erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Blocking);
                        erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Forwarding);
                        ring->sendDNF  = FALSE;
                        ring->sendType = ERPS_PKT_NRRB;
                        erps_send_pkt(&pkt, inst, ring, ring->port0.portId, FALSE); 
                        erps_send_pkt(&pkt, inst, ring, ring->port1.portId, FALSE); 
                        erps_flush_mac(inst);
                    }
                }
                else if (ring->port1.portRole == ERPS_RPL_Owner_PORT)
                {
                    ring->port0.sendBPR = 1;
                    ring->port1.sendBPR = 1;
                    ring->sendTimes = 3;
                    if (ring->port1.state == ERPS_Blocking)
                    {
                        erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Forwarding);
                        ring->sendDNF  = TRUE;
                        ring->sendType = ERPS_PKT_NRRB;
                        erps_send_pkt(&pkt, inst, ring, ring->port0.portId, FALSE);
                        erps_send_pkt(&pkt, inst, ring, ring->port1.portId, FALSE);
                    }
                    else
                    {
                        erps_set_port_state(inst, ring, ring->port0.portId, ERPS_Forwarding);
                        erps_set_port_state(inst, ring, ring->port1.portId, ERPS_Blocking);
                        ring->sendDNF = FALSE;
                        ring->sendType = ERPS_PKT_NRRB;
                        erps_send_pkt(&pkt, inst, ring, ring->port0.portId, FALSE); 
                        erps_send_pkt(&pkt, inst, ring, ring->port1.portId, FALSE); 
                        erps_flush_mac(inst);
                    }
                }
            }
            #if 1
            sal_trap_set_rma_port_rma_lldp_learn_disable_enable(ENABLED);
			#endif
            ring->stmStatus = ERPS_STATUS_IDLE;
            break;
        case ERPS_EVENT_NRRB:
            if (ring->role == ERPS_RPL_Owner_Node)
            {
                erps_wtr_timer_stop(inst);
                
            }
            else if (ring->role == ERPS_RPL_Neighbour_Node)
            {
                if (eventPort->portRole == ERPS_RPL_Neighbour_PORT)
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Blocking);
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                }
                if (partnPort->state == ERPS_RPL_Neighbour_PORT)
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Blocking);
                }
                erps_tx_raps_timer_stop(inst);

                if (ring->rxDNF == FALSE)
                {
                    erps_flush_mac(inst);
                }
            }
            else
            {
                if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
                }

                if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
                {
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                }
                erps_tx_raps_timer_stop(inst);

                if (ring->rxDNF == FALSE)
                {
                    erps_flush_mac(inst);
                }
            }
            #if 1 
            sal_trap_set_rma_port_rma_lldp_learn_disable_enable(ENABLED);
			#endif

            ring->stmStatus = ERPS_STATUS_IDLE;
            break;
        case ERPS_EVENT_NR:
        	SYS_DBG(DBG_ERPS, " ERPS_EVENT_NR");
        	if(nodeIdCmp(inst->bridgeId, erps_bridge.bridgeId) > 0)
        	{            	
                if ((eventPort->faultState == FALSE) && (eventPort->state != ERPS_Forwarding))
                {	
                    erps_set_port_state(inst, ring, eventPort->portId, ERPS_Forwarding);
                    
                }
                if ((partnPort->faultState == FALSE) && (partnPort->state != ERPS_Forwarding))
                {
                	
                    erps_set_port_state(inst, ring, partnPort->portId, ERPS_Forwarding);
                    
                }

                erps_tx_raps_timer_stop(inst);
	            
            }
            break;
        default:
            break;
    }
    return SYS_ERR_OK;
}
int erps_event_stm(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id)
{
    erps_port_t *eventPort = NULL;
    erps_port_t *partnPort = NULL;
    erps_stm_state_t preNodeState = ring->stmStatus;

	if((NULL == ring) || (NULL == inst))
	{
		return SYS_ERR_FAILED;
	}
    erps_get_port_enable(inst->ring.port0.portId, &inst->ring.port0.portEnable);
	erps_get_port_enable(inst->ring.port1.portId, &inst->ring.port1.portEnable);
    if ((event != ERPS_EVENT_NONE) && (event != ERPS_EVENT_WTR_EXPIRES)
         && (event != ERPS_EVENT_GUARDTIME_EXPIRES))
    {
        if (port_id == ring->port0.portId)
        {
            eventPort = &ring->port0;
            partnPort = &ring->port1;
        }
        else if (port_id == ring->port1.portId)
        {
            eventPort = &ring->port1;
            partnPort = &ring->port0;
        }
        else
        {
            return SYS_ERR_FAILED;
        }
        if ((event == ERPS_EVENT_LOCAL_SF) && (eventPort->faultState == FALSE))
        {
            memset(eventPort->nodeId, 0, 6);
            eventPort->BPR = 0;
            eventPort->faultState = TRUE;

            memset(partnPort->nodeId, 0, 6);
            partnPort->BPR = 0;
        }
        if ((event == ERPS_EVENT_CLEAR_SF) && (eventPort->faultState == TRUE))
        {
            memset(eventPort->nodeId, 0, 6);
            eventPort->BPR = 0;
            eventPort->faultState = FALSE;

            memset(partnPort->nodeId, 0, 6);
            partnPort->BPR = 0;
        }
    }
    switch (ring->stmStatus)
    {
        case ERPS_STATUS_INIT:          
            erps_stm_init(inst, ring, event, port_id);
            break;
        case ERPS_STATUS_IDLE:          
            erps_stm_idle(inst, ring, event, port_id);
            break;
        case ERPS_STATUS_PROTECTION:    
            erps_stm_protection(inst, ring, event, port_id);
            break;
        case ERPS_STATUS_MANUAL_SWITCH: 
            erps_stm_ms(inst, ring, event, port_id);
            break;
        case ERPS_STATUS_FORCED_SWITCH: 
            erps_stm_fs(inst, ring, event, port_id);
            break;
        case ERPS_STATUS_PENDING:       
        	{

            erps_stm_pending(inst, ring, event, port_id);
            }
            break;
        default:
            break;
    }
    if ((NULL != inst) && (ring->stmStatus != preNodeState))
    {
        char  logStr[128] = {'\0'};
        char *roleStr[]    = {"Common", "Owner", "Neighbour","Next-Neighbour","NONE"};
        char *stateStr[] = {"Init", "Idle", "Protection", "Manual Switch", "Forced Switch", "Pending"};

        sprintf(logStr, "%s node(instance %d, ringId %d), STM [%s]-->[%s]",
            roleStr[(int)ring->role], inst->instId, ring->ringId,
            stateStr[preNodeState], stateStr[ring->stmStatus]);

		SYS_DBG(DBG_ERPS, "NODE-STM: %s", logStr);
		
    }
    return SYS_ERR_OK;
}
void erps_rcv_nrrb(erps_raps_pdu_t *pkt, erps_inst_t *inst, erps_ring_t *ring, int portId)
{
    erps_port_t *eventPort = NULL;
    erps_port_t *partnPort = NULL;
    int reTrans = FALSE;

    if (portId == ring->port0.portId)
    {
        eventPort = &ring->port0;
        partnPort = &ring->port1;
    }
    else if(portId == ring->port1.portId)
    {
        partnPort = &ring->port0;
        eventPort = &ring->port1;
    }
    else
    {
        return;
    }

    if ((memcmp(pkt->node_id, eventPort->nodeId, 6) != 0) || (pkt->status_BPR != eventPort->BPR))
    {
        if (ring->role != ERPS_RPL_Owner_Node)
        {
            memcpy(eventPort->nodeId, pkt->node_id, 6);
            eventPort->BPR = pkt->status_BPR;
        }
    }
    else
    {
        ring->rxDNF = TRUE;
    }

    if (eventPort->state == ERPS_Forwarding)
    {
        reTrans = TRUE;
    }

    erps_event_stm(inst, ring, ERPS_EVENT_NRRB, portId);

    if (memcmp(pkt->ethHeader.src_mac, erps_bridge.bridgeId, 6) == 0)
	{
		return;
	}

    if ((pkt != NULL) && (reTrans == TRUE) && (ring->role != ERPS_RPL_Owner_Node)
        && (partnPort->state == ERPS_Forwarding))
    {
        erps_send_pkt(pkt, inst, ring, partnPort->portId, TRUE);
    }
}

void erps_rcv_nr(erps_raps_pdu_t *pkt, erps_inst_t *inst, erps_ring_t *ring, int portId)
{
    erps_port_t *eventPort = NULL;
    erps_port_t *partnPort = NULL;
    int reTrans = FALSE;
    uint8 allZero[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    if (portId == ring->port0.portId)
    {
        eventPort = &ring->port0;
        partnPort = &ring->port1;
    }
    else if (portId == ring->port1.portId)
    {
        partnPort = &ring->port0;
        eventPort = &ring->port1;
    }
    else
    {
        return;
    }
    
    if (memcmp(pkt->node_id, erps_bridge.bridgeId, 6) == 0)
    {
       return;
    }

    if (eventPort->state == ERPS_Forwarding)
    {
        reTrans = TRUE;
    }

    if (memcmp(eventPort->nodeId, allZero, 6) != 0)
    {
        memset(eventPort->nodeId, 0, 6);
        eventPort->BPR = 0;
    }
	memcpy(inst->bridgeId, pkt->node_id,6);

    erps_event_stm(inst, ring, ERPS_EVENT_NR, portId);

    if ((pkt != NULL) && (reTrans = TRUE) && (partnPort->state == ERPS_Forwarding))
    {
        erps_send_pkt(pkt, inst, ring, partnPort->portId, TRUE);
    }
    return;

}

void erps_rcv_sf(erps_raps_pdu_t *pkt, erps_inst_t *inst, erps_ring_t *ring, int portId)
{
    erps_port_t *eventPort = NULL;
    erps_port_t *partnPort = NULL;
    int event = ERPS_EVENT_SF;

    if( portId == ring->port0.portId)
    {
        eventPort = &ring->port0;
        partnPort = &ring->port1;
    }
    else if (portId == ring->port1.portId)
    {
        partnPort = &ring->port0;
        eventPort = &ring->port1;
    }
    else
    {
        return;
    }
    
    if (memcmp(pkt->node_id, erps_bridge.bridgeId, 6) == 0)
    {
       return;
    }

    if (eventPort->state == ERPS_Forwarding)
    {
        if ((memcmp(pkt->node_id, eventPort->nodeId, 6) != 0) || (pkt->status_BPR != eventPort->BPR))
        {
            memcpy(eventPort->nodeId, pkt->node_id, 6);
            eventPort->BPR = pkt->status_BPR;
        }
        else
        {
            ring->rxDNF = TRUE;
        }
    }

    erps_event_stm(inst, ring, event, portId);

    if ((pkt != NULL) &&(partnPort->state == ERPS_Forwarding))
    {
        erps_send_pkt(pkt, inst, ring, partnPort->portId, TRUE);
    }

}

int erps_stm_rcv(erps_raps_pdu_t *pkt, uint32 portId, uint32 ulVlanId)
{
    erps_inst_t *inst = NULL;
    erps_ring_t *ring = NULL;
    int ringId;
    uint8 pkttype = 0;

    if (NULL == pkt)
    {
        return SYS_ERR_FAILED;
    }

    if (!erps_bridge.bErpsEnable)
    {
        return SYS_ERR_OK;
    }
    if (pkt->ethHeader.etherType != htons(ERPS_ETHER_TYPE))
    {
        return SYS_ERR_OK;
    }

    ringId = pkt->ethHeader.dst_mac[5];
    
    ring = erps_find_ring(&inst, ringId, portId, ulVlanId);

    if( (NULL == inst) || (NULL == ring) )
    {
        return SYS_ERR_FAILED;
    }
    if (pkt->cmnHeader.mel != inst->mel)
    {
        return SYS_ERR_FAILED;
    }
    if(!ring->ringEnable)
    {
        return SYS_ERR_FAILED;
    }

    if ((ring->ringId == ringId) && (ring->ctrlVlanId == ulVlanId))
    {
        
        if (ring->guardTimer.active == TRUE)
        {
            return SYS_ERR_OK;
        }
        ring->rxDNF = pkt->status_DNF;
        switch (pkt->request_state)
        {
            case ERPS_PKT_NR:
                if (pkt->status_RB)
                {
                    erps_rcv_nrrb(pkt, inst, ring, portId);
                    pkttype = ERPS_PKT_NR;
                }
                else
                {
                    erps_rcv_nr(pkt, inst, ring, portId);
                    pkttype = ERPS_PKT_NRRB;
                }
                break;
            case ERPS_PKT_SF:
                erps_rcv_sf(pkt, inst, ring, portId);
                pkttype = ERPS_PKT_SF;
                break;
            case ERPS_PKT_MS:
            case ERPS_PKT_FS:
            case ERPS_PKT_EVENT:
            default:
                break;
        }
        erps_pkt_cnt(ring, portId, pkttype, 1);
    }

    return SYS_ERR_OK;
}
int erps_clear_port(erps_port_t *port)
{
    if (port->portId == ERPS_RING_PORT_ID_DFT)
    {
        return SYS_ERR_FAILED;
    }
    port->rcvNRCnt    = 0;
    port->rcvSFCnt    = 0;
    port->rcvNRRBCnt  = 0;
    port->sendNRCnt   = 0;
    port->sendSFCnt   = 0;
    port->sendNRRBCnt = 0;
    return SYS_ERR_OK;
}

void erps_pkt_cnt(erps_ring_t *ring, int portId, uint8 pkttype, uint8 ucInpkt)
{
    erps_port_t *eventPort = NULL;
    if (portId == ring->port0.portId)
    {
        eventPort = &ring->port0;
    }
    else if (portId == ring->port1.portId)
    {
        eventPort = &ring->port1;
    }
    else
    {
        return;
    }
    switch (pkttype)
    {
        case ERPS_PKT_NR:
            if (ucInpkt)
            {
                eventPort->rcvNRCnt ++;
            }
            else
            {
                eventPort->sendNRCnt ++;
            }
            break;
        case ERPS_PKT_NRRB:
            if (ucInpkt)
            {
                eventPort->rcvNRRBCnt ++;
            }
            else
            {
                eventPort->sendNRRBCnt ++;
            }
            break;
        case ERPS_PKT_SF:
            if (ucInpkt)
            {
                eventPort->rcvSFCnt ++;
            }
            else
            {
                eventPort->sendSFCnt ++;
            }
            break;
        case ERPS_PKT_MS:
            if (ucInpkt)
            {
                eventPort->rcvMSCnt ++;
            }
            else
            {
                eventPort->sendMSCnt ++;
            }
            break;
        case ERPS_PKT_FS:
            if (ucInpkt)
            {
                eventPort->rcvFSCnt ++;
            }
            else
            {
                eventPort->sendFSCnt ++;
            }
            break;
        case ERPS_PKT_EVENT:
            if (ucInpkt)
            {
                eventPort->rcvEventCnt ++;
            }
            else
            {
                eventPort->sendEventCnt ++;
            }
            break;
        default:
            break;
    }
}

void erps_compose_pkt(erps_raps_pdu_t *pkt, erps_inst_t *inst, erps_ring_t *ring, int portId)
{
    uint8 erpsDMac[6] = {0x01, 0x19, 0xa7, 0x00, 0x00, 0x00};

    memcpy(pkt->ethHeader.src_mac, erps_bridge.bridgeId, 6);
    erpsDMac[5] = ring->ringId;
    memcpy(pkt->ethHeader.dst_mac, erpsDMac, 6);
    pkt->ethHeader.usTpid     = htons(0x8100);
    pkt->ethHeader.usVlanTag  = htons((uint16)((ERPS_FRAME_PRIORITY << 13) | ring->ctrlVlanId));
    pkt->ethHeader.etherType  = htons(ERPS_ETHER_TYPE);
    pkt->cmnHeader.version    = 0x01;
    pkt->cmnHeader.mel        = inst->mel;
    pkt->cmnHeader.opCode     = 0x28;
    pkt->cmnHeader.flags      = 0x00;
    pkt->cmnHeader.tlv_Offset = 0x20;

    pkt->request_state        = (ring->sendType & 0x000f);
    pkt->sub_code             = 0x00;
    
    if ((ring->role == ERPS_RPL_Owner_Node) && (ring->sendType == ERPS_PKT_NRRB))
    {
        pkt->status_RB        = 0x01;
    }
    else
    {
        pkt->status_RB        = 0x00;
    }

    pkt->status_DNF           = ring->sendDNF;

    if (portId == ring->port0.portId)
    {
        pkt->status_BPR       = ring->port0.sendBPR;
    }
    else
    {
        pkt->status_BPR       = ring->port1.sendBPR;
    }
    pkt->status_reserved      = 0x00;
    memcpy(pkt->node_id, erps_bridge.bridgeId, 6);
    memset(pkt->reserved, 0, 24);
    pkt->end_tlv              = 0x00;

}

void erps_send_pkt(erps_raps_pdu_t *pkt, erps_inst_t *inst, erps_ring_t *ring, int portId, uint32 transPkt)
{
    int ctrlVlan = 0;

    if ((ring == NULL) || (portId == ERPS_RING_PORT_ID_DFT))
    {
		return;
    }
    if (((ring->port0.portId == portId) && (!ring->port0.portEnable))
        ||((ring->port1.portId == portId) && (!ring->port1.portEnable)))
    {
		return;
    }

    ctrlVlan = ring->ctrlVlanId;
    if (transPkt == FALSE)
    {
        erps_compose_pkt(pkt, inst, ring, portId);
    }

    if (erps_pkt_tx(portId, ctrlVlan, pkt, sizeof(erps_raps_pdu_t)) == SYS_ERR_FAILED)
    {
		return;
    }

    erps_pkt_cnt(ring, portId, ring->sendType, 0);

    return;
}
