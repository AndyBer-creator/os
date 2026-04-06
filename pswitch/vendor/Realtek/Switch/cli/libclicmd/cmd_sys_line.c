
#include <libsal/sal_sys.h>
#include <libcmd/cmd.h>

#include <termios.h>

sys_line_cliType_t g_cliLineType = SYS_LINE_CLI_CONSOLE;

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT

int32 cmd_sys_line_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    uint32 timeout;
#endif
    uint32 cnt;
    sys_enable_t enable;
    #ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    sys_cliRetrySilent_t retrySilent;
    #endif 

    if (argc == 0 || (argc == 1 && argv[0][0] == 'c'))
    {
        XMORE("Console ==============================\n");

        #ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
        SYS_ERR_CHK(sal_sys_cliSessionTimeout_get(SYS_LINE_CLI_CONSOLE, &timeout));
        XMORE("    Session Timeout : %d (minutes)\n", timeout);
        #endif 

        SYS_ERR_CHK(sal_sys_cliHistory_count_get(SYS_LINE_CLI_CONSOLE, &cnt));
        XMORE("    History Count   : %d\n", cnt);

        #ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
        SYS_MEM_CLEAR(retrySilent);
        SYS_ERR_CHK(sal_sys_cliRetrySilent_get(SYS_LINE_CLI_CONSOLE, &retrySilent));
        XMORE("    Password Retry  : %d\n", retrySilent.retryNum);
        XMORE("    Silent Time     : %d (seconds)\n", retrySilent.silentTime);
        #endif 
    }

    if (argc == 0 || (argc == 1 && argv[0][0] == 't'))
    {
        XMORE("Telnet ===============================\n");

        #ifdef CONFIG_SYS_APP_TELNETD
        SYS_ERR_CHK(sal_sys_telnetdEnable_get(&enable));
        XMORE("    Telnet Server   : %s\n", text_enable[enable].text);
        #endif

        #ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
        SYS_ERR_CHK(sal_sys_cliSessionTimeout_get(SYS_LINE_CLI_TELNET, &timeout));
        XMORE("    Session Timeout : %d (minutes)\n", timeout);
        #endif 

        SYS_ERR_CHK(sal_sys_cliHistory_count_get(SYS_LINE_CLI_TELNET, &cnt));
        XMORE("    History Count   : %d\n", cnt);

        #ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
        SYS_MEM_CLEAR(retrySilent);
        SYS_ERR_CHK(sal_sys_cliRetrySilent_get(SYS_LINE_CLI_TELNET, &retrySilent));
        XMORE("    Password Retry  : %d\n", retrySilent.retryNum);
        XMORE("    Silent Time     : %d (seconds)\n", retrySilent.silentTime);
        #endif 
    }

  #ifdef CONFIG_SYS_APP_SSH
    if (argc == 0 || (argc == 1 && argv[0][0] == 's'))
    {
        XMORE("SSH ==================================\n");

        #ifdef CONFIG_USER_SSH_SSHD
        SYS_ERR_CHK(sal_ssh_sshdEnable_get(&enable));
        XMORE("    SSH Server      : %s\n", text_enable[enable].text);
        #endif

        #ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
        SYS_ERR_CHK(sal_sys_cliSessionTimeout_get(SYS_LINE_CLI_SSH, &timeout));
        XMORE("    Session Timeout : %d (minutes)\n", timeout);
        #endif 

        SYS_ERR_CHK(sal_sys_cliHistory_count_get(SYS_LINE_CLI_SSH, &cnt));
        XMORE("    History Count   : %d\n", cnt);

        #ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
        SYS_MEM_CLEAR(retrySilent);
        SYS_ERR_CHK(sal_sys_cliRetrySilent_get(SYS_LINE_CLI_SSH, &retrySilent));
        XMORE("    Password Retry  : %d\n", retrySilent.retryNum);
        XMORE("    Silent Time     : %d (seconds)\n", retrySilent.silentTime);
        #endif 
    }
  #endif

    return SYS_ERR_OK;
}
#endif 

int32 cmd_sys_lineHttp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_line_webType_t type;
#ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT
    uint32 timeout;
#endif

    enable = DISABLED;

    if (osal_strlen(argv[0]) == 5)
        type = SYS_LINE_WEB_HTTPS;
    else
        type = SYS_LINE_WEB_HTTP;
#ifdef CONFIG_SYS_UI_WEB
    if (SYS_LINE_WEB_HTTP == type)
    {
        SYS_ERR_CHK(sal_sys_httpEnable_get(&enable));
        XMORE("    HTTP daemon : %s\n", text_enable[enable].text);
    }
#endif

  #ifdef CONFIG_USER_BOA_WITH_SSL
    if (SYS_LINE_WEB_HTTPS == type)
    {
        SYS_ERR_CHK(sal_sys_httpsEnable_get(&enable));
        XMORE("   HTTPS daemon : %s\n", text_enable[enable].text);
    }
  #endif

  #ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT
    SYS_ERR_CHK(sal_sys_webSessionTimeout_get(type, &timeout));
    XMORE("Session Timeout : %d (minutes)\n", timeout);
  #endif 

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_APP_TELNETD

int32 cmd_sys_telnet_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    system("handler -c telnet &");

    return SYS_ERR_OK;
}

int32 cmd_sys_linetelnet_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    enable = DISABLED;
    uint32 port = 0;
    
	SYS_ERR_CHK(sal_sys_telnetdEnable_get(&enable));
	SYS_ERR_CHK(sal_sys_telnetd_port_get(&port));
	XMORE("Telnet config info:\n");
	if(enable)
	{
		XMORE("Telnet Status       :  enable\n");
	}
	else 
	{
		XMORE("Telnet Status       :  disable\n");
	}
	XMORE("Telnet Tcp Port     :  %u\n", port);
    return SYS_ERR_OK;
}

#endif

#ifdef CONFIG_USER_SSH_SSHD

int32 cmd_sys_ssh_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    system("handler -c ssh &");

    return SYS_ERR_OK;
}

int32 cmd_sys_linessh_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    enable = DISABLED;
    uint32 port = 0;
    
	SYS_ERR_CHK(sal_ssh_sshdEnable_get(&enable));
	SYS_ERR_CHK(sal_sys_sshd_port_get(&port));
	XMORE("SSH config info:\n");
	if(enable)
	{
		XMORE("SSH Status       :  enable\n");
	}
	else 
	{
		XMORE("SSH Status       :  disable\n");
	}
	XMORE("SSH Tcp Port     :  %u\n", port);
    return SYS_ERR_OK;
}

#endif

int32 cmd_sys_lineUsers_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_line_session_t session;

    sal_sys_lineSessionInfo_clear();

    XMORE("   Username       Protocol          Location\n");
    XMORE("--------------- ------------ -----------------------\n");

    SYS_MEM_CLEAR(session);
    sal_sys_lineSession_get(SYS_ACCESS_CLI_CONSOLE, 0, &session);
    XMORE("%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_CLI_CONSOLE].text, "0.0.0.0");

    for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_CLI_TELNET); i++)
    {
        SYS_MEM_CLEAR(session);
        SYS_ERR_CONTINUE(sal_sys_lineSession_get(SYS_ACCESS_CLI_TELNET, i, &session));

        XMORE("%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_CLI_TELNET].text, session.host);
    }

#ifdef CONFIG_USER_SSH_SSHD
    for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_CLI_SSH); i++)
    {
        SYS_MEM_CLEAR(session);
        SYS_ERR_CONTINUE(sal_sys_lineSession_get(SYS_ACCESS_CLI_SSH, i, &session));

        XMORE("%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_CLI_SSH].text, session.host);
    }
#endif

    for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_WEB_HTTP); i++)
    {
        SYS_MEM_CLEAR(session);
        SYS_ERR_CONTINUE(sal_sys_lineSession_get(SYS_ACCESS_WEB_HTTP, i, &session));

        XMORE("%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_WEB_HTTP].text, session.host);
    }

    for (i = 0; i < sal_sys_lineSessionCnt_ret(SYS_ACCESS_WEB_HTTPS); i++)
    {
        SYS_MEM_CLEAR(session);
        SYS_ERR_CONTINUE(sal_sys_lineSession_get(SYS_ACCESS_WEB_HTTPS, i, &session));

        XMORE("%12s       %7s      %s\n", session.user, text_access_type[SYS_ACCESS_WEB_HTTPS].text, session.host);
    }

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT

int32 cmd_sys_cliLineType_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if ('c' == argv[0][0])
        g_cliLineType = SYS_LINE_CLI_CONSOLE;
    else if ('t' == argv[0][0])
        g_cliLineType = SYS_LINE_CLI_TELNET;
#ifdef CONFIG_USER_SSH_SSHD
    else if ('s' == argv[0][0])
        g_cliLineType = SYS_LINE_CLI_SSH;
#endif

    if (g_cliLineType == SYS_LINE_CLI_CONSOLE)
        vty->node = VTY_CONSOLE_NODE;
    else
        vty->node = VTY_NODE;

    return SYS_ERR_OK;
}

int32 cmd_sys_cliHistoryCnt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 count = 0;

    if (!CMD_IS_NO_FORM_EXIST())
        count = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_sys_cliHistory_count_set(g_cliLineType, count));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT

int32 cmd_sys_cliSessionTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_sys_cliSessionTimeout_set(g_cliLineType, SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT

int32 cmd_sys_cliPasswordRetry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_cliRetrySilent_t retrySilent;

    SYS_MEM_CLEAR(retrySilent);

    SYS_ERR_CHK(sal_sys_cliRetrySilent_get(g_cliLineType, &retrySilent));

    retrySilent.retryNum = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_sys_cliRetrySilent_set(g_cliLineType, retrySilent));

    return SYS_ERR_OK;
}

int32 cmd_sys_cliSilentTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_cliRetrySilent_t retrySilent;

    SYS_MEM_CLEAR(retrySilent);

    SYS_ERR_CHK(sal_sys_cliRetrySilent_get(g_cliLineType, &retrySilent));

    retrySilent.silentTime = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_sys_cliRetrySilent_set(g_cliLineType, retrySilent));

    return SYS_ERR_OK;
}
#endif 
#endif 

#ifdef CONFIG_SYS_APP_TELNETD

int32 cmd_sys_telnetdEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_CLI_RUN())
    {
        sys_enable_t enable;

        SYS_ERR_CHK(sal_sys_telnetdEnable_get(&enable));

        if (CMD_IS_NO_FORM_EXIST())
        {
            if (enable == DISABLED)
                return SYS_ERR_OK;

            SYS_ERR_CHK(sal_sys_telnetdEnable_set(DISABLED));
            sal_sys_inetdConfig_set();
            sal_inetd_restart();
            XMORE("Telnetd daemon disabled.\n");
        }
        else
        {
            if (enable == ENABLED)
                return SYS_ERR_OK;

            SYS_ERR_CHK(sal_sys_telnetdEnable_set(ENABLED));
            sal_sys_inetdConfig_set();
            sal_inetd_restart();
            XMORE("Telnetd daemon enabled.\n");
        }
        return SYS_ERR_OK;
    }

    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_sys_telnetdEnable_set(DISABLED));
    else
        SYS_ERR_CHK(sal_sys_telnetdEnable_set(ENABLED));

    SYS_ERR_CHK(sal_sys_inetdConfig_set());

    return SYS_ERR_OK;
}

int32 cmd_sys_telnetd_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_sys_telnetd_port_set(DEFAULT_TELNET_PORT_VAL));
    }
    else
    {
        SYS_ERR_CHK(sal_sys_telnetd_port_set(SYS_STR2UINT(argv[0])));
    }

    return SYS_ERR_OK;
}
#endif
#ifdef CONFIG_SYS_APP_TELNET

int32 cmd_sys_telnet_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char cmdstr[SYS_BUF128_LEN];

    osal_memset(cmdstr, 0 ,sizeof(cmdstr));
    
    osal_sprintf(cmdstr, "telnet %s", argv[0]);

    system(cmdstr);

    return SYS_ERR_OK;
}

#endif 

#ifdef CONFIG_USER_SSH_SSHD

int32 cmd_sys_sshdEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_CLI_RUN())
    {
        sys_enable_t enable;

        SYS_ERR_CHK(sal_ssh_sshdEnable_get(&enable));

        if (CMD_IS_NO_FORM_EXIST())
        {
            if (enable == DISABLED)
                return SYS_ERR_OK;

            SYS_ERR_CHK(sal_ssh_sshdEnable_set(DISABLED));
            sal_sys_inetdConfig_set();
            sal_inetd_restart();
            XMORE("SSH daemon disabled.\n");

            system("handler -c ssh &");
        }
        else
        {
            if (enable == ENABLED)
                return SYS_ERR_OK;

            SYS_ERR_CHK(sal_ssh_sshdEnable_set(ENABLED));
            sal_sys_inetdConfig_set();
            sal_inetd_restart();
            XMORE("SSH daemon enabled.\n");
        }

        return SYS_ERR_OK;
    }

    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_ssh_sshdEnable_set(DISABLED));
    else
        SYS_ERR_CHK(sal_ssh_sshdEnable_set(ENABLED));

    SYS_ERR_CHK(sal_sys_inetdConfig_set());

    return SYS_ERR_OK;
}

int32 cmd_sys_sshd_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_sys_sshd_port_set(DEFAULT_SSH_PORT_VAL));
    }
    else
    {
        SYS_ERR_CHK(sal_sys_sshd_port_set(SYS_STR2UINT(argv[0])));
    }

    return SYS_ERR_OK;
}

#endif 
#ifdef CONFIG_USER_SSH_SSH

int32 cmd_sys_ssh_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char cmdstr[SYS_BUF128_LEN];

    osal_memset(cmdstr, 0 ,sizeof(cmdstr));
    
    osal_sprintf(cmdstr, "ssh %s", argv[0]);

    system(cmdstr);

    return SYS_ERR_OK;
}
#endif

int32 cmd_sys_cliTermLength_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if ('n' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
        vty_maxline_set((int32)CAPA_CLI_MAX_LINES_NUM);
    else
        vty_maxline_set(SYS_STR2UINT(argv[0]));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT

int32 cmd_sys_webSessionTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 timeout;
    sys_line_webType_t type;

    timeout = SYS_STR2UINT(argv[1]);

    if (osal_strlen(argv[0]) > 4 && 's' == argv[0][4])
        type = SYS_LINE_WEB_HTTPS;
    else
        type = SYS_LINE_WEB_HTTP;

    SYS_ERR_CHK(sal_sys_webSessionTimeout_set(type, timeout));

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_APP_HTTPD

int32 cmd_sys_httpEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_sys_httpEnable_set(DISABLED));
    else
        SYS_ERR_CHK(sal_sys_httpEnable_set(ENABLED));

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_USER_BOA_WITH_SSL

int32 cmd_sys_httpsEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_sys_httpsEnable_set(DISABLED));
    else
        SYS_ERR_CHK(sal_sys_httpsEnable_set(ENABLED));

    return SYS_ERR_OK;
}
#endif 

 #ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT
static int32 _cmd_sys_webSessionTimeout_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 timeout;

    SYS_ERR_CHK(sal_sys_webSessionTimeout_get(SYS_LINE_WEB_HTTP, &timeout));

    if (DFLT_SYS_SESSION_TIMEOUT_HTTP != timeout)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip http session-timeout %d\n", timeout);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_sys_webSessionTimeout_get(SYS_LINE_WEB_HTTPS, &timeout));

    if (DFLT_SYS_SESSION_TIMEOUT_HTTPS != timeout)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip https session-timeout %d\n", timeout);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_AAA
static int32 _cmd_aaa_webLoginAuthenList_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char authListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];

    SYS_MEM_CLEAR(authListName);
    SYS_ERR_CHK(sal_aaa_loginAuthListName_get(SYS_ACCESS_WEB_HTTP, authListName));

    if (0 != osal_strcmp(DFLT_AAA_LOGIN_AUTHLIST_NAME, authListName))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip http login authentication %s\n", authListName);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_MEM_CLEAR(authListName);
    SYS_ERR_CHK(sal_aaa_loginAuthListName_get(SYS_ACCESS_WEB_HTTPS, authListName));

    if (0 != osal_strcmp(DFLT_AAA_LOGIN_AUTHLIST_NAME, authListName))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip https login authentication %s\n", authListName);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_sys_line_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;
	uint32 port;
    SYS_MEM_CLEAR(cmd);
    enable = DISABLED;

#ifdef CONFIG_SYS_APP_TELNETD
    
    SYS_ERR_CHK(sal_sys_telnetd_port_get(&port));
   
    if (DEFAULT_TELNET_PORT_VAL != port)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip telnet port %u\n", port);
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    
    SYS_ERR_CHK(sal_sys_telnetdEnable_get(&enable));

    if (DFLT_SYS_TELNET_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip telnet\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

#ifdef CONFIG_USER_SSH_SSHD
    
    SYS_ERR_CHK(sal_sys_sshd_port_get(&port));
    if (DEFAULT_SSH_PORT_VAL != port)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip ssh port %u\n", port);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_ssh_sshdEnable_get(&enable));

    if (DFLT_SYS_SSH_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip ssh\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_APP_HTTPD
    
    SYS_ERR_CHK(sal_sys_httpEnable_get(&enable));

    if (DFLT_SYS_HTTP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip http\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

#ifdef CONFIG_USER_BOA_WITH_SSL
    
    SYS_ERR_CHK(sal_sys_httpsEnable_get(&enable));

    if (DFLT_SYS_HTTPS_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip https\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT
    SYS_ERR_CHK(_cmd_sys_webSessionTimeout_save(fp));
#endif

#ifdef CONFIG_SYS_AAA
    SYS_ERR_CHK(_cmd_aaa_webLoginAuthenList_save(fp));
#endif

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT

int32 cmd_sys_vty_line_save(FILE *fp, sys_line_cliType_t type)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 cnt;
#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    uint32 dfltTimeout = 0;
    uint32 timeout = 0;
#endif
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    sys_cliRetrySilent_t dfltRetrySilent;
    sys_cliRetrySilent_t retrySilent;
#endif

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    SYS_ERR_CHK(sal_sys_cliRetrySilent_get(type, &retrySilent));
#endif

    SYS_ERR_CHK(sal_sys_cliHistory_count_get(type, &cnt));

    if (DFLT_CLI_HISTORY_MAX_ENTRY != cnt)
    {
        SYS_MEM_CLEAR(cmd);
        if (0 == cnt)
            osal_sprintf(cmd, "no history\n");
        else
            osal_sprintf(cmd, "history %d\n", cnt);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT
    
    SYS_ERR_CHK(sal_sys_cliSessionTimeout_get(type, &timeout));

    if (SYS_LINE_CLI_CONSOLE == type)
        dfltTimeout = DFLT_SYS_SESSION_TIMEOUT_CONSOLE;
    else if (SYS_LINE_CLI_TELNET == type)
        dfltTimeout = DFLT_SYS_SESSION_TIMEOUT_TELNET;
  #ifdef CONFIG_USER_SSH_SSHD
    else if (SYS_LINE_CLI_SSH == type)
        dfltTimeout = DFLT_SYS_SESSION_TIMEOUT_SSH;
  #endif

    if (dfltTimeout != timeout)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "exec-timeout %d\n", timeout);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT
    SYS_MEM_CLEAR(dfltRetrySilent);
    SYS_MEM_CLEAR(retrySilent);
    SYS_ERR_CHK(sal_sys_cliRetrySilent_get(type, &retrySilent));

    if (SYS_LINE_CLI_CONSOLE == type)
    {
        dfltRetrySilent.retryNum = DFLT_CLI_PASSRETRY_CONSOLE;
        dfltRetrySilent.silentTime = DFLT_CLI_SILENTTIME_CONSOLE;
    }
    else if (SYS_LINE_CLI_TELNET == type)
    {
        dfltRetrySilent.retryNum = DFLT_CLI_PASSRETRY_TELNET;
        dfltRetrySilent.silentTime = DFLT_CLI_SILENTTIME_TELNET;
    }
  #ifdef CONFIG_USER_SSH_SSHD
    else if (SYS_LINE_CLI_SSH == type)
    {
        dfltRetrySilent.retryNum = DFLT_CLI_PASSRETRY_SSH;
        dfltRetrySilent.silentTime = DFLT_CLI_SILENTTIME_SSH;
    }
  #endif

    if (dfltRetrySilent.retryNum != retrySilent.retryNum)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "password-thresh %d\n", retrySilent.retryNum);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (dfltRetrySilent.silentTime != retrySilent.silentTime)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "silent-time %d\n", retrySilent.silentTime);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

    return SYS_ERR_OK;
}
#endif

