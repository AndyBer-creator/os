
#ifndef __SAL_LOG_H__
#define __SAL_LOG_H__

#include <common/sys_type.h>
#include <common/sys_log.h>
#include <common/sys_dbg.h>

#define LOG_CONFIG_FILE         "/etc/syslogd.conf"
#define LOG_PID_FILE            "/etc/syslogd.pid"
#define LOG_FILE_RAM_BASENAME   "ram.log"
#define LOG_FILE_RAM            "/var/log/"LOG_FILE_RAM_BASENAME
#define LOG_FILE_FLASH_BASENAME "flash.log"
#define LOG_FILE_FLASH_TMP      "/var/log/"LOG_FILE_FLASH_BASENAME
#define LOG_FILE_FLASH          JFFS2_LOG_TARGET_DIR"/"LOG_FILE_FLASH_BASENAME
#define LOG_FILE_CONSOLE        "/dev/console"

#define LOG_MAX_READ        (16 * 1024)
#define LOG_REMOTE_MAX      CAPA_LOG_HOST_NUM
#define LOG_REMOTE_DELAY    DFLT_LOG_HOST_DELAY_TIME

typedef enum log_target_e
{
    LOG_LOCAL = 0,
    LOG_CONSOLE = LOG_LOCAL,
    LOG_RAM,
    LOG_FLASH,
    LOG_LOCAL_MAX,

    LOG_REMOTE = LOG_LOCAL_MAX,
    LOG_SERVER1 = LOG_REMOTE,
    LOG_SERVER2,
    LOG_SERVER3,
    LOG_SERVER4,
    LOG_SERVER5,
    LOG_SERVER6,
    LOG_SERVER7,
    LOG_SERVER8,
    LOG_TARGET_MAX,
} log_target_t;

typedef enum log_facility_e
{
    LOG_FACILITY_LOCAL0 = 16,
    LOG_FACILITY_LOCAL1,
    LOG_FACILITY_LOCAL2,
    LOG_FACILITY_LOCAL3,
    LOG_FACILITY_LOCAL4,
    LOG_FACILITY_LOCAL5,
    LOG_FACILITY_LOCAL6,
    LOG_FACILITY_LOCAL7,
    LOG_FACILITY_LOCAL_END,
} log_facility_t;

typedef struct sal_log_target_t
{
    sys_enable_t enable;

    uint32 severity;
    uint32 totalCount;

    char host[CAPA_HOSTNAME_LEN + 1];
    uint32 port;
    log_facility_t facility;
} sal_log_target_t;

extern const sys_text_t text_log_sev[];
extern const sys_text_t text_log_target[];

#ifdef CONFIG_SYS_SYSLOG

extern int32 sal_log_enable_set(sys_enable_t enable);

extern int32 sal_log_enable_get(sys_enable_t *pEnable);

extern int32 sal_log_target_set(log_target_t index, sal_log_target_t *pTarget);

extern int32 sal_log_remoteIndexByIp_get(char *pIP, uint32 *pIndex);

extern int32 sal_log_remoteEmptyIndex_get(uint32 *pIndex);

extern int32 sal_log_target_get(log_target_t index, sal_log_target_t *pTarget);

extern int32 sal_log_file_clear(log_target_t index);

extern int32 sal_log_daemon_restart(uint8 isAuto);

extern int32 sal_log_config_create(void);

extern int32 sal_log_autoRestart_set(sys_enable_t autoRestart);

extern int32 sal_log_autoRestart_get(sys_enable_t *autoRestart);

extern int32 sal_log_ratelimitEnable_set(sys_enable_t enable);

extern int32 sal_log_ratelimitEnable_get(sys_enable_t *pEnable);

extern int32 sal_log_ratelimitInterval_set(uint32 interval);

extern int32 sal_log_ratelimitInterval_get(uint32 *pInterval);
#endif
#endif

