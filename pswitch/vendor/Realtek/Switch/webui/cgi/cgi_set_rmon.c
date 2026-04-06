
#include <include/cgi.h>

#include <libsal/sal_mib.h>

static int32 cgi_rmon_statisticsClear_set(void);
static int32 cgi_rmon_historyAdd_set(void);
static int32 cgi_rmon_historyEdit_set(void);
static int32 cgi_rmon_historyDel_set(void);
static int32 cgi_rmon_eventAdd_set(void);
static int32 cgi_rmon_eventEdit_set(void);
static int32 cgi_rmon_eventDel_set(void);
static int32 cgi_rmon_alarmAdd_set(void);
static int32 cgi_rmon_alarmEdit_set(void);
static int32 cgi_rmon_alarmDel_set(void);

const cgi_cmdmap_t cmdSetRmonTbl[] =
{
    { CGI_CMD_RMON_STATS_CLEAR,     cgi_rmon_statisticsClear_set    },
    { CGI_CMD_RMON_HISTORY_ADD,     cgi_rmon_historyAdd_set         },
    { CGI_CMD_RMON_HISTORY_EDIT,    cgi_rmon_historyEdit_set        },
    { CGI_CMD_RMON_HISTORY_DEL,     cgi_rmon_historyDel_set         },
    { CGI_CMD_RMON_EVENT_ADD,       cgi_rmon_eventAdd_set           },
    { CGI_CMD_RMON_EVENT_EDIT,      cgi_rmon_eventEdit_set          },
    { CGI_CMD_RMON_EVENT_DEL,       cgi_rmon_eventDel_set           },
    { CGI_CMD_RMON_ALARM_ADD,       cgi_rmon_alarmAdd_set           },
    { CGI_CMD_RMON_ALARM_EDIT,      cgi_rmon_alarmEdit_set          },
    { CGI_CMD_RMON_ALARM_DEL,       cgi_rmon_alarmDel_set           },
    {NULL, NULL}
};

static int32 cgi_rmon_statisticsClear_set(void)
{
    int32 ret = SYS_ERR_OK;
    char *pStrValue = NULL;

    sys_logic_port_t    lp;
    sys_logic_portmask_t lpm;

    SYS_MEM_CLEAR(lpm);

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, pStrValue))
        STR2LPM(pStrValue, &lpm);

    if (IS_LOGIC_PORTMASK_CLEAR(lpm))
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
        CGI_SET_PORT_ERR_CONTINUE(sal_mib_cntr_clr(lp), lp);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_historyAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i;

    char *pStrValue = NULL;

    sys_logic_port_t lp = 0;
    sal_rmon_history_t his_new;
    sal_rmon_history_t his_tmp;

    SYS_MEM_CLEAR(his_new);
    SYS_MEM_CLEAR(his_tmp);

    if (CGI_IS_PARAM_EXIST(CGI_PORT, pStrValue))
        STR2LP(pStrValue, &lp);

    his_new.arrayIndex = CAPA_RMON_HISTORY_NUM;
    his_new.datasource = lp;

    if (CGI_IS_PARAM_EXIST(CGI_RMON_INDEX, pStrValue))
        his_new.historyIndex = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_REQUEST_BUCKETS, pStrValue))
        his_new.bucketRequested = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_INTERVAL, pStrValue))
        his_new.interval = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_OWNER, pStrValue))
        strlcpy(his_new.owner, pStrValue, sizeof(his_new.owner));

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        SYS_MEM_CLEAR(his_tmp);

        his_tmp.arrayIndex = i;

        CGI_SET_ERR_HDL(sal_rmon_history_get(&his_tmp), ret);

        if (his_tmp.historyIndex == his_new.historyIndex)
        {
            his_new.arrayIndex = i;
            break;
        }

        if ((his_new.arrayIndex == CAPA_RMON_HISTORY_NUM) && (0 == his_tmp.historyIndex))
            his_new.arrayIndex = i;
    }

    if (his_new.arrayIndex == CAPA_RMON_HISTORY_NUM)
    {
        ret = CGI_ERR_RMON_HISTORY_TBL_FULL;
        goto cgi_end;
    }

    CGI_SET_ERR_HDL(sal_rmon_history_set(&his_new), ret);
    CGI_SET_ERR_HDL(sal_rmon_history_apply(&his_new), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_historyEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    int32 found = FALSE;
    uint32 index = 0;
    uint32 i;

    char *pStrValue = NULL;

    sys_logic_port_t lp = 0;
    sal_rmon_history_t his;

    if (CGI_IS_PARAM_EXIST(CGI_RMON_INDEX, pStrValue))
        index = SYS_STR2UINT(pStrValue);
    else
        goto cgi_end;

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        SYS_MEM_CLEAR(his);

        his.arrayIndex = i;

        sal_rmon_history_get(&his);

        if (index == his.historyIndex)
        {
            found = TRUE;
            break;
        }
    }

    if (found)
    {
        if (CGI_IS_PARAM_EXIST(CGI_PORT, pStrValue))
            STR2LP(pStrValue, &lp);

        his.datasource = lp;

        if (CGI_IS_PARAM_EXIST(CGI_RMON_INDEX, pStrValue))
            his.historyIndex = SYS_STR2UINT(pStrValue);

        if (CGI_IS_PARAM_EXIST(CGI_RMON_REQUEST_BUCKETS, pStrValue))
            his.bucketRequested = SYS_STR2UINT(pStrValue);

        if (CGI_IS_PARAM_EXIST(CGI_RMON_INTERVAL, pStrValue))
            his.interval = SYS_STR2UINT(pStrValue);

        if (CGI_IS_PARAM_EXIST(CGI_RMON_OWNER, pStrValue))
            strlcpy(his.owner, pStrValue, sizeof(his.owner));
        else
            strlcpy(his.owner, "", sizeof(his.owner));

        CGI_SET_ERR_HDL(sal_rmon_history_set(&his), ret);
        CGI_SET_ERR_HDL(sal_rmon_history_apply(&his), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_historyDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 index = 0;
    uint32 i = 0;
    uint32 j = 0;
    char **array = NULL;

    sal_rmon_history_t his;

    if (!CGI_GET_PARAM_ARRAY_DATA(CGI_RMON_ENTRY, array, &len))
        goto cgi_end;

    for (i = 0; i < len; i++)
    {
        index = SYS_STR2UINT(array[i]);

        SYS_MEM_CLEAR(his);

        CGI_SET_ERR_HDL(sal_rmon_historyByIndex_get(&his, index), ret);
        CGI_SET_ERR_HDL(sal_rmon_history_unapply(&his), ret);

        j = his.arrayIndex;

        SYS_MEM_CLEAR(his);

        his.arrayIndex = j;

        CGI_SET_ERR_HDL(sal_rmon_history_set(&his), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_eventAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i;

    char *pStrValue = NULL;

    sal_rmon_event_t event_new;
    sal_rmon_event_t event_tmp;

    SYS_MEM_CLEAR(event_new);
    SYS_MEM_CLEAR(event_tmp);

    event_new.arrayIndex = CAPA_RMON_EVENT_NUM;

    if (CGI_IS_PARAM_EXIST(CGI_RMON_INDEX, pStrValue))
        event_new.eventIndex = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_COMMUNITY, pStrValue))
        osal_strncpy(event_new.community, pStrValue, sizeof(event_new.community));

    if (CGI_IS_PARAM_EXIST(CGI_RMON_DESC, pStrValue))
        osal_strncpy(event_new.description, pStrValue, sizeof(event_new.description));

    if (CGI_IS_PARAM_EXIST(CGI_RMON_OWNER, pStrValue))
        osal_strncpy(event_new.owner, pStrValue, sizeof(event_new.owner));

    if (CGI_IS_PARAM_EXIST(CGI_RMON_NOTIFICATION_TYPE, pStrValue))
    {
        switch (SYS_STR2UINT(pStrValue))
        {
            case 1:
                event_new.eventType = RMON_EVENT_LOG;
                break;

            case 2:
                event_new.eventType = RMON_EVENT_TRAP;
                break;

            case 3:
                event_new.eventType = RMON_EVENT_LOG_AND_TRAP;
                break;

            default:
                event_new.eventType = RMON_EVENT_NONE;
                break;
        }
    }

    if (RMON_EVENT_TRAP <= event_new.eventType)
    {
        if (!strlen(event_new.community))
            return SYS_ERR_SNMP_COMMUNITY_NAME;
    }

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        SYS_MEM_CLEAR(event_tmp);

        event_tmp.arrayIndex = i;

        CGI_SET_ERR_HDL(sal_rmon_event_get(&event_tmp), ret);

        if (event_tmp.eventIndex == event_new.eventIndex)
        {
            event_new.arrayIndex = i;
            break;
        }

        if ((event_new.arrayIndex == CAPA_RMON_EVENT_NUM) && (0 == event_tmp.eventIndex))
            event_new.arrayIndex = i;
    }

    if (event_new.arrayIndex == CAPA_RMON_EVENT_NUM)
    {
        ret = CGI_ERR_RMON_EVENT_TBL_FULL;
        goto cgi_end;
    }

    CGI_SET_ERR_HDL(sal_rmon_event_set(&event_new), ret);
    CGI_SET_ERR_HDL(sal_rmon_event_apply(&event_new), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_eventEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    int32 found = FALSE;
    uint32 index = 0;
    uint32 i;

    char *pStrValue = NULL;

    sal_rmon_event_t event;

    if (CGI_IS_PARAM_EXIST(CGI_RMON_INDEX, pStrValue))
        index = SYS_STR2UINT(pStrValue);
    else
        goto cgi_end;

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        SYS_MEM_CLEAR(event);

        event.arrayIndex = i;

        sal_rmon_event_get(&event);

        if (index == event.eventIndex)
        {
            found = TRUE;
            break;
        }
    }

    if (found)
    {
        SYS_MEM_CLEAR(event);

        event.arrayIndex = i;
        event.eventIndex = index;

        if (CGI_IS_PARAM_EXIST(CGI_RMON_COMMUNITY, pStrValue))
            osal_strncpy(event.community, pStrValue, sizeof(event.community));

        if (CGI_IS_PARAM_EXIST(CGI_RMON_DESC, pStrValue))
            osal_strncpy(event.description, pStrValue, sizeof(event.description));

        if (CGI_IS_PARAM_EXIST(CGI_RMON_OWNER, pStrValue))
            osal_strncpy(event.owner, pStrValue, sizeof(event.owner));

        if (CGI_IS_PARAM_EXIST(CGI_RMON_NOTIFICATION_TYPE, pStrValue))
        {
            switch (SYS_STR2UINT(pStrValue))
            {
                case 1:
                    event.eventType = RMON_EVENT_LOG;
                    break;

                case 2:
                    event.eventType = RMON_EVENT_TRAP;
                    break;

                case 3:
                    event.eventType = RMON_EVENT_LOG_AND_TRAP;
                    break;

                default:
                    event.eventType = RMON_EVENT_NONE;
                    break;
            }
        }

        if (RMON_EVENT_TRAP <= event.eventType)
        {
            if (!strlen(event.community))
                return SYS_ERR_SNMP_COMMUNITY_NAME;
        }

        CGI_SET_ERR_HDL(sal_rmon_event_set(&event), ret);
        CGI_SET_ERR_HDL(sal_rmon_event_apply(&event), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_eventDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 index = 0;
    uint32 i = 0;
    uint32 j = 0;
    char **array;

    sal_rmon_event_t event;

    if (!CGI_GET_PARAM_ARRAY_DATA(CGI_RMON_ENTRY, array, &len))
        goto cgi_end;

    for (i = 0; i < len; i++)
    {
        index = SYS_STR2UINT(array[i]);

        SYS_MEM_CLEAR(event);

        CGI_SET_ERR_HDL(sal_rmon_eventByIndex_get(&event, index), ret);
        CGI_SET_ERR_HDL(sal_rmon_event_unapply(&event), ret);

        j = event.arrayIndex;

        SYS_MEM_CLEAR(event);

        event.arrayIndex = j;

        CGI_SET_ERR_HDL(sal_rmon_event_set(&event), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_alarmAdd_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 i;

    char *pStrValue = NULL;

    sys_logic_port_t lp = 0;
    sal_rmon_alarm_t alarm_new;
    sal_rmon_alarm_t alarm_tmp;

    SYS_MEM_CLEAR(alarm_new);
    SYS_MEM_CLEAR(alarm_tmp);

    if (CGI_IS_PARAM_EXIST(CGI_PORT, pStrValue))
        STR2LP(pStrValue, &lp);

    alarm_new.arrayIndex = CAPA_RMON_ALARM_NUM;
    alarm_new.samplePort = lp;

    if (CGI_IS_PARAM_EXIST(CGI_RMON_INDEX, pStrValue))
        alarm_new.alarmIndex = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_CNT_NAME, pStrValue))
    {
        switch(SYS_STR2UINT(pStrValue))
        {
            case 0:
                alarm_new.variable = RMON_SAMPLE_DROPEVENTS;
                break;

            case 1:
                alarm_new.variable = RMON_SAMPLE_OCTETS;
                break;

            case 2:
                alarm_new.variable = RMON_SAMPLE_PKTS;
                break;

            case 3:
                alarm_new.variable = RMON_SAMPLE_BROADCASTPKTS;
                break;

            case 4:
                alarm_new.variable = RMON_SAMPLE_MULTICASTPKTS;
                break;

            case 5:
                alarm_new.variable = RMON_SAMPLE_CRCALIGNERRORS;
                break;

            case 6:
                alarm_new.variable = RMON_SAMPLE_UNDERSIZEPKTS;
                break;

            case 7:
                alarm_new.variable = RMON_SAMPLE_OVERSIZEPKTS;
                break;

            case 8:
                alarm_new.variable = RMON_SAMPLE_FRAGMENTS;
                break;

            case 9:
                alarm_new.variable = RMON_SAMPLE_JABBERS;
                break;

            case 10:
                alarm_new.variable = RMON_SAMPLE_COLLISIONS;
                break;

            case 11:
                alarm_new.variable = RMON_SAMPLE_PKTS64OCTETS;
                break;

            case 12:
                alarm_new.variable = RMON_SAMPLE_PKTS65TO127OCTETS;
                break;

            case 13:
                alarm_new.variable = RMON_SAMPLE_PKTS128TO255OCTETS;
                break;

            case 14:
                alarm_new.variable = RMON_SAMPLE_PKTS256TO511OCTETS;
                break;

            case 15:
                alarm_new.variable = RMON_SAMPLE_PKTS512TO1023OCTETS;
                break;

            case 16:
                alarm_new.variable = RMON_SAMPLE_PKTS1024TO1518OCTETS;
                break;

            default:
                alarm_new.variable = RMON_SAMPLE_DROPEVENTS;
                break;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_RMON_SAMPLE_TYPE, pStrValue))
        alarm_new.sampleType = SYS_STR2UINT(pStrValue) ?
                            RMON_ALARM_SAMPLE_DELTA : RMON_ALARM_SAMPLE_ABSOLUTE;

    alarm_new.risingThreshold = 100;
    if (CGI_IS_PARAM_EXIST(CGI_RMON_RISING_THRESHOLD, pStrValue))
        alarm_new.risingThreshold = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_RISING_EVENT, pStrValue))
        alarm_new.risingEventIndex = SYS_STR2UINT(pStrValue);

    alarm_new.fallingThreshold = 20;
    if (CGI_IS_PARAM_EXIST(CGI_RMON_FALLING_THRESHOLD, pStrValue))
        alarm_new.fallingThreshold = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_FALLING_EVENT, pStrValue))
        alarm_new.fallingEventIndex = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_STARTUP_ALARM, pStrValue))
    {
        switch (SYS_STR2UINT(pStrValue))
        {
            case 0:
                alarm_new.alarmType = RMON_ALARM_RISING;
                break;

            case 1:
                alarm_new.alarmType = RMON_ALARM_FALLING;
                break;

            case 2:
                alarm_new.alarmType = RMON_ALARM_FALLING_OR_RISING;
                break;

            default:
                alarm_new.alarmType = RMON_ALARM_RISING;
                break;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_RMON_INTERVAL, pStrValue))
        alarm_new.interval = SYS_STR2UINT(pStrValue);

    if (CGI_IS_PARAM_EXIST(CGI_RMON_OWNER, pStrValue))
        osal_strncpy(alarm_new.owner, pStrValue, sizeof(alarm_new.owner));

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        SYS_MEM_CLEAR(alarm_tmp);

        alarm_tmp.arrayIndex = i;

        CGI_SET_ERR_HDL(sal_rmon_alarm_get(&alarm_tmp), ret);

        if (alarm_tmp.alarmIndex == alarm_new.alarmIndex)
        {
            alarm_new.arrayIndex = i;
            break;
        }

        if ((alarm_new.arrayIndex == CAPA_RMON_ALARM_NUM) && (0 == alarm_tmp.alarmIndex))
            alarm_new.arrayIndex = i;
    }

    if (alarm_new.arrayIndex == CAPA_RMON_ALARM_NUM)
    {
        ret = CGI_ERR_RMON_ALARM_TBL_FULL;
        goto cgi_end;
    }

    CGI_SET_ERR_HDL(sal_rmon_alarm_set(&alarm_new), ret);
    CGI_SET_ERR_HDL(sal_rmon_alarm_apply(&alarm_new), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_alarmEdit_set(void)
{
    int32 ret = SYS_ERR_OK;
    int32 found = FALSE;
    uint32 index = 0;
    uint32 i;

    char *pStrValue = NULL;

    sys_logic_port_t lp = 0;
    sal_rmon_alarm_t alarm;

    if (CGI_IS_PARAM_EXIST(CGI_RMON_INDEX, pStrValue))
        index = SYS_STR2UINT(pStrValue);
    else
        goto cgi_end;

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        SYS_MEM_CLEAR(alarm);

        alarm.arrayIndex = i;

        sal_rmon_alarm_get(&alarm);

        if (index == alarm.alarmIndex)
        {
            found = TRUE;
            break;
        }
    }

    if (found)
    {
        SYS_MEM_CLEAR(alarm);

        alarm.arrayIndex = i;
        alarm.alarmIndex = index;

        if (CGI_IS_PARAM_EXIST(CGI_PORT, pStrValue))
            STR2LP(pStrValue, &lp);

        alarm.samplePort = lp;

        if (CGI_IS_PARAM_EXIST(CGI_RMON_CNT_NAME, pStrValue))
        {
            switch(SYS_STR2UINT(pStrValue))
            {
                case 0:
                    alarm.variable = RMON_SAMPLE_DROPEVENTS;
                    break;

                case 1:
                    alarm.variable = RMON_SAMPLE_OCTETS;
                    break;

                case 2:
                    alarm.variable = RMON_SAMPLE_PKTS;
                    break;

                case 3:
                    alarm.variable = RMON_SAMPLE_BROADCASTPKTS;
                    break;

                case 4:
                    alarm.variable = RMON_SAMPLE_MULTICASTPKTS;
                    break;

                case 5:
                    alarm.variable = RMON_SAMPLE_CRCALIGNERRORS;
                    break;

                case 6:
                    alarm.variable = RMON_SAMPLE_UNDERSIZEPKTS;
                    break;

                case 7:
                    alarm.variable = RMON_SAMPLE_OVERSIZEPKTS;
                    break;

                case 8:
                    alarm.variable = RMON_SAMPLE_FRAGMENTS;
                    break;

                case 9:
                    alarm.variable = RMON_SAMPLE_JABBERS;
                    break;

                case 10:
                    alarm.variable = RMON_SAMPLE_COLLISIONS;
                    break;

                case 11:
                    alarm.variable = RMON_SAMPLE_PKTS64OCTETS;
                    break;

                case 12:
                    alarm.variable = RMON_SAMPLE_PKTS65TO127OCTETS;
                    break;

                case 13:
                    alarm.variable = RMON_SAMPLE_PKTS128TO255OCTETS;
                    break;

                case 14:
                    alarm.variable = RMON_SAMPLE_PKTS256TO511OCTETS;
                    break;

                case 15:
                    alarm.variable = RMON_SAMPLE_PKTS512TO1023OCTETS;
                    break;

                case 16:
                    alarm.variable = RMON_SAMPLE_PKTS1024TO1518OCTETS;
                    break;

                default:
                    alarm.variable = RMON_SAMPLE_DROPEVENTS;
                    break;
            }
        }

        if (CGI_IS_PARAM_EXIST(CGI_RMON_SAMPLE_TYPE, pStrValue))
            alarm.sampleType = SYS_STR2UINT(pStrValue) ?
                                RMON_ALARM_SAMPLE_DELTA : RMON_ALARM_SAMPLE_ABSOLUTE;

        alarm.risingThreshold = 100;
        if (CGI_IS_PARAM_EXIST(CGI_RMON_RISING_THRESHOLD, pStrValue))
            alarm.risingThreshold = SYS_STR2UINT(pStrValue);

        if (CGI_IS_PARAM_EXIST(CGI_RMON_RISING_EVENT, pStrValue))
            alarm.risingEventIndex = SYS_STR2UINT(pStrValue);

        alarm.fallingThreshold = 20;
        if (CGI_IS_PARAM_EXIST(CGI_RMON_FALLING_THRESHOLD, pStrValue))
            alarm.fallingThreshold = SYS_STR2UINT(pStrValue);

        if (CGI_IS_PARAM_EXIST(CGI_RMON_FALLING_EVENT, pStrValue))
            alarm.fallingEventIndex = SYS_STR2UINT(pStrValue);

        if (CGI_IS_PARAM_EXIST(CGI_RMON_STARTUP_ALARM, pStrValue))
        {
            switch (SYS_STR2UINT(pStrValue))
            {
                case 0:
                    alarm.alarmType = RMON_ALARM_RISING;
                    break;

                case 1:
                    alarm.alarmType = RMON_ALARM_FALLING;
                    break;

                case 2:
                    alarm.alarmType = RMON_ALARM_FALLING_OR_RISING;
                    break;

                default:
                    alarm.alarmType = RMON_ALARM_RISING;
                    break;
            }
        }

        if (CGI_IS_PARAM_EXIST(CGI_RMON_INTERVAL, pStrValue))
            alarm.interval = SYS_STR2UINT(pStrValue);

        if (CGI_IS_PARAM_EXIST(CGI_RMON_OWNER, pStrValue))
            osal_strncpy(alarm.owner, pStrValue, sizeof(alarm.owner));

        CGI_SET_ERR_HDL(sal_rmon_alarm_set(&alarm), ret);
        CGI_SET_ERR_HDL(sal_rmon_alarm_apply(&alarm), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_rmon_alarmDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 index = 0;
    uint32 i = 0;
    uint32 j = 0;
    char **array = NULL;

    sal_rmon_alarm_t alarm;

    if (!CGI_GET_PARAM_ARRAY_DATA(CGI_RMON_ENTRY, array, &len))
        goto cgi_end;

    for (i = 0; i < len; i++)
    {
        index = SYS_STR2UINT(array[i]);

        SYS_MEM_CLEAR(alarm);

        CGI_SET_ERR_HDL(sal_rmon_alarmByIndex_get(&alarm, index), ret);
        CGI_SET_ERR_HDL(sal_rmon_alarm_unapply(&alarm), ret);

        j = alarm.arrayIndex;

        SYS_MEM_CLEAR(alarm);

        alarm.arrayIndex = j;

        CGI_SET_ERR_HDL(sal_rmon_alarm_set(&alarm), ret);
    }

cgi_end:
    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}
