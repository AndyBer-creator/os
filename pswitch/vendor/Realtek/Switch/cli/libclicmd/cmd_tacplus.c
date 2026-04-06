
#include <dirent.h>

#include <libsal/sal_tacplus.h>
#include <libsal/sal_sys.h>
#include <libcmd/cmd.h>

int32 cmd_tacplus_dfltParam_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_tacplus_t tacplusSrv;

    osal_memset(&tacplusSrv, 0, sizeof(tacplusSrv));
    SYS_ERR_CHK(sal_tacplus_dftParam_get(&tacplusSrv));

    XMORE(" Timeout|   Key   \n");
    XMORE("--------+---------\n");

    XMORE("  %4d  |", tacplusSrv.timeout);
    XMORE(" %6s \n", tacplusSrv.key);

    return SYS_ERR_OK;
}

int32 cmd_tacplus_server_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_tacplus_t tacplusSrv;
    uint32 i;

    XMORE(" Prio | Timeout |     IP Address    |  Port  |   Key  \n");
    XMORE("------+---------+-------------------+--------+----------\n");
    for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
    {
        osal_memset(&tacplusSrv, 0, sizeof(tacplusSrv));
        SYS_ERR_CHK(sal_tacplus_server_get(i, &tacplusSrv));
        if (tacplusSrv.priority == TACPLUS_NOT_UESD_FLAG)
            continue;

        XMORE("%5d |", tacplusSrv.priority);
        XMORE("   %4d  |", tacplusSrv.timeout);
        XMORE("  %16s |", tacplusSrv.srvHost);
        XMORE(" %6d |", tacplusSrv.srvPort);
        XMORE(" %6s \n", tacplusSrv.key);
    }

    return SYS_ERR_OK;
}

int32 cmd_tacplus_dftParam_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32              i;
    sys_tacplus_t   tacplusSrv;

    SYS_MEM_CLEAR(tacplusSrv);
    SYS_ERR_CHK(sal_tacplus_dftParam_get(&tacplusSrv));

    for (i = 0; i < argc; i++)
    {
        if ('t' == CMD_TOKENSTR_GET(cmd->vline, 2 + i*2)[0])
        {
            tacplusSrv.timeout = SYS_STR2UINT(argv[i]);
        }
        else if ('k' == CMD_TOKENSTR_GET(cmd->vline, 2 + i*2)[0])
        {
            if (CMD_IS_PARSER_RUN())
            {
                sal_util_str_decrypt(argv[i], tacplusSrv.key);
            }
            else
            {
                if (osal_strlen(argv[i]) > CAPA_AAA_KEY_LEN)
                    return SYS_ERR_STR_LEN_EXCEED;

                if (SYS_ERR_OK != STRING_VALIDATE(argv[i], FALSE))
                    return SYS_ERR_STRING;

                osal_strcpy(tacplusSrv.key, argv[i]);
            }
        }
    }

    SYS_ERR_CHK(sal_tacplus_dftParam_set(tacplusSrv));

    return SYS_ERR_OK;
}

int32 cmd_tacplus_server_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_tacplus_t tacplusSrv;

    if (CMD_IS_NO_FORM_EXIST())
    {
        uint32 i = 0;
        sys_tacplus_t tacplusSrv;

        if (0 == argc)
        {
            for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
            {
                SYS_ERR_CHK(sal_tacplus_server_del(i));
            }

            return SYS_ERR_OK;
        }

        for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
        {
            osal_memset(&tacplusSrv, 0, sizeof(tacplusSrv));

            SYS_ERR_CHK(sal_tacplus_server_get(i, &tacplusSrv));

            if (TACPLUS_NOT_UESD_FLAG == tacplusSrv.priority)
                continue;

            if (0 == osal_strcmp(tacplusSrv.srvHost, argv[0]))
            {
                SYS_ERR_CHK(sal_tacplus_server_del(i));
            }
        }

        return SYS_ERR_OK;
    }

    if (CMD_IS_CLI_RUN())
    {
        uint32 srvIp = 0;

        if (IS_IPV4_ADDRSTR_VALID(argv[0]))
        {
            if (IS_IPV4_UADDRSTR_INVALID(argv[0]))
            {
                XMORE("Invalid TACACS+ server address\n");

                return SYS_ERR_OK;
            }
        }
        else if (IS_IPV6_ADDRSTR_VALID(argv[0]))
        {
            if (IS_IPV6_UADDRSTR_INVALID(argv[0]))
            {
                XMORE("Invalid TACACS+ server address\n");

                return SYS_ERR_OK;
            }
        }
        else
        {
            if (sal_util_getHostByName(&srvIp, argv[0]))
                SYS_ERR_CHK(SYS_ERR_DNS_RESOLVE);
        }
    }

    osal_memset(&tacplusSrv, 0, sizeof(tacplusSrv));

    SYS_ERR_CHK(sal_tacplus_dftParam_get(&tacplusSrv));

    osal_strcpy(tacplusSrv.srvHost, argv[0]);

    tacplusSrv.srvPort = DFLT_TACPLUS_AUTHPORT; 
    tacplusSrv.priority = SYS_AAA_TACPLUS_SRV_PRIO_AUTO_SEL;
    tacplusSrv.keyUseDft = TRUE;
    tacplusSrv.timeoutUseDft = TRUE;

    for (i = 0; i < argc - 1; i++)
    {
        if ('a' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[0])
        {
            tacplusSrv.srvPort = SYS_STR2UINT(argv[1 + i]);
        }
        else if ('k' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[0])
        {
            if (CMD_IS_PARSER_RUN())
            {
                sal_util_str_decrypt(argv[1 + i], tacplusSrv.key);
            }
            else
            {
                if (osal_strlen(argv[1 + i]) > CAPA_AAA_KEY_LEN)
                    return SYS_ERR_STR_LEN_EXCEED;

                if (SYS_ERR_OK != STRING_VALIDATE(argv[1 + i], FALSE))
                    return SYS_ERR_STRING;

                osal_strcpy(tacplusSrv.key, argv[1 + i]);
            }
            tacplusSrv.keyUseDft = FALSE;
        }
        else if ('p' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[0])
        {
            if ('o' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[1])
                tacplusSrv.srvPort = SYS_STR2UINT(argv[1 + i]);
            else
                tacplusSrv.priority = SYS_STR2UINT(argv[1 + i]);
        }
        else if ('t' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[0])
        {
            tacplusSrv.timeout = SYS_STR2UINT(argv[1 + i]);
            tacplusSrv.timeoutUseDft = FALSE;
        }
        else if ('s' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[0])
        {
            if ('e' == argv[1 + i][0])
                tacplusSrv.singleConnection = TRUE;
            else if ('d' == argv[1 + i][0])
                tacplusSrv.singleConnection = FALSE;
        }
    }

    SYS_ERR_CHK(sal_tacplus_server_set(-1, tacplusSrv));

    return SYS_ERR_OK;
}

int32 cmd_tacplus_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char keyStr[CAPA_AAA_KEY_LEN + 8], timeoutStr[20];
    char strBuf[CAPA_AAA_KEY_LEN + 8];
    uint32 i;
    sys_tacplus_t tacplusSrv;

    SYS_MEM_CLEAR(tacplusSrv);
    SYS_ERR_CHK(sal_tacplus_dftParam_get(&tacplusSrv));

    if (0 != osal_strlen(tacplusSrv.key) || DFLT_TACPLUS_TIMEOUT != tacplusSrv.timeout)
    {
        SYS_MEM_CLEAR(cmd);

        if (0 == osal_strlen(tacplusSrv.key))
        {
            osal_sprintf(cmd, "tacacs default-config timeout %d\n", tacplusSrv.timeout);
        }
        else
        {
            SYS_MEM_CLEAR(keyStr);

            sal_util_str_encrypt(tacplusSrv.key, keyStr);

            if (DFLT_TACPLUS_TIMEOUT == tacplusSrv.timeout)
                osal_sprintf(cmd, "tacacs default-config key %s\n", keyStr);
            else
                osal_sprintf(cmd, "tacacs default-config key %s timeout %d\n", keyStr, tacplusSrv.timeout);
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
    {
        osal_memset(&tacplusSrv, 0, sizeof(tacplusSrv));

        SYS_ERR_CHK(sal_tacplus_server_get(i, &tacplusSrv));

        if (TACPLUS_NOT_UESD_FLAG == tacplusSrv.priority)
            continue;

        osal_memset(cmd, 0, sizeof(cmd));

        if (tacplusSrv.keyUseDft)
        {
            osal_strcpy(keyStr, "");
        }
        else
        {
            SYS_MEM_CLEAR(strBuf);

            sal_util_str_encrypt(tacplusSrv.key, strBuf);
            osal_sprintf(keyStr, "key %s", strBuf);
        }

        if (tacplusSrv.timeoutUseDft)
            osal_strcpy(timeoutStr, "");
        else
            osal_sprintf(timeoutStr, "timeout %d", tacplusSrv.timeout);

        osal_sprintf(cmd, "tacacs host %s port %d %s priority %d %s\n",
                      tacplusSrv.srvHost,
                      tacplusSrv.srvPort,
                      keyStr,
                      tacplusSrv.priority,
                      timeoutStr);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}
