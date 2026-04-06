#include <common/sys_pktfmt.h>
#include <authmgr/proto_authmgr.h>
extern sys_mac_t g_sysMac;
extern sys_mac_t g_dot1xEapolMac;
uint32 eapol_isEapPkt_ret(sys_nic_pkt_t *pPkt)
{
    ethhdr_t *ethhdr = NULL;
    sys_mac_t *pDstMac = NULL;

    ethhdr = (ethhdr_t*)(pPkt->data);
    pDstMac = (sys_mac_t *)(pPkt->data);

    if ((0 == osal_memcmp(pDstMac, &g_dot1xEapolMac, sizeof(sys_mac_t)))
        && (ethhdr->h_proto == EAPOL_FRAME))
        return TRUE;

    return FALSE;
}

static int32 _eapol_ver_ret(dot1x_pacp_t *pPacp)
{
    return 2;
}

int32 eapol_pkt_passthrough(dot1x_pacp_t *pPacp)
{
    sys_nic_pkt_t        *pPkt     = NULL;
    uint32               pktLen    = sizeof(ethhdr_t) + sizeof(eapol_header_t);
    uint32               eapLen    = pPacp->pae_sm.fromSvr.length;
    uint8                *eapData  = pPacp->pae_sm.fromSvr.pkt;
    ethhdr_t             *ethhdr   = NULL;
    eapol_header_t       *eapolhdr = NULL;
    eap_header_t         *eaphdr = (eap_header_t*) eapData;
    sys_logic_portmask_t lpm;
    sys_mac_t            portMac;
    authmgr_sess_t       *pSess = NULL;

    LOGIC_PORTMASK_CLEAR_ALL(lpm);
    LOGIC_PORTMASK_SET_PORT(lpm, pPacp->port);

    pktLen += eapLen;

    pPacp->currentId = eaphdr->eap_identifier;

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(pktLen, &pPkt))
    {
        ethhdr = (ethhdr_t*)(pPkt->data);
        eapolhdr = (eapol_header_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));

        pSess = (authmgr_sess_t *)pPacp->pParentSess;
        if (AUTHMGR_MODE_MULTI_HOST == pSess->pPortCfg->mode
            && 0 == osal_memcmp(ethhdr->h_dest, &pSess->suppInfo.mac, sizeof(sys_mac_t)))
            osal_memcpy(ethhdr->h_dest, &g_dot1xEapolMac, sizeof(sys_mac_t));
        else
            osal_memcpy(ethhdr->h_dest, &pSess->suppInfo.mac, sizeof(sys_mac_t));

        SYS_MEM_CLEAR(portMac);
        rsd_sys_portMacAddr_get(pPacp->port, &portMac);
        osal_memcpy(ethhdr->h_source, &portMac, sizeof(sys_mac_t));

        ethhdr->h_proto = EAPOL_FRAME;

        eapolhdr->eapol_version = _eapol_ver_ret(pPacp);
        eapolhdr->eapol_type = EAPOL_EAP_PACKET;
        eapolhdr->eapol_body_length = eapLen;
        osal_memcpy((((uint8*)eapolhdr) + sizeof(eapol_header_t)), eapData, eapLen);

        pPkt->tail += pktLen;
        pPkt->length = pktLen;

        if (SYS_ERR_OK != rsd_nic_pkt_tx(pPkt, &lpm, SYS_CPU_PORT_PRIORITY_DOT1X, 0, RSD_NIC_TX_CARE_NONE))
        {
            AUTHMGR_DBG("pkt Tx falied!");
            return SYS_ERR_FAILED;
        }

        DOT1X_DBG(pPacp, "TX EAPOL code %d id %d len %u",
                         eaphdr->eap_code,
                         eaphdr->eap_identifier,
                         eaphdr->eap_length);
    }

    return SYS_ERR_OK;
}

int32 eapol_pkt_tx(dot1x_pacp_t *pPacp, eap_code_t code)
{
    sys_nic_pkt_t        *pPkt = NULL;
    uint32               pktLen = sizeof(ethhdr_t) + sizeof(eapol_header_t);
    uint32               eapLen = sizeof(eap_header_t);
    ethhdr_t             *ethhdr = NULL;
    eapol_header_t       *eapolhdr = NULL;
    eap_header_t         *eaphdr = NULL;
    sys_logic_portmask_t lpm;
    sys_mac_t            portMac;
    authmgr_sess_t       *pSess = NULL;

    LOGIC_PORTMASK_CLEAR_ALL(lpm);
    LOGIC_PORTMASK_SET_PORT(lpm, pPacp->port);

    if (EAP_SUCCESS == code || EAP_FAILURE == code)
        eapLen -= 1;

    pktLen += eapLen;

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(pktLen, &pPkt))
    {
        ethhdr = (ethhdr_t*)(pPkt->data);
        eapolhdr = (eapol_header_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));
        eaphdr = (eap_header_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t) + sizeof(eapol_header_t));

        pSess = (authmgr_sess_t *)pPacp->pParentSess;
        if (AUTHMGR_MODE_MULTI_HOST == pSess->pPortCfg->mode
            && 0 == osal_memcmp(ethhdr->h_dest, &pSess->suppInfo.mac, sizeof(sys_mac_t)))
            osal_memcpy(ethhdr->h_dest, &g_dot1xEapolMac, sizeof(sys_mac_t));
        else
            osal_memcpy(ethhdr->h_dest, &pSess->suppInfo.mac, sizeof(sys_mac_t));

        SYS_MEM_CLEAR(portMac);
        rsd_sys_portMacAddr_get(pPacp->port, &portMac);
        osal_memcpy(ethhdr->h_source, &portMac, sizeof(sys_mac_t));

        ethhdr->h_proto = EAPOL_FRAME;

        eapolhdr->eapol_version = _eapol_ver_ret(pPacp);
        eapolhdr->eapol_type = EAPOL_EAP_PACKET;
        eapolhdr->eapol_body_length = eapLen;

        eaphdr->eap_code = code;
        eaphdr->eap_identifier = pPacp->currentId;

        if (EAP_REQUEST == code)
        {
            pPacp->backend_sm.eapReq = TRUE;
        }

        eaphdr->eap_length = eapLen;

        if (!(EAP_SUCCESS == code || EAP_FAILURE == code))
            eaphdr->eap_type = EAP_IDENTITY;

        pPkt->tail += pktLen;
        pPkt->length = pktLen;

        if (SYS_ERR_OK != rsd_nic_pkt_tx(pPkt, &lpm, SYS_CPU_PORT_PRIORITY_DOT1X, 0, RSD_NIC_TX_CARE_NONE))
        {
            DOT1X_DBG(pPacp, "eapol pkt Tx falied");
            return SYS_ERR_FAILED;
        }

        DOT1X_DBG(pPacp, "TX EAPOL code %d id %d len %u", code, eaphdr->eap_identifier, eaphdr->eap_length);
    }

    return SYS_ERR_OK;
}

int32 eapol_pkt_rx(dot1x_pacp_t *pPacp, sys_nic_pkt_t *pPkt)
{
    dot1x_pae_sm_t  *pPaeSm = NULL;
    ethhdr_t        *ethhdr = NULL;
    eapol_header_t  *eapolhdr = NULL;
    eap_header_t    *eaphdr = NULL;

    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pPkt), SYS_ERR_NULL_POINTER);

    pPaeSm = &pPacp->pae_sm;

    ethhdr = (ethhdr_t*)(pPkt->data);
    eapolhdr = (eapol_header_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));
    eaphdr = (eap_header_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t) + sizeof(eapol_header_t));

    if ((pPkt->length - sizeof(ethhdr_t)) < (EAPOL_HDRLEN))
    {
        return SYS_ERR_FAILED;
    }

    switch(eapolhdr->eapol_type)
    {
        case EAPOL_LOGOFF:
            pPaeSm->eapolLogoff = TRUE;
            SYS_MEM_CLEAR(pPacp->userName);
            SYS_MEM_CLEAR(pPacp->backend_sm.stateAvp);
            DOT1X_DBG(pPacp, "RX EAPOL Logoff");
            break;

        case EAPOL_START:
            pPaeSm->eapolStart = TRUE;
            SYS_MEM_CLEAR(pPacp->userName);
            SYS_MEM_CLEAR(pPacp->backend_sm.stateAvp);
            DOT1X_DBG(pPacp, "RX EAPOL Start");
            break;

        case EAPOL_EAP_PACKET:
            DOT1X_DBG(pPacp, "RX EAPOL EAP Packet type %d code %d id %d len %d currentId %d",
                             eaphdr->eap_type,
                             eaphdr->eap_code,
                             eaphdr->eap_identifier,
                             eaphdr->eap_length,
                             pPacp->currentId);

            if (pPacp->currentId != eaphdr->eap_identifier)
            {
                DOT1X_DBG(pPacp, "RX EAPOL packet is dropped cause id %d mismatch currentId %d",
                                 eaphdr->eap_identifier,
                                 pPacp->currentId);

                return SYS_ERR_FAILED;
            }

            if (eaphdr->eap_code == EAP_RESPONSE)
            {
                uint16 eapLen = ntohs(eapolhdr->eapol_body_length);

                if (eapLen > ETHER_MAXSIZE)
                {
                    DOT1X_DBG(pPacp, "RX EAPOL packet is dropped (eapLen:%d > %d)!!!!!", eapLen, ETHER_MAXSIZE);

                    return SYS_ERR_FAILED;
                }

                if (EAP_IDENTITY == eaphdr->eap_type)
                {
                    uint32 userNameLen = eapLen - sizeof(eap_header_t);
                    char   *eapdata = (char*)(((uint8*) ethhdr) + sizeof(ethhdr_t) + sizeof(eapol_header_t) + sizeof(eap_header_t));

                    if (userNameLen >= CAPA_AUTHMGR_DOT1X_USERNAME_LEN)
                    {
                        return SYS_ERR_FAILED;
                    }
                    SYS_MEM_CLEAR(pPacp->userName);
                    osal_strncpy(pPacp->userName, eapdata, userNameLen);
                }

                pPaeSm->fromSupp.pkt = osal_alloc(eapLen);
                if (NULL == pPaeSm->fromSupp.pkt)
                {
                    DOT1X_DBG(pPacp, "memory allocate failed");
                    return SYS_ERR_FAILED;
                }
                osal_memcpy(pPaeSm->fromSupp.pkt, eaphdr, eapLen);
                pPaeSm->fromSupp.length = eapLen;

                if (eaphdr->eap_type == EAP_IDENTITY
                    || eaphdr->eap_type == EAP_MD5_CHALLENGE
                    || eaphdr->eap_type == EAP_TLS
                    || eaphdr->eap_type == EAP_PEAP
                    || eaphdr->eap_type == EAP_TTLS
                    || eaphdr->eap_type == EAP_NAK)
                {
                    pPacp->eapolEap = TRUE;

                    INC_EAPID(pPacp->currentId);
                }
                else
                {
                    break;
                }
            }
            break;

        default:
            break;
    }

    dot1x_sm_check_trigger(pPacp);

    return SYS_ERR_OK;
}

int32 eapol_radiusRx_process(dot1x_pacp_t *pPacp, dot1x_rad_recv_t *pRecv)
{
    dot1x_pae_sm_t     *pPaeSm = NULL;
    dot1x_backend_sm_t *pBackendSm = NULL;
    eap_header_t       *eaphdr = NULL;
    int32              ret = SYS_ERR_OK;

    SYS_PARAM_CHK((NULL == pPacp), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pRecv), SYS_ERR_NULL_POINTER);

    pPaeSm = &pPacp->pae_sm;
    pBackendSm = &pPacp->backend_sm;
    eaphdr = (eap_header_t*) ((uint8 *) pRecv->eapData);

    pBackendSm->idFromServer = eaphdr->eap_identifier;

    if (NULL != pPacp->pae_sm.fromSvr.pkt)
    {
        osal_free(pPacp->pae_sm.fromSvr.pkt);
        pPacp->pae_sm.fromSvr.pkt  = NULL;
        pPacp->pae_sm.fromSvr.length = 0;
    }

    switch (eaphdr->eap_code)
    {
        case EAP_REQUEST:
            DOT1X_DBG(pPacp, "RX EAP Request id %d", eaphdr->eap_identifier);
            if (NULL == (pPaeSm->fromSvr.pkt = osal_alloc(pRecv->eapLen)))
            {
                DOT1X_DBG(pPacp, "memory allocate failed");
                return SYS_ERR_FAILED;
            }
            osal_memcpy(pPaeSm->fromSvr.pkt, pRecv->eapData, pRecv->eapLen);
            pPaeSm->fromSvr.length = pRecv->eapLen;
            pBackendSm->eapReq = TRUE;
            pPaeSm->eapReq = FALSE;
            break;

        case EAP_FAILURE:
            DOT1X_DBG(pPacp, "RX EAP Failure id %d", eaphdr->eap_identifier);
            pPacp->eapFail = TRUE;
            pPacp->portStatus = DOT1X_STATE_UNAUTHORIZED;
            if (NULL == (pPaeSm->fromSvr.pkt = osal_alloc(pRecv->eapLen)))
            {
                DOT1X_DBG(pPacp, "memory allocate failed");
                return SYS_ERR_FAILED;
            }
            osal_memcpy(pPaeSm->fromSvr.pkt, pRecv->eapData, pRecv->eapLen);
            pPaeSm->fromSvr.length = pRecv->eapLen;
            break;

        case EAP_SUCCESS:
            DOT1X_DBG(pPacp, "RX EAP Success id %d", eaphdr->eap_identifier);

            ret = authmgr_action_authorInfo_set((authmgr_sess_t *)pPacp->pParentSess, &pRecv->author, &pRecv->authorSrc);

            if (SYS_ERR_OK == ret)
            {
                pPacp->eapSuccess = TRUE;
                pPacp->portStatus = DOT1X_STATE_AUTHORIZED;
                if (NULL == (pPaeSm->fromSvr.pkt = osal_alloc(pRecv->eapLen)))
                {
                    DOT1X_DBG(pPacp, "memory allocate failed");
                    return SYS_ERR_FAILED;
                }
                osal_memcpy(pPaeSm->fromSvr.pkt, pRecv->eapData, pRecv->eapLen);
                pPaeSm->fromSvr.length = pRecv->eapLen;

                if (DOT1X_STATE_UNAUTHORIZED == pPacp->authPortStatus)
                {
                    pPacp->sessionTime = 0;
                }
            }
            else
            {
                pPacp->eapFail = TRUE;
                pPacp->portStatus = DOT1X_STATE_UNAUTHORIZED;
                if (NULL == (pPaeSm->fromSvr.pkt = osal_alloc(pRecv->eapLen)))
                {
                    DOT1X_DBG(pPacp, "memory allocate failed");
                    return SYS_ERR_FAILED;
                }
                osal_memcpy(pPaeSm->fromSvr.pkt, pRecv->eapData, pRecv->eapLen);
                pPaeSm->fromSvr.length = pRecv->eapLen;
            }
            break;

        default:
            break;
    }

    dot1x_sm_check_trigger(pPacp);

    return SYS_ERR_OK;
}

int32 eapol_pkt_unauthMcast_tx(sys_logic_port_t port, eap_code_t code, uint32 eapid)
{
    sys_nic_pkt_t        *pPkt = NULL;
    uint32               pktLen = sizeof(ethhdr_t) + sizeof(eapol_header_t);
    uint32               eapLen = sizeof(eap_header_t);
    ethhdr_t             *ethhdr = NULL;
    eapol_header_t       *eapolhdr = NULL;
    eap_header_t         *eaphdr = NULL;
    sys_logic_portmask_t lpm;
    sys_mac_t            portMac;

    LOGIC_PORTMASK_CLEAR_ALL(lpm);
    LOGIC_PORTMASK_SET_PORT(lpm, port);

    if (EAP_SUCCESS == code || EAP_FAILURE == code)
        eapLen -= 1;

    pktLen += eapLen;

    if (SYS_ERR_OK == rsd_nic_pkt_alloc(pktLen, &pPkt))
    {
        ethhdr = (ethhdr_t*)(pPkt->data);
        eapolhdr = (eapol_header_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t));
        eaphdr = (eap_header_t*)(((uint8*) ethhdr) + sizeof(ethhdr_t) + sizeof(eapol_header_t));

        osal_memcpy(ethhdr->h_dest, &g_dot1xEapolMac, sizeof(sys_mac_t));

        SYS_MEM_CLEAR(portMac);
        rsd_sys_portMacAddr_get(port, &portMac);
        osal_memcpy(ethhdr->h_source, &portMac, sizeof(sys_mac_t));

        ethhdr->h_proto = EAPOL_FRAME;

        eapolhdr->eapol_version = _eapol_ver_ret(NULL);
        eapolhdr->eapol_type = EAPOL_EAP_PACKET;
        eapolhdr->eapol_body_length = eapLen;

        eaphdr->eap_code = code;
        eaphdr->eap_identifier = eapid;
        eaphdr->eap_length = eapLen;

        if (!(EAP_SUCCESS == code || EAP_FAILURE == code))
            eaphdr->eap_type = EAP_IDENTITY;

        pPkt->tail += pktLen;
        pPkt->length = pktLen;

        if (SYS_ERR_OK != rsd_nic_pkt_tx(pPkt, &lpm, SYS_CPU_PORT_PRIORITY_DOT1X, 0, RSD_NIC_TX_CARE_NONE))
            return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}
