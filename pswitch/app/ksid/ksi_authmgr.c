/*
 * Copyright (C) 2010 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 14054 $
 * $Date: 2010-11-09 19:03:46 +0800 (Tue, 09 Nov 2010) $
 *
 * Purpose : Export the public APIs in lower layer module
 *
 * Feature : Export the public APIs in lower layer module
 *
 */

/*
 * Include Files
 */
#include <stdlib.h>
#include <ksi_authmgr.h>
#include <libsal/sal_sys.h>
#include <libsal/sal_util.h>
#include <libsal/sal_authmgr.h>

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

/* Function Name:
 *      ksi_authmgr_cmd_mux
 * Description:
 *      Dispatch command in ksi message for authentication manger module
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 * Note:
 *      None
 */
static int32 ksi_authmgr_cmd_mux(unsigned int cmd, ksi_msg_t *pMsg)
{
    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    switch (cmd)
    {
#ifdef CONFIG_SYS_PROTO_DOT1X
        case KSI_AUTHMGR_DOT1X_EAP_RADIUS_TX:
            SYS_ERR_CHK(ksi_authmgr_dot1xEapRadius_tx(pMsg));
            break;
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
        case KSI_AUTHMGR_MACAUTH_AUTHING:
            SYS_ERR_CHK(ksi_authmgr_macauth_authing(pMsg));
            break;
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
        case KSI_AUTHMGR_WEBAUTH_ARP_ADD:
            SYS_ERR_CHK(ksi_authmgr_webauthArp_add(pMsg));
            break;

        case KSI_AUTHMGR_WEBAUTH_ARP_DEL:
            SYS_ERR_CHK(ksi_authmgr_webauthArp_del(pMsg));
            break;
#endif

        default:
            return SYS_ERR_KSI_CMD_NOT_FOUND;
    }

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_authmgr_init
 * Description:
 *      init function of ksi authentication manger module
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 * Note:
 *      None
 */
int32 ksi_authmgr_init(void)
{
    SYS_ERR_CHK(ksi_handler_register(KSI_MOD_AUTHMGR, ksi_authmgr_cmd_mux));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_DOT1X
/* Function Name:
 *      ksi_authmgr_dot1xEapRadius_tx
 * Description:
 *      Passthrough EAP packet to Radius server
 * Input:
 *      pMsg - pointer of kernel to user message
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 *      SYS_ERR_FILE_OPEN
 * Note:
 *      None
 */
int32 ksi_authmgr_dot1xEapRadius_tx(ksi_msg_t *pMsg)
{
    dot1x_rad_xmit_t xmit;
    uint8 *pXmitInfo;
    dot1x_rad_recv_t radRecv;

    SYS_MEM_CLEAR(xmit);
    SYS_MEM_CLEAR(radRecv);

    pXmitInfo = (uint8*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    osal_memcpy(&xmit.info, pXmitInfo, sizeof(dot1x_rad_xmitInfo_t));
    xmit.pEapPkt = pXmitInfo + sizeof(dot1x_rad_xmitInfo_t);

    sal_authmgr_dot1xRadius_tx(&xmit, &radRecv);

    //send radRecv to kernel
    if (NULL != radRecv.eapData)
    {
        osal_memcpy(&radRecv.sessId, &xmit.info.sessId, sizeof(authmgr_sessId_t));

        sal_authmgr_dot1xRadius_rx(radRecv);

        /* Send done. Free memory */
        osal_free(radRecv.eapData);
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_MACAUTH
/* Function Name:
 *      ksi_authmgr_macauth_authing
 * Description:
 *      Do mac authentication authing action
 * Input:
 *      pMsg - pointer of kernel to user message
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 *      SYS_ERR_FILE_OPEN
 * Note:
 *      None
 */
int32 ksi_authmgr_macauth_authing(ksi_msg_t *pMsg)
{
    int32 ret = SYS_ERR_FAILED;
    authmgr_methodOrder_t methodOrder;
    macauth_authInfo_t *pAuthInfo = NULL;
    macauth_authInfo_t authInfo;
    uint32 i = 0;

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pAuthInfo = (macauth_authInfo_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    SYS_MEM_CLEAR(authInfo);
    SYS_MEM_CLEAR(methodOrder);

    osal_memcpy(&authInfo, pAuthInfo, sizeof(macauth_authInfo_t));
    authInfo.authSts = SYS_UNAUTH;

    SYS_ERR_CHK(sal_authmgr_portMethodOrder_get(authInfo.port, &methodOrder));

    for (i = 0; i < (AUTHMGR_METHOD_END - 1); i++)
    {
        if (AUTHMGR_METHOD_NONE == methodOrder.order[i])
            break;
        else if (AUTHMGR_METHOD_LOCAL == methodOrder.order[i])
            ret = sal_authmgr_macauthLocal_authen(&authInfo);
        else if (AUTHMGR_METHOD_RADIUS == methodOrder.order[i])
            ret = sal_authmgr_macauthRadius_authen(&authInfo);

        if (SYS_ERR_OK == ret)
        {
            authInfo.authSts = SYS_AUTH;
            break;
        }
    }

    SYS_ERR_CHK(sal_authmgr_macauthStatus_set(&authInfo));

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_PROTO_WEBAUTH
/* Function Name:
 *      ksi_authmgr_webauthArp_add
 * Description:
 *      Add NAT IP ARP entry
 * Input:
 *      pMsg - pointer of kernel to user message
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 *      SYS_ERR_FILE_OPEN
 * Note:
 *      None
 */
int32 ksi_authmgr_webauthArp_add(ksi_msg_t *pMsg)
{
    webauth_arp_t *pReq = NULL;
    char ipv4Str[CAPA_IPV4_STR_LEN];
    char macStr[CAPA_MAC_STR_LEN];
    char cmd[SYS_BUF64_LEN];

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pReq = (webauth_arp_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    if (NULL == pReq)
        return SYS_ERR_KSI_DATA_EMPTY;

    SYS_MEM_CLEAR(ipv4Str);
    SYS_MEM_CLEAR(macStr);
    SYS_MEM_CLEAR(cmd);

    sys_util_ip2Str(pReq->ip, ipv4Str);
    sys_util_mac2Str(pReq->mac.octet, macStr);

    osal_sprintf(cmd, "arp -s %s %s 1> /dev/null 2> /dev/null", ipv4Str, macStr);

    system(cmd);

    return SYS_ERR_OK;
}

/* Function Name:
 *      ksi_authmgr_webauthArp_del
 * Description:
 *      Delete NAT IP ARP entry
 * Input:
 *      pMsg - pointer of kernel to user message
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 *      SYS_ERR_FILE_OPEN
 * Note:
 *      None
 */
int32 ksi_authmgr_webauthArp_del(ksi_msg_t *pMsg)
{
    webauth_arp_t *pReq;
    char ipv4Str[CAPA_IPV4_STR_LEN];
    char cmd[SYS_BUF64_LEN];

    SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    pReq = (webauth_arp_t*) KSI_NOTIFY_MSG_DATA_RET(pMsg);

    if (NULL == pReq)
        return SYS_ERR_KSI_DATA_EMPTY;

    SYS_MEM_CLEAR(ipv4Str);
    SYS_MEM_CLEAR(cmd);

    sys_util_ip2Str(pReq->ip, ipv4Str);

    osal_sprintf(cmd, "arp -d %s 1> /dev/null 2> /dev/null", ipv4Str);

    system(cmd);

    return SYS_ERR_OK;
}
#endif

