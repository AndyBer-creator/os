#include "gvrp_common.h"
extern gvrp_cfg_t  gvrp_cfg;
extern Gvd gGvd[GVRP_DB_SIZE+1];
Applicant_tt_entry
applicant_tt[Number_of_gid_rcv_events + Number_of_gid_req_events +
Number_of_gid_amgt_events + Number_of_gid_rmgt_events][Number_of_applicant_states]=
{

    { 
        {Va, Nt},{Aa, Nt},{Qa, Nt},{La, Nt},
            {Vp, Nt},{Ap, Nt},{Vp, Nt},
            {Vo, Nt},{Ao, Nt},{Qo, Nt},{Lo, Nt},
            {Von,Nt},{Aon,Nt},{Qon,Nt}
    },

    { 
        {Vp, Nt},{Vp, Nt},{Vp, Jt},{Vo, Nt},
            {Vp, Nt},{Vp, Nt},{Vp, Jt},
            {Lo, Nt},{Lo, Jt},{Lo, Jt},{Vo, Nt},
            {Von,Nt},{Von,Nt},{Von,Nt}
    },

    { 
        {Va, Nt},{Va, Nt},{Vp, Jt},{La, Jt},
            
            {Vp, Nt},{Vp, Nt},{Vp, Jt},
            {Lo, Nt},{Lo, Jt},{Lo, Jt},{Vo, Nt},
            {Von,Nt},{Von,Nt},{Von,Nt}
    },

    { 
        {Vp, Nt},{Vp, Nt},{Vp, Jt},{Vo, Nt},
            {Vp, Nt},{Vp, Nt},{Vp, Jt},
            {Lo, Nt},{Lo, Jt},{Lo, Jt},{Vo, Nt},
            {Von,Nt},{Von,Nt},{Von,Nt}
    },

    { 
        {Vp, Nt},{Vp, Nt},{Vp, Jt},{Vo, Nt},
            {Vp, Nt},{Vp, Nt},{Vp, Jt},
            {Lo, Nt},{Lo, Jt},{Lo, Jt},{Vo, Nt},
            {Von,Nt},{Von,Nt},{Von,Nt}
    },

    { 
        {Va, Nt},{Va, Nt},{Va, Jt},{La, Nt},
            {Vp, Nt},{Vp, Nt},{Vp, Jt},
            {Vo, Nt},{Vo, Nt},{Vo, Nt},{Vo, Nt},
            {Von,Nt},{Von,Nt},{Von,Nt}
    },

    { 
        {Va, Nt},{Va, Nt},{Va, Jt},{Vo, Nt},
            {Vp, Nt},{Vp, Nt},{Vp, Jt},
            {Vo, Nt},{Vo, Nt},{Vo, Nt},{Vo, Nt},
            {Von,Nt},{Von,Nt},{Von,Jt}
    },

    { 
        {Aa, Nt},{Qa, Nt},{Qa, Nt},{La, Nt},
            {Ap, Nt},{Qp, Nt},{Qp, Nt},
            {Ao, Nt},{Qo, Nt},{Qo, Nt},{Ao, Nt},
            {Aon,Nt},{Qon,Nt},{Qon,Nt}
    },
    { 

        {Va, Nt},{Aa, Nt},{Qa, Nt},{Va, Nt},
            {Vp, Nt},{Ap, Nt},{Qp, Nt},
            {Vp, Jt},{Ap, Jt},{Qp, Nt},{Vp, Jt},
            {Von,Nt},{Aon,Nt},{Qon,Nt}
    },
    { 
        {La, Nt},{La, Nt},{La, Nt},{La, Nt},
            {Vo, Nt},{Ao, Nt},{Qo, Nt},
            {Vo, Nt},{Ao, Nt},{Qo, Nt},{Lo, Nt},
            {Von,Nt},{Aon,Nt},{Qon,Nt}
    },

    { 
        {Vp, Nt},{Vp, Nt},{Vp, Jt},{La, Nt},
            {Vp, Nt},{Vp, Nt},{Vp, Jt},
            {Va, Nt},{Va, Nt},{Va, Jt},{Lo, Nt},
            {Va, Nt},{Va, Nt},{Va, Jt}
    },

    { 
        {Von,Nt},{Aon,Nt},{Qon,Nt},{Von,Nt},
            {Von,Nt},{Aon,Nt},{Qon,Nt},
            {Von,Nt},{Aon,Nt},{Qon,Nt},{Von,Nt},
            {Von,Nt},{Aon,Nt},{Qon,Nt}
    },

    { 
        {Va, Nt},{Aa, Nt},{Qa, Nt},{La, Nt},
            {Vp, Nt},{Ap, Nt},{Vp, Nt},
            {Vo, Nt},{Ao, Nt},{Qo, Nt},{Lo, Nt},
            {Von,Nt},{Aon,Nt},{Qon,Nt}
    },

    { 
        {Va, Nt},{Aa, Nt},{Qa, Nt},{La, Nt},
            {Vp, Nt},{Ap, Nt},{Vp, Nt},
            {Vo, Nt},{Ao, Nt},{Qo, Nt},{Lo, Nt},
            {Von,Nt},{Aon,Nt},{Qon,Nt}
    },

    { 
        {Va, Nt},{Aa, Nt},{Qa, Nt},{La, Nt},
            {Vp, Nt},{Ap, Nt},{Vp, Nt},
            {Vo, Nt},{Ao, Nt},{Qo, Nt},{Lo, Nt},
            {Von,Nt},{Aon,Nt},{Qon,Nt}
    }

};

Registrar_tt_entry
registrar_tt[Number_of_gid_rcv_events + Number_of_gid_req_events +
Number_of_gid_amgt_events + Number_of_gid_rmgt_events][Number_of_registrar_states] =
{

    { 
        {Inn,Ni,Nt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1, Ni,Nt},
            {Mt, Ni,Nt},
            {Inr,Ni,Nt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Inf,Ni,Nt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Lv, Li,Lt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1,Ni,Nt},
            {Mt, Ni,Nt},
            {Inr,Ni,Lt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Lvf,Ni,Lt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Lv, Li,Lt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1, Ni,Nt},
            {Mt, Ni,Nt},
            {Inr,Ni,Lt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Lvf,Ni,Lt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Lv, Ni,Lt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1, Ni,Nt},
            {Mt, Ni,Nt},
            {Inr,Ni,Nt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Lvf,Ni,Lt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Lv, Ni,Lt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1, Ni,Nt},
            {Mt, Ni,Nt},
            {Lvr,Ni,Lt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Lvf,Ni,Lt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Inn,Ni,Nt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1, Ni,Nt},
            {Mt, Ni,Nt},
            {Inr,Ni,Nt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Inf,Ni,Nt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Inn,Ni,Nt},
            {Inn,Ni,Nt},
            {Inn,Ni,Nt},{Inn,Ni,Nt},{Inn,Ni,Nt},
            {Inn,Ji,Nt},
            {Inr,Ni,Nt},
            {Inr,Ni,Nt},
            {Inr,Ni,Nt},{Inr,Ni,Nt},{Inr,Ni,Nt},
            {Inr,Ni,Nt},
            {Inf,Ni,Nt},
            {Inf,Ni,Nt},
            {Inf,Ni,Nt},{Inf,Ni,Nt},{Inf,Ni,Nt},
            {Inf,Ni,Nt}
    },

    { 
        {Inn,Ni,Nt},
            {Inn,Ni,Nt},
            {Inn,Ni,Nt},{Inn,Ni,Nt},{Inn,Ni,Nt},
            {Inn,Ji,Nt},
            {Inr,Ni,Nt},
            {Inr,Ni,Nt},
            {Inr,Ni,Nt},{Inr,Ni,Nt},{Inr,Ni,Nt},
            {Inr,Ni,Nt},
            {Inf,Ni,Nt},
            {Inf,Ni,Nt},
            {Inf,Ni,Nt},{Inf,Ni,Nt},{Inf,Ni,Nt},
            {Inf,Ni,Nt}
    },

    { 
        {Inn,Ni,Nt},
            {Lv,Ni,Nt},
            {L3,Ni,Nt},{L2,Ni,Nt},{L1,Ni,Nt},
            {Mt,Ni,Nt},
            {Inr,Ni,Nt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Inf,Ni,Nt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Inn,Ni,Nt},
            {Lv,Ni,Nt},
            {L3,Ni,Nt},{L2,Ni,Nt},{L1,Ni,Nt},
            {Mt,Ni,Nt},
            {Inr,Ni,Nt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Inf,Ni,Nt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Inn,Ni,Nt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1, Ni,Nt},
            {Mt, Ni,Nt},
            {Inr,Ni,Nt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Inf,Ni,Nt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Inn,Ni,Nt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1, Ni,Nt},
            {Mt, Ni,Nt},
            {Inr,Ni,Nt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Inf,Ni,Nt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    },

    { 
        {Inn,Ni,Nt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1, Ni,Nt},
            {Mt, Ni,Nt},
            {Inn,Ni,Nt},
            {Lv, Ni,Nt},
            {L3, Ni,Nt},{L2, Ni,Nt},{L1, Ni,Nt},
            {Mt, Li,Nt},
            {Inn,Ji,Nt},
            {Lv, Ji,Nt},
            {L3, Ji,Nt},{L2, Ji,Nt},{L1, Ji,Nt},
            {Mt, Ni,Nt}
    },

    { 
        {Inr,Ni,Nt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ji,Nt},
            {Inr,Ni,Nt},
            {Lvr,Ni,Nt},
            {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},
            {Mtr,Ni,Nt},
            {Inr,Ji,Nt},
            {Lvr,Ji,Nt},
            {L3r,Ji,Nt},{L2r,Ji,Nt},{L1r,Ji,Nt},
            {Mtr,Ji,Nt}
    },

    { 
        {Inf,Li,Nt},
            {Lvf,Li,Nt},
            {L3f,Li,Nt},{L2f,Li,Nt},{L1f,Li,Nt},
            {Mtf,Ni,Nt},
            {Inr,Li,Nt},
            {Lvr,Li,Nt},
            {L3r,Li,Nt},{L2r,Li,Nt},{L1r,Li,Nt},
            {Mtr,Li,Nt},
            {Inf,Ni,Nt},
            {Lvf,Ni,Nt},
            {L3f,Ni,Nt},{L2f,Ni,Nt},{L1f,Ni,Nt},
            {Mtf,Ni,Nt}
    }

};
Applicant_txtt_entry
applicant_txtt[Number_of_applicant_states] =
{
    {Aa, Jm,Jt},{Qa, Jm,Nt},{Qa, Nm,Nt},{Vo, Lm,Nt},
    {Aa, Jm,Jt},{Qa, Jm,Nt},{Qp, Nm,Nt},
    {Vo, Nm,Nt},{Ao, Nm,Nt},{Qo, Nm,Nt},{Vo, Nm,Nt},
    {Von,Nm,Nt},{Aon,Nm,Nt},{Qon,Nm,Nt}
};
Registrar_leave_timer_entry
registrar_leave_timer_table[Number_of_registrar_states] =
{
    {Inn, Ni,Nt},
    {L3, Ni,Lt},{L2, Ni,Lt},{L1, Ni,Lt},{Mt, Li,Nt},
    {Mt, Ni,Nt},
    {Inr,Ni,Nt},
    {L3r,Ni,Nt},{L2r,Ni,Nt},{L1r,Ni,Nt},{Mtr,Ni,Nt},
    {Mtr,Ni,Nt},
    {Inf,Ni,Nt},
    {L3f,Ni,Lt},{L2f,Ni,Lt},{L1f,Ni,Lt},{Mtf,Ni,Lt},
    {Mtf,Ni,Nt}
};
Gid_applicant_state applicant_state_table[Number_of_applicant_states] =
{
    Very_anxious,Anxious,Quiet,Leaving,
    Very_anxious,Anxious,Quiet,
    Very_anxious,Anxious,Quiet,Leaving,
    Very_anxious,Anxious,Quiet
};

Gid_applicant_mgt applicant_mgt_table[Number_of_applicant_states] =
{
    Normal,Normal,Normal,
    Normal,
    Normal,Normal,Normal,
    Normal,Normal,Normal,
    Normal,
    No_protocol,No_protocol,No_protocol
};

Gid_registrar_state registrar_state_table[Number_of_registrar_states] =
{
    In,
    Leave,Leave,Leave,Leave,Empty,
    In,
    Leave,Leave,Leave,Leave,Empty,
    In,
    Leave,Leave,Leave,Leave,Empty
};

Gid_registrar_mgt registrar_mgt_table[Number_of_registrar_states] =
{
    Normal_registration,
    Normal_registration,Normal_registration,
    Normal_registration,Normal_registration,
    Normal_registration,
    Registration_fixed,
    Registration_fixed,Registration_fixed,
    Registration_fixed,Registration_fixed,
    Registration_fixed,
    Registration_forbidden,
    Registration_forbidden,Registration_forbidden,
    Registration_forbidden,Registration_forbidden,
    Registration_forbidden
};

BOOL registrar_in_table[Number_of_registrar_states] =
{
    TRUE,TRUE,TRUE,TRUE,TRUE,
    FALSE,
    TRUE,TRUE,TRUE,TRUE,TRUE,
    TRUE,
    FALSE,FALSE,FALSE,FALSE,FALSE,
    FALSE
};
Gid_event gidtt_event(Gid *my_port, Gid_machine *machine, Gid_event event)
{
    Applicant_tt_entry *atransition;
    Registrar_tt_entry *rtransition;
    Gid_machine old_machine;

#if 0
    if(event == Gid_rcv_joinin || event == Gid_rcv_leaveempty || event == Gid_rcv_leavein )
        SYS_PRINTF(" gidtt_event: port_no=%d event=%d Old: applicant=%d registrar=%d \n",
        my_port->port_no,
        event,
        machine->applicant,
        machine->registrar);

#endif

    old_machine=*machine; 

    atransition = &applicant_tt[event][machine->applicant];
    rtransition = &registrar_tt[event][machine->registrar];

    machine->applicant = atransition->new_app_state;
    machine->registrar = rtransition->new_reg_state;

#if 0
    if(event == Gid_rcv_joinin || event == Gid_rcv_leaveempty ||  event == Gid_rcv_leavein)
        SYS_PRINTF(" gidtt_event: port_no=%d event=%d New: applicant=%d registrar=%d \n",
        my_port->port_no,
        event,
        machine->applicant,
        machine->registrar);
#endif

    if ((event == Gid_join ) && (atransition->cstart_join_timer))
        my_port->cschedule_tx_now = TRUE;
    my_port->cstart_join_timer = my_port->cstart_join_timer
        || atransition->cstart_join_timer;
    my_port->cstart_leave_timer = my_port->cstart_leave_timer
        || rtransition->cstart_leave_timer;
#if 0
    
    if( ((old_machine.registrar==Lv) ||
        (old_machine.registrar==L3) ||
        (old_machine.registrar==L2) ||
        (old_machine.registrar==L1)) &&
        (event==Gid_rcv_joinempty || event==Gid_rcv_joinin) )
    {
        if(TRUE ==my_port->leave_timer_running)
        {
            systime_stop_timer( &my_port->sLeave_timer);
            my_port->leave_timer_running=FALSE;
        }
    }
#endif

    switch (rtransition->indications)
    {
    case Ji:
        return(Gid_join);
        break;
    case Li:
        return(Gid_leave);
        break;
    case Ni:
    default:
        return(Gid_null);
    }
}
BOOL gidtt_in(Gid_machine *machine)
{
    return(registrar_in_table[machine->registrar]);
}
Gid_event gidtt_tx(Gid *my_port,Gid_machine *machine, uint32 gid_index)
{
    uint32 msg = 0;
    uint32 rin = Empty;
    
    if ((msg = applicant_txtt[machine->applicant].msg_to_transmit) != Nm)
    {
        
        if(gGvd[gid_index].vid != 0)
        {
            if(IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].portBmp, my_port->port_no))
            {
                if ( VLAN_STATIC == gGvd[gid_index].isdynVlan )
                {
                    if(IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].dynBmp,my_port->port_no))
                        return Gid_null;
                }

                if (IS_LOGIC_PORTMASK_PORTSET(gGvd[gid_index].dynBmp, my_port->port_no))
                    rin = In;
            }
        }
    }

    my_port->cstart_join_timer = my_port->cstart_join_timer
        || applicant_txtt[machine->applicant].cstart_join_timer;

    switch (msg)
    {
    case Jm: return(rin != Empty ? Gid_tx_joinin : Gid_tx_joinempty);
        break;
    case Lm: return(rin != Empty ? Gid_tx_leavein : Gid_tx_leaveempty);
        break;
    case Em: return(Gid_tx_empty);
        break;
    case Nm:
    default: return(Gid_null);
    }
}
Gid_event gidtt_leave_timer_expiry(Gid *my_port,Gid_machine *machine)
{

    Registrar_leave_timer_entry *rtransition;

    rtransition = &registrar_leave_timer_table[machine->registrar];

    machine->registrar = rtransition->new_reg_state;

    my_port->cstart_leave_timer = my_port->cstart_leave_timer
        || rtransition->cstart_leave_timer;

    return((machine->registrar >= Lv && machine->registrar <= L1) ? Gid_leave : Gid_null);
}
BOOL gidtt_machine_active(Gid_machine *machine)
{
    if ( (machine->applicant == Vo) && (machine->registrar == Mt))
        return(FALSE);
    else
        return(TRUE);
}
void gidtt_states(Gid_machine *machine, Gid_states *state)
{
    state->applicant_state = applicant_state_table[machine->applicant];
    state->applicant_mgt = applicant_mgt_table[machine->applicant]; 
    state->registrar_state = registrar_state_table[machine->registrar];
    state->registrar_mgt = registrar_mgt_table[machine->registrar];
}
