#include <rsd/rsd_sys.h>
#include <rsd/rsd_nic.h>

#include <authmgr/proto_authmgr.h>
extern sys_mac_t g_dot1xEapolMac;
extern int32 dot1x_sm_backend_check(dot1x_pacp_t *pPacp);
extern int32 dot1x_sm_pae_check(dot1x_pacp_t *pPacp);
extern int32 dot1x_sm_reauth_check(dot1x_pacp_t *pPacp);
extern int32 dot1x_sm_timer_check(dot1x_pacp_t *pPacp);

int32 dot1x_init(void)
{
    g_dot1xEapolMac.octet[0] = 0x01;
    g_dot1xEapolMac.octet[1] = 0x80;
    g_dot1xEapolMac.octet[2] = 0xC2;
    g_dot1xEapolMac.octet[3] = 0x00;
    g_dot1xEapolMac.octet[4] = 0x00;
    g_dot1xEapolMac.octet[5] = 0x03;

    return SYS_ERR_OK;
}

dot1x_pacp_t *dot1x_pacp_create(sys_logic_port_t port)
{
    dot1x_pacp_t *pPacp = NULL;

    pPacp = (dot1x_pacp_t *) osal_alloc(sizeof(dot1x_pacp_t));

    if (NULL != pPacp)
    {
        osal_memset(pPacp, 0, sizeof(dot1x_pacp_t));

        switch (g_authCfg.portCfg[port].ctrlType)
        {
            case AUTHMGR_PORTCTRL_AUTO:
                pPacp->portControl = DOT1X_MODE_AUTO;
                break;

            case AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED:
                pPacp->portControl = DOT1X_MODE_FORCE_UNAUTHORIZED;
                break;

            case AUTHMGR_PORTCTRL_FORCE_AUTHORIZED:
                pPacp->portControl = DOT1X_MODE_FORCE_AUTHORIZED;
                break;

            default:
                pPacp->portControl = DOT1X_MODE_FORCE_AUTHORIZED;
                break;
        }

        pPacp->port = port;
        pPacp->portEnabled = TRUE;
        pPacp->portValid = TRUE;
        pPacp->portMode = g_authCfg.portCfg[port].ctrlType;

        pPacp->reqMax = g_authCfg.portCfg[port].maxReq;

        pPacp->pae_sm.reAuthMax = g_authCfg.portCfg[port].maxReq;
        pPacp->pae_sm.quietPeriod = g_authCfg.portCfg[port].quietPeriod;
        pPacp->pae_sm.txPeriod = g_authCfg.portCfg[port].txPeriod;
        pPacp->reAuth_sm.reAuthEnabled = g_authCfg.portCfg[port].reauthEnbl;
        pPacp->reAuth_sm.reAuthPeriod = g_authCfg.portCfg[port].reauthPeriod;
        pPacp->reAuthWhen = g_authCfg.portCfg[port].reauthPeriod;

        pPacp->backend_sm.serverTimeout = g_authCfg.portCfg[port].serverTimeout;
        pPacp->backend_sm.suppTimeout = g_authCfg.portCfg[port].suppTimeout;

        pPacp->currentId = 1;
    }

    return pPacp;
}

void dot1x_sm_check_trigger(dot1x_pacp_t *pPacp)
{
    if (pPacp->smChecking)
        return;

    pPacp->smChecking = TRUE;

    pPacp->pae_sm.chkNow = TRUE;
    pPacp->backend_sm.chkNow = TRUE;

    while (pPacp->pae_sm.chkNow || pPacp->backend_sm.chkNow)
    {
        if (pPacp->pae_sm.chkNow)
        {
            dot1x_sm_pae_check(pPacp);
        }

        if (pPacp->backend_sm.chkNow)
        {
            dot1x_sm_backend_check(pPacp);
        }
    }

    pPacp->smChecking = FALSE;

    return;
}

void dot1x_timer_count(dot1x_pacp_t *pPacp)
{
    dot1x_sm_timer_check(pPacp);

    return;
}

void dot1x_timer_routine(dot1x_pacp_t *pPacp)
{
    
    dot1x_sm_check_trigger(pPacp);

    return;
}

void dot1x_timer_chk(dot1x_pacp_t *pPacp)
{
    authmgr_sess_t *pSess = (authmgr_sess_t *)pPacp->pParentSess;

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

                pPacp->inactTimeout = TRUE;
            }

            if ((pSess->status.authorInfo.reauthTime != 0)
                && (pPacp->reAuthWhen == 0))
            {
                if (ENABLED == pSess->pPortCfg->reauthEnbl )
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

                    SYS_MEM_CLEAR(pPacp->backend_sm.stateAvp);
                    pPacp->reAuthenticate = TRUE;
                }

                pPacp->reAuthWhen = pSess->status.authorInfo.reauthTime;
            }
            break;

        case AUTHMGR_STATUS_LOCKED:
            
            break;

        case AUTHMGR_STATUS_UNAUTHED:
            break;

        case AUTHMGR_STATUS_GUEST:
            pPacp->guestChk = TRUE;
            break;

        default:
            break;
    }

    return;
}
int32 authmgr_dot1xRadius_rx(dot1x_rad_recv_t recv)
{
    int32 ret = SYS_ERR_OK;
    authmgr_sess_t *pSess = NULL;

    if (NULL != (pSess = authmgr_sess_getById_ret(&recv.sessId)))
    {
        AUTHMGR_SEM_LOCK(authmgr_sem);

        if (0 != recv.stateAvp.len)
        {
            osal_memcpy(&pSess->pDot1x->backend_sm.stateAvp, &recv.stateAvp, sizeof(dot1x_radius_stateAvp_t));
        }

        ret = eapol_radiusRx_process(pSess->pDot1x, &recv);

        AUTHMGR_SEM_UNLOCK(authmgr_sem);
    }

    return ret;
}
int32 authmgr_dot1xRadiusSendId_get(authmgr_sessId_t sessId, uint32 *pId)
{
    int32 ret = SYS_ERR_OK;
    authmgr_sess_t *pSess = NULL;

    if (NULL != (pSess = authmgr_sess_getById_ret(&sessId)))
    {
        AUTHMGR_SEM_LOCK(authmgr_sem);

        *pId = pSess->pDot1x->backend_sm.radiusSendId;

        AUTHMGR_SEM_UNLOCK(authmgr_sem);
    }

    return ret;
}
