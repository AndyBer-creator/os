
#ifndef __PROTO_GVRP_H__
#define __PROTO_GVRP_H__
#include <common/sys_error.h>
#include <common/sys_type.h>
#include <common/sys_portview.h>

#define GVRP_LOG(mid, args...)               \
do {                                        \
        SYS_LOG(LOG_CAT_VLAN, mid, ##args); \
} while(0)
#define GVRP_PORT_TRUNK_NUM     (8)

#define GVRP_DB_SIZE 128
#define GVRP_JOIN_CNT  2
#define GVRP_MAX_TX_TIME 20
#define GVRP_MAX_VLANATTR_ONE_PKT 205
typedef enum register_mode_e{
    GVRP_NORMAL,
    GVRP_FIXED,
    GVRP_FORBIDDEN,
    GVRP_MODE_END
}register_mode_t;
typedef enum vlan_form_s
{
    VLAN_STATIC,
    VLAN_DYNAMIC,
    VLAN_END,
}vlan_form_t;

typedef enum gvrp_timer_type_s
{
    GVRP_TIMER_HOLD,
    GVRP_TIMER_JOIN,
    GVRP_TIMER_LEAVE,
    GVRP_TIMER_LEAVEALL,
    GVRP_TIMER_END
}gvrp_timer_type_t;

typedef struct
{
    uint16   vid;
    uint16     isdynVlan;
    sys_logic_portmask_t  portBmp;
    sys_logic_portmask_t  dynBmp;
} Gvd;
typedef struct Gid_machine_s
{

    uint32 applicant : 5; 
    uint32 registrar : 5; 
} Gid_machine;

typedef struct Garp_s
{
 uint32 process_id;
 void *gid; 

 uint32 gip[GVRP_DB_SIZE+1];  
 uint32 max_gid_index;
 uint32 last_gid_used;
 void(*join_indication_fn)( void *, void *my_port, uint32 joining_gid_index);
 void(*leave_indication_fn)(void *, void *gid,uint32 leaving_gid_index);
 void(*join_propagated_fn)( void *, void *gid,uint32 joining_gid_index);
 void(*leave_propagated_fn)(void *, void *gid,uint32 leaving_gid_index);
 void(*transmit_fn)( void *, void *gid);
 uint32 (*receive_fn)( void *, void *gid, void *pdu);
 void (*added_port_fn)( void *, uint32 port_no);
 void (*removed_port_fn)( void *, uint32 port_no);
} Garp;
typedef struct sys_gvrp_info_s
{
    sys_enable_t         enable;
    uint32      holdtime;
    uint32      jointime;
    uint32      leavetime;
    uint32      leavealltime;
}sys_gvrp_info_t;
typedef struct gvrp_statistics_s{
    uint32 Total_rx;
    uint32 JoinEmpty_rx;
    uint32 JoinIn_rx;
    uint32 Empty_rx;
    uint32 LeaveIn_rx;
    uint32 LeaveEmpty_rx;
    uint32 LeaveAll_rx;
    uint32 Total_tx;
    uint32 JoinEmpty_tx;
    uint32 JoinIn_tx;
    uint32 Empty_tx;
    uint32 LeaveIn_tx;
    uint32 LeaveEmpty_tx;
    uint32 LeaveAll_tx;
}gvrp_statistics_t;

typedef struct gvrp_error_statistics_s{
    uint32 invProt;  
    uint32 invAtyp; 
    uint32 invAlen;  
    uint32 invAval;  
    uint32 invEvent;  
}gvrp_error_statistics_t;
extern int32 gvrp_enable_set(sys_enable_t enable);
extern int32 gvrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 gvrp_timer_set(gvrp_timer_type_t time_type, uint32 timeout);
extern int32 gvrp_info_get(sys_gvrp_info_t *pInfo);
extern int32 gvrp_port_regmode_set(sys_logic_port_t port, register_mode_t  regmode);
extern int32 gvrp_port_forbidVlan_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 gvrp_statistics_clear(sys_logic_port_t  port);
extern int32  gvrp_error_statistics_clear(sys_logic_port_t port);
extern int32 gvrp_port_statistics_get(sys_logic_port_t port,  gvrp_statistics_t  *pStat);
extern int32 gvrp_port_error_statistics_get(sys_logic_port_t port,  gvrp_error_statistics_t  *pStat);
extern int32 gvrp_vlanMaskExist_get(sys_vlanmask_t *pVlanMask);
extern int32 gvrp_gidtt_machine_get(sys_logic_port_t port, sys_vid_t vid, Gid_machine *pMachine);
extern int32 gvrp_add_staticPort(sys_vid_t vid, sys_logic_port_t lPort);
extern int32 gvrp_del_staticPort(sys_vid_t vid, sys_logic_port_t lPort);
extern int32 gvrp_del_static_vlan(sys_vid_t vid);
extern int32 gvrp_vlan_db_get(sys_vid_t vid, Gvd  *show_db);
extern int32 gvrp_vlan_db_size_get(uint32 *pSize);
extern int32 gvrp_vlan_db_set(sys_vid_t vid, Gvd  *vlandb);
extern int32 gvrp_stp_connect_port(sys_logic_port_t  port_no);
extern int32 gvrp_stp_disconnect_port(sys_logic_port_t port_no);

extern int32 gvrp_init(void);
extern int32 gvrp_exit(void);
#endif

