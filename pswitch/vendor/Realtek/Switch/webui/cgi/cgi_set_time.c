
#include <include/cgi.h>
#include <libclicmd/cmd_time.h>

static int32 cgi_sys_time_set(void);

const cgi_cmdmap_t cmdSetTimeTbl[] =
{
    {CGI_CMD_TIME_TIME,                 cgi_sys_time_set},
    {NULL, NULL}
};

static int32 cgi_sys_time_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
#ifdef CONFIG_SYS_APP_MSNTP
    time_t usr_time;
    struct tm tm_time;
#endif
    sys_time_t systime;

    SYS_MEM_CLEAR(systime);
    sal_time_get(&systime);

    if (CGI_IS_PARAM_EXIST(CGI_SYS_TIME_SNTP, value))
    {
		switch(SYS_STR2UINT(value))
		{
			case 0:
				systime.sntpEnable = SNTP_DISABLED;
				break;
			case 1:
				systime.sntpEnable = SNTP_ENABLED;
				break;
			case 2:
				systime.sntpEnable = SNTP_COMPUTER;
				break;
			default:
				goto cgi_end;
		}
    }        
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_SYS_TIME_TZ, value))
        systime.tzdiff = atoi(value);

    if (CGI_IS_PARAM_EXIST(CGI_SYS_TIME_DS_TYPE, value))
        systime.dls.dlsType = SYS_STR2UINT(value);

#ifdef CONFIG_SYS_APP_MSNTP
    if (systime.sntpEnable == SNTP_DISABLED || systime.sntpEnable == SNTP_COMPUTER)
    {
        SYS_MEM_CLEAR(tm_time);

        if (CGI_IS_PARAM_EXIST(CGI_SYS_TIME_DATE, value))
        {
            tm_time.tm_year = atoi(value) - 1900;
            value = index(value, '-');
            tm_time.tm_mon = atoi(++value) - 1;
            value = index(value, '-');
            tm_time.tm_mday = atoi(++value);
        }

        if (CGI_IS_PARAM_EXIST(CGI_SYS_TIME_TIME, value))
        {
            tm_time.tm_hour = atoi(value);
            value = index(value, ':');
            tm_time.tm_min = atoi(++value);
            value = index(value, ':');
            tm_time.tm_sec = atoi(++value);
        }

        usr_time = mktime(&tm_time);

        if (0 != systime.tzdiff)
            usr_time -= systime.tzdiff * 60;

        stime(&usr_time);
    }
    else
    {
        if (CGI_IS_PARAM_EXIST(CGI_SYS_TIME_SRVHOST, value))
        osal_sprintf(systime.sntpServer, "%s",  value);

        if (CGI_IS_PARAM_EXIST(CGI_SYS_TIME_SRVPORT, value))
            systime.sntpPort = SYS_STR2UINT(value);
        else
            systime.sntpPort = DFLT_SYS_SNTP_PORT;
    }
#endif

    if (SYS_TIME_DLS_TYPE_DISABLED != systime.dls.dlsType)
    {
        if (CGI_IS_PARAM_EXIST(CGI_SYS_TIME_DS_OFFSET, value))
            systime.dls.dls_offset = SYS_STR2UINT(value);

        if (systime.dls.dlsType == SYS_TIME_DLS_TYPE_USA)
        {
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
        }
        else if (systime.dls.dlsType == SYS_TIME_DLS_TYPE_EUROPEAN)
        {
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
        }
        else if (systime.dls.dlsType == SYS_TIME_DLS_TYPE_RECURRING)
        {
            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_REC_DAYS, value))
                systime.dls.dls_rec_day_start = atoi(value);

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_REC_WEEKS, value))
                systime.dls.dls_rec_week_start = atoi(value);

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_REC_MONTHS, value))
                systime.dls.dls_rec_month_start = atoi(value);

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_REC_TIMES, value))
            {
                
                GET_HM(value, systime.dls.dls_rec_min_start, systime.dls.dls_rec_hour_start);
            }

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_REC_DAYE, value))
                systime.dls.dls_rec_day_end = atoi(value);

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_REC_WEEKE, value))
                systime.dls.dls_rec_week_end = atoi(value);

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_REC_MONTHE, value))
                systime.dls.dls_rec_month_end = atoi(value);

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_REC_TIMEE, value))
            {
                
                GET_HM(value, systime.dls.dls_rec_min_end, systime.dls.dls_rec_hour_end);
            }
        }
        else if (systime.dls.dlsType == SYS_TIME_DLS_TYPE_NON_RECURRING)
        {
            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_STATIC_DATES, value))
            {
                
                systime.dls.dls_non_year_start = atoi(value);
                value = index(value, '-');
                systime.dls.dls_non_month_start = atoi(++value);
                value = index(value, '-');
                systime.dls.dls_non_date_start = atoi(++value);
            }

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_STATIC_TIMES, value))
            {
                
                GET_HM(value, systime.dls.dls_non_min_start, systime.dls.dls_non_hour_start);
            }

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_STATIC_DATEE, value))
            {
                
                systime.dls.dls_non_year_end = atoi(value);
                value = index(value, '-');
                systime.dls.dls_non_month_end = atoi(++value);
                value = index(value, '-');
                systime.dls.dls_non_date_end = atoi(++value);
            }

            if (CGI_IS_PARAM_EXIST(CGI_SYS_DLS_STATIC_TIMEE, value))
            {
                
                GET_HM(value, systime.dls.dls_non_min_end, systime.dls.dls_non_hour_end);
            }
        }

        if (systime.dls.dlsType == SYS_TIME_DLS_TYPE_NON_RECURRING)
        {
            systime.dls.dls_rec_week_start = 0;
            systime.dls.dls_rec_day_start = 0;
            systime.dls.dls_rec_month_start = 0;
            systime.dls.dls_rec_hour_start = 0;
            systime.dls.dls_rec_min_start = 0;
            systime.dls.dls_rec_week_end = 0;
            systime.dls.dls_rec_day_end = 0;
            systime.dls.dls_rec_month_end = 0;
            systime.dls.dls_rec_hour_end = 0;
            systime.dls.dls_rec_min_end = 0;
        }
        else
        {
            systime.dls.dls_non_year_start = 0;
            systime.dls.dls_non_month_start = 0;
            systime.dls.dls_non_date_start = 0;
            systime.dls.dls_non_hour_start = 0;
            systime.dls.dls_non_min_start = 0;
            systime.dls.dls_non_year_end = 0;
            systime.dls.dls_non_month_end = 0;
            systime.dls.dls_non_date_end = 0;
            systime.dls.dls_non_hour_end = 0;
            systime.dls.dls_non_min_end = 0;
        }
    }
    else
    {
        SYS_MEM_CLEAR(systime.dls);
    }

    CGI_SET_ERR_HDL(sal_time_dls_convert(&systime), ret);
    CGI_SET_ERR_HDL(sal_time_set(&systime), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

