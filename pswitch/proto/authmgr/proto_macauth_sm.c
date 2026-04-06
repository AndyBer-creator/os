#include <authmgr/proto_authmgr.h>
#include <ksi_authmgr.h>
static int32 macauth_sm_disable_transit(macauth_t *pMacauth);
static int32 macauth_sm_init_transit(macauth_t *pMacauth);
static int32 macauth_sm_authed_transit(macauth_t *pMacauth);
static int32 macauth_sm_unauthed_transit(macauth_t *pMacauth);
static int32 macauth_sm_locked_transit(macauth_t *pMacauth);
static int32 macauth_sm_guest_transit(macauth_t *pMacauth);
static int32 macauth_sm_disable_transit(macauth_t *pMacauth)
{
    MACAUTH_DBG(pMacauth, "");

    pMacauth->currState = AUTHMGR_STATUS_DISABLE;

    return SYS_ERR_OK;
}

static int32 macauth_sm_disable_check(macauth_t *pMacauth)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *)pMacauth->pParentSess;

    if (pMacauth->initialize)
    {
        pMacauth->initialize = FALSE;
        macauth_sm_init_transit(pMacauth);
        return SYS_ERR_OK;
    }

    pSess->delSess = TRUE;

    return SYS_ERR_OK;
}
static int32 macauth_sm_init_transit(macauth_t *pMacauth)
{
    authmgr_sess_t *pSess = NULL;

    MACAUTH_DBG(pMacauth, "");

    pMacauth->currState = AUTHMGR_STATUS_INIT;

    pMacauth->authing = TRUE;
    pMacauth->authSuccess = FALSE;
    pMacauth->authFail = FALSE;
    pMacauth->reauthTimeout = FALSE;
    pMacauth->inactTimeout = FALSE;
    pMacauth->quietTimeout = FALSE;

    pSess = (authmgr_sess_t *)pMacauth->pParentSess;

    authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_INIT);
    authmgr_action_authStatus_clear(pSess);

    return SYS_ERR_OK;
}

static int32 macauth_sm_init_check(macauth_t *pMacauth)
{
    if (pMacauth->authing)
    {
        
        macauth_authInfo_t authInfo;
        authmgr_sess_t *pSess = NULL;

        SYS_MEM_CLEAR(authInfo);

        pSess = (authmgr_sess_t *)pMacauth->pParentSess;

        osal_memcpy(&authInfo.sessId, &pSess->sessId, sizeof(authmgr_sessId_t));
        osal_memcpy(&authInfo.mac, &pSess->suppInfo.mac, sizeof(sys_mac_t));
        authInfo.port = pSess->suppInfo.port;

        KSI_NOTIFY_MSG_DATA(KSI_AUTHMGR_MACAUTH_AUTHING, sizeof(macauth_authInfo_t), &authInfo);

        pMacauth->authing = FALSE;

        return SYS_ERR_OK;
    }

    if (pMacauth->authSuccess)
    {
        macauth_sm_authed_transit(pMacauth);
        return SYS_ERR_OK;
    }

    if (pMacauth->authFail)
    {
        macauth_sm_unauthed_transit(pMacauth);
        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 macauth_sm_authed_transit(macauth_t *pMacauth)
{
    MACAUTH_DBG(pMacauth, "");

    pMacauth->currState = AUTHMGR_STATUS_AUTHED;

    authmgr_action_authStatus_set((authmgr_sess_t *)pMacauth->pParentSess, AUTHMGR_STATUS_AUTHED);

    return SYS_ERR_OK;
}

static int32 macauth_sm_authed_check(macauth_t *pMacauth)
{
    if (pMacauth->reauthTimeout)
    {
        if (ENABLED == pMacauth->reauthEnbl)
            macauth_sm_init_transit(pMacauth);
        else
            macauth_sm_disable_transit(pMacauth);
        return SYS_ERR_OK;
    }

    if (pMacauth->inactTimeout)
    {
        macauth_sm_disable_transit(pMacauth);
        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 macauth_sm_unauthed_transit(macauth_t *pMacauth)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *) pMacauth->pParentSess;

    MACAUTH_DBG(pMacauth, "");

    pMacauth->currState = AUTHMGR_STATUS_UNAUTHED;

    authmgr_action_authStatus_set((authmgr_sess_t *)pMacauth->pParentSess, AUTHMGR_STATUS_UNAUTHED);

    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_MACAUTH_FAIL,
                pSess->suppInfo.mac.octet[0],
                pSess->suppInfo.mac.octet[1],
                pSess->suppInfo.mac.octet[2],
                pSess->suppInfo.mac.octet[3],
                pSess->suppInfo.mac.octet[4],
                pSess->suppInfo.mac.octet[5],
                LPSTR(pSess->suppInfo.port));

    return SYS_ERR_OK;
}

static int32 macauth_sm_unauthed_check(macauth_t *pMacauth)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *) pMacauth->pParentSess;

    if (pMacauth->forceAuthFail)
        return SYS_ERR_OK;

    if (AUTHMGR_TYPE_NONE != authmgr_action_nextType_ret(pSess))
    {
        pSess->gotoNextType = TRUE;

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

    if (IS_PORT_GUEST_VLAN_ENABLE(pSess->suppInfo.port))
    {
        macauth_sm_guest_transit(pMacauth);
    }
    else
    {
        macauth_sm_locked_transit(pMacauth);
    }

    return SYS_ERR_OK;
}
static int32 macauth_sm_locked_transit(macauth_t *pMacauth)
{
    MACAUTH_DBG(pMacauth, "");

    pMacauth->currState = AUTHMGR_STATUS_LOCKED;

    authmgr_action_authStatus_set((authmgr_sess_t *)pMacauth->pParentSess, AUTHMGR_STATUS_LOCKED);

    return SYS_ERR_OK;
}

static int32 macauth_sm_locked_check(macauth_t *pMacauth)
{
    if (pMacauth->quietTimeout)
    {
        macauth_sm_disable_transit(pMacauth);

        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 macauth_sm_guest_transit(macauth_t *pMacauth)
{
    MACAUTH_DBG(pMacauth, "");

    pMacauth->currState = AUTHMGR_STATUS_GUEST;

    authmgr_action_authStatus_set((authmgr_sess_t *)pMacauth->pParentSess, AUTHMGR_STATUS_GUEST);

    return SYS_ERR_OK;
}

static int32 macauth_sm_guest_check(macauth_t *pMacauth)
{
    authmgr_sess_t *pSess = NULL;

    if (pMacauth->guestChk)
    {
        pMacauth->guestChk = FALSE;

        pSess = (authmgr_sess_t *) pMacauth->pParentSess;

        if (FALSE == authmgr_action_guestActiveChk_ret(pSess))
        {
            authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_DISABLE);
        }
    }

    return SYS_ERR_OK;
}

int32 macauth_sm_check(macauth_t *pMacauth)
{
    SYS_PARAM_CHK((NULL == pMacauth), SYS_ERR_NULL_POINTER);

    switch (pMacauth->currState)
    {

        case AUTHMGR_STATUS_DISABLE:
            macauth_sm_disable_check(pMacauth);
            break;

        case AUTHMGR_STATUS_INIT:
            macauth_sm_init_check(pMacauth);
            break;

        case AUTHMGR_STATUS_AUTHED:
            macauth_sm_authed_check(pMacauth);
            break;

        case AUTHMGR_STATUS_UNAUTHED:
            macauth_sm_unauthed_check(pMacauth);
            break;

        case AUTHMGR_STATUS_LOCKED:
            macauth_sm_locked_check(pMacauth);
            break;

        case AUTHMGR_STATUS_GUEST:
            macauth_sm_guest_check(pMacauth);
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}
