#ifndef __PROTO_STP_MSTP_H__
#define __PROTO_STP_MSTP_H__
#include <common/sys_type.h>
#include <common/sys_error.h>
#include <stp/proto_stp.h>
typedef int8    mstp_bool_t;
typedef uint8   mstp_u8_t;
typedef uint16  mstp_u16_t;
typedef uint32  mstp_u32_t;
typedef int8    mstp_i8_t;
typedef int16   mstp_i16_t;
typedef int32   mstp_i32_t;
typedef uint16  mstp_timer_t;
typedef mstp_u8_t   mstp_fsm_state_t;
typedef mstp_u8_t   mstp_info_t;
typedef mstp_u8_t   mstp_port_role_t;

#ifndef Enabled
#define Enabled     (1)
#endif
#ifndef Disabled
#define Disabled    (0)
#endif

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN      (6)
#endif
#define MAX_MSTP_PORT_NUM               (SYS_LOGIC_PORT_NUM)    
#define MAX_MSTP_INSTANCE               (MSTP_INSTANCE_NUM)
#define MAX_MSTP_VLAN_NUM               (SYS_VLAN_NUM_MAX)

#define MSTP_INSTANCE_CIST              (0)
#ifndef MSTP_CONF_NAME_LEN
#define MSTP_CONF_NAME_LEN              CAPA_STP_MST_CFG_NAME_LEN
#endif
#ifndef MSTP_CONF_DIGEST_LEN
#define MSTP_CONF_DIGEST_LEN            (16)
#endif
#define MSTP_DFLT_BRIDGE_PRIORITY       (0x8000)
#define MSTP_DFLT_PORT_PRIORITY         (0x80)
#define MSTP_DFLT_FORCE_VERSION         (3)
#define MSTP_DFLT_MESSAGE_AGE           (0x0000)    
#define MSTP_DFLT_HELLO_TIME            (0x0200)    
#define MSTP_DFLT_FORWARD_DELAY         (0x0F00)    
#define MSTP_DFLT_MAX_AGE               (0x1400)    
#define MSTP_DFLT_MAX_HOPS              (20)        
#define MSTP_DFLT_TRANSMIT_HOLD_COUNT   (6)
#define MSTP_DFLT_MIGRATE_TIME          (0x0300)    
#define MSTP_DFLT_PORT_EXT_PATH_COST    (0)
#define MSTP_DFLT_PORT_INT_PATH_COST    (0)

#define MSTP_TIME_ONE_SECOND            (0x0100)    
#define MSTP_HELLO_TIME_MIN             (0x0100)    
#define MSTP_HELLO_TIME_MAX             (0x0A00)    

#define MSTP_DFLT_PORT_ENABLE               (FALSE)
#define MSTP_DFLT_PORT_EDGE                 (FALSE)
#define MSTP_DFLT_PORT_LOOPBACK_GUARD       (TRUE)
#define MSTP_DFLT_PORT_BPDU_FILTER          (FALSE)
#define MSTP_DFLT_PORT_BPDU_GUARD           (FALSE)
#define MSTP_DFLT_PORT_POINT_TO_POINT_MAC   (TRUE)
#define MSTP_DFLT_PORT_RESTRICTED_ROLE      (FALSE)
#define MSTP_DFLT_PORT_RESTRICTED_TCN       (FALSE)

#define MSTP_BPDU_LEN_ETH               (6 + 6 + 2)
#define MSTP_BPDU_LEN_LLC               (3)
#define MSTP_BPDU_LEN_BASE              (MSTP_BPDU_LEN_ETH + MSTP_BPDU_LEN_LLC)
#define MSTP_BPDU_LEN_CONFIG            (35)
#define MSTP_BPDU_LEN_TCN               (4)
#define MSTP_BPDU_LEN_RST               (36)
#define MSTP_BPDU_LEN_MST               (102)
#define MSTP_BPDU_LEN_MSTI              (16)
#define MSTP_BPDU_LEN_MSTI_BASE         (MSTP_BPDU_LEN_BASE + MSTP_BPDU_LEN_MST)

#define MSTP_BPDU_LEN_CONFIG_MAX        (MSTP_BPDU_LEN_BASE + MSTP_BPDU_LEN_CONFIG)
#define MSTP_BPDU_LEN_TCN_MAX           (MSTP_BPDU_LEN_BASE + MSTP_BPDU_LEN_TCN)
#define MSTP_BPDU_LEN_MSTP_MAX          (MSTP_BPDU_LEN_BASE + MSTP_BPDU_LEN_MST + \
                                         MSTP_BPDU_LEN_MSTI * (MAX_MSTP_INSTANCE - 1))

#define MSTP_BPDU_PROTO_ID              (0x0000)

#define MSTP_BPDU_VERSION_STP           (0x00)
#define MSTP_BPDU_VERSION_STP1          (0x01)
#define MSTP_BPDU_VERSION_RSTP          (0x02)
#define MSTP_BPDU_VERSION_MSTP          (0x03)

#define MSTP_BPDU_TYPE_CONFIG           (0x00)
#define MSTP_BPDU_TYPE_TCN              (0x80)
#define MSTP_BPDU_TYPE_RST              (0x02)

#define MSTP_PORT_STATE_DISABLED        (0)
#define MSTP_PORT_STATE_DISCARDING      (1)
#define MSTP_PORT_STATE_LEARNING        (2)
#define MSTP_PORT_STATE_FORWARDING      (3)

#define MSTP_BPDU_OPER_STP_CONFIG       (0)
#define MSTP_BPDU_OPER_STP_TCN          (1)
#define MSTP_BPDU_OPER_STP_RST          (2)
#define MSTP_BPDU_OPER_STP_MST          (3)
#define MSTP_BPDU_FLAG_TC               (0x1 << 0)
#define MSTP_BPDU_FLAG_PROPOSAL         (0x1 << 1)
#define MSTP_BPDU_FLAG_PORT_ROLE        (0x3 << 2)
#define MSTP_BPDU_FLAG_LEARNING         (0x1 << 4)
#define MSTP_BPDU_FLAG_FORWARDING       (0x1 << 5)
#define MSTP_BPDU_FLAG_AGREEMENT        (0x1 << 6)
#define MSTP_BPDU_FLAG_TC_ACK           (0x1 << 7)
#define MSTP_BPDU_FLAG_MASTER           (0x1 << 7)
#define MSTP_BPDU_PORT_ROLE_MASTER                  (0x0 << 2)
#define MSTP_BPDU_PORT_ROLE_ALTERNATE_OR_BACKUP     (0x1 << 2)
#define MSTP_BPDU_PORT_ROLE_ROOT                    (0x2 << 2)
#define MSTP_BPDU_PORT_ROLE_DESIGNATED              (0x3 << 2)
typedef enum sys_inst_tag_e
{
    SYS_INST_MSTP,
    SYS_INST_ERPS,
    SYS_INST_LOOPBACK,
    SYS_INST_END,
}sys_ints_tag_t;

typedef struct mstp_func_ops_s
{
    int32 (*mstp_pkt_tx_cb_f)(mstp_u32_t portId, mstp_u8_t *pData, mstp_i32_t len);
    int32 (*mstp_fdb_flush_cb_f)(mstp_u32_t instance, mstp_u32_t portId);
    int32 (*mstp_portState_change_cb_f)(mstp_u32_t instance, mstp_u32_t portId, mstp_u32_t portState);
} mstp_func_ops_t;

typedef struct mstp_fsm_s
{
    mstp_fsm_state_t state;
    mstp_bool_t changeState;
} mstp_fsm_t;

typedef struct mstp_bridge_id_s
{
    mstp_u16_t  priority;
    mstp_u8_t   addr[ETHER_ADDR_LEN];
} mstp_bridge_id_t;

typedef struct mstp_port_id_s
{
    mstp_u8_t   priority;
    mstp_u16_t  number;
} mstp_port_id_t;

typedef struct mstp_times_s
{
    mstp_u16_t  MessageAge;
    mstp_u16_t  MaxAge;
    mstp_u16_t  ForwardDelay;
    mstp_u16_t  HelloTime;          
    mstp_u8_t   remainingHops;      
    mstp_u8_t   MaxHops;            
} mstp_times_t;

typedef struct mstp_pri_vctr_s
{
    mstp_bridge_id_t    RootID;
    mstp_u32_t          ExtRootPathCost;
    mstp_bridge_id_t    RRootID;
    mstp_u32_t          IntRootPathCost;
    mstp_bridge_id_t    DesignatedBridgeID;
    mstp_port_id_t      DesignatedPortID;
    mstp_port_id_t      RcvPortID;
} mstp_pri_vctr_t;

typedef struct mstp_mst_conf_id_s
{
    mstp_u8_t           idFormatSelector;
    mstp_u8_t           name[MSTP_CONF_NAME_LEN];
    mstp_u16_t          revisionLevel;
    mstp_u8_t           digest[MSTP_CONF_DIGEST_LEN];
} mstp_mst_conf_id_t;

typedef struct mstp_bpdu_rx_cist_s
{
    mstp_u8_t           operBpduType;                   
    mstp_u16_t          ethLen;
    mstp_u16_t          numMsti;                        

    mstp_u16_t          protoId;                        
    mstp_u8_t           protoVerId;                     
    mstp_u8_t           bpduType;                       
    mstp_u8_t           flags;                          
    mstp_bridge_id_t    rootId;                         
    mstp_u32_t          externalPathCost;               
    mstp_bridge_id_t    regionalRootId;                 
    mstp_port_id_t      portId;                         
    mstp_u16_t          messageAge;                     
    mstp_u16_t          maxAge;                         
    mstp_u16_t          helloTime;                      
    mstp_u16_t          forwardDelay;                   

    mstp_u8_t           ver1Len;                        

    mstp_u16_t          ver3Len;                        
    mstp_mst_conf_id_t  mstConfigId;                    
    mstp_u32_t          internalRootPathCost;           
    mstp_bridge_id_t    bridgeId;                       
    mstp_u8_t           remainingHops;                  
} mstp_bpdu_rx_cist_t;

typedef struct mstp_bpdu_rx_msti_s
{
    mstp_u8_t           flags;
    mstp_bridge_id_t    regionalRootId;
    mstp_u32_t          internalRootPathCost;
    mstp_u8_t           bridgePriority;
    mstp_u8_t           portPriority;
    mstp_u8_t           remainingHops;
} mstp_bpdu_rx_msti_t;

typedef struct mstp_port_s
{
    
    struct mstp_bridge_s    *pBridge;
    struct mstp_xst_port_s  *pCistPort;
    struct mstp_xst_port_s  *pXstPort[MAX_MSTP_INSTANCE];

    sys_enable_t        adminEnable; 
    sys_enable_t        forwardBpdu;   

    mstp_u8_t           portIdx;

    mstp_u32_t          adminExtPathCost;

    mstp_u8_t           ethAddrSrc[ETHER_ADDR_LEN];

    mstp_bpdu_rx_cist_t bpduRxCist;

    mstp_u32_t          cntRxConfig;
    mstp_u32_t          cntRxTcn;
    mstp_u32_t          cntRxMstp;
    mstp_u32_t          cntTxConfig;
    mstp_u32_t          cntTxTcn;
    mstp_u32_t          cntTxMstp;

#ifdef CONFIG_SYS_STP_SELFLOOP_PREVENT
    mstp_bool_t         adminLoopbackGuard;
    mstp_timer_t        loopbackDelayWhile;
#endif  
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    mstp_bool_t         adminBpduFilter;
#endif  
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    mstp_bool_t         adminBpduGuard;
#endif  
    mstp_bool_t         adminPointToPointMAC;
    mstp_bool_t         autoPointToPointMAC;
    mstp_bool_t         operPointToPointMAC;

    mstp_fsm_t          fsmPortTimers;              
    mstp_fsm_t          fsmPortProtocolMigration;   
    mstp_fsm_t          fsmPortReceive;             
    mstp_fsm_t          fsmPortTransmit;            
    mstp_fsm_t          fsmBridgeDetection;         

    mstp_timer_t        mdelayWhile;                
    mstp_timer_t        helloWhen;                  
    mstp_timer_t        edgeDelayWhile;             

    mstp_u16_t          PortHelloTime;              
    mstp_bool_t         AdminEdgePort;              
        mstp_bool_t         AutoEdge;                   
    mstp_u8_t           PortIdentifierPriority;     
    mstp_u32_t          ExternalPortPathCost;       

        mstp_bool_t         operEdge;                   
    mstp_bool_t         portEnabled;                
    mstp_bool_t         tick;                       
    mstp_timer_t        txCount;                    
    mstp_bool_t         infoInternal;               
    mstp_bool_t         rcvdInternal;               
    mstp_bool_t         restrictedRole;             
    mstp_bool_t         restrictedTcn;              
    mstp_bool_t         newInfoMsti;                
    mstp_bool_t         mcheck;                     
    mstp_bool_t         rcvdBpdu;                   
    mstp_bool_t         rcvdRSTP;                   
    mstp_bool_t         rcvdSTP;                    
    mstp_bool_t         rcvdTcAck;                  
    mstp_bool_t         rcvdTcn;                    
    mstp_bool_t         sendRSTP;                   
    mstp_bool_t         tcAck;                      
    mstp_bool_t         newInfo;                    
} mstp_port_t;

typedef struct mstp_xst_port_s
{
    
    struct mstp_bridge_s    *pBridge;
    struct mstp_port_s      *pPort;
    struct mstp_xst_s       *pXst;
    struct mstp_xst_s       *pCist;
    struct mstp_xst_port_s  *pCistPort;

    mstp_u8_t           xstFlag; 
    mstp_u16_t          xstIdx;
    mstp_u8_t           portIdx;

    mstp_u32_t          adminIntPathCost;

    mstp_u8_t           portState;
    mstp_bool_t         portStateChanged;

    mstp_bool_t         rcvdBpduMsti;
    mstp_bpdu_rx_msti_t bpduRxMsti;

    mstp_fsm_t          fsmPortInformation;         
    mstp_fsm_t          fsmPortRoleTransitions;     
    mstp_fsm_t          fsmPortStateTransition;     
    mstp_fsm_t          fsmTopologyChange;          

    mstp_timer_t        fdWhile;                    
    mstp_timer_t        rrWhile;                    
    mstp_timer_t        rbWhile;                    
    mstp_timer_t        tcWhile;                    
    mstp_timer_t        rcvdInfoWhile;              

    mstp_u32_t          InternalPortPathCost;       

    mstp_bool_t         fdbFlush;                   
    mstp_bool_t         agree;                      
    mstp_bool_t         disputed;                   
    mstp_bool_t         forward;                    
    mstp_bool_t         forwarding;                 
    mstp_info_t         infoIs;                     
    mstp_bool_t         learn;                      
    mstp_bool_t         learning;                   
    mstp_bool_t         proposed;                   
    mstp_bool_t         proposing;                  
    mstp_info_t         rcvdInfo;                   
    mstp_bool_t         rcvdMsg;                    
    mstp_bool_t         rcvdTc;                     
    mstp_bool_t         reRoot;                     
    mstp_bool_t         reselect;                   
    mstp_bool_t         selected;                   
    mstp_bool_t         tcProp;                     
    mstp_bool_t         updtInfo;                   
    mstp_bool_t         agreed;                     
    mstp_pri_vctr_t     designatedPriority;         
    mstp_times_t        designatedTimes;            
    mstp_pri_vctr_t     msgPriority;                
    mstp_times_t        msgTimes;                   
    mstp_port_id_t      portId;                     
    mstp_pri_vctr_t     portPriority;               
    mstp_times_t        portTimes;                  
    mstp_port_role_t    role;                       
    mstp_port_role_t    selectedRole;               
    mstp_bool_t         sync;                       
    mstp_bool_t         synced;                     
    mstp_bool_t         master;                     
    mstp_bool_t         mastered;                   
} mstp_xst_port_t;

typedef struct mstp_xst_s
{
    
    struct mstp_bridge_s    *pBridge;
    mstp_u16_t              xstIdx;
    mstp_u8_t               rootPortIdx;
        mstp_bool_t             xstEnabled;
    mstp_u8_t               xstFlag; 
    mstp_u32_t              tcTimer;
    mstp_u32_t              tcTimes;

    mstp_fsm_t          fsmPortRoleSelection;       
    mstp_xst_port_t     xstPort[SYS_LOGICPORT_NUM_MAX];

    mstp_u16_t          BridgeIdentifierPriority;   

    mstp_bridge_id_t    BridgeIdentifier;           
    mstp_pri_vctr_t     BridgePriority;             
    mstp_times_t        BridgeTimes;                
    mstp_port_id_t      rootPortId;                 
    mstp_pri_vctr_t     rootPriority;               
    mstp_times_t        rootTimes;                  
} mstp_xst_t;

typedef mstp_i32_t (*mstp_pkt_tx_cb_f)(mstp_u32_t portId, mstp_u8_t *pData, mstp_i32_t len);
typedef mstp_i32_t (*mstp_fdb_flush_cb_f)(mstp_u32_t instance, mstp_u32_t portId);
typedef mstp_i32_t (*mstp_portState_change_cb_f)(mstp_u32_t instance, mstp_u32_t portId, mstp_u32_t portState);

typedef struct mstp_bridge_s
{
    
    struct mstp_bridge_s    *pSelfValid;
    mstp_bool_t             resetBridge;
    
    mstp_port_t             port[SYS_LOGICPORT_NUM_MAX];
    mstp_xst_t              xst[MAX_MSTP_INSTANCE];

    mstp_u8_t               macAddr[ETHER_ADDR_LEN];

    mstp_u16_t              helloTime;
    
    mstp_pkt_tx_cb_f            cbf_pkt_tx;
    mstp_fdb_flush_cb_f         cbf_fdb_flush;
    mstp_portState_change_cb_f  cbf_portState_change;

    mstp_u8_t           ForceProtocolVersion;       
    mstp_u16_t          BridgeForwardDelay;         
    mstp_u8_t           TransmitHoldCount;          
    mstp_u16_t          MigrateTime;                
    mstp_u16_t          BridgeMaxAge;               
    mstp_u8_t           MaxHops;                    

    mstp_bool_t         BEGIN;                      
    mstp_mst_conf_id_t  MstConfigId;                
} mstp_bridge_t;
extern void mstp_bridge_init(mstp_bridge_t *pBridge, mstp_func_ops_t *pOps);
extern void mstp_bridge_exit(mstp_bridge_t *pBridge);
extern void mstp_bridge_fsm(mstp_bridge_t *pBridge);
extern void mstp_bridge_reset(mstp_bridge_t *pBridge);
extern void mstp_bridge_routine(mstp_bridge_t *pBridge);
extern void mstp_bridge_tick(mstp_bridge_t *pBridge);
extern int32 mstp_bridge_pkt_rx(mstp_bridge_t *pBridge, mstp_u32_t portId, mstp_u8_t *pData, mstp_i32_t len);

#endif 
