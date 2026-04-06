
#ifndef __SKI_TRUNK_H__
#define __SKI_TRUNK_H__

#include <ski/ski_core.h>
#include <rsd/rsd_trunk.h>

typedef enum ski_trunk_cmd_e
{
    
    SKI_TRUNK_SET_BASE = (SKI_CORE_MOD_TRUNK * SKI_CORE_MODCMD_TOTAL),
    SKI_TRUNK_SET_STATIC_GROUP,
    SKI_TRUNK_SET_STATIC_GROUP_DEL,
    SKI_TRUNK_SET_LACP_GROUP,
    SKI_TRUNK_SET_LACP_GROUP_DEL,
    SKI_TRUNK_SET_HASH_ALGO,
    SKI_TRUNK_SET_BACKUP_PORTS_ADD,
    SKI_TRUNK_SET_BACKUP_PORTS_DEL,
    SKI_TRUNK_SET_BACKUP_PORTS_REFRESH,

    SKI_TRUNK_GET_BASE = (SKI_TRUNK_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_TRUNK_GET_MODE,
    SKI_TRUNK_GET_ACTIVE_MEMBER,
    SKI_TRUNK_GET_PORT_ROLE,
    SKI_TRUNK_GET_HASH_ALGO,
    SKI_TRUNK_GET_MEMBER_PORT_TRUNK_ID,
    SKI_TRUNK_GET_BACKUP_PORTS_ACTIVE,

    SKI_TRUNK_CMD_END
} ski_trunk_cmd_t;

typedef struct ski_trunk_group_s
{
    uint32 trunkId;
    sys_logic_portmask_t *pMemberMask;
} ski_trunk_group_t;

typedef struct ski_trunk_portRole_s
{
    sys_logic_port_t  port;
    sys_trunk_portRole_t portRole;
} ski_trunk_portRole_t;

typedef struct ski_trunk_portId_s
{
    sys_logic_port_t port;
    uint32 trunkId;
} ski_trunk_portId_t;

typedef struct ski_trunk_port_mapping_s
{
    sys_logic_port_t port;
    sys_logic_portmask_t *pPortmask;
} ski_trunk_port_mapping_t;

typedef struct ski_trunk_backupPorts_s
{
    uint32 trunkId;
    uint32 priority;
    sys_logic_portmask_t *pPortmask;
} ski_trunk_backupPorts_t;

typedef union ski_trunk_ioctl_u
{
    uint32 trunkId;
    sys_trunk_hashAlgo_t trunkHashAlgo;
    ski_trunk_group_t trunkGroup;
    ski_trunk_portRole_t trunkPortRole;
    ski_trunk_portId_t portTrunkId;
    ski_trunk_port_mapping_t trunkPortMapping;
}ski_trunk_ioctl_t;

extern int32 ski_trunk_init(void);

#endif  

