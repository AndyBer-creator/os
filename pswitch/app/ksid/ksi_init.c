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
 * $Revision: 13829 $
 * $Date: 2010-11-03 11:10:51 +0800 (Wed, 03 Nov 2010) $
 *
 * Purpose : KSI init
 *
 * Feature : KSI init
 *
 */

/*
 * Include Files
 */
#include <ksi_port.h>
#include <ksi_sys.h>

#ifdef CONFIG_SYS_PROTO_AUTHMGR
#include <ksi_authmgr.h>
#endif
#ifdef CONFIG_SYS_PROTO_ISG
#include <ksi_isg.h>
#endif
#ifdef CONFIG_SYS_L3_ROUTE
#include <ksi_l3.h>
#endif

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
 *      ksi_init
 * Description:
 *      init function of ksi module
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
int32 ksi_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    SYS_ERR_HDL(ksi_sys_init(), error, ret);

    SYS_ERR_HDL(ksi_port_init(), error, ret);
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    SYS_ERR_HDL(ksi_authmgr_init(), error, ret);
#endif
#ifdef CONFIG_SYS_PROTO_ISG
    SYS_ERR_HDL(ksi_isg_init(), error, ret);
#endif
#ifdef CONFIG_SYS_L3_ROUTE
    SYS_ERR_HDL(ksi_l3_init(), error, ret);
#endif

error:
    return ret;
}

