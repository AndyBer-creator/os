#ifndef __INC_erps_task_h_
#define __INC_erps_task_h_

#ifdef CONFIG_SYS_PROTO_ERPS

#include <common/sys_type.h>
#include <common/sys_portview.h>
#include <osal/thread.h>
#include <osal/time.h>

#include <rsd/rsd_nic.h>
#include <erps/proto_sys_erps.h>

typedef int8    erps_bool_t;
#define ERPS_ETHER_TYPE          0x8902
#define ERPS_FRAME_PRIORITY      0x07

#define ERPS_BUFSIZ              512
#define ERPS_MAX_RING_NUM        240

#define ERPS_INVALID_CTRLVLAN_ID 0
#define ERPS_DEFAULT_CTRLVLAN_ID 1

#define ERPS_DFT_INST_MEL        0
#define ERPS_DFT_RING_ID         1
#define ERPS_RING_PORT_ID_DFT   -1

#define ERPS_DFT_TxRAPS_TIMEOUT  2
#define ERPS_DFT_WTB_TIMEOUT     6   
#define ERPS_DFT_WTR_TIMEOUT     5   
#define ERPS_DFT_GUARD_TIMEOUT   500 
#define ERPS_DFT_HOLDOFF_TIMEOUT 0   

#define ERPS_Disabled                 0
#define ERPS_Listening                1
#define ERPS_Learning                 2
#define ERPS_Forwarding               3
#define ERPS_Blocking                 4
#define SAL_PORT_STATE_VALUE_ZERO     0
#define SAL_PORT_STATE_VALUE_ONE      1
enum erps_mst_stg_state{
    erps_mst_stg_disabled = 1,
    erps_mst_stg_blocking,
    erps_mst_stg_learning,
    erps_mst_stg_listening,
    erps_mst_stg_forwarding,
    erps_mst_stg_brSYS_ERR_OKen
};

typedef enum erps_timer_type_s
{
	ERPS_TIMER_RAPS,
	ERPS_TIMER_WTB,
	ERPS_TIMER_WTR,
	ERPS_TIMER_GUARD,
	ERPS_TIMER_HOLDOFF,
	ERPS_TIMER_END
}erps_timer_type_t;
typedef struct erps_eth_header_s {
    uint8  dst_mac[6];         
    uint8  src_mac[6];         
    uint16 usTpid;             
    uint16 usVlanTag;          
    uint16 etherType;          
}__attribute__((packed)) erps_eth_header_t;

typedef struct erps_common_header_s{
    #if	0
    uint8 version : 5;         
    uint8 mel : 3;             
    #else
    uint8 mel : 3;             
    uint8 version : 5;         
    #endif
    uint8 opCode;              
    uint8 flags;               
    uint8 tlv_Offset;          
}__attribute__((packed)) erps_common_header_t;

typedef struct erps_raps_pdu_s{
    erps_eth_header_t    ethHeader;
    erps_common_header_t cmnHeader;
    #if 0
    uint8              sub_code : 4;        
    uint8              request_state : 4;   
    uint8              status_reserved : 5; 
    uint8              status_BPR : 1;      
    uint8              status_DNF : 1;      
    uint8              status_RB  : 1;      
    #else
    uint8              request_state : 4;   
    uint8              sub_code : 4;        
    uint8              status_RB  : 1;      
    uint8              status_DNF : 1;      
    uint8              status_BPR : 1;      
    uint8              status_reserved : 5; 
    #endif
    uint8              node_id[6];          
    uint8              reserved[24];        
    uint8              end_tlv;             
}__attribute__((packed)) erps_raps_pdu_t;

enum erps_erps_status{
    ERPS_ENABLE = 1,
    ERPS_DISABLE = 2,
};
enum erps_inst_status{
    Erps_Inst_Create = 1,
    Erps_Inst_Delete = 2
};
#if 0
enum erps_Protected_inst_status{
    Erps_Protected_Inst_Add = 1,
    Erps_Protected_Inst_Del = 2
};
#endif
enum erps_ring_status{
    Erps_Ring_Create = 1,
    Erps_Ring_Active = 2,
    Erps_Ring_Destroy = 3,
    Erps_Ring_Inactive = 4
};

enum erps_ringPort_status{
    erps_ringPort_create = 0,
    erps_ringPort_delete = 1
};

enum erps_domain_work_mode{
    ERPS_MODE_REVERTIVE,            
    ERPS_MODE_NON_REVERTIVE         
};
enum erps_node_state{
    ERPS_NODE_STATE_HEALTH,
    ERPS_NODE_STATE_FAULT
};
enum {
    Sub_Ring_With_Virtual_Channel,
    Sub_Ring_Without_Virtual_Channel
};
typedef enum erps_pkt_type_s{
    ERPS_PKT_NR ,               
    ERPS_PKT_MS = 7,            
    ERPS_PKT_SF = 11,           
    ERPS_PKT_FS = 13,           
    ERPS_PKT_EVENT = 14,        
    ERPS_PKT_NRRB =16           
}erps_pkt_type_t;
typedef enum erps_bridge_role_s{
    ERPS_RPL_Node,
    ERPS_RPL_Owner_Node,
    ERPS_RPL_Neighbour_Node,
    ERPS_RPL_Next_Neighbour_Node,
    ERPS_RPL_NONE
}erps_bridge_role_t;

#if 0

typedef enum erps_port_role_s{
    ERPS_RPL_PORT,
    ERPS_RPL_Owner_PORT,
    ERPS_RPL_Neighbour_PORT,
    ERPS_RPL_Next_Neighbour_PORT,
    ERPS_PORT_ROLE_NONE
}erps_port_role_t;
typedef enum erps_msg_code_s{
    ERPS_MSG_PKT_RCVD = 1,
    ERPS_MSG_AG_DOWN,
    ERPS_MSG_AG_UP,
    ERPS_MSG_LINK_FAULT,
    ERPS_MSG_LINK_RECOVER,
    ERPS_MSG_TICK,
    ERPS_MSG_PROTO_ONOFF,
    ERPS_MSG_WTR_TIME,
    ERPS_MSG_WTB_TIME,
    ERPS_MSG_GUARD_TIME,
    ERPS_MSG_HOLDOFF_TIME,
    ERPS_MSG_CONTROL_VLAN,
    ERPS_MSG_RING_ID,
    ERPS_MSG_PORT0_ADD,
    ERPS_MSG_PORT0_DEL,
    ERPS_MSG_PORT1_ADD,
    ERPS_MSG_PORT1_DEL,
    ERPS_MSG_RING_STATUS,
    ERPS_MSG_RING_LEVEL,
    ERPS_MSG_INST,
    ERPS_MSG_INST_MEL,
    ERPS_MSG_INST_MODE,
    ERPS_MSG_UPDT_PORT_STATE,
    ERPS_MSG_PROT_INST
}erps_msg_code_t;

#endif

typedef enum erps_event_s{
    ERPS_EVENT_CLEAR,            
    ERPS_EVENT_LOCAL_FS,         
    ERPS_EVENT_FS,               
    ERPS_EVENT_LOCAL_SF,         
    ERPS_EVENT_CLEAR_SF,         
    ERPS_EVENT_SF,               
    ERPS_EVENT_MS,               
    ERPS_EVENT_LOCAL_MS,         
    ERPS_EVENT_WTR_EXPIRES,      
    ERPS_EVENT_WTR_RUNNING,      
    ERPS_EVENT_WTB_EXPIRES,      
    ERPS_EVENT_WTB_RUNNING,      
    ERPS_EVENT_NRRB,             
    ERPS_EVENT_NR,               
    ERPS_EVENT_GUARDTIME_EXPIRES,
    ERPS_EVENT_HOLDOFF_EXPIRES,  
    ERPS_EVENT_NONE              
}erps_event_t;

typedef enum erps_stm_state_s{
    ERPS_STATUS_INIT,           
    ERPS_STATUS_IDLE,           
    ERPS_STATUS_PROTECTION,     
    ERPS_STATUS_MANUAL_SWITCH,  
    ERPS_STATUS_FORCED_SWITCH,  
    ERPS_STATUS_PENDING         
}erps_stm_state_t;

typedef struct erps_port_s{
    int portId;                 
    int bLacpPort;              
    int portEnable;             
    int state;                  
    erps_bool_t faultState;         
    int salErpsPortState;         
    int portRole;  
    erps_bool_t BPR;                
    uint8 nodeId[6];          
    erps_bool_t sendBPR;            
    
    int rcvNRCnt;
    int rcvNRRBCnt;
    int rcvMSCnt;
    int rcvSFCnt;
    int rcvFSCnt;
    int rcvEventCnt;
    int rcvOtherCnt;
    
    int sendNRCnt;
    int sendNRRBCnt;
    int sendMSCnt;
    int sendSFCnt;
    int sendFSCnt;
    int sendEventCnt;
    int sendOtherCnt;
}erps_port_t;

typedef struct erps_timer_s{
   int active;
   struct timer_list value;
} erps_timer_t;
typedef struct erps_virtual_channel_s{
    erps_bool_t bVirCha;        
    int vlanId;                 
    int subRingId;              
}erps_virtual_channel_t;
typedef struct erps_ring_s{
    
    int ringId;                            
    int ringLevel;                         
    int ctrlVlanId;                        
    erps_bool_t ringEnable;                    
    erps_port_t port0;                     
    erps_port_t port1;                     
    erps_bridge_role_t role;               
    erps_timer_t TxRAPSTimer;
    erps_timer_t WTRTimer;
    erps_timer_t WTBTimer;
    erps_timer_t guardTimer;
    erps_timer_t holdoffTimer;
    erps_timer_t TxRAPSSFTimer;
    erps_bool_t sendTimes;                     
    erps_stm_state_t stmStatus;            
    erps_virtual_channel_t VirCha[ERPS_MAX_RING_INS];     
    erps_bool_t rxDNF;
    erps_bool_t sendDNF;
    erps_pkt_type_t sendType;
}erps_ring_t;
typedef struct erps_inst_s{
    struct erps_inst_s *next;
    uint32 instId;                
    uint8 mel;               
    int workMode;              
    int TxRAPSTime;
    int WTRTime;
    int WTBTime;
    int guardTime;
    int holdoffTime;
    erps_bool_t bStgMap[CAPA_STP_MST_INSTANCE_NUM + 1];
    erps_ring_t ring;
    uint8 bridgeId[ETHER_ADDR_LEN];
}erps_inst_t;

typedef struct erps_bridge_info_s{
    erps_bool_t bErpsEnable;
    erps_inst_t instInfo;
    uint8 bridgeId[ETHER_ADDR_LEN];
    
}erps_bridge_info_t;
#if 0
int32 erps_init(void);
int32 erps_exit(void);
int32 erps_enable_set(sys_enable_t enable);
int32 erps_enable_get(sys_enable_t *pEnable);
int erps_inst_create(uint32 instId);
int erps_inst_delete(uint32 instId);
int erps_timer_set(erps_msg_code_t code, int instId, uint32 value);
int erps_ringPort_set(int instId, int portId, erps_port_role_t portRole, uint32 isPort0, uint32 isPortAdd);
int erps_ringId_set(int instId, int ringId);
int erps_ctrlVlan_set(int instId, int vlanId);
int erps_ringLevel_set(int instId, int level);
int erps_mel_set(uint32 instId, int mel);
int erps_workMode_set(uint32 instId, int mode);
int erps_inst_pgInst_set(uint32 instId, uint32 instList, uint32 status);
int32 erps_ringState_set(int instId, sys_enable_t state);
#endif
int erps_ring_create(erps_inst_t *inst, int ringId);
int erps_ring_active(erps_inst_t *inst, int ringId);
int erps_ring_inactive(erps_inst_t *inst, int ringId);
int erps_ring_destroy(erps_inst_t *inst, int ringId);
int erps_ringStatus(erps_inst_t *inst, int status);

int erps_clear_port(erps_port_t *port);
void erps_start_timer(void (*expiry_fn) (unsigned long), unsigned long instId,
	uint32 timeout, struct timer_list *tEntry);
erps_bool_t erps_stop_timer(struct timer_list *tEntry);

void erps_tx_raps_timer_start(erps_inst_t *inst);
void erps_tx_raps_timer_expired(unsigned long instId);
void erps_tx_raps_timer_stop(erps_inst_t *inst);
void erps_wtr_timer_start(erps_inst_t *inst);
void erps_wtr_timer_expired(unsigned long instId);
void erps_wtr_timer_stop(erps_inst_t *inst);
void erps_wtb_timer_start(erps_inst_t *inst);
void erps_wtb_timer_expired(unsigned long instId);
void erps_wtb_timer_stop(erps_inst_t *inst);
void erps_guard_timer_start(erps_inst_t *inst);
void erps_guard_timer_expired(unsigned long instId);
void erps_guard_timer_stop(erps_inst_t *inst);
void erps_holdoff_timer_start(erps_inst_t *inst);
void erps_holdoff_timer_expired(unsigned long instId);
void erps_holdoff_timer_stop(erps_inst_t *inst);
int erps_set_port_state(erps_inst_t *inst, erps_ring_t *ring, int portId, int state);
int erps_get_port_enable(int portId, int *enable);
void erps_flush_mac(erps_inst_t *inst);
uint8 erps_get_node_status(erps_ring_t *ring);
erps_inst_t * erps_find_inst(uint32  instId);
erps_bool_t erps_protected_inst_used_by_port(uint32 instId, int port_id, int stg);
int erps_event_stm(erps_inst_t *inst, erps_ring_t *ring, erps_event_t event, int port_id);
void erps_rcv_nrrb(erps_raps_pdu_t *pkt, erps_inst_t *inst, erps_ring_t *ring, int portId);
void erps_rcv_nr(erps_raps_pdu_t *pkt, erps_inst_t *inst, erps_ring_t *ring, int portId);
void erps_rcv_sf(erps_raps_pdu_t *pkt, erps_inst_t *inst, erps_ring_t *ring, int portId);
int erps_stm_rcv(erps_raps_pdu_t *pkt, uint32 portId, uint32 ulVlanId);
void erps_pkt_cnt(erps_ring_t *ring, int portId, uint8 pkttype, uint8 ucInpkt);
void erps_send_pkt(erps_raps_pdu_t *pkt, erps_inst_t *inst, erps_ring_t *ring, int portId, uint32 transPkt);

#endif 

#endif 

