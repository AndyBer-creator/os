 
#ifndef _VFDB_H_ 
#define _VFDB_H_
extern BOOL vfdb_filter( uint32 port_no, uint16 vlan_id);
extern BOOL vfdb_forward(uint32 port_no, uint16 vlan_id);
extern int32 gvrp_trunk_process(sys_trunk_event_t *pTrunkEvent, uint32 event, uint8 isAdd, uint8 isDel);
#endif 
