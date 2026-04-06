
#ifndef __SKI_LOG_H__
#define __SKI_LOG_H__

#include <ski/ski_core.h>

typedef enum ski_log_cmd_e
{
    
    SKI_LOG_SET_BASE = (SKI_CORE_MOD_LOG * SKI_CORE_MODCMD_TOTAL),
    SKI_LOG_SET_RESTART,
    SKI_LOG_SET_DBG_ENABLE,

    SKI_LOG_GET_BASE = (SKI_LOG_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_LOG_GET_DBG_ENABLE,

    SKI_LOG_CMD_END
} ski_log_cmd_t;

typedef struct ski_log_mask_s
{
    struct {
        int32 dbg;
        sys_enable_t enable;
    } dbg;
} ski_log_mask_t;

extern int32 ski_log_init(void);

#endif  

