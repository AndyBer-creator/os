
#include <libsal/sal_util.h>
#include <libsal/sal_authmgr.h>

int32 sal_authmgr_radiusUserFmt_set(authmgr_radiusUserFmt_t userFmt)
{
    SYS_ERR_CHK(cfg_authmgr_radiusUserFmt_set(userFmt));

#if 0
    SYS_LOG(LOG_SEV_DEBUG, LOG_CAT_MACAUTH, LOG_MACAUTH_RADIUS_USER_FMT,
            userFmt.delimiter,
            userFmt.macCase,
            userFmt.gap);
#endif

    return SYS_ERR_OK;
}

int32 sal_authmgr_radiusUserFmt_get(authmgr_radiusUserFmt_t *pUserFmt)
{
    SYS_PARAM_CHK((NULL == pUserFmt), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_authmgr_radiusUserFmt_get(pUserFmt));

    return SYS_ERR_OK;
}

int32 sal_authmgr_radiusUser_get(sys_mac_t *pMac,
                                            authmgr_radiusUserFmt_t *pUserFmt,
                                            char *pUserStr)
{
    authmgr_radiusUserFmt_t userFmt;
    char delimiter = 0;
    char buf[SYS_BUF32_LEN];
    uint32 i = 0;
    uint32 j = 0;

    SYS_PARAM_CHK((NULL == pUserStr), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(userFmt);

    if (NULL == pUserFmt)
        SYS_ERR_CHK(cfg_authmgr_radiusUserFmt_get(&userFmt));
    else
        osal_memcpy(&userFmt, pUserFmt, sizeof(authmgr_radiusUserFmt_t));

    switch (userFmt.delimiter)
    {
        case AUTHMGR_DELIMITER_COLON:
            delimiter = ':';
            break;

        case AUTHMGR_DELIMITER_HYPHEN:
            delimiter = '-';
            break;

        case AUTHMGR_DELIMITER_DOT:
            delimiter = '.';
            break;

        default:
            break;
    }

    SYS_MEM_CLEAR(buf);

    if (NULL == pMac)
    {
        if (AUTHMGR_MACCASE_UPPER == userFmt.macCase)
            osal_sprintf(buf, "XXXXXXXXXXXX");
        else
            osal_sprintf(buf, "xxxxxxxxxxxx");
    }
    else
    {
        if (AUTHMGR_MACCASE_UPPER == userFmt.macCase)
            osal_sprintf(buf, "%02X%02X%02X%02X%02X%02X", pMac->octet[0],
                                                          pMac->octet[1],
                                                          pMac->octet[2],
                                                          pMac->octet[3],
                                                          pMac->octet[4],
                                                          pMac->octet[5]);
        else
            osal_sprintf(buf, "%02x%02x%02x%02x%02x%02x", pMac->octet[0],
                                                          pMac->octet[1],
                                                          pMac->octet[2],
                                                          pMac->octet[3],
                                                          pMac->octet[4],
                                                          pMac->octet[5]);
    }

    if (AUTHMGR_DELIMITER_NONE != userFmt.delimiter)
    {
        j = 0;

        for (i = 0; i < 12; i++)
        {
            *(pUserStr + j) = buf[i];
            j++;

            if (((i + 1) % userFmt.gap == 0) && (i + 1) != 12)
            {
                *(pUserStr + j) = delimiter;
                j++;
            }
        }
    }
    else
    {
        osal_strcpy(pUserStr, buf);
    }

    return SYS_ERR_OK;
}

int32 sal_authmgr_macauthLocalInfo_set(authmgr_macauth_local_t localInfo)
{
    int32 idx = -1;
    int32 emptyIdx = -1;
    uint32 i = 0;
    sys_mac_t emptyMac;
    authmgr_macauth_local_t tmpLocalInfo;

    SYS_MEM_CLEAR(emptyMac);

    for (i = 0; i < CAPA_AUTHMGR_MACAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(tmpLocalInfo);

        SYS_ERR_CONTINUE(cfg_authmgr_macauthLocalInfo_get(i, &tmpLocalInfo));

        if (-1 == emptyIdx)
        {
            if (0 == osal_memcmp(&tmpLocalInfo.mac, &emptyMac, sizeof(sys_mac_t)))
            {
                emptyIdx = i;

                continue;
            }
        }

        if (0 == osal_memcmp(&localInfo.mac, &tmpLocalInfo.mac, sizeof(sys_mac_t)))
        {
            idx = i;

            break;
        }
    }

    if (-1 != idx)
    {
        SYS_ERR_CHK(cfg_authmgr_macauthLocalInfo_set(idx, localInfo));
    }
    else
    {
        if (-1 != emptyIdx)
        {
            SYS_ERR_CHK(cfg_authmgr_macauthLocalInfo_set(emptyIdx, localInfo));
        }
        else
        {
            return SYS_ERR_ENTRY_FULL;
        }
    }

    return SYS_ERR_OK;
}

int32 sal_authmgr_macauthLocalInfo_del(sys_mac_t mac)
{
    uint32 i = 0;
    authmgr_macauth_local_t localInfo;

    for (i = 0; i < CAPA_AUTHMGR_MACAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(localInfo);

        SYS_ERR_CONTINUE(cfg_authmgr_macauthLocalInfo_get(i, &localInfo));

        if (0 == osal_memcmp(&mac, &localInfo.mac, sizeof(sys_mac_t)))
        {
            SYS_MEM_CLEAR(localInfo);

            SYS_ERR_CHK(cfg_authmgr_macauthLocalInfo_set(i, localInfo));

            break;
        }
    }

    return SYS_ERR_OK;
}

int32 sal_authmgr_macauthLocalInfo_getByIdx(uint32 index, authmgr_macauth_local_t *pLocalInfo)
{
    SYS_PARAM_CHK((index >= CAPA_AUTHMGR_MACAUTH_LOCAL_NUM), SYS_ERR_INPUT);
    SYS_PARAM_CHK((NULL == pLocalInfo), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_authmgr_macauthLocalInfo_get(index, pLocalInfo));

    return SYS_ERR_OK;
}

int32 sal_authmgr_macauthLocal_authen(macauth_authInfo_t *pAuthInfo)
{
    uint32 i = 0;
    authmgr_macauth_local_t localInfo;

    SYS_PARAM_CHK((NULL == pAuthInfo), SYS_ERR_NULL_POINTER);

    for (i = 0; i < CAPA_AUTHMGR_MACAUTH_LOCAL_NUM; i++)
    {
        SYS_MEM_CLEAR(localInfo);

        SYS_ERR_CONTINUE(cfg_authmgr_macauthLocalInfo_get(i, &localInfo));

        if (0 == osal_memcmp(&localInfo.mac, &pAuthInfo->mac, sizeof(sys_mac_t)))
        {
            pAuthInfo->ctrl = localInfo.state;

            if (AUTHMGR_PORTCTRL_FORCE_AUTHORIZED == localInfo.state)
            {
                osal_memcpy(&pAuthInfo->author, &localInfo.authorInfo, sizeof(authmgr_author_t));

                if (pAuthInfo->author.authVid != 0)
                    pAuthInfo->authorSrc.vidSrc = AUTHMGR_AUTHOR_SRC_LOCAL;

                if (pAuthInfo->author.reauthTime != 0)
                    pAuthInfo->authorSrc.reauthTimeSrc = AUTHMGR_AUTHOR_SRC_LOCAL;

                if (pAuthInfo->author.inactTime != 0)
                    pAuthInfo->authorSrc.inactTimeSrc = AUTHMGR_AUTHOR_SRC_LOCAL;

                return SYS_ERR_OK;
            }
            else
            {
                return SYS_ERR_FAILED;
            }
        }
    }

    return SYS_ERR_FAILED;
}

int32 sal_authmgr_macauthRadius_authen(macauth_authInfo_t *pAuthInfo)
{
    char *s = NULL;
    char macStr[CAPA_MAC_STR_LEN];
    char nas_ip_str[CAPA_IPV4_STR_LEN];
    const void *data = NULL;
    char *pVlanStr = NULL;
    int32 attrtype;
    int32 err = 0;
    int32 ret = 0;
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

    SYS_PARAM_CHK((NULL == pAuthInfo), SYS_ERR_NULL_POINTER);

    osal_memset(nas_ip_str, 0, sizeof(nas_ip_str));
    osal_memset(prioArry, 0, sizeof(prioArry));
    osal_memset(usedArry, 0, sizeof(usedArry));
    SYS_MEM_CLEAR(macStr);

    SYS_ERR_CHK(sal_sys_ip_get(&nas_ip, &netmask));
    SYS_ERR_CHK(sys_util_ip2Str(nas_ip,nas_ip_str));
    inet_aton(nas_ip_str, &nas_addr);

    SYS_ERR_CHK(sal_authmgr_radiusUser_get(&pAuthInfo->mac, NULL, macStr));

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

            if ((RADIUS_NOT_UESD_FLAG != prioArry[i]) && (prioArry[i] < prio))
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
        rad_put_string(h, RAD_USER_NAME, macStr);
        rad_put_string(h, RAD_USER_PASSWORD, macStr);

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

int32 sal_authmgr_macauthStatus_set(macauth_authInfo_t *pAuthInfo)
{
    SYS_ERR_CHK(authmgr_macauthStatus_set(pAuthInfo));

    return SYS_ERR_OK;
}

