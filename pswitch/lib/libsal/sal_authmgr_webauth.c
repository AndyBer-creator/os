
#include <libsal/sal_util.h>
#include <libsal/sal_authmgr.h>

int32 sal_authmgr_portWebauthLoginAttempt_set(sys_logic_port_t port, uint32 loginAttemp)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(loginAttemp != 0 &&
                  ((loginAttemp < BOND_AUTHMGR_WEBAUTH_LOGIN_NUM_MIN)
                  || (loginAttemp > BOND_AUTHMGR_WEBAUTH_LOGIN_NUM_MAX)), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.loginAttempt = loginAttemp;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portWebauthLoginAttempt_get(sys_logic_port_t port, uint32 *pLoginAttemp)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pLoginAttemp), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pLoginAttemp = portCfg.loginAttempt;

    return SYS_ERR_OK;
}

int32 sal_authmgr_webauthLocalInfo_set(authmgr_webauth_local_t localInfo)
{
    int32 idx = -1;
    int32 emptyIdx = -1;
    uint32 i = 0;
    authmgr_webauth_local_t tmpLocalInfo;

    for (i = 0; i < CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpLocalInfo);

        SYS_ERR_CONTINUE(cfg_authmgr_webauthLocalInfo_get(i, &tmpLocalInfo));

        if (-1 == emptyIdx)
        {
            if (0 == osal_strlen(tmpLocalInfo.username))
            {
                emptyIdx = i;

                continue;
            }
        }

        if (0 == osal_strncmp(localInfo.username, tmpLocalInfo.username, CAPA_SYS_USERNAME_LEN))
        {
            idx = i;

            break;
        }
    }

    if (-1 != idx)
    {
        SYS_ERR_CHK(cfg_authmgr_webauthLocalInfo_set(idx, localInfo));
    }
    else
    {
        if (-1 != emptyIdx)
        {
            SYS_ERR_CHK(cfg_authmgr_webauthLocalInfo_set(emptyIdx, localInfo));
        }
        else
        {
            return SYS_ERR_ENTRY_FULL;
        }
    }

    return SYS_ERR_OK;
}

int32 sal_authmgr_webauthLocalInfo_del(char *username)
{
    uint32 i = 0;
    authmgr_webauth_local_t localInfo;

    for (i = 0; i < CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(localInfo);

        SYS_ERR_CONTINUE(cfg_authmgr_webauthLocalInfo_get(i, &localInfo));

        if (0 == osal_strncmp(username, localInfo.username, CAPA_SYS_USERNAME_LEN))
        {
            SYS_MEM_CLEAR(localInfo);

            SYS_ERR_CHK(cfg_authmgr_webauthLocalInfo_set(i, localInfo));

            break;
        }
    }

    return SYS_ERR_OK;
}

int32 sal_authmgr_webauthLocalInfo_getByIdx(uint32 index, authmgr_webauth_local_t *pLocalInfo)
{
    SYS_PARAM_CHK((index >= CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pLocalInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_authmgr_webauthLocalInfo_get(index, pLocalInfo));

    return SYS_ERR_OK;
}

int32 sal_authmgr_webauthIpv6Neigh_refresh(void)
{
    static sys_ipv6_t sysIpv6;
    static sys_ipv6_t natIpv6;
    static sys_mac_t sysMac;
    char ipv6Str[CAPA_IPV6_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    char cmd[SYS_BUF256_LEN];

    SYS_MEM_CLEAR(sysIpv6);
    SYS_MEM_CLEAR(natIpv6);
    SYS_MEM_CLEAR(sysMac);
    SYS_MEM_CLEAR(ipv6Str);
    SYS_MEM_CLEAR(macStr);

    SYS_ERR_CHK(sal_sys_macAddr_get(&sysMac));

    sysIpv6.hex[15] = sysMac.octet[5];
    sysIpv6.hex[14] = sysMac.octet[4];
    sysIpv6.hex[13] = sysMac.octet[3];
    sysIpv6.hex[12] = 0xfe;
    sysIpv6.hex[11] = 0xff;
    sysIpv6.hex[10] = sysMac.octet[2];
    sysIpv6.hex[9] = sysMac.octet[1];
    sysIpv6.hex[8] = sysMac.octet[0];

    if (sysIpv6.hex[8] & (1 << 1))
        sysIpv6.hex[8] -=0x2;
    else
        sysIpv6.hex[8] +=0x2;

    sysIpv6.hex[1] = 0x80;
    sysIpv6.hex[0] = 0xfe;

    osal_memcpy(&natIpv6, &sysIpv6, sizeof(sys_ipv6_t));

    if ((natIpv6.hex[15] + 1) > 0xff)
    {
        if (0xff == natIpv6.hex[14])
            natIpv6.hex[13] +=1;

        natIpv6.hex[14]+=1;
    }

    natIpv6.hex[15] += 1;

    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "ip -6 neigh flush dev eth0 1> /dev/null 2> /dev/null");
    system(cmd);

    SYS_MEM_CLEAR(cmd);
    inet_ntop(AF_INET6, natIpv6.hex, ipv6Str, sizeof(ipv6Str));
    sys_util_mac2Str(sysMac.octet, macStr);
    osal_sprintf(cmd, "ip -6 neigh add %s lladdr %s dev eth0 1> /dev/null 2> /dev/null", ipv6Str, macStr);
    system(cmd);

    return SYS_ERR_OK;
}

int32 sal_authmgr_webauthStatusByKey_get(uint32 key, authmgr_status_t *pStatus)
{
    SYS_PARAM_CHK((key > 65535), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pStatus), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(authmgr_webauthStatusByKey_get(key, pStatus));

    return SYS_ERR_OK;
}

int32 sal_authmgr_webauthSuppInfoByKey_get(uint32 key, authmgr_suppInfo_t *pSupp)
{
    SYS_PARAM_CHK((key > 65535), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pSupp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(authmgr_webauthSuppInfoByKey_get(key, pSupp));

    return SYS_ERR_OK;
}

int32 sal_authmgr_webauthLocal_authen(char *username, char *password, webauth_authInfo_t *pAuthInfo)
{
    char pass[CAPA_SYS_CRYPT_PASSWORD_LEN + 1];
    uint32 i;
    authmgr_webauth_local_t localInfo;

    SYS_PARAM_CHK((NULL == username), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == password), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pAuthInfo), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_AUTHMGR_WEBAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(localInfo);

        SYS_ERR_CONTINUE(sal_authmgr_webauthLocalInfo_getByIdx(i, &localInfo));

        if (0 == osal_strlen(localInfo.username))
            continue;

        if (0 == osal_strcmp(localInfo.username, username))
        {
            SYS_MEM_CLEAR(pass);

            sal_util_str_decrypt(localInfo.password, pass);

            if (0 == osal_strcmp(pass, password))
            {
                osal_memcpy(&pAuthInfo->author, &localInfo.authorInfo, sizeof(authmgr_author_t));
                osal_strncpy(pAuthInfo->username, username, CAPA_SYS_USERNAME_LEN);

                if (pAuthInfo->author.authVid != 0)
                    pAuthInfo->authorSrc.vidSrc = AUTHMGR_AUTHOR_SRC_LOCAL;

                if (pAuthInfo->author.reauthTime != 0)
                    pAuthInfo->authorSrc.reauthTimeSrc = AUTHMGR_AUTHOR_SRC_LOCAL;

                if (pAuthInfo->author.inactTime != 0)
                    pAuthInfo->authorSrc.inactTimeSrc = AUTHMGR_AUTHOR_SRC_LOCAL;

                return SYS_ERR_OK;
            }

            break;
        }
    }

    return SYS_ERR_FAILED;
}

int32 sal_authmgr_webauthRadius_authen(char *username, char *password, webauth_authInfo_t *pAuthInfo)
{
    char *s = NULL;
    char nas_ip_str[CAPA_IPV4_STR_LEN];
    const void *data = NULL;
    char *pVlanStr = NULL;
    int32 attrtype;
    int32 err = 0;
    int32 ret = SYS_ERR_FAILED;
    uint32 nas_ip;
    uint32 netmask;
    int32 prio;
    int32 foundPrioIdx;
    int32 prioArry[CAPA_RADIUS_HOST_NUM_MAX];
    int32 usedArry[CAPA_RADIUS_HOST_NUM_MAX];
    uint32 i;
    uint32 usedCount = 0;
    size_t len;
    struct in_addr nas_addr;
    struct rad_handle *h;
    sys_radius_t radiusSrv;
    uint32 tunnelType = 0;
    uint32 mediumType = 0;
    sys_vid_t vid = 0;

    SYS_PARAM_CHK((NULL == username), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == password), SYS_ERR_NULL_POINTER);

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

        if (RADIUS_NOT_UESD_FLAG == radiusSrv.priority)
        {
            usedArry[i] = TRUE;
            usedCount++;
        }
    }

    if (CAPA_RADIUS_HOST_NUM == usedCount)
    {
        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_EMPTY);

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

            if ((0 != prioArry[i]) && (prioArry[i] < prio))
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

        if (osal_strlen(radiusSrv.hostName) > 0)
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

                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_CONN_FAIL,
                            radiusSrv.hostName,
                            radiusSrv.srvPort);

                continue;
            }
        }

        rad_create_request(h, RAD_ACCESS_REQUEST);
        rad_put_addr(h, RAD_NAS_IP_ADDRESS, nas_addr);
        rad_put_string(h, RAD_USER_NAME, username);
        rad_put_string(h, RAD_USER_PASSWORD, password);

        err = rad_send_request(h);

        if (err < 0)
        {
            ret = SYS_ERR_RADIUS_SRV_ERR; 

            SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_CONN_FAIL,
                        radiusSrv.hostName,
                        radiusSrv.srvPort);
        }
        else
        {
            SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_CONN_SUCCESS,
                        radiusSrv.hostName,
                        radiusSrv.srvPort);
        }

        switch (err)
        {
            case RAD_ACCESS_ACCEPT:
                while ((attrtype = rad_get_attr(h, &data, &len)) > 0)
                {
                    switch (attrtype)
                    {
                        case RAD_SESSION_TIMEOUT:
                            pAuthInfo->author.reauthTime = rad_cvt_int(data);
                            if (0 == pAuthInfo->author.reauthTime)
                                pAuthInfo->authorSrc.reauthTimeSrc = AUTHMGR_AUTHOR_SRC_CFG;
                            else
                                pAuthInfo->authorSrc.reauthTimeSrc = AUTHMGR_AUTHOR_SRC_RADIUS;
                            break;

                        case RAD_IDLE_TIMEOUT:
                            pAuthInfo->author.inactTime = rad_cvt_int(data);
                            if (0 == pAuthInfo->author.inactTime)
                                pAuthInfo->authorSrc.inactTimeSrc = AUTHMGR_AUTHOR_SRC_CFG;
                            else
                                pAuthInfo->authorSrc.inactTimeSrc = AUTHMGR_AUTHOR_SRC_RADIUS;
                            break;

                        case RAD_TUNNEL_TYPE:
                            tunnelType = rad_cvt_int(data);
                            
                            tunnelType = (tunnelType & 0x00FFFFFF);

                            if (RAD_TUNNEL_VLAN != tunnelType)
                                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_TUNNELTYPE_INVALID,
                                            tunnelType);
                            break;

                        case RAD_TUNNEL_MEDIUM_TYPE:
                            mediumType = rad_cvt_int(data);
                            
                            mediumType = (mediumType & 0x00FFFFFF);

                            if (RAD_IEEE_802 != mediumType)
                                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_TUNNELMEDIUMTYPE_INVALID,
                                            mediumType);
                            break;

                        case RAD_TUNNEL_PRIVATE_GROUP_ID:
                            s = rad_cvt_string(data, len);
                            
                            if (*s <= 0x1F)
                                pVlanStr = s + 1;
                            else
                                pVlanStr = s;

                            vid = SYS_STR2UINT(pVlanStr);
                            if (IS_VID_INVALID(vid))
                            {
                                sys_namestr_t vlanName;
                                sys_vid_t _vid = vid;

                                SYS_MEM_CLEAR(vlanName);

                                vid = 0;

                                osal_strncpy(vlanName.str, pVlanStr, sizeof(vlanName.str));
                                sal_vlan_vidByName_get(&vlanName, &vid);

                                if (0 == _vid && 0 == vid)
                                {
                                    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_VLAN_NOTFOUND,
                                                vlanName.str);
                                }
                                else if (0 != _vid && 0 == vid)
                                {
                                    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_VLAN_INVALID,
                                                _vid);
                                }
                            }
                            else if (!IS_VLAN_OPER_ENTRY_EXIST(vid))
                            {
                                vid = 0;

                                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_VLAN_NOTFOUND,
                                           pVlanStr);
                            }
                            osal_free(s);
                            break;

                        default:
                                break;
                    }
                }

                pAuthInfo->authorSrc.vidSrc = AUTHMGR_AUTHOR_SRC_RADIUS;
                if (RAD_TUNNEL_VLAN == tunnelType && RAD_IEEE_802 == mediumType && vid != 0)
                {
                    pAuthInfo->author.authVid = vid;
                }

                ret = SYS_ERR_OK;
                break;

            case RAD_ACCESS_REJECT:
                ret = SYS_ERR_RADIUS_AUTH_REJECT;
                break;

            case RAD_ACCESS_CHALLENGE:
                ret = SYS_ERR_OK;
                break;

            case SYS_ERR_RADIUS_SRV_ERR:
                ret = SYS_ERR_FAILED;
                break;

            default:
                ret = SYS_ERR_USERNAME_INVALID;
                break;
        }

        if (NULL != h)
            rad_close(h);

        if (SYS_ERR_OK == ret)
            return SYS_ERR_OK;
        else
            continue;

    }

    return ret;
}

int32 sal_authmgr_webauthStatus_set(webauth_authInfo_t *pAuthInfo)
{
    SYS_PARAM_CHK((NULL == pAuthInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(authmgr_webauthStatus_set(pAuthInfo));

    return SYS_ERR_OK;
}

