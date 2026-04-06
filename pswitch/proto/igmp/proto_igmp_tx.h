
#ifndef __PROTO_IGMP_TX_H__
#define __PROTO_IGMP_TX_H__

int32 mcast_snooping_tx(sys_nic_pkt_t * pkt, uint16 vid, uint32 length, sys_logic_portmask_t * portmask);

#ifdef CONFIG_SYS_PROTO_MVR
int32 mcast_mvr_tx(sys_nic_pkt_t * pkt, uint32 vid,  uint32 length, sys_logic_portmask_t * portmask);
#endif
 #endif

