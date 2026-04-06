
#ifndef __SAL_QOS_H__
#define __SAL_QOS_H__

#include <rsd/rsd_qos.h>
#include <libsal/sal_type.h>

extern const sys_text_t text_qos_mode[];
extern const sys_text_t text_qos_trust[];
extern const sys_text_t text_qos_schedAlgo[];

#define UQID2QID(uqid) (uqid - 1)
#define QID2UQID(qid)  (qid + 1)
#define QOS_MODE_CHK(mode) (sal_qos_mode_check(mode))

extern int32 sal_qos_mode_set(sys_qos_mode_t mode);

extern int32 sal_qos_mode_get(sys_qos_mode_t *pMode);

extern int32 sal_qos_mode_check(sys_qos_mode_t mode);

extern int32 sal_qos_strictPriQueueNum_set(uint32 spqNum);

extern int32 sal_qos_strictPriQueueNum_get(uint32 *pSpqNum);

extern int32 sal_qos_queueWeight_set(sys_qos_queueWeight_t weight);

extern int32 sal_qos_queueWeight_get(sys_qos_queueWeight_t *pWeight);

extern int32 sal_qos_portQueueWeight_set(sys_logic_port_t port, sys_qos_queueWeight_t *pWeight);

extern int32 sal_qos_portQueueWeight_get(sys_logic_port_t port, sys_qos_queueWeight_t *pWeight);

extern int32 sal_qos_portSchedulingAlgorithm_set(
    sys_logic_port_t         port,
    sys_qos_schedulingType_t schedAlgo);

extern int32 sal_qos_portSchedulingAlgorithm_get(
    sys_logic_port_t         port,
    sys_qos_schedulingType_t *pSchedAlgo);

extern int32 sal_qos_portMixQueueType_set(sys_logic_port_t port, sys_qos_mixQueueType_t queueType);

extern int32 sal_qos_portMixQueueType_get(sys_logic_port_t port, sys_qos_mixQueueType_t *pQueueType);

extern int32 sal_qos_portCos_set(sys_logic_port_t port, sys_qos_cos_t cos);

extern int32 sal_qos_portCos_get(sys_logic_port_t port, sys_qos_cos_t *pCos);

extern int32 sal_qos_cosQueue_set(sys_qos_cos_t cos, sys_qid_t qid);

extern int32 sal_qos_cosQueue_get(sys_qos_cos_t cos, sys_qid_t *pQid);

extern int32 sal_qos_dscpQueue_set(sys_qos_dscp_t dscp, sys_qid_t qid);

extern int32 sal_qos_dscpQueue_get(sys_qos_dscp_t dscp, sys_qid_t *pQid);

extern int32 sal_qos_ippQueue_set(sys_qos_ipp_t ipp, sys_qid_t qid);

extern int32 sal_qos_ippQueue_get(sys_qos_ipp_t ipp, sys_qid_t *pQid);

extern int32 sal_qos_trustType_set(sys_qos_trustType_t trustType);

extern int32 sal_qos_trustType_get(sys_qos_trustType_t *pTrustType);

extern int32 sal_qos_portTrustState_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_qos_portTrustState_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_qos_queueCos_set(sys_qid_t qid, sys_qos_cos_t cos);

extern int32 sal_qos_queueCos_get(sys_qid_t qid, sys_qos_cos_t *pCos);

extern int32 sal_qos_queueDscp_set(sys_qid_t qid, sys_qos_dscp_t dscp);

extern int32 sal_qos_queueDscp_get(sys_qid_t qid, sys_qos_dscp_t *pDscp);

extern int32 sal_qos_queueIpp_set(sys_qid_t qid, sys_qos_ipp_t ipp);

extern int32 sal_qos_queueIpp_get(sys_qid_t qid, sys_qos_ipp_t *pIpp);

extern int32 sal_qos_portRemarkCos_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_qos_portRemarkCos_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_qos_portRemarkDscp_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_qos_portRemarkDscp_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_qos_portRemarkIpp_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_qos_portRemarkIpp_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_qos_ipRemarkAbility_check(sys_logic_portmask_t ippRmkLpm,
                                                sys_logic_portmask_t dscpRmkLpm);

extern int32 sal_qos_ippRemarkPortMask_get(sys_logic_portmask_t* pIppRmkLpm);

extern int32 sal_qos_dscpRemarkPortMask_get(sys_logic_portmask_t* pDscpRmkLpm);

extern int32 sal_qos_mapDefault_restore(sys_qos_mapType_t mapType);

extern int32 sal_qos_congAvoid_set(sys_enable_t enable);

extern int32 sal_qos_congAvoid_get(sys_enable_t *pEnable);

#endif 

