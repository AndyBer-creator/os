#include <rsd/rsd_auth.h>
#include <rsd/rsd_l2.h>
#include <rsd/rsd_l2_limit.h>
#include <rsd/rsd_sys.h>
#include <rsd/rsd_port.h>
#include <rsd/rsd_vlan.h>
#include <rsd/rsd_trunk.h>
#include <ksi/ksi_core.h>
#include <authmgr/proto_authmgr.h>
static volatile int32 g_isSessProcess = FALSE;
osal_mutex_t authmgr_sem;
sys_logic_portmask_t      g_portAuthLpm;

sys_mac_t                 g_sysMac;
sys_mac_t                 g_emptyMac;
#ifdef CONFIG_SYS_PROTO_DOT1X
sys_mac_t                 g_dot1xEapolMac;
#endif
static authmgr_sess_t     *g_sessList = NULL;
static authmgr_sess_ptr_t g_portSessFirst[SYS_NORMALPORT_NUM_MAX];
static uint32             g_sessCnt = 0;
static uint32             g_sessCntHistory = 0;
static uint32             g_portSessCnt[SYS_NORMALPORT_NUM_MAX];
authmgr_cfg_t             g_authCfg;

static uint32             g_maxSessNum = 0;

const sys_text_t text_authmgr_type[] =
{
    {    "none"  },
#ifdef CONFIG_SYS_PROTO_DOT1X
    {   "dot1x" },
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    {     "mac"   },
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    {     "web"   },
#endif
};

ROUTINE_INIT(count_routine, SYS_TIMER_SEC);
ROUTINE_INIT(sess_routine, SYS_TIMER_MSEC);
static l2g_act_t _authmgr_pkt_chk(sys_nic_pkt_t *pPkt, void *pCookie);
static int32 _authmgr_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie);
static authmgr_sess_t *_authmgr_sess_add(sys_logic_port_t port, sys_mac_t *pHost, sys_vid_t vid);
static authmgr_sess_t *_authmgr_sess_ret(sys_mac_t *pHost);
static authmgr_sess_t *_authmgr_sess4intr_ret(sys_logic_port_t port, sys_mac_t *pHost);
static int32 _authmgr_portSess_clear(sys_logic_port_t port);
static int32 _authmgr_portState_set(sys_logic_port_t port);
static int32 _authmgr_sess_del(void);

static rsd_nic_handler_t l2gAuthmgrHandler =
{
    .priority       = RSD_NIC_HANDLER_PRI_AUTHMGR,
    .noCondition    = TRUE, 
    .action         = RSD_NIC_HANDLER_ACT_TRAP,
    .rx_cbf         = _authmgr_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_NONE,
    .chk_cbf        = _authmgr_pkt_chk,
    .pName          = "AUTHMGR RX Thread",
    .pCookie        = NULL,
};

#ifdef CONFIG_SYS_PROTO_DOT1X
static rsd_nic_handler_t l2gEapolHandler =
{
    .priority       = RSD_NIC_HANDLER_PRI_AUTHMGR,
    .ethertype      = 0x888E,
    .action         = RSD_NIC_HANDLER_ACT_TRAP,
    .rx_cbf         = _authmgr_pkt_rx,
    .rx_careMask    = RSD_NIC_RX_CARE_NONE,
    .chk_cbf        = _authmgr_pkt_chk,
    .pName          = "AUTHMGR EAPOL RX Thread",
    .pCookie        = NULL,
};
#endif
static l2g_act_t _authmgr_pkt_chk(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_mac_t *pSrcMac = NULL;
    sys_logic_port_t srcPort;
    authmgr_sess_t *pSess = NULL;

    if (pPkt == NULL)
        return RSD_NIC_HANDLER_ACT_NONE;

    srcPort = pPkt->rx_tag.source_port;
    pSrcMac = (sys_mac_t *)(pPkt->data + sizeof(sys_mac_t));

    if (eapol_isEapPkt_ret(pPkt))
    {
        if (ENABLED == g_authCfg.typeEnbl[AUTHMGR_TYPE_DOT1X]
            && AUTHMGR_PORTCTRL_DISABLE != g_authCfg.portCfg[srcPort].ctrlType)
            return RSD_NIC_HANDLER_ACT_TRAP;
    }

    if (IS_NIC_REASON_MASK_SET(*pPkt, NIC_RX_REASON_L2_PMV))
    {
        return RSD_NIC_HANDLER_ACT_TRAP;
    }

    if (!IS_LOGIC_PORTMASK_PORTSET(g_portAuthLpm, srcPort))
        return RSD_NIC_HANDLER_ACT_NONE;

    if (NULL == (pSess = _authmgr_sess4intr_ret(srcPort, pSrcMac)))
    {
        
        if (AUTHMGR_MODE_MULTI_HOST == g_authCfg.portCfg[srcPort].mode && (g_portSessFirst[srcPort].pSess != NULL))
        {
            
            if (AUTHMGR_STATUS_INIT == g_portSessFirst[srcPort].pSess->status.authStatus)
                return RSD_NIC_HANDLER_ACT_DROP;
            else if (AUTHMGR_STATUS_AUTHED == g_portSessFirst[srcPort].pSess->status.authStatus)
                return RSD_NIC_HANDLER_ACT_NONE;
        }

        return RSD_NIC_HANDLER_ACT_TRAP;
    }

    switch (pSess->typeOrder.order[pSess->currTypeIdx])
    {
#ifdef CONFIG_SYS_PROTO_DOT1X
        case AUTHMGR_TYPE_DOT1X:
            if (eapol_isEapPkt_ret(pPkt))
                return RSD_NIC_HANDLER_ACT_TRAP;

            if (AUTHMGR_STATUS_AUTHED == pSess->status.authStatus)
                return RSD_NIC_HANDLER_ACT_NONE;
            else
                return RSD_NIC_HANDLER_ACT_DROP;
            break;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
        case AUTHMGR_TYPE_MAC:
            if (AUTHMGR_STATUS_AUTHED == pSess->status.authStatus)
                return RSD_NIC_HANDLER_ACT_NONE;
            else
                return RSD_NIC_HANDLER_ACT_DROP;
            break;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        case AUTHMGR_TYPE_WEB:
            return webauth_pkt_chk(pPkt, pCookie);
            break;
#endif

        default:
            break;
    }

    return RSD_NIC_HANDLER_ACT_NONE;
}

static int32 _authmgr_pkt_rx(sys_nic_pkt_t *pPkt, void *pCookie)
{
    sys_mac_t *pSrcMac = NULL;
    sys_logic_port_t srcPort;
    authmgr_sess_t *pSess = NULL;

    if (pPkt == NULL)
        return SYS_ERR_OK;

    srcPort = pPkt->rx_tag.source_port;
    pSrcMac = (sys_mac_t *)(pPkt->data + sizeof(sys_mac_t));

    if (eapol_isEapPkt_ret(pPkt))
    {
        if (AUTHMGR_MODE_MULTI_HOST == g_authCfg.portCfg[srcPort].mode)
        {
            if (AUTHMGR_PORTCTRL_FORCE_AUTHORIZED == g_authCfg.portCfg[srcPort].ctrlType)
            {
                eapol_pkt_unauthMcast_tx(srcPort, EAP_SUCCESS, 1);

                goto exit;
            }
            else if (AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED == g_authCfg.portCfg[srcPort].ctrlType)
            {
                eapol_pkt_unauthMcast_tx(srcPort, EAP_FAILURE, 1);

                goto exit;
            }
        }
    }

    if (IS_NIC_REASON_MASK_SET(*pPkt, NIC_RX_REASON_L2_PMV))
    {
        sys_logic_port_t orgPort = pPkt->rx_tag.ext_source_port;

        if (IS_LOGIC_PORTMASK_PORTSET(g_portAuthLpm, orgPort))
        {
            if (NULL != (pSess = _authmgr_sess_ret(pSrcMac)))
            {
                if (orgPort == pSess->suppInfo.port)
                {
                    pSess->delSess = TRUE;
                    _authmgr_sess_del();

                    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_MAC_MOVE,
                            SHOW_MAC(pSrcMac->octet),
                            LPSTR(orgPort),
                            LPSTR(srcPort));

                    goto exit;
                }
            }
        }
        else
        {
            rsd_l2_mac_clearByPort(orgPort);
        }

        if (!IS_LOGIC_PORTMASK_PORTSET(g_portAuthLpm, srcPort))
            goto exit;
    }

    if (NULL == (pSess = _authmgr_sess_ret(pSrcMac)))
    {
        sys_vid_t vid = 0;
        rsd_nic_pktVid_get(pPkt, &vid);

        if (AUTHMGR_MODE_MULTI_HOST == g_authCfg.portCfg[srcPort].mode && (g_portSessFirst[srcPort].pSess != NULL))
        {
            if (AUTHMGR_STATUS_INIT == g_portSessFirst[srcPort].pSess->status.authStatus
                || AUTHMGR_STATUS_AUTHED == g_portSessFirst[srcPort].pSess->status.authStatus)
                goto exit;

            _authmgr_portSess_clear(srcPort);
            goto exit;
        }
        else
        {
            uint32 i = 0;
            uint32 authExist = FALSE;

            for (i = 0; i < AUTHMGR_TYPE_END; i++)
            {
                if ((ENABLED == g_authCfg.typeEnbl[i])
                    && (ENABLED == g_authCfg.portCfg[srcPort].typeEnbl[i]))
                {
                    authExist = TRUE;
                    break;
                }
            }

            if (FALSE == authExist)
                goto exit;

            if (NULL == (pSess = _authmgr_sess_add(srcPort, pSrcMac, vid)))
            {
                AUTHMGR_DBG("Do not find available session");
                goto exit;
            }

#ifdef CONFIG_SYS_PROTO_WEBAUTH
            
            pSess->pWebauth = NULL;
#endif
        }
    }

    switch (pSess->typeOrder.order[pSess->currTypeIdx])
    {
#ifdef CONFIG_SYS_PROTO_DOT1X
        case AUTHMGR_TYPE_DOT1X:
            if (eapol_isEapPkt_ret(pPkt))
            {
                if (NULL == pSess->pDot1x)
                {
                    if (NULL != (pSess->pDot1x = dot1x_pacp_create(pSess->suppInfo.port)))
                    {
                        pSess->pDot1x->pParentSess = pSess;

                        AUTHMGR_DBG("Create dot1x pacp for sess:%08X%08X on port %s success",
                                    pSess->sessId.cnt,
                                    pSess->sessId.time,
                                    LPSTR(pSess->suppInfo.port));
                    }
                    else
                    {
                        break;
                    }
                }
                if (!((pSess->pDot1x->pae_sm.curState == DOT1X_PAE_HELD)
                       && !IS_PORT_GUEST_VLAN_ENABLE(pSess->suppInfo.port)))
                    eapol_pkt_rx(pSess->pDot1x, pPkt);
            }
            break;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
        case AUTHMGR_TYPE_MAC:
            break;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        case AUTHMGR_TYPE_WEB:
            if (NULL == pSess->pWebauth)
            {
                if (NULL != (pSess->pWebauth = webauth_create(pSess->suppInfo.port)))
                {
                    pSess->pWebauth->pParentSess = pSess;

                    rsd_auth_unauthOpenPort_add(srcPort);

                    AUTHMGR_DBG("Create web struct for sess:%08X%08X on port %s success",
                                pSess->sessId.cnt,
                                pSess->sessId.time,
                                LPSTR(pSess->suppInfo.port));
                }
                else
                {
                    break;
                }
            }

            pSess->pWebauth->inactWhile = 0;

            if (pSess->status.authStatus != AUTHMGR_STATUS_LOCKED)
            {
                
                if (TRUE == webauth_pkt_rx(pPkt, pCookie))
                    return SYS_ERR_OK;
            }
            break;
#endif

        default:
            break;
    }

exit:
    if (SYS_ERR_OK != rsd_nic_pkt_free(pPkt))
    {
        AUTHMGR_DBG("AUTHMGR pkt free falied!");
    }

    return SYS_ERR_OK;
}

static authmgr_sess_t *_authmgr_sess_create(sys_logic_port_t port, sys_mac_t *pHost)
{
    authmgr_sess_t *pNewSess = NULL;
    uint32 uptime = 0;

    if ((AUTHMGR_MODE_SINGLE_HOST == g_authCfg.portCfg[port].mode) && (g_portSessCnt[port] != 0))
    {
        AUTHMGR_DBG("Detect more than one host on single host mode port %s", LPSTR(port));

        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_SECURITY_VIOLATION,
                    LPSTR(port),
                    pHost->octet[0],
                    pHost->octet[1],
                    pHost->octet[2],
                    pHost->octet[3],
                    pHost->octet[4],
                    pHost->octet[5]);
        return NULL;
    }
    else if ((0 != g_authCfg.portCfg[port].maxHosts) && (g_portSessCnt[port] == g_authCfg.portCfg[port].maxHosts))
    {
        AUTHMGR_DBG("Exceed max hosts number %u on port %s", g_authCfg.portCfg[port].maxHosts, LPSTR(port));

        SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_SECURITY_VIOLATION,
                    LPSTR(port),
                    pHost->octet[0],
                    pHost->octet[1],
                    pHost->octet[2],
                    pHost->octet[3],
                    pHost->octet[4],
                    pHost->octet[5]);
        return NULL;
    }

    pNewSess = (authmgr_sess_t *) osal_alloc(sizeof(authmgr_sess_t));

    if (NULL != pNewSess)
    {
        uint32 i = 0;

        osal_memset(pNewSess, 0, sizeof(authmgr_sess_t));
        osal_time_sysUptime_get(&uptime);

        if (0 == g_portSessCnt[port])
            g_portSessFirst[port].pSess = pNewSess;

        INC(g_sessCnt);
        INC(g_portSessCnt[port]);
        INC(g_sessCntHistory);
        pNewSess->sessId.cnt = g_sessCntHistory;
        pNewSess->sessId.time = uptime;
        pNewSess->suppInfo.port = port;
        osal_memcpy(&pNewSess->suppInfo.mac, pHost, sizeof(sys_mac_t));
        osal_memcpy(&pNewSess->typeOrder, &g_authCfg.portCfg[port].typeOrder, sizeof(authmgr_order_t));

        pNewSess->status.authStatus = AUTHMGR_STATUS_INIT;
        pNewSess->pPortCfg = &g_authCfg.portCfg[port];
        pNewSess->currMode = g_authCfg.portCfg[port].mode;

        for (i = 0; i < AUTHMGR_TYPE_END; i++)
        {
            if (AUTHMGR_TYPE_NONE == pNewSess->typeOrder.order[i])
                break;

            pNewSess->currTypeIdx = i;
            if (ENABLED == pNewSess->pPortCfg->typeEnbl[pNewSess->typeOrder.order[i]]
                && ENABLED == g_authCfg.typeEnbl[pNewSess->typeOrder.order[i]])
            {
                break;
            }
        }

        AUTHMGR_DBG("Add session with id %08X%08X for host %02x:%02x:%02x:%02x:%02x:%02x on port %s order [%s,%s,%s]",
                    pNewSess->sessId.cnt,
                    pNewSess->sessId.time,
                    pNewSess->suppInfo.mac.octet[0],
                    pNewSess->suppInfo.mac.octet[1],
                    pNewSess->suppInfo.mac.octet[2],
                    pNewSess->suppInfo.mac.octet[3],
                    pNewSess->suppInfo.mac.octet[4],
                    pNewSess->suppInfo.mac.octet[5],
                    LPSTR(pNewSess->suppInfo.port),
                    text_authmgr_type[pNewSess->typeOrder.order[0]].text,
                    text_authmgr_type[pNewSess->typeOrder.order[1]].text,
                    text_authmgr_type[pNewSess->typeOrder.order[2]].text);
    }

    return pNewSess;
}

static authmgr_sess_t *_authmgr_sess_add(sys_logic_port_t port, sys_mac_t *pHost, sys_vid_t vid)
{
    uint32 sessNum = 0;
    authmgr_sess_t *pSess = NULL;
    authmgr_sess_t *pSessLast = NULL;
    authmgr_sess_t *pNewSess = NULL;
    sys_vid_t      pktVid = vid;
    sys_vid_t      currVid = vid;

    if (IS_PORT_GUEST_VLAN_ENABLE(port) && vid == g_authCfg.guestVlan)
    {
        rsd_vlan_portAdminPvid_get(port, &currVid);

        AUTHMGR_DBG("Paket from guest vid %d port %s and org vid %d", g_authCfg.guestVlan, LPSTR(port), currVid);
    }

    AUTHMGR_SEM_LOCK(authmgr_sem);

    g_isSessProcess = TRUE;

    if (NULL == g_sessList)
    {
        g_sessList = _authmgr_sess_create(port, pHost);
        if (NULL != g_sessList)
        {
            g_sessList->suppInfo.vid = pktVid;
            g_sessList->status.currVid = currVid;
        }

        g_isSessProcess = FALSE;

        AUTHMGR_SEM_UNLOCK(authmgr_sem);

        return g_sessList;
    }

    pSess = g_sessList;

    while (pSess != NULL)
    {
        
        if ((pSess->suppInfo.port == port)
             && (0 == osal_memcmp(pHost, &pSess->suppInfo.mac, sizeof(sys_mac_t))))
        {
            g_isSessProcess = FALSE;

            AUTHMGR_SEM_UNLOCK(authmgr_sem);

            return pSess;
        }

        sessNum++;

        pSessLast = pSess;
        pSess = pSess->next;
    }

    if ((sessNum > g_maxSessNum) || (NULL == pSessLast))
    {
        g_isSessProcess = FALSE;

        AUTHMGR_SEM_UNLOCK(authmgr_sem);

        return NULL;
    }

    pNewSess = _authmgr_sess_create(port, pHost);
    if (NULL != pNewSess)
    {
        pNewSess->suppInfo.vid = pktVid;
        pNewSess->status.currVid = currVid;
        pSessLast->next = pNewSess;
    }

    g_isSessProcess = FALSE;

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    return pNewSess;
}

static authmgr_sess_t *_authmgr_sess_ret(sys_mac_t *pHost)
{
    authmgr_sess_t *pSess = NULL;

    AUTHMGR_SEM_LOCK(authmgr_sem);

    pSess = g_sessList;

    while (pSess != NULL)
    {
        if ((0 == osal_memcmp(pHost, &pSess->suppInfo.mac, sizeof(sys_mac_t))))
        {
            AUTHMGR_SEM_UNLOCK(authmgr_sem);

            return pSess;
        }

        pSess = pSess->next;
    }

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    return NULL;
}

static authmgr_sess_t *_authmgr_sess4intr_ret(sys_logic_port_t port, sys_mac_t *pHost)
{
    authmgr_sess_t *pSess = NULL;

    pSess = g_sessList;

    while (pSess != NULL)
    {
        if ((0 == osal_memcmp(pHost, &pSess->suppInfo.mac, sizeof(sys_mac_t)))
             && (pSess->suppInfo.port == port))
        {
            return pSess;
        }

        pSess = pSess->next;
    }

    return NULL;
}
authmgr_sess_t *authmgr_sess_getById_ret(authmgr_sessId_t *pSessId)
{
    authmgr_sess_t *pSess = NULL;

    AUTHMGR_SEM_LOCK(authmgr_sem);

    pSess = g_sessList;

    while (pSess != NULL)
    {
        if ((0 == osal_memcmp(pSessId, &pSess->sessId, sizeof(authmgr_sessId_t))))
        {
            AUTHMGR_SEM_UNLOCK(authmgr_sem);

            return pSess;
        }

        pSess = pSess->next;
    }

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    return NULL;
}

authmgr_sess_t *authmgr_sess_getBySuppInfo_ret(authmgr_suppInfo_t *pSuppInfo)
{
    authmgr_sess_t *pSess = NULL;

    AUTHMGR_SEM_LOCK(authmgr_sem);

    pSess = g_sessList;

    while (pSess != NULL)
    {
        if ((0 == osal_memcmp(pSuppInfo, &pSess->suppInfo, sizeof(authmgr_suppInfo_t))))
        {
            AUTHMGR_SEM_UNLOCK(authmgr_sem);

            return pSess;
        }

        pSess = pSess->next;
    }

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    return NULL;
}

void _authmgr_sess_typeData_free(authmgr_sess_t *pSess, authmgr_type_t type)
{
    switch(type)
    {
#ifdef CONFIG_SYS_PROTO_DOT1X
        case AUTHMGR_TYPE_DOT1X:
            if (NULL != pSess->pDot1x)
            {
                
                switch (g_authCfg.portCfg[pSess->suppInfo.port].ctrlType)
                {
                    case AUTHMGR_PORTCTRL_DISABLE:
                    case AUTHMGR_PORTCTRL_FORCE_AUTHORIZED:
                        eapol_pkt_tx(pSess->pDot1x, EAP_SUCCESS);
                        break;

                    case AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED:
                    case AUTHMGR_PORTCTRL_AUTO:
                        eapol_pkt_tx(pSess->pDot1x, EAP_FAILURE);
                        break;

                    default:
                        break;
                }

                if (NULL != pSess->pDot1x->pae_sm.fromSupp.pkt)
                    osal_free(pSess->pDot1x->pae_sm.fromSupp.pkt);
                if (NULL != pSess->pDot1x->pae_sm.fromSvr.pkt)
                    osal_free(pSess->pDot1x->pae_sm.fromSvr.pkt);

                osal_free(pSess->pDot1x);
            }
            break;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
        case AUTHMGR_TYPE_MAC:
            if (NULL != pSess->pMacauth)
            {
                osal_free(pSess->pMacauth);
            }
            break;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        case AUTHMGR_TYPE_WEB:
            if (NULL != pSess->pWebauth)
            {
                osal_free(pSess->pWebauth);

                rsd_auth_unauthOpenPort_del(pSess->suppInfo.port);
            }
            break;
#endif

        default:
            break;
    }

    return;
}

static int32 _authmgr_sess_del(void)
{
    authmgr_sess_t *pSess = NULL;
    authmgr_sess_t *pSessPrev = NULL;
    sys_logic_port_t lp = 0;

    AUTHMGR_SEM_LOCK(authmgr_sem);

    g_isSessProcess = TRUE;

    pSess = g_sessList;

    while (pSess != NULL)
    {
        if (pSess->delSess == TRUE)
        {
            lp = pSess->suppInfo.port;

            AUTHMGR_DBG("Delete sess:%08X%08X on port %s",
                        pSess->sessId.cnt,
                        pSess->sessId.time,
                        LPSTR(lp));

            authmgr_action_authStatus_set(pSess, AUTHMGR_STATUS_DISABLE);

#ifdef CONFIG_SYS_PROTO_DOT1X
            _authmgr_sess_typeData_free(pSess, AUTHMGR_TYPE_DOT1X);
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
            _authmgr_sess_typeData_free(pSess, AUTHMGR_TYPE_MAC);
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
            _authmgr_sess_typeData_free(pSess, AUTHMGR_TYPE_WEB);
#endif

            if (pSess == g_sessList)
            {
                g_sessList = pSess->next;

                osal_free(pSess);

                pSess = g_sessList;

                DEC(g_sessCnt);
                DEC(g_portSessCnt[lp]);

                if (0 == g_portSessCnt[lp])
                    g_portSessFirst[lp].pSess = NULL;

                continue;
            }
            else
            {
                pSessPrev->next = pSess->next;

                osal_free(pSess);

                pSess = pSessPrev->next;

                DEC(g_sessCnt);
                DEC(g_portSessCnt[lp]);

                if (0 == g_portSessCnt[lp])
                    g_portSessFirst[lp].pSess = NULL;

                continue;
            }
        }

        pSessPrev = pSess;
        pSess = pSess->next;
    }

    g_isSessProcess = FALSE;

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    return SYS_ERR_OK;
}

static int32 _authmgr_portSess_clear(sys_logic_port_t port)
{
    authmgr_sess_t *pSess = NULL;

    AUTHMGR_SEM_LOCK(authmgr_sem);

    pSess = g_sessList;

    while (pSess != NULL)
    {
        if (pSess->suppInfo.port == port)
        {
            pSess->delSess = TRUE;
        }

        pSess = pSess->next;
    }

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    return SYS_ERR_OK;
}

static int32 _authmgr_allSess_clear(void)
{
    authmgr_sess_t *pSess = NULL;

    AUTHMGR_SEM_LOCK(authmgr_sem);

    pSess = g_sessList;

    while (pSess != NULL)
    {
        pSess->delSess = TRUE;
        pSess = pSess->next;
    }

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    return SYS_ERR_OK;
}
static void authmgr_timer_count(unsigned long id)
{
    authmgr_sess_t *pSess = NULL;
    uint32 isDelSess = FALSE;

    AUTHMGR_SEM_LOCK(authmgr_sem);

    pSess = g_sessList;

    while (pSess != NULL)
    {
        switch (pSess->typeOrder.order[pSess->currTypeIdx])
        {
#ifdef CONFIG_SYS_PROTO_DOT1X
            case AUTHMGR_TYPE_DOT1X:
                if ((NULL != pSess->pDot1x) && (FALSE == pSess->pDot1x->delPacp))
                {
                    dot1x_timer_count(pSess->pDot1x);
                }
                break;
#endif

            default:
                break;
        }

        switch(pSess->status.authStatus)
        {
            case AUTHMGR_STATUS_AUTHED:
                INC(pSess->status.authedWhile);
                INC(pSess->status.inactWhile);
                break;

            case AUTHMGR_STATUS_GUEST:
                INC(pSess->status.authedWhile);
                break;

            case AUTHMGR_STATUS_LOCKED:
                INC(pSess->status.quietWhile);
                break;

            case AUTHMGR_STATUS_UNAUTHED:
                INC(pSess->status.unauthedWhile);
                break;

            default:
                break;
        }

        switch (pSess->typeOrder.order[pSess->currTypeIdx])
        {
#ifdef CONFIG_SYS_PROTO_DOT1X
            case AUTHMGR_TYPE_DOT1X:
                if (NULL != pSess->pDot1x)
                    dot1x_timer_chk(pSess->pDot1x);
                break;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
            case AUTHMGR_TYPE_MAC:
                if (NULL != pSess->pMacauth)
                    macauth_timer_chk(pSess->pMacauth);
                break;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
            case AUTHMGR_TYPE_WEB:
                if (NULL != pSess->pWebauth)
                    webauth_timer_chk(pSess->pWebauth);
                break;
#endif

            default:
                break;
        }

        if (TRUE == pSess->delSess)
            isDelSess = TRUE;

        pSess = pSess->next;
    }

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    if (isDelSess)
        _authmgr_sess_del();
}

static void authmgr_timer_routine(unsigned long id)
{
    authmgr_sess_t *pSess = NULL;

    AUTHMGR_SEM_LOCK(authmgr_sem);

    pSess = g_sessList;

    while (pSess != NULL)
    {
        if (pSess->gotoNextType)
        {
            
            SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_FAILOVER,
                        text_authmgr_type[pSess->typeOrder.order[pSess->currTypeIdx]].text,
                        pSess->suppInfo.mac.octet[0],
                        pSess->suppInfo.mac.octet[1],
                        pSess->suppInfo.mac.octet[2],
                        pSess->suppInfo.mac.octet[3],
                        pSess->suppInfo.mac.octet[4],
                        pSess->suppInfo.mac.octet[5],
                        LPSTR(pSess->suppInfo.port),
                        pSess->sessId.cnt,
                        pSess->sessId.time);

            pSess->currTypeIdx = authmgr_action_nextTypeIdx_ret(pSess);

            pSess->gotoNextType = FALSE;

            AUTHMGR_DBG("sess:%08X%08X on port %s go to next auth %s typeIdx %d",
                        pSess->sessId.cnt,
                        pSess->sessId.time,
                        LPSTR(pSess->suppInfo.port),
                        text_authmgr_type[pSess->currTypeIdx].text,
                        pSess->currTypeIdx);
        }

        switch (pSess->typeOrder.order[pSess->currTypeIdx])
        {
#ifdef CONFIG_SYS_PROTO_DOT1X
            case AUTHMGR_TYPE_DOT1X:
                if ((NULL != pSess->pDot1x) && (FALSE == pSess->pDot1x->delPacp))
                {
                    dot1x_timer_routine(pSess->pDot1x);
                }
                else
                {
                    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_START,
                               text_authmgr_type[AUTHMGR_TYPE_DOT1X].text,
                               pSess->suppInfo.mac.octet[0],
                               pSess->suppInfo.mac.octet[1],
                               pSess->suppInfo.mac.octet[2],
                               pSess->suppInfo.mac.octet[3],
                               pSess->suppInfo.mac.octet[4],
                               pSess->suppInfo.mac.octet[5],
                               LPSTR(pSess->suppInfo.port),
                               pSess->sessId.cnt,
                               pSess->sessId.time);

                    if (NULL != (pSess->pDot1x = dot1x_pacp_create(pSess->suppInfo.port)))
                    {
                        pSess->pDot1x->pParentSess = pSess;

                        AUTHMGR_DBG("Create %s pacp for sess:%08X%08X on port %s success",
                                    text_authmgr_type[AUTHMGR_TYPE_DOT1X].text,
                                    pSess->sessId.cnt,
                                    pSess->sessId.time,
                                    LPSTR(pSess->suppInfo.port));

                        dot1x_timer_routine(pSess->pDot1x);
                    }
                    
                    else
                    {
                        pSess->gotoNextType = TRUE;

                        AUTHMGR_DBG("Create %s pacp for sess:%08X%08X on port %s fail",
                                    text_authmgr_type[AUTHMGR_TYPE_DOT1X].text,
                                    pSess->sessId.cnt,
                                    pSess->sessId.time,
                                    LPSTR(pSess->suppInfo.port));
                    }
                }
                break;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
            case AUTHMGR_TYPE_MAC:
                if ((NULL != pSess->pMacauth))
                {
                    macauth_timer_routine(pSess->pMacauth);
                }
                else
                {
                    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_START,
                               text_authmgr_type[AUTHMGR_TYPE_MAC].text,
                               pSess->suppInfo.mac.octet[0],
                               pSess->suppInfo.mac.octet[1],
                               pSess->suppInfo.mac.octet[2],
                               pSess->suppInfo.mac.octet[3],
                               pSess->suppInfo.mac.octet[4],
                               pSess->suppInfo.mac.octet[5],
                               LPSTR(pSess->suppInfo.port),
                               pSess->sessId.cnt,
                               pSess->sessId.time);

                    if (NULL != (pSess->pMacauth = macauth_create(pSess->suppInfo.port)))
                    {
                        pSess->pMacauth->pParentSess = pSess;

                        AUTHMGR_DBG("Create %s for sess:%08X%08X on port %s success",
                                    text_authmgr_type[AUTHMGR_TYPE_MAC].text,
                                    pSess->sessId.cnt,
                                    pSess->sessId.time,
                                    LPSTR(pSess->suppInfo.port));

                        macauth_timer_routine(pSess->pMacauth);
                    }
                    
                    else
                    {
                        pSess->gotoNextType = TRUE;

                        AUTHMGR_DBG("Create %s for sess:%08X%08X on port %s fail",
                                    text_authmgr_type[AUTHMGR_TYPE_MAC].text,
                                    pSess->sessId.cnt,
                                    pSess->sessId.time,
                                    LPSTR(pSess->suppInfo.port));
                    }
                }
                break;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
            case AUTHMGR_TYPE_WEB:
                if ((NULL != pSess->pWebauth))
                {
                    webauth_timer_routine(pSess->pWebauth);
                }
                else
                {
                    SYS_LOG(LOG_CAT_AUTHMGR, LOG_AUTHMGR_START,
                               text_authmgr_type[AUTHMGR_TYPE_WEB].text,
                               pSess->suppInfo.mac.octet[0],
                               pSess->suppInfo.mac.octet[1],
                               pSess->suppInfo.mac.octet[2],
                               pSess->suppInfo.mac.octet[3],
                               pSess->suppInfo.mac.octet[4],
                               pSess->suppInfo.mac.octet[5],
                               LPSTR(pSess->suppInfo.port),
                               pSess->sessId.cnt,
                               pSess->sessId.time);

                    if (NULL != (pSess->pWebauth = webauth_create(pSess->suppInfo.port)))
                    {
                        pSess->pWebauth->pParentSess = pSess;

                        AUTHMGR_DBG("Create %s for sess:%08X%08X on port %s success",
                                    text_authmgr_type[AUTHMGR_TYPE_WEB].text,
                                    pSess->sessId.cnt,
                                    pSess->sessId.time,
                                    LPSTR(pSess->suppInfo.port));

                        webauth_timer_routine(pSess->pWebauth);
                    }
                    
                    else
                    {
                        pSess->gotoNextType = TRUE;

                        AUTHMGR_DBG("Create %s for sess:%08X%08X on port %s fail",
                                    text_authmgr_type[AUTHMGR_TYPE_WEB].text,
                                    pSess->sessId.cnt,
                                    pSess->sessId.time,
                                    LPSTR(pSess->suppInfo.port));
                    }
                }
                break;
#endif

            default:
                break;
        }

        pSess = pSess->next;
    }

    AUTHMGR_SEM_UNLOCK(authmgr_sem);
}
static int32 _authmgr_linkEvent(uint32 event, char *pData)
{
    sys_port_event_t *pPortEvent = NULL;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pPortEvent = (sys_port_event_t *)pData;

    if (PORT_LINKDOWN == pPortEvent->new_linkStatus)
    {
        _authmgr_portSess_clear(pPortEvent->lport);
    }

    return SYS_ERR_OK;
}

static int32 _authmgr_trunkEvent(uint32 event, char *pData)
{
    sys_trunk_event_t *pEvent = NULL;
    sys_logic_port_t lp;

    SYS_PARAM_CHK((NULL == pData), SYS_ERR_NULL_POINTER);

    pEvent = (sys_trunk_event_t *) pData;

    switch (event)
    {
        case SYS_NT_TRK_MBR_JOIN:
        case SYS_NT_TRK_MBR_LEAVE:

            FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, pEvent->portmask)
            {
                _authmgr_portState_set(lp);
            }

            break;

        default:
            break;
    }

    return SYS_ERR_OK;
}
void authmgr_dbgStr_get(authmgr_type_t type, void *pInfo, char *str, char* fmt,...)
{
    static char tmp[SYS_LOG_BUF_SIZE];
    static va_list args;
    authmgr_sess_t *pSess = NULL;

    va_start(args, fmt);
    vsprintf(tmp, fmt, args);
    va_end(args);

#ifdef CONFIG_SYS_PROTO_DOT1X
    if (AUTHMGR_TYPE_DOT1X == type)
    {
        dot1x_pacp_t *pPacp = (dot1x_pacp_t *)pInfo;
        pSess = pPacp->pParentSess;
        osal_sprintf(str, "{port:%u sess:%08X%08X} %s",
                          pSess->suppInfo.port,
                          pSess->sessId.cnt,
                          pSess->sessId.time,
                          tmp);
    }
    else
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    if (AUTHMGR_TYPE_MAC == type)
    {
        macauth_t *pMacauth = (macauth_t *)pInfo;
        pSess = pMacauth->pParentSess;
        osal_sprintf(str, "{port:%u sess:%08X%08X} %s",
                          pSess->suppInfo.port,
                          pSess->sessId.cnt,
                          pSess->sessId.time,
                          tmp);
    }
    else
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    if (AUTHMGR_TYPE_WEB == type)
    {
        webauth_t *pWebauth = (webauth_t *)pInfo;
        pSess = pWebauth->pParentSess;
        osal_sprintf(str, "{port:%u sess:%08X%08X} %s",
                          pSess->suppInfo.port,
                          pSess->sessId.cnt,
                          pSess->sessId.time,
                          tmp);
    }
    else
#endif
    if (AUTHMGR_TYPE_END == type)
    {
        pSess = (authmgr_sess_t *)pInfo;
        osal_sprintf(str, "{port:%u sess:%08X%08X} %s",
                          pSess->suppInfo.port,
                          pSess->sessId.cnt,
                          pSess->sessId.time,
                          tmp);
    }

    return;
}
int32 authmgr_init(void)
{
    SYS_MEM_CLEAR(g_sysMac);
    SYS_MEM_CLEAR(g_emptyMac);
    SYS_MEM_CLEAR(g_portAuthLpm);
    SYS_MEM_CLEAR(g_authCfg);
    SYS_MEM_CLEAR(g_portSessFirst);
    SYS_MEM_CLEAR(g_portSessCnt);

    rsd_sys_macAddr_get(SYS_DEV_ID_DEFAULT, &g_sysMac);

    g_maxSessNum = CAPA_AUTHMGR_SESSION_NUM;

    SYS_ERR_CHK(rsd_auth_init());

#ifdef CONFIG_SYS_PROTO_DOT1X
    SYS_ERR_CHK(rsd_nic_handler_register(&l2gEapolHandler));
#endif
    SYS_ERR_CHK(rsd_l2_limit_handler_register(SYS_L2_LIMIT_MOD_AUTHMGR, &l2gAuthmgrHandler));

    authmgr_sem = osal_sem_mutex_create();

    if (0 == authmgr_sem)
    {
        AUTHMGR_DBG("semaphore create failed");
        return SYS_ERR_FAILED;
    }

    SYS_ERR_CHK(sys_notifier_observer_register("authmgr", SYS_NOTIFIER_SUBJECT_PORT, SYS_NT_PORT_LINK_CHG, _authmgr_linkEvent, SYS_NOTIFIER_PRI_LOW));

    SYS_ERR_CHK(sys_notifier_observer_register("authmgr",
                                               SYS_NOTIFIER_SUBJECT_TRUNK,
                                               (SYS_NT_TRK_MBR_JOIN | SYS_NT_TRK_MBR_LEAVE),
                                               _authmgr_trunkEvent,
                                               SYS_NOTIFIER_PRI_MEDIUM));

#ifdef CONFIG_SYS_PROTO_DOT1X
    SYS_ERR_CHK(dot1x_init());
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    SYS_ERR_CHK(webauth_init());
#endif

    return SYS_ERR_OK;
}
int32 authmgr_exit(void)
{
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    webauth_exit();
#endif

    ROUTINE_STOP(count_routine);
    ROUTINE_STOP(sess_routine);

    _authmgr_allSess_clear();

    osal_sem_mutex_destroy(authmgr_sem);

#ifdef CONFIG_SYS_PROTO_DOT1X
    rsd_nic_handler_unregister(&l2gEapolHandler);
#endif
    rsd_nic_handler_unregister(&l2gAuthmgrHandler);

    sys_notifier_observer_unregister("authmgr", SYS_NOTIFIER_SUBJECT_PORT, _authmgr_linkEvent);

    return SYS_ERR_OK;
}
static int32 _authmgr_portState_set(sys_logic_port_t port)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);

    if ((AUTHMGR_PORTCTRL_DISABLE == g_authCfg.portCfg[port].ctrlType)
        || IS_LP_TRKMBR(port)
        || (AUTHMGR_PORTCTRL_FORCE_AUTHORIZED == g_authCfg.portCfg[port].ctrlType))
    {
        SYS_ERR_CHK(rsd_auth_portBasedState_set(port, DISABLED));
        SYS_ERR_CHK(rsd_auth_macBasedState_set(port, DISABLED));
        SYS_ERR_CHK(rsd_auth_unauthPktAction_set(port, SYS_AUTH_ACTION_FORWARD));

        LOGIC_PORTMASK_CLEAR_PORT(g_portAuthLpm, port);

        rsd_l2_limit_portCtrl_set(SYS_L2_LIMIT_MOD_AUTHMGR, port, DISABLED);
    }
    else if (AUTHMGR_PORTCTRL_FORCE_UNAUTHORIZED == g_authCfg.portCfg[port].ctrlType)
    {
        rsd_l2_limit_portCtrl_set(SYS_L2_LIMIT_MOD_AUTHMGR, port, ENABLED);

        SYS_ERR_CHK(rsd_auth_macBasedState_set(port, DISABLED));
        SYS_ERR_CHK(rsd_auth_portBasedState_set(port, ENABLED));
        SYS_ERR_CHK(rsd_auth_unauthPktAction_set(port, SYS_AUTH_ACTION_DROP));

        LOGIC_PORTMASK_SET_PORT(g_portAuthLpm, port);
    }
    else if (AUTHMGR_PORTCTRL_AUTO == g_authCfg.portCfg[port].ctrlType)
    {
        uint32 i = 0;
        uint32 isAllDisable = TRUE;
        rsd_l2_limit_portCtrl_set(SYS_L2_LIMIT_MOD_AUTHMGR, port, ENABLED);

        for (i = 0; i < AUTHMGR_TYPE_END; i++)
        {
            if ((ENABLED == g_authCfg.typeEnbl[i]) || (ENABLED == g_authCfg.portCfg[port].typeEnbl[i]))
            {
                isAllDisable = FALSE;
                break;
            }
        }

        if (isAllDisable)
        {
            LOGIC_PORTMASK_CLEAR_PORT(g_portAuthLpm, port);

            SYS_ERR_CHK(rsd_auth_portBasedState_set(port, DISABLED));
            SYS_ERR_CHK(rsd_auth_macBasedState_set(port, DISABLED));
            SYS_ERR_CHK(rsd_auth_unauthPktAction_set(port, SYS_AUTH_ACTION_FORWARD));
        }
        else
        {
            uint32 isPortAuth = FALSE;

            for (i = 0; i < AUTHMGR_TYPE_END; i++)
            {
                if (AUTHMGR_TYPE_NONE == g_authCfg.portCfg[port].typeOrder.order[i])
                    break;

                if (ENABLED == g_authCfg.portCfg[port].typeEnbl[g_authCfg.portCfg[port].typeOrder.order[i]])
                {
                    isPortAuth = TRUE;
                    break;
                }
            }

            if (isPortAuth)
            {
                LOGIC_PORTMASK_SET_PORT(g_portAuthLpm, port);

                switch (g_authCfg.portCfg[port].mode)
                {
                    case AUTHMGR_MODE_MULTI_AUTH:
                    case AUTHMGR_MODE_SINGLE_HOST:
                        SYS_ERR_CHK(rsd_auth_portBasedState_set(port, DISABLED));
                        SYS_ERR_CHK(rsd_auth_macBasedState_set(port, ENABLED));
                        break;

                    case AUTHMGR_MODE_MULTI_HOST:
                        SYS_ERR_CHK(rsd_auth_macBasedState_set(port, DISABLED));
                        SYS_ERR_CHK(rsd_auth_portBasedState_set(port, ENABLED));
                        break;

                    default:
                        break;
                }

                SYS_ERR_CHK(rsd_auth_unauthPktAction_set(port, SYS_AUTH_ACTION_TRAP2CPU));
            }
            else
            {
                LOGIC_PORTMASK_CLEAR_PORT(g_portAuthLpm, port);

                SYS_ERR_CHK(rsd_auth_portBasedState_set(port, DISABLED));
                SYS_ERR_CHK(rsd_auth_macBasedState_set(port, DISABLED));
                SYS_ERR_CHK(rsd_auth_unauthPktAction_set(port, SYS_AUTH_ACTION_FORWARD));
            }
        }
    }

    SYS_ERR_CHK(_authmgr_portSess_clear(port));

    SYS_ERR_CHK(rsd_l2_mac_clearByPort(port));

    if (IS_LOGIC_PORTMASK_CLEAR(g_portAuthLpm))
    {
        ROUTINE_STOP(count_routine);
        ROUTINE_STOP(sess_routine);

        authmgr_timer_count(0);
    }
    else
    {
        
        ROUTINE_START(sess_routine, "AUTHMGR Sess", authmgr_timer_routine, SYS_THREAD_PRI_LOW, 32768);
        ROUTINE_START(count_routine, "AUTHMGR Count", authmgr_timer_count, SYS_THREAD_PRI_DFLT, 32768);
    }

    return SYS_ERR_OK;
}
int32 authmgr_typeEnbl_set(authmgr_type_t type, sys_enable_t enable)
{
    sys_logic_port_t lp;

    SYS_PARAM_CHK((IS_ENABLE_INVALID(enable)), SYS_ERR_INPUT);

    g_authCfg.typeEnbl[type] = enable;

#ifdef CONFIG_SYS_PROTO_DOT1X
    if (AUTHMGR_TYPE_DOT1X == type)
    {
        rsd_auth_eapolEnable_set(enable);
    }
#endif

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        _authmgr_portState_set(lp);
    }

    return SYS_ERR_OK;
}
int32 authmgr_guestVlan_set(sys_vid_t vid)
{
    sys_logic_port_t lp;

    g_authCfg.guestVlan = vid;

    SYS_ERR_CHK(rsd_vlan_guestVid_set(vid));

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        if (ENABLED == g_authCfg.portCfg[lp].guestVlanEnbl)
            _authmgr_portState_set(lp);
    }

    return SYS_ERR_OK;
}
int32 authmgr_portCfg_set(sys_logic_port_t port, authmgr_cfg_port_t *pPortCfg)
{
    authmgr_cfg_port_t oldPortCfg;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pPortCfg), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(oldPortCfg);
    osal_memcpy(&oldPortCfg, &g_authCfg.portCfg[port], sizeof(authmgr_cfg_port_t));

    osal_memcpy(&g_authCfg.portCfg[port], pPortCfg, sizeof(authmgr_cfg_port_t));

    if ((g_authCfg.portCfg[port].ctrlType != oldPortCfg.ctrlType)
        || (g_authCfg.portCfg[port].mode != oldPortCfg.mode)
        || (g_authCfg.portCfg[port].guestVlanEnbl != oldPortCfg.guestVlanEnbl)
        || (0 != osal_memcmp(g_authCfg.portCfg[port].typeEnbl, oldPortCfg.typeEnbl, (sizeof(uint8)*(AUTHMGR_TYPE_END))))
        || (0 != osal_memcmp(&g_authCfg.portCfg[port].typeOrder, &oldPortCfg.typeOrder, sizeof(authmgr_order_t)))
        || (g_authCfg.portCfg[port].maxHosts < oldPortCfg.maxHosts))
    {
        _authmgr_portState_set(port);

#ifdef CONFIG_SYS_PROTO_DOT1X
        if (g_authCfg.portCfg[port].typeEnbl[AUTHMGR_TYPE_DOT1X] != oldPortCfg.typeEnbl[AUTHMGR_TYPE_DOT1X])
        {
            rsd_auth_portEapolEnable_set(port, g_authCfg.portCfg[port].typeEnbl[AUTHMGR_TYPE_DOT1X]);
        }
#endif
    }

    return SYS_ERR_OK;
}
int32 authmgr_sessCnt_get(uint32 *pCnt)
{
    SYS_PARAM_CHK((NULL == pCnt), SYS_ERR_NULL_POINTER);

    *pCnt = g_sessCnt;

    return SYS_ERR_OK;
}
int32 authmgr_sessInfo_getByIdx(uint32 idx, authmgr_sess_info_t *pSessInfo)
{
    authmgr_sess_t *pSess = NULL;
    uint32 i = 0;

    SYS_PARAM_CHK((NULL == pSessInfo), SYS_ERR_NULL_POINTER);

    AUTHMGR_SEM_LOCK(authmgr_sem);

    pSess = g_sessList;

    while (pSess != NULL)
    {
        if (i == idx)
        {
            osal_memcpy(&pSessInfo->sessId, &pSess->sessId, sizeof(authmgr_sessId_t));
            osal_memcpy(&pSessInfo->status, &pSess->status, sizeof(authmgr_sess_status_t));
            osal_memcpy(&pSessInfo->suppInfo, &pSess->suppInfo, sizeof(authmgr_suppInfo_t));
            pSessInfo->currType = pSess->typeOrder.order[pSess->currTypeIdx];

            break;
        }

        i++;

        pSess = pSess->next;
    }

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    return SYS_ERR_OK;
}
int32 authmgr_sess_clear(authmgr_sess_clear_t *pSessClear)
{
    authmgr_sess_t *pSess = NULL;

    SYS_PARAM_CHK((NULL == pSessClear), SYS_ERR_NULL_POINTER);

    AUTHMGR_SEM_LOCK(authmgr_sem);

    pSess = g_sessList;

    while (pSess != NULL)
    {
        if (AUTHMGR_SESS_CLEAR_ALL == pSessClear->clearType)
        {
            pSess->delSess = TRUE;
        }
        else if (AUTHMGR_SESS_CLEAR_INTF == pSessClear->clearType)
        {
            if (IS_LOGIC_PORTMASK_PORTSET(pSessClear->data.lpm, pSess->suppInfo.port))
                pSess->delSess = TRUE;
        }
        else if (AUTHMGR_SESS_CLEAR_MAC == pSessClear->clearType)
        {
            if (0 == osal_memcmp(&pSessClear->data.mac, &pSess->suppInfo.mac, sizeof(sys_mac_t)))
            {
                pSess->delSess = TRUE;
                break;
            }
        }
        else if (AUTHMGR_SESS_CLEAR_ID == pSessClear->clearType)
        {
            if (0 == osal_memcmp(&pSessClear->data.sessId, &pSess->sessId, sizeof(authmgr_sessId_t)))
            {
                pSess->delSess = TRUE;
                break;
            }
        }
        else if (AUTHMGR_SESS_CLEAR_TYPE == pSessClear->clearType)
        {
            if ((pSessClear->data.type == pSess->typeOrder.order[pSess->currTypeIdx]))
                pSess->delSess = TRUE;
        }

        pSess = pSess->next;
    }

    AUTHMGR_SEM_UNLOCK(authmgr_sem);

    return SYS_ERR_OK;
}

