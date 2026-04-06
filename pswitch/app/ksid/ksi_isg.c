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
 * Purpose : Kernel to system interface - ISG
 *
 * Feature : Kernel to system interface - ISG
 *
 */

/*
 * Include Files
 */
#include <ksi_isg.h>
#include <libsal/sal_isg.h>

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
 *      ksi_isg_cmd_mux
 * Description:
 *      Dispatch command in ksi message for isg module
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      SYS_ERR_NULL_POINTER
 * Note:
 *      None
 */
static int32 ksi_isg_cmd_mux(unsigned int cmd, ksi_msg_t *pMsg)
{
    //SYS_PARAM_CHK((NULL == pMsg), SYS_ERR_NULL_POINTER);

    switch (cmd)
    {
        case KSI_ISG_DB_MODIFY:
            SYS_ERR_CHK(ksi_isg_dbModify_exec(pMsg));
            break;

        default:
            return SYS_ERR_KSI_CMD_NOT_FOUND;
    }

    return SYS_ERR_OK;
}

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
 * Note:
 *      None
 */
int32 ksi_isg_init(void)
{
    SYS_ERR_CHK(ksi_handler_register(KSI_MOD_ISG, ksi_isg_cmd_mux));

    return SYS_ERR_OK;
}

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
int32 ksi_isg_dbModify_exec(ksi_msg_t *pMsg)
{
    char cmd[SYS_BUF128_LEN];

    SYS_MEM_CLEAR(cmd);
    osal_sprintf(cmd, "handler -i isgdb_backup &");
    system(cmd);

    return SYS_ERR_OK;
}

