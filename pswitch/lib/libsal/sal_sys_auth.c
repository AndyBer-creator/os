
#include <libsal/sal_util.h>
#include <libsal/sal_sys.h>

const sys_text_t text_user_passwd_type[] =
{
    {      "no password"  },
    {     "text"         },
    {       "encrypted"    },
};

static int32 _sal_sys_localUser_add(sys_userInfo_t userInfo);

static int32 _sal_sys_localUser_add(sys_userInfo_t userInfo)
{
    char crypt_password[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    uint32 i;
    sys_userInfo_t tmpUserInfo;

    osal_memset(crypt_password, 0, sizeof(crypt_password));

    if (SYS_PASSWD_TYPE_ENCRYPT == userInfo.passwdType)
    {
        SYS_ERR_CHK(sal_util_password_encrypt(userInfo.password, NULL, crypt_password));

        osal_memset(userInfo.password, 0, sizeof(userInfo.password));
        osal_strcpy(userInfo.password, crypt_password);
    }

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpUserInfo);

        SYS_ERR_CHK(cfg_sys_localUser_get(i, &tmpUserInfo));

        if ((tmpUserInfo.priv >= 0) && (0 == osal_strcmp(userInfo.username, tmpUserInfo.username)))
        {
        	if(userInfo.accessMethod == 0)
        		userInfo.accessMethod = tmpUserInfo.accessMethod;
            SYS_ERR_CHK(cfg_sys_localUser_set(i, userInfo));

            SYS_ERR_CHK(sal_sys_htpasswd_create());
            SYS_ERR_CHK(sal_sys_passwd_create());

            SYS_LOG(LOG_CAT_AAA, LOG_AAA_USER_EDIT,
                        userInfo.username,
                        userInfo.priv,
                        text_user_passwd_type[userInfo.passwdType].text);

            return SYS_ERR_OK;
        }
    }

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpUserInfo);

        SYS_ERR_CHK(cfg_sys_localUser_get(i, &tmpUserInfo));

        if (tmpUserInfo.priv >= 0)
            continue;
            
		if(userInfo.accessMethod == 0)
			userInfo.accessMethod = DFLT_SYS_USER_ACCESS_METH_ALL;

        SYS_ERR_CHK(cfg_sys_localUser_set(i, userInfo));

        SYS_ERR_CHK(sal_sys_htpasswd_create());
        SYS_ERR_CHK(sal_sys_passwd_create());

        SYS_LOG(LOG_CAT_AAA, LOG_AAA_USER_ADD,
                    userInfo.username,
                    userInfo.priv,
                    text_user_passwd_type[userInfo.passwdType].text);

        return SYS_ERR_OK;
    }

    return SYS_ERR_LOCALUSER_FULL;
}

int32 sal_sys_localUser_add(sys_userInfo_t userInfo)
{
    SYS_ERR_CHK(_sal_sys_localUser_add(userInfo));

    return SYS_ERR_OK;
}

int32 sal_sys_localUserForce_add(sys_userInfo_t userInfo)
{
    SYS_ERR_CHK(_sal_sys_localUser_add(userInfo));

    return SYS_ERR_OK;
}

int32 sal_sys_localUserPasswdDirect_add(sys_userInfo_t userInfo)
{
    uint32 i;
    sys_userInfo_t tmpUserInfo;

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        osal_memset(&tmpUserInfo, 0, sizeof(tmpUserInfo));

        SYS_ERR_CHK(cfg_sys_localUser_get(i, &tmpUserInfo));

        if ((tmpUserInfo.priv >= 0) && (0 == osal_strcmp(userInfo.username, tmpUserInfo.username)))
        {
        	if(userInfo.accessMethod == 0)
        		userInfo.accessMethod = tmpUserInfo.accessMethod;
            SYS_ERR_CHK(cfg_sys_localUser_set(i, userInfo));

            SYS_ERR_CHK(sal_sys_htpasswd_create());
            SYS_ERR_CHK(sal_sys_passwd_create());

            return SYS_ERR_OK;
        }
    }

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        SYS_ERR_CHK(cfg_sys_localUser_get(i, &tmpUserInfo));

        if (tmpUserInfo.priv >= 0)
            continue;
            
		if(userInfo.accessMethod == 0)
			userInfo.accessMethod = DFLT_SYS_USER_ACCESS_METH_ALL;

        SYS_ERR_CHK(cfg_sys_localUser_set(i, userInfo));

        SYS_ERR_CHK(sal_sys_htpasswd_create());
        SYS_ERR_CHK(sal_sys_passwd_create());

        return SYS_ERR_OK;
    }

    return SYS_ERR_LOCALUSER_FULL;
}

int32 sal_sys_localUser_del(char *username)
{
    uint32 i;
    sys_userInfo_t tmpUserInfo;

    SYS_PARAM_CHK((NULL == username), SYS_ERR_NULL_POINTER);
#ifdef CONFIG_SYS_DEL_DEF_USERNAME
	if(sal_sys_localUser_is_theLastUser(username) == SYS_ERR_LAST_LOCALUSER)
	{
		return SYS_ERR_LAST_LOCALUSER;
	}
#else
    if (0 == osal_strcmp(username, DFLT_SYS_ADMIN_USERNAME))
        return SYS_ERR_LOCALUSER_DEFAULT_DEL;
#endif

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        osal_memset(&tmpUserInfo, 0, sizeof(tmpUserInfo));

        SYS_ERR_CHK(cfg_sys_localUser_get(i, &tmpUserInfo));

        if (tmpUserInfo.priv >= 0 && 0 == osal_strcmp(tmpUserInfo.username, username))
        {
            
            osal_memset(&tmpUserInfo, 0, sizeof(sys_userInfo_t));
            tmpUserInfo.priv = -1;

            SYS_ERR_CHK(cfg_sys_localUser_set(i, tmpUserInfo));

            SYS_ERR_CHK(sal_sys_htpasswd_create());
            SYS_ERR_CHK(sal_sys_passwd_create());

            SYS_LOG(LOG_CAT_AAA, LOG_AAA_USER_DEL,
                        username);

            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_LOCALUSER_NOT_EXIST;
}

int32 sal_sys_localUser_get(uint32 index, sys_userInfo_t *pUserInfo)
{
    SYS_PARAM_CHK((index >= CAPA_SYS_LOCALUSER_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pUserInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_localUser_get(index, pUserInfo));

    return SYS_ERR_OK;
}

int32 sal_sys_localUser_getByName(char *username, sys_userInfo_t *pUserInfo)
{
    uint32 i;
    sys_userInfo_t userInfo;

    SYS_PARAM_CHK((NULL == username), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pUserInfo), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        SYS_MEM_CLEAR(userInfo);

        SYS_ERR_CHK(cfg_sys_localUser_get(i, &userInfo));

        if (userInfo.priv < 0)
            continue;

        if (0 != osal_strcmp(userInfo.username, username))
            continue;

        osal_memcpy(pUserInfo, &userInfo, sizeof(sys_userInfo_t));

        return SYS_ERR_OK;
    }

    return SYS_ERR_LOCALUSER_NOT_EXIST;
}

int32 sal_sys_localLogin_auth(char *username, char *password, int32 *pPriv)
{
    uint32 i;
    char crypt_password[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    sys_userInfo_t userInfo;

    SYS_PARAM_CHK((NULL == username), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == password), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pPriv), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(crypt_password);

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        SYS_MEM_CLEAR(userInfo);

        SYS_ERR_CHK(cfg_sys_localUser_get(i, &userInfo));

        if (userInfo.priv < 0)
            continue;

        if (0 != osal_strcmp(userInfo.username, username))
            continue;

        switch (userInfo.passwdType)
        {
            case SYS_PASSWD_TYPE_ENCRYPT:
                SYS_ERR_CHK(sal_util_password_encrypt(password, userInfo.password, crypt_password));

                if (0 == osal_strcmp(userInfo.password, crypt_password))
                {
                    *pPriv = userInfo.priv;

                    return SYS_ERR_OK;
                }
                break;

            case SYS_PASSWD_TYPE_CLEARTEXT:
                if (0 == osal_strcmp(userInfo.password, password))
                {
                    *pPriv = userInfo.priv;

                    return SYS_ERR_OK;
                }
                break;

            default:
                *pPriv = userInfo.priv;
                return SYS_ERR_OK;
                break;
        }

        return SYS_ERR_PASSWORD_INVALID;
    }

    return SYS_ERR_LOCALUSER_NOT_EXIST;
}

int32 sal_sys_enablePassword_set(int32 priv, sys_enblPasswd_t password)
{
    char cryptPasswd[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];

    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(priv, BOND_AAA_PRIV_MIN, BOND_AAA_PRIV_MAX), SYS_ERR_INPUT);

    osal_memset(cryptPasswd, 0, sizeof(cryptPasswd));

    if (SYS_PASSWD_TYPE_ENCRYPT == password.passwdType)
    {
        SYS_ERR_CHK(sal_util_password_encrypt(password.password, NULL, cryptPasswd));

        osal_memset(password.password, 0, sizeof(password.password));
        osal_strcpy(password.password, cryptPasswd);
    }

    SYS_ERR_CHK(cfg_sys_enablePassword_set(priv, password));

    return SYS_ERR_OK;
}

int32 sal_sys_enablePasswordDirect_set(int32 priv, sys_enblPasswd_t password)
{
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(priv, BOND_AAA_PRIV_MIN, BOND_AAA_PRIV_MAX), SYS_ERR_INPUT);

    SYS_ERR_CHK(cfg_sys_enablePassword_set(priv, password));

    return SYS_ERR_OK;
}

int32 sal_sys_enablePassword_get(int32 priv, sys_enblPasswd_t *pPassword)
{
    SYS_PARAM_CHK(IS_PARAM_NUM_INVALID(priv, BOND_AAA_PRIV_MIN, BOND_AAA_PRIV_MAX), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pPassword), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_sys_enablePassword_get(priv, pPassword));

    return SYS_ERR_OK;
}

int32 sal_sys_localUser_is_theLastUser(char *username)
{
    uint32 i;
    sys_userInfo_t tmpUserInfo;

    SYS_PARAM_CHK((NULL == username), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        osal_memset(&tmpUserInfo, 0, sizeof(tmpUserInfo));

        SYS_ERR_CHK(cfg_sys_localUser_get(i, &tmpUserInfo));

        if (tmpUserInfo.priv >= 0)
        {
        	if(0 == osal_strcmp(tmpUserInfo.username, username))
        	{
        		continue;
        	}		
            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_LAST_LOCALUSER;
}

int32 sal_sys_localUser_access_method_set(uint32 index, sys_userInfo_t userInfo)
{
	
    SYS_ERR_CHK(cfg_sys_localUser_set(index, userInfo));

    return SYS_ERR_OK;
}

int32 sal_sys_get_sys_defaul_user(sys_userInfo_t* userInfo)
{
	
    FILE *confp = NULL;
    char buf[SYS_BUF256_LEN];
    char *tok[5];
    int i,j;
    SYS_MEM_CLEAR(buf);
    
    confp = fopen (SYS_OEM_USR_CONF_FILE, "r");
    if (confp == NULL)
    {
        return SYS_ERR_FAILED;
    }
    while(fgets(buf, sizeof(buf), confp))
    {
       if(osal_strstr(buf, "username") && osal_strstr(buf, "secret") && osal_strstr(buf, "encrypted"))
       {
         tok[0] = strtok(buf, " \n"); 

         tok[1] = strtok(NULL, " \n"); 
  
         j = 0;

         for(i = 1; i< (strlen(tok[1])-1);i++)
         {
           userInfo->username[j] = tok[1][i];
           j++;
         
         }
           userInfo->username[j] = '\0';        
 
         tok[2] = strtok(NULL, " \n"); 

         tok[3] = strtok(NULL, " \n");

         tok[4] = strtok(NULL, " \n");
         
         i = 0;
         j = 0;
         while((tok[4][i]!= '\0') && (tok[4][i]!= '\r') && (tok[4][i]!= '\n'))
        {
          userInfo->password[j] = tok[4][i];
          i++;
          j++;
        }
        
        userInfo->password[j] = '\0';
         break; 
       }
       else
       {
         continue; 
       }
    }
    return SYS_ERR_OK;
}

