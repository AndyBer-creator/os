#include <common/sys_def.h>
#include <rsd/rsd_nic.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_qos.h>
#include <rsd/rsd_trunk.h>
#include <rsd/rsd_sys.h>
#include <rsd/rsd_trap.h>

#include <rsd/rsd_l2.h>
#include <lacp/proto_lacp.h>
#define INVALID_VALUE 0xffff
#define LACP_TICK_TIME 1

#define LACP_ETH_TYPE 0x8809
#define LACP_VERSION 0x1 
#define LACP_SUBTYPE 0x1

#define MARKER_TERMINATOR 0x0
#define MARKER_INFORMATION 0x1  
#define MARKER_RESPONSE_INFORMATION 0x2  
#define FAST_PERIODIC_TIME 1    
#define SLOW_PERIODIC_TIME 30   
#define SHORT_TIMEOUT_TIME 3    
#define LONG_TIMEOUT_TIME 90    
#define CHURN_DETECT_TIME 60    
#define AGG_WAIT_TIME 2         
static int32 lacp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
static rsd_nic_handler_t lacpHandler = {
    .priority       = RSD_NIC_HANDLER_PRI_LACP,
    .dmac           = { 0x01, 0x80, 0xC2, 0x00, 0x00, 0x02 },
    .dmac_caremask  = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
    .ethertype      = 0x8809,
    .proto          = 0,
    .sport          = 0,
    .dport          = 0,
    .action         = RSD_NIC_HANDLER_ACT_TRAP,
    .rx_cbf         = lacp_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_NONE,
    .pName          = "LACP RX Thread",
    .pCookie        = NULL,
};

static uint8 slowProtocolMAC[CAPA_MAC_BYTE_NUM] = {0x01,0x80,0xc2,0x00,0x00,0x02};
static uint32 lacp_enable = DISABLED;
static sys_system_var_t lacp_sysvar;
static sys_lacp_port_t lacp_ports[SYS_LOGICPORT_NUM_MAX];
static sys_lacp_agg_t  lacp_aggs[CAPA_LAG_NUM_MAX];

static sys_logic_portmask_t lacp_linkup_pm;
static sys_logic_portmask_t lacp_enable_pm;
static sys_logic_portmask_t lacp_active_pm;
static sys_logic_portmask_t lacp_backup_pm[CAPA_LAG_NUM_MAX];
static uint32 port2aggId[SYS_LOGICPORT_NUM_MAX];
static uint8 trkL2FlushCntDwn[SYS_LOGICPORT_NUM_MAX];
static sys_lacp_counter_t lacpCounter[SYS_LOGICPORT_NUM_MAX];

ROUTINE_INIT(lacpTimer, SYS_TIMER_SEC);
static int32 lacp_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    uint8 lacp_header_offset = 0;
    sys_logic_port_t srcPort;
    sys_lacp_pdu_t *lacp_header = NULL;
    sys_marker_pdu_t *marker_header = NULL;
    sys_lacp_port_t *pLacpPort = NULL;

    SYS_PARAM_CHK(NULL == pPkt, SYS_ERR_NULL_POINTER);

    if (pPkt->rx_tag.svid_tagged)
        lacp_header_offset += 4;
    if (pPkt->rx_tag.cvid_tagged)
        lacp_header_offset += 4;

    lacp_header = (sys_lacp_pdu_t *)(pPkt->data + 14 + lacp_header_offset);
    srcPort = pPkt->rx_tag.source_port;
    pLacpPort = &lacp_ports[srcPort];

    switch( (sys_lacp_subtype_t)(lacp_header->subtype) ){
        case LACPAU_SUBTYPE_LACP:
            if(ENABLED == pLacpPort->pv.port_enabled)
            {
                lacp_recv_machine(pPkt, 0, 0);
            }
            break;
        case LACPAU_SUBTYPE_MARKER:
            marker_header = (sys_marker_pdu_t *)(pPkt->data + 14 + lacp_header_offset);
            lacp_marker_responser(marker_header, srcPort);
            break;
        default:
            break;
    }

    rsd_nic_pkt_free(pPkt);

    return 0;
}
static void lacp_timer_routine(unsigned long id)
{
    uint32 trunkId;

    lacp_tick(1);

    FOR_EACH_TRUNK(trunkId)
    {
        if(trkL2FlushCntDwn[trunkId] > 0)
        {
            trkL2FlushCntDwn[trunkId]--;

            if(trkL2FlushCntDwn[trunkId]==0)
            {
                rsd_l2_mac_clearByPort(TRK2LP(trunkId));
            }
        }
    }
}
int32 lacp_linkEvent(uint32 event, char *pData)
{
    sys_port_event_t *pPortEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pPortEvent = (sys_port_event_t *)pData;

    if (PORT_LINKUP == pPortEvent->new_linkStatus)
    {
        LOGIC_PORTMASK_SET_PORT(lacp_linkup_pm, pPortEvent->lport);

        if (IS_LOGIC_PORTMASK_PORTSET(lacp_enable_pm, pPortEvent->lport))
            SYS_ERR_CHK(lacp_portState_set(pPortEvent->lport, STP_STATE_BLOCKING));
    }
    else if (PORT_LINKDOWN == pPortEvent->new_linkStatus)
    {
        if(IS_LOGIC_PORTMASK_PORTSET(lacp_linkup_pm, pPortEvent->lport))
        {
            
            if(IS_LOGIC_PORTMASK_PORTSET(lacp_enable_pm, pPortEvent->lport))
            {
                
                SYS_ERR_CHK(lacp_set_port_enb(pPortEvent->lport, DISABLED));
                SYS_ERR_CHK(lacp_set_port_enb(pPortEvent->lport, ENABLED));
                if(IS_LOGIC_PORTMASK_PORTSET(lacp_active_pm, pPortEvent->lport))
                    SYS_ERR_CHK(lacp_portActivity_set(pPortEvent->lport, TRUE));
                else
                    SYS_ERR_CHK(lacp_portActivity_set(pPortEvent->lport, FALSE));

                SYS_ERR_CHK(lacp_portState_set(pPortEvent->lport, STP_STATE_DISABLED));
            }
        }

        LOGIC_PORTMASK_CLEAR_PORT(lacp_linkup_pm, pPortEvent->lport);
    }

    return SYS_ERR_OK;
}
int32 lacp_init(void)
{
    sys_logic_port_t port;
    uint32 trunk;
    sys_mac_t SysMac;

    osal_memset(lacp_ports, 0, sizeof(sys_lacp_port_t) * SYS_LOGICPORT_NUM_MAX);
    osal_memset(&lacp_linkup_pm, 0, sizeof(sys_logic_portmask_t));
    osal_memset(&lacp_enable_pm, 0, sizeof(sys_logic_portmask_t));
    osal_memset(&lacp_active_pm, 0, sizeof(sys_logic_portmask_t));
    osal_memset(lacp_backup_pm, 0, sizeof(sys_logic_portmask_t) * CAPA_LAG_NUM_MAX);
    osal_memset(lacpCounter, 0, sizeof(lacpCounter));

    rsd_sys_macAddr_get(0, &SysMac);
    osal_memcpy(lacp_sysvar.actor_sys, SysMac.octet, 6);
    lacp_sysvar.actor_sys_pri = 0x8000;

    FOR_EACH_TRUNK(trunk)
    {
        lacp_init_aggregator(trunk);
    }

    FOR_EACH_NORMAL_LOGIC_PORT(port)
    {
        lacp_ports[port].pv.actor_port_num = port + 1;
        lacp_ports[port].pv.actor_port_pri = 0x8000;
        lacp_ports[port].pv.port_enabled = DISABLED;
        lacp_ports[port].pv.actor_admin_port_state = PS_AGGREGATION | PS_DEFAULTED;
        lacp_init_pn(port);
        port2aggId[port] = INVALID_VALUE; 
    }

    SYS_ERR_CHK(rsd_nic_handler_register(&lacpHandler));

    SYS_ERR_CHK(sys_notifier_observer_register("lacp", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG, lacp_linkEvent, SYS_NOTIFIER_PRI_MEDIUM));

    return SYS_ERR_OK;
}
int32 lacp_exit(void)
{
    ROUTINE_STOP(lacpTimer);

    SYS_ERR_CHK(rsd_nic_handler_unregister(&lacpHandler));

    SYS_ERR_CHK(sys_notifier_observer_unregister("lacp", SYS_NOTIFIER_SUBJECT_PORT, lacp_linkEvent));

    return SYS_ERR_OK;
}
int32 lacp_portCounter_get(sys_logic_port_t port, sys_lacp_counter_t *pPortCounter)
{
    *pPortCounter = lacpCounter[port];

    return SYS_ERR_OK;
}
int32 lacp_portCounter_reset(sys_logic_port_t port)
{
    osal_memset(&lacpCounter[port], 0, sizeof(lacpCounter[port]));

    return SYS_ERR_OK;
}
int32 lacp_group_set(
    uint32 trunkId,
    sys_logic_portmask_t *pPortmask,
    sys_logic_portmask_t *pActmask)
{
    sys_logic_port_t port;

    FOR_EACH_NORMAL_LOGIC_PORT(port)
    {
        
        if (IS_LOGIC_PORTMASK_PORTSET(*pPortmask, port))
        {
            
            if (port2aggId[port] == trunkId)
                continue;

            port2aggId[port] = trunkId;
            SYS_ERR_CHK(lacp_set_port_enb(port, ENABLED));
            if(IS_LOGIC_PORTMASK_PORTSET(*pActmask, port))
                SYS_ERR_CHK(lacp_portActivity_set(port, 1));
            else
                SYS_ERR_CHK(lacp_portActivity_set(port, 0));

            lacp_init_pn(port);
        }
        
        else if (port2aggId[port] == trunkId)
        {
            SYS_ERR_CHK(lacp_set_port_enb(port, DISABLED));
            SYS_ERR_CHK(lacp_portActivity_set(port, 0));
            port2aggId[port] = INVALID_VALUE;

            SYS_ERR_CHK(lacp_portState_set(port, STP_STATE_FORWARDING));

            lacp_init_pn(port);
        }
    }

    return SYS_ERR_OK;
}
int32 lacp_group_del(uint32 trunkId)
{
    sys_logic_port_t port;

    FOR_EACH_NORMAL_LOGIC_PORT(port)
    {
        if(port2aggId[port] == trunkId)
        {
            SYS_ERR_CHK(lacp_set_port_enb(port, DISABLED));
            SYS_ERR_CHK(lacp_portActivity_set(port, 0));
            port2aggId[port] = INVALID_VALUE;

            SYS_ERR_CHK(lacp_portState_set(port, STP_STATE_FORWARDING));
        }
    }

    return SYS_ERR_OK;
}
int32 lacp_enable_set(sys_enable_t enable)
{
    sys_logic_port_t port;
    sys_enable_t lacpEnable;

    SYS_ERR_CHK(lacp_enable_get(&lacpEnable));

    if (enable == lacpEnable)
        return SYS_ERR_OK;

    if ((ENABLED == lacpEnable) || (ENABLED == enable))
        SYS_ERR_CHK(rsd_acl_rsv_ruleEnable_set(ACL_RSV_LACP, enable));

    lacp_enable = enable;

    if (ENABLED == enable)
    {
        ROUTINE_START(lacpTimer, "LACP", lacp_timer_routine, SYS_THREAD_PRI_HIGH, 32768);
    }
    else if(DISABLED == enable)
    {
        ROUTINE_STOP(lacpTimer);

        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, lacp_enable_pm)
        {
            
            SYS_ERR_CHK(lacp_set_port_enb(port, DISABLED));
            SYS_ERR_CHK(lacp_set_port_enb(port, ENABLED));
            if(IS_LOGIC_PORTMASK_PORTSET(lacp_active_pm, port))
                SYS_ERR_CHK(lacp_portActivity_set(port, TRUE));
            else
                SYS_ERR_CHK(lacp_portActivity_set(port, FALSE));

            osal_memset(lacp_backup_pm, 0, sizeof(sys_logic_portmask_t) * CAPA_LAG_NUM_MAX);

            SYS_ERR_CHK(lacp_portState_set(port, STP_STATE_FORWARDING));
        }
    }

    return SYS_ERR_OK;
}
int32 lacp_enable_get(sys_enable_t *pEnable)
{
    *pEnable = lacp_enable;

    return SYS_ERR_OK;
}
int32 lacp_port_state_get(sys_logic_port_t port, sys_lacp_port_t *pPortState)
{
    *pPortState = lacp_ports[port];

    return SYS_ERR_OK;
}
int32 lacp_system_priority_set(uint32 sysPri)
{
    lacp_sysvar.actor_sys_pri = sysPri;
    return SYS_ERR_OK;
}
int32 lacp_system_priority_get(uint32 *pSysPri)
{
    *pSysPri = lacp_sysvar.actor_sys_pri;
    return SYS_ERR_OK;
}
int32 lacp_set_port_enb(sys_logic_port_t port, sys_enable_t enable)
{
    sys_enable_t enb_org;
    uint32 agg_id;

    enb_org = lacp_ports[port].pv.port_enabled;

    lacp_ports[port].pv.port_enabled = enable;

    if(ENABLED == enable)
    {
        LOGIC_PORTMASK_SET_PORT(lacp_enable_pm, port);

        lacp_portState_set(port, STP_STATE_BLOCKING);
    }
    else 
    {
        agg_id = port2aggId[port];
        LOGIC_PORTMASK_CLEAR_PORT(lacp_enable_pm, port);
        if(!IS_TRUNK_INVALID(agg_id))
        {
            
            if (IS_LOGIC_PORTMASK_PORTSET(lacp_aggs[agg_id].av.lag_ports, port))
            {
                
                LOGIC_PORTMASK_CLEAR_PORT(lacp_aggs[agg_id].av.lag_ports, port);
                lacp_aggs[agg_id].n_ports--;
                SYS_DBG(DBG_LAG, "!!![%d:P%d][pm %x lacp_aggs[%d].n_ports=0x%d]", (uint16)__LINE__, port, lacp_aggs[agg_id].av.lag_ports.bits[0], agg_id, lacp_aggs[agg_id].n_ports);

                if(0 == lacp_aggs[agg_id].n_ports)
                {
                    lacp_aggs[agg_id].av.avs &= ~AVS_USING;
                    SYS_DBG(DBG_LAG, "!!![%d:P%d][lacp_aggs[%d].av.avs=0x%x]", (uint16)__LINE__, port, agg_id, lacp_aggs[agg_id].av.avs);
                }

                rsd_trunk_port_del(agg_id, port);

                lacp_portState_set(port, STP_STATE_FORWARDING);
            }

            LOGIC_PORTMASK_CLEAR_PORT(lacp_backup_pm[agg_id], port);
        }
    }

    if (enb_org != enable)
        lacp_init_pn(port);

    return SYS_ERR_OK;
}
int32 lacp_portActivity_set(sys_logic_port_t port, uint32 active)
{
    if(TRUE == active)
    {
        lacp_ports[port].pv.actor_admin_port_state |= PS_LACP_ACTIVITY;
        lacp_ports[port].pv.actor_oper_port_state |= PS_LACP_ACTIVITY;
        LOGIC_PORTMASK_SET_PORT(lacp_active_pm, port);
    }
    else
    {
        lacp_ports[port].pv.actor_admin_port_state &= ~PS_LACP_ACTIVITY;
        lacp_ports[port].pv.actor_oper_port_state &= ~PS_LACP_ACTIVITY;
        LOGIC_PORTMASK_CLEAR_PORT(lacp_active_pm, port);
    }

    return SYS_ERR_OK;
}
int32 lacp_portPriority_set(sys_logic_port_t port, uint16 pri)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    lacp_ports[port].pv.actor_port_pri = pri;

    return SYS_ERR_OK;
}
int32 lacp_portTimeout_set(sys_logic_port_t port, sys_lacp_timeout_t timeout)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if (LACP_TIMEOUT_SHORT == timeout)
    {
        lacp_ports[port].pv.actor_admin_port_state |= PS_LACP_TIMEOUT;
        lacp_ports[port].pv.actor_oper_port_state |= PS_LACP_TIMEOUT;
    }
    else
    {
        lacp_ports[port].pv.actor_admin_port_state &= ~PS_LACP_TIMEOUT;
        lacp_ports[port].pv.actor_oper_port_state &= ~PS_LACP_TIMEOUT;
    }

    return SYS_ERR_OK;
}
int32 lacp_backupMember_get(uint32 trunkId, sys_logic_portmask_t *pPortmask)
{
    SYS_PARAM_CHK(IS_TRUNK_INVALID(trunkId), SYS_ERR_TRUNK_INVALID);
    SYS_PARAM_CHK((NULL == pPortmask), SYS_ERR_NULL_POINTER);

    *pPortmask = lacp_backup_pm[trunkId];

    return SYS_ERR_OK;
}
int32 lacp_tick(uint8 secs)
{
    sys_logic_port_t port;
    static uint16 lacp_ticks = 0;

    lacp_ticks += secs;

    if(0 == (lacp_ticks % LACP_TICK_TIME))
    {
        FOR_EACH_NORMAL_LOGIC_PORT(port)
        {
            if(IS_LOGIC_PORTMASK_PORTSET(lacp_enable_pm, port))
            {
                lacp_run_fsm(port, LACP_TICK_TIME);
            }
        }
        lacp_ticks = 0;
    }
    return SYS_ERR_OK;
}
int32 lacp_run_fsm(sys_logic_port_t port, uint8 secs)
{
    lacp_recv_machine(NULL, port, secs);
    lacp_periodic_xmit(port, secs);
    lacp_selection_logic(&lacp_ports[port]);
    lacp_mux_machine(&lacp_ports[port], secs);

    return SYS_ERR_OK;
}
int32 lacp_recv_machine(sys_nic_pkt_t *pPkt, sys_logic_port_t port, uint8 sec)
{
    uint8 lacp_header_offset = 0;
    sys_lacp_pdu_t *lacp_header = NULL;
    sys_rcvm_state_t last_state;
    sys_lacp_port_t *iter;

    if (NULL != pPkt){
                if (pPkt->rx_tag.svid_tagged)
            lacp_header_offset += 4;
        if (pPkt->rx_tag.cvid_tagged)
            lacp_header_offset += 4;

        lacp_header = (sys_lacp_pdu_t *)(pPkt->data + 14 + lacp_header_offset);

        port = pPkt->rx_tag.source_port;
                lacpCounter[port].lacpRecv ++;
    }

    iter = &lacp_ports[port];

    if (iter->pv.mosm & MOSM_BEGIN)
    {
        lacp_recv_init(iter);
    }
    else if (!(iter->pv.mosm & MOSM_BEGIN) &&
              FALSE == iter->pv.port_enabled &&
              0 == (iter->pv.mosm & MOSM_PORT_MOVED))
    {
        lacp_recv_port_dis(iter);
    }

    do
    {
        last_state = iter->rcvm.state;

        if (INITIALIZE == iter->rcvm.state)
        {
            iter->pv.mosm &= ~MOSM_SELECTED_MASK;
            iter->pv.mosm |= MOSM_SELECTED_NO;

            lacp_record_default(iter);
            iter->pv.actor_oper_port_state &= ~PS_EXPIRED;
            iter->pv.mosm &= (~MOSM_PORT_MOVED)&(~MOSM_BEGIN);
            iter->rcvm.state = PORT_DISABLED;
        }

        if (PORT_DISABLED == iter->rcvm.state)
        {
            if (TRUE == iter->rcvm.do_it)
            {
                iter->pv.partner_oper_port_state &= ~PS_SYNCHRONIZATION;
                iter->pv.actor_oper_port_state |= PS_AGGREGATION;
                iter->pv.mosm &= ~MOSM_PORT_LOOP;
                iter->rcvm.do_it = FALSE;
            }

            if(TRUE == iter->pv.port_enabled)
            {
                if(0 == (iter->pv.mosm & MOSM_LACP_ENALBE))
                {
                    iter->rcvm.state = LACP_DISABLED;
                }
                else
                {
                    iter->rcvm.state = EXPIRED;
                }
                iter->rcvm.do_it = TRUE;
            }
            if (MOSM_PORT_MOVED & iter->pv.mosm){
                iter->rcvm.state = INITIALIZE;
            }
        }

        if (EXPIRED == iter->rcvm.state)
        {
            if (TRUE == iter->rcvm.do_it)
            {
                iter->pv.partner_oper_port_state &= (~PS_SYNCHRONIZATION);

                iter->current_while_timer = SHORT_TIMEOUT_TIME;
                iter->pv.actor_oper_port_state |= PS_EXPIRED;
                iter->rcvm.do_it = FALSE;
            }

            iter->current_while_timer -= sec;
            sec = 0;
            if( 0>=iter->current_while_timer )
            {
                iter->rcvm.state = DEFAULTED;
                iter->rcvm.do_it = TRUE;
            }

            if ( NULL!=lacp_header )
            {
                iter->rcvm.state = CURRENT;
                iter->rcvm.do_it = TRUE;
            }
        }

        if (LACP_DISABLED == iter->rcvm.state)
        {
            if (TRUE == iter->rcvm.do_it)
            {
                iter->pv.mosm &= ~MOSM_SELECTED_MASK;
                iter->pv.mosm |= MOSM_SELECTED_NO;

                lacp_record_default(iter);
                iter->pv.partner_oper_port_state &= ~PS_AGGREGATION;
                iter->pv.actor_oper_port_state &= ~PS_EXPIRED;
                iter->rcvm.do_it = FALSE;
            }
            if((iter->pv.mosm & MOSM_LACP_ENALBE))
            {
                iter->rcvm.state = EXPIRED;
                iter->rcvm.do_it = TRUE;
            }
        }

        if (DEFAULTED == iter->rcvm.state)
        {
            if (TRUE == iter->rcvm.do_it)
            {
                lacp_update_default_selected(iter);
                lacp_record_default(iter);
                iter->pv.actor_oper_port_state &= ~PS_EXPIRED;
                iter->rcvm.do_it = FALSE;
            }

            if (NULL != lacp_header)
            {
                iter->rcvm.state = CURRENT;
                iter->rcvm.do_it = TRUE;
            }
        }

        if (CURRENT == iter->rcvm.state)
        {
            
            if (NULL != lacp_header)
            {
                lacp_update_selected(lacp_header, iter);
                lacp_record_pdu(lacp_header, iter);
                lacp_update_ntt(lacp_header, iter);

                iter->current_while_timer = (iter->pv.actor_oper_port_state & PS_LACP_TIMEOUT)? SHORT_TIMEOUT_TIME : LONG_TIMEOUT_TIME;
                iter->pv.actor_oper_port_state &= ~PS_EXPIRED;

            }
            lacp_header = NULL;

            iter->current_while_timer -= sec;
            sec = 0;
            if(0 >= iter->current_while_timer)
            {
                iter->rcvm.state = EXPIRED;
                SYS_LOG(LOG_CAT_LAG, LOG_LAG_LACP_MBR_TIMEOUT, LPSTR(port));
            }
        }
    }while(last_state != iter->rcvm.state);

    return SYS_ERR_OK;
}
int32 lacp_periodic_xmit(sys_logic_port_t port, uint8 sec)
{
    sys_perio_state_t last_state;
    sys_lacp_port_t *iter;
    uint8 enter_no_periodic;

    iter = &lacp_ports[port];

    enter_no_periodic = \
        (iter->pv.mosm & MOSM_BEGIN)          ||
        (!(iter->pv.mosm & MOSM_LACP_ENALBE)) ||
        (FALSE==iter->pv.port_enabled)        ||
        ( (!(iter->pv.actor_oper_port_state&PS_LACP_ACTIVITY)) &&
          (!(iter->pv.partner_oper_port_state&PS_LACP_ACTIVITY)) );

    if (enter_no_periodic)
    {
        iter->prm.state = NO_PERIODIC;
        return SYS_ERR_OK;
    }

    do
    {
        last_state = iter->prm.state;

        if (NO_PERIODIC == iter->prm.state)
        {
            iter->periodic_timer = (sys_lacp_timer_t)(-1);
            iter->prm.state = FAST_PERIODIC;
            iter->prm.do_it = TRUE;
        }

        if (FAST_PERIODIC == iter->prm.state)
        {
            if ( !(iter->pv.partner_oper_port_state&PS_LACP_TIMEOUT) )
            
            {
                iter->prm.state = SLOW_PERIODIC;
                iter->prm.do_it = TRUE;
            }else if ( TRUE==iter->prm.do_it  )
            {
                iter->periodic_timer = FAST_PERIODIC_TIME;
                iter->prm.do_it = FALSE;
            }

            iter->periodic_timer -= sec;
            sec = 0;

            if ( iter->periodic_timer<=0 )
            {
                iter->prm.state = PERIODIC_TX;
            }

        }

        if (SLOW_PERIODIC == iter->prm.state)
        {
            if (TRUE == iter->prm.do_it)
            {
                iter->periodic_timer = SLOW_PERIODIC_TIME;
                iter->prm.do_it = FALSE;
            }
            iter->periodic_timer -= sec;
            sec = 0;

            if ( iter->periodic_timer<=0
                     || (iter->pv.partner_oper_port_state & PS_LACP_TIMEOUT)
                
               )
            {
                iter->prm.state = PERIODIC_TX;
            }
        }

        if (PERIODIC_TX == iter->prm.state)
        {
                        lacp_xmit(iter);

            if ( iter->pv.partner_oper_port_state&PS_LACP_TIMEOUT
            
               )
            {
                iter->prm.state = FAST_PERIODIC;
            }
            else
            {
                iter->prm.state = SLOW_PERIODIC;
            }
            iter->prm.do_it = TRUE;
        }

    }while( last_state!=iter->prm.state );

    return SYS_ERR_OK;
}

int32 _lacp_backupPort_select(sys_lacp_port_t *p)
{
#define LACP_BAK_PORT_NUM (CAPA_LAG_MBR_NORMAL_NUM * 2)
    uint32 i;
    uint32 trunkId;
    uint32 tmpTrunkId;
    uint32 portNum;
    uint32 selectActor = FALSE;
    sys_logic_port_t port;
    sys_logic_port_t tmpPort;
    sys_lacp_backupPort_t bakInfo[LACP_BAK_PORT_NUM];
    sys_lacp_backupPort_t tmpBakInfo;
    osal_memset(bakInfo, 0, sizeof(sys_lacp_backupPort_t ) * LACP_BAK_PORT_NUM);
    port = p->pv.actor_port_num - 1;
    trunkId = port2aggId[port];

    if (lacp_sysvar.actor_sys_pri < p->pv.partner_oper_sys_pri)
    {
        selectActor = TRUE;
    }
    
    else if (lacp_sysvar.actor_sys_pri == p->pv.partner_oper_sys_pri)
    {
        selectActor = TRUE;

        for (i = 0; i < CAPA_MAC_BYTE_NUM; i++)
        {
            
            if (lacp_sysvar.actor_sys[i] == p->pv.partner_oper_sys[i])
            {
                continue;
            }
            
            if (lacp_sysvar.actor_sys[i] < p->pv.partner_oper_sys[i])
            {
                selectActor = TRUE;
                break;
            }
            else
            {
                selectActor = FALSE;
                break;
            }
        }
    }

    portNum = 0;
    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(tmpPort, lacp_enable_pm)
    {
        tmpTrunkId = port2aggId[tmpPort];
        if (IS_TRUNK_INVALID(tmpTrunkId) || trunkId != tmpTrunkId || portNum >= (CAPA_LAG_MBR_NORMAL_NUM * 2))
            continue;

        bakInfo[portNum].port = tmpPort;

        if (TRUE == selectActor)
            bakInfo[portNum].cmpPortId = tmpPort;
        else
            bakInfo[portNum].cmpPortId = lacp_ports[tmpPort].pv.partner_oper_port_num - 1;

        if (IS_LOGIC_PORTMASK_PORTSET(lacp_linkup_pm, tmpPort))
        {
            if (TRUE == selectActor)
                bakInfo[portNum].pri = lacp_ports[tmpPort].pv.actor_port_pri;
            else
                bakInfo[portNum].pri = lacp_ports[tmpPort].pv.partner_oper_port_pri;

            if (lacp_ports[tmpPort].rcvm.state == DEFAULTED)
                bakInfo[portNum].pri = 0xFFFFFFFF;
        }
        
        else
        {
            bakInfo[portNum].pri = 0xFFFFFFFF;
        }

        portNum++;
    }

    for(i = 0; i < LACP_BAK_PORT_NUM;)
    {
        if (0 == bakInfo[i].pri)
            break;

        if ((i == 0) || (bakInfo[i - 1].pri < bakInfo[i].pri)
            || ((bakInfo[i - 1].pri == bakInfo[i].pri) && (bakInfo[i - 1].cmpPortId <= bakInfo[i].cmpPortId)))
        {
            
            i++;
        }
        else
        {
            
            tmpBakInfo = bakInfo[i - 1];
            bakInfo[i - 1] = bakInfo[i];
            bakInfo[i] = tmpBakInfo;

            i--;
        }
    }

    for (i = CAPA_LAG_MBR_NORMAL_NUM; i < LACP_BAK_PORT_NUM; i++)
    {
        if (0 == bakInfo[i].pri)
            break;

        LOGIC_PORTMASK_SET_PORT(lacp_backup_pm[trunkId], bakInfo[i].port);

        if (!IS_LOGIC_PORTMASK_PORTSET(lacp_aggs[trunkId].av.lag_ports, bakInfo[i].port))
            continue;

        lacp_ports[bakInfo[i].port].muxm.state = DETACHED;
        lacp_ports[bakInfo[i].port].muxm.do_it = TRUE;
        lacp_ports[bakInfo[i].port].pv.mosm &= ~MOSM_SELECTED_MASK;
        SYS_DBG(DBG_LAG, "Detached port %u from trunk %u", LP2UP(bakInfo[i].port), TRK2UID(trunkId));
    }

    for (i = 0; i < CAPA_LAG_MBR_NORMAL_NUM; i++)
    {
        if (0 == bakInfo[i].pri)
            break;

        LOGIC_PORTMASK_CLEAR_PORT(lacp_backup_pm[trunkId], bakInfo[i].port);

        if (IS_LOGIC_PORTMASK_PORTSET(lacp_aggs[trunkId].av.lag_ports, bakInfo[i].port))
            continue;

        if (port != bakInfo[i].port)
            continue;

        LOGIC_PORTMASK_SET_PORT(lacp_aggs[trunkId].av.lag_ports, port);
        p->pv.actor_port_agg_id = TRK2UID(trunkId);
        p->pv.mosm &= ~MOSM_SELECTED_MASK;
        p->pv.mosm |= MOSM_SELECTED_STANDBY;
        lacp_aggs[trunkId].n_ports++;

        SYS_DBG(DBG_LAG, "Select port %u to trunk %u", LP2UP(bakInfo[i].port), TRK2UID(trunkId));
    }

    return SYS_ERR_OK;
}
int32 lacp_selection_logic(sys_lacp_port_t *p)
{
    sys_logic_port_t port;
    uint16 i, agg_id, trunk_port;
    sys_logic_portmask_t pm;
    sys_mosm_t mosm_tmp;

    agg_id = p->pv.actor_port_agg_id;
    
    if ((osal_memcmp(lacp_sysvar.actor_sys, p->pv.partner_oper_sys, CAPA_MAC_BYTE_NUM) == 0) &&
        (p->pv.partner_oper_key) == (p->pv.actor_oper_port_key))
    {
        if (agg_id){
            p->pv.mosm &= ~MOSM_SELECTED_MASK ;
            p->pv.mosm |= MOSM_SELECTED_NO | MOSM_PORT_LOOP;
            p->pv.actor_oper_port_state &= ~PS_AGGREGATION; 
        }
        return SYS_ERR_OK;
    }

    if ( p->pv.actor_oper_port_state & PS_DEFAULTED        || \
         !(p->pv.actor_oper_port_state & PS_AGGREGATION)   || \
         !(p->pv.partner_oper_port_state & PS_AGGREGATION))
    {
        return SYS_ERR_OK;
    }

    port = p->pv.actor_port_num - 1;
    mosm_tmp = p->pv.mosm & MOSM_SELECTED_MASK ;
    if ( 0==agg_id                    &&
         mosm_tmp == MOSM_SELECTED_NO &&
         p->muxm.state == DETACHED       )
    {
        if (INVALID_VALUE != port2aggId[port])
        {
            
            agg_id = port2aggId[port];

            if (lacp_aggs[agg_id].av.avs & AVS_USING)
            {
                
                pm = lacp_aggs[agg_id].av.lag_ports;
                FOR_EACH_NORMAL_LOGIC_PORT(trunk_port)
                {
                                        if(IS_LOGIC_PORTMASK_PORTSET(pm, trunk_port))
                    {
                        break;
                    }
                }

                if (lacp_aggs[agg_id].n_ports >= CAPA_LAG_MBR_NORMAL_NUM)
                {
                    _lacp_backupPort_select(p);
                    SYS_LOG(LOG_CAT_LAG, LOG_LAG_LACP_MBR_EXCEED, CAPA_LAG_MBR_NORMAL_NUM);
                }
                else
                if (trunk_port < SYS_NORMAL_LOGIC_PORT_NUM && \
                     lacp_compare_link_ret(&lacp_ports[port], &lacp_ports[trunk_port]))
                {
                    
                    LOGIC_PORTMASK_CLEAR_PORT(lacp_backup_pm[agg_id], port);

                    p->pv.actor_port_agg_id = agg_id + 1;
                    
                    LOGIC_PORTMASK_SET_PORT(lacp_aggs[agg_id].av.lag_ports, port);
                    p->pv.mosm &= ~MOSM_SELECTED_MASK;
                    p->pv.mosm |= MOSM_SELECTED_STANDBY;
                    lacp_aggs[agg_id].n_ports++;
                    SYS_DBG(DBG_LAG, "[%d:P%d][lag_ports=0x%x]", (uint16)__LINE__, port, lacp_aggs[agg_id].av.lag_ports.bits[0]);
                }
            }
            else
            {
                
                LOGIC_PORTMASK_CLEAR_PORT(lacp_backup_pm[agg_id], port);

                p->pv.actor_port_agg_id         = agg_id+1;
                lacp_aggs[agg_id].av.avs       |= AVS_USING;
                
                osal_memset(&lacp_aggs[agg_id].av.lag_ports, 0, sizeof(sys_logic_portmask_t));
                LOGIC_PORTMASK_SET_PORT(lacp_aggs[agg_id].av.lag_ports, (p->pv.actor_port_num-1));
                lacp_aggs[agg_id].n_ports       = 1;
                p->pv.mosm &= ~MOSM_SELECTED_MASK;
                p->pv.mosm |= MOSM_SELECTED_STANDBY;
                SYS_DBG(DBG_LAG, "[%d:P%d][lag_ports=0x%x]", (uint16)__LINE__, port, lacp_aggs[agg_id].av.lag_ports.bits[0]);
                SYS_DBG(DBG_LAG, "[%d:P%d][SELECT:UNUSING]", (uint16)__LINE__, port);
            }
        }
        else
        {
            
            FOR_EACH_TRUNK(i)
            {
                if ( lacp_aggs[i].av.avs & AVS_USING )
                {
                    pm = lacp_aggs[i].av.lag_ports;
                    FOR_EACH_NORMAL_LOGIC_PORT(trunk_port)
                    {
                                                if (IS_LOGIC_PORTMASK_PORTSET(pm, trunk_port))
                        {
                            break;
                        }
                    }

                    if (trunk_port < SYS_NORMAL_LOGIC_PORT_NUM && \
                         lacp_compare_link_ret(&lacp_ports[port], &lacp_ports[trunk_port]))
                    {
                        p->pv.actor_port_agg_id = i+1;
                        SYS_DBG(DBG_LAG, "[%d:P%d][SELECT:USING]", (uint16)__LINE__, port);
                        
                        LOGIC_PORTMASK_SET_PORT(lacp_aggs[i].av.lag_ports, (p->pv.actor_port_num - 1));
                        p->pv.mosm &= ~MOSM_SELECTED_MASK;
                        p->pv.mosm |= MOSM_SELECTED_STANDBY;
                        lacp_aggs[i].n_ports++;
                        break;
                    }
                }
            }

            if (i >= CAPA_LAG_NUM)
            {
                FOR_EACH_TRUNK(i)
                {
                    if ( !(lacp_aggs[i].av.avs & AVS_USING) )
                    {
                        p->pv.actor_port_agg_id    = i+1;
                        lacp_aggs[i].av.avs       |= AVS_USING;
                        
                        osal_memset(&lacp_aggs[i].av.lag_ports, 0, sizeof(sys_logic_portmask_t));
                        LOGIC_PORTMASK_SET_PORT(lacp_aggs[i].av.lag_ports, p->pv.actor_port_num-1);
                        lacp_aggs[i].n_ports       = 1;
                        p->pv.mosm &= ~MOSM_SELECTED_MASK;
                        p->pv.mosm |= MOSM_SELECTED_STANDBY;
                        SYS_DBG(DBG_LAG, "[%d:P%d][aggregator=%d]", (uint16)__LINE__, port, i);
                        SYS_DBG(DBG_LAG, "[%d:P%d][SELECT:UNUSING]", (uint16)__LINE__, port);
                        break;
                    }
                }
            }
        }
    }

    mosm_tmp = p->pv.mosm & MOSM_SELECTED_MASK ;
    if ( mosm_tmp == MOSM_SELECTED_STANDBY )
    {
        SYS_DBG(DBG_LAG, "[%d:P%d][SELECT:SELECTED_STANDBY]", (uint16)__LINE__, port);

        agg_id = p->pv.actor_port_agg_id;
        
        FOR_EACH_NORMAL_LOGIC_PORT(i)
        {
            
            if((IS_LOGIC_PORTMASK_PORTSET(lacp_aggs[agg_id-1].av.lag_ports, i)) &&\
                p->pv.actor_port_num != lacp_ports[i].pv.actor_port_num    &&\
                !(lacp_ports[i].pv.mosm&MOSM_READY_N)                         )
            {
                SYS_DBG(DBG_LAG, "[%d:P%d][SELECT:ready_n==false]", (uint16)__LINE__, port);
                break;
            }
        }

        if (SYS_NORMAL_LOGIC_PORT_NUM == i)
        {
            lacp_aggs[agg_id-1].av.avs |= AVS_READY;
            p->pv.mosm &= ~MOSM_SELECTED_MASK;
            p->pv.mosm |= MOSM_SELECTED_YES;
            SYS_DBG(DBG_LAG, "[%d:P%d][SELECT:SELECTED_YES]", (uint16)__LINE__, port);
        }
        else
        {
            lacp_aggs[agg_id-1].av.avs &= ~AVS_READY;
        }
    }

    return SYS_ERR_OK;
}
int32 lacp_mux_machine(sys_lacp_port_t *p, uint8 sec)
{
    sys_muxm_state_t last_state;
    sys_mosm_t mosm_tmp;
    sys_lacp_agg_t *agg_iter;
    uint8 need_to_xmit = FALSE;

    if ((p->pv.mosm & MOSM_BEGIN) &&
         (MUXM_RESERVED == p->muxm.state))
    {
        p->muxm.state = DETACHED;
        p->muxm.do_it = TRUE;
    }

    do
    {
        last_state = p->muxm.state;

        if (DETACHED == p->muxm.state)
        {
            
            if (TRUE == p->muxm.do_it)
            {
                lacp_detach_mux_from_agg(p);
                p->pv.actor_oper_port_state &= ~PS_SYNCHRONIZATION;
                lacp_dis_distrib(p);
                lacp_dis_collect(p);
                                                need_to_xmit = TRUE;
                p->muxm.do_it = FALSE;
                SYS_DBG(DBG_LAG, "[%d:P%d][MUX:DETACHED]", (uint16)__LINE__, p->pv.actor_port_num - 1);
            }

            mosm_tmp = p->pv.mosm & MOSM_SELECTED_MASK;

            if((MOSM_SELECTED_YES == mosm_tmp) || (MOSM_SELECTED_STANDBY == mosm_tmp))
            {
                p->muxm.state = WAITING;
                p->muxm.do_it = TRUE;
            }
        }

        if (WAITING == p->muxm.state)
        {
            if (TRUE == p->muxm.do_it)
            {
                p->wait_while_timer = AGG_WAIT_TIME;
                p->pv.mosm |= MOSM_READY_N;
                p->muxm.do_it = FALSE;
                SYS_DBG(DBG_LAG, "[%d:P%d][MUX:WAITING]", (uint16)__LINE__, p->pv.actor_port_num - 1);
            }
            else if (p->wait_while_timer > 0)
            {
                p->wait_while_timer -= sec;
                sec = 0;
            }

            mosm_tmp = p->pv.mosm & MOSM_SELECTED_MASK;
            if (p->wait_while_timer <= 0)
            {
                SYS_DBG(DBG_LAG, "[%d:P%d][MUX:WAIT_DONE]", (uint16)__LINE__, p->pv.actor_port_num - 1);

                if(0 != p->pv.actor_port_agg_id)
                {
                    agg_iter = &lacp_aggs[(p->pv.actor_port_agg_id)-1];
                }
                else
                {
                    return SYS_ERR_OK;
                }

                if((MOSM_SELECTED_YES == mosm_tmp) && (agg_iter->av.avs & AVS_READY))
                {
                    p->muxm.state = ATTACHED;
                    p->muxm.do_it = TRUE;
                }
            }

            if((MOSM_SELECTED_NO == mosm_tmp))
            {
                p->muxm.state = DETACHED;
                p->muxm.do_it = TRUE;
            }

        }

        if (ATTACHED == p->muxm.state)
        {
            if (TRUE == p->muxm.do_it)
            {
                lacp_attach_mux_to_agg(p);
                p->pv.actor_oper_port_state |= PS_SYNCHRONIZATION;
                SYS_DBG(DBG_LAG, "[%d:P%d][MUX:ATTACHED]", (uint16)__LINE__, p->pv.actor_port_num - 1);
                lacp_dis_collect(p);
                                                need_to_xmit = TRUE;
                p->muxm.do_it = FALSE;
            }

            mosm_tmp = p->pv.mosm & MOSM_SELECTED_MASK;
            if((MOSM_SELECTED_YES == mosm_tmp) && \
                (p->pv.partner_oper_port_state & PS_SYNCHRONIZATION))
            {
                p->muxm.state = COLLECTING;
                p->muxm.do_it = TRUE;
            }

            if((MOSM_SELECTED_NO == mosm_tmp) || \
                (MOSM_SELECTED_STANDBY == mosm_tmp))
            {
                p->muxm.state = DETACHED;
                p->muxm.do_it = TRUE;
            }
        }

        if (COLLECTING == p->muxm.state)
        {
            if (TRUE == p->muxm.do_it)
            {
                lacp_ena_collect(p);
                lacp_dis_distrib(p);

                                                need_to_xmit = TRUE;
                p->muxm.do_it = FALSE;
                SYS_DBG(DBG_LAG, "[%d:P%d][MUX:COLLECTING]", (uint16)__LINE__, p->pv.actor_port_num - 1);
            }

            mosm_tmp = p->pv.mosm & MOSM_SELECTED_MASK;
            if ((MOSM_SELECTED_NO == mosm_tmp)      ||
                (MOSM_SELECTED_STANDBY == mosm_tmp) ||
                (0 == (p->pv.partner_oper_port_state & PS_SYNCHRONIZATION)))
            {
                 p->muxm.state = ATTACHED;
                 p->muxm.do_it = TRUE;
            }

            if ((MOSM_SELECTED_YES == mosm_tmp) &&
                (p->pv.partner_oper_port_state & PS_SYNCHRONIZATION) &&
                (p->pv.partner_oper_port_state & PS_COLLECTING)
                 )
            {
                 p->muxm.state = DISTRIBUTING;
                 p->muxm.do_it = TRUE;
            }
        }

        if (DISTRIBUTING == p->muxm.state)
        {
            if (TRUE == p->muxm.do_it)
            {
                                                lacp_ena_distrib(p);
                p->muxm.do_it = FALSE;
                SYS_DBG(DBG_LAG, "[%d:P%d][MUX:DISTRIBUTING]", (uint16)__LINE__, p->pv.actor_port_num - 1);
            }

            mosm_tmp = p->pv.mosm & MOSM_SELECTED_MASK;
            if ((MOSM_SELECTED_NO == mosm_tmp) ||
                (MOSM_SELECTED_STANDBY == mosm_tmp) ||
                (0 == (p->pv.partner_oper_port_state & PS_SYNCHRONIZATION)) ||
                (0 == (p->pv.partner_oper_port_state & PS_COLLECTING)))
            {
                 p->muxm.state = COLLECTING;
                 p->muxm.do_it = TRUE;
            }
        }
    }while( last_state!=p->muxm.state );

    if ( TRUE==need_to_xmit )
    {
        lacp_xmit(p);
    }

    return SYS_ERR_OK;
}
int32 lacp_set_admin_port_vars(sys_lacp_port_t *p, uint16 key)
{
    p->pv.actor_admin_port_key = p->pv.actor_oper_port_key = key;
        p->pv.actor_admin_port_state |= PS_DEFAULTED | PS_AGGREGATION;
    p->pv.partner_admin_key = key;

    osal_memset(p->pv.partner_admin_sys, 0, CAPA_MAC_BYTE_NUM);
    p->pv.partner_admin_sys_pri = 0x8000;
    p->pv.partner_admin_port_num = p->pv.actor_port_num;
    p->pv.partner_admin_port_pri = 0x8000;
    p->pv.partner_admin_port_state = PS_DEFAULTED | PS_SYNCHRONIZATION | \
        PS_COLLECTING | PS_AGGREGATION | PS_LACP_TIMEOUT;

    return SYS_ERR_OK;
}
int32 lacp_init_pn(sys_logic_port_t port)
{
    sys_lacp_port_t *p;

    p = &lacp_ports[port];

    p->pv.mosm = MOSM_BEGIN | MOSM_LACP_ENALBE;
    p->pv.actor_oper_port_state = p->pv.actor_admin_port_state;

    p->prm.state  = PERIO_RESERVED;
    p->muxm.state = MUXM_RESERVED;
    p->rcvm.state = RCVM_RESERVED;
    p->current_while_timer = (sys_lacp_timer_t) (-1);
    p->actor_churn_timer   = (sys_lacp_timer_t) (-1);
    p->periodic_timer      = (sys_lacp_timer_t) (-1);
    p->partner_churn_timer = (sys_lacp_timer_t) (-1);
    p->wait_while_timer    = (sys_lacp_timer_t) (-1);
    p->xmit_timer          = (sys_lacp_timer_t) (-1);

    lacp_set_admin_port_vars(p, LP2UP(TRK2LP(port2aggId[port])));
    lacp_periodic_xmit(port, 0);
    lacp_mux_machine(p, 0);
    lacp_recv_machine(NULL, port, 0); 
    return SYS_ERR_OK;
}
int32 lacp_init_aggregator(uint32 trunk)
{
    osal_memset( &lacp_aggs[trunk], 0, sizeof(sys_lacp_agg_t));
    osal_memcpy(&lacp_aggs[trunk].av.agg_mac, &lacp_sysvar.actor_sys, CAPA_MAC_BYTE_NUM);
    lacp_aggs[trunk].av.agg_id = trunk + 1;

    return SYS_ERR_OK;
}
int32 lacp_record_default(sys_lacp_port_t *p)
{
    p->pv.partner_oper_port_num = p->pv.partner_admin_port_num;
    p->pv.partner_oper_port_pri = p->pv.partner_admin_port_pri;
    osal_memcpy(p->pv.partner_oper_sys, p->pv.partner_admin_sys, CAPA_MAC_BYTE_NUM);
    p->pv.partner_oper_sys_pri = p->pv.partner_admin_sys_pri;
    p->pv.partner_oper_key = p->pv.partner_admin_key;
    p->pv.partner_oper_port_state = p->pv.partner_admin_port_state;
    p->pv.partner_oper_port_state &= ~PS_SYNCHRONIZATION;
    p->pv.actor_oper_port_state |= PS_DEFAULTED;

    return SYS_ERR_OK;
}
int32 lacp_update_default_selected(sys_lacp_port_t *p)
{
    uint8 param_match;

    param_match =
        (p->pv.partner_admin_port_num == p->pv.partner_oper_port_num) &&
        (p->pv.partner_admin_port_pri == p->pv.partner_oper_port_pri) &&
        (osal_memcmp(p->pv.partner_admin_sys, p->pv.partner_oper_sys, CAPA_MAC_BYTE_NUM) == 0) &&
        (p->pv.partner_admin_sys_pri == p->pv.partner_oper_sys_pri) &&
        (p->pv.partner_admin_key == p->pv.partner_oper_key) &&
        ((p->pv.partner_admin_port_state & PS_AGGREGATION) ==
        (p->pv.partner_oper_port_state & PS_AGGREGATION));

    if (!param_match)
    {
        p->pv.mosm &= ~MOSM_SELECTED_MASK;
        p->pv.mosm |= MOSM_SELECTED_NO;
    }
    else
    {
        
        p->pv.partner_oper_port_state |= PS_SYNCHRONIZATION;
    }

    return SYS_ERR_OK;
}
int32 lacp_record_pdu(sys_lacp_pdu_t *buf, sys_lacp_port_t *p){
    uint8 param_match;
    int32 is_active_link;
    int32 aggNo;

    p->pv.partner_oper_port_num = ntohs(buf->actor_port);
    p->pv.partner_oper_port_pri = ntohs(buf->actor_port_pri);
    osal_memcpy(p->pv.partner_oper_sys, buf->actor_sys, CAPA_MAC_BYTE_NUM);

    p->pv.partner_oper_sys_pri = ntohs(buf->actor_sys_pri);
    p->pv.partner_oper_key = ntohs(buf->actor_key);
    p->pv.partner_oper_port_state = buf->actor_state;

    p->pv.actor_oper_port_state &= ~PS_DEFAULTED;
    param_match =
        (p->pv.actor_port_num == ntohs(buf->partner_port)) &&
        (p->pv.actor_port_pri == ntohs(buf->partner_port_pri)) &&
        (osal_memcmp(lacp_sysvar.actor_sys, buf->partner_sys, CAPA_MAC_BYTE_NUM) == 0) &&
        (lacp_sysvar.actor_sys_pri == ntohs(buf->partner_sys_pri)) &&
        (p->pv.actor_oper_port_key == ntohs(buf->partner_key)) &&
        ((p->pv.actor_oper_port_state & PS_AGGREGATION) ==
         (buf->partner_state & PS_AGGREGATION));

    is_active_link = (buf->actor_state & PS_LACP_ACTIVITY) ||
        ((p->pv.actor_oper_port_state & PS_LACP_ACTIVITY) &&
        (buf->partner_state & PS_LACP_ACTIVITY));

    if (is_active_link &&
        ((param_match && (buf->actor_state & PS_SYNCHRONIZATION)) ||
        ((buf->actor_state & (PS_AGGREGATION | PS_SYNCHRONIZATION)) == PS_SYNCHRONIZATION)))
    {
        p->pv.partner_oper_port_state |= PS_SYNCHRONIZATION;
    }
    else
    {
        p->pv.partner_oper_port_state &= ~PS_SYNCHRONIZATION;
    }
    
    if (p->pv.actor_port_agg_id)
    {
        aggNo = p->pv.actor_port_agg_id-1;

        osal_memcpy(lacp_aggs[aggNo].av.partner_sys, p->pv.partner_oper_sys, CAPA_MAC_BYTE_NUM);
        lacp_aggs[aggNo].av.partner_sys_pri = p->pv.partner_oper_sys_pri;
    }

    return SYS_ERR_OK;
}
int32 lacp_update_selected(sys_lacp_pdu_t *buf, sys_lacp_port_t *p)
{
    uint8 param_match;
    param_match = (p->pv.partner_oper_port_num == ntohs(buf->actor_port)) &&
        (p->pv.partner_oper_port_pri == ntohs(buf->actor_port_pri)) &&
        (osal_memcmp(p->pv.partner_oper_sys, buf->actor_sys, CAPA_MAC_BYTE_NUM) == 0) &&
        (p->pv.partner_oper_sys_pri == ntohs(buf->actor_sys_pri)) &&
        (p->pv.partner_oper_key == ntohs(buf->actor_key)) &&
        ((p->pv.partner_oper_port_state & PS_AGGREGATION) ==
        (buf->actor_state & PS_AGGREGATION));

    if (!param_match){
        p->pv.mosm &= ~MOSM_SELECTED_MASK;
        p->pv.mosm |= MOSM_SELECTED_NO;
    }

    return SYS_ERR_OK;
}
int32 lacp_xmit(sys_lacp_port_t *p)
{
    sys_logic_port_t port;
    sys_logic_portmask_t txPortmask;
    sys_nic_pkt_t *pPktNew;
    sys_lacp_pdu_t *pLacpBuf;
    sys_eth_hdr_t *pEth_hdr;
    sys_mac_t portMac;

    if (0 == IS_LOGIC_PORTMASK_PORTSET(lacp_linkup_pm, p->pv.actor_port_num-1))
    {
        return SYS_ERR_FAILED;
    }

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(1500, &pPktNew))
    {
        port = htons(p->pv.actor_port_num);

        pEth_hdr = (sys_eth_hdr_t *)pPktNew->data;

        SYS_MEM_CLEAR(portMac);
        rsd_sys_portMacAddr_get(port, &portMac);
        osal_memcpy(pEth_hdr->src_mac, &portMac, CAPA_MAC_BYTE_NUM);

        osal_memcpy(pEth_hdr->dst_mac, slowProtocolMAC, CAPA_MAC_BYTE_NUM);
        pEth_hdr->ether_type = htons(LACP_ETH_TYPE);

        pPktNew->tail += sizeof(sys_eth_hdr_t);

        pLacpBuf = (sys_lacp_pdu_t *)(pPktNew->data + sizeof(sys_eth_hdr_t));

        pLacpBuf->subtype = LACP_SUBTYPE;
        pLacpBuf->version_number = LACP_VERSION;

        pLacpBuf->first_tlv_type = 0x1; 
        pLacpBuf->actor_info_len = 0x14;
        pLacpBuf->actor_sys_pri = htons(lacp_sysvar.actor_sys_pri);

        osal_memcpy(pLacpBuf->actor_sys, lacp_sysvar.actor_sys, CAPA_MAC_BYTE_NUM);
        pLacpBuf->actor_key = htons(p->pv.actor_oper_port_key);
        pLacpBuf->actor_port_pri = htons(p->pv.actor_port_pri);
        pLacpBuf->actor_port = port;
        pLacpBuf->actor_state = p->pv.actor_oper_port_state;
        pLacpBuf->pad1[0] = 0;
        pLacpBuf->pad1[1] = 0;
        pLacpBuf->pad1[2] = 0;

        pLacpBuf->second_tlv_type = 0x2; 
        pLacpBuf->partner_info_len = 0x14;
        pLacpBuf->partner_sys_pri = htons(p->pv.partner_oper_sys_pri);
        osal_memcpy(pLacpBuf->partner_sys, p->pv.partner_oper_sys, CAPA_MAC_BYTE_NUM);
        pLacpBuf->partner_key = htons(p->pv.partner_oper_key);
        pLacpBuf->partner_port_pri = htons(p->pv.partner_oper_port_pri);
        pLacpBuf->partner_port = htons(p->pv.partner_oper_port_num);
        pLacpBuf->partner_state = p->pv.partner_oper_port_state;
        pLacpBuf->pad2[0] = 0;
        pLacpBuf->pad2[1] = 0;
        pLacpBuf->pad2[2] = 0;

        pLacpBuf->third_tlv_type = 0x3; 
        pLacpBuf->collector_info_len = 0x10;
        pLacpBuf->collector_max_del = 0;
        osal_memset( pLacpBuf->pad3, 0, 12);

        pLacpBuf->fourth_tlv_type = 0x0; 
        pLacpBuf->terminator_len = 0x0;
        osal_memset( pLacpBuf->pad4, 0, 50);

        pPktNew->tail += sizeof(*pLacpBuf);
        pPktNew->length = sizeof(sys_eth_hdr_t) + sizeof(*pLacpBuf);
        osal_memset(&txPortmask, 0, sizeof(txPortmask));
        LOGIC_PORTMASK_SET_PORT(txPortmask, port - 1);

        rsd_nic_pkt_tx(pPktNew, &txPortmask, SYS_CPU_PORT_PRIORITY_LACP, 0 , RSD_NIC_TX_CARE_DOT1X );

        lacpCounter[port - 1].lacpSent ++;
    }

        return SYS_ERR_OK;
}
int32 lacp_update_ntt(sys_lacp_pdu_t *buf, sys_lacp_port_t *p){
    uint8 param_match;

    param_match =
        (p->pv.actor_port_num == ntohs(buf->partner_port)) &&
        (p->pv.actor_port_pri == ntohs(buf->partner_port_pri)) &&
        (osal_memcmp(lacp_sysvar.actor_sys, buf->partner_sys, CAPA_MAC_BYTE_NUM) == 0) &&
        (lacp_sysvar.actor_sys_pri == ntohs(buf->partner_sys_pri)) &&
        (p->pv.actor_oper_port_key == ntohs(buf->partner_key)) &&
        ((p->pv.actor_oper_port_state & (PS_LACP_ACTIVITY | PS_LACP_TIMEOUT | PS_AGGREGATION)) ==
        (buf->partner_state & (PS_LACP_ACTIVITY | PS_LACP_TIMEOUT | PS_AGGREGATION)));

    if (!param_match)
    {
                lacp_xmit(p);
    }

    return SYS_ERR_OK;
}
int32 lacp_recv_init(sys_lacp_port_t * p)
{
    p->rcvm.state = INITIALIZE;
    p->rcvm.do_it = TRUE;

    return SYS_ERR_OK;
}
int32 lacp_recv_port_dis(sys_lacp_port_t * p)
{
    p->rcvm.state = PORT_DISABLED;
    p->rcvm.do_it = TRUE;

    return SYS_ERR_OK;
}
uint32 lacp_compare_link_ret(sys_lacp_port_t *p1, sys_lacp_port_t *p2)
{
    sys_port_speedDuplex_t speedDuplex1, speedDuplex2;
    uint8  check_state;

    rsd_port_speedDuplex_get(p1->pv.actor_port_num-1, &speedDuplex1);

    rsd_port_speedDuplex_get(p2->pv.actor_port_num-1, &speedDuplex2);

    check_state = \
                                       \
        (p1->pv.actor_oper_port_key == p2->pv.actor_oper_port_key)          &&\
        (osal_memcmp(p1->pv.partner_oper_sys, p2->pv.partner_oper_sys, CAPA_MAC_BYTE_NUM) == 0)&&\
        (p1->pv.partner_oper_key == p2->pv.partner_oper_key)                &&\
        (speedDuplex1.speed == speedDuplex2.speed)                                      &&\
        (speedDuplex1.duplex == speedDuplex2.duplex);

    SYS_DBG(DBG_LAG, "[%d:P%d]Compare to P%d:%d %d %d %d %d", __LINE__,
        p1->pv.actor_port_num - 1,
        p2->pv.actor_port_num - 1,
        (p1->pv.actor_oper_port_key == p2->pv.actor_oper_port_key),
        (osal_memcmp(p1->pv.partner_oper_sys, p2->pv.partner_oper_sys, CAPA_MAC_BYTE_NUM) == 0),
        (p1->pv.partner_oper_key == p2->pv.partner_oper_key),
        (speedDuplex1.speed == speedDuplex2.speed),
        (speedDuplex1.duplex == speedDuplex2.duplex));

    return check_state;
}
int32 lacp_ena_collect(sys_lacp_port_t *p)
{
    p->pv.actor_oper_port_state |= PS_COLLECTING;
    return SYS_ERR_OK;
}
int32 lacp_dis_collect(sys_lacp_port_t *p)
{
    p->pv.actor_oper_port_state &= ~PS_COLLECTING;
    return SYS_ERR_OK;
}
int32 lacp_ena_distrib(sys_lacp_port_t *p)
{
        int32 ret;
    int agg_id;
    sys_logic_port_t port;

    agg_id = p->pv.actor_port_agg_id - 1;
    port = p->pv.actor_port_num - 1;

    p->pv.actor_oper_port_state |= PS_DISTRIBUTING;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, lacp_aggs[agg_id].av.lag_ports)
    {
        
        lacp_portState_set(port, STP_STATE_FORWARDING);

        ret = rsd_trunk_port_set(agg_id, port);
        if( SYS_ERR_OK != ret )
        {
            SYS_DBG(DBG_LAG, "LACP: set trunk fail, trunk %d port %d fail number:%d %s", agg_id, port, ret, SYS_ERR_STR(ret));
            lacp_portState_set(port, STP_STATE_BLOCKING);
            continue;
        }

        trkL2FlushCntDwn[agg_id] = 4;
    }

    return SYS_ERR_OK;
}
int32 lacp_dis_distrib(sys_lacp_port_t *p)
{
    p->pv.actor_oper_port_state &= ~PS_DISTRIBUTING;

    return SYS_ERR_OK;
}
int32 lacp_attach_mux_to_agg(sys_lacp_port_t *p)
{
    p->pv.mosm |= MOSM_PORT_ATTACHED;
    return SYS_ERR_OK;
}
int32 lacp_detach_mux_from_agg(sys_lacp_port_t *p)
{
    uint32 agg, port;
    sys_logic_portmask_t pm;
    sys_lacp_agg_t *ag;

    osal_memset(&pm, 0, sizeof(pm));

    p->pv.mosm &= ~MOSM_PORT_ATTACHED;

    agg = p->pv.actor_port_agg_id;
    if (!agg){
        
        return SYS_ERR_OK;
    }

    port = p->pv.actor_port_num;

    if (!port){
        
        return SYS_ERR_FAILED;
    }
    port--;

    agg--;

    ag = lacp_aggs + agg;
    pm = ag->av.lag_ports;

        if ((IS_LOGIC_PORTMASK_PORTSET(pm, port)) && ((p->pv.mosm&MOSM_SELECTED_MASK) == MOSM_SELECTED_NO))
    {
                LOGIC_PORTMASK_CLEAR_PORT(pm, port);

        rsd_trunk_port_del(agg, port);

        if (IS_LOGIC_PORTMASK_PORTSET(lacp_enable_pm, port))
        {
            
            lacp_portState_set(p->pv.actor_port_num - 1, STP_STATE_BLOCKING);
        }
        else
        {
            
            lacp_portState_set(p->pv.actor_port_num - 1, STP_STATE_FORWARDING);
        }

        ag->av.lag_ports = pm;
        ag->n_ports --;

        SYS_DBG(DBG_LAG, "!!![%d:P%d][pm 0x%x lacp_aggs[%d].n_port=%d]", (uint16)__LINE__, port, ag->av.lag_ports.bits[0], agg, ag->n_ports);
                if (IS_LOGIC_PORTMASK_CLEAR(pm))
        { 
            osal_memset(ag->av.partner_sys, 0, CAPA_MAC_BYTE_NUM);
            ag->av.partner_sys_pri = 0;
            ag->av.avs &= ~AVS_USING;
            SYS_DBG(DBG_LAG, "!!![%d:P%d][lacp_aggs[%d].av.avs=0x%x]", (uint16)__LINE__, port, agg, ag->av.avs);
        }
    }
    p->pv.actor_port_agg_id = 0;

    return SYS_ERR_OK;
}
int32 lacp_marker_responser(sys_marker_pdu_t *pMarkerPDU, sys_logic_port_t srcPort)
{
    sys_nic_pkt_t *pPktNew;
    sys_eth_hdr_t *pEth_hdr;
    sys_logic_portmask_t DstPm;
    sys_mac_t portMac;

    if ((MARKER_INFORMATION != pMarkerPDU->first_tlv_type) ||
         (MARKER_TERMINATOR != pMarkerPDU->second_tlv_type)                         )
    {
        return SYS_ERR_OK;
    }

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(1500, &pPktNew))
    {
        
        pEth_hdr = (sys_eth_hdr_t *)pPktNew->data;

        SYS_MEM_CLEAR(portMac);
        rsd_sys_portMacAddr_get(srcPort, &portMac);
        osal_memcpy(pEth_hdr->src_mac, &portMac, CAPA_MAC_BYTE_NUM);

        osal_memcpy(pEth_hdr->dst_mac, slowProtocolMAC, CAPA_MAC_BYTE_NUM);
        pEth_hdr->ether_type = htons(LACP_ETH_TYPE);

        pPktNew->tail += sizeof(sys_eth_hdr_t);

        pMarkerPDU->version_number = 0x01;
        pMarkerPDU->first_tlv_type = MARKER_RESPONSE_INFORMATION;
        pMarkerPDU->marker_info_len = 0x10;
        pMarkerPDU->second_tlv_type = 0x00;
        pMarkerPDU->terminator_len = 0x00;
        pMarkerPDU->pad1[0] = 0;
        pMarkerPDU->pad1[1] = 0;
        osal_memset( pMarkerPDU->pad2, 0, 90);
        osal_memcpy((sys_marker_pdu_t *)(pPktNew->data + sizeof(sys_eth_hdr_t)), pMarkerPDU, sizeof(sys_marker_pdu_t));

        pPktNew->tail += sizeof(*pMarkerPDU);
        pPktNew->length = sizeof(sys_eth_hdr_t) + sizeof(*pMarkerPDU);
        osal_memset(&DstPm, 0, sizeof(DstPm));
        LOGIC_PORTMASK_SET_PORT(DstPm, srcPort);

        rsd_nic_pkt_tx(pPktNew, &DstPm, SYS_CPU_PORT_PRIORITY_LACP, 0 , RSD_NIC_TX_CARE_DOT1X);
    }

    return SYS_ERR_OK;
}
int32 lacp_portState_set(sys_logic_port_t port, sys_stp_state_t state)
{
    sys_port_stateCtrl_t stateCtrl;
    uint32 i;

    osal_memset(&stateCtrl, 0, sizeof(sys_port_stateCtrl_t));
    stateCtrl.proto = SYS_PORT_STATECTRLPROTO_LACP;
    stateCtrl.state = state;
    stateCtrl.port = port;

    for (i = 0; i < CAPA_MST_INSTANCE_NUM; i++)
    {
        stateCtrl.msti = i;
        SYS_ERR_CHK(rsd_port_state_set(stateCtrl));
    }

    return SYS_ERR_OK;
}

