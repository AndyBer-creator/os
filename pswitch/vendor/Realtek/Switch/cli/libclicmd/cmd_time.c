
#include <libcmd/cmd.h>
#include <libsal/sal_time.h>
#include <libsal/sal_util.h>

static char days[7][4] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
static char mons[12][4] = {"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};

int32 cmd_time_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint8 i = 0;
    char *ridx = NULL;
    char *tmp = NULL;
    char buf[SYS_BUF64_LEN];
    struct tm tm_time;
    time_t usr_time;

    sys_time_t systime;
    sys_time_str_t timeStr;

    SYS_MEM_CLEAR(systime);
    SYS_MEM_CLEAR(timeStr);
    SYS_MEM_CLEAR(tm_time);
    SYS_MEM_CLEAR(buf);

    tmp = argv[0];
    ridx = rindex(tmp, ':');

    if (NULL == ridx)
        return SYS_ERR_INPUT;

    *ridx = '\0';
    ridx++;
    for (i = 0; i < osal_strlen(ridx) ; i++)
    {
        if (!isdigit(ridx[i]))
            return SYS_ERR_INPUT;
    }
    tm_time.tm_sec = SYS_STR2UINT(ridx);

    if ( tm_time.tm_sec > 59 )
        return SYS_ERR_INPUT;

    ridx = rindex(tmp, ':');

    if (NULL == ridx)
        return SYS_ERR_INPUT;

    *ridx = '\0';
    ridx++;
    for (i = 0; i < osal_strlen(ridx) ; i++)
    {
        if (!isdigit(ridx[i]))
            return SYS_ERR_INPUT;
    }
    tm_time.tm_min = SYS_STR2UINT(ridx);

    if ( tm_time.tm_min > 59 )
        return SYS_ERR_INPUT;

    tm_time.tm_hour = SYS_STR2UINT(tmp);
    for (i = 0; i < osal_strlen(tmp) ; i++)
    {
        if (!isdigit(tmp[i]))
            return SYS_ERR_INPUT;
    }

    if ( tm_time.tm_hour > 23 )
        return SYS_ERR_INPUT;

    GET_MONTH(argv[1], tm_time.tm_mon);

    tm_time.tm_year = SYS_STR2UINT(argv[3]) - 1900;
    tm_time.tm_mday = SYS_STR2UINT(argv[2]);

    usr_time = mktime(&tm_time);

    SYS_ERR_CHK(sal_time_get(&systime));

    if (0 != systime.tzdiff)
        usr_time -= systime.tzdiff * 60;

    stime(&usr_time);

    SYS_ERR_CHK(sal_time_currentTimeStr_get(&timeStr));
    XMORE("%s\n", timeStr.tstr);

    return SYS_ERR_OK;
}

int32 cmd_time_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_time_str_t timeStr;
    sys_time_t systime;

    SYS_MEM_CLEAR(timeStr);
    SYS_MEM_CLEAR(systime);
    SYS_ERR_CHK(sal_time_get(&systime));
    SYS_ERR_CHK(sal_time_currentTimeStr_get(&timeStr));

    XMORE("\n%s\n", timeStr.tstr);

	switch(systime.sntpEnable)
	{
		case SNTP_DISABLED:
			XMORE("Time set manually\n\n");
			break;
		case SNTP_ENABLED:
			XMORE("Time source is sntp\n\n");
			break;
		case SNTP_COMPUTER:
			XMORE("Time source is computer\n\n");
			break;
		default:
			return SYS_ERR_OK;
	}

    if (2 == CMD_TOKENNUM_GET(cmd->vline))
        return SYS_ERR_OK;

    XMORE("Time zone:\n");
    XMORE("Acronym is %s\n", systime.tzAcronym);
    XMORE("Offset is %s\n\n", timeStr.tzstr);

    if (0 == systime.dls.dls_offset)
        return SYS_ERR_OK;

    XMORE("Summertime:\n");
    XMORE("Acronym is %s\n", systime.dls.dlsAcronym);

    if (SYS_TIME_DLS_TYPE_NON_RECURRING == systime.dls.dlsType)
    {
        XMORE("Starting and ending on a specific date.\n");
        XMORE("Begins at %u %u %u %u:%u\n", systime.dls.dls_non_month_start, systime.dls.dls_non_date_start,
            systime.dls.dls_non_year_start-2000, systime.dls.dls_non_hour_start, systime.dls.dls_non_min_start);
        XMORE("Ends at %u %u %u %u:%u\n", systime.dls.dls_non_month_end, systime.dls.dls_non_date_end,
            systime.dls.dls_non_year_end-2000, systime.dls.dls_non_hour_end, systime.dls.dls_non_min_end);

        XMORE("Offset is %u minutes.\n", systime.dls.dls_offset);
    }
    else
    {
        XMORE("Recurring every year.\n");
        XMORE("Begins at %u %u %u %u:%u\n", systime.dls.dls_rec_week_start, systime.dls.dls_rec_day_start,
            systime.dls.dls_rec_month_start, systime.dls.dls_rec_hour_start, systime.dls.dls_rec_min_start);
        XMORE("Ends at %u %u %u %u:%u\n", systime.dls.dls_rec_week_end, systime.dls.dls_rec_day_end,
            systime.dls.dls_rec_month_end, systime.dls.dls_rec_hour_end, systime.dls.dls_rec_min_end);
        XMORE("Offset is %u minutes.\n", systime.dls.dls_offset);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_APP_MSNTP

int32 cmd_time_sntp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_time_t systime;

    SYS_MEM_CLEAR(systime);
    SYS_ERR_CHK(sal_time_get(&systime));

    if (SNTP_DISABLED == systime.sntpEnable)
        XMORE("SNTP is Disabled\n");
    else
        XMORE("SNTP is Enabled\n");

    XMORE("SNTP Server address: %s\n", systime.sntpServer);
    XMORE("SNTP Server port: %u\n\n", systime.sntpPort);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_APP_MSNTP

int32 cmd_time_sntpEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_time_t systime;
    SYS_ERR_CHK(sal_time_get(&systime));

    if ('l' == argv[0][0])
    {
       systime.sntpEnable = SNTP_DISABLED;
       SYS_LOG(LOG_CAT_TIME, LOG_TIME_STOP);
    }
    else
    {
       systime.sntpEnable = SNTP_ENABLED;
       if (0 != osal_strlen(systime.sntpServer))
            SYS_LOG(LOG_CAT_TIME, LOG_TIME_START);
    }

    SYS_ERR_CHK(sal_time_set(&systime));

    return SYS_ERR_OK;
}

int32 cmd_time_sntp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char tmp[SYS_BUF64_LEN];
    sys_time_t systime;
    SYS_ERR_CHK(sal_time_get(&systime));

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_MEM_CLEAR(systime.sntpServer);
        systime.sntpPort = DFLT_SYS_SNTP_PORT;

        SYS_LOG(LOG_CAT_TIME, LOG_TIME_STOP);
    }
    else
    {
        if (osal_strlen(argv[0]) > (sizeof(systime.sntpServer) - 1))
            return SYS_ERR_INPUT;

        if (!sal_util_hostnameStrValidate_ret(argv[0]))
            return SYS_ERR_INPUT;

        osal_strcpy(systime.sntpServer, argv[0]);

        if (2 == argc)
           systime.sntpPort = SYS_STR2UINT(argv[1]);
        else
           systime.sntpPort = DFLT_SYS_SNTP_PORT;

        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "ping -c 1 %s 1> /dev/null 2> /dev/null", systime.sntpServer);

        if (0 == system(tmp))
        {
            SYS_LOG(LOG_CAT_TIME, LOG_TIME_CONN_SUCCESS, systime.sntpServer);

            if (SNTP_ENABLED == systime.sntpEnable)
                SYS_LOG(LOG_CAT_TIME, LOG_TIME_START);
        }
        else
        {
            SYS_LOG(LOG_CAT_TIME, LOG_TIME_CONN_FAIL, systime.sntpServer);
        }
    }

    SYS_ERR_CHK(sal_time_set(&systime));

    return SYS_ERR_OK;
}
#endif

int32 cmd_time_timezone_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    int8 hour = 0;
    uint8 min = 0;
    sys_time_t systime;

    SYS_MEM_CLEAR(systime);
    SYS_ERR_CHK(sal_time_get(&systime));

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_MEM_CLEAR(systime.tzAcronym);
        sprintf(systime.tzAcronym, "%s", DFLT_SYS_TIME_ZONE_ACRONYM);
        systime.tzIdx = 0;
        systime.tzdiff= DFLT_SYS_TIME_ZONE;
    }
    else
    {
        if (osal_strlen(argv[0]) > 4)
            return SYS_ERR_OUT_OF_RANGE;

        if ((1 == osal_strlen(argv[1])) && !isdigit(argv[1][0]))
            return SYS_ERR_INPUT;

        if (2 == osal_strlen(argv[1]))
        {
            if((!isdigit(argv[1][0])) && ('+' != argv[1][0]) && ('-' != argv[1][0]))
                return SYS_ERR_INPUT;

            if (!isdigit(argv[1][1]))
                return SYS_ERR_INPUT;
        }

        if (3 == osal_strlen(argv[1]))
        {
            if(('+' != argv[1][0]) && ('-' != argv[1][0]))
                return SYS_ERR_INPUT;

            if (!isdigit(argv[1][1]) || !isdigit(argv[1][2]))
                return SYS_ERR_INPUT;
        }

        hour = atoi(argv[1]);

        if ((hour < -12) || (hour > 13))
            return SYS_ERR_INPUT;

        if (3 == argc)
            min = SYS_STR2UINT(argv[2]);

        systime.tzdiff = 0;

        if (hour < 0)
            systime.tzdiff = (hour*60) - min;
        else
            systime.tzdiff = systime.tzdiff + (hour*60) + min;

        osal_strcpy(systime.tzAcronym, argv[0]);
    }

    SYS_ERR_CHK(sal_time_set(&systime));

    return SYS_ERR_OK;
}

int32 cmd_time_dls_date_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_time_t systime;

    if (osal_strlen(argv[0]) > 4)
        return SYS_ERR_OUT_OF_RANGE;

    SYS_ERR_CHK(sal_time_get(&systime));

    SYS_MEM_CLEAR(systime.dls.dlsAcronym);

    systime.dls.dlsType = SYS_TIME_DLS_TYPE_NON_RECURRING;

    osal_strcpy(systime.dls.dlsAcronym, argv[0]);

    GET_MONTH(argv[1], systime.dls.dls_non_month_start);
    systime.dls.dls_non_month_start++;
    systime.dls.dls_non_date_start = SYS_STR2UINT(argv[2]);
    systime.dls.dls_non_year_start = SYS_STR2UINT(argv[3]);

    GET_HM(argv[4], systime.dls.dls_non_min_start, systime.dls.dls_non_hour_start);

    GET_MONTH(argv[5], systime.dls.dls_non_month_end);
    systime.dls.dls_non_month_end++;
    systime.dls.dls_non_date_end = SYS_STR2UINT(argv[6]);
    systime.dls.dls_non_year_end = SYS_STR2UINT(argv[7]);

    GET_HM(argv[8], systime.dls.dls_non_min_end, systime.dls.dls_non_hour_end);

    if (10 == argc)
        systime.dls.dls_offset = SYS_STR2UINT(argv[9]);
    else
        systime.dls.dls_offset = 60;

    SYS_ERR_CHK(sal_time_dls_convert(&systime));
    SYS_ERR_CHK(sal_time_set(&systime));

    return SYS_ERR_OK;
}

int32 cmd_time_dls_rec_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_time_t systime;

    SYS_ERR_CHK(sal_time_get(&systime));

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_MEM_CLEAR(systime.dls);

        SYS_ERR_CHK(sal_time_set(&systime));

        return SYS_ERR_OK;
    }

    if (osal_strlen(argv[0]) > 4)
        return SYS_ERR_OUT_OF_RANGE;

    osal_strcpy(systime.dls.dlsAcronym, argv[0]);

    if ('u' == argv[1][0])
    {
        systime.dls.dlsType = SYS_TIME_DLS_TYPE_USA;

        systime.dls.dls_rec_week_start = 2;
        systime.dls.dls_rec_day_start = 0;
        systime.dls.dls_rec_month_start = 3;
        systime.dls.dls_rec_hour_start = 2;
        systime.dls.dls_rec_min_start = 0;

        systime.dls.dls_rec_week_end = 1;
        systime.dls.dls_rec_day_end = 0;
        systime.dls.dls_rec_month_end = 11;
        systime.dls.dls_rec_hour_end = 2;
        systime.dls.dls_rec_min_end = 0;

        if (3 == argc)
            systime.dls.dls_offset = SYS_STR2UINT(argv[2]);
        else
            systime.dls.dls_offset = 60;
    }
    else if ('e' == argv[1][0])
    {
        systime.dls.dlsType = SYS_TIME_DLS_TYPE_EUROPEAN;

        systime.dls.dls_rec_week_start = 5;
        systime.dls.dls_rec_day_start = 0;
        systime.dls.dls_rec_month_start = 3;
        systime.dls.dls_rec_hour_start = 1;
        systime.dls.dls_rec_min_start = 0;

        systime.dls.dls_rec_week_end = 5;
        systime.dls.dls_rec_day_end = 0;
        systime.dls.dls_rec_month_end = 10;
        systime.dls.dls_rec_hour_end = 1;
        systime.dls.dls_rec_min_end = 0;

        if (3 == argc)
            systime.dls.dls_offset = SYS_STR2UINT(argv[2]);
        else
            systime.dls.dls_offset = 60;
    }
    else
    {
        systime.dls.dlsType = SYS_TIME_DLS_TYPE_RECURRING;

        if ('f' == argv[1][0])
            systime.dls.dls_rec_week_start = 1;
        else if ('l' == argv[1][0])
            systime.dls.dls_rec_week_start = 5;
        else
            systime.dls.dls_rec_week_start = SYS_STR2UINT(argv[1]);

        GET_WEEKDAY(argv[2], systime.dls.dls_rec_day_start);
        GET_MONTH(argv[3], systime.dls.dls_rec_month_start);
        systime.dls.dls_rec_month_start++;
        GET_HM(argv[4], systime.dls.dls_rec_min_start, systime.dls.dls_rec_hour_start);

        if ('f' == argv[5][0])
            systime.dls.dls_rec_week_end = 1;
        else if ('l' == argv[5][0])
            systime.dls.dls_rec_week_end = 5;
        else
            systime.dls.dls_rec_week_end = SYS_STR2UINT(argv[5]);

        GET_WEEKDAY(argv[6], systime.dls.dls_rec_day_end);
        GET_MONTH(argv[7], systime.dls.dls_rec_month_end);
        systime.dls.dls_rec_month_end++;
        GET_HM(argv[8], systime.dls.dls_rec_min_end, systime.dls.dls_rec_hour_end);

        if (10 == argc)
                systime.dls.dls_offset = SYS_STR2UINT(argv[9]);
            else
                systime.dls.dls_offset = 60;
    }

    SYS_ERR_CHK(sal_time_dls_convert(&systime));
    SYS_ERR_CHK(sal_time_set(&systime));

    return SYS_ERR_OK;
}

int32 cmd_time_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_time_t systime;
    uint8 len = 0;

    SYS_MEM_CLEAR(systime);
    SYS_ERR_CHK(sal_time_get(&systime));

#ifdef CONFIG_SYS_APP_MSNTP
    
    if (DFLT_SYS_SNTP_EBL != systime.sntpEnable)
    {
        SYS_MEM_CLEAR(cmd);

        if(SNTP_ENABLED == systime.sntpEnable)
            osal_sprintf(cmd, "clock source sntp\n");
        else
            osal_sprintf(cmd, "clock source local\n");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (0 != osal_strlen(systime.sntpServer))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "sntp host %s port %d\n", systime.sntpServer, systime.sntpPort);
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif
    
    if (DFLT_SYS_TIME_ZONE != systime.tzdiff)
    {
        int32 min = 0;

        min = systime.tzdiff%60;

        if (min < 0)
            min *= -1;

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "clock timezone \"%s\" %d minutes %d\n", systime.tzAcronym, systime.tzdiff/60, min);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (0 != systime.dls.dls_offset)
    {
        SYS_MEM_CLEAR(cmd);

        if (SYS_TIME_DLS_TYPE_USA == systime.dls.dlsType)
        {
            len += osal_sprintf(cmd + len, "clock summer-time \"%s\" recurring %s ", systime.dls.dlsAcronym, "usa");
        }
        else if (SYS_TIME_DLS_TYPE_EUROPEAN == systime.dls.dlsType)
        {
            len += osal_sprintf(cmd + len, "clock summer-time \"%s\" recurring %s ", systime.dls.dlsAcronym, "eu");
        }
        else if (SYS_TIME_DLS_TYPE_RECURRING == systime.dls.dlsType)
        {
            len += osal_sprintf(cmd + len, "clock summer-time \"%s\" recurring %u %s %s %u:%u %u %s %s %u:%u ",
                systime.dls.dlsAcronym,
                systime.dls.dls_rec_week_start,  days[systime.dls.dls_rec_day_start],
                mons[systime.dls.dls_rec_month_start-1], systime.dls.dls_rec_hour_start,
                systime.dls.dls_rec_min_start , systime.dls.dls_rec_week_end,
                days[systime.dls.dls_rec_day_end], mons[systime.dls.dls_rec_month_end-1],
                systime.dls.dls_rec_hour_end,  systime.dls.dls_rec_min_end);
        }
        else if (SYS_TIME_DLS_TYPE_NON_RECURRING == systime.dls.dlsType)
        {
            len += osal_sprintf(cmd + len, "clock summer-time \"%s\" date %s %u %u %u:%u %s %u %u %u:%u ",
                systime.dls.dlsAcronym,
                mons[systime.dls.dls_non_month_start-1], systime.dls.dls_non_date_start,
                systime.dls.dls_non_year_start, systime.dls.dls_non_hour_start, systime.dls.dls_non_min_start,
                mons[systime.dls.dls_non_month_end-1], systime.dls.dls_non_date_end,
                systime.dls.dls_non_year_end, systime.dls.dls_non_hour_end, systime.dls.dls_non_min_end);
        }

        if (60 != systime.dls.dls_offset)
            len += osal_sprintf(cmd + len, "%u\n", systime.dls.dls_offset);
        else
            len += osal_sprintf(cmd + len, "\n");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

