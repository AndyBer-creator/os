
#ifndef __CFG_LOG_H__
#define __CFG_LOG_H__

#include <libsal/sal_syslog.h>

typedef struct cfg_log_s
{
    sal_log_target_t target[LOG_TARGET_MAX] ;
    sys_enable_t autoRestart;
    sys_enable_t enable;
    
    sys_enable_t ratelimit;
    uint32 interval;
    
    struct
    {
        sys_enable_t    enable;
        sys_dbg_extmask_t  extmask;
    } dbg_cat[DBG_CAT_ALL];
} cfg_log_t;

extern int32 cfg_log_enable_set(sys_enable_t enable);

extern int32 cfg_log_enable_get(sys_enable_t *pEnable);

extern int32 cfg_log_target_set(log_target_t index, sal_log_target_t *pTarget);

extern int32 cfg_log_target_get(log_target_t index, sal_log_target_t *pTarget);

extern int32 cfg_log_dbgCatMask_set(dbg_cat_t cat, sys_enable_t enable, sys_dbg_extmask_t submask);

extern int32 cfg_log_dbgCatMask_get(dbg_cat_t cat, sys_enable_t *pEnable, sys_dbg_extmask_t *pSubMask);

extern int32 cfg_log_autoRestart_set(sys_enable_t autoRestart);

extern int32 cfg_log_autoRestart_get(sys_enable_t *autoRestart);

extern int32 cfg_log_ratelimitEnable_set(sys_enable_t enable);

extern int32 cfg_log_ratelimitEnable_get(sys_enable_t *pEnable);

extern int32 cfg_log_ratelimitInterval_set(uint32 interval);

extern int32 cfg_log_ratelimitInterval_get(uint32 *pInterval);

extern int32 cfg_log_init(void);
#endif
