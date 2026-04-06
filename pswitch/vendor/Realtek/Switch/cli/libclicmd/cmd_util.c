
#include <sys/sysinfo.h>
#include <libcmd/cmd.h>

uint32  g_isCliFwProcess = FALSE;
char pidFile[SYS_BUF32_LEN];

int32 cmd_util_install(
    uint32       nodemask,
    uint32       attrmask,
    cmd_set_func cmdset,
    char         *cmdstr,
    char         *cmddesc,
    uint32       isConfig,
    cmd_ext_hidmatch_func hidmatch)
{
    uint32 node = 0;
    uint32 nodemask_tmp;
    uint32 privmask_tmp;
    int32 priv;
    struct cmd_element *cmd = NULL;

    cmd = malloc(sizeof(struct cmd_element));

    if (NULL == cmd)
        return SYS_ERR_FAILED;

    osal_memset(cmd, 0, sizeof(struct cmd_element));

    cmd->string = cmdstr;
    cmd->func = cmdset;
    cmd->doc = cmddesc;

    if (ATTR_COND_HIDDEN & (attrmask))
    {
        cmd->attr = CMD_ATTR_CONDITION_HIDDEN;
        cmd->hidmatch = hidmatch;
    }

    nodemask_tmp = nodemask;

    if (TRUE == isConfig)
        node = CONFIG_NODE;

    for (; node < END_NODE; node++)
    {
        if (SYS_SHIFT_BIT(node) & (nodemask_tmp))
        {
            privmask_tmp = attrmask & 0xFFFF;

            for (priv = CMD_ATTR_PRIV_15; priv >= 0; priv--)
            {
                if (SYS_SHIFT_BIT(priv) & (privmask_tmp))
                {
                    INSTALL_ELEMENT(node, priv, cmd);

                    privmask_tmp = ~(SYS_SHIFT_BIT(priv)) & privmask_tmp;

                    if (0 == privmask_tmp)
                        break;
                }
            }

            nodemask_tmp = ~(SYS_SHIFT_BIT(node)) & nodemask_tmp;

            if (0 == nodemask_tmp)
                break;
        }
    }

    free(cmd);

    return SYS_ERR_OK;
}

uint32 cmd_util_parserInstall_check(uint32 cmdidx)
{
    return TRUE;
}

int32 cmd_util_nosortCmd_install(
    uint32       nodemask,
    uint32       attrmask,
    cmd_set_func cmdset,
    char         *cmdstr,
    char         *cmddesc,
    uint32       isConfig,
    uint32       nosortidx)
{
    uint32 node = 0;
    uint32 nodemask_tmp;
    uint32 privmask_tmp;
    int32 priv;
    struct cmd_element *cmd = NULL;

    cmd = malloc(sizeof(struct cmd_element));

    if (NULL == cmd)
        return SYS_ERR_FAILED;

    osal_memset(cmd, 0, sizeof(struct cmd_element));

    cmd->string = cmdstr;
    cmd->func = cmdset;
    cmd->doc = cmddesc;

    nodemask_tmp = nodemask;

    if (TRUE == isConfig)
        node = CONFIG_NODE;

    for (; node < END_NODE; node++)
    {
        if (SYS_SHIFT_BIT(node) & (nodemask_tmp))
        {
            privmask_tmp = attrmask & 0xFFFF;

            for (priv = CMD_ATTR_PRIV_15; priv >= 0; priv--)
            {
                if (SYS_SHIFT_BIT(priv) & (privmask_tmp))
                {
                    INSTALL_NOSORT_ELEMENT(node, priv, nosortidx, cmd);

                    privmask_tmp = ~(SYS_SHIFT_BIT(priv)) & privmask_tmp;

                    if (0 == privmask_tmp)
                        break;
                }
            }

            nodemask_tmp = ~(SYS_SHIFT_BIT(node)) & nodemask_tmp;

            if (0 == nodemask_tmp)
                break;
        }
    }

    free(cmd);

    return SYS_ERR_OK;
}

int32 cmd_util_portlist_get(
    int                  argc,
    char                 *argv[],
    int                  token,
    sys_logic_portmask_t *pLogicPortmask)
{
    if ('a' == argv[token][0]) 
        LOGIC_PORTMASK_SET_ALL(*pLogicPortmask);
    else
    {
        if (SYS_ERR_OK != STR2LPM(argv[token], pLogicPortmask))
            return SYS_ERR_PORT_ID;
    }

    return SYS_ERR_OK;
}

int32 cmd_util_normalPortlist_get(
    int                  argc,
    char                 *argv[],
    int                  token,
    sys_logic_portmask_t *pLogicPortmask)
{
    if ('a' == argv[token][0]) 
        NORMAL_LOGIC_PORTMASK_SET_ALL(*pLogicPortmask);
    else
        SYS_ERR_CHK(STR2LPM(argv[token], pLogicPortmask));

    return SYS_ERR_OK;
}

int32 cmd_util_seqTokAry_get(
    char   *tokStr,
    uint32 maxTokSize,
    uint32 maxTokStrLen,
    char   **pTokAry)
{
    char *ptr;
    char *tmp;
    int32 len = 0;
    uint32 index = 0;
    uint32 tokCnt = 0;
    char buf[maxTokSize][maxTokStrLen];

    ptr = tokStr;
    index = 0;

    osal_memset(buf, 0, (sizeof(char) * maxTokSize * maxTokStrLen));

    while (0 != osal_strlen(ptr))
    {
        if (index >= maxTokSize)
            return SYS_ERR_CMD_SEQUENCE_TOKEN;

        tmp = ptr;
        while (!((*tmp == ' ') || (*tmp == '\0')))
        {
            
            tmp++;
        }

        if ((tmp - ptr) > maxTokStrLen)
            return SYS_ERR_CMD_SEQUENCE_TOKEN;

        len = 0;
        SYS_ERR_CHK(sys_util_getnext(ptr, ' ', buf[index++], &len));
        tokCnt++;

        ptr += len;

        if (*ptr == '\n' || *ptr == '\0')
            break;

        if (index >= maxTokSize)
            return SYS_ERR_CMD_SEQUENCE_TOKEN;
    }

    if (0 == tokCnt)
        return SYS_ERR_CMD_SEQUENCE_TOKEN;

    osal_memcpy(pTokAry, buf, (sizeof(char) * maxTokSize * maxTokStrLen));

    return SYS_ERR_OK;
}

int32 cmd_util_rangeValue_get(char *rangeStr, uint32 *pMin, uint32 *pMax)
{
    char *p = NULL;
    char buf[SYS_BUF16_LEN];

    p = osal_strchr(rangeStr, '-');

    if (NULL == p)
    {
        *pMin = SYS_STR2UINT(rangeStr);
        *pMax = *pMin;

        return SYS_ERR_OK;
    }

    osal_memset(buf, 0, sizeof(buf));
    osal_strncpy(buf, rangeStr, (p - rangeStr));

    *pMin = SYS_STR2UINT(buf);

    *pMax = SYS_STR2UINT(p + 1);

    if (*pMin > *pMax)
        return SYS_ERR_CMD_RANGE;

    return SYS_ERR_OK;
}

void cmd_util_progressPrint_start(void)
{
    char cmd[SYS_BUF128_LEN];

    SYS_MEM_CLEAR(cmd);
    SYS_MEM_CLEAR(pidFile);

    osal_sprintf(pidFile, "%s%d", PROGRESS_PID_FILE_PREFIX, getpid());

    if (SYS_ERR_OK != sal_util_file_exist(pidFile))
    {
        osal_sprintf(cmd, "handler -f print_cli_progress %s &", pidFile);
        system(cmd);
    }
}

void cmd_util_progressPrint_stop(void)
{
    uint32 i = 0;
    FILE *fp = NULL;
    int32 pid = -1;

    for (i = 0; i < 3; i++)
    {
        if (NULL != (fp = fopen(pidFile, "r")))
            break;

        sleep(1);
    }

    if (NULL == fp)
        return;

    fscanf(fp, "%d", &pid);

    if (-1 != pid)
        kill(pid, SIGKILL);

    unlink(pidFile);

    SYS_MEM_CLEAR(pidFile);

    fprintf(stderr, "\n");

    fclose(fp);

    return;
}

#define CFG_FILE_HDR_LINE(hdr, name, fmt, args...)    \
do {                                                  \
    char __line[256];                                 \
    osal_memset(__line, 0, sizeof(__line));           \
    osal_sprintf(__line, "! %s: "fmt"\n", name, ## args); \
    osal_strcat(hdr, __line);                         \
} while (0)

int32 cmd_util_cfgFileHdr_print(char *hdr)
{
    char boardName[SYS_BOARD_NAME_LEN];
    char sysName[CAPA_SYS_NAME_LEN+1];
    int32 updays;
    int32 uphrs;
    int32 upmins;
    int32 upsecs;
    struct sysinfo info;
    struct tm *current_time;
    time_t current_secs;

    osal_memset(boardName, 0, sizeof(boardName));

    sal_sys_bdinfoVar_get("modelname", boardName);

    sal_sys_systemName_get(sysName);

    time(&current_secs);
    current_time = localtime(&current_secs);
    sysinfo(&info);
    updays = (int) info.uptime / (60*60*24);
    upmins = (int) info.uptime / 60;
    uphrs = (upmins / 60) %24;
    upmins %= 60;
    upsecs = current_time->tm_sec;

    if (osal_strlen(DFLT_SYS_CFG_HEAD_START) > 0)
    {
        osal_strcat(hdr, DFLT_SYS_CFG_HEAD_START);
        osal_strcat(hdr, "\n");
    }
    CFG_FILE_HDR_LINE(hdr, "System Description", "%s %s Switch", DFLT_SYS_PLATFORM_NAME, boardName);
    CFG_FILE_HDR_LINE(hdr, "System Version", "v%s", DFLT_SYS_VER);
    CFG_FILE_HDR_LINE(hdr, "System Name", "%s", sysName);
    CFG_FILE_HDR_LINE(hdr, "System Up Time", "%d days, %d hours, %d mins, %d secs", updays, uphrs, upmins, upsecs);

    if (osal_strlen(DFLT_SYS_CFG_HEAD_END) > 0)
    {
        osal_strcat(hdr, DFLT_SYS_CFG_HEAD_END);
        osal_strcat(hdr, "\n");
    }

    return SYS_ERR_OK;
}

