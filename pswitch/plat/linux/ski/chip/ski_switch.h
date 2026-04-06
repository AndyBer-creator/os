
#ifndef __SKI_SWITCH_H__
#define __SKI_SWITCH_H__

#include <ski/ski_core.h>
#include <rsd/rsd_switch.h>

typedef enum ski_switch_cmd_e
{
    
    SKI_SWITCH_SET_BASE = (SKI_CORE_MOD_SWITCH * SKI_CORE_MODCMD_TOTAL),
    SKI_SWITCH_SET_MAX_FRAME_SIZE,

    SKI_SWITCH_GET_BASE = (SKI_SWITCH_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_SWITCH_GET_MAX_FRAME_SIZE,

    SKI_SWITCH_CMD_END
} ski_switch_cmd_t;

typedef union ski_switch_ioctl_u
{
    uint32 maxFrameSize;
} ski_switch_ioctl_t;

extern int32 ski_switch_init(void);

#endif  

