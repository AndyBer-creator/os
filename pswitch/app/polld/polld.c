/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 16227 $
 * $Date: 2011-03-10 18:23:52 +0800 (Thu, 10 Mar 2011) $
 *
 * Purpose : Polling some events and information
 *
 * Feature : Polling some events and information
 */

/*
 * Include Files
 */
#include <signal.h>
#include <dirent.h>

#include <common/sys_util.h>
#include <libsal/sal_util.h>
#include <libsal/sal_led.h>
#include <libsal/sal_mib.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_port.h>
#include <libsal/sal_tftp.h>
#include <libsal/sal_crash.h>
#include <libcmd/cmd_conf.h>
#include <libcmd/cmd_parser.h>
#include <libsal/sal_l3.h>
/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
#ifdef CONFIG_SYS_SYSLOG
char logcmd[SYS_BUF64_LEN];
#endif
#ifdef CONFIG_SYS_PLAT_LXKSI
  /* empty */
#else
  uint8 portLinkStatus[SYS_LOGICPORT_NUM_MAX];
#endif

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

void handlePolldAction(int signo)
{
    FILE *fp = NULL;

    if (SYS_ERR_OK == sal_util_file_exist(SYS_POLLD_ACT_FWLOCK_TIMER))
    {
        /* Clear action file */
        unlink(SYS_POLLD_ACT_FWLOCK_TIMER);

        /* Do action */
        system("handler -f lockfwtimer 1> /dev/null 2> /dev/null &");
        //system("handler -f lockfwtimer&");
    }

    if (NULL != (fp = fopen(SYS_POLLD_ACT_CUSTOM, "r")))
    {
        char cmd[SYS_BUF128_LEN];

        SYS_MEM_CLEAR(cmd);

        fgets(cmd, sizeof(cmd), fp);

        if (0 != osal_strlen(cmd))
            system(cmd);

        fclose(fp);
        unlink(SYS_POLLD_ACT_CUSTOM);
    }

    if (NULL != (fp = fopen(SYS_POLLD_ACT_WEB_LOGIN, "r")))
    {
        char cmd[SYS_BUF128_LEN];

        SYS_MEM_CLEAR(cmd);

        fgets(cmd, sizeof(cmd), fp);

        if (0 != osal_strlen(cmd))
            system(cmd);

        fclose(fp);
        unlink(SYS_POLLD_ACT_WEB_LOGIN);
    }

#ifdef CONFIG_SYS_PING_TEST
    if (NULL != (fp = fopen(SYS_POLLD_ACT_WEB_PING, "r")))
    {
        char cmd[SYS_BUF128_LEN];
        SYS_MEM_CLEAR(cmd);
        fgets(cmd, sizeof(cmd), fp);

        if (0 != osal_strlen(cmd))
            system(cmd);

        fclose(fp);
        unlink(SYS_POLLD_ACT_WEB_PING);
    }
#endif
#ifdef CONFIG_SYS_TECH_SUPPORT
    if (NULL != (fp = fopen(SYS_POLLD_ACT_TECH_SUPPORT_GEN, "r")))
    {
        char cmd[SYS_BUF128_LEN];
        SYS_MEM_CLEAR(cmd);
        fgets(cmd, sizeof(cmd), fp);
        if (0 != osal_strlen(cmd))
            system(cmd);

        fclose(fp);
        unlink(SYS_POLLD_ACT_TECH_SUPPORT_GEN);
    }
#endif
#ifdef CONFIG_SYS_TRACE_ROUTE
    if (NULL != (fp = fopen(SYS_POLLD_ACT_WEB_TRACEROUTE, "r")))
    {
        char cmd[SYS_BUF512_LEN];
        SYS_MEM_CLEAR(cmd);
        fgets(cmd, sizeof(cmd), fp);

        if (0 != osal_strlen(cmd))
            system(cmd);

        fclose(fp);
        unlink(SYS_POLLD_ACT_WEB_TRACEROUTE);
    }
#endif
}

#ifdef CONFIG_SYS_APP_HTTPD
void httpd_restart(int signo)
{
#if 0
#define HTTPD_HIDE_MSG ""
#else
#define HTTPD_HIDE_MSG "1> /dev/null 2> /dev/null"
#endif

    char cmd[SYS_BUF128_LEN];
    uint32 pid = 0;
    sys_enable_t http = DISABLED;
    sys_enable_t https = DISABLED;
    FILE *fp = NULL;

    sal_sys_httpEnable_get(&http);
  #ifdef CONFIG_USER_BOA_WITH_SSL
    sal_sys_httpsEnable_get(&https);
  #endif

    if (ENABLED == http && ENABLED == https)
    {
        /* If this type already enabled, do nothing.*/
        if (SYS_ERR_OK != sal_util_file_exist(HTTPS_PID_FILE))
        {
            osal_sprintf(cmd, "boa -c /home/ -s %s &", HTTPD_HIDE_MSG);
            system(cmd);
        }

        /* If this type already enabled, do nothing.*/
        if (SYS_ERR_OK != sal_util_file_exist(HTTP_PID_FILE))
        {
            osal_sprintf(cmd, "boa -c /home/ %s &", HTTPD_HIDE_MSG);
            system(cmd);
        }
    }
    else if (DISABLED == http && ENABLED == https)
    {
        /* If this type already enabled, do nothing.*/
        if (SYS_ERR_OK != sal_util_file_exist(HTTPS_PID_FILE))
        {
            osal_sprintf(cmd, "boa -c /home/ -s %s &", HTTPD_HIDE_MSG);
            system(cmd);
        }

        /* If other type enabled, disable it.*/
        if (SYS_ERR_OK == sal_util_file_exist(HTTP_PID_FILE))
        {
            fp = fopen(HTTP_PID_FILE, "r");
            fscanf(fp, "%d", &pid);
            fclose(fp);

            unlink(HTTP_PID_FILE);

            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "kill -9 %d 1> /dev/null 2> /dev/null", pid);
            system(cmd);
        }
    }
    else if (ENABLED == http && DISABLED == https)
    {
        /* If this type already enabled, do nothing.*/
        if (SYS_ERR_OK != sal_util_file_exist(HTTP_PID_FILE))
        {
            osal_sprintf(cmd, "boa -c /home/ %s &", HTTPD_HIDE_MSG);
            system(cmd);
        }

        /* If other type enabled, disable it.*/
        if (SYS_ERR_OK == sal_util_file_exist(HTTPS_PID_FILE))
        {
            fp = fopen(HTTPS_PID_FILE, "r");
            fscanf(fp, "%d", &pid);
            fclose(fp);

            unlink(HTTPS_PID_FILE);

            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "kill -9 %d 1> /dev/null 2> /dev/null", pid);
            system(cmd);
        }
    }
    else
    {
        unlink(HTTP_PID_FILE);
        unlink(HTTPS_PID_FILE);
        sal_util_process_kill("boa");
    }

  #ifdef CONFIG_SYS_PROTO_WEBAUTH
    {
        sys_enable_t webauthEnbl = DISABLED;

        sal_authmgr_typeEnbl_get(AUTHMGR_TYPE_WEB, &webauthEnbl);

        if (ENABLED == webauthEnbl)
        {
            /* If this type already enabled, do nothing.*/
            if (SYS_ERR_OK != sal_util_file_exist(WEBAUTH_HTTPS_PID_FILE))
            {
                osal_sprintf(cmd, "boa -c /home/ -s -w %s &", HTTPD_HIDE_MSG);
                system(cmd);
            }

            /* If this type already enabled, do nothing.*/
            if (SYS_ERR_OK != sal_util_file_exist(WEBAUTH_HTTP_PID_FILE))
            {
                osal_sprintf(cmd, "boa -c /home/ -w %s &", HTTPD_HIDE_MSG);
                system(cmd);
            }
        }
        else
        {
            if (SYS_ERR_OK == sal_util_file_exist(WEBAUTH_HTTP_PID_FILE))
            {
                fp = fopen(WEBAUTH_HTTP_PID_FILE, "r");
                fscanf(fp, "%d", &pid);
                fclose(fp);

                unlink(WEBAUTH_HTTP_PID_FILE);

                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "kill -9 %d 1> /dev/null 2> /dev/null", pid);
                system(cmd);
            }

            if (SYS_ERR_OK == sal_util_file_exist(WEBAUTH_HTTPS_PID_FILE))
            {
                fp = fopen(WEBAUTH_HTTPS_PID_FILE, "r");
                fscanf(fp, "%d", &pid);
                fclose(fp);

                unlink(WEBAUTH_HTTPS_PID_FILE);

                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "kill -9 %d 1> /dev/null 2> /dev/null", pid);
                system(cmd);
            }
        }
    }
  #endif
}
#endif

void handleUpgradeFw(int signo)
{
    system("handler -f fwupgrade 1> /dev/null 2> /dev/null &");
}

void handleSaveRunning(int signo)
{
    system("handler -f saverunning 1> /dev/null 2> /dev/null &");
}

void handleSaveOem(int signo)
{
    if(SYS_ERR_OK == sal_util_file_exist(CONF_HTTP_LOGO_TMPFILE))
    {
        system("handler -f saveOemtoFlash 1> /dev/null 2> /dev/null &");
    }
    if(SYS_ERR_OK == sal_util_file_exist(CONF_HTTP_USRCFG_TMPFILE))
    {
       system("handler -f saveUsrCfgtoFlash 1> /dev/null 2> /dev/null &");
    }
}

#ifdef CONFIG_SYS_PLAT_LXKSI
#else /* If KSI is disabled, use polld to process reset button */
static void monitorResetBtn(void)
{
    uint32 resetStatus = SYS_RESET_ACT_NONE;
    /* If KSI is disabled, use polld to process reset button */

    SYS_ERR_CHK(sal_sys_resetStatus_get(&resetStatus));

    switch (resetStatus)
    {
        case SYS_RESET_ACT_RESTORE_FACTORY:
            SYS_ERR_CHK(sal_sys_restore_defaults(ENABLED));
            SYS_ERR_CHK(sal_led_systemCtrl_set(SYS_LED_STATE_ALLPORT_ON));
            SYS_PRINTF("Restore Factory Default Success!\n");
            break;

        case SYS_RESET_ACT_RESTORE_ACCOUNT:
            {
                uint32 i;
                sys_userInfo_t userInfo;

                for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
                {
                    osal_memset(&userInfo, 0, sizeof(userInfo));
                    SYS_ERR_CHK(sal_sys_localUser_get(i, &userInfo));
                    if (userInfo.priv < 0)
                        continue;

                    SYS_ERR_CHK(sal_sys_localUser_del(userInfo.username));
                }
                osal_memset(&userInfo, 0, sizeof(userInfo));
                sal_sys_get_sys_defaul_user(&userInfo);
                //osal_strcpy(userInfo.username, DFLT_SYS_ADMIN_USERNAME);
               // osal_strcpy(userInfo.password, DFLT_SYS_ADMIN_PASSWORD);
                userInfo.priv = DFLT_SYS_ADMIN_PRIV;
                userInfo.passwdType = SYS_PASSWD_TYPE_ENCRYPT;
                SYS_ERR_CHK(sal_sys_localUserForce_add(userInfo));
            }
            SYS_PRINTF("Restore Default Username/Password Success!\n");
            break;

        case SYS_RESET_ACT_REBOOT:
        default:
            break;
    }

    if (SYS_RESET_ACT_NONE != resetStatus)
    {
        SYS_PRINTF("Rebooting now...\n");
        SYS_ERR_CHK(sal_sys_reboot());
    }

    return;
}
#endif

#ifdef CONFIG_SYS_UI_SNMP
  #ifdef CONFIG_SYS_MIB_TRAP
    #ifdef CONFIG_SYS_PLAT_LXKSI
    #else /* If KSI is disabled, use polld to process snmp link trap */
static void monitorSnmpLinkTrap(void)
{
    sys_logic_port_t lp;
    sys_port_linkStatus_t link;

    /* Send snmp link-up/down trap */
    FOR_EACH_LOGIC_PORT(lp)
    {
        sal_port_linkStatus_get(lp, &link);
        if(link != portLinkStatus[lp])
        {
           //sal_snmp_linkUpDownTrap_send(lp, link);
           sal_snmp_linkUpDownTrap_send_new(lp,link);
           portLinkStatus[lp] = link;
        }
    }
    return;
}

    #endif
  #endif
#endif

#ifdef CONFIG_SYS_SYSLOG
static void monitorSyslogFile(void)
{
    uint32 ramFilesize = 0;
    uint32 flashFilesize = 0;

    ramFilesize = sal_util_fileSize_ret(LOG_FILE_FLASH_TMP);
    flashFilesize = sal_util_fileSize_ret(LOG_FILE_FLASH);

    if(ramFilesize != flashFilesize)
        system(logcmd);

    return;
}
static void monitorSyslogFileSize(void)
{
    uint32 ramFileLineLen = 0;
    uint32 flashFileLineLen = 0;
    uint32 delLine = 0;
	sal_log_target_t bufTarget;
	sal_log_target_t flashTarget;
	
	//get buffconf 
	sal_log_target_get(1, &bufTarget);
	//get flashconf
	sal_log_target_get(2, &flashTarget);

	if(bufTarget.enable)
	{
		ramFileLineLen = sal_util_file_line(LOG_FILE_RAM);
		if(ramFileLineLen > bufTarget.totalCount)
		{
			delLine = bufTarget.totalCount/10 + ramFileLineLen - bufTarget.totalCount;
			sal_util_file_del_head_line(LOG_FILE_RAM, delLine);
		}
	}

	if(flashTarget.enable)
	{
		flashFileLineLen = sal_util_file_line(LOG_FILE_FLASH);

		if(flashFileLineLen > flashTarget.totalCount)
		{
			delLine = flashTarget.totalCount/10 + flashFileLineLen - flashTarget.totalCount;
			sal_util_file_del_head_line(LOG_FILE_FLASH, delLine);
		}
	}
    return;
}



#endif

#ifdef CONFIG_SYS_L3_ROUTE
static void monitorL3Engine(void)
{
    FILE *fp = NULL;
    int32 pid = 0;
    char buf[SYS_BUF64_LEN];

    if (NULL == (fp = fopen(L3_ENGINE_PID_FILE, "r")))
        return;

    fscanf(fp, "%d", &pid);
    fclose(fp);

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "/proc/%d/cmdline", pid);

    if (NULL != (fp = fopen(buf, "r")))
    {
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);

        if (0 == osal_strstr(buf, "zebra"))
            sal_l3_engine_restart();

        fclose(fp);
    }
    else
    {
        sal_l3_engine_restart();
    }

    return;
}

static void monitorL3IntfvSts(void)
{
    uint32 i = 0;
    sys_l3_intfStsChg_t stsChg;
    char intfStr[CAPA_L3_INTFNAME_LEN];
    sys_l3_intfId_t intfId;

    for (i = 0; i < CAPA_L3_INTFVLAN_NUM_MAX; i++)
    {
        SYS_MEM_CLEAR(intfStr);
        SYS_MEM_CLEAR(intfId);

        osal_sprintf(intfStr, "%s%d", L3_INTF_PREFIX_VLAN, i);

        STR2INTFID(intfStr, &intfId);

        if (!IS_L3_INTF_EXIST(&intfId))
            continue;

        SYS_MEM_CLEAR(stsChg);
        sal_l3_intfLinkStatusChg_get(&intfId, &stsChg);

        if (TRUE == stsChg.stsChg && FALSE == stsChg.process)
        {
            SYSTEM_CMD_BG("handler -f l3intf_sts %s %d", intfStr, stsChg.linkup);
        }
    }

    return;
}

#endif

/*
 * Check console cli is dead or not. If console cli is dead, reboot the system automatically.
 */
static void monitorCliConsole(void)
{
    FILE *fp = NULL;
    int32 pid = 0;
    char buf[SYS_BUF64_LEN];

    if (NULL == (fp = fopen(SYS_CONSOLE_CLI_PID_FILE, "r")))
        return;

    fscanf(fp, "%d", &pid);
    fclose(fp);

    SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "/proc/%d/cmdline", pid);

    if (NULL != (fp = fopen(buf, "r")))
    {
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);

        /* PID file exist but process is not cli, reboot system */
        if (0 == osal_strstr(buf, "cli"))
            sal_sys_reboot();

        fclose(fp);
    }
    else
    {
        /* PID file exist but process dead, reboot system */
        sal_sys_reboot();
    }

    return;
}

static void monitorSessionInfo(void)
{
    char filename[SYS_BUF32_LEN];
    char buf[SYS_BUF256_LEN];
    DIR *dir = NULL;
    struct dirent *entry;
    FILE *fp = NULL;
	//int ppid = -1;
	int pid = -1;
    if ((dir = opendir(SYS_SESSION_DIR)) == NULL)
        return;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
                continue;
        }

        SYS_MEM_CLEAR(filename);
        osal_sprintf(filename, "%s/%s", SYS_SESSION_DIR, entry->d_name);
        if (NULL == (fp = fopen(filename, "r")))
            continue;

        /* pid */
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);
        pid = atoi(buf);
#if 0
        /* ppid */
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);
		ppid = atoi(buf); 
#endif		
		fclose(fp);
		
		if(FALSE == sal_util_processExist_ret(pid))
		{
			unlink(filename);
		}
    }
    closedir(dir);
}

static void monitorLineInfo(void)
{
    char *c = NULL;
    char typeStr[SYS_BUF8_LEN];
    char filename[SYS_BUF32_LEN];
    char buf[SYS_BUF256_LEN];
    DIR *dir = NULL;
    struct dirent *entry;
    FILE *fp = NULL;
    sys_line_session_t session;

    if ((dir = opendir(SYS_LINEINFO_DIR)) == NULL)
        return;

    SYS_MEM_CLEAR(session);

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
                continue;
        }

        /* Skip type console */
        if (NULL != osal_strstr(entry->d_name, text_access_type[SYS_ACCESS_CLI_CONSOLE].text))
            continue;

        /* Skip type http/https */
        if (NULL != osal_strstr(entry->d_name, text_access_type[SYS_ACCESS_WEB_HTTP].text))
            continue;

        c = entry->d_name;
        while (*c != '_' && osal_strlen(c) > 0)
        {
            c++;
        }

        /* Do nothing if process is still exist */
        if (TRUE == sal_util_processExist_ret(SYS_STR2UINT(c + 1)))
            continue;

        SYS_MEM_CLEAR(typeStr);
        osal_strncpy(typeStr, entry->d_name, c - entry->d_name);

        SYS_MEM_CLEAR(filename);
        osal_sprintf(filename, "%s/%s", SYS_LINEINFO_DIR, entry->d_name);
        if (NULL == (fp = fopen(filename, "r")))
            continue;

        /* pid */
        fgets(buf, sizeof(buf), fp);

        /* time */
        fgets(buf, sizeof(buf), fp);

        /* username */
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);
        buf[osal_strlen(buf) - 1] = '\0';
        osal_strcpy(session.user, buf);

        /* host */
        SYS_MEM_CLEAR(buf);
        fgets(buf, sizeof(buf), fp);
        osal_strcpy(session.host, buf);

        fclose(fp);

        SYS_LOG(LOG_CAT_AAA, LOG_AAA_DISCONNECT,
                    typeStr,
                    session.user,
                    session.host);
        #ifdef CONFIG_SYS_MIB_TRAP
    	       /*tarp reboot msg*/
              sal_snmp_system_auth_logout_Trap_send_new(session.user, typeStr, session.host);
        #endif

        unlink(filename);
    }
    closedir(dir);
}

static void monitorwebLineCheck(void)
{
    char *c = NULL;
    char typeStr[SYS_BUF8_LEN];
    char filename[SYS_BUF256_LEN];
    char linefile[SYS_BUF128_LEN];
    char buf[SYS_BUF256_LEN];
    DIR *dir = NULL;
    struct dirent *entry;
    FILE *fp = NULL;
    sys_line_session_t session;
    uint32 now_sec = 0;
    uint32 last = 0;
    uint32 timeout = 0;
    char *httpname = NULL;
    char *httpsname = NULL;

    if ((dir = opendir("/tmp")) == NULL)
        return;

	osal_time_sysUptime_get(&now_sec);

    SYS_MEM_CLEAR(session);

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
                continue;
        }

        /* find http */
        if ((NULL != (httpname = osal_strstr(entry->d_name, text_access_type[SYS_ACCESS_WEB_HTTP].text))) ||
        	(NULL != (httpsname = osal_strstr(entry->d_name, text_access_type[SYS_ACCESS_WEB_HTTPS].text))))
        {
			SYS_MEM_CLEAR(filename);
        	osal_sprintf(filename, "/tmp/%s", entry->d_name);
        	if (NULL == (fp = fopen(filename, "r")))
            	continue;
            /* priv */
	        fgets(buf, sizeof(buf), fp);

	        /* last time*/
	        SYS_MEM_CLEAR(buf);
	        fgets(buf, sizeof(buf), fp);
	        buf[osal_strlen(buf) - 1] = '\0';
	        last = SYS_STR2UINT(buf);
			fclose(fp);
			if(httpname)
			{
	        	/* Convert minutes to seconds */
    			sal_sys_webSessionTimeout_get(SYS_LINE_WEB_HTTP, &timeout);
    		}
    		else 
    		{
				/* Convert minutes to seconds */
    			sal_sys_webSessionTimeout_get(SYS_LINE_WEB_HTTPS, &timeout);
    		}
    		timeout *= 60;
		    if (timeout != 0)
		    {
		        if((now_sec - last)/1000 > timeout)
		        {
		            //超时
		            unlink(filename);
		            SYS_MEM_CLEAR(linefile);
		            if(httpname)
		            {
		            	osal_sprintf(linefile, "%s/%s", SYS_LINEINFO_DIR, httpname);
		            }
		            else 
		            {
						osal_sprintf(linefile, "%s/%s", SYS_LINEINFO_DIR, httpsname);
		            }
		            unlink(linefile);
		        }
		    }
        }       
    }
    closedir(dir);
}


/*
 * 检查在加载SNMP配置时只加载了配置未能重启SNMP
 */
static void monitorRunConfigSnmpconfig(void)
{
	int ret = SYS_ERR_OK;
	sys_enable_t enable;

	sal_snmp_enable_get(&enable);
	if(enable && CMD_IS_CLI_RUN())
	{
		ret = sal_util_file_exist(SNMP_CONF_VAR_FILE);
		if(ret == SYS_ERR_FILE_NOT_EXIST)
		{
			sal_snmp_restart();
		}
	}

    return;
}

#ifdef CONFIG_SYS_TEST_SW_CHECK
/*设备自动重启*/
static void monitorDeviceReboot(void)
{
	static int times = 0;
	times += 60*10;
	if(times > (60*60*CONFIG_SYS_TESTSW_REBOOT_TIMES))
	{
		SYS_PRINTF("Do not use commercial test software!!! Restart after 5 seconds...\n");
		sleep(5);
        sal_sys_reboot();
	}
	return;
}
#endif

int main(int argc, char **argv)
{
    uint32 monitorTime = 0;


    signal(SIGUSR1, handlePolldAction);
    handlePolldAction(SIGUSR1);

    signal(SIGTSTP, handleSaveRunning);

#ifdef CONFIG_SYS_UI_WEB
    signal(SIGURG,  httpd_restart);
    httpd_restart(0);
#endif
    signal(SIGHUP,  handleUpgradeFw);

    signal(SIGUSR2, handleSaveOem);
    handleSaveOem(SIGUSR2);

#ifdef CONFIG_SYS_SYSLOG
    /* Copy log file in flash card to ram when start-up */
    if(SYS_ERR_OK == sal_util_file_exist(LOG_FILE_FLASH))
    {
        osal_memset(logcmd, 0, sizeof(logcmd));
        osal_sprintf(logcmd, "cp %s %s", LOG_FILE_FLASH, LOG_FILE_FLASH_TMP);
        system(logcmd);
    }

    /* for later use */
    osal_memset(logcmd, 0, sizeof(logcmd));
    osal_sprintf(logcmd, "cp %s %s", LOG_FILE_FLASH_TMP, LOG_FILE_FLASH);
#endif

    while(1)
    {
        /* Check every 1 second */
#ifdef CONFIG_SYS_PLAT_LXKSI
#else
        monitorResetBtn();
  #ifdef CONFIG_SYS_UI_SNMP
    #ifdef CONFIG_SYS_MIB_TRAP
        monitorSnmpLinkTrap();
    #endif
  #endif
#endif
#ifdef CONFIG_SYS_L3_ROUTE
        monitorL3IntfvSts();
#endif

        /* Check every 5 seconds */
        if (0 == (monitorTime % 5))
        {
            monitorCliConsole();
            monitorSessionInfo();
            monitorLineInfo();
			monitorwebLineCheck();

#ifdef CONFIG_SYS_L3_ROUTE
            monitorL3Engine();
#endif
			monitorRunConfigSnmpconfig();
        }

        /* Check every 30 seconds */
        if (0 == (monitorTime % 30))
        {
            monitorSyslogFile();
        }
#ifdef CONFIG_SYS_TEST_SW_CHECK
		/* check every 10 mins*/
		if(0 == (monitorTime % 60*10))
		{
			monitorDeviceReboot();
		}
#endif

        /* Check every 60 seconds */
        if (0 == (monitorTime % 300))
        {
            monitorSyslogFileSize();
        }

        monitorTime++;

        sleep(1);
    }

    return SYS_ERR_OK;
}

