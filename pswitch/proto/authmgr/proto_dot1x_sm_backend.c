#include <authmgr/proto_authmgr.h>
#include <ksi/ksi_core.h>
#include <ksi_authmgr.h>
static int32 dot1x_sm_backend_success_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_backend_fail_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_backend_timeout_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_backend_ignore_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_backend_response_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_backend_request_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_backend_idle_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_backend_init_transit(dot1x_pacp_t *pPacp);
static int32 _dot1x_sm_backend_txReq(dot1x_pacp_t *pPacp)
{
    if (TRUE == pPacp->eapFail && TRUE == pPacp->pae_sm.eapReq)
    {
        
        eapol_pkt_tx(pPacp, EAP_FAILURE);
    }
    else if (TRUE == pPacp->pae_sm.eapReq)
    {
        eapol_pkt_tx(pPacp, EAP_REQUEST);
    }
    else
    {
        eapol_pkt_passthrough(pPacp);
    }

    return SYS_ERR_OK;
}
static int32 _dot1x_sm_backend_sendRespToServer(dot1x_pacp_t *pPacp)
{
    if (NULL != pPacp->pae_sm.fromSupp.pkt)
    {
        uint8 *toKsiData = NULL;
        uint32 toKsiDataLen = sizeof(dot1x_rad_xmitInfo_t) + pPacp->pae_sm.fromSupp.length;

        DOT1X_DBG(pPacp, "From supplicant EAP len %u id %u",
                         pPacp->pae_sm.fromSupp.length,
                         *((uint8*)(pPacp->pae_sm.fromSupp.pkt + 1)));

        if (NULL != (toKsiData = osal_alloc(toKsiDataLen)))
        {
            dot1x_rad_xmitInfo_t *pXmitInfo = NULL;
            authmgr_sess_t *pSess = (authmgr_sess_t *) pPacp->pParentSess;

            osal_memset(toKsiData, 0, toKsiDataLen);

            INC(pPacp->backend_sm.radiusSendId);

            pXmitInfo = (dot1x_rad_xmitInfo_t*)(toKsiData);
            osal_memcpy(&pXmitInfo->sessId, &pSess->sessId, sizeof(authmgr_sessId_t));
            pXmitInfo->dataLen = pPacp->pae_sm.fromSupp.length;
            osal_memcpy(&pXmitInfo->callingId, &pSess->suppInfo.mac, sizeof(sys_mac_t));
            pXmitInfo->nasPort = pPacp->port;
            osal_strncpy(pXmitInfo->userName, pPacp->userName, CAPA_AUTHMGR_DOT1X_USERNAME_LEN);
            osal_memcpy(&pXmitInfo->stateAvp, &pPacp->backend_sm.stateAvp, sizeof(dot1x_radius_stateAvp_t));
            pXmitInfo->radiusSendId = pPacp->backend_sm.radiusSendId;
            osal_memcpy(toKsiData + sizeof(dot1x_rad_xmitInfo_t), pPacp->pae_sm.fromSupp.pkt, pPacp->pae_sm.fromSupp.length);

            KSI_NOTIFY_MSG_DATA(KSI_AUTHMGR_DOT1X_EAP_RADIUS_TX, toKsiDataLen, toKsiData);

            osal_free(toKsiData);
        }

        osal_free(pPacp->pae_sm.fromSupp.pkt);
        pPacp->pae_sm.fromSupp.pkt = NULL;
        pPacp->pae_sm.fromSupp.length = 0;
    }
    else
    {
        pPacp->backend_sm.eapNoReq = TRUE;
    }

    return SYS_ERR_OK;
}
static int32 _dot1x_sm_backend_abortAuth(dot1x_pacp_t *pPacp)
{
    DOT1X_DBG(pPacp, "From supp EAP len %u. From radius EAP len %u",
                     pPacp->pae_sm.fromSupp.length,
                     pPacp->pae_sm.fromSvr.length);

    if (NULL != pPacp->pae_sm.fromSupp.pkt)
    {
        osal_free(pPacp->pae_sm.fromSupp.pkt);
        pPacp->pae_sm.fromSupp.pkt = NULL;
    }
    if (NULL != pPacp->pae_sm.fromSvr.pkt)
    {
        osal_free(pPacp->pae_sm.fromSvr.pkt);
        pPacp->pae_sm.fromSvr.pkt = NULL;
    }
    pPacp->pae_sm.fromSupp.length = 0;
    pPacp->pae_sm.fromSvr.length = 0;

    return SYS_ERR_OK;
}
static int32 dot1x_sm_backend_success_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->backend_sm.curState = DOT1X_BACKEND_SUCCESS;
    _dot1x_sm_backend_txReq(pPacp);
    pPacp->authSuccess = TRUE;
    pPacp->keyRun = TRUE;
    pPacp->currentId = pPacp->backend_sm.idFromServer;

    DOT1X_SM_BACKEND_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_backend_success_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    dot1x_sm_backend_idle_transit(pPacp);

    return SYS_ERR_OK;
}
static int32 dot1x_sm_backend_fail_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->backend_sm.curState = DOT1X_BACKEND_FAIL;
    _dot1x_sm_backend_txReq(pPacp);
    pPacp->authFail = TRUE;
    pPacp->currentId = pPacp->backend_sm.idFromServer;

    DOT1X_SM_BACKEND_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_backend_fail_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    dot1x_sm_backend_idle_transit(pPacp);

    return SYS_ERR_OK;
}
static int32 dot1x_sm_backend_timeout_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->backend_sm.curState = DOT1X_BACKEND_TIMEOUT;
    pPacp->authTimeout = TRUE;

    DOT1X_SM_BACKEND_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_backend_timeout_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    dot1x_sm_backend_idle_transit(pPacp);

    return SYS_ERR_OK;
}
static int32 dot1x_sm_backend_ignore_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->backend_sm.curState = DOT1X_BACKEND_IGNORE;
    pPacp->backend_sm.eapNoReq = FALSE;

    DOT1X_SM_BACKEND_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_backend_ignore_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->eapolEap)
    {
        dot1x_sm_backend_response_transit(pPacp);
        return SYS_ERR_OK;
    }

    if (pPacp->backend_sm.eapReq)
    {
        dot1x_sm_backend_request_transit(pPacp);
        return SYS_ERR_OK;
    }

    if (pPacp->eapTimeout)
    {
        DOT1X_DBG(pPacp, "Supplicant request timeout");

        dot1x_sm_backend_timeout_transit(pPacp);
        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_backend_response_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->backend_sm.reqCnt = 0;

    pPacp->backend_sm.curState = DOT1X_BACKEND_RESPONSE;
    pPacp->authTimeout = FALSE;
    pPacp->eapolEap = FALSE;
    pPacp->backend_sm.eapNoReq = FALSE;
    pPacp->aWhile = pPacp->backend_sm.serverTimeout;
    pPacp->backend_sm.eapResp = TRUE;
    _dot1x_sm_backend_sendRespToServer(pPacp);

    DOT1X_SM_BACKEND_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_backend_response_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->backend_sm.eapNoReq)
    {
        dot1x_sm_backend_ignore_transit(pPacp);
        return SYS_ERR_OK;
    }

    if (pPacp->aWhile == 0)
    {
        DOT1X_DBG(pPacp, "Radius server response timeout");

        dot1x_sm_backend_timeout_transit(pPacp);
        return SYS_ERR_OK;
    }

    if (pPacp->eapFail)
    {
        dot1x_sm_backend_fail_transit(pPacp);
        return SYS_ERR_OK;
    }

    if (pPacp->eapSuccess)
    {
        dot1x_sm_backend_success_transit(pPacp);
        return SYS_ERR_OK;
    }

    if (pPacp->backend_sm.eapReq)
    {
        dot1x_sm_backend_request_transit(pPacp);
        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_backend_request_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->backend_sm.curState = DOT1X_BACKEND_REQUEST;

    _dot1x_sm_backend_txReq(pPacp);

    pPacp->backend_sm.eapReq = FALSE;

    if (TRUE == pPacp->pae_sm.eapReq)
    {
        pPacp->pae_sm.reqCnt++;
        pPacp->pae_sm.txWhile = pPacp->pae_sm.txPeriod;
    }
    else
    {
        pPacp->backend_sm.reqCnt++;
        pPacp->backend_sm.suppWhile = pPacp->backend_sm.suppTimeout;
    }

    DOT1X_SM_BACKEND_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_backend_request_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (TRUE == pPacp->pae_sm.eapReq)
    {
        if (pPacp->pae_sm.reqCnt < pPacp->reqMax)
        {
            if (0 == pPacp->pae_sm.txWhile)
            {
                pPacp->pae_sm.txWhile = pPacp->pae_sm.txPeriod;

                dot1x_sm_backend_request_transit(pPacp);
            }
        }
        else
        {
            authmgr_sess_t *pSess = NULL;

            pSess = (authmgr_sess_t *)pPacp->pParentSess;

            if (AUTHMGR_TYPE_NONE != authmgr_action_nextType_ret(pSess))
            {
                pSess->gotoNextType = TRUE;
                pPacp->delPacp = TRUE;

                DOT1X_DBG(pPacp, "No response from supplicant. Go to next method");

                return SYS_ERR_OK;
            }
            else
            {
                dot1x_sm_backend_timeout_transit(pPacp);

                return SYS_ERR_OK;
            }
        }
    }
    else
    {
        if (pPacp->backend_sm.reqCnt < pPacp->reqMax)
        {
            if (0 == pPacp->backend_sm.suppWhile)
            {
                pPacp->backend_sm.suppWhile = pPacp->backend_sm.suppTimeout;

                dot1x_sm_backend_request_transit(pPacp);
            }
        }
        else
        {
            pPacp->eapTimeout = TRUE;
        }
    }

    if (pPacp->eapTimeout)
    {
        DOT1X_DBG(pPacp, "Supplicant request timeout");

        dot1x_sm_backend_timeout_transit(pPacp);

        return SYS_ERR_OK;
    }

    if (pPacp->eapolEap)
    {
        dot1x_sm_backend_response_transit(pPacp);
        return SYS_ERR_OK;
    }

    if (pPacp->backend_sm.eapReq)
    {
        dot1x_sm_backend_request_transit(pPacp);
        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_backend_idle_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->eapTimeout = FALSE;
    pPacp->backend_sm.curState = DOT1X_BACKEND_IDLE;
    pPacp->authStart = FALSE;

    if (NULL != pPacp->pae_sm.fromSupp.pkt)
    {
        osal_free(pPacp->pae_sm.fromSupp.pkt);
        pPacp->pae_sm.fromSupp.pkt = NULL;
    }
    if (NULL != pPacp->pae_sm.fromSvr.pkt)
    {
        osal_free(pPacp->pae_sm.fromSvr.pkt);
        pPacp->pae_sm.fromSvr.pkt  = NULL;
    }
    pPacp->pae_sm.fromSupp.length = 0;
    pPacp->pae_sm.fromSvr.length = 0;

    pPacp->backend_sm.reqCnt = 0;

    DOT1X_SM_BACKEND_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_backend_idle_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->eapFail && pPacp->authStart)
    {
        dot1x_sm_backend_fail_transit(pPacp);
        return SYS_ERR_OK;
    }

    if (pPacp->backend_sm.eapReq && pPacp->authStart)
    {
        dot1x_sm_backend_request_transit(pPacp);
        return SYS_ERR_OK;
    }

    if (pPacp->eapSuccess && pPacp->authStart)
    {
        dot1x_sm_backend_success_transit(pPacp);
        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 dot1x_sm_backend_init_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->backend_sm.curState = DOT1X_BACKEND_INITIALIZE;
    _dot1x_sm_backend_abortAuth(pPacp);
    pPacp->backend_sm.eapNoReq = FALSE;
    pPacp->authAbort = FALSE;

    DOT1X_SM_BACKEND_CHK_NOW(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_backend_init_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    dot1x_sm_backend_idle_transit(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_backend_global_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if ( pPacp->initialize || pPacp->authAbort)
    {
        DOT1X_DBG(pPacp, "Backend SM is initialized. portControl:%d initialize:%d authAbort:%d",
                         pPacp->portControl,
                         pPacp->initialize,
                         pPacp->authAbort);

        dot1x_sm_backend_init_transit(pPacp);
    }

    return SYS_ERR_OK;
}

int32 dot1x_sm_backend_check(dot1x_pacp_t *pPacp)
{
    dot1x_sm_backend_global_check(pPacp);

    pPacp->backend_sm.chkNow = FALSE;

    switch (pPacp->backend_sm.curState)
    {
        case DOT1X_BACKEND_INITIALIZE:
            dot1x_sm_backend_init_check(pPacp);
            break;
        case DOT1X_BACKEND_IDLE:
            dot1x_sm_backend_idle_check(pPacp);
            break;
        case DOT1X_BACKEND_REQUEST:
            dot1x_sm_backend_request_check(pPacp);
            break;
        case DOT1X_BACKEND_RESPONSE:
            dot1x_sm_backend_response_check(pPacp);
            break;
        case DOT1X_BACKEND_IGNORE:
            dot1x_sm_backend_ignore_check(pPacp);
            break;
        case DOT1X_BACKEND_TIMEOUT:
            dot1x_sm_backend_timeout_check(pPacp);
            break;
        case DOT1X_BACKEND_FAIL:
            dot1x_sm_backend_fail_check(pPacp);
            break;
        case DOT1X_BACKEND_SUCCESS:
            dot1x_sm_backend_success_check(pPacp);
            break;
    }

    return SYS_ERR_OK;
}

