
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include <common/sys_error.h>
#include <libsal/sal_sys.h>
#include <lib/libcJSON/cJSON.h>

#include "cgi_util.h"

char g_username[CAPA_SYS_USERNAME_LEN + 1];
int32 g_priv = 1;
uint32 g_authenType = 0;

void cgi_util_session_userAgent_get(cgi_session_userAgent_t *pAgent)
{
    char *userAgent = getenv("HTTP_USER_AGENT");

    if (NULL != userAgent)
    {
        if (NULL != osal_strstr(userAgent, "Chrome"))
            *pAgent = CGI_SESSION_AGENT_CHROME;
        else if (NULL != osal_strstr(userAgent, "Firefox"))
            *pAgent = CGI_SESSION_AGENT_FIREFOX;
        else if (NULL != osal_strstr(userAgent, "Safari"))
            *pAgent = CGI_SESSION_AGENT_SAFARI;
        else if (NULL != osal_strstr(userAgent, "MSIE") || NULL != osal_strstr(userAgent, "Trident"))
            *pAgent = CGI_SESSION_AGENT_IE;
        else
            *pAgent = CGI_SESSION_AGENT_OTHER;
    }

}

int32 cgi_util_session_create(cgi_session_t *pSession)
{
    FILE *fp = NULL;
    char file[SYS_BUF128_LEN];
    uint32 now_sec = 0;
    sys_line_session_t lineSession;

    SYS_MEM_CLEAR(file);
    SYS_MEM_CLEAR(lineSession);

    osal_time_sysUptime_get(&now_sec);

    osal_sprintf(file, "/tmp/login_%s_%s_%d", (SYS_ACCESS_WEB_HTTP == pSession->accessType) ? "http":"https", pSession->remoteIp, pSession->userAgent);

    if (NULL != (fp = fopen(file, "w")))
    {
        fprintf(fp, "%d\n%u\n%u\n%u\n%s\n", pSession->priv, now_sec, now_sec, pSession->authenType, pSession->username);

        fclose(fp);
    }

    lineSession.pid = pSession->userAgent;
    strlcpy(lineSession.user, pSession->username, sizeof(lineSession.user));
    strlcpy(lineSession.host, pSession->remoteIp, sizeof(lineSession.host));
    sal_sys_lineSession_set(pSession->accessType, lineSession);

    return SYS_ERR_OK;
}

int32 cgi_util_session_destroy(cgi_session_t *pSession)
{
    char file[SYS_BUF128_LEN];

    SYS_LOG(LOG_CAT_AAA, LOG_AAA_DISCONNECT,
                (SYS_ACCESS_WEB_HTTP == pSession->accessType) ? "http":"https",
                pSession->username,
                pSession->remoteIp);

    osal_sprintf(file, "/tmp/login_%s_%s_%d", (SYS_ACCESS_WEB_HTTP == pSession->accessType) ? "http":"https", pSession->remoteIp, pSession->userAgent);

    unlink(file);

    osal_sprintf(file, "%s/%s_%s_%d", SYS_LINEINFO_DIR, text_access_type[pSession->accessType].text, pSession->remoteIp, pSession->userAgent);

    unlink(file);

    system("rm "WEB_TMP_REAL_DIR"/*");
    return SYS_ERR_OK;
}

static uint32 _cgi_util_sessionTimeout_check(uint32 now, uint32 last, sys_line_webType_t type)
{
    uint32 timeout = 0;
    sal_sys_webSessionTimeout_get(type, &timeout);

    timeout *= 60;

    if (timeout != 0)
    {
        if((now - last)/1000 > timeout)
        {
            return TRUE;
        }
    }

    return FALSE;
}

cgi_session_status_t cgi_util_session_check(uint32 isUpdate)
{
    FILE *fp = NULL;
    char file[SYS_BUF128_LEN];
    char *remoteIp = getenv("REMOTE_ADDR");
    char *accessTypeEnv = getenv("ACCESS_TYPE");
    char username[CAPA_SYS_USERNAME_LEN + 1];
    int32 priv = 0;
    uint32 now_sec = 0;
    uint32 last = 0;
    uint32 refresh = 0;
    uint32 authenType = 0;
    sys_access_type_t accessType;
    sys_line_webType_t lineType;
    cgi_session_t sessionInfo;
    cgi_session_userAgent_t userAgent = 0;

    SYS_MEM_CLEAR(file);
    SYS_MEM_CLEAR(username);

    osal_time_sysUptime_get(&now_sec);

    if (accessTypeEnv != NULL && 0 == osal_strcmp(accessTypeEnv, "https"))
    {
        accessType = SYS_ACCESS_WEB_HTTPS;
        lineType = SYS_LINE_WEB_HTTPS;
    }
    else
    {
        accessType = SYS_ACCESS_WEB_HTTP;
        lineType = SYS_LINE_WEB_HTTP;
    }

    if (0 == osal_strncmp(remoteIp, "::ffff:", 7))
        remoteIp = remoteIp + 7;

    cgi_util_session_userAgent_get(&userAgent);

    osal_sprintf(file, "/tmp/login_%s_%s_%d", (accessType == SYS_ACCESS_WEB_HTTP) ? "http":"https", remoteIp, userAgent);

    if (NULL != (fp = fopen(file, "r+")))
    {
        fscanf(fp, "%d\n%u\n%u\n%u\n%s\n", &priv, &last, &refresh, &authenType, username);

        SYS_MEM_CLEAR(g_username);
        strlcpy(g_username, username, sizeof(g_username));
        g_authenType = authenType;

        g_priv = priv;

        if (TRUE == _cgi_util_sessionTimeout_check(now_sec, last, lineType))
        {
            cgi_util_sessionResp_send(CGI_SESSION_TIMEOUT);

            fclose(fp);

            SYS_MEM_CLEAR(sessionInfo);
            sessionInfo.accessType = accessType;
            sessionInfo.remoteIp = remoteIp;
            sessionInfo.userAgent = userAgent;
            sessionInfo.username = g_username;

            cgi_util_session_destroy(&sessionInfo);

            SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_WEB_TIMEOUT,
                        g_username,
                        remoteIp);

            return CGI_SESSION_TIMEOUT;
        }
        
        else
        {
            if (TRUE == isUpdate)
            {
                rewind(fp);
                fprintf(fp, "%d\n%u\n%u\n%u\n%s\n", priv, now_sec, now_sec, authenType, username);
            }
            fclose(fp);

            return CGI_SESSION_ACTIVE;
        }
    }
    else
    {
        cgi_util_sessionResp_send(CGI_SESSION_NONE);

        return CGI_SESSION_NONE;
    }
}

uint32 cgi_util_sessionNum_ret(void)
{
    uint32 sessNum = 0;
    DIR *dir = NULL;
    struct dirent *entry;

    if ((dir = opendir("/tmp")) == NULL)
        return sessNum;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
                continue;
        }

        if (NULL == osal_strstr(entry->d_name, "login_"))
            continue;

        sessNum++;
    }
    closedir(dir);

    return sessNum;
}

void cgi_util_sessionResp_send(cgi_session_status_t status)
{
    cJSON *session = NULL;

    session = cJSON_CreateObject();
    cJSON_AddTrueToObject(session, "logout");

    if (CGI_SESSION_NONE == status)
        cJSON_AddStringToObject(session, "reason", "notAuth");
    else if (CGI_SESSION_TIMEOUT)
        cJSON_AddStringToObject(session, "reason", "timeout");

    CGI_RESPONSE_JSON(session);
    cJSON_Delete(session);
}

void cgi_util_authingSessionName_get(char *pName)
{
    char *remoteIp = getenv("REMOTE_ADDR");
    char *accessTypeEnv = getenv("ACCESS_TYPE");
    sys_access_type_t accessType;
    cgi_session_userAgent_t userAgent = 0;

    if (accessTypeEnv != NULL && 0 == osal_strcmp(accessTypeEnv, "https"))
        accessType = SYS_ACCESS_WEB_HTTPS;
    else
        accessType = SYS_ACCESS_WEB_HTTP;

    cgi_util_session_userAgent_get(&userAgent);

    osal_sprintf(pName, "/tmp/guiAuth_%s_%s_%d", (SYS_ACCESS_WEB_HTTP == accessType) ? "http":"https", remoteIp, userAgent);
}

void cgi_util_authingSession_create(cgi_session_t *pSession)
{
    FILE *fp = NULL;
    char file[SYS_BUF128_LEN];
    char file2[SYS_BUF128_LEN];
    cgi_session_userAgent_t userAgent = 0;

    SYS_MEM_CLEAR(file);
    SYS_MEM_CLEAR(file2);

    osal_sprintf(file, "/tmp/guiAuth_info_%d", getpid());
    cgi_util_authingSessionName_get(file2);

    cgi_util_session_userAgent_get(&userAgent);

    if (NULL != (fp = fopen(file, "w")))
    {
        fprintf(fp, "%s\n", pSession->username);
        fprintf(fp, "%s\n", pSession->password);
        fprintf(fp, "%s\n", file2);
        fprintf(fp, "%s\n", getenv("REMOTE_ADDR"));
        fprintf(fp, "%s\n", getenv("ACCESS_TYPE"));
        fprintf(fp, "%d\n", userAgent);

        fclose(fp);
    }

    return;
}

void cgi_util_authingSession_destroy(void)
{
    char file[SYS_BUF128_LEN];

    SYS_MEM_CLEAR(file);

    cgi_util_authingSessionName_get(file);

    unlink(file);

    return;
}

void cgi_util_authingSessionStatus_get(cgi_session_authingStatus_t *pStatus)
{
    FILE *fp = NULL;
    char file[SYS_BUF128_LEN];

    SYS_MEM_CLEAR(file);

    cgi_util_authingSessionName_get(file);

    *pStatus = CGI_SESSION_AUTHING;

    if (NULL != (fp = fopen(file, "r")))
    {
        fscanf(fp, "%d", (int32 *)pStatus);

        fclose(fp);
    }

    return;
}

