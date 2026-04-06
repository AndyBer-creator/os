
#include <libcfg/cfg_rmon.h>
#include <libsal/sal_rmon.h>

#define RMON_SET_COMMAND_COMMUNITY  "jGyJcd033cKS48J6U1WCHhgLC7Vso6N"

#define RMON_DFL_IP    " 127.0.0.1 "
#define RMON_SET_COMMAND    "snmpset -v1 -c "RMON_SET_COMMAND_COMMUNITY RMON_DFL_IP
#define RMON_GET_COMMAND    "snmpget -v1 -c "RMON_SET_COMMAND_COMMUNITY RMON_DFL_IP
#define RMON_WALK_COMMAND   "snmpwalk -v1 -c "RMON_SET_COMMAND_COMMUNITY RMON_DFL_IP

const sys_text_t text_rmon_event_type[] =
{
    {         "Unuse"    },
    {          "None"    },
    {           "Log"     },
    {          "SNMP-Trap"    },
    {  "Log and Trap" },
};

sys_text_t text_rmon_alarm_type[] =
{
    {           "none"    },
    {           "Falling"    },
    {            "Rising"},
    { "Rising or Falling"   },
};

sys_text_t text_rmon_alarm_sample_type[] =
{
    {          "none"    },
    {      "absolute"},
    {         "delta"   },
};

const sys_text_t text_rmon_sample[] =
{
    {           "NOUSE" },
    {           "NOUSE" },
    {           "NOUSE" },
    {       "DropEvents"},
    {           "Octets"},
    {             "Pkts"},
    {    "BroadcastPkts"},
    {    "MulticastPkts"},
    {   "CRCAlignErrors"},
    {    "UnderSizePkts"},
    {     "OverSizePkts"},
    {        "Fragments"},
    {          "Jabbers"},
    {       "Collisions"},
    {     "Pkts64Octets"},
    {    "Pkts65to127Octets"},
    {   "Pkts128to255Octets"},
    {   "Pkts256to511Octets"},
    {  "Pkts512to1023Octets"},
    { "Pkts1024to1518Octets"},
};

const sys_text_t text_rmon_history[] =
{
    {           "NOUSE" },
    {           "NOUSE" },
    {     "Sample Index"},
    {   "Interval Start"},
    {       "DropEvents"},
    {           "Octets"},
    {             "Pkts"},
    {    "BroadcastPkts"},
    {    "MulticastPkts"},
    {   "CRCAlignErrors"},
    {    "UnderSizePkts"},
    {     "OverSizePkts"},
    {        "Fragments"},
    {          "Jabbers"},
    {       "Collisions"},
    {      "Utilization"},
};

const sys_text_t text_rmon_sample_etherOID[] =
{
    {           "NOUSE" },
    {           "NOUSE" },
    {           "NOUSE" },
    {       RMON_ETHER_STATE_OID"3."},
    {           RMON_ETHER_STATE_OID"4."},
    {             RMON_ETHER_STATE_OID"5."},
    {    RMON_ETHER_STATE_OID"6."},
    {    RMON_ETHER_STATE_OID"7."},
    {   RMON_ETHER_STATE_OID"8."},
    {    RMON_ETHER_STATE_OID"9."},
    {     RMON_ETHER_STATE_OID"10."},
    {        RMON_ETHER_STATE_OID"11."},
    {          RMON_ETHER_STATE_OID"12."},
    {       RMON_ETHER_STATE_OID"13."},
    {     RMON_ETHER_STATE_OID"14."},
    {    RMON_ETHER_STATE_OID"15."},
    {   RMON_ETHER_STATE_OID"16."},
    {   RMON_ETHER_STATE_OID"17."},
    {  RMON_ETHER_STATE_OID"18."},
    { RMON_ETHER_STATE_OID"19."},
};

#define RMON_CMD_DBG(str)                   \
do                                          \
{                                           \
    char *p = cmd;                          \
    char *pp = p;                           \
    if (!IS_DBG_SET(DBG_RMON))              \
        break;                              \
    while (p)                               \
    {                                       \
        if (NULL != (pp = strchr(p, '\n'))) \
            *pp = '\0';                     \
        SYS_DBG(DBG_RMON, "cmd %s", p);     \
        if (NULL != pp)                     \
            *pp++ = '\n';                   \
        p = pp;                             \
    }                                       \
} while(0)

static inline void _escapeChar_add(char *in, char*out)
{
    int i = 0;
    int j = 0;
    int len = osal_strlen(in);

    for (i = 0; i < len; i++)
    {

        if (ispunct(in[i]))
            out[j++] = '\\';

        out[j++] = in[i];
    }

    return;
}

char* sal_rmon_cstr_ret(void)
{
    return RMON_SET_COMMAND_COMMUNITY;
}

int32 sal_rmon_event_set(sal_rmon_event_t *pEvent)
{
    SYS_PARAM_CHK((NULL == pEvent), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rmon_event_set(pEvent));

    return SYS_ERR_OK;
}

int32 sal_rmon_event_get(sal_rmon_event_t *pEvent)
{
    SYS_PARAM_CHK((NULL == pEvent), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pEvent->arrayIndex > CAPA_RMON_EVENT_NUM), SYS_ERR_RANGE);

    SYS_ERR_CHK(cfg_rmon_event_get(pEvent));

    return SYS_ERR_OK;
}

int32 sal_rmon_eventByIndex_get(sal_rmon_event_t *pEvent, uint32 eventIndex)
{
    uint32 i = 0;
    sal_rmon_event_t event;

    SYS_PARAM_CHK((NULL == pEvent), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((eventIndex < 1) || (eventIndex > 65535)), SYS_ERR_OUT_OF_RANGE);

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        osal_memset(&event, 0 , sizeof(sal_rmon_event_t));
        event.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_event_get(&event));

        if (eventIndex == event.eventIndex)
        {
            osal_memcpy(pEvent, &event, sizeof(sal_rmon_event_t));

            return SYS_ERR_OK;
        }
    }

    pEvent->eventIndex = 0;

    return SYS_ERR_OK;
}

int32 sal_rmon_eventRuntime_get(sal_rmon_event_t *pEvent)
{
    FILE *fp = NULL;
    char *tmp = NULL;
    char buf[SYS_BUF256_LEN];
    sys_enable_t enable = DISABLED;

    SYS_PARAM_CHK((NULL == pEvent), SYS_ERR_NULL_POINTER);

    if (0 == pEvent->eventIndex)
        return SYS_ERR_FAILED;

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    remove(RMON_COMMAND_TMPFILE);

    osal_memset(buf, 0, sizeof(buf));
    sprintf(buf, RMON_GET_COMMAND RMON_EVENT_LASTTIME_OID "%u " RMON_COMMAND_POSTFIX, pEvent->eventIndex);
    system(buf);
    SYS_DBG(DBG_RMON, "%s", buf);

    fp = fopen(RMON_COMMAND_TMPFILE, "r");

    if (NULL != fp)
    {
        osal_memset(buf, 0, sizeof(buf));
        if (NULL != fgets(buf, sizeof(buf), fp))
        {
            SYS_DBG(DBG_RMON, "Result %s", buf);

            tmp = strchr(buf, ':');

            if (NULL != tmp)
            {
                strcpy(pEvent->eventLastTimeSent, ++tmp);
                pEvent->eventLastTimeSent[strlen(pEvent->eventLastTimeSent) - 1] = '\0';
            }
        }

        pclose(fp);
    }
    else
    {
        SYS_DBG(DBG_RMON, "Cat %s error", RMON_COMMAND_TMPFILE);
    }

    remove(RMON_COMMAND_TMPFILE);

    return SYS_ERR_OK;
}

int32 sal_rmon_eventLog_get(sal_rmon_event_log_t *pEventLog, uint32 eventIndex)
{
    FILE *fp = NULL;
    int32 i = 0;
    uint32 cnt = 0;
    uint32 nowIndex = 0;
    char *tmp = NULL;
    char cmd[SYS_BUF512_LEN];
    char buf[SYS_BUF256_LEN];
    char mibName[SYS_BUF256_LEN];
    sal_rmon_alarm_t alarm;
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    remove(RMON_COMMAND_TMPFILE);

    osal_memset(&alarm, 0, sizeof(alarm));
    osal_memset(buf, 0, sizeof(buf));
    osal_memset(cmd, 0, sizeof(cmd));

    sprintf(cmd, RMON_WALK_COMMAND RMON_EVENT_LOG_INDEX_OID "%u " RMON_COMMAND_POSTFIX, eventIndex);
    system(cmd);

    SYS_DBG(DBG_RMON, "%s", cmd);

    fp = fopen(RMON_COMMAND_TMPFILE, "r");

    if (NULL == fp)
        return SYS_ERR_FAILED;

    while (NULL != fgets(buf, sizeof(buf), fp))
    {
        
        tmp = strstr(buf, ":");

        if (NULL != tmp)
        {
            pEventLog[cnt].index = atoi(++tmp);
            cnt++;

            if(cnt > RMON_EVENT_LOG_DFL)
            {
                cnt = RMON_EVENT_LOG_DFL;
                SYS_DBG(DBG_RMON, "rmon event log should not over %u", RMON_EVENT_LOG_DFL);
                break;
            }
        }

        osal_memset(buf, 0, strlen(buf));
    }

    fclose(fp);
    remove(RMON_COMMAND_TMPFILE);

    SYS_DBG(DBG_RMON, "cnt %u", cnt);

    if (0 == cnt)
        return SYS_ERR_OK;

    osal_memset(cmd, 0, sizeof(cmd));
    i = 0;
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_EVENT_LOG_TIME_OID "%u " RMON_COMMAND_POSTFIX"\n", eventIndex);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_EVENT_LOG_DESC_OID "%u " RMON_COMMAND_POSTFIX"\n", eventIndex);
    system(cmd);
    RMON_CMD_DBG(cmd);

    fp = fopen(RMON_COMMAND_TMPFILE, "r");

    if (NULL == fp)
        return SYS_ERR_FAILED;

    for (i = 0; i < cnt; )
    {
        if (NULL != fgets(buf, sizeof(buf), fp))
        {
            osal_memset(mibName, 0, sizeof(mibName));
            tmp = index(buf, '=');
            memcpy(mibName,buf,tmp-buf);

            tmp = rindex(mibName, '.');
            nowIndex = atoi(++tmp);
            SYS_DBG(DBG_RMON, "start index=%d", nowIndex);

            while(pEventLog[i].index != nowIndex)
            {
                pEventLog[i].index = 0;
                SYS_DBG(DBG_RMON, "delet index=%d because description", pEventLog[i].index);
                i++;
            }

            tmp = strchr(buf, ':');

            if (NULL != tmp)
            {
                osal_strcpy(pEventLog[i].logTime, ++tmp);
                pEventLog[i].logTime[strlen(pEventLog[i].logTime) - 1] = '\0';
                SYS_DBG(DBG_RMON, "%s", pEventLog[i].logTime);
            }
            i++;

            osal_memset(buf, 0, strlen(buf));
        }
        else
        {
            i++;
        }
    }

    for (i = 0; i < cnt; )
    {
        if (NULL != fgets(buf, sizeof(buf), fp))
        {
            
            if(strstr(buf, RMON_EVENT_LOG_DESC_OID))
            {
                osal_memset(mibName, 0, sizeof(mibName));
                tmp = index(buf, '=');
                memcpy(mibName,buf,tmp-buf);

                tmp = rindex(mibName, '.');
                nowIndex = atoi(++tmp);
                SYS_DBG(DBG_RMON, "start index=%d", nowIndex);

                while(pEventLog[i].index != nowIndex)
                {
                    pEventLog[i].index = 0;
                    SYS_DBG(DBG_RMON, "delet index=%d because description", pEventLog[i].index);
                    i++;
                }

                tmp = strchr(buf, '"');

                if (NULL != tmp)
                    sprintf(pEventLog[i].description, "%s",tmp);

                i++;
            }

            osal_memset(buf, 0, strlen(buf));
        }
        else
        {
            i++;
        }
    }

    fclose(fp);
    remove(RMON_COMMAND_TMPFILE);

    return SYS_ERR_OK;
}

int32 sal_rmon_event_apply(sal_rmon_event_t *pEvent)
{
    char cmd[SYS_BUF1024_LEN];
    char tmp[SYS_BUF256_LEN];
    uint32 len = 0;
    sys_enable_t enable = DISABLED;

    SYS_PARAM_CHK((NULL == pEvent), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (0 == pEvent->eventIndex)
        return SYS_ERR_FAILED;

    osal_memset(cmd, 0, sizeof(cmd));

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_EVENT_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pEvent->eventIndex, RMON_STATUS_INVALID);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_EVENT_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pEvent->eventIndex, RMON_STATUS_CREATE_REQUEST);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_EVENT_TYPE_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pEvent->eventIndex, pEvent->eventType);

    if(0 != osal_strlen(pEvent->community))
    {
        SYS_MEM_CLEAR(tmp);
        _escapeChar_add(pEvent->community, tmp);
        len += sprintf(cmd + len, RMON_SET_COMMAND RMON_EVENT_COMMUNITY_OID "%u s \"%s\" " RMON_COMMAND_NULL_POSTFIX "\n", pEvent->eventIndex, tmp);
    }
    
    if(0 != osal_strlen(pEvent->description))
    {
        SYS_MEM_CLEAR(tmp);
        _escapeChar_add(pEvent->description, tmp);
        len += sprintf(cmd + len, RMON_SET_COMMAND RMON_EVENT_DESCRIPTION_OID "%u s \"%s\" " RMON_COMMAND_NULL_POSTFIX "\n", pEvent->eventIndex, tmp);
    }

    if(0 != osal_strlen(pEvent->owner))
    {
        SYS_MEM_CLEAR(tmp);
        _escapeChar_add(pEvent->owner, tmp);
        len += sprintf(cmd + len, RMON_SET_COMMAND RMON_EVENT_OWNER_OID "%u s \"%s\" " RMON_COMMAND_NULL_POSTFIX "\n", pEvent->eventIndex, tmp);
    }

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_EVENT_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pEvent->eventIndex, RMON_STATUS_VALID);

    system(cmd);
    RMON_CMD_DBG(cmd);

    return SYS_ERR_OK;
}

int32 sal_rmon_event_unapply(sal_rmon_event_t *pEvent)
{
    char cmd[SYS_BUF256_LEN];
    sys_enable_t enable = DISABLED;

    SYS_PARAM_CHK((NULL == pEvent), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (0 == pEvent->eventIndex)
        return SYS_ERR_FAILED;

    osal_memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, RMON_SET_COMMAND RMON_EVENT_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX, pEvent->eventIndex, RMON_STATUS_INVALID);
    system(cmd);
    SYS_DBG(DBG_RMON, "%s", cmd);

    return SYS_ERR_OK;
}

int32 sal_rmon_alarm_set(sal_rmon_alarm_t *pAlarm)
{
    SYS_PARAM_CHK((NULL == pAlarm), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rmon_alarm_set(pAlarm));

    return SYS_ERR_OK;
}

int32 sal_rmon_alarm_get(sal_rmon_alarm_t *pAlarm)
{
    SYS_PARAM_CHK((NULL == pAlarm), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pAlarm->arrayIndex > CAPA_RMON_EVENT_NUM), SYS_ERR_RANGE);

    SYS_ERR_CHK(cfg_rmon_alarm_get(pAlarm));

    return SYS_ERR_OK;
}

int32 sal_rmon_alarmByIndex_get(sal_rmon_alarm_t *pAlarm, uint32 alarmIndex)
{
    uint32 i = 0;
    sal_rmon_alarm_t alarm;

    SYS_PARAM_CHK((NULL == pAlarm), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((alarmIndex < 1) || (alarmIndex > 65535)), SYS_ERR_OUT_OF_RANGE);

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        osal_memset(&alarm, 0 , sizeof(sal_rmon_alarm_t));
        alarm.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_alarm_get(&alarm));

        if (alarmIndex == alarm.alarmIndex)
        {
            osal_memcpy(pAlarm, &alarm, sizeof(sal_rmon_alarm_t));

            return SYS_ERR_OK;
        }
    }

    pAlarm->alarmIndex = 0;

    return SYS_ERR_OK;
}

int32 sal_rmon_alarm_apply(sal_rmon_alarm_t *pAlarm)
{
    char cmd[SYS_BUF1536_LEN];
    char tmp[SYS_BUF64_LEN];
    uint32 len = 0;
    sys_enable_t enable = DISABLED;

    SYS_PARAM_CHK((NULL == pAlarm), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (0 == pAlarm->alarmIndex)
        return SYS_ERR_FAILED;

    osal_memset(cmd, 0, sizeof(cmd));
    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, RMON_STATUS_INVALID);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, RMON_STATUS_CREATE_REQUEST);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_INTERVAL_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, pAlarm->interval);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_VARIABLE_OID "%u o %s%u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex,
        text_rmon_sample_etherOID[pAlarm->variable].text, LP2UP(pAlarm->samplePort));

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_SAMPLE_TYPE_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, pAlarm->sampleType);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_START_ALARM_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, pAlarm->alarmType);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_RISING_TH_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, pAlarm->risingThreshold);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_FALLING_TH_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, pAlarm->fallingThreshold);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_RISING_EVENT_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, pAlarm->risingEventIndex);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_FALLING_EVENT_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, pAlarm->fallingEventIndex);

    if(0 != osal_strlen(pAlarm->owner))
    {
        SYS_MEM_CLEAR(tmp);
        _escapeChar_add(pAlarm->owner, tmp);
        len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_OWNER_OID "%u s %s " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, tmp);
    }

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_ALARM_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pAlarm->alarmIndex, RMON_STATUS_VALID);

    system(cmd);
    RMON_CMD_DBG(cmd);

    return SYS_ERR_OK;
}

int32 sal_rmon_alarm_unapply(sal_rmon_alarm_t *pAlarm)
{
    char cmd[SYS_BUF256_LEN];
    sys_enable_t enable = DISABLED;

    SYS_PARAM_CHK((NULL == pAlarm), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (0 == pAlarm->alarmIndex)
        return SYS_ERR_FAILED;

    osal_memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, RMON_SET_COMMAND RMON_ALARM_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX, pAlarm->alarmIndex, RMON_STATUS_INVALID);
    system(cmd);
    SYS_DBG(DBG_RMON, "%s", cmd);

    return SYS_ERR_OK;
}

int32 sal_rmon_history_set(sal_rmon_history_t *pHistory)
{
    SYS_PARAM_CHK((NULL == pHistory), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_rmon_history_set(pHistory));

    return SYS_ERR_OK;
}

int32 sal_rmon_history_get(sal_rmon_history_t *pHistory)
{
    SYS_PARAM_CHK((NULL == pHistory), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((pHistory->arrayIndex > CAPA_RMON_HISTORY_NUM), SYS_ERR_RANGE);

    SYS_ERR_CHK(cfg_rmon_history_get(pHistory));

    return SYS_ERR_OK;
}

int32 sal_rmon_historyByIndex_get(sal_rmon_history_t *pHistory, uint32 historyIndex)
{
    uint32 i = 0;
    sal_rmon_history_t history;

    SYS_PARAM_CHK((NULL == pHistory), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((historyIndex < 1) || (historyIndex > 65535)), SYS_ERR_OUT_OF_RANGE);

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        osal_memset(&history, 0 , sizeof(sal_rmon_history_t));
        history.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_history_get(&history));

        if (historyIndex == history.historyIndex)
        {
            osal_memcpy(pHistory, &history, sizeof(sal_rmon_history_t));

            return SYS_ERR_OK;
        }
    }

    pHistory->historyIndex = 0;

    return SYS_ERR_OK;
}

int32 sal_rmon_history_apply(sal_rmon_history_t *pHistory)
{
    char cmd[SYS_BUF1024_LEN];
    char tmp[SYS_BUF64_LEN];
    uint32 len = 0;
    sys_enable_t enable = DISABLED;

    SYS_PARAM_CHK((NULL == pHistory), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (0 == pHistory->historyIndex)
        return SYS_ERR_FAILED;

    osal_memset(cmd, 0, sizeof(cmd));

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_HISTORY_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pHistory->historyIndex, RMON_STATUS_INVALID);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_HISTORY_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pHistory->historyIndex, RMON_STATUS_CREATE_REQUEST);

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_HISTORY_INTERFACE_OID "%u o %s%u " RMON_COMMAND_NULL_POSTFIX "\n", pHistory->historyIndex,
        RMON_IFINDEX_OID, LP2UP(pHistory->datasource));

    if (RMON_HISTORY_BUCKET_DFL != pHistory->bucketRequested)
        len += sprintf(cmd + len, RMON_SET_COMMAND RMON_HISTORY_BUCKET_REQUEST_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pHistory->historyIndex, pHistory->bucketRequested);

    if (RMON_HISTORY_INTERVAL_DFL != pHistory->interval)
        len += sprintf(cmd + len, RMON_SET_COMMAND RMON_HISTORY_INTERVAL_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pHistory->historyIndex, pHistory->interval);

    if(0 != osal_strlen(pHistory->owner))
    {
        SYS_MEM_CLEAR(tmp);
        _escapeChar_add(pHistory->owner, tmp);
        len += sprintf(cmd + len, RMON_SET_COMMAND RMON_HISTORY_OWNER_OID "%u s %s " RMON_COMMAND_NULL_POSTFIX "\n", pHistory->historyIndex, tmp);
    }

    len += sprintf(cmd + len, RMON_SET_COMMAND RMON_HISTORY_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX "\n", pHistory->historyIndex, RMON_STATUS_VALID);

    system(cmd);
    RMON_CMD_DBG(cmd);

    return SYS_ERR_OK;
}

int32 sal_rmon_history_unapply(sal_rmon_history_t *pHistory)
{
    char cmd[SYS_BUF256_LEN];
    sys_enable_t enable = DISABLED;

    SYS_PARAM_CHK((NULL == pHistory), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    if (0 == pHistory->historyIndex)
        return SYS_ERR_FAILED;

    osal_memset(cmd, 0, sizeof(cmd));
    sprintf(cmd, RMON_SET_COMMAND RMON_HISTORY_STATUS_OID "%u i %u " RMON_COMMAND_NULL_POSTFIX, pHistory->historyIndex, RMON_STATUS_INVALID);
    system(cmd);
    SYS_DBG(DBG_RMON, "%s", cmd);

    return SYS_ERR_OK;
}

int32 sal_rmon_historyTable_get(sal_rmon_history_log_t *pTable, uint32 historyIndex)
{
    uint32 i = 0;
    uint32 cnt = 0;
    FILE *fp = NULL;
    char *tmp = NULL;
    char idStr[SYS_BUF8_LEN];
    char cmd[SYS_BUF2048_LEN];
    char buf[SYS_BUF256_LEN];
    sys_enable_t enable = DISABLED;

    SYS_PARAM_CHK((NULL == pTable), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(((historyIndex < 1) || (historyIndex > 65535)), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    osal_memset(idStr, 0, sizeof(idStr));
    osal_memset(cmd, 0, sizeof(cmd));
    osal_memset(buf, 0, sizeof(buf));

    remove(RMON_COMMAND_TMPFILE);

    sprintf(idStr, ".%u", historyIndex);
    sprintf(cmd, RMON_WALK_COMMAND RMON_HISTORY_LOG_SAMPLE_INDEX_OID "%s " RMON_COMMAND_POSTFIX, idStr);
    SYS_DBG(DBG_RMON, "cmd %s",cmd);
    system(cmd);

    fp = fopen(RMON_COMMAND_TMPFILE, "r");

    if (NULL == fp)
        return SYS_ERR_FAILED;

    while (NULL != fgets(buf, sizeof(buf), fp))
    {
        
        tmp = strstr(buf, idStr);

        if (NULL == tmp)
            break;

        tmp = rindex(tmp, '.');

        if (NULL == tmp)
            continue;

        if (RMON_HISTORY_BUCKET_DFL == cnt)
        {
            for (i = 0; i < RMON_HISTORY_BUCKET_DFL - 1 ; i++)
                pTable[i].sampleIndex = pTable[i+1].sampleIndex;

            cnt--;
        }

        pTable[cnt].sampleIndex = atoi(++tmp);
        SYS_DBG(DBG_RMON, "id %u", pTable[cnt].sampleIndex);
        cnt++;

        osal_memset(buf, 0, strlen(buf));
    }

    fclose(fp);
    remove(RMON_COMMAND_TMPFILE);
    SYS_DBG(DBG_RMON, "cnt %u", cnt);

    if (0 == cnt)
        return SYS_ERR_OK;

    osal_memset(cmd, 0, sizeof(cmd));
    i = 0;
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_INTERVAL_START_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_DROP_EVENTS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_OCTETS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_PKTS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_BROADCAST_PKTS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_MULTICAST_PKTS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_CRCALIGN_ERROR_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_UNDERSIZE_PKTS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_OVERSIZE_PKTS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_FRAGMENTS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_JABBERS_PKTS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_COLLISIONS_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    i += sprintf(cmd + i, RMON_WALK_COMMAND RMON_HISTORY_LOG_ULTILIZATION_OID "%s " RMON_COMMAND_POSTFIX"\n", idStr);
    RMON_CMD_DBG(cmd);
    system(cmd);

    fp = fopen(RMON_COMMAND_TMPFILE, "r");

    if (NULL == fp)
        return SYS_ERR_FAILED;

    for (i = 0; i < cnt ; i++)
    {
        if (NULL != fgets(buf, sizeof(buf), fp))
        {
            tmp = strchr(buf, ':');

            if (NULL != tmp)
            {
                osal_strcpy(pTable[i].intervalStart, ++tmp);
                pTable[i].intervalStart[strlen(pTable[i].intervalStart) - 1] = '\0';
            }

            osal_memset(buf, 0, strlen(buf));
        }
    }

#define GET_COUNTER(__idx, __counter)   \
do{                                     \
    for (i = 0; i < cnt ; i++)          \
    {                                   \
        if (NULL != fgets(buf, sizeof(buf), fp))   \
        {                                           \
            SYS_DBG(DBG_RMON, "%s", buf);       \
            tmp = strchr(buf, ':');                 \
            if (NULL != tmp)                        \
            {                                       \
                __counter = atol(++tmp);            \
                SYS_DBG(DBG_RMON, "id %u Counter: %d",__idx, __counter);  \
            }                                                           \
            osal_memset(buf, 0, strlen(buf));                           \
        }                                                               \
    }                                                                   \
} while(0);

    GET_COUNTER(pTable[i].sampleIndex, pTable[i].dropEvent);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].octets);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].pkts);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].broadcastPkts);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].multicastPkts);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].crcAlignErrors);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].undersizePkts);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].oversizePkts);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].fragments);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].jabbers);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].collisions);
    GET_COUNTER(pTable[i].sampleIndex, pTable[i].utilization);

    fclose(fp);
    remove(RMON_COMMAND_TMPFILE);

    return SYS_ERR_OK;
}

int32 sal_rmon_all_apply(void)
{
    uint32 i = 0;
    sal_rmon_alarm_t alarm;
    sal_rmon_event_t event;
    sal_rmon_history_t history;
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_snmp_enable_get(&enable));

    if (DISABLED == enable)
        return SYS_ERR_OK;

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        osal_memset(&event, 0 , sizeof(sal_rmon_event_t));
        event.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_event_get(&event));

        if (0 != event.eventIndex)
            SYS_ERR_CHK(sal_rmon_event_apply(&event));
    }

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        osal_memset(&alarm, 0 , sizeof(sal_rmon_alarm_t));
        alarm.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_alarm_get(&alarm));

        if (0 != alarm.alarmIndex)
            SYS_ERR_CHK(sal_rmon_alarm_apply(&alarm));
    }

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        osal_memset(&history, 0 , sizeof(sal_rmon_history_t));
        history.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_history_get(&history));

        if (0 != history.historyIndex)
            SYS_ERR_CHK(sal_rmon_history_apply(&history));
    }

    return SYS_ERR_OK;
}
