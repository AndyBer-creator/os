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
 * $Revision: 16227 $
 * $Date: 2011-03-10 18:23:52 +0800 (Thu, 10 Mar 2011) $
 *
 * Purpose : Process special event which can't be processed in normal thread
 *
 * Feature : Process special event which can't be processed in normal thread
 */

/*
 * Include Files
 */
#include <common/sys_util.h>
#include <libsal/sal_util.h>
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
#include <libsal/sal_rmon.h>
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
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
void handleRmonApply(void)
{
    sal_rmon_all_apply();

    return;
}
#endif

