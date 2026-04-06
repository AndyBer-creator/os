#ifndef __PROTO_LACP_H__
#define __PROTO_LACP_H__
#include <common/sys_error.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_nic.h>
#define PS_LACP_ACTIVITY    0x1
#define PS_LACP_TIMEOUT     0x2
#define PS_AGGREGATION      0x4
#define PS_SYNCHRONIZATION  0x8
#define PS_COLLECTING      0x10
#define PS_DISTRIBUTING    0x20
#define PS_DEFAULTED       0x40
#define PS_EXPIRED         0x80
typedef enum sys_lacp_timeout_e
{
    LACP_TIMEOUT_LONG = 0,
    LACP_TIMEOUT_SHORT,
    LACP_TIMEOUT_END
} sys_lacp_timeout_t;
typedef struct sys_lacp_pdu_s
{
    uint8  subtype;                    
    uint8  version_number;             
    uint8  first_tlv_type;             
    uint8  actor_info_len;             
    uint16 actor_sys_pri;              
    uint8  actor_sys[CAPA_MAC_BYTE_NUM];  
    uint16 actor_key;                  
    uint16 actor_port_pri;             
    uint16 actor_port;                 
    uint8  actor_state;                
    uint8  pad1[3];
    uint8  second_tlv_type;            
    uint8  partner_info_len;           
    uint16 partner_sys_pri;            
    uint8  partner_sys[CAPA_MAC_BYTE_NUM];
    uint16 partner_key;                
    uint16 partner_port_pri;           
    uint16 partner_port;               
    uint8  partner_state;              
    uint8  pad2[3];
    uint8  third_tlv_type;             
    uint8  collector_info_len;         
    uint16 collector_max_del;          
    uint8  pad3[12];
    uint8  fourth_tlv_type;            
    uint8  terminator_len;             
    uint8  pad4[50];
}sys_lacp_pdu_t;

typedef struct sys_marker_pdu_s
{
    uint8  subtype;                        
    uint8  version_number;                 
    uint8  first_tlv_type;                 
    uint8  marker_info_len;                
    uint16 requester_port;                 
    uint8  requester_sys[CAPA_MAC_BYTE_NUM];  
    uint32 requester_transaction_ID;       
    uint8  pad1[2];                        
    uint8  second_tlv_type;                
    uint8  terminator_len;                 
    uint8  pad2[90];                       
} sys_marker_pdu_t;

typedef enum sys_lacp_subtype_e
{
    LACPAU_SUBTYPE_ILLEAGL = 0,
    LACPAU_SUBTYPE_LACP = 1,
    LACPAU_SUBTYPE_MARKER = 2,
    LACPAU_SUBTYPE_OAM = 3
} sys_lacp_subtype_t;

typedef struct sys_eth_hdr_s
{
    uint8 dst_mac[CAPA_MAC_BYTE_NUM];
    uint8 src_mac[CAPA_MAC_BYTE_NUM];
    uint16 ether_type;
} sys_eth_hdr_t;
typedef int8 sys_lacp_timer_t;
typedef enum sys_rcvm_state_e
{
    RCVM_RESERVED = 0,
    INITIALIZE,
    PORT_DISABLED,
    EXPIRED,
    LACP_DISABLED,
    DEFAULTED,
    CURRENT
} sys_rcvm_state_t;
typedef enum sys_muxm_state_e
{
    MUXM_RESERVED = 0,
    DETACHED,
    WAITING,
    ATTACHED,
    COLLECTING,
    DISTRIBUTING
} sys_muxm_state_t;
typedef enum sys_perio_state_e
{
    PERIO_RESERVED,
    NO_PERIODIC,
    FAST_PERIODIC,
    SLOW_PERIODIC,
    PERIODIC_TX
} sys_perio_state_t;
typedef struct sys_rcv_mach_s
{
    sys_rcvm_state_t state;
    uint8 do_it;
} sys_rcv_mach_t;

typedef struct sys_mux_mach_s
{
    sys_muxm_state_t state;
    uint8 do_it;
} sys_mux_mach_t;
typedef struct sys_perio_mach_s
{
    sys_perio_state_t state;
    uint8 do_it;
} sys_perio_mach_t;
typedef enum sys_mosm_e
{
    MOSM_BEGIN         = (1<<0),
    MOSM_LACP_ENALBE   = (1<<1),
    MOSM_ACTOR_CHURN   = (1<<2),
    MOSM_PARTNER_CHURN = (1<<3),
    MOSM_READY_N       = (1<<4),
    MOSM_READY         = (1<<5),    
    MOSM_SELECTED_NO   = (0<<6),    
    MOSM_SELECTED_YES  = (1<<6),    
    MOSM_SELECTED_STANDBY = (2<<6), 
    MOSM_SELECTED_MASK = (3<<6),    
    MOSM_PORT_MOVED    = (1<<8),
    
    MOSM_PORT_ATTACHED = (1<<9),
    MOSM_PORT_LOOP     = (1<<10)
} sys_mosm_t;

typedef enum sys_agg_var_state_e
{
    AVS_INDIVIDUAL    = (1<<0),
    AVS_RECEIVE       = (1<<1),
    AVS_TRANSMIT      = (1<<2),
    
    AVS_USING         = (1<<3),
    AVS_READY         = (1<<4)
} sys_agg_var_state_t;
typedef struct sys_system_var_s
{
    uint8 actor_sys[CAPA_MAC_BYTE_NUM];
    uint16 actor_sys_pri;
} sys_system_var_t;
typedef struct sys_agg_var_s
{
    uint8  agg_mac[CAPA_MAC_BYTE_NUM];
    uint8  partner_sys[CAPA_MAC_BYTE_NUM];
    uint8  agg_id;
    uint16 actor_admin_agg_key;
    uint16 actor_oper_agg_key;
    uint16 partner_sys_pri;
    uint16 partner_oper_agg_key;
    sys_agg_var_state_t avs;
        sys_logic_portmask_t lag_ports;
} sys_agg_var_t;

typedef struct sys_lacp_agg_s
{
    sys_agg_var_t av;
    uint8 n_ports;           
} sys_lacp_agg_t;
typedef struct sys_port_var_s
{
    uint16 actor_port_num; 
    uint16 actor_port_pri;
    uint16 actor_port_agg_id;
    uint16 actor_admin_port_key;
    uint16 actor_oper_port_key;
    uint8  actor_admin_port_state;
    uint8  actor_oper_port_state;

    uint8  partner_admin_sys[CAPA_MAC_BYTE_NUM];
    uint8  partner_oper_sys[CAPA_MAC_BYTE_NUM];
    uint16 partner_admin_sys_pri;
    uint16 partner_oper_sys_pri;
    uint16 partner_admin_key;
    uint16 partner_oper_key;
    uint16 partner_admin_port_num;
    uint16 partner_oper_port_num;
    uint16 partner_admin_port_pri;
    uint16 partner_oper_port_pri;
    uint8  partner_admin_port_state;
    uint8  partner_oper_port_state;
    uint8  port_enabled;

    sys_mosm_t mosm;
}sys_port_var_t;

typedef struct sys_lacp_port_s
{
    sys_port_var_t pv;

    sys_lacp_timer_t current_while_timer;
    sys_lacp_timer_t actor_churn_timer;
    sys_lacp_timer_t periodic_timer;
    sys_lacp_timer_t partner_churn_timer;
    sys_lacp_timer_t wait_while_timer;
    sys_lacp_timer_t xmit_timer;

    sys_rcv_mach_t rcvm;
    sys_mux_mach_t muxm;
    sys_perio_mach_t prm;
}sys_lacp_port_t;

typedef struct sys_lacp_backupPort_s
{
    sys_logic_port_t port;
    sys_logic_port_t cmpPortId;
    uint32           pri;
} sys_lacp_backupPort_t;

typedef struct sys_lacp_counter_s
{
    uint32 lacpSent;
    uint32 lacpRecv;
    uint32 lacpRecvErr;
} sys_lacp_counter_t;
extern int32 lacp_init(void);
extern int32 lacp_exit(void);
extern int32 lacp_portCounter_get(sys_logic_port_t port, sys_lacp_counter_t *pPortCounter);
extern int32 lacp_portCounter_reset(sys_logic_port_t port);
extern int32 lacp_group_set(
    uint32 trunkId,
    sys_logic_portmask_t *pPortmask,
    sys_logic_portmask_t *pActmask);
extern int32 lacp_group_del(uint32 trunkId);
extern int32 lacp_enable_set(sys_enable_t enable);
extern int32 lacp_enable_get(sys_enable_t *pEnable);
extern int32 lacp_port_state_get(sys_logic_port_t port, sys_lacp_port_t *pPortState);
extern int32 lacp_system_priority_set(uint32 sysPri);
extern int32 lacp_system_priority_get(uint32 *pSysPri);
extern int32 lacp_set_port_enb(sys_logic_port_t port, sys_enable_t enable);
extern int32 lacp_portActivity_set(sys_logic_port_t port, uint32 active);
extern int32 lacp_portPriority_set(sys_logic_port_t port, uint16 pri);
extern int32 lacp_portTimeout_set(sys_logic_port_t port, sys_lacp_timeout_t timeout);
extern int32 lacp_tick(uint8 secs);
extern int32 lacp_run_fsm(sys_logic_port_t port, uint8 secs);
extern int32 lacp_recv_machine(sys_nic_pkt_t *pPkt, sys_logic_port_t port, uint8 sec);
extern int32 lacp_periodic_xmit(sys_logic_port_t port, uint8 sec);
extern int32 lacp_selection_logic(sys_lacp_port_t *p);
extern int32 lacp_mux_machine(sys_lacp_port_t *p, uint8 sec);
extern int32 lacp_prelink_chk(sys_logic_port_t port, uint8 sec);
extern int32 lacp_set_admin_port_vars(sys_lacp_port_t *p, uint16 key);
extern int32 lacp_init_pn(sys_logic_port_t port);
extern int32 lacp_init_aggregator(uint32 trunk);
extern int32 lacp_record_default(sys_lacp_port_t *p);
extern int32 lacp_update_default_selected(sys_lacp_port_t *p);
extern int32 lacp_record_pdu(sys_lacp_pdu_t *buf, sys_lacp_port_t *p);
extern int32 lacp_update_selected(sys_lacp_pdu_t *buf, sys_lacp_port_t *p);
extern int32 lacp_xmit(sys_lacp_port_t *p);
extern int32 lacp_update_ntt(sys_lacp_pdu_t *buf, sys_lacp_port_t *p);
extern int32 lacp_recv_init(sys_lacp_port_t * p);
extern int32 lacp_recv_port_dis(sys_lacp_port_t * p);
extern uint32 lacp_compare_link_ret(sys_lacp_port_t *p1, sys_lacp_port_t *p2);
extern int32 lacp_ena_collect(sys_lacp_port_t *p);
extern int32 lacp_dis_collect(sys_lacp_port_t *p);
extern int32 lacp_ena_distrib(sys_lacp_port_t *p);
extern int32 lacp_dis_distrib(sys_lacp_port_t *p);
extern int32 lacp_attach_mux_to_agg(sys_lacp_port_t *p);
extern int32 lacp_detach_mux_from_agg(sys_lacp_port_t *p);
extern int32 lacp_marker_responser(sys_marker_pdu_t *pMarkerPDU, sys_logic_port_t srcPort);
extern int32 lacp_portState_set(sys_logic_port_t port, sys_stp_state_t state);
extern int32 lacp_backupMember_get(uint32 trunkId, sys_logic_portmask_t *pPortmask);

#endif 

