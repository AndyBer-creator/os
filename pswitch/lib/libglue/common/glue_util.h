/*
 * Copyright (C) 2009 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision: 10229 $
 * $Date: 2010-06-12 18:52:20 +0800 (Sat, 12 Jun 2010) $
 *
 * Purpose : Glue Utilities
 *
 * Feature : Glue Utilities
 *
 */
#ifndef __GLUE_UTIL_H__
#define __GLUE_UTIL_H__

/*
 * Include Files
 */
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <ski/ski_core.h>

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
 *      glue_util_ioctl
 * Description:
 *      Kernel ioctl function
 * Input:
 *      cmd   - command id that kernel will take action
 *      pData - pointer of data to be passed or retrieved
 * Output:
 *      None
 * Return:
 *      SYS_ERR_OK
 *      SYS_ERR_FAILED
 * Note:
 *    None
 */
extern int32 glue_util_ioctl(uint32 cmd, void *pData);

#endif /* __GLUE_UTIL_H__ */


