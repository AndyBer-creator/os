#include <authmgr/proto_authmgr.h>
extern int32 macauth_sm_check(macauth_t *pMacauth);

macauth_t *macauth_create(sys_logic_port_t port)
{
    macauth_t *pMacauth = NULL;

    pMacauth = (macauth_t *) osal_alloc(sizeof(macauth_t));

    if (NULL != pMacauth)
    {
        osal_memset(pMacauth, 0, sizeof(macauth_t));

        pMacauth->reauthEnbl = g_authCfg.portCfg[port].reauthEnbl;
        pMacauth->initialize = TRUE;
        pMacauth->authing = TRUE;
    }

    return pMacauth;
}
void macauth_timer_routine(macauth_t *pMacauth)
{
    macauth_sm_check(pMacauth);

    return;
}

void macauth_timer_chk(macauth_t *pMacauth)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *)pMacauth->pParentSess;

    switch(pSess->status.authStatus)
    {
        case AUTHMGR_STATUS_AUTHED:
            
            if (authmgr_action_activeChk_ret(pSess))
                pSess->status.inactWhile = 0;

            if ((pSess->status.authorInfo.inactTime != 0)
                 && (pSess->status.inactWhile >= pSess->status.authorInfo.inactTime))
            {
                SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_TIMEOUT_INACT,
                            pSess->suppInfo.mac.octet[0],
                            pSess->suppInfo.mac.octet[1],
                            pSess->suppInfo.mac.octet[2],
                            pSess->suppInfo.mac.octet[3],
                            pSess->suppInfo.mac.octet[4],
                            pSess->suppInfo.mac.octet[5],
                            LPSTR(pSess->suppInfo.port),
                            pSess->sessId.cnt,
                            pSess->sessId.time);

                pMacauth->inactTimeout = TRUE;
            }
            
            if (pSess->status.authedWhile >= pSess->status.authorInfo.reauthTime)
            {
                if (ENABLED == pSess->pPortCfg->reauthEnbl)
                {
                    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_TIMEOUT_REAUTH,
                                pSess->suppInfo.mac.octet[0],
                                pSess->suppInfo.mac.octet[1],
                                pSess->suppInfo.mac.octet[2],
                                pSess->suppInfo.mac.octet[3],
                                pSess->suppInfo.mac.octet[4],
                                pSess->suppInfo.mac.octet[5],
                                LPSTR(pSess->suppInfo.port),
                                pSess->sessId.cnt,
                                pSess->sessId.time);

                    pMacauth->reauthTimeout = TRUE;
                }
            }
            break;

        case AUTHMGR_STATUS_LOCKED:
            
            if (pSess->status.quietWhile >= pSess->pPortCfg->quietPeriod)
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

                pMacauth->quietTimeout = TRUE;
            }
            break;

        case AUTHMGR_STATUS_UNAUTHED:
            break;

        case AUTHMGR_STATUS_GUEST:
            pMacauth->guestChk = TRUE;
            break;

        default:
            break;
    }

    return;
}
int32 authmgr_macauthStatus_set(macauth_authInfo_t *pAuthInfo)
{
    authmgr_sess_t *pSess = NULL;
    macauth_t      *pMacauth = NULL;
    int32          ret = SYS_ERR_OK;

    SYS_PARAM_CHK((NULL == pAuthInfo), SYS_ERR_NULL_POINTER);

    if (NULL != (pSess = authmgr_sess_getById_ret(&pAuthInfo->sessId)))
    {
        AUTHMGR_SEM_LOCK(authmgr_sem);

        if (NULL != (pMacauth = pSess->pMacauth))
        {
            switch (pAuthInfo->authSts)
            {
                case SYS_AUTH:
                    ret = authmgr_action_authorInfo_set(pSess, &pAuthInfo->author, &pAuthInfo->authorSrc);
                    if (SYS_ERR_OK == ret)
                        pMacauth->authSuccess = TRUE;
                    else
                        pMacauth->authFail = TRUE;
                    break;

                case SYS_UNAUTH:
                    pMacauth->authFail = TRUE;
                    if (pAuthInfo->ctrl == AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED)
                        pMacauth->forceAuthFail = TRUE;
                    break;

                default:
                    break;
            }
        }

        AUTHMGR_SEM_UNLOCK(authmgr_sem);
    }

    return SYS_ERR_OK;
}

