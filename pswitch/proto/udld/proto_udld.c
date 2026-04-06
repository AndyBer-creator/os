#include <common/sys_def.h>
#include <udld/proto_udld.h>
static int32 _udld_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
static int32 _udld_pkt_tx(uint16 opcode, sys_logic_port_t port, uint8 flag);
static rsd_nic_handler_t l2gHandler =
{
    .priority       = RSD_NIC_HANDLER_PRI_UDLD,
    .dmac           = { 0x01, 0x00, 0x0C, 0xCC, 0xCC, 0xCC },
    .dmac_caremask  = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
    .ethertype      = 0x111,
    .proto          = 0,
    .sport          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_TRAP,
    .rx_cbf         = _udld_pkt_rx,
    .pName          = "UDLD RX Thread",
    .pCookie        = NULL,
};

static const sys_udld_tok_t udld_code_values[] =
{
    { 0x00, "Reserved"},
    { 0x01, "Probe message"},
    { 0x02, "Echo message"},
    { 0x03, "Flush message"},
    { 0, ""}
};

static const sys_udld_tok_t udld_phase_values[] =
{
    { 0, "LinkDown"},
    { 1, "Inactive"},
    { 2, "LinkUp_P1"},
    { 3, "LinkUp_P2"},
    { 4, "Detection"},
    { 5, "Detection_Extension"},
    { 6, "Advertisement-Single"},
    { 7, "Advertisement-Multi"},
    { 8, "ErrorDisable"},
    { 9, "ReEstablish"},
    { 0, ""}
};

const sys_text_t text_udld_mode[] =
{
    {          "disabled"    },
    {       "normal mode"     },
    {   "aggessive mode"     },
};

static osal_mutex_t udld_neighbor_sem[SYS_LOGICPORT_NUM_MAX];
static volatile int32 udld_tick_thread_status = 0;
static bool isDeviceIdOK = FALSE;
static char system_device_id[CAPA_SYS_SN_LEN+1];
static uint16 probe_interval;
static sys_udld_port_table_t udld_portTable;
static sys_enable_t udld_global_enable = DISABLED;
static sys_udld_mode_t udld_fiber_mode = UDLD_MODE_DISABLE;

static uint8 pktTxData[TEMPLATE_TX_ULDLD] =
{
    0x01, 0x00, 0x0C, 0xCC, 0xCC, 0xCC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x28, 0xAA, 0xAA, 0x03, 0x00, 0x00, 0x0C, 0x01, 0x11, 
    0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00 ,0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#if 1
#define UDLD_DBG(fmt, args...) SYS_DBG(DBG_UDLD, fmt"", ##args)
#else
#define UDLD_DBG(fmt, args...) printk(fmt"\n", ##args)
#endif
#define UDLD_NEIGHBOR_SEM_LOCK(port)   \
    do {} while (osal_sem_mutex_take(udld_neighbor_sem[port], OSAL_SEM_WAIT_FOREVER) != SYS_ERR_OK)
#define UDLD_NEIGHBOR_SEM_UNLOCK(port) \
    do {} while (osal_sem_mutex_give(udld_neighbor_sem[port]) != SYS_ERR_OK)
static int32 _udld_echo_parse(uint8  *pEcho, sys_udld_neighbor_entry_t *pNbr, uint16 echo_length)
{
    int i;
    uint16 echo_deviceid_len, echo_portid_len;
    uint16 target = ECHO_PAIRSNUM_OFFSET;

    if (pEcho != NULL)
    {
        pNbr->echoNum = READ32(pEcho);

        if(pNbr->echoNum > CAPA_UDLD_ECHO_NUM)
           pNbr->echoNum = CAPA_UDLD_ECHO_NUM;

        pNbr->pEchoPair = osal_alloc(pNbr->echoNum * sizeof(sys_udld_echo_pair_t));
        osal_memset(pNbr->pEchoPair, 0, sizeof(pNbr->pEchoPair));

        for (i = 0; i < pNbr->echoNum; i++)
        {
            echo_deviceid_len = READ16(&pEcho[target]);
            echo_portid_len = READ16(&pEcho[target + ECHO_LEN_OFFSET + echo_deviceid_len]);
            if(target + 2*ECHO_LEN_OFFSET + echo_deviceid_len + echo_portid_len > echo_length)
            {
                return SYS_ERR_FAILED;
            }

            osal_memset(pNbr->pEchoPair[i].echo_device_id, 0, sizeof(pNbr->pEchoPair[i].echo_device_id));
            osal_memcpy(pNbr->pEchoPair[i].echo_device_id,
                &pEcho[target + ECHO_LEN_OFFSET],
                echo_deviceid_len);

            osal_memset(pNbr->pEchoPair[i].echo_port_id, 0, sizeof(pNbr->pEchoPair[i].echo_port_id));
            osal_memcpy(pNbr->pEchoPair[i].echo_port_id,
                &pEcho[target + 2*ECHO_LEN_OFFSET + echo_deviceid_len],
                echo_portid_len);

            target += 2*ECHO_LEN_OFFSET + echo_deviceid_len + echo_portid_len;
        }
    }

    return SYS_ERR_OK;
}

static int32 _udld_neighbor_free(sys_udld_neighbor_entry_t *pNbr)
{
    if(pNbr->pEchoPair != NULL)
        osal_free(pNbr->pEchoPair);

    osal_free(pNbr);

    return SYS_ERR_OK;
}

static sys_udld_neighbor_entry_t* _udld_neighbor_parse(uint8  *pUdld , uint16 tlvs_length)
{
    int32 ret;
    uint8 i =0;
    uint16 tlv_length = 0, tlv_type;
    uint16 tlv_count  = 0;
    uint8  *pTmp = NULL ;
    sys_udld_neighbor_entry_t *pNbr;

    pNbr = osal_alloc(sizeof(sys_udld_neighbor_entry_t));
    pTmp = pUdld;
    osal_memset(pNbr, 0x00, sizeof(*pNbr));

    for(i = 0; i < MAX_TLV_TYPE ; i++)
    {
        tlv_type = READ16(pTmp);
        tlv_length = READ16(pTmp + TYPE_OFFSET);

        tlv_count +=  tlv_length;

        if(tlv_length < TLV_MIN_LEN)
        {
            UDLD_DBG("_udld_neighbor_parse UDLD tlv Error: Invilad tlv length!");
            osal_free(pNbr);
            return NULL;
        }

        switch(tlv_type)
        {
            case UDLD_DEVICE_ID_TLV:
                osal_memcpy(pNbr->device_id, pTmp + TLV_HEADER_LEN, tlv_length - TLV_HEADER_LEN);
                break;
            case UDLD_PORT_ID_TLV:
                osal_memcpy(pNbr->port_id, pTmp + TLV_HEADER_LEN, tlv_length - TLV_HEADER_LEN);
                break;
            case UDLD_ECHO_TLV:
                ret = _udld_echo_parse(pTmp + TLV_HEADER_LEN, pNbr, tlv_length - TLV_HEADER_LEN);
                if(SYS_ERR_OK != ret)
                {
                    UDLD_DBG("[UDLD] Echo Parse Failed!");
                    _udld_neighbor_free(pNbr);
                    return NULL;
                }
                break;
            case UDLD_MESSAGE_INTERVAL_TLV:
                osal_memcpy(&pNbr->messageInterval, pTmp + TLV_HEADER_LEN, tlv_length - TLV_HEADER_LEN);
                break;
            case UDLD_TIMEOUT_INTERVAL_TLV:
                osal_memcpy(&pNbr->timeOutInterval, pTmp + TLV_HEADER_LEN, tlv_length - TLV_HEADER_LEN);
                break;
            case UDLD_DEVICE_NAME_TLV:
                osal_memcpy(&pNbr->deviceName, pTmp + TLV_HEADER_LEN, tlv_length - TLV_HEADER_LEN);
                break;
            case UDLD_SEQ_NUMBER_TLV:
                break;
            default:
                break;
        }

        pTmp = pTmp + tlv_length;

        if(tlv_count >= tlvs_length)
        {
            break;
        }
    }

    pNbr->hold_timer = pNbr->messageInterval * TLL_R;

    return pNbr;
}

static int32 _udld_neighbor_delete(sys_logic_port_t port, sys_udld_neighbor_entry_t *pNbr)
{
    sys_udld_neighbor_entry_t *pNbrPre, *pNbrNext;

    if(pNbr != NULL)
    {
        if(pNbr->pPreNbr == NULL) 
        {
            udld_portTable.port_entry[port].pNeighbor = pNbr->pNextNbr;
            if(udld_portTable.port_entry[port].pNeighbor != NULL)
                udld_portTable.port_entry[port].pNeighbor->pPreNbr = NULL;
            _udld_neighbor_free(pNbr);
            udld_portTable.port_entry[port].neighborNum --;
        }
        else
        {
            pNbrPre = pNbr->pPreNbr;
            pNbrNext = pNbr->pNextNbr;
            pNbrPre->pNextNbr = pNbrNext;
            if(pNbrNext != NULL)
                pNbrNext->pPreNbr = pNbrPre;

            _udld_neighbor_free(pNbr);
            udld_portTable.port_entry[port].neighborNum --;
        }
    }

    return SYS_ERR_OK;
}

static int32 _udld_neighbor_flush(sys_logic_port_t port, sys_udld_neighbor_entry_t *pNbr)
{
    sys_udld_neighbor_entry_t *pNbrCur;

    if(pNbr != NULL)
    {
        pNbrCur = udld_portTable.port_entry[port].pNeighbor;

        while(pNbrCur != NULL)
        {
            if(0 == osal_strcmp(pNbrCur->device_id, pNbr->device_id) &&
                0 == osal_strcmp(pNbrCur->port_id, pNbr->port_id))
            {
                _udld_neighbor_delete(port, pNbrCur);
                if(udld_portTable.port_entry[port].neighborNum == 0)
                    udld_portTable.port_entry[port].lastFlush = TRUE;
                break;
            }

            pNbrCur = pNbrCur->pNextNbr;
        }
    }

    _udld_neighbor_free(pNbr);

    return SYS_ERR_OK;
}

static sys_udld_neighbor_save_result_t _udld_neighbor_store(sys_logic_port_t port, sys_udld_neighbor_entry_t *pNbr)
{
    int found;
    sys_udld_neighbor_entry_t *pNowNbr, *pPreNbr, *pNextNbr;

    pNowNbr = udld_portTable.port_entry[port].pNeighbor;
    if(pNowNbr == NULL)
    {
        pNbr->pPreNbr = pNbr->pNextNbr = NULL;
        udld_portTable.port_entry[port].pNeighbor = pNbr;
        udld_portTable.port_entry[port].neighborNum ++;
        pNbr->reSyncChance = TRUE;
        return UDLD_NEIGHBOR_NEW;
    }
    else if(0 == osal_strcmp(pNowNbr->device_id, pNbr->device_id) &&
                0 == osal_strcmp(pNowNbr->port_id, pNbr->port_id))
    {
        pNbr->pNextNbr = pNowNbr->pNextNbr;
        pNbr->reSyncChance = pNowNbr->reSyncChance;
        _udld_neighbor_free(pNowNbr);
        pNbr->pPreNbr = NULL;
        if(NULL != pNbr->pNextNbr)
            pNbr->pNextNbr->pPreNbr = pNbr;
        udld_portTable.port_entry[port].pNeighbor = pNbr;

        return UDLD_NEIGHBOR_OLD;
    }
    else
    {
        found = FALSE;

        while(pNowNbr != NULL)
        {
            pPreNbr = pNowNbr->pPreNbr;
            pNextNbr = pNowNbr->pNextNbr;

            if(0 == osal_strcmp(pNowNbr->device_id, pNbr->device_id) &&
                    0 == osal_strcmp(pNowNbr->port_id, pNbr->port_id))
            {
                pNbr->reSyncChance = pNowNbr->reSyncChance;
                _udld_neighbor_free(pNowNbr);
                pNbr->pPreNbr = pPreNbr;
                pNbr->pNextNbr = pNextNbr;
                if(NULL != pPreNbr)
                    pPreNbr->pNextNbr = pNbr;
                if(NULL != pNextNbr)
                    pNextNbr->pPreNbr = pNbr;
                found = TRUE;

                return UDLD_NEIGHBOR_OLD;
            }
            pPreNbr = pNowNbr;
            pNowNbr = pNextNbr;
        }

        if(found == FALSE)
        {
            if(udld_portTable.port_entry[port].neighborNum < CAPA_UDLD_NEIGHBOR_NUM)
            {
                if(NULL != pPreNbr)
                    pPreNbr->pNextNbr = pNbr;
                pNbr->pPreNbr = pPreNbr;
                pNbr->pNextNbr = NULL;
                udld_portTable.port_entry[port].neighborNum ++;
                pNbr->reSyncChance = TRUE;

                return UDLD_NEIGHBOR_NEW;
            }
        }
    }

    return UDLD_NEIGHBOR_FAILED;
}

static uint8 _self_loop_check(sys_logic_port_t port, sys_udld_neighbor_entry_t *pNeighbor)
{
    char ingress_port_id[SYS_BUF16_LEN];

    if (pNeighbor != NULL)
    {
        LP2STR(port, ingress_port_id);
        if((osal_strcmp(pNeighbor->device_id, system_device_id) == 0)
                && (osal_strcmp(pNeighbor->port_id, ingress_port_id) == 0))
        {
            return TRUE;
        }
    }

    return FALSE;
}
static uint8 _echo_verify(sys_logic_port_t port, sys_udld_neighbor_entry_t *pNeighbor)
{
    uint16 i = 0;
    char ingress_port_id[SYS_BUF16_LEN];

    if (pNeighbor != NULL)
    {
        if(0 == pNeighbor->echoNum)
        {
            return UDLD_EMPTY_ECHO;
        }

        osal_memset(ingress_port_id, 0, sizeof(ingress_port_id));
        LP2STR(port, ingress_port_id);

        for (i = 0 ; i < pNeighbor->echoNum; i++)
        {
            if((osal_strcmp(pNeighbor->pEchoPair[i].echo_device_id, system_device_id) == 0)
                && (osal_strcmp(pNeighbor->pEchoPair[i].echo_port_id, ingress_port_id) == 0))
            {
                if(pNeighbor->echoNum > 1)
                {
                    UDLD_DBG("Echo tlv with multiple pair match");
                    return UDLD_MATCH_MULTI_ECHO;
                }
                else
                {
                    UDLD_DBG("Echo tlv match");
                    return UDLD_MATCH_ECHO;
                }
            }
        }
    }

    UDLD_DBG("Echo tlv dose not match");
    return UDLD_UNMATCH_ECHO;
}

static void _udld_timer_flag_get(sys_logic_port_t port, uint16 *pTimeCtrl,uint8  *pFlag)
{
    switch(udld_portTable.port_entry[port].operational_state)
    {
        case UDLD_OP_INACTIVE :
            *pTimeCtrl = LINKUP_PHASE1_TIME;
            *pFlag = 0;
            break;

        case UDLD_OP_REESTABLISH :
            *pTimeCtrl = LINKUP_PHASE1_TIME;
            *pFlag = RT_RSY_FLAG;
            break;

        case UDLD_OP_LINKUP_P1:
            *pTimeCtrl = LINKUP_PHASE1_TIME;
            *pFlag = RT_RSY_FLAG;
            break;

        case UDLD_OP_LINKUP_P2:
            *pTimeCtrl = LINKUP_PHASE2_TIME;
            *pFlag = RT_FLAG;
            break;

        case UDLD_OP_DETECTION:
            *pTimeCtrl = DETECTION_PHASE1_TIME;
            *pFlag = 0;
            break;

        case UDLD_OP_EXTENDED_DETECTION:
            *pTimeCtrl = DETECTION_PHASE2_TIME;
            *pFlag = 0;
            break;

        case UDLD_OP_ADVERTISEMENT_SINGLE :
        case UDLD_OP_ADVERTISEMENT_MULTI :
            *pTimeCtrl = probe_interval;
            *pFlag = RT_FLAG;
            break;

        case UDLD_OP_ERROR_DISABLE :
            break;

        case UDLD_OP_LINKDOWN :
            break;

        default:
            break;
    }

    return;
}
static sys_udld_op_state_t _next_phase_logic_1(sys_logic_port_t port)
{
    uint8 isAnyBidirect = FALSE, isSelfLoop = FALSE, isAnySelfLoop = FALSE, isAllEmpty = TRUE,
        isAnyMismatch = FALSE;
    sys_udld_echoTlv_t result;
    sys_udld_neighbor_entry_t *pNbrCurrent;

    UDLD_DBG("Port %d %s",  port, __func__);

    pNbrCurrent = udld_portTable.port_entry[port].pNeighbor;

    while(pNbrCurrent != NULL)
    {
        isSelfLoop = _self_loop_check(port, pNbrCurrent);

        if(isSelfLoop == TRUE)
        {
            pNbrCurrent->status = UDLD_BIDIR_SELF_LOOP;
            isAnySelfLoop = TRUE;
            pNbrCurrent = pNbrCurrent->pNextNbr;
            continue;
        }

        result = _echo_verify(port, pNbrCurrent);

        switch(result)
        {
            case UDLD_EMPTY_ECHO:
                pNbrCurrent->status = UDLD_BIDIR_NEIGHBOR_ECHO_EMPTY;
                break;
            case UDLD_MATCH_ECHO:
                pNbrCurrent->status = UDLD_BIDIR_BIDIRECTIONAL;
                isAnyBidirect = TRUE;
                isAllEmpty = FALSE;
                break;
            case UDLD_MATCH_MULTI_ECHO:
                if(udld_portTable.port_entry[port].neighborNum > 1)
                {
                    pNbrCurrent->status = UDLD_BIDIR_BIDIRECTIONAL;
                    isAnyBidirect = TRUE;
                }
                else
                {
                    pNbrCurrent->status = UDLD_BIDIR_NEIGHBOR_ECHO_MISMATCH;
                    isAnyMismatch = TRUE;
                }
                isAllEmpty = FALSE;
                break;
            case UDLD_UNMATCH_ECHO:
                pNbrCurrent->status = UDLD_BIDIR_UNIDIRECTIONAL;
                isAllEmpty = FALSE;
                break;
            default:
                break;
        }

        pNbrCurrent = pNbrCurrent->pNextNbr;
    }

    if(TRUE == isAnySelfLoop)
    {
        udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_SELF_LOOP;
        return UDLD_OP_ERROR_DISABLE;
    }
    else if(TRUE == isAnyBidirect)
    {
        if(udld_portTable.port_entry[port].neighborNum > 1)
            return UDLD_OP_ADVERTISEMENT_MULTI;
        else
            return UDLD_OP_ADVERTISEMENT_SINGLE;
    }
    else if(TRUE == isAllEmpty)
    {
        return UDLD_OP_EXTENDED_DETECTION;
    }
    else
    {
        if(TRUE == isAnyMismatch)
        {
            udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_NEIGHBOR_ECHO_MISMATCH;
        }
        else
            udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNIDIRECTIONAL;

        return UDLD_OP_ERROR_DISABLE;
    }

}
static sys_udld_op_state_t _next_phase_logic_2(sys_logic_port_t port)
{
    sys_udld_echoTlv_t result;

    UDLD_DBG("Port %d %s",  port, __func__);

    if(TRUE == udld_portTable.port_entry[port].rxNewPDU)
    {
        udld_portTable.port_entry[port].pRxNeighbor->status = UDLD_BIDIR_UNDETERMINED;
        return UDLD_OP_DETECTION;
    }

    result = _echo_verify(port, udld_portTable.port_entry[port].pRxNeighbor);
    switch(result)
    {
        case UDLD_EMPTY_ECHO:
            udld_portTable.port_entry[port].pRxNeighbor->status = UDLD_BIDIR_NEIGHBOR_ECHO_EMPTY;
            return UDLD_OP_ERROR_DISABLE;
        case UDLD_MATCH_ECHO:
            udld_portTable.port_entry[port].pRxNeighbor->status = UDLD_BIDIR_BIDIRECTIONAL;
            if(udld_portTable.port_entry[port].neighborNum > 1)
                return UDLD_OP_ADVERTISEMENT_MULTI;
            else
                return UDLD_OP_ADVERTISEMENT_SINGLE;
        case UDLD_MATCH_MULTI_ECHO:
            if(udld_portTable.port_entry[port].neighborNum > 1)
            {
                udld_portTable.port_entry[port].pRxNeighbor->status = UDLD_BIDIR_BIDIRECTIONAL;
                return UDLD_OP_ADVERTISEMENT_MULTI;
            }
            else
            {
                udld_portTable.port_entry[port].pRxNeighbor->status = UDLD_BIDIR_NEIGHBOR_ECHO_MISMATCH;
                return UDLD_OP_ERROR_DISABLE;
            }
        case UDLD_UNMATCH_ECHO:
            udld_portTable.port_entry[port].pRxNeighbor->status = UDLD_BIDIR_UNIDIRECTIONAL;
            return UDLD_OP_ERROR_DISABLE;
        default:
            break;
    }

    return UDLD_OP_EXTENDED_DETECTION;
}
static sys_udld_op_state_t _next_phase_logic_3(sys_logic_port_t port)
{
    uint8 isAnyBidirect = FALSE;
    sys_udld_echoTlv_t result;
    sys_udld_neighbor_entry_t *pNbrCurrent;

    UDLD_DBG("Port %d %s",  port, __func__);

    if(TRUE == udld_portTable.port_entry[port].rxNewPDU)
    {
        udld_portTable.port_entry[port].pRxNeighbor->status = UDLD_BIDIR_UNDETERMINED;
        udld_portTable.port_entry[port].doRSY = FALSE;
        return UDLD_OP_DETECTION;
    }

    pNbrCurrent = udld_portTable.port_entry[port].pNeighbor;

    while(pNbrCurrent != NULL)
    {
        result = _echo_verify(port, pNbrCurrent);

        switch(result)
        {
            case UDLD_EMPTY_ECHO:
                pNbrCurrent->status = UDLD_BIDIR_NEIGHBOR_ECHO_EMPTY;
                break;
            case UDLD_MATCH_ECHO:
                pNbrCurrent->status = UDLD_BIDIR_BIDIRECTIONAL;
                isAnyBidirect = TRUE;
                break;
            case UDLD_MATCH_MULTI_ECHO:
                if(udld_portTable.port_entry[port].neighborNum > 1)
                {
                    pNbrCurrent->status = UDLD_BIDIR_BIDIRECTIONAL;
                    isAnyBidirect = TRUE;
                }
                else
                {
                    pNbrCurrent->status = UDLD_BIDIR_NEIGHBOR_ECHO_MISMATCH;
                }
                break;
            case UDLD_UNMATCH_ECHO:
                pNbrCurrent->status = UDLD_BIDIR_UNIDIRECTIONAL;
                break;
            default:
                break;
        }

        pNbrCurrent = pNbrCurrent->pNextNbr;
    }

    if(TRUE == isAnyBidirect)
    {
        if(udld_portTable.port_entry[port].neighborNum > 1)
            return UDLD_OP_ADVERTISEMENT_MULTI;
        else
            return UDLD_OP_ADVERTISEMENT_SINGLE;
    }
    else
    {
        udld_portTable.port_entry[port].doRSY = FALSE;
        return UDLD_OP_DETECTION;
    }
}
static int32 _udld_errPort_disable(sys_logic_port_t lp)
{
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    switch(udld_portTable.port_entry[lp].bidirectional_state)
    {
        case UDLD_BIDIR_UNDETERMINED: 
            SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_AGGRESSIVE_FAIL, LPSTR(lp));
            break;
        case UDLD_BIDIR_SELF_LOOP:
            SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_LOOP_DETECT, LPSTR(lp));
            break;
        case UDLD_BIDIR_NEIGHBOR_ECHO_EMPTY:
        case UDLD_BIDIR_UNIDIRECTIONAL:
            SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_UNIDIRECTIONAL_DETECT, LPSTR(lp));
            break;
        case UDLD_BIDIR_NEIGHBOR_ECHO_MISMATCH:
            SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_NEIGHBOR_MISMATCH_DETECT, LPSTR(lp));
            break;
        default:
            break;
    }
    if (SYS_ERR_FAILED == rsd_port_errDisable_set(lp, SYS_PORT_ERRDISABLE_UDLD))
        SYS_DBG(DBG_UDLD, "errdisabled port %s failed", LPSTR(lp));
#endif

    _udld_pkt_tx(OPCODE_FLUSH, lp, DEFAULT_FLAG);

    return SYS_ERR_OK;
}
static void _state_translation(sys_logic_port_t port)
{
    sys_udld_op_state_t    operational_state;

    if (TRUE == udld_portTable.port_entry[port].lastFlush)
    {
        udld_portTable.port_entry[port].operational_state = UDLD_OP_LINKUP_P2;
        udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNDETERMINED;
        udld_portTable.port_entry[port].sending_timer = 0;
        udld_portTable.port_entry[port].txCount = 0;
        UDLD_DBG("Port %d GOTO UDLD_OP_LINKUP_P2 state", port);

        udld_portTable.port_entry[port].lastFlush = FALSE;
        return;
    }

    if (TRUE == udld_portTable.port_entry[port].lastTimeOut)
    {
        if (UDLD_MODE_NORMAL == udld_portTable.port_entry[port].udld_operMode)
        {
            udld_portTable.port_entry[port].operational_state = UDLD_OP_LINKUP_P1;
            udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNDETERMINED;
            udld_portTable.port_entry[port].sending_timer = 0;
            udld_portTable.port_entry[port].txCount = 0;
            UDLD_DBG("Port %d GOTO UDLD_OP_LINKUP_P2 state", port);
        }
        else if (UDLD_MODE_AGGRESSIVE == udld_portTable.port_entry[port].udld_operMode)
        {
            udld_portTable.port_entry[port].operational_state = UDLD_OP_REESTABLISH;
            udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNDETERMINED;
            udld_portTable.port_entry[port].sending_timer = 0;
            udld_portTable.port_entry[port].txCount = 0;
            UDLD_DBG("Port %d GOTO UDLD_OP_REESTABLISH state", port);
        }

        udld_portTable.port_entry[port].lastTimeOut = FALSE;
        return;
    }

    switch(udld_portTable.port_entry[port].operational_state)
    {
        case UDLD_OP_INACTIVE:
            break;

        case UDLD_OP_REESTABLISH:
            break;

        case UDLD_OP_LINKUP_P1:
        case UDLD_OP_LINKUP_P2:
            if (TRUE == udld_portTable.port_entry[port].rxPDU)
            {
                 udld_portTable.port_entry[port].operational_state = UDLD_OP_DETECTION;
                 udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNDETERMINED;
                 udld_portTable.port_entry[port].sending_timer = 0;
                 udld_portTable.port_entry[port].txCount = 0;
                 UDLD_DBG("Port %d GOTO UDLD_OP_DETECTION state", port);

                 udld_portTable.port_entry[port].rxPDU = FALSE;
                 udld_portTable.port_entry[port].rxNewPDU = FALSE;
            }
            break;

        case UDLD_OP_DETECTION:
            if (TRUE == udld_portTable.port_entry[port].rxPDU)
            {
                udld_portTable.port_entry[port].rxPDU = FALSE;
                udld_portTable.port_entry[port].rxNewPDU = FALSE;
            }
            break;

        case UDLD_OP_EXTENDED_DETECTION:
            if(TRUE == udld_portTable.port_entry[port].rxPDU)
            {
                operational_state = _next_phase_logic_2(port);

                udld_portTable.port_entry[port].operational_state = operational_state;
                udld_portTable.port_entry[port].bidirectional_state = udld_portTable.port_entry[port].pRxNeighbor->status;
                udld_portTable.port_entry[port].sending_timer = 0;
                udld_portTable.port_entry[port].txCount = 0;

                switch(operational_state)
                {
                    case UDLD_OP_DETECTION:
                        UDLD_DBG("Port %d GOTO UDLD_OP_DETECTION state", port);
                        break;
                    case UDLD_OP_ERROR_DISABLE:
                        _udld_errPort_disable(port);
                        UDLD_DBG("Port %d GOTO UDLD_OP_ERROR_DISABLE state", port);
                        break;
                    case UDLD_OP_ADVERTISEMENT_SINGLE:
                    case UDLD_OP_ADVERTISEMENT_MULTI:
                        UDLD_DBG("Port %d GOTO UDLD_OP_ADVERTISEMENT state", port);
                        break;
                    default:
                        UDLD_DBG("Port %d GOTO WRONG state", port);
                        break;
                }

                udld_portTable.port_entry[port].rxPDU = FALSE;
                udld_portTable.port_entry[port].rxNewPDU = FALSE;
            }

            break;

        case UDLD_OP_ADVERTISEMENT_SINGLE:
        case UDLD_OP_ADVERTISEMENT_MULTI:
            if(TRUE == udld_portTable.port_entry[port].rxPDU)
            {
                sys_udld_op_state_t operational_state;

                operational_state = _next_phase_logic_3(port);

                udld_portTable.port_entry[port].operational_state = operational_state;
                udld_portTable.port_entry[port].bidirectional_state = udld_portTable.port_entry[port].pRxNeighbor->status;

                switch(operational_state)
                {
                    case UDLD_OP_DETECTION:
                        udld_portTable.port_entry[port].sending_timer = 0;
                        udld_portTable.port_entry[port].txCount = 0;
                        UDLD_DBG("Port %d GOTO UDLD_OP_DETECTION state", port);
                        break;
                    case UDLD_OP_ERROR_DISABLE:
                        udld_portTable.port_entry[port].sending_timer = 0;
                        udld_portTable.port_entry[port].txCount = 0;
                        _udld_errPort_disable(port);
                        UDLD_DBG("Port %d GOTO UDLD_OP_ERROR_DISABLE state", port);
                        break;
                    case UDLD_OP_ADVERTISEMENT_SINGLE:
                    case UDLD_OP_ADVERTISEMENT_MULTI:
                        UDLD_DBG("Port %d GOTO UDLD_OP_ADVERTISEMENT state", port);
                        break;
                    default:
                        UDLD_DBG("Port %d GOTO WRONG state", port);
                        break;
                }

                udld_portTable.port_entry[port].rxPDU = FALSE;
                udld_portTable.port_entry[port].rxNewPDU = FALSE;
            }

            break;

        case UDLD_OP_ERROR_DISABLE :
            break;

        case UDLD_OP_LINKDOWN :
            break;

        default:
            break;
    }

    if(TRUE == udld_portTable.port_entry[port].rxPDU)
    {
        udld_portTable.port_entry[port].rxPDU = FALSE;
        udld_portTable.port_entry[port].rxNewPDU = FALSE;
    }

    return;
}

static void _state_machine_chain(sys_logic_port_t port)
{
    uint16 cycleTime = 0;
    uint8 flag = 0;

    if(udld_portTable.port_entry[port].sending_timer > 0)
        udld_portTable.port_entry[port].sending_timer --;

    switch (udld_portTable.port_entry[port].operational_state)
    {
        case UDLD_OP_REESTABLISH :
            if (0 == udld_portTable.port_entry[port].sending_timer)
            {
                udld_portTable.port_entry[port].txCount++;
                _udld_timer_flag_get(port,&cycleTime,&flag);
                _udld_pkt_tx(OPCODE_PROBE,port,flag);
                UDLD_DBG("Port %d UDLD_OP_REESTABLISH cycleTime=%d, flag=0x%x, TX count=%d",
                    port, cycleTime, flag, udld_portTable.port_entry[port].txCount);

                if (udld_portTable.port_entry[port].txCount >= N_REESTABLISH_PROBE)
                {
                    if (0 == udld_portTable.port_entry[port].neighborNum)
                    {
                        udld_portTable.port_entry[port].operational_state = UDLD_OP_ERROR_DISABLE;
                        udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNIDIRECTIONAL;
                        _udld_errPort_disable(port);
                        _state_translation(port);
                    }
                    else
                    {
                        udld_portTable.port_entry[port].operational_state = UDLD_OP_DETECTION;
                        udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNDETERMINED;
                        _state_translation(port);
                        _udld_timer_flag_get(port,&cycleTime,&flag);
                    }

                    udld_portTable.port_entry[port].txCount = 0;
                }

                udld_portTable.port_entry[port].sending_timer = cycleTime;
            }

            break;

        case UDLD_OP_INACTIVE : 
            break;

        case UDLD_OP_LINKUP_P1:    
            if(0 == udld_portTable.port_entry[port].sending_timer)
            {
                udld_portTable.port_entry[port].txCount++;
                _udld_timer_flag_get(port,&cycleTime,&flag);
                _udld_pkt_tx(OPCODE_PROBE,port,flag);
                UDLD_DBG("Port %d UDLD_OP_LINKUP_P1 cycleTime=%d, flag=0x%x, TX count=%d",
                    port, cycleTime, flag, udld_portTable.port_entry[port].txCount);

                if(udld_portTable.port_entry[port].txCount >= 8)
                {
                    udld_portTable.port_entry[port].operational_state = UDLD_OP_LINKUP_P2;
                    udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNDETERMINED;
                    udld_portTable.port_entry[port].txCount = 0;
                    _state_translation(port);
                    _udld_timer_flag_get(port,&cycleTime,&flag);
                }

                udld_portTable.port_entry[port].sending_timer = cycleTime;
            }

            break;

        case UDLD_OP_LINKUP_P2:    
            if(0 == udld_portTable.port_entry[port].sending_timer)
            {
                udld_portTable.port_entry[port].txCount++;
                _udld_timer_flag_get(port, &cycleTime, &flag);
                _udld_pkt_tx(OPCODE_PROBE, port, flag);
                UDLD_DBG("Port %d UDLD_OP_LINKUP_P2 cycleTime=%d, flag=0x%x", port, cycleTime, flag);
                udld_portTable.port_entry[port].sending_timer = cycleTime;
            }

            break;

        case UDLD_OP_DETECTION:
            if(0 == udld_portTable.port_entry[port].sending_timer)
            {
                udld_portTable.port_entry[port].txCount++;
                _udld_timer_flag_get(port,&cycleTime,&flag);
                _udld_pkt_tx(OPCODE_ECHO,port,DEFAULT_FLAG);
                UDLD_DBG("Port %d UDLD_OP_DETECTION cycleTime=%d, flag=0x%x, TX count=%d",
                    port, cycleTime, flag, udld_portTable.port_entry[port].txCount);

                if (udld_portTable.port_entry[port].txCount >= DETECTION_PHASE_N_ECHO_REPLY)
                {
                    sys_udld_op_state_t operational_state;
                    operational_state = _next_phase_logic_1(port);

                    udld_portTable.port_entry[port].operational_state = operational_state;
                    udld_portTable.port_entry[port].txCount = 0;

                    switch(operational_state)
                    {
                        case UDLD_OP_ERROR_DISABLE:
                            
                            _udld_errPort_disable(port);
                            UDLD_DBG("Port %d GOTO UDLD_OP_ERROR_DISABLE state", port);
                            break;
                        case UDLD_OP_EXTENDED_DETECTION:
                            udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNDETERMINED;
                            UDLD_DBG("Port %d GOTO UDLD_OP_EXTENDED_DETECTION state", port);
                            break;
                        case UDLD_OP_ADVERTISEMENT_SINGLE:
                        case UDLD_OP_ADVERTISEMENT_MULTI:
                            udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_BIDIRECTIONAL;
                            UDLD_DBG("Port %d GOTO UDLD_OP_ADVERTISEMENT state", port);
                            break;
                        default:
                            break;
                    }

                    _state_translation(port);
                    _udld_timer_flag_get(port,&cycleTime,&flag);
                }

                udld_portTable.port_entry[port].sending_timer = cycleTime;
            }

            break;

        case UDLD_OP_EXTENDED_DETECTION:
            if (0 == udld_portTable.port_entry[port].sending_timer)
            {
                udld_portTable.port_entry[port].txCount++;
                _udld_timer_flag_get(port,&cycleTime,&flag);
                _udld_pkt_tx(OPCODE_ECHO, port, DEFAULT_FLAG);
                UDLD_DBG("Port %d UDLD_OP_EXTENDED_DETECTION cycleTime=%d, flag=0x%x", port, cycleTime, flag);

                udld_portTable.port_entry[port].sending_timer = cycleTime;
            }

            break;

        case UDLD_OP_ADVERTISEMENT_SINGLE:
        case UDLD_OP_ADVERTISEMENT_MULTI:
            if (0 == udld_portTable.port_entry[port].sending_timer)
            {
                udld_portTable.port_entry[port].txCount++;

                _udld_timer_flag_get(port,&cycleTime,&flag);
                if(TRUE == udld_portTable.port_entry[port].doRSY) 
                {
                    _udld_pkt_tx(OPCODE_ECHO,port,DEFAULT_FLAG);

                    if(5 == udld_portTable.port_entry[port].txCount)
                    {
                        udld_portTable.port_entry[port].doRSY = FALSE;
                        udld_portTable.port_entry[port].txCount = 0;
                        udld_portTable.port_entry[port].sending_timer = cycleTime;
                    }
                    else
                        udld_portTable.port_entry[port].sending_timer = 1;
                }
                else
                {
                    _udld_pkt_tx(OPCODE_PROBE,port,RT_FLAG);

                    udld_portTable.port_entry[port].sending_timer = cycleTime;
                }

                UDLD_DBG("Port %d UDLD_OP_ADVERTISEMENT cycleTime=%d, flag=0x%x", port, cycleTime, flag);
            }

            break;

        case UDLD_OP_ERROR_DISABLE :
            break;

        case UDLD_OP_LINKDOWN :
            break;

        default:
            break;
    }
    _state_translation(port);

    return;
}

static uint16 _csum_compute(unsigned char *buff, unsigned short len)
{
        unsigned short csum;
        int     odd;
        unsigned long   result = 0;
        unsigned short leftover;
        unsigned short *p;

        if (len > 0) {
                odd = 1 & (unsigned long)buff;
                if (odd) {
                        result = *buff << 8;
                        len--;
                        buff++;
                }
                while (len > 1) {
                        p = (unsigned short *)buff;
                        result += *p++;
                        buff = (unsigned char *)p;
                        if (result & 0x80000000)
                                result = (result & 0xFFFF) + (result >> 16);
                        len -= 2;
                }
                if (len) {
                        leftover = (signed short)(*(const signed char *)buff);
                        
                        result = (result & 0xffff0000) | ((result + leftover) & 0x0000ffff);
                }
                while (result >> 16)
                        result = (result & 0xFFFF) + (result >> 16);

                if (odd)
                        result = ((result >> 8) & 0xff) | ((result & 0xff) << 8);
        }

        result = (result & 0xffff) + (result >> 16);
        
        result = (result & 0xffff) + (result >> 16);
        
        result = (result & 0xffff) + (result >> 16);

        csum = ~(unsigned short)result;

        return csum;
}

static int32 _udld_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    uint8 macStr[CAPA_MAC_STR_LEN];
    uint8 udldh_offset = 0;
    uint8 udldtlv_offset = 0;
    uint8 snaplen_offset = 0;
    uint8 *pUdld = NULL;
    uint8 *pFlag = NULL;
    uint16 *pTlvs = NULL;
    uint16 snap_length = 0;
    uint16 udld_length = 0;
    uint16 tlvs_length = 0;
    uint16 llc_pid = 0;
    uint16 check_sum = 0 ;
    udld_pkt_t *udld_pkt = NULL;
    sys_logic_port_t port;
    sys_udld_neighbor_entry_t *pNbr;
    sys_udld_neighbor_save_result_t result;

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);

    port = pPkt->rx_tag.source_port;

    if(PORT_LINKDOWN == udld_portTable.port_entry[port].port_link)
    {
        rsd_nic_pkt_free(pPkt);

        return SYS_ERR_OK;
    }

    if (UDLD_MODE_DISABLE == udld_portTable.port_entry[port].udld_operMode)
    {
        SYS_MEM_CLEAR(macStr);
        sys_util_mac2Str(pPkt->data + SA_OFFSET, macStr);
        SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_PKT_PORT_DROP, LPSTR(port), macStr);
        UDLD_DBG("[UDLD] Port not enable on %s", LPSTR(port));
        rsd_nic_pkt_free(pPkt);

        return SYS_ERR_OK;
    }

    udld_pkt = (udld_pkt_t *)osal_alloc(sizeof (udld_pkt_t));
    if (NULL == udld_pkt)
    {
        SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_ALLOCATE_FAIL);
        UDLD_DBG("[UDLD] Can not allocate memory ,free UDLD pkt on %s", LPSTR(port));
        rsd_nic_pkt_free(pPkt);

        return SYS_ERR_OK;
    }

    udldh_offset = UDLDH_OFFSET;
    snaplen_offset = SNAPLEN_OFFSET;
    pUdld = (uint8 *)pPkt->data + DA_SA_OFFSET + FRMAE_LEN_OFFSET + LLC_PID_OFFSET;
    llc_pid = HTONS(READ16(pUdld));

    if(llc_pid != UDLD_PID)
    {
        SYS_MEM_CLEAR(macStr);
        sys_util_mac2Str(pPkt->data + SA_OFFSET, macStr);
        SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_PKT_INVALID_DROP, LPSTR(port), macStr);
        UDLD_DBG("[UDLD] This is not UDLD packet format and drop it on %s",LPSTR(port));
        osal_free(udld_pkt);
        rsd_nic_pkt_free(pPkt);

        return SYS_ERR_OK;
    }

    pUdld = (uint8 *)(pPkt->data + DA_SA_OFFSET);
    snap_length = READ16(pUdld);

    udld_length = snap_length - LLC_LEN_OFFSET;
    tlvs_length = udld_length - UDLD_HEADER_LEN;

    pUdld = pPkt->data + DA_SA_OFFSET + FRMAE_LEN_OFFSET + LLC_LEN_OFFSET;
    osal_memcpy(udld_pkt, pUdld, UDLD_HEADER_LEN);
    pFlag = (pUdld + 1);
    check_sum = _csum_compute(pUdld,udld_length);

    if(0 != check_sum)
    {
        SYS_MEM_CLEAR(macStr);
        sys_util_mac2Str(pPkt->data + SA_OFFSET, macStr);
        SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_PKT_CHKSUM_DROP, LPSTR(port), macStr);
        UDLD_DBG("[UDLD] Header Error: Incorrect Checksum on %s", LPSTR(port));
        UDLD_DBG("udld_length = %d check_sum=%0xx",udld_length,check_sum);
        osal_free(udld_pkt);
        rsd_nic_pkt_free(pPkt);

        return SYS_ERR_OK;
    }

    udldtlv_offset = udldh_offset + UDLD_HEADER_LEN;
    pTlvs = (uint16*)(pPkt->data + udldtlv_offset);
    pUdld = (pPkt->data + udldtlv_offset);

    UDLD_DBG("Port %d RX PDU<%s> operation<%s>", port,
            udld_code_values[udld_pkt->opcode].value,
            udld_phase_values[udld_portTable.port_entry[port].operational_state].value);

    switch (udld_pkt->opcode)
    {
        case OPCODE_RESERVED:   
            break;
        case OPCODE_PROBE:      
        case OPCODE_ECHO:       
            pUdld = (pPkt->data + udldtlv_offset);

            UDLD_NEIGHBOR_SEM_LOCK(port);
            pNbr = _udld_neighbor_parse(pUdld , tlvs_length);
            if(pNbr == NULL)
            {
                UDLD_NEIGHBOR_SEM_UNLOCK(port);

                SYS_MEM_CLEAR(macStr);
                sys_util_mac2Str(pPkt->data + SA_OFFSET, macStr);
                SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_PKT_INVALID_DROP, LPSTR(port), macStr);
                UDLD_DBG("[UDLD] UDLD PDU parsed fail on %s", LPSTR(port));
                osal_free(udld_pkt);
                rsd_nic_pkt_free(pPkt);

                return SYS_ERR_OK;
            }

            result = _udld_neighbor_store(port, pNbr);
            UDLD_NEIGHBOR_SEM_UNLOCK(port);
            if(UDLD_NEIGHBOR_FAILED != result)
            {
                if(UDLD_NEIGHBOR_NEW == result)
                    udld_portTable.port_entry[port].rxNewPDU = TRUE;
                else
                    udld_portTable.port_entry[port].rxNewPDU = FALSE;

                udld_portTable.port_entry[port].rxPDU = TRUE;
                udld_portTable.port_entry[port].pRxNeighbor = pNbr;

                _state_translation(port);
            }
            else
            {
                _udld_neighbor_free(pNbr);
            }

            if(UDLD_NEIGHBOR_OLD == result &&
                (UDLD_OP_ADVERTISEMENT_SINGLE == udld_portTable.port_entry[port].operational_state ||
                UDLD_OP_ADVERTISEMENT_MULTI == udld_portTable.port_entry[port].operational_state))
            {
                if(*pFlag & (1 << 1))
                {
                    if(TRUE == pNbr->reSyncChance)
                    {
                        udld_portTable.port_entry[port].doRSY = TRUE;
                        udld_portTable.port_entry[port].txCount = 0;
                        pNbr->reSyncChance = FALSE;
                        udld_portTable.port_entry[port].sending_timer = 0;
                    }
                }
                else
                {
                    
                    if(OPCODE_PROBE == udld_pkt->opcode && UDLD_BIDIR_BIDIRECTIONAL == pNbr->status)
                    {
                        pNbr->reSyncChance = TRUE;
                    }
                }
            }

            break;

        case OPCODE_FLUSH: 
            pUdld = (pPkt->data + udldtlv_offset);

            pNbr = _udld_neighbor_parse(pUdld , tlvs_length);
            if(pNbr == NULL)
            {

                SYS_MEM_CLEAR(macStr);
                sys_util_mac2Str(pPkt->data + SA_OFFSET, macStr);
                SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_PKT_INVALID_DROP, LPSTR(port), macStr);
                UDLD_DBG("[UDLD] tlv Error: Invilad tlv length on logicalPort %d",port);
                osal_free(udld_pkt);
                rsd_nic_pkt_free(pPkt);

                return SYS_ERR_OK;
            }

            _udld_neighbor_flush(port, pNbr);

            if(udld_portTable.port_entry[port].lastFlush == TRUE)
                _state_translation(port);
            break;

        default:
            break;
    }

    if(NULL != udld_pkt)
        osal_free(udld_pkt);

    rsd_nic_pkt_free(pPkt);

    return SYS_ERR_OK;
}

static int32 _udld_pkt_tx(uint16 opcode, sys_logic_port_t port, uint8 flag)
{
    int32 ret = SYS_ERR_OK;
    char *pktData = NULL;
    char tmp[SYS_BUF32_LEN];
    uint16 packet_len = 0;
    uint16 tlv_offset = 0;
    uint16 checksum = 0;
    uint16 length = 0;
    uint16 tlv_length_count = 0;
    uint32 echo_target = 0;
    sys_mac_t port_mac;
    sys_nic_pkt_t *pPkt = NULL;
    sys_logic_portmask_t logic_portmask;
    sys_sysinfo_t *pSysinfo = NULL;

    SYS_MEM_ALLOC_HDL(pktData, TEMPLATE_TX_ULDLD, _exit, ret);
    SYS_MEM_ALLOC_HDL(pSysinfo, sizeof(sys_sysinfo_t), _exit, ret);

    osal_memcpy(pktData,pktTxData,TEMPLATE_TX_ULDLD);
    osal_memset(tmp, 0, sizeof(tmp));

    switch (opcode)
    {
        case OPCODE_RESERVED: 
            *(pktData + UDLDH_OFFSET) = OPCODE_RESERVED|0x20;
            break;

        case OPCODE_PROBE: 
            *(pktData + UDLDH_OFFSET) = OPCODE_PROBE|0x20;
            break;

        case OPCODE_ECHO: 
            *(pktData + UDLDH_OFFSET) = OPCODE_ECHO|0x20;
            break;

        case OPCODE_FLUSH: 
            *(pktData + UDLDH_OFFSET) = OPCODE_FLUSH|0x20;
            break;

        default:
            break;
    }

    pktData[FLAG_OFFSET] = flag;

    if(FALSE == isDeviceIdOK)
    {
        SYS_ERR_HDL(rsd_sys_info_get(pSysinfo), _exit, ret);
        osal_memcpy(system_device_id, pSysinfo->serialnum, sizeof(pSysinfo->serialnum));
        isDeviceIdOK = TRUE;
    }

    WRITE16(&pktData[UDLDTLV_OFFSET],UDLD_DEVICE_ID_TLV);
    osal_memcpy(&pktData[UDLDTLV_OFFSET + TLV_HEADER_LEN], &system_device_id, osal_strlen(system_device_id));
    length = TLV_VALUE_LEN(osal_strlen(system_device_id)) + TLV_HEADER_LEN;

    osal_memcpy(&pktData[UDLDTLV_OFFSET + UDLD_LENGTH_OFFSET], &length, UDLD_LENGTH_OFFSET);
    tlv_length_count += length;
    tlv_offset = UDLDTLV_OFFSET + length;

    WRITE16(&pktData[tlv_offset],UDLD_PORT_ID_TLV);
    osal_memset(tmp, 0, sizeof(tmp));
    LP2STR(port, tmp);
    osal_memcpy(&pktData[tlv_offset + TLV_HEADER_LEN], &tmp, osal_strlen(tmp));
    length = TLV_VALUE_LEN(osal_strlen(tmp)) + TLV_HEADER_LEN;

    osal_memcpy(&pktData[tlv_offset + UDLD_LENGTH_OFFSET], &length, UDLD_LENGTH_OFFSET);
    tlv_length_count += length;
    tlv_offset = tlv_offset + length;
    
    UDLD_NEIGHBOR_SEM_LOCK(port);
    {
         sys_udld_neighbor_entry_t *pNbr;
         uint16 echoLen = 0;

         WRITE16(&pktData[tlv_offset],UDLD_ECHO_TLV);
         echo_target = tlv_offset + TLV_HEADER_LEN;

         WRITE32(&pktData[echo_target], udld_portTable.port_entry[port].neighborNum);

         echo_target = echo_target + ECHO_PAIRSNUM_OFFSET;

         pNbr = udld_portTable.port_entry[port].pNeighbor;

         while(pNbr != NULL)
         {
             
             WRITE16(&pktData[echo_target], osal_strlen(pNbr->device_id));
             echo_target = echo_target + ECHO_LEN_OFFSET;

             osal_memcpy(&pktData[echo_target], pNbr->device_id, osal_strlen(pNbr->device_id));
             echo_target = echo_target + osal_strlen(pNbr->device_id);

             WRITE16(&pktData[echo_target], osal_strlen(pNbr->port_id));
             echo_target = echo_target + ECHO_LEN_OFFSET;

             osal_memcpy(&pktData[echo_target], pNbr->port_id, osal_strlen(pNbr->port_id));
             echo_target = echo_target + osal_strlen(pNbr->port_id);

             echoLen += ECHO_LEN_OFFSET*2 + osal_strlen(pNbr->device_id) + osal_strlen(pNbr->port_id);

             pNbr = pNbr->pNextNbr;
         }

         length = TLV_HEADER_LEN + ECHO_PAIRSNUM_OFFSET + echoLen;
         WRITE16(&pktData[tlv_offset + UDLD_LENGTH_OFFSET],length);

         tlv_length_count += length;
         tlv_offset = tlv_offset + length;
    }
    UDLD_NEIGHBOR_SEM_UNLOCK(port);

    WRITE16(&pktData[tlv_offset],UDLD_MESSAGE_INTERVAL_TLV);
    pktData[tlv_offset + TLV_HEADER_LEN] = probe_interval;
    length = TIMEOUT_TLV_LEN;
    WRITE16(&pktData[tlv_offset + UDLD_LENGTH_OFFSET], length);
    tlv_length_count += length;

    tlv_offset = tlv_offset + length;

    WRITE16(&pktData[tlv_offset],UDLD_TIMEOUT_INTERVAL_TLV);
    pktData[tlv_offset + TLV_HEADER_LEN] = TIMEOUT_TLV_VAL;
    length = TIMEOUT_TLV_LEN;
    WRITE16(&pktData[tlv_offset + UDLD_LENGTH_OFFSET],length);
    tlv_length_count += length;
    tlv_offset = tlv_offset + length;

    WRITE16(&pktData[tlv_offset],UDLD_DEVICE_NAME_TLV);
    rsd_sys_info_get(pSysinfo);
    osal_memcpy(&pktData[tlv_offset + TLV_HEADER_LEN], &pSysinfo->name, osal_strlen(pSysinfo->name));
    length = TLV_VALUE_LEN(osal_strlen(pSysinfo->name)) + TLV_HEADER_LEN;
    WRITE16(&pktData[tlv_offset + UDLD_LENGTH_OFFSET],length);
    tlv_length_count += length;
    tlv_offset = tlv_offset + length;

    WRITE16(&pktData[tlv_offset],UDLD_SEQ_NUMBER_TLV);
    WRITE32(&pktData[tlv_offset + TLV_HEADER_LEN], udld_portTable.port_entry[port].txCount);
    length = UDLD_SEQ_TLV_LEN;
    WRITE16(&pktData[tlv_offset + UDLD_LENGTH_OFFSET],UDLD_SEQ_TLV_LEN);
    tlv_length_count += length;
    tlv_offset = tlv_offset + length;

    packet_len = UDLDTLV_OFFSET + tlv_length_count;

    packet_len = packet_len - DA_SA_OFFSET - UDLD_LENGTH_OFFSET;
    osal_memcpy(&pktData[SNAPLEN_OFFSET],&packet_len,UDLD_LENGTH_OFFSET);
    packet_len = packet_len + DA_SA_OFFSET + UDLD_LENGTH_OFFSET;

    checksum = _csum_compute(&pktData[UDLDH_OFFSET],(packet_len - UDLDH_OFFSET));
    WRITE16(&pktData[UDLDH_OFFSET + CHECKSUM_OFFSET],checksum);

    rsd_sys_portMacAddr_get(port, &port_mac);
    osal_memcpy(&pktData[SA_OFFSET],&port_mac.octet,ETHER_ADDR_LEN);

    if(ENABLED == udld_global_enable)
    {
        UDLD_DBG("Port %d TX PDU<%s> operation<%s>",port,
            udld_code_values[opcode].value,
            udld_phase_values[udld_portTable.port_entry[port].operational_state].value);

        LOGIC_PORTMASK_CLEAR_ALL(logic_portmask);
        LOGIC_PORTMASK_SET_PORT(logic_portmask, port);

        if (SYS_ERR_OK == rsd_nic_pkt_alloc(packet_len, &pPkt))
        {
            osal_memcpy(pPkt->data, pktData, packet_len);
            pPkt->tail += packet_len;
            pPkt->length = packet_len;
            SYS_ERR_HDL(rsd_nic_pkt_tx(pPkt, &logic_portmask, SYS_CPU_PORT_PRIORITY_UDLD, 0 , RSD_NIC_TX_CARE_DOT1X), _exit, ret);
        }
        else
        {
            SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_ALLOCATE_FAIL);
        }
    }

_exit:
    SYS_MEM_FREE(pktData);
    SYS_MEM_FREE(pSysinfo);
    return ret;
}

static int32 _udld_port_init(uint32 port)
{
    sys_port_linkStatus_t link;
    sys_udld_neighbor_entry_t *pNbr, *pNbrNext;

    pNbr = udld_portTable.port_entry[port].pNeighbor;
    UDLD_NEIGHBOR_SEM_LOCK(port);
    while(pNbr != NULL)
    {
        pNbrNext = pNbr->pNextNbr;
        _udld_neighbor_delete(port, pNbr);
        pNbr = pNbrNext;
    }
    UDLD_NEIGHBOR_SEM_UNLOCK(port);

    udld_portTable.port_entry[port].sending_timer = 0;

    udld_portTable.port_entry[port].txCount = 0;

    rsd_port_linkStatus_get(port, &link);

    if(PORT_LINKUP == link)
    {
        udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNDETERMINED;

        if(UDLD_MODE_DISABLE != udld_portTable.port_entry[port].udld_operMode &&
            ENABLED == udld_global_enable)
        {
            udld_portTable.port_entry[port].operational_state = UDLD_OP_LINKUP_P1;
            UDLD_DBG("Port %d GOTO UDLD_OP_LINKUP_P1 state", port);
        }
        else
        {
            udld_portTable.port_entry[port].operational_state = UDLD_OP_INACTIVE;
            UDLD_DBG("Port %d GOTO UDLD_OP_INACTIVE state", port);
        }
    }
    else
    {
        if(udld_portTable.port_entry[port].operational_state != UDLD_OP_ERROR_DISABLE)
        {
            udld_portTable.port_entry[port].operational_state = UDLD_OP_LINKDOWN;
            udld_portTable.port_entry[port].bidirectional_state = UDLD_BIDIR_UNDETERMINED;
            UDLD_DBG("Port %d GOTO UDLD_OP_LINKDOWN state", port);
        }
        else
            UDLD_DBG("Port %d STAY UDLD_OP_ERROR_DISABLE state", port);
    }

    return SYS_ERR_OK;
}

static int32 _udld_portOperMode_check(sys_logic_port_t port)
{
    sys_port_media_t media;
    sys_udld_mode_t udld_cfg_mode;

    rsd_port_linkMedia_get(port, &media);

    if(PORT_MEDIA_FIBER == media && TRUE == udld_portTable.port_entry[port].udld_modeFollowGlobal)
        udld_cfg_mode = udld_fiber_mode;
    else
        udld_cfg_mode = udld_portTable.port_entry[port].udld_cfgMode;

    if(udld_cfg_mode != udld_portTable.port_entry[port].udld_operMode)
    {
        if(UDLD_MODE_DISABLE == udld_cfg_mode) 
        {
            _udld_pkt_tx(OPCODE_FLUSH, port, DEFAULT_FLAG);
            udld_portTable.port_entry[port].udld_operMode = udld_cfg_mode;
            _udld_port_init(port);

            if(UDLD_OP_ERROR_DISABLE == udld_portTable.port_entry[port].operational_state)
            {
                rsd_port_enable_set(port, ENABLED);
                SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_RESET, LPSTR(port));
            }
        }
        else if(UDLD_MODE_DISABLE == udld_portTable.port_entry[port].udld_operMode) 
        {
            udld_portTable.port_entry[port].udld_operMode = udld_cfg_mode;
            if (PORT_LINKUP == udld_portTable.port_entry[port].port_link)
            {
                _udld_port_init(port);
                udld_portTable.port_entry[port].operational_state = UDLD_OP_LINKUP_P1;
            }
        }
        else
            udld_portTable.port_entry[port].udld_operMode = udld_cfg_mode;
    }

    return SYS_ERR_OK;
}

static int32 _linkevent_handler(uint32 event, char *pData)
{
    sys_port_event_t *pPortEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pPortEvent = (sys_port_event_t *)pData;

    UDLD_DBG("Port %d Link event happened newSt:%d", pPortEvent->lport, pPortEvent->new_linkStatus);

    udld_portTable.port_entry[pPortEvent->lport].port_link = pPortEvent->new_linkStatus;

    switch (udld_portTable.port_entry[pPortEvent->lport].port_link)
    {
        case PORT_LINKUP:
            _udld_portOperMode_check(pPortEvent->lport);

            if (UDLD_MODE_DISABLE != udld_portTable.port_entry[pPortEvent->lport].udld_operMode &&
                ENABLED == udld_global_enable)
            {
                _udld_port_init(pPortEvent->lport);
            }
            break;

        case PORT_LINKDOWN:
            _udld_port_init(pPortEvent->lport);
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}

static void _update_port_timer(uint32 port)
{
    sys_udld_neighbor_entry_t *pNbr, *pNbrNext;

    UDLD_NEIGHBOR_SEM_LOCK(port);

    pNbr = udld_portTable.port_entry[port].pNeighbor;

    while(pNbr != NULL)
    {
        pNbrNext = pNbr->pNextNbr;

        if(pNbr->hold_timer > 0)
        {
            pNbr->hold_timer --;
            if(0 == pNbr->hold_timer)
            {
                _udld_neighbor_delete(port, pNbr);

                if(udld_portTable.port_entry[port].neighborNum == 0)
                {
                    udld_portTable.port_entry[port].lastTimeOut = TRUE;
                    _state_translation(port);
                }
            }
        }
        else
        {
            _udld_neighbor_delete(port, pNbr);
            if(udld_portTable.port_entry[port].neighborNum == 0)
            {
                udld_portTable.port_entry[port].lastTimeOut = TRUE;
                _state_translation(port);
            }
        }

        pNbr = pNbrNext;
    }

    UDLD_NEIGHBOR_SEM_UNLOCK(port);

    return;
}

static void _udld_tick_thread(void *pInput)
{
    sys_logic_port_t lp;

    UDLD_DBG("Tx pkt process thread start running!");

    for (;;)
    {
        FOR_EACH_LOGIC_PORT(lp)
        {
            if ((UDLD_MODE_DISABLE != udld_portTable.port_entry[lp].udld_operMode)
                && (ENABLED == udld_global_enable)
                && (PORT_LINKUP == udld_portTable.port_entry[lp].port_link))
            {
                _update_port_timer(lp);
                _state_machine_chain(lp);
            }
        }

        if (udld_tick_thread_status)
            break;

        osal_time_usleep(1 * 1000 * 1000);
    }

    udld_tick_thread_status = -1;
    osal_thread_exit(0);

    return;
}
int32 udld_fiberMode_set(sys_udld_mode_t mode)
{
    sys_logic_port_t port;

    udld_fiber_mode = mode;

    UDLD_DBG("UDLD fiber mode set to %s", text_udld_mode[mode].text);

    FOR_EACH_LOGIC_PORT(port)
    {
        _udld_portOperMode_check(port);
    }

    return SYS_ERR_OK;
}
int32 udld_fiberMode_get(sys_udld_mode_t *pMode)
{
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    *pMode = udld_fiber_mode;

    return SYS_ERR_OK;
}
int32 udld_messageTime_set(uint32 interval)
{
    uint32 oInterval;
    sys_logic_port_t port;

    UDLD_DBG("udld_message_timer_set probe_interval=%d", probe_interval);

    oInterval = probe_interval;
    probe_interval = interval;

    if(probe_interval > oInterval)
    {
        FOR_EACH_LOGIC_PORT(port)
        {
            _udld_pkt_tx(OPCODE_FLUSH, port, DEFAULT_FLAG);
            _udld_port_init(port);
        }
    }

    return SYS_ERR_OK;
}
int32 udld_messageTime_get(uint32 *pInterval)
{
    SYS_PARAM_CHK((NULL == pInterval), SYS_ERR_NULL_POINTER);

    *pInterval = probe_interval;

    return SYS_ERR_OK;
}
int32 udld_portMode_set(sys_logic_port_t port, sys_udld_mode_t mode, uint8 isFollow)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(mode >= UDLD_MODE_END, SYS_ERR_INPUT);
    SYS_PARAM_CHK(isFollow > TRUE, SYS_ERR_INPUT);

    UDLD_DBG("Port %s UDLD mode is set to %s, isfollow %u",
        LPSTR(port), text_udld_mode[mode].text, isFollow);

    udld_portTable.port_entry[port].udld_cfgMode = mode;
    udld_portTable.port_entry[port].udld_modeFollowGlobal = isFollow;

    _udld_portOperMode_check(port);

    return SYS_ERR_OK;
}
int32 udld_portMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode, uint8 *pIsFollow)
{
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    *pMode = udld_portTable.port_entry[port].udld_cfgMode;
    *pIsFollow = udld_portTable.port_entry[port].udld_modeFollowGlobal;

    return SYS_ERR_OK;
}
int32 udld_portOperMode_get(sys_logic_port_t port, sys_udld_mode_t *pMode)
{
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    *pMode = udld_portTable.port_entry[port].udld_operMode;

    return SYS_ERR_OK;
}
int32 udld_portBidiretionState_get(sys_logic_port_t port, sys_udld_bidir_state_t *pBidirState)
{
    SYS_PARAM_CHK((NULL == pBidirState), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    *pBidirState = udld_portTable.port_entry[port].bidirectional_state;

    return SYS_ERR_OK;
}
int32 udld_portOperState_get(sys_logic_port_t port, sys_udld_op_state_t *pOperational_state)
{
    SYS_PARAM_CHK((NULL == pOperational_state), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    *pOperational_state = udld_portTable.port_entry[port].operational_state;

    return SYS_ERR_OK;
}
int32 udld_portNeighborNumber_get(sys_logic_port_t port, uint16 *pNeighborNum)
{
    SYS_PARAM_CHK((NULL == pNeighborNum), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    *pNeighborNum = udld_portTable.port_entry[port].neighborNum;

    return SYS_ERR_OK;
}
int32 udld_portNeighbor_get(sys_logic_port_t port, uint16 neighborId, sys_udld_neighbor_info_t *pNbrInfo)
{
    int i;
    sys_udld_neighbor_entry_t *pNbr;

    SYS_PARAM_CHK((NULL == pNbrInfo), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    UDLD_NEIGHBOR_SEM_LOCK(port);
    pNbr = udld_portTable.port_entry[port].pNeighbor;

    for(i = 1; i <= neighborId; i ++)
    {
        if(NULL != pNbr)
            pNbr = pNbr->pNextNbr;
        else
            break;
    }

    if(NULL != pNbr)
    {
        pNbrInfo->valid = TRUE;
        pNbrInfo->status = pNbr->status;
        pNbrInfo->hold_timer = pNbr->hold_timer;
        osal_memcpy(pNbrInfo->device_id, pNbr->device_id, sizeof(pNbrInfo->device_id));
        osal_memcpy(pNbrInfo->port_id, pNbr->port_id, sizeof(pNbrInfo->port_id));
        pNbrInfo->echoNum = pNbr->echoNum;
        pNbrInfo->messageInterval = pNbr->messageInterval;
        pNbrInfo->timeOutInterval = pNbr->timeOutInterval;
        osal_memcpy(pNbrInfo->deviceName, pNbr->deviceName, sizeof(pNbrInfo->deviceName));
    }
    else
        pNbrInfo->valid = FALSE;

    UDLD_NEIGHBOR_SEM_UNLOCK(port);

    return SYS_ERR_OK;
}
int32 udld_portNeighborEcho_get(sys_logic_port_t port, uint16 neighborId, uint16 echoId, sys_udld_echo_info_t  *pEchoInfo)
{
    int i;
    sys_udld_neighbor_entry_t *pNbr;

    SYS_PARAM_CHK((NULL == pEchoInfo), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    UDLD_NEIGHBOR_SEM_LOCK(port);
    pNbr = udld_portTable.port_entry[port].pNeighbor;

    for(i = 1; i <= neighborId; i ++)
    {
        if(NULL != pNbr)
            pNbr = pNbr->pNextNbr;
        else
            break;
    }

    if(NULL != pNbr)
    {
        if(pNbr->echoNum > echoId)
        {
            pEchoInfo->valid = TRUE;
            osal_memcpy(&pEchoInfo->echo, &pNbr->pEchoPair[echoId], sizeof(pEchoInfo->echo));
        }
    }
    else
        pEchoInfo->valid = FALSE;

    UDLD_NEIGHBOR_SEM_UNLOCK(port);

    return SYS_ERR_OK;
}
int32 udld_error_disable_reset(void)
{
    sys_logic_port_t lp;

    UDLD_DBG("UDLD reset manully");

    FOR_EACH_LOGIC_PORT(lp)
    {
        if(UDLD_OP_ERROR_DISABLE == udld_portTable.port_entry[lp].operational_state)
        {
            udld_portTable.port_entry[lp].operational_state = UDLD_OP_LINKDOWN;
            SYS_ERR_CHK(rsd_port_enable_set(lp, ENABLED));
            SYS_LOG(LOG_CAT_UDLD, LOG_UDLD_RESET, LPSTR(lp));
        }
    }

    return SYS_ERR_OK;
}
int32 udld_init(void)
{
    sys_logic_port_t lp;

    probe_interval = DFLT_UDLD_INTERVAL_TIME;

    FOR_EACH_LOGIC_PORT(lp)
    {
        udld_neighbor_sem[lp] = osal_sem_mutex_create();
        if (0 == udld_neighbor_sem[lp])
        {
            UDLD_DBG("Neighbor semaphore create failed for port %d", lp);

            return SYS_ERR_FAILED;
        }
        _udld_port_init(lp);
    }

    SYS_ERR_CHK(rsd_nic_handler_register(&l2gHandler));
    SYS_ERR_CHK(sys_notifier_observer_register("udld", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG, _linkevent_handler, SYS_NOTIFIER_PRI_MEDIUM));

    if ((osal_thread_t)NULL == (osal_thread_create("UDLD Tick Thread", 32768, DFLT_THREAD_PRI_UDLD_TX, (void *)_udld_tick_thread, NULL)))
    {
        UDLD_DBG("UDLD Tick thread create failed");

        return SYS_ERR_FAILED;;
    }

    udld_global_enable = ENABLED;
    SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_UDLD, ENABLED));

    return SYS_ERR_OK;
}
int32 udld_exit(void)
{
    
    udld_tick_thread_status = 1;
    do { } while (udld_tick_thread_status >= 0);

    SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_UDLD, DISABLED));
    SYS_ERR_CHK(rsd_nic_handler_unregister(&l2gHandler));
    SYS_ERR_CHK(sys_notifier_observer_unregister("udld", SYS_NOTIFIER_SUBJECT_PORT, _linkevent_handler));
    udld_global_enable = DISABLED;

    return SYS_ERR_OK;
}

