
#include <dirent.h>

#include <libsal/sal_aaa.h>
#include <libsal/sal_sys.h>
#include <libcmd/cmd.h>

int32 cmd_aaa_authenList_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i, j;
    char            authListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    sys_auth_type_t authTypePrio[CAPA_AAA_AUTHTYPE_PRI_NUM];
    sys_authList_type_t authListType = SYS_AUTHLIST_TYPE_NONE;
    sys_authList_type_t selAuthListType = SYS_AUTHLIST_TYPE_NONE;

    if (argv[0][0] == 'l')
    {
        selAuthListType = SYS_AUTHLIST_TYPE_AUTHEN_LOGIN;
        XMORE(" Login List Name    Authentication Method List\n");
    }
    else
    {
        selAuthListType = SYS_AUTHLIST_TYPE_AUTHEN_ENABLE;
        XMORE(" Enable List Name   Authentication Method List\n");
    }

    XMORE("------------------ -------------------------------\n");
    for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
    {
        osal_memset(authListName, 0, sizeof(authListName));
        osal_memset(authTypePrio, 0, sizeof(authTypePrio));
        SYS_ERR_CHK(sal_aaa_authList_getByIdx(i, authListName, authTypePrio, &authListType));
        if (0 == osal_strcmp(authListName, "")
            || (authListType != selAuthListType))
            continue;
        XMORE(" %16s  ", authListName);
        for(j = 0; j < CAPA_AAA_AUTHTYPE_PRI_NUM; j++)
        {
            if (SYS_AUTH_TYPE_NONE == authTypePrio[j])
                XMORE(" %s ", SYS_AAA_AUTH_METHOD_STR_NONE);
            else if (SYS_AUTH_TYPE_LOCAL == authTypePrio[j])
                XMORE(" %s ", SYS_AAA_AUTH_METHOD_STR_LOCAL);
            else if (SYS_AUTH_TYPE_ENABLE == authTypePrio[j])
                XMORE(" %s ", SYS_AAA_AUTH_METHOD_STR_ENABLE);
#ifdef CONFIG_SYS_LIB_RADIUS
            else if (SYS_AUTH_TYPE_RADIUS == authTypePrio[j])
                XMORE(" %s ", SYS_AAA_AUTH_METHOD_STR_RADIUS);
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
            else if (SYS_AUTH_TYPE_TACPLUS == authTypePrio[j])
                XMORE(" %s ", SYS_AAA_AUTH_METHOD_STR_TACPLUS);
#endif
        }
        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_aaa_lineLists_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    uint32 j;
    char authListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];

    XMORE("  Line Type  |    AAA Type     |  List Name\n");
    XMORE("-------------+-----------------+-----------------\n");

    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        switch (i)
        {
            case SYS_ACCESS_CLI_CONSOLE:
                XMORE(" %11s |", "console");
                break;

            case SYS_ACCESS_CLI_TELNET:
                XMORE(" %11s |", "telnet");
                break;

            case SYS_ACCESS_CLI_SSH:
                XMORE(" %11s |", "ssh");
                break;

            case SYS_ACCESS_WEB_HTTP:
                XMORE(" %11s |", "http");
                break;

            case SYS_ACCESS_WEB_HTTPS:
                XMORE(" %11s |", "https");
                break;

            default:
                break;
        }

        for (j = SYS_AUTHLIST_TYPE_AUTHEN_LOGIN; j < SYS_AUTHLIST_TYPE_END; j++)
        {
            if ((i == SYS_ACCESS_WEB_HTTP || i == SYS_ACCESS_WEB_HTTPS) && j != SYS_AUTHLIST_TYPE_AUTHEN_LOGIN)
                continue;

            if (j != SYS_AUTHLIST_TYPE_AUTHEN_LOGIN)
                XMORE("             |");
            switch (j)
            {
                case SYS_AUTHLIST_TYPE_AUTHEN_LOGIN:
                    SYS_ERR_CHK(sal_aaa_loginAuthListName_get(i, authListName));
                    XMORE(" %15s |", "login");
                    XMORE(" %s\n", authListName);
                    break;

                case SYS_AUTHLIST_TYPE_AUTHEN_ENABLE:
                    SYS_ERR_CHK(sal_aaa_enableAuthListName_get(i, authListName));
                    XMORE(" %15s |", "enable");
                    XMORE(" %s\n", authListName);
                    break;

                default:
                    break;
            }
        }

    }

    return SYS_ERR_OK;
}

int32 cmd_aaa_authenLoginList_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    uint32 authIdx = 0;
    sys_auth_type_t authTypes[CAPA_AAA_AUTHTYPE_PRI_NUM];

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (0 == osal_strcmp(argv[0], SYS_AAA_DFLT_AUTHEN_LOGIN_LIST))
        {
            XMORE("'%s' is default list and cannot be deleted\n", SYS_AAA_DFLT_AUTHEN_LOGIN_LIST);
            return SYS_ERR_OK;
        }
        SYS_ERR_CHK(sal_aaa_authList_del(argv[0], SYS_AUTHLIST_TYPE_AUTHEN_LOGIN));

        return SYS_ERR_OK;
    }

    if (osal_strlen(argv[0]) > CAPA_AAA_AUTHLIST_NAME_LEN)
        return SYS_ERR_STR_LEN_EXCEED;

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], FALSE))
        return SYS_ERR_STRING;

    osal_memset(authTypes, 0, sizeof(authTypes));

    for (i = 1; i < argc; i++)
    {
        if ('l' == argv[i][0]) 
        {
            authTypes[authIdx] = SYS_AUTH_TYPE_LOCAL;
            authIdx++;
        }
        else if ('e' == argv[i][0]) 
        {
            authTypes[authIdx] = SYS_AUTH_TYPE_ENABLE;
            authIdx++;
        }
#ifdef CONFIG_SYS_LIB_TACPLUS
        else if ('t' == argv[i][0]) 
        {
            authTypes[authIdx] = SYS_AUTH_TYPE_TACPLUS;
            authIdx++;
        }
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
        else if ('r' == argv[i][0]) 
        {
            authTypes[authIdx] = SYS_AUTH_TYPE_RADIUS;
            authIdx++;
        }
#endif
        else if ('n' == argv[i][0]) 
        {
            
            authTypes[authIdx] = SYS_AUTH_TYPE_NONE;

            break;
        }
        else
        {
            XMORE("%s method is not supported in login authentication\n", argv[i]);

            return SYS_ERR_OK;
        }
    }

    SYS_ERR_CHK(sal_aaa_authList_set(argv[0], authTypes, SYS_AUTHLIST_TYPE_AUTHEN_LOGIN));

    return SYS_ERR_OK;
}

int32 cmd_aaa_authenEnableList_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_auth_type_t authTypes[CAPA_AAA_AUTHTYPE_PRI_NUM];
    uint32 authIdx = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (0 == osal_strcmp(argv[0], SYS_AAA_DFLT_AUTHEN_ENABLE_LIST))
        {
            XMORE("'%s' is default list and cannot be deleted\n", SYS_AAA_DFLT_AUTHEN_ENABLE_LIST);
            return SYS_ERR_OK;
        }
        SYS_ERR_CHK(sal_aaa_authList_del(argv[0], SYS_AUTHLIST_TYPE_AUTHEN_ENABLE));

        return SYS_ERR_OK;
    }

    osal_memset(authTypes, 0, sizeof(authTypes));

    for (i = 1; i < argc; i++)
    {
        if ('e' == argv[i][0]) 
        {
            authTypes[authIdx] = SYS_AUTH_TYPE_ENABLE;
            authIdx++;
        }
#ifdef CONFIG_SYS_LIB_TACPLUS
        else if ('t' == argv[i][0]) 
        {
            authTypes[authIdx] = SYS_AUTH_TYPE_TACPLUS;
            authIdx++;
        }
#endif
#ifdef CONFIG_SYS_LIB_RADIUS
        else if ('r' == argv[i][0]) 
        {
            authTypes[authIdx] = SYS_AUTH_TYPE_RADIUS;
            authIdx++;
        }
#endif
        else if ('n' == argv[i][0]) 
        {
            
            authTypes[authIdx] = SYS_AUTH_TYPE_NONE;
            break;
        }
        else
        {
            XMORE("%s method is not supported in enable authentication\n", argv[i]);

            return SYS_ERR_OK;
        }
    }

    SYS_ERR_CHK(sal_aaa_authList_set(argv[0], authTypes, SYS_AUTHLIST_TYPE_AUTHEN_ENABLE));

    return SYS_ERR_OK;
}

int32 cmd_aaa_lineLoginAuthenList_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 isIpHttp = FALSE;
    char listName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    sys_access_type_t type = SYS_ACCESS_CLI_CONSOLE;

    SYS_MEM_CLEAR(listName);

    if (CMD_IS_NO_FORM_EXIST())
    {
        if ('i' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
        {
            type = SYS_ACCESS_WEB_HTTP;

            if (5 == osal_strlen(argv[0]))
                type = SYS_ACCESS_WEB_HTTPS;

            isIpHttp = TRUE;
        }

        osal_strcpy(listName, DFLT_AAA_LOGIN_AUTHLIST_NAME);
    }
    else
    {
        if ('i' == CMD_TOKENSTR_GET(cmd->vline, 0)[0])
        {
            type = SYS_ACCESS_WEB_HTTP;

            if (5 == osal_strlen(argv[0]))
                type = SYS_ACCESS_WEB_HTTPS;

            if (osal_strlen(argv[1]) > CAPA_AAA_AUTHLIST_NAME_LEN)
                return SYS_ERR_STR_LEN_EXCEED;

            osal_strcpy(listName, argv[1]);

            isIpHttp = TRUE;
        }
        else
        {
            if (osal_strlen(argv[0]) > CAPA_AAA_AUTHLIST_NAME_LEN)
                return SYS_ERR_STR_LEN_EXCEED;

            osal_strcpy(listName, argv[0]);
        }
    }

    if (FALSE == isIpHttp)
    {
        switch (g_cliLineType)
        {
            case SYS_LINE_CLI_CONSOLE:
                type = SYS_ACCESS_CLI_CONSOLE;
                break;

            case SYS_LINE_CLI_TELNET:
                type = SYS_ACCESS_CLI_TELNET;
                break;

#ifdef CONFIG_USER_SSH_SSHD
            case SYS_LINE_CLI_SSH:
                type = SYS_ACCESS_CLI_SSH;
                break;
#endif

            default:
                break;
        }
    }

    SYS_ERR_CHK(sal_aaa_loginAuthList_set(type, listName));

    return SYS_ERR_OK;
}

int32 cmd_aaa_lineEnableAuthenList_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_access_type_t type = SYS_ACCESS_CLI_CONSOLE;

    switch (g_cliLineType)
    {
        case SYS_LINE_CLI_CONSOLE:
            type = SYS_ACCESS_CLI_CONSOLE;
            break;

        case SYS_LINE_CLI_TELNET:
            type = SYS_ACCESS_CLI_TELNET;
            break;

        case SYS_ACCESS_CLI_SSH:
            type = SYS_ACCESS_CLI_SSH;
            break;

        default:
            break;
    }

    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_aaa_enableAuthList_set(type, DFLT_AAA_ENABLE_AUTHLIST_NAME));
    else
        SYS_ERR_CHK(sal_aaa_enableAuthList_set(type, argv[0]));

    return SYS_ERR_OK;
}

static int32 _cmd_aaa_listMethod_save(char *cmd, sys_auth_type_t *pAuthPri)
{
    uint32 i;

    for (i = 0; i < CAPA_AAA_AUTHTYPE_PRI_NUM; i++)
    {
        if (SYS_AUTH_TYPE_EMPTY == pAuthPri[i])
            break;

        if (SYS_AUTH_TYPE_NONE == pAuthPri[i])
        {
            osal_strcat(cmd, SYS_AAA_AUTH_METHOD_STR_NONE);
            osal_strcat(cmd, " ");
        }
        else if (SYS_AUTH_TYPE_LOCAL == pAuthPri[i])
        {
            osal_strcat(cmd, SYS_AAA_AUTH_METHOD_STR_LOCAL);
            osal_strcat(cmd, " ");
        }
        else if (SYS_AUTH_TYPE_ENABLE == pAuthPri[i])
        {
            osal_strcat(cmd, SYS_AAA_AUTH_METHOD_STR_ENABLE);
            osal_strcat(cmd, " ");
        }
#ifdef CONFIG_SYS_LIB_RADIUS
        else if (SYS_AUTH_TYPE_RADIUS == pAuthPri[i])
        {
            osal_strcat(cmd, SYS_AAA_AUTH_METHOD_STR_RADIUS);
            osal_strcat(cmd, " ");
        }
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
        else if (SYS_AUTH_TYPE_TACPLUS == pAuthPri[i])
        {
            osal_strcat(cmd, SYS_AAA_AUTH_METHOD_STR_TACPLUS);
            osal_strcat(cmd, " ");
        }
#endif
    }

    return SYS_ERR_OK;
}

static int32 _cmd_aaa_authList_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char authListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    uint32 i;
    sys_auth_type_t authTypePrio[CAPA_AAA_AUTHTYPE_PRI_NUM];
    sys_authList_type_t authListType = SYS_AUTHLIST_TYPE_NONE;

    for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
    {
        SYS_MEM_CLEAR(authListName);
        SYS_MEM_CLEAR(authTypePrio);

        SYS_ERR_CHK(sal_aaa_authList_getByIdx(i, authListName, authTypePrio, &authListType));

        if (0 == osal_strlen(authListName))
            continue;

        if (SYS_AUTHLIST_TYPE_AUTHEN_LOGIN == authListType)
        {
            if ((0 != osal_strcmp(DFLT_AAA_LOGIN_AUTHLIST_NAME, authListName))
                    ||(authTypePrio[0] != DFLT_AAA_LOGIN_AUTH_TYPE)
                    || (authTypePrio[1] != SYS_AUTH_TYPE_EMPTY))
            {
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "aaa authentication login %s ", authListName);
                _cmd_aaa_listMethod_save(cmd, authTypePrio);
                osal_strcat(cmd, "\n");

                CMD_WRITE_TO_FILE(fp, cmd);
            }
        }
        else if (SYS_AUTHLIST_TYPE_AUTHEN_ENABLE == authListType)
        {
            if ((0 != osal_strcmp(DFLT_AAA_ENABLE_AUTHLIST_NAME, authListName))
                    || (authTypePrio[0] != DFLT_AAA_ENABLE_AUTH_TYPE)
                    || (authTypePrio[1] != SYS_AUTH_TYPE_EMPTY))
            {
                SYS_MEM_CLEAR(cmd);
                osal_sprintf(cmd, "aaa authentication enable %s ", authListName);
                _cmd_aaa_listMethod_save(cmd, authTypePrio);
                osal_strcat(cmd, "\n");

                CMD_WRITE_TO_FILE(fp, cmd);
            }
        }

    }

    return SYS_ERR_OK;
}

int32 cmd_aaa_save(FILE *fp)
{
    SYS_ERR_CHK(_cmd_aaa_authList_save(fp));

    return SYS_ERR_OK;
}

int32 cmd_aaa_line_save(FILE *fp, sys_line_cliType_t type)
{
    char cmd[MAX_CMD_STRLEN];
    char listName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];

    SYS_MEM_CLEAR(listName);
    SYS_ERR_CHK(sal_aaa_loginAuthListName_get(type, listName));

    if (0 != osal_strcmp(DFLT_AAA_LOGIN_AUTHLIST_NAME, listName))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "login authentication %s\n", listName);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_MEM_CLEAR(listName);
    SYS_ERR_CHK(sal_aaa_enableAuthListName_get(type, listName));

    if (0 != osal_strcmp(DFLT_AAA_ENABLE_AUTHLIST_NAME, listName))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "enable authentication %s\n", listName);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

