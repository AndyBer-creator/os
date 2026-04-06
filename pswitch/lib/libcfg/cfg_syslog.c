
#include <libcfg/cfg.h>

int32 cfg_log_enable_set(sys_enable_t enable)
{
    cfg_log_t *log = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    log->enable = enable;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_enable_get(sys_enable_t *pEnable)
{
    cfg_log_t  *log = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    *pEnable = log->enable;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_target_set(log_target_t index, sal_log_target_t *pTarget)
{
    cfg_log_t *log = NULL;

    SYS_PARAM_CHK((NULL == pTarget), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    osal_memcpy(&(log->target[index]), pTarget, sizeof(sal_log_target_t));

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_target_get(log_target_t index, sal_log_target_t *pTarget)
{
    cfg_log_t  *log = NULL;

    SYS_PARAM_CHK((NULL == pTarget), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    osal_memcpy(pTarget, &(log->target[index]), sizeof(sal_log_target_t));

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_dbgCatMask_set(dbg_cat_t cat, sys_enable_t enable, sys_dbg_extmask_t extmask)
{
    cfg_log_t *log = NULL;

    SYS_PARAM_CHK(DBG_CAT_ALL <= cat, SYS_ERR_OK);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    log->dbg_cat[cat].enable = enable;
    log->dbg_cat[cat].extmask = extmask;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_dbgCatMask_get(dbg_cat_t cat, sys_enable_t *pEnable, sys_dbg_extmask_t *pExtMask)
{
    cfg_log_t *log = NULL;

    SYS_PARAM_CHK(DBG_CAT_ALL <= cat, SYS_ERR_OK);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pExtMask), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    *pEnable = log->dbg_cat[cat].enable;
    *pExtMask = log->dbg_cat[cat].extmask;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_autoRestart_set(sys_enable_t autoRestart)
{
    cfg_log_t *log = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(autoRestart), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    log->autoRestart = autoRestart;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_autoRestart_get(sys_enable_t *autoRestart)
{
    cfg_log_t *log = NULL;

    SYS_PARAM_CHK((NULL == autoRestart), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    *autoRestart = log->autoRestart;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_ratelimitEnable_set(sys_enable_t enable)
{
    cfg_log_t *log = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    log->ratelimit = enable;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_ratelimitEnable_get(sys_enable_t *pEnable)
{
    cfg_log_t *log = NULL;

    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    *pEnable = log->ratelimit;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_ratelimitInterval_set(uint32 interval)
{
    cfg_log_t *log = NULL;

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    log->interval = interval;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_ratelimitInterval_get(uint32 *pInterval)
{
    cfg_log_t *log = NULL;

    SYS_PARAM_CHK(NULL == pInterval, SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_LOG, (cfg_addr)&log);

    *pInterval = log->interval;

    CFG_CLOSE(CFG_ID_LOG);
}

int32 cfg_log_init(void)
{
    cfg_log_autoRestart_set(ENABLED);

    return SYS_ERR_OK;
}
