
#include <include/cgi.h>

#include <sys/mman.h>

static int32 cgi_log_syslog_get(void);
static int32 cgi_log_global_get(void);
static int32 cgi_log_remote_get(void);
static int32 cgi_log_remoteAdd_get(void);
static int32 cgi_log_remoteEdit_get(void);

const cgi_cmdmap_t cmdGetLogTbl[] =
{
    { CGI_CMD_LOG_SYSLOG,       cgi_log_syslog_get      },
    { CGI_CMD_LOG_GLOBAL,       cgi_log_global_get      },
    { CGI_CMD_LOG_REMOTE,       cgi_log_remote_get      },
    { CGI_CMD_LOG_REMOTE_ADD,   cgi_log_remoteAdd_get   },
    { CGI_CMD_LOG_REMOTE_EDIT,  cgi_log_remoteEdit_get  },
    { NULL,                     NULL                    }
};

static int32 _cgi_log_msg_get(cJSON *logs, char *file, uint32 start, uint32 end, uint32 *pEntry)
{
    cJSON *log  = NULL;

    uint32 i = 0;
    uint32 l = 0;
    uint32 len = 0;

    uint32 sevid    = 0;
    char str[SYS_BUF1024_LEN];

    int fd;

    char *map;
    char *p;
    char *e;

    char *sevp  = NULL;
    char *timep = NULL;
    char *catp  = NULL;
    char *msgp  = NULL;
    char *mnemp = NULL;
    char *tmpp  = NULL;

    char  strbuf[SYS_BUF4096_LEN];

    struct stat s;

    SYS_PARAM_CHK(NULL == file, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);

    *pEntry = 0;

    fd = open(file, O_RDONLY);

    if (fd < 0)
        return SYS_ERR_OK;

    if (fstat(fd, &s))
    {
        close(fd);
        return SYS_ERR_OK;
    }

    map = mmap(0, s.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        return SYS_ERR_FILE_OPEN;
    }

    e = p = map + s.st_size - 1;

    while (p >= map)
    {
        if (*p == '\n')
            p--;

        while (p >= map && *p != '\n')
            p--;

        len = e - p > sizeof(strbuf) - 1 ? sizeof(strbuf) - 1 : e - p;

        osal_memset(strbuf, 0, sizeof(strbuf));
        osal_memcpy(strbuf, p + 1, len);

        if (strbuf[strlen(strbuf) - 1] == '\n')
            strbuf[strlen(strbuf) - 1] = '\0';

        tmpp = strbuf;
        sevp = NULL;
        catp = NULL;
        timep = NULL;

        if (*tmpp == '<')
        {
            sevp = tmpp + 1;
            if ((tmpp = strchr(sevp, '>')) != NULL)
                *tmpp++ = '\0';
        }

        if (tmpp && *tmpp != '\0')
        {
            if (strlen(tmpp) >= 21 && tmpp[0] == '*' &&
                tmpp[4] == ' ' && tmpp[7] == ' ' && tmpp[12] == ' ' &&
                tmpp[15] == ':' && tmpp[18] == ':' && tmpp[21] == ':')
            {
                timep = tmpp;
                tmpp += 21;
                timep++;
                *tmpp++ = '\0';
            }
        }

        if (tmpp && *tmpp != '\0')
        {
            if (*tmpp == ' ')
                tmpp++;

            if (*tmpp == '%' || *tmpp == '!')
            {
                catp = tmpp;
                if ((tmpp = strchr(catp, ':')) != NULL)
                    *tmpp++ = '\0';
            }
        }

        if ((NULL == sevp) || (NULL == timep) || (NULL == catp))
            goto next;

        msgp = catp + strlen(catp) + 1;

        if (*msgp == ' ')
            msgp++;

        if (msgp > strbuf + len)
            goto next;

        if (msgp[strlen(msgp) - 1] == '\n')
            msgp[strlen(msgp) - 1] = '\0';

        if ('%' == catp[0])
        {
            
            catp++;

            catp = osal_strtok(catp, "-");
            tmpp = osal_strtok(NULL, "-");
            mnemp = osal_strtok(NULL, "-");

            if (tmpp)
                sevp = tmpp;

            if (mnemp)
                snprintf(str, sizeof(str), "%s-%u-%s: %s", catp, sevid, mnemp, msgp);
            else
                snprintf(str, sizeof(str), "%s-%u: %s", catp, sevid, msgp);
        }
        else if ('!' == catp[0])
        {
            
            catp++;
            snprintf(str, sizeof(str), "%s: %s", catp, msgp);
        }
        else
        {
            goto next;
        }

        sevid = sys_util_str2ul_ret(sevp);

        if (start <= i && end >= i)
        {
            if (logs)
            {
                log = cJSON_CreateObject();

                cJSON_AddNumberToObject(log, CGI_LOG_MSG_IDX, i + 1);
                cJSON_AddStringToObject(log, CGI_LOG_MSG_TIME, timep);
                cJSON_AddStringToObject(log, CGI_LOG_MSG_SEV, text_log_sev[sevid].text);
                cJSON_AddStringToObject(log, CGI_LOG_MSG_DESC, str);

                cJSON_AddItemToArray(logs, log);
            }

            l++;
        }

        i++;
next:
        e = p;
    }

    munmap(map, s.st_size);
    close(fd);

    *pEntry = l;

    return SYS_ERR_OK;
}

static int32 cgi_log_syslog_get(void)
{
    cJSON *root = NULL;
    cJSON *data = NULL;
    cJSON *logs = NULL;

    int fd;
    char tmpfile[] = "/tmp/ram_XXXXXX";
    char logfile[sizeof(tmpfile) + 4]; 
    char *file = NULL;
    char *pStrValue = NULL;

    uint32 entry = 0;

    log_target_t target = LOG_RAM;

    if (CGI_IS_PARAM_EXIST(CGI_LOG_TARGET, pStrValue))
       target = SYS_STR2UINT(pStrValue) % 2 ? LOG_FLASH : LOG_RAM;

    switch (target)
    {
        case LOG_RAM:
            file = LOG_FILE_RAM;
            break;

        case LOG_FLASH:
            file = LOG_FILE_FLASH;
            break;

        default:
            file = LOG_FILE_RAM;
            break;
    }

    if (0 <= (fd = mkstemp(tmpfile)))
    {
        close(fd);
        remove(tmpfile);
    }

    snprintf(logfile, sizeof(logfile), "%s.log", tmpfile);

    sal_util_file_copy(file, logfile);

    CGI_GET_JSON_CREATE(root, data);

    logs = cJSON_CreateArray();

    _cgi_log_msg_get(logs, logfile, 0, (uint32)-1, &entry);

    remove(logfile);

    if (0 == cJSON_GetArraySize(logs))
    {
        cJSON_Delete(logs);
        logs = cJSON_CreateNull();
    }

    cJSON_AddItemToObject(data, CGI_LOG_LOGS, logs);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_log_global_get(void)
{
    cJSON *data = NULL;
    cJSON *root = NULL;

    int32 sev = 0;
    uint32 interval = 0;

    sys_enable_t enable;
    sys_enable_t ratelimit;
    sal_log_target_t log_console;
    sal_log_target_t log_ram;
    sal_log_target_t log_file;

    sal_log_enable_get(&enable);

    sal_log_ratelimitEnable_get(&ratelimit);
    sal_log_ratelimitInterval_get(&interval);

    sal_log_target_get(LOG_CONSOLE, &log_console);
    sal_log_target_get(LOG_RAM, &log_ram);
    sal_log_target_get(LOG_FLASH, &log_file);

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddBoolToObject(data, CGI_LOG_STATE, enable);

    cJSON_AddBoolToObject(data, CGI_LOG_AGG_STATE, ratelimit);
    cJSON_AddNumberToObject(data, CGI_LOG_AGG_AGING_TIME, interval);
    cJSON_AddNumberToObject(data, CGI_LOG_AGG_AGING_TIME_MIN, BOND_LOG_AGGREGATION_INTERVAL_MIN);
    cJSON_AddNumberToObject(data, CGI_LOG_AGG_AGING_TIME_MAX, BOND_LOG_AGGREGATION_INTERVAL_MAX);
    cJSON_AddNumberToObject(data, CGI_LOG_AGG_AGING_TIME_DFLT, DFLT_LOG_AGGREGATION_INTERVAL);

    cJSON_AddBoolToObject(data, CGI_LOG_CONSOLE_STATE, log_console.enable);
    cJSON_AddBoolToObject(data, CGI_LOG_RAM_STATE, log_ram.enable);
    cJSON_AddBoolToObject(data, CGI_LOG_FILE_STATE, log_file.enable);

    sev = (ENABLED == log_console.enable) ? log_console.severity : DFLT_LOG_CONSOLE_SEVERITY;
    cJSON_AddNumberToObject(data, CGI_LOG_CONSOLE_SEV, sev);

    sev = (ENABLED == log_ram.enable) ? log_ram.severity : DFLT_LOG_BUFFERED_SEVERITY;
    cJSON_AddNumberToObject(data, CGI_LOG_RAM_SEV, sev);

    sev = (ENABLED == log_file.enable) ? log_file.severity : DFLT_LOG_FILE_SEVERITY;
    cJSON_AddNumberToObject(data, CGI_LOG_FILE_SEV, sev);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
static int32 cgi_log_remote_get(void)
{
    cJSON *data    = NULL;
    cJSON *root    = NULL;
    cJSON *remotes = NULL;
    cJSON *remote  = NULL;

    uint32 host;

    char *sev_str;

    sal_log_target_t log_remote;

    SYS_MEM_CLEAR(log_remote);

    CGI_GET_JSON_CREATE(root, data);

    for (host = LOG_REMOTE; host < LOG_TARGET_MAX; host++)
    {
        SYS_MEM_CLEAR(log_remote);

        if (SYS_ERR_OK != sal_log_target_get(host, &log_remote))
            continue;

        if (0 == strlen(log_remote.host))
            continue;

        if (NULL == remotes)
            remotes = cJSON_CreateArray();

        remote = cJSON_CreateObject();

        cJSON_AddItemToArray(remotes, remote);

        cJSON_AddStringToObject(remote, CGI_LOG_REMOTE_HOST, log_remote.host);
        cJSON_AddNumberToObject(remote, CGI_LOG_REMOTE_UDP_PORT, log_remote.port);
        cJSON_AddNumberToObject(remote, CGI_LOG_REMOTE_FACILITY, log_remote.facility - LOG_FACILITY_LOCAL0);

        switch (log_remote.severity)
        {
            case LOG_SEV_EMERG:
                sev_str = "txtEmergency";
                break;

            case LOG_SEV_ALERT:
                sev_str = "txtAlert";
                break;

            case LOG_SEV_CRIT:
                sev_str = "txtCritical";
                break;

            case LOG_SEV_ERR:
                sev_str = "txtError";
                break;

            case LOG_SEV_WARNING:
                sev_str = "txtWarning";
                break;

            case LOG_SEV_NOTICE:
                sev_str = "txtNotice";
                break;

            case LOG_SEV_INFO:
                sev_str = "txtInformational";
                break;

            case LOG_SEV_DEBUG:
                sev_str = "txtDebug";
                break;

            default:
                sev_str = "txtDebug";
                break;
        }

        cJSON_AddLangStringToObject(remote, CGI_LOG_REMOTE_SEV, "log", sev_str);
    }

    if (!remotes)
        remotes = cJSON_CreateNull();

    cJSON_AddItemToObject(data, CGI_LOG_REMOTE_HOSTS, remotes);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_log_remoteAdd_get(void)
{
    cJSON *data    = NULL;
    cJSON *root    = NULL;

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_HOST_MAXLEN, CAPA_HOSTNAME_LEN);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_UDP_MIN, BOND_LOG_HOST_PORT_MIN);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_UDP_MAX, BOND_LOG_HOST_PORT_MAX);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_UDP_DFLT, DFLT_LOG_HOST_PORT);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_FACILITY_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_FACILITY_MAX, 7);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_FACILITY, LOG_FACILITY_LOCAL7 - LOG_FACILITY_LOCAL0);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_SEV, DFLT_LOG_HOST_SEVERITY);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_log_remoteEdit_get(void)
{
    cJSON *data    = NULL;
    cJSON *root    = NULL;

    char *pStrValue = NULL;

    uint32 host;

    sal_log_target_t log_remote;

    SYS_MEM_CLEAR(log_remote);

    if (CGI_IS_PARAM_EXIST(CGI_LOG_REMOTE_HOST, pStrValue))
    {
        for (host = LOG_REMOTE; host < LOG_TARGET_MAX; host++)
        {
            SYS_MEM_CLEAR(log_remote);

            if (SYS_ERR_OK != sal_log_target_get(host, &log_remote))
                continue;

            if (0 == strlen(log_remote.host))
                continue;

            if (0 == osal_strcmp(log_remote.host, pStrValue))
                break;
        }
    }

    if (0 == strlen(log_remote.host))
    {
        log_remote.port = DFLT_LOG_HOST_PORT;
        log_remote.facility = LOG_FACILITY_LOCAL0 + LOG_FACILITY_LOCAL7;
        log_remote.severity = DFLT_LOG_HOST_SEVERITY;
    }

    CGI_GET_JSON_CREATE(root, data);

    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_UDP_MIN, BOND_LOG_HOST_PORT_MIN);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_UDP_MAX, BOND_LOG_HOST_PORT_MAX);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_UDP_DFLT, DFLT_LOG_HOST_PORT);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_UDP_PORT, log_remote.port);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_FACILITY, log_remote.facility - LOG_FACILITY_LOCAL0);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_FACILITY_MIN, 0);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_FACILITY_MAX, 7);
    cJSON_AddNumberToObject(data, CGI_LOG_REMOTE_SEV, log_remote.severity);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

#if 0
static int32 _cgi_log_msg_get(cJSON *logs, char *file, uint32 start, uint32 end, uint32 *pEntry)
{
    cJSON *log  = NULL;

    uint32 i = 0;
    uint32 l = 0;
    uint32 len = 0;

    uint32 sevid    = 0;

    int fd;

    char *map;
    char *p;
    char *e;

    char *sevp  = NULL;
    char *timep = NULL;
    char *catp  = NULL;
    char *msgp  = NULL;
    char *mnemp = NULL;
    char *tmpp  = NULL;

    char  strbuf[SYS_BUF4096_LEN];

    struct stat s;

    SYS_PARAM_CHK(NULL == file, SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(NULL == pEntry, SYS_ERR_NULL_POINTER);

    *pEntry = 0;

    fd = open(file, O_RDONLY);

    if (fd < 0 || fstat(fd, &s))
        return SYS_ERR_OK;

    map = mmap(0, s.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        return SYS_ERR_FILE_OPEN;
    }

    e = p = map + s.st_size - 1;

    while (p >= map)
    {
        if (*p == '\n')
            p--;

        while (p >= map && *p != '\n')
            p--;

        len = e - p > sizeof(strbuf) - 1 ? sizeof(strbuf) - 1 : e - p;

        osal_memset(strbuf, 0, sizeof(strbuf));
        osal_memcpy(strbuf, p + 1, len);

        sevp = osal_strtok(strbuf, ";");
        timep = osal_strtok(NULL, ";");
        catp = osal_strtok(NULL, ":");

        if ((NULL == sevp) || (NULL == timep) || (NULL == catp))
            goto next;

        msgp = catp + strlen(catp) + 1;

        if (*msgp == ' ')
            msgp++;

        if (msgp > strbuf + len)
            goto next;

        if (catp[0] == '%')
        {
            
            catp++;

            catp = osal_strtok(catp, "-");
            tmpp = osal_strtok(NULL, "-");
            mnemp = osal_strtok(NULL, "-");

            if (tmpp)
                sevp = tmpp;
        }

        sevid = sys_util_str2ul_ret(sevp);

        if (mnemp)
        {
            msgp = mnemp;
            msgp[strlen(mnemp)] = ':';
        }

        if (msgp[strlen(msgp) - 1] == '\n')
            msgp[strlen(msgp) - 1] = '\0';

        if (start <= i && end >= i)
        {
            if (logs)
            {
                log = cJSON_CreateObject();

                cJSON_AddNumberToObject(log, CGI_LOG_IDX, i + 1);
                cJSON_AddStringToObject(log, CGI_LOG_TIME, timep);
                cJSON_AddStringToObject(log, CGI_LOG_SEVERITY, text_log_sev[sevid].text);
                cJSON_AddStringToObject(log, CGI_LOG_DESC, msgp);

                cJSON_AddItemToArray(logs, log);
            }

            l++;
        }

        i++;
next:
        e = p;
    }

    munmap(map, s.st_size);
    close(fd);

    *pEntry = l;

    return SYS_ERR_OK;
}

static int32 cgi_log_ram_get(void)
{
    cJSON *data = NULL;
    cJSON *root = NULL;
    cJSON *logs = NULL;
    cJSON *page = NULL;

    cgi_paging_t paging;

    uint32 entry = 0;

    int fd;
    char tmpfile[] = "/tmp/ram_XXXXXX";
    char logfile[sizeof(tmpfile) + 4]; 
    char *pStrValue;

    SYS_MEM_CLEAR(paging);

    if (!access(SYS_LOG_USER_ALARM_FILE, F_OK))
        remove(SYS_LOG_USER_ALARM_FILE);

    if (0 <= (fd = mkstemp(tmpfile)))
    {
        close(fd);
        remove(tmpfile);
    }
    snprintf(logfile, sizeof(logfile), "%s.log", tmpfile);

    sal_util_file_copy(LOG_FILE_RAM, logfile);

    _cgi_log_msg_get(NULL, logfile, 0, (uint32)-1, &entry);

    paging.maxEntries = entry;

    if (CGI_IS_PARAM_EXIST(CGI_PAGING_PAGESIZE, pStrValue))
        paging.pageSize = SYS_STR2UINT(pStrValue);
    else
        paging.pageSize = CGI_PAGING_DFLT_SIZE;

    if (CGI_IS_PARAM_EXIST(CGI_PAGING_GOTOPAGE,pStrValue))
        paging.currPage = SYS_STR2UINT(pStrValue);
    else
        paging.currPage = 1;

    CGI_GET_JSON_CREATE(root, data);

    cgi_get_pagingEntry_get(&paging);

    page = cJSON_CreateObject();

    cJSON_AddItemToObject(data, CGI_PAGING, page);

    cJSON_AddNumberToObject(page, CGI_PAGING_CURRPAGE, paging.currPage);
    cJSON_AddNumberToObject(page, CGI_PAGING_PAGESIZE, paging.pageSize);
    cJSON_AddNumberToObject(page, CGI_PAGING_MAXENTRY, paging.maxEntries);

    logs = cJSON_CreateArray();

    _cgi_log_msg_get(logs, logfile, paging.start, paging.end, &entry);

    remove(logfile);

    if (0 == cJSON_GetArraySize(logs))
    {
        cJSON_Delete(logs);
        logs = cJSON_CreateNull();
    }

    cJSON_AddItemToObject(data, CGI_LOG_LOGS, logs);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}

static int32 cgi_log_file_get(void)
{
    cJSON *data = NULL;
    cJSON *root = NULL;
    cJSON *logs = NULL;
    cJSON *page = NULL;

    cgi_paging_t paging;

    uint32 entry = 0;

    int fd;
    char tmpfile[] = "/tmp/flash_XXXXXX";
    char logfile[sizeof(tmpfile) + 4]; 
    char *pStrValue;

    SYS_MEM_CLEAR(paging);

    if (0 <= (fd = mkstemp(tmpfile)))
    {
        close(fd);
        remove(tmpfile);
    }

    snprintf(logfile, sizeof(logfile), "%s.log", tmpfile);

    sal_util_file_copy(LOG_FILE_FLASH, logfile);

    _cgi_log_msg_get(NULL, logfile, 0, (uint32)-1, &entry);

    paging.maxEntries = entry;

    if (CGI_IS_PARAM_EXIST(CGI_PAGING_PAGESIZE, pStrValue))
        paging.pageSize = SYS_STR2UINT(pStrValue);
    else
        paging.pageSize = CGI_PAGING_DFLT_SIZE;

    if (CGI_IS_PARAM_EXIST(CGI_PAGING_GOTOPAGE,pStrValue))
        paging.currPage = SYS_STR2UINT(pStrValue);
    else
        paging.currPage = 1;

    CGI_GET_JSON_CREATE(root, data);

    cgi_get_pagingEntry_get(&paging);

    page = cJSON_CreateObject();

    cJSON_AddItemToObject(data, CGI_PAGING, page);

    cJSON_AddNumberToObject(page, CGI_PAGING_CURRPAGE, paging.currPage);
    cJSON_AddNumberToObject(page, CGI_PAGING_PAGESIZE, paging.pageSize);
    cJSON_AddNumberToObject(page, CGI_PAGING_MAXENTRY, paging.maxEntries);

    logs = cJSON_CreateArray();

    _cgi_log_msg_get(logs, logfile, paging.start, paging.end, &entry);

    remove(logfile);

    if (0 == cJSON_GetArraySize(logs))
    {
        cJSON_Delete(logs);
        logs = cJSON_CreateNull();
    }

    cJSON_AddItemToObject(data, CGI_LOG_LOGS, logs);

    CGI_GET_JSON_RESPONSE_AND_DESTROY(root);

    return SYS_ERR_OK;
}
#endif
