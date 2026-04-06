
#ifndef __SAL_SYS_LINE_H__
#define __SAL_SYS_LINE_H__

#define HTTP_PID_FILE               "/var/run/boa.pid"
#define HTTPS_PID_FILE              "/var/run/boa_https.pid"
#define SYS_CONSOLE_CLI_PID_FILE    "/var/run/consoleCli.pid"
#define SYS_LINEINFO_DIR            "/tmp/lineinfo"
#define ETC_INETD_CONFIG_FILE       "/etc/inetd.conf"
#define ETC_SERVERS_FILE            "/etc/services"

#define DEFAULT_TELNET_PORT_VAL     23
#define DEFAULT_SSH_PORT_VAL        22
#define SYS_SESSION_DIR             "/var/session"

typedef enum sys_access_type_e
{
    SYS_ACCESS_CLI_CONSOLE = 0,
    SYS_ACCESS_CLI_TELNET,
#ifdef CONFIG_USER_SSH_SSHD
    SYS_ACCESS_CLI_SSH,
#endif
    SYS_ACCESS_WEB_HTTP,
    SYS_ACCESS_WEB_HTTPS,
    SYS_ACCESS_END
} sys_access_type_t;

typedef struct sys_line_session_s
{
    int32  pid;
    uint32 time;
    char   user[CAPA_SYS_USERNAME_LEN];
    char   host[CAPA_HOSTNAME_LEN + 1];
    char   localIp[CAPA_IPV6_STR_LEN];
} sys_line_session_t;

typedef struct sys_cliRetrySilent_s
{
    uint32 retryNum;
    uint32 silentTime;
} sys_cliRetrySilent_t;

typedef enum sys_line_cliType_e
{
    SYS_LINE_CLI_CONSOLE = 0,
    SYS_LINE_CLI_TELNET,
#ifdef CONFIG_USER_SSH_SSHD
    SYS_LINE_CLI_SSH,
#endif
    SYS_LINE_CLI_END
} sys_line_cliType_t;

typedef struct sys_line_cli_s
{

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    
    uint32 sessionTime; 
#endif

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    sys_cliRetrySilent_t retrySilent;
#endif

    sys_enable_t historyState;
    uint32       historyCnt;

    char   password[CAPA_SYS_PASSWORD_LEN + 1];
} sys_line_cli_t;

typedef enum sys_line_webType_e
{
    SYS_LINE_WEB_HTTP = 0,
    SYS_LINE_WEB_HTTPS,
    SYS_LINE_WEB_END
} sys_line_webType_t;

typedef struct sys_line_web_s
{
#ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT
    
    uint32 sessionTime; 
#endif
} sys_line_web_t;

typedef enum sys_conf_syncAction_e
{
    SYS_CONF_SYNC_STARTUP,
#ifdef CONFIG_SYS_BACKUP_CONFIGURATION
    SYS_CONF_SYNC_BACKUP,
#endif
    SYS_CONF_SYNC_ALL,
    SYS_CONF_SYNC_END
} sys_conf_syncAction_t;

extern const sys_text_t text_access_type[];
extern const sys_text_t text_line_cli[];
extern const sys_text_t text_line_web[];

extern int32 sal_sys_get_current_web_session_info(sys_line_session_t *pSession);

extern int32 sal_sys_lineSession_by_pid_get(int32 pid, sys_line_session_t *pSession, char *typeStr);
extern int32 sal_sys_lineSession_by_pid_get_new(int32 pid, sys_line_session_t *pSession);

extern int32 sal_sys_lineSession_set(sys_access_type_t type, sys_line_session_t session);

extern int32 sal_sys_lineSession_get(sys_access_type_t type, uint32 idx, sys_line_session_t *pSession);

extern uint32 sal_sys_lineSessionCnt_ret(sys_access_type_t type);

extern int32 sal_sys_lineSessionInfo_clear(void);

extern int32 sal_sys_cliHistory_count_set(sys_line_cliType_t type, uint32 count);

extern int32 sal_sys_cliHistory_count_get(sys_line_cliType_t type, uint32 *pCount);

extern int32 sal_sys_cliHistoryState_set(sys_line_cliType_t type, sys_enable_t enable);

extern int32 sal_sys_cliHistoryState_get(sys_line_cliType_t type, sys_enable_t *pEnable);

#ifdef CONFIG_SYS_APP_TELNETD
extern int32 sal_sys_telnetd_port_set(uint32 port);

extern int32 sal_sys_telnetdEnable_set(sys_enable_t enable);

extern int32 sal_sys_telnetdEnable_get(sys_enable_t *pEnable);
extern int32 sal_sys_telnetd_port_get(uint32 *port);
#endif

#ifdef CONFIG_SYS_APP_HTTPD

extern int32 sal_sys_httpEnable_set(sys_enable_t enable);

extern int32 sal_sys_httpEnable_get(sys_enable_t *pEnable);
#endif

#ifdef CONFIG_USER_BOA_WITH_SSL

extern int32 sal_sys_httpsEnable_set(sys_enable_t enable);

extern int32 sal_sys_httpsEnable_get(sys_enable_t *pEnable);
#endif

extern int32 sal_sys_inetdConfig_set(void);

extern int32 sal_inetd_restart(void);

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT

extern int32 sal_sys_cliSessionTimeout_set(sys_line_cliType_t type, uint32 timeout);

extern int32 sal_sys_cliSessionTimeout_get(sys_line_cliType_t type, uint32 *pTimeout);
#endif 

#ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT

extern int32 sal_sys_webSessionTimeout_set(sys_line_webType_t type, uint32 timeout);

extern int32 sal_sys_webSessionTimeout_get(sys_line_webType_t type, uint32 *pTimeout);
#endif 

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT

extern int32 sal_sys_cliRetrySilent_set(sys_line_cliType_t type, sys_cliRetrySilent_t retrySilent);

extern int32 sal_sys_cliRetrySilent_get(sys_line_cliType_t type, sys_cliRetrySilent_t *pRetrySilent);
#endif 

#endif 

