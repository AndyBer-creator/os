
#include <libcfg/cfg.h>

int32 cfg_time_set(sys_time_t *systime)
{
    cfg_time_t *time = NULL;

    CFG_OPEN(CFG_ID_TIME, (cfg_addr)&time);

    osal_memcpy(&(time->systime), systime, sizeof(sys_time_t));

    CFG_CLOSE(CFG_ID_TIME);
}

int32 cfg_time_sntp_reason_set(sys_time_t *systime)
{
    cfg_time_t *time = NULL;

    CFG_OPEN(CFG_ID_TIME, (cfg_addr)&time);

    time->systime.sntp_time = systime->sntp_time;
    time->systime.sntp_attemps = systime->sntp_attemps;
    time->systime.sntp_failures = systime->sntp_failures;

    CFG_CLOSE(CFG_ID_TIME);
}

int32 cfg_time_get(sys_time_t *systime)
{
    cfg_time_t *time = NULL;

    CFG_OPEN(CFG_ID_TIME, (cfg_addr)&time);

    osal_memcpy(systime, &(time->systime), sizeof(sys_time_t));

    CFG_CLOSE(CFG_ID_TIME);
}

