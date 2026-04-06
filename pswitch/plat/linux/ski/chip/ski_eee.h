
#ifndef __SKI_EEE_H__
#define __SKI_EEE_H__

#include <ski/ski_core.h>
#include <rsd/rsd_eee.h>

typedef enum ski_eee_cmd_e
{
    
    SKI_EEE_SET_BASE = (SKI_CORE_MOD_EEE * SKI_CORE_MODCMD_TOTAL),
    SKI_EEE_SET_PORT_ENABLE,
    SKI_EEE_SET_ENABLE_PORTMASK,

    SKI_EEE_GET_BASE = (SKI_EEE_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_EEE_GET_PORT_ENABLE,
    SKI_EEE_GET_PORT_STATE,
    SKI_EEE_GET_ENABLE_PORTMASK,

    SKI_EEE_CMD_END
} ski_eee_cmd_t;

typedef struct ski_eee_enable_s
{
    sys_logic_port_t port;
    sys_enable_t enable;
} ski_eee_enable_t;

typedef struct ski_eee_enablePortmask_s
{
    sys_enable_t enable;
    sys_logic_portmask_t portmask;
} ski_eee_enablePortmask_t;

typedef union ski_eee_ioctl_u
{
    ski_eee_enable_t         eeeEnable;
    ski_eee_enablePortmask_t eeeEnablePortmask;
} ski_eee_ioctl_t;

extern int32 ski_eee_init(void);

#endif  

