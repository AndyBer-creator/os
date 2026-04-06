
#ifndef __CFG_QOS_H__
#define __CFG_QOS_H__

#include <libsal/sal_qos.h>

typedef struct cfg_qos_s
{
    sys_qos_mode_t            mode;
    uint32                    spqNum;
    sys_qos_queueWeight_t     queWeight;
    sys_qos_cosQueue_t        cosQueueMap;
    sys_qos_dscpQueue_t       dscpQueueMap;
    sys_qos_ippQueue_t        ippQueueMap;
    sys_qos_portCos_t         portCosMap;
    sys_qos_trustType_t       trustType;
    sys_qos_trustType_t       portTrustType[SYS_LOGICPORT_NUM_MAX];

    sys_qos_queueCos_t        queueCosMap;
    sys_qos_queueDscp_t       queueDscpMap;
    sys_qos_queueIpp_t        queueIppMap;
    sys_enable_t              portRemarkCos[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t              portRemarkDscp[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t              portRemarkIpp[SYS_LOGICPORT_NUM_MAX];

    sys_enable_t              congAvoid;
} cfg_qos_t;

extern int32 cfg_qos_mode_set(sys_qos_mode_t mode);

extern int32 cfg_qos_mode_get(sys_qos_mode_t *pMode);

extern int32 cfg_qos_strictPriQueueNum_set(uint32 spqNum);

extern int32 cfg_qos_strictPriQueueNum_get(uint32 *pSpqNum);

extern int32 cfg_qos_queueWeight_set(sys_qos_queueWeight_t weight);

extern int32 cfg_qos_queueWeight_get(sys_qos_queueWeight_t *pWeight);

extern int32 cfg_qos_portCos_set(sys_logic_port_t port, sys_qos_cos_t cos);

extern int32 cfg_qos_portCos_get(sys_logic_port_t port, sys_qos_cos_t *pCos);

extern int32 cfg_qos_cosQueue_set(sys_qos_cos_t cos, sys_qid_t qid);

extern int32 cfg_qos_cosQueue_get(sys_qos_cos_t cos, sys_qid_t *pQid);

extern int32 cfg_qos_dscpQueue_set(sys_qos_dscp_t dscp, sys_qid_t qid);

extern int32 cfg_qos_dscpQueue_get(sys_qos_dscp_t dscp, sys_qid_t *pQid);

extern int32 cfg_qos_ippQueue_set(sys_qos_ipp_t ipp, sys_qid_t qid);

extern int32 cfg_qos_ippQueue_get(sys_qos_ipp_t ipp, sys_qid_t *pQid);

extern int32 cfg_qos_trustType_set(sys_qos_trustType_t trustType);

extern int32 cfg_qos_trustType_get(sys_qos_trustType_t *pTrustType);

extern int32 cfg_qos_portTrustType_set(sys_logic_port_t port, sys_qos_trustType_t trustType);

extern int32 cfg_qos_portTrustType_get(sys_logic_port_t port, sys_qos_trustType_t *pTrustType);

extern int32 cfg_qos_advTrustType_set(sys_qos_trustType_t trustType);

extern int32 cfg_qos_advTrustType_get(sys_qos_trustType_t *pTrustType);

extern int32 cfg_qos_queueCos_set(sys_qid_t qid, sys_qos_cos_t cos);

extern int32 cfg_qos_queueCos_get(sys_qid_t qid, sys_qos_cos_t *pCos);

extern int32 cfg_qos_queueDscp_set(sys_qid_t qid, sys_qos_dscp_t dscp);

extern int32 cfg_qos_queueDscp_get(sys_qid_t qid, sys_qos_dscp_t *pDscp);

extern int32 cfg_qos_queueIpp_set(sys_qid_t qid, sys_qos_ipp_t ipp);

extern int32 cfg_qos_queueIpp_get(sys_qid_t qid, sys_qos_ipp_t *pIpp);

extern int32 cfg_qos_portRemarkCos_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_qos_portRemarkCos_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_qos_portRemarkDscp_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_qos_portRemarkDscp_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_qos_portRemarkIpp_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_qos_portRemarkIpp_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_qos_congAvoid_set(sys_enable_t enable);

extern int32 cfg_qos_congAvoid_get(sys_enable_t *pEnable);

#endif 

