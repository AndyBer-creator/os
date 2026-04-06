
#ifndef __RSD_QOS_H__
#define __RSD_QOS_H__

#include <rtk/qos.h>
#include <rsd/rsd_sys.h>

#define SYS_QOS_DP_MAX          RTK_DROP_PRECEDENCE_MAX
#define SYS_QOS_DFLT_QUEUE      (0)

typedef rtk_qos_queue_weights_t sys_qos_queueWeight_t;
typedef rtk_qos_priSelWeight_t sys_qos_priSelWeight_t;

typedef enum sys_qos_mode_e
{
    SYS_QOS_MODE_DISABLE = 0,
    SYS_QOS_MODE_BASIC,
    SYS_QOS_MODE,
    SYS_QOS_MODE_END
} sys_qos_mode_t;

typedef enum sys_qos_trustType_e
{
    SYS_QOS_TRUST_NONE = 0, 
    SYS_QOS_TRUST_COS,
    SYS_QOS_TRUST_DSCP,
    SYS_QOS_TRUST_COS_DSCP,
    SYS_QOS_TRUST_IPP,
    SYS_QOS_TRUST_END
} sys_qos_trustType_t;

typedef enum sys_qos_schedAlgo_e
{
    SYS_QOS_SCHED_WFQ = WFQ,
    SYS_QOS_SCHED_WRR,
    SYS_QOS_SCHED_SP,
    SYS_QOS_SCHED_SP_WFQ,
    SYS_QOS_SCHED_SP_WRR,
    SYS_QOS_SCHED_END
} sys_qos_schedulingType_t;

typedef enum sys_qos_cos_e
{
    SYS_QOS_COS_BE = 0, 
    SYS_QOS_COS_BK,     
    SYS_QOS_COS_EE,     
    SYS_QOS_COS_CA,     
    SYS_QOS_COS_VI,     
    SYS_QOS_COS_VO,     
    SYS_QOS_COS_IC,     
    SYS_QOS_COS_NC,     
    SYS_QOS_COS_END
} sys_qos_cos_t;

typedef enum sys_qos_ipp_e
{
    SYS_QOS_IPP_ROUTINE = 0,
    SYS_QOS_IPP_PRIORITY,
    SYS_QOS_IPP_IMMEDIATE,
    SYS_QOS_IPP_FLASH,
    SYS_QOS_IPP_FLASH_OVERRIDE,
    SYS_QOS_IPP_CRITICAL,
    SYS_QOS_IPP_INTERNET,
    SYS_QOS_IPP_NETWORK,
    SYS_QOS_IPP_END
} sys_qos_ipp_t;

typedef enum sys_qos_dscp_e
{
    SYS_QOS_DSCP_DEFAULT = 0,
    SYS_QOS_DSCP_AF11 = 10,
    SYS_QOS_DSCP_AF12 = 12,
    SYS_QOS_DSCP_AF13 = 16,
    SYS_QOS_DSCP_AF21 = 18,
    SYS_QOS_DSCP_AF22 = 20,
    SYS_QOS_DSCP_AF23 = 22,
    SYS_QOS_DSCP_AF31 = 26,
    SYS_QOS_DSCP_AF32 = 28,
    SYS_QOS_DSCP_AF33 = 30,
    SYS_QOS_DSCP_AF41 = 34,
    SYS_QOS_DSCP_AF42 = 36,
    SYS_QOS_DSCP_AF43 = 38,
    SYS_QOS_DSCP_CS1 = 8,
    SYS_QOS_DSCP_CS2 = 16,
    SYS_QOS_DSCP_CS3 = 24,
    SYS_QOS_DSCP_CS4 = 32,
    SYS_QOS_DSCP_CS5 = 40,
    SYS_QOS_DSCP_CS6 = 48,
    SYS_QOS_DSCP_CS7 = 56,
    SYS_QOS_DSCP_EF = 46,
    SYS_QOS_DSCP_END = 64
} sys_qos_dscp_t;

typedef enum sys_qos_queueWeightType_e
{
    SYS_QOS_QUEWEIGHT_DISABLE = 0,
    SYS_QOS_QUEWEIGHT_SP,
    SYS_QOS_QUEWEIGHT_WFQ_WRR,
    SYS_QOS_QUEWEIGHT_SP_WFQ_WRR,
    SYS_QOS_QUEWEIGHT_END
} sys_qos_queueWeightType_t;

typedef enum sys_qos_qid_e
{
    QOS_QUEUE0 = 0,
    QOS_QUEUE1,
    QOS_QUEUE2,
    QOS_QUEUE3,
    QOS_QUEUE4,
    QOS_QUEUE5,
    QOS_QUEUE6,
    QOS_QUEUE7,
    QOS_QUEUE_MAX,
} sys_qos_qid_t;

typedef enum sys_qos_pri_e
{
    QOS_PRIO_0 = 0,
    QOS_PRIO_1,
    QOS_PRIO_2,
    QOS_PRIO_3,
    QOS_PRIO_4,
    QOS_PRIO_5,
    QOS_PRIO_6,
    QOS_PRIO_7,
    QOS_PRIO_MAX,
} sys_qos_pri_t;

typedef enum sys_qos_mapType_e
{
    SYS_QOS_MAP_COS2QUEUE = 0,
    SYS_QOS_MAP_DSCP2QUEUE,
    SYS_QOS_MAP_IPP2QUEUE,
    SYS_QOS_MAP_QUEUE2COS,
    SYS_QOS_MAP_QUEUE2DSCP,
    SYS_QOS_MAP_QUEUE2IPP,
    SYS_QOS_MAP_END
} sys_qos_mapType_t;

typedef struct sys_qos_event_s
{
    sys_qid_t           qid;
    sys_qos_cos_t       cos;
} sys_qos_event_t;

typedef struct sys_qos_mixQueueType_s
{
    sys_qos_queueWeightType_t type[CAPA_QOS_QUEUE_NUM];
} sys_qos_mixQueueType_t;

typedef struct sys_qos_queueInfo_s
{
    uint32 weight;
    uint32 dp;
} sys_qos_queueInfo_t;

typedef struct sys_qos_cosQueue_s
{
    sys_qid_t cos2Queue[CAPA_QOS_COS_NUM];
} sys_qos_cosQueue_t;

typedef struct sys_qos_dscpQueue_s
{
    sys_qid_t dscp2Queue[CAPA_QOS_DSCP_NUM];
} sys_qos_dscpQueue_t;

typedef struct sys_qos_ippQueue_s
{
    sys_qid_t ipp2Queue[CAPA_QOS_IPP_NUM];
} sys_qos_ippQueue_t;

typedef struct sys_qos_portCos_s
{
    sys_qos_cos_t port2Cos[SYS_LOGICPORT_NUM_MAX];
} sys_qos_portCos_t;

typedef struct sys_qos_queueCos_s
{
    sys_qos_cos_t queue2Cos[CAPA_QOS_QUEUE_NUM];
} sys_qos_queueCos_t;

typedef struct sys_qos_queueDscp_s
{
    sys_qos_dscp_t queue2Dscp[CAPA_QOS_QUEUE_NUM];
} sys_qos_queueDscp_t;

typedef struct sys_qos_queueIpp_s
{
    sys_qos_dscp_t queue2Ipp[CAPA_QOS_QUEUE_NUM];
} sys_qos_queueIpp_t;

#define QOS_PROC_NAME    "qos"
#define QOS_PROC_SEQNAME qos
#define QOS_PROC_BUFLEN  4096

#define DSCP2TOS(dscp) ((dscp << 2) & 0xFC)
#define IPP2TOS(ipp)   ((ipp << 5) & 0xE0)
#define DSCP2IPP(dscp) ((dscp >> 3) & 0x7)
#define IPP2DSCP(ipp)  ((ipp << 3) & 0x38)

extern int32 _rsd_qos_portTrustType_set(sys_logic_port_t port, sys_qos_trustType_t trust);

extern int32 rsd_qos_init(void);

extern int32 rsd_qos_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_qos_mode_set(sys_qos_mode_t mode);

extern int32 rsd_qos_mode_get(sys_qos_mode_t *pMode);

extern int32 rsd_qos_trustType_set(sys_qos_trustType_t trustType);

extern int32 rsd_qos_portTrustType_set(sys_logic_port_t port, sys_qos_trustType_t trust);

extern int32 rsd_qos_portQueueWeight_set(sys_logic_port_t port, sys_qos_queueWeight_t *pWeight);

extern int32 rsd_qos_portTrustType_get(sys_logic_port_t port, sys_qos_trustType_t *pTrust);

extern int32 rsd_qos_portCos_set(sys_logic_port_t port, sys_qos_cos_t cos);

extern int32 rsd_qos_portCos_get(sys_logic_port_t port, sys_qos_cos_t *pCos);

extern int32 rsd_qos_cosQueue_set(sys_qos_cos_t cos, sys_qid_t qid);

extern int32 rsd_qos_cosQueue_get(sys_qos_cos_t cos, sys_qid_t *pQid);

extern int32 rsd_qos_dscpQueue_set(sys_qos_dscp_t dscp, sys_qid_t qid);

extern int32 rsd_qos_ippQueue_set(sys_qos_ipp_t ipp, sys_qid_t qid);

extern int32 rsd_qos_queueCos_set(sys_qid_t qid, sys_qos_cos_t cos);

extern int32 rsd_qos_queueDscp_set(sys_qid_t qid, sys_qos_dscp_t dscp);

extern int32 rsd_qos_queueIpp_set(sys_qid_t qid, sys_qos_ipp_t ipp);

extern int32 rsd_qos_portRemarkCos_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_qos_portRemarkDscp_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_qos_portRemarkIpp_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 rsd_qos_ipRemarkAbility_check(sys_logic_portmask_t ippRmkLpm,
                                                sys_logic_portmask_t dscpRmkLpm);

extern int32 rsd_qos_portVlanTunnel_set(sys_logic_port_t port, rtk_enable_t enable);
#endif 

