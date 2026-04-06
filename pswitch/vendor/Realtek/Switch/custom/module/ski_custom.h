
#ifndef __SKI_CUSTOM_H__
#define __SKI_CUSTOM_H__

#include <ski/ski_core.h>

#define SKI_CORE_CUSTOMCMD_RANGE (64)  

typedef enum ski_custom_cmd_e
{
    SKI_CUSTOM_SET_BASE = (SKI_CORE_MOD_CUSTOM * SKI_CORE_MODCMD_TOTAL),
    SKI_CUSTOM_PORT_ENABLE_SET,
    SKI_CUSTOM_ENABLE_SET,

    SKI_CUSTOM_GET_BASE = (SKI_CUSTOM_SET_BASE + SKI_CORE_CUSTOMCMD_RANGE),
    SKI_CUSTOM_PORT_ENABLE_GET,
    SKI_CUSTOM_ENABLE_GET,

    SKI_CUSTOM_CMD_END
} ski_custom_cmd_t;

typedef struct ski_custom_ioctl_s
{
    sys_logic_port_t port;
    sys_enable_t enable;
} ski_custom_ioctl_t;

#endif

