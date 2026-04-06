#include "gvrp_common.h"
#include <osal/time.h>
extern gvrp_statistics_t  gvrp_statistics[SYS_LOGICPORT_NUM_MAX];
extern Gvr      *my_gvr;
extern gvrp_cfg_t gvrp_cfg;
extern Garp    gGarp;
extern Gvd  gGvd[GVRP_DB_SIZE+1];
osal_usecs_t rcvLeaveall;
BOOL gvr_create_gvr(uint32 process_id, Gvr **gvr)
{
    Gvr *pGvr;

    if (!sysmalloc(sizeof(Gvr),(void **)&pGvr))
    {
        return (FALSE);
    }

    osal_memset(&gGarp, 0, sizeof(Garp));

    pGvr->garp = &gGarp;

    pGvr->garp->process_id = process_id;
    pGvr->garp->gid = NULL;

    pGvr->garp->max_gid_index = GVRP_DB_SIZE ;
    pGvr->garp->last_gid_used = 0;

    pGvr->garp->join_indication_fn  = gvr_join_indication;
    pGvr->garp->leave_indication_fn = gvr_leave_indication;
    pGvr->garp->join_propagated_fn  = gvr_join_propagated;
    pGvr->garp->leave_propagated_fn = gvr_join_propagated;
    pGvr->garp->transmit_fn         = gvr_tx;
    pGvr->garp->receive_fn          = gvr_rcv;
    pGvr->garp->added_port_fn       = gvr_added_port;
    pGvr->garp->removed_port_fn     = gvr_removed_port;

    pGvr->number_of_gvd_entries = GVRP_DB_SIZE;
    pGvr->last_gvd_used_plus1   = 0;

    *gvr = pGvr;
    return(TRUE);

}
void gvr_destroy_gvr(void *gvr)
{
    Gid *my_port;
    Garp *application= my_gvr->garp;

    while((my_port = application->gid) != NULL)
    {
        gid_stop_all_timer(my_port);
        gid_destroy_port( application, my_port->port_no);
        gvrp_statistics_clear(my_port->port_no);
        gvrp_error_statistics_clear(my_port->port_no);
        
    }

    gvd_destroy_gvd();
}
void gvr_added_port(void *gvr, uint32 port_no)
{
#if 0
    Garp *application =  my_gvr->garp;

    sys_vid_t  pvid;
    sys_stg_t  msti;
    sys_stp_state_t  stpStates;
    rsd_vlan_portOperPvid_get(port_no, &pvid);

#ifdef CONFIG_SYS_PROTO_STP
    rsd_vlan_stg_get(pvid, &msti);
    rsd_stp_mstpState_get(msti, port_no, &stpStates);
#else
    rsd_stp_mstpState_get(0, port_no, &stpStates);
#endif

    if (STP_STATE_FORWARDING == stpStates)
    {
        gip_connect_port(application, port_no);
    }

    gip_connect_port(application, port_no);
#endif

}
void gvr_removed_port(void *gvr, uint32 port_no)
{
}
void gvr_join_indication(void *gvr, void *port, uint32 joining_gid_index)
{
    Vlan_id key;
    Gid *my_port=(Gid *)port;

    if (gvd_get_key( my_gvr->gvd, joining_gid_index, &key))
    {
        vfdb_forward(my_port->port_no, key);
    }
}
void gvr_leave_indication(void *gvr, void *port, uint32 leaving_gid_index)
{
    Vlan_id key;
    Gid *my_port=(Gid *)port;

    if (gvd_get_key( my_gvr->gvd, leaving_gid_index, &key))
    {
        vfdb_filter(my_port->port_no, key);
    }
}
void gvr_db_full(Gvr *gvr, Gid *my_port)
{

    SYS_DBG(DBG_VLAN_GVRP,"GVRP: GVRP Database Full...");
    GVRP_LOG(LOG_VLAN_GVRP_DB_FULL);
}
void gvr_rcv_msg(Gvr *gvr, Gid *my_port, Gvf_msg *msg)
{
    uint32 gid_index  = GVRP_DB_SIZE + 1;
    Garp    *application = my_gvr->garp;
    sys_logic_portmask_t   stportmsk;
    sys_vlan_membership_t vlanMbrship = SYS_VLAN_EXCLUDED;

    if (msg->event == Gid_rcv_leaveall)
    {
         gid_rcv_leaveall(my_port);
         my_port->join_cnt = GVRP_JOIN_CNT;
         my_port->rcv_leaveall_event = TRUE;
         gid_do_actions(my_port);
    }
    else{
         
        if (GVRP_NORMAL != GVRP_PORT_REGMODE_GET(my_port->port_no))
        {
            SYS_DBG(DBG_VLAN_GVRP,"Port-%s is not normal mode", LPSTR(my_port->port_no));
            return;
        }

        rsd_vlan_portOperVlanMembership_get(my_port->port_no, msg->key1, &vlanMbrship);
        if (SYS_VLAN_FORBIDDEN == vlanMbrship)
        {
            SYS_DBG(DBG_VLAN_GVRP,"Port-%s in vlan %d is forbiden", LPSTR(my_port->port_no), msg->key1);
            return ;
        }

        if (FALSE == rsd_nic_isPortSTPForwad_ret(my_port->port_no, msg->key1))
        {
            SYS_DBG(DBG_VLAN_GVRP,"Port-%s in vlan %d is not forward!", LPSTR(my_port->port_no), msg->key1);
            
            return;
        }

        if (!gvd_find_entry(my_gvr->gvd, msg->key1, &gid_index))
        {
            if ((msg->event == Gid_rcv_joinin)|| (msg->event == Gid_rcv_joinempty))
            {
                if (DISABLED == GVRP_PORT_FORBIDVLAN_GET(my_port->port_no))
                {
                    if (!gvd_create_entry(my_gvr->gvd, msg->key1, VLAN_DYNAMIC, &gid_index))
                    {
                        if (gid_find_unused( application, 0 , &gid_index))
                        {
                            gvd_delete_entry(my_gvr->gvd, gid_index);
                            gvd_create_entry(my_gvr->gvd, msg->key1, VLAN_DYNAMIC, &gid_index);
                            gid_machine_set(my_port,gid_index,Vo,Mt);
                        }
                        else
                        {
                            gvr_db_full(my_gvr, my_port);
                        }
                    }
                    else
                    {
                        SYS_DBG(DBG_VLAN_GVRP,"gvd create vlan %d entry",msg->key1);
                        gid_machine_set(my_port,gid_index,Vo,Mt);
                    }
                }
            }
        }
        else 
        {
            SYS_DBG(DBG_VLAN_GVRP,"The vlan %d entry is exist", msg->key1);
            LOGIC_PORTMASK_ANDNOT(stportmsk, gGvd[gid_index].portBmp, gGvd[gid_index].dynBmp);
            if(IS_LOGIC_PORTMASK_PORTSET(stportmsk, my_port->port_no))
            {
                SYS_DBG(DBG_VLAN_GVRP,"VLAN ID %d on Port-%s  is static type!", msg->key1, LPSTR(my_port->port_no));
                return;
            }
        }

        if (gid_index != GVRP_DB_SIZE+1)
        {
            gid_rcv_msg(my_port, gid_index, msg->event);
        }
    }

}
uint32 gvr_rcv(void *gvr, void *port, void *rx_pdu)
{

    Gvf gvf;
    Gvf_msg msg;

    Gid *my_port= (Gid *)port;
    Gvr_pdu *my_pdu = (Gvr_pdu *)rx_pdu;
    gvf_rdmsg_init(&gvf, my_pdu);
    while (gvf_rdmsg(my_gvr, my_port, &gvf, &msg))
        ;

    return(TRUE);
}
BOOL gvr_tx_msg(Gvr *my_gvr, uint32 gid_index, Gvf_msg *msg, uint32 port_no)
{
    Garp *application= my_gvr->garp;
    register_mode_t regmode;
    sys_vlanmask_t vlanExist;
    sys_vid_t dftVid;
    msg->attribute = Vlan_attribute; 
    if (msg->event != Gid_tx_leaveall)
    {
        if (FALSE == rsd_nic_isPortSTPForwad_ret(port_no, gGvd[gid_index].vid))
        {
            
            return FALSE;
        }

        if (0 != gid_index)
        {
            msg->key1 = gGvd[gid_index].vid;
            if ( (msg->key1==0))
            {

                if (0 == application->gip[gid_index])
                {
                    SYS_DBG(DBG_VLAN_GVRP,"The VLAN %d on Port-%s  gip is zero!", msg->key1, LPSTR(port_no));
                }

                if (1 != gid_index) 
                    return FALSE;
            }
            else
            {
                regmode = GVRP_PORT_REGMODE_GET(port_no);
                if (GVRP_FIXED == regmode)
                {
                    if(gvd_is_dynamic_entry(gGvd, msg->key1))
                    {
                        SYS_DBG(DBG_VLAN_GVRP,"Register mode is FIXED, the vlan %d is dynamic", msg->key1);
                        return FALSE;
                    }
                }
                else if (GVRP_FORBIDDEN == regmode)
                {
                    rsd_vlan_defaultVid_get(&dftVid);
                    if ( dftVid != msg->key1) 
                    {
                        SYS_DBG(DBG_VLAN_GVRP,"Register mode is FORBIDDEN, the vlan %d is not default VLAN", msg->key1);
                        return FALSE;
                    }
                }

                VLANMASK_CLEAR_ALL(vlanExist);
                if (SYS_ERR_OK == rsd_vlan_portTrunkAllowedVlan_get(port_no, &vlanExist))
                {
                    if (!VLANMASK_IS_VIDSET(vlanExist, msg->key1))
                    {
                        SYS_DBG(DBG_VLAN_GVRP,"Port %s is not allowed VLAN %d!", LPSTR(port_no), msg->key1);
                        
                        return FALSE;
                    }
                }
                else
                    return FALSE;

            }
        }
        else
        {
            SYS_DBG(DBG_VLAN_GVRP,"gid not found!");
            return FALSE;
        }
    }

    return TRUE;
}
void gvr_join_leave_propagated(void *gvr, void *port, uint32 gid_index)
{
}
void gvr_join_propagated(void *gvr, void *port, uint32 gid_index)
{
}
void gvr_leave_propagated(void *gvr, void *port,  sys_vlanmask_t *pVlanmask)
{
    sys_nic_pkt_t             *pdu;
    gvrp_statistics_t     tmpcnt;
    sys_logic_portmask_t pmsk;

    Gid   *my_port = (Gid *)port;

    uint16  begin, end;
    Gvf_msg msg;
    sys_vid_t vid, tmpVid;
    uint32 vlanAttrIndex;
    uint32 isContinue;
    uint32 txPktIndex;

    tmpVid = BOND_VLAN_MIN;

    for (txPktIndex = 0; txPktIndex < GVRP_MAX_TX_TIME; txPktIndex++)
    {
        begin = 0;
        end = 0;
        vlanAttrIndex = 0;
        isContinue = 0;

        LOGIC_PORTMASK_CLEAR_ALL(pmsk);

        if (SYS_ERR_OK != syspdu_alloc(1500, &pdu))
        {
            SYS_DBG(DBG_VLAN_GVRP,"system alloc mem failure!");
            syspdu_free(pdu);
            return;
        }

        osal_memset(&tmpcnt, 0, sizeof(tmpcnt));

        gvrp_wrmsg_init(pdu, &begin);

        msg.attribute = Vlan_attribute; 

        FOR_EACH_VID_IN_VLANMASK(vid, *pVlanmask)
        {
            if (vid < tmpVid)
                continue;

            msg.key1 = vid;
            msg.event = Gid_tx_leaveempty;
            tmpcnt.LeaveEmpty_tx ++;

            if (!gvrp_wrmsg(pdu, &msg, begin, &end))
            {
                SYS_DBG(DBG_VLAN_GVRP,"gvrp_wrmsg failue");
                syspdu_free(pdu);
                return;
            }

            begin = end;
            vlanAttrIndex++;

            if (GVRP_MAX_VLANATTR_ONE_PKT == vlanAttrIndex)
            {
                tmpVid = vid;
                isContinue = 1;
                break;
            }
        }

        gvrp_wrmsgEnd(pdu, begin, &end);

        LOGIC_PORTMASK_SET_PORT(pmsk, my_port->port_no);
        syspdu_tx(pdu, end,pmsk, &tmpcnt);
        gvrp_statistics[my_port->port_no].Total_tx++;

        syspdu_free(pdu);

        if (0 == isContinue)
            return ;
   }
}
void gvr_tx(void *gvr, void *port)
{
    sys_nic_pkt_t             *pdu;
    uint16      begin, end ;

    Gvf_msg             msg;
    Gid_event       tx_event;
    uint32         gid_index = 0;
    Gid             *my_port= (Gid *)port;

    uint32         pkt_valid=FALSE;
    gvrp_statistics_t  tmp_counter;
    sys_logic_portmask_t  pmsk;
    uint32  txPktIndex;
    uint32  vlanAttrIndex;

    if (!GVRP_ENABLE())
        return ;

    if (!GVRP_PORT_ENABLE(my_port->port_no))
        return;
    for (txPktIndex = 0; txPktIndex < GVRP_MAX_TX_TIME; txPktIndex++)
    {
        begin = 0;
        end = 0;
        pkt_valid = FALSE;
        vlanAttrIndex = 0;

        osal_memset(&tmp_counter,0,sizeof(gvrp_statistics_t));
        LOGIC_PORTMASK_CLEAR_ALL(pmsk);

        if ((tx_event = gid_next_tx(my_port, &gid_index)) != Gid_null)
        {
            if (SYS_ERR_OK == syspdu_alloc(1500, &pdu))
            {
                gvrp_wrmsg_init(pdu, &begin);
                do {
                        msg.event = tx_event;

                        if (!gvr_tx_msg(my_gvr, gid_index, &msg, my_port->port_no))
                        {
                            continue;
                        }

                        if (!gvrp_wrmsg(pdu, &msg, begin, &end))
                        {
                            continue;
                        }

                        vlanAttrIndex++;

                        begin = end;

                        if (msg.event==0)
                            tmp_counter.LeaveAll_tx++;
                        else if (msg.event==1)
                            tmp_counter.JoinEmpty_tx++;
                        else if (msg.event==2)
                            tmp_counter.JoinIn_tx++;
                        else if (msg.event==3)
                            tmp_counter.LeaveEmpty_tx++;
                        else if (msg.event==4)
                            tmp_counter.LeaveIn_tx++;
                        else if (msg.event==5)
                            tmp_counter.Empty_tx++;

                        pkt_valid=TRUE;

                        if ( GVRP_MAX_VLANATTR_ONE_PKT == vlanAttrIndex )
                            break;

                    } while( ((tx_event = gid_next_tx(my_port, &gid_index))!= Gid_null) );

                gvrp_wrmsgEnd(pdu, begin, &end);
                if (pkt_valid==FALSE)
                {
                    syspdu_free(pdu);
                    continue;
                }

                LOGIC_PORTMASK_SET_PORT(pmsk, my_port->port_no);
                tmp_counter.Total_tx++;
                syspdu_tx(pdu, end, pmsk, &tmp_counter);

                syspdu_free(pdu);

            }
        }
        else
        {
            
            SYS_DBG(DBG_VLAN_GVRP,"There is no vlan attribute to found");
            return ;
        }

    }
}
void gvr_leaveall_propagated(Gid *port)
{
    sys_nic_pkt_t             *pdu;
    uint16      begin = 0, end = 0;

    Gvf_msg             msg;
    Gid_event       tx_event;
    uint32         gid_index = 0;
    Gid             *my_port;

    gvrp_statistics_t  tmp_counter;
    sys_logic_portmask_t  pmsk;
    uint32  txPktIndex;
    uint32  vlanAttrIndex;

    if (!GVRP_ENABLE())
        return ;

    if (NULL == port)
        return;

    my_port = port;

    LOGIC_PORTMASK_CLEAR_ALL(pmsk);
    LOGIC_PORTMASK_SET_PORT(pmsk, my_port->port_no);

     if (IS_LP_LINKUP(my_port->port_no))
     {
        my_port->last_to_transmit = 1;
        my_port->tx_pending = TRUE;
     }
     else
        return;

    osal_memset(&tmp_counter,0,sizeof(gvrp_statistics_t));
    if (SYS_ERR_OK == syspdu_alloc(1500, &pdu))
    {
        gvrp_wrmsg_init(pdu, &begin);

        msg.event =Gid_tx_leaveall;
        msg.attribute = Vlan_attribute;

        gvrp_wrmsg(pdu, &msg, begin, &end);
        begin = end;

        tmp_counter.LeaveAll_tx = 1;

        for (txPktIndex = 0; txPktIndex < GVRP_MAX_TX_TIME; txPktIndex++)
        {
            vlanAttrIndex = 0;

            if (0 != txPktIndex)
            {
                begin = 0;
                end = 0;
                osal_memset(&tmp_counter,0,sizeof(gvrp_statistics_t));
                if (SYS_ERR_OK != syspdu_alloc(1500, &pdu))
                {
                    break;
                }

                 gvrp_wrmsg_init(pdu, &begin);
            }

            if ((tx_event = gid_next_tx(my_port, &gid_index)) != Gid_null)
            {
                do {
                        msg.event = tx_event;

                        if (!gvr_tx_msg(my_gvr, gid_index, &msg, my_port->port_no))
                        {
                            continue;
                        }

                        if (!gvrp_wrmsg(pdu, &msg, begin, &end))
                        {
                            continue;
                        }

                        vlanAttrIndex++;

                        begin = end;

                        if (msg.event==0)
                            tmp_counter.LeaveAll_tx++;
                        else if (msg.event==1)
                            tmp_counter.JoinEmpty_tx++;
                        else if (msg.event==2)
                            tmp_counter.JoinIn_tx++;
                        else if (msg.event==3)
                            tmp_counter.LeaveEmpty_tx++;
                        else if (msg.event==4)
                            tmp_counter.LeaveIn_tx++;
                        else if (msg.event==5)
                            tmp_counter.Empty_tx++;

                        if ( GVRP_MAX_VLANATTR_ONE_PKT == vlanAttrIndex )
                            break;

                    } while( ((tx_event = gid_next_tx(my_port, &gid_index))!= Gid_null) );

                    gvrp_wrmsgEnd(pdu, begin, &end);

                    tmp_counter.Total_tx++;
                    syspdu_tx(pdu, end, pmsk, &tmp_counter);
                    syspdu_free(pdu);

            }
            else
            {
                if (0 == txPktIndex )
                {
                    gvrp_wrmsgEnd(pdu, begin, &end);
                    tmp_counter.Total_tx++;
                    syspdu_tx(pdu, end, pmsk, &tmp_counter);
                    syspdu_free(pdu);
                    break;
                }
                else
                {
                    syspdu_free(pdu);
                    break;
                }
            }
        }
    }
}

