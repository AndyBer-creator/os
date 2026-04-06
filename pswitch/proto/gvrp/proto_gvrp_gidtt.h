#ifndef _GARP_GIDTT_H__
#define _GARP_GIDTT_H__
enum {Number_of_applicant_states = Qon + 1}; 
enum {Number_of_registrar_states = Mtf + 1}; 

enum Timers
{
 Nt = 0,  
 Jt = 1,  
 Lt = 1   
};

enum Applicant_msg
{
 Nm = 0, 
 Jm, 
 Lm, 
 Em  
};

enum Registrar_indications
{
 Ni = 0, 
 Li = 1, 
 Ji = 2  
};
typedef struct 
{
 uint32 new_app_state : 5; 
 uint32 cstart_join_timer : 1;  
} Applicant_tt_entry;

typedef struct 
{
 uint32 new_reg_state : 5;
 uint32 indications : 2;
 uint32 cstart_leave_timer : 1;
} Registrar_tt_entry;

typedef struct 
{
 uint32 new_app_state : 5;
 uint32 msg_to_transmit : 2; 
 uint32 cstart_join_timer : 1;
} Applicant_txtt_entry;

typedef struct 
{
 uint32 new_reg_state : 5; 
 uint32 leave_indication : 1;
 uint32 cstart_leave_timer : 1;
} Registrar_leave_timer_entry;
extern BOOL registrar_in_table[];
extern Gid_registrar_mgt registrar_mgt_table[];
extern Gid_registrar_state registrar_state_table[];
extern Gid_applicant_mgt applicant_mgt_table[];
extern Gid_applicant_state applicant_state_table[];
extern Registrar_leave_timer_entry registrar_leave_timer_table[];
extern Applicant_txtt_entry applicant_txtt[];

extern Applicant_tt_entry applicant_tt[Number_of_gid_rcv_events  +
                                       Number_of_gid_req_events  +
                                       Number_of_gid_amgt_events +
                       Number_of_gid_rmgt_events]
                       [Number_of_applicant_states];

extern Registrar_tt_entry registrar_tt[Number_of_gid_rcv_events  +
                                       Number_of_gid_req_events  +
                                       Number_of_gid_amgt_events +
                       Number_of_gid_rmgt_events]
                       [Number_of_registrar_states];

extern Gid_event gidtt_event(Gid *my_port,Gid_machine *machine,Gid_event event);
extern Gid_event gidtt_tx(Gid *my_port,Gid_machine *machine, uint32 gid_index);
extern Gid_event gidtt_leave_timer_expiry(Gid *my_port,Gid_machine *machine);
extern BOOL gidtt_in(Gid_machine *machine);
extern BOOL gidtt_machine_active(Gid_machine *machine);
extern void gidtt_states(Gid_machine *machine,Gid_states *state);

#endif 

