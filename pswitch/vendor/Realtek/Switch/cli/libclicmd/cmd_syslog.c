
#include <time.h>
#include <sys/mman.h>

#include <libsal/sal_syslog.h>
#include <libcmd/cmd.h>

static int32 _cli_log_show(uint32 targetIndex, char *filepath, uint32* lineTotal)
{
    uint32 len = 0;

    uint32 sevid = 0;

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

    (*lineTotal) = 0;

    fd = open(filepath, O_RDONLY);

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

            sevid = sys_util_str2ul_ret(sevp);
            sevid &= 0xF;

            if (mnemp)
                xmore_out("*%16s: %s-%u-%s: %s\n", timep, catp, sevid, mnemp, msgp);
            else
                xmore_out("*%16s: %s-%u: %s\n", timep, catp, sevid, msgp);
        }
        else if ('!' == catp[0])
        {
            
            catp++;
            xmore_out("*%16s: %s: %s\n", timep, catp, msgp);
        }
        (*lineTotal) ++;

next:
        e = p;
    }

    munmap(map, s.st_size);
    close(fd);

    return SYS_ERR_OK;
}

int32 cmd_log_target_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char buf[SYS_BUF1024_LEN] = "";
    uint32 i = 0;
    uint32 j = 0;
    sal_log_target_t target;
    sys_enable_t enable = DISABLED;
    uint32 currentLine;

    uint32 ratelimitInterval = 0;
    sys_enable_t ratelimitEnable = DISABLED;

    SYS_ERR_CHK(sal_log_enable_get(&enable));
    SYS_ERR_CHK(sal_log_ratelimitEnable_get(&ratelimitEnable));
    SYS_ERR_CHK(sal_log_ratelimitInterval_get(&ratelimitInterval));

    XMORE("\nLogging service is %s\n\n", text_enable[enable].text);

    XMORE("Aggregation: %s\n", text_enable[ratelimitEnable].text);
    XMORE("Aggregation aging time: %u sec\n\n", ratelimitInterval);

    for (i = LOG_LOCAL; i < LOG_LOCAL_MAX; i++)
    {
        j = 0;
        SYS_MEM_CLEAR(buf);
        SYS_MEM_CLEAR(target);

        SYS_ERR_CHK(sal_log_target_get(i, &target));

        switch (i)
        {
            case LOG_CONSOLE:
                j = osal_sprintf(buf, "Console Logging: ");
                break;
            case LOG_RAM:
                j = osal_sprintf(buf, "Buffer Logging : ");
                break;
            case LOG_FLASH:
                j = osal_sprintf(buf, "File Logging   : ");
                break;
        }

        if (ENABLED == target.enable)
            j += osal_sprintf(buf + j, "level %s", text_log_sev[target.severity].text);
        else
            j += osal_sprintf(buf + j, "%s", text_enable[target.enable].text);

        XMORE("%s\n", buf);
    }

    XMORE("\n");

    for (i = LOG_REMOTE; i < LOG_TARGET_MAX; i++)
    {
        SYS_MEM_CLEAR(buf);
        SYS_MEM_CLEAR(target);

        SYS_ERR_CHK(sal_log_target_get(i, &target));

        if (0 == osal_strlen(target.host))
            continue;

        XMORE("Logging Server: %s, port %u, level %s, facility local%u\n",
               target.host, target.port, text_log_sev[target.severity].text,
               target.facility - LOG_FACILITY_LOCAL0);
    }

    XMORE("\n");

    if (0 != argc)
    {
        if (text_log_target[LOG_RAM].text[0] == argv[0][0])
        {
            XMORE("Buffer Logging\n");
            XMORE("--------------\n");

            SYS_ERR_CHK(_cli_log_show(LOG_RAM, LOG_FILE_RAM, &currentLine));

            XMORE("\n");
            XMORE("Buffer logging current number of log entries:%d \n", currentLine);
            
        }
        else if (text_log_target[LOG_FLASH].text[0] == argv[0][0])
        {
            XMORE("File Logging\n");
            XMORE("------------\n");

            SYS_ERR_CHK(_cli_log_show(LOG_FLASH, LOG_FILE_FLASH, &currentLine));

            XMORE("\n");
            XMORE("File logging current number of log entries:%d \n", currentLine);
            
        }
    }
    else
    {
        XMORE("Buffer Logging\n");
        XMORE("--------------\n");

        SYS_ERR_CHK(_cli_log_show(LOG_RAM, LOG_FILE_RAM, &currentLine));

        XMORE("\n");
        XMORE("Buffer logging current number of log entries:%d \n", currentLine);
    }

    return SYS_ERR_OK;
}

int32 cmd_log_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (text_log_target[LOG_FLASH].text[0] == argv[0][0])
        SYS_ERR_CHK(sal_log_file_clear(LOG_FLASH));
    else if (text_log_target[LOG_RAM].text[0] == argv[0][0])
        SYS_ERR_CHK(sal_log_file_clear(LOG_RAM));

    SYS_ERR_CHK(sal_log_daemon_restart(FALSE));

    return SYS_ERR_OK;
}

int32 cmd_log_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_log_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED : ENABLED));

    if (CMD_IS_CLI_RUN())
    {
        SYS_ERR_CHK(sal_log_daemon_restart(FALSE));
    }

    return SYS_ERR_OK;
}

int32 cmd_log_target_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char *ridx = NULL;
    uint32 i = 0;
    uint32 port = DFLT_LOG_HOST_PORT;
    uint32 severity =  DFLT_LOG_HOST_SEVERITY;
    uint32 targetIndex = 0;
    uint32 tokenCnt = 0;
    log_facility_t facility = LOG_FACILITY_LOCAL7;
    sal_log_target_t target;

    int32 port_set = FALSE;
    int32 severity_set = FALSE;
    int32 facility_set = FALSE;

    osal_memset(&target, 0, sizeof(target));

    if ((0 != osal_strncmp(text_log_target[LOG_FLASH].text, argv[0], strlen(argv[0])))
        && (0 != osal_strncmp(text_log_target[LOG_CONSOLE].text, argv[0], strlen(argv[0])))
        && (0 != osal_strncmp(text_log_target[LOG_RAM].text, argv[0], strlen(argv[0])))) 
    {
        if (osal_strlen(argv[0]) > (sizeof(target.host) - 1))
            return SYS_ERR_INPUT;

        tokenCnt = CMD_TOKENNUM_GET(cmd->vline);

        for (i = 3; i < tokenCnt ; i++)
        {
            ridx = CMD_TOKENSTR_GET(cmd->vline, i);

            if ('p' == ridx[0]) 
            {
                port = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
                port_set = TRUE;
            }
            else if ('s' == ridx[0]) 
            {
                severity = SYS_STR2UINT(CMD_TOKENSTR_GET(cmd->vline, ++i));
                severity_set = TRUE;
            }
            else if ('f' == ridx[0]) 
            {
                ridx = CMD_TOKENSTR_GET(cmd->vline, ++i);

                switch (ridx[5])
                {
                    case '0':
                        facility = LOG_FACILITY_LOCAL0;
                        break;

                    case '1':
                        facility = LOG_FACILITY_LOCAL1;
                        break;

                    case '2':
                        facility = LOG_FACILITY_LOCAL2;
                        break;

                    case '3':
                        facility = LOG_FACILITY_LOCAL3;
                        break;

                    case '4':
                        facility = LOG_FACILITY_LOCAL4;
                        break;

                    case '5':
                        facility = LOG_FACILITY_LOCAL5;
                        break;

                    case '6':
                        facility = LOG_FACILITY_LOCAL6;
                        break;

                    case '7':
                    default:
                        facility = LOG_FACILITY_LOCAL7;
                }

                facility_set = TRUE;
            }
        }

        SYS_ERR_CHK(sal_log_remoteIndexByIp_get(argv[0], &targetIndex));

        if (LOG_TARGET_MAX == targetIndex)
        {
            SYS_ERR_CHK(sal_log_remoteEmptyIndex_get(&targetIndex));

            if (LOG_TARGET_MAX == targetIndex)
                return SYS_ERR_LOG_SERVER_FULL;
        }

        SYS_ERR_CHK(sal_log_target_get(targetIndex, &target));

        if (0 == osal_strlen(target.host))
        {
            target.enable = ENABLED;
            osal_strcpy(target.host, argv[0]);
            target.port = port;
            target.severity = severity;
            target.facility = facility;
        }
        else
        {
            if (TRUE == port_set)
                target.port = port;

            if (TRUE == severity_set)
                target.severity = severity;

            if (TRUE == facility_set)
                target.facility = facility;
        }

        SYS_ERR_CHK(sal_log_target_set(targetIndex, &target));
    }
    else
    {
        if (text_log_target[LOG_FLASH].text[0] == argv[0][0])
        {
            targetIndex = LOG_FLASH;
            severity = DFLT_LOG_FILE_SEVERITY;
        }
        else if (text_log_target[LOG_CONSOLE].text[0] == argv[0][0])
        {
            targetIndex = LOG_CONSOLE;
            severity = DFLT_LOG_CONSOLE_SEVERITY;
        }
        else if (text_log_target[LOG_RAM].text[0] == argv[0][0])
        {
            targetIndex = LOG_RAM;
            severity = DFLT_LOG_BUFFERED_SEVERITY;
        }

        if (argc == 2)
            severity = SYS_STR2UINT(argv[1]);

        target.enable = ENABLED;
        target.severity = severity;

        SYS_ERR_CHK(sal_log_target_set(targetIndex, &target));
    }

    SYS_ERR_CHK(sal_log_config_create());

    if (CMD_IS_CLI_RUN())
    {
        SYS_ERR_CHK(sal_log_daemon_restart(TRUE));
    }

    return SYS_ERR_OK;
}

int32 cmd_log_noTarget_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 targetIndex = LOG_TARGET_MAX;
    sal_log_target_t target;

    osal_memset(&target, 0, sizeof(target));

    if (0 == osal_strncmp(text_log_target[LOG_RAM].text, argv[0], strlen(argv[0])))
    {
        
        targetIndex = LOG_RAM;
    }
    else if (0 == osal_strncmp(text_log_target[LOG_CONSOLE].text, argv[0], strlen(argv[0])))
    {
        
        targetIndex =  LOG_CONSOLE;
    }
    else if (0 == osal_strncmp(text_log_target[LOG_FLASH].text, argv[0], strlen(argv[0])))
    {
        
        targetIndex =  LOG_FLASH;
    }
    else
    {
        
        if (osal_strlen(argv[0]) > (sizeof(target.host) - 1))
            return SYS_ERR_INPUT;

        SYS_ERR_CHK(sal_log_remoteIndexByIp_get(argv[0], &targetIndex));

        if (LOG_TARGET_MAX == targetIndex)
            return SYS_ERR_LOG_SERVER_NOT_EXIST;
    }

    SYS_ERR_CHK(sal_log_target_set(targetIndex, &target));

    SYS_ERR_CHK(sal_log_config_create());

    if (CMD_IS_CLI_RUN())
    {
        SYS_ERR_CHK(sal_log_daemon_restart(TRUE));
    }

    return SYS_ERR_OK;
}

int32 cmd_log_ratelimitEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_log_ratelimitEnable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED : ENABLED));

    SYS_ERR_CHK(sal_log_config_create());

    if (CMD_IS_CLI_RUN())
    {
        SYS_ERR_CHK(sal_log_daemon_restart(FALSE));
    }

    return SYS_ERR_OK;
}

int32 cmd_log_ratelimitInterval_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 interval = 0;
    sys_enable_t enable = DISABLED;

    interval = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_log_ratelimitInterval_set((interval)));
    SYS_ERR_CHK(sal_log_ratelimitEnable_get((&enable)));

    if (ENABLED == enable)
    {
        SYS_ERR_CHK(sal_log_config_create());

        if (CMD_IS_CLI_RUN())
        {
            SYS_ERR_CHK(sal_log_daemon_restart(FALSE));
        }
    }

    return SYS_ERR_OK;
}

static int32 _cmd_log_target_save(FILE *fp)
{
    uint32 i;
    char cmd[MAX_CMD_STRLEN];
    sal_log_target_t target;
    sys_enable_t dfltLocalEnable[] =
    {
        DFLT_LOG_BUFFERED_EBL,      
        DFLT_LOG_CONSOLE_EBL,       
        DFLT_LOG_FILE_EBL,          
    };
    uint32 dfltLocalSeverity[] =
    {
        DFLT_LOG_BUFFERED_SEVERITY, 
        DFLT_LOG_CONSOLE_SEVERITY,  
        DFLT_LOG_FILE_SEVERITY,     
    };

    for (i = LOG_LOCAL; i < LOG_LOCAL_MAX; i++)
    {
        osal_memset(cmd, 0, sizeof(cmd));
        osal_memset(&target, 0, sizeof(target));

        SYS_ERR_CHK(sal_log_target_get(i, &target));

        if (DISABLED == target.enable)
        {
            if (DISABLED == dfltLocalEnable[i])
                continue;
            else
                osal_sprintf(cmd, "no logging %s\n", text_log_target[i].text);
        }
        else
        {
            if (dfltLocalSeverity[i] == target.severity)
            {
                if (ENABLED == dfltLocalEnable[i])
                    continue;
                else
                    osal_sprintf(cmd, "logging %s\n", text_log_target[i].text);
            }
            else
            {
                osal_sprintf(cmd, "logging %s severity %u\n", text_log_target[i].text, target.severity);

            }
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    for (i = LOG_REMOTE; i < LOG_TARGET_MAX; i++)
    {
        osal_memset(&target, 0, sizeof(target));
        SYS_ERR_CHK(sal_log_target_get(i, &target));

        if (0 == osal_strlen(target.host))
            continue;

        osal_memset(cmd, 0, sizeof(cmd));

        osal_sprintf(cmd, "logging host %s port %u severity %u facility local%u\n",
                          target.host, target.port, target.severity, target.facility - 16);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_log_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 interval = 0;
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_log_enable_get(&enable));

    if (DFLT_LOG_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%slogging\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_log_ratelimitEnable_get(&enable));
    SYS_ERR_CHK(sal_log_ratelimitInterval_get(&interval));

    if (DFLT_LOG_AGGREGATION_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%slogging aggregation\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (DFLT_LOG_AGGREGATION_INTERVAL != interval)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "logging aggregation aging-time %u\n", interval);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(_cmd_log_target_save(fp));

    return SYS_ERR_OK;
}

