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
 * $Revision: 14862 $
 * $Date: 2010-12-17 14:44:06 +0800 (Fri, 17 Dec 2010) $
 *
 * Purpose : Kernel and Systerm communication interface
 *
 * Feature : Kernel and Systerm communication interface
 *
 */

#ifndef __KSI_CORE_USR_H__
#define __KSI_CORE_USR_H__

/*
 * Include Files
 */
#include <ksi/ksi_core.h>

/*
 * Symbol Definition
 */
typedef enum ksi_mod_e
{
    KSI_MOD_ISG = 0,
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    KSI_MOD_AUTHMGR,
#endif
    KSI_MOD_PORT,
    KSI_MOD_SYS,
    KSI_MOD_L3,
    /* max module id */
    KSI_MOD_END
} ksi_mod_t;

typedef struct ksi_execParam_s
{
    uint32    cmd;
    ksi_msg_t *pMsg;
} ksi_execParam_t;

typedef struct ksi_mapper_s
{
    int32 no;
    int32 (*fCallback)(unsigned int cmd, ksi_msg_t *pMsg);
} ksi_mapper_t;

#define KSI_MODCMD_TOTAL    1024

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
 *      ksi_handler_register
 * Description:
 *      Register message process callback function for specific module
 * Input:
 *      modId     - id of moudule
 *      fCallback - call back function
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern int32 ksi_handler_register(int modId, int (*fCallback)(unsigned int cmd, ksi_msg_t *pMsg));

/* Function Name:
 *      ksi_msg_reply
 * Description:
 *      Reply with data to specific message coming from kernel
 * Input:
 *      pMsg    - pointer of reply message
 *      datalen - length of reply data
 *      data    - pointer of reply data
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 * Note:
 *      None
 */
extern int32 ksi_msg_reply(ksi_msg_t *pMsg, uint32 datalen, void *data);


#endif  /* __KSI_CORE_USR_H__ */

