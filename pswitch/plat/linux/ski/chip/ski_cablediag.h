
#ifndef __SKI_CABLEDIAG_H__
#define __SKI_CABLEDIAG_H__

#include <ski/ski_core.h>
#include <rsd/rsd_cablediag.h>

typedef enum ski_cablediag_cmd_e
{
    
    SKI_CABLEDIAG_SET_BASE = (SKI_CORE_MOD_CABLE_DIAG * SKI_CORE_MODCMD_TOTAL),
    SKI_CABLEDIAG_COPPER_SET,

    SKI_CABLEDIAG_GET_BASE = (SKI_CABLEDIAG_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_CABLEDIAG_COPPER_GET,
    
    SKI_CABLEDIAG_CMD_END
} ski_cablediag_cmd_t;

typedef struct ski_cablediag_copper_s
{
    sys_rtctResult_t    rtctResult;
    sys_logic_port_t    port;
} ski_cablediag_copper_t;

typedef union ski_cabledaig_ioctl_u
{
    ski_cablediag_copper_t  copperDiag;
} ski_cablediag_ioctl_t;

extern int32 ski_cablediag_init(void);

#endif  

