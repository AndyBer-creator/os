#include <authmgr/proto_authmgr.h>
#include <ksi_authmgr.h>
static int32 webauth_sm_disable_transit(webauth_t *pWebauth);
static int32 webauth_sm_init_transit(webauth_t *pWebauth);
static int32 webauth_sm_authed_transit(webauth_t *pWebauth);
static int32 webauth_sm_unauthed_transit(webauth_t *pWebauth);
static int32 webauth_sm_locked_transit(webauth_t *pWebauth);
static int32 webauth_sm_guest_transit(webauth_t *pWebauth);
static int32 webauth_sm_disable_transit(webauth_t *pWebauth)
{
    WEBAUTH_DBG(pWebauth, "");

    pWebauth->currState = AUTHMGR_STATUS_DISABLE;

    return SYS_ERR_OK;
}

static int32 webauth_sm_disable_check(webauth_t *pWebauth)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *)pWebauth->pParentSess;

    if (pWebauth->initialize)
    {
        pWebauth->initialize = FALSE;
        webauth_sm_init_transit(pWebauth);
        return SYS_ERR_OK;
    }

    pSess->delSess = TRUE;

    return SYS_ERR_OK;
}
static int32 webauth_sm_init_transit(webauth_t *pWebauth)
{
    authmgr_sess_t *pSess = NULL;

    WEBAUTH_DBG(pWebauth, "");

    pWebauth->currState = AUTHMGR_STATUS_INIT;

    pWebauth->authSuccess = FALSE;
    pWebauth->authFail = FALSE;
    pWebauth->reauthTimeout = FALSE;
    pWebauth->inactTimeout = FALSE;
    pWebauth->quietTimeout = FALSE;
    pWebauth->loginFailNum = 0;

    pSess = (authmgr_sess_t *)pWebauth->pParentSess;

    authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_INIT);
    authmgr_action_authStatus_clear(pSess);

    return SYS_ERR_OK;
}

static int32 webauth_sm_init_check(webauth_t *pWebauth)
{
    if (pWebauth->authSuccess)
    {
        webauth_sm_authed_transit(pWebauth);
        return SYS_ERR_OK;
    }

    if (pWebauth->authFail)
    {
        webauth_sm_unauthed_transit(pWebauth);
        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 webauth_sm_authed_transit(webauth_t *pWebauth)
{
    WEBAUTH_DBG(pWebauth, "");

    pWebauth->currState = AUTHMGR_STATUS_AUTHED;

    authmgr_action_authStatus_set((authmgr_sess_t *)pWebauth->pParentSess, AUTHMGR_STATUS_AUTHED);

    return SYS_ERR_OK;
}

static int32 webauth_sm_authed_check(webauth_t *pWebauth)
{
    if (pWebauth->reauthTimeout)
    {
        if (ENABLED == pWebauth->reauthEnbl)
            webauth_sm_init_transit(pWebauth);
        else
            webauth_sm_disable_transit(pWebauth);
        return SYS_ERR_OK;
    }

    if (pWebauth->inactTimeout)
    {
        webauth_sm_disable_transit(pWebauth);
        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 webauth_sm_unauthed_transit(webauth_t *pWebauth)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *) pWebauth->pParentSess;

    WEBAUTH_DBG(pWebauth, "");

    pWebauth->currState = AUTHMGR_STATUS_UNAUTHED;

    authmgr_action_authStatus_set((authmgr_sess_t *)pWebauth->pParentSess, AUTHMGR_STATUS_UNAUTHED);

    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_WEBAUTH_FAIL,
                pSess->suppInfo.mac.octet[0],
                pSess->suppInfo.mac.octet[1],
                pSess->suppInfo.mac.octet[2],
                pSess->suppInfo.mac.octet[3],
                pSess->suppInfo.mac.octet[4],
                pSess->suppInfo.mac.octet[5],
                LPSTR(pSess->suppInfo.port));

    return SYS_ERR_OK;
}

static int32 webauth_sm_unauthed_check(webauth_t *pWebauth)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *) pWebauth->pParentSess;

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

    if (IS_PORT_GUEST_VLAN_ENABLE(pSess->suppInfo.port))
    {
        webauth_sm_guest_transit(pWebauth);
    }
    else
    {
        webauth_sm_locked_transit(pWebauth);
    }

    return SYS_ERR_OK;
}
static int32 webauth_sm_locked_transit(webauth_t *pWebauth)
{
    WEBAUTH_DBG(pWebauth, "");

    pWebauth->currState = AUTHMGR_STATUS_LOCKED;

    authmgr_action_authStatus_set((authmgr_sess_t *)pWebauth->pParentSess, AUTHMGR_STATUS_LOCKED);

    return SYS_ERR_OK;
}

static int32 webauth_sm_locked_check(webauth_t *pWebauth)
{
    if (pWebauth->quietTimeout)
    {
        webauth_sm_disable_transit(pWebauth);
        return SYS_ERR_OK;
    }

    return SYS_ERR_OK;
}
static int32 webauth_sm_guest_transit(webauth_t *pWebauth)
{
    WEBAUTH_DBG(pWebauth, "");

    pWebauth->currState = AUTHMGR_STATUS_GUEST;

    authmgr_action_authStatus_set((authmgr_sess_t *)pWebauth->pParentSess, AUTHMGR_STATUS_GUEST);

    return SYS_ERR_OK;
}

static int32 webauth_sm_guest_check(webauth_t *pWebauth)
{
    authmgr_sess_t *pSess = NULL;

    if (pWebauth->guestChk)
    {
        pWebauth->guestChk = FALSE;

        pSess = (authmgr_sess_t *) pWebauth->pParentSess;

        if (FALSE == authmgr_action_guestActiveChk_ret(pSess))
        {
            authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_DISABLE);
        }
    }

    return SYS_ERR_OK;
}

int32 webauth_sm_check(webauth_t *pWebauth)
{
    SYS_PARAM_CHK((NULL == pWebauth), SYS_ERR_NULL_POINTER);

    switch (pWebauth->currState)
    {

        case AUTHMGR_STATUS_DISABLE:
            webauth_sm_disable_check(pWebauth);
            break;

        case AUTHMGR_STATUS_INIT:
            webauth_sm_init_check(pWebauth);
            break;

        case AUTHMGR_STATUS_AUTHED:
            webauth_sm_authed_check(pWebauth);
            break;

        case AUTHMGR_STATUS_UNAUTHED:
            webauth_sm_unauthed_check(pWebauth);
            break;

        case AUTHMGR_STATUS_LOCKED:
            webauth_sm_locked_check(pWebauth);
            break;

        case AUTHMGR_STATUS_GUEST:
            webauth_sm_guest_check(pWebauth);
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}
