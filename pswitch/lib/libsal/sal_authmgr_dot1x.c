
#include <libsal/sal_util.h>
#include <libsal/sal_authmgr.h>

int32 sal_authmgr_portMaxEapReq_set(sys_logic_port_t port, uint32 maxReq)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((maxReq < BOND_AUTHMGR_MAX_REQ_MIN) || (maxReq > BOND_AUTHMGR_MAX_REQ_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.maxReq = maxReq;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portMaxEapReq_get(sys_logic_port_t port, uint32 *pMaxReq)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMaxReq), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pMaxReq = portCfg.maxReq;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portEapTxPeriod_set(sys_logic_port_t port, uint32 time)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((time < BOND_AUTHMGR_EAP_TX_PERIOD_MIN) || (time > BOND_AUTHMGR_EAP_TX_PERIOD_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.txPeriod = time;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portEapTxPeriod_get(sys_logic_port_t port, uint32 *pTime)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pTime = portCfg.txPeriod;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portSuppTimeout_set(sys_logic_port_t port, uint32 time)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((time < BOND_AUTHMGR_SUPP_TIMEOUT_MIN) || (time > BOND_AUTHMGR_SUPP_TIMEOUT_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.suppTimeout = time;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portSuppTimeout_get(sys_logic_port_t port, uint32 *pTime)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pTime = portCfg.suppTimeout;

    return SYS_ERR_OK;
}

int32 sal_authmgr_portSrvTimeout_set(sys_logic_port_t port, uint32 time)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((time < BOND_AUTHMGR_SRV_TIMEOUT_MIN) || (time > BOND_AUTHMGR_SRV_TIMEOUT_MAX), SYS_ERR_INPUT);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    portCfg.serverTimeout = time;

    SYS_ERR_CHK(cfg_authmgr_port_set(port, &portCfg));
    SYS_ERR_CHK(authmgr_portCfg_set(port, &portCfg));

    return SYS_ERR_OK;
}

int32 sal_authmgr_portSrvTimeout_get(sys_logic_port_t port, uint32 *pTime)
{
    authmgr_cfg_port_t portCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pTime), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(portCfg);
    SYS_ERR_CHK(cfg_authmgr_port_get(port, &portCfg));

    *pTime = portCfg.serverTimeout;

    return SYS_ERR_OK;
}

int32 sal_authmgr_dot1xRadius_tx(dot1x_rad_xmit_t *pXmit, dot1x_rad_recv_t *pRecv)
{
    char *s = NULL;
    char sysMacStr[CAPA_MAC_STR_LEN];
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
    sys_mac_t sysMac;
    char eapMsgTmp[SYS_BUF2048_LEN];
    char *eapMsgPtr = NULL;

    SYS_PARAM_CHK((NULL == pXmit), SYS_ERR_NULL_POINTER);

    osal_memset(nas_ip_str, 0, sizeof(nas_ip_str));
    osal_memset(prioArry, 0, sizeof(prioArry));
    osal_memset(usedArry, 0, sizeof(usedArry));
    SYS_MEM_CLEAR(sysMacStr);
    SYS_MEM_CLEAR(macStr);
    SYS_MEM_CLEAR(sysMac);

    SYS_ERR_CHK(sal_sys_macAddr_get(&sysMac));
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

        if (osal_strlen(radiusSrv.hostName) > 0)
        {
            err = rad_add_server(h,
                                 radiusSrv.hostName,
                                 radiusSrv.srvPort,
                                 radiusSrv.key,
                                 radiusSrv.timeout,
                                 1); 
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
        rad_put_int(h, RAD_NAS_PORT, pXmit->info.nasPort);
        rad_put_string(h, RAD_USER_NAME, pXmit->info.userName);

        SYS_MEM_CLEAR(sysMacStr);
        osal_sprintf(sysMacStr, "%02X-%02X-%02X-%02X-%02X-%02X", sysMac.octet[0],
                                                                 sysMac.octet[1],
                                                                 sysMac.octet[2],
                                                                 sysMac.octet[3],
                                                                 sysMac.octet[4],
                                                                 sysMac.octet[5]);
        rad_put_string(h, RAD_CALLED_STATION_ID, sysMacStr);

        SYS_MEM_CLEAR(macStr);
        osal_sprintf(macStr, "%02X-%02X-%02X-%02X-%02X-%02X", pXmit->info.callingId.octet[0],
                                                              pXmit->info.callingId.octet[1],
                                                              pXmit->info.callingId.octet[2],
                                                              pXmit->info.callingId.octet[3],
                                                              pXmit->info.callingId.octet[4],
                                                              pXmit->info.callingId.octet[5]);
        rad_put_string(h, RAD_CALLING_STATION_ID, macStr);

        rad_put_int(h, RAD_SERVICE_TYPE, RAD_FRAMED);
        rad_put_int(h, RAD_FRAMED_MTU, DOT1X_RAD_FRAMED_MTU);
        rad_put_int(h, RAD_NAS_PORT_TYPE, RAD_ETHERNET);

        if (pXmit->info.dataLen > RADIUS_MAX_EAP_ATTR_LEN)
        {
            uint8 *_s = pXmit->pEapPkt;
            uint32 _len = pXmit->info.dataLen;

            while (_len > RADIUS_MAX_EAP_ATTR_LEN)
            {
                rad_put_attr(h, RAD_EAP_MESSAGE, _s, RADIUS_MAX_EAP_ATTR_LEN);
                _s += RADIUS_MAX_EAP_ATTR_LEN;
                _len -= RADIUS_MAX_EAP_ATTR_LEN;
            }

            if (_len > 0)
                rad_put_attr(h, RAD_EAP_MESSAGE, _s, _len);
        }
        else
        {
            rad_put_attr(h, RAD_EAP_MESSAGE, pXmit->pEapPkt, pXmit->info.dataLen);
        }

        if (0 != pXmit->info.stateAvp.len)
            rad_put_attr(h, RAD_STATE, pXmit->info.stateAvp.val, pXmit->info.stateAvp.len);

        for (i = 0; i < radiusSrv.retransmit; i++)
        {
            uint32 currRadiusSendId = 0;

            authmgr_dot1xRadiusSendId_get(pXmit->info.sessId, &currRadiusSendId);

            if (pXmit->info.radiusSendId != currRadiusSendId)
            {
                if (NULL != h)
                    rad_close(h);

                return SYS_ERR_OK;
            }

            err = rad_send_request(h);

            if (err < 0)
            {
                ret = SYS_ERR_RADIUS_SRV_ERR; 

                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_CONN_FAIL,
                            radiusSrv.hostName,
                            radiusSrv.srvPort);
                continue;
            }
            else
            {
                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_RADIUS_CONN_SUCCESS,
                            radiusSrv.hostName,
                            radiusSrv.srvPort);

                break;
            }
        }

        SYS_MEM_CLEAR(eapMsgTmp);
        eapMsgPtr = eapMsgTmp;
        switch (err)
        {
            case RAD_ACCESS_ACCEPT:
                while ((attrtype = rad_get_attr(h, &data, &len)) > 0)
                {
                    switch (attrtype)
                    {
                        case RAD_SESSION_TIMEOUT:
                            pRecv->author.reauthTime = rad_cvt_int(data);
                            if (0 == pRecv->author.reauthTime)
                                pRecv->authorSrc.reauthTimeSrc = AUTHMGR_AUTHOR_SRC_CFG;
                            else
                                pRecv->authorSrc.reauthTimeSrc = AUTHMGR_AUTHOR_SRC_RADIUS;
                            break;

                        case RAD_IDLE_TIMEOUT:
                            pRecv->author.inactTime = rad_cvt_int(data);
                            if (0 == pRecv->author.inactTime)
                                pRecv->authorSrc.inactTimeSrc = AUTHMGR_AUTHOR_SRC_CFG;
                            else
                                pRecv->authorSrc.inactTimeSrc = AUTHMGR_AUTHOR_SRC_RADIUS;
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

                        case RAD_EAP_MESSAGE:
                            pRecv->eapLen += len;
                            osal_memcpy(eapMsgPtr, data, len);
                            eapMsgPtr += len;
                            break;

                        default:
                            break;
                    }
                }

                pRecv->authorSrc.vidSrc = AUTHMGR_AUTHOR_SRC_RADIUS;
                if (RAD_TUNNEL_VLAN == tunnelType && RAD_IEEE_802 == mediumType && vid != 0)
                {
                    pRecv->author.authVid = vid;
                }

                if (pRecv->eapLen > 0)
                {
                    pRecv->eapData = osal_alloc(pRecv->eapLen);

                    if (NULL != pRecv->eapData)
                        osal_memcpy(pRecv->eapData, eapMsgTmp, pRecv->eapLen);
                }

                ret = SYS_ERR_OK;
                break;

            case RAD_ACCESS_REJECT:
                while ((attrtype = rad_get_attr(h, &data, &len)) > 0)
                {
                    switch (attrtype)
                    {
                        case RAD_EAP_MESSAGE:
                            pRecv->eapLen += len;
                            osal_memcpy(eapMsgPtr, data, len);
                            eapMsgPtr += len;
                            break;

                        default:
                            break;
                    }
                }

                if (pRecv->eapLen > 0)
                {
                    pRecv->eapData = osal_alloc(pRecv->eapLen);

                    if (NULL != pRecv->eapData)
                        osal_memcpy(pRecv->eapData, eapMsgTmp, pRecv->eapLen);
                }

                ret = SYS_ERR_RADIUS_AUTH_REJECT;
                break;

            case RAD_ACCESS_CHALLENGE:
                while ((attrtype = rad_get_attr(h, &data, &len)) > 0)
                {
                    switch (attrtype)
                    {
                        case RAD_STATE:
                            osal_memcpy(pRecv->stateAvp.val, data, len);
                            pRecv->stateAvp.len = len;
                            break;

                        case RAD_EAP_MESSAGE:
                            pRecv->eapLen += len;
                            osal_memcpy(eapMsgPtr, data, len);
                            eapMsgPtr += len;
                            break;

                        default:
                            break;
                    }
                }

                if (pRecv->eapLen > 0)
                {
                    pRecv->eapData = osal_alloc(pRecv->eapLen);

                    if (NULL != pRecv->eapData)
                        osal_memcpy(pRecv->eapData, eapMsgTmp, pRecv->eapLen);
                }

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

int32 sal_authmgr_dot1xRadius_rx(dot1x_rad_recv_t recv)
{
    SYS_ERR_CHK(authmgr_dot1xRadius_rx(recv));

    return SYS_ERR_OK;
}

