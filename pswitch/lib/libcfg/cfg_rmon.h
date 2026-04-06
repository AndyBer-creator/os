
#ifndef __CFG_RMON_H__
#define __CFG_RMON_H__

#include <libsal/sal_rmon.h>

typedef struct cfg_rmon_s
{
    sal_rmon_event_t rmonEvents[CAPA_RMON_EVENT_NUM];
    sal_rmon_alarm_t rmonAlarms[CAPA_RMON_ALARM_NUM];
    sal_rmon_history_t rmonHistories[CAPA_RMON_HISTORY_NUM];
} cfg_rmon_t;

extern int32 cfg_rmon_event_set(sal_rmon_event_t *pEvent);

extern int32 cfg_rmon_event_get(sal_rmon_event_t *pEvent);

extern int32 cfg_rmon_alarm_set(sal_rmon_alarm_t *pAlarm);

extern int32 cfg_rmon_alarm_get(sal_rmon_alarm_t *pAlarm);

extern int32 cfg_rmon_history_set(sal_rmon_history_t *pHistory);

extern int32 cfg_rmon_history_get(sal_rmon_history_t *pHistory);
#endif

