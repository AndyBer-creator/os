
#include <libsal/sal_util.h>
#include <libsal/sal_time.h>
#ifdef CONFIG_SYS_POE
#include <libsal/sal_poe.h>
#endif

int32 sal_time_set(sys_time_t *systime)
{
    SYS_ERR_CHK(cfg_time_set(systime));

#ifdef CONFIG_SYS_APP_MSNTP
    system("handler -u time &");
#endif

    system("killall -q -SIGFPE timed");

#ifdef CONFIG_SYS_POE
	sal_poe_timeZone_set(systime->tzdiff);
#endif
    return SYS_ERR_OK;
}

int32 sal_default_time_set(void)
{
    sys_time_t systime;
    char timezone[12];

    osal_memset(&systime, 0 , sizeof(sys_time_t));
    SYS_MEM_CLEAR(timezone);

#ifdef CONFIG_SYS_APP_MSNTP
    systime.sntpEnable  = DFLT_SYS_SNTP_EBL;
    systime.sntpPort = DFLT_SYS_SNTP_PORT;
#endif
    systime.tzdiff = DFLT_SYS_TIME_ZONE;
    sprintf(systime.tzAcronym, "%s", DFLT_SYS_TIME_ZONE_ACRONYM);

    SYS_ERR_CHK(sal_time_set(&systime));
    
    return SYS_ERR_OK;
}

int32 sal_time_get(sys_time_t *systime)
{
    SYS_ERR_CHK(cfg_time_get(systime));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_APP_MSNTP

int32 sal_time_update(uint8 isFromPoold)
{
    FILE *fp = NULL;
    char cmd[SYS_BUF128_LEN];
    int ret = 0;
    sys_time_t systime;
    sys_time_str_t timeStr;

    SYS_MEM_CLEAR(cmd);
    SYS_MEM_CLEAR(systime);
    SYS_ERR_CHK(sal_time_get(&systime));

    if(SNTP_DISABLED == systime.sntpEnable)
        return SYS_ERR_OK;

    if(0 == osal_strlen(systime.sntpServer))
        return SYS_ERR_OK;

    if (0 != sal_util_processNum_ret("msntp"))
    {
        sal_util_process_kill("msntp");
        sleep(2);
    }

    if (0 == systime.sntpPort)
        osal_sprintf(cmd, "msntp -r -c 1 -E 1 %s 1> /dev/null 2> /dev/null", systime.sntpServer);
    else
        osal_sprintf(cmd, "msntp -r -c 1 -E 1 -p %d %s 1> /dev/null 2> /dev/null", systime.sntpPort, systime.sntpServer);

    ret = system(cmd);

    systime.sntp_attemps++;

    if(0 != ret)
    {
        systime.sntp_failures++;

        fp = fopen(SNTP_TRY_REASON_FILE, "r");
        if (NULL != fp)
        {
            SYS_MEM_CLEAR(cmd);
            fgets(cmd, sizeof(cmd), fp);
            fclose(fp);
        }

        if (0 == osal_strcmp(cmd, "Bad data encoded"))
            SYS_LOG(LOG_CAT_TIME, LOG_TIME_PKT_ERR);
    }
    else
    {
        time((time_t *)(&systime.sntp_time));

        if (!isFromPoold)
        {
            SYS_MEM_CLEAR(timeStr);
            SYS_ERR_CHK(sal_time_currentTimeStr_get(&timeStr));
            SYS_LOG(LOG_CAT_TIME, LOG_TIME_TIME_SYNC, timeStr.tstr, systime.sntpServer);
        }
    }

    SYS_ERR_CHK(sal_time_sntp_reason_set(&systime));

    return SYS_ERR_OK;
}

int32 sal_time_sntp_reason_set(sys_time_t *systime)
{
    SYS_ERR_CHK(cfg_time_sntp_reason_set(systime));

    return SYS_ERR_OK;
}
#endif

int32 sal_timezoneStr_get(char *tzStr)
{
    char timeDiffStr[SYS_BUF16_LEN];
    sys_time_t systime;
    int min = 0;
    int timediff = 0;
    time_t now_time;

    SYS_MEM_CLEAR(timeDiffStr);

    SYS_ERR_CHK(sal_time_get(&systime));
    SYS_ERR_CHK(sal_time_dls_convert(&systime));

    if (0 == osal_strlen(systime.tzAcronym))
    {
        osal_sprintf(tzStr, "UTC+0");
    }
    else
    {
        if (systime.tzdiff < 0)
            osal_sprintf(tzStr, "UTC%d", systime.tzdiff/60);
        else
            osal_sprintf(tzStr, "UTC+%d", systime.tzdiff/60);

        if (0 != systime.tzdiff%60)
        {
            min = systime.tzdiff%60;

            if (min < 0)
                min *= -1;

            osal_sprintf(tzStr + osal_strlen(tzStr), ":%02u", min);
        }
    }

    time(&now_time);

    timediff += systime.tzdiff;
    now_time += timediff*60;
    if (0 != systime.dls.dls_offset)
    {
        if(now_time > systime.dls.dls_start_time && now_time < systime.dls.dls_end_time)
        {
            timediff += (systime.dls.dls_offset);
            now_time += systime.dls.dls_offset *60;
        }
    }

    if (timediff < 0)
        osal_sprintf(timeDiffStr, "UTC%d", timediff/60);
    else
        osal_sprintf(timeDiffStr, "UTC+%d", timediff/60);

    if (0 != timediff%60)
    {
        min = timediff%60;

        if (min < 0)
            min *= -1;

        osal_sprintf(timeDiffStr + osal_strlen(timeDiffStr), ":%02u", min);
    }

    return SYS_ERR_OK;
}

int32 sal_time_staticTimeInt_get(uint32 inTime, uint32 *pOutTime)
{
    sys_time_t systime;

    SYS_ERR_CHK(sal_time_get(&systime));
    SYS_ERR_CHK(sal_time_dls_convert(&systime));

    inTime += systime.tzdiff * 60;

    if (0 != systime.dls.dls_offset)
    {
        if(inTime > systime.dls.dls_start_time && inTime < systime.dls.dls_end_time)
            inTime += systime.dls.dls_offset *60;
    }

    *pOutTime = inTime;

    return SYS_ERR_OK;
}

int32 sal_time_currentTimeInt_get(uint32 *pTime)
{
    time_t now_time;
    time(&now_time);

    sal_time_staticTimeInt_get(now_time, pTime);

    return SYS_ERR_OK;
}

int32 sal_time_currentTimeStr_get(sys_time_str_t *str)
{
    char tmp[SYS_BUF16_LEN];
    char timeDiffStr[SYS_BUF16_LEN];
    char isDlsUsed = FALSE;
    int min = 0;
    int timediff = 0;
    sys_time_t systime;
    time_t now_time;
    struct tm *tm;

    SYS_MEM_CLEAR(tmp);
    SYS_MEM_CLEAR(timeDiffStr);

    SYS_ERR_CHK(sal_time_get(&systime));
    SYS_ERR_CHK(sal_time_dls_convert(&systime));

    if (0 == systime.tzdiff)
    {
        osal_sprintf(str->tzstr, "UTC+0");
    }
    else
    {
        if (systime.tzdiff < 0)
            osal_sprintf(str->tzstr, "UTC%d", systime.tzdiff/60);
        else
            osal_sprintf(str->tzstr, "UTC+%d", systime.tzdiff/60);

        if (0 != systime.tzdiff%60)
        {
            min = systime.tzdiff%60;

            if (min < 0)
                min *= -1;

            osal_sprintf(str->tzstr + osal_strlen(str->tzstr), ":%02u", min);
        }
    }

    time(&now_time);

    timediff += systime.tzdiff;
    now_time += timediff*60;
    if (0 != systime.dls.dls_offset)
    {
        if(now_time > systime.dls.dls_start_time && now_time < systime.dls.dls_end_time)
        {
            timediff += (systime.dls.dls_offset);
            now_time += systime.dls.dls_offset *60;
            isDlsUsed = TRUE;
        }
    }

    if (timediff < 0)
        osal_sprintf(timeDiffStr, "UTC%d", timediff/60);
    else
        osal_sprintf(timeDiffStr, "UTC+%d", timediff/60);

    if (0 != timediff%60)
    {
        min = timediff%60;

        if (min < 0)
            min *= -1;

        osal_sprintf(timeDiffStr + osal_strlen(timeDiffStr), ":%02u", min);
    }

    tm = gmtime(&now_time);

    SYS_MEM_CLEAR(tmp);
    strftime(tmp, 16, "%Y-%m-%d", tm);
    osal_sprintf(str->tstr + osal_strlen(str->tstr), "%s ", tmp);

    strftime(tmp, 16, "%T" , tm);
    osal_sprintf(str->tstr + osal_strlen(str->tstr), "%s ", tmp);

    if (FALSE == isDlsUsed)
    {
        if (0 == osal_strlen(systime.tzAcronym))
            osal_sprintf(str->tstr + osal_strlen(str->tstr), "%s ", str->tzstr);
        else
            osal_sprintf(str->tstr + osal_strlen(str->tstr), "%s(%s) ", systime.tzAcronym, timeDiffStr);
    }
    else
    {
        if (0 == osal_strlen(systime.dls.dlsAcronym))
            osal_sprintf(str->tstr + osal_strlen(str->tstr), "%s ", timeDiffStr);
        else
            osal_sprintf(str->tstr + osal_strlen(str->tstr), "%s(%s) ", systime.dls.dlsAcronym, timeDiffStr);

    }

    return SYS_ERR_OK;
}

int32 sal_time_dls_convert(sys_time_t *systime)
{
    time_t temp_time;
    struct tm *tm_time;
    struct tm tm_temp;
    struct tm time_s;
    struct tm time_e;

    uint32 now_year;
    uint32 m = 0;
    char temp[8] = "";

    if (SYS_TIME_DLS_TYPE_NON_RECURRING == systime->dls.dlsType)
    {
        time_s.tm_sec = 0;
        time_s.tm_min = systime->dls.dls_non_min_start;
        time_s.tm_hour = systime->dls.dls_non_hour_start;
        time_s.tm_mday = systime->dls.dls_non_date_start;
        time_s.tm_mon = systime->dls.dls_non_month_start - 1;
        time_s.tm_year = systime->dls.dls_non_year_start - 1900;
        time_s.tm_isdst = 0;

        time_e.tm_sec = 0;
        time_e.tm_min = systime->dls.dls_non_min_end;
        time_e.tm_hour = systime->dls.dls_non_hour_end;
        time_e.tm_mday = systime->dls.dls_non_date_end;
        time_e.tm_mon = systime->dls.dls_non_month_end - 1;
        time_e.tm_year = systime->dls.dls_non_year_end - 1900;
        time_e.tm_isdst = 0;

        systime->dls.dls_start_time = mktime(&time_s);
        systime->dls.dls_end_time = mktime(&time_e);
        if(systime->dls.dls_start_time >= systime->dls.dls_end_time)
            return SYS_ERR_TIME_DLS_RANGE_ERR;

        if(((systime->dls.dls_end_time - systime->dls.dls_start_time)/60) <= systime->dls.dls_offset)
            return SYS_ERR_TIME_DLS_RANGE_OFFSET_ERR;
    }
    else
    {
        time(&temp_time);
        tm_time = gmtime(&temp_time);
        strftime(temp, 8, "%Y", tm_time);
        now_year = SYS_STR2UINT(temp) - 1900;

        m = systime->dls.dls_rec_month_end;
        tm_temp.tm_sec = 0;
        tm_temp.tm_min = systime->dls.dls_rec_min_end;
        tm_temp.tm_hour = systime->dls.dls_rec_hour_end;
        tm_temp.tm_mday = 1;
        tm_temp.tm_mon = systime->dls.dls_rec_month_end - 1;
        tm_temp.tm_year = now_year;

        temp_time = mktime(&tm_temp);
        tm_time = gmtime(&temp_time);

        if(systime->dls.dls_rec_day_end < tm_time->tm_wday)
            tm_temp.tm_mday = tm_temp.tm_mday + 7 -(tm_time->tm_wday - systime->dls.dls_rec_day_end);
        else if(systime->dls.dls_rec_day_end > tm_time->tm_wday)
            tm_temp.tm_mday += (systime->dls.dls_rec_day_end - tm_time->tm_wday);

        tm_temp.tm_mday += 7 * (systime->dls.dls_rec_week_end - 1);

        if(((m == 1) || (m == 3) || (m == 5) || (m == 7) || (m == 8) || (m == 10) || (m == 12)) && (tm_temp.tm_mday > 31))
            tm_temp.tm_mday -= 7;
        else if(((m == 4) || (m == 6) || (m == 9) || (m == 11)) && (tm_temp.tm_mday > 30))
            tm_temp.tm_mday -= 7;
        else if (m == 2)
        {
            if((0 == (now_year % 4)) && (tm_temp.tm_mday > 29))
                tm_temp.tm_mday -= 7;
            else if((0 != (now_year % 4)) && tm_temp.tm_mday > 28)
                    tm_temp.tm_mday -= 7;
        }

        systime->dls.dls_end_time = mktime(&tm_temp);

        m = systime->dls.dls_rec_month_start;
        tm_temp.tm_sec = 0;
        tm_temp.tm_min = systime->dls.dls_rec_min_start;
        tm_temp.tm_hour = systime->dls.dls_rec_hour_start;
        tm_temp.tm_mday = 1;
        tm_temp.tm_mon = systime->dls.dls_rec_month_start - 1;
        tm_temp.tm_year = now_year;

        temp_time = mktime(&tm_temp);
        tm_time = gmtime(&temp_time);

        if(systime->dls.dls_rec_day_start < tm_time->tm_wday)
            tm_temp.tm_mday = tm_temp.tm_mday + 7 -(tm_time->tm_wday - systime->dls.dls_rec_day_start);
        else if(systime->dls.dls_rec_day_start > tm_time->tm_wday)
            tm_temp.tm_mday += (systime->dls.dls_rec_day_start - tm_time->tm_wday);

        tm_temp.tm_mday += 7 * (systime->dls.dls_rec_week_start - 1);

        if(((m == 1) || (m == 3) || (m == 5) || (m == 7) || (m == 8) || (m == 10) || (m == 12)) && (tm_temp.tm_mday > 31))
            tm_temp.tm_mday -= 7;
        else if(((m == 4) || (m == 6) || (m == 9) || (m == 11)) && (tm_temp.tm_mday > 30))
            tm_temp.tm_mday -=7;
        else if (m == 2)
        {
            if((0 == (now_year % 4)) && (tm_temp.tm_mday > 29))
                tm_temp.tm_mday -= 7;
            else if((0 != (now_year % 4)) && tm_temp.tm_mday > 28)
                    tm_temp.tm_mday -= 7;
        }

        systime->dls.dls_start_time = mktime(&tm_temp);

        if(systime->dls.dls_start_time > systime->dls.dls_end_time)
        {
            tm_temp.tm_year--;
            systime->dls.dls_start_time = mktime(&tm_temp);
        }
    }

    return SYS_ERR_OK;
}

int32 sal_time_timeInt2Str(time_t *time_int, char *time_str)
{
    struct tm *tm_time;

    SYS_PARAM_CHK((NULL == time_int), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == time_str), SYS_ERR_NULL_POINTER);

    tm_time = gmtime(time_int);
    strftime(time_str, 32, "%H:%M %b %d %Y" , tm_time);

    return SYS_ERR_OK;
}

int32 sal_time_time_hms_get(uint32 *pHour, uint32 *pMin, uint32 *pSec)
{
    struct tm *tm_time;
	time_t time_int;
	char time_str[10] = {0};
	char *ridx = NULL;
	uint8 i = 0;

    SYS_PARAM_CHK((NULL == pHour), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pMin), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pSec), SYS_ERR_NULL_POINTER);
    
	sal_time_currentTimeInt_get((uint32*)&time_int);
    tm_time = gmtime(&time_int);
    strftime(time_str, 10, "%T", tm_time);
    
    ridx = rindex(time_str, ':');
    if (NULL == ridx)
        return SYS_ERR_FAILED;
    *ridx = '\0';
    ridx++;
    for (i = 0; i < osal_strlen(ridx) ; i++)
    {
        if (!isdigit(ridx[i]))
            return SYS_ERR_FAILED;
    }
    *pSec = SYS_STR2UINT(ridx);

    ridx = rindex(time_str, ':');
    if (NULL == ridx)
        return SYS_ERR_FAILED;
    *ridx = '\0';
    ridx++;
    for (i = 0; i < osal_strlen(ridx) ; i++)
    {
        if (!isdigit(ridx[i]))
            return SYS_ERR_FAILED;
    }
    *pMin = SYS_STR2UINT(ridx);

    *pHour = SYS_STR2UINT(time_str);

    return SYS_ERR_OK;
}

