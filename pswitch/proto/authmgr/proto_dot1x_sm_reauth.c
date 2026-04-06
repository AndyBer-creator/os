#include <authmgr/proto_authmgr.h>
static int32 dot1x_sm_reauth_reAuthenticate_transit(dot1x_pacp_t *pPacp);
static int32 dot1x_sm_reauth_init_transit(dot1x_pacp_t *pPacp);

static int32 dot1x_sm_reauth_reAuthenticate_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->reAuth_sm.curState = DOT1X_REAUTH_REAUTHENTICATE;
    pPacp->reAuthenticate = TRUE;

    return SYS_ERR_OK;
}

static int32 dot1x_sm_reauth_reAuthenticate_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    dot1x_sm_reauth_init_transit(pPacp);

    return SYS_ERR_OK;
}

static int32 dot1x_sm_reauth_init_transit(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    DOT1X_DBG(pPacp, "");

    pPacp->reAuth_sm.curState = DOT1X_REAUTH_INITIALIZE;
    pPacp->reAuthWhen = pPacp->reAuth_sm.reAuthPeriod;

    return SYS_ERR_OK;
}

static int32 dot1x_sm_reauth_init_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (pPacp->reAuthWhen == 0)
    {
        dot1x_sm_reauth_reAuthenticate_transit(pPacp);
    }

    return SYS_ERR_OK;
}

static int32 dot1x_sm_reauth_global_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    if (
        (pPacp->initialize)
        || (pPacp->portStatus == DOT1X_STATE_UNAUTHORIZED)
        || (!pPacp->reAuth_sm.reAuthEnabled))
    {
        dot1x_sm_reauth_init_transit(pPacp);
    }

    return SYS_ERR_OK;
}

int32 dot1x_sm_reauth_check(dot1x_pacp_t *pPacp)
{
    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);

    dot1x_sm_reauth_global_check(pPacp);

    switch (pPacp->reAuth_sm.curState)
    {
        case DOT1X_REAUTH_INITIALIZE:
            dot1x_sm_reauth_init_check(pPacp);
            break;
        case DOT1X_REAUTH_REAUTHENTICATE:
            dot1x_sm_reauth_reAuthenticate_check(pPacp);
            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}

