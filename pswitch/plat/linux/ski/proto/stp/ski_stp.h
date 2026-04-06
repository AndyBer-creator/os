
#ifndef __SKI_STP_H__
#define __SKI_STP_H__

#include <ski/ski_core.h>
#include <stp/proto_stp.h>

typedef enum ski_stp_cmd_e
{
    
    SKI_STP_SET_BASE = (SKI_CORE_MOD_STP * SKI_CORE_MODCMD_TOTAL),
    SKI_STP_SET_ENABLE,
    SKI_STP_SET_FORWARD_BPDU,
    SKI_STP_SET_FORCE_VERSION,
    SKI_STP_SET_HELLO_TIME,
    SKI_STP_SET_MAX_HOPS,
    SKI_STP_SET_FORWARD_DELAY,
    SKI_STP_SET_MAXIMUM_AGE,
    SKI_STP_SET_TX_HOLD_COUNT,
    SKI_STP_SET_PATHCOST_METHOD,
    SKI_STP_SET_PORT_ADMIN_ENABLE,
    SKI_STP_SET_PORT_FORWARD_BPDU,
    SKI_STP_SET_PORT_PATH_COST,
    SKI_STP_SET_PORT_COUNTER_CLEAR,
#if 0
    SKI_STP_SET_PORT_PRIORITY,
#endif
    SKI_STP_SET_PORT_EDGE_PORT,
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    SKI_STP_SET_PORT_BPDU_FILTER,
#endif
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    SKI_STP_SET_PORT_BPDU_GUARD,
#endif
    SKI_STP_SET_PORT_POINT_TO_POINT_MAC,
    SKI_STP_SET_PORT_MCHECK,
    SKI_STP_SET_MST_DFLTINIT,
    SKI_STP_SET_MST_CONFIG_NAME,
    SKI_STP_SET_MST_CONFIG_REVISION,
    SKI_STP_SET_MST_VLAN_INSTANCE,
    SKI_STP_SET_MST_VLANMASK,
    SKI_STP_SET_MST_PRIORITY,
    SKI_STP_SET_MST_PORT_PATH_COST,
    SKI_STP_SET_MST_PORT_PRIORITY,

    SKI_STP_GET_BASE = (SKI_STP_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_STP_GET_ENABLE,
    SKI_STP_GET_FORWARD_BPDU,
    SKI_STP_GET_FORCE_VERSION,
    SKI_STP_GET_HELLO_TIME,
    SKI_STP_GET_MAX_HOPS,
    SKI_STP_GET_FORWARD_DELAY,
    SKI_STP_GET_MAXIMUM_AGE,
    SKI_STP_GET_TX_HOLD_COUNT,
    SKI_STP_GET_PATHCOST_METHOD,
    SKI_STP_GET_PORT_ADMIN_ENABLE,
    SKI_STP_GET_PORT_FORWARD_BPDU,
    SKI_STP_GET_PORT_PATH_COST,
#if 0
    SKI_STP_GET_PORT_PRIORITY,
#endif
    SKI_STP_GET_PORT_EDGE_PORT,
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    SKI_STP_GET_PORT_BPDU_FILTER,
#endif  
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    SKI_STP_GET_PORT_BPDU_GUARD,
#endif  
    SKI_STP_GET_PORT_POINT_TO_POINT_MAC,
    SKI_STP_GET_PORT_MCHECK,
    SKI_STP_GET_MST_CONFIG_NAME,
    SKI_STP_GET_MST_CONFIG_REVISION,
    SKI_STP_GET_MST_VLAN_INSTANCE,
    SKI_STP_GET_MST_VLANMASK,
    SKI_STP_GET_MST_PRIORITY,
    SKI_STP_GET_MST_PORT_PATH_COST,
    SKI_STP_GET_MST_PORT_PRIORITY,

    SKI_STP_GET_INFO,
    SKI_STP_GET_PORT_INFO,
    SKI_STP_GET_MST_INFO,
    SKI_STP_GET_MST_PORT_INFO,
    SKI_STP_GET_MST_PORT_MSG,

    SKI_STP_CMD_END
} ski_stp_cmd_t;
#define STP_NAME_LEN_MAX            (32)
#define STP_VLAN_TO_INSTANCE_SIZE   (4096)

typedef union ski_stp_ioctl_u
{
    struct {
        sys_enable_t        enable;
    } enable;
    struct {
        sys_enable_t        enable;
    } forwardBpdu;
    struct {
        uint32              forceVersion;
    } forceVersion;
    struct {
        uint32              helloTime;
    } helloTime;
    struct {
        uint32              maxHops;
    } maxHops;
    struct {
        uint32              forwardDelay;
    } forwardDelay;
    struct {
        uint32              maximumAge;
    } maximumAge;
    struct {
        uint32              txHoldCount;
    } txHoldCount;
    struct {
        uint32              pathcostMethod;
    } pathcostMethod;
    struct {
        sys_logic_port_t    port;
        sys_enable_t        adminEnable;
    } portAdminEnable;
    struct {
        sys_logic_port_t    port;
        sys_enable_t        forwardBpdu;
    } portForwardBpdu;
    struct {
        sys_logic_port_t    port;
        uint32              pathCost;
    } portPathCost;
    struct {
        sys_logic_port_t    port;
        uint32              priority;
    } portPriority;
    struct {
        sys_logic_port_t    port;
        uint32              edgePort;
    } portEdgePort;
#ifdef CONFIG_SYS_STP_BPDU_FILTER
    struct {
        sys_logic_port_t    port;
        uint32              bpduFilter;
    } portBpduFilter;
#endif
#ifdef CONFIG_SYS_STP_BPDU_GUARD
    struct {
        sys_logic_port_t    port;
        uint32              bpduGuard;
    } portBpduGuard;
#endif
    struct {
        sys_logic_port_t    port;
        uint32              pointToPointMac;
    } portPointToPointMac;
    struct {
        sys_logic_port_t    port;
        uint32              mcheck;
    } portMcheck;
    struct {
        char                name[STP_NAME_LEN_MAX];
    } mstConfigName;
    struct {
        uint32              revision;
    } mstConfigRevision;
    struct {
        uint32              vlanId;
        uint32              instance;
    } mstVlanInstance;
    struct {
        uint32              instance;
        sys_vlanmask_t      vlanmask;
    } mstVlanmask;
    struct {
        uint32              instance;
        uint32              priority;
    } mstPriority;
    struct {
        uint32              instance;
        sys_logic_port_t    port;
        uint32              priority;
    } mstPortPriority;
    struct {
        uint32              instance;
        sys_logic_port_t    port;
        uint32              pathCost;
    } mstPortPathCost;
    struct {
        sys_stp_info_t      info;
    } info;
    struct {
        sys_logic_port_t    port;
        sys_stp_port_info_t portInfo;
    } portInfo;
    struct {
        sys_logic_port_t        port;
        sys_stp_port_counter_t  portCounter;
    } portCounter;
    struct {
        uint32              instance;
        sys_stp_mst_info_t  mstInfo;
    } mstInfo;
    struct {
        sys_stp_mst_dflt_t  mstDflt;
    } mstDflt;
    struct {
        uint32                      instance;
        sys_logic_port_t            port;
        sys_stp_mst_port_info_t     mstPortInfo;
    } mstPortInfo;
    struct {
        uint32                      instance;
        sys_logic_port_t            port;
        sys_stp_mstPortMsg_t        mstPortMsg;
    } mstPortMsg;
} ski_stp_ioctl_t;

extern int32 ski_stp_init(void);

#endif  

