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
 * Purpose : Kernel to system interface - Port
 *
 * Feature : Kernel to system interface - Port
 *
 */
#ifndef __KSI_PORT_H__
#define __KSI_PORT_H__

/*
 * Include Files
 */
#include <ksi_core_usr.h>

/*
 * Symbol Definition
 */
typedef enum ksi_port_cmd_e
{
    KSI_PORT_START = (KSI_MOD_PORT * KSI_MODCMD_TOTAL),
    KSI_PORT_LINK_UP_EXEC,
    KSI_PORT_LINK_DOWN_EXEC,
    KSI_PORT_COMBO_OPTICAL_INSERT_EXEC,
    KSI_PORT_COMBO_OPTICAL_REMOVE_EXEC,
    KSI_PORT_END
} ksi_port_cmd_t;

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
 *      ksi_port_init
 * Description:
 *      init function of ksi port module
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
extern int32 ksi_port_init(void);

/* Function Name:
 *      ksi_port_linkUp_exec
 * Description:
 *      Execute actions on port link up
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
extern int32 ksi_port_linkUp_exec(ksi_msg_t *pMsg);

/* Function Name:
 *      ksi_port_linkDown_exec
 * Description:
 *      Execute actions on port link down
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
extern int32 ksi_port_linkDown_exec(ksi_msg_t *pMsg);

/* Function Name:
 *      ksi_port_opticalInsert_exec
 * Description:
 *      Execute actions on optical module inserted
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
extern int32 ksi_port_opticalInsert_exec(ksi_msg_t *pMsg);

/* Function Name:
 *      ksi_port_opticalRemove_exec
 * Description:
 *      Execute actions on optical module removed
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
extern int32 ksi_port_opticalRemove_exec(ksi_msg_t *pMsg);

#endif  /* __KSI_PORT_H__ */

