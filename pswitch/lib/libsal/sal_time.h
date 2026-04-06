
#ifndef __SAL_TIME_H__
#define __SAL_TIME_H__

#define UPTIME_FILE             "/proc/uptime"
#define TIMEZONE_ALL            92
#define TIME_CURRENT            "/tmp/currentTime"
#define SNTP_TRY_REASON_FILE    "/tmp/msntp_reason"
#define SNTP_UPDATE_INTERVAL    30

typedef enum sys_time_dls_type_e
{
    SYS_TIME_DLS_TYPE_DISABLED = 0,
    SYS_TIME_DLS_TYPE_RECURRING,
    SYS_TIME_DLS_TYPE_NON_RECURRING,
    SYS_TIME_DLS_TYPE_USA,
    SYS_TIME_DLS_TYPE_EUROPEAN,
    SYS_TIME_DLS_TYPE_END,
} sys_time_dls_type_t;

typedef struct sys_time_str_s
{
    char tstr[SYS_BUF64_LEN];
    char tzstr[SYS_BUF16_LEN];
}sys_time_str_t;

typedef struct sys_time_dls_s
{
    
    sys_time_dls_type_t dlsType;
    char dlsAcronym[SYS_BUF8_LEN];
    uint32 dls_start_time;
    uint32 dls_end_time;
    uint16 dls_offset;          

    uint8 dls_rec_week_start;    
    uint8 dls_rec_day_start;     
    uint8 dls_rec_month_start;   
    uint8 dls_rec_hour_start;    
    uint8 dls_rec_min_start;     

    uint8 dls_rec_week_end;    
    uint8 dls_rec_day_end;     
    uint8 dls_rec_month_end;   
    uint8 dls_rec_hour_end;    
    uint8 dls_rec_min_end;     

    uint16 dls_non_year_start;   
    uint8 dls_non_month_start;   
    uint8 dls_non_date_start;    
    uint8 dls_non_hour_start;    
    uint8 dls_non_min_start;     

    uint16 dls_non_year_end;   
    uint8 dls_non_month_end;   
    uint8 dls_non_date_end;    
    uint8 dls_non_hour_end;    
    uint8 dls_non_min_end;     
}sys_time_dls_t;

typedef enum sntp_enable_type_s
{
	SNTP_DISABLED = 0,
    SNTP_ENABLED,
    SNTP_COMPUTER,
    SNTP_ENABLE_END
}sntp_enable_type_t;

typedef struct sys_time_s
{
    
    sntp_enable_type_t sntpEnable;
    char sntpServer[CAPA_HOSTNAME_LEN + 1];
    uint32 sntpPort;
    uint32 sntp_time;
    uint32 sntp_attemps;
    uint32 sntp_failures;

    int32   tzdiff;     
    uint32  tzIdx;
    char    tzAcronym[8];

    sys_time_dls_t dls;
} sys_time_t;

extern int32 sal_time_set(sys_time_t *systime);

extern int32 sal_time_get(sys_time_t *systime);

#ifdef CONFIG_SYS_APP_MSNTP

extern int32 sal_time_update(uint8 isFromPoold);

extern int32 sal_time_sntp_reason_set(sys_time_t *systime);
#endif

extern int32 sal_time_staticTimeInt_get(uint32 inTime, uint32 *pOutTime);

extern int32 sal_time_currentTimeInt_get(uint32 *pTime);

extern int32 sal_timezoneStr_get(char *tzStr);

extern int32 sal_time_currentTimeStr_get(sys_time_str_t *str);

extern int32 sal_time_dls_convert(sys_time_t *systime);

extern int32 sal_time_timeInt2Str(time_t *time_int, char *time_str);

extern int32 sal_default_time_set(void);

extern int32 sal_time_time_hms_get(uint32 *pHour, uint32 *pMin, uint32 *pSec);
#endif

