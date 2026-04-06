
#include <sys/un.h>
#include <sys/socket.h>
#include <stdarg.h>

#include <libsal/sal_sys.h>
#include <libsal/sal_util.h>
#include <libsal/sal_syslog.h>

const sys_text_t text_log_sev[] =
{
    {        "emerg"     },
    {        "alert"     },
    {         "crit"      },
    {          "error"     },
    {      "warning"   },
    {       "notice"    },
    {         "info"      },
    {        "debug"     },
};

const sys_text_t text_log_target[] =
{
    {   "console"      },
    {       "buffered"     },
    {     "file"         },
    {   "host"         },
    {   "host"         },
    {   "host"         },
    {   "host"         },
    {   "host"         },
    {   "host"         },
    {   "host"         },
    {   "host"         },
};

const sys_text_t text_log_target4conf[] =
{
    {   "local-console"   },
    {       "local-ram"       },
    {     "local-flash"     },
    {   "server1"         },
    {   "server2"         },
    {   "server3"         },
    {   "server4"         },
    {   "server5"         },
    {   "server6"         },
    {   "server7"         },
    {   "server8"         },
};

#ifdef CONFIG_SYS_SYSLOG

int32 sal_log_enable_set(sys_enable_t enable)
{
    sys_enable_t oldEnable = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_log_enable_get(&oldEnable));

    if (enable == oldEnable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(cfg_log_enable_set(enable));

    return SYS_ERR_OK;
}

int32 sal_log_enable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_log_enable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_log_target_set(log_target_t index, sal_log_target_t *pTarget)
{
    sal_log_target_t target;

    uint32 sev;
    char host[CAPA_HOSTNAME_LEN + 1];
    char ipaddr[CAPA_HOSTNAME_LEN + 1];

    SYS_PARAM_CHK(LOG_LOCAL > index || LOG_TARGET_MAX <= index, SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pTarget), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(LOG_SEV_ALL <= pTarget->severity, SYS_ERR_INPUT);

    osal_memset(ipaddr, 0 , sizeof(ipaddr));
    osal_memset(host, 0 , sizeof(host));

    sev = pTarget->severity;

    if (LOG_SEV_ALL <= sev)
        return SYS_ERR_INPUT;

    if (LOG_REMOTE <= index && strlen(pTarget->host))
    {
        
        SYS_ERR_CHK(sal_util_hostname_resolv(pTarget->host, ipaddr, sizeof(ipaddr)));

        osal_strncpy(host, pTarget->host, sizeof(host));

        if (osal_strcmp(host, ipaddr))
        {
            osal_strncpy(pTarget->host, ipaddr, sizeof(pTarget->host));

            SYS_ERR_CHK(sal_log_remoteIndexByIp_get(ipaddr, &index));

            if (LOG_TARGET_MAX <= index)
            {
                SYS_ERR_CHK(sal_log_remoteEmptyIndex_get(&index));

                if (LOG_TARGET_MAX <= index)
                    return SYS_ERR_LOG_SERVER_FULL;
            }
        }
    }

    SYS_ERR_CHK(cfg_log_target_get(index, &target));
	if(pTarget->totalCount == 0)
	{
		pTarget->totalCount = target.totalCount;
	}
    SYS_ERR_CHK(cfg_log_target_set(index, pTarget));

    if (index < LOG_LOCAL_MAX)
    {
        if (ENABLED == pTarget->enable)
        {
            if (target.enable != pTarget->enable || target.severity != pTarget->severity)
            {
                switch (index)
                {
                    case LOG_RAM:
                        SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_BUF_START, text_log_sev[pTarget->severity].text);
                        break;

                    case LOG_CONSOLE:
                        SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_CONSOLE_START, text_log_sev[pTarget->severity].text);
                        break;

                    case LOG_FLASH:
                        SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_FILE_START, text_log_sev[pTarget->severity].text);
                        break;

                    default:
                        break;
                }
            }
        }
        else
        {
            if (target.enable != pTarget->enable)
            {
                switch (index)
                {
                    case LOG_RAM:
                        SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_BUF_STOP);
                        break;

                    case LOG_CONSOLE:
                        SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_CONSOLE_STOP);
                        break;

                    case LOG_FLASH:
                        SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_FILE_STOP);
                        break;

                    default:
                        break;
                }
            }
        }
    }
    else if (LOG_SERVER1 <= index && LOG_SERVER1 + LOG_REMOTE_MAX > index)
    {
        if (0 < strlen(pTarget->host))
        {
            SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_HOST_ADD,
                    host, ipaddr, pTarget->port, text_log_sev[sev].text);
        }
        else
        {
            SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_HOST_DEL,
                    target.host);
        }
    }

    return SYS_ERR_OK;
}

int32 sal_log_target_get(log_target_t index, sal_log_target_t *pTarget)
{
    SYS_PARAM_CHK(LOG_LOCAL > index || LOG_TARGET_MAX <= index, SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pTarget), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_log_target_get(index, pTarget));

    return SYS_ERR_OK;
}

int32 sal_log_remoteIndexByIp_get(char *pIP, uint32 *pIndex)
{
    uint32 i = 0;
    sal_log_target_t target;
    char ipaddr[CAPA_HOSTNAME_LEN + 1];

    SYS_PARAM_CHK((NULL == pIP), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    osal_memset(ipaddr, 0 , sizeof(ipaddr));

    if (0 == strlen(pIP))
    {
        SYS_ERR_CHK(sal_log_remoteEmptyIndex_get(pIndex));

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_util_hostname_resolv(pIP, ipaddr, sizeof(ipaddr)));

    *pIndex = LOG_TARGET_MAX;

    for (i = LOG_REMOTE; i < LOG_TARGET_MAX ; i++)
    {
        SYS_ERR_CHK(cfg_log_target_get(i, &target));

        if(0 == osal_strcmp(ipaddr, target.host))
        {
            *pIndex = i;

            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_OK;
}

int32 sal_log_remoteEmptyIndex_get(uint32 *pIndex)
{
    uint32 i = 0;
    sal_log_target_t target;

    SYS_PARAM_CHK((NULL == pIndex), SYS_ERR_NULL_POINTER);

    *pIndex = LOG_TARGET_MAX;

    for (i = LOG_REMOTE; i < LOG_TARGET_MAX ; i++)
    {
        SYS_ERR_CHK(cfg_log_target_get(i, &target));

        if (0 == strlen(target.host))
        {
            *pIndex = i;

            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_OK;
}

int32 sal_log_file_clear(log_target_t index)
{
    switch (index)
    {
        case LOG_RAM:
            unlink(LOG_FILE_RAM);
            break;

        case LOG_FLASH:
            unlink(LOG_FILE_FLASH);
            unlink(LOG_FILE_FLASH_TMP);
            break;

        default:
            return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}

int32 sal_log_daemon_restart(uint8 isAuto)
{
    int32 running = FALSE;
    char cmd[SYS_BUF128_LEN];
    sys_enable_t enable = DISABLED;
    sys_enable_t enableAutoRestart = DISABLED;

    SYS_ERR_CHK(sal_log_enable_get(&enable));
    SYS_ERR_CHK(sal_log_autoRestart_get(&enableAutoRestart));

    running = (0 == access(SYS_LOG_DAEMON_PID_FILE, F_OK)) ? TRUE : FALSE;

    if (DISABLED == enable)
    {
        if (running)
        {
            unlink(SYS_LOG_DAEMON_PID_FILE);
            SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_STOP);
        }

        system("killall -q syslogd > /dev/null 2>&1");
        usleep(500 * 1000);

        return SYS_ERR_OK;
    }

    if ((!isAuto) || (enableAutoRestart && isAuto))
    {
#if 0
        if (IS_SYSTEM_READY())
            osal_sprintf(cmd, "syslogd > /dev/null 2>&1");
        else
            osal_sprintf(cmd, "syslogd -D %u > /dev/null 2>&1", LOG_REMOTE_DELAY);
#else
        osal_sprintf(cmd, "syslogd > /dev/null 2>&1");
#endif

        system("killall -q syslogd > /dev/null 2>&1");
        usleep(500 * 1000);
        system(cmd);
        usleep(500 * 1000);

        SYS_ERR_CHK(sys_log_restart_set());

        if (!isAuto && !running)
        {
            SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_START);
        }
    }

    return SYS_ERR_OK;
}

int32 sal_log_config_create(void)
{
    char buf[SYS_BUF1024_LEN];
    uint32 i = 0;
    uint32 j = 0;
    uint32 len = 0;
    uint32 interval = 0;
    sys_enable_t ratelimit = DISABLED;
    sal_log_target_t target;
    FILE *fp    = NULL;

    osal_memset(buf, 0, sizeof(buf));

    SYS_ERR_CHK(cfg_log_ratelimitEnable_get(&ratelimit));
    SYS_ERR_CHK(cfg_log_ratelimitInterval_get(&interval));

    fp = fopen(LOG_CONFIG_FILE, "w");

    if (NULL == fp)
        return SYS_ERR_FAILED;

    if (ENABLED == ratelimit)
    {
#if defined(CONFIG_SYS_KERNEL_LINUX_2_6_X)
        len = osal_sprintf(buf, "global ratelimit=%u\n", interval);
#else
        len = osal_sprintf(buf, "global ratelimit %u\n", interval);
#endif
        fwrite(buf, 1, len, fp);
    }

    for (i = 0; i < LOG_TARGET_MAX; i++)
    {
        SYS_MEM_CLEAR(target);
        SYS_ERR_CONTINUE(cfg_log_target_get(i, &target));

#if defined(CONFIG_SYS_KERNEL_LINUX_2_6_X)
        if (ENABLED == target.enable)
        {
            len = 0;

            len += osal_sprintf(buf + len, "%s ", text_log_target4conf[i].text);

            if (i >= LOG_REMOTE)
            {
                if (0 == osal_strlen(target.host))
                    continue;

                len += osal_sprintf(buf + len, "ip=%s ", target.host);
                len += osal_sprintf(buf + len, "port=%d ", target.port);
                len += osal_sprintf(buf + len, "facility=%u ", target.facility);
            }

            for (j = 0; j <= target.severity; j++)
                len += osal_sprintf(buf + len, "level=%s ", text_log_sev[j].text);

            len += osal_sprintf(buf + len, "\n");
            fwrite(buf, 1, len, fp);
        }
#else
        if (ENABLED == target.enable)
        {
            len = 0;

            len += osal_sprintf(buf + len, "%s", text_log_target4conf[i].text);

            if (0 != target.facility)
                len += osal_sprintf(buf + len, "=%u", target.facility);

            len += osal_sprintf(buf + len, " ");

            for (j = 0; j <= target.severity; j++)
                len += osal_sprintf(buf + len, "*.%s;", text_log_sev[j].text);

            len += osal_sprintf(buf + len, " ");

            switch (i)
            {
                case LOG_RAM:
                    len += osal_sprintf(buf + len, "%s", LOG_FILE_RAM);
                    break;
                case LOG_CONSOLE:
                    len += osal_sprintf(buf + len, "%s", LOG_FILE_CONSOLE);
                    break;
                case LOG_FLASH:
                    len += osal_sprintf(buf + len, "%s", LOG_FILE_FLASH_TMP);
                    break;
                case LOG_SERVER1 ... LOG_SERVER8:
                    if (0 != osal_strlen(target.host))
                       len += osal_sprintf(buf + len, "%s", target.host);

                    if (0 != target.port)
                        len += osal_sprintf(buf + len, ":%d", target.port);
                    break;
                default:
                    break;
            }

            len += osal_sprintf(buf + len, "\n");
            fwrite(buf, 1, len, fp);
            fflush(fp);
        }
#endif
    }

    fclose(fp);

    return SYS_ERR_OK;
}

int32 sal_log_autoRestart_set(sys_enable_t autoRestart)
{
    SYS_ERR_CHK(cfg_log_autoRestart_set(autoRestart));

    return SYS_ERR_OK;
}

int32 sal_log_autoRestart_get(sys_enable_t *autoRestart)
{
    SYS_PARAM_CHK((NULL == autoRestart), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_log_autoRestart_get(autoRestart));

    return SYS_ERR_OK;
}

int32 sal_log_ratelimitEnable_set(sys_enable_t enable)
{
    sys_enable_t oldEnable = DISABLED;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_log_ratelimitEnable_get(&oldEnable));

    if(enable == oldEnable)
        return SYS_ERR_OK;

    SYS_ERR_CHK(cfg_log_ratelimitEnable_set(enable));

    if (ENABLED == enable)
        SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_AGG_START);
    else
        SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_AGG_STOP);

    return SYS_ERR_OK;
}

int32 sal_log_ratelimitEnable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(NULL == pEnable, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_log_ratelimitEnable_get(pEnable));

    return SYS_ERR_OK;
}

int32 sal_log_ratelimitInterval_set(uint32 interval)
{
    SYS_ERR_CHK(cfg_log_ratelimitInterval_set(interval));

    SYS_LOG(LOG_CAT_LOGGING, LOG_LOGGING_AGG_AGETIME, interval);

    return SYS_ERR_OK;
}

int32 sal_log_ratelimitInterval_get(uint32 *pInterval)
{
    SYS_PARAM_CHK(NULL == pInterval, SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_log_ratelimitInterval_get(pInterval));

    return SYS_ERR_OK;
}
#endif
