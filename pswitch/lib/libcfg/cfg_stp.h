
#ifndef __CFG_STP_H__
#define __CFG_STP_H__

#include <libsal/sal_stp.h>

 typedef struct cfg_stp_s
{
    
    sys_enable_t enable;
    sys_enable_t forwardBpdu;
    uint32  forceVersion;
    uint32  helloTime;
    uint32  maxHops;
    uint32  forwardDelay;
    uint32  maximumAge;
    uint32  timerSeq[STP_TIMER_SEQUENCE_NUM]; 
    uint32  txHoldCount;
    uint32  pathcostMethod;
#ifndef CONFIG_SYS_PROTO_STP_MSTP
    uint32  priority;
#endif

    struct
    {
        sys_enable_t adminEnable;
        sys_enable_t forwardBpdu;
        uint32 confPathCost;
        uint32 confEdgePort;
        uint32 confPointToPointMac;

#ifdef CONFIG_SYS_STP_BPDU_FILTER
        uint32 confBpduFilter;
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD
        uint32 confBpduGuard;
#endif

#ifndef CONFIG_SYS_PROTO_STP_MSTP
    uint32  priority;
#endif
    } port[SYS_LOGICPORT_NUM_MAX];

#ifdef CONFIG_SYS_PROTO_STP_MSTP
    
    struct
    {
        char    name[MSTP_CONF_NAME_LEN + 1];
        uint32  revision;
    } mst_config;

    uint32 mstVlanInstance[SYS_VLAN_NUM_MAX];

    struct
    {
        uint32 priority;
        sys_vlanmask_t vlanmask;

        struct
        {
            uint32 internalPathCost;
            uint32 priority;
        } mstPort[SYS_LOGICPORT_NUM_MAX];
    } mst[MSTP_INSTANCE_NUM];
#endif
} cfg_stp_t;

 extern int32 cfg_stp_init(void);

extern int32 cfg_stp_enable_set(sys_enable_t enable);

extern int32 cfg_stp_enable_get(sys_enable_t *pEnable);

extern int32 cfg_stp_forwardBpdu_set(sys_enable_t enable);

extern int32 cfg_stp_forwardBpdu_get(sys_enable_t *pEnable);

extern int32 cfg_stp_forceVersion_set(uint32 forceVersion);

extern int32 cfg_stp_forceVersion_get(uint32 *pForceVersion);

extern int32 cfg_stp_helloTime_set(uint32 helloTime);

extern int32 cfg_stp_helloTime_get(uint32 *pHelloTime);

#ifdef CONFIG_SYS_PROTO_STP_MSTP

extern int32 cfg_stp_maxHops_set(uint32 maxHops);

extern int32 cfg_stp_maxHops_get(uint32 *pMaxHops);
#endif

extern int32 cfg_stp_forwardDelay_set(uint32 forwardDelay);

extern int32 cfg_stp_forwardDelay_get(uint32 *pForwardDelay);

extern int32 cfg_stp_maximumAge_set(uint32 maximumAge);

extern int32 cfg_stp_maximumAge_get(uint32 *pMaximumAge);

extern int32 cfg_stp_timer_sequence_set(uint32 seqIdx,uint32 timerSeq);

extern int32 cfg_stp_timer_sequence_get(uint32 seqIdx,uint32 *timerSeq);

extern int32 cfg_stp_txHoldCount_set(uint32 txHoldCount);

extern int32 cfg_stp_txHoldCount_get(uint32 *pTxHoldCount);

extern int32 cfg_stp_pathcostMethod_set(uint32 method);

extern int32 cfg_stp_pathcostMethod_get(uint32 *pMethod);

extern int32 cfg_stp_priority_set(uint32 priority);

extern int32 cfg_stp_priority_get(uint32 *pPriority);

extern int32 cfg_stp_portAdminEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_stp_portAdminEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_stp_portForwardBpdu_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_stp_portForwardBpdu_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_stp_portPathCost_set(sys_logic_port_t port, uint32 pathCost);

extern int32 cfg_stp_portPathCost_get(sys_logic_port_t port, uint32 *pPathCost);

extern int32 cfg_stp_portEdgePort_set(sys_logic_port_t port, uint32 conf);

extern int32 cfg_stp_portEdgePort_get(sys_logic_port_t port, uint32 *pConf);

#ifdef CONFIG_SYS_STP_BPDU_FILTER

extern int32 cfg_stp_portBpduFilter_set(sys_logic_port_t port, uint32 conf);

extern int32 cfg_stp_portBpduFilter_get(sys_logic_port_t port, uint32 *pConf);
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD

extern int32 cfg_stp_portBpduGuard_set(sys_logic_port_t port, uint32 conf);

extern int32 cfg_stp_portBpduGuard_get(sys_logic_port_t port, uint32 *pConf);
#endif

extern int32 cfg_stp_portPointToPointMac_set(sys_logic_port_t port, uint32 conf);

extern int32 cfg_stp_portPointToPointMac_get(sys_logic_port_t port, uint32 *pConf);

extern int32 cfg_stp_portPriority_set(sys_logic_port_t port, uint32 priority);

extern int32 cfg_stp_portPriority_get(sys_logic_port_t port, uint32 *pPriority);

#ifdef CONFIG_SYS_PROTO_STP_MSTP

extern int32 cfg_stp_mstConfigName_set(char *name);

extern int32 cfg_stp_mstConfigName_get(char *name);

extern int32 cfg_stp_mstConfigRevision_set(uint32 revision);

extern int32 cfg_stp_mstConfigRevision_get(uint32 *pRevision);

extern int32 cfg_stp_mstVlanInstance_set(sys_vid_t vlanId, uint32 instance);

extern int32 cfg_stp_mstVlanInstance_get(sys_vid_t vlanId, uint32 *pInstance);

extern int32 cfg_stp_mstVlanmask_set(uint32 instance, sys_vlanmask_t vlanmask);

extern int32 cfg_stp_mstVlanmask_get(uint32 instance, sys_vlanmask_t *pVlanmask);

extern int32 cfg_stp_mstPriority_set(uint32 instance, uint32 priority);

extern int32 cfg_stp_mstPriority_get(uint32 instance, uint32 *pPriority);

extern int32 cfg_stp_mstPortPathCost_set(uint32 instance, sys_logic_port_t port, uint32 pathCost);

extern int32 cfg_stp_mstPortPathCost_get(uint32 instance, sys_logic_port_t port, uint32 *pPathCost);

extern int32 cfg_stp_mstPortPriority_set(uint32 instance, sys_logic_port_t port, uint32 priority);

extern int32 cfg_stp_mstPortPriority_get(uint32 instance, sys_logic_port_t port, uint32 *pPriority);
#endif
#endif

