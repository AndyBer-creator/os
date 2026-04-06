
#include <libsal/sal_sys.h>
#include <libcmd/cmd.h>

int32 cmd_sys_username_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_userInfo_t userInfo;

    XMORE(" Priv  |  Type  |           User Name            |            Password\n");
    XMORE("-------+--------+--------------------------------+----------------------------------\n");
    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        osal_memset(&userInfo, 0, sizeof(userInfo));
        SYS_ERR_CHK(sal_sys_localUser_get(i, &userInfo));
        if (userInfo.priv < 0)
            continue;

#ifdef CONFIG_SYS_AAA
        XMORE("  %02d   |", userInfo.priv);
#else
        if (userInfo.priv == SYS_LOCALUSER_PRIV_ADMIN)
            XMORE(" %5s |", "admin");
        else
            XMORE(" %5s |", "user");
#endif
        switch(userInfo.passwdType)
        {
            case SYS_PASSWD_TYPE_NOPASSWD:
                XMORE(" nopass |");
                break;
            case SYS_PASSWD_TYPE_CLEARTEXT:
                XMORE("   text |");
                break;
            case SYS_PASSWD_TYPE_ENCRYPT:
                XMORE(" secret |");
                break;

            default:
                break;
        }

        XMORE(" %30s |", userInfo.username);
        XMORE(" %s\n", userInfo.password);

    }

    return SYS_ERR_OK;
}

int32 cmd_sys_privilege_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    XMORE("Current CLI Username:  %s\n", currUserName);
#ifdef CONFIG_SYS_AAA
    XMORE("Current CLI Privilege: %d\n", currpriv);
#else
    if (currpriv == SYS_LOCALUSER_PRIV_ADMIN)
        XMORE("Current CLI Privilege: Admin\n");
    else
        XMORE("Current CLI Privilege: User\n");
#endif 

    return SYS_ERR_OK;
}

int32 cmd_sys_username_access_method_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i;
    sys_userInfo_t userInfo;

    XMORE("   User Name                  access method\n");
    XMORE("-----------------------------------------------------\n");
    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        osal_memset(&userInfo, 0, sizeof(userInfo));
        SYS_ERR_CHK(sal_sys_localUser_get(i, &userInfo));
        if (userInfo.priv < 0)
            continue;

        XMORE(" %-20s |", userInfo.username);
        if(userInfo.accessMethod & 0x1 << SYS_ACCESS_CLI_CONSOLE)
        	XMORE(" %s ", "console");
        if(userInfo.accessMethod & 0x1 << SYS_ACCESS_CLI_TELNET)
        	XMORE(" %s ", "telnet");
        if(userInfo.accessMethod & 0x1 << SYS_ACCESS_CLI_SSH)
        	XMORE(" %s ", "ssh");
        if(userInfo.accessMethod & 0x1 << SYS_ACCESS_WEB_HTTP)
        	XMORE(" %s ", "http");
        if(userInfo.accessMethod & 0x1 << SYS_ACCESS_WEB_HTTPS)
        	XMORE(" %s ", "https");
        XMORE(" \n");
    }
	XMORE(" \n");

    return SYS_ERR_OK;
}

int32 cmd_sys_localUser_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 directCopy = FALSE;
    sys_userInfo_t userInfo;

    if(osal_strlen(argv[0]) > CAPA_SYS_USERNAME_LEN)
    {
        XMORE("username should not exceeds %d bytes!\n",CAPA_SYS_USERNAME_LEN);
        return SYS_ERR_OK;
    }

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
        return SYS_ERR_STRING;

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (0 == osal_strcmp((argv[0]), currUserName))
        {
	        XMORE("The current user name cannot be deleted!\n");
	        return SYS_ERR_OK;
		}
        SYS_ERR_CHK(sal_sys_localUser_del(argv[0]));

        return SYS_ERR_OK;
    }

    SYS_MEM_CLEAR(userInfo);
    osal_strcpy(userInfo.username, argv[0]);

    if (argc < 3)
    {
        userInfo.priv = SYS_LOCALUSER_PRIV_ADMIN;

        if ('p' == CMD_TOKENSTR_GET(cmd->vline, 2)[0])
        {
            userInfo.passwdType = SYS_PASSWD_TYPE_CLEARTEXT;

            if(osal_strlen(argv[1]) > CAPA_SYS_PASSWORD_LEN)
            {
                XMORE("password should not exceeds %d bytes!\n", CAPA_SYS_PASSWORD_LEN);
                return SYS_ERR_OK;
            }
            if(osal_strlen(argv[1]) < CAPA_SYS_PASSWORD_MIX_LEN)
            {
                XMORE("Password cannot be less than %d bytes!\n", CAPA_SYS_PASSWORD_MIX_LEN);
                return SYS_ERR_OK;
            }

            if (SYS_ERR_OK != STRING_VALIDATE(argv[1], TRUE))
                return SYS_ERR_STRING;
        }
        else if ('s' == CMD_TOKENSTR_GET(cmd->vline, 2)[0])
        {
            userInfo.passwdType = SYS_PASSWD_TYPE_ENCRYPT;

            if (5 == CMD_TOKENNUM_GET(cmd->vline))
            {
                directCopy = TRUE;

                if(osal_strlen(argv[1]) > CAPA_SYS_CRYPT_PASSWORD_LEN)
                {
                    XMORE("encrypted password should not exceeds %d bytes!\n", CAPA_SYS_CRYPT_PASSWORD_LEN);
                    return SYS_ERR_OK;
                }
                if(osal_strlen(argv[1]) < CAPA_SYS_PASSWORD_MIX_LEN)
	            {
	                XMORE("encrypted password cannot be less than %d bytes!\n", CAPA_SYS_PASSWORD_MIX_LEN);
	                return SYS_ERR_OK;
	            }
            }
            else
            {
                directCopy = FALSE;

                if(osal_strlen(argv[1]) > CAPA_SYS_PASSWORD_LEN)
                {
                    XMORE("password should not exceeds %d bytes!\n", CAPA_SYS_PASSWORD_LEN);
                    return SYS_ERR_OK;
                }
                if(osal_strlen(argv[1]) < CAPA_SYS_PASSWORD_MIX_LEN)
	            {
	                XMORE("Password cannot be less than %d bytes!\n", CAPA_SYS_PASSWORD_MIX_LEN);
	                return SYS_ERR_OK;
	            }

                if (SYS_ERR_OK != STRING_VALIDATE(argv[1], TRUE))
                    return SYS_ERR_STRING;
            }
        }

        osal_strcpy(userInfo.password, argv[1]);
    }
    else
    {
        if ('a' == argv[1][0])
            userInfo.priv = SYS_LOCALUSER_PRIV_ADMIN;
        else if ('u' == argv[1][0])
            userInfo.priv = SYS_LOCALUSER_PRIV_USER;
        else
            userInfo.priv = SYS_STR2UINT(argv[1]);

        if ('p' == CMD_TOKENSTR_GET(cmd->vline, 4)[0])
        {
            userInfo.passwdType = SYS_PASSWD_TYPE_CLEARTEXT;

            if(osal_strlen(argv[2]) > CAPA_SYS_PASSWORD_LEN)
            {
                XMORE("password should not exceeds %d bytes!\n", CAPA_SYS_PASSWORD_LEN);
                return SYS_ERR_OK;
            }
            if(osal_strlen(argv[2]) < CAPA_SYS_PASSWORD_MIX_LEN)
            {
                XMORE("Password cannot be less than %d bytes!\n", CAPA_SYS_PASSWORD_MIX_LEN);
                return SYS_ERR_OK;
            }

            if (SYS_ERR_OK != STRING_VALIDATE(argv[2], TRUE))
                return SYS_ERR_STRING;
        }
        else if ('s' == CMD_TOKENSTR_GET(cmd->vline, 4)[0])
        {
            userInfo.passwdType = SYS_PASSWD_TYPE_ENCRYPT;

            if (7 == CMD_TOKENNUM_GET(cmd->vline))
            {
                directCopy = TRUE;

                if(osal_strlen(argv[2]) > CAPA_SYS_CRYPT_PASSWORD_LEN)
                {
                    XMORE("encrypted password should not exceeds %d bytes!\n", CAPA_SYS_CRYPT_PASSWORD_LEN);
                    return SYS_ERR_OK;
                }
                if(osal_strlen(argv[2]) < CAPA_SYS_PASSWORD_MIX_LEN)
	            {
	                XMORE("encrypted password cannot be less than %d bytes!\n", CAPA_SYS_PASSWORD_MIX_LEN);
	                return SYS_ERR_OK;
	            }
            }
            else
            {
                directCopy = FALSE;

                if(osal_strlen(argv[2]) > CAPA_SYS_PASSWORD_LEN)
                {
                    XMORE("password should not exceeds %d bytes!\n", CAPA_SYS_PASSWORD_LEN);
                    return SYS_ERR_OK;
                }
                if(osal_strlen(argv[2]) < CAPA_SYS_PASSWORD_MIX_LEN)
	            {
	                XMORE("Password cannot be less than %d bytes!\n", CAPA_SYS_PASSWORD_MIX_LEN);
	                return SYS_ERR_OK;
	            }

                if (SYS_ERR_OK != STRING_VALIDATE(argv[2], TRUE))
                    return SYS_ERR_STRING;
            }
        }

        osal_strcpy(userInfo.password, argv[2]);
    }

    if (CMD_IS_CLI_RUN())
    {
        if (TRUE == isCopy2Running)
        {
            SYS_ERR_CHK(sal_sys_localUserPasswdDirect_add(userInfo));
        }
        else
        {
            int32 priv = 0;
            int32 ret = SYS_ERR_FAILED;
            uint32 i;
            char oldPasswd[CAPA_SYS_PASSWORD_LEN + 1];
            sys_userInfo_t oldUserInfo;

            for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
            {
                SYS_MEM_CLEAR(oldUserInfo);

                SYS_ERR_CHK(sal_sys_localUser_get(i, &oldUserInfo));

                if (oldUserInfo.priv < 0)
                    continue;

                if (0 == osal_strcmp(oldUserInfo.username, userInfo.username))
                {
                    
                    SYS_MEM_CLEAR(oldPasswd);

                    XMORE("Old password: ");
                    input_str(oldPasswd, CAPA_SYS_PASSWORD_LEN + 1, '*');

                    ret = sal_sys_localLogin_auth(oldUserInfo.username, oldPasswd, &priv);

                    if (SYS_ERR_OK != ret)
                    {
                        XMORE("Old password is incorrect !\n");

                        return SYS_ERR_OK;
                    }

                    break;
                }
            }

            if (TRUE == directCopy)
                SYS_ERR_CHK(sal_sys_localUserPasswdDirect_add(userInfo));
            else
                SYS_ERR_CHK(sal_sys_localUser_add(userInfo));
        }
    }
    else
    {
        SYS_ERR_CHK(sal_sys_localUserPasswdDirect_add(userInfo));
    }

    return SYS_ERR_OK;
}

int32 cmd_sys_localUserNoPasswd_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_userInfo_t userInfo;

    osal_memset(&userInfo, 0, sizeof(userInfo));

    if(osal_strlen(argv[0]) > CAPA_SYS_USERNAME_LEN)
    {
        XMORE("username should not exceeds %d bytes!\n",CAPA_SYS_USERNAME_LEN);
        return SYS_ERR_OK;
    }

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
        return SYS_ERR_STRING;

    osal_strcpy(userInfo.username, argv[0]);

    if (argc < 2)
    {
        userInfo.priv = SYS_LOCALUSER_PRIV_ADMIN;
    }
    else
    {
        if ('a' == argv[1][0])
            userInfo.priv = SYS_LOCALUSER_PRIV_ADMIN;
        else if ('u' == argv[1][0])
            userInfo.priv = SYS_LOCALUSER_PRIV_USER;
        else
            userInfo.priv = SYS_STR2UINT(argv[1]);
    }

    userInfo.passwdType = SYS_PASSWD_TYPE_NOPASSWD;

    SYS_ERR_CHK(sal_sys_localUser_add(userInfo));

    return SYS_ERR_OK;
}

int32 cmd_sys_enblPasswd_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 directCopy = FALSE;
    sys_enblPasswd_t enblPasswd;
#ifdef CONFIG_SYS_AAA
    int32 priv = SYS_LOCALUSER_PRIV_ADMIN;
#endif

    osal_memset(&enblPasswd, 0, sizeof(enblPasswd));

    if (CMD_IS_NO_FORM_EXIST())
    {
#ifdef CONFIG_SYS_AAA
        if (argc >= 1)
            SYS_ERR_CHK(sal_sys_enablePassword_set(SYS_STR2UINT(argv[0]), enblPasswd));
        else
#endif
            SYS_ERR_CHK(sal_sys_enablePassword_set(BOND_AAA_PRIV_MAX, enblPasswd));

        return SYS_ERR_OK;
    }

    if (argc < 2)
    {
        if ('p' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
        {
            enblPasswd.passwdType = SYS_PASSWD_TYPE_CLEARTEXT;
        }
        else if ('s' == CMD_TOKENSTR_GET(cmd->vline, 1)[0])
        {
            enblPasswd.passwdType = SYS_PASSWD_TYPE_ENCRYPT;

            if (4 == CMD_TOKENNUM_GET(cmd->vline))
                directCopy = TRUE;
            else
                directCopy = FALSE;
        }

        osal_strcpy(enblPasswd.password, argv[0]);
#ifdef CONFIG_SYS_AAA
        priv = SYS_LOCALUSER_PRIV_ADMIN;
#endif
    }
#ifdef CONFIG_SYS_AAA
    else
    {
        if ('p' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
        {
            enblPasswd.passwdType = SYS_PASSWD_TYPE_CLEARTEXT;
        }
        else if ('s' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
        {
            enblPasswd.passwdType = SYS_PASSWD_TYPE_ENCRYPT;

            if (6 == CMD_TOKENNUM_GET(cmd->vline))
                directCopy = TRUE;
            else
                directCopy = FALSE;
        }

        osal_strcpy(enblPasswd.password, argv[1]);

        priv = SYS_STR2UINT(argv[0]);
    }
#endif 
#ifdef CONFIG_SYS_AAA
    if (CMD_IS_CLI_RUN())
    {
        if (TRUE == directCopy)
            SYS_ERR_CHK(sal_sys_enablePasswordDirect_set(priv, enblPasswd));
        else
            SYS_ERR_CHK(sal_sys_enablePassword_set(priv, enblPasswd));
    }
    else
    {
        SYS_ERR_CHK(sal_sys_enablePasswordDirect_set(priv, enblPasswd));
    }
#else
    if (CMD_IS_CLI_RUN())
    {
        if (TRUE == directCopy)
            SYS_ERR_CHK(sal_sys_enablePasswordDirect_set(0, enblPasswd));
        else
            SYS_ERR_CHK(sal_sys_enablePassword_set(0, enblPasswd));
    }
    else
    {
        SYS_ERR_CHK(sal_sys_enablePasswordDirect_set(0, enblPasswd));
    }
#endif 

    return SYS_ERR_OK;
}

int32 cmd_sys_localUser_access_method_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_userInfo_t userInfo;
    uint32 enable = 0;
    uint32 access = 0;
	int i ; 
	sys_userInfo_t oldUserInfo;
	
    if(osal_strlen(argv[0]) > CAPA_SYS_USERNAME_LEN)
    {
        XMORE("username should not exceeds %d bytes!\n",CAPA_SYS_USERNAME_LEN);
        return SYS_ERR_OK;
    }

    if (SYS_ERR_OK != STRING_VALIDATE(argv[0], TRUE))
        return SYS_ERR_STRING;

    SYS_MEM_CLEAR(userInfo);
    osal_strcpy(userInfo.username, argv[0]);

    if ('p' == argv[1][0])
    	enable = 1;
    else if ('d' == argv[1][0])
        enable = 0;

    if ('c' == argv[2][0])
    	access = 0x1 << SYS_ACCESS_CLI_CONSOLE;
    else if ('s' == argv[2][0])
        access = 0x1 << SYS_ACCESS_CLI_SSH;
    else if ('t' == argv[2][0])
        access = 0x1 << SYS_ACCESS_CLI_TELNET;
    else if ('h' == argv[2][0])
    {
    	if(osal_strlen(argv[2]) == 4)
        	access = 0x1 << SYS_ACCESS_WEB_HTTP;
        else
        	access = 0x1 << SYS_ACCESS_WEB_HTTPS;
	}
	
	for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
	{
		SYS_MEM_CLEAR(oldUserInfo);
	
		SYS_ERR_CHK(sal_sys_localUser_get(i, &oldUserInfo));
	
		if (oldUserInfo.priv < 0)
			continue;
	
		if (0 == osal_strcmp(oldUserInfo.username, userInfo.username))
		{
			if(enable)
			{
				oldUserInfo.accessMethod =  oldUserInfo.accessMethod  | access;
			}
			else 
			{
				oldUserInfo.accessMethod = oldUserInfo.accessMethod & (~access);
			}

			sal_sys_localUser_access_method_set(i, oldUserInfo);
			break;
		}
	}

    return SYS_ERR_OK;
}

static int32 _cmd_sys_localUser_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 i;
    sys_userInfo_t userInfo;
    sys_enblPasswd_t enblPasswd;

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        osal_memset(&userInfo, 0, sizeof(userInfo));

        SYS_ERR_CHK(sal_sys_localUser_get(i, &userInfo));

        if (userInfo.priv < 0)
            continue;

        osal_memset(cmd, 0, sizeof(cmd));

        if (SYS_LOCALUSER_PRIV_ADMIN == userInfo.priv)
        {
            if (SYS_PASSWD_TYPE_NOPASSWD == userInfo.passwdType)
            {
                osal_sprintf(cmd, "username \"%s\" nopassword\n", userInfo.username);
            }
            else if (SYS_PASSWD_TYPE_CLEARTEXT == userInfo.passwdType)
            {
                osal_sprintf(cmd, "username \"%s\" password \"%s\"\n",
                                  userInfo.username,
                                  userInfo.password);
            }
            else
            {
                osal_sprintf(cmd, "username \"%s\" secret encrypted %s\n",
                                  userInfo.username,
                                  userInfo.password);
            }
        }
        else if (SYS_LOCALUSER_PRIV_USER == userInfo.priv)
        {
            if (SYS_PASSWD_TYPE_NOPASSWD == userInfo.passwdType)
            {
                osal_sprintf(cmd, "username \"%s\" privilege user nopassword\n", userInfo.username);
            }
            else if (SYS_PASSWD_TYPE_CLEARTEXT == userInfo.passwdType)
            {
                osal_sprintf(cmd, "username \"%s\" privilege user password \"%s\"\n",
                                  userInfo.username,
                                  userInfo.password);
            }
            else
            {
                osal_sprintf(cmd, "username \"%s\" privilege user secret encrypted %s\n",
                                  userInfo.username,
                                  userInfo.password);
            }
        }
#ifdef CONFIG_SYS_AAA
        else
        {
            if (SYS_PASSWD_TYPE_NOPASSWD == userInfo.passwdType)
            {
                osal_sprintf(cmd, "username \"%s\" nopassword\n", userInfo.username);
            }
            else if (SYS_PASSWD_TYPE_CLEARTEXT == userInfo.passwdType)
            {
                osal_sprintf(cmd, "username \"%s\" privilege %d password \"%s\"\n",
                                  userInfo.username,
                                  userInfo.priv,
                                  userInfo.password);
            }
            else
            {
                osal_sprintf(cmd, "username \"%s\" privilege %d secret encrypted %s\n",
                                  userInfo.username,
                                  userInfo.priv,
                                  userInfo.password);
            }
        }
#endif 

        CMD_WRITE_TO_FILE(fp, cmd);

        if((userInfo.accessMethod & 0x1 << SYS_ACCESS_CLI_CONSOLE) == 0)
        {
        	osal_memset(cmd, 0, sizeof(cmd));
			osal_sprintf(cmd, "username \"%s\" access-method deny console\n", userInfo.username);
			CMD_WRITE_TO_FILE(fp, cmd);
        }
        
        if((userInfo.accessMethod & 0x1 << SYS_ACCESS_CLI_TELNET) == 0)
        {
			osal_memset(cmd, 0, sizeof(cmd));
			osal_sprintf(cmd, "username \"%s\" access-method deny telnet\n", userInfo.username);
			CMD_WRITE_TO_FILE(fp, cmd);
        }

        if((userInfo.accessMethod & 0x1 << SYS_ACCESS_CLI_SSH) == 0)
        {
			osal_memset(cmd, 0, sizeof(cmd));
			osal_sprintf(cmd, "username \"%s\" access-method deny ssh\n", userInfo.username);
			CMD_WRITE_TO_FILE(fp, cmd);
        }

        if((userInfo.accessMethod & 0x1 << SYS_ACCESS_WEB_HTTP)== 0)
        {
			osal_memset(cmd, 0, sizeof(cmd));
			osal_sprintf(cmd, "username \"%s\" access-method deny http\n", userInfo.username);
			CMD_WRITE_TO_FILE(fp, cmd);
        }

        if((userInfo.accessMethod & 0x1 << SYS_ACCESS_WEB_HTTPS)== 0)
        {
			osal_memset(cmd, 0, sizeof(cmd));
			osal_sprintf(cmd, "username \"%s\" access-method deny https\n", userInfo.username);
			CMD_WRITE_TO_FILE(fp, cmd);
        }
        
    }

#ifdef CONFIG_SYS_AAA
    for (i = 0; i < CAPA_AAA_PRIV_NUM; i++)
    {
        osal_memset(&enblPasswd, 0, sizeof(enblPasswd));
        SYS_ERR_CHK(sal_sys_enablePassword_get(i, &enblPasswd));

        if (0 == osal_strlen(enblPasswd.password))
            continue;

        if (SYS_ERR_OK == sal_aaa_localEnablePassword_authen(i, DFLT_SYS_ENABLE_PASSWORD))
            continue;

        osal_memset(cmd, 0, sizeof(cmd));
        if (SYS_LOCALUSER_PRIV_ADMIN == i)
        {
            osal_sprintf(cmd, "enable %s \"%s\"\n",
                              ((SYS_PASSWD_TYPE_CLEARTEXT == enblPasswd.passwdType) ? "password" : "secret"),
                              enblPasswd.password);
        }
        else
        {
            osal_sprintf(cmd, "enable privilege %d %s \"%s\"\n",
                              i,
                              ((SYS_PASSWD_TYPE_CLEARTEXT == enblPasswd.passwdType) ? "password" : "secret"),
                              enblPasswd.password);
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }
#else
    osal_memset(&enblPasswd, 0, sizeof(enblPasswd));

    SYS_ERR_CHK(sal_sys_enablePassword_get(0, &enblPasswd));

    if (SYS_PASSWD_TYPE_NOPASSWD == enblPasswd.passwdType)
        return SYS_ERR_OK;

    osal_memset(cmd, 0, sizeof(cmd));
    osal_sprintf(cmd, "enable %s \"%s\"\n",
                      ((enblPasswd.passwdType == SYS_PASSWD_TYPE_CLEARTEXT) ? "password" : "secret"),
                      enblPasswd.password);

    CMD_WRITE_TO_FILE(fp, cmd);
#endif 

    return SYS_ERR_OK;
}

int32 cmd_sys_auth_save(FILE *fp)
{
    
    SYS_ERR_CHK(_cmd_sys_localUser_save(fp));

    return SYS_ERR_OK;
}

