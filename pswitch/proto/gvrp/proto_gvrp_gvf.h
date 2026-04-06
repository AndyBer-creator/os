#ifndef _GVRP_GVF_H_
#define _GVRP_GVF_H_

#include "proto_gvrp_gid.h"
#include "proto_gvrp_gvr.h"
typedef struct Gvf_s
{  

    Gvr_pdu     *pdu;

    uint16  uiPduPtr;       

    uint8   ucAttrType;     
    BOOL bBeginOfMsg;    

} Gvf;

typedef struct Gvf_msg_s
{
    Attribute_type attribute;
    Gid_event event;
    Vlan_id key1;

} Gvf_msg;
typedef struct
{
  uint16   attr;
  uint8    used;
} gvrp_db;

typedef struct
{
  uint16   vid;
  uint8    name[8];
  uint32   member_port;
  uint8    status:1;
} vlan_data;

typedef struct _set_pvid {
  uint8    port_id;
  uint16   pvid;
} set_pvid;

typedef struct {
  uint16   usVlanId;
  sys_logic_portmask_t  stPortMask;
} TstGvrpVlan;
#ifdef __cplusplus
extern "C" {
#endif

extern void     gvf_rdmsg_init(Gvf *gvf, Gvr_pdu *pdu);
extern BOOL  gvf_rdmsg(Gvr *my_gvr, Gid *my_port, Gvf *gvf, Gvf_msg *msg);
extern void     gvr_rcv_msg(Gvr *my_gvr, Gid *my_port, Gvf_msg *msg);

extern BOOL gvrp_wrmsg_init(sys_nic_pkt_t * pkt, uint16 *offset);
extern BOOL gvrp_wrmsg(sys_nic_pkt_t *pkt, Gvf_msg *msg, uint16 startOffset, uint16 *endOffset);
extern BOOL gvrp_wrmsgEnd(sys_nic_pkt_t *pkt,  uint16 startOffset, uint16 *endOffset);

#ifdef __cplusplus
}
#endif
void gvf_skipmsg(Gvf *gvf);

void gvf_skip_event(Gvf *gvf);

#endif 
