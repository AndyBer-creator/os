
#include <libcfg/cfg.h>

int32 cfg_rmon_event_set(sal_rmon_event_t *pEvent)
{
    cfg_rmon_t *pRmon = NULL;

    SYS_PARAM_CHK((NULL == pEvent), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pEvent->arrayIndex > CAPA_RMON_EVENT_NUM), SYS_ERR_RANGE);

    CFG_OPEN(CFG_ID_RMON, (cfg_addr)&pRmon);

    osal_memcpy(&pRmon->rmonEvents[pEvent->arrayIndex], pEvent,  sizeof(sal_rmon_event_t));

    CFG_CLOSE(CFG_ID_RMON);
}

int32 cfg_rmon_event_get(sal_rmon_event_t *pEvent)
{
    cfg_rmon_t *pRmon = NULL;

    SYS_PARAM_CHK((NULL == pEvent), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pEvent->arrayIndex > CAPA_RMON_EVENT_NUM), SYS_ERR_RANGE);

    CFG_OPEN(CFG_ID_RMON, (cfg_addr)&pRmon);

    osal_memcpy(pEvent, &pRmon->rmonEvents[pEvent->arrayIndex], sizeof(sal_rmon_event_t));

    CFG_CLOSE(CFG_ID_RMON);
}

int32 cfg_rmon_alarm_set(sal_rmon_alarm_t *pAlarm)
{
    cfg_rmon_t *pRmon = NULL;

    SYS_PARAM_CHK((NULL == pAlarm), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pAlarm->arrayIndex > CAPA_RMON_ALARM_NUM), SYS_ERR_RANGE);

    CFG_OPEN(CFG_ID_RMON, (cfg_addr)&pRmon);

    osal_memcpy(&pRmon->rmonAlarms[pAlarm->arrayIndex], pAlarm,  sizeof(sal_rmon_alarm_t));

    CFG_CLOSE(CFG_ID_RMON);
}

int32 cfg_rmon_alarm_get(sal_rmon_alarm_t *pAlarm)
{
    cfg_rmon_t *pRmon = NULL;

    SYS_PARAM_CHK((NULL == pAlarm), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pAlarm->arrayIndex > CAPA_RMON_EVENT_NUM), SYS_ERR_RANGE);

    CFG_OPEN(CFG_ID_RMON, (cfg_addr)&pRmon);

    osal_memcpy(pAlarm, &pRmon->rmonAlarms[pAlarm->arrayIndex], sizeof(sal_rmon_alarm_t));

    CFG_CLOSE(CFG_ID_RMON);
}

int32 cfg_rmon_history_set(sal_rmon_history_t *pHistory)
{
    cfg_rmon_t *pRmon = NULL;

    SYS_PARAM_CHK((NULL == pHistory), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pHistory->arrayIndex > CAPA_RMON_HISTORY_NUM), SYS_ERR_RANGE);

    CFG_OPEN(CFG_ID_RMON, (cfg_addr)&pRmon);

    osal_memcpy(&pRmon->rmonHistories[pHistory->arrayIndex], pHistory,  sizeof(sal_rmon_history_t));

    CFG_CLOSE(CFG_ID_RMON);
}

int32 cfg_rmon_history_get(sal_rmon_history_t *pHistory)
{
    cfg_rmon_t *pRmon = NULL;

    SYS_PARAM_CHK((NULL == pHistory), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pHistory->arrayIndex > CAPA_RMON_HISTORY_NUM), SYS_ERR_RANGE);

    CFG_OPEN(CFG_ID_RMON, (cfg_addr)&pRmon);

    osal_memcpy(pHistory, &pRmon->rmonHistories[pHistory->arrayIndex], sizeof(sal_rmon_history_t));

    CFG_CLOSE(CFG_ID_RMON);
}
