
#ifndef __SKI_TRAP_H__
#define __SKI_TRAP_H__

#include <ski/ski_core.h>
#include <rsd/rsd_trap.h>

typedef enum ski_trap_cmd_e
{
    
    SKI_TRAP_SET_BASE = (SKI_CORE_MOD_TRAP * SKI_CORE_MODCMD_TOTAL),
    SKI_TRAP_SET_RMA_ACTION,
    SKI_TRAP_RELEASE_RMA_ACTION,
    SKI_TRAP_SET_PORTRMA_ACTION,
    SKI_TRAP_RELEASE_PORTRMA_ACTION,

    SKI_TRAP_GET_BASE = (SKI_TRAP_SET_BASE + SKI_CORE_MODCMD_RANGE),

    SKI_TRAP_CMD_END
} ski_trap_cmd_t;

typedef struct ski_trap_ioctl_u
{
    sys_logic_port_t        port;
    sys_mac_t               mac;
    sys_trap_rma_action_t   action;
    sys_trap_mgmtType_t     frameType;
    sys_callerType_t        callerType;
} ski_trap_ioctl_t;

extern int32 ski_trap_init(void);

#endif  

