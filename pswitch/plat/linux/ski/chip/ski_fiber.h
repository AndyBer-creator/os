
#ifndef __SKI_FIBER_H__
#define __SKI_FIBER_H__

#include <ski/ski_core.h>
#include <rsd/rsd_fiber.h>

#include <board/fiber/fiber.h>

typedef enum ski_fiber_cmd_e
{
    
    SKI_FIBER_SET_BASE = (SKI_CORE_MOD_FIBER * SKI_CORE_MODCMD_TOTAL),
    SKI_FIBER_INIT,

    SKI_FIBER_GET_BASE = (SKI_FIBER_SET_BASE + SKI_CORE_MODCMD_RANGE),

    SKI_FIBER_OESTATUS_GET,
    SKI_FIBER_LOSTATUS_GET,
    SKI_FIBER_DETECTSTATUS_GET,

    SKI_FIBER_CMD_END
} ski_fiber_cmd_t;

typedef  struct ski_fiber_ioctl_s
{
    sys_logic_port_t        port;
    sys_fiber_detect_status_t                  data;
} ski_fiber_ioctl_t;

extern int32 ski_fiber_init(void);

#endif  

