#ifndef __PROTO_STP_H__
#define __PROTO_STP_H__
#include <common/sys_type.h>
#include <common/sys_portview.h>
#ifndef MSTP_PORT_TRUNK_NUM
#define MSTP_PORT_TRUNK_NUM     CAPA_LAG_NUM
#endif
#ifndef MSTP_INSTANCE_NUM
#define MSTP_INSTANCE_NUM       CAPA_STP_MST_INSTANCE_NUM
#endif
#ifndef MSTP_PATH_COST_MAX
#define MSTP_PATH_COST_MAX      BOND_STP_PATHCOST_LONG_MAX
#endif
#ifndef MSTP_CONF_NAME_LEN
#define MSTP_CONF_NAME_LEN      CAPA_STP_MST_CFG_NAME_LEN
#endif
#ifndef MSTP_CONF_DIGEST_LEN
#define MSTP_CONF_DIGEST_LEN    (16)
#endif
#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN          (6)
#endif
#ifndef STATE_STRING_LEN
#define STATE_STRING_LEN        (32)
#endif
#ifndef MSTP_BPDU_PRIORITY
#define MSTP_BPDU_PRIORITY      (6)
#endif

#define MSTP_BRIDGE_PRIORITY_MASK   (0xF000)
#define MSTP_BRIDGE_EXTID_MASK      (0x0FFF)
#define MSTP_PORT_PRIORITY_MASK     (0xF0)
#define MSTP_PORT_NUMBER_MASK       (0x0F)

typedef enum sys_stp_port_role_e
{
    STP_PORT_ROLE_DISABLED = 0,
    STP_PORT_ROLE_MASTER,
    STP_PORT_ROLE_ROOT,
    STP_PORT_ROLE_DESIGNATED,
    STP_PORT_ROLE_ALTERNATE,
    STP_PORT_ROLE_BACKUP,
    END_STP_PORT_ROLE
} sys_stp_port_role_t;

typedef enum sys_stp_port_state_e
{
    STP_PORT_STATE_DISABLED = 0,
    STP_PORT_STATE_DISCARDING,
    STP_PORT_STATE_LEARNING,
    STP_PORT_STATE_FORWARDING,
    END_STP_PORT_STATE
} sys_stp_port_state_t;

typedef enum sys_stp_port_conf_e
{
    STP_PORT_CONF_FALSE = 0,
    STP_PORT_CONF_TRUE,
    STP_PORT_CONF_AUTO,
} sys_stp_port_conf_t;

typedef enum sys_stp_port_mode_e
{
    STP_PORT_MODE_STP = 0,
    STP_PORT_MODE_RSTP = 2,
    STP_PORT_MODE_MSTP = 3,
} sys_stp_port_mode_t;

typedef enum sys_stp_port_type_e
{
    STP_PORT_TYPE_BOUNDARY = 0,
    STP_PORT_TYPE_INTERNAL,
} sys_stp_port_type_t;

typedef enum sys_stp_forceversion_s
{
    STP_VERSION_STP = 0,
    STP_VERSION_NULL,
    STP_VERSION_RSTP,
    STP_VERSION_MSTP,
} sys_stp_forceversion_t;

typedef enum sys_stp_pathcost_method_s
{
    STP_PATHCOST_METHOD_LONG = 0,
    STP_PATHCOST_METHOD_SHORT,
} sys_stp_pathcost_method_t;
typedef struct sys_stp_bridge_id_s
{
    uint16  priority;
    uint8   addr[ETHER_ADDR_LEN];
} sys_stp_bridge_id_t;

typedef struct sys_stp_port_id_s
{
    uint8   priority;
    uint16  number;
} sys_stp_port_id_t;

typedef struct sys_stp_pri_vctr_s
{
    sys_stp_bridge_id_t rootBridgeId;
    uint32              extRootPathCost;
    sys_stp_bridge_id_t regionalRootBridgeId;
    uint32              intRootPathCost;
    sys_stp_bridge_id_t designatedBridgeId;
    sys_stp_port_id_t   designatedPortId;
    sys_stp_port_id_t   rcvPortId;
} sys_stp_pri_vctr_t;

typedef struct sys_stp_info_s
{
    sys_enable_t enable;
    sys_enable_t forwardBpdu;
    uint8   forceVersion;

    uint8   macAddr[ETHER_ADDR_LEN];

    uint8   helloTime;
    uint8   maxHops;
    uint8   forwardDelay;
    uint8   maximumAge;
    uint8   txHoldCount;
    uint8   pathcostMethod;

    struct
    {
        char    name[MSTP_CONF_NAME_LEN + 1];
        uint16  revision;
        uint8   digest[MSTP_CONF_DIGEST_LEN];
    } mstConfId;
} sys_stp_info_t;

typedef struct sys_stp_port_counter_s
{
    uint32  rxConfig;
    uint32  rxTcn;
    uint32  rxMstp;
    uint32  txConfig;
    uint32  txTcn;
    uint32  txMstp;
} sys_stp_port_counter_t;

typedef struct sys_stp_port_info_s
{
    sys_stp_port_id_t portId;
    sys_enable_t adminEnable;
    sys_enable_t forwardBpdu;
    sys_stp_port_mode_t mode;
    sys_stp_port_type_t type;

    uint32  confExtPathCost;
    uint32  operExtPathCost;
    uint8   confEdge;
    uint8   operEdge;
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    uint8   confBpduFilter;
#endif
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    uint8   confBpduGuard;
#endif
    uint8   confPointToPointMac;
    uint8   operPointToPointMac;

    uint8   confRestrictedRole;
    uint8   confRestrictedTcn;

    sys_stp_port_counter_t pktCount;

    struct
    {
        uint16  mdelayWhile;
        uint16  helloWhen;
        uint16  edgeDelayWhile;
        uint16  txCount;
    } portTimers;

    uint8   strStatePTM[STATE_STRING_LEN];
    uint8   strStatePPM[STATE_STRING_LEN];
    uint8   strStatePRX[STATE_STRING_LEN];
    uint8   strStatePTX[STATE_STRING_LEN];
    uint8   strStateBDT[STATE_STRING_LEN];
} sys_stp_port_info_t;

typedef struct sys_stp_mst_info_s
{
    sys_enable_t    enable;
    sys_stp_bridge_id_t bridgeId;

    struct
    {
        
        sys_stp_bridge_id_t rootBridgeId;
        uint32              externalRootPathCost;
        
        sys_stp_bridge_id_t regionalRootBridgeId;
        uint32              internalRootPathCost;
        sys_stp_bridge_id_t designatedBridgeId;
    } rootPriority;

    sys_stp_port_id_t   rootPortId;
    uint32  tcTimer;    
    uint32  tcTimes;
    
    uint16  helloTime;
    uint16  maxAge;
    uint16  forwardDelay;
    
    uint8   remainingHops;

    sys_vlanmask_t vlanmask;
    uint16  vlanCount;

    uint8   strStatePRS[STATE_STRING_LEN];
} sys_stp_mst_info_t;

typedef struct sys_stp_mst_port_info_s
{
    sys_stp_port_id_t   portId;
    uint32              confIntPathCost;
    uint32              operIntPathCost;

    struct
    {
        
        sys_stp_bridge_id_t rootBridgeId;
        uint32              externalRootPathCost;
        
        sys_stp_bridge_id_t regionalRootBridgeId;
        uint32              internalRootPathCost;
        sys_stp_bridge_id_t designatedBridgeId;
        sys_stp_port_id_t   designatedPortId;
    } portPriority;

    struct
    {
        uint8               remainingHops;
    } portTimes;

    sys_stp_port_role_t     portRole;
    sys_stp_port_state_t    portState;
    struct
    {
        uint16  tcWhile;
        uint16  fdWhile;
        uint16  rcvdInfoWhile;
        uint16  rrWhile;
        uint16  rbWhile;
    } portTimers;

    uint8   strStatePIF[STATE_STRING_LEN];
    uint8   strStatePRT[STATE_STRING_LEN];
    uint8   strStatePST[STATE_STRING_LEN];
    uint8   strStateTCG[STATE_STRING_LEN];
} sys_stp_mst_port_info_t;

typedef struct sys_stp_mst_dflt_s
{
    uint32 priority;
    uint32 portPathCost;
    uint32 portPriority;
} sys_stp_mst_dflt_t;
typedef struct sys_stp_portMsg_s
{

} sys_stp_portMsg_t;

typedef struct sys_stp_mstMsg_s
{

} sys_stp_mstMsg_t;

typedef struct sys_stp_mstPortMsg_s
{
    uint8 rcvdInternal;

    sys_stp_pri_vctr_t  designatedPriority;
    sys_stp_pri_vctr_t  msgPriority;
    sys_stp_pri_vctr_t  portPriority;
} sys_stp_mstPortMsg_t;
#define STP_LOG(mid, args...)               \
do {                                        \
    sys_enable_t enable = DISABLED;         \
    stp_enable_get(&enable);                \
    if (ENABLED == enable)                  \
        SYS_LOG(LOG_CAT_STP, mid, ##args); \
} while(0)
extern int32 stp_init(void);
extern int32 stp_exit(void);
extern int32 stp_enable_set(sys_enable_t enable);
extern int32 stp_enable_get(sys_enable_t *pEnable);
extern int32 stp_forwardBpdu_set(sys_enable_t enable);
extern int32 stp_forwardBpdu_get(sys_enable_t *pEnable);
extern int32 stp_forceVersion_set(uint32 forceVersion);
extern int32 stp_forceVersion_get(uint32 *pForceVersion);
extern int32 stp_helloTime_set(uint32 helloTime);
extern int32 stp_helloTime_get(uint32 *pHelloTime);
extern int32 stp_maxHops_set(uint32 maxHops);
extern int32 stp_maxHops_get(uint32 *pMaxHops);
extern int32 stp_forwardDelay_set(uint32 forwardDelay);
extern int32 stp_forwardDelay_get(uint32 *pForwardDelay);
extern int32 stp_maximumAge_set(uint32 maximumAge);
extern int32 stp_maximumAge_get(uint32 *pMaximumAge);
extern int32 stp_txHoldCount_set(uint32 txHoldCount);
extern int32 stp_txHoldCount_get(uint32 *pTxHoldCount);
extern int32 stp_pathcostMethod_set(uint32 method);
extern int32 stp_pathcostMethod_get(uint32 *pMethod);
extern int32 stp_portAdminEnable_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 stp_portAdminEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 stp_portForwardBpdu_set(sys_logic_port_t port, sys_enable_t enable);
extern int32 stp_portForwardBpdu_get(sys_logic_port_t port, sys_enable_t *pEnable);
extern int32 stp_portPathCost_set(sys_logic_port_t port, uint32 pathCost);
extern int32 stp_portPathCost_get(sys_logic_port_t port, uint32 *pPathCost);
extern int32 stp_portEdgePort_set(sys_logic_port_t port, uint32 conf);
extern int32 stp_portEdgePort_get(sys_logic_port_t port, uint32 *pConf);
extern int32 stp_portPointToPointMac_set(sys_logic_port_t port, uint32 conf);
extern int32 stp_portPointToPointMac_get(sys_logic_port_t port, uint32 *pConf);
extern int32 stp_portMcheck_set(sys_logic_port_t port, uint32 conf);
extern int32 stp_portMcheck_get(sys_logic_port_t port, uint32 *pConf);

#ifdef CONFIG_SYS_STP_BPDU_FILTER

extern int32 stp_portBpduFilter_set(sys_logic_port_t port, uint32 conf);
extern int32 stp_portBpduFilter_get(sys_logic_port_t port, uint32 *pConf);
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD

extern int32 stp_portBpduGuard_set(sys_logic_port_t port, uint32 conf);
extern int32 stp_portBpduGuard_get(sys_logic_port_t port, uint32 *pConf);
#endif
extern int32 stp_mstDflt_init(sys_stp_mst_dflt_t dfltInfo);
extern int32 stp_mstConfigName_set(char *name);
extern int32 stp_mstConfigName_get(char *name);
extern int32 stp_mstConfigRevision_set(uint32 revision);
extern int32 stp_mstConfigRevision_get(uint32 *pRevision);
extern int32 stp_mstVlanInstance_set(sys_vid_t vlanId, uint32 instance);
extern int32 stp_mstVlanInstance_get(sys_vid_t vlanId, uint32 *pInstance);
extern int32 stp_mstVlanmask_set(uint32 instance, sys_vlanmask_t vlanmask);

#if 0

extern int32 stp_mstVlanInstance_set(sys_vid_t vlanId, uint32 instance);
extern int32 stp_mstVlanInstance_get(sys_vid_t vlanId, uint32 *pInstance);
#endif
extern int32 stp_mstPriority_set(uint32 instance, uint32 priority);
extern int32 stp_mstPriority_get(uint32 instance, uint32 *pPriority);
extern int32 stp_mstPortPathCost_set(uint32 instance, sys_logic_port_t port, uint32 pathCost);
extern int32 stp_mstPortPathCost_get(uint32 instance, sys_logic_port_t port, uint32 *pPathCost);
extern int32 stp_mstPortPriority_set(uint32 instance, sys_logic_port_t port, uint32 priority);
extern int32 stp_mstPortPriority_get(uint32 instance, sys_logic_port_t port, uint32 *pPriority);
extern int32 stp_info_get(sys_stp_info_t *pInfo);
extern int32 stp_portInfo_get(sys_logic_port_t port, sys_stp_port_info_t *pPortInfo);
extern int32 stp_portCounter_clear(sys_logic_port_t port);
extern int32 stp_mstInfo_get(uint32 instance, sys_stp_mst_info_t *pMstInfo);
extern int32 stp_mstPortInfo_get(uint32 instance, sys_logic_port_t port, sys_stp_mst_port_info_t *pMstPortInfo);
extern int32 stp_portMsg_get(sys_logic_port_t port, sys_stp_portMsg_t *pPortMsg);
extern int32 stp_mstMsg_get(uint32 instance, sys_stp_mstMsg_t *pMstMsg);
extern int32 stp_mstPortMsg_get(uint32 instance, sys_logic_port_t port, sys_stp_mstPortMsg_t *pMstPortMsg);

#endif 
