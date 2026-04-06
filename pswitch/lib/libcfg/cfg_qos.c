
#include <libcfg/cfg.h>

int32 cfg_qos_mode_set(sys_qos_mode_t mode)
{
    cfg_qos_t *pQos = NULL;

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->mode = mode;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_mode_get(sys_qos_mode_t *pMode)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pMode = pQos->mode;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_strictPriQueueNum_set(uint32 spqNum)
{
    cfg_qos_t *pQos = NULL;

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->spqNum = spqNum;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_strictPriQueueNum_get(uint32 *pSpqNum)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((NULL == pSpqNum), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pSpqNum = pQos->spqNum;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_queueWeight_set(sys_qos_queueWeight_t weight)
{
    cfg_qos_t *pQos = NULL;

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->queWeight = weight;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_queueWeight_get(sys_qos_queueWeight_t *pWeight)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((NULL == pWeight), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pWeight = pQos->queWeight;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portCos_set(sys_logic_port_t port, sys_qos_cos_t cos)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((cos >= CAPA_QOS_COS_NUM), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->portCosMap.port2Cos[port] = cos;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portCos_get(sys_logic_port_t port, sys_qos_cos_t *pCos)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCos), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pCos = pQos->portCosMap.port2Cos[port];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_cosQueue_set(sys_qos_cos_t cos, sys_qid_t qid)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((cos >= CAPA_QOS_COS_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((qid >= CAPA_QOS_QUEUE_NUM), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->cosQueueMap.cos2Queue[cos] = qid;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_cosQueue_get(sys_qos_cos_t cos, sys_qid_t *pQid)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((cos >= CAPA_QOS_COS_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pQid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pQid = pQos->cosQueueMap.cos2Queue[cos];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_dscpQueue_set(sys_qos_dscp_t dscp, sys_qid_t qid)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((dscp >= CAPA_QOS_DSCP_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((qid >= CAPA_QOS_QUEUE_NUM), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->dscpQueueMap.dscp2Queue[dscp] = qid;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_dscpQueue_get(sys_qos_dscp_t dscp, sys_qid_t *pQid)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((dscp >= CAPA_QOS_DSCP_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pQid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pQid = pQos->dscpQueueMap.dscp2Queue[dscp];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_ippQueue_set(sys_qos_ipp_t ipp, sys_qid_t qid)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((ipp >= CAPA_QOS_IPP_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((qid >= CAPA_QOS_QUEUE_NUM), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->ippQueueMap.ipp2Queue[ipp] = qid;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_ippQueue_get(sys_qos_ipp_t ipp, sys_qid_t *pQid)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((ipp >= CAPA_QOS_IPP_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pQid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pQid = pQos->ippQueueMap.ipp2Queue[ipp];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_trustType_set(sys_qos_trustType_t trustType)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((trustType >= SYS_QOS_TRUST_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->trustType = trustType;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_trustType_get(sys_qos_trustType_t *pTrustType)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((NULL == pTrustType), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pTrustType = pQos->trustType;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portTrustType_set(sys_logic_port_t port, sys_qos_trustType_t trustType)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((trustType >= SYS_QOS_TRUST_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->portTrustType[port] = trustType;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portTrustType_get(sys_logic_port_t port, sys_qos_trustType_t *pTrustType)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTrustType), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pTrustType = pQos->portTrustType[port];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_queueCos_set(sys_qid_t qid, sys_qos_cos_t cos)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((cos >= CAPA_QOS_COS_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((qid >= CAPA_QOS_QUEUE_NUM), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->queueCosMap.queue2Cos[qid] = cos;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_queueCos_get(sys_qid_t qid, sys_qos_cos_t *pCos)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((qid >= CAPA_QOS_QUEUE_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pCos), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pCos = pQos->queueCosMap.queue2Cos[qid];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_queueDscp_set(sys_qid_t qid, sys_qos_dscp_t dscp)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((dscp >= CAPA_QOS_DSCP_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((qid >= CAPA_QOS_QUEUE_NUM), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->queueDscpMap.queue2Dscp[qid] = dscp;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_queueDscp_get(sys_qid_t qid, sys_qos_dscp_t *pDscp)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((qid >= CAPA_QOS_QUEUE_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pDscp), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pDscp = pQos->queueDscpMap.queue2Dscp[qid];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_queueIpp_set(sys_qid_t qid, sys_qos_ipp_t ipp)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((ipp >= CAPA_QOS_IPP_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((qid >= CAPA_QOS_QUEUE_NUM), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->queueIppMap.queue2Ipp[qid] = ipp;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_queueIpp_get(sys_qid_t qid, sys_qos_ipp_t *pIpp)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((qid >= CAPA_QOS_QUEUE_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pIpp), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pIpp = pQos->queueIppMap.queue2Ipp[qid];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portRemarkCos_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->portRemarkCos[port] = enable;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portRemarkCos_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pEnable = pQos->portRemarkCos[port];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portRemarkDscp_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->portRemarkDscp[port] = enable;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portRemarkDscp_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pEnable = pQos->portRemarkDscp[port];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portRemarkIpp_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->portRemarkIpp[port] = enable;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_portRemarkIpp_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pEnable = pQos->portRemarkIpp[port];

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_congAvoid_set(sys_enable_t enable)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    pQos->congAvoid = enable;

    CFG_CLOSE(CFG_ID_QOS);
}

int32 cfg_qos_congAvoid_get(sys_enable_t *pEnable)
{
    cfg_qos_t *pQos = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_QOS, (cfg_addr)&pQos);

    *pEnable = pQos->congAvoid;

    CFG_CLOSE(CFG_ID_QOS);
}

