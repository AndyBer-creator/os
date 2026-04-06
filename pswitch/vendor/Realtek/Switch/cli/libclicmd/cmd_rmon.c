
#include <libcmd/cmd.h>

#include <libsal/sal_mib.h>
#include <libsal/sal_rmon.h>

int32 cmd_rmon_port_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    if (0 == argc)
        LOGIC_PORTMASK_SET_ALL(lpm);
    else
        CMD_PORTLIST_GET(0, &lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        sal_mib_cntr_clr(lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_rmon_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    char port_str[8];
    uint32 i = 0;
    uint32 ret = SYS_ERR_OK;
    uint64 result = 0;

    CMD_PORTLIST_GET(0, &logicPortmask);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        i = 0;
        osal_memset(port_str, 0, sizeof(port_str));
        SYS_ERR_CHK(LP2STR(logicPort, port_str));

        XMORE("==== Port %2s =============================   \n", port_str);

        while(rmonMibTbl[i].name !=NULL)
        {
            if (rmonMibTbl[i].sal_mib_func != NULL)
                ret = rmonMibTbl[i].sal_mib_func(logicPort, &result);

            if (SYS_ERR_OK == ret)
                XMORE("%-32s: %llu\n", rmonMibTbl[i].name, result);

            i++;
        }

        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_rmon_event_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i = 0;
    uint32 eventIndex = 0;
    sal_rmon_event_t event;

    if ('a' != argv[0][0])
        eventIndex = SYS_STR2UINT(argv[0]);

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        osal_memset(&event, 0 , sizeof(sal_rmon_event_t));
        event.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_event_get(&event));

        if (((0 == eventIndex) && (0 != event.eventIndex)) ||
            ((0 != eventIndex) && (eventIndex == event.eventIndex)))
        {
            SYS_ERR_CHK(sal_rmon_eventRuntime_get(&event));

            XMORE("Rmon Event Index       : %u \n", event.eventIndex);
            XMORE("Rmon Event Type        : %s  \n", text_rmon_event_type[event.eventType].text);
            XMORE("Rmon Event Community   : %s  \n", event.community);
            XMORE("Rmon Event Description : %s  \n", event.description);
            XMORE("Rmon Event Last Sent   : %s  \n", event.eventLastTimeSent);
            XMORE("Rmon Event Owner       : %s  \n", event.owner);
            XMORE("\n");
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_rmon_eventLog_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int32 i = 0;
    uint32 eventIndex = 0;
    sal_rmon_event_t event;
    sal_rmon_event_log_t eventLog[RMON_EVENT_LOG_DFL];

    eventIndex = SYS_STR2UINT(argv[0]);

    osal_memset(&event, 0 , sizeof(sal_rmon_event_t));
    SYS_ERR_CHK(sal_rmon_eventByIndex_get(&event, eventIndex));

    if (0 == event.eventIndex)
    {
        XMORE("Event entry not exist\n");

        return SYS_ERR_OK;
    }

    osal_memset(&eventLog, 0 , sizeof(eventLog));

    SYS_ERR_CHK(sal_rmon_eventLog_get(eventLog, eventIndex));

    for (i = RMON_EVENT_LOG_DFL -1; i >= 0 ; i--)
    {
        if (0 == eventLog[i].index)
            continue;

        XMORE("=======================================================\n");
        XMORE("Index       : %u\n", eventLog[i].index);
        XMORE("Time        : %s\n", eventLog[i].logTime);
        XMORE("Description : %s\n", eventLog[i].description);
        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_rmon_alarm_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    uint32 i = 0;
    uint32 alarmIndex = 0;
    sal_rmon_alarm_t alarm;

    if ('a' != argv[0][0])
        alarmIndex = SYS_STR2UINT(argv[0]);

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        SYS_MEM_CLEAR(alarm);
        alarm.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_alarm_get(&alarm));

        if (((0 == alarmIndex) && (0 != alarm.alarmIndex)) ||
            ((0 != alarmIndex) && (alarmIndex == alarm.alarmIndex)))
        {
            
            SYS_MEM_CLEAR(portStr);
            SYS_ERR_CHK(LP2STR(alarm.samplePort, portStr));

            XMORE("Rmon Alarm Index             : %u  \n", alarm.alarmIndex);
            XMORE("Rmon Alarm Sample Interval   : %u  \n", alarm.interval);
            XMORE("Rmon Alarm Sample Interface  : %s  \n", portStr);
            XMORE("Rmon Alarm Sample Variable   : %s  \n", text_rmon_sample[alarm.variable].text);
            XMORE("Rmon Alarm Sample Type       : %s  \n", text_rmon_alarm_sample_type[alarm.sampleType].text);
            XMORE("Rmon Alarm Type              : %s  \n", text_rmon_alarm_type[alarm.alarmType].text);
            XMORE("Rmon Alarm Rising Threshold  : %u  \n", alarm.risingThreshold);
            XMORE("Rmon Alarm Rising Event      : %u  \n", alarm.risingEventIndex);
            XMORE("Rmon Alarm Falling Threshold : %u  \n", alarm.fallingThreshold);
            XMORE("Rmon Alarm Falling Event     : %u  \n", alarm.fallingEventIndex);
            XMORE("Rmon Alarm Owner             : %s  \n", alarm.owner);
            XMORE("\n");
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_rmon_history_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    uint32 i = 0;
    uint32 historyIndex = 0;
    sal_rmon_history_t history;

    if ('a' != argv[0][0])
        historyIndex = SYS_STR2UINT(argv[0]);

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        SYS_MEM_CLEAR(history);

        history.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_history_get(&history));

        if (((0 == historyIndex) && (0 != history.historyIndex)) ||
            ((0 != historyIndex) && (historyIndex == history.historyIndex)))
        {
            SYS_MEM_CLEAR(portStr);
            SYS_ERR_CHK(sal_rmon_history_get(&history));
            SYS_ERR_CHK(LP2STR(history.datasource, portStr));

            XMORE("Rmon History Index       : %u \n", history.historyIndex);
            XMORE("Rmon Collection Interface: %s \n", portStr);
            XMORE("Rmon History Bucket      : %u \n", history.bucketRequested);
            XMORE("Rmon history Interval    : %u \n", history.interval);
            XMORE("Rmon History Owner       : %s \n", history.owner);
            XMORE("\n");
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_rmon_historyStatistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int32 i = 0;
    uint32 isShow = FALSE;
    uint32 historyIndex = 0;
    sal_rmon_history_t history;
    sal_rmon_history_log_t historyLog[RMON_HISTORY_BUCKET_DFL];

    historyIndex = SYS_STR2UINT(argv[0]);

    osal_memset(&history,0 ,sizeof(history));
    SYS_ERR_CHK(sal_rmon_historyByIndex_get(&history, historyIndex));

    if (0 == history.historyIndex)
    {
        XMORE("History entry not exist!\n");

        return SYS_ERR_OK;
    }

    osal_memset(historyLog,0 ,sizeof(historyLog));

    SYS_ERR_CHK(sal_rmon_historyTable_get(historyLog, historyIndex));

    for (i = RMON_HISTORY_BUCKET_DFL -1; i >= 0 ; i--)
    {
        if (0 == historyLog[i].sampleIndex)
            continue;

        isShow = TRUE;

        XMORE("=======================================================\n");
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_SAMPLE_INDEX].text, historyLog[i].sampleIndex);
        XMORE("%-20s : %s\n", text_rmon_history[RMON_HISTORY_INTERVAL_START].text, historyLog[i].intervalStart);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_DROPEVENTS].text, historyLog[i].dropEvent);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_OCTETS].text, historyLog[i].octets);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_PKTS].text, historyLog[i].pkts);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_BROADCASTPKTS].text, historyLog[i].broadcastPkts);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_MULTICASTPKTS].text, historyLog[i].multicastPkts);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_CRCALIGNERRORS].text, historyLog[i].crcAlignErrors);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_UNDERSIZEPKTS].text, historyLog[i].undersizePkts);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_OVERSIZEPKTS].text, historyLog[i].oversizePkts);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_FRAGMENTS].text, historyLog[i].fragments);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_JABBERS].text, historyLog[i].jabbers);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_COLLISIONS].text, historyLog[i].collisions);
        XMORE("%-20s : %u\n", text_rmon_history[RMON_HISTORY_UTILIZATION].text, historyLog[i].utilization);
        XMORE("\n");
    }

    if(FALSE == isShow)
        XMORE("No Data Avaliable!\n");

    return SYS_ERR_OK;
}

int32 cmd_rmon_event_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char *pToken = NULL;
    uint32 i = 0;
    uint32 tokenCnt = 0;
    uint32 argCnt = 0;
    uint32 isLog = FALSE;
    uint32 isTrap = FALSE;
    sal_rmon_event_t newEvent;
    sal_rmon_event_t tmpEvent;

    if (CMD_IS_NO_FORM_EXIST())
    {
        uint32 i = 0;
        uint32 eventIndex = 0;
        sal_rmon_event_t event;

        eventIndex = SYS_STR2UINT(argv[0]);

        for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
        {
            osal_memset(&event, 0 , sizeof(sal_rmon_event_t));
            event.arrayIndex = i;
            SYS_ERR_CHK(sal_rmon_event_get(&event));

            if(event.eventIndex == eventIndex)
            {
                SYS_ERR_CHK(sal_rmon_event_unapply(&event));

                osal_memset(&event, 0 , sizeof(sal_rmon_event_t));
                event.arrayIndex = i;
                SYS_ERR_CHK(sal_rmon_event_set(&event));

                return SYS_ERR_OK;
            }
        }

        XMORE("Rmon event index %u doesn't exist!\n", eventIndex);

        return SYS_ERR_OK;
    }

    osal_memset(&newEvent, 0 , sizeof(sal_rmon_event_t));

    tokenCnt = CMD_TOKENNUM_GET(cmd->vline);

    newEvent.arrayIndex = CAPA_RMON_EVENT_NUM;
    newEvent.eventIndex = SYS_STR2UINT(argv[argCnt++]);

    for (i = 3; i < tokenCnt ; i++)
    {
        pToken = CMD_TOKENSTR_GET(cmd->vline, i);

        if('l' == pToken[0])
        {
            isLog = TRUE;
        }
        else if('t' == pToken[0])
        {
            if(osal_strlen(argv[argCnt]) > CAPA_SNMP_COMMUNITY_LEN)
            {
                XMORE("Community string length is too long.");

                return SYS_ERR_INPUT;
            }

            isTrap = TRUE;
            i++;
            osal_strcpy(newEvent.community, argv[argCnt++]);
        }
        else if('d' == pToken[0])
        {
            if(osal_strlen(argv[argCnt]) >= sizeof(newEvent.description))
            {
                XMORE("Description string length is too long.");

                return SYS_ERR_INPUT;
            }

            i++;
            osal_strcpy(newEvent.description, argv[argCnt++]);
        }
        else if('o' == pToken[0])
        {
            if(osal_strlen(argv[argCnt]) >= RMON_OWNER_NAME_LEN)
            {
                XMORE("Owner string length is too long.");

                return SYS_ERR_INPUT;
            }

            i++;
            osal_strcpy(newEvent.owner, argv[argCnt++]);
        }
    }

    if (isLog && isTrap)
        newEvent.eventType = RMON_EVENT_LOG_AND_TRAP;
    else if (isLog)
        newEvent.eventType = RMON_EVENT_LOG;
    else if (isTrap)
        newEvent.eventType = RMON_EVENT_TRAP;
    else
        newEvent.eventType = RMON_EVENT_NONE;

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        osal_memset(&tmpEvent, 0 , sizeof(sal_rmon_event_t));
        tmpEvent.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_event_get(&tmpEvent));

        if(tmpEvent.eventIndex == newEvent.eventIndex)
        {
            newEvent.arrayIndex = i;
            break;
        }

        if((newEvent.arrayIndex == CAPA_RMON_EVENT_NUM) && (0 == tmpEvent.eventIndex))
            newEvent.arrayIndex = i;
    }

    if (newEvent.arrayIndex == CAPA_RMON_EVENT_NUM)
    {
        XMORE("Rmon event entry has full\n");

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_rmon_event_set(&newEvent));

    if (CMD_IS_CLI_RUN())
    {
        uint32 index = 0;
        sal_rmon_event_t event;

        osal_memset(&event, 0 , sizeof(sal_rmon_event_t));

        index = SYS_STR2UINT(argv[0]);

        SYS_ERR_CHK(sal_rmon_eventByIndex_get(&event, index));

        if (0 != event.eventIndex)
            SYS_ERR_CHK(sal_rmon_event_apply(&event));

        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}

int32 cmd_rmon_alarm_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i = 0;
    uint32 argCnt = 0;
    sal_rmon_event_t tmpEvent;
    sal_rmon_alarm_t newAlarm;
    sal_rmon_alarm_t tmpAlarm;

    if (CMD_IS_NO_FORM_EXIST())
    {
        uint32 alarmIndex = 0;
        sal_rmon_alarm_t alarm;

        alarmIndex = SYS_STR2UINT(argv[0]);

        for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
        {
            osal_memset(&alarm, 0 , sizeof(sal_rmon_alarm_t));
            alarm.arrayIndex = i;
            SYS_ERR_CHK(sal_rmon_alarm_get(&alarm));

            if(alarm.alarmIndex == alarmIndex)
            {
                SYS_ERR_CHK(sal_rmon_alarm_unapply(&alarm));

                osal_memset(&alarm, 0 , sizeof(sal_rmon_alarm_t));
                alarm.arrayIndex = i;

                SYS_ERR_CHK(sal_rmon_alarm_set(&alarm));

                return SYS_ERR_OK;
            }
        }

        XMORE("Rmon alarm index %u doesn't exist!\n", alarmIndex);

        return SYS_ERR_OK;
    }

    osal_memset(&newAlarm, 0 , sizeof(sal_rmon_alarm_t));

    newAlarm.arrayIndex = CAPA_RMON_ALARM_NUM;
    newAlarm.alarmIndex = SYS_STR2UINT(argv[argCnt++]);
    STR2LP(argv[argCnt++], &newAlarm.samplePort);

    switch (argv[argCnt][0])
    {
        case 'b':
            newAlarm.variable = RMON_SAMPLE_BROADCASTPKTS;
            break;

        case 'c':
            if ('r' == argv[argCnt][1])
                newAlarm.variable = RMON_SAMPLE_CRCALIGNERRORS;
            else
                newAlarm.variable = RMON_SAMPLE_COLLISIONS;
            break;

        case 'd':
            newAlarm.variable = RMON_SAMPLE_DROPEVENTS;
            break;

        case 'f':
            newAlarm.variable = RMON_SAMPLE_FRAGMENTS;
            break;

        case 'j':
            newAlarm.variable = RMON_SAMPLE_JABBERS;
            break;

        case 'm':
            newAlarm.variable = RMON_SAMPLE_MULTICASTPKTS;
            break;

        case 'o':
            if ('c' == argv[argCnt][1])
                newAlarm.variable = RMON_SAMPLE_OCTETS;
            else
                newAlarm.variable = RMON_SAMPLE_OVERSIZEPKTS;
            break;

        case 'p':
            if (4 == osal_strlen(argv[argCnt]))
                newAlarm.variable = RMON_SAMPLE_PKTS;
            else if ('4' == argv[argCnt][5])
                newAlarm.variable = RMON_SAMPLE_PKTS64OCTETS;
            else if ('2' == argv[argCnt][5])
                newAlarm.variable = RMON_SAMPLE_PKTS128TO255OCTETS;
            else if ('1' == argv[argCnt][5])
                newAlarm.variable = RMON_SAMPLE_PKTS512TO1023OCTETS;
            else if ('0' == argv[argCnt][5])
                newAlarm.variable = RMON_SAMPLE_PKTS1024TO1518OCTETS;
            else if ('5' == argv[argCnt][5])
            {
                if ('6' == argv[argCnt][4])
                    newAlarm.variable = RMON_SAMPLE_PKTS65TO127OCTETS;
                else
                    newAlarm.variable = RMON_SAMPLE_PKTS256TO511OCTETS;
            }
            break;

        case 'u':
            newAlarm.variable = RMON_SAMPLE_UNDERSIZEPKTS;
            break;

        default:
            break;
    }

    argCnt++;

    newAlarm.interval = SYS_STR2UINT(argv[argCnt++]);

    if('a' == argv[argCnt++][0])
        newAlarm.sampleType = RMON_ALARM_SAMPLE_ABSOLUTE;
    else
        newAlarm.sampleType = RMON_ALARM_SAMPLE_DELTA;

    newAlarm.risingThreshold = SYS_STR2UINT(argv[argCnt++]);
    newAlarm.risingEventIndex = SYS_STR2UINT(argv[argCnt++]);
    newAlarm.fallingThreshold = SYS_STR2UINT(argv[argCnt++]);
    newAlarm.fallingEventIndex = SYS_STR2UINT(argv[argCnt++]);

    if (newAlarm.risingThreshold <= newAlarm.fallingThreshold)
    {
        SYS_PRINTF("Error: Rising Threshold must greater than Falling Threshold\n");
        return SYS_ERR_INPUT;
    }

    if ('r' == argv[argCnt][0])
    {
        if (6 == osal_strlen(argv[argCnt]))
            newAlarm.alarmType = RMON_ALARM_RISING;
        else
            newAlarm.alarmType = RMON_ALARM_FALLING_OR_RISING;
    }
    else
    {
         newAlarm.alarmType = RMON_ALARM_FALLING;
    }

    argCnt++;

    if (0 != newAlarm.risingEventIndex)
    {
        SYS_ERR_CHK(sal_rmon_eventByIndex_get(&tmpEvent, newAlarm.risingEventIndex));

        if(0 == tmpEvent.eventIndex)
        {
            XMORE("Rising Event is not exist.");

            return SYS_ERR_INPUT;
        }
    }

    if (0 != newAlarm.fallingEventIndex)
    {
        SYS_ERR_CHK(sal_rmon_eventByIndex_get(&tmpEvent, newAlarm.fallingEventIndex));

        if(0 == tmpEvent.eventIndex)
        {
            XMORE("Falling Event is not exist.");

            return SYS_ERR_INPUT;
        }
    }

    if (11 == argc)
    {
        if(osal_strlen(argv[argCnt]) >= RMON_OWNER_NAME_LEN)
        {
            XMORE("Owner string length is too long.");

            return SYS_ERR_INPUT;
        }

        osal_strcpy(newAlarm.owner, argv[argCnt]);
    }

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        osal_memset(&tmpAlarm, 0 , sizeof(sal_rmon_alarm_t));
        tmpAlarm.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_alarm_get(&tmpAlarm));

        if(tmpAlarm.alarmIndex == newAlarm.alarmIndex)
        {
            newAlarm.arrayIndex = i;
            break;
        }

        if((newAlarm.arrayIndex == CAPA_RMON_ALARM_NUM) && (0 == tmpAlarm.alarmIndex))
            newAlarm.arrayIndex = i;
    }

    if (newAlarm.arrayIndex == CAPA_RMON_ALARM_NUM)
    {
        XMORE("Rmon alarm entry has full\n");

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_rmon_alarm_set(&newAlarm));

    if (CMD_IS_CLI_RUN())
    {
        uint32 index = 0;
        sal_rmon_alarm_t alarm;

        osal_memset(&alarm, 0 , sizeof(sal_rmon_alarm_t));

        index = SYS_STR2UINT(argv[0]);

        SYS_ERR_CHK(sal_rmon_alarmByIndex_get(&alarm, index));

        if (0 != alarm.alarmIndex)
            SYS_ERR_CHK(sal_rmon_alarm_apply(&alarm));
    }

    return SYS_ERR_OK;
}

int32 cmd_rmon_history_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char *pToken = NULL;
    uint32 i = 0;
    uint32 tokenCnt = 0;
    uint32 argCnt = 0;
    sal_rmon_history_t newHistory;
    sal_rmon_history_t tmpHistory;

    if (CMD_IS_NO_FORM_EXIST())
    {
        uint32 historyIndex = 0;
        sal_rmon_history_t history;

        historyIndex = SYS_STR2UINT(argv[0]);

        for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
        {
            osal_memset(&history, 0 , sizeof(sal_rmon_history_t));

            history.arrayIndex = i;
            SYS_ERR_CHK(sal_rmon_history_get(&history));

            if(history.historyIndex == historyIndex)
            {
                SYS_ERR_CHK(sal_rmon_history_unapply(&history));

                osal_memset(&history, 0 , sizeof(sal_rmon_history_t));
                history.arrayIndex = i;

                SYS_ERR_CHK(sal_rmon_history_set(&history));

                return SYS_ERR_OK;
            }
        }

        XMORE("Rmon history index %u doesn't exist!\n", historyIndex);

        return SYS_ERR_OK;
    }

    osal_memset(&newHistory, 0 , sizeof(sal_rmon_history_t));

    tokenCnt = CMD_TOKENNUM_GET(cmd->vline);

    newHistory.arrayIndex = CAPA_RMON_HISTORY_NUM;
    newHistory.historyIndex = SYS_STR2UINT(argv[argCnt++]);
    STR2LP(argv[argCnt++], &newHistory.datasource);

    newHistory.bucketRequested = RMON_HISTORY_BUCKET_DFL;
    newHistory.interval = RMON_HISTORY_INTERVAL_DFL;

    for (i = 5; i < tokenCnt ; i++)
    {
        pToken = CMD_TOKENSTR_GET(cmd->vline, i);

        if('b' == pToken[0])
        {
            i++;
            newHistory.bucketRequested = SYS_STR2UINT(argv[argCnt++]);
        }
        else if('i' == pToken[0])
        {
            i++;
            newHistory.interval = SYS_STR2UINT(argv[argCnt++]);
        }
        else if('o' == pToken[0])
        {
            if(osal_strlen(argv[argCnt]) >= RMON_OWNER_NAME_LEN)
            {
                XMORE("Owner name string length is too long.");

                return SYS_ERR_INPUT;
            }

            i++;
            osal_strcpy(newHistory.owner, argv[argCnt++]);
        }
    }

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        osal_memset(&tmpHistory, 0 , sizeof(sal_rmon_history_t));
        tmpHistory.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_history_get(&tmpHistory));

        if(tmpHistory.historyIndex == newHistory.historyIndex)
        {
            newHistory.arrayIndex = i;
            break;
        }

        if((newHistory.arrayIndex == CAPA_RMON_EVENT_NUM) && (0 == tmpHistory.historyIndex))
            newHistory.arrayIndex = i;
    }

    if (newHistory.arrayIndex == CAPA_RMON_EVENT_NUM)
    {
        XMORE("Rmon history entry has full\n");

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_rmon_history_set(&newHistory));

    if (CMD_IS_CLI_RUN())
    {
        uint32 index = 0;
        sal_rmon_history_t history;

        osal_memset(&history, 0 , sizeof(sal_rmon_history_t));

        index = SYS_STR2UINT(argv[0]);

        SYS_ERR_CHK(sal_rmon_historyByIndex_get(&history, index));

        if (0 != history.historyIndex)
            SYS_ERR_CHK(sal_rmon_history_apply(&history));
    }

    return SYS_ERR_OK;
}

static int32 _cmd_rmon_event_save(FILE *fp)
{
    uint32 i = 0;
    uint32 len = 0;
    sal_rmon_event_t event;
    char cmd[MAX_CMD_STRLEN];

    for (i = 0; i < CAPA_RMON_EVENT_NUM; i++)
    {
        osal_memset(&event, 0 , sizeof(sal_rmon_event_t));
        event.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_event_get(&event));

        if(0 == event.eventIndex)
            continue;

        osal_memset(cmd, 0, sizeof(cmd));
        len = 0;

        len += osal_sprintf(cmd + len, "rmon event %u ", event.eventIndex);

        if((event.eventType == RMON_EVENT_LOG) || (event.eventType == RMON_EVENT_LOG_AND_TRAP))
            len += osal_sprintf(cmd + len, "log ");

        if((event.eventType == RMON_EVENT_TRAP) || (event.eventType == RMON_EVENT_LOG_AND_TRAP))
            len += osal_sprintf(cmd + len, "trap \"%s\" ", event.community);

        if(0 != osal_strlen(event.description))
            len += osal_sprintf(cmd + len, "description \"%s\" ", event.description);

        if(0 != osal_strlen(event.owner))
            len += osal_sprintf(cmd + len, "owner \"%s\" ", event.owner);

        len += osal_sprintf(cmd + len, "\n");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

static int32 _cmd_rmon_alarm_save(FILE *fp)
{
    uint32 i = 0;
    uint32 len = 0;
    sal_rmon_alarm_t alarm;
    char cmd[MAX_CMD_STRLEN];
    char portStr[CAPA_PORT_STR_LEN];

    for (i = 0; i < CAPA_RMON_ALARM_NUM; i++)
    {
        osal_memset(&alarm, 0 , sizeof(sal_rmon_alarm_t));
        alarm.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_alarm_get(&alarm));

        if(0 == alarm.alarmIndex)
            continue;

        osal_memset(cmd, 0, sizeof(cmd));
        SYS_MEM_CLEAR(portStr);
        LP2STR(alarm.samplePort, portStr);

        len = 0;

        len += osal_sprintf(cmd + len, "rmon alarm %u ", alarm.alarmIndex);
        len += osal_sprintf(cmd + len, "interface %s ", portStr);

        switch (alarm.variable)
        {
            case RMON_SAMPLE_DROPEVENTS:
                len += osal_sprintf(cmd + len, "drop-events ");
                break;

            case RMON_SAMPLE_OCTETS:
                len += osal_sprintf(cmd + len, "octets ");
                break;

            case RMON_SAMPLE_PKTS:
                len += osal_sprintf(cmd + len, "pkts ");
                break;

            case RMON_SAMPLE_BROADCASTPKTS:
                len += osal_sprintf(cmd + len, "broadcast-pkts ");
                break;

            case RMON_SAMPLE_MULTICASTPKTS:
                len += osal_sprintf(cmd + len, "multicast-pkts ");
                break;

            case RMON_SAMPLE_CRCALIGNERRORS:
                len += osal_sprintf(cmd + len, "crc-align-errors ");
                break;

            case RMON_SAMPLE_UNDERSIZEPKTS:
                len += osal_sprintf(cmd + len, "undersize-pkts ");
                break;

            case RMON_SAMPLE_OVERSIZEPKTS:
                len += osal_sprintf(cmd + len, "oversize-pkts ");
                break;

            case RMON_SAMPLE_FRAGMENTS:
                len += osal_sprintf(cmd + len, "fragments ");
                break;

            case RMON_SAMPLE_JABBERS:
                len += osal_sprintf(cmd + len, "jabbers ");
                break;

            case RMON_SAMPLE_COLLISIONS:
                len += osal_sprintf(cmd + len, "collisions ");
                break;

            case RMON_SAMPLE_PKTS64OCTETS:
                len += osal_sprintf(cmd + len, "pkts64octets ");
                break;

            case RMON_SAMPLE_PKTS65TO127OCTETS:
                len += osal_sprintf(cmd + len, "pkts65to127octets ");
                break;

            case RMON_SAMPLE_PKTS128TO255OCTETS:
                len += osal_sprintf(cmd + len, "pkts128to255octets ");
                break;

            case RMON_SAMPLE_PKTS256TO511OCTETS:
                len += osal_sprintf(cmd + len, "pkts256to511octets ");
                break;

            case RMON_SAMPLE_PKTS512TO1023OCTETS:
                len += osal_sprintf(cmd + len, "pkts512to1023octets ");
                break;

            case RMON_SAMPLE_PKTS1024TO1518OCTETS:
                len += osal_sprintf(cmd + len, "pkts1024to1518octets ");
                break;

            default:
                break;
        }

        len += osal_sprintf(cmd + len, "%u ", alarm.interval);
        len += osal_sprintf(cmd + len, "%s ", text_rmon_alarm_sample_type[alarm.sampleType].text);
        len += osal_sprintf(cmd + len, "rising %u %u ", alarm.risingThreshold, alarm.risingEventIndex);
        len += osal_sprintf(cmd + len, "falling %u %u ", alarm.fallingThreshold, alarm.fallingEventIndex);

        if (alarm.alarmType == RMON_ALARM_RISING)
            len += osal_sprintf(cmd + len, "startup rising ");
        else if (alarm.alarmType == RMON_ALARM_FALLING)
            len += osal_sprintf(cmd + len, "startup falling ");
        else
            len += osal_sprintf(cmd + len, "startup rising-falling ");

        if(0 != osal_strlen(alarm.owner))
            len += osal_sprintf(cmd + len, "owner \"%s\" ", alarm.owner);

        len += osal_sprintf(cmd + len, "\n");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

static int32 _cmd_rmon_history_save(FILE *fp)
{
    uint32 i = 0;
    uint32 len = 0;
    sal_rmon_history_t history;
    char cmd[MAX_CMD_STRLEN];
    char portStr[CAPA_PORT_STR_LEN];

    for (i = 0; i < CAPA_RMON_HISTORY_NUM; i++)
    {
        osal_memset(&history, 0 , sizeof(sal_rmon_history_t));
        history.arrayIndex = i;
        SYS_ERR_CHK(sal_rmon_history_get(&history));

        if(0 == history.historyIndex)
            continue;

        osal_memset(cmd, 0, sizeof(cmd));
        SYS_MEM_CLEAR(portStr);
        LP2STR(history.datasource, portStr);
        len = 0;

        len += osal_sprintf(cmd + len, "rmon history %u ", history.historyIndex);
        len += osal_sprintf(cmd + len, "interface %s ", portStr);

        if(RMON_HISTORY_BUCKET_DFL != history.bucketRequested)
            len += osal_sprintf(cmd + len, "buckets %u ", history.bucketRequested);

         if(RMON_HISTORY_INTERVAL_DFL != history.interval)
            len += osal_sprintf(cmd + len, "interval %u ", history.interval);

        if(0 != osal_strlen(history.owner))
            len += osal_sprintf(cmd + len, "owner \"%s\" ", history.owner);

        len += osal_sprintf(cmd + len, "\n");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_rmon_save(FILE *fp)
{
    SYS_ERR_CHK(_cmd_rmon_event_save(fp));

    SYS_ERR_CHK(_cmd_rmon_alarm_save(fp));

    SYS_ERR_CHK(_cmd_rmon_history_save(fp));

    return SYS_ERR_OK;
}

