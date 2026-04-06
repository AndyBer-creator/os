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
 * $Revision: 9340 $
 * $Date: 2010-05-03 20:04:17 +0800 (Mon, 03 May 2010) $
 *
 * Purpose : Export the public APIs in lower layer module
 *
 * Feature : Export the public APIs in lower layer module
 *
 */
#ifndef __KSI_AUTHMGR_H__
#define __KSI_AUTHMGR_H__

/*
 * Include Files
 */
#include <ksi_core_usr.h>
#include <authmgr/proto_authmgr.h>

/*
 * Symbol Definition
 */
typedef enum ksi_authmgr_cmd_e
{
    KSI_AUTHMGR_START = (KSI_MOD_AUTHMGR * KSI_MODCMD_TOTAL),
#ifdef CONFIG_SYS_PROTO_DOT1X
    KSI_AUTHMGR_DOT1X_EAP_RADIUS_TX,
#endif
#ifdef CONFIG_SYS_PROTO_MACAUTH
    KSI_AUTHMGR_MACAUTH_AUTHING,
#endif
#ifdef CONFIG_SYS_PROTO_WEBAUTH
    KSI_AUTHMGR_WEBAUTH_ARP_ADD,
    KSI_AUTHMGR_WEBAUTH_ARP_DEL,
#endif
    KSI_AUTHMGR_END
} ksi_authmgr_cmd_t;

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
extern int32 ksi_authmgr_init(void);

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
extern int32 ksi_authmgr_dot1xEapRadius_tx(ksi_msg_t *pMsg);
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
extern int32 ksi_authmgr_macauth_authing(ksi_msg_t *pMsg);
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
extern int32 ksi_authmgr_webauthArp_add(ksi_msg_t *pMsg);

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
extern int32 ksi_authmgr_webauthArp_del(ksi_msg_t *pMsg);
#endif

#endif  /* __KSI_AUTHMGR_H__ */

