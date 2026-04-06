
#include <include/cgi.h>
#include <libcmd/cmd.h>

static int32 cgi_home_loginAuth_set(void);
static int32 cgi_home_save_set(void);
static int32 cgi_home_logout_set(void);
static int32 cgi_home_refresh_set(void);
static int32 cgi_home_debugDel_set(void);
static int32 cgi_home_systemSet_set(void);
static int32 cgi_home_sysOemSet_set(void);

const cgi_cmdmap_t cmdSetHomeTbl[] =
{
    {CGI_CMD_HOME_LOGINAUTH,        cgi_home_loginAuth_set},
    {CGI_CMD_HOME_SAVE,             cgi_home_save_set},
    {CGI_CMD_HOME_LOGOUT,           cgi_home_logout_set},
    {CGI_CMD_HOME_REFRESH,          cgi_home_refresh_set},
    {CGI_CMD_HOME_DEBUGDEL,         cgi_home_debugDel_set},
    {CGI_CMD_HOME_SYSTEMSET,        cgi_home_systemSet_set},  
    {CGI_CMD_HOME_SYSOEMSET,        cgi_home_sysOemSet_set},
    {NULL, NULL}
};

static int32 cgi_home_loginAuth_set(void)
{
    FILE *fp = NULL;
    char *value = NULL;
    char username[CAPA_SYS_USERNAME_LEN + 1];
    char password[SYS_BUF256_LEN + 1];
    cgi_session_t sessionInfo;

    SYS_MEM_CLEAR(username);
    SYS_MEM_CLEAR(password);

    if (CGI_IS_PARAM_EXIST(CGI_HOME_LOGIN_USERNAME, value))
    {
        strlcpy(username, value, sizeof(username));
    }

    if (CGI_IS_PARAM_EXIST(CGI_HOME_LOGIN_PASSWORD, value))
    {
        if (getenv("ACCESS_TYPE") == NULL)
        {
            if(sal_util_rsa_decrypt(SYS_SEC_HTTP_RSA_FILE, value, password) != SYS_ERR_OK)
                strlcpy(password, value, sizeof(password));
        }
        else
            strlcpy(password, value, sizeof(password));
    }

    SYS_MEM_CLEAR(sessionInfo);

    sessionInfo.username = username;
    sessionInfo.password = password;

    cgi_util_authingSession_create(&sessionInfo);

    if (NULL != (fp = fopen(SYS_POLLD_ACT_WEB_LOGIN, "w")))
    {
        fprintf(fp, "/home/web/cgi/login.cgi %d &", getpid());
        fclose(fp);
    }
    
	system("handler -w login 1> /dev/null 2> /dev/null &");
	
    cgi_set_respGeneral_send(SYS_ERR_OK);

    return SYS_ERR_OK;
}

static int32 cgi_home_save_set(void)
{
    int32 ret = SYS_ERR_FAILED;

    ret = cmd_conf_startupConfig_save();

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_home_logout_set(void)
{
    char file[SYS_BUF128_LEN];
    char *remoteIp = getenv("REMOTE_ADDR");
    char *accessTypeEnv = getenv("ACCESS_TYPE");
    cgi_session_t sessionInfo;
    cgi_session_userAgent_t userAgent = 0;

    SYS_MEM_CLEAR(file);
    SYS_MEM_CLEAR(sessionInfo);

    cgi_util_session_userAgent_get(&userAgent);

    if (accessTypeEnv != NULL && 0 == osal_strcmp(accessTypeEnv, "https"))
        sessionInfo.accessType = SYS_ACCESS_WEB_HTTPS;
    else
        sessionInfo.accessType = SYS_ACCESS_WEB_HTTP;

    if (0 == osal_strncmp(remoteIp, "::ffff:", 7))
        remoteIp = remoteIp + 7;

    sessionInfo.remoteIp = remoteIp;
    sessionInfo.userAgent = userAgent;

    cgi_util_session_destroy(&sessionInfo);

    cgi_set_respGeneral_send(SYS_ERR_OK);

    return SYS_ERR_OK;
}

static int32 cgi_home_refresh_set(void)
{
    cJSON *status = NULL;
    status = cJSON_CreateObject();

    cJSON_AddStringToObject(status, "status", "ok");
    cJSON_AddStringToObject(status, "msgType", "success");

    CGI_RESPONSE_JSON(status);

    cJSON_Delete(status);

    return SYS_ERR_OK;
}

static int32 cgi_home_debugDel_set(void)
{
    int32 ret = SYS_ERR_OK;

    unlink(CGI_DBG_TARGET_FILE);

    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_home_systemSet_set(void)
{
    char *value = NULL;
    char macStr[CAPA_MAC_STR_LEN];
    char snStr[CAPA_SYS_NAME_LEN + 1];
    char sysname[CAPA_SYS_NAME_LEN + 1];

    SYS_MEM_CLEAR(macStr);
    SYS_MEM_CLEAR(snStr);
    SYS_MEM_CLEAR(sysname);

    if (CGI_IS_PARAM_EXIST(CGI_HOME_SYSTEM_MAC, value))
    {
        osal_strcpy(macStr, value);
        if(TRUE == sal_util_macStrValidate_ret(macStr, SYS_MAC_TYPE_UNICAST))
        {
            sal_sys_bdinfoVar_set("ethaddr",macStr);
		}
		else
		{
            cgi_set_respGeneral_send(SYS_ERR_FAILED);
		}
    }

    if (CGI_IS_PARAM_EXIST(CGI_HOME_SYSTEM_SN, value))
    {
        osal_strcpy(snStr, value);
		sal_sys_bdinfoVar_set("serialnumber",snStr);
    }

    if (CGI_IS_PARAM_EXIST(CGI_HOME_SYSTEM_ID, value))
    {
        osal_strcpy(sysname, value);
        sal_sys_bdinfoVar_set("modelname",sysname);
    }

	cgi_set_respGeneral_send(SYS_ERR_OK);

    return SYS_ERR_OK;
}

static int32 cgi_home_sysOemSet_set(void)
{
    char *value = NULL;
    char filetype[CAPA_MAC_STR_LEN];
	FILE *fp = NULL;
	char sysHomeLogo[CAPA_SYS_NAME_LEN + 1];

    SYS_MEM_CLEAR(filetype);

    if (CGI_IS_PARAM_EXIST(CGI_HOME_OEM_LOGO, value))
    {

		if (NULL == (fp =  fopen(CONF_HTTP_OEM_CFG, "w")))
        {
             fclose(fp);
             goto end;     
        }
		if(osal_strcmp(value, "logo_ch.png") == 0)
		{
			sal_sys_system_home_logo_get(sysHomeLogo);
			fprintf(fp, "%s\n", sysHomeLogo);
		}
		else if(osal_strcmp(value, "logo_login_ch.png") == 0)
		{
			sal_sys_system_login_logo_get(sysHomeLogo);
			fprintf(fp, "%s\n", sysHomeLogo);
		}
		else 
		{
			fprintf(fp, "%s\n", value);
		}
        
        fclose(fp);     
    }
end:
	cgi_set_respGeneral_send(SYS_ERR_OK);

    return SYS_ERR_OK;
}

