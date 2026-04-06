
#include <libsal/sal_radius.h>
#include <libcmd/cmd.h>

int32 cmd_radius_dfltConfig_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_radius_t radiusSrv;

    osal_memset(&radiusSrv, 0, sizeof(radiusSrv));

    SYS_ERR_CHK(sal_radiusSrv_dftParam_get(&radiusSrv));

    XMORE(" Retries| Timeout|   Key   \n");
    XMORE("--------+--------+---------\n");

    XMORE("    %2d  |", radiusSrv.retransmit);
    XMORE("    %2d  |", radiusSrv.timeout);
    XMORE(" %6s  \n", radiusSrv.key);

    return SYS_ERR_OK;
}

int32 cmd_radius_server_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_radius_t radiusSrv;
    char ipStr[16];
    uint32 i;

    XMORE(" Prio |   IP Address    | Auth-Port| Retries| Timeout|    Type   |   Key   \n");
    XMORE("------+-----------------+----------+--------+--------+-----------+---------\n");
    for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
    {
        osal_memset(&radiusSrv, 0, sizeof(radiusSrv));
        SYS_ERR_CHK(sal_radiusSrv_get(i, &radiusSrv));
        if (RADIUS_NOT_UESD_FLAG == radiusSrv.priority)
            continue;

        SYS_ERR_CHK(sys_util_ip2Str(radiusSrv.srvIp, ipStr));
        XMORE("%5d |", radiusSrv.priority);
        if (0 == osal_strcmp(radiusSrv.hostName, ""))
            XMORE("%16s |", ipStr);
        else
            XMORE("%16s |", radiusSrv.hostName);
        XMORE(" %6d   |", radiusSrv.srvPort);
        XMORE("   %2d   |", radiusSrv.retransmit);
        XMORE("   %2d   |", radiusSrv.timeout);
        if (SYS_RADIUS_SRV_USGTYPE_LOGIN == radiusSrv.usgType)
            XMORE("    Login  |");
        else if (SYS_RADIUS_SRV_USGTYPE_8021X == radiusSrv.usgType)
            XMORE("   802.1x  |");
        else if (SYS_RADIUS_SRV_USGTYPE_ALL == radiusSrv.usgType)
            XMORE("     All   |");
        XMORE(" %6s  \n", radiusSrv.key);

    }

    return SYS_ERR_OK;
}

int32 cmd_radiusSrv_dftParam_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_radius_t radiusSrv;

    SYS_MEM_CLEAR(radiusSrv);

    SYS_ERR_CHK(sal_radiusSrv_dftParam_get(&radiusSrv));

    for (i = 0; i < argc; i++)
    {
        if ('r' == CMD_TOKENSTR_GET(cmd->vline, 2 + i*2)[0])
            radiusSrv.retransmit = SYS_STR2UINT(argv[i]);
        else if ('t' == CMD_TOKENSTR_GET(cmd->vline, 2 + i*2)[0])
            radiusSrv.timeout = SYS_STR2UINT(argv[i]);
        else if ('k' == CMD_TOKENSTR_GET(cmd->vline, 2 + i*2)[0])
        {
            if (CMD_IS_PARSER_RUN())
            {
                sal_util_str_decrypt(argv[i], radiusSrv.key);
            }
            else
            {
                if (osal_strlen(argv[i]) > CAPA_AAA_KEY_LEN)
                    return SYS_ERR_STR_LEN_EXCEED;

                if (SYS_ERR_OK != STRING_VALIDATE(argv[i], FALSE))
                    return SYS_ERR_STRING;

                osal_strcpy(radiusSrv.key, argv[i]);
            }
        }
    }

    SYS_ERR_CHK(sal_radiusSrv_dftParam_set(radiusSrv));

    return SYS_ERR_OK;
}

int32 cmd_radiusSrv_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    uint32 srvIp = 0;
    sys_radius_t radiusSrv;

    if (CMD_IS_NO_FORM_EXIST())
    {
        uint32 i;
        sys_radius_t radiusSrv;

        if (0 == argc)
        {
            for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
            {
                SYS_ERR_CHK(sal_radiusSrv_del(i));
            }

            return SYS_ERR_OK;
        }

        for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
        {
            osal_memset(&radiusSrv, 0, sizeof(radiusSrv));
            SYS_ERR_CHK(sal_radiusSrv_get(i, &radiusSrv));
            if (RADIUS_NOT_UESD_FLAG == radiusSrv.priority)
                continue;

            if (0 == osal_strcmp(radiusSrv.hostName, argv[0]))
            {
                SYS_ERR_CHK(sal_radiusSrv_del(i));

                return SYS_ERR_OK;
            }
        }

        return SYS_ERR_OK;
    }

    if (CMD_IS_CLI_RUN())
    {
        if (IS_IPV4_ADDRSTR_VALID(argv[0]))
        {
            if (IS_IPV4_UADDRSTR_INVALID(argv[0]))
            {
                XMORE("Invalid RADIUS server address\n");

                return SYS_ERR_OK;
            }
        }
        else if (IS_IPV6_ADDRSTR_VALID(argv[0]))
        {
            if (IS_IPV6_UADDRSTR_INVALID(argv[0]))
            {
                XMORE("Invalid RADIUS server address\n");

                return SYS_ERR_OK;
            }
        }
        else
        {
            if (sal_util_getHostByName(&srvIp, argv[0]))
                SYS_ERR_CHK(SYS_ERR_DNS_RESOLVE);
        }
    }

    osal_memset(&radiusSrv, 0, sizeof(radiusSrv));

    SYS_ERR_CHK(sal_radiusSrv_dftParam_get(&radiusSrv));

    osal_strcpy(radiusSrv.hostName, argv[0]);

    radiusSrv.srvIp = srvIp;
    radiusSrv.srvPort = DFLT_RADIUS_AUTHPORT; 
    radiusSrv.acctPort = DFLT_RADIUS_ACCTPORT;
    radiusSrv.priority = SYS_RADIUS_SRV_PRIO_AUTO_SEL;
    radiusSrv.usgType = SYS_RADIUS_SRV_USGTYPE_ALL;
    radiusSrv.keyUseDft = TRUE;
    radiusSrv.timeoutUseDft = TRUE;
    radiusSrv.retransmitUseDft = TRUE;

    for (i = 0; i < argc - 1; i++)
    {
        if ('u' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[1])
        {
            radiusSrv.srvPort = SYS_STR2UINT(argv[1 + i]);
        }
        else if ('c' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[1])
        {
            radiusSrv.acctPort = SYS_STR2UINT(argv[1 + i]);
        }
        else if ('k' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[0])
        {
            if (CMD_IS_PARSER_RUN())
            {
                sal_util_str_decrypt(argv[1 + i], radiusSrv.key);
            }
            else
            {
                if (osal_strlen(argv[1+i]) > CAPA_AAA_KEY_LEN)
                    return SYS_ERR_STR_LEN_EXCEED;

                if (SYS_ERR_OK != STRING_VALIDATE(argv[1+i], FALSE))
                    return SYS_ERR_STRING;

                osal_strcpy(radiusSrv.key, argv[1 + i]);
            }
            radiusSrv.keyUseDft = FALSE;
        }
        else if ('p' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[0])
            radiusSrv.priority = SYS_STR2UINT(argv[1 + i]);
        else if ('r' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[0])
        {
            radiusSrv.retransmit = SYS_STR2UINT(argv[1 + i]);
            radiusSrv.retransmitUseDft = FALSE;
        }
        else if ('i' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[1])
        {
            radiusSrv.timeout = SYS_STR2UINT(argv[1 + i]);
            radiusSrv.timeoutUseDft = FALSE;
        }
        else if ('y' == CMD_TOKENSTR_GET(cmd->vline, 3 + i*2)[1])
        {
            if ('l' == argv[1 + i][0])
                radiusSrv.usgType = SYS_RADIUS_SRV_USGTYPE_LOGIN;
            else if ('8' == argv[1 + i][0])
                radiusSrv.usgType = SYS_RADIUS_SRV_USGTYPE_8021X;
            else if ('a' == argv[1 + i][0])
                radiusSrv.usgType = SYS_RADIUS_SRV_USGTYPE_ALL;
        }
    }

    SYS_ERR_CHK(sal_radiusSrv_set(-1, radiusSrv));

    return SYS_ERR_OK;
}

static int32 _cmd_radius_dftParam_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char keyStr[CAPA_AAA_KEY_LEN + 8];
    sys_radius_t radiusSrv;

    SYS_MEM_CLEAR(radiusSrv);

    SYS_ERR_CHK(sal_radiusSrv_dftParam_get(&radiusSrv));

    if ((0 != osal_strlen(radiusSrv.key))
            || (radiusSrv.retransmit != DFLT_RADIUS_RETRY)
            || (radiusSrv.timeout != DFLT_RADIUS_TIMEOUT))
    {
        SYS_MEM_CLEAR(cmd);

        if (0 == osal_strlen(radiusSrv.key))
        {
            osal_sprintf(cmd, "radius default-config retransmit %d timeout %d\n",
                          radiusSrv.retransmit,
                          radiusSrv.timeout);
        }
        else
        {
            SYS_MEM_CLEAR(keyStr);

            sal_util_str_encrypt(radiusSrv.key, keyStr);

            osal_sprintf(cmd, "radius default-config key %s retransmit %d timeout %d\n",
                          keyStr,
                          radiusSrv.retransmit,
                          radiusSrv.timeout);
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

static int32 _cmd_radiusSrv_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char buf[8], keyStr[CAPA_AAA_KEY_LEN + 8], timeoutStr[20], retransmitStr[20];
    char strBuf[CAPA_AAA_KEY_LEN + 8];
    char strIp[CAPA_IPV4_STR_LEN];
    uint32 i;
    sys_radius_t radiusSrv;

    for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
    {
        osal_memset(&radiusSrv, 0, sizeof(radiusSrv));

        SYS_ERR_CHK(sal_radiusSrv_get(i, &radiusSrv));

        if (RADIUS_NOT_UESD_FLAG == radiusSrv.priority)
            continue;

        SYS_ERR_CONTINUE(sys_util_ip2Str(radiusSrv.srvIp, strIp));

        osal_memset(cmd, 0, sizeof(cmd));
        if (SYS_RADIUS_SRV_USGTYPE_LOGIN == radiusSrv.usgType)
            osal_strcpy(buf, "login");
        else if (SYS_RADIUS_SRV_USGTYPE_8021X == radiusSrv.usgType)
            osal_strcpy(buf, "802.1x");
        else if (SYS_RADIUS_SRV_USGTYPE_ALL == radiusSrv.usgType)
            osal_strcpy(buf, "all");

        if (radiusSrv.keyUseDft)
        {
            osal_strcpy(keyStr, "");
        }
        else
        {
            SYS_MEM_CLEAR(strBuf);

            sal_util_str_encrypt(radiusSrv.key, strBuf);
            osal_sprintf(keyStr, "key %s", strBuf);
        }

        if (radiusSrv.timeoutUseDft)
            osal_strcpy(timeoutStr, "");
        else
            osal_sprintf(timeoutStr, "timeout %d", radiusSrv.timeout);

        if (radiusSrv.retransmitUseDft)
            osal_strcpy(retransmitStr, "");
        else
            osal_sprintf(retransmitStr, "retransmit %d", radiusSrv.retransmit);

        if (0 == osal_strcmp(radiusSrv.hostName, ""))
        {
            osal_sprintf(cmd, "radius host %s auth-port %d %s priority %d %s %s type %s\n",
                          strIp,
                          radiusSrv.srvPort,
                          keyStr,
                          radiusSrv.priority,
                          retransmitStr,
                          timeoutStr,
                          buf);
        }
        else
        {
            osal_sprintf(cmd, "radius host %s auth-port %d %s priority %d %s %s type %s\n",
                          radiusSrv.hostName,
                          radiusSrv.srvPort,
                          keyStr,
                          radiusSrv.priority,
                          retransmitStr,
                          timeoutStr,
                          buf);
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_radius_save(FILE *fp)
{
    
    SYS_ERR_CHK(_cmd_radius_dftParam_save(fp));

    SYS_ERR_CHK(_cmd_radiusSrv_save(fp));

    return SYS_ERR_OK;
}

