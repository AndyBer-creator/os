
#include <dirent.h>
#include <mtd/mtd-user.h>
#include <libsal/sal_util.h>
#include <libsal/sal_sys.h>

#define SSHD    "ssh stream tcp nowait root /bin/sshd -i\n"
#define TELD    "telnet stream tcp nowait root /bin/telnetd\n"
#define SSHDV6  "ssh stream tcp6 nowait root /bin/sshd -i\n"
#define TELDV6  "telnet stream tcp6 nowait root /bin/telnetd\n"

const sys_text_t text_access_type[] =
{
    {  "console" },
    {   "telnet"},
#ifdef CONFIG_USER_SSH_SSHD
    {      "ssh"},
#endif
    {     "http"},
    {    "https"},
};

const sys_text_t text_line_cli[] =
{
    {  "console" },
    {   "telnet"},
#ifdef CONFIG_USER_SSH_SSHD
    {      "ssh"},
#endif
};

const sys_text_t text_line_web[] =
{
    {     "http"},
    {    "https"},
};

int32 sal_sys_get_current_web_session_info(sys_line_session_t *pSession)
{
    FILE *fp = NULL;
    char buf[SYS_BUF256_LEN];
    char *remoteIp = getenv("REMOTE_ADDR");
    char *accessTypeEnv = getenv("ACCESS_TYPE");
    uint32 now_sec = 0;
    sys_access_type_t accessType;
    char *userAgent = getenv("HTTP_USER_AGENT");
    int cuSerAgent = 0;
    SYS_MEM_CLEAR(buf);

    osal_time_sysUptime_get(&now_sec);

    if (accessTypeEnv != NULL && 0 == osal_strcmp(accessTypeEnv, "https"))
    {
        accessType = SYS_ACCESS_WEB_HTTPS;
    }
    else
    {
        accessType = SYS_ACCESS_WEB_HTTP;
 
    }

    if (0 == osal_strncmp(remoteIp, "::ffff:", 7))
        remoteIp = remoteIp + 7;

   if (NULL != userAgent)
    {
        if (NULL != osal_strstr(userAgent, "Chrome"))
            cuSerAgent = 1; 
        else if (NULL != osal_strstr(userAgent, "Firefox"))
            cuSerAgent = 2;
        else if (NULL != osal_strstr(userAgent, "Safari"))
            cuSerAgent = 4;
        else if (NULL != osal_strstr(userAgent, "MSIE") || NULL != osal_strstr(userAgent, "Trident"))
            cuSerAgent = 3;
        else
            cuSerAgent = 5;
    }

    osal_sprintf(buf, "%s/%s_%s_%d", SYS_LINEINFO_DIR,(accessType == SYS_ACCESS_WEB_HTTP) ? "http":"https", remoteIp, cuSerAgent);
    if (NULL == (fp = fopen(buf, "r")))
    {
    	return SYS_ERR_ENTRY_NOT_FOUND;
    }

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    pSession->pid = SYS_STR2UINT(buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    pSession->time = SYS_STR2UINT(buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[osal_strlen(buf) - 1] = '\0';
    osal_strcpy(pSession->user, buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    osal_strcpy(pSession->host, buf);

    fclose(fp);
    return SYS_ERR_OK;
    
}

int32 sal_sys_lineSession_by_pid_get_new(int32 pid, sys_line_session_t *pSession)
{
    char *c = NULL;
    char buf[SYS_BUF256_LEN];
    DIR *dir = NULL;
    struct dirent *entry;
    FILE *fp = NULL;
    char filenanme[SYS_BUF16_LEN];

    SYS_PARAM_CHK((NULL == pSession), SYS_ERR_NULL_POINTER);

	if(sal_util_processExist_ret(pid))
	{
	    if ((dir = opendir(SYS_LINEINFO_DIR)) == NULL)
	        return SYS_ERR_OK;

	    while ((entry = readdir(dir)) != NULL)
	    {
	        if (entry->d_name[0] == '.')
	        {
	            if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
	                continue;
	        }

	        SYS_MEM_CLEAR(filenanme);

	        c = entry->d_name;
	        while (*c != '_' && osal_strlen(c) > 0)
	        {
	            c++;
	        }
	           
			if(SYS_STR2UINT(c + 1) !=  pid)
			{
				continue;	
			}

			osal_sprintf(&filenanme, "%s", entry->d_name);
			closedir(dir);
			goto OK;
			break;
		}
		closedir(dir);

		return SYS_ERR_OK;
	}
	else
		return SYS_ERR_ENTRY_NOT_FOUND;
	
OK:    
	SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%s/%s", SYS_LINEINFO_DIR, filenanme);
    if (NULL == (fp = fopen(buf, "r")))
    {
    	closedir(dir);
    	return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    pSession->pid = SYS_STR2UINT(buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    pSession->time = SYS_STR2UINT(buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[osal_strlen(buf) - 1] = '\0';
    osal_strcpy(pSession->user, buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    osal_strcpy(pSession->host, buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    osal_strcpy(pSession->localIp, buf);

    fclose(fp);
    return SYS_ERR_OK;            
  
}

int32 sal_sys_lineSession_by_pid_get(int32 pid, sys_line_session_t *pSession, char *typeStr)
{
    char *c = NULL;
    char buf[SYS_BUF256_LEN];
    DIR *dir = NULL;
    struct dirent *entry;
    FILE *fp = NULL;
    char typeName[SYS_BUF16_LEN];
    char filenanme[SYS_BUF16_LEN];

    SYS_PARAM_CHK((NULL == pSession), SYS_ERR_NULL_POINTER);

	if(sal_util_processExist_ret(pid) == FALSE)
	{

		if ((dir = opendir(SYS_LINEINFO_DIR)) == NULL)
	        return SYS_ERR_OK;
	        
		while ((entry = readdir(dir)) != NULL)
	    {
	        if (entry->d_name[0] == '.')
	        {
	            if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
	                continue;
	        }

	        SYS_MEM_CLEAR(typeName);
			SYS_MEM_CLEAR(filenanme);

	        c = entry->d_name;
	        while (*c != '_' && osal_strlen(c) > 0)
	        {
	            c++;
	        }

			osal_strncpy(typeName, entry->d_name, c - entry->d_name);
			if(osal_strcmp(typeName, "http") == 0)
			{
				osal_sprintf(typeStr, "web");
				osal_sprintf(&filenanme, "%s", entry->d_name);
				
				closedir(dir);
				goto OK;
				break;	
			}
		}
		closedir(dir);
		return SYS_ERR_OK;
	}
	else
	{
	    if ((dir = opendir(SYS_LINEINFO_DIR)) == NULL)
	        return SYS_ERR_OK;

	    while ((entry = readdir(dir)) != NULL)
	    {
	        if (entry->d_name[0] == '.')
	        {
	            if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
	                continue;
	        }

	        SYS_MEM_CLEAR(filenanme);

	        c = entry->d_name;
	        while (*c != '_' && osal_strlen(c) > 0)
	        {
	            c++;
	        }
	           
			if(SYS_STR2UINT(c + 1) !=  pid)
			{
				continue;	
			}

			osal_strncpy(typeStr, entry->d_name, c - entry->d_name);
			osal_sprintf(&filenanme, "%s", entry->d_name);
			closedir(dir);
			goto OK;
			break;
		}
		closedir(dir);
		osal_sprintf(typeStr, "snmp");
		return SYS_ERR_OK;
	}
	
OK:    
	SYS_MEM_CLEAR(buf);
    osal_sprintf(buf, "%s/%s", SYS_LINEINFO_DIR, filenanme);
    if (NULL == (fp = fopen(buf, "r")))
    {
    	closedir(dir);
    	return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    pSession->pid = SYS_STR2UINT(buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    pSession->time = SYS_STR2UINT(buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[osal_strlen(buf) - 1] = '\0';
    osal_strcpy(pSession->user, buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    buf[strlen(buf) - 1] = '\0';
    osal_strcpy(pSession->host, buf);

    SYS_MEM_CLEAR(buf);
    fgets(buf, sizeof(buf), fp);
    osal_strcpy(pSession->localIp, buf);

    fclose(fp);
    return SYS_ERR_OK;            
  
}

int32 sal_sys_lineSession_set(sys_access_type_t type, sys_line_session_t session)
{
    char buf[SYS_BUF256_LEN];
    uint32 currUptime = 0;
    FILE *fp = NULL;

    sal_sys_lineSessionInfo_clear();

    osal_time_sysUptime_get(&currUptime);

    SYS_MEM_CLEAR(buf);

    if ((SYS_ACCESS_WEB_HTTP == type) || (SYS_ACCESS_WEB_HTTPS == type))
        osal_sprintf(buf, "%s/%s_%s_%d", SYS_LINEINFO_DIR, text_access_type[type].text, session.host, session.pid);
    else
        osal_sprintf(buf, "%s/%s_%d", SYS_LINEINFO_DIR, text_access_type[type].text, session.pid);

    if (NULL == (fp = fopen(buf, "w")))
        return SYS_ERR_FILE_OPEN;

    fprintf(fp, "%d\n%u\n%s\n%s\n%s", session.pid,
                                      currUptime,
                                      (0 == osal_strlen(session.user)) ? "(empty)" : session.user,
                                      session.host,
                                      session.localIp);

    fclose(fp);

    return SYS_ERR_OK;
}

int32 sal_sys_lineSession_get(sys_access_type_t type, uint32 idx, sys_line_session_t *pSession)
{
    char *c = NULL;
    char typeStr[SYS_BUF8_LEN];
    char buf[SYS_BUF256_LEN];
    uint32 currIdx = 0;
    DIR *dir = NULL;
    struct dirent *entry;
    FILE *fp = NULL;

    SYS_PARAM_CHK((NULL == pSession), SYS_ERR_NULL_POINTER);

    if ((dir = opendir(SYS_LINEINFO_DIR)) == NULL)
        return SYS_ERR_OK;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
                continue;
        }

        SYS_MEM_CLEAR(typeStr);

        c = entry->d_name;
        while (*c != '_' && osal_strlen(c) > 0)
        {
            c++;
        }
        osal_strncpy(typeStr, entry->d_name, c - entry->d_name);

        if ((0 == osal_strcmp(typeStr, text_access_type[type].text)))
        {
            if (idx == currIdx)
            {
                SYS_MEM_CLEAR(buf);
                osal_sprintf(buf, "%s/%s", SYS_LINEINFO_DIR, entry->d_name);
                if (NULL == (fp = fopen(buf, "r")))
                    continue;

                SYS_MEM_CLEAR(buf);
                fgets(buf, sizeof(buf), fp);
                buf[strlen(buf) - 1] = '\0';
                pSession->pid = SYS_STR2UINT(buf);

                SYS_MEM_CLEAR(buf);
                fgets(buf, sizeof(buf), fp);
                buf[strlen(buf) - 1] = '\0';
                pSession->time = SYS_STR2UINT(buf);

                SYS_MEM_CLEAR(buf);
                fgets(buf, sizeof(buf), fp);
                buf[osal_strlen(buf) - 1] = '\0';
                osal_strcpy(pSession->user, buf);

                SYS_MEM_CLEAR(buf);
                fgets(buf, sizeof(buf), fp);
                buf[strlen(buf) - 1] = '\0';
                osal_strcpy(pSession->host, buf);

                SYS_MEM_CLEAR(buf);
                fgets(buf, sizeof(buf), fp);
                osal_strcpy(pSession->localIp, buf);

                fclose(fp);

                closedir(dir);

                return SYS_ERR_OK;
            }
            else
            {
                currIdx++;
            }
        }
    }
    closedir(dir);

    return SYS_ERR_OK;
}

uint32 sal_sys_lineSessionCnt_ret(sys_access_type_t type)
{
    char *c = NULL;
    char typeStr[SYS_BUF8_LEN];
    uint32 cnt = 0;
    DIR *dir = NULL;
    struct dirent *entry;

    if ((dir = opendir(SYS_LINEINFO_DIR)) == NULL)
        return 0;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
                continue;
        }

        SYS_MEM_CLEAR(typeStr);

        c = entry->d_name;
        while (*c != '_' && osal_strlen(c) > 0)
        {
            c++;
        }
        osal_strncpy(typeStr, entry->d_name, c - entry->d_name);

        if (0 == osal_strcmp(typeStr, text_access_type[type].text))
        {
            cnt++;
        }
    }
    closedir(dir);

    return cnt;
}

int32 sal_sys_lineSessionInfo_clear(void)
{
    char *c = NULL;
    char buf[SYS_BUF256_LEN];
    char typeStr[SYS_BUF8_LEN];
    char infoStr[SYS_BUF64_LEN];
    int32 ret = SYS_ERR_FAILED;
    uint32 currUptime = 0;
    DIR *dir = NULL;
    struct dirent *entry;
#ifdef CONFIG_SYS_UI_WEB
    uint32 webTimeout = 0;
    FILE *fp = NULL;
    sys_line_session_t oldSession;
#endif

    osal_time_sysUptime_get(&currUptime);

    if ((dir = opendir(SYS_LINEINFO_DIR)) == NULL)
    {
        SYS_MEM_CLEAR(buf);

        osal_sprintf(buf, "mkdir %s\n", SYS_LINEINFO_DIR);

        system(buf);
    }
    
    else
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] == '.')
            {
                if((entry->d_name[1] == 0 || (entry->d_name[1] == '.' && entry->d_name[2] == 0)))
                    continue;
            }

            SYS_MEM_CLEAR(typeStr);
            SYS_MEM_CLEAR(infoStr);

            c = entry->d_name;
            while (*c != '_' && osal_strlen(c) > 0)
            {
                c++;
            }
            osal_strncpy(typeStr, entry->d_name, c - entry->d_name);
            osal_strcpy(infoStr, (c + 1));

#ifdef CONFIG_SYS_UI_WEB
            if ((0 == osal_strcmp(typeStr, text_access_type[SYS_ACCESS_WEB_HTTP].text))
                || (0 == osal_strcmp(typeStr, text_access_type[SYS_ACCESS_WEB_HTTPS].text)))
            {
                SYS_MEM_CLEAR(buf);
                osal_sprintf(buf, "%s/%s", SYS_LINEINFO_DIR, entry->d_name);
                if (NULL == (fp = fopen(buf, "r")))
                    continue;

                SYS_MEM_CLEAR(oldSession);

                fscanf(fp, "%d\n%u\n%*s\n%*s\n", &oldSession.pid, &oldSession.time);
                fclose(fp);

                if (0 == osal_strcmp(typeStr, text_access_type[SYS_ACCESS_WEB_HTTP].text))
                    sal_sys_webSessionTimeout_get(SYS_LINE_WEB_HTTP, &webTimeout);
                else
                    sal_sys_webSessionTimeout_get(SYS_LINE_WEB_HTTPS, &webTimeout);

                webTimeout *= (60 * 1000);

                if (webTimeout != 0)
                {
                    if((currUptime - oldSession.time) > webTimeout)
                    {
                        SYS_MEM_CLEAR(buf);
                        osal_sprintf(buf, "%s/%s", SYS_LINEINFO_DIR, entry->d_name);
                        unlink(buf);
                    }
                }
            }
            else
#endif
            {
                SYS_MEM_CLEAR(buf);
                osal_sprintf(buf, "/proc/%s", infoStr);

                ret = sal_util_file_exist(buf);
                if (ret != SYS_ERR_OK)
                {
                    SYS_MEM_CLEAR(buf);
                    osal_sprintf(buf, "%s/%s", SYS_LINEINFO_DIR, entry->d_name);
                    unlink(buf);
                }
            }
        }
        closedir(dir);
    }

    return SYS_ERR_OK;
}

int32 sal_sys_cliHistory_count_set(sys_line_cliType_t type, uint32 count)
{
    sys_line_cli_t line;

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(count, 0, BOND_CLI_HISTORY_ENTRY_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineCli_get(type, &line));

    line.historyCnt = count;

    SYS_ERR_CHK(cfg_sys_lineCli_set(type, &line));

    return SYS_ERR_OK;
}

int32 sal_sys_cliHistory_count_get(sys_line_cliType_t type, uint32 *pCount)
{
    sys_line_cli_t line;

    SYS_PARAM_CHK((NULL == pCount), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineCli_get(type, &line));

    *pCount = line.historyCnt;

    return SYS_ERR_OK;
}

int32 sal_sys_cliHistoryState_set(sys_line_cliType_t type, sys_enable_t enable)
{
    sys_line_cli_t line;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineCli_get(type, &line));

    line.historyState = enable;

    SYS_ERR_CHK(cfg_sys_lineCli_set(type, &line));

    return SYS_ERR_OK;
}

int32 sal_sys_cliHistoryState_get(sys_line_cliType_t type, sys_enable_t *pEnable)
{
    sys_line_cli_t line;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineCli_get(type, &line));

    *pEnable = line.historyState;

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_APP_TELNETD

int32 sal_sys_telnetdEnable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_telnetdEnable_set(enable));

    if (ENABLED == enable)
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TELNET_START);
    }
    else
    {
        SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_TELNET_STOP);

        system("handler -c telnet &");
    }

    SYS_ERR_CHK(sal_sys_inetdConfig_set());

    return SYS_ERR_OK;
}

int32 sal_sys_telnetd_port_set(uint32 port)
{
	uint32        oldport = 0;
	FILE  *fp = NULL;
	char buf[SYS_BUF256_LEN];
	char telnetstr[SYS_BUF64_LEN] = {0};
	char serverstr[SYS_BUF64_LEN] = {0};
	
	SYS_ERR_CHK(cfg_sys_telnetd_port_get(&oldport));

	if(port == oldport)
		return SYS_ERR_OK;
		
	if(port != DEFAULT_TELNET_PORT_VAL) 
	{
		if (NULL == (fp = fopen(ETC_SERVERS_FILE, "r")))
			return SYS_ERR_OK;
		osal_sprintf(telnetstr, "telnet%u", port);	
		SYS_MEM_CLEAR(buf);
		while (fgets(buf, sizeof(buf), fp))
		{

			if(osal_strstr(buf, telnetstr))
			{
				fclose(fp);
				SYS_ERR_CHK(cfg_sys_telnetd_port_set(port));
				return SYS_ERR_OK;
			}
			SYS_MEM_CLEAR(buf);
		}
		fclose(fp);

		if(NULL == (fp = fopen(ETC_SERVERS_FILE, "a+")))
			return SYS_ERR_FAILED;

		osal_sprintf(serverstr, "telnet%u %u/tcp\n", port, port);
		fputs(serverstr, fp);
		osal_sprintf(serverstr, "telnet%u %u/udp\n", port, port);
		fputs(serverstr, fp);
		fclose(fp);
	}

    SYS_ERR_CHK(cfg_sys_telnetd_port_set(port));
    return SYS_ERR_OK;
}

int32 sal_sys_telnetdEnable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_telnetdEnable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_sys_telnetd_port_get(uint32 *port)
{
    SYS_PARAM_CHK((NULL == port), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_telnetd_port_get(port));

    return SYS_ERR_OK;
}

#endif

#ifdef CONFIG_SYS_APP_HTTPD

int32 sal_sys_httpEnable_set(sys_enable_t enable)
{
    sys_enable_t ori_enable;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_httpEnable_get(&ori_enable));

    if(ori_enable != enable)
    {
        SYS_ERR_CHK(cfg_sys_httpEnable_set(enable));

		system("handler -w httpServer 1> /dev/null 2> /dev/null &");
		
        if (ENABLED == enable)
            SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_HTTP_START);
        else
            SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_HTTP_STOP);
    }

    return SYS_ERR_OK;
}

int32 sal_sys_httpEnable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_httpEnable_get(pEnable));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_USER_BOA_WITH_SSL

int32 sal_sys_httpsEnable_set(sys_enable_t enable)
{
    sys_enable_t ori_enable;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_httpsEnable_get(&ori_enable));

    if(ori_enable != enable)
    {
        SYS_ERR_CHK(cfg_sys_httpsEnable_set(enable));

        system("handler -w httpServer 1> /dev/null 2> /dev/null &");

        if (ENABLED == enable)
            SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_HTTPS_START);
        else
            SYS_LOG(LOG_CAT_SYSTEM, LOG_SYSTEM_HTTPS_STOP);
    }

    return SYS_ERR_OK;
}

int32 sal_sys_httpsEnable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_httpsEnable_get(pEnable));

    return SYS_ERR_OK;
}
#endif

int32 sal_sys_inetdConfig_set(void)
{
    int32 ret = SYS_ERR_OK;
    FILE *fp = NULL;
#if defined(CONFIG_USER_SSH_SSHD) || (CONFIG_SYS_APP_TELNETD)
    sys_enable_t enable = DISABLED;
#endif
	uint32 port;
	char buf[64];

    if (NULL == (fp = fopen(ETC_INETD_CONFIG_FILE, "w")))
        return SYS_ERR_FAILED;

#ifdef CONFIG_USER_SSH_SSHD
    
    SYS_ERR_HDL(sal_ssh_sshdEnable_get(&enable), _exit, ret);

    if (ENABLED == enable)
    {
    	SYS_ERR_HDL(sal_sys_sshd_port_get(&port), _exit, ret);
    	if(port == DEFAULT_SSH_PORT_VAL)
    	{
#ifdef CONFIG_SYS_IPV6
        	fwrite(SSHDV6, 1, osal_strlen(SSHDV6), fp);
#else
        	fwrite(SSHD, 1, osal_strlen(SSHD), fp);
#endif
		}
		else 
		{
			SYS_MEM_CLEAR(buf);
#ifdef CONFIG_SYS_IPV6
			osal_sprintf(buf, "ssh%u stream tcp6 nowait root /bin/sshd -i\n", port);
        	fwrite(buf, 1, osal_strlen(buf), fp);
#else
			osal_sprintf(buf, "ssh%u stream tcp nowait root /bin/sshd -i\n", port);
        	fwrite(buf, 1, osal_strlen(buf), fp);
#endif			
		}
    }
#endif 

#ifdef CONFIG_SYS_APP_TELNETD
    SYS_ERR_HDL(sal_sys_telnetdEnable_get(&enable), _exit, ret);

    if (ENABLED == enable)
    {
    	SYS_ERR_HDL(sal_sys_telnetd_port_get(&port), _exit, ret);
    	if(port == DEFAULT_TELNET_PORT_VAL)
    	{
    	
#ifdef CONFIG_SYS_IPV6
        	fwrite(TELDV6, 1, osal_strlen(TELDV6), fp);
#else
        	fwrite(TELD, 1, osal_strlen(TELD), fp);
#endif
		}
		else 
		{
			SYS_MEM_CLEAR(buf);
#ifdef CONFIG_SYS_IPV6
			osal_sprintf(buf, "telnet%u stream tcp6 nowait root /bin/telnetd\n", port);
        	fwrite(buf, 1, osal_strlen(buf), fp);
#else
			osal_sprintf(buf, "telnet%u stream tcp nowait root /bin/telnetd\n", port);
        	fwrite(buf, 1, osal_strlen(buf), fp);
#endif			
		}
    }
#endif 

_exit:
    fclose(fp);

    return SYS_ERR_OK;
}

int32 sal_inetd_restart(void)
{

    system("killall -9 inetd 1> /dev/null 2> /dev/null");
    system("inetd 1> /dev/null 2> /dev/null &");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT

int32 sal_sys_cliSessionTimeout_set(sys_line_cliType_t type, uint32 timeout)
{
    sys_line_cli_t line;

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineCli_get(type, &line));

    line.sessionTime = timeout;

    SYS_ERR_CHK(cfg_sys_lineCli_set(type, &line));

    return SYS_ERR_OK;
}

int32 sal_sys_cliSessionTimeout_get(sys_line_cliType_t type, uint32 *pTimeout)
{
    sys_line_cli_t line;

    SYS_PARAM_CHK((NULL == pTimeout), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineCli_get(type, &line));

    *pTimeout = line.sessionTime;

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_UI_WEB_SESSION_TIMEOUT

int32 sal_sys_webSessionTimeout_set(sys_line_webType_t type, uint32 timeout)
{
    sys_line_web_t line;

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineWeb_get(type, &line));

    line.sessionTime = timeout;

    SYS_ERR_CHK(cfg_sys_lineWeb_set(type, &line));

    return SYS_ERR_OK;
}

int32 sal_sys_webSessionTimeout_get(sys_line_webType_t type, uint32 *pTimeout)
{
    sys_line_web_t line;

    SYS_PARAM_CHK((NULL == pTimeout), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineWeb_get(type, &line));

    *pTimeout = line.sessionTime;

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT

int32 sal_sys_cliRetrySilent_set(sys_line_cliType_t type, sys_cliRetrySilent_t retrySilent)
{
    sys_line_cli_t line;

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineCli_get(type, &line));

    line.retrySilent = retrySilent;

    SYS_ERR_CHK(cfg_sys_lineCli_set(type, &line));

    return SYS_ERR_OK;
}

int32 sal_sys_cliRetrySilent_get(sys_line_cliType_t type, sys_cliRetrySilent_t *pRetrySilent)
{
    sys_line_cli_t line;

    SYS_PARAM_CHK((NULL == pRetrySilent), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(line);

    SYS_ERR_CHK(cfg_sys_lineCli_get(type, &line));

    *pRetrySilent = line.retrySilent;

    return SYS_ERR_OK;
}
#endif 

