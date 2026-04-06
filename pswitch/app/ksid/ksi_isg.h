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
 * Purpose : Kernel to system interface - ISG
 *
 * Feature : Kernel to system interface - ISG
 *
 */
#ifndef __KSI_ISG_H__
#define __KSI_ISG_H__

/*
 * Include Files
 */
#include <ksi_core_usr.h>

/*
 * Symbol Definition
 */
typedef enum ksi_isg_cmd_e
{
    KSI_ISG_START = (KSI_MOD_ISG * KSI_MODCMD_TOTAL),
    KSI_ISG_DB_MODIFY,
    KSI_ISG_END
} ksi_isg_cmd_t;

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
 *      ksi_isg_init
 * Description:
 *      init function of ksi isg module
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
extern int32 ksi_isg_init(void);

/* Function Name:
 *      ksi_isg_dbModify_exec
 * Description:
 *      Execute db modify action
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
extern int32 ksi_isg_dbModify_exec(ksi_msg_t *pMsg);

#endif  /* __KSI_ISG_H__ */

