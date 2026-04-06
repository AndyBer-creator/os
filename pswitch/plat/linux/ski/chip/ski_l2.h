
#ifndef __SKI_L2_H__
#define __SKI_L2_H__

#include <ski/ski_core.h>
#include <rsd/rsd_l2.h>

typedef enum ski_l2_cmd_e
{
    
    SKI_L2_SET_BASE = (SKI_CORE_MOD_L2 * SKI_CORE_MODCMD_TOTAL),
    SKI_L2_SET_DB_GENERATE,
    SKI_L2_SET_AGING_TIME,
    SKI_L2_SET_MAC_ADD,
    SKI_L2_SET_MAC_DEL,
    SKI_L2_SET_MAC_CLEAR,

    SKI_L2_GET_BASE = (SKI_L2_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_L2_GET_AGING_TIME,
    SKI_L2_GET_MAC_SEARCH,

    SKI_L2_CMD_END
} ski_l2_cmd_t;

typedef union ski_l2_ioctl_u
{
    sys_l2_entry_t    l2Entry;
    sys_l2_clearCfg_t clearCfg;
    uint32 agingTime;
} ski_l2_ioctl_t;

extern int32 ski_l2_init(void);

#endif  

