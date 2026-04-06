
#ifndef __SKI_OPTICAL_H__
#define __SKI_OPTICAL_H__

#include <ski/ski_core.h>
#include <rsd/rsd_optical.h>

#define MAX_WORD_NO_LONG_INFO 4

typedef enum ski_optical_cmd_e
{
    
    SKI_OPTICAL_SET_BASE = (SKI_CORE_MOD_OPTICAL * SKI_CORE_MODCMD_TOTAL),
    SKI_OPTICAL_INIT,

    SKI_OPTICAL_GET_BASE = (SKI_OPTICAL_SET_BASE + SKI_CORE_MODCMD_RANGE),

    SKI_OPTICAL_DATA_GET,

    SKI_OPTICAL_CMD_END
} ski_optical_cmd_t;

typedef union ski_optical_ioctl_u
{
    struct
    {
        sys_logic_port_t    port;
        sys_optical_cmd_t   cmd;
        sys_optical_data_t  data;
    } optical;
} ski_optical_ioctl_t;

extern int32 ski_optical_init(void);

#endif  

