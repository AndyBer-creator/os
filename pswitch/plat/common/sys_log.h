#ifndef __COMMON_SYS_LOG_H__
#define __COMMON_SYS_LOG_H__
#include <stdarg.h>

#include <common/sys_type.h>
#include <common/sys_log_desc.h>
#define SYS_LOG_BUF_SIZE            512
#define SYS_LOG_KERN_KEY            "<RKERN>"
#define SYS_LOG_RTK_KEY             "<RTKLOG>"
#define SYS_LOG_USER_ALARM_FILE     "/tmp/log_alarm"        
#define SYS_LOG_DAEMON_PID_FILE     "/var/run/syslogd.pid"  
#define SYS_LOG_JUNK_MSG            "dont remove"
#define SYS_LOG_TIME_FILE           "/tmp/currentTime"
#define SYS_LOG_MSG_FACILITY        22
typedef enum log_sev_e
{
    LOG_SEV_EMERG = 0,  
    LOG_SEV_ALERT,      
    LOG_SEV_CRIT,       
    LOG_SEV_ERR,        
    LOG_SEV_WARNING,    
    LOG_SEV_NOTICE,     
    LOG_SEV_INFO,       
    LOG_SEV_DEBUG,      
    LOG_SEV_ALL,        
} log_sev_t;
#define SYS_LOG(cat, mid, args...)      sys_log(cat, mid , ## args)
#define SYS_LOG_ES(errCode, str)        sys_log_errStr_set(errCode, str)

#define SYS_LOG_HANDLE(cat, mid, argstr)                     \
do {                                                         \
    char __cmd[256];                                         \
    memset(__cmd, 0, sizeof(__cmd));                         \
    sprintf(__cmd, "handler -l %d %d %s", cat, mid, argstr); \
    system(__cmd);                                           \
} while(0)
extern int32 sys_log(uint32 cat, uint32 mid, ...);
extern int32 sys_log2(uint32 cat, uint32 mid, va_list args);
extern int32 sys_log_restart_set(void);
extern int32 sys_log_dbg_set(char *fmt, ...);
extern int32 sys_log_errStr_set(int32 errCode, char *pStr);

#endif

