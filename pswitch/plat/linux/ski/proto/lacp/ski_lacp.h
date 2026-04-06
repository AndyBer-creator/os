
#ifndef __SKI_LACP_H__
#define __SKI_LACP_H__

#include <lacp/proto_lacp.h>
#include <ski/ski_core.h>

typedef enum ski_lacp_cmd_e
{
    
    SKI_LACP_SET_BASE = (SKI_CORE_MOD_LACP * SKI_CORE_MODCMD_TOTAL),
    SKI_LACP_SET_EBL,
    SKI_LACP_SET_SYSTEM_PRI,
    SKI_LACP_SET_GROUP,
    SKI_LACP_SET_GROUP_DEL,
    SKI_LACP_SET_PORT_ACTIVE,
    SKI_LACP_SET_PORT_PRI,
    SKI_LACP_SET_PORT_TIMEOUT,
    SKI_LACP_RESET_PORT_COUNTER,

    SKI_LACP_GET_BASE = (SKI_LACP_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_LACP_GET_EBL,
    SKI_LACP_GET_SYSTEM_PRI,
    SKI_LACP_GET_PORT_STATE,
    SKI_LACP_GET_BACKUP_MEMBER,
    SKI_LACP_GET_PORT_COUNTER,

    SKI_LACP_CMD_END
} ski_lacp_cmd_t;

typedef struct ski_lacp_group_s
{
    uint32 trunkId;
    sys_logic_portmask_t *pMemberPm;
    sys_logic_portmask_t *pActPm;
} ski_lacp_group_t;

typedef struct ski_lacp_port_state_s
{
    sys_logic_port_t port;
    sys_lacp_port_t *pPortState;
} ski_lacp_port_state_t;

typedef struct ski_lacp_portPri_s
{
    sys_logic_port_t port;
    uint16           pri;
} ski_lacp_portPri_t;

typedef struct ski_lacp_portAct_s
{
    sys_logic_port_t port;
    uint32           active;
} ski_lacp_portAct_t;

typedef struct ski_lacp_portTimeout_s
{
    sys_logic_port_t   port;
    sys_lacp_timeout_t timeout;
} ski_lacp_portTimeout_t;

typedef struct ski_lacp_portCounter_s
{
    sys_logic_port_t   port;
    sys_lacp_counter_t counter;
} ski_lacp_portCounter_t;

typedef union ski_lacp_ioctl_u
{
    sys_enable_t           enable;
    uint32                 system_priority;
    ski_lacp_group_t       lacp_group;
    ski_lacp_port_state_t  portState;
    ski_lacp_portPri_t     portPri;
    ski_lacp_portAct_t     portActive;
    ski_lacp_portTimeout_t portTimeout;
    ski_lacp_portCounter_t portCounter;
}ski_lacp_ioctl_t;

extern int32 ski_lacp_init(void);

#endif  

