
#ifndef __RSD_TRUNK_H__
#define __RSD_TRUNK_H__

#include <rtk/trunk.h>
#include <common/sys_def.h>

typedef enum sys_trunk_type_e
{
    TRUNK_TYPE_EMPTY = 0,
    TRUNK_TYPE_STATIC,
    TRUNK_TYPE_LACP,
    TRUNK_TYPE_END
} sys_trunk_type_t;

typedef struct sys_trunk_event_s
{
    sys_notifier_subject_trunk_event_t event;
    uint32 trunkIdx;
    sys_logic_portmask_t portmask;
} sys_trunk_event_t;

typedef enum sys_trunk_hashAlgo_e
{
    LAG_HASH_ALGO_SPA,
    LAG_HASH_ALGO_MAC,
    LAG_HASH_ALGO_IP_MAC,
    LAG_HASH_ALGO_DMAC,
    LAG_HASH_ALGO_SMAC,
    LAG_HASH_ALGO_DIP,
    LAG_HASH_ALGO_SIP,
    LAG_HASH_ALGO_END
} sys_trunk_hashAlgo_t;

#define TRUNK_PROC_NAME    "trunk"
#define TRUNK_PROC_SEQNAME trunk
#define TRUNK_PROC_BUFLEN  4096

extern sys_trunk_hashAlgo_t trunkHashAlgo;
extern sys_logic_portmask_t trunkMbrCfg[CAPA_LAG_MBR_NORMAL_NUM];

#define DFLT_TRUNK_HASH_ALGO_IDX    0

extern int32 rsd_trunk_init(void);

extern int32 rsd_trunk_proc_dump(char *buf, int32 buflen, int32 *pLen);

extern int32 rsd_trunk_chip_init(void);

extern int32 rsd_trunk_port2Trunk_get(sys_logic_port_t  port, uint32 *pTrunkId);

extern int32 rsd_trunk_portRole_get(sys_logic_port_t port, sys_trunk_portRole_t *pRole);

extern int32 rsd_trunk_memberPortTrunkId_get(sys_logic_port_t port, uint32 *pTrunkId);

extern int32 rsd_trunk_txPortmask_get(sys_logic_portmask_t *pTxSrcPm, sys_logic_portmask_t *pTxDstPm);

extern int32 rsd_trunk_member_get(uint32 trunkId, sys_logic_portmask_t *pMbrPm);

extern int32 rsd_trunk_activeMember_get(uint32 trunkId,  sys_logic_portmask_t *pPortmask);

extern int32 rsd_trunk_hashAlgo_set(sys_trunk_hashAlgo_t algo);

extern int32 rsd_trunk_hashAlgo_get(sys_trunk_hashAlgo_t *pAlgo);

extern int32 rsd_trunk_staticGroup_del(uint32 trunkId);

extern int32 rsd_trunk_staticGroup_set(
    uint32               trunkId,
    sys_logic_portmask_t *pPortmask);

#ifdef CONFIG_SYS_PROTO_LACP

extern int32 rsd_trunk_lacpGroup_del(uint32 trunkId);

extern int32 rsd_trunk_lacpGroup_set(
    uint32               trunkId,
    sys_logic_portmask_t *pPortmask);

extern int32 rsd_trunk_port_set(uint32 trunkId,  sys_logic_port_t port);

extern int32 rsd_trunk_port_del(uint32 trunkId,  sys_logic_port_t port);
#endif 

#endif 

