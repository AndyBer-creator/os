
#include <sys/timeb.h>
#include <libsal/sal_util.h>
#ifdef CONFIG_SYS_LIB_RADIUS
#include <libsal/sal_radius.h>
#endif
#include <libsal/sal_aaa.h>

const sys_text_t text_aaa_auth_method[] =
{
    {       ""       },
    {        "none"   },
    {       "local"  },
    {        "line"   },
    {      "enable" },
#ifdef CONFIG_SYS_LIB_RADIUS
    {      "radius" },
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
    {     "tacacs+"},
#endif
};

const sys_text_t text_aaa_authList_type[] =
{
    {                "" },
    {        "login authentication" },
    {       "enable authentication"},
};

int32 sal_aaa_authList_set(
    char                *pAuthListName,
    sys_auth_type_t     *pTypePrio,
    sys_authList_type_t authListType)
{
    char log_typePrio[64];
    int32 index = -1;
    uint32 i;

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pTypePrio), SYS_ERR_NULL_POINTER);

    osal_memset(log_typePrio, 0, sizeof(log_typePrio));

    SYS_ERR_CHK(cfg_aaa_authListIndex_get(pAuthListName, authListType, &index));

    if (index < 0)
    {
        SYS_ERR_CHK(cfg_aaa_authListFirstEmptyIndex_get(&index));
        
        if (index < 0)
            return SYS_ERR_AAA_AUTHLIST_FULL;
    }

    SYS_ERR_CHK(cfg_aaa_authList_set(index, pAuthListName, pTypePrio, authListType));

    for (i = 0; i < CAPA_AAA_AUTHTYPE_PRI_NUM; i++)
    {
        if (0 != i && SYS_AUTH_TYPE_NONE == pTypePrio[i])
            break;

        switch (pTypePrio[i])
        {
            case SYS_AUTH_TYPE_LOCAL:
                osal_strcat(log_typePrio, "local ");
                break;

            case SYS_AUTH_TYPE_LINE:
                osal_strcat(log_typePrio, "line ");
                break;

            case SYS_AUTH_TYPE_ENABLE:
                osal_strcat(log_typePrio, "enable ");
                break;

#ifdef CONFIG_SYS_LIB_RADIUS
            case SYS_AUTH_TYPE_RADIUS:
                osal_strcat(log_typePrio, "radius ");
                break;
#endif
#ifdef CONFIG_SYS_LIB_TACPLUS
            case SYS_AUTH_TYPE_TACPLUS:
                osal_strcat(log_typePrio, "tacacs+ ");
                break;
#endif

            default:
                osal_strcat(log_typePrio, "none ");
                break;
        }
    }

    if (osal_strlen(log_typePrio) > 0)
        log_typePrio[osal_strlen(log_typePrio) - 1] = '\0'; 

    return SYS_ERR_OK;
}

int32 sal_aaa_authList_getByIdx(
    int32               index,
    char                *pAuthListName,
    sys_auth_type_t     *pTypePrio,
    sys_authList_type_t *pAuthListType)
{
    SYS_PARAM_CHK((index < 0 || index > CAPA_AAA_AUTHLIST_NUM - 1), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pTypePrio), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_aaa_authList_get(index, pAuthListName, pTypePrio, pAuthListType));

    return SYS_ERR_OK;
}

int32 sal_aaa_authList_del(char *pAuthListName, sys_authList_type_t authListType)
{
    int32 index = -1;
    int32 accessIdx = -1;
    uint32 i = 0;
    uint32 j = 0;
    sys_auth_type_t authTypes[CAPA_AAA_AUTHTYPE_PRI_NUM];

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);

    osal_memset(authTypes, 0, sizeof(authTypes));

    SYS_ERR_CHK(cfg_aaa_authListIndex_get(pAuthListName, authListType, &index));

    for (i = 0; i < SYS_ACCESS_END; i++)
    {
        for (j = SYS_AUTHLIST_TYPE_AUTHEN_LOGIN; j < SYS_AUTHLIST_TYPE_END; j++)
        {
            
            if ((i == SYS_ACCESS_WEB_HTTP || i == SYS_ACCESS_WEB_HTTPS) && j != SYS_AUTHLIST_TYPE_AUTHEN_LOGIN)
                continue;

            if (j != authListType)
                continue;

            accessIdx = -1;

            switch (j)
            {
                case SYS_AUTHLIST_TYPE_AUTHEN_LOGIN:
                    SYS_ERR_CHK(cfg_aaa_loginAuthListIndex_get(i, &accessIdx));
                    if (index == accessIdx)
                        SYS_ERR_CHK(cfg_aaa_loginAuthListIndex_set(i, 0));
                    break;

                case SYS_AUTHLIST_TYPE_AUTHEN_ENABLE:
                    SYS_ERR_CHK(cfg_aaa_enableAuthListIndex_get(i, &accessIdx));
                    if (index == accessIdx)
                        SYS_ERR_CHK(cfg_aaa_enableAuthListIndex_set(i, 0));
                    break;

                default:
                    break;
            }
        }
    }

    if (index < 0)
    {
        return SYS_ERR_AAA_AUTHLIST_NOT_EXIST;
    }

    SYS_ERR_CHK(cfg_aaa_authList_set(index, "", authTypes, SYS_AUTHLIST_TYPE_NONE));

    return SYS_ERR_OK;
}

int32 sal_aaa_authListTypePrio_get(
    char                *authListName,
    sys_authList_type_t authListType,
    sys_auth_type_t     *pTypePrio)
{
    char tmpAythListName[CAPA_AAA_AUTHLIST_NAME_LEN + 1];
    uint32 i;
    sys_authList_type_t tmpAuthListType;
    sys_auth_type_t tmpAuthTypePrio[CAPA_AAA_AUTHTYPE_PRI_NUM];

    SYS_PARAM_CHK((NULL == authListName), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pTypePrio), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_AAA_AUTHLIST_NUM; i++)
    {
        osal_memset(tmpAythListName, 0, sizeof(tmpAythListName));
        osal_memset(tmpAuthTypePrio, 0, sizeof(tmpAuthTypePrio));

        SYS_ERR_CHK(cfg_aaa_authList_get(i, tmpAythListName, tmpAuthTypePrio, &tmpAuthListType));

        if (tmpAuthListType != authListType)
            continue;

        if (osal_strcmp(tmpAythListName, authListName) != 0)
            continue;

        osal_memcpy(pTypePrio, &tmpAuthTypePrio, sizeof(tmpAuthTypePrio));

        break;
    }

    return SYS_ERR_OK;
}

int32 sal_aaa_loginAuthList_set(sys_access_type_t type, char *pAuthListName)
{
    int32 index = -1;
    sys_auth_type_t typePrio[CAPA_AAA_AUTHTYPE_PRI_NUM] = {0};

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_aaa_authListIndex_get(pAuthListName, SYS_AUTHLIST_TYPE_AUTHEN_LOGIN, &index));

    if (index < 0)
        return SYS_ERR_AAA_AUTHLIST_NOT_EXIST;
	
    cfg_aaa_authTypePrio_get(index, typePrio);

	if (((type == SYS_ACCESS_WEB_HTTP) || (type == SYS_ACCESS_WEB_HTTPS)) && (typePrio[0] == SYS_AUTH_TYPE_ENABLE))
        return SYS_ERR_AAA_HTTP_NOT_SUPPORTED_ENABLE;
		
    SYS_ERR_CHK(cfg_aaa_loginAuthListIndex_set(type, index));

    return SYS_ERR_OK;
}

int32 sal_aaa_loginAuthListName_get(sys_access_type_t type, char *pAuthListName)
{
    int32 index = -1;

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_aaa_loginAuthListIndex_get(type, &index));

    SYS_ERR_CHK(cfg_aaa_authListName_get(index, pAuthListName));

    return SYS_ERR_OK;
}

int32 sal_aaa_loginAuthTypePrio_get(sys_access_type_t type, sys_auth_type_t *pTypePrio)
{
    int32 index = -1;

    SYS_PARAM_CHK((NULL == pTypePrio), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_aaa_loginAuthListIndex_get(type, &index));

    SYS_ERR_CHK(cfg_aaa_authTypePrio_get(index, pTypePrio));

    return SYS_ERR_OK;
}

int32 sal_aaa_enableAuthList_set(sys_access_type_t type, char *pAuthListName)
{
    int32 index = -1;

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_aaa_authListIndex_get(pAuthListName, SYS_AUTHLIST_TYPE_AUTHEN_ENABLE, &index));

    if (index < 0)
        return SYS_ERR_AAA_AUTHLIST_NOT_EXIST;

    SYS_ERR_CHK(cfg_aaa_enableAuthListIndex_set(type, index));

    return SYS_ERR_OK;
}

int32 sal_aaa_enableAuthListName_get(sys_access_type_t type, char *pAuthListName)
{
    int32 index = -1;

    SYS_PARAM_CHK((NULL == pAuthListName), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_aaa_enableAuthListIndex_get(type, &index));

    SYS_ERR_CHK(cfg_aaa_authListName_get(index, pAuthListName));

    return SYS_ERR_OK;
}

int32 sal_aaa_enableAuthTypePrio_get(sys_access_type_t type, sys_auth_type_t *pTypePrio)
{
    int32 index = -1;

    SYS_PARAM_CHK((NULL == pTypePrio), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_aaa_enableAuthListIndex_get(type, &index));

    SYS_ERR_CHK(cfg_aaa_authTypePrio_get(index, pTypePrio));

    return SYS_ERR_OK;
}

int32 sal_aaa_localLoginUsername_authen(char *username, uint32 *pLocalUsrIdx)
{
    uint32 i;
    sys_userInfo_t userInfo;

    SYS_PARAM_CHK((NULL == username), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        SYS_ERR_CHK(cfg_sys_localUser_get(i, &userInfo));

        if (userInfo.priv < 0)
            continue;

        if (0 == osal_strcmp(userInfo.username, username))
        {
            *pLocalUsrIdx = i;

            return SYS_ERR_OK;
        }
    }

    return SYS_ERR_USERNAME_INVALID;
}

int32 sal_aaa_localLoginPassword_authen(uint32 localUsrIdx, char *password, int32 *pPriv)
{
    char crypt_password[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    sys_userInfo_t userInfo;

    SYS_PARAM_CHK((NULL == password), SYS_ERR_NULL_POINTER);

    osal_memset(crypt_password, 0, sizeof(crypt_password));

    SYS_ERR_CHK(cfg_sys_localUser_get(localUsrIdx, &userInfo));

    if (userInfo.priv < 0)
        return SYS_ERR_PASSWORD_INVALID;

    if (SYS_PASSWD_TYPE_ENCRYPT == userInfo.passwdType)
    {
        SYS_ERR_CHK(sal_util_password_encrypt(password, userInfo.password, crypt_password));

        if (0 == osal_strcmp(userInfo.password, crypt_password))
        {
            *pPriv = userInfo.priv;

            return SYS_ERR_OK;
        }
    }
    else if (SYS_PASSWD_TYPE_CLEARTEXT == userInfo.passwdType)
    {
        if (0 == osal_strcmp(userInfo.password, password))
        {
            *pPriv = userInfo.priv;

            return SYS_ERR_OK;
        }
    }
    else
    {
        *pPriv = userInfo.priv;

        return SYS_ERR_OK;
    }

    return SYS_ERR_PASSWORD_INVALID;
}

int32 sal_aaa_localEnablePassword_authen(int32 priv, char *password)
{
    char crypt_password[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    sys_enblPasswd_t enablePassword;

    SYS_PARAM_CHK((NULL == password), SYS_ERR_NULL_POINTER);

    osal_memset(crypt_password, 0, sizeof(crypt_password));
    osal_memset(&enablePassword, 0, sizeof(enablePassword));

    SYS_ERR_CHK(sal_sys_enablePassword_get(priv, &enablePassword));

    if (SYS_PASSWD_TYPE_ENCRYPT == enablePassword.passwdType)
    {
        SYS_ERR_CHK(sal_util_password_encrypt(password, enablePassword.password, crypt_password));

        if (0 == osal_strcmp(enablePassword.password, crypt_password))
            return SYS_ERR_OK;
    }
    else
    {
        if (0 == osal_strcmp(enablePassword.password, password))
            return SYS_ERR_OK;
    }

    return SYS_ERR_PASSWORD_INVALID;
}

#ifdef CONFIG_SYS_LIB_TACPLUS

int32 sal_aaa_tacplusSession_init(struct session **ppTacplusSession)
{
    uint32 i;
    uint32 usedCount = 0;
    int32 prio = 0;
    int32 foundPrioIdx = 0;
    int32 prioArry[CAPA_TACPLUS_HOST_NUM_MAX];
    int32 usedArry[CAPA_TACPLUS_HOST_NUM_MAX];
    sys_tacplus_t tacplusSrv;

    osal_memset(prioArry, 0, sizeof(prioArry));
    osal_memset(usedArry, 0, sizeof(usedArry));
    osal_memset(&tacplusSrv, 0, sizeof(tacplusSrv));

    for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
    {
        SYS_ERR_CHK(cfg_tacplus_server_get(i, &tacplusSrv));

        prioArry[i] = tacplusSrv.priority;

        if (TACPLUS_NOT_UESD_FLAG == tacplusSrv.priority)
        {
            usedArry[i] = TRUE;
            usedCount++;
        }
    }

    if (CAPA_TACPLUS_HOST_NUM == usedCount)
    {
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_TACACS_EMPTY);

        return SYS_ERR_TACPLUS_SRV_EMPTY;
    }

    while (usedCount < CAPA_TACPLUS_HOST_NUM)
    {
        prio = 0xFFFF;
        foundPrioIdx = -1;

        for (i = 0; i < CAPA_TACPLUS_HOST_NUM; i++)
        {
            if (TRUE == usedArry[i])
                continue;

            if ((TACPLUS_NOT_UESD_FLAG != prioArry[i]) && (prioArry[i] <= prio))
            {
                prio = prioArry[i];
                foundPrioIdx = i;
            }
        }

        if (-1 == foundPrioIdx)
            break;

        usedCount++;
        usedArry[foundPrioIdx] = TRUE;

        SYS_ERR_CHK(cfg_tacplus_server_get(foundPrioIdx, &tacplusSrv));

        if (NULL != (*ppTacplusSession = tac_connect(tacplusSrv.srvHost,
                                                      tacplusSrv.timeout,
                                                      tacplusSrv.key,
                                                      tacplusSrv.srvPort)))
        {
            SYS_LOG(LOG_CAT_AAA, LOG_AAA_TACACS_CONN_SUCCESS,
                    tacplusSrv.srvHost,
                    tacplusSrv.srvPort);

            return SYS_ERR_OK;
        }

        SYS_LOG(LOG_CAT_AAA, LOG_AAA_TACACS_CONN_FAIL,
                tacplusSrv.srvHost,
                tacplusSrv.srvPort);
    }

    return SYS_ERR_TACPLUS_SRV_ERR;
}

int32 sal_aaa_tacplusSession_close(struct session *pTacplusSession)
{
    SYS_PARAM_CHK((NULL == pTacplusSession), SYS_ERR_NULL_POINTER);

    tac_close(pTacplusSession);

    return SYS_ERR_OK;
}

int32 sal_aaa_tacplusLoginUsername_authen(
    struct session *pTacplusSession,
    char           *username)
{
    char serv_msg[256];
    char data_msg[256];
    int32 ret = -1;

    SYS_PARAM_CHK((NULL == pTacplusSession), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == username), SYS_ERR_NULL_POINTER);

    osal_memset(serv_msg, 0, sizeof(serv_msg));
    osal_memset(data_msg, 0, sizeof(data_msg));

    tac_authen_send_start(pTacplusSession, SYS_AAA_TACPLUS_TTY_PORT, username, TACACS_ASCII_LOGIN, "");

    ret = tac_authen_get_reply(pTacplusSession, serv_msg, data_msg);

    if (ret <= 0)
        return SYS_ERR_TACPLUS_SRV_ERR; 

    if (TAC_PLUS_AUTHEN_STATUS_PASS == ret || TAC_PLUS_AUTHEN_STATUS_GETPASS == ret)
        return SYS_ERR_OK;

    if (TAC_PLUS_AUTHEN_STATUS_FAIL == ret)
    {
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_TACACS_USER_INVALID,
                    username);

        return SYS_ERR_USERNAME_INVALID;
    }
    else
    {
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_TACACS_REPLYMSG,
                    ret,
                    serv_msg);

        return SYS_ERR_USERNAME_INVALID;
    }

    return SYS_ERR_OK;
}

int32 sal_aaa_tacplusLoginPassword_authen(
    struct session *pTacplusSession,
    char           *password)
{
    char serv_msg[256];
    char data_msg[256];
    int32 ret;

    SYS_PARAM_CHK((NULL == pTacplusSession), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == password), SYS_ERR_NULL_POINTER);

    osal_memset(serv_msg, 0, sizeof(serv_msg));
    osal_memset(data_msg, 0, sizeof(data_msg));

    tac_authen_send_cont(pTacplusSession, password, "");

    ret = tac_authen_get_reply(pTacplusSession, serv_msg, data_msg);

    if (ret <= 0)
        return SYS_ERR_TACPLUS_SRV_ERR; 

    if (ret != TAC_PLUS_AUTHEN_STATUS_PASS)
    {
        if (ret != TAC_PLUS_AUTHEN_STATUS_FAIL)
            SYS_LOG(LOG_CAT_AAA, LOG_AAA_TACACS_REPLYMSG,
                        ret,
                        serv_msg);

        return SYS_ERR_PASSWORD_INVALID;
    }

    return SYS_ERR_OK;
}
#endif 

#ifdef CONFIG_SYS_LIB_RADIUS

int32 sal_aaa_radiusServerRank_get(int32 rank, int32 *pServerIndex)
{
    char radSrvIpStr[16];
    int32 prio;
    int32 foundPrioIdx;
    int32 prioArry[CAPA_RADIUS_HOST_NUM_MAX];
    int32 usedArry[CAPA_RADIUS_HOST_NUM_MAX];
    uint32 i;
    uint32 usedCount = 0;
    sys_radius_t radiusSrv;

    SYS_PARAM_CHK((NULL == pServerIndex), SYS_ERR_NULL_POINTER);

    osal_memset(prioArry, 0, sizeof(prioArry));
    osal_memset(usedArry, 0, sizeof(usedArry));
    osal_memset(&radiusSrv, 0, sizeof(radiusSrv));

    for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
    {
        SYS_ERR_CHK(cfg_radiusSrv_get(i, &radiusSrv));

        prioArry[i] = radiusSrv.priority;

        if (RADIUS_NOT_UESD_FLAG == radiusSrv.priority || SYS_RADIUS_SRV_USGTYPE_8021X == radiusSrv.usgType)
        {
            usedArry[i] = TRUE;
            usedCount++;
        }
    }

    if (CAPA_RADIUS_HOST_NUM == usedCount)
        return SYS_ERR_RADIUS_SRV_EMPTY;

    while (usedCount < CAPA_RADIUS_HOST_NUM)
    {
        prio = 0xFFFF;
        foundPrioIdx = -1;

        for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
        {
            if (TRUE == usedArry[i])
                continue;

            if (prioArry[i] != RADIUS_NOT_UESD_FLAG && prioArry[i] < prio)
            {
                prio = prioArry[i];
                foundPrioIdx = i;
            }
        }

        if (-1 == foundPrioIdx)
            break;

        if (usedCount == rank)
        {
            *pServerIndex = foundPrioIdx;

            return SYS_ERR_OK;
        }

        usedCount++;
        usedArry[foundPrioIdx] = TRUE;

        SYS_ERR_CHK(cfg_radiusSrv_get(foundPrioIdx, &radiusSrv));
        SYS_ERR_CHK(sys_util_ip2Str(radiusSrv.srvIp, radSrvIpStr));

    }

    return SYS_ERR_RADIUS_SRV_ERR;
}

int32 sal_aaa_radiusLoginUsername_authen(char *user, char *passwd)
{
    char nas_ip_str[AAA_MIDDLE_STR_LENGTH];
    int32 err = 0;
    int32 ret = SYS_ERR_RADIUS_SRV_ERR;
    uint32 nas_ip;
    uint32 netmask;
    int32 prio;
    int32 foundPrioIdx;
    int32 prioArry[CAPA_RADIUS_HOST_NUM_MAX];
    int32 usedArry[CAPA_RADIUS_HOST_NUM_MAX];
    uint32 i;
    uint32 usedCount = 0;
    struct in_addr nas_addr;
    struct rad_handle *h;
    sys_radius_t radiusSrv;

    SYS_PARAM_CHK((NULL == user), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == passwd), SYS_ERR_NULL_POINTER);

    osal_memset(nas_ip_str, 0, sizeof(nas_ip_str));
    osal_memset(prioArry, 0, sizeof(prioArry));
    osal_memset(usedArry, 0, sizeof(usedArry));

    SYS_ERR_CHK(sal_sys_ip_get(&nas_ip, &netmask));
    SYS_ERR_CHK(sys_util_ip2Str(nas_ip,nas_ip_str));
    inet_aton(nas_ip_str, &nas_addr);

    for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
    {
        SYS_ERR_CHK(cfg_radiusSrv_get(i, &radiusSrv));

        prioArry[i] = radiusSrv.priority;

        if (RADIUS_NOT_UESD_FLAG == radiusSrv.priority || SYS_RADIUS_SRV_USGTYPE_8021X == radiusSrv.usgType)
        {
            usedArry[i] = TRUE;
            usedCount++;
        }
    }

    if (CAPA_RADIUS_HOST_NUM == usedCount)
    {
        SYS_LOG(LOG_CAT_AAA, LOG_AAA_RADIUS_EMPTY);

        return SYS_ERR_RADIUS_SRV_EMPTY;
    }

    while (usedCount < CAPA_RADIUS_HOST_NUM)
    {
        prio = 0xFFFF;
        foundPrioIdx = -1;

        for (i = 0; i < CAPA_RADIUS_HOST_NUM; i++)
        {
            if (TRUE == usedArry[i])
               continue;

            if ((RADIUS_NOT_UESD_FLAG != prioArry[i]) && (prioArry[i] <= prio))
            {
               prio = prioArry[i];
               foundPrioIdx = i;
            }
        }

        if (-1 == foundPrioIdx)
            break;

        usedCount++;
        usedArry[foundPrioIdx] = TRUE;

        SYS_ERR_CHK(cfg_radiusSrv_get(foundPrioIdx, &radiusSrv));

        h = rad_auth_open();

        if (NULL == h)
        {
            continue;
        }

        if (0 != osal_strlen(radiusSrv.hostName))
        {
            err = rad_add_server(h,
                                 radiusSrv.hostName,
                                 radiusSrv.srvPort,
                                 radiusSrv.key,
                                 radiusSrv.timeout,
                                 radiusSrv.retransmit);
            if (0 != err)
            {
                if (NULL != h)
                    rad_close(h);

                SYS_LOG(LOG_CAT_AAA, LOG_AAA_TACACS_CONN_FAIL,
                            radiusSrv.hostName,
                            radiusSrv.srvPort);

                continue;
            }
        }

        rad_create_request(h, RAD_ACCESS_REQUEST);
        rad_put_addr(h, RAD_NAS_IP_ADDRESS, nas_addr);
        rad_put_string(h, RAD_USER_NAME, user);
        rad_put_string(h, RAD_USER_PASSWORD, passwd);

        err = rad_send_request(h);

        if (err < 0)
        {
            ret = SYS_ERR_RADIUS_SRV_ERR; 

            SYS_LOG(LOG_CAT_AAA, LOG_AAA_RADIUS_CONN_FAIL,
                        radiusSrv.hostName,
                        radiusSrv.srvPort);
        }
        else
        {
            SYS_LOG(LOG_CAT_AAA, LOG_AAA_RADIUS_CONN_SUCCESS,
                        radiusSrv.hostName,
                        radiusSrv.srvPort);
        }

        switch (err)
        {
            case RAD_ACCESS_ACCEPT:
                ret = SYS_ERR_OK;
                break;

            case RAD_ACCESS_REJECT:
                ret = SYS_ERR_RADIUS_AUTH_REJECT;
                break;

            case RAD_ACCESS_CHALLENGE:
                ret = SYS_ERR_OK;
                break;

            case SYS_ERR_RADIUS_SRV_ERR:
                break;

            default:
                ret = SYS_ERR_USERNAME_INVALID;
                break;
        }

        if (NULL != h)
            rad_close(h);

        if (SYS_ERR_OK == ret || SYS_ERR_RADIUS_AUTH_REJECT == ret)
            return ret;
        else
            continue;

    }

    if (err < 0)
    {
        ret = SYS_ERR_RADIUS_SRV_ERR; 
    }

    return ret;
}
#endif 

int32 sal_aaa_loginAuthUserAccessMethod(sys_access_type_t type, char *username)
{
    uint32 i;
    sys_userInfo_t userInfo;
    uint32 access = 0x01 << type;

    SYS_PARAM_CHK((NULL == username), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_SYS_LOCALUSER_NUM; i++)
    {
        SYS_ERR_CHK(cfg_sys_localUser_get(i, &userInfo));

        if (userInfo.priv < 0)
            continue;

        if (0 == osal_strcmp(userInfo.username, username))
        {
            if(userInfo.accessMethod & access)
            	return SYS_ERR_OK;
        }
    }

    return SYS_ERR_USERNAME_ACCESSMETH;
}

