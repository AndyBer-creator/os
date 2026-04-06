#ifndef _GARP_GIP_H__
#define _GARP_GIP_H__
#include "proto_gvrp_gid.h"
#include "gvrp_common.h"
extern void gip_connect_port(Garp *application, uint32 port_no);
extern void gip_disconnect_port(Garp *application, uint32 port_no);
extern void gip_propagate_join(Gid *my_port, uint32 index);
extern void gip_propagate_leave(Gid *my_port, uint32 index);
extern BOOL gip_propagates_to(Gid *my_port, uint32 index);
extern void gip_do_actions(Gid *my_port);
extern void gip_ring_info(Garp *application,uint8 gip_ring[]);
#endif 

