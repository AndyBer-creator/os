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
 * Purpose : Process special event which can't be processed in normal thread
 *
 * Feature : Process special event which can't be processed in normal thread
 */

/*
 * Include Files
 */
#include <common/sys_util.h>
#include <libcmd/cmd.h>
#include <libsal/sal_util.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_port.h>
#include <libsal/sal_tftp.h>
#include <libcmd/cmd_conf.h>
#ifdef CONFIG_SYS_TECH_SUPPORT
#include <libsal/sal_sys_tech.h>
#endif
#include <libfds/fds_default_init.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

#define HANDLER_DBG_TARGET_FILE "/tmp/HANDLER_DEBUG"
#if 0
#define HANDLER_DBG(a,args...) \
 do { \
	 char tmp_str[256] = ""; \
	 char cmd_str[300] = ""; \
	 sprintf(tmp_str, "[HANDLER_DBG] %s:%d:"#a"", __FUNCTION__,__LINE__, ##args); \
	 sprintf(cmd_str, "echo \"%s\" >> %s", tmp_str, HANDLER_DBG_TARGET_FILE); \
	 system(cmd_str); \
 } while (0)
#else
#define HANDLER_DBG(a,args...) {}
#endif

void handleWebloginAuth(void)
{
	FILE *fp = NULL;
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
}

void handleHttpd_restart(void)
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
	int i = 0;
	sys_line_session_t session;
	
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
  	if(http == DISABLED)
  	{
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "rm -f "SYS_LINEINFO_DIR"/http_* 1> /dev/null 2> /dev/null");
        system(cmd);
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "rm -f tmp/login_http_*  1> /dev/null 2> /dev/null");
        system(cmd);
  	}
  	if(https == DISABLED)
  	{
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "rm -f "SYS_LINEINFO_DIR"/https_* 1> /dev/null 2> /dev/null");
        system(cmd);
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "rm -f tmp/login_https_*  1> /dev/null 2> /dev/null");
        system(cmd);
  	}
}


void handleSaveOemtoFlash(void)
{
	FILE *fp = NULL;
	char buf[SYS_BUF256_LEN];
	char filename[SYS_BUF256_LEN];
	char cmd[SYS_BUF128_LEN];
	char logoFile[SYS_BUF128_LEN]={0};

	if (NULL == (fp = fopen(CONF_HTTP_OEM_CFG, "r")))
        return ;
	
	fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    osal_strcpy(filename, buf);

    fclose(fp);
    
    if (NULL == (fp = fopen(CONF_HTTP_LOGO_TMPFILE, "r")))
	{
        unlink(CONF_HTTP_OEM_CFG);
        return ;
    }
    fclose(fp);

    osal_sprintf(logoFile, "%s%s", CONF_LOGO_FLASH_DIR, filename);
    unlink(logoFile);
    
	//HANDLER_DBG("logo exist");
   	osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "cp %s %s%s> /dev/null", CONF_HTTP_LOGO_TMPFILE, CONF_LOGO_FLASH_DIR, filename);

	system(cmd);
	
	unlink(CONF_HTTP_OEM_CFG);
	unlink(CONF_HTTP_LOGO_TMPFILE);

	return ;
}

void handleSaveUsrCfgtoFlash(void)
{
	FILE *fp = NULL;
	char cmd[SYS_BUF128_LEN];
	char logoFile[SYS_BUF128_LEN]={0};

	if (NULL == (fp = fopen(CONF_HTTP_USRCFG_TMPFILE, "r")))
        return ;

    fclose(fp);

    osal_sprintf(logoFile, "%s", CONF_USRCFG_FLASH_FILE);
    unlink(logoFile);
    
	//HANDLER_DBG("logo exist");
   	osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "cp %s %s> /dev/null", CONF_HTTP_USRCFG_TMPFILE, CONF_USRCFG_FLASH_FILE);

	system(cmd);
	unlink(CONF_HTTP_USRCFG_TMPFILE);

	return ;
}

void handleSnmpTrapConfig(void)
{
	sal_snmp_target_t hostEntry;
	sal_snmp_target_t tmpHostEntry;
	FILE *fp = NULL;
	char buf[SYS_BUF256_LEN];
	int i = 0;
	
	if (NULL == (fp = fopen(SNMP_CONF_TRAP_FILE, "r")))
        return ;

    SYS_MEM_CLEAR(buf);
    SYS_MEM_CLEAR(hostEntry);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    osal_strcpy(hostEntry.server, buf);
    HANDLER_DBG("\r\n server=%s", hostEntry.server);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    osal_strcpy(hostEntry.name, buf);
    HANDLER_DBG("\r\n name=%s", hostEntry.name);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    hostEntry.secModel = SYS_STR2UINT(buf);
    HANDLER_DBG("\r\n secModel=%d", hostEntry.secModel);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    hostEntry.trapType = SYS_STR2UINT(buf);
    HANDLER_DBG("\r\n trapType=%d", hostEntry.trapType);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    hostEntry.secLevel = SYS_STR2UINT(buf);
    HANDLER_DBG("\r\n secLevel=%d", hostEntry.secLevel);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    hostEntry.portNum = SYS_STR2UINT(buf);
    HANDLER_DBG("\r\n portNum=%d", hostEntry.portNum);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    hostEntry.retries = SYS_STR2UINT(buf);
    HANDLER_DBG("\r\n retries=%d", hostEntry.retries);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    hostEntry.timeout = SYS_STR2UINT(buf);
    HANDLER_DBG("\r\n timeout=%d", hostEntry.timeout);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    if(osal_strcmp(buf, "add") == 0)
    {
		HANDLER_DBG("\r\n add entry");
		sal_snmp_target_add(&hostEntry);
    }
    else
    {
		for (i = 0; i < CAPA_SNMP_HOST_NUM; i++)
		{
			osal_memset(&tmpHostEntry, 0, sizeof(sal_snmp_target_t));
			sal_snmp_target_get(i, &tmpHostEntry);
			if (0 != osal_strlen(tmpHostEntry.server))
			{
				if (0 != osal_memcmp(hostEntry.server, tmpHostEntry.server, osal_strlen(hostEntry.server)))
					continue;
	
				sal_snmp_target_del(hostEntry.server, hostEntry.secModel, hostEntry.trapType);
				break;
			}
		}
		HANDLER_DBG("\r\n del entry");
    }
    
   	sal_snmp_reload_t reload;
    sal_snmp_config_reload_get(&reload);
    if(reload == SNMP_RELOAD_AUTO)
    {
       sal_snmp_restart();

       HANDLER_DBG("\r\n config trap success");
    }

	unlink(SNMP_CONF_TRAP_FILE);
	fclose(fp);

	return ;
}


void handleMibtftpUpload(void)
{
	int32 ret = SYS_ERR_FAILED;
	char localFile[CAPA_FILE_NAME_LEN];
    char remoteFile[CAPA_FILE_NAME_LEN];
    char ipstr[128] = "";
	FILE *fp = NULL;

    SYS_MEM_CLEAR(localFile);
    SYS_MEM_CLEAR(remoteFile);


	ret = sal_util_file_exist(CONF_TFTP_UPLOAD_CFG_FILE);
	if(ret == SYS_ERR_FILE_NOT_EXIST)
		return;

	if (NULL != (fp = fopen(CONF_TFTP_UPLOAD_CFG_FILE, "r")))
	{
	    fscanf(fp, "%s %% %s %% %s", ipstr, remoteFile, localFile);
	    fclose(fp);
	}
	
	ret = sal_util_file_exist(localFile);
	if(ret == SYS_ERR_FILE_NOT_EXIST)
	{
		if (0 == osal_strcmp(localFile, CONF_CLONE_FILE))
		{
			cmd_parser_common_running_clone_config_File_print(CAT_ID_END, CONF_CLONE_FILE);
		}
		else if(0 == osal_strcmp(localFile, CONF_RUNNING_FILE))
		{
			cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE);
		}
		else if(0 == osal_strcmp(localFile, CONF_STARTUP_FILE))
		{
			cmd_conf_startupConfig_save();
		}
	}

	fp = fopen(CONF_TFTP_UPLOAD_STATU_FILE, "w");
	ret = sal_tftp_config_upload(localFile, remoteFile, ipstr);
	if(ret != SYS_ERR_OK)
    {
    	fprintf(fp, "fail");
    	//SNMP_CMD_TRAP("#upload config *upload run-config/startup-config/clone-config fail");
    }
    else
    {
		fprintf(fp, "success");
		//SNMP_CMD_TRAP("#upload config *upload run-config/startup-config/clone-config success");
    }
    fclose(fp);

	return ;
}
void handleMibtftpDownload(void)
{
	int32 ret = SYS_ERR_FAILED;
	char localFile[CAPA_FILE_NAME_LEN];
    char remoteFile[CAPA_FILE_NAME_LEN];
    char ipstr[128] = "";
	FILE *fp = NULL;

    SYS_MEM_CLEAR(localFile);
    SYS_MEM_CLEAR(remoteFile);


	ret = sal_util_file_exist(CONF_TFTP_DOWNLOAD_CFG_FILE);
	if(ret == SYS_ERR_FILE_NOT_EXIST)
		return;

	if (NULL != (fp = fopen(CONF_TFTP_DOWNLOAD_CFG_FILE, "r")))
	{
	    fscanf(fp, "%s %% %s %% %s", ipstr, remoteFile, localFile);
	    fclose(fp);
	}
	
	ret = sal_util_file_exist(localFile);
	if(ret != SYS_ERR_FILE_NOT_EXIST)
	{
		if (0 == osal_strcmp(localFile, CONF_CLONE_TMPFILE))
		{
			unlink(CONF_CLONE_FILE);
		}
		else if(0 == osal_strcmp(localFile, CONF_RUNNING_TMPFILE))
		{
			unlink(CONF_RUNNING_FILE);
		}
		else if(0 == osal_strcmp(localFile, CONF_STARTUP_TMPFILE))
		{
			unlink(CONF_STARTUP_FILE);
		}
	}

	fp = fopen(CONF_TFTP_DOWNLOAD_STATU_FILE, "w");
	
	ret = sal_tftp_config_download(localFile, remoteFile, ipstr);
	if(ret != SYS_ERR_OK)
    {
    	fprintf(fp, "fail");
    }
    else
    {
		fprintf(fp, "success");
    }
    fclose(fp);

	if (0 == osal_strcmp(localFile, CONF_CLONE_TMPFILE))
	{
		if (IS_CONFIG_FILEHDR_INVALID(CONF_CLONE_TMPFILE))
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_CFG_INVALID);

            unlink(CONF_CLONE_TMPFILE);
            return ;
        }
        
        ret = cmd_configFile2Shm_copy(CONF_CLONE_TMPFILE);
        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL, CONF_CLONE_BASENAME);
            //SNMP_CMD_TRAP("#download config *download clone-config fail");
        }
        else
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS, CONF_CLONE_BASENAME);
            //SNMP_CMD_TRAP("#download config *download clone-config success");
        }
	}
	else if(0 == osal_strcmp(localFile, CONF_RUNNING_TMPFILE))
	{
		if (IS_CONFIG_FILEHDR_INVALID(CONF_RUNNING_TMPFILE))
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_CFG_INVALID);

            unlink(CONF_RUNNING_TMPFILE);
            return ;
        }
        
        ret = cmd_configFile2Shm_copy(CONF_RUNNING_TMPFILE);
        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL, CONF_RUNNING_BASENAME);
            //SNMP_CMD_TRAP("#download config *download run-config fail");
        }
        else
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS, CONF_RUNNING_BASENAME);
            //SNMP_CMD_TRAP("#download config *download run-config success");
        }
	}
	else if(0 == osal_strcmp(localFile, CONF_STARTUP_TMPFILE))
	{
		if (IS_CONFIG_FILEHDR_INVALID(CONF_STARTUP_TMPFILE))
        {
            SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_CFG_INVALID);

            return ;
        }


        cmd_conf_startupConfig_copy(CONF_STARTUP_TMPFILE);

        /* Remove invalid commands */
        config_valid_file_convert(CONF_STARTUP_FILE);

        SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS,CONF_STARTUP_BASENAME);
        //SNMP_CMD_TRAP("#download config *download startup-config success");
        
	}

	return ;
}


void handleMibStartupToRunconfig(void)
{
    int32 ret = SYS_ERR_FAILED;
    FILE *fp = NULL;

	fp = fopen(CONF_STARTUP_TO_RUN_TMPSTATUFILE, "w");
	cmd_configFile2Shm_copy(CONF_STARTUP_FILE);	
	if(ret != SYS_ERR_OK)
    {
    	fprintf(fp, "fail");
    	//SNMP_CMD_TRAP("#copy config *copy startup-config to run-config fail");
    }
    else
    {
		fprintf(fp, "success");
		//SNMP_CMD_TRAP("#copy config *copy startup-config to run-config success");
    }
    fclose(fp);
	return ;
}

void handleMibCloneToRunconfig(void)
{
    int32 ret = SYS_ERR_FAILED;
    FILE *fp = NULL;

	fp = fopen(CONF_ClONE_TO_RUN_TMPSTATUFILE, "w");
	cmd_configFile2Shm_copy(CONF_CLONE_FILE);	
	if(ret != SYS_ERR_OK)
    {
    	fprintf(fp, "fail");
    	//SNMP_CMD_TRAP("#copy config *copy clone-config to run-config fail");
    }
    else
    {
		fprintf(fp, "success");
		//SNMP_CMD_TRAP("#copy config *copy clone-config to run-config success");
    }
    fclose(fp);
	return ;
}

void handleMibSaveFlashConfig(void)
{    
    int32 ret = SYS_ERR_FAILED;
    FILE *fp = NULL;

    fp = fopen(CONF_STARTUP_TMPSTATUFILE, "w");
	ret = cmd_conf_startupConfig_save();	
	if(ret != SYS_ERR_OK)
    {
    	fprintf(fp, "fail");
    	//SNMP_CMD_TRAP("#copy config *copy run-config to startup-config fail");
    }
    else
    {
		fprintf(fp, "success");
		//SNMP_CMD_TRAP("#copy config *copy run-config to startup-config success");
    }
    fclose(fp);
    return ;
}

//生成run-config 配置文件
void handleBuildRunning(void)
{
    int32 ret = SYS_ERR_FAILED;
    FILE *fp = NULL;

    unlink(CONF_RUNNING_FILE);

    fp = fopen(CONF_RUNNING_TMPSTATUSFILE, "w");

    ret = cmd_parser_runningFile_print(CAT_ID_END, CONF_RUNNING_FILE);
    if(ret != SYS_ERR_OK)
    {
    	fprintf(fp, "fail");
    	//SNMP_CMD_TRAP("#build config *build run-config  fail");
    }
    else
    {
		fprintf(fp, "success");
		//SNMP_CMD_TRAP("#build config *build run-config  success");
    }
    fclose(fp);
    return ;
}

//生成clone-config 配置文件
void handleCloneRunning(void)
{
    int32 ret = SYS_ERR_FAILED;
    FILE *fp = NULL;

    unlink(CONF_CLONE_FILE);
    
	//ret = sal_util_file_exist(CONF_CLONE_TMPSTATUSFILE);
	//(ret == SYS_ERR_OK)
		//unlink(CONF_CLONE_TMPSTATUSFILE);

    fp = fopen(CONF_CLONE_TMPSTATUSFILE, "w");

    ret = cmd_parser_common_running_clone_config_File_print(CAT_ID_END, CONF_CLONE_FILE);
    if(ret != SYS_ERR_OK)
    {
    	fprintf(fp, "fail");
    	//SNMP_CMD_TRAP("#build config *build clone-config  fail");
    }
    else
    {
		fprintf(fp, "success");
		//SNMP_CMD_TRAP("#build config *build run-config  success");
    }
    fclose(fp);
    return ;
}

void handleReboot(void)
{
    sleep(1);
    sal_sys_reboot();
}

void handleUpgradeFw(void)
{
    int32 ret = SYS_ERR_FAILED;
    FILE *fp;
    uint32 partition = SYS_DFLT_FW_PARTITION;

#if 0 /* New boa http server with multi thread function do not have busy waiting loop issue */
    /* During image file downloading, the panel refresh request will keep sending.
       Boa web server will be blocked and enter busy waiting loop after receiving number of requests.
       This is temp solution to avoid the blocking situation.
       Here, we restart the Boa web server to eliminate those blocked requests. */
    sleep(1);
    system("killall -q -SIGTERM boa");
    sleep(1);
    system("boa -c /home &");
#endif

#ifdef CONFIG_SYS_DUAL_IMAGE
    sys_filenamestr_t filename;
    FILE *fp2;

    if ((fp = fopen("/tmp/http_write2partition", "r")) != NULL)
    {
        fscanf(fp, "%d", &partition);

        SYS_MEM_CLEAR(filename);

        if ((fp2 = fopen("/tmp/http_write2filename", "r")) != NULL)
        {
            fgets(filename.str, sizeof(filename),fp2);

            fclose(fp2);
            unlink("/tmp/http_write2filename");
        }

        sleep(1);
        ret = sal_sys_fwImage_write(TMP_FWIMG_FILENAME, partition, &filename);

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_FAIL,
                        partition,
                        "WEB",
                        "HTTP");
        }
        else
        {
            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_SUCCESS,
                        partition,
                        filename.str,
                        "WEB",
                        "HTTP");
        }

        fclose(fp);

        unlink("/tmp/http_write2partition");
    }
    else
#endif
    if ((fp = fopen("/tmp/tftp_write2partition", "r")) != NULL)
    {
        sys_filenamestr_t filename;
        char ipstr[128] = "";

        /* ip % filename % partition */
        SYS_MEM_CLEAR(filename);
        fscanf(fp, "%s %% %s %% %d", ipstr, filename.str, &partition);
        fclose(fp);

        ret = sal_sys_fwImage_write(TMP_FWIMG_FILENAME, partition, &filename);

        if (SYS_ERR_OK != ret)
        {
            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_FAIL,
                        partition,
                        "WEB",
                        "TFTP");
        }
        else
        {
            SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_SUCCESS,
                        partition,
                        filename.str,
                        "WEB",
                        "TFTP");
        }

        unlink("/tmp/tftp_write2partition");
    }
    else if((fp = fopen("/tmp/http_uploadfail", "r")) != NULL)
    {
        unlink("/tmp/http_uploadfail");
        fclose(fp);
        return;
    }
    else
    {
    	//http upgrade 
        sleep(10);
        sys_filenamestr_t filename;
        SYS_MEM_CLEAR(filename);
        ret = sal_sys_fwImage_write(TMP_FWIMG_FILENAME, 0, &filename);
    }

    if (NULL != (fp =  fopen(TMP_FWIMG_WRITESTATUS, "w")))
    {
        uint32 cnt;

        sleep(1);
        if(SYS_ERR_OK == ret)
        {
            fwrite(TMP_FWIMG_WRITESTATUS_OK, 1, sizeof(TMP_FWIMG_WRITESTATUS_OK), fp);
        }
        else if(SYS_ERR_STR_LEN_EXCEED == ret)  /*The length of file name is too long*/
        {
            fwrite(TMP_FWIMG_WRITESTATUS_FAILED_FILENAME, 1, sizeof(TMP_FWIMG_WRITESTATUS_FAILED_FILENAME), fp);
        }
        else    /*other error type*/
        {
            fwrite(TMP_FWIMG_WRITESTATUS_FAILED, 1, sizeof(TMP_FWIMG_WRITESTATUS_FAILED), fp);
        }
        fclose(fp);

        /* Check webui received write status or not, after timeout, delete status file automatically. */
        cnt = 0;

        while (cnt < 8)
        {
            sleep(1);
            cnt++;

            if (SYS_ERR_OK != sal_util_file_exist(TMP_FWIMG_WRITESTATUS))
                return;
        }

        //unlink(TMP_FWIMG_WRITESTATUS);
    }
}

void handleWebTftpAction(char *action)
{
    FILE *fp;
    int32 ret = SYS_ERR_OK;
    uint32 partition = SYS_DFLT_FW_PARTITION;
    char host[CAPA_HOSTNAME_LEN + 1];
    char remoteFile[CAPA_FILE_NAME_LEN];

    SYS_MEM_CLEAR(host);
    SYS_MEM_CLEAR(remoteFile);

    if (0 == osal_strcmp(action, "dl_fw"))
    {
        if ((fp = fopen("/tmp/tftp_write2partition", "r")) != NULL)
        {

            /* host % filename % partition */
            fscanf(fp, "%s %% %s %% %d", host, remoteFile, &partition);
            fclose(fp);

            ret = sal_tftp_fwImage_download(TMP_FWIMG_FILENAME, remoteFile, host);

            if (ret != SYS_ERR_OK)
            {
                if (NULL != (fp =  fopen("/tmp/tftp_uploadfail", "w")))
                {
                    if (SYS_ERR_IMAGE_MAGIC_INVALID == ret || SYS_ERR_IMAGE_INVALID == ret)
                        fprintf(fp, "Copy: Illegal software format");
                    else
                        fprintf(fp, "%s", SYS_ERR_STR(ret));
                    fclose(fp);
                }

                unlink("/tmp/tftp_write2partition");

                SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_FAIL,
                            partition,
                            "WEB",
                            "TFTP");

                return;
            }
            else
            {
                SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_UPGRADE_SUCCESS,
                            partition,
                            remoteFile,
                            "WEB",
                            "TFTP");
            }

            if (NULL != (fp = fopen("/tmp/tftp_uploadsize", "w")))
            {
                fprintf(fp, "%d", sal_util_fileSize_ret(TMP_FWIMG_FILENAME));
                fclose(fp);
            }

            handleUpgradeFw();
        }
    }
    else if (0 == osal_strcmp(action, "ul_fw"))
    {
        uint32 size = 0;
        uint32 tmp_time;
        uint32 magic;
        char name[SYS_BUF32_LEN];

        if ((fp = fopen("/tmp/tftp_ulinfo", "r")) != NULL)
        {
            /* host % filename % partition */
            fscanf(fp, "%s %% %s %% %d", host, remoteFile, &partition);
            fclose(fp);

            unlink("/tmp/tftp_ulinfo");

            ret = sal_tftp_fwImage_upload(partition, remoteFile, host);

            sal_sys_fwImageHdrInfo_get(partition, &magic, &tmp_time, &size, name);

            if (ret != SYS_ERR_OK)
            {
                if (NULL != (fp =  fopen("/tmp/tftp_backupfail", "w")))
                {
                    fprintf(fp, "%s", SYS_ERR_STR(ret));
                    fclose(fp);
                }

                SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_BACKUP_FAIL,
                                partition,
                                "WEB",
                                "TFTP");
            }
            else
            {
                if (NULL != (fp =  fopen("/tmp/tftp_backupDone", "w")))
                {
                    fprintf(fp, "%d", size);
                    fclose(fp);
                }

                SYS_LOG(LOG_CAT_IMAGEMGR, LOG_IMAGEMGR_BACKUP_SUCCESS,
                                partition,
                                "WEB",
                                "TFTP");
            }
        }
    }
}


void handleSaveRunning(void)
{
    int32 ret = SYS_ERR_FAILED;
    FILE *fp = NULL;

    unlink(CONF_HTTP_SAVERUNNING_STATUS);

    ret = cmd_configFile2Shm_copy(CONF_HTTP_SAVERUNNING_TMPFILE);

    unlink(CONF_HTTP_SAVERUNNING_TMPFILE);

    if (NULL == (fp = fopen(CONF_HTTP_SAVERUNNING_STATUS, "w")))
        return;

    if (SYS_ERR_OK != ret)
        SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_FAIL,
                    CONF_RUNNING_BASENAME);
    else
        SYS_LOG(LOG_CAT_CFGMGR, LOG_CFGMGR_UPGRADE_SUCCESS,
                    CONF_RUNNING_BASENAME);

    fprintf(fp, "%d", ret);
    fclose(fp);
}

void handleHttpdRestart(void)
{
    int32 pid = 0;
    FILE *fp = NULL;
    char cmd[SYS_BUF128_LEN];

    if (SYS_ERR_OK == sal_util_file_exist(HTTP_PID_FILE))
    {
        fp = fopen(HTTP_PID_FILE, "r");
        fscanf(fp, "%d", &pid);
        fclose(fp);

        unlink(HTTP_PID_FILE);

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "kill -9 %d"SYSCMD_NULL_MSG, pid);
        system(cmd);
    }
    if (SYS_ERR_OK == sal_util_file_exist(HTTPS_PID_FILE))
    {
        fp = fopen(HTTPS_PID_FILE, "r");
        fscanf(fp, "%d", &pid);
        fclose(fp);

        unlink(HTTPS_PID_FILE);

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "kill -9 %d"SYSCMD_NULL_MSG, pid);
        system(cmd);
    }

    /* Restart http/https */
    //system("killall -q -SIGURG polld");
    handleHttpd_restart();
}

void handleLockFwTimer(void)
{
#define LOCK_FW_MAX_TIME 600 //seconds

    int32 pid = -1;
    uint32 i = 0;
    FILE *fp = NULL;

    pid = getpid();

    /* Write current handler pid file */
    if (NULL == (fp = fopen(SYS_FWLOCK_TIMER_PID_FILE, "w")))
        return;

    fprintf(fp, "%d", pid);
    fclose(fp);

    for (i = 0; i < LOCK_FW_MAX_TIME; i += 3)
    {
        //SYS_PRINTF("%s:%d time=%d\n", __func__, __LINE__, i);
        /* Lock file is already be deleted. Kill this check process. */
        if (SYS_ERR_OK != sal_util_file_exist(FWIMG_UPGRAING_LOCK_FILE))
            return;

        sleep(3);
    }

    //SYS_PRINTF("%s:%d Delete lock automatically\n", __func__, __LINE__);

    unlink(SYS_FWLOCK_TIMER_PID_FILE);
    /* Lock file exist over maximum time. Delete it automatically. */
    unlink(FWIMG_UPGRAING_LOCK_FILE);

    handleHttpdRestart();
}

#ifdef CONFIG_SYS_ADVANCE_DNS
void handleAdvDns(uint8 ver)
{
    if (4 == ver)
        sal_sys_dnsDhcpConf_refresh();
    else if (6 == ver)
        sal_sys_dnsDhcpv6Conf_refresh();
}
#endif

void handleCliProgressPrint(char *pidFile)
{
#define CLI_PROCESS_PRINT_MAX_TIME 21600 //seconds
    uint32 i = 0;
    FILE *fp = NULL;

    if (NULL == (fp = fopen(pidFile, "w")))
        return;

    fprintf(fp, "%d", getpid());
    fclose(fp);

    for(i = 0; i < CLI_PROCESS_PRINT_MAX_TIME; i++)
    {
        fprintf(stderr, "!");
        sleep(1);
    }
}

#ifdef CONFIG_SYS_PING_TEST
/* new way */
void handleWebPing(char *ipVer, char *cnt, char *address)
{
    char cmd[SYS_BUF128_LEN];

    SYS_MEM_CLEAR(cmd);

    if ('6' == ipVer[0])
        osal_sprintf(cmd, "ping -6 -c %s %s 1> %s 2> %s", cnt, address, WEB_PING_RESULT_FILE, "/dev/null");
    else
        osal_sprintf(cmd, "ping -4 -c %s %s 1> %s 2> %s", cnt, address, WEB_PING_RESULT_FILE, "/dev/null");

    system(cmd);

    return;
}

/* old way */
void handlePing(char *count, char *size, char *interval, char *pingIp)
{
    uint32 ipaddr = 0;
    char cmd[128];
    FILE *fp = NULL;

    if(IS_IPV4_ADDRSTR_VALID(pingIp))
    {
        sys_util_str2Ip(pingIp, &ipaddr);

        osal_sprintf(cmd, "ping -c %s -s %s -i %s %s > %s", count, size, interval, pingIp, SYS_PING_TEST_TMP_FILE);
        system(cmd);
    }
    else
    {
        if(SYS_ERR_OK == sal_util_getHostByName(&ipaddr, pingIp))
        {
            osal_sprintf(cmd, "ping -c %s -s %s -i %s %s > %s", count, size, interval, pingIp, SYS_PING_TEST_TMP_FILE);
            system(cmd);
        }
        else
        {
            /* Write comment */
            fp = fopen(SYS_PING_TEST_TMP_FILE, "w");
            if ( NULL != fp )
            {
                fprintf(fp, "Domain name error!\n");
                fclose(fp);
            }
        }
    }

    unlink(SYS_PING_TEST_REFRESH_TMP_FILE);

}
#endif

#ifdef CONFIG_SYS_TRACE_ROUTE
void handleTraceroute(char *host, uint32 ttl)
{
    int32 fd;
    char cmd[SYS_BUF512_LEN];

    SYS_MEM_CLEAR(cmd);

    fd = open(SYS_TRACEROUTE_LOCK_FILE, O_RDWR | O_CREAT | O_TRUNC, 0640);

    if (fd < 0)
        return;

    if (lockf(fd, F_TLOCK, 0) < 0)
        goto exit;

    snprintf(cmd, sizeof(cmd), "%u", getpid());

    write(fd, cmd, strlen(cmd));

    SYS_MEM_CLEAR(cmd);

    unlink(SYS_TRACEROUTE_RESULT_FILE);
    snprintf(cmd, sizeof(cmd), "traceroute -I -m %u %s > %s 2>&1", ttl, host, SYS_TRACEROUTE_RESULT_FILE);

    system(cmd);

    /* wait for read */
    sleep(2);

exit:
    close(fd);
    unlink(SYS_TRACEROUTE_LOCK_FILE);
}
#endif

#ifdef CONFIG_SYS_TECH_SUPPORT
void handleTechSupportGen(void)
{
    FILE *fp;
    int32 ret = SYS_ERR_OK;

    if (NULL != (fp = fopen(SYS_POLLD_ACT_WEB_TECH_LOCK, "w")))
    {
        fprintf(fp, "LOCK");
        fclose(fp);
    }

    ret = sal_tech_file_gen("");

    unlink(SYS_POLLD_ACT_WEB_TECH_LOCK);

    if (ret != SYS_ERR_OK)
    {
        if (NULL != (fp = fopen(CONF_TECH_GEN_FAIL, "w")))
        {
            fprintf(fp, "%s", SYS_ERR_STR(ret));
            fclose(fp);
        }
    }
    else
    {
        if (NULL != (fp = fopen(CONF_TECH_GEN_DONE, "w")))
        {
            fprintf(fp, "%s", SYS_ERR_STR(ret));
            fclose(fp);
        }
    }

}
#endif

void handleInitPort(void)
{
    fds_port_default_init();

    sal_util_file_touch("/tmp/init_port_done");
}

