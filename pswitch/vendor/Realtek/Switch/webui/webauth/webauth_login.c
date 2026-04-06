#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <webui/webauth/webauth.h>
#include <libsal/sal_authmgr.h>

int32 _login_auth(char *user, char *pass, char *remotePort)
{
    int32 ret = SYS_ERR_FAILED;
    uint32 status = FALSE;
    uint32 i = 0;
    webauth_authInfo_t authInfo;
    authmgr_methodOrder_t methodOrder;
    authmgr_suppInfo_t suppInfo;

    SYS_MEM_CLEAR(authInfo);
    SYS_MEM_CLEAR(suppInfo);

    authInfo.tcpPort = SYS_STR2UINT(remotePort);

    sal_authmgr_webauthSuppInfoByKey_get(authInfo.tcpPort, &suppInfo);

    sal_authmgr_portMethodOrder_get(suppInfo.port, &methodOrder);

    for (i = 0; i < (AUTHMGR_METHOD_END - 1); i++)
    {
        if (AUTHMGR_METHOD_NONE == methodOrder.order[i])
            break;
        else if (AUTHMGR_METHOD_LOCAL == methodOrder.order[i])
            ret = sal_authmgr_webauthLocal_authen(user, pass, &authInfo);
        else if (AUTHMGR_METHOD_RADIUS == methodOrder.order[i])
            ret = sal_authmgr_webauthRadius_authen(user, pass, &authInfo);

        if (SYS_ERR_OK == ret)
        {
            authInfo.authSts = SYS_AUTH;
            break;
        }
    }

    if (ret == SYS_ERR_OK)
    {
        authInfo.authSts = SYS_AUTH;
        status = TRUE;

        cgi_util_authingSession_destroy(authInfo.tcpPort);
    }
    else
    {
        authInfo.authSts = SYS_UNAUTH;
        status = FALSE;
    }

    sal_authmgr_webauthStatus_set(&authInfo);

    return status;
}

int main(int argc, char* argv[], char* envp[])
{
    FILE *fp = NULL;
    char file[SYS_BUF128_LEN];
    char file2[SYS_BUF128_LEN];
    char username[SYS_BUF256_LEN + 1];
    char password[SYS_BUF256_LEN + 1];
    char remotePort[SYS_BUF16_LEN];

    SYS_MEM_CLEAR(file);
    SYS_MEM_CLEAR(file2);
    SYS_MEM_CLEAR(remotePort);
    SYS_MEM_CLEAR(username);
    SYS_MEM_CLEAR(password);

    osal_sprintf(file, "/tmp/webauthSess_info_%s", argv[1]);

    usleep(500000);

    if (NULL != (fp = fopen(file, "r")))
    {
        fgets(username, sizeof(username), fp);
        fgets(password, sizeof(password), fp);
        fgets(remotePort, sizeof(remotePort), fp);
        fgets(file2, sizeof(file2), fp);

        if (osal_strlen(username) > 0)
            username[osal_strlen(username) - 1] = '\0';

        if (osal_strlen(password) > 0)
            password[osal_strlen(password) - 1] = '\0';

        if (osal_strlen(file2) > 0)
            file2[osal_strlen(file2) - 1] = '\0';

        if (osal_strlen(remotePort) > 0)
            remotePort[osal_strlen(remotePort) - 1] = '\0';

        fclose(fp);
    }

    unlink(file);

    if (NULL != (fp = fopen(file2, "w")))
    {
        fprintf(fp, "~webauth~%d", CGI_SESSION_AUTHING);
        fclose(fp);
    }

    if (TRUE == _login_auth(username, password, remotePort))
    {
        #if 0
        if (NULL != (fp = fopen(file2, "w")))
        {
            fprintf(fp, "%d", CGI_SESSION_AUTHING_SUCCESS);
            fclose(fp);
        }
        #endif
    }
    else
    {
        if (NULL != (fp = fopen(file2, "w")))
        {
            fprintf(fp, "~webauth~%d", CGI_SESSION_AUTHING_FAIL);
            fclose(fp);
        }
    }

    return 0;

}

