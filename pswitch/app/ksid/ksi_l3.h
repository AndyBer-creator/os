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
 * Purpose : Kernel to system interface - L3
 *
 * Feature : Kernel to system interface - L3
 *
 */
#ifndef __KSI_L3_H__
#define __KSI_L3_H__

/*
 * Include Files
 */
#include <ksi_core_usr.h>

/*
 * Symbol Definition
 */
typedef enum ksi_l3_cmd_e
{
    KSI_L3_START = (KSI_MOD_L3 * KSI_MODCMD_TOTAL),
    KSI_L3_INTF_LINKDOWN_EXEC,
    KSI_L3_INTF_LINKUP_EXEC,
    KSI_L3_INTF_EXIST_EXEC,
    KSI_L3_INTF_NOEXIST_EXEC,
    KSI_L3_INTF_MGMTVLAN_CHG_EXEC,
    KSI_L3_INTF_LINKADMINIPCHG_EXEC,
    KSI_L3_END
} ksi_l3_cmd_t;

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
 *      ksi_l3_init
 * Description:
 *      init function of ksi l3 module
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
extern int32 ksi_l3_init(void);

#endif  /* __KSI_L3_H__ */

