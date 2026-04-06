
#ifndef __SKI_DBG_H__
#define __SKI_DBG_H__

#include <ski/ski_core.h>
#include <rsd/rsd_debug.h>

typedef enum ski_dbg_cmd_e
{
    
    SKI_DBG_DUMP_BASE = (SKI_CORE_MOD_DBG * SKI_CORE_MODCMD_TOTAL),
    SKI_DBG_DUMP_SOC_REG,
    SKI_DBG_DUMP_MAC_REG,
    SKI_DBG_DUMP_MAC_TABLE,
    SKI_DBG_DUMP_PHY_REG,

    SKI_DBG_CMD_END
} ski_dbg_cmd_t;

extern int32 ski_debug_init(void);

#endif  

