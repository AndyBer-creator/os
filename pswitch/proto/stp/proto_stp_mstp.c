#include <rsd/rsd_sys.h>
#include <stp/proto_stp_mstp.h>
#include <common/sys_def.h>
#ifdef CONFIG_SYS_PROTO_ERPS
#include <rsd/rsd_erps.h>
#endif
enum mstp_fsm_state_e
{
    
    STATE_ONE_SECOND = 0,
    STATE_TICK,

    STATE_DISCARD,
    STATE_RECEIVE,

    STATE_CHECKING_RSTP,
    STATE_SELECTING_STP,
    STATE_SENSING,

    STATE_EDGE,
    STATE_NOT_EDGE,

    STATE_TRANSMIT_INIT,
    STATE_TRANSMIT_PERIODIC,
    STATE_TRANSMIT_CONFIG,
    STATE_TRANSMIT_TCN,
    STATE_TRANSMIT_RSTP,
    STATE_IDLE,

    STATE_DISABLED,
    STATE_AGED,
    STATE_UPDATE,
    STATE_SUPERIOR_DESIGNATED,
    STATE_REPEATED_DESIGNATED,
    STATE_INFERIOR_DESIGNATED,
    STATE_NOT_DESIGNATED,
    STATE_OTHER,
    STATE_CURRENT,
    
    STATE_INIT_TREE,
    STATE_ROLE_SELECTION,

    STATE_INIT_PORT,
    STATE_DISABLE_PORT,
    STATE_DISABLED_PORT,
    STATE_MASTER_PROPOSED,
    STATE_MASTER_AGREED,
    STATE_MASTER_SYNCED,
    STATE_MASTER_RETIRED,
    STATE_MASTER_FORWARD,
    STATE_MASTER_LEARN,
    STATE_MASTER_DISCARD,
    STATE_MASTER_PORT,
    STATE_ROOT_PROPOSED,
    STATE_ROOT_AGREED,
    STATE_ROOT_SYNCED,
    STATE_REROOT,
    STATE_ROOT_FORWARD,
    STATE_ROOT_LEARN,
    STATE_REROOTED,
    STATE_ROOT_PORT,
    STATE_DESIGNATED_PROPOSE,
    STATE_DESIGNATED_AGREED,
    STATE_DESIGNATED_SYNCED,
    STATE_DESIGNATED_RETIRED,
    STATE_DESIGNATED_FORWARD,
    STATE_DESIGNATED_LEARN,
    STATE_DESIGNATED_DISCARD,
    STATE_DESIGNATED_PORT,
    STATE_ALTERNATE_PROPOSED,
    STATE_ALTERNATE_AGREED,
    STATE_BLOCK_PORT,
    STATE_BACKUP_PORT,
    STATE_ALTERNATE_PORT,

    STATE_DISCARDING,
    STATE_LEARNING,
    STATE_FORWARDING,

    STATE_INACTIVE,
    
    STATE_DETECTED,
    STATE_NOTIFIED_TCN,
    STATE_NOTIFIED_TC,
    STATE_PROPAGATING,
    STATE_ACKNOWLEDGED,
    STATE_ACTIVE,

    END_MSTP_FSM_STATE  
};
enum mstp_info_e
{
    INFO_Received = 0,
    INFO_Mine,
    INFO_Aged,
    INFO_Disabled,

    INFO_SuperiorDesignatedInfo,
    INFO_RepeatedDesignatedInfo,
    INFO_InferiorDesignatedInfo,
    INFO_InferiorRootAlternateInfo,
    INFO_OtherInfo,
};
enum mstp_port_role_e
{
    ROLE_DisabledPort = 0,
    ROLE_MasterPort,
    ROLE_RootPort,
    ROLE_DesignatedPort,
    ROLE_AlternatePort,
    ROLE_BackupPort,
};
static mstp_u8_t ethAddrBpdu[ETHER_ADDR_LEN] = { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x00 };
#define MSTP_FSM_CHANGE_STATE(f, s) \
    do { (f)->state = s; (f)->changeState = TRUE; } while(0);

#define MSTP_ETH_ADDR_COPY(dst,src) \
    do { \
        dst[0] = src[0]; \
        dst[1] = src[1]; \
        dst[2] = src[2]; \
        dst[3] = src[3]; \
        dst[4] = src[4]; \
        dst[5] = src[5]; \
        } while (0)
#if 0
static void PriorityVectorPrint(const mstp_u8_t *pStr, const mstp_pri_vctr_t * pPV)
{
    OSAL_PRINTF("--- PriorityVectorPrint [%s] ---------------------------------- \n", pStr);
    OSAL_PRINTF("  RootID: %04X / %02X:%02X:%02X:%02X:%02X:%02X \n", \
        pPV->RootID.priority, \
        pPV->RootID.addr[0], pPV->RootID.addr[1], pPV->RootID.addr[2], \
        pPV->RootID.addr[3], pPV->RootID.addr[4], pPV->RootID.addr[5]);
    OSAL_PRINTF("  ExtRPC: %d \n", pPV->ExtRootPathCost);
    OSAL_PRINTF(" RRootID: %04X / %02X:%02X:%02X:%02X:%02X:%02X \n", \
        pPV->RRootID.priority, \
        pPV->RRootID.addr[0], pPV->RRootID.addr[1], pPV->RRootID.addr[2], \
        pPV->RRootID.addr[3], pPV->RRootID.addr[4], pPV->RRootID.addr[5]);
    OSAL_PRINTF("  IntRPC: %d \n", pPV->IntRootPathCost);
    OSAL_PRINTF("  DesBID: %04X / %02X:%02X:%02X:%02X:%02X:%02X \n", \
        pPV->DesignatedBridgeID.priority, \
        pPV->DesignatedBridgeID.addr[0], pPV->DesignatedBridgeID.addr[1], pPV->DesignatedBridgeID.addr[2], \
        pPV->DesignatedBridgeID.addr[3], pPV->DesignatedBridgeID.addr[4], pPV->DesignatedBridgeID.addr[5]);
    OSAL_PRINTF("  DesPID: %02X / %02X \n", pPV->DesignatedPortID.priority, pPV->DesignatedPortID.number);
    OSAL_PRINTF("  RcvPID: %02X / %02X \n", pPV->RcvPortID.priority, pPV->RcvPortID.number);
    OSAL_PRINTF("============================================================================== \n");
}
#endif

static inline mstp_i32_t cmpBridgeId(const mstp_bridge_id_t *pBID1, const mstp_bridge_id_t *pBID2)
{
    if (pBID1->priority < pBID2->priority)
        return (1);
    else if (pBID1->priority > pBID2->priority)
        return (-1);
    else if (pBID1->addr[0] < pBID2->addr[0])
        return (2);
    else if (pBID1->addr[0] > pBID2->addr[0])
        return (-2);
    else if (pBID1->addr[1] < pBID2->addr[1])
        return (3);
    else if (pBID1->addr[1] > pBID2->addr[1])
        return (-3);
    else if (pBID1->addr[2] < pBID2->addr[2])
        return (4);
    else if (pBID1->addr[2] > pBID2->addr[2])
        return (-4);
    else if (pBID1->addr[3] < pBID2->addr[3])
        return (5);
    else if (pBID1->addr[3] > pBID2->addr[3])
        return (-5);
    else if (pBID1->addr[4] < pBID2->addr[4])
        return (6);
    else if (pBID1->addr[4] > pBID2->addr[4])
        return (-6);
    else if (pBID1->addr[5] < pBID2->addr[5])
        return (7);
    else if (pBID1->addr[5] > pBID2->addr[5])
        return (-7);
    else
        return (0);
}

static inline mstp_i32_t cmpPortId(const mstp_port_id_t *pPID1, const mstp_port_id_t *pPID2)
{
    if (pPID1->priority < pPID2->priority)
        return (1);
    else if (pPID1->priority > pPID2->priority)
        return (-1);
    else if (pPID1->number < pPID2->number)
        return (2);
    else if (pPID1->number > pPID2->number)
        return (-2);
    else
        return (0);
}

static inline mstp_i32_t cmpMstConfId(const mstp_mst_conf_id_t *pCID1, const mstp_mst_conf_id_t *pCID2)
{
    uint32 i;

    if (pCID1->idFormatSelector != pCID2->idFormatSelector)
        return 1;
    for (i=0; i<MSTP_CONF_NAME_LEN; i++)
    {
        if (pCID1->name[i] != pCID2->name[i])
        {
            return 2;
        }
    }
    if (pCID1->revisionLevel != pCID2->revisionLevel)
    {
        return 3;
    }
    for (i=0; i<MSTP_CONF_DIGEST_LEN; i++)
    {
        if (pCID1->digest[i] != pCID2->digest[i])
        {
            return 4;
        }
    }

    return 0;
}
static mstp_i32_t PriorityVectorCmp(const mstp_pri_vctr_t *pPV1, const mstp_pri_vctr_t *pPV2);
static mstp_i32_t BridgeIdCmp(const mstp_bridge_id_t *pBID1, const mstp_bridge_id_t *pBID2);
static mstp_i32_t TimesCmp(const mstp_times_t *pT1, const mstp_times_t *pT2);
static mstp_i32_t cmpCistPriorityMessagePort(const mstp_pri_vctr_t *pM, const mstp_pri_vctr_t *pP);

static mstp_i32_t cmpMstiPriorityMessagePort(const mstp_pri_vctr_t *pM, const mstp_pri_vctr_t *pP);
static mstp_bool_t AdminEdge(mstp_port_t *pPort);
static mstp_bool_t AutoEdge(mstp_port_t *pPort);
static mstp_u8_t EdgeDelay(mstp_xst_port_t *pXstPort);
static mstp_u8_t forwardDelay(mstp_port_t *pPort);
static mstp_u8_t MigrateTime(mstp_bridge_t *pBridge);
static mstp_u8_t PortRole_Encode(mstp_port_role_t portRole);
static mstp_bool_t reRooted(mstp_xst_port_t *pXstPort);
static mstp_bool_t rstpVersion(mstp_bridge_t *pBridge);

static mstp_u8_t TxHoldCount(mstp_port_t *pPort);
static mstp_bool_t allSynced(mstp_xst_port_t *pXstPort);
static mstp_u8_t FwdDelay(mstp_port_t *pPort);
static mstp_u8_t HelloTime(mstp_port_t *pPort);
static mstp_u8_t MaxAge(mstp_port_t *pPort);
static mstp_bool_t allTransmitReady(mstp_port_t *pPort);
static mstp_bool_t cist(mstp_xst_port_t *pXstPort);
static mstp_bool_t cistRootPort(mstp_port_t *pPort);
static mstp_bool_t cistDesignatedPort(mstp_port_t *pPort);
static mstp_bool_t mstiDesignatedOrTCpropagatingRootPort(mstp_port_t *pPort);
static mstp_bool_t mstiMasterPort(mstp_port_t *pPort);
static mstp_bool_t rcvdAnyMsg(mstp_port_t *pPort);
static mstp_bool_t rcvdCistMsg(mstp_xst_port_t *pXstPort);
static mstp_bool_t rcvdMstiMsg(mstp_xst_port_t *pXstPort);
static mstp_bool_t rcvdXstMsg(mstp_xst_port_t *pXstPort);
static mstp_bool_t restrictedRole(mstp_port_t *pPort);
static mstp_bool_t restrictedTcn(mstp_xst_port_t *pXstPort);
static mstp_bool_t updtCistInfo(mstp_xst_port_t *pXstPort);
static mstp_bool_t updtMstiInfo(mstp_xst_port_t *pXstPort);
static mstp_bool_t updtXstInfo(mstp_xst_port_t *pXstPort);
static void newInfoXst(mstp_xst_port_t *pXstPort, mstp_bool_t bool);
static void mstp_fsm_port_timers(mstp_fsm_t *pFSM, mstp_port_t *pPort);
static void mstp_fsm_port_receive(mstp_fsm_t *pFSM, mstp_port_t *pPort);
static void mstp_fsm_port_protocol_migration(mstp_fsm_t *pFSM, mstp_port_t *pPort);
static void mstp_fsm_bridge_detection(mstp_fsm_t *pFSM, mstp_port_t *pPort);
static void mstp_fsm_port_transmit(mstp_fsm_t *pFSM, mstp_port_t *pPort);
static void mstp_fsm_port_information(mstp_fsm_t *pFSM, mstp_xst_port_t *pXstPort);
static void mstp_fsm_port_role_selection(mstp_fsm_t *pFSM, mstp_xst_t *pXst);
static void mstp_fsm_port_role_transitions(mstp_fsm_t *pFSM, mstp_xst_port_t *pXstPort);
static void mstp_fsm_port_state_transition(mstp_fsm_t *pFSM, mstp_xst_port_t *pXstPort);
static void mstp_fsm_topology_change(mstp_fsm_t *pFSM, mstp_xst_port_t *pXstPort);
static mstp_i32_t PriorityVectorCmp(const mstp_pri_vctr_t *pPV1, const mstp_pri_vctr_t *pPV2)
{
    mstp_u32_t i;

    if (pPV1->RootID.priority < pPV2->RootID.priority)
    {
        return (1);
    }
    else if (pPV1->RootID.priority > pPV2->RootID.priority)
    {
        return (-1);
    }

    for (i=0; i<ETHER_ADDR_LEN; i++)
    {
        if (pPV1->RootID.addr[i] < pPV2->RootID.addr[i])
        {
            return (2);
        }
        else if (pPV1->RootID.addr[i] > pPV2->RootID.addr[i])
        {
            return (-2);
        }
    }

    if (pPV1->ExtRootPathCost < pPV2->ExtRootPathCost)
    {
        return (3);
    }
    else if (pPV1->ExtRootPathCost > pPV2->ExtRootPathCost)
    {
        return (-3);
    }

    if (pPV1->RRootID.priority < pPV2->RRootID.priority)
    {
        return (4);
    }
    else if (pPV1->RRootID.priority > pPV2->RRootID.priority)
    {
        return (-4);
    }

    for (i=0; i<ETHER_ADDR_LEN; i++)
    {
        if (pPV1->RRootID.addr[i] < pPV2->RRootID.addr[i])
        {
            return (5);
        }
        else if (pPV1->RRootID.addr[i] > pPV2->RRootID.addr[i])
        {
            return (-5);
        }
    }

    if (pPV1->IntRootPathCost < pPV2->IntRootPathCost)
    {
        return (6);
    }
    else if (pPV1->IntRootPathCost > pPV2->IntRootPathCost)
    {
        return (-6);
    }

    if (pPV1->DesignatedBridgeID.priority < pPV2->DesignatedBridgeID.priority)
    {
        return (7);
    }
    else if (pPV1->DesignatedBridgeID.priority > pPV2->DesignatedBridgeID.priority)
    {
        return (-7);
    }

    for (i=0; i<ETHER_ADDR_LEN; i++)
    {
        if (pPV1->DesignatedBridgeID.addr[i] < pPV2->DesignatedBridgeID.addr[i])
        {
            return (8);
        }
        else if (pPV1->DesignatedBridgeID.addr[i] > pPV2->DesignatedBridgeID.addr[i])
        {
            return (-8);
        }
    }

    if (pPV1->DesignatedPortID.priority < pPV2->DesignatedPortID.priority)
    {
        return (9);
    }
    else if (pPV1->DesignatedPortID.priority > pPV2->DesignatedPortID.priority)
    {
        return (-9);
    }

    if (pPV1->DesignatedPortID.number < pPV2->DesignatedPortID.number)
    {
        return (10);
    }
    else if (pPV1->DesignatedPortID.number > pPV2->DesignatedPortID.number)
    {
        return (-10);
    }

    if (pPV1->RcvPortID.priority < pPV2->RcvPortID.priority)
    {
        return (11);
    }
    else if (pPV1->RcvPortID.priority > pPV2->RcvPortID.priority)
    {
        return (-11);
    }

    if (pPV1->RcvPortID.number < pPV2->RcvPortID.number)
    {
        return (12);
    }
    else if (pPV1->RcvPortID.number > pPV2->RcvPortID.number)
    {
        return (-12);
    }

    return (0);
}
static mstp_i32_t BridgeIdCmp(const mstp_bridge_id_t *pBID1, const mstp_bridge_id_t *pBID2)
{
    mstp_u32_t i;

    if (pBID1->priority < pBID2->priority)
    {
        return (1);
    }
    else if (pBID1->priority > pBID2->priority)
    {
        return (-1);
    }

    for (i=0; i<ETHER_ADDR_LEN; i++)
    {
        if (pBID1->addr[i] < pBID2->addr[i])
        {
            return (2);
        }
        else if (pBID1->addr[i] > pBID2->addr[i])
        {
            return (-2);
        }
    }

    return (0);
}

static mstp_i32_t TimesCmp(const mstp_times_t *pT1, const mstp_times_t *pT2)
{
#define MSTP_CMP_TIME_RET(pt1,pt2,time,ret) \
    do { \
        if ((pt1)->time > (pt2)->time) \
            return (ret); \
        else if ((pt1)->time < (pt2)->time) \
            return (-ret); \
        } while (0);

    MSTP_CMP_TIME_RET(pT1, pT2, MessageAge, 1);
    MSTP_CMP_TIME_RET(pT1, pT2, MaxAge, 2);
    MSTP_CMP_TIME_RET(pT1, pT2, ForwardDelay, 3);
    MSTP_CMP_TIME_RET(pT1, pT2, HelloTime, 4);
    MSTP_CMP_TIME_RET(pT1, pT2, remainingHops, 5);

    return (0);
}
static mstp_i32_t cmpCistPriorityMessagePort(const mstp_pri_vctr_t *pM, const mstp_pri_vctr_t *pP)
{
    mstp_i32_t cmpResult;

    cmpResult = cmpBridgeId(&pM->RootID, &pP->RootID);
    if (cmpResult > 0)
        return (1);
    else if (cmpResult < 0)
        return (-1);

    if (pM->ExtRootPathCost < pP->ExtRootPathCost)
        return (2);
    else if (pM->ExtRootPathCost > pP->ExtRootPathCost)
        return (-2);

    cmpResult = cmpBridgeId(&pM->RRootID, &pP->RRootID);
    if (cmpResult > 0)
        return (3);
    else if (cmpResult < 0)
        return (-3);

    if (pM->IntRootPathCost < pP->IntRootPathCost)
        return (4);
    else if (pM->IntRootPathCost > pP->IntRootPathCost)
        return (-4);

    cmpResult = cmpBridgeId(&pM->DesignatedBridgeID, &pP->DesignatedBridgeID);
    if (cmpResult > 0)
        return (5);
    else if (cmpResult < 0)
        return (-5);

    cmpResult = cmpPortId(&pM->DesignatedPortID, &pP->DesignatedPortID);
    if (cmpResult > 0)
        return (6);
    else if (cmpResult < 0)
        return (-6);

    cmpResult = cmpPortId(&pM->RcvPortID, &pP->RcvPortID);
    if (cmpResult > 0)
        return (7);
    else if (cmpResult < 0)
        return (-7);

    return (0); 
}
static mstp_i32_t cmpMstiPriorityMessagePort(const mstp_pri_vctr_t *pM, const mstp_pri_vctr_t *pP)
{
    mstp_i32_t cmpResult;

    cmpResult = cmpBridgeId(&pM->RRootID, &pP->RRootID);
    if (cmpResult > 0)
        return (3);
    else if (cmpResult < 0)
        return (-3);

    if (pM->IntRootPathCost < pP->IntRootPathCost)
        return (4);
    else if (pM->IntRootPathCost > pP->IntRootPathCost)
        return (-4);

    cmpResult = cmpBridgeId(&pM->DesignatedBridgeID, &pP->DesignatedBridgeID);
    if (cmpResult > 0)
        return (5);
    else if (cmpResult < 0)
        return (-5);

    cmpResult = cmpPortId(&pM->DesignatedPortID, &pP->DesignatedPortID);
    if (cmpResult > 0)
        return (6);
    else if (cmpResult < 0)
        return (-6);

    return (0); 
}
static mstp_bool_t AdminEdge(mstp_port_t *pPort)
{   
    return (pPort->AdminEdgePort);
}
static mstp_bool_t AutoEdge(mstp_port_t *pPort)
{   
    return (pPort->AutoEdge);
}
static mstp_u8_t EdgeDelay(mstp_xst_port_t *pXstPort)
{   
    if (pXstPort->pPort->operPointToPointMAC == TRUE)
    {
        return MigrateTime(pXstPort->pBridge);
    }

    return MaxAge(pXstPort->pPort);
}
static mstp_u8_t forwardDelay(mstp_port_t *pPort)
{   
    if (pPort->sendRSTP == TRUE)
    {
        return HelloTime(pPort);
    }

    return FwdDelay(pPort);
}
static mstp_u8_t MigrateTime(mstp_bridge_t *pBridge)
{   
    mstp_u8_t migrateTime = (mstp_u8_t)(pBridge->MigrateTime >> 8);

    return (migrateTime);
}
static mstp_u8_t PortRole_Encode(mstp_port_role_t portRole)
{
    switch (portRole)
    {
        case ROLE_DisabledPort:
        case ROLE_MasterPort:
        default:
            return (0);

        case ROLE_AlternatePort:
        case ROLE_BackupPort:
            return (1);

        case ROLE_RootPort:
            return (2);

        case ROLE_DesignatedPort:
            return (3);
    }
}

static mstp_bool_t reRooted(mstp_xst_port_t *pXstPort)
{   
    mstp_i32_t i;

    for (i=0; i<MAX_MSTP_PORT_NUM; i++)
    {
        if (pXstPort->pXst->xstPort[i].pPort->portEnabled == FALSE)
        {
            continue;
        }

        if (i != pXstPort->portIdx)
        {
            if (pXstPort->pXst->xstPort[i].rrWhile == 0)
            {
                continue;
            }
            else
            {
                return (FALSE);
            }
        }
    }

    return (TRUE);
}
static mstp_bool_t rstpVersion(mstp_bridge_t *pBridge)
{   
    return ((pBridge->ForceProtocolVersion >= MSTP_BPDU_VERSION_RSTP)? TRUE : FALSE);
}
#if 0
static mstp_bool_t stpVersion(mstp_bridge_t *pBridge)
{   
    return ((pBridge->ForceProtocolVersion < MSTP_BPDU_VERSION_RSTP)? TRUE : FALSE);
}
#endif
static mstp_u8_t TxHoldCount(mstp_port_t *pPort)
{   
    
    return (pPort->pBridge->TransmitHoldCount);
}
static mstp_bool_t allSynced(mstp_xst_port_t *pXstPort)
{   
    mstp_i32_t p;
    mstp_xst_port_t *pXstPortTmp;

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        pXstPortTmp = &pXstPort->pXst->xstPort[p];

        if (pXstPortTmp->pPort->portEnabled == FALSE)
        {
            continue;
        }

        if ((pXstPortTmp->selected == TRUE) && \
            (pXstPortTmp->role == pXstPortTmp->selectedRole) && \
            (pXstPortTmp->updtInfo == FALSE))
        {
            continue;
        }
        else
        {
            return (FALSE);
        }
    }

    switch (pXstPort->role)
    {
    
    case ROLE_RootPort:
    case ROLE_AlternatePort:
        {
            for (p=0; p<MAX_MSTP_PORT_NUM; p++)
            {
                if (pXstPort->pXst->xstPort[p].pPort->portEnabled == FALSE)
                {
                    continue;
                }

                if ((pXstPort->pXst->rootPortId.number == 0) || \
                    (p != pXstPort->pXst->rootPortIdx)) 
                {
                    if (pXstPort->pXst->xstPort[p].synced == TRUE)
                    {
                        continue;
                    }
                    else
                    {
                        return (FALSE);
                    }
                }
            }
        }
        break;

    case ROLE_DesignatedPort:
    
    case ROLE_MasterPort:
        {
            for (p=0; p<MAX_MSTP_PORT_NUM; p++)
            {
                if (pXstPort->pXst->xstPort[p].pPort->portEnabled == FALSE)
                {
                    continue;
                }

                if (p != pXstPort->portIdx) 
                {
                    if (pXstPort->pXst->xstPort[p].synced == TRUE)
                    {
                        continue;
                    }
                    else
                    {
                        return (FALSE);
                    }
                }
            }
        }
        break;

    default:
        {
            return (FALSE);
        }
    }

    return (TRUE);
}
static mstp_u8_t FwdDelay(mstp_port_t *pPort)
{   
    mstp_u8_t fwdDelay = (mstp_u8_t)(pPort->pCistPort->designatedTimes.ForwardDelay >> 8);

    return (fwdDelay);
}
static mstp_u8_t HelloTime(mstp_port_t *pPort)
{   
    mstp_u8_t helloTime = (mstp_u8_t)(pPort->pCistPort->portTimes.HelloTime >> 8);

    return (helloTime);
}
static mstp_u8_t MaxAge(mstp_port_t *pPort)
{   
    mstp_u8_t maxAge = (mstp_u8_t)(pPort->pCistPort->designatedTimes.MaxAge >> 8);

    return (maxAge);
}
static mstp_bool_t allTransmitReady(mstp_port_t *pPort)
{   
    mstp_i32_t i = 0;

    for (i=0; i<MAX_MSTP_INSTANCE; i++)
    {
        if (pPort->pBridge->xst[i].xstEnabled == FALSE)
        {
            continue;
        }

        if ((pPort->pXstPort[i]->selected == TRUE) && \
            (pPort->pXstPort[i]->updtInfo == FALSE))
        {
            continue;
        }
        else
        {
            return (FALSE);
        }
    }

    return (TRUE);
}
static mstp_bool_t cist(mstp_xst_port_t *pXstPort)
{   
    return ((pXstPort->pCist == pXstPort->pXst)? TRUE : FALSE);
}
static mstp_bool_t cistRootPort(mstp_port_t *pPort)
{   
    return ((pPort->pCistPort->role == ROLE_RootPort)? TRUE : FALSE);
}
static mstp_bool_t cistDesignatedPort(mstp_port_t *pPort)
{   
    return ((pPort->pCistPort->role == ROLE_DesignatedPort)? TRUE : FALSE);
}
static mstp_bool_t mstiDesignatedOrTCpropagatingRootPort(mstp_port_t *pPort)
{   
    mstp_i32_t i;

    for (i=1; i<MAX_MSTP_INSTANCE; i++)
    {
        if (pPort->pBridge->xst[i].xstEnabled == FALSE)
        {
            continue;
        }

        if ((pPort->pXstPort[i]->role == ROLE_DesignatedPort) || \
            ((pPort->pXstPort[i]->role == ROLE_RootPort) && \
             (pPort->pXstPort[i]->tcWhile != 0)))
        {
            return (TRUE);
        }
    }

    return (FALSE);
}
static mstp_bool_t mstiMasterPort(mstp_port_t *pPort)
{   
    mstp_i32_t i;

    for (i=1; i<MAX_MSTP_INSTANCE; i++)
    {
        if (pPort->pBridge->xst[i].xstEnabled == FALSE)
        {
            continue;
        }

        if (pPort->pXstPort[i]->role == ROLE_MasterPort)
        {
            return (TRUE);
        }
    }

    return (FALSE);
}
static mstp_bool_t rcvdAnyMsg(mstp_port_t *pPort)
{   
    mstp_i32_t i;

    for (i=0; i<MAX_MSTP_INSTANCE; i++)
    {
        if (pPort->pBridge->xst[i].xstEnabled == FALSE)
        {
            continue;
        }

        if (pPort->pXstPort[i]->rcvdMsg == TRUE)
        {
            return (TRUE);
        }
    }

    return (FALSE);
}
static mstp_bool_t rcvdCistMsg(mstp_xst_port_t *pXstPort)
{   
    return (pXstPort->pCistPort->rcvdMsg);
}
static mstp_bool_t rcvdMstiMsg(mstp_xst_port_t *pXstPort)
{   
    if ((pXstPort->pCistPort->rcvdMsg == FALSE) && \
        (pXstPort->rcvdMsg == TRUE))
    {
        return (TRUE);
    }

    return (FALSE);
}
static mstp_bool_t rcvdXstMsg(mstp_xst_port_t *pXstPort)
{
    
    if (cist(pXstPort) == TRUE)
    {
        return rcvdCistMsg(pXstPort);
    }

    return rcvdMstiMsg(pXstPort);
}
static mstp_bool_t restrictedRole(mstp_port_t *pPort)
{   
    return (pPort->restrictedRole);
}
static mstp_bool_t restrictedTcn(mstp_xst_port_t *pXstPort)
{   
    return (pXstPort->pPort->restrictedTcn);
}
static mstp_bool_t updtCistInfo(mstp_xst_port_t *pXstPort)
{   
    return (pXstPort->pCistPort->updtInfo);
}
static mstp_bool_t updtMstiInfo(mstp_xst_port_t *pXstPort)
{   
    if ((pXstPort->updtInfo == TRUE) || \
        (pXstPort->pCistPort->updtInfo == TRUE))
    {
        return (TRUE);
    }

    return (FALSE);
}
static mstp_bool_t updtXstInfo(mstp_xst_port_t *pXstPort)
{
    
    if (cist(pXstPort) == TRUE)
    {
        return updtCistInfo(pXstPort);
    }

    return updtMstiInfo(pXstPort);
}
static void txTcn(mstp_port_t *pPort)
{   
    mstp_u8_t bpduTcn[MSTP_BPDU_LEN_TCN_MAX];
    mstp_u8_t *pBpdu = bpduTcn;

    if (pPort->pBridge->cbf_pkt_tx == NULL)
        return;

#ifdef CONFIG_SYS_STP_BPDU_FILTER
    if (pPort->adminBpduFilter == TRUE)
        return;
#endif  

    pPort->cntTxTcn += 1;

    *pBpdu++ = ethAddrBpdu[0];
    *pBpdu++ = ethAddrBpdu[1];
    *pBpdu++ = ethAddrBpdu[2];
    *pBpdu++ = ethAddrBpdu[3];
    *pBpdu++ = ethAddrBpdu[4];
    *pBpdu++ = ethAddrBpdu[5];
    
    *pBpdu++ = pPort->ethAddrSrc[0];
    *pBpdu++ = pPort->ethAddrSrc[1];
    *pBpdu++ = pPort->ethAddrSrc[2];
    *pBpdu++ = pPort->ethAddrSrc[3];
    *pBpdu++ = pPort->ethAddrSrc[4];
    *pBpdu++ = pPort->ethAddrSrc[5];
    
    *pBpdu++ = 0x00;
    *pBpdu++ = sizeof(bpduTcn) - MSTP_BPDU_LEN_ETH; 
    
    *pBpdu++ = 0x42;
    *pBpdu++ = 0x42;
    *pBpdu++ = 0x03;
    
    *pBpdu++ = 0x00;
    *pBpdu++ = 0x00;
    
    *pBpdu++ = MSTP_BPDU_VERSION_STP;
    
    *pBpdu++ = MSTP_BPDU_TYPE_TCN;

    pPort->pBridge->cbf_pkt_tx((mstp_u32_t)pPort->portIdx, bpduTcn, sizeof(bpduTcn));
}
static void disableForwarding(mstp_xst_port_t *pXstPort)
{   
    
}
static void disableLearning(mstp_xst_port_t *pXstPort)
{   
    
}
static void enableForwarding(mstp_xst_port_t *pXstPort)
{   
    
}
static void enableLearning(mstp_xst_port_t *pXstPort)
{   
    
}
static void recordPriority(mstp_xst_port_t *pXstPort)
{   
    pXstPort->portPriority = pXstPort->msgPriority;
}

static mstp_bool_t betterorsameInfo(mstp_info_t newInfoIs, mstp_xst_port_t *pXstPort)
{   
    
#if 0
    PriorityVectorPrint("betterorsameInfo(): msgPriority", &pXstPort->msgPriority);
    PriorityVectorPrint("betterorsameInfo(): portPriority", &pXstPort->portPriority);
    PriorityVectorPrint("betterorsameInfo(): designatedPriority", &pXstPort->designatedPriority);
#endif

    if ((newInfoIs == INFO_Received) && \
        (pXstPort->infoIs == INFO_Received) && \
        (PriorityVectorCmp(&pXstPort->msgPriority, &pXstPort->portPriority) >= 0))
    {
        return (TRUE);
    }
    else if ((newInfoIs == INFO_Mine) && \
             (pXstPort->infoIs == INFO_Mine) && \
             (PriorityVectorCmp(&pXstPort->designatedPriority, &pXstPort->portPriority) >= 0))
    {
        return (TRUE);
    }

    return (FALSE);
}
static void clearAllRcvdMsgs(mstp_port_t *pPort)
{   
    mstp_u32_t i;

    for (i=0; i<MAX_MSTP_INSTANCE; i++)
    {
        if (pPort->pBridge->xst[i].xstEnabled == FALSE)
        {
            continue;
        }

        pPort->pXstPort[i]->rcvdMsg = FALSE;
    }
}
static void clearReselectTree(mstp_xst_t *pXst)
{   
    mstp_u32_t j;

    for (j=0; j<MAX_MSTP_PORT_NUM; j++)
    {
        pXst->xstPort[j].reselect = FALSE;
    }
}
static mstp_bool_t fromSameRegion(mstp_port_t *pPort)
{   
    if ((pPort->rcvdRSTP == TRUE) && \
        (pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_MST) && \
        (cmpMstConfId(&pPort->bpduRxCist.mstConfigId, &pPort->pBridge->MstConfigId) == 0))
    {
        return (TRUE);
    }

    return (FALSE);
}
static void newTcWhile(mstp_xst_port_t *pXstPort)
{   
    if (pXstPort->tcWhile == 0) 
    {
        pXstPort->pXst->tcTimer = 0;    
        pXstPort->pXst->tcTimes += 1;   
    }

    if ((pXstPort->tcWhile == 0) && \
        (pXstPort->pPort->sendRSTP == TRUE))
    {
        pXstPort->tcWhile = HelloTime(pXstPort->pPort) + 1;
        newInfoXst(pXstPort, TRUE);
    }
    else if ((pXstPort->tcWhile == 0) && \
             (pXstPort->pPort->sendRSTP == FALSE))
    {
        pXstPort->tcWhile = \
            (pXstPort->pXst->rootTimes.MaxAge >> 8) + \
            (pXstPort->pXst->rootTimes.ForwardDelay >> 8);
    }
}
static mstp_info_t rcvInfo(mstp_xst_port_t *pXstPort)
{   
    mstp_u32_t i;
    mstp_i32_t cmpMP;
    mstp_bool_t cmpSame;
    mstp_bool_t cmpTimeMP;

    if (cist(pXstPort) && (pXstPort->pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_TCN))
    {
        pXstPort->pPort->rcvdTcn = TRUE;

        for (i=0; i<MAX_MSTP_INSTANCE; i++)
        {
            if (pXstPort->pBridge->xst[i].xstEnabled == FALSE)
            {
                continue;
            }

            pXstPort->pPort->pXstPort[i]->rcvdTc = TRUE;
        }
    }

    if (cist(pXstPort))
    {
        
        pXstPort->msgPriority.RootID = pXstPort->pPort->bpduRxCist.rootId;
        pXstPort->msgPriority.ExtRootPathCost = pXstPort->pPort->bpduRxCist.externalPathCost;
        pXstPort->msgPriority.RRootID = pXstPort->pPort->bpduRxCist.regionalRootId;
        pXstPort->msgPriority.IntRootPathCost = pXstPort->pPort->bpduRxCist.internalRootPathCost;
        pXstPort->msgPriority.DesignatedBridgeID = pXstPort->pPort->bpduRxCist.bridgeId;
        pXstPort->msgPriority.DesignatedPortID = pXstPort->pPort->bpduRxCist.portId;
        pXstPort->msgPriority.RcvPortID = pXstPort->portId;

        pXstPort->msgTimes.MessageAge = pXstPort->pPort->bpduRxCist.messageAge;
        pXstPort->msgTimes.MaxAge = pXstPort->pPort->bpduRxCist.maxAge;
        pXstPort->msgTimes.ForwardDelay = pXstPort->pPort->bpduRxCist.forwardDelay;
        pXstPort->msgTimes.HelloTime = pXstPort->pPort->bpduRxCist.helloTime;
        if ((pXstPort->pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_CONFIG) || \
            (pXstPort->pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_RST))
        {
            pXstPort->msgTimes.remainingHops = pXstPort->pBridge->MaxHops;
        }
        else
        {
            pXstPort->msgTimes.remainingHops = pXstPort->bpduRxMsti.remainingHops;
        }
    }
    else
    {
        
        pXstPort->msgPriority.RRootID = pXstPort->bpduRxMsti.regionalRootId;
        pXstPort->msgPriority.IntRootPathCost = pXstPort->bpduRxMsti.internalRootPathCost;
        pXstPort->msgPriority.DesignatedBridgeID = pXstPort->pPort->bpduRxCist.bridgeId;
        pXstPort->msgPriority.DesignatedPortID = pXstPort->pPort->bpduRxCist.portId;
        pXstPort->msgPriority.RcvPortID = pXstPort->portId;

        pXstPort->msgPriority.DesignatedBridgeID.priority = \
            ((pXstPort->bpduRxMsti.bridgePriority & 0xF0) << 8) | \
            ((pXstPort->bpduRxMsti.regionalRootId.priority & 0x0FFF) << 0);
        pXstPort->msgPriority.DesignatedPortID.priority &= 0x0F;
        pXstPort->msgPriority.DesignatedPortID.priority |= \
            (pXstPort->bpduRxMsti.portPriority & 0xF0) << 0;

        pXstPort->msgTimes.remainingHops = pXstPort->bpduRxMsti.remainingHops;
    }

    if (cist(pXstPort))
    {
        cmpMP = cmpCistPriorityMessagePort(&pXstPort->msgPriority, &pXstPort->portPriority);
        cmpSame = (cmpMP == 0 || cmpMP == 7 || cmpMP == -7)? TRUE : FALSE;
        cmpTimeMP = ((pXstPort->msgTimes.MessageAge != pXstPort->portTimes.MessageAge) || \
                     (pXstPort->msgTimes.MaxAge != pXstPort->portTimes.MaxAge) || \
                     (pXstPort->msgTimes.ForwardDelay != pXstPort->portTimes.ForwardDelay) || \
                     (pXstPort->msgTimes.HelloTime != pXstPort->portTimes.HelloTime) || \
                     (pXstPort->msgTimes.remainingHops != pXstPort->portTimes.remainingHops));
    }
    else
    {
        cmpMP = cmpMstiPriorityMessagePort(&pXstPort->msgPriority, &pXstPort->portPriority);
        cmpSame = (cmpMP == 0 || cmpMP == 7 || cmpMP == -7)? TRUE : FALSE;
        cmpTimeMP = (pXstPort->msgTimes.remainingHops != pXstPort->portTimes.remainingHops);
    }

    if (((pXstPort->bpduRxMsti.flags & MSTP_BPDU_FLAG_PORT_ROLE) == \
         MSTP_BPDU_PORT_ROLE_DESIGNATED) || \
        (pXstPort->pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_CONFIG))
    {   
        if ((cmpMP > 0) || ((cmpSame) && (cmpTimeMP)))
        {   
            return (INFO_SuperiorDesignatedInfo);
        }
        else if ((cmpSame) && (pXstPort->infoIs == INFO_Received))
        {   
            return (INFO_RepeatedDesignatedInfo);
        }
        else
        {   
            return (INFO_InferiorDesignatedInfo);
        }
    }
    else
    {   
        if (cmpMP <= 0)
        {   
            return (INFO_InferiorRootAlternateInfo);
        }
    }

    return (INFO_OtherInfo);
}
static void recordAgreement(mstp_xst_port_t *pXstPort)
{   
    mstp_u32_t i;

    if (cist(pXstPort))
    {   
        if ((rstpVersion(pXstPort->pBridge) == TRUE) && \
            (pXstPort->pPort->operPointToPointMAC == TRUE) && \
            (pXstPort->pPort->bpduRxCist.flags & MSTP_BPDU_FLAG_AGREEMENT))
        {
            pXstPort->agreed = TRUE;
            pXstPort->proposing = FALSE;
        }
        else
        {
            pXstPort->agreed = FALSE;
        }

        if (pXstPort->pPort->rcvdInternal == FALSE)
        {
            for (i=1; i<MAX_MSTP_INSTANCE; i++)
            {
                if (pXstPort->pBridge->xst[i].xstEnabled == FALSE)
                {
                    continue;
                }

                pXstPort->pPort->pXstPort[i]->agreed = pXstPort->agreed;
                pXstPort->pPort->pXstPort[i]->proposing = pXstPort->proposing;
            }
        }
    }
    else
    {   
        
        if (pXstPort->pPort->rcvdInternal == FALSE)
            return;

        if ((pXstPort->pPort->operPointToPointMAC == TRUE) && \
            (cmpBridgeId(&pXstPort->pCistPort->msgPriority.RootID, \
                         &pXstPort->pCistPort->portPriority.RootID) == 0) && \
            (pXstPort->pCistPort->msgPriority.ExtRootPathCost == \
             pXstPort->pCistPort->portPriority.ExtRootPathCost) && \
            (cmpBridgeId(&pXstPort->pCistPort->msgPriority.RRootID, \
                         &pXstPort->pCistPort->portPriority.RRootID) == 0) && \
            (pXstPort->bpduRxMsti.flags & MSTP_BPDU_FLAG_AGREEMENT))
        {
            pXstPort->agreed = TRUE;
            pXstPort->proposing = FALSE;
        }
        else
        {
            pXstPort->agreed = FALSE;
        }
    }
}
static void recordDispute(mstp_xst_port_t *pXstPort)
{   
    mstp_u32_t i;

    if (cist(pXstPort))
    {   
        if (pXstPort->learning == TRUE)
        {
            pXstPort->disputed = TRUE;
            pXstPort->agreed = FALSE;

            if (pXstPort->pPort->rcvdInternal == FALSE)
            {
                for (i=1; i<MAX_MSTP_INSTANCE; i++)
                {
                    if (pXstPort->pBridge->xst[i].xstEnabled == FALSE)
                    {
                        continue;
                    }

                    pXstPort->pPort->pXstPort[i]->disputed = TRUE;
                    pXstPort->pPort->pXstPort[i]->agreed = FALSE;
                }
            }
        }
    }
    else
    {   
        if (pXstPort->learning == TRUE)
        {
            pXstPort->disputed = TRUE;
            pXstPort->agreed = FALSE;
        }
    }
}
static void recordMastered(mstp_xst_port_t *pXstPort)
{   
    mstp_u32_t i;

    if (cist(pXstPort))
    {
        if (pXstPort->pPort->rcvdInternal == FALSE)
        {
            for (i=0; i<MAX_MSTP_INSTANCE; i++) 
            {
                if (pXstPort->pBridge->xst[i].xstEnabled == FALSE)
                {
                    continue;
                }

                pXstPort->pXst->xstPort[pXstPort->portIdx].mastered = FALSE;
            }
        }
    }
    else
    {
        if ((pXstPort->pPort->operPointToPointMAC == TRUE) && \
            (pXstPort->bpduRxMsti.flags & MSTP_BPDU_FLAG_MASTER))
        {
            pXstPort->mastered = TRUE;
        }
        else
        {
            pXstPort->mastered = FALSE; 
        }
    }
}
static void recordProposal(mstp_xst_port_t *pXstPort)
{   
    mstp_u32_t i;

    if (cist(pXstPort))
    {   
        if (((pXstPort->pPort->bpduRxCist.flags & MSTP_BPDU_FLAG_PORT_ROLE) == \
             MSTP_BPDU_PORT_ROLE_DESIGNATED) && \
            (pXstPort->pPort->bpduRxCist.flags & MSTP_BPDU_FLAG_PROPOSAL))
        {
            pXstPort->proposed = TRUE;
        }

        if (pXstPort->pPort->rcvdInternal == FALSE)
        {
            for (i=1; i<MAX_MSTP_INSTANCE; i++)
            {
                if (pXstPort->pBridge->xst[i].xstEnabled == FALSE)
                {
                    continue;
                }

                pXstPort->pPort->pXstPort[i]->proposed = pXstPort->proposed;
            }
        }
    }
    else
    {   
        
        if (pXstPort->pPort->rcvdInternal == FALSE)
            return;

        if (((pXstPort->bpduRxMsti.flags & MSTP_BPDU_FLAG_PORT_ROLE) == \
             MSTP_BPDU_PORT_ROLE_DESIGNATED) && \
            (pXstPort->bpduRxMsti.flags & MSTP_BPDU_FLAG_PROPOSAL))
        {
            pXstPort->proposed = TRUE;
        }
    }
}
static void recordTimes(mstp_xst_port_t *pXstPort)
{   
    if (cist(pXstPort))
    {   
        pXstPort->portTimes.MessageAge = pXstPort->msgTimes.MessageAge;
        pXstPort->portTimes.MaxAge = pXstPort->msgTimes.MaxAge;
        pXstPort->portTimes.ForwardDelay = pXstPort->msgTimes.ForwardDelay;
        pXstPort->portTimes.remainingHops = pXstPort->msgTimes.remainingHops;
        if ((pXstPort->msgTimes.HelloTime >= MSTP_HELLO_TIME_MIN) && \
            (pXstPort->msgTimes.HelloTime <= MSTP_HELLO_TIME_MAX))
        {
            pXstPort->portTimes.HelloTime = pXstPort->msgTimes.HelloTime;
        }
        else
        {
            pXstPort->portTimes.HelloTime = pXstPort->pBridge->helloTime;
        }
    }
    else
    {   
        pXstPort->portTimes.remainingHops = pXstPort->msgTimes.remainingHops;
    }
}
static void setRcvdMsgs(mstp_port_t *pPort)
{   
    mstp_u32_t i;
    mstp_xst_port_t *pXstPort;

    pPort->pCistPort->rcvdMsg = TRUE;

    if (pPort->rcvdInternal)
    {
        for (i=1; i<MAX_MSTP_INSTANCE; i++)
        {
            if (pPort->pBridge->xst[i].xstEnabled == FALSE)
            {
                continue;
            }

            pXstPort = pPort->pXstPort[i];

            if (pXstPort->rcvdBpduMsti == TRUE) 
            {
                pXstPort->rcvdBpduMsti = FALSE; 
                pXstPort->rcvdMsg = TRUE;
            }
        }
    }
}
static void setReRootTree(mstp_xst_port_t *pXstPort)
{   
    mstp_xst_t *pXst = pXstPort->pXst;
    mstp_u32_t j;
    for (j=0; j<MAX_MSTP_PORT_NUM; j++)
    {
        if (pXst->xstPort[j].pPort->portEnabled == FALSE)
        {
            continue;
        }

        pXst->xstPort[j].reRoot = TRUE;
    }
}
static void setSelectedTree(mstp_xst_t *pXst)
{   
    mstp_u32_t j;

    for (j=0; j<MAX_MSTP_PORT_NUM; j++)
    {
        if (pXst->xstPort[j].pPort->portEnabled == FALSE)
        {
            continue;
        }

        if (pXst->xstPort[j].reselect == TRUE)
        {
            return;
        }
    }

    for (j=0; j<MAX_MSTP_PORT_NUM; j++)
    {
        if (pXst->xstPort[j].pPort->portEnabled == FALSE)
        {
            continue;
        }

        pXst->xstPort[j].selected = TRUE;
    }
}
static void setSyncTree(mstp_xst_port_t *pXstPort)
{   
    mstp_xst_t *pXst = pXstPort->pXst;
    mstp_u32_t j;

    for (j=0; j<MAX_MSTP_PORT_NUM; j++)
    {
        if (pXst->xstPort[j].pPort->portEnabled == FALSE)
        {
            continue;
        }

        pXst->xstPort[j].sync = TRUE;
    }
}
static void setTcFlags(mstp_xst_port_t *pXstPort)
{   
    mstp_u32_t i;

    if (cist(pXstPort))
    {   
        
        if (pXstPort->pPort->bpduRxCist.flags & MSTP_BPDU_FLAG_TC_ACK)
        {
            pXstPort->pPort->rcvdTcAck = TRUE;
        }

        if (pXstPort->pPort->bpduRxCist.flags & MSTP_BPDU_FLAG_TC)
        {
            if (pXstPort->pPort->rcvdInternal == FALSE)
            {   
                for (i=0; i<MAX_MSTP_INSTANCE; i++)
                {
                    if (pXstPort->pBridge->xst[i].xstEnabled == FALSE)
                    {
                        continue;
                    }

                    pXstPort->rcvdTc = TRUE;
                }
            }
            else
            {   
                pXstPort->rcvdTc = TRUE;
            }
        }
    }
    else
    {   
        if (pXstPort->bpduRxMsti.flags & MSTP_BPDU_FLAG_TC)
        {
            pXstPort->rcvdTc = TRUE;
        }
    }
}
static void setTcPropTree(mstp_xst_port_t *pXstPort)
{   
    mstp_u32_t j;
    if (restrictedTcn(pXstPort) == FALSE)
    {
        for (j=0; j<MAX_MSTP_PORT_NUM; j++)
        {
            if (pXstPort->pXst->xstPort[j].pPort->portEnabled == FALSE)
            {
                continue;
            }

            if (pXstPort->pXst->xstPort[j].portIdx != pXstPort->portIdx)
            {
                pXstPort->pXst->xstPort[j].tcProp = TRUE;
            }
        }
    }
}
static void syncMaster(mstp_bridge_t *pBridge)
{   
    mstp_u32_t i, j;
    mstp_xst_port_t *pXstPort;

    for (i=1; i<MAX_MSTP_INSTANCE; i++)
    {
        if (pBridge->xst[i].xstEnabled == FALSE)
        {
            continue;
        }

        for (j=0; j<MAX_MSTP_PORT_NUM; j++)
        {
            pXstPort = &pBridge->xst[i].xstPort[j];

            if (pXstPort->pPort->portEnabled == FALSE)
            {
                continue;
            }

            if (pXstPort->pPort->infoInternal == TRUE)
            {
                
                pXstPort->agree = FALSE;
                pXstPort->agreed = FALSE;
                pXstPort->synced = FALSE;

                pXstPort->sync = TRUE;
            }
        }
    }
}
static void txConfig(mstp_port_t *pPort)
{   
    mstp_u8_t bpduConfig[MSTP_BPDU_LEN_CONFIG_MAX];
    mstp_u8_t *pBpdu = bpduConfig;

    if (pPort->pBridge->cbf_pkt_tx == NULL)
        return;

    if (pPort->adminEnable == DISABLED)
        return;

    if (pPort->pCistPort->designatedTimes.MessageAge >= pPort->pCistPort->designatedTimes.MaxAge)
        return;

#ifdef CONFIG_SYS_STP_BPDU_FILTER
    if (pPort->adminBpduFilter == TRUE)
        return;
#endif  

    pPort->cntTxConfig += 1;

    *pBpdu++ = ethAddrBpdu[0];
    *pBpdu++ = ethAddrBpdu[1];
    *pBpdu++ = ethAddrBpdu[2];
    *pBpdu++ = ethAddrBpdu[3];
    *pBpdu++ = ethAddrBpdu[4];
    *pBpdu++ = ethAddrBpdu[5];
    
    *pBpdu++ = pPort->ethAddrSrc[0];
    *pBpdu++ = pPort->ethAddrSrc[1];
    *pBpdu++ = pPort->ethAddrSrc[2];
    *pBpdu++ = pPort->ethAddrSrc[3];
    *pBpdu++ = pPort->ethAddrSrc[4];
    *pBpdu++ = pPort->ethAddrSrc[5];
    
    *pBpdu++ = 0x00;
    *pBpdu++ = sizeof(bpduConfig) - MSTP_BPDU_LEN_ETH;
    
    *pBpdu++ = 0x42;
    *pBpdu++ = 0x42;
    *pBpdu++ = 0x03;
    
    *pBpdu++ = 0x00;
    *pBpdu++ = 0x00;
    
    *pBpdu++ = MSTP_BPDU_VERSION_STP;
    
    *pBpdu++ = MSTP_BPDU_TYPE_CONFIG;
    
    *pBpdu++ = (((pPort->pCistPort->tcWhile != 0)? MSTP_BPDU_FLAG_TC : 0) | \
                ((pPort->tcAck)? MSTP_BPDU_FLAG_TC_ACK : 0));
    
    *pBpdu++ = (pPort->pCistPort->designatedPriority.RootID.priority >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.RootID.priority >> 0) & 0xFF;
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[0];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[1];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[2];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[3];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[4];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[5];
    
    *pBpdu++ = (pPort->pCistPort->designatedPriority.ExtRootPathCost >> 24) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.ExtRootPathCost >> 16) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.ExtRootPathCost >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.ExtRootPathCost >> 0) & 0xFF;
    
    *pBpdu++ = (pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 0) & 0xFF;
    *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[0];
    *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[1];
    *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[2];
    *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[3];
    *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[4];
    *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[5];
    
    *pBpdu++ = ((pPort->pCistPort->designatedPriority.DesignatedPortID.priority >> 0) & 0xF0) | \
               ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 8) & 0x0F);
    *pBpdu++ = ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 0) & 0xFF);
    
    *pBpdu++ = (pPort->pCistPort->designatedTimes.MessageAge >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedTimes.MessageAge >> 0) & 0xFF;
    
    *pBpdu++ = (pPort->pCistPort->designatedTimes.MaxAge >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedTimes.MaxAge >> 0) & 0xFF;
    
    *pBpdu++ = (pPort->pCistPort->portTimes.HelloTime >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->portTimes.HelloTime >> 0) & 0xFF;
    
    *pBpdu++ = (pPort->pCistPort->designatedTimes.ForwardDelay >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedTimes.ForwardDelay >> 0) & 0xFF;

    pPort->pBridge->cbf_pkt_tx((mstp_u32_t)pPort->portIdx, bpduConfig, sizeof(bpduConfig));
}
static void txMstp(mstp_port_t *pPort)
{   
    mstp_u8_t bpduMstp[MSTP_BPDU_LEN_MSTP_MAX];
    mstp_u8_t *pBpdu = bpduMstp;
    mstp_u8_t mstiAlive[MAX_MSTP_INSTANCE];
    mstp_u16_t numMsti = 0;
    mstp_u8_t bpduVerId = 0;
    mstp_u16_t bpduSize = 0;
    mstp_u32_t i;

    if (pPort->pBridge->cbf_pkt_tx == NULL)
        return;

    if (pPort->pCistPort->designatedTimes.MessageAge > pPort->pCistPort->designatedTimes.MaxAge)
        return;

#ifdef CONFIG_SYS_STP_BPDU_FILTER
    if (pPort->adminBpduFilter == TRUE)
        return;
#endif  

    pPort->cntTxMstp += 1;

    for (i=1; i<MAX_MSTP_INSTANCE; i++)
    {
        if (pPort->pBridge->xst[i].xstEnabled == TRUE)
        {
            mstiAlive[i] = TRUE;
            numMsti += 1;
        }
        else
        {
            mstiAlive[i] = FALSE;
        }
    }

    bpduVerId = (pPort->pBridge->ForceProtocolVersion >= MSTP_BPDU_VERSION_MSTP)? \
                MSTP_BPDU_VERSION_MSTP : MSTP_BPDU_VERSION_RSTP;

    bpduSize = (bpduVerId == MSTP_BPDU_VERSION_MSTP)? \
        MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_MST + (MSTP_BPDU_LEN_MSTI * numMsti) : \
        MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_RST;

    *pBpdu++ = ethAddrBpdu[0];
    *pBpdu++ = ethAddrBpdu[1];
    *pBpdu++ = ethAddrBpdu[2];
    *pBpdu++ = ethAddrBpdu[3];
    *pBpdu++ = ethAddrBpdu[4];
    *pBpdu++ = ethAddrBpdu[5];
    
    *pBpdu++ = pPort->ethAddrSrc[0];
    *pBpdu++ = pPort->ethAddrSrc[1];
    *pBpdu++ = pPort->ethAddrSrc[2];
    *pBpdu++ = pPort->ethAddrSrc[3];
    *pBpdu++ = pPort->ethAddrSrc[4];
    *pBpdu++ = pPort->ethAddrSrc[5];
    
    *pBpdu++ = (bpduSize >> 8) & 0xFF;
    *pBpdu++ = (bpduSize >> 0) & 0xFF;
    
    *pBpdu++ = 0x42;
    *pBpdu++ = 0x42;
    *pBpdu++ = 0x03;
    
    *pBpdu++ = 0x00;
    *pBpdu++ = 0x00;
    
    *pBpdu++ = bpduVerId;
    
    *pBpdu++ = MSTP_BPDU_TYPE_RST;
    
    *pBpdu++ = (((pPort->pCistPort->tcWhile != 0)? MSTP_BPDU_FLAG_TC : 0) | \
                ((pPort->pCistPort->proposing)? MSTP_BPDU_FLAG_PROPOSAL : 0) | \
                ((PortRole_Encode(pPort->pCistPort->role) & 0x3) << 2) | \
                ((pPort->pCistPort->learning)? MSTP_BPDU_FLAG_LEARNING: 0) | \
                ((pPort->pCistPort->forwarding)? MSTP_BPDU_FLAG_FORWARDING : 0) | \
                ((pPort->pCistPort->agree)? MSTP_BPDU_FLAG_AGREEMENT : 0));
    
    *pBpdu++ = (pPort->pCistPort->designatedPriority.RootID.priority >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.RootID.priority >> 0) & 0xFF;
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[0];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[1];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[2];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[3];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[4];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RootID.addr[5];
    
    *pBpdu++ = (pPort->pCistPort->designatedPriority.ExtRootPathCost >> 24) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.ExtRootPathCost >> 16) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.ExtRootPathCost >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.ExtRootPathCost >> 0) & 0xFF;
    
    *pBpdu++ = (pPort->pCistPort->designatedPriority.RRootID.priority >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedPriority.RRootID.priority >> 0) & 0xFF;
    *pBpdu++ = pPort->pCistPort->designatedPriority.RRootID.addr[0];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RRootID.addr[1];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RRootID.addr[2];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RRootID.addr[3];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RRootID.addr[4];
    *pBpdu++ = pPort->pCistPort->designatedPriority.RRootID.addr[5];
    
    *pBpdu++ = ((pPort->pCistPort->designatedPriority.DesignatedPortID.priority >> 0) & 0xF0) | \
               ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 8) & 0x0F);
    *pBpdu++ = ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 0) & 0xFF);
    
    *pBpdu++ = (pPort->pCistPort->designatedTimes.MessageAge >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedTimes.MessageAge >> 0) & 0xFF;
    
    *pBpdu++ = (pPort->pCistPort->designatedTimes.MaxAge >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedTimes.MaxAge >> 0) & 0xFF;
    
    *pBpdu++ = (pPort->pCistPort->portTimes.HelloTime >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->portTimes.HelloTime >> 0) & 0xFF;
    
    *pBpdu++ = (pPort->pCistPort->designatedTimes.ForwardDelay >> 8) & 0xFF;
    *pBpdu++ = (pPort->pCistPort->designatedTimes.ForwardDelay >> 0) & 0xFF;
    
    *pBpdu++ = 0x00;

    if (bpduVerId == MSTP_BPDU_VERSION_MSTP)
    {
        
        *pBpdu++ = ((0x40 + (MSTP_BPDU_LEN_MSTI * numMsti)) >> 8) & 0xFF;
        *pBpdu++ = ((0x40 + (MSTP_BPDU_LEN_MSTI * numMsti)) >> 0) & 0xFF;
        
        *pBpdu++ = pPort->pBridge->MstConfigId.idFormatSelector;
        
        *pBpdu++ = pPort->pBridge->MstConfigId.name[0];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[1];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[2];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[3];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[4];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[5];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[6];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[7];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[8];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[9];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[10];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[11];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[12];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[13];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[14];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[15];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[16];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[17];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[18];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[19];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[20];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[21];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[22];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[23];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[24];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[25];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[26];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[27];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[28];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[29];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[30];
        *pBpdu++ = pPort->pBridge->MstConfigId.name[31];
        
        *pBpdu++ = (pPort->pBridge->MstConfigId.revisionLevel >> 8) & 0xFF;
        *pBpdu++ = (pPort->pBridge->MstConfigId.revisionLevel >> 0) & 0xFF;
        
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[0];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[1];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[2];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[3];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[4];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[5];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[6];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[7];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[8];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[9];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[10];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[11];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[12];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[13];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[14];
        *pBpdu++ = pPort->pBridge->MstConfigId.digest[15];
        
        *pBpdu++ = (pPort->pCistPort->designatedPriority.IntRootPathCost >> 24) & 0xFF;
        *pBpdu++ = (pPort->pCistPort->designatedPriority.IntRootPathCost >> 16) & 0xFF;
        *pBpdu++ = (pPort->pCistPort->designatedPriority.IntRootPathCost >> 8) & 0xFF;
        *pBpdu++ = (pPort->pCistPort->designatedPriority.IntRootPathCost >> 0) & 0xFF;
        
        *pBpdu++ = (pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 8) & 0xFF;
        *pBpdu++ = (pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 0) & 0xFF;
        *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[0];
        *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[1];
        *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[2];
        *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[3];
        *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[4];
        *pBpdu++ = pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[5];

        *pBpdu++ = pPort->pCistPort->portTimes.remainingHops;

        for (i=1; i<MAX_MSTP_INSTANCE; i++)
        {
            if (mstiAlive[i] == FALSE)
            {
                continue;
            }

            pPort->pXstPort[i]->master = FALSE; 

            *pBpdu++ = (((pPort->pXstPort[i]->tcWhile != 0)? MSTP_BPDU_FLAG_TC : 0) | \
                        ((pPort->pXstPort[i]->proposing)? MSTP_BPDU_FLAG_PROPOSAL : 0) | \
                        ((PortRole_Encode(pPort->pXstPort[i]->role) & 0x3) << 2) | \
                        ((pPort->pXstPort[i]->learning)? MSTP_BPDU_FLAG_LEARNING: 0) | \
                        ((pPort->pXstPort[i]->forwarding)? MSTP_BPDU_FLAG_FORWARDING : 0) | \
                        ((pPort->pXstPort[i]->agree)? MSTP_BPDU_FLAG_AGREEMENT : 0) | \
                        ((pPort->pXstPort[i]->master)? MSTP_BPDU_FLAG_MASTER : 0));
            
            *pBpdu++ = (pPort->pXstPort[i]->designatedPriority.RRootID.priority >> 8) & 0xFF;
            *pBpdu++ = (pPort->pXstPort[i]->designatedPriority.RRootID.priority >> 0) & 0xFF;
            *pBpdu++ = pPort->pXstPort[i]->designatedPriority.RRootID.addr[0];
            *pBpdu++ = pPort->pXstPort[i]->designatedPriority.RRootID.addr[1];
            *pBpdu++ = pPort->pXstPort[i]->designatedPriority.RRootID.addr[2];
            *pBpdu++ = pPort->pXstPort[i]->designatedPriority.RRootID.addr[3];
            *pBpdu++ = pPort->pXstPort[i]->designatedPriority.RRootID.addr[4];
            *pBpdu++ = pPort->pXstPort[i]->designatedPriority.RRootID.addr[5];
            
            *pBpdu++ = (pPort->pXstPort[i]->designatedPriority.IntRootPathCost >> 24) & 0xFF;
            *pBpdu++ = (pPort->pXstPort[i]->designatedPriority.IntRootPathCost >> 16) & 0xFF;
            *pBpdu++ = (pPort->pXstPort[i]->designatedPriority.IntRootPathCost >> 8) & 0xFF;
            *pBpdu++ = (pPort->pXstPort[i]->designatedPriority.IntRootPathCost >> 0) & 0xFF;
            
            *pBpdu++ = (pPort->pXstPort[i]->pXst->BridgeIdentifierPriority >> 8) & 0xF0;
            
            *pBpdu++ = pPort->pXstPort[i]->portId.priority & 0xF0;

            *pBpdu++ = pPort->pXstPort[i]->portTimes.remainingHops;
        } 
    } 

    pPort->pBridge->cbf_pkt_tx((mstp_u32_t)pPort->portIdx, bpduMstp, MSTP_BPDU_LEN_ETH + bpduSize);
}
static void updtBPDUVersion(mstp_port_t *pPort)
{   
    if (((pPort->bpduRxCist.protoVerId == MSTP_BPDU_VERSION_STP) || \
         (pPort->bpduRxCist.protoVerId == MSTP_BPDU_VERSION_STP1)) && \
        ((pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_TCN) || \
         (pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_CONFIG)))
    {
        pPort->rcvdSTP = TRUE;
    }
    else if ((pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_RST) || \
             (pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_MST))
    {
        pPort->rcvdRSTP = TRUE;
    }
}
static void updtRcvdInfoWhile(mstp_xst_port_t *pXstPort)
{   
    if ((((pXstPort->pCistPort->portTimes.MessageAge + MSTP_TIME_ONE_SECOND) <= \
          (pXstPort->pCistPort->portTimes.MaxAge)) && \
         (pXstPort->pPort->rcvdInternal == FALSE)) || \
        (((pXstPort->pCistPort->portTimes.remainingHops - 1) > 0) && \
         (pXstPort->pPort->rcvdInternal == TRUE)))
    {
        pXstPort->rcvdInfoWhile = 3 * HelloTime(pXstPort->pPort);
    }
    else
    {
        pXstPort->rcvdInfoWhile = 0;
    }
}
static void updtRolesTree(mstp_xst_t *pXst)
{   
    mstp_u32_t p;
    mstp_xst_port_t *pXstPort;
    mstp_xst_port_t *pCistPort;
    mstp_pri_vctr_t rootPathPriVctr;
    mstp_pri_vctr_t *pMsgPriVctr;
    mstp_bool_t calculated;
    mstp_bool_t cistRecalculated;
    mstp_bool_t rootPriVctrChanged;
    mstp_u32_t prevExtRootPathCost = 0;
    
    pXst->BridgeIdentifier.priority = pXst->BridgeIdentifierPriority;
    if (MSTP_INSTANCE_CIST == pXst->xstIdx)
    {   
        
        pXst->BridgePriority.RootID = pXst->BridgeIdentifier;
        pXst->BridgePriority.ExtRootPathCost = 0;
        pXst->BridgePriority.RRootID = pXst->BridgeIdentifier;
        pXst->BridgePriority.IntRootPathCost = 0;
        pXst->BridgePriority.DesignatedBridgeID = pXst->BridgeIdentifier;
        pXst->BridgePriority.DesignatedPortID.priority = 0;
        pXst->BridgePriority.DesignatedPortID.number = 0;
        pXst->BridgePriority.RcvPortID.priority = 0;
        pXst->BridgePriority.RcvPortID.number = 0;
    }
    else
    {   
        
        pXst->BridgePriority.RootID.priority = 0;
        pXst->BridgePriority.RootID.addr[0] = 0x00;
        pXst->BridgePriority.RootID.addr[1] = 0x00;
        pXst->BridgePriority.RootID.addr[2] = 0x00;
        pXst->BridgePriority.RootID.addr[3] = 0x00;
        pXst->BridgePriority.RootID.addr[4] = 0x00;
        pXst->BridgePriority.RootID.addr[5] = 0x00;
        pXst->BridgePriority.ExtRootPathCost = 0;
        pXst->BridgePriority.RRootID = pXst->BridgeIdentifier;
        pXst->BridgePriority.IntRootPathCost = 0;
        pXst->BridgePriority.DesignatedBridgeID = pXst->BridgeIdentifier;
        pXst->BridgePriority.DesignatedPortID.priority = 0;
        pXst->BridgePriority.DesignatedPortID.number = 0;
        pXst->BridgePriority.RcvPortID.priority = 0;
        pXst->BridgePriority.RcvPortID.number = 0;
    }

    pXst->BridgeTimes.MessageAge = 0;
    pXst->BridgeTimes.MaxAge = pXst->pBridge->BridgeMaxAge;
    pXst->BridgeTimes.ForwardDelay = pXst->pBridge->BridgeForwardDelay;
    pXst->BridgeTimes.HelloTime = pXst->pBridge->helloTime;
    pXst->BridgeTimes.MaxHops = pXst->pBridge->MaxHops;
    pXst->BridgeTimes.remainingHops = pXst->BridgeTimes.MaxHops;

    pXst->rootPriority = pXst->BridgePriority;
    pXst->rootTimes = pXst->BridgeTimes;

    pXst->rootPortId.priority = 0;
    pXst->rootPortId.number = 0;
    pXst->rootPortIdx = 0;
    cistRecalculated = FALSE;
    rootPriVctrChanged = FALSE;

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        pXstPort = &pXst->xstPort[p];

        if (pXstPort->pPort->portEnabled == FALSE)
        {
            continue;
        }

        calculated = FALSE;
        if (pXstPort->infoIs == INFO_Received)
        {
            if (cist(pXstPort))
            {   
                if (pXstPort->pPort->rcvdInternal)  
                {
                    
                    pMsgPriVctr = &pXstPort->msgPriority;

                    rootPathPriVctr.RootID = pMsgPriVctr->RootID;
                    rootPathPriVctr.ExtRootPathCost = pMsgPriVctr->ExtRootPathCost;
                    rootPathPriVctr.RRootID = pMsgPriVctr->RRootID;
                    rootPathPriVctr.IntRootPathCost = \
                        pMsgPriVctr->IntRootPathCost + \
                        pXstPort->InternalPortPathCost;
                    rootPathPriVctr.DesignatedBridgeID = pMsgPriVctr->DesignatedBridgeID;
                    rootPathPriVctr.DesignatedPortID = pMsgPriVctr->DesignatedPortID;
                    rootPathPriVctr.RcvPortID = pMsgPriVctr->RcvPortID;
                }
                else
                {
                    
                    pMsgPriVctr = &pXstPort->msgPriority;

                    rootPathPriVctr.RootID = pMsgPriVctr->RootID;
                    rootPathPriVctr.ExtRootPathCost = \
                        pMsgPriVctr->ExtRootPathCost + \
                        pXstPort->pPort->ExternalPortPathCost;
                    rootPathPriVctr.RRootID = pXstPort->pXst->BridgeIdentifier;
                    rootPathPriVctr.IntRootPathCost = 0;
                    rootPathPriVctr.DesignatedBridgeID = pMsgPriVctr->DesignatedBridgeID;
                    rootPathPriVctr.DesignatedPortID = pMsgPriVctr->DesignatedPortID;
                    rootPathPriVctr.RcvPortID = pMsgPriVctr->RcvPortID;
                }

                calculated = TRUE;
                cistRecalculated = TRUE;
            }
            else
            {   
                if (pXstPort->pPort->rcvdInternal)  
                {
                    pMsgPriVctr = &pXstPort->msgPriority;

                    rootPathPriVctr.RootID.priority = 0;
                    rootPathPriVctr.RootID.addr[0] = 0x00;
                    rootPathPriVctr.RootID.addr[1] = 0x00;
                    rootPathPriVctr.RootID.addr[2] = 0x00;
                    rootPathPriVctr.RootID.addr[3] = 0x00;
                    rootPathPriVctr.RootID.addr[4] = 0x00;
                    rootPathPriVctr.RootID.addr[5] = 0x00;
                    rootPathPriVctr.ExtRootPathCost = 0;
                    rootPathPriVctr.RRootID = pMsgPriVctr->RRootID;
                    rootPathPriVctr.IntRootPathCost = \
                        pMsgPriVctr->IntRootPathCost + \
                        pXstPort->InternalPortPathCost;
                    rootPathPriVctr.DesignatedBridgeID = pMsgPriVctr->DesignatedBridgeID;
                    rootPathPriVctr.DesignatedPortID = pMsgPriVctr->DesignatedPortID;
                    rootPathPriVctr.RcvPortID = pMsgPriVctr->RcvPortID;

                    calculated = TRUE;
                }
            }

            if ((calculated) && \
                
                (BridgeIdCmp(&rootPathPriVctr.DesignatedBridgeID, \
                             &pXstPort->pXst->BridgeIdentifier) != 0) && \
                (restrictedRole(pXstPort->pPort) == FALSE))
            {
                
                if (PriorityVectorCmp(&rootPathPriVctr, &pXst->rootPriority) > 0)
                {
                    prevExtRootPathCost = pXst->rootPriority.ExtRootPathCost;

                    pXst->rootPriority = rootPathPriVctr;
                    pXst->rootPortId = pXstPort->portId;
                    pXst->rootPortIdx = pXstPort->portIdx;

                    rootPriVctrChanged = TRUE;
                }
            }
        }
    }

    if (pXst->rootPortId.number == 0)   
    {
        
        if (MSTP_INSTANCE_CIST == pXst->xstIdx)
        {
            pXst->BridgeTimes.ForwardDelay = pXst->pBridge->BridgeForwardDelay;
            pXst->BridgeTimes.MaxAge = pXst->pBridge->BridgeMaxAge;
            pXst->BridgeTimes.MessageAge = 0;
            pXst->BridgeTimes.MaxHops = pXst->pBridge->MaxHops;
        }
        else
        {
            pXst->BridgeTimes.MaxHops = pXst->pBridge->MaxHops;
        }

        pXst->rootTimes = pXst->BridgeTimes;
    }
    else
    {
        pXst->rootTimes = pXst->xstPort[pXst->rootPortIdx].portTimes;

        if (pXst->xstPort[pXst->rootPortIdx].pPort->rcvdInternal == FALSE)
        {
            pXst->rootTimes.MessageAge += MSTP_TIME_ONE_SECOND;
        }

        if (pXst->xstPort[pXst->rootPortIdx].pPort->rcvdInternal == TRUE)
        {
            pXst->rootTimes.remainingHops -= 1;
        }
    }

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        pXstPort = &pXst->xstPort[p];

        if (cist(pXstPort))
        {
            pXstPort->designatedPriority = pXst->rootPriority;

            pXstPort->designatedPriority.DesignatedBridgeID = pXst->BridgeIdentifier;
            pXstPort->designatedPriority.DesignatedPortID = pXstPort->portId;
            pXstPort->designatedPriority.RcvPortID = pXstPort->portId;

            if (pXstPort->pPort->sendRSTP == FALSE)
            {
                pXstPort->designatedPriority.RRootID = pXst->BridgeIdentifier;
            }
        }
        else
        {
            pXstPort->designatedPriority = pXst->rootPriority;

            pXstPort->designatedPriority.DesignatedBridgeID = pXst->BridgeIdentifier;
            pXstPort->designatedPriority.DesignatedPortID = pXstPort->portId;
            pXstPort->designatedPriority.RcvPortID = pXstPort->portId;
        }
    }

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        pXstPort = &pXst->xstPort[p];

        pXstPort->designatedTimes = pXst->rootTimes;
    }

    if (cistRecalculated && rootPriVctrChanged && \
        ((pXst->pBridge->xst[MSTP_INSTANCE_CIST].rootPriority.ExtRootPathCost != 0) || \
         (prevExtRootPathCost != 0)))
    {
        syncMaster(pXst->pBridge);
    }

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        pXstPort = &pXst->xstPort[p];
        pCistPort = pXst->xstPort[p].pCistPort;

        if (pXstPort->infoIs == INFO_Disabled)
        {
            pXstPort->selectedRole = ROLE_DisabledPort;
            continue;
        }

        if (((pCistPort->selectedRole == ROLE_RootPort) || \
              (pCistPort->selectedRole == ROLE_AlternatePort)) && \
            (pCistPort->infoIs == INFO_Received) && \
            (pCistPort->pPort->infoInternal == FALSE) && \
            !cist(pXstPort))
        {   
            if (pCistPort->selectedRole == ROLE_RootPort)
            {
                pXstPort->selectedRole = ROLE_MasterPort;
            }
            else if (pCistPort->selectedRole == ROLE_AlternatePort)
            {
                pXstPort->selectedRole = ROLE_AlternatePort;
            }

            if ((PriorityVectorCmp(&pXstPort->portPriority, \
                                   &pXstPort->designatedPriority) != 0) || \
                (TimesCmp(&pXstPort->portTimes, &pXst->rootTimes) != 0))
            {
                pXstPort->updtInfo = TRUE;
            }
        }
        else
        {   
            if (pXstPort->infoIs == INFO_Aged)
            {   
                pXstPort->updtInfo = TRUE;
                pXstPort->selectedRole = ROLE_DesignatedPort;
            }
            else if (pXstPort->infoIs == INFO_Mine)
            {   
                pXstPort->selectedRole = ROLE_DesignatedPort;
                if ((PriorityVectorCmp(&pXstPort->portPriority, \
                                       &pXstPort->designatedPriority) != 0) || \
                    (TimesCmp(&pXstPort->portTimes, &pXst->rootTimes) != 0))
                {
                    pXstPort->updtInfo = TRUE;
                }
            }
            else if (pXstPort->infoIs == INFO_Received)
            {   
                if (pXstPort->portId.number == pXst->rootPortId.number) 
                {   
                    pXstPort->selectedRole = ROLE_RootPort;
                    pXstPort->updtInfo = FALSE;  
                }
                else
                {   
                    if (PriorityVectorCmp(&pXstPort->designatedPriority, &pXstPort->portPriority) <= 0)
                    {   
                        if (cmpBridgeId(&pXstPort->portPriority.DesignatedBridgeID, &pXstPort->pXst->BridgeIdentifier) != 0)  
                        {   
                            pXstPort->selectedRole = ROLE_AlternatePort;
                            pXstPort->updtInfo = FALSE; 
                        }
                        else
                        {   
                            pXstPort->selectedRole = ROLE_BackupPort;
                            pXstPort->updtInfo = FALSE; 
                        }
                    }
                    else
                    {   
                        pXstPort->selectedRole = ROLE_DesignatedPort;
                        pXstPort->updtInfo = TRUE;
                    }
                }
            }

        }
    } 
}
static void updtRolesDisabledTree(mstp_xst_t *pXst)
{   
    mstp_u32_t p;

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        pXst->xstPort[p].selectedRole = ROLE_DisabledPort;
    }
}
static void newInfoXst(mstp_xst_port_t *pXstPort, mstp_bool_t bool)
{
    if (cist(pXstPort))
    {
        pXstPort->pPort->newInfo = bool;
    }
    else
    {
        pXstPort->pPort->newInfoMsti = bool;
    }
}

static void updtPortState(mstp_xst_port_t *pXstPort)
{
    mstp_u8_t oldPortState = pXstPort->portState;

    if (pXstPort->pBridge->BEGIN == FALSE)
    {   
        if (pXstPort->pPort->portEnabled)
        {
            if (pXstPort->forwarding)
            {
                pXstPort->portState = MSTP_PORT_STATE_FORWARDING;
            }
            else if (pXstPort->learning)
            {
                pXstPort->portState = MSTP_PORT_STATE_LEARNING;
            }
            else
            {
                pXstPort->portState = MSTP_PORT_STATE_DISCARDING;
            }
        }
        else
        {
            pXstPort->portState = MSTP_PORT_STATE_DISABLED;
        }
    }
    else
    {   
        if (pXstPort->pPort->portEnabled)
        {
            pXstPort->portState = MSTP_PORT_STATE_FORWARDING;
        }
        else
        {
            pXstPort->portState = MSTP_PORT_STATE_DISABLED;
        }
    }

    if (pXstPort->portState != oldPortState)
    {
        pXstPort->portStateChanged = TRUE;
    }
}
static void mstp_fsm_port_timers(mstp_fsm_t *pFSM, mstp_port_t *pPort)
{
    mstp_bridge_t *pBridge = pPort->pBridge;
    mstp_i32_t i;

    if (pBridge->BEGIN)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_ONE_SECOND);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_ONE_SECOND:
            {
                if (pPort->tick == TRUE)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_TICK);
                }
            }
            break;

        case STATE_TICK:
            {
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ONE_SECOND);
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_ONE_SECOND:
        {
            pPort->tick = FALSE;
        }
        break;

    case STATE_TICK:
        {
            DEC(pPort->helloWhen);
            
            for (i=0; i<MAX_MSTP_INSTANCE; i++)
            {
                if (pBridge->xst[i].xstEnabled == FALSE)
                {
                    continue;
                }

                DEC(pPort->pXstPort[i]->tcWhile);
                DEC(pPort->pXstPort[i]->fdWhile);
                DEC(pPort->pXstPort[i]->rcvdInfoWhile);
                DEC(pPort->pXstPort[i]->rrWhile);
                DEC(pPort->pXstPort[i]->rbWhile);
            }
            DEC(pPort->mdelayWhile);
            DEC(pPort->edgeDelayWhile);
            DEC(pPort->txCount);
#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
            DEC(pPort->loopbackDelayWhile);
#endif
        }
        break;

    default:
        
        break;
    }
}
static void mstp_fsm_port_receive(mstp_fsm_t *pFSM, mstp_port_t *pPort)
{
    mstp_bridge_t *pBridge = pPort->pBridge;

    if (pBridge->BEGIN || \
        ((pPort->rcvdBpdu || pPort->edgeDelayWhile != MigrateTime(pBridge)) && !pPort->portEnabled))
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISCARD);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_DISCARD:
            {
                if (pPort->rcvdBpdu && pPort->portEnabled)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_RECEIVE);
                }
            }
            break;

        case STATE_RECEIVE:
            {
                if (pPort->rcvdBpdu && pPort->portEnabled && !rcvdAnyMsg(pPort))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_RECEIVE);
                }
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_DISCARD:
        {
            pPort->rcvdBpdu = pPort->rcvdRSTP = pPort->rcvdSTP = FALSE;
            clearAllRcvdMsgs(pPort);
            pPort->edgeDelayWhile = MigrateTime(pBridge);
        }
        break;

    case STATE_RECEIVE:
        {
            updtBPDUVersion(pPort);
            pPort->rcvdInternal = fromSameRegion(pPort);
            setRcvdMsgs(pPort);
            pPort->operEdge = pPort->rcvdBpdu = FALSE;
            pPort->edgeDelayWhile = MigrateTime(pBridge);
        }
        break;

    default:
        
        break;
    }
}
static void mstp_fsm_port_protocol_migration(mstp_fsm_t *pFSM, mstp_port_t *pPort)
{
    mstp_bridge_t *pBridge = pPort->pBridge;

    if (pBridge->BEGIN)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_CHECKING_RSTP);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_CHECKING_RSTP:
            {
                
                if (pPort->mdelayWhile == 0)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_SENSING);
                }
                else if ((pPort->mdelayWhile != MigrateTime(pBridge)) && \
                         (!pPort->portEnabled))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_CHECKING_RSTP);
                }
            }
            break;

        case STATE_SELECTING_STP:
            {
                if ((pPort->mdelayWhile == 0) || \
                    !pPort->portEnabled || pPort->mcheck)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_SENSING);
                }
            }
            break;

        case STATE_SENSING:
            {
                
                if (!pPort->portEnabled || pPort->mcheck || \
                    ((rstpVersion(pBridge)) && !pPort->sendRSTP && pPort->rcvdRSTP))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_CHECKING_RSTP);
                }
                else if (pPort->sendRSTP && pPort->rcvdSTP)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_SELECTING_STP);
                }
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_CHECKING_RSTP:
        {
            pPort->mcheck = FALSE;
            pPort->sendRSTP = rstpVersion(pBridge);
            pPort->mdelayWhile = MigrateTime(pBridge);
        }
        break;

    case STATE_SELECTING_STP:
        {
            pPort->sendRSTP = FALSE;
            pPort->mdelayWhile = MigrateTime(pBridge);
        }
        break;

    case STATE_SENSING:
        {
            pPort->rcvdRSTP = pPort->rcvdSTP = FALSE;
        }
        break;

    default:
        
        break;
    }
}
static void mstp_fsm_bridge_detection(mstp_fsm_t *pFSM, mstp_port_t *pPort)
{
    mstp_bridge_t *pBridge = pPort->pBridge;

    if ((pBridge->BEGIN || !pPort->portEnabled) && AdminEdge(pPort))
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_EDGE);
    }
    
    else if ((pBridge->BEGIN || !pPort->portEnabled) && !AdminEdge(pPort))
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_NOT_EDGE);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_EDGE:
            {
                if ((!pPort->portEnabled && !AdminEdge(pPort)) || !pPort->operEdge)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_NOT_EDGE);
                }
            }
            break;

        case STATE_NOT_EDGE:
            {
                if ((!pPort->portEnabled && AdminEdge(pPort)) || \
                    ((pPort->edgeDelayWhile == 0) && AutoEdge(pPort) \
                     && pPort->sendRSTP && pPort->pCistPort->proposing))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_EDGE);
                }
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_EDGE:
        {
            pPort->operEdge = TRUE;
        }
        break;

    case STATE_NOT_EDGE:
        {
            pPort->operEdge = FALSE;
        }
        break;

    default:
        
        break;
    }
}
static void mstp_fsm_port_transmit(mstp_fsm_t *pFSM, mstp_port_t *pPort)
{
    mstp_bridge_t *pBridge = pPort->pBridge;

#if 0
    if ((pBridge->BEGIN || !pPort->portEnabled) \
        && allTransmitReady(pPort))
#endif
    if (pBridge->BEGIN || !pPort->portEnabled)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_TRANSMIT_INIT);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_TRANSMIT_INIT:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_IDLE);
            }
            break;

        case STATE_TRANSMIT_PERIODIC:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_IDLE);
            }
            break;

        case STATE_TRANSMIT_CONFIG:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_IDLE);
            }
            break;

        case STATE_TRANSMIT_TCN:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_IDLE);
            }
            break;

        case STATE_TRANSMIT_RSTP:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_IDLE);
            }
            break;

        case STATE_IDLE:
            {
                if ((pPort->helloWhen == 0) \
                    && allTransmitReady(pPort))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_TRANSMIT_PERIODIC);
                }
                else if ((!pPort->sendRSTP && pPort->newInfo && cistDesignatedPort(pPort) && \
                          (pPort->txCount < TxHoldCount(pPort)) && (pPort->helloWhen != 0)) \
                         && allTransmitReady(pPort))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_TRANSMIT_CONFIG);
                }
                else if ((!pPort->sendRSTP && pPort->newInfo && cistRootPort(pPort) && \
                          (pPort->txCount < TxHoldCount(pPort)) && (pPort->helloWhen != 0)) \
                         && allTransmitReady(pPort))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_TRANSMIT_TCN);
                }
                else if ((pPort->sendRSTP && \
                          (pPort->newInfo || (pPort->newInfoMsti && !mstiMasterPort(pPort))) && \
                          (pPort->txCount < TxHoldCount(pPort)) && (pPort->helloWhen != 0)) \
                         && allTransmitReady(pPort))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_TRANSMIT_RSTP);
                }
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_TRANSMIT_INIT:
        {
            pPort->newInfo = pPort->newInfoMsti = TRUE;
            pPort->txCount = 0;
        }
        break;

    case STATE_TRANSMIT_PERIODIC:
        {
            pPort->newInfo = pPort->newInfo || (cistDesignatedPort(pPort) || \
                             (cistRootPort(pPort) && (pPort->pCistPort->tcWhile != 0)));
            pPort->newInfoMsti = pPort->newInfoMsti || \
                                 mstiDesignatedOrTCpropagatingRootPort(pPort);
        }
        break;

    case STATE_TRANSMIT_CONFIG:
        {
            pPort->newInfo = FALSE;
            txConfig(pPort);
            pPort->txCount += 1;
            pPort->tcAck = FALSE;
        }
        break;

    case STATE_TRANSMIT_TCN:
        {
            pPort->newInfo = FALSE;
            txTcn(pPort);
            pPort->txCount += 1;
        }
        break;

    case STATE_TRANSMIT_RSTP:
        {
            pPort->newInfo = pPort->newInfoMsti = FALSE;
            txMstp(pPort);
            pPort->txCount += 1;
            pPort->tcAck = FALSE;
        }
        break;

    case STATE_IDLE:
        {
            pPort->helloWhen = HelloTime(pPort);
        }
        break;

    default:
        
        break;
    }
}
static void mstp_fsm_port_information(mstp_fsm_t *pFSM, mstp_xst_port_t *pXstPort)
{
    mstp_bridge_t *pBridge = pXstPort->pBridge;
    mstp_port_t *pPort = pXstPort->pPort;

    if ((!pPort->portEnabled && (pXstPort->infoIs != INFO_Disabled)) || pBridge->BEGIN)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISABLED);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_DISABLED:
            {
                
                if (pXstPort->rcvdMsg)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISABLED);
                }
                else if (pPort->portEnabled)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_AGED);
                }
            }
            break;

        case STATE_AGED:
            {
                if (pXstPort->selected && pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_UPDATE);
                }
            }
            break;

        case STATE_UPDATE:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_CURRENT);
            }
            break;

        case STATE_SUPERIOR_DESIGNATED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_CURRENT);
            }
            break;

        case STATE_REPEATED_DESIGNATED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_CURRENT);
            }
            break;

        case STATE_INFERIOR_DESIGNATED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_CURRENT);
            }
            break;

        case STATE_NOT_DESIGNATED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_CURRENT);
            }
            break;

        case STATE_OTHER:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_CURRENT);
            }
            break;

        case STATE_CURRENT:
            {
                if (pXstPort->selected && pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_UPDATE);
                }
                else if ((pXstPort->infoIs == INFO_Received) && \
                         (pXstPort->rcvdInfoWhile == 0) && \
                         !pXstPort->updtInfo && \
                         !rcvdXstMsg(pXstPort))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_AGED);
                }
                else if (rcvdXstMsg(pXstPort) && !updtXstInfo(pXstPort))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_RECEIVE);
                }
            }
            break;

        case STATE_RECEIVE:
            {
                if (pXstPort->rcvdInfo == INFO_SuperiorDesignatedInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_SUPERIOR_DESIGNATED);
                }
                else if (pXstPort->rcvdInfo == INFO_RepeatedDesignatedInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_REPEATED_DESIGNATED);
                }
                else if (pXstPort->rcvdInfo == INFO_InferiorDesignatedInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_INFERIOR_DESIGNATED);
                }
                else if (pXstPort->rcvdInfo == INFO_InferiorRootAlternateInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_NOT_DESIGNATED);
                }
                else if (pXstPort->rcvdInfo == INFO_OtherInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_OTHER);
                }
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_DISABLED:
        {
            pXstPort->rcvdMsg = FALSE;
            pXstPort->proposing = pXstPort->proposed = pXstPort->agree = pXstPort->agreed = FALSE;
            pXstPort->rcvdInfoWhile = 0;
            pXstPort->infoIs = INFO_Disabled;
            pXstPort->reselect = TRUE;
            pXstPort->selected = FALSE;
        }
        break;

    case STATE_AGED:
        {
            pXstPort->infoIs = INFO_Aged;
            pXstPort->reselect = TRUE;
            pXstPort->selected = FALSE;
        }
        break;

    case STATE_UPDATE:
        {
            pXstPort->proposing = pXstPort->proposed = FALSE;
            pXstPort->agreed = pXstPort->agreed && betterorsameInfo(INFO_Mine, pXstPort);
            pXstPort->synced = pXstPort->synced && pXstPort->agreed;
            pXstPort->portPriority = pXstPort->designatedPriority;
            pXstPort->portTimes = pXstPort->designatedTimes;
            pXstPort->updtInfo = FALSE;
            pXstPort->infoIs = INFO_Mine;
            
            newInfoXst(pXstPort, TRUE);
        }
        break;

    case STATE_SUPERIOR_DESIGNATED:
        {
            pPort->infoInternal = pPort->rcvdInternal;
            pXstPort->agreed = pXstPort->proposing = FALSE;
            recordProposal(pXstPort);
            setTcFlags(pXstPort);
            pXstPort->agree = pXstPort->agree && betterorsameInfo(INFO_Received, pXstPort);
            recordAgreement(pXstPort);
            pXstPort->synced = pXstPort->synced && pXstPort->agreed;
            recordPriority(pXstPort);
            recordTimes(pXstPort);
            updtRcvdInfoWhile(pXstPort);
            pXstPort->infoIs = INFO_Received;
            pXstPort->reselect = TRUE;
            pXstPort->selected = FALSE;
            pXstPort->rcvdMsg = FALSE;

            if (cist(pXstPort))
            {
                mstp_i32_t i;

                for (i=1; i<MAX_MSTP_INSTANCE; i++)
                {
                    if ((pXstPort->pBridge->xst[i].xstEnabled == FALSE) ||
                        (!pXstPort->pPort->portEnabled))
                    {
                        continue;
                    }

                    pBridge->xst[i].xstPort[pXstPort->portIdx].reselect = TRUE;
                    pBridge->xst[i].xstPort[pXstPort->portIdx].selected = FALSE;
                }
            }
        }
        break;

    case STATE_REPEATED_DESIGNATED:
        {
            
            if (cist(pXstPort) && (pPort->infoInternal != pPort->rcvdInternal))
            {
                mstp_u32_t xstIdx;

                for (xstIdx=0; xstIdx<MAX_MSTP_INSTANCE; xstIdx++)
                {
                    pXstPort->pXst[xstIdx].xstPort[pXstPort->portIdx].reselect = TRUE;
                    pXstPort->pXst[xstIdx].xstPort[pXstPort->portIdx].selected = FALSE;
                }
            }
            pPort->infoInternal = pPort->rcvdInternal;
            recordProposal(pXstPort);
            setTcFlags(pXstPort);
            recordAgreement(pXstPort);
            updtRcvdInfoWhile(pXstPort);
            pXstPort->rcvdMsg = FALSE;
        }
        break;

    case STATE_INFERIOR_DESIGNATED:
        {
            recordDispute(pXstPort);
            pXstPort->rcvdInfoWhile = 0;    
            pXstPort->rcvdMsg = FALSE;
        }
        break;

    case STATE_NOT_DESIGNATED:
        {
            recordAgreement(pXstPort);
            setTcFlags(pXstPort);
            pXstPort->rcvdMsg = FALSE;
        }
        break;

    case STATE_OTHER:
        {
            pXstPort->rcvdMsg = FALSE;
        }
        break;

    case STATE_CURRENT:
        {
            
        }
        break;

    case STATE_RECEIVE:
        {
            pXstPort->rcvdInfo = rcvInfo(pXstPort);
            recordMastered(pXstPort);

            if (cist(pXstPort) && (pPort->infoInternal != pPort->rcvdInternal))
            {
                mstp_i32_t i;

                for (i=0; i<MAX_MSTP_INSTANCE; i++)
                {
                    if ((pXstPort->pBridge->xst[i].xstEnabled == FALSE) ||
                        (!pXstPort->pPort->portEnabled))
                    {
                        continue;
                    }

                    pBridge->xst[i].xstPort[pXstPort->portIdx].reselect = TRUE;
                    pBridge->xst[i].xstPort[pXstPort->portIdx].selected = FALSE;
                }
            }
        }
        break;

    default:
        
        break;
    }

    if (cist(pXstPort) && (pXstPort->reselect == TRUE) && (pXstPort->selected == FALSE))
    {
        mstp_i32_t i;

        for (i=1; i<MAX_MSTP_INSTANCE; i++)
        {
            if ((pXstPort->pBridge->xst[i].xstEnabled == FALSE) ||
                (!pXstPort->pPort->portEnabled))
            {
                continue;
            }

            pBridge->xst[i].xstPort[pXstPort->portIdx].reselect = TRUE;
            pBridge->xst[i].xstPort[pXstPort->portIdx].selected = FALSE;
        }
    }
}
static void mstp_fsm_port_role_selection(mstp_fsm_t *pFSM, mstp_xst_t *pXst)
{
    mstp_bridge_t *pBridge = pXst->pBridge;
    mstp_i32_t j;

    if (pBridge->BEGIN)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_INIT_TREE);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_INIT_TREE:
            {
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROLE_SELECTION);
            }
            break;

        case STATE_ROLE_SELECTION:
            {
                for (j=0; j<MAX_MSTP_PORT_NUM; j++)
                {
                    if (pXst->xstPort[j].reselect)
                    {
                        MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROLE_SELECTION);
                        break;
                    }
                }
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_INIT_TREE:
        {
            updtRolesDisabledTree(pXst);
        }
        break;

    case STATE_ROLE_SELECTION:
        {
            clearReselectTree(pXst);
            updtRolesTree(pXst);
            setSelectedTree(pXst);
        }
        break;

    default:
        
        break;
    }
}
static void mstp_fsm_port_role_transitions(mstp_fsm_t *pFSM, mstp_xst_port_t *pXstPort)
{
    mstp_bridge_t *pBridge = pXstPort->pBridge;
    mstp_port_t *pPort = pXstPort->pPort;

    if (pBridge->BEGIN || !pPort->portEnabled)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_INIT_PORT);
    }
    else if (((pXstPort->selectedRole == ROLE_DisabledPort) && \
              (pXstPort->role != pXstPort->selectedRole)) \
             && pXstPort->selected && !pXstPort->updtInfo)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISABLE_PORT);
    }
    else if (((pXstPort->selectedRole == ROLE_MasterPort) && \
              (pXstPort->role != pXstPort->selectedRole)) \
             && pXstPort->selected && !pXstPort->updtInfo)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_PORT);
    }
    else if (((pXstPort->selectedRole == ROLE_RootPort) && \
              (pXstPort->role != pXstPort->selectedRole)) \
             && pXstPort->selected && !pXstPort->updtInfo)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PORT);
    }
    else if (((pXstPort->selectedRole == ROLE_DesignatedPort) && \
              (pXstPort->role != pXstPort->selectedRole)) \
             && pXstPort->selected && !pXstPort->updtInfo)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_PORT);
    }
    else if ((((pXstPort->selectedRole == ROLE_AlternatePort) || \
               (pXstPort->selectedRole == ROLE_BackupPort))&& \
              (pXstPort->role != pXstPort->selectedRole)) \
             && pXstPort->selected && !pXstPort->updtInfo)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_BLOCK_PORT);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_INIT_PORT:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISABLE_PORT);
            }
            break;

        case STATE_DISABLE_PORT:
            {
                if ((!pXstPort->learning && !pXstPort->forwarding) \
                    && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISABLED_PORT);
                }
            }
            break;

        case STATE_DISABLED_PORT:
            {
                if (((pXstPort->fdWhile != MaxAge(pPort)) || \
                     pXstPort->sync || pXstPort->reRoot || !pXstPort->synced) \
                    && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISABLED_PORT);
                }
            }
            break;

        case STATE_MASTER_PROPOSED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_PORT);
            }
            break;

        case STATE_MASTER_AGREED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_PORT);
            }
            break;

        case STATE_MASTER_SYNCED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_PORT);
            }
            break;

        case STATE_MASTER_RETIRED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_PORT);
            }
            break;

        case STATE_MASTER_FORWARD:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_PORT);
            }
            break;

        case STATE_MASTER_LEARN:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_PORT);
            }
            break;

        case STATE_MASTER_DISCARD:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_PORT);
            }
            break;

        case STATE_MASTER_PORT:
            {
                if ((pXstPort->proposed && !pXstPort->agree) \
                    && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_PROPOSED);
                }
                else if (((allSynced(pXstPort) && !pXstPort->agree) || \
                          (pXstPort->proposed && pXstPort->agree)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_AGREED);
                }
                else if (((!pXstPort->learning && !pXstPort->forwarding && !pXstPort->synced) || \
                          (pXstPort->agreed && !pXstPort->synced) || \
                          (pPort->operEdge && !pXstPort->synced) || \
                          (pXstPort->sync && pXstPort->synced)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_SYNCED);
                }
                else if ((pXstPort->reRoot && (pXstPort->rrWhile == 0)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_RETIRED);
                }
                else if ((((pXstPort->fdWhile == 0) || allSynced(pXstPort)) && \
                          (pXstPort->learn && !pXstPort->forward)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_FORWARD);
                }
                else if ((((pXstPort->fdWhile == 0) || allSynced(pXstPort)) && \
                          !pXstPort->learn) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_LEARN);
                }
                else if ((((pXstPort->sync && !pXstPort->synced) || \
                           (pXstPort->reRoot && (pXstPort->rrWhile != 0)) || \
                           pXstPort->disputed) && \
                          !pPort->operEdge && (pXstPort->learn || pXstPort->forward)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_MASTER_DISCARD);
                }
            }
            break;

        case STATE_ROOT_PROPOSED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PORT);
            }
            break;

        case STATE_ROOT_AGREED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PORT);
            }
            break;

        case STATE_ROOT_SYNCED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PORT);
            }
            break;

        case STATE_REROOT:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PORT);
            }
            break;

        case STATE_ROOT_FORWARD:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PORT);
            }
            break;

        case STATE_ROOT_LEARN:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PORT);
            }
            break;

        case STATE_REROOTED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PORT);
            }
            break;

        case STATE_ROOT_PORT:
            {
                if ((pXstPort->proposed && !pXstPort->agree) \
                    && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PROPOSED);
                }
                else if (((allSynced(pXstPort) && !pXstPort->agree) || \
                          (pXstPort->proposed && pXstPort->agree)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_AGREED);
                }
                else if (((pXstPort->agreed && !pXstPort->synced) || \
                          (pXstPort->sync && pXstPort->synced)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_SYNCED);
                }
                else if ((!pXstPort->forward && !pXstPort->reRoot) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_REROOT);
                }
                else if ((((pXstPort->fdWhile == 0) || \
                           ((reRooted(pXstPort) && (pXstPort->rbWhile == 0)) && \
                            (rstpVersion(pBridge)))) && \
                          pXstPort->learn && !pXstPort->forward) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_FORWARD);
                }
                else if ((((pXstPort->fdWhile == 0) || \
                           ((reRooted(pXstPort) && (pXstPort->rbWhile == 0)) && \
                            (rstpVersion(pBridge)))) && \
                          !pXstPort->learn) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_LEARN);
                }
                else if ((pXstPort->reRoot && pXstPort->forward) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_REROOTED);
                }
                else if ((pXstPort->rrWhile != FwdDelay(pPort)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ROOT_PORT);
                }
            }
            break;

        case STATE_DESIGNATED_PROPOSE:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_PORT);
            }
            break;

        case STATE_DESIGNATED_AGREED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_PORT);
            }
            break;

        case STATE_DESIGNATED_SYNCED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_PORT);
            }
            break;

        case STATE_DESIGNATED_RETIRED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_PORT);
            }
            break;

        case STATE_DESIGNATED_FORWARD:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_PORT);
            }
            break;

        case STATE_DESIGNATED_LEARN:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_PORT);
            }
            break;

        case STATE_DESIGNATED_DISCARD:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_PORT);
            }
            break;

        case STATE_DESIGNATED_PORT:
            {
                if ((!pXstPort->forward && !pXstPort->agreed && \
                     !pXstPort->proposing && !pPort->operEdge) \
                    && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_PROPOSE);
                }
                else if ((allSynced(pXstPort) && (pXstPort->proposed || !pXstPort->agree)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_AGREED);
                }
                else if (((!pXstPort->learning && !pXstPort->forwarding && !pXstPort->synced) || \
                          (pXstPort->agreed && !pXstPort->synced) || \
                          (pPort->operEdge && !pXstPort->synced) || \
                          (pXstPort->sync && pXstPort->synced)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_SYNCED);
                }
                else if ((pXstPort->reRoot && (pXstPort->rrWhile == 0)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_RETIRED);
                }
                else if ((((pXstPort->fdWhile == 0) || pXstPort->agreed || pPort->operEdge) && \
                          ((pXstPort->rrWhile == 0) || !pXstPort->reRoot) && \
                          !pXstPort->sync && (pXstPort->learn && !pXstPort->forward)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_FORWARD);
                }
                else if ((((pXstPort->fdWhile == 0) || pXstPort->agreed || pPort->operEdge) && \
                          ((pXstPort->rrWhile == 0) || !pXstPort->reRoot) && \
                          !pXstPort->sync && !pXstPort->learn) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_LEARN);
                }
                else if ((((pXstPort->sync && !pXstPort->synced) || \
                           (pXstPort->reRoot && (pXstPort->rrWhile != 0)) || \
                           pXstPort->disputed) &&\
                          !pPort->operEdge && \
                          (pXstPort->learn || pXstPort->forward)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DESIGNATED_DISCARD);
                }
            }
            break;

        case STATE_ALTERNATE_PROPOSED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ALTERNATE_PORT);
            }
            break;

        case STATE_ALTERNATE_AGREED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ALTERNATE_PORT);
            }
            break;

        case STATE_BLOCK_PORT:
            {
                if ((!pXstPort->learning && !pXstPort->forwarding) \
                    && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ALTERNATE_PORT);
                }
            }
            break;

        case STATE_BACKUP_PORT:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ALTERNATE_PORT);
            }
            break;

        case STATE_ALTERNATE_PORT:
            {
                if ((pXstPort->proposed && !pXstPort->agree) \
                    && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ALTERNATE_PROPOSED);
                }
                else if (((allSynced(pXstPort) && !pXstPort->agree) || \
                          (pXstPort->proposed && pXstPort->agree)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ALTERNATE_AGREED);
                }
                else if (((pXstPort->rbWhile != 2 * HelloTime(pPort)) && \
                          (pXstPort->role == ROLE_BackupPort)) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_BACKUP_PORT);
                }
                else if (((pXstPort->fdWhile != forwardDelay(pPort)) || \
                          pXstPort->sync || pXstPort->reRoot || !pXstPort->synced) \
                         && pXstPort->selected && !pXstPort->updtInfo)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ALTERNATE_PORT);
                }
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_INIT_PORT:
        {
            pXstPort->role = ROLE_DisabledPort;
            pXstPort->learn = pXstPort->forward = FALSE;
            pXstPort->synced = FALSE;
            pXstPort->sync = pXstPort->reRoot = TRUE;
            pXstPort->rrWhile = FwdDelay(pPort);
            pXstPort->fdWhile = MaxAge(pPort);
            pXstPort->rbWhile = 0;
        }
        break;

    case STATE_DISABLE_PORT:
        {
            pXstPort->role = pXstPort->selectedRole;
            pXstPort->learn = pXstPort->forward = FALSE;
        }
        break;

    case STATE_DISABLED_PORT:
        {
            pXstPort->fdWhile = MaxAge(pPort);
            pXstPort->synced = TRUE;
            pXstPort->rrWhile = 0;
            pXstPort->sync = pXstPort->reRoot = FALSE;
        }
        break;

    case STATE_MASTER_PROPOSED:
        {
            setSyncTree(pXstPort);
            pXstPort->proposed = FALSE;
        }
        break;

    case STATE_MASTER_AGREED:
        {
            pXstPort->proposed = pXstPort->sync = FALSE;
            pXstPort->agree = TRUE;
        }
        break;

    case STATE_MASTER_SYNCED:
        {
            pXstPort->rrWhile = 0;
            pXstPort->synced = TRUE;
            pXstPort->sync = FALSE;
        }
        break;

    case STATE_MASTER_RETIRED:
        {
            pXstPort->reRoot = FALSE;
        }
        break;

    case STATE_MASTER_FORWARD:
        {
            pXstPort->forward = TRUE;
            pXstPort->fdWhile = 0;
            pXstPort->agreed = pPort->sendRSTP;
        }
        break;

    case STATE_MASTER_LEARN:
        {
            pXstPort->learn = TRUE;
            pXstPort->fdWhile = forwardDelay(pPort);
        }
        break;

    case STATE_MASTER_DISCARD:
        {
            pXstPort->learn = pXstPort->forward = pXstPort->disputed = FALSE;
            pXstPort->fdWhile = forwardDelay(pPort);
        }
        break;

    case STATE_MASTER_PORT:
        {
            pXstPort->role = ROLE_MasterPort;
        }
        break;

    case STATE_ROOT_PROPOSED:
        {
            setSyncTree(pXstPort);
            pXstPort->proposed = FALSE;
        }
        break;

    case STATE_ROOT_AGREED:
        {
            pXstPort->proposed = pXstPort->sync = FALSE;
            pXstPort->agree = TRUE;
            
            newInfoXst(pXstPort, TRUE);
        }
        break;

    case STATE_ROOT_SYNCED:
        {
            pXstPort->synced = TRUE;
            pXstPort->sync = FALSE;
        }
        break;

    case STATE_REROOT:
        {
            setReRootTree(pXstPort);
        }
        break;

    case STATE_ROOT_FORWARD:
        {
            pXstPort->fdWhile = 0;
            pXstPort->forward = TRUE;
        }
        break;

    case STATE_ROOT_LEARN:
        {
            pXstPort->fdWhile = forwardDelay(pPort);
            pXstPort->learn = TRUE;
        }
        break;

    case STATE_REROOTED:
        {
            pXstPort->reRoot = FALSE;
        }
        break;

    case STATE_ROOT_PORT:
        {
            pXstPort->role = ROLE_RootPort;
            pXstPort->rrWhile = FwdDelay(pPort);
        }
        break;

    case STATE_DESIGNATED_PROPOSE:
        {
            pXstPort->proposing = TRUE;
            if (cist(pXstPort))
            {
                pPort->edgeDelayWhile = EdgeDelay(pXstPort);
            }
            
            newInfoXst(pXstPort, TRUE);
        }
        break;

    case STATE_DESIGNATED_AGREED:
        {
            pXstPort->proposed = pXstPort->sync = FALSE;
            pXstPort->agree = TRUE;
            
            newInfoXst(pXstPort, TRUE);
        }
        break;

    case STATE_DESIGNATED_SYNCED:
        {
            pXstPort->rrWhile = 0;
            pXstPort->synced = TRUE;
            pXstPort->sync = FALSE;
        }
        break;

    case STATE_DESIGNATED_RETIRED:
        {
            pXstPort->reRoot = FALSE;
        }
        break;

    case STATE_DESIGNATED_FORWARD:
        {
            pXstPort->forward = TRUE;
            pXstPort->fdWhile = 0;
            pXstPort->agreed = pPort->sendRSTP;
        }
        break;

    case STATE_DESIGNATED_LEARN:
        {
            pXstPort->learn = TRUE;
            pXstPort->fdWhile = forwardDelay(pPort);
        }
        break;

    case STATE_DESIGNATED_DISCARD:
        {
            pXstPort->learn = pXstPort->forward = pXstPort->disputed = FALSE;
            pXstPort->fdWhile = forwardDelay(pPort);
        }
        break;

    case STATE_DESIGNATED_PORT:
        {
            pXstPort->role = ROLE_DesignatedPort;
        }
        break;

    case STATE_ALTERNATE_PROPOSED:
        {
            setSyncTree(pXstPort);
            pXstPort->proposed = FALSE;
        }
        break;

    case STATE_ALTERNATE_AGREED:
        {
            pXstPort->proposed = FALSE;
            pXstPort->agree = TRUE;
            
            newInfoXst(pXstPort, TRUE);
        }
        break;

    case STATE_BLOCK_PORT:
        {
            pXstPort->role = pXstPort->selectedRole;
            pXstPort->learn = pXstPort->forward = FALSE;
        }
        break;

    case STATE_BACKUP_PORT:
        {
            pXstPort->rbWhile = 2 * HelloTime(pPort);
        }
        break;

    case STATE_ALTERNATE_PORT:
        {
            pXstPort->fdWhile = forwardDelay(pPort);
            pXstPort->synced = TRUE;
            pXstPort->rrWhile = 0;
            pXstPort->sync = pXstPort->reRoot = FALSE;
        }
        break;

    default:
        
        break;
    }
}
static void mstp_fsm_port_state_transition(mstp_fsm_t *pFSM, mstp_xst_port_t *pXstPort)
{
    mstp_bridge_t *pBridge = pXstPort->pBridge;
    mstp_port_t *pPort = pXstPort->pPort;

    if (pBridge->BEGIN || !pPort->portEnabled)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISCARDING);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_DISCARDING:
            {
                if (pXstPort->learn)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_LEARNING);
                }
            }
            break;

        case STATE_LEARNING:
            {
                
                if (pXstPort->forward)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_FORWARDING);
                }
                else if (!pXstPort->learn)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISCARDING);
                }
            }
            break;

        case STATE_FORWARDING:
            {
                if (!pXstPort->forward)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DISCARDING);
                }
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_DISCARDING:
        {
            disableLearning(pXstPort);
            pXstPort->learning = FALSE;
            disableForwarding(pXstPort);
            pXstPort->forwarding = FALSE;
        }
        break;

    case STATE_LEARNING:
        {
            enableLearning(pXstPort);
            pXstPort->learning = TRUE;
        }
        break;

    case STATE_FORWARDING:
        {
            enableForwarding(pXstPort);
            pXstPort->forwarding = TRUE;
            
            pXstPort->proposing = FALSE;
        }
        break;

    default:
        
        break;
    }
}
static void mstp_fsm_topology_change(mstp_fsm_t *pFSM, mstp_xst_port_t *pXstPort)
{
    mstp_bridge_t *pBridge = pXstPort->pBridge;
    mstp_port_t *pPort = pXstPort->pPort;

    if (pBridge->BEGIN)
    {
        MSTP_FSM_CHANGE_STATE(pFSM, STATE_INACTIVE);
    }
    else
    {
        switch (pFSM->state)
        {
        case STATE_INACTIVE:
            {
                if (pXstPort->learn && !pXstPort->fdbFlush)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_LEARNING);
                }
            }
            break;

        case STATE_LEARNING:
            {
                if (((pXstPort->role == ROLE_RootPort) || \
                     (pXstPort->role == ROLE_DesignatedPort) || \
                     (pXstPort->role == ROLE_MasterPort)) && \
                    pXstPort->forward && !pPort->operEdge)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_DETECTED);
                }
                else if ((pXstPort->role != ROLE_RootPort) && \
                         (pXstPort->role != ROLE_DesignatedPort) && \
                         (pXstPort->role != ROLE_MasterPort) && \
                         !(pXstPort->learn || pXstPort->learning) && \
                         !(pXstPort->rcvdTc || pPort->rcvdTcn || \
                           pPort->rcvdTcAck || pXstPort->tcProp))
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_INACTIVE);
                }
                else if (pXstPort->rcvdTc || pPort->rcvdTcn || \
                         pPort->rcvdTcAck || pXstPort->tcProp)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_LEARNING);
                }
            }
            break;

        case STATE_DETECTED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ACTIVE);
            }
            break;

        case STATE_NOTIFIED_TCN:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_NOTIFIED_TC);
            }
            break;

        case STATE_NOTIFIED_TC:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ACTIVE);
            }
            break;

        case STATE_PROPAGATING:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ACTIVE);
            }
            break;

        case STATE_ACKNOWLEDGED:
            {
                
                MSTP_FSM_CHANGE_STATE(pFSM, STATE_ACTIVE);
            }
            break;

        case STATE_ACTIVE:
            {
                
                if (((pXstPort->role != ROLE_RootPort) && \
                     (pXstPort->role != ROLE_DesignatedPort) && \
                     (pXstPort->role != ROLE_MasterPort)) || \
                     pPort->operEdge)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_LEARNING);
                }
                else if (pPort->rcvdTcn)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_NOTIFIED_TCN);
                }
                else if (pXstPort->rcvdTc)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_NOTIFIED_TC);
                }
                else if (pXstPort->tcProp && !pPort->operEdge)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_PROPAGATING);
                }
                else if (pPort->rcvdTcAck)
                {
                    MSTP_FSM_CHANGE_STATE(pFSM, STATE_ACKNOWLEDGED);
                }
            }
            break;

        default:
            
            break;
        }
    }

    if (pFSM->changeState == FALSE)
    {
        return;
    }
    pFSM->changeState = FALSE;

    switch (pFSM->state)
    {
    case STATE_INACTIVE:
        {
            pXstPort->fdbFlush = TRUE;
            pXstPort->tcWhile = 0;
            if (cist(pXstPort))
            {
                pPort->tcAck = FALSE;
            }
        }
        break;

    case STATE_LEARNING:
        {
            if (cist(pXstPort))
            {
                pXstPort->rcvdTc = pPort->rcvdTcn = pPort->rcvdTcAck = FALSE;
            }
            pXstPort->rcvdTc = pXstPort->tcProp = FALSE;
        }
        break;

    case STATE_DETECTED:
        {
            newTcWhile(pXstPort);
            setTcPropTree(pXstPort);
            
            newInfoXst(pXstPort, TRUE);
        }
        break;

    case STATE_NOTIFIED_TCN:
        {
            newTcWhile(pXstPort);
        }
        break;

    case STATE_NOTIFIED_TC:
        {
            if (cist(pXstPort))
            {
                pPort->rcvdTcn = FALSE;
            }
            pXstPort->rcvdTc = FALSE;
            if (cist(pXstPort) && (pXstPort->role == ROLE_DesignatedPort))
            {
                pPort->tcAck = TRUE;
            }
            setTcPropTree(pXstPort);
        }
        break;

    case STATE_PROPAGATING:
        {
            newTcWhile(pXstPort);
            pXstPort->fdbFlush = TRUE;
            pXstPort->tcProp = FALSE;
        }
        break;

    case STATE_ACKNOWLEDGED:
        {
            pXstPort->tcWhile = 0;
            pPort->rcvdTcAck = FALSE;
        }
        break;

    case STATE_ACTIVE:
        {
            
        }
        break;

    default:
        
        break;
    }
}
void mstp_bridge_init(mstp_bridge_t *pBridge, mstp_func_ops_t *pOps)
{
    mstp_i32_t i, p;
    mstp_xst_port_t *pXstPort;
    mstp_xst_t *pXst;
    mstp_port_t *pPort;
    sys_mac_t port_mac;

    pBridge->resetBridge = FALSE;
    pBridge->BEGIN = TRUE;
    pBridge->MstConfigId.idFormatSelector = 0x00;
    pBridge->MstConfigId.name[0] = '\0';
    pBridge->MstConfigId.revisionLevel = 0;
    pBridge->MstConfigId.digest[0] = 0x00;
    
    pBridge->ForceProtocolVersion = MSTP_DFLT_FORCE_VERSION;
    pBridge->BridgeForwardDelay = MSTP_DFLT_FORWARD_DELAY;
    pBridge->TransmitHoldCount = MSTP_DFLT_TRANSMIT_HOLD_COUNT;
    pBridge->MigrateTime = MSTP_DFLT_MIGRATE_TIME;
    pBridge->BridgeMaxAge = MSTP_DFLT_MAX_AGE;
    pBridge->MaxHops = MSTP_DFLT_MAX_HOPS;
    pBridge->helloTime = MSTP_DFLT_HELLO_TIME;
    pBridge->cbf_pkt_tx = pOps->mstp_pkt_tx_cb_f;
    pBridge->cbf_fdb_flush = pOps->mstp_fdb_flush_cb_f;
    pBridge->cbf_portState_change = pOps->mstp_portState_change_cb_f;

    for (p = 0;  p < MAX_MSTP_PORT_NUM; p++)
    {
        pPort = &pBridge->port[p];
        pPort->pBridge = pBridge;
        pPort->pCistPort = &pBridge->xst[MSTP_INSTANCE_CIST].xstPort[p];

        for (i=0; i<MAX_MSTP_INSTANCE; i++)
        {
            pPort->pXstPort[i] = &pBridge->xst[i].xstPort[p];
        }

        pPort->portIdx = p;

        pPort->adminEnable = ENABLED;
#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
        pPort->adminLoopbackGuard = MSTP_DFLT_PORT_LOOPBACK_GUARD;
#endif
#ifdef CONFIG_SYS_STP_BPDU_FILTER
        pPort->adminBpduFilter = MSTP_DFLT_PORT_BPDU_FILTER;
#endif
#ifdef CONFIG_SYS_STP_BPDU_GUARD
        pPort->adminBpduGuard = MSTP_DFLT_PORT_BPDU_GUARD;
#endif
        pPort->adminPointToPointMAC = MSTP_DFLT_PORT_POINT_TO_POINT_MAC;
        pPort->autoPointToPointMAC = TRUE;
        pPort->operPointToPointMAC = MSTP_DFLT_PORT_POINT_TO_POINT_MAC;
        pPort->PortHelloTime = pBridge->helloTime;
        pPort->AdminEdgePort = MSTP_DFLT_PORT_EDGE;
        
        pPort->AutoEdge = FALSE;
        pPort->PortIdentifierPriority = MSTP_DFLT_PORT_PRIORITY;
        pPort->ExternalPortPathCost = MSTP_DFLT_PORT_EXT_PATH_COST;
        
        pPort->operEdge = MSTP_DFLT_PORT_EDGE;
        pPort->portEnabled = MSTP_DFLT_PORT_ENABLE;
        pPort->restrictedRole = MSTP_DFLT_PORT_RESTRICTED_ROLE;
        pPort->restrictedTcn = MSTP_DFLT_PORT_RESTRICTED_TCN;

        SYS_MEM_CLEAR(port_mac);
        rsd_sys_portMacAddr_get(pPort->portIdx, &port_mac);
        osal_memcpy(pPort->ethAddrSrc, &port_mac, ETHER_ADDR_LEN);
        
        pPort->cntRxConfig = 0;
        pPort->cntRxTcn = 0;
        pPort->cntRxMstp = 0;
        pPort->cntTxConfig = 0;
        pPort->cntTxTcn = 0;
        pPort->cntTxMstp = 0;
    }

    for (i=0; i<MAX_MSTP_INSTANCE; i++)
    {
        for (p=0; p<MAX_MSTP_PORT_NUM; p++)
        {
            pXstPort = &pBridge->xst[i].xstPort[p];

            pXstPort->pBridge = pBridge;
            pXstPort->pPort = &pBridge->port[p];
            pXstPort->pXst = &pBridge->xst[i];
            pXstPort->pCist = &pBridge->xst[MSTP_INSTANCE_CIST];
            pXstPort->pCistPort = &pBridge->xst[MSTP_INSTANCE_CIST].xstPort[p];

            pXstPort->xstIdx = i;
            pXstPort->portIdx = p;  

            pXstPort->portId.priority = pXstPort->pPort->PortIdentifierPriority;
            pXstPort->portId.number = p + 1;    

            pXstPort->InternalPortPathCost = 0;

            pXstPort->msgPriority.RootID.priority = 0x0000;
            pXstPort->msgPriority.RootID.addr[0] = 0x00;
            pXstPort->msgPriority.RootID.addr[1] = 0x00;
            pXstPort->msgPriority.RootID.addr[2] = 0x00;
            pXstPort->msgPriority.RootID.addr[3] = 0x00;
            pXstPort->msgPriority.RootID.addr[4] = 0x00;
            pXstPort->msgPriority.RootID.addr[5] = 0x00;
            pXstPort->msgPriority.ExtRootPathCost = 0;

            if (MSTP_INSTANCE_CIST == i)
            {   
                pXstPort->portTimes.MessageAge = MSTP_DFLT_MESSAGE_AGE;
                pXstPort->portTimes.MaxAge = MSTP_DFLT_MAX_AGE;
                pXstPort->portTimes.ForwardDelay = MSTP_DFLT_FORWARD_DELAY;
                pXstPort->portTimes.HelloTime = pXstPort->pPort->PortHelloTime;
                pXstPort->portTimes.remainingHops = 0;
                pXstPort->portTimes.MaxHops = 0;
            }
            else
            {   
                pXstPort->portTimes.remainingHops = 0;
            }

            pXstPort->portStateChanged = TRUE;
            pXstPort->xstFlag = SYS_INST_MSTP;
        }

        pXst = &pBridge->xst[i];
        pXst->xstFlag = SYS_INST_MSTP;
        pXst->pBridge = pBridge;

        pXst->xstIdx = i;
        pXst->xstEnabled= (i == MSTP_INSTANCE_CIST)? TRUE : FALSE;
        pXst->tcTimer = 0;
        pXst->tcTimes = 0;

        pXst->BridgeIdentifierPriority = ((MSTP_DFLT_BRIDGE_PRIORITY & (0xF000)) | (i & 0x0FFF));

        pXst->BridgeIdentifier.priority = pXst->BridgeIdentifierPriority;
        pXst->BridgeIdentifier.addr[0] = pBridge->macAddr[0];
        pXst->BridgeIdentifier.addr[1] = pBridge->macAddr[1];
        pXst->BridgeIdentifier.addr[2] = pBridge->macAddr[2];
        pXst->BridgeIdentifier.addr[3] = pBridge->macAddr[3];
        pXst->BridgeIdentifier.addr[4] = pBridge->macAddr[4];
        pXst->BridgeIdentifier.addr[5] = pBridge->macAddr[5];

        pXst->BridgeTimes.MessageAge = 0;
        pXst->BridgeTimes.MaxAge = pBridge->BridgeMaxAge;
        pXst->BridgeTimes.ForwardDelay = pBridge->BridgeForwardDelay;
        pXst->BridgeTimes.HelloTime = pBridge->helloTime;
        pXst->BridgeTimes.remainingHops = 0;
        pXst->BridgeTimes.MaxHops = pBridge->MaxHops;
    }

    pBridge->pSelfValid = pBridge;

    mstp_bridge_routine(pBridge);
}
void mstp_bridge_exit(mstp_bridge_t *pBridge)
{
    
    if (pBridge == NULL)
    {
        return;
    }

    pBridge->pSelfValid = NULL;
}
void mstp_bridge_fsm(mstp_bridge_t *pBridge)
{
    mstp_i32_t i, p;
    mstp_xst_port_t *pXstPort = NULL;
    mstp_port_t *pPort;
    mstp_bool_t origBegin = pBridge->BEGIN;

    if ((pBridge == NULL) || (pBridge->pSelfValid != pBridge))
    {
        return;
    }

    if (pBridge->resetBridge)
    {
        pBridge->BEGIN = TRUE;
    }

    do
    {
        
        for (i=0; i<MAX_MSTP_INSTANCE; i++)
        {
        	if(pBridge->xst[i].xstEnabled == FALSE)
        	{
				continue;
        	}
            for (p=0; p<MAX_MSTP_PORT_NUM; p++)
            {
                
                pXstPort = &pBridge->xst[i].xstPort[p];

                mstp_fsm_port_information(&pXstPort->fsmPortInformation, pXstPort);

                mstp_fsm_port_role_transitions(&pXstPort->fsmPortRoleTransitions, pXstPort);

                mstp_fsm_port_state_transition(&pXstPort->fsmPortStateTransition, pXstPort);

                mstp_fsm_topology_change(&pXstPort->fsmTopologyChange, pXstPort);
            }

            mstp_fsm_port_role_selection(&pXstPort->pXst->fsmPortRoleSelection, &pBridge->xst[i]);
        }

        for (p=0; p<MAX_MSTP_PORT_NUM; p++)
        {
            
            pPort = &pBridge->port[p];

            mstp_fsm_port_timers(&pPort->fsmPortTimers, pPort);

            mstp_fsm_port_protocol_migration(&pPort->fsmPortProtocolMigration, pPort);

            mstp_fsm_port_receive(&pPort->fsmPortReceive, pPort);

            mstp_fsm_port_transmit(&pPort->fsmPortTransmit, pPort);

            mstp_fsm_bridge_detection(&pPort->fsmBridgeDetection, pPort);
        }
    }
    while (0);

    if (pBridge->resetBridge)
    {
        pBridge->resetBridge = FALSE;
        pBridge->BEGIN = origBegin;

        mstp_bridge_routine(pBridge);
    }
}
void mstp_bridge_reset(mstp_bridge_t *pBridge)
{
    
    if ((pBridge == NULL) || (pBridge->pSelfValid != pBridge))
    {
        return;
    }

    pBridge->resetBridge = TRUE;
}
void mstp_bridge_routine(mstp_bridge_t *pBridge)
{
    mstp_i32_t i, p;
    mstp_xst_port_t *pXstPort;

#if 0   
    mstp_port_t *pPort;
#endif

    if ((pBridge == NULL) || (pBridge->pSelfValid != pBridge))
    {
        return;
    }

    do
    {
        for (i=0; i<MAX_MSTP_INSTANCE; i++)
        {
            if (pBridge->xst[i].xstEnabled == FALSE)
            {
                continue;
            }

            for (p=0; p<MAX_MSTP_PORT_NUM; p++)
            {
                
                pXstPort = &pBridge->xst[i].xstPort[p];
#ifdef CONFIG_SYS_PROTO_ERPS
				if ((pBridge->xst[i].xstFlag & (1 << SYS_INST_ERPS)) && (pXstPort->xstFlag & (1 << SYS_INST_ERPS)))
				{
					continue;
				}
#endif
#ifdef CONFIG_SYS_PROTO_LOOPBACK
				if ((pBridge->xst[i].xstFlag & (1 << SYS_INST_LOOPBACK)) && (pXstPort->xstFlag & (1 << SYS_INST_LOOPBACK)))
				{
					continue;
				}

#endif

                updtPortState(pXstPort);
                if ((pBridge->cbf_portState_change) && \
                    (pXstPort->portStateChanged))
                {
                    pXstPort->portStateChanged = FALSE;
                    pBridge->cbf_portState_change(pXstPort->xstIdx, \
                        pXstPort->portIdx, pXstPort->portState);
                }

                if ((pXstPort->pPort->portEnabled) && \
                    (pXstPort->fdbFlush))
                {
                    if ((!pXstPort->pPort->operEdge) &&
                        (pBridge->cbf_fdb_flush))
                    {   
                        pBridge->cbf_fdb_flush(pXstPort->xstIdx, pXstPort->portIdx);
                    }

                    pXstPort->fdbFlush = FALSE;
                }
            }

        }

#if 0   
        for (p=0; p<MAX_MSTP_PORT_NUM; p++)
        {
            
            pPort = &pBridge->port[p];

        }
#endif
    }
    while (0);
}
void mstp_bridge_tick(mstp_bridge_t *pBridge)
{
    mstp_u32_t i, p;

    if ((pBridge == NULL) || (pBridge->pSelfValid != pBridge))
    {
        return;
    }

    for (i=0; i<MAX_MSTP_INSTANCE; i++)
    {
        if ((pBridge->BEGIN == FALSE) && \
            (pBridge->xst[i].xstEnabled))
        {
            INC(pBridge->xst[i].tcTimer);
        }
        else
        {
            pBridge->xst[i].tcTimer = 0;
        }
    }

    for (p=0; p<MAX_MSTP_PORT_NUM; p++)
    {
        pBridge->port[p].tick = TRUE;
    }
}
int32 mstp_bridge_pkt_rx(mstp_bridge_t * pBridge, mstp_u32_t portId, mstp_u8_t * pData, mstp_i32_t len)
{
    mstp_port_t *pPort;
    
    mstp_u32_t i;
    mstp_u16_t mstId;

    if ((pBridge == NULL) || (pBridge->pSelfValid != pBridge))
    {
        return SYS_ERR_OK;
    }

    if (pBridge->port[portId].rcvdBpdu)
    {
        return SYS_ERR_OK;
    }

    if (len < (MSTP_BPDU_LEN_BASE + MSTP_BPDU_LEN_TCN))
        return SYS_ERR_OK;

    pPort = &pBridge->port[portId];

    pPort->bpduRxCist.ethLen = (pData[12] << 8) | (pData[13] << 0);
    if (pPort->bpduRxCist.ethLen < (MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_TCN))
        return SYS_ERR_OK;

    pPort->bpduRxCist.numMsti = 0;

    pPort->bpduRxCist.protoId = (pData[MSTP_BPDU_LEN_BASE + 0] << 8) | \
                                (pData[MSTP_BPDU_LEN_BASE + 1] << 0);
    
    pPort->bpduRxCist.protoVerId = pData[MSTP_BPDU_LEN_BASE + 2];
    
    pPort->bpduRxCist.bpduType = pData[MSTP_BPDU_LEN_BASE + 3];

    if ((pPort->bpduRxCist.protoId == MSTP_BPDU_PROTO_ID) && \
        (pPort->bpduRxCist.bpduType == MSTP_BPDU_TYPE_CONFIG) && \
        (pPort->bpduRxCist.ethLen >= (MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_CONFIG)))
    {   
        pPort->bpduRxCist.operBpduType = MSTP_BPDU_OPER_STP_CONFIG;
    }
    else if ((pPort->bpduRxCist.protoId == MSTP_BPDU_PROTO_ID) && \
             (pPort->bpduRxCist.bpduType == MSTP_BPDU_TYPE_TCN) && \
             (pPort->bpduRxCist.ethLen >= (MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_TCN)))
    {   
        pPort->bpduRxCist.operBpduType = MSTP_BPDU_OPER_STP_TCN;
    }
    else if ((pPort->bpduRxCist.protoId == MSTP_BPDU_PROTO_ID) && \
             (pPort->bpduRxCist.protoVerId == MSTP_BPDU_VERSION_RSTP) && \
             (pPort->bpduRxCist.bpduType == MSTP_BPDU_TYPE_RST) && \
             (pPort->bpduRxCist.ethLen >= (MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_RST)))
    {   
        pPort->bpduRxCist.operBpduType = MSTP_BPDU_OPER_STP_RST;
    }
    else if ((pPort->bpduRxCist.protoId == MSTP_BPDU_PROTO_ID) && \
             (pPort->bpduRxCist.protoVerId >= MSTP_BPDU_VERSION_MSTP) && \
             (pPort->bpduRxCist.bpduType == MSTP_BPDU_TYPE_RST))
    {   
        if ((pPort->bpduRxCist.ethLen >= (MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_CONFIG) && \
            (pPort->bpduRxCist.ethLen <= (MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_MST))))
        {   
            pPort->bpduRxCist.operBpduType = MSTP_BPDU_OPER_STP_RST;
        }
        else if (pPort->bpduRxCist.ethLen >= (MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_RST))
        {
            
            pPort->bpduRxCist.ver1Len = pData[MSTP_BPDU_LEN_BASE + 35];
            if (pPort->bpduRxCist.ver1Len != 0)
            {   
                pPort->bpduRxCist.operBpduType = MSTP_BPDU_OPER_STP_RST;
            }
            else
            {
                if (pPort->bpduRxCist.ethLen >= (MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_MST))
                {
                    
                    pPort->bpduRxCist.ver3Len = (pData[MSTP_BPDU_LEN_BASE + 36] << 8) | \
                                                (pData[MSTP_BPDU_LEN_BASE + 37] << 0);
                    pPort->bpduRxCist.numMsti = ((pPort->bpduRxCist.ver3Len & 0xFFF0) - 0x40) >> 4;
                    if (((pPort->bpduRxCist.ver3Len & 0x000F) == 0x0000) && \
                        (pPort->bpduRxCist.ethLen == MSTP_BPDU_LEN_LLC + MSTP_BPDU_LEN_MST + \
                         (MSTP_BPDU_LEN_MSTI * pPort->bpduRxCist.numMsti)))
                    {   
                        pPort->bpduRxCist.operBpduType = MSTP_BPDU_OPER_STP_MST;
                    }
                    else
                    {   
                        pPort->bpduRxCist.operBpduType = MSTP_BPDU_OPER_STP_RST;
                    }
                }
                else
                {
                    return SYS_ERR_OK;
                }
            }
        }
        else
        {
            return SYS_ERR_OK;
        }
    }
    else
    {   
        return SYS_ERR_OK; 
    }

    switch (pPort->bpduRxCist.operBpduType)
    {
    case MSTP_BPDU_OPER_STP_CONFIG:
        pPort->cntRxConfig += 1;

#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
        if (pPort->adminLoopbackGuard == TRUE && pPort->loopbackDelayWhile == 0)
        {
            
            if (((pData[6]  == pPort->ethAddrSrc[0])  &&   
                 (pData[7]  == pPort->ethAddrSrc[1])  &&
                 (pData[8]  == pPort->ethAddrSrc[2])  &&
                 (pData[9]  == pPort->ethAddrSrc[3])  &&
                 (pData[10] == pPort->ethAddrSrc[4])  &&
                 (pData[11] == pPort->ethAddrSrc[5])) &&
                ((pData[MSTP_BPDU_LEN_BASE + 17] ==        
                  ((pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 8) & 0xFF)) &&
                 (pData[MSTP_BPDU_LEN_BASE + 18] ==
                  ((pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 0) & 0xFF)) &&
                 (pData[MSTP_BPDU_LEN_BASE + 19] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[0]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 20] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[1]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 21] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[2]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 22] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[3]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 23] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[4]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 24] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[5])) &&
                (pData[MSTP_BPDU_LEN_BASE + 25] ==          
                 (((pPort->pCistPort->designatedPriority.DesignatedPortID.priority >> 0) & 0xF0) |
                  ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 8) & 0x0F)) &&
                 (pData[MSTP_BPDU_LEN_BASE + 26] ==
                  ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 0) & 0xFF))))
            {
                return SYS_ERR_STP_BPDU_SELFLOOP;
            }
        }
#endif

        pPort->bpduRxCist.flags = pData[MSTP_BPDU_LEN_BASE + 4] & \
                                  (MSTP_BPDU_FLAG_TC | MSTP_BPDU_FLAG_TC_ACK);
        
        pPort->bpduRxCist.rootId.priority = (pData[MSTP_BPDU_LEN_BASE + 5] << 8) |
                                            (pData[MSTP_BPDU_LEN_BASE + 6] << 0);
        pPort->bpduRxCist.rootId.addr[0] = pData[MSTP_BPDU_LEN_BASE + 7];
        pPort->bpduRxCist.rootId.addr[1] = pData[MSTP_BPDU_LEN_BASE + 8];
        pPort->bpduRxCist.rootId.addr[2] = pData[MSTP_BPDU_LEN_BASE + 9];
        pPort->bpduRxCist.rootId.addr[3] = pData[MSTP_BPDU_LEN_BASE + 10];
        pPort->bpduRxCist.rootId.addr[4] = pData[MSTP_BPDU_LEN_BASE + 11];
        pPort->bpduRxCist.rootId.addr[5] = pData[MSTP_BPDU_LEN_BASE + 12];
        
        pPort->bpduRxCist.externalPathCost = (pData[MSTP_BPDU_LEN_BASE + 13] << 24) | \
                                             (pData[MSTP_BPDU_LEN_BASE + 14] << 16) | \
                                             (pData[MSTP_BPDU_LEN_BASE + 15] << 8) | \
                                             (pData[MSTP_BPDU_LEN_BASE + 16] << 0);
        
        pPort->bpduRxCist.regionalRootId.priority = (pData[MSTP_BPDU_LEN_BASE + 17] << 8) | \
                                                    (pData[MSTP_BPDU_LEN_BASE + 18] << 0);
        pPort->bpduRxCist.regionalRootId.addr[0] = pData[MSTP_BPDU_LEN_BASE + 19];
        pPort->bpduRxCist.regionalRootId.addr[1] = pData[MSTP_BPDU_LEN_BASE + 20];
        pPort->bpduRxCist.regionalRootId.addr[2] = pData[MSTP_BPDU_LEN_BASE + 21];
        pPort->bpduRxCist.regionalRootId.addr[3] = pData[MSTP_BPDU_LEN_BASE + 22];
        pPort->bpduRxCist.regionalRootId.addr[4] = pData[MSTP_BPDU_LEN_BASE + 23];
        pPort->bpduRxCist.regionalRootId.addr[5] = pData[MSTP_BPDU_LEN_BASE + 24];
        
        pPort->bpduRxCist.portId.priority = pData[MSTP_BPDU_LEN_BASE + 25] & 0xF0;
        pPort->bpduRxCist.portId.number = ((pData[MSTP_BPDU_LEN_BASE + 25] << 8) | \
                                           (pData[MSTP_BPDU_LEN_BASE + 26] << 0)) & 0x0FFF;
        
        pPort->bpduRxCist.messageAge = (pData[MSTP_BPDU_LEN_BASE + 27] << 8) | \
                                       (pData[MSTP_BPDU_LEN_BASE + 28] << 0);
        
        pPort->bpduRxCist.maxAge = (pData[MSTP_BPDU_LEN_BASE + 29] << 8) | \
                                   (pData[MSTP_BPDU_LEN_BASE + 30] << 0);
        
        pPort->bpduRxCist.helloTime = (pData[MSTP_BPDU_LEN_BASE + 31] << 8) | \
                                      (pData[MSTP_BPDU_LEN_BASE + 32] << 8);
        
        pPort->bpduRxCist.forwardDelay = (pData[MSTP_BPDU_LEN_BASE + 33] << 8) | \
                                         (pData[MSTP_BPDU_LEN_BASE + 34] << 0);
        
        pPort->bpduRxCist.internalRootPathCost = 0;
        pPort->bpduRxCist.bridgeId = pPort->bpduRxCist.regionalRootId;
        
        if (pPort->bpduRxCist.messageAge >= pPort->bpduRxCist.maxAge)
        {
            
            return SYS_ERR_OK;
        }

        if ((pPort->bpduRxCist.rootId.addr[0] == pPort->bpduRxCist.regionalRootId.addr[0]) && \
            (pPort->bpduRxCist.rootId.addr[1] == pPort->bpduRxCist.regionalRootId.addr[1]) && \
            (pPort->bpduRxCist.rootId.addr[2] == pPort->bpduRxCist.regionalRootId.addr[2]) && \
            (pPort->bpduRxCist.rootId.addr[3] == pPort->bpduRxCist.regionalRootId.addr[3]) && \
            (pPort->bpduRxCist.rootId.addr[4] == pPort->bpduRxCist.regionalRootId.addr[4]) && \
            (pPort->bpduRxCist.rootId.addr[5] == pPort->bpduRxCist.regionalRootId.addr[5]) && \
            (pPort->bpduRxCist.rootId.priority != pPort->bpduRxCist.regionalRootId.priority) && \
            (pPort->bpduRxCist.messageAge != 0))
        {
            
            return SYS_ERR_OK;
        }
        else if ((pPort->bpduRxCist.rootId.addr[0] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[0]) && \
                 (pPort->bpduRxCist.rootId.addr[1] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[1]) && \
                 (pPort->bpduRxCist.rootId.addr[2] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[2]) && \
                 (pPort->bpduRxCist.rootId.addr[3] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[3]) && \
                 (pPort->bpduRxCist.rootId.addr[4] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[4]) && \
                 (pPort->bpduRxCist.rootId.addr[5] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[5]) && \
                 (pPort->bpduRxCist.rootId.priority != pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.priority) && \
                 (pPort->bpduRxCist.messageAge != 0))
        {
            
            return SYS_ERR_OK;
        }

        break;

    case MSTP_BPDU_OPER_STP_TCN:
        pPort->cntRxTcn += 1;

        break;

    case MSTP_BPDU_OPER_STP_RST:
        pPort->cntRxMstp += 1;

#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
        if (pPort->adminLoopbackGuard == TRUE && pPort->loopbackDelayWhile == 0)
        {
            
            if (((pData[6]  == pPort->ethAddrSrc[0])  &&   
                 (pData[7]  == pPort->ethAddrSrc[1])  &&
                 (pData[8]  == pPort->ethAddrSrc[2])  &&
                 (pData[9]  == pPort->ethAddrSrc[3])  &&
                 (pData[10] == pPort->ethAddrSrc[4])  &&
                 (pData[11] == pPort->ethAddrSrc[5])) &&
                ((pData[MSTP_BPDU_LEN_BASE + 17] ==        
                  ((pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 8) & 0xFF)) &&
                 (pData[MSTP_BPDU_LEN_BASE + 18] ==
                  ((pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 0) & 0xFF)) &&
                 (pData[MSTP_BPDU_LEN_BASE + 19] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[0]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 20] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[1]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 21] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[2]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 22] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[3]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 23] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[4]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 24] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[5])) &&
                (pData[MSTP_BPDU_LEN_BASE + 25] ==          
                 (((pPort->pCistPort->designatedPriority.DesignatedPortID.priority >> 0) & 0xF0) |
                  ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 8) & 0x0F)) &&
                 (pData[MSTP_BPDU_LEN_BASE + 26] ==
                  ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 0) & 0xFF))))
            {
                return SYS_ERR_STP_BPDU_SELFLOOP;
            }
        }
#endif

        pPort->bpduRxCist.flags = pData[MSTP_BPDU_LEN_BASE + 4] & \
                                  (MSTP_BPDU_FLAG_TC | \
                                   MSTP_BPDU_FLAG_PROPOSAL | \
                                   MSTP_BPDU_FLAG_PORT_ROLE | \
                                   MSTP_BPDU_FLAG_LEARNING | \
                                   MSTP_BPDU_FLAG_FORWARDING | \
                                   MSTP_BPDU_FLAG_AGREEMENT);
        
        pPort->bpduRxCist.rootId.priority = (pData[MSTP_BPDU_LEN_BASE + 5] << 8) |
                                            (pData[MSTP_BPDU_LEN_BASE + 6] << 0);
        pPort->bpduRxCist.rootId.addr[0] = pData[MSTP_BPDU_LEN_BASE + 7];
        pPort->bpduRxCist.rootId.addr[1] = pData[MSTP_BPDU_LEN_BASE + 8];
        pPort->bpduRxCist.rootId.addr[2] = pData[MSTP_BPDU_LEN_BASE + 9];
        pPort->bpduRxCist.rootId.addr[3] = pData[MSTP_BPDU_LEN_BASE + 10];
        pPort->bpduRxCist.rootId.addr[4] = pData[MSTP_BPDU_LEN_BASE + 11];
        pPort->bpduRxCist.rootId.addr[5] = pData[MSTP_BPDU_LEN_BASE + 12];
        
        pPort->bpduRxCist.externalPathCost = (pData[MSTP_BPDU_LEN_BASE + 13] << 24) | \
                                             (pData[MSTP_BPDU_LEN_BASE + 14] << 16) | \
                                             (pData[MSTP_BPDU_LEN_BASE + 15] << 8) | \
                                             (pData[MSTP_BPDU_LEN_BASE + 16] << 0);
        
        pPort->bpduRxCist.regionalRootId.priority = (pData[MSTP_BPDU_LEN_BASE + 17] << 8) | \
                                                    (pData[MSTP_BPDU_LEN_BASE + 18] << 0);
        pPort->bpduRxCist.regionalRootId.addr[0] = pData[MSTP_BPDU_LEN_BASE + 19];
        pPort->bpduRxCist.regionalRootId.addr[1] = pData[MSTP_BPDU_LEN_BASE + 20];
        pPort->bpduRxCist.regionalRootId.addr[2] = pData[MSTP_BPDU_LEN_BASE + 21];
        pPort->bpduRxCist.regionalRootId.addr[3] = pData[MSTP_BPDU_LEN_BASE + 22];
        pPort->bpduRxCist.regionalRootId.addr[4] = pData[MSTP_BPDU_LEN_BASE + 23];
        pPort->bpduRxCist.regionalRootId.addr[5] = pData[MSTP_BPDU_LEN_BASE + 24];
        
        pPort->bpduRxCist.portId.priority = pData[MSTP_BPDU_LEN_BASE + 25] & 0xF0;
        pPort->bpduRxCist.portId.number = ((pData[MSTP_BPDU_LEN_BASE + 25] << 8) | \
                                           (pData[MSTP_BPDU_LEN_BASE + 26] << 0)) & 0x0FFF;
        
        pPort->bpduRxCist.messageAge = (pData[MSTP_BPDU_LEN_BASE + 27] << 8) | \
                                       (pData[MSTP_BPDU_LEN_BASE + 28] << 0);
        
        pPort->bpduRxCist.maxAge = (pData[MSTP_BPDU_LEN_BASE + 29] << 8) | \
                                   (pData[MSTP_BPDU_LEN_BASE + 30] << 0);
        
        pPort->bpduRxCist.helloTime = (pData[MSTP_BPDU_LEN_BASE + 31] << 8) | \
                                      (pData[MSTP_BPDU_LEN_BASE + 32] << 8);
        
        pPort->bpduRxCist.forwardDelay = (pData[MSTP_BPDU_LEN_BASE + 33] << 8) | \
                                         (pData[MSTP_BPDU_LEN_BASE + 34] << 0);
        
        pPort->bpduRxCist.ver1Len = pData[MSTP_BPDU_LEN_BASE + 35];
        
        pPort->bpduRxCist.internalRootPathCost = 0;
        pPort->bpduRxCist.bridgeId = pPort->bpduRxCist.regionalRootId;
        
        if (pPort->bpduRxCist.messageAge >= pPort->bpduRxCist.maxAge)
        {
            
            return SYS_ERR_OK;
        }

        if ((pPort->bpduRxCist.rootId.addr[0] == pPort->bpduRxCist.regionalRootId.addr[0]) && \
            (pPort->bpduRxCist.rootId.addr[1] == pPort->bpduRxCist.regionalRootId.addr[1]) && \
            (pPort->bpduRxCist.rootId.addr[2] == pPort->bpduRxCist.regionalRootId.addr[2]) && \
            (pPort->bpduRxCist.rootId.addr[3] == pPort->bpduRxCist.regionalRootId.addr[3]) && \
            (pPort->bpduRxCist.rootId.addr[4] == pPort->bpduRxCist.regionalRootId.addr[4]) && \
            (pPort->bpduRxCist.rootId.addr[5] == pPort->bpduRxCist.regionalRootId.addr[5]) && \
            (pPort->bpduRxCist.rootId.priority != pPort->bpduRxCist.regionalRootId.priority) && \
            (pPort->bpduRxCist.messageAge != 0))
        {
            
            return SYS_ERR_OK;
        }
        else if ((pPort->bpduRxCist.rootId.addr[0] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[0]) && \
                 (pPort->bpduRxCist.rootId.addr[1] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[1]) && \
                 (pPort->bpduRxCist.rootId.addr[2] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[2]) && \
                 (pPort->bpduRxCist.rootId.addr[3] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[3]) && \
                 (pPort->bpduRxCist.rootId.addr[4] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[4]) && \
                 (pPort->bpduRxCist.rootId.addr[5] == pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.addr[5]) && \
                 (pPort->bpduRxCist.rootId.priority != pBridge->xst[MSTP_INSTANCE_CIST].BridgeIdentifier.priority) && \
                 (pPort->bpduRxCist.messageAge != 0))
        {
            
            return SYS_ERR_OK;
        }
        break;

    case MSTP_BPDU_OPER_STP_MST:
        pPort->cntRxMstp += 1;

#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
        if (pPort->adminLoopbackGuard == TRUE && pPort->loopbackDelayWhile == 0)
        {
            
            if (((pData[6]  == pPort->ethAddrSrc[0])  &&   
                 (pData[7]  == pPort->ethAddrSrc[1])  &&
                 (pData[8]  == pPort->ethAddrSrc[2])  &&
                 (pData[9]  == pPort->ethAddrSrc[3])  &&
                 (pData[10] == pPort->ethAddrSrc[4])  &&
                 (pData[11] == pPort->ethAddrSrc[5])) &&
                ((pData[MSTP_BPDU_LEN_BASE + 93] ==        
                  ((pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 8) & 0xFF)) &&
                 (pData[MSTP_BPDU_LEN_BASE + 94] ==
                  ((pPort->pCistPort->designatedPriority.DesignatedBridgeID.priority >> 0) & 0xFF)) &&
                 (pData[MSTP_BPDU_LEN_BASE + 95] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[0]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 96] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[1]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 97] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[2]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 98] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[3]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 99] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[4]) &&
                 (pData[MSTP_BPDU_LEN_BASE + 100] ==
                  pPort->pCistPort->designatedPriority.DesignatedBridgeID.addr[5])) &&
                (pData[MSTP_BPDU_LEN_BASE + 25] ==          
                 (((pPort->pCistPort->designatedPriority.DesignatedPortID.priority >> 0) & 0xF0) |
                  ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 8) & 0x0F)) &&
                 (pData[MSTP_BPDU_LEN_BASE + 26] ==
                  ((pPort->pCistPort->designatedPriority.DesignatedPortID.number >> 0) & 0xFF))))
            {
                return SYS_ERR_STP_BPDU_SELFLOOP;
            }
        }
#endif

        pPort->bpduRxCist.flags = pData[MSTP_BPDU_LEN_BASE + 4] & \
                                  (MSTP_BPDU_FLAG_TC | \
                                   MSTP_BPDU_FLAG_PROPOSAL | \
                                   MSTP_BPDU_FLAG_PORT_ROLE | \
                                   MSTP_BPDU_FLAG_LEARNING | \
                                   MSTP_BPDU_FLAG_FORWARDING | \
                                   MSTP_BPDU_FLAG_AGREEMENT);
        
        pPort->bpduRxCist.rootId.priority = (pData[MSTP_BPDU_LEN_BASE + 5] << 8) |
                                            (pData[MSTP_BPDU_LEN_BASE + 6] << 0);
        pPort->bpduRxCist.rootId.addr[0] = pData[MSTP_BPDU_LEN_BASE + 7];
        pPort->bpduRxCist.rootId.addr[1] = pData[MSTP_BPDU_LEN_BASE + 8];
        pPort->bpduRxCist.rootId.addr[2] = pData[MSTP_BPDU_LEN_BASE + 9];
        pPort->bpduRxCist.rootId.addr[3] = pData[MSTP_BPDU_LEN_BASE + 10];
        pPort->bpduRxCist.rootId.addr[4] = pData[MSTP_BPDU_LEN_BASE + 11];
        pPort->bpduRxCist.rootId.addr[5] = pData[MSTP_BPDU_LEN_BASE + 12];
        
        pPort->bpduRxCist.externalPathCost = (pData[MSTP_BPDU_LEN_BASE + 13] << 24) | \
                                             (pData[MSTP_BPDU_LEN_BASE + 14] << 16) | \
                                             (pData[MSTP_BPDU_LEN_BASE + 15] << 8) | \
                                             (pData[MSTP_BPDU_LEN_BASE + 16] << 0);
        
        pPort->bpduRxCist.regionalRootId.priority = (pData[MSTP_BPDU_LEN_BASE + 17] << 8) | \
                                                    (pData[MSTP_BPDU_LEN_BASE + 18] << 0);
        pPort->bpduRxCist.regionalRootId.addr[0] = pData[MSTP_BPDU_LEN_BASE + 19];
        pPort->bpduRxCist.regionalRootId.addr[1] = pData[MSTP_BPDU_LEN_BASE + 20];
        pPort->bpduRxCist.regionalRootId.addr[2] = pData[MSTP_BPDU_LEN_BASE + 21];
        pPort->bpduRxCist.regionalRootId.addr[3] = pData[MSTP_BPDU_LEN_BASE + 22];
        pPort->bpduRxCist.regionalRootId.addr[4] = pData[MSTP_BPDU_LEN_BASE + 23];
        pPort->bpduRxCist.regionalRootId.addr[5] = pData[MSTP_BPDU_LEN_BASE + 24];
        
        pPort->bpduRxCist.portId.priority = pData[MSTP_BPDU_LEN_BASE + 25] & 0xF0;
        pPort->bpduRxCist.portId.number = ((pData[MSTP_BPDU_LEN_BASE + 25] << 8) | \
                                           (pData[MSTP_BPDU_LEN_BASE + 26] << 0)) & 0x0FFF;
        
        pPort->bpduRxCist.messageAge = (pData[MSTP_BPDU_LEN_BASE + 27] << 8) | \
                                       (pData[MSTP_BPDU_LEN_BASE + 28] << 0);
        
        pPort->bpduRxCist.maxAge = (pData[MSTP_BPDU_LEN_BASE + 29] << 8) | \
                                   (pData[MSTP_BPDU_LEN_BASE + 30] << 0);
        
        pPort->bpduRxCist.helloTime = (pData[MSTP_BPDU_LEN_BASE + 31] << 8) | \
                                      (pData[MSTP_BPDU_LEN_BASE + 32] << 8);
        
        pPort->bpduRxCist.forwardDelay = (pData[MSTP_BPDU_LEN_BASE + 33] << 8) | \
                                         (pData[MSTP_BPDU_LEN_BASE + 34] << 0);
        
        pPort->bpduRxCist.ver1Len = pData[MSTP_BPDU_LEN_BASE + 35];
        
#if 0   
        pPort->bpduRxCist.ver3Len = (pData[MSTP_BPDU_LEN_BASE + 36] << 8) | \
                                    (pData[MSTP_BPDU_LEN_BASE + 37] << 0);
#endif

        if (pPort->bpduRxCist.messageAge >= pPort->bpduRxCist.maxAge)
        {
            
            return SYS_ERR_OK;
        }

        if ((pPort->bpduRxCist.rootId.addr[0] == pPort->bpduRxCist.regionalRootId.addr[0]) && \
            (pPort->bpduRxCist.rootId.addr[1] == pPort->bpduRxCist.regionalRootId.addr[1]) && \
            (pPort->bpduRxCist.rootId.addr[2] == pPort->bpduRxCist.regionalRootId.addr[2]) && \
            (pPort->bpduRxCist.rootId.addr[3] == pPort->bpduRxCist.regionalRootId.addr[3]) && \
            (pPort->bpduRxCist.rootId.addr[4] == pPort->bpduRxCist.regionalRootId.addr[4]) && \
            (pPort->bpduRxCist.rootId.addr[5] == pPort->bpduRxCist.regionalRootId.addr[5]) && \
            (pPort->bpduRxCist.rootId.priority != pPort->bpduRxCist.regionalRootId.priority) && \
            (pPort->bpduRxCist.messageAge != 0))
        {
            
            return SYS_ERR_OK;
        }
        
        pPort->bpduRxCist.mstConfigId.idFormatSelector = pData[MSTP_BPDU_LEN_BASE + 38];
        pPort->bpduRxCist.mstConfigId.name[0] = pData[MSTP_BPDU_LEN_BASE + 39];
        pPort->bpduRxCist.mstConfigId.name[1] = pData[MSTP_BPDU_LEN_BASE + 40];
        pPort->bpduRxCist.mstConfigId.name[2] = pData[MSTP_BPDU_LEN_BASE + 41];
        pPort->bpduRxCist.mstConfigId.name[3] = pData[MSTP_BPDU_LEN_BASE + 42];
        pPort->bpduRxCist.mstConfigId.name[4] = pData[MSTP_BPDU_LEN_BASE + 43];
        pPort->bpduRxCist.mstConfigId.name[5] = pData[MSTP_BPDU_LEN_BASE + 44];
        pPort->bpduRxCist.mstConfigId.name[6] = pData[MSTP_BPDU_LEN_BASE + 45];
        pPort->bpduRxCist.mstConfigId.name[7] = pData[MSTP_BPDU_LEN_BASE + 46];
        pPort->bpduRxCist.mstConfigId.name[8] = pData[MSTP_BPDU_LEN_BASE + 47];
        pPort->bpduRxCist.mstConfigId.name[9] = pData[MSTP_BPDU_LEN_BASE + 48];
        pPort->bpduRxCist.mstConfigId.name[10] = pData[MSTP_BPDU_LEN_BASE + 49];
        pPort->bpduRxCist.mstConfigId.name[11] = pData[MSTP_BPDU_LEN_BASE + 50];
        pPort->bpduRxCist.mstConfigId.name[12] = pData[MSTP_BPDU_LEN_BASE + 51];
        pPort->bpduRxCist.mstConfigId.name[13] = pData[MSTP_BPDU_LEN_BASE + 52];
        pPort->bpduRxCist.mstConfigId.name[14] = pData[MSTP_BPDU_LEN_BASE + 53];
        pPort->bpduRxCist.mstConfigId.name[15] = pData[MSTP_BPDU_LEN_BASE + 54];
        pPort->bpduRxCist.mstConfigId.name[16] = pData[MSTP_BPDU_LEN_BASE + 55];
        pPort->bpduRxCist.mstConfigId.name[17] = pData[MSTP_BPDU_LEN_BASE + 56];
        pPort->bpduRxCist.mstConfigId.name[18] = pData[MSTP_BPDU_LEN_BASE + 57];
        pPort->bpduRxCist.mstConfigId.name[19] = pData[MSTP_BPDU_LEN_BASE + 58];
        pPort->bpduRxCist.mstConfigId.name[20] = pData[MSTP_BPDU_LEN_BASE + 59];
        pPort->bpduRxCist.mstConfigId.name[21] = pData[MSTP_BPDU_LEN_BASE + 60];
        pPort->bpduRxCist.mstConfigId.name[22] = pData[MSTP_BPDU_LEN_BASE + 61];
        pPort->bpduRxCist.mstConfigId.name[23] = pData[MSTP_BPDU_LEN_BASE + 62];
        pPort->bpduRxCist.mstConfigId.name[24] = pData[MSTP_BPDU_LEN_BASE + 63];
        pPort->bpduRxCist.mstConfigId.name[25] = pData[MSTP_BPDU_LEN_BASE + 64];
        pPort->bpduRxCist.mstConfigId.name[26] = pData[MSTP_BPDU_LEN_BASE + 65];
        pPort->bpduRxCist.mstConfigId.name[27] = pData[MSTP_BPDU_LEN_BASE + 66];
        pPort->bpduRxCist.mstConfigId.name[28] = pData[MSTP_BPDU_LEN_BASE + 67];
        pPort->bpduRxCist.mstConfigId.name[29] = pData[MSTP_BPDU_LEN_BASE + 68];
        pPort->bpduRxCist.mstConfigId.name[30] = pData[MSTP_BPDU_LEN_BASE + 69];
        pPort->bpduRxCist.mstConfigId.name[31] = pData[MSTP_BPDU_LEN_BASE + 70];
        pPort->bpduRxCist.mstConfigId.revisionLevel = (pData[MSTP_BPDU_LEN_BASE + 71] << 8) | \
                                                      (pData[MSTP_BPDU_LEN_BASE + 72] << 0);
        pPort->bpduRxCist.mstConfigId.digest[0] = pData[MSTP_BPDU_LEN_BASE + 73];
        pPort->bpduRxCist.mstConfigId.digest[1] = pData[MSTP_BPDU_LEN_BASE + 74];
        pPort->bpduRxCist.mstConfigId.digest[2] = pData[MSTP_BPDU_LEN_BASE + 75];
        pPort->bpduRxCist.mstConfigId.digest[3] = pData[MSTP_BPDU_LEN_BASE + 76];
        pPort->bpduRxCist.mstConfigId.digest[4] = pData[MSTP_BPDU_LEN_BASE + 77];
        pPort->bpduRxCist.mstConfigId.digest[5] = pData[MSTP_BPDU_LEN_BASE + 78];
        pPort->bpduRxCist.mstConfigId.digest[6] = pData[MSTP_BPDU_LEN_BASE + 79];
        pPort->bpduRxCist.mstConfigId.digest[7] = pData[MSTP_BPDU_LEN_BASE + 80];
        pPort->bpduRxCist.mstConfigId.digest[8] = pData[MSTP_BPDU_LEN_BASE + 81];
        pPort->bpduRxCist.mstConfigId.digest[9] = pData[MSTP_BPDU_LEN_BASE + 82];
        pPort->bpduRxCist.mstConfigId.digest[10] = pData[MSTP_BPDU_LEN_BASE + 83];
        pPort->bpduRxCist.mstConfigId.digest[11] = pData[MSTP_BPDU_LEN_BASE + 84];
        pPort->bpduRxCist.mstConfigId.digest[12] = pData[MSTP_BPDU_LEN_BASE + 85];
        pPort->bpduRxCist.mstConfigId.digest[13] = pData[MSTP_BPDU_LEN_BASE + 86];
        pPort->bpduRxCist.mstConfigId.digest[14] = pData[MSTP_BPDU_LEN_BASE + 87];
        pPort->bpduRxCist.mstConfigId.digest[15] = pData[MSTP_BPDU_LEN_BASE + 88];
        
        pPort->bpduRxCist.internalRootPathCost = (pData[MSTP_BPDU_LEN_BASE + 89] << 24) | \
                                                 (pData[MSTP_BPDU_LEN_BASE + 90] << 16) | \
                                                 (pData[MSTP_BPDU_LEN_BASE + 91] << 8) | \
                                                 (pData[MSTP_BPDU_LEN_BASE + 92] << 0);
        
        pPort->bpduRxCist.bridgeId.priority = (pData[MSTP_BPDU_LEN_BASE + 93] << 8) | \
                                              (pData[MSTP_BPDU_LEN_BASE + 94] << 0);
        pPort->bpduRxCist.bridgeId.addr[0] = pData[MSTP_BPDU_LEN_BASE + 95];
        pPort->bpduRxCist.bridgeId.addr[1] = pData[MSTP_BPDU_LEN_BASE + 96];
        pPort->bpduRxCist.bridgeId.addr[2] = pData[MSTP_BPDU_LEN_BASE + 97];
        pPort->bpduRxCist.bridgeId.addr[3] = pData[MSTP_BPDU_LEN_BASE + 98];
        pPort->bpduRxCist.bridgeId.addr[4] = pData[MSTP_BPDU_LEN_BASE + 99];
        pPort->bpduRxCist.bridgeId.addr[5] = pData[MSTP_BPDU_LEN_BASE + 100];
        pPort->bpduRxCist.remainingHops = pData[MSTP_BPDU_LEN_BASE + 101];

        if (pPort->bpduRxCist.remainingHops == 0)
        {
           
            return SYS_ERR_OK;
        }

        for (i=0; i<pPort->bpduRxCist.numMsti; i++)
        {
            mstId = ((pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 1] & 0xF) << 8) | \
                    ((pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 2]) << 0);

            if ((mstId < MAX_MSTP_INSTANCE) && (pPort->pBridge->xst[mstId].xstEnabled))
            {
                
                pPort->pXstPort[mstId]->bpduRxMsti.flags = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 0];
                
                pPort->pXstPort[mstId]->bpduRxMsti.regionalRootId.priority = \
                    (pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 1] << 8) | \
                    (pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 2] << 0);
                pPort->pXstPort[mstId]->bpduRxMsti.regionalRootId.addr[0] = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 3];
                pPort->pXstPort[mstId]->bpduRxMsti.regionalRootId.addr[1] = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 4];
                pPort->pXstPort[mstId]->bpduRxMsti.regionalRootId.addr[2] = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 5];
                pPort->pXstPort[mstId]->bpduRxMsti.regionalRootId.addr[3] = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 6];
                pPort->pXstPort[mstId]->bpduRxMsti.regionalRootId.addr[4] = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 7];
                pPort->pXstPort[mstId]->bpduRxMsti.regionalRootId.addr[5] = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 8];
                
                pPort->pXstPort[mstId]->bpduRxMsti.internalRootPathCost = \
                    (pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 9] << 24) | \
                    (pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 10] << 16) | \
                    (pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 11] << 8) | \
                    (pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 12] << 0);
                
                pPort->pXstPort[mstId]->bpduRxMsti.bridgePriority = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 13];
                
                pPort->pXstPort[mstId]->bpduRxMsti.portPriority = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 14];
                
                pPort->pXstPort[mstId]->bpduRxMsti.remainingHops = \
                    pData[MSTP_BPDU_LEN_MSTI_BASE + (MSTP_BPDU_LEN_MSTI * i) + 15];

                if (pPort->pXstPort[mstId]->bpduRxMsti.remainingHops == 0)
                {
                    
                    continue;
                }

                pPort->pXstPort[mstId]->rcvdBpduMsti = TRUE;
            }
        }
        break;

    default:
        return SYS_ERR_OK;
    }

#if 0
    
    DBG_PRINTF("-------- Dump BPDU --------------------------------\n");
    DBG_PRINTF(" RxPort: %d\n", portId);
    DBG_PRINTF(" operBpduType: %d\n", pPort->bpduRxCist.operBpduType);
    DBG_PRINTF(" ethLen: %d\n", pPort->bpduRxCist.ethLen);
    DBG_PRINTF("  VerId: %d\n", pPort->bpduRxCist.protoVerId);
    DBG_PRINTF("  Flags: %02X\n", pPort->bpduRxCist.flags);
    DBG_PRINTF(" PortID: %02X-%02X\n", \
        pPort->bpduRxCist.portId.priority, \
        pPort->bpduRxCist.portId.number);
    DBG_PRINTF("---------------------------------------------------\n");
#endif

    if ((pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_CONFIG) || \
        (pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_RST) || \
        (pPort->bpduRxCist.operBpduType == MSTP_BPDU_OPER_STP_MST))
    {
        pPort->pCistPort->bpduRxMsti.flags = pPort->bpduRxCist.flags;
        pPort->pCistPort->bpduRxMsti.regionalRootId = pPort->bpduRxCist.rootId;
        pPort->pCistPort->bpduRxMsti.internalRootPathCost = pPort->bpduRxCist.externalPathCost;
        pPort->pCistPort->bpduRxMsti.bridgePriority = \
            (pPort->bpduRxCist.regionalRootId.priority >> 8) & 0xF0;
        pPort->pCistPort->bpduRxMsti.portPriority = \
            (pPort->bpduRxCist.portId.priority) & 0xF0;
        pPort->pCistPort->bpduRxMsti.remainingHops = pPort->bpduRxCist.remainingHops;
    }

    pBridge->port[portId].rcvdBpdu = TRUE;

    return SYS_ERR_OK;
}
