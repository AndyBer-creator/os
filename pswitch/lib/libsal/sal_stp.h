
#ifndef __SAL_STP_H__
#define __SAL_STP_H__

#include <stp/proto_stp.h>
#include <stp/proto_stp_mstp.h>

#define STP_TIMER_SEQUENCE_NUM     3

typedef enum stp_timer_sequence_s
{
    STP_FORWARD_DELAY = 0,
    STP_MAX_AGE,
    STP_HELLO_TIME,
}stp_timer_sequence_t;

extern const sys_text_t text_stp_forceversion[];

extern int32 sal_stp_enable_set(sys_enable_t enable);

extern int32 sal_stp_enable_get(sys_enable_t *pEnable);

extern int32 sal_stp_forwardBpdu_set(sys_enable_t enable);

extern int32 sal_stp_forwardBpdu_get(sys_enable_t *pEnable);

extern int32 sal_stp_forceVersion_set(uint32 forceVersion);

extern int32 sal_stp_forceVersion_get(uint32 *pForceVersion);

extern int32 sal_stp_helloTime_set(uint32 helloTime);

extern int32 sal_stp_helloTime_get(uint32 *pHelloTime);

#ifdef CONFIG_SYS_PROTO_STP_MSTP

extern int32 sal_stp_maxHops_set(uint32 maxHops);

extern int32 sal_stp_maxHops_get(uint32 *pMaxHops);
#endif

extern int32 sal_stp_forwardDelay_set(uint32 forwardDelay);

extern int32 sal_stp_forwardDelay_get(uint32 *pForwardDelay);

extern int32 sal_stp_maximumAge_set(uint32 maximumAge);

extern int32 sal_stp_maximumAge_get(uint32 *pMaximumAge);

extern int32 sal_stp_timer_set(uint32 helloTime,uint32 forwardDelay,int32 maxAge);

extern int32 sal_stp_timer_sequence_generate(uint32 forwardDelay,uint32 maxAge,uint32 helloTime,uint32 sequence[3]);

extern int32 sal_stp_timer_sequence_get(uint32 sequence[STP_TIMER_SEQUENCE_NUM]);

extern int32 sal_stp_txHoldCount_set(uint32 txHoldCount);

extern int32 sal_stp_txHoldCount_get(uint32 *pTxHoldCount);

extern int32 sal_stp_pathcostMethod_set(uint32 method);

extern int32 sal_stp_pathcostMethod_get(uint32 *pMethod);

extern int32 sal_stp_priority_set(uint32 priority);

extern int32 sal_stp_priority_get(uint32 *pPriority);

extern int32 sal_stp_portAdminEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_stp_portAdminEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_stp_portForwardBpdu_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_stp_portForwardBpdu_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_stp_portPriority_set(sys_logic_port_t port, uint32 priority);

extern int32 sal_stp_portPriority_get(sys_logic_port_t port, uint32 *pPriority);

extern int32 sal_stp_portPathCost_set(sys_logic_port_t port, uint32 pathCost);

extern int32 sal_stp_portPathCost_get(sys_logic_port_t port, uint32 *pPathCost);

extern int32 sal_stp_portEdgePort_set(sys_logic_port_t port, uint32 conf);

extern int32 sal_stp_portEdgePort_get(sys_logic_port_t port, uint32 *pConf);

#ifdef CONFIG_SYS_STP_BPDU_FILTER

extern int32 sal_stp_portBpduFilter_set(sys_logic_port_t port, uint32 conf);

extern int32 sal_stp_portBpduFilter_get(sys_logic_port_t port, uint32 *pConf);
#endif

#ifdef CONFIG_SYS_STP_BPDU_GUARD

extern int32 sal_stp_portBpduGuard_set(sys_logic_port_t port, uint32 conf);

extern int32 sal_stp_portBpduGuard_get(sys_logic_port_t port, uint32 *pConf);
#endif

extern int32 sal_stp_portPointToPointMac_set(sys_logic_port_t port, uint32 conf);

extern int32 sal_stp_portPointToPointMac_get(sys_logic_port_t port, uint32 *pConf);

extern int32 sal_stp_portMcheck_set(sys_logic_port_t port, uint32 conf);

extern int32 sal_stp_portMcheck_get(sys_logic_port_t port, uint32 *pConf);

#ifdef CONFIG_SYS_PROTO_STP_MSTP

extern int32 sal_stp_mstDflt_init(sys_stp_mst_dflt_t dfltInfo);

extern int32 sal_stp_mstConfigName_set(char *name);

extern int32 sal_stp_mstConfigName_get(char *name);

extern int32 sal_stp_mstConfigRevision_set(uint32 revision);

extern int32 sal_stp_mstConfigRevision_get(uint32 *pRevision);

extern int32 sal_stp_mstVlanInstance_set(uint32 vlanId, uint32 instance);

extern int32 sal_stp_mstVlanInstance_get(uint32 vlanId, uint32 *pInstance);

extern int32 sal_stp_mstVlanmask_set(uint32 instance, sys_vlanmask_t vlanmask);

extern int32 sal_stp_mstVlanmask_get(uint32 instance, sys_vlanmask_t *pVlanmask);

extern int32 sal_stp_mstPriority_set(uint32 instance, uint32 priority);

extern int32 sal_stp_mstPriority_get(uint32 instance, uint32 *pPriority);

extern int32 sal_stp_mstPortPathCost_set(uint32 instance, sys_logic_port_t port, uint32 pathCost);

extern int32 sal_stp_mstPortPathCost_get(uint32 instance, sys_logic_port_t port, uint32 *pPathCost);

extern int32 sal_stp_mstPortPriority_set(uint32 instance, sys_logic_port_t port, uint32 priority);

extern int32 sal_stp_mstPortPriority_get(uint32 instance, sys_logic_port_t port, uint32 *pPriority);
#endif

extern int32 sal_stp_info_get(sys_stp_info_t *pStpInfo);

extern int32 sal_stp_portInfo_get(sys_logic_port_t port, sys_stp_port_info_t *pPortInfo);

extern int32 sal_stp_portCounter_clear(sys_logic_port_t port);

extern int32 sal_stp_mstInfo_get(uint32 instance, sys_stp_mst_info_t *pMstInfo);

extern int32 sal_stp_mstPortInfo_get(uint32 instance, sys_logic_port_t port, sys_stp_mst_port_info_t *pMstPortInfo);

extern int32 sal_stp_mstPortMsg_get(uint32 instance, sys_logic_port_t port, sys_stp_mstPortMsg_t *pMstPortMsg);

#endif 

