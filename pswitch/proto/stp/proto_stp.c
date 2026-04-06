#include <common/sys_def.h>

#include <rsd/rsd_l2.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_qos.h>
#include <rsd/rsd_stp.h>
#include <rsd/rsd_sys.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_port.h>
#include <stp/proto_stp.h>
#include <stp/proto_stp_mstp_digest.h>
#include <stp/proto_stp_mstp.h>
#ifdef CONFIG_SYS_PROTO_ERPS
#include <rsd/rsd_erps.h>
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
#include <rsd/rsd_loopback.h>
#endif
static int32 stp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
static int32 stp_linkEvent(uint32 event, char *pData);
static void mstiBridgeParam_update(mstp_bridge_t *pBridge, mstp_u32_t xstIdx);
static int32 stp_mstpState_set(uint32 msti, sys_logic_port_t port, sys_stp_state_t stp_state);
static int32 stp_portState_change(uint32 instance, uint32 portId, uint32 portState);

static rsd_nic_handler_t l2gHandler = {
    .priority       = RSD_NIC_HANDLER_PRI_STP,
    .dmac           = { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x00 },
    .dmac_caremask  = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
    .ethertype      = 0,
    .proto          = 0,
    .sport          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_TRAP,
    .rx_cbf         = stp_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_TRUNK,
    .pName          = "STP RX Thread",
    .pCookie        = NULL,
};

static volatile int32 stp_fsm_thread_status = 0;
static volatile int32 stp_routine_thread_status = 0;
static volatile int32 stp_tick_thread_status = 0;

typedef struct stp_mst_vlan_s
{
    uint16 msti[MAX_MSTP_VLAN_NUM];
    uint16 vlanCount[MAX_MSTP_INSTANCE];
    sys_vlanmask_t vlanMask[MAX_MSTP_INSTANCE];
} stp_mst_vlan_t;

enum stp_trunk_port_role_e
{
    STP_TRUNK_PORT_ROLE_NONE = 0,
    STP_TRUNK_PORT_ROLE_MAJOR,
    STP_TRUNK_PORT_ROLE_MINOR,
};

static osal_mutex_t stp_sem;
static mstp_bridge_t mstpBridge;

static sys_enable_t stp_forwardBPDU = ENABLED;
static sys_logic_portmask_t stp_pmForwardingBPDU;
static sys_logic_portmask_t stp_pmLinkup;
static sys_stp_pathcost_method_t stp_pathcostMethod = STP_PATHCOST_METHOD_LONG;
static stp_mst_vlan_t mstVlan;

#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT

#define STP_SELFLOOP_DELAY_TIME     5   
#endif

static uint8 strState[69][24] = {
    "ONE_SECOND",
    "TICK",

    "DISCARD",
    "RECEIVE",

    "CHECKING_RSTP",
    "SELECTING_STP",
    "SENSING",

    "EDGE",
    "NOT_EDGE",

    "TRANSMIT_INIT",
    "TRANSMIT_PERIODIC",
    "TRANSMIT_CONFIG",
    "TRANSMIT_TCN",
    "TRANSMIT_RSTP",
    "IDLE",

    "DISABLED",
    "AGED",
    "UPDATE",
    "SUPERIOR_DESIGNATED",
    "REPEATED_DESIGNATED",
    "INFERIOR_DESIGNATED",
    "NOT_DESIGNATED",
    "OTHER",
    "CURRENT",
    "INIT_TREE",
    "ROLE_SELECTION",

    "INIT_PORT",
    "DISABLE_PORT",
    "DISABLED_PORT",
    "MASTER_PROPOSED",
    "MASTER_AGREED",
    "MASTER_SYNCED",
    "MASTER_RETIRED",
    "MASTER_FORWARD",
    "MASTER_LEARN",
    "MASTER_DISCARD",
    "MASTER_PORT",
    "ROOT_PROPOSED",
    "ROOT_AGREED",
    "ROOT_SYNCED",
    "REROOT",
    "ROOT_FORWARD",
    "ROOT_LEARN",
    "REROOTED",
    "ROOT_PORT",
    "DESIGNATED_PROOPOSE",
    "DESIGNATED_AGREED",
    "DESIGNATED_SYNCED",
    "DESIGNATED_RETIRED",
    "DESIGNATED_FORWARD",
    "DESIGNATED_LEARN",
    "DESIGNATED_DISCARD",
    "DESIGNATED_PORT",
    "ALTERNATE_PROPOSED",
    "ALTERNATE_AGREED",
    "BLOCK_PORT",
    "BACKUP_PORT",
    "ALTERNATE_PORT",

    "DISCARDING",
    "LEARNING",
    "FORWARDING",

    "INACTIVE",
    "LEARNING",
    "DETECTED",
    "NOTIFIED_TCN",
    "NOTIFIED_TC",
    "PROPAGATING",
    "ACKNOWLEDGED",
    "ACTIVE"
};

const static sys_text_t log_stp_portState[] =
{
    {         "Disabled"   },
    {         "Blocking"   },
    {         "Learning"   },
    {       "Forwarding" },
};
#define STP_SEM_LOCK(sem)   \
    do { } while (osal_sem_mutex_take(sem, OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)
#define STP_SEM_UNLOCK(sem) \
    do { } while (osal_sem_mutex_give(sem) != SYS_ERR_OK)
static int32 stp_portPathCost_calc(sys_logic_port_t port, mstp_u32_t *pPathCost)
{
    sys_port_type_t portType = PORT_TYPE_100M;
    sys_logic_port_t lPort;
    sys_logic_portmask_t lPortmask;
    sys_port_speedDuplex_t portSpeedDuplex;
    mstp_u32_t linkSpeed = 0;   

    if (!IS_LOGIC_PORTMASK_PORTSET(stp_pmLinkup, port))
    {
        portType = LP2TYPE(port);

        switch (portType)
        {
            case PORT_TYPE_100M:
                linkSpeed = 100;
                break;
            case PORT_TYPE_1000M:
            case PORT_TYPE_LAG:
                linkSpeed = 1000;
                break;
            case PORT_TYPE_10G:
                linkSpeed = 10000;
                break;
            default:
                break;
        }
    }
    else
    {
        LOGIC_PORTMASK_CLEAR_ALL(lPortmask);

        if (IS_TRKLP(port))
            LP2TRKACTMBRLPM(port, &lPortmask);
        else
            LOGIC_PORTMASK_SET_PORT(lPortmask, port);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lPort, lPortmask)
        {
            SYS_ERR_CONTINUE(rsd_port_speedDuplex_get(lPort, &portSpeedDuplex));

            switch (portSpeedDuplex.speed)
            {
                case PORT_SPEED_10M:
                    linkSpeed += 10;
                    break;
                case PORT_SPEED_100M:
                    linkSpeed += 100;
                    break;
                case PORT_SPEED_1000M:
                    linkSpeed += 1000;
                    break;
                case PORT_SPEED_10G:
                    linkSpeed += 10000;
                    break;
                default:
                    break;
            }
        }
    }

    if (STP_PATHCOST_METHOD_LONG == stp_pathcostMethod)
    {
        *pPathCost = (linkSpeed != 0) ? (20000000 / linkSpeed) : 0;
    }
    else
    {
        if (linkSpeed <= 10)
        {   
            *pPathCost = 100;
        }
        else if (linkSpeed <= 100)
        {   
            *pPathCost = 19;
        }
        else if (linkSpeed <= 1000)
        {   
            *pPathCost = 4;
        }
        else if (linkSpeed < 10000)
        {   
            *pPathCost = 3;
        }
        else if (linkSpeed == 10000)
        {   
            *pPathCost = 2;
        }
        else
        {   
            *pPathCost = 0;
        }
    }

    SYS_DBG(DBG_STP, "port %s linkSpeed = %d, pathCost = %d", LPSTR(port), linkSpeed, *pPathCost);

    return SYS_ERR_OK;
}

static int32 stp_portPathCost_update(sys_logic_port_t port)
{
    mstp_u32_t autoPathCost;
    mstp_u32_t i;

    SYS_ERR_CHK(stp_portPathCost_calc(port, &autoPathCost));

    if (mstpBridge.port[port].adminExtPathCost == 0) 
        mstpBridge.port[port].ExternalPortPathCost = autoPathCost;
    else 
        mstpBridge.port[port].ExternalPortPathCost = mstpBridge.port[port].adminExtPathCost;

    for (i = 0; i < MAX_MSTP_INSTANCE; i++)
    {
        if (mstpBridge.xst[i].xstPort[port].adminIntPathCost == 0) 
        {
            mstpBridge.xst[i].xstPort[port].InternalPortPathCost = autoPathCost;
            mstiBridgeParam_update(&mstpBridge, i);
        }
        else 
        {
            mstpBridge.xst[i].xstPort[port].InternalPortPathCost = mstpBridge.xst[i].xstPort[port].adminIntPathCost;
        }
    }

    return SYS_ERR_OK;
}

static int32 stp_portStatus_reset(sys_logic_port_t port)
{
    mstp_port_t *pPort = &mstpBridge.port[port];
    uint32 i;

    pPort->operEdge = (pPort->AutoEdge)? FALSE : pPort->AdminEdgePort;
    pPort->operPointToPointMAC = (pPort->autoPointToPointMAC)? FALSE : pPort->adminPointToPointMAC;

    for (i = 0; i < MAX_MSTP_INSTANCE; i++)
    {
        
        SYS_MEM_CLEAR(mstpBridge.xst[i].xstPort[port].portPriority);
    }

    SYS_ERR_CHK(stp_portPathCost_update(port));

    return SYS_ERR_OK;
}

static int32 mstpBridgePortStatus_update(mstp_bridge_t *pBridge, uint32 portIdx)
{
    mstp_u32_t xstIdx;

    SYS_DBG(DBG_STP, "mstpBridgePortStatus_update portIdx = %s", LPSTR(portIdx));

    if (!IS_LP_TRKMBR(portIdx))
    {
        if (IS_LOGIC_PORTMASK_PORTSET(stp_pmLinkup, portIdx))
        {
#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
            
            if (pBridge->port[portIdx].portEnabled == FALSE)
                pBridge->port[portIdx].loopbackDelayWhile = STP_SELFLOOP_DELAY_TIME;
#endif
            pBridge->port[portIdx].portEnabled = TRUE;
        }
        else
        {
            pBridge->port[portIdx].portEnabled = FALSE;
        }

        for (xstIdx=0; xstIdx<MAX_MSTP_INSTANCE; xstIdx++)
        {
            pBridge->xst[xstIdx].xstPort[portIdx].reselect = TRUE;
            pBridge->xst[xstIdx].xstPort[portIdx].selected = FALSE;
        }
    }

    return SYS_ERR_OK;
}

static int32 stp_portStatus_update(sys_logic_port_t port)
{
    sys_port_speedDuplex_t portSpeedDuplex;

    if (IS_LOGIC_PORTMASK_PORTSET(stp_pmLinkup, port))
    {
        stp_portPathCost_update(port);

        if (mstpBridge.port[port].autoPointToPointMAC)      
        {
            if (SYS_ERR_OK == rsd_port_speedDuplex_get(port, &portSpeedDuplex))
                mstpBridge.port[port].operPointToPointMAC = (PORT_FULL_DUPLEX == portSpeedDuplex.duplex)? TRUE : FALSE;
            else
                mstpBridge.port[port].operPointToPointMAC = FALSE;
        }
        else    
        {
            mstpBridge.port[port].operPointToPointMAC = mstpBridge.port[port].adminPointToPointMAC;
        }
    }
    else
    {
        
        stp_portStatus_reset(port);
    }

    mstpBridgePortStatus_update(&mstpBridge, port);

    return SYS_ERR_OK;
}

static int32 stp_linkEvent(uint32 event, char *pData)
{
    sys_port_event_t *pPortEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pPortEvent = (sys_port_event_t *)pData;

    SYS_DBG(DBG_STP, "link event port:%s up %u",  LPSTR(pPortEvent->lport), pPortEvent->new_linkStatus);

    if (PORT_LINKUP == pPortEvent->new_linkStatus)
    {
        LOGIC_PORTMASK_SET_PORT(stp_pmLinkup, pPortEvent->lport);
    }
    else
    {
        LOGIC_PORTMASK_CLEAR_PORT(stp_pmLinkup, pPortEvent->lport);
    }

    if (!IS_LP_TRKMBR(pPortEvent->lport))
    {
        STP_SEM_LOCK(stp_sem);
        stp_portStatus_update(pPortEvent->lport);
        STP_SEM_UNLOCK(stp_sem);
    }
    else
    {
        STP_SEM_LOCK(stp_sem);
        
        stp_portPathCost_update(LP2TRKLP(pPortEvent->lport));
        STP_SEM_UNLOCK(stp_sem);
    }

    return SYS_ERR_OK;
}

static int32 stp_trunkEvent_update(uint32 event, char *pData)
{
    uint32 i;
    sys_mac_t port_mac;
    sys_logic_port_t logicPort;
    sys_trunk_event_t *pTrunkEvent;
    sys_stp_state_t state = STP_STATE_FORWARDING;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pTrunkEvent = (sys_trunk_event_t *)pData;

    STP_SEM_LOCK(stp_sem);

    switch (event)
    {
        case SYS_NT_TRK_MBR_JOIN:
            
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, pTrunkEvent->portmask)
            {
                for (i = 0; i < MAX_MSTP_INSTANCE; i++)
                    stp_mstpState_set(i, logicPort, STP_STATE_FORWARDING);
            }

            logicPort = TRK2LP(pTrunkEvent->trunkIdx);

            for (i = 0; i < MAX_MSTP_INSTANCE; i++)
            {
                rsd_stp_mstpState_get(i, logicPort, &state);
                rsd_stp_mstpState_set(i, logicPort, state);
            }

            break;

        case SYS_NT_TRK_MBR_LEAVE:
            
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, pTrunkEvent->portmask)
            {
                
                for (i = 0; i < MAX_MSTP_INSTANCE; i++)
                    stp_mstpState_set(i, logicPort, mstpBridge.xst[i].xstPort[logicPort].portState);

                stp_portStatus_update(logicPort);
            }

            break;

        default:
            break;
    }

    logicPort = TRK2LP(pTrunkEvent->trunkIdx);

    if (SYS_ERR_OK == rsd_sys_portMacAddr_get(logicPort, &port_mac))
        osal_memcpy(mstpBridge.port[logicPort].ethAddrSrc, &port_mac, ETHER_ADDR_LEN);

    stp_portPathCost_update(TRK2LP(pTrunkEvent->trunkIdx));

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
#ifdef CONFIG_SYS_PROTO_ERPS
static int32 stp_erpsEvent_update(uint32 event, char *pData)
{
    sys_logic_port_t logicPort;
    sys_erps_event_t *pErpsEvent;
	mstp_u16_t				xstIdx = 0;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pErpsEvent = (sys_erps_event_t *)pData;

    STP_SEM_LOCK(stp_sem);

    switch (event)
    {
        case SYS_NT_ERPS_INST_ADD:	
			xstIdx = pErpsEvent->erps_inst;
			mstpBridge.xst[xstIdx].xstFlag |= (1 << SYS_INST_ERPS);
			break;
		case SYS_NT_ERPS_INST_DEL:
			xstIdx = pErpsEvent->erps_inst;
			mstpBridge.xst[xstIdx].xstFlag &= ~(1 << SYS_INST_ERPS);
			break;
		case SYS_NT_ERPS_PORT_ADD:
			xstIdx = pErpsEvent->erps_inst;
			mstpBridge.xst[xstIdx].xstFlag |= (1 << SYS_INST_ERPS);
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, pErpsEvent->portmask)
            {
                mstpBridge.xst[xstIdx].xstPort[logicPort].xstFlag |= (1 << SYS_INST_ERPS);
                
            }
            break;
		case SYS_NT_ERPS_PORT_DEL:
			xstIdx = pErpsEvent->erps_inst;
			mstpBridge.xst[xstIdx].xstFlag &= ~(1 << SYS_INST_ERPS);
            FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, pErpsEvent->portmask)
            {
                mstpBridge.xst[xstIdx].xstPort[logicPort].xstFlag &= ~(1 << SYS_INST_ERPS);
            }
            break;

        default:
            break;
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_LOOPBACK
static int32 stp_loopbackEvent_update(uint32 event, char *pData)
{
    sys_loopback_event_t *ploopbackEvent;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    ploopbackEvent = (sys_loopback_event_t *)pData;

    STP_SEM_LOCK(stp_sem);

    switch (event)
    {
		case SYS_NT_LOOPBACK_PORT_ADD:
			mstpBridge.xst[0].xstFlag |= (1 << SYS_INST_LOOPBACK);
            mstpBridge.xst[0].xstPort[ploopbackEvent->port].xstFlag |= (1 << SYS_INST_LOOPBACK);
            break;
		case SYS_NT_LOOPBACK_PORT_DEL:
			mstpBridge.xst[0].xstFlag &= ~(1 << SYS_INST_LOOPBACK);
            mstpBridge.xst[0].xstPort[ploopbackEvent->port].xstFlag &= ~(1 << SYS_INST_LOOPBACK);
            break;

        default:
            break;
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
#endif

static int32 stp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    uint16 etherType = 0;
    int32 ret = SYS_ERR_OK;
    sys_enable_t enable = DISABLED;
    sys_logic_port_t port = pPkt->rx_tag.source_port;
    sys_logic_portmask_t stp_pmForwardBpduPort;

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);

    SYS_DBG(DBG_STP, "stp rx port %s\n", LPSTR( port));

    etherType = *(uint16*) (pPkt->data + 12);
    if (0x88cc == etherType)
    {
        rsd_nic_pkt_free(pPkt);
        SYS_DBG(DBG_STP, "stp rx port %s packet ethertype is %x\n", LPSTR(port), etherType);

        return SYS_ERR_OK;
    }

    if (!IS_LOGIC_PORTMASK_PORTSET(stp_pmLinkup, port))
    {
        rsd_nic_pkt_free(pPkt);
        SYS_DBG(DBG_STP, "stp rx port %s is not linkup\n", LPSTR(port));

        return SYS_ERR_OK;
    }

    stp_enable_get(&enable);

    if (DISABLED == enable)
    {
        stp_forwardBpdu_get(&enable);

        if (DISABLED == enable)
        {
            rsd_nic_pkt_free(pPkt);
        }
        else
        {
            LOGIC_PORTMASK_SET_ALL(stp_pmForwardBpduPort);
            LOGIC_PORTMASK_CLEAR_PORT(stp_pmForwardBpduPort, port);
            SYS_ERR_CHK(rsd_nic_pkt_tx(pPkt, &stp_pmForwardBpduPort , SYS_CPU_PORT_PRIORITY_BPDU, 0, RSD_NIC_TX_CARE_TRUNK | RSD_NIC_TX_CARE_DOT1X));
        }

        return SYS_ERR_OK;
    }

    if (DISABLED == mstpBridge.port[port].adminEnable)
    {
        if (DISABLED == mstpBridge.port[port].forwardBpdu)
        {
            rsd_nic_pkt_free(pPkt);
        }
        else
        {
            
            osal_memcpy(&stp_pmForwardBpduPort, &stp_pmForwardingBPDU, sizeof(sys_logic_portmask_t));
            LOGIC_PORTMASK_CLEAR_PORT(stp_pmForwardBpduPort, port);
            SYS_ERR_CHK(rsd_nic_pkt_tx(pPkt, &stp_pmForwardBpduPort , SYS_CPU_PORT_PRIORITY_BPDU, 0, RSD_NIC_TX_CARE_TRUNK | RSD_NIC_TX_CARE_DOT1X));
        }

        return SYS_ERR_OK;
    }

#ifdef CONFIG_SYS_STP_BPDU_FILTER
    if (mstpBridge.port[port].adminBpduFilter == TRUE)
    {
        rsd_nic_pkt_free(pPkt);
        STP_LOG(LOG_STP_BPDUFILTER, LPSTR(port));

        return SYS_ERR_OK;
    }
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD
    if (mstpBridge.port[port].adminBpduGuard == TRUE)
    {
        rsd_nic_pkt_free(pPkt);

        enable = DISABLED;
        rsd_port_enable_get(port, &enable);

        if (ENABLED == enable)
        {
            STP_LOG(LOG_STP_BPDUGUARD, LPSTR(port));
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
            ret = rsd_port_errDisable_set(port, SYS_PORT_ERRDISABLE_BPDUGUARD);
#else
            ret = rsd_port_enable_set(port, DISABLED);
#endif
            if (SYS_ERR_OK != ret)
                SYS_DBG(DBG_STP, "BPDU Guard shutdown port %s failed (%d)", LPSTR(port), ret);

        }

        return SYS_ERR_OK;
    }
#endif

    if (mstpBridge.port[port].AdminEdgePort)
        STP_LOG(LOG_STP_EDGEPORT, LPSTR(port));

    STP_SEM_LOCK(stp_sem);
    ret = mstp_bridge_pkt_rx(&mstpBridge, port, pPkt->data, pPkt->length);
    STP_SEM_UNLOCK(stp_sem);

    rsd_nic_pkt_free(pPkt);

    switch (ret)
    {
        case SYS_ERR_STP_BPDU_SELFLOOP:
        {
            enable = DISABLED;
            rsd_port_enable_get(port, &enable);

            if (ENABLED == enable)
            {
                STP_LOG(LOG_STP_SELFLOOP, LPSTR(port),
                        mstpBridge.port[port].ethAddrSrc[0],
                        mstpBridge.port[port].ethAddrSrc[1],
                        mstpBridge.port[port].ethAddrSrc[2],
                        mstpBridge.port[port].ethAddrSrc[3],
                        mstpBridge.port[port].ethAddrSrc[4],
                        mstpBridge.port[port].ethAddrSrc[5]);
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
                ret = rsd_port_errDisable_set(port, SYS_PORT_ERRDISABLE_SELFLOOP);
#else
                ret = rsd_port_enable_set(port, DISABLED);
#endif
                if (SYS_ERR_OK != ret)
                    SYS_DBG(DBG_STP, "BPDU Self-Loop shutdown port %s failed (%d)", LPSTR(port), ret);
            }
            break;
        }

        default:
            break;
    }

    return SYS_ERR_OK;
}

static int32 stp_pkt_tx(uint32 portId, uint8 *pData, int32 len)
{
    sys_nic_pkt_t *pPkt;
    sys_logic_portmask_t logic_portmask;

    if(DISABLED == mstpBridge.port[portId].adminEnable)
        return SYS_ERR_OK;

    LOGIC_PORTMASK_CLEAR_ALL(logic_portmask);

    if (!IS_LOGIC_PORTMASK_PORTSET(stp_pmLinkup, portId))
    {
        SYS_DBG(DBG_STP, "stp tx port %u is not linkup\n", LPSTR(portId));
        return SYS_ERR_OK;
    }

    LOGIC_PORTMASK_SET_PORT(logic_portmask, portId);

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(len, &pPkt))
    {
        osal_memcpy(pPkt->data, pData, len);
        pPkt->tail += len;
        pPkt->length = len;

        SYS_ERR_CHK(rsd_nic_pkt_tx(pPkt, &logic_portmask, SYS_CPU_PORT_PRIORITY_BPDU, 0 ,RSD_NIC_TX_CARE_TRUNK | RSD_NIC_TX_CARE_DOT1X));
    }

    return SYS_ERR_OK;
}

static int32 stp_fdb_flush(uint32 instance, uint32 portId)
{
    sys_enable_t enStp;
    sys_enable_t enStpPort;
    int32 ret;

    SYS_ERR_CHK(stp_enable_get(&enStp));
    SYS_ERR_CHK(stp_portAdminEnable_get(portId, &enStpPort));
    if ((ENABLED == enStp) && (ENABLED == enStpPort))
    {
        ret = rsd_l2_mac_clearByPort(portId);

        SYS_DBG(DBG_STP, "instance: %d, port: %s, ret = %d", instance, LPSTR(portId), ret);
    }

    return SYS_ERR_OK;
}
static int32 stp_mstpState_set(uint32 msti, sys_logic_port_t port, sys_stp_state_t stp_state)
{
    uint32 portState;

    SYS_ERR_CHK(rsd_stp_mstpState_get(msti, port, &portState));

    if (portState != stp_state)
    {
        
        SYS_ERR_CHK(rsd_stp_mstpState_set(msti, port, stp_state));

        if (MSTP_BPDU_VERSION_MSTP == mstpBridge.ForceProtocolVersion)
        {
            
            STP_LOG(LOG_STP_MSTI_PORT_STATE,
                    LPSTR(port), msti,
                    log_stp_portState[portState].text,
                    log_stp_portState[stp_state].text);
        }
        else
        {
            
            if (MSTP_INSTANCE_CIST == msti)
            {
                STP_LOG(LOG_STP_PORT_STATE,
                        LPSTR(port),
                        log_stp_portState[portState].text,
                        log_stp_portState[stp_state].text);
            }
        }
    }

    return SYS_ERR_OK;
}

static int32 stp_portState_change(uint32 instance, sys_logic_port_t portId, sys_stp_state_t portState)
{
    uint32 i;
    #ifdef CONFIG_SYS_PROTO_ERPS
	mstp_xst_port_t *pXstPort;
	#endif

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(portId), SYS_ERR_PORT_ID);

    SYS_DBG(DBG_STP, "instance: %d, port: %s, new portState = %d", instance, LPSTR(portId), portState);

    if ((mstpBridge.ForceProtocolVersion != MSTP_BPDU_VERSION_MSTP) && (instance != MSTP_INSTANCE_CIST))
        return SYS_ERR_OK;

    if (!IS_LP_TRKMBR(portId))
    {
        if (mstpBridge.ForceProtocolVersion != MSTP_BPDU_VERSION_MSTP)
        {
            
            SYS_DBG(DBG_STP, "IDVPORT (stp/rstp) port:%s state: %d", LPSTR(portId), portState);
            for (i = 0; i < MAX_MSTP_INSTANCE; i++)
            {
            	#ifdef CONFIG_SYS_PROTO_ERPS
            	pXstPort = &mstpBridge.xst[i].xstPort[portId];
            	if ((mstpBridge.xst[i].xstFlag & (1 << SYS_INST_ERPS)) && (pXstPort->xstFlag & (1 << SYS_INST_ERPS)))
            	{
                	continue;
            	}
            	#endif
                SYS_ERR_CHK(stp_mstpState_set(i, portId, portState));
            }
        }
        else
        {
            SYS_DBG(DBG_STP, "IDVPORT (mstp) port:%s state: %d", LPSTR(portId), portState);
            SYS_ERR_CHK(stp_mstpState_set(instance, portId, portState));
        }
    }

    return SYS_ERR_OK;
}
static void stp_fsm_thread(void *pInput)
{
    
    for (;;)
    {
        
        STP_SEM_LOCK(stp_sem);
        mstp_bridge_fsm(&mstpBridge);
        STP_SEM_UNLOCK(stp_sem);

        osal_time_usleep(20 * 1000);    

        if (stp_fsm_thread_status)
        {
            break;
        }
    }

    stp_fsm_thread_status = -1;

    osal_thread_exit(0);
}

static void stp_routine_thread(void *pInput)
{
    
    for (;;)
    {
        
        STP_SEM_LOCK(stp_sem);
        mstp_bridge_routine(&mstpBridge);
        STP_SEM_UNLOCK(stp_sem);

        osal_time_usleep(250 * 1000);   

        if (stp_routine_thread_status)
        {
            break;
        }
    }

    stp_routine_thread_status = -1;

    osal_thread_exit(0);
}

static void stp_tick_thread(void *pInput)
{
    
    for (;;)
    {
        mstp_bridge_tick(&mstpBridge);

        osal_time_usleep(1 * 1000 * 1000);  

        if (stp_tick_thread_status)
        {
            break;
        }

    }

    stp_tick_thread_status = -1;

    osal_thread_exit(0);
}

static mstp_func_ops_t mstp_func_ops =
{
    .mstp_pkt_tx_cb_f               = stp_pkt_tx,
    .mstp_fdb_flush_cb_f            = stp_fdb_flush,
    .mstp_portState_change_cb_f     = stp_portState_change,
};
int32 stp_init(void)
{
    sys_mac_t macAddr;
    int32 i;

    if (MAX_MSTP_PORT_NUM < SYS_LOGIC_PORT_NUM)
    {
        SYS_PRINTF("%s():%d [FATAL]  MAX_MSTP_PORT_NUM < SYS_LOGIC_PORT_NUM !\n", __FUNCTION__, __LINE__);
        return SYS_ERR_FAILED;
    }

    stp_sem = osal_sem_mutex_create();
    if (0 == stp_sem)
    {
        osal_printf("%s():%d - semaphore create failed\n", __FUNCTION__, __LINE__);
        return SYS_ERR_FAILED;
    }

    SYS_ERR_CHK(rsd_nic_handler_register(&l2gHandler));

    SYS_ERR_CHK(rsd_sys_macAddr_get(0, &macAddr));
    osal_memcpy(mstpBridge.macAddr, macAddr.octet, ETHER_ADDR_LEN);
    osal_memset(mstVlan.msti, 0, sizeof(mstVlan.msti));
    osal_memset(mstVlan.vlanCount, 0, sizeof(mstVlan.vlanCount));
    mstVlan.vlanCount[MSTP_INSTANCE_CIST] = 4094;   
    VLANMASK_SET_ALL(mstVlan.vlanMask[MSTP_INSTANCE_CIST]);
    for (i = 1; i < MAX_MSTP_INSTANCE; i++)
    {
        VLANMASK_CLEAR_ALL(mstVlan.vlanMask[i]);
    }

    LOGIC_PORTMASK_CLEAR_ALL(stp_pmForwardingBPDU);

    LOGIC_PORTMASK_CLEAR_ALL(stp_pmLinkup);

    mstp_bridge_init(&mstpBridge, &mstp_func_ops);
    mstp_digest_calc(mstVlan.msti, mstpBridge.MstConfigId.digest);

    SYS_DBG(DBG_STP, "mstp port num %u", MAX_MSTP_PORT_NUM);

#if 0   

    osal_printf("%s():%d digest: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", \
        __FUNCTION__, __LINE__, \
        mstpBridge.MstConfigId.digest[ 0], mstpBridge.MstConfigId.digest[ 1], mstpBridge.MstConfigId.digest[ 2], mstpBridge.MstConfigId.digest[ 3], \
        mstpBridge.MstConfigId.digest[ 4], mstpBridge.MstConfigId.digest[ 5], mstpBridge.MstConfigId.digest[ 6], mstpBridge.MstConfigId.digest[ 7], \
        mstpBridge.MstConfigId.digest[ 8], mstpBridge.MstConfigId.digest[ 9], mstpBridge.MstConfigId.digest[10], mstpBridge.MstConfigId.digest[11], \
        mstpBridge.MstConfigId.digest[12], mstpBridge.MstConfigId.digest[13], mstpBridge.MstConfigId.digest[14], mstpBridge.MstConfigId.digest  [15]);

    for (i = 1; i < 4095; i++)
    {
        stp_mstVlanToInstanceMap[i] = 1;
    }
    mstp_digest_calc(stp_mstVlanToInstanceMap, mstpBridge.MstConfigId.digest);
    osal_printf("%s():%d digest: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", \
        __FUNCTION__, __LINE__, \
        mstpBridge.MstConfigId.digest[ 0], mstpBridge.MstConfigId.digest[ 1], mstpBridge.MstConfigId.digest[ 2], mstpBridge.MstConfigId.digest[ 3], \
        mstpBridge.MstConfigId.digest[ 4], mstpBridge.MstConfigId.digest[ 5], mstpBridge.MstConfigId.digest[ 6], mstpBridge.MstConfigId.digest[ 7], \
        mstpBridge.MstConfigId.digest[ 8], mstpBridge.MstConfigId.digest[ 9], mstpBridge.MstConfigId.digest[10], mstpBridge.MstConfigId.digest[11], \
        mstpBridge.MstConfigId.digest[12], mstpBridge.MstConfigId.digest[13], mstpBridge.MstConfigId.digest[14], mstpBridge.MstConfigId.digest  [15]);

    for (i = 1; i < 4095; i++)
    {
        stp_mstVlanToInstanceMap[i] = (i % 32) + 1;
    }
    mstp_digest_calc(stp_mstVlanToInstanceMap, mstpBridge.MstConfigId.digest);
    osal_printf("%s():%d digest: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n", \
        __FUNCTION__, __LINE__, \
        mstpBridge.MstConfigId.digest[ 0], mstpBridge.MstConfigId.digest[ 1], mstpBridge.MstConfigId.digest[ 2], mstpBridge.MstConfigId.digest[ 3], \
        mstpBridge.MstConfigId.digest[ 4], mstpBridge.MstConfigId.digest[ 5], mstpBridge.MstConfigId.digest[ 6], mstpBridge.MstConfigId.digest[ 7], \
        mstpBridge.MstConfigId.digest[ 8], mstpBridge.MstConfigId.digest[ 9], mstpBridge.MstConfigId.digest[10], mstpBridge.MstConfigId.digest[11], \
        mstpBridge.MstConfigId.digest[12], mstpBridge.MstConfigId.digest[13], mstpBridge.MstConfigId.digest[14], mstpBridge.MstConfigId.digest  [15]);
#endif

    if ((osal_thread_t)NULL == (osal_thread_create("MSTP FSM Thread", 32768, DFLT_THREAD_PRI_MSTP_FSM, (void *)stp_fsm_thread, NULL)))
    {
        SYS_PRINTF("MSTP FSM thread create failed\n");

        return SYS_ERR_FAILED;
    }

    if ((osal_thread_t)NULL == (osal_thread_create("MSTP Routine Thread", 32768, DFLT_THREAD_PRI_MSTP_ROUTINE, (void *)stp_routine_thread, NULL)))
    {
        SYS_PRINTF("MSTP Routine thread create failed\n");

        return SYS_ERR_FAILED;
    }

    if ((osal_thread_t)NULL == (osal_thread_create("MSTP Tick Thread", 32768, DFLT_THREAD_PRI_MSTP_TIMER, (void *)stp_tick_thread, NULL)))
    {
        SYS_PRINTF("MSTP Tick thread create failed\n");

        return SYS_ERR_FAILED;
    }

    for (i=1; i<MAX_MSTP_INSTANCE; i++)
    {
        SYS_ERR_CHK(rsd_stp_mstpInstance_create(i));
    }

    SYS_ERR_CHK(sys_notifier_observer_register("stp", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG, stp_linkEvent, SYS_NOTIFIER_PRI_HIGH));

    SYS_ERR_CHK(sys_notifier_observer_register("stp", SYS_NOTIFIER_SUBJECT_TRUNK,
            (SYS_NT_TRK_ACT_MBR_JOIN | SYS_NT_TRK_ACT_MBR_LEAVE | SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE),
            stp_trunkEvent_update, SYS_NOTIFIER_PRI_MEDIUM));

#ifdef CONFIG_SYS_PROTO_ERPS
    
    SYS_ERR_CHK(sys_notifier_observer_register("stp", SYS_NOTIFIER_SUBJECT_ERPS,
            (SYS_NT_ERPS_INST_ADD | SYS_NT_ERPS_INST_DEL | SYS_NT_ERPS_PORT_ADD | SYS_NT_ERPS_PORT_DEL),
            stp_erpsEvent_update, SYS_NOTIFIER_PRI_MEDIUM));
#endif

#ifdef CONFIG_SYS_PROTO_LOOPBACK
		
		SYS_ERR_CHK(sys_notifier_observer_register("stp", SYS_NOTIFIER_SUBJECT_LOOPBACK,
				(SYS_NT_LOOPBACK_PORT_ADD | SYS_NT_LOOPBACK_PORT_DEL),
				stp_loopbackEvent_update, SYS_NOTIFIER_PRI_MEDIUM));
#endif

    return SYS_ERR_OK;
}
int32 stp_exit(void)
{
    int32 i;

    stp_tick_thread_status = 1;
    do { } while (stp_tick_thread_status >= 0);

    stp_routine_thread_status = 1;
    do { } while (stp_routine_thread_status >= 0);

    stp_fsm_thread_status = 1;
    do { } while (stp_fsm_thread_status >= 0);

    SYS_ERR_CHK(rsd_nic_handler_unregister(&l2gHandler));

    SYS_ERR_CHK(sys_notifier_observer_unregister("stp", SYS_NOTIFIER_SUBJECT_PORT, stp_linkEvent));

    SYS_ERR_CHK(sys_notifier_observer_unregister("stp", SYS_NOTIFIER_SUBJECT_TRUNK, stp_trunkEvent_update));

    mstp_bridge_exit(&mstpBridge);

    osal_sem_mutex_destroy(stp_sem);

    for (i=1; i<MAX_MSTP_INSTANCE; i++)
    {
        SYS_ERR_CHK(rsd_stp_mstpInstance_destroy(i));
    }

    return SYS_ERR_OK;
}
static void cistBridgeParam_update(mstp_bridge_t *pBridge)
{   
    mstp_u32_t p;

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        pBridge->xst[MSTP_INSTANCE_CIST].xstPort[p].reselect = TRUE;
        pBridge->xst[MSTP_INSTANCE_CIST].xstPort[p].selected = FALSE;
    }
}

static void mstiBridgeParam_update(mstp_bridge_t *pBridge, mstp_u32_t xstIdx)
{   
    mstp_u32_t p;

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        if (pBridge->xst[xstIdx].xstPort[p].pPort->portEnabled == FALSE)
        {
            continue;
        }

        pBridge->xst[xstIdx].xstPort[p].reselect = TRUE;
        pBridge->xst[xstIdx].xstPort[p].selected = FALSE;
    }
}

static void cistPortParam_update(mstp_bridge_t *pBridge, mstp_u32_t portIdx)
{   
    pBridge->xst[MSTP_INSTANCE_CIST].xstPort[portIdx].reselect = TRUE;
    pBridge->xst[MSTP_INSTANCE_CIST].xstPort[portIdx].selected = FALSE;
}

static void mstiPortParam_update(mstp_bridge_t *pBridge, mstp_u32_t xstIdx, mstp_u32_t portIdx)
{   
    pBridge->xst[xstIdx].xstPort[portIdx].reselect = TRUE;
    pBridge->xst[xstIdx].xstPort[portIdx].selected = FALSE;
}
int32 stp_enable_set(sys_enable_t enable)
{
    mstp_bool_t oriBegin = mstpBridge.BEGIN;

    SYS_ERR_CHK(rsd_stp_enable_set(enable, stp_forwardBPDU));

    STP_SEM_LOCK(stp_sem);
    mstpBridge.BEGIN = (enable)? FALSE : TRUE;
    STP_SEM_UNLOCK(stp_sem);

    if (mstpBridge.BEGIN != oriBegin)
    {
        mstp_bridge_reset(&mstpBridge);
    }

    return SYS_ERR_OK;
}
int32 stp_enable_get(sys_enable_t *pEnable)
{
    *pEnable = (FALSE == mstpBridge.BEGIN)? ENABLED : DISABLED;

    return SYS_ERR_OK;
}
int32 stp_forwardBpdu_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    STP_SEM_LOCK(stp_sem);

    stp_forwardBPDU = enable;

    if (TRUE == mstpBridge.BEGIN)
        SYS_ERR_CHK(rsd_stp_enable_set(DISABLED, stp_forwardBPDU));

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_forwardBpdu_get(sys_enable_t *pEnable)
{
    *pEnable = stp_forwardBPDU;

    return SYS_ERR_OK;
}
int32 stp_forceVersion_set(uint32 forceVersion)
{
    mstp_u8_t oriVersion = mstpBridge.ForceProtocolVersion;
    uint32 i, portState;
    sys_logic_port_t logicPort;

    STP_SEM_LOCK(stp_sem);
    mstpBridge.ForceProtocolVersion = (mstp_u8_t)forceVersion;

    if (forceVersion == MSTP_BPDU_VERSION_MSTP)
    {   

        for (i=1; i<MAX_MSTP_INSTANCE; i++)
        {
            FOR_EACH_LOGIC_PORT(logicPort)
            {
                stp_mstpState_set(i, logicPort, mstpBridge.xst[i].xstPort[logicPort].portState);
            }
        }
    }
    else
    {   

        for (i=1; i<MAX_MSTP_INSTANCE; i++)
        {
            FOR_EACH_LOGIC_PORT(logicPort)
            {
                rsd_stp_mstpState_get(MSTP_INSTANCE_CIST, logicPort, &portState);
                stp_mstpState_set(i, logicPort, portState);
            }
        }
    }

    STP_SEM_UNLOCK(stp_sem);

    if (oriVersion != mstpBridge.ForceProtocolVersion)
    {
        mstp_bridge_reset(&mstpBridge);
    }

    return SYS_ERR_OK;
}
int32 stp_forceVersion_get(uint32 *pForceVersion)
{
    *pForceVersion = (uint32)mstpBridge.ForceProtocolVersion;

    return SYS_ERR_OK;
}
int32 stp_helloTime_set(uint32 helloTime)
{
    uint32 p;

    STP_SEM_LOCK(stp_sem);

    mstpBridge.helloTime = (mstp_u16_t)(helloTime << 8);

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        mstpBridge.xst[MSTP_INSTANCE_CIST].xstPort[p].portTimes.HelloTime = mstpBridge.helloTime;
    }

    if (mstpBridge.xst[MSTP_INSTANCE_CIST].rootPortId.number == 0)
    {
        cistBridgeParam_update(&mstpBridge);
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_helloTime_get(uint32 *pHelloTime)
{
    *pHelloTime = (uint32)(mstpBridge.helloTime >> 8);

    return SYS_ERR_OK;
}
int32 stp_maxHops_set(uint32 maxHops)
{
    uint32 instance;
    sys_logic_port_t port;

    STP_SEM_LOCK(stp_sem);

    mstpBridge.MaxHops = (mstp_u8_t)maxHops;

    for (instance = 0; instance < MAX_MSTP_INSTANCE; instance++)
    {
        if (mstpBridge.xst[instance].rootPortId.number == 0)
        {   
            mstpBridge.xst[instance].BridgeTimes.MaxHops = mstpBridge.MaxHops;
            for (port=0; port<MAX_MSTP_PORT_NUM; port++)
            {
                mstpBridge.xst[instance].xstPort[port].portTimes.remainingHops = mstpBridge.MaxHops;
            }
        }
    }

    if (mstpBridge.xst[MSTP_INSTANCE_CIST].rootPortId.number == 0)
    {
        cistBridgeParam_update(&mstpBridge);
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_maxHops_get(uint32 *pMaxHops)
{
    *pMaxHops = (uint32)mstpBridge.MaxHops;

    return SYS_ERR_OK;
}
int32 stp_forwardDelay_set(uint32 forwardDelay)
{
    STP_SEM_LOCK(stp_sem);

    mstpBridge.BridgeForwardDelay = (mstp_u16_t)(forwardDelay << 8);

    if (mstpBridge.xst[MSTP_INSTANCE_CIST].rootPortId.number == 0)
    {   
        mstpBridge.xst[MSTP_INSTANCE_CIST].BridgeTimes.ForwardDelay = mstpBridge.BridgeForwardDelay;
        mstiBridgeParam_update(&mstpBridge, MSTP_INSTANCE_CIST);
    }

    if (mstpBridge.xst[MSTP_INSTANCE_CIST].rootPortId.number == 0)
    {
        cistBridgeParam_update(&mstpBridge);
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_forwardDelay_get(uint32 *pForwardDelay)
{
    *pForwardDelay = (uint32)(mstpBridge.BridgeForwardDelay >> 8);

    return SYS_ERR_OK;
}
int32 stp_maximumAge_set(uint32 maximumAge)
{
    STP_SEM_LOCK(stp_sem);

    mstpBridge.BridgeMaxAge = (mstp_u16_t)(maximumAge << 8);

    mstiBridgeParam_update(&mstpBridge, MSTP_INSTANCE_CIST);

    if (mstpBridge.xst[MSTP_INSTANCE_CIST].rootPortId.number == 0)
    {
        cistBridgeParam_update(&mstpBridge);
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_maximumAge_get(uint32 *pMaximumAge)
{
    *pMaximumAge = (uint32)(mstpBridge.BridgeMaxAge >> 8);

    return SYS_ERR_OK;
}
int32 stp_txHoldCount_set(uint32 txHoldCount)
{
    sys_logic_port_t p;

    STP_SEM_LOCK(stp_sem);

    mstpBridge.TransmitHoldCount = (mstp_u8_t)(txHoldCount);

    for (p = 0; p < MAX_MSTP_PORT_NUM; p++)
    {
        mstpBridge.port[p].txCount = 0;
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_txHoldCount_get(uint32 *pTxHoldCount)
{
    *pTxHoldCount = (uint32)(mstpBridge.TransmitHoldCount);

    return SYS_ERR_OK;
}
int32 stp_pathcostMethod_set(uint32 method)
{
    sys_logic_port_t port;

    if (stp_pathcostMethod != method)
    {
        stp_pathcostMethod = method;

        FOR_EACH_LOGIC_PORT(port)
        {
            stp_portPathCost_update(port);
        }
    }

    return SYS_ERR_OK;
}
int32 stp_pathcostMethod_get(uint32 *pMethod)
{
    *pMethod = stp_pathcostMethod;

    return SYS_ERR_OK;
}
int32 stp_portAdminEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    STP_SEM_LOCK(stp_sem);

    pPort->adminEnable = enable;

    if (ENABLED == enable)
    {
        LOGIC_PORTMASK_CLEAR_PORT(stp_pmForwardingBPDU, port);
    }
    else
    {
        if (ENABLED == pPort->forwardBpdu)
            LOGIC_PORTMASK_SET_PORT(stp_pmForwardingBPDU, port);
    }
    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_portAdminEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    *pEnable = (uint32)pPort->adminEnable;

    return SYS_ERR_OK;
}
int32 stp_portForwardBpdu_set(sys_logic_port_t port, sys_enable_t enable)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    STP_SEM_LOCK(stp_sem);

    pPort->forwardBpdu = enable;

    if (DISABLED == enable)
    {
        LOGIC_PORTMASK_CLEAR_PORT(stp_pmForwardingBPDU, port);
    }
    else
    {
        if (DISABLED == pPort->adminEnable)
            LOGIC_PORTMASK_SET_PORT(stp_pmForwardingBPDU, port);
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_portForwardBpdu_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    *pEnable = (uint32)pPort->forwardBpdu;

    return SYS_ERR_OK;
}
int32 stp_portPathCost_set(sys_logic_port_t port, uint32 pathCost)
{
    mstp_port_t *pPort = &mstpBridge.port[port];
    mstp_u32_t autoPathCost;
    mstp_u32_t msti;

    SYS_ERR_CHK(stp_portPathCost_calc(port, &autoPathCost));

    STP_SEM_LOCK(stp_sem);

    pPort->adminExtPathCost = (mstp_u32_t)pathCost;

    if (pPort->adminExtPathCost == 0)
    {   
        pPort->ExternalPortPathCost = autoPathCost;
    }
    else
    {   
        pPort->ExternalPortPathCost = pPort->adminExtPathCost;
    }

    cistPortParam_update(&mstpBridge, port);

    for (msti=1; msti<MAX_MSTP_INSTANCE; msti++)
    {
        mstiPortParam_update(&mstpBridge, msti, port);
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_portPathCost_get(sys_logic_port_t port, uint32 *pPathCost)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    *pPathCost = (uint32)pPort->adminExtPathCost;

    return SYS_ERR_OK;
}
int32 stp_portEdgePort_set(sys_logic_port_t port, uint32 conf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    STP_SEM_LOCK(stp_sem);

    if (STP_PORT_CONF_AUTO == conf)
    {
        pPort->AutoEdge = TRUE;
        pPort->AdminEdgePort = FALSE;
    }
    else
    {
        pPort->AutoEdge = FALSE;
        pPort->AdminEdgePort = (STP_PORT_CONF_TRUE == conf)? TRUE: FALSE;
        pPort->operEdge = pPort->AdminEdgePort;
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_portEdgePort_get(sys_logic_port_t port, uint32 *pConf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    if (TRUE == pPort->AutoEdge)
    {
        *pConf = STP_PORT_CONF_AUTO;
    }
    else
    {
        *pConf = (TRUE == pPort->AdminEdgePort)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_STP_BPDU_FILTER

int32 stp_portBpduFilter_set(sys_logic_port_t port, uint32 conf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    STP_SEM_LOCK(stp_sem);

    pPort->adminBpduFilter = (STP_PORT_CONF_TRUE == conf)? TRUE : FALSE;

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_portBpduFilter_get(sys_logic_port_t port, uint32 *pConf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    *pConf = (pPort->adminBpduFilter)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;

    return SYS_ERR_OK;
}
#endif  

#ifdef CONFIG_SYS_STP_BPDU_GUARD

int32 stp_portBpduGuard_set(sys_logic_port_t port, uint32 conf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    STP_SEM_LOCK(stp_sem);

    pPort->adminBpduGuard = (STP_PORT_CONF_TRUE == conf)? TRUE : FALSE;

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_portBpduGuard_get(sys_logic_port_t port, uint32 *pConf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    *pConf = (pPort->adminBpduGuard)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;

    return SYS_ERR_OK;
}
#endif  
int32 stp_portPointToPointMac_set(sys_logic_port_t port, uint32 conf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];
    sys_port_speedDuplex_t portSpeedDuplex;

    STP_SEM_LOCK(stp_sem);

    if (STP_PORT_CONF_AUTO == conf)
    {
        pPort->autoPointToPointMAC = TRUE;

        if (SYS_ERR_OK == rsd_port_speedDuplex_get(port, &portSpeedDuplex))
            pPort->operPointToPointMAC = (PORT_FULL_DUPLEX == portSpeedDuplex.duplex)? TRUE : FALSE;
        else
            pPort->operPointToPointMAC = FALSE;
    }
    else
    {
        pPort->autoPointToPointMAC = FALSE;
        pPort->adminPointToPointMAC = (STP_PORT_CONF_TRUE == conf)? TRUE : FALSE;
        pPort->operPointToPointMAC = pPort->adminPointToPointMAC;
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_portPointToPointMac_get(sys_logic_port_t port, uint32 *pConf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    if (TRUE == pPort->autoPointToPointMAC)
    {
        *pConf = STP_PORT_CONF_AUTO;
    }
    else
    {
        *pConf = (TRUE == pPort->adminPointToPointMAC)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;
    }

    return SYS_ERR_OK;
}
int32 stp_portMcheck_set(sys_logic_port_t port, uint32 conf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    STP_SEM_LOCK(stp_sem);

    pPort->mcheck = (STP_PORT_CONF_TRUE == conf)? TRUE : FALSE;

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_portMcheck_get(sys_logic_port_t port, uint32 *pConf)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    *pConf = (pPort->mcheck)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;

    return SYS_ERR_OK;
}
int32 stp_mstConfigName_set(char *name)
{
    uint32 i;

    STP_SEM_LOCK(stp_sem);

    for (i = 0; i < sizeof(mstpBridge.MstConfigId.name); i++)
    {
        if (name[i] == '\0')
        {
            do {
               mstpBridge.MstConfigId.name[i++] = '\0';
            } while (i<sizeof(mstpBridge.MstConfigId.name));
            break;
        }
        mstpBridge.MstConfigId.name[i] = name[i];
    }

    for (i = 0; i < MAX_MSTP_INSTANCE; i++)
    {
        mstiBridgeParam_update(&mstpBridge, i);
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_mstDflt_init(sys_stp_mst_dflt_t dfltInfo)
{
    uint32 msti;
    sys_logic_port_t lp;

    for (msti = 0; msti < MSTP_INSTANCE_NUM; msti++)
    {
        SYS_ERR_CHK(stp_mstPriority_set(msti, dfltInfo.priority));

        FOR_EACH_LOGIC_PORT(lp)
        {
            SYS_ERR_CHK(stp_mstPortPathCost_set(msti, lp, dfltInfo.portPathCost));
            SYS_ERR_CHK(stp_mstPortPriority_set(msti, lp, dfltInfo.portPriority));
        }
    }

    return SYS_ERR_OK;
}
int32 stp_mstConfigName_get(char *name)
{
    osal_memcpy(name, mstpBridge.MstConfigId.name, sizeof(mstpBridge.MstConfigId.name));

    return SYS_ERR_OK;
}
int32 stp_mstConfigRevision_set(uint32 revision)
{
    uint32 i;

    STP_SEM_LOCK(stp_sem);

    mstpBridge.MstConfigId.revisionLevel = (mstp_u16_t)revision;

    for (i = 0; i < MAX_MSTP_INSTANCE; i++)
    {
        mstiBridgeParam_update(&mstpBridge, i);
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_mstConfigRevision_get(uint32 *pRevision)
{
    *pRevision = (uint32)mstpBridge.MstConfigId.revisionLevel;

    return SYS_ERR_OK;
}
int32 stp_mstVlanInstance_set(sys_vid_t vlanId, uint32 instance)
{
    uint16 vid = (uint16)(vlanId & 0xFFF);
    uint16 instanceId = (uint16)(instance & (MAX_MSTP_INSTANCE - 1));
    uint16 oldInstanceId;
    uint32 i;
	sys_logic_port_t logicPort;
	
    SYS_ERR_CHK(rsd_vlan_stg_set(vid, (sys_stg_t)instanceId));

    STP_SEM_LOCK(stp_sem);

    oldInstanceId = mstVlan.msti[vid] & (MAX_MSTP_INSTANCE - 1);
    VLANMASK_CLEAR_VID(mstVlan.vlanMask[oldInstanceId], vid);
    mstVlan.vlanCount[oldInstanceId] -= 1;
    if (mstVlan.vlanCount[oldInstanceId] == 0)
    {
        mstpBridge.xst[oldInstanceId].xstEnabled = FALSE;
    }

    mstVlan.msti[vid] = instanceId;
    VLANMASK_SET_VID(mstVlan.vlanMask[instanceId], vid);
    mstVlan.vlanCount[instanceId] += 1;
	
    if (mstVlan.vlanCount[instanceId] != 0)
    {
		FOR_EACH_LOGIC_PORT(logicPort)
		{
			stp_mstpState_set(oldInstanceId, logicPort, STP_STATE_FORWARDING);

			mstpBridge.xst[oldInstanceId].xstPort[logicPort].portStateChanged = TRUE;
			mstpBridge.xst[instanceId].xstPort[logicPort].portStateChanged = TRUE;
		}
		
        mstpBridge.xst[instanceId].xstEnabled = TRUE;
    }

    mstp_digest_calc(mstVlan.msti, mstpBridge.MstConfigId.digest);

    for (i =0; i < MAX_MSTP_INSTANCE; i++)
    {
        mstiBridgeParam_update(&mstpBridge, i);
    }

    STP_SEM_UNLOCK(stp_sem);
	
	mstp_bridge_reset(&mstpBridge);

    return SYS_ERR_OK;
}
int32 stp_mstVlanInstance_get(sys_vid_t vlanId, uint32 *pInstance)
{
    uint16 vid = (uint16)(vlanId & 0xFFF);

    *pInstance = (uint32)mstVlan.msti[vid];

    return SYS_ERR_OK;
}
int32 stp_mstVlanmask_set(uint32 instance, sys_vlanmask_t vlanmask)
{
    uint16 vid;
    uint16 instanceId = (uint16)(instance & (MAX_MSTP_INSTANCE - 1));
    uint16 oldInstanceId;
    uint32 vlanId;
    uint32 i;
	sys_logic_port_t logicPort;
	
    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        vid = (uint16)(vlanId & 0xFFF);

        SYS_ERR_CHK(rsd_vlan_stg_set(vid, (sys_stg_t)instanceId));

        STP_SEM_LOCK(stp_sem);

        oldInstanceId = mstVlan.msti[vid] & (MAX_MSTP_INSTANCE - 1);
        VLANMASK_CLEAR_VID(mstVlan.vlanMask[oldInstanceId], vid);
        mstVlan.vlanCount[oldInstanceId] -= 1;
        if (mstVlan.vlanCount[oldInstanceId] == 0)
        {
            mstpBridge.xst[oldInstanceId].xstEnabled = FALSE;
        }

        mstVlan.msti[vid] = instanceId;
        VLANMASK_SET_VID(mstVlan.vlanMask[instanceId], vid);
        mstVlan.vlanCount[instanceId] += 1;
		
        if (mstVlan.vlanCount[instanceId] != 0)
        {
            FOR_EACH_LOGIC_PORT(logicPort)
			{
				stp_mstpState_set(oldInstanceId, logicPort, STP_STATE_FORWARDING);

				mstpBridge.xst[oldInstanceId].xstPort[logicPort].portStateChanged = TRUE;
				mstpBridge.xst[instanceId].xstPort[logicPort].portStateChanged = TRUE;
			}

            mstpBridge.xst[instanceId].xstEnabled = TRUE;
        }

        STP_SEM_UNLOCK(stp_sem);

    }

    STP_SEM_LOCK(stp_sem);
    mstp_digest_calc(mstVlan.msti, mstpBridge.MstConfigId.digest);

    for (i = 0; i < MAX_MSTP_INSTANCE; i++)
    {
        mstiBridgeParam_update(&mstpBridge, i);
    }
    STP_SEM_UNLOCK(stp_sem);
	
	mstp_bridge_reset(&mstpBridge);

    return SYS_ERR_OK;
}

#if 0

int32 stp_mstVlanInstanceMap_set(sys_vid_t vlanId, uint32 instance)
{
    uint16 vid = (uint16)(vlanId & 0xFFF);
    uint16 instanceId = (uint16)(instance & (MAX_MSTP_INSTANCE - 1));
    uint16 oldInstanceId;

    SYS_ERR_CHK(rsd_vlan_stg_set(vid, (sys_stg_t)instanceId));

    STP_SEM_LOCK(stp_sem);

    oldInstanceId = mstVlan.msti[vid] & (MAX_MSTP_INSTANCE - 1);
    VLANMASK_CLEAR_VID(mstVlan.vlanMask[oldInstanceId], vid);
    mstVlan.vlanCount[oldInstanceId] -= 1;
    if (mstVlan.vlanCount[oldInstanceId] == 0)
    {
        mstpBridge.xst[oldInstanceId].xstEnabled = FALSE;
    }

    mstVlan.msti[vid] = instanceId;
    VLANMASK_SET_VID(mstVlan.vlanMask[instanceId], vid);
    mstVlan.vlanCount[instanceId] += 1;
    if (mstVlan.vlanCount[instanceId] != 0)
    {
        mstpBridge.xst[instanceId].xstEnabled = TRUE;
    }

    mstp_digest_calc(mstVlan.msti, mstpBridge.MstConfigId.digest);

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_mstVlanInstanceMap_get(sys_vid_t vlanId, uint32 *pInstance)
{
    uint16 vid = (uint16)(vlanId & 0xFFF);

    *pInstance = (uint32)mstVlan.msti[vid];

    return SYS_ERR_OK;
}
#endif
int32 stp_mstPriority_set(uint32 instance, uint32 priority)
{
    mstp_xst_t *pXst = &mstpBridge.xst[instance % MAX_MSTP_INSTANCE];

    STP_SEM_LOCK(stp_sem);

    pXst->BridgeIdentifierPriority &= MSTP_BRIDGE_EXTID_MASK;
    pXst->BridgeIdentifierPriority |= ((mstp_u16_t)priority) & MSTP_BRIDGE_PRIORITY_MASK;

    pXst->BridgeIdentifier.priority = pXst->BridgeIdentifierPriority;

    mstiBridgeParam_update(&mstpBridge, instance % MAX_MSTP_INSTANCE);

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_mstPriority_get(uint32 instance, uint32 *pPriority)
{
    mstp_xst_t *pXst = &mstpBridge.xst[instance];

    *pPriority = (uint32)pXst->BridgeIdentifierPriority;

    return SYS_ERR_OK;
}
int32 stp_mstPortPathCost_set(uint32 instance, sys_logic_port_t port, uint32 pathCost)
{
    mstp_xst_port_t *pXstPort = &mstpBridge.xst[instance % MAX_MSTP_INSTANCE].xstPort[port];
    mstp_u32_t autoPathCost;

    SYS_ERR_CHK(stp_portPathCost_calc(port, &autoPathCost));

    STP_SEM_LOCK(stp_sem);

    pXstPort->adminIntPathCost = (mstp_u32_t)pathCost;

    if (pXstPort->adminIntPathCost == 0)
    {   
        pXstPort->InternalPortPathCost = autoPathCost;
    }
    else
    {   
        pXstPort->InternalPortPathCost = pXstPort->adminIntPathCost;
    }

    mstiPortParam_update(&mstpBridge, instance % MAX_MSTP_INSTANCE, port);

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_mstPortPathCost_get(uint32 instance, sys_logic_port_t port, uint32 *pPathCost)
{
    mstp_xst_port_t *pXstPort = &mstpBridge.xst[instance].xstPort[port];

    *pPathCost = (uint32)pXstPort->adminIntPathCost;

    return SYS_ERR_OK;
}
int32 stp_mstPortPriority_set(uint32 instance, sys_logic_port_t port, uint32 priority)
{
    mstp_xst_port_t *pXstPort = &mstpBridge.xst[instance].xstPort[port];
    mstp_u32_t msti;

    STP_SEM_LOCK(stp_sem);

    pXstPort->portId.priority = (mstp_u8_t)priority;

    if (MSTP_INSTANCE_CIST == instance)
    {
        cistPortParam_update(&mstpBridge, port);

        for (msti=1; msti<MAX_MSTP_INSTANCE; msti++)
        {
            mstiPortParam_update(&mstpBridge, msti, port);
        }
    }
    else
    {
        mstiPortParam_update(&mstpBridge, instance % MAX_MSTP_INSTANCE, port);
    }

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_mstPortPriority_get(uint32 instance, sys_logic_port_t port, uint32 *pPriority)
{
    mstp_xst_port_t *pXstPort = &mstpBridge.xst[instance].xstPort[port];

    *pPriority = (uint32)pXstPort->portId.priority;

    return SYS_ERR_OK;
}
int32 stp_info_get(sys_stp_info_t *pInfo)
{
    pInfo->enable = (mstpBridge.BEGIN == FALSE)? Enabled : Disabled;
    pInfo->forceVersion = (uint8)mstpBridge.ForceProtocolVersion;
    pInfo->forwardBpdu = stp_forwardBPDU;
    osal_memcpy(pInfo->mstConfId.name, mstpBridge.MstConfigId.name, MSTP_CONF_NAME_LEN);
    pInfo->mstConfId.name[MSTP_CONF_NAME_LEN] = '\0';
    pInfo->mstConfId.revision = (uint16)mstpBridge.MstConfigId.revisionLevel;
    osal_memcpy(pInfo->mstConfId.digest, mstpBridge.MstConfigId.digest, \
        sizeof(pInfo->mstConfId.digest));

    osal_memcpy(pInfo->macAddr, mstpBridge.macAddr, ETHER_ADDR_LEN);

    pInfo->helloTime = (uint8)(mstpBridge.helloTime >> 8);
    pInfo->maxHops = (uint8)mstpBridge.MaxHops;
    pInfo->forwardDelay = (uint8)(mstpBridge.BridgeForwardDelay >> 8);
    pInfo->maximumAge = (uint8)(mstpBridge.BridgeMaxAge >> 8);
    pInfo->txHoldCount = (uint8)mstpBridge.TransmitHoldCount;
    pInfo->pathcostMethod = (uint8)stp_pathcostMethod;

    return SYS_ERR_OK;
}
int32 stp_portInfo_get(sys_logic_port_t port, sys_stp_port_info_t *pPortInfo)
{

    mstp_port_t *pPort = &mstpBridge.port[port];

    pPortInfo->portId.priority = pPort->pCistPort->portId.priority;
    pPortInfo->portId.number = pPort->pCistPort->portId.number;

    pPortInfo->adminEnable = pPort->adminEnable;
    pPortInfo->forwardBpdu = pPort->forwardBpdu;

    if (pPort->sendRSTP == TRUE)
    {
        pPortInfo->mode = (pPort->pBridge->ForceProtocolVersion >= MSTP_BPDU_VERSION_MSTP)? STP_PORT_MODE_MSTP : STP_PORT_MODE_RSTP;
    }
    else
    {
        pPortInfo->mode = STP_PORT_MODE_STP;
    }

    pPortInfo->type = (((pPort->pCistPort->infoIs != 0) && (pPortInfo->mode == STP_PORT_MODE_MSTP)) || (pPort->infoInternal == TRUE))? STP_PORT_TYPE_INTERNAL : STP_PORT_TYPE_BOUNDARY;

    pPortInfo->confExtPathCost = pPort->adminExtPathCost;
    pPortInfo->operExtPathCost = pPort->ExternalPortPathCost;

    pPortInfo->confEdge = (pPort->AutoEdge == TRUE)? STP_PORT_CONF_AUTO : \
        (pPort->AdminEdgePort == TRUE)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;
    pPortInfo->operEdge = (pPort->operEdge == TRUE)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;

#ifdef CONFIG_SYS_STP_BPDU_FILTER
    pPortInfo->confBpduFilter = (pPort->adminBpduFilter == TRUE)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD
    pPortInfo->confBpduGuard = (pPort->adminBpduGuard == TRUE)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;
#endif

    pPortInfo->confPointToPointMac = (pPort->autoPointToPointMAC == TRUE)? STP_PORT_CONF_AUTO : \
        (pPort->adminPointToPointMAC == TRUE)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;
    pPortInfo->operPointToPointMac = (pPort->operPointToPointMAC == TRUE)? STP_PORT_CONF_TRUE : STP_PORT_CONF_FALSE;

    pPortInfo->portTimers.mdelayWhile = pPort->mdelayWhile;
    pPortInfo->portTimers.helloWhen = pPort->helloWhen;
    pPortInfo->portTimers.edgeDelayWhile = pPort->edgeDelayWhile;
    pPortInfo->portTimers.txCount = pPort->txCount;

    pPortInfo->pktCount.rxConfig = pPort->cntRxConfig;
    pPortInfo->pktCount.rxTcn = pPort->cntRxTcn;
    pPortInfo->pktCount.rxMstp = pPort->cntRxMstp;
    pPortInfo->pktCount.txConfig = pPort->cntTxConfig;
    pPortInfo->pktCount.txTcn = pPort->cntTxTcn;
    pPortInfo->pktCount.txMstp = pPort->cntTxMstp;

    osal_memcpy(pPortInfo->strStatePTM, strState[mstpBridge.port[port].fsmPortTimers.state], STATE_STRING_LEN);
    osal_memcpy(pPortInfo->strStatePPM, strState[mstpBridge.port[port].fsmPortProtocolMigration.state], STATE_STRING_LEN);
    osal_memcpy(pPortInfo->strStatePRX, strState[mstpBridge.port[port].fsmPortReceive.state], STATE_STRING_LEN);
    osal_memcpy(pPortInfo->strStatePTX, strState[mstpBridge.port[port].fsmPortTransmit.state], STATE_STRING_LEN);
    osal_memcpy(pPortInfo->strStateBDT, strState[mstpBridge.port[port].fsmBridgeDetection.state], STATE_STRING_LEN);

    return SYS_ERR_OK;
}
int32 stp_portCounter_clear(sys_logic_port_t port)
{
    mstp_port_t *pPort = &mstpBridge.port[port];

    STP_SEM_LOCK(stp_sem);

    pPort->cntRxConfig = 0;
    pPort->cntRxTcn = 0;
    pPort->cntRxMstp = 0;
    pPort->cntTxConfig = 0;
    pPort->cntTxTcn = 0;
    pPort->cntTxMstp = 0;

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_mstInfo_get(uint32 instance, sys_stp_mst_info_t *pMstInfo)
{
    mstp_xst_t *pXst = (mstpBridge.ForceProtocolVersion == MSTP_BPDU_VERSION_MSTP)? &mstpBridge.xst[instance] : &mstpBridge.xst[MSTP_INSTANCE_CIST];

    pMstInfo->enable = ((mstpBridge.BEGIN == FALSE) && pXst->xstEnabled)? ENABLED : DISABLED;

    pMstInfo->bridgeId.priority = pXst->BridgeIdentifier.priority;
    pMstInfo->bridgeId.addr[0] = pXst->BridgeIdentifier.addr[0];
    pMstInfo->bridgeId.addr[1] = pXst->BridgeIdentifier.addr[1];
    pMstInfo->bridgeId.addr[2] = pXst->BridgeIdentifier.addr[2];
    pMstInfo->bridgeId.addr[3] = pXst->BridgeIdentifier.addr[3];
    pMstInfo->bridgeId.addr[4] = pXst->BridgeIdentifier.addr[4];
    pMstInfo->bridgeId.addr[5] = pXst->BridgeIdentifier.addr[5];

    pMstInfo->rootPriority.rootBridgeId.priority = pXst->rootPriority.RootID.priority;
    pMstInfo->rootPriority.rootBridgeId.addr[0] = pXst->rootPriority.RootID.addr[0];
    pMstInfo->rootPriority.rootBridgeId.addr[1] = pXst->rootPriority.RootID.addr[1];
    pMstInfo->rootPriority.rootBridgeId.addr[2] = pXst->rootPriority.RootID.addr[2];
    pMstInfo->rootPriority.rootBridgeId.addr[3] = pXst->rootPriority.RootID.addr[3];
    pMstInfo->rootPriority.rootBridgeId.addr[4] = pXst->rootPriority.RootID.addr[4];
    pMstInfo->rootPriority.rootBridgeId.addr[5] = pXst->rootPriority.RootID.addr[5];

    pMstInfo->rootPriority.externalRootPathCost = pXst->rootPriority.ExtRootPathCost;

    pMstInfo->rootPriority.regionalRootBridgeId.priority = pXst->rootPriority.RRootID.priority;
    pMstInfo->rootPriority.regionalRootBridgeId.addr[0] = pXst->rootPriority.RRootID.addr[0];
    pMstInfo->rootPriority.regionalRootBridgeId.addr[1] = pXst->rootPriority.RRootID.addr[1];
    pMstInfo->rootPriority.regionalRootBridgeId.addr[2] = pXst->rootPriority.RRootID.addr[2];
    pMstInfo->rootPriority.regionalRootBridgeId.addr[3] = pXst->rootPriority.RRootID.addr[3];
    pMstInfo->rootPriority.regionalRootBridgeId.addr[4] = pXst->rootPriority.RRootID.addr[4];
    pMstInfo->rootPriority.regionalRootBridgeId.addr[5] = pXst->rootPriority.RRootID.addr[5];

    pMstInfo->rootPriority.internalRootPathCost = pXst->rootPriority.IntRootPathCost;

    pMstInfo->rootPriority.designatedBridgeId.priority = pXst->rootPriority.DesignatedBridgeID.priority;
    pMstInfo->rootPriority.designatedBridgeId.addr[0] = pXst->rootPriority.DesignatedBridgeID.addr[0];
    pMstInfo->rootPriority.designatedBridgeId.addr[1] = pXst->rootPriority.DesignatedBridgeID.addr[1];
    pMstInfo->rootPriority.designatedBridgeId.addr[2] = pXst->rootPriority.DesignatedBridgeID.addr[2];
    pMstInfo->rootPriority.designatedBridgeId.addr[3] = pXst->rootPriority.DesignatedBridgeID.addr[3];
    pMstInfo->rootPriority.designatedBridgeId.addr[4] = pXst->rootPriority.DesignatedBridgeID.addr[4];
    pMstInfo->rootPriority.designatedBridgeId.addr[5] = pXst->rootPriority.DesignatedBridgeID.addr[5];

    pMstInfo->rootPortId.priority = pXst->rootPortId.priority;
    pMstInfo->rootPortId.number = pXst->rootPortId.number;

    pMstInfo->tcTimer = pXst->tcTimer;
    pMstInfo->tcTimes = pXst->tcTimes;

    pMstInfo->helloTime = pXst->rootTimes.HelloTime >> 8;
    pMstInfo->maxAge = pXst->rootTimes.MaxAge >> 8;
    pMstInfo->forwardDelay = pXst->rootTimes.ForwardDelay >> 8;
    pMstInfo->remainingHops = pXst->rootTimes.remainingHops;

    osal_memcpy(&pMstInfo->vlanmask, &mstVlan.vlanMask[instance], sizeof(pMstInfo->vlanmask));
    pMstInfo->vlanCount = mstVlan.vlanCount[instance];

    osal_memcpy(pMstInfo->strStatePRS, strState[pXst->fsmPortRoleSelection.state], STATE_STRING_LEN);

    return SYS_ERR_OK;
}
int32 stp_mstPortInfo_get(uint32 instance, sys_logic_port_t port, sys_stp_mst_port_info_t *pMstPortInfo)
{

    mstp_xst_port_t *pXstPort = (mstpBridge.ForceProtocolVersion == MSTP_BPDU_VERSION_MSTP)? &mstpBridge.xst[instance].xstPort[port] : &mstpBridge.xst[MSTP_INSTANCE_CIST].xstPort[port];

    STP_SEM_LOCK(stp_sem);

    pMstPortInfo->portId.priority = pXstPort->portId.priority;
    pMstPortInfo->portId.number = pXstPort->portId.number;

    pMstPortInfo->confIntPathCost = pXstPort->adminIntPathCost;
    pMstPortInfo->operIntPathCost = pXstPort->InternalPortPathCost;

    pMstPortInfo->portPriority.rootBridgeId.priority = pXstPort->portPriority.RootID.priority;
    pMstPortInfo->portPriority.rootBridgeId.addr[0] = pXstPort->portPriority.RootID.addr[0];
    pMstPortInfo->portPriority.rootBridgeId.addr[1] = pXstPort->portPriority.RootID.addr[1];
    pMstPortInfo->portPriority.rootBridgeId.addr[2] = pXstPort->portPriority.RootID.addr[2];
    pMstPortInfo->portPriority.rootBridgeId.addr[3] = pXstPort->portPriority.RootID.addr[3];
    pMstPortInfo->portPriority.rootBridgeId.addr[4] = pXstPort->portPriority.RootID.addr[4];
    pMstPortInfo->portPriority.rootBridgeId.addr[5] = pXstPort->portPriority.RootID.addr[5];
    pMstPortInfo->portPriority.externalRootPathCost = pXstPort->portPriority.ExtRootPathCost;
    pMstPortInfo->portPriority.regionalRootBridgeId.priority = pXstPort->portPriority.RRootID.priority;
    pMstPortInfo->portPriority.regionalRootBridgeId.addr[0] = pXstPort->portPriority.RRootID.addr[0];
    pMstPortInfo->portPriority.regionalRootBridgeId.addr[1] = pXstPort->portPriority.RRootID.addr[1];
    pMstPortInfo->portPriority.regionalRootBridgeId.addr[2] = pXstPort->portPriority.RRootID.addr[2];
    pMstPortInfo->portPriority.regionalRootBridgeId.addr[3] = pXstPort->portPriority.RRootID.addr[3];
    pMstPortInfo->portPriority.regionalRootBridgeId.addr[4] = pXstPort->portPriority.RRootID.addr[4];
    pMstPortInfo->portPriority.regionalRootBridgeId.addr[5] = pXstPort->portPriority.RRootID.addr[5];
    pMstPortInfo->portPriority.internalRootPathCost = pXstPort->portPriority.IntRootPathCost;
    pMstPortInfo->portPriority.designatedBridgeId.priority = pXstPort->portPriority.DesignatedBridgeID.priority;
    pMstPortInfo->portPriority.designatedBridgeId.addr[0] = pXstPort->portPriority.DesignatedBridgeID.addr[0];
    pMstPortInfo->portPriority.designatedBridgeId.addr[1] = pXstPort->portPriority.DesignatedBridgeID.addr[1];
    pMstPortInfo->portPriority.designatedBridgeId.addr[2] = pXstPort->portPriority.DesignatedBridgeID.addr[2];
    pMstPortInfo->portPriority.designatedBridgeId.addr[3] = pXstPort->portPriority.DesignatedBridgeID.addr[3];
    pMstPortInfo->portPriority.designatedBridgeId.addr[4] = pXstPort->portPriority.DesignatedBridgeID.addr[4];
    pMstPortInfo->portPriority.designatedBridgeId.addr[5] = pXstPort->portPriority.DesignatedBridgeID.addr[5];
    pMstPortInfo->portPriority.designatedPortId.priority = pXstPort->portPriority.DesignatedPortID.priority;
    pMstPortInfo->portPriority.designatedPortId.number = pXstPort->portPriority.DesignatedPortID.number;

    pMstPortInfo->portTimes.remainingHops = pXstPort->portTimes.remainingHops;

    pMstPortInfo->portRole = (pXstPort->portState != MSTP_PORT_STATE_DISABLED)? pXstPort->selectedRole : 0;
    pMstPortInfo->portState = pXstPort->portState;

    pMstPortInfo->portTimers.tcWhile = pXstPort->tcWhile;
    pMstPortInfo->portTimers.fdWhile = pXstPort->fdWhile;
    pMstPortInfo->portTimers.rcvdInfoWhile = pXstPort->rcvdInfoWhile;
    pMstPortInfo->portTimers.rrWhile = pXstPort->rrWhile;
    pMstPortInfo->portTimers.rbWhile = pXstPort->rbWhile;

    osal_memcpy(pMstPortInfo->strStatePIF, strState[pXstPort->fsmPortInformation.state], STATE_STRING_LEN);
    osal_memcpy(pMstPortInfo->strStatePRT, strState[pXstPort->fsmPortRoleTransitions.state], STATE_STRING_LEN);
    osal_memcpy(pMstPortInfo->strStatePST, strState[pXstPort->fsmPortStateTransition.state], STATE_STRING_LEN);
    osal_memcpy(pMstPortInfo->strStateTCG, strState[pXstPort->fsmTopologyChange.state], STATE_STRING_LEN);

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}
int32 stp_portMsg_get(sys_logic_port_t port, sys_stp_portMsg_t *pPortMsg)
{
    return SYS_ERR_OK;
}
int32 stp_mstMsg_get(uint32 instance, sys_stp_mstMsg_t *pMstMsg)
{
    return SYS_ERR_OK;
}
int32 stp_mstPortMsg_get(uint32 instance, sys_logic_port_t port, sys_stp_mstPortMsg_t *pMstPortMsg)
{
    mstp_xst_port_t *pXstPort = &mstpBridge.xst[instance].xstPort[port];

    STP_SEM_LOCK(stp_sem);

    pMstPortMsg->rcvdInternal = pXstPort->pPort->rcvdInternal;

    pMstPortMsg->designatedPriority.rootBridgeId.priority = pXstPort->designatedPriority.RootID.priority;
    pMstPortMsg->designatedPriority.rootBridgeId.addr[0] = pXstPort->designatedPriority.RootID.addr[0];
    pMstPortMsg->designatedPriority.rootBridgeId.addr[1] = pXstPort->designatedPriority.RootID.addr[1];
    pMstPortMsg->designatedPriority.rootBridgeId.addr[2] = pXstPort->designatedPriority.RootID.addr[2];
    pMstPortMsg->designatedPriority.rootBridgeId.addr[3] = pXstPort->designatedPriority.RootID.addr[3];
    pMstPortMsg->designatedPriority.rootBridgeId.addr[4] = pXstPort->designatedPriority.RootID.addr[4];
    pMstPortMsg->designatedPriority.rootBridgeId.addr[5] = pXstPort->designatedPriority.RootID.addr[5];
    pMstPortMsg->designatedPriority.extRootPathCost = pXstPort->designatedPriority.ExtRootPathCost;
    pMstPortMsg->designatedPriority.regionalRootBridgeId.priority = pXstPort->designatedPriority.RRootID.priority;
    pMstPortMsg->designatedPriority.regionalRootBridgeId.addr[0] = pXstPort->designatedPriority.RRootID.addr[0];
    pMstPortMsg->designatedPriority.regionalRootBridgeId.addr[1] = pXstPort->designatedPriority.RRootID.addr[1];
    pMstPortMsg->designatedPriority.regionalRootBridgeId.addr[2] = pXstPort->designatedPriority.RRootID.addr[2];
    pMstPortMsg->designatedPriority.regionalRootBridgeId.addr[3] = pXstPort->designatedPriority.RRootID.addr[3];
    pMstPortMsg->designatedPriority.regionalRootBridgeId.addr[4] = pXstPort->designatedPriority.RRootID.addr[4];
    pMstPortMsg->designatedPriority.regionalRootBridgeId.addr[5] = pXstPort->designatedPriority.RRootID.addr[5];
    pMstPortMsg->designatedPriority.intRootPathCost = pXstPort->designatedPriority.IntRootPathCost;
    pMstPortMsg->designatedPriority.designatedBridgeId.priority = pXstPort->designatedPriority.DesignatedBridgeID.priority;
    pMstPortMsg->designatedPriority.designatedBridgeId.addr[0] = pXstPort->designatedPriority.DesignatedBridgeID.addr[0];
    pMstPortMsg->designatedPriority.designatedBridgeId.addr[1] = pXstPort->designatedPriority.DesignatedBridgeID.addr[1];
    pMstPortMsg->designatedPriority.designatedBridgeId.addr[2] = pXstPort->designatedPriority.DesignatedBridgeID.addr[2];
    pMstPortMsg->designatedPriority.designatedBridgeId.addr[3] = pXstPort->designatedPriority.DesignatedBridgeID.addr[3];
    pMstPortMsg->designatedPriority.designatedBridgeId.addr[4] = pXstPort->designatedPriority.DesignatedBridgeID.addr[4];
    pMstPortMsg->designatedPriority.designatedBridgeId.addr[5] = pXstPort->designatedPriority.DesignatedBridgeID.addr[5];
    pMstPortMsg->designatedPriority.designatedPortId.priority = pXstPort->designatedPriority.DesignatedPortID.priority;
    pMstPortMsg->designatedPriority.designatedPortId.number = pXstPort->designatedPriority.DesignatedPortID.number;
    pMstPortMsg->designatedPriority.rcvPortId.priority = pXstPort->designatedPriority.RcvPortID.priority;
    pMstPortMsg->designatedPriority.rcvPortId.number = pXstPort->designatedPriority.RcvPortID.number;

    pMstPortMsg->msgPriority.rootBridgeId.priority = pXstPort->msgPriority.RootID.priority;
    pMstPortMsg->msgPriority.rootBridgeId.addr[0] = pXstPort->msgPriority.RootID.addr[0];
    pMstPortMsg->msgPriority.rootBridgeId.addr[1] = pXstPort->msgPriority.RootID.addr[1];
    pMstPortMsg->msgPriority.rootBridgeId.addr[2] = pXstPort->msgPriority.RootID.addr[2];
    pMstPortMsg->msgPriority.rootBridgeId.addr[3] = pXstPort->msgPriority.RootID.addr[3];
    pMstPortMsg->msgPriority.rootBridgeId.addr[4] = pXstPort->msgPriority.RootID.addr[4];
    pMstPortMsg->msgPriority.rootBridgeId.addr[5] = pXstPort->msgPriority.RootID.addr[5];
    pMstPortMsg->msgPriority.extRootPathCost = pXstPort->msgPriority.ExtRootPathCost;
    pMstPortMsg->msgPriority.regionalRootBridgeId.priority = pXstPort->msgPriority.RRootID.priority;
    pMstPortMsg->msgPriority.regionalRootBridgeId.addr[0] = pXstPort->msgPriority.RRootID.addr[0];
    pMstPortMsg->msgPriority.regionalRootBridgeId.addr[1] = pXstPort->msgPriority.RRootID.addr[1];
    pMstPortMsg->msgPriority.regionalRootBridgeId.addr[2] = pXstPort->msgPriority.RRootID.addr[2];
    pMstPortMsg->msgPriority.regionalRootBridgeId.addr[3] = pXstPort->msgPriority.RRootID.addr[3];
    pMstPortMsg->msgPriority.regionalRootBridgeId.addr[4] = pXstPort->msgPriority.RRootID.addr[4];
    pMstPortMsg->msgPriority.regionalRootBridgeId.addr[5] = pXstPort->msgPriority.RRootID.addr[5];
    pMstPortMsg->msgPriority.intRootPathCost = pXstPort->msgPriority.IntRootPathCost;
    pMstPortMsg->msgPriority.designatedBridgeId.priority = pXstPort->msgPriority.DesignatedBridgeID.priority;
    pMstPortMsg->msgPriority.designatedBridgeId.addr[0] = pXstPort->msgPriority.DesignatedBridgeID.addr[0];
    pMstPortMsg->msgPriority.designatedBridgeId.addr[1] = pXstPort->msgPriority.DesignatedBridgeID.addr[1];
    pMstPortMsg->msgPriority.designatedBridgeId.addr[2] = pXstPort->msgPriority.DesignatedBridgeID.addr[2];
    pMstPortMsg->msgPriority.designatedBridgeId.addr[3] = pXstPort->msgPriority.DesignatedBridgeID.addr[3];
    pMstPortMsg->msgPriority.designatedBridgeId.addr[4] = pXstPort->msgPriority.DesignatedBridgeID.addr[4];
    pMstPortMsg->msgPriority.designatedBridgeId.addr[5] = pXstPort->msgPriority.DesignatedBridgeID.addr[5];
    pMstPortMsg->msgPriority.designatedPortId.priority = pXstPort->msgPriority.DesignatedPortID.priority;
    pMstPortMsg->msgPriority.designatedPortId.number = pXstPort->msgPriority.DesignatedPortID.number;
    pMstPortMsg->msgPriority.rcvPortId.priority = pXstPort->msgPriority.RcvPortID.priority;
    pMstPortMsg->msgPriority.rcvPortId.number = pXstPort->msgPriority.RcvPortID.number;

    pMstPortMsg->portPriority.rootBridgeId.priority = pXstPort->portPriority.RootID.priority;
    pMstPortMsg->portPriority.rootBridgeId.addr[0] = pXstPort->portPriority.RootID.addr[0];
    pMstPortMsg->portPriority.rootBridgeId.addr[1] = pXstPort->portPriority.RootID.addr[1];
    pMstPortMsg->portPriority.rootBridgeId.addr[2] = pXstPort->portPriority.RootID.addr[2];
    pMstPortMsg->portPriority.rootBridgeId.addr[3] = pXstPort->portPriority.RootID.addr[3];
    pMstPortMsg->portPriority.rootBridgeId.addr[4] = pXstPort->portPriority.RootID.addr[4];
    pMstPortMsg->portPriority.rootBridgeId.addr[5] = pXstPort->portPriority.RootID.addr[5];
    pMstPortMsg->portPriority.extRootPathCost = pXstPort->portPriority.ExtRootPathCost;
    pMstPortMsg->portPriority.regionalRootBridgeId.priority = pXstPort->portPriority.RRootID.priority;
    pMstPortMsg->portPriority.regionalRootBridgeId.addr[0] = pXstPort->portPriority.RRootID.addr[0];
    pMstPortMsg->portPriority.regionalRootBridgeId.addr[1] = pXstPort->portPriority.RRootID.addr[1];
    pMstPortMsg->portPriority.regionalRootBridgeId.addr[2] = pXstPort->portPriority.RRootID.addr[2];
    pMstPortMsg->portPriority.regionalRootBridgeId.addr[3] = pXstPort->portPriority.RRootID.addr[3];
    pMstPortMsg->portPriority.regionalRootBridgeId.addr[4] = pXstPort->portPriority.RRootID.addr[4];
    pMstPortMsg->portPriority.regionalRootBridgeId.addr[5] = pXstPort->portPriority.RRootID.addr[5];
    pMstPortMsg->portPriority.intRootPathCost = pXstPort->portPriority.IntRootPathCost;
    pMstPortMsg->portPriority.designatedBridgeId.priority = pXstPort->portPriority.DesignatedBridgeID.priority;
    pMstPortMsg->portPriority.designatedBridgeId.addr[0] = pXstPort->portPriority.DesignatedBridgeID.addr[0];
    pMstPortMsg->portPriority.designatedBridgeId.addr[1] = pXstPort->portPriority.DesignatedBridgeID.addr[1];
    pMstPortMsg->portPriority.designatedBridgeId.addr[2] = pXstPort->portPriority.DesignatedBridgeID.addr[2];
    pMstPortMsg->portPriority.designatedBridgeId.addr[3] = pXstPort->portPriority.DesignatedBridgeID.addr[3];
    pMstPortMsg->portPriority.designatedBridgeId.addr[4] = pXstPort->portPriority.DesignatedBridgeID.addr[4];
    pMstPortMsg->portPriority.designatedBridgeId.addr[5] = pXstPort->portPriority.DesignatedBridgeID.addr[5];
    pMstPortMsg->portPriority.designatedPortId.priority = pXstPort->portPriority.DesignatedPortID.priority;
    pMstPortMsg->portPriority.designatedPortId.number = pXstPort->portPriority.DesignatedPortID.number;
    pMstPortMsg->portPriority.rcvPortId.priority = pXstPort->portPriority.RcvPortID.priority;
    pMstPortMsg->portPriority.rcvPortId.number = pXstPort->portPriority.RcvPortID.number;

    STP_SEM_UNLOCK(stp_sem);

    return SYS_ERR_OK;
}

