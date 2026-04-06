
#include "gvrp_common.h"
extern   Gvr *my_gvr;
extern    Gvd  gGvd[GVRP_DB_SIZE+1];

extern osal_mutex_t            gvrp_sem_db;
extern uint32      Gid_leaveall_count;

extern gvrp_statistics_t  gvrp_statistics[SYS_LOGICPORT_NUM_MAX];   
extern gvrp_cfg_t  gvrp_cfg;
extern Gid    gGid[SYS_LOGICPORT_NUM_MAX];
static BOOL gid_create_gid(Garp *application, uint32 port_no, Gid **gid)
{

    uint32 index=0;

    gGid[port_no].application = application;
    gGid[port_no].port_no       = port_no;
    gGid[port_no].next_in_port_ring     =  &gGid[port_no];
    gGid[port_no].next_in_connected_ring= &gGid[port_no];
    gGid[port_no].is_enabled        = FALSE;
    gGid[port_no].is_connected      = FALSE;
    gGid[port_no].is_point_to_pouint32 = TRUE;
    gGid[port_no].cschedule_tx_now  = FALSE;
    gGid[port_no].cstart_join_timer = FALSE;
    gGid[port_no].cstart_leave_timer    = FALSE;
    gGid[port_no].tx_now_scheduled  = FALSE;
    gGid[port_no].join_timer_running    = FALSE;
    gGid[port_no].leave_timer_running   = FALSE;
    gGid[port_no].hold_tx       = FALSE;
    gGid[port_no].join_timeout      = GVRP_JOIN_TIME() ;
    gGid[port_no].hold_timeout      = GVRP_HOLD_TIME() ;
    gGid[port_no].join_cnt  = GVRP_JOIN_CNT;

    gGid[port_no].sLeaveAll_timer.data=port_no;
    gGid[port_no].sLeave_timer.data=port_no;
    gGid[port_no].sJoin_timer.data=port_no;
    gGid[port_no].sHold_timer.data=port_no;

    if (!sysmalloc(sizeof(Gid_machine)*(application->max_gid_index + 2),(void **)&gGid[port_no].machines))
    {
        return FALSE;
    }

    gGid[port_no].leaveall_countdown = Gid_leaveall_count;
    gGid[port_no].leaveall_timeout_n = GVRP_LEAVE_ALL_TIME()/Gid_leaveall_count;
    gGid[port_no].rcv_leaveall_event = FALSE;

    for(index=0;index <= application->max_gid_index;index++)
    {
        gGid[port_no].machines[index].applicant= Vo;
        gGid[port_no].machines[index].registrar= Mt;
    }

    gGid[port_no].tx_pending = TRUE;
    gGid[port_no].last_transmitted = application->last_gid_used;
    gGid[port_no].last_to_transmit = application->last_gid_used;
    gGid[port_no].untransmit_machine = application->max_gid_index;
    *gid = &gGid[port_no];
    return(TRUE);
}

BOOL gid_registered_here(Gid *gid, uint32 gid_index)
{
    if(gid->machines[gid_index].registrar != Mt)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}
static void gid_destroy_gid(Gid *gid)
{
    uint32 gid_index;
    
    for (gid_index = 1; gid_index <= gid->application->last_gid_used; gid_index++)
    {
        if (gid_registered_here(gid, gid_index))
        {
            gid->application->leave_indication_fn(gid->application,gid, gid_index);
        }
    }

    sysfree(gid->machines);
}
static Gid *gid_add_port(Gid *existing_ports, Gid *new_port)
{
    Gid *prior;
    Gid *next;
    uint32 new_port_no;

    if (existing_ports != NULL)
    {

        new_port_no = new_port->port_no;
        next = existing_ports;

        for(;;)
        {

            prior = next;
            next = prior->next_in_port_ring;

            if (prior->port_no <= new_port_no)
            {
                if ((next->port_no <= prior->port_no)|| (next->port_no > new_port_no))
                {
                    break;
                }
            }

            else
            { 
                if ((next->port_no <= prior->port_no)&& (next->port_no > new_port_no))
                {
                    break;
                }
            }

        }

        if(prior->port_no == new_port_no)
        {
            syserr_panic();
        }

        prior->next_in_port_ring = new_port;
        new_port->next_in_port_ring = next;
    }

    new_port->is_enabled = TRUE;
    return(new_port);
}

static Gid *gid_remove_port(Gid *my_port)
{
    Gid *prior;
    Gid *next;

    prior = my_port;

    while((next = prior->next_in_port_ring) != my_port)
    {
        prior = next;
    }

    prior->next_in_port_ring = my_port->next_in_port_ring;

    if (prior == my_port)
    {
        return(NULL);
    }
    else
    {
        return(prior);
    }

}
BOOL gid_create_port(Garp *application, uint32 port_no)
{
    Gid *my_port;

    if(application==NULL)
    {
        return (FALSE);
    }

    if (!gid_find_port(application->gid, port_no, &my_port))
    {
        if(gid_create_gid(application, port_no, &my_port))
        {
            application->gid = gid_add_port(application->gid, my_port);
            application->added_port_fn(application, port_no);
            return(TRUE);
        }
    }

    return(FALSE);
}
void gid_destroy_port(Garp *application, uint32 port_no)
{
    Gid *my_port;
    if (gid_find_port(application->gid, port_no, &my_port))
    {
        gip_disconnect_port(application, port_no);
        application->gid = gid_remove_port(my_port);
        gid_destroy_gid(my_port); 
    }

}
BOOL gid_find_port(Gid *first_port, uint32 port_no, Gid **gid)
{
    Gid *next_port = first_port;

    if(first_port==NULL)
    {
        return (FALSE);
    }

    while (next_port->port_no != port_no)
    {
        if ((next_port = next_port->next_in_port_ring) == first_port)
        {
            return(FALSE);
        }
    }

    *gid = next_port;

    return(TRUE);
}
Gid *gid_next_port(Gid *this_port)
{
    return(this_port->next_in_port_ring);
}
void gid_read_attribute_state(Gid *my_port, uint32 index, Gid_states *state)
{
    gidtt_states(&my_port->machines[index], state);
}
void gid_manage_attribute(Gid *my_port, uint32 index, Gid_event directive)
{
    Gid_machine *machine;
    Gid_event event;
    machine = &my_port->machines[index];
    event = gidtt_event(my_port, machine, directive);

    if (event == Gid_join)
    {
        my_port->application->join_indication_fn(my_port->application,my_port, index);
        gip_propagate_join(my_port, index);
    }

    else if (event == Gid_leave)
    {
        my_port->application->leave_indication_fn(my_port->application,my_port, index);  
        gip_propagate_leave(my_port, index);
    }

}

BOOL gid_find_unused(Garp *application, uint32 from_index,uint32 *found_index)
{
    uint32 gid_index = 0;
    Gid *check_port;

    gid_index = from_index;
    check_port = application->gid;
    for (;;)
    { 
        if (gidtt_machine_active(&check_port->machines[gid_index]))
        {
            if(gid_index++ >= application->last_gid_used)
            {
                return(FALSE);
            }

            check_port = application->gid;
        }
        else if ((check_port = check_port->next_in_port_ring)== application->gid)
        {
            *found_index = gid_index;
            return (TRUE);
        }

    }

}
void gid_leaveall(Gid *my_port)
{
    uint32 i;
    Garp *application=my_port->application;

    for (i = 1; i <= application->last_gid_used; i++)
    {
        
        if(IS_LOGIC_PORTMASK_PORTSET(gGvd[i].dynBmp, my_port->port_no))
        {
            (void)gidtt_event(my_port, &my_port->machines[i], Gid_rcv_leaveall);
        }
    }
}

void gid_rcv_leaveall(Gid *my_port)
{
    if(my_port->leaveall_countdown != 0)
    {
        my_port->leaveall_countdown = Gid_leaveall_count;
        gid_leaveall(my_port);
    }
}
void gid_rcv_msg(Gid *my_port, uint32 index, Gid_event msg)
{
    Gid_machine *machine;
    Gid_event event;

    machine = &my_port->machines[index];

    event = gidtt_event(my_port, machine, msg);

    if (event == Gid_join)
    {
        SYS_DBG(DBG_VLAN_GVRP,"gvrp join port %s to vlan %d", LPSTR(my_port->port_no),gGvd[index].vid);
        my_port->application->join_indication_fn(my_port->application,my_port, index); 
        gip_propagate_join(my_port, index);
    }
    else if (event == Gid_leave)
    {
      if(IS_LOGIC_PORTMASK_PORTSET(gGvd[index].dynBmp, my_port->port_no)) 
      {
        SYS_DBG(DBG_VLAN_GVRP,"gvrp leave port %s  from vlan %d", LPSTR(my_port->port_no),gGvd[index].vid);
        my_port->application->leave_indication_fn(my_port->application,my_port, index); 
        gip_propagate_leave(my_port, index);
      }
    }

}
void gid_join_request(Gid *my_port, uint32 gid_index)
{
    (void)(gidtt_event(my_port, &my_port->machines[gid_index], Gid_join));
}
void gid_leave_request(Gid *my_port, uint32 gid_index)
{
    (void)(gidtt_event(my_port, &my_port->machines[gid_index], Gid_leave));
}

BOOL gid_registrar_in(Gid_machine *machine)
{
    return(gidtt_in(machine));
}
void gid_rcv_pdu(void *app, uint32 port_no, void *pdu)
{
    Gid *my_port;
    Garp *application=(Garp *)app;
    uint32 return_value;

    if(gid_find_port(application->gid, port_no, &my_port))
    {
        if(my_port->is_enabled)
        {

            return_value=application->receive_fn(NULL, my_port, pdu);      

            if ( FALSE != return_value )
            {
                gid_do_actions(my_port);
            }
        }
    }
}
Gid_event gid_next_tx(Gid *my_port, uint32 *index)
{
    uint32 check_index;
    uint32 stop_after;
    Gid_event msg;

    if (my_port->hold_tx)
    {
        return(Gid_null);
    }

    if (!my_port->tx_pending)
    {
        return(Gid_null);
    }

    check_index = my_port->last_to_transmit;
    stop_after = my_port->application->last_gid_used;

    for( ; ;check_index++)
    {

        if (check_index > stop_after)
        {
            my_port->tx_pending = FALSE;
            return(Gid_null);
        }

            if ((msg = gidtt_tx(my_port, &my_port->machines[check_index], check_index))!= Gid_null)
            {
                *index = my_port->last_transmitted = check_index;
                my_port->last_to_transmit=check_index+1; 

                my_port->tx_pending = (check_index != stop_after); 
                return(msg);
            }                                   
    }  
}
void gid_untx(Gid *my_port)
{
    my_port->machines[my_port->last_transmitted].applicant =
        my_port->machines[my_port->untransmit_machine].applicant;

    if (my_port->last_transmitted == 0)
    {
        my_port->last_transmitted = my_port->application->last_gid_used;
    }
    else
    {
        my_port->last_transmitted--;
    }

    my_port->tx_pending = TRUE;

}
#if 0
void gid_do_actions(Gid *my_port)
{

  if((!my_port->tx_pending || (my_port->leaveall_countdown == 0))
        &&(!my_port->join_timer_running))
    {
        if( 0 != my_port->leaveall_countdown)
            my_port->join_cnt = 1;

        if(my_port->leaveall_countdown == 0)
        {
            my_port->join_cnt = GVRP_JOIN_CNT;
        }

        systime_stop_timer(&my_port->sJoin_timer);
        systime_start_random_timer(my_port->application->process_id,
            gid_join_timer_expired,my_port->port_no,
            my_port->join_timeout ,
            &my_port->sJoin_timer,
            GVRP_TIMER_JOIN);

        my_port->join_timer_running = TRUE;
    }
    if(my_port->rcv_leaveall_event == TRUE)
    {

        if(my_port->join_timer_running == FALSE)
        {
            systime_stop_timer(&my_port->sJoin_timer);
            systime_start_random_timer(my_port->application->process_id,
                gid_join_timer_expired,my_port->port_no,
                my_port->join_timeout ,
                &my_port->sJoin_timer,
                GVRP_TIMER_JOIN);
        }
        my_port->rcv_leaveall_event = FALSE;
    }
    else
    {
        if (my_port->cstart_leave_timer && (!my_port->leave_timer_running))
        {

            systime_stop_timer(&my_port->sLeave_timer);
            systime_start_timer(my_port->application->process_id,gid_leave_timer_expired,
                my_port->port_no, GVRP_LEAVE_TIME(),&my_port->sLeave_timer);

            my_port->leave_timer_running = TRUE; 
        }
    }
}
#endif
void gid_do_actions(Gid *my_port)
{

    if (my_port->cstart_join_timer)
    {
        my_port->last_to_transmit = my_port->last_transmitted = 1;
        my_port->tx_pending = TRUE;
        my_port->cstart_join_timer = FALSE;
    }

    if (!my_port->hold_tx)
    {
        if (my_port->cschedule_tx_now)
        {

            if (!my_port->tx_now_scheduled)
            {
                if(my_port->join_timer_running)
                {
                    systime_stop_timer(&my_port->sJoin_timer);
                    my_port->join_timer_running=FALSE;

                }

                systime_schedule(my_port->application->process_id,
                    gid_join_timer_expired,
                    my_port->port_no,&my_port->sJoin_timer);

            }

            my_port->cschedule_tx_now = FALSE;
        }

        else if((!my_port->tx_pending || (my_port->leaveall_countdown == 0))
            &&(!my_port->join_timer_running))
        {
            if( 0 != my_port->leaveall_countdown)
                my_port->join_cnt = 1;

            if(my_port->leaveall_countdown == 0)
            {
                my_port->join_cnt = GVRP_JOIN_CNT;
#if 0       
                if(TRUE == my_port->leave_timer_running)
                {
                    systime_stop_timer(&my_port->sLeave_timer);
                    my_port->leave_timer_running = FALSE;
                }
#endif
            }

            systime_stop_timer(&my_port->sJoin_timer);
            systime_start_random_timer(my_port->application->process_id,
                gid_join_timer_expired,my_port->port_no,
                my_port->join_timeout ,
                &my_port->sJoin_timer,
                GVRP_TIMER_JOIN);

            my_port->join_timer_running = TRUE;

        }

    }

    if (my_port->cstart_leave_timer && (!my_port->leave_timer_running))
    {

        systime_stop_timer(&my_port->sLeave_timer);
        systime_start_timer(my_port->application->process_id,gid_leave_timer_expired,
            my_port->port_no, GVRP_LEAVE_TIME(),&my_port->sLeave_timer);

        my_port->leave_timer_running = TRUE; 
    }

    my_port->cstart_leave_timer = FALSE;
    if(my_port->rcv_leaveall_event == TRUE)
    {

        my_port->leaveall_countdown = Gid_leaveall_count;
        systime_stop_timer(&my_port->sLeaveAll_timer);
        systime_start_random_timer(my_port->application->process_id,gid_leaveall_timer_expired,
                my_port->port_no,GVRP_LEAVE_ALL_TIME_N(),&my_port->sLeaveAll_timer, GVRP_TIMER_LEAVEALL);
        if(my_port->join_timer_running == FALSE)
        {
            systime_stop_timer(&my_port->sJoin_timer);
            systime_start_random_timer(my_port->application->process_id,
                gid_join_timer_expired,my_port->port_no,
                GVRP_JOIN_TIME(),
                &my_port->sJoin_timer,
                GVRP_TIMER_JOIN);
        }

    if(my_port->leave_timer_running == FALSE)
       {
           systime_stop_timer(&my_port->sLeave_timer);
           systime_start_timer(my_port->application->process_id,gid_leave_timer_expired,
            my_port->port_no, GVRP_LEAVE_TIME(),&my_port->sLeave_timer);

            my_port->leave_timer_running = TRUE; 
       }

        my_port->rcv_leaveall_event = FALSE;
    }
}

void gid_leave_timer_expired(unsigned long data)
{
    Gid *my_port;
    uint32 gid_index;
    Garp *application= my_gvr->garp;
    uint32 port_no= data;
    if (gid_find_port((Gid *)application->gid, port_no, &my_port))
    {
         if (IS_LP_TRKMBR(my_port->port_no))
            return;

        my_port->leave_timer_running = FALSE; 

        for(gid_index=1; gid_index <= my_port->application->last_gid_used;gid_index++)
        {
            if (!gid_registered_here(my_port, gid_index))
                continue;

           if(gidtt_leave_timer_expiry(my_port,&my_port->machines[gid_index])== Gid_leave)
            {
                gip_propagate_leave(my_port, gid_index);  
                my_port->application->leave_indication_fn(my_port->application, my_port, gid_index); 
                if (gid_registered_here(my_port, gid_index))
                {
                    
                    my_port->machines[gid_index].registrar = Mt;
                }
            }
        }
    } 

}

void gid_leaveall_timer_expired(unsigned long data)
{
    Gid *my_port;
    Garp *application=my_gvr->garp;
    uint32 port_no=data;

    if (gid_find_port((Gid *)application->gid, port_no, &my_port))
    {
        if (IS_LP_TRKMBR(my_port->port_no))
            return;

        if (my_port->leaveall_countdown > 1)
        {
            my_port->leaveall_countdown--;
            systime_start_random_timer(my_port->application->process_id,gid_leaveall_timer_expired,
                my_port->port_no,GVRP_LEAVE_ALL_TIME_N(),&my_port->sLeaveAll_timer, GVRP_TIMER_LEAVEALL);
        }
        else
        {
            gid_leaveall(my_port);

            gvr_leaveall_propagated(my_port);

            my_port->leaveall_countdown = Gid_leaveall_count;
            my_port->last_transmitted = 0; 

            my_port->join_cnt = GVRP_JOIN_CNT - 1;
            if (!my_port->join_timer_running)
            {
                systime_stop_timer(&my_port->sJoin_timer);
                systime_start_timer(my_port->application->process_id,
                gid_join_timer_expired,my_port->port_no,
                GVRP_JOIN_TIME() + GVRP_JOIN_TIME()/10,
                &my_port->sJoin_timer);
                my_port->cstart_join_timer = TRUE;  
                my_port->join_timer_running = TRUE;
            }

            systime_start_random_timer(my_port->application->process_id,
                gid_leaveall_timer_expired,
                my_port->port_no,   GVRP_LEAVE_ALL_TIME_N(),
                &my_port->sLeaveAll_timer, GVRP_TIMER_LEAVEALL);
            
            if(my_port->leave_timer_running == FALSE)
            {
                systime_stop_timer(&my_port->sLeave_timer);
                systime_start_timer(my_port->application->process_id,
                    gid_leave_timer_expired,
                    my_port->port_no,   GVRP_LEAVE_TIME(),
                    &my_port->sLeave_timer);
                my_port->leave_timer_running = TRUE;
            }
        }
    }
}

void gid_join_timer_expired(unsigned long data)
{
    Gid *my_port;
    Garp *application=my_gvr->garp;
    uint32 port_no=data;
    
    if (gid_find_port((Gid *)application->gid, port_no, &my_port))
    {
        if (my_port->is_enabled)
        {
            if (IS_LP_TRKMBR(my_port->port_no))
                return;

            my_port->join_timer_running=FALSE;

            if(my_port->join_cnt > 0)
                my_port->join_cnt--;

            if (IS_LP_LINKUP(my_port->port_no))
            {
                my_port->last_to_transmit = 1; 
                my_port->tx_pending = TRUE;
                 
                application->transmit_fn(application, my_port);    
                
            }

            if(my_port->join_cnt > 0)
            {
                systime_stop_timer(&my_port->sJoin_timer);
                systime_start_timer(my_port->application->process_id,gid_join_timer_expired,
                    my_port->port_no,GVRP_JOIN_TIME(),&my_port->sJoin_timer);
                my_port->join_timer_running = TRUE;
            }

#if 0
            if(!my_port->hold_timer_running)
            {
                systime_start_timer(my_port->application->process_id,gid_hold_timer_expired,
                    my_port->port_no,GVRP_HOLD_TIME(),&my_port->sHold_timer);

                my_port->hold_timer_running=TRUE;
            }
#endif

        }
    }

}

void gid_hold_timer_expired(unsigned long data)
{
    Gid *my_port;

    Garp *application=my_gvr->garp;
    uint32 port_no=data;
    int32 result;

    if (gid_find_port((Gid *)(application->gid), port_no, &my_port))
    {
        my_port->hold_timer_running=FALSE;

        VLANMASK_IS_CLEAR(my_port->leave_vlanmask, result);
        if(!result)
        {
            gvr_leave_propagated(NULL, my_port,  &my_port->leave_vlanmask);
            VLANMASK_CLEAR_ALL(my_port->leave_vlanmask);
        }

    }

}

BOOL gid_port_enable(Garp *application, sys_logic_port_t port, sys_enable_t enable)
{
    Gid        *my_gid;
    uint32 gid_index;

    if ( NULL == application)
        return FALSE;

    GVRP_PORT_ENABLE_SET(port, enable);

    if(enable == ENABLED)
    {
        
        ;
    }
    else if(enable == DISABLED)
    {
        if(gid_find_port((Gid *)application->gid, port, &my_gid))
        {
            gid_stop_all_timer(my_gid);
            
        }

        for (gid_index=1;gid_index <= GVRP_DB_SIZE;gid_index++)
        {
           
            gGid[port].machines[gid_index].registrar= Mt;
        }

        if(application->process_id==GVRP_PROCESS_ID)
        {
            gvrp_statistics_clear(port);
            gvrp_error_statistics_clear(port);
        }
    }
    return TRUE;
}

void gid_stop_all_timer(Gid *my_gid)
{
    systime_stop_timer(&my_gid->sLeaveAll_timer);
    systime_stop_timer(&my_gid->sLeave_timer);
    systime_stop_timer(&my_gid->sJoin_timer);
    systime_stop_timer(&my_gid->sHold_timer);
    my_gid->hold_timer_running = FALSE;
    my_gid->join_timer_running = FALSE;
    my_gid->leave_timer_running = FALSE;
    my_gid->leaveall_timer_running = FALSE;
}

void gid_machine_set(Gid *my_gid, uint32 gid_index, enum Applicant_states app, enum Registrar_states reg)
{
    my_gid->machines[gid_index].applicant= app;
    my_gid->machines[gid_index].registrar= reg;
}

void gid_machine_get(Gid *my_gid, uint32 gid_index, enum Applicant_states *pApp, enum Registrar_states *pReg)
{
    if (NULL == pApp || NULL == pReg)
        return;

    *pApp = my_gid->machines[gid_index].applicant;
    *pReg = my_gid->machines[gid_index].registrar;
}

