
#include <sys/sysinfo.h>
#include <include/cgi.h>
#include <libsal/sal_time.h>

static int32 cgi_time_time_get(void);

const cgi_cmdmap_t cmdGetTimeTbl[] =
{
    {CGI_CMD_TIME_TIME,                 cgi_time_time_get},
    {NULL, NULL}
};

static int32 cgi_time_time_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    uint32 timeInt = 0;
    sys_time_t systime;
    sys_time_str_t timeStr;
    char tmp[SYS_BUF16_LEN];

    SYS_MEM_CLEAR(systime);
    SYS_MEM_CLEAR(timeStr);
    sal_time_get(&systime);
    sal_time_currentTimeStr_get(&timeStr);
    sal_time_currentTimeInt_get(&timeInt);

    CGI_GET_JSON_CREATE(root, data);

	cJSON_AddNumberToObject(data, CGI_SYS_TIME_SNTP, systime.sntpEnable);
    cJSON_AddBoolToObject(data, CGI_SYS_TIME_ISV4, sal_util_ipv4StrValidate_ret(systime.sntpServer, SYS_IP_TYPE_UNICAST));
    cJSON_AddStringToObject(data, CGI_SYS_TIME_SRVHOST, systime.sntpServer);
    cJSON_AddNumberToObject(data, CGI_SYS_TIME_SRVPORT, systime.sntpPort);
    cJSON_AddNumberToObject(data, CGI_SYS_TIME_DEFPORT, 123);
    cJSON_AddNumberToObject(data, CGI_SYS_TIME_TIME, timeInt);
    cJSON_AddStringToObject(data, CGI_SYS_TIME_TIMESTR, timeStr.tstr);
    cJSON_AddNumberToObject(data, CGI_SYS_TIME_TZ, systime.tzdiff);
    cJSON_AddNumberToObject(data, CGI_SYS_TIME_DS_TYPE, systime.dls.dlsType);
    cJSON_AddNumberToObject(data, CGI_SYS_TIME_DS_OFFSET, (systime.dls.dls_offset == 0) ? 60 : systime.dls.dls_offset);

    if (systime.dls.dlsType == SYS_TIME_DLS_TYPE_NON_RECURRING)
    {
        sal_time_dls_convert(&systime);
        cJSON_AddNumberToObject(data, "dlsStaticSecS", systime.dls.dls_start_time);
        cJSON_AddNumberToObject(data, "dlsStaticSecE", systime.dls.dls_end_time);

    }
    else
    {
        cJSON_AddNumberToObject(data, CGI_SYS_DLS_REC_DAYS, systime.dls.dls_rec_day_start);
        cJSON_AddNumberToObject(data, CGI_SYS_DLS_REC_WEEKS, systime.dls.dls_rec_week_start);
        cJSON_AddNumberToObject(data, CGI_SYS_DLS_REC_MONTHS, systime.dls.dls_rec_month_start);
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%02d:%02d", systime.dls.dls_rec_hour_start, systime.dls.dls_rec_min_start);
        cJSON_AddStringToObject(data, CGI_SYS_DLS_REC_TIMES, tmp);

        cJSON_AddNumberToObject(data, CGI_SYS_DLS_REC_DAYE, systime.dls.dls_rec_day_end);
        cJSON_AddNumberToObject(data, CGI_SYS_DLS_REC_WEEKE, systime.dls.dls_rec_week_end);
        cJSON_AddNumberToObject(data, CGI_SYS_DLS_REC_MONTHE, systime.dls.dls_rec_month_end);
        SYS_MEM_CLEAR(tmp);
        osal_sprintf(tmp, "%02d:%02d", systime.dls.dls_rec_hour_end, systime.dls.dls_rec_min_end);
        cJSON_AddStringToObject(data, CGI_SYS_DLS_REC_TIMEE, tmp);
    }

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

