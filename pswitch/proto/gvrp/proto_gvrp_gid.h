#ifndef _GARP_GID_H_
#define _GARP_GID_H_

#include "proto_gvrp_sys.h"
typedef enum {Very_anxious, Anxious, Quiet, Leaving} Gid_applicant_state;
typedef enum {Normal, No_protocol} Gid_applicant_mgt;
typedef enum {In, Leave, Empty} Gid_registrar_state ;
typedef enum {Normal_registration, Registration_fixed, Registration_forbidden
} Gid_registrar_mgt;

typedef struct 
{
uint32 applicant_state : 2;
uint32 applicant_mgt : 1;
uint32 registrar_state : 2;
uint32 registrar_mgt : 2;
} Gid_states;
typedef enum 
{
  Gid_null, Gid_rcv_leaveempty, Gid_rcv_leavein, Gid_rcv_leaveall,
 Gid_rcv_leaveall_range, Gid_rcv_empty, Gid_rcv_joinempty, Gid_rcv_joinin,

  Gid_join, Gid_leave,

  Gid_normal_operation, Gid_no_protocol,

  Gid_normal_registration, Gid_fix_registration, Gid_forbid_registration,

  Gid_tx_leaveempty, Gid_tx_leavein, Gid_tx_empty, Gid_tx_joinempty,
 Gid_tx_joinin, Gid_tx_leaveall, Gid_tx_leaveall_range

} Gid_event;

enum
{
 Number_of_gid_rcv_events = (Gid_rcv_joinin + 1),
 Number_of_gid_req_events = 2,
 Number_of_gid_amgt_events = 2,
 Number_of_gid_rmgt_events = 3,
 Number_of_gid_tx_events = 7
};
typedef struct 
{

Garp *application;
uint32 port_no;
void *next_in_port_ring;
void *next_in_connected_ring;
uint32 is_enabled : 1;
uint32 is_connected : 1;
uint32 is_point_to_pouint32 : 1;
uint32 cschedule_tx_now : 1;
uint32 cstart_join_timer : 1;
uint32 cstart_leave_timer : 1;
uint32 tx_now_scheduled : 1;
uint32 join_timer_running : 1;
uint32 leave_timer_running : 1;
uint32 leaveall_timer_running: 1;
uint32 hold_timer_running : 1;
uint32 rcv_leaveall_event : 1;
uint32 hold_tx : 1;
uint32 tx_pending : 1;
uint32 join_timeout;
uint32 hold_timeout;
uint32 join_cnt;
uint32 leaveall_countdown;
uint32 leaveall_timeout_n;
Gid_machine *machines;
uint32 last_transmitted;
uint32 last_to_transmit;
uint32 untransmit_machine;
sys_vlanmask_t leave_vlanmask;
struct timer_list sLeaveAll_timer;
struct timer_list sLeave_timer;
struct timer_list sJoin_timer;
struct timer_list sHold_timer;
} Gid;
extern BOOL gid_create_port(Garp *application, uint32 port_no);
extern void gid_destroy_port(Garp *application, uint32 port_no);
extern BOOL gid_find_port(Gid *first_port, uint32 port_no, Gid **gid);
extern Gid *gid_next_port(Gid *this_port);

extern BOOL gid_find_unused(Garp *application, uint32 from_index,uint32 *found_index);
extern void gid_rcv_pdu(void *gvr, uint32 port_no, void *pdu);
extern void gid_read_attribute_state(Gid *my_port, uint32 index,Gid_states *state);
extern void gid_manage_attribute(Gid *my_port, uint32 index,Gid_event directive);
extern void gid_rcv_msg(Gid *my_port, uint32 gid_index, Gid_event msg);
extern void gid_join_request(Gid *my_port, uint32 gid_index);
extern void gid_leave_request(Gid *my_port, uint32 gid_index);
extern void gid_rcv_leaveall(Gid *my_port);
extern void gid_rcv_leavein(Gid *my_port, uint16 vid);
extern void gid_rcv_leaveempty(Gid *my_port, uint16 vid);
extern void gid_leaveall(Gid *my_port);
void gid_do_actions(Gid *my_port);
extern void gid_leave_timer_expired(unsigned long data);
extern void gid_join_timer_expired(unsigned long  data);
extern void gid_leaveall_timer_expired(unsigned long data);
extern void gid_hold_timer_expired(unsigned long data);
extern Gid_event gid_next_tx(Gid *my_port, uint32 *gid_index);

extern void gid_untx(Gid *my_port);
extern BOOL gid_registered_here(Gid *my_port, uint32 gid_index);
extern BOOL gid_port_enable(Garp *application, sys_logic_port_t port, sys_enable_t enable);

extern void gid_stop_all_timer(Gid *my_gid);
extern void gid_machine_set(Gid *my_gid, uint32 gid_index, enum Applicant_states app, enum Registrar_states reg);
extern void gid_machine_get(Gid *my_gid, uint32 gid_index, enum Applicant_states *pApp, enum Registrar_states *pReg);
#endif 

