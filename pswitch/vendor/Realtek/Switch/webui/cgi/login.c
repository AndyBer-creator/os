#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <include/cgi.h>

int32 _login_auth(char *user, char *pass, char *remoteIp, char *accessTypeStr, uint32 userAgent, int32 *pPriv)
{
#ifdef CONFIG_SYS_AAA
    int32 currPriv = -1;
    int32   ret = SYS_ERR_FAILED;
    uint32  userIdx;
    int     i = 0;
    sys_auth_type_t typePrio[CAPA_AAA_AUTHTYPE_PRI_NUM];
    sys_access_type_t accessType = 0;
#ifdef CONFIG_SYS_LIB_TACPLUS
    struct session *pTacplusSession = NULL;
#endif
    cgi_session_t sessionInfo;

    osal_memset(typePrio, 0, sizeof(typePrio));
    SYS_MEM_CLEAR(sessionInfo);

    if (accessTypeStr != NULL && 0 == osal_strcmp(accessTypeStr, "https"))
        accessType = SYS_ACCESS_WEB_HTTPS;
    else
        accessType = SYS_ACCESS_WEB_HTTP;

    sal_aaa_loginAuthTypePrio_get(accessType, typePrio);

    if (typePrio[0] == SYS_AUTH_TYPE_ENABLE)
    {
        return FALSE;
    }

    currPriv = -1;

    for (i = 0; i < CAPA_AAA_AUTHTYPE_PRI_NUM; i++)
    {
        if (SYS_AUTH_TYPE_EMPTY == typePrio[i])
            break;

        if ((i > 0) && (typePrio[i] > SYS_AUTH_TYPE_NONE))
        {
            SYS_LOG(LOG_CAT_AAA, LOG_AAA_METHOD_NEW,
                        text_aaa_auth_method[typePrio[i]].text);
        }

        switch(typePrio[i])
        {
            case SYS_AUTH_TYPE_LOCAL:
                userIdx = 0;
                ret = sal_aaa_localLoginUsername_authen(user, &userIdx);
                if (ret != SYS_ERR_OK)
                    break;
                ret = sal_aaa_localLoginPassword_authen(userIdx, pass, &currPriv);
                if (ret != SYS_ERR_OK)
                    goto auth_fail;
                sessionInfo.authenType = SYS_AUTH_TYPE_LOCAL;
                break;
#ifdef CONFIG_SYS_LIB_TACPLUS
            case SYS_AUTH_TYPE_TACPLUS:

                ret = sal_aaa_tacplusSession_init(&pTacplusSession);
                if (ret != SYS_ERR_OK)
                    break;

                ret = sal_aaa_tacplusLoginUsername_authen(pTacplusSession, user);
                if (ret != SYS_ERR_OK)
                {
                    if (pTacplusSession != NULL)
                        sal_aaa_tacplusSession_close(pTacplusSession);
                    
                    if (ret == SYS_ERR_USERNAME_INVALID)
                    
                        goto auth_fail;
                    else
                        break;
                    break;
                }
                ret = sal_aaa_tacplusLoginPassword_authen(pTacplusSession, pass);
                if (pTacplusSession != NULL)
                    sal_aaa_tacplusSession_close(pTacplusSession);
                if (ret != SYS_ERR_OK)
                    
                    goto auth_fail;
                else
                    currPriv = DFLT_AAA_PRIV_ADMIN;

                sessionInfo.authenType = SYS_AUTH_TYPE_TACPLUS;
                break;
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
            case SYS_AUTH_TYPE_RADIUS:
                ret = sal_aaa_radiusLoginUsername_authen(user, pass);
                
                if (ret == SYS_ERR_RADIUS_AUTH_REJECT)
                    goto auth_fail;
                if(ret == SYS_ERR_OK)
                    currPriv = DFLT_AAA_PRIV_ADMIN;

                sessionInfo.authenType = SYS_AUTH_TYPE_RADIUS;
                break;
#endif
            case SYS_AUTH_TYPE_NONE:
                currPriv = DFLT_AAA_PRIV_ADMIN;
                ret = SYS_ERR_OK;

                sessionInfo.authenType = SYS_AUTH_TYPE_NONE;
                break;

            default:
                break;
        }

        if (ret == SYS_ERR_OK)
        {
            sessionInfo.username = user;
            sessionInfo.priv = currPriv;
            sessionInfo.accessType = accessType;

            if (0 == osal_strncmp(remoteIp, "::ffff:", 7))
                sessionInfo.remoteIp = remoteIp + 7;
            else
                sessionInfo.remoteIp = remoteIp;

            sessionInfo.userAgent = userAgent;

            cgi_util_session_create(&sessionInfo);

            *pPriv = currPriv;

            SYS_LOG(LOG_CAT_AAA, LOG_AAA_AUTH_SUCCESS,
                        text_aaa_auth_method[typePrio[i]].text,
                        "login");

            SYS_LOG(LOG_CAT_AAA, LOG_AAA_CONNECT,
                        text_access_type[accessType].text,
                        user,
                        sessionInfo.remoteIp);
            return TRUE;
        }
        else
        {
            SYS_LOG(LOG_CAT_AAA, LOG_AAA_METHOD_FAIL,
                        text_aaa_auth_method[typePrio[i]].text);
        }
    }
#else
    int32 currPriv = -1;
    int32 ret = SYS_ERR_FAILED;
    sys_access_type_t accessType = 0;
    cgi_session_t sessionInfo;

    SYS_MEM_CLEAR(sessionInfo);

    ret = sal_sys_localLogin_auth(user, pass, &currPriv);

    if (accessTypeStr != NULL && 0 == osal_strcmp(accessTypeStr, "https"))
        accessType = SYS_ACCESS_WEB_HTTPS;
    else
        accessType = SYS_ACCESS_WEB_HTTP;

    if (0 == osal_strncmp(remoteIp, "::ffff:", 7))
        sessionInfo.remoteIp = remoteIp + 7;
    else
        sessionInfo.remoteIp = remoteIp;

    if (ret == SYS_ERR_OK)
    {
        sessionInfo.username = user;
        sessionInfo.priv = currPriv;
        sessionInfo.accessType = accessType;

        sessionInfo.userAgent = userAgent;

        cgi_util_session_create(&sessionInfo);

        *pPriv = currPriv;

        SYS_LOG(LOG_CAT_AAA, LOG_AAA_CONNECT,
                text_access_type[accessType].text,
                user,
                sessionInfo.remoteIp);
        return TRUE;
    }

    if (SYS_ERR_USERNAME_INVALID != ret)
    {
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_USER_REJECT,
                text_access_type[accessType].text,
                user,
                sessionInfo.remoteIp);
    }
    else
    {
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_REJECT,
                text_access_type[accessType].text,
                sessionInfo.remoteIp);
    }
#endif

#ifdef CONFIG_SYS_AAA
auth_fail:
    if (0 == osal_strncmp(remoteIp, "::ffff:", 7))
        sessionInfo.remoteIp = remoteIp + 7;
    else
        sessionInfo.remoteIp = remoteIp;

    if (SYS_ERR_USERNAME_INVALID != ret)
    {
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_USER_REJECT,
                text_access_type[accessType].text,
                user,
                sessionInfo.remoteIp);
    }
    else
    {
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_REJECT,
                text_access_type[accessType].text,
                sessionInfo.remoteIp);
    }

    SYS_LOG(LOG_CAT_AAA, LOG_AAA_AUTH_FAIL,
                "login");
#endif

    return FALSE;
}

int main(int argc, char* argv[], char* envp[])
{
    FILE *fp = NULL;
    char file[SYS_BUF128_LEN];
    char file2[SYS_BUF128_LEN];
    char username[SYS_BUF64_LEN];
    char password[SYS_BUF256_LEN + 1];
    char remoteIp[SYS_BUF128_LEN];
    char accessTypeStr[SYS_BUF8_LEN];
    char userAgentStr[SYS_BUF8_LEN];
    uint32 userAgent = 0;
    int32 priv = 0;

    SYS_MEM_CLEAR(file);
    SYS_MEM_CLEAR(file2);
    SYS_MEM_CLEAR(remoteIp);
    SYS_MEM_CLEAR(username);
    SYS_MEM_CLEAR(password);
    SYS_MEM_CLEAR(accessTypeStr);

    osal_sprintf(file, "/tmp/guiAuth_info_%s", argv[1]);

    if (NULL != (fp = fopen(file, "r")))
    {
        fgets(username, sizeof(username), fp);
        fgets(password, sizeof(password), fp);
        fgets(file2, sizeof(file2), fp);
        fgets(remoteIp, sizeof(remoteIp), fp);
        fgets(accessTypeStr, sizeof(accessTypeStr), fp);
        fgets(userAgentStr, sizeof(userAgentStr), fp);

        if (osal_strlen(username) > 0)
            username[osal_strlen(username) -1] = '\0';

        if (osal_strlen(password) > 0)
            password[osal_strlen(password) -1] = '\0';

        if (osal_strlen(file2) > 0)
            file2[osal_strlen(file2) -1] = '\0';

        if (osal_strlen(remoteIp) > 0)
            remoteIp[osal_strlen(remoteIp) -1] = '\0';

        if (osal_strlen(accessTypeStr) > 0)
            accessTypeStr[osal_strlen(accessTypeStr) -1] = '\0';

        if (osal_strlen(userAgentStr) > 0)
        {
            userAgentStr[osal_strlen(userAgentStr) -1] = '\0';
            userAgent = SYS_STR2UINT(userAgentStr);
        }
        fclose(fp);
    }

    unlink(file);

    if (NULL != (fp = fopen(file2, "w")))
    {
        fprintf(fp, "%d", CGI_SESSION_AUTHING);
        fclose(fp);
    }

    if (cgi_util_sessionNum_ret() >= CAPA_SYS_WEB_SESSION_NUM)
    {
        if (NULL != (fp = fopen(file2, "w")))
        {
            fprintf(fp, "%d", CGI_SESSION_AUTHING_SESSION_EXCEED);
            fclose(fp);
        }
    }
    else if (TRUE == _login_auth(username, password, remoteIp, accessTypeStr, userAgent, &priv))
    {
        if (NULL != (fp = fopen(file2, "w")))
        {
            fprintf(fp, "%d", CGI_SESSION_AUTHING_SUCCESS);
            fclose(fp);
        }
    }
    else
    {
        if (NULL != (fp = fopen(file2, "w")))
        {
            fprintf(fp, "%d", CGI_SESSION_AUTHING_FAIL);
            fclose(fp);
        }
    }

    return 0;

}

