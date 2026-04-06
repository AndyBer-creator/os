
#ifndef _CGI_SESSION_H_
#define _CGI_SESSION_H_

typedef struct cgi_session_s
{
    char   *username;
    char   *password;
    char   *remoteIp;
    int32  priv;
    uint32 loginTime;
    uint32 accessType;
    uint32 authenType;
    uint32 userAgent;
} cgi_session_t;

typedef enum cgi_session_status_e
{
    CGI_SESSION_NONE = 0,
    CGI_SESSION_ACTIVE,
    CGI_SESSION_TIMEOUT,
    CGI_SESSION_END
} cgi_session_status_t;

typedef enum cgi_session_authingStatus_e
{
    CGI_SESSION_AUTHING = 0,
    CGI_SESSION_AUTHING_SUCCESS,
    CGI_SESSION_AUTHING_FAIL,
    CGI_SESSION_AUTHING_SESSION_EXCEED,
    CGI_SESSION_AUTHING_END
} cgi_session_authingStatus_t;

typedef enum cgi_session_userAgent_e
{
    CGI_SESSION_AGENT_NONE = 0,
    CGI_SESSION_AGENT_CHROME,
    CGI_SESSION_AGENT_FIREFOX,
    CGI_SESSION_AGENT_IE,
    CGI_SESSION_AGENT_SAFARI,
    CGI_SESSION_AGENT_OTHER,
    CGI_SESSION_AGENT_END
} cgi_session_userAgent_t;

extern char g_username[];
extern int32 g_priv;
extern uint32 g_authenType;

extern void cgi_util_session_userAgent_get(cgi_session_userAgent_t *pAgent);
extern int32 cgi_util_session_create(cgi_session_t *pSession);
extern int32 cgi_util_session_destroy(cgi_session_t *pSession);
extern cgi_session_status_t cgi_util_session_check(uint32 isUpdate);
extern uint32 cgi_util_sessionNum_ret(void);
extern void cgi_util_sessionResp_send(cgi_session_status_t status);

extern void cgi_util_authingSessionName_get(char *pName);
extern void cgi_util_authingSession_create(cgi_session_t *pSession);
extern void cgi_util_authingSession_destroy(void);
extern void cgi_util_authingSessionStatus_get(cgi_session_authingStatus_t *pStatus);

#endif

