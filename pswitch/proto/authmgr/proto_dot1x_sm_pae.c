#include <authmgr/proto_authmgr.h>
static int32 dot1x_sm_pae_force_auth_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_pae_force_unauth_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_pae_held_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_pae_aborting_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_pae_authenticated_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_pae_authenticating_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_pae_connecting_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_pae_restart_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_pae_disconnected_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_pae_init_transit(dot1x_pacp_t *pPacp);
static int32 _dot1x_sm_pae_txCannedSuccess(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(eapol_pkt_tx(pPacp, EAP_SUCCESS));

    return SYS_ERR_OK;
}
static int32 _dot1x_sm_pae_txCannedFail(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(eapol_pkt_tx(pPacp, EAP_FAILURE));

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_force_auth_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->pae_sm.curState = DOT1X_PAE_FORCE_AUTH;
    pPacp->authPortStatus = DOT1X_STATE_UNAUTHORIZED;  
    pPacp->portMode = DOT1X_MODE_FORCE_AUTHORIZED;
    pPacp->pae_sm.eapolStart = FALSE;

    _dot1x_sm_pae_txCannedSuccess(pPacp);

    DOT1X_SM_PAE_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_force_auth_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->pae_sm.eapolStart)
    {
        dot1x_sm_pae_force_auth_transit(pPacp);
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_force_unauth_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->pae_sm.curState = DOT1X_PAE_FORCE_UNAUTH;
    pPacp->authPortStatus = DOT1X_STATE_UNAUTHORIZED;
    pPacp->portMode = DOT1X_MODE_FORCE_UNAUTHORIZED;
    pPacp->pae_sm.eapolStart = FALSE;
    _dot1x_sm_pae_txCannedFail(pPacp);

    DOT1X_SM_PAE_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_force_unauth_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->pae_sm.eapolStart)
    {
        dot1x_sm_pae_force_unauth_transit(pPacp);
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_held_transit(dot1x_pacp_t *pPacp)
{
    authmgr_sess_t *pSess = NULL;

    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->pae_sm.curState = DOT1X_PAE_HELD;
    pPacp->authPortStatus = DOT1X_STATE_UNAUTHORIZED;
    pPacp->quietWhile = pPacp->pae_sm.quietPeriod;
    pPacp->pae_sm.eapolLogoff = FALSE;

    pSess = (authmgr_sess_t *) pPacp->pParentSess;

    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_DOT1X_FAIL,
                pSess->suppInfo.mac.octet[0],
                pSess->suppInfo.mac.octet[1],
                pSess->suppInfo.mac.octet[2],
                pSess->suppInfo.mac.octet[3],
                pSess->suppInfo.mac.octet[4],
                pSess->suppInfo.mac.octet[5],
                LPSTR(pPacp->port));

    if (AUTHMGR_TYPE_NONE != authmgr_action_nextType_ret(pSess))
    {
        pSess->gotoNextType = TRUE;
        pPacp->delPacp = TRUE;

        return SYS_ERR_OK;
    }
    else
    {
        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_NOMORE_METHODS,
                    pSess->suppInfo.mac.octet[0],
                    pSess->suppInfo.mac.octet[1],
                    pSess->suppInfo.mac.octet[2],
                    pSess->suppInfo.mac.octet[3],
                    pSess->suppInfo.mac.octet[4],
                    pSess->suppInfo.mac.octet[5],
                    LPSTR(pSess->suppInfo.port),
                    pSess->sessId.cnt,
                    pSess->sessId.time);

        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_FAIL,
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

    if (IS_PORT_GUEST_VLAN_ENABLE(pPacp->port))
    {
        pPacp->sessionTime = 0;

        authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_GUEST);
    }
    else
    {
        authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_LOCKED);
    }

    DOT1X_SM_PAE_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_held_check(dot1x_pacp_t *pPacp)
{
    authmgr_sess_t *pSess = NULL;

    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    pSess = (authmgr_sess_t *) pPacp->pParentSess;

    if (IS_PORT_GUEST_VLAN_ENABLE(pPacp->port))
    {
        if (pPacp->guestChk)
        {
            pPacp->guestChk = FALSE;

            if (FALSE == authmgr_action_guestActiveChk_ret(pSess))
            {
                authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_DISABLE);
                return SYS_ERR_OK;
            }
        }

        if (pPacp->pae_sm.eapolStart)
            dot1x_sm_pae_restart_transit(pPacp);
    }
    else if (pPacp->quietWhile == 0)
    {
        if (AUTHMGR_STATUS_DISABLE != pSess->status.authStatus)
        {
            SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_TIMEOUT_QUIET,
                        pSess->suppInfo.mac.octet[0],
                        pSess->suppInfo.mac.octet[1],
                        pSess->suppInfo.mac.octet[2],
                        pSess->suppInfo.mac.octet[3],
                        pSess->suppInfo.mac.octet[4],
                        pSess->suppInfo.mac.octet[5],
                        LPSTR(pSess->suppInfo.port),
                        pSess->sessId.cnt,
                        pSess->sessId.time);

            authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_DISABLE);
        }

            }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_aborting_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->pae_sm.curState = DOT1X_PAE_ABORTING;
    pPacp->authAbort = TRUE;
    pPacp->keyRun = FALSE;
    pPacp->keyDone = FALSE;

    DOT1X_SM_PAE_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_aborting_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->pae_sm.eapolLogoff && !pPacp->authAbort)
    {
        dot1x_sm_pae_disconnected_transit(pPacp);
    }

    if (!pPacp->pae_sm.eapolLogoff && !pPacp->authAbort)
    {
        dot1x_sm_pae_restart_transit(pPacp);
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_authenticated_transit(dot1x_pacp_t *pPacp)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *)pPacp->pParentSess;

    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->pae_sm.curState = DOT1X_PAE_AUTHENTICATED;
    pPacp->authPortStatus = DOT1X_STATE_AUTHORIZED;
    pPacp->pae_sm.reAuthCount = 0;

    if (pPacp->authFail)
    {
        pPacp->authFail = FALSE;
        pPacp->authSuccess = TRUE;

        authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_UNAUTHED);
    }
    else
    {
        authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_AUTHED);

        pPacp->reAuthWhen = pSess->status.authorInfo.reauthTime;
    }

    DOT1X_SM_PAE_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_authenticated_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->pae_sm.eapolStart || pPacp->reAuthenticate)
    {
        DOT1X_DBG(pPacp, "eapolStart %d reAuthenticate %d", pPacp->pae_sm.eapolStart, pPacp->reAuthenticate);
        dot1x_sm_pae_restart_transit(pPacp);
    }

    if (pPacp->initialize)
    {
        DOT1X_DBG(pPacp, "initialize %d", pPacp->initialize);
        dot1x_sm_pae_init_transit(pPacp);
    }

    #if 0
    if (pPacp->pae_sm.eapolLogoff || !pPacp->portValid)
    {
        dot1x_sm_pae_disconnected_transit(pPacp);
    }
    #endif

    if (pPacp->inactTimeout || pPacp->pae_sm.eapolLogoff || !pPacp->portValid)
    {
        authmgr_sess_t *pSess = NULL;
        pSess = (authmgr_sess_t *)pPacp->pParentSess;
        pSess->delSess = TRUE;
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_authenticating_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->pae_sm.curState = DOT1X_PAE_AUTHENTICATING;
    pPacp->pae_sm.eapolStart = FALSE;
    pPacp->authSuccess = FALSE;
    pPacp->authFail = FALSE;
    pPacp->authTimeout = FALSE;
    pPacp->authStart = TRUE;
    pPacp->keyRun = FALSE;
    pPacp->keyDone = FALSE;

    DOT1X_SM_PAE_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_authenticating_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->authSuccess && pPacp->portValid)
    {
        dot1x_sm_pae_authenticated_transit(pPacp);
    }

    if (pPacp->pae_sm.eapolStart || pPacp->pae_sm.eapolLogoff || pPacp->authTimeout)
    {
        DOT1X_DBG(pPacp, "eapolStart %d eapolLogoff %d authTimeout %d",
                         pPacp->pae_sm.eapolStart,
                         pPacp->pae_sm.eapolLogoff,
                         pPacp->authTimeout);

        if (pPacp->authTimeout)
            dot1x_sm_pae_held_transit(pPacp);
        else
            dot1x_sm_pae_aborting_transit(pPacp);
    }

    if (pPacp->authFail || (pPacp->keyDone && !pPacp->portValid))
    {
        dot1x_sm_pae_held_transit(pPacp);
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_connecting_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    if (!pPacp->reAuthenticate)
    {
        authmgr_action_authStatus_set((authmgr_sess_t *)pPacp->pParentSess, AUTHMGR_STATUS_INIT);
        authmgr_action_authStatus_clear((authmgr_sess_t *)pPacp->pParentSess);
    }

    pPacp->pae_sm.curState = DOT1X_PAE_CONNECTING;
    pPacp->reAuthenticate = FALSE;

    INC(pPacp->pae_sm.reAuthCount);

    pPacp->pae_sm.eapReq = TRUE;
    pPacp->pae_sm.reqCnt = 0;

    DOT1X_SM_PAE_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_connecting_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->pae_sm.eapolLogoff || (pPacp->pae_sm.reAuthCount > pPacp->pae_sm.reAuthMax))
    {
        dot1x_sm_pae_disconnected_transit(pPacp);
        return SYS_ERR_OK;
    }

    if ((pPacp->pae_sm.eapReq && (pPacp->pae_sm.reAuthCount <= pPacp->pae_sm.reAuthMax)) || pPacp->eapSuccess || pPacp->eapFail)
    {
        pPacp->backend_sm.eapReq = TRUE;

        dot1x_sm_pae_authenticating_transit(pPacp);
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_restart_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->pae_sm.curState = DOT1X_PAE_RESTART;

    pPacp->eapFail = FALSE;
    pPacp->eapSuccess = FALSE;

    DOT1X_SM_PAE_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_restart_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (!pPacp->pae_sm.eapRestart)
    {
        dot1x_sm_pae_connecting_transit(pPacp);

        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_disconnected_transit(dot1x_pacp_t *pPacp)
{
    authmgr_sess_t *pSess = NULL;

    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    pSess = (authmgr_sess_t *)pPacp->pParentSess;

    DOT1X_DBG(pPacp, "");

    pPacp->portStatus = DOT1X_STATE_UNAUTHORIZED;
    pPacp->authPortStatus = DOT1X_STATE_UNAUTHORIZED;

    if (pPacp->pae_sm.reAuthCount > pPacp->pae_sm.reAuthMax)
    {
        
        if (AUTHMGR_TYPE_NONE != authmgr_action_nextType_ret(pSess))
        {
            pSess->gotoNextType = TRUE;
            pPacp->delPacp = TRUE;

            return SYS_ERR_OK;
        }
        else
        {
            SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_NOMORE_METHODS,
                        pSess->suppInfo.mac.octet[0],
                        pSess->suppInfo.mac.octet[1],
                        pSess->suppInfo.mac.octet[2],
                        pSess->suppInfo.mac.octet[3],
                        pSess->suppInfo.mac.octet[4],
                        pSess->suppInfo.mac.octet[5],
                        LPSTR(pSess->suppInfo.port),
                        pSess->sessId.cnt,
                        pSess->sessId.time);

            SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_FAIL,
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

        if (IS_PORT_GUEST_VLAN_ENABLE(pPacp->port))
        {
            int32 ret = SYS_ERR_FAILED;

            pPacp->sessionTime = 0;

            ret = authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_GUEST);

            if (SYS_ERR_OK == ret)
                eapol_pkt_tx(pPacp, EAP_SUCCESS);
        }
    }
    else
    {
        authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_UNAUTHED);
    }

    pPacp->pae_sm.curState = DOT1X_PAE_DISCONNECTED;
    pPacp->sessionTime = 0;

    pPacp->pae_sm.reAuthCount = 0;
    pPacp->pae_sm.eapolLogoff = FALSE;

    DOT1X_SM_PAE_CHK_NOW(pPacp);
    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_disconnected_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (IS_PORT_GUEST_VLAN_ENABLE(pPacp->port))
    {
        authmgr_sess_t *pSess = NULL;

        pSess = (authmgr_sess_t *) pPacp->pParentSess;

        if (pPacp->guestChk)
        {
            pPacp->guestChk = FALSE;

            if (FALSE == authmgr_action_guestActiveChk_ret(pSess))
            {
                authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_DISABLE);
                return SYS_ERR_OK;
            }
        }

        if (AUTHMGR_STATUS_GUEST == authmgr_action_authStatus_ret(pSess))
        {
            if (!pPacp->pae_sm.eapolStart)
                return SYS_ERR_OK;
        }
    }

    dot1x_sm_pae_restart_transit(pPacp);

    return SYS_ERR_OK;
}
static int32 dot1x_sm_pae_init_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->initialize = FALSE;
    pPacp->pae_sm.curState = DOT1X_PAE_INITIALIZE;
    pPacp->portMode = DOT1X_MODE_AUTO;

    authmgr_action_authStatus_set((authmgr_sess_t *)pPacp->pParentSess, AUTHMGR_STATUS_INIT);

    DOT1X_SM_PAE_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_init_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    dot1x_sm_pae_disconnected_transit(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_pae_global_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (((pPacp->portControl == DOT1X_MODE_AUTO) && (pPacp->portMode != pPacp->portControl)) || (pPacp->initialize) || (!pPacp->portEnabled))
    {
        dot1x_sm_pae_init_transit(pPacp);
    }

    if (((pPacp->portControl == DOT1X_MODE_FORCE_AUTHORIZED) && (pPacp->portMode != pPacp->portControl)) && !(pPacp->initialize || !pPacp->portEnabled))
        dot1x_sm_pae_force_auth_transit(pPacp);

    if (((pPacp->portControl == DOT1X_MODE_FORCE_UNAUTHORIZED) && (pPacp->portMode != pPacp->portControl)) && !(pPacp->initialize || !pPacp->portEnabled))
        dot1x_sm_pae_force_unauth_transit(pPacp);

    return SYS_ERR_OK;
}

int32 dot1x_sm_pae_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    pPacp->pae_sm.chkNow = FALSE;

    dot1x_sm_pae_global_check(pPacp);

    switch (pPacp->pae_sm.curState)
    {
        case DOT1X_PAE_INITIALIZE:
            dot1x_sm_pae_init_check(pPacp);
            break;
        case DOT1X_PAE_DISCONNECTED:
            dot1x_sm_pae_disconnected_check(pPacp);
            break;
        case DOT1X_PAE_RESTART:
            dot1x_sm_pae_restart_check(pPacp);
            break;
        case DOT1X_PAE_CONNECTING:
            dot1x_sm_pae_connecting_check(pPacp);
            break;
        case DOT1X_PAE_AUTHENTICATING:
            dot1x_sm_pae_authenticating_check(pPacp);
            break;
        case DOT1X_PAE_AUTHENTICATED:
            dot1x_sm_pae_authenticated_check(pPacp);
            break;
        case DOT1X_PAE_ABORTING:
            dot1x_sm_pae_aborting_check(pPacp);
            break;
        case DOT1X_PAE_HELD:
            dot1x_sm_pae_held_check(pPacp);
            break;
        case DOT1X_PAE_FORCE_AUTH:
            dot1x_sm_pae_force_auth_check(pPacp);
            break;
        case DOT1X_PAE_FORCE_UNAUTH:
            dot1x_sm_pae_force_unauth_check(pPacp);
            break;
    }

    return SYS_ERR_OK;
}

