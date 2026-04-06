#include <authmgr/proto_authmgr.h>
#include <rsd/rsd_l2.h>
#include <rsd/rsd_l2_limit.h>
static uint32 _aclKey_ret(sys_mac_t mac)
{
    uint32 key = 0;
    uint32 tmp = 0;

    tmp = mac.octet[2];
    key = (tmp & 0xFF) << 24 ;
    tmp = mac.octet[3];
    key |= (tmp & 0xFF) << 16 ;
    tmp = mac.octet[4];
    key |= (tmp & 0xFF) << 8 ;
    tmp = mac.octet[5];
    key |= (tmp & 0xFF) << 0 ;

    return key;
}

static uint32 _pktCntChk_ret(authmgr_sess_t *pSess)
{
    sys_acl_protoType_t blkType = ACL_PROTOTYPE_AUTH;
    sys_acl_entryKey_t  key;
    uint64              currCnt = 0;

    SYS_MEM_CLEAR(key);

    key.rsv.type = pSess->status.aclKey;

    rsd_acl_entryCounter_get(blkType, &key, &currCnt);

    if (currCnt > pSess->status.pktCnt)
    {
        pSess->status.pktCnt = currCnt;

        return TRUE;
    }

    return FALSE;
}

static int32 _acl_add(authmgr_sess_t *pSess)
{
    sys_acl_protoType_t blkType = ACL_PROTOTYPE_AUTH;
    sys_acl_entryKey_t  key;
    sys_acl_action_t    action;
    sys_acl_field_t     field;

    SYS_MEM_CLEAR(key);
    SYS_MEM_CLEAR(action);
    SYS_MEM_CLEAR(field);

    key.rsv.type = _aclKey_ret(pSess->suppInfo.mac);

    action.actionMask = ACL_ACTIONMASK_LOG;

    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SMAC);
    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SPN);
    osal_memcpy(&field.smac, &pSess->suppInfo.mac, sizeof(sys_mac_t));
    osal_memset(&field.smacMask, 0xFF, 6);
    field.spn = pSess->suppInfo.port;

    SYS_ERR_CHK(rsd_acl_entry_add(blkType, &key, &action, &field, ENABLED));

    AUTHMGR_DBG("add entry %x", key.rsv.type);

    pSess->status.aclKey = key.rsv.type;

    return SYS_ERR_OK;
}

static int32 _assignVlanAcl_add(authmgr_sess_t *pSess, sys_vid_t vid)
{
    sys_acl_protoType_t blkType = ACL_PROTOTYPE_AUTH;
    sys_acl_entryKey_t  key;
    sys_acl_action_t    action;
    sys_acl_field_t     field;

    SYS_MEM_CLEAR(key);
    SYS_MEM_CLEAR(action);
    SYS_MEM_CLEAR(field);

    key.rsv.type = _aclKey_ret(pSess->suppInfo.mac);

    action.actionMask = ACL_ACTIONMASK_SVID | ACL_ACTIONMASK_LOG;
    action.svid = vid;

    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SMAC);
    RSD_ACL_FIELD_BMP_SET(field.fieldMask, ACL_FIELD_SPN);
    osal_memcpy(&field.smac, &pSess->suppInfo.mac, sizeof(sys_mac_t));
    osal_memset(&field.smacMask, 0xFF, 6);
    field.spn = pSess->suppInfo.port;

    pSess->status.pktCnt = 0;

    SYS_ERR_CHK(rsd_acl_entry_add(blkType, &key, &action, &field, ENABLED));

    AUTHMGR_DBG("add entry %x svid %d", key.rsv.type, action.svid);

    pSess->status.aclKey = key.rsv.type;

    return SYS_ERR_OK;
}

static int32 _acl_del(authmgr_sess_t *pSess)
{
    sys_acl_protoType_t blkType = ACL_PROTOTYPE_AUTH;
    sys_acl_entryKey_t  key;

    SYS_MEM_CLEAR(key);

    key.rsv.type = pSess->status.aclKey;

    SYS_ERR_CHK(rsd_acl_entryCounter_clr(blkType, &key));
    SYS_ERR_CHK(rsd_acl_entry_del(blkType, &key));

    AUTHMGR_DBG("delete entry %x", key.rsv.type);

    pSess->status.pktCnt = 0;
    pSess->status.aclKey = 0;

    return SYS_ERR_OK;
}

static int32 _vlan_join(authmgr_sess_t *pSess)
{
    int32 ret = SYS_ERR_FAILED;
    authmgr_mode_t mode = pSess->currMode;

    pSess->status.currVid = pSess->suppInfo.vid;

    if (pSess->status.authorInfo.authVid != 0)
    {
        if (AUTHMGR_MODE_MULTI_HOST == mode)
        {
            SYS_ERR_CHK(_acl_add(pSess));

            ret = rsd_vlan_portAuthorVlan_set(pSess->suppInfo.port, pSess->status.authorInfo.authVid);

            if (SYS_ERR_OK != ret)
            {
                AUTHMGR_DBG("VLAN Authorized Fail");

                pSess->status.authorInfo.authVid = 0;

                return SYS_ERR_OK;
            }
            else
            {
                sys_l2_entry_t l2Entry;

                SYS_MEM_CLEAR(l2Entry);

                l2Entry.vid = pSess->status.authorInfo.authVid;
                osal_memcpy(&l2Entry.mac, &pSess->suppInfo.mac, sizeof(sys_mac_t));

                ret = rsd_l2_mac_search(&l2Entry);

                if ((SYS_ERR_OK == ret)
                    && (l2Entry.port != pSess->suppInfo.port)
                    && (SYS_L2_TYPE_DYNAMIC == l2Entry.type))
                {
                    rsd_l2_mac_del(&l2Entry);
                }

                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_VLAN_ASSIGN,
                            pSess->status.authorInfo.authVid,
                            pSess->suppInfo.mac.octet[0],
                            pSess->suppInfo.mac.octet[1],
                            pSess->suppInfo.mac.octet[2],
                            pSess->suppInfo.mac.octet[3],
                            pSess->suppInfo.mac.octet[4],
                            pSess->suppInfo.mac.octet[5],
                            LPSTR(pSess->suppInfo.port),
                            pSess->sessId.cnt,
                            pSess->sessId.time);
            }
        }
        else if (AUTHMGR_MODE_MULTI_AUTH == mode || AUTHMGR_MODE_SINGLE_HOST == mode)
        {
            SYS_ERR_CHK(_assignVlanAcl_add(pSess, pSess->status.authorInfo.authVid));

            AUTHMGR_DBG("Add ACL for host %02x:%02x:%02x:%02x:%02x:%02x VLAN author with key %x vid %d",
                        pSess->suppInfo.mac.octet[0],
                        pSess->suppInfo.mac.octet[1],
                        pSess->suppInfo.mac.octet[2],
                        pSess->suppInfo.mac.octet[3],
                        pSess->suppInfo.mac.octet[4],
                        pSess->suppInfo.mac.octet[5],
                        pSess->status.aclKey,
                        pSess->status.authorInfo.authVid);

            ret = rsd_vlan_hostAuthorVlan_add(pSess->suppInfo.port, pSess->status.authorInfo.authVid);

            if (SYS_ERR_OK != ret)
            {
                AUTHMGR_DBG("VLAN Authorized Fail");

                pSess->status.authorInfo.authVid = 0;

                return SYS_ERR_OK;
            }
            else
            {
                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_VLAN_ASSIGN,
                            pSess->status.authorInfo.authVid,
                            pSess->suppInfo.mac.octet[0],
                            pSess->suppInfo.mac.octet[1],
                            pSess->suppInfo.mac.octet[2],
                            pSess->suppInfo.mac.octet[3],
                            pSess->suppInfo.mac.octet[4],
                            pSess->suppInfo.mac.octet[5],
                            LPSTR(pSess->suppInfo.port),
                            pSess->sessId.cnt,
                            pSess->sessId.time);
            }
        }

        pSess->status.currVid = pSess->status.authorInfo.authVid;
    }
    else
    {
        if (IS_PORT_GUEST_VLAN_ENABLE(pSess->suppInfo.port)
            && (AUTHMGR_MODE_MULTI_AUTH == mode || AUTHMGR_MODE_SINGLE_HOST == mode))
        {
            sys_vid_t adminVid;

            rsd_vlan_portAdminPvid_get(pSess->suppInfo.port, &adminVid);

            pSess->status.currVid = adminVid;

            SYS_ERR_CHK(_assignVlanAcl_add(pSess, adminVid));
        }
        else
        {
            
            SYS_ERR_CHK(_acl_add(pSess));
        }

        AUTHMGR_DBG("Add ACL for %02x:%02x:%02x:%02x:%02x:%02x inactivity timer with key %x vid %d",
                    pSess->suppInfo.mac.octet[0],
                    pSess->suppInfo.mac.octet[1],
                    pSess->suppInfo.mac.octet[2],
                    pSess->suppInfo.mac.octet[3],
                    pSess->suppInfo.mac.octet[4],
                    pSess->suppInfo.mac.octet[5],
                    pSess->status.aclKey,
                    pSess->status.currVid);
    }

    return SYS_ERR_OK;
}

static int32 _vlan_leave(authmgr_sess_t *pSess)
{
    authmgr_mode_t mode = pSess->currMode;

    if (pSess->status.authorInfo.authVid != 0)
    {
        if (AUTHMGR_MODE_MULTI_HOST == mode)
        {
            pSess->status.authorInfo.authVid = 0;
            SYS_ERR_CHK(rsd_vlan_portAuthorVlan_set(pSess->suppInfo.port, pSess->status.authorInfo.authVid));
        }
        else if (AUTHMGR_MODE_MULTI_AUTH == mode || AUTHMGR_MODE_SINGLE_HOST == mode)
        {
            SYS_ERR_CHK(rsd_vlan_hostAuthorVlan_del(pSess->suppInfo.port, pSess->status.authorInfo.authVid));
            pSess->status.authorInfo.authVid = 0;
        }
    }

    if (pSess->status.aclKey != 0)
    {
        SYS_ERR_CHK(_acl_del(pSess));
    }

    return SYS_ERR_OK;
}

static int32 _guest_vlan_join(authmgr_sess_t *pSess)
{
    authmgr_mode_t mode = pSess->currMode;

    if (IS_PORT_GUEST_VLAN_ENABLE(pSess->suppInfo.port))
    {
        if (AUTHMGR_MODE_MULTI_HOST == mode)
        {
            SYS_ERR_CHK(rsd_auth_portBasedAuthStatus_set(pSess->suppInfo.port, SYS_AUTH));

            rsd_l2_limit_portCtrl_set(SYS_L2_LIMIT_MOD_AUTHMGR, pSess->suppInfo.port, DISABLED);

            SYS_ERR_CHK(rsd_vlan_portGuestVlanActive_set(pSess->suppInfo.port, ENABLED));
        }
        else if (AUTHMGR_MODE_MULTI_AUTH == mode || AUTHMGR_MODE_SINGLE_HOST == mode)
        {
            SYS_ERR_CHK(rsd_auth_macBasedAuthMac_add(g_authCfg.guestVlan, pSess->suppInfo.port, &pSess->suppInfo.mac));

            SYS_ERR_CHK(_assignVlanAcl_add(pSess, g_authCfg.guestVlan));

            SYS_ERR_CHK(rsd_vlan_hostAuthorVlan_add(pSess->suppInfo.port, g_authCfg.guestVlan));
        }

        pSess->status.currVid = g_authCfg.guestVlan;

        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_GUESTVLAN,
                    pSess->suppInfo.mac.octet[0],
                    pSess->suppInfo.mac.octet[1],
                    pSess->suppInfo.mac.octet[2],
                    pSess->suppInfo.mac.octet[3],
                    pSess->suppInfo.mac.octet[4],
                    pSess->suppInfo.mac.octet[5],
                    pSess->status.currVid,
                    LPSTR(pSess->suppInfo.port),
                    pSess->sessId.cnt,
                    pSess->sessId.time);

        return SYS_ERR_OK;
    }

    return SYS_ERR_FAILED;
}

static int32 _guest_vlan_leave(authmgr_sess_t *pSess)
{
    authmgr_mode_t mode = pSess->currMode;

    if (IS_PORT_GUEST_VLAN_ENABLE(pSess->suppInfo.port))
    {
        if (AUTHMGR_MODE_MULTI_HOST == mode)
        {
            SYS_ERR_CHK(rsd_vlan_portGuestVlanActive_set(pSess->suppInfo.port, DISABLED));

            if (IS_LOGIC_PORTMASK_PORTSET(g_portAuthLpm, pSess->suppInfo.port))
            {
                rsd_l2_limit_portCtrl_set(SYS_L2_LIMIT_MOD_AUTHMGR, pSess->suppInfo.port, ENABLED);

                SYS_ERR_CHK(rsd_auth_portBasedAuthStatus_set(pSess->suppInfo.port, SYS_UNAUTH));
            }
        }
        else if (AUTHMGR_MODE_MULTI_AUTH == mode || AUTHMGR_MODE_SINGLE_HOST == mode)
        {
            SYS_ERR_CHK(rsd_vlan_hostAuthorVlan_del(pSess->suppInfo.port, pSess->status.currVid));

            if (pSess->status.aclKey != 0)
            {
                SYS_ERR_CHK(_acl_del(pSess));
            }

            SYS_ERR_CHK(rsd_auth_macBasedAuthMac_del(pSess->status.currVid, pSess->suppInfo.port, &pSess->suppInfo.mac));
        }

        pSess->status.currVid = pSess->suppInfo.vid;

        return SYS_ERR_OK;
    }

    return SYS_ERR_FAILED;
}

static int32 _sess_statusAuth_set(authmgr_sess_t *pSess)
{
    authmgr_mode_t mode = pSess->currMode;

    if (AUTHMGR_MODE_MULTI_HOST == mode)
    {
        SYS_ERR_CHK(rsd_auth_portBasedAuthStatus_set(pSess->suppInfo.port, SYS_AUTH));

        rsd_l2_limit_portCtrl_set(SYS_L2_LIMIT_MOD_AUTHMGR, pSess->suppInfo.port, DISABLED);
    }
    else if (AUTHMGR_MODE_MULTI_AUTH == mode || AUTHMGR_MODE_SINGLE_HOST == mode)
    {
        SYS_ERR_CHK(rsd_auth_macBasedAuthMac_add(pSess->status.currVid, pSess->suppInfo.port, &pSess->suppInfo.mac));
    }

    return SYS_ERR_OK;
}

static int32 _sess_statusUnauth_set(authmgr_sess_t *pSess)
{
    authmgr_mode_t mode = pSess->currMode;

    if (AUTHMGR_MODE_MULTI_HOST == mode)
    {
        if (IS_LOGIC_PORTMASK_PORTSET(g_portAuthLpm, pSess->suppInfo.port))
        {
            rsd_l2_limit_portCtrl_set(SYS_L2_LIMIT_MOD_AUTHMGR, pSess->suppInfo.port, ENABLED);

            SYS_ERR_CHK(rsd_auth_portBasedAuthStatus_set(pSess->suppInfo.port, SYS_UNAUTH));
        }
    }
    else if (AUTHMGR_MODE_MULTI_AUTH == mode || AUTHMGR_MODE_SINGLE_HOST == mode)
    {
        SYS_ERR_CHK(rsd_auth_macBasedAuthMac_del(pSess->status.currVid , pSess->suppInfo.port, &pSess->suppInfo.mac));
    }

    pSess->status.currVid = pSess->suppInfo.vid;

    return SYS_ERR_OK;
}

int32 authmgr_action_authStatus_set(authmgr_sess_t *pSess, authmgr_status_t status)
{
    authmgr_status_t oldStatus = pSess->status.authStatus;

    if (oldStatus == status)
        return SYS_ERR_OK;

    switch (oldStatus)
    {
        case AUTHMGR_STATUS_INIT:
            if (AUTHMGR_STATUS_AUTHED == status)
            {
                _vlan_join(pSess);
                _sess_statusAuth_set(pSess);

                switch (pSess->typeOrder.order[pSess->currTypeIdx])
                {
#ifdef CONFIG_SYS_PROTO_DOT1X
                    case AUTHMGR_TYPE_DOT1X:
                        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_DOT1X_SUCCESS,
                                    pSess->suppInfo.mac.octet[0],
                                    pSess->suppInfo.mac.octet[1],
                                    pSess->suppInfo.mac.octet[2],
                                    pSess->suppInfo.mac.octet[3],
                                    pSess->suppInfo.mac.octet[4],
                                    pSess->suppInfo.mac.octet[5],
                                    LPSTR(pSess->suppInfo.port));
                        break;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
                    case AUTHMGR_TYPE_MAC:
                        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_MACAUTH_SUCCESS,
                                    pSess->suppInfo.mac.octet[0],
                                    pSess->suppInfo.mac.octet[1],
                                    pSess->suppInfo.mac.octet[2],
                                    pSess->suppInfo.mac.octet[3],
                                    pSess->suppInfo.mac.octet[4],
                                    pSess->suppInfo.mac.octet[5],
                                    LPSTR(pSess->suppInfo.port));
                        break;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
                    case AUTHMGR_TYPE_WEB:
                        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_WEBAUTH_SUCCESS,
                                    pSess->suppInfo.mac.octet[0],
                                    pSess->suppInfo.mac.octet[1],
                                    pSess->suppInfo.mac.octet[2],
                                    pSess->suppInfo.mac.octet[3],
                                    pSess->suppInfo.mac.octet[4],
                                    pSess->suppInfo.mac.octet[5],
                                    LPSTR(pSess->suppInfo.port));
                        break;
#endif
                    default:
                        break;
                }

                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_SUCCESS,
                            pSess->suppInfo.mac.octet[0],
                            pSess->suppInfo.mac.octet[1],
                            pSess->suppInfo.mac.octet[2],
                            pSess->suppInfo.mac.octet[3],
                            pSess->suppInfo.mac.octet[4],
                            pSess->suppInfo.mac.octet[5],
                            LPSTR(pSess->suppInfo.port),
                            pSess->sessId.cnt,
                            pSess->sessId.time);
            }
            else if (AUTHMGR_STATUS_UNAUTHED == status)
            {
                _sess_statusUnauth_set(pSess);
            }
            else if (AUTHMGR_STATUS_LOCKED == status)
            {
                _sess_statusUnauth_set(pSess);

                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_LOCK,
                            pSess->suppInfo.mac.octet[0],
                            pSess->suppInfo.mac.octet[1],
                            pSess->suppInfo.mac.octet[2],
                            pSess->suppInfo.mac.octet[3],
                            pSess->suppInfo.mac.octet[4],
                            pSess->suppInfo.mac.octet[5],
                            LPSTR(pSess->suppInfo.port),
                            pSess->sessId.cnt,
                            pSess->sessId.time);
            }
            else if (AUTHMGR_STATUS_GUEST == status)
            {
                _guest_vlan_join(pSess);
            }
            break;

        case AUTHMGR_STATUS_AUTHED:
            _vlan_leave(pSess);
            _sess_statusUnauth_set(pSess);
            break;

        case AUTHMGR_STATUS_UNAUTHED:
            if (AUTHMGR_STATUS_GUEST == status)
            {
                _guest_vlan_join(pSess);
            }
            break;

        case AUTHMGR_STATUS_LOCKED:
            break;

        case AUTHMGR_STATUS_GUEST:
            _guest_vlan_leave(pSess);
            break;

        default:
            break;
    }

    pSess->status.authStatus = status;

    if (AUTHMGR_STATUS_DISABLE == status)
    {
        pSess->delSess = TRUE;
    }

    return SYS_ERR_OK;
}

authmgr_status_t authmgr_action_authStatus_ret(authmgr_sess_t *pSess)
{
    if (NULL != pSess)
    {
        return pSess->status.authStatus;
    }

    return AUTHMGR_STATUS_DISABLE;
}

int32 authmgr_action_authStatus_clear(authmgr_sess_t *pSess)
{
    authmgr_status_t orgStatus = pSess->status.authStatus;

    osal_memset(&pSess->status, 0, sizeof(authmgr_sess_status_t));

    pSess->status.authStatus = orgStatus;
    pSess->status.currVid = pSess->suppInfo.vid;

    return SYS_ERR_OK;
}

uint32 authmgr_action_nextType_ret(authmgr_sess_t *pSess)
{
    uint32 i = 0;

    for (i = (pSess->currTypeIdx + 1); i < (AUTHMGR_TYPE_END - 1); i++)
    {
        if (AUTHMGR_TYPE_NONE == pSess->typeOrder.order[i])
            break;

        if (ENABLED == pSess->pPortCfg->typeEnbl[pSess->typeOrder.order[i]])
        {
            return pSess->typeOrder.order[i];
        }
    }

    return AUTHMGR_TYPE_NONE; 
}

uint32 authmgr_action_nextTypeIdx_ret(authmgr_sess_t *pSess)
{
    uint32 i = 0;

    for (i = (pSess->currTypeIdx + 1); i < (AUTHMGR_TYPE_END - 1); i++)
    {
        if (AUTHMGR_TYPE_NONE == pSess->typeOrder.order[i])
            break;

        if (DISABLED == g_authCfg.typeEnbl[pSess->typeOrder.order[i]])
            continue;

        if (ENABLED == pSess->pPortCfg->typeEnbl[pSess->typeOrder.order[i]])
        {
            return i;
        }
    }

    return pSess->currTypeIdx;
}

uint32 authmgr_action_activeChk_ret(authmgr_sess_t *pSess)
{
    return _pktCntChk_ret(pSess);
}

uint32 authmgr_action_guestActiveChk_ret(authmgr_sess_t *pSess)
{
    sys_l2_entry_t addr;
    authmgr_mode_t mode = pSess->currMode;

    SYS_MEM_CLEAR(addr);

    if (AUTHMGR_MODE_MULTI_HOST == mode)
        return TRUE;

    osal_memcpy(&addr.mac, &pSess->suppInfo.mac, sizeof(sys_mac_t));
    addr.vid = g_authCfg.guestVlan;

    if (SYS_ERR_OK != rsd_l2_mac_search(&addr))
        return FALSE;

    return TRUE;
}

int32 authmgr_action_authorInfo_set(authmgr_sess_t *pSess, authmgr_author_t *pAuthor, authmgr_authorSrc_t *pAuthorSrc)
{
    SYS_PARAM_CHK((NULL == pAuthor), SYS_ERR_NULL_POINTER);

    osal_memcpy(&pSess->status.authorInfo, pAuthor, sizeof(authmgr_author_t));
    osal_memcpy(&pSess->status.authorSrc, pAuthorSrc, sizeof(authmgr_authorSrc_t));

    if (0 == pSess->status.authorInfo.inactTime)
        pSess->status.authorInfo.inactTime = pSess->pPortCfg->inactTimeout;

    if (0 == pSess->status.authorInfo.reauthTime)
        pSess->status.authorInfo.reauthTime = pSess->pPortCfg->reauthPeriod;

    switch (pSess->pPortCfg->vlanAssignMode)
    {
        
        case AUTHMGR_VLAN_ASSIGN_DISABLE:
            if (pAuthorSrc->vidSrc == AUTHMGR_AUTHOR_SRC_RADIUS)
                pSess->status.authorInfo.authVid = 0;
            else
                pSess->status.authorInfo.authVid = pAuthor->authVid;
            break;

        case AUTHMGR_VLAN_ASSIGN_REJECT:
            if (0 == pAuthor->authVid && pAuthorSrc->vidSrc == AUTHMGR_AUTHOR_SRC_RADIUS)
                return SYS_ERR_FAILED;
            else
                pSess->status.authorInfo.authVid = pAuthor->authVid;
            break;

        case AUTHMGR_VLAN_ASSIGN_STATIC:
            pSess->status.authorInfo.authVid = pAuthor->authVid;
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}

