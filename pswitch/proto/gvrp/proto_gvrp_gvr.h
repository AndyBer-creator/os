#ifndef _GARP_GVR_H__
#define _GARP_GVR_H__

#include "proto_gvrp.h"
typedef uint16 Vlan_id;
typedef struct 
{
  Garp     *garp;
  Vlan_id     gvd[GVRP_DB_SIZE+1]; 
  uint32   vlan_id;
  uint32   number_of_gvd_entries;
  uint32   last_gvd_used_plus1;
} Gvr;
extern BOOL gvr_create_gvr(uint32 process_id, Gvr **gvr);
extern void  gvr_destroy_gvr(void *gvr);
extern void  gvr_added_port(void *my_gvr, uint32 port_no);
extern void  gvr_removed_port(void *my_gvr, uint32 port_no);
extern void gvr_join_indication(void *my_gvr, void *my_port,uint32 joining_gid_index);
extern void gvr_join_propagated(void *my_gvr, void *my_port,uint32 gid_index);
extern void gvr_leave_propagated(void *gvr, void *port,  sys_vlanmask_t *pVlanmask);

extern void gvr_join_leave_propagated(void *my_gvr, void *my_port,uint32 gid_index);
extern void gvr_leave_indication(void *my_gvr, void *my_port,uint32 leaving_gid_index);
extern uint32 gvr_rcv(void *my_gvr, void *port, void *rx_pdu);
extern void gvr_tx(void *my_gvr, void *my_port);
extern void gvr_leaveall_propagated(Gid *port);

extern void gvr_db_full(Gvr *gvr, Gid *my_port);

#endif 
