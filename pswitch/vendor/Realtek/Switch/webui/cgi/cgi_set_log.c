
#include <include/cgi.h>

static int32 cgi_log_clear_set(void);
static int32 cgi_log_global_set(void);
static int32 cgi_log_remoteAdd_set(void);
static int32 cgi_log_remoteEdit_set(void);
static int32 cgi_log_remoteDel_set(void);

const cgi_cmdmap_t cmdSetLogTbl[] =
{
    { CGI_CMD_LOG_CLEAR,        cgi_log_clear_set       },
    { CGI_CMD_LOG_GLOBAL,       cgi_log_global_set      },
    { CGI_CMD_LOG_REMOTE_ADD,   cgi_log_remoteAdd_set   },
    { CGI_CMD_LOG_REMOTE_EDIT,  cgi_log_remoteEdit_set  },
    { CGI_CMD_LOG_REMOTE_DEL,   cgi_log_remoteDel_set   },
    { NULL,                     NULL                    }
};

static int32 cgi_log_clear_set(void)
{
    int32 retval = SYS_ERR_OK;
    char *pStrValue = NULL;

    log_target_t target = LOG_RAM;

    if (CGI_IS_PARAM_EXIST(CGI_LOG_TARGET, pStrValue))
       target = SYS_STR2UINT(pStrValue) % 2 ? LOG_FLASH : LOG_RAM;

    CGI_SET_ERR_HDL(sal_log_file_clear(target), retval);
    CGI_SET_ERR_HDL(sal_log_daemon_restart(FALSE), retval);

cgi_end:
    cgi_set_respGeneral_send(retval);

    return SYS_ERR_OK;
}

static int32 cgi_log_global_set(void)
{
    int32 retval = SYS_ERR_OK;

    char *pStrValue;

    uint32 sev;
    uint32 interval;

    log_target_t t;
    sal_log_target_t target;
    sys_enable_t enable;
    sys_enable_t ratelimit;

    t = LOG_CONSOLE;
    osal_memset(&target, 0, sizeof(target));

    if (CGI_IS_PARAM_EXIST(CGI_LOG_CONSOLE_STATE, pStrValue))
    {
        
        sev = LOG_SEV_ALL;

        if (CGI_IS_PARAM_EXIST(CGI_LOG_CONSOLE_SEV, pStrValue))
            sev = SYS_STR2UINT(pStrValue);

        target.severity = (sev < LOG_SEV_ALL) ? sev : DFLT_LOG_CONSOLE_SEVERITY;

        target.enable = ENABLED;
    }

    CGI_SET_ERR_HDL(sal_log_target_set(t, &target), retval);

    t = LOG_RAM;
    osal_memset(&target, 0, sizeof(target));

    if (CGI_IS_PARAM_EXIST(CGI_LOG_RAM_STATE, pStrValue))
    {
        
        sev = LOG_SEV_ALL;

        if (CGI_IS_PARAM_EXIST(CGI_LOG_RAM_SEV, pStrValue))
            sev = SYS_STR2UINT(pStrValue);

        target.severity = (sev < LOG_SEV_ALL) ? sev : DFLT_LOG_BUFFERED_SEVERITY;

        target.enable = ENABLED;
    }

    CGI_SET_ERR_HDL(sal_log_target_set(t, &target), retval);

    t = LOG_FLASH;
    osal_memset(&target, 0, sizeof(target));

    if (CGI_IS_PARAM_EXIST(CGI_LOG_FILE_STATE, pStrValue))
    {
        
        sev = LOG_SEV_ALL;

        if (CGI_IS_PARAM_EXIST(CGI_LOG_FILE_SEV, pStrValue))
            sev = SYS_STR2UINT(pStrValue);

        target.severity = (sev < LOG_SEV_ALL) ? sev : DFLT_LOG_FILE_SEVERITY;

        target.enable = ENABLED;
    }

    CGI_SET_ERR_HDL(sal_log_target_set(t, &target), retval);

    CGI_SET_ERR_HDL(sal_log_daemon_restart(TRUE), retval);

    if (CGI_IS_PARAM_EXIST(CGI_LOG_AGG_STATE, pStrValue))
        ratelimit = ENABLED;
    else
        ratelimit = DISABLED;

    interval = DFLT_LOG_AGGREGATION_INTERVAL;

    if (CGI_IS_PARAM_EXIST(CGI_LOG_AGG_AGING_TIME, pStrValue))
        interval = SYS_STR2UINT(pStrValue);

    CGI_SET_ERR_HDL(sal_log_ratelimitEnable_set(ratelimit), retval);
    CGI_SET_ERR_HDL(sal_log_ratelimitInterval_set(interval), retval);
    CGI_SET_ERR_HDL(sal_log_daemon_restart(TRUE), retval);

    if (CGI_IS_PARAM_EXIST(CGI_LOG_STATE, pStrValue))
        enable = ENABLED;
    else
        enable = DISABLED;

    CGI_SET_ERR_HDL(sal_log_enable_set(enable), retval);
    CGI_SET_ERR_HDL(sal_log_config_create(), retval);
    CGI_SET_ERR_HDL(sal_log_daemon_restart(FALSE), retval);

cgi_end:
    cgi_set_respGeneral_send(retval);

    return SYS_ERR_OK;
}

static int32 cgi_log_remoteAdd_set(void)
{
    int32 retval = SYS_ERR_OK;

    char *pStrValue;

    uint32 sev;
    uint32 facility;
    uint32 targetIndex = 0;

    sal_log_target_t target;

    osal_memset(&target, 0, sizeof(target));

    target.enable = ENABLED;

    if (CGI_IS_PARAM_EXIST(CGI_SRVHOST, pStrValue))
        strncpy(target.host, pStrValue, sizeof(target.host));
    else
        goto cgi_end;

    target.port = DFLT_LOG_HOST_PORT;

    if (CGI_IS_PARAM_EXIST(CGI_LOG_REMOTE_UDP_PORT, pStrValue))
        target.port = SYS_STR2UINT(pStrValue);

    target.facility = LOG_FACILITY_LOCAL7;

    if (CGI_IS_PARAM_EXIST(CGI_LOG_REMOTE_FACILITY, pStrValue))
    {
        facility = SYS_STR2UINT(pStrValue);

        switch (facility)
        {
            case 0:
                target.facility = LOG_FACILITY_LOCAL0;
                break;

            case 1:
                target.facility = LOG_FACILITY_LOCAL1;
                break;

            case 2:
                target.facility = LOG_FACILITY_LOCAL2;
                break;

            case 3:
                target.facility = LOG_FACILITY_LOCAL3;
                break;

            case 4:
                target.facility = LOG_FACILITY_LOCAL4;
                break;

            case 5:
                target.facility = LOG_FACILITY_LOCAL5;
                break;

            case 6:
                target.facility = LOG_FACILITY_LOCAL6;
                break;
            case 7:

                target.facility = LOG_FACILITY_LOCAL7;
                break;

            default:
                target.facility = LOG_FACILITY_LOCAL7;
                break;
        }
    }

    sev = LOG_SEV_ALL;

    if (CGI_IS_PARAM_EXIST(CGI_LOG_REMOTE_SEV, pStrValue))
        sev = SYS_STR2UINT(pStrValue);

    target.severity = (sev < LOG_SEV_ALL) ? sev : DFLT_LOG_HOST_SEVERITY;

    CGI_SET_ERR_HDL(sal_log_remoteIndexByIp_get(target.host, &targetIndex), retval);

    if (LOG_TARGET_MAX == targetIndex)
    {
        SYS_ERR_CHK(sal_log_remoteEmptyIndex_get(&targetIndex));

        if (LOG_TARGET_MAX == targetIndex)
        {
            retval = SYS_ERR_LOG_SERVER_FULL;
            goto cgi_end;
        }
    }
    else
    {
        retval = SYS_ERR_LOG_SERVER_EXIST;
        goto cgi_end;
    }

    CGI_SET_ERR_HDL(sal_log_target_set(targetIndex, &target), retval);
    CGI_SET_ERR_HDL(sal_log_config_create(), retval);
    CGI_SET_ERR_HDL(sal_log_daemon_restart(TRUE), retval);

    retval = SYS_ERR_OK;

cgi_end:
    cgi_set_respGeneral_send(retval);

    return SYS_ERR_OK;
}

static int32 cgi_log_remoteEdit_set(void)
{
    int32 retval = SYS_ERR_OK;

    char *pStrValue;

    uint32 facility;
    uint32 index;

    uint32 port_set = FALSE;
    uint32 facility_set = FALSE;
    uint32 severity_set = FALSE;

    char host[CAPA_HOSTNAME_LEN + 1];

    sal_log_target_t target;
    sal_log_target_t target_mod;

    SYS_MEM_CLEAR(host);
    SYS_MEM_CLEAR(target);
    SYS_MEM_CLEAR(target_mod);

    if (CGI_IS_PARAM_EXIST(CGI_LOG_REMOTE_HOST, pStrValue))
    {
        strncpy(host, pStrValue, sizeof(host));
    }
    else
    {
        retval = SYS_ERR_OK;
        goto cgi_end;
    }

    SYS_ERR_CHK(sal_util_hostname_resolv(host, target.host, sizeof(target.host)));

    if (CGI_IS_PARAM_EXIST(CGI_LOG_REMOTE_UDP_PORT, pStrValue))
    {
        port_set = TRUE;
        target.port = SYS_STR2UINT(pStrValue);
    }

    if (CGI_IS_PARAM_EXIST(CGI_LOG_REMOTE_FACILITY, pStrValue))
    {
        facility_set = TRUE;
        facility = SYS_STR2UINT(pStrValue);

        switch (facility)
        {
            case 0:
                target.facility = LOG_FACILITY_LOCAL0;
                break;

            case 1:
                target.facility = LOG_FACILITY_LOCAL1;
                break;

            case 2:
                target.facility = LOG_FACILITY_LOCAL2;
                break;

            case 3:
                target.facility = LOG_FACILITY_LOCAL3;
                break;

            case 4:
                target.facility = LOG_FACILITY_LOCAL4;
                break;

            case 5:
                target.facility = LOG_FACILITY_LOCAL5;
                break;

            case 6:
                target.facility = LOG_FACILITY_LOCAL6;
                break;
            case 7:

                target.facility = LOG_FACILITY_LOCAL7;
                break;

            default:
                target.facility = LOG_FACILITY_LOCAL7;
                break;
        }
    }

    if (CGI_IS_PARAM_EXIST(CGI_LOG_REMOTE_SEV, pStrValue))
    {
        target.severity = SYS_STR2UINT(pStrValue);
        severity_set = TRUE;
    }

    index = LOG_TARGET_MAX;

    sal_log_remoteIndexByIp_get(target.host, &index);

    if (LOG_TARGET_MAX == index)
    {
        retval = SYS_ERR_OK;
        goto cgi_end;
    }

    if (SYS_ERR_OK != sal_log_target_get(index, &target_mod))
    {
        retval = SYS_ERR_OK;
        goto cgi_end;
    }

    if (TRUE == port_set)
        target_mod.port = target.port;

    if (TRUE == facility_set)
        target_mod.facility = target.facility;

    if (TRUE == severity_set)
        target_mod.severity = target.severity;

    CGI_SET_ERR_HDL(sal_log_target_set(index, &target_mod), retval);

    CGI_SET_ERR_HDL(sal_log_config_create(), retval);
    CGI_SET_ERR_HDL(sal_log_daemon_restart(TRUE), retval);

    retval = SYS_ERR_OK;

cgi_end:
    cgi_set_respGeneral_send(retval);

    return SYS_ERR_OK;
}

static int32 cgi_log_remoteDel_set(void)
{
    int32 retval = SYS_ERR_OK;

    uint32 i = 0;
    uint32 len = 0;
    char **array = NULL;

    uint32 index;
    sal_log_target_t target;

    index = 0;
    osal_memset(&target, 0, sizeof(target));

    if (!CGI_GET_PARAM_ARRAY_DATA(CGI_LOG_REMOTE_HOST, array, &len))
        goto cgi_end;

    for (i = 0; i < len; i++)
    {
        if (SYS_ERR_OK != sal_log_remoteIndexByIp_get(array[i], &index))
            goto cgi_restart;

        if (LOG_TARGET_MAX <= index)
            goto cgi_restart;

        if (SYS_ERR_OK != sal_log_target_set(index, &target))
            goto cgi_restart;
    }

cgi_restart:
    CGI_SET_ERR_HDL(sal_log_config_create(), retval);
    CGI_SET_ERR_HDL(sal_log_daemon_restart(TRUE), retval);

cgi_end:
    if (array)
        free(array);

    cgi_set_respGeneral_send(retval);

    return SYS_ERR_OK;
}
