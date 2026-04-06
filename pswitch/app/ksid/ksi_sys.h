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
 * Purpose : Kernel to system interface - System
 *
 * Feature : Kernel to system interface - System
 *
 */
#ifndef __KSI_SYS_H__
#define __KSI_SYS_H__

/*
 * Include Files
 */
#include <ksi_core_usr.h>

/*
 * Symbol Definition
 */
typedef enum ksi_sys_cmd_e
{
    KSI_SYS_START = (KSI_MOD_SYS * KSI_MODCMD_TOTAL),
    KSI_SYS_RESET_BUTTON_ACTION_EXEC,
    KSI_SYS_SNMP_MSG_SEND,
    KSI_SYS_HOSTADDR_MSG_GET,
#ifdef CONFIG_SYS_ADVANCE_DNS
    KSI_SYS_DNS_NOTIFY,
#endif

    KSI_SYS_END
} ksi_sys_cmd_t;

/*
 * Data Declaration
 */
#ifdef CONFIG_SYS_SNMP
typedef struct ksi_sys_snmp_msg_s
{
    char oidStr[CAPA_SNMP_OID_STR_LEN];
    char msgStr[SYS_BUF256_LEN];
} ksi_sys_snmp_msg_t;
#endif

/*
 * Macro Definition
 */

/*
 * Function Declaration
 */

/* Function Name:
 *      ksi_sys_init
 * Description:
 *      init function of ksi sys module
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
extern int32 ksi_sys_init(void);

/* Function Name:
 *      ksi_sys_resetButtonAction_exec
 * Description:
 *      Execute reset button action base on status
 * Input:
 *      pMsg - pointer of kernel to user message
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 *      SYS_ERR_KSI_DATA_EMPTY
 * Note:
 *      None
 */
extern int32 ksi_sys_resetButtonAction_exec(ksi_msg_t *pMsg);

#ifdef CONFIG_SYS_UI_SNMP
/* Function Name:
 *      ksi_sys_snmp_msg_send
 * Description:
 *      Send snmp trap message
 * Input:
 *      pMsg - pointer of kernel to user message
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 *      SYS_ERR_KSI_DATA_EMPTY
 * Note:
 *      None
 */
extern int32 ksi_sys_snmp_msg_send(ksi_msg_t *pMsg);
#endif

extern int32 ksi_sys_hostAddr_get(ksi_msg_t *pMsg);

#endif  /* __KSI_SYS_H__ */

