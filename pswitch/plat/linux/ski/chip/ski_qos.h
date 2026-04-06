
#ifndef __SKI_QOS_H__
#define __SKI_QOS_H__

#include <ski/ski_core.h>
#include <rsd/rsd_qos.h>

typedef enum ski_qos_cmd_e
{
    
    SKI_QOS_SET_BASE = (SKI_CORE_MOD_QOS * SKI_CORE_MODCMD_TOTAL),
    SKI_QOS_SET_MODE,
    SKI_QOS_SET_TRUST_TYPE,
    SKI_QOS_SET_PORT_TRUST_TYPE,
    SKI_QOS_SET_PORT_QWEIGHT,
    SKI_QOS_SET_PORT_COS,
    SKI_QOS_SET_COS_QUEUE,
    SKI_QOS_SET_DSCP_QUEUE,
    SKI_QOS_SET_IPP_QUEUE,
    SKI_QOS_SET_QUEUE_COS,
    SKI_QOS_SET_QUEUE_DSCP,
    SKI_QOS_SET_QUEUE_IPP,
    SKI_QOS_SET_PORT_REMARK_COS,
    SKI_QOS_SET_PORT_REMARK_DSCP,
    SKI_QOS_SET_PORT_REMARK_IPP,

    SKI_QOS_GET_BASE = (SKI_QOS_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_QOS_GET_IPRMK_ABILITY,  

    SKI_QOS_CMD_END
} ski_qos_cmd_t;

typedef struct ski_qos_portTrustType_s
{
    sys_logic_port_t    port;
    sys_qos_trustType_t trust;
} ski_qos_portTrustType_t;

typedef struct ski_qos_queueWeight_s
{
    sys_logic_port_t      port;
    sys_qos_queueWeight_t *pWeight;
} ski_qos_queueWeight_t;

typedef struct ski_qos_portCos_s
{
    sys_logic_port_t port;
    sys_qos_cos_t    cos;
} ski_qos_portCos_t;

typedef struct ski_qos_cosQueue_s
{
    sys_qos_cos_t      cos;
    sys_qid_t qid;
} ski_qos_cosQueue_t;

typedef struct ski_qos_dscpQueue_s
{
    sys_qos_dscp_t     dscp;
    sys_qid_t qid;
} ski_qos_dscpQueue_t;

typedef struct ski_qos_ippQueue_s
{
    sys_qos_ipp_t      ipp;
    sys_qid_t qid;
} ski_qos_ippQueue_t;

typedef struct ski_qos_portEnbl_s
{
    sys_logic_port_t port;
    sys_enable_t     enable;
} ski_qos_portEnbl_t;

typedef struct ski_qos_rmkEnPortMask_s
{
    sys_logic_portmask_t ippRmkEnPm; 
    sys_logic_portmask_t dscpRmkEnPm;
}ski_qos_rmkEnPortMask_t;

typedef union ski_qos_ioctl_u
{
    sys_qos_mode_t mode;
    sys_qos_trustType_t trustType;
    ski_qos_portTrustType_t portTrustType;
    ski_qos_queueWeight_t portQueueWeight;
    ski_qos_portCos_t portCosMap;
    ski_qos_cosQueue_t cosQueueMap;
    ski_qos_dscpQueue_t dscpQueueMap;
    ski_qos_ippQueue_t ippQueueMap;
    ski_qos_portEnbl_t portEnbl;
    ski_qos_rmkEnPortMask_t remarkEnPm;
} ski_qos_ioctl_t;

int32 ski_qos_init(void);

#endif 

