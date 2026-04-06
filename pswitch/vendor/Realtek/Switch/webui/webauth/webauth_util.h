
#ifndef _WEBAUTH_UTIL_H_
#define _WEBAUTH_UTIL_H_

#include <common/sys_log.h>
#include <libsal/sal_type.h>
#include <libsal/sal_util.h>
#include <libsal/sal_authmgr.h>

#define HTML_SPACE1   "&nbsp;"
#define HTML_SPACE2   "&nbsp;&nbsp;"
#define HTML_SPACE3   "&nbsp;&nbsp;&nbsp;"
#define HTML_SPACE4   "&nbsp;&nbsp;&nbsp;&nbsp;"
#define HTML_TAB      HTML_SPACE4

#define HTTP_PARAM_FIELD_LEN_MAX 1200     
#define HTTP_PARAM_LEN_MAX 36768       
#define HTTP_PARAM_NUM_MAX 4200          

#define WEB_NOT_EXIST 0
#define WEB_EXIST 1

#define WEB_ROOT_DIR       "/home/web/tmp"
#define PRINT_FILE_PATH    "/home/web/"

typedef struct cgi_paramPair_s
{
    char *name;
    char *val;
}cgi_paramPair_t;

typedef struct cgi_requestParameter_s
{
    cgi_paramPair_t pair[HTTP_PARAM_NUM_MAX];
    int num;
}cgi_requestParameter_t;

typedef struct cgi_webauthSess_s
{
    char   *username;
    char   *password;
    uint32 remotePort;
} cgi_webauthSess_t;

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
    CGI_SESSION_AUTHING_NOT_EXIST,
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

#define WEB_IS_FORM_PARAM_IS_NULL(param, value) \
    (web_util_inputVal_chkIsNull(value = web_util_paramVal_ret(param)))

#define WEB_IS_FORM_PARAM_EXIST(param, value) \
    (web_util_inputVal_chkExist(value = web_util_paramVal_ret(param)))

#define WEB_IS_FORM_PARAM_NOT_EXIST(param, value) \
    !(web_util_inputVal_chkExist(value = web_util_paramVal_ret(param)))

#define WEB_PRINTF(fmt, args...) printf(fmt"\n", ##args)

#define WEB_DBG_TARGET_FILE "/tmp/WEB_DEBUG"
#define WEB_DBG(a,args...) \
do { \
    char tmp_str[256] = ""; \
    char cmd_str[300] = ""; \
    sprintf(tmp_str, "[WEB_DBG] %s:%d:"#a"", __FUNCTION__,__LINE__, ##args); \
    sprintf(cmd_str, "echo \"%s\" >> %s", tmp_str, WEB_DBG_TARGET_FILE); \
    system(cmd_str); \
} while (0)

int32 web_util_inputVal_chkIsNull(char *val);
int32 web_util_inputVal_chkExist(char *val);

int32 web_util_initParaPairs(void);
char* web_util_paramVal_ret(char *param);

void cgi_util_authingSessionName_get(uint32 remotePort, char *pName);
void cgi_util_authingSession_create(cgi_webauthSess_t *pSession);
void cgi_util_authingSession_destroy(uint32 remotePort);
void cgi_util_authingSessionStatus_get(uint32 remotePort, cgi_session_authingStatus_t *pStatus);
#endif
