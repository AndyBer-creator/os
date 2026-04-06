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
 * $Revision: 16067 $
 * $Date: 2011-03-03 09:57:34 +0800 (Thu, 03 Mar 2011) $
 *
 * Purpose : Forward plane manager daemon
 *
 * Feature : Forward plane manager daemon
 *
 */

#ifndef __FPMD_H__
#define __FPMD_H__

/*
 * Include Files
 */
#include <net/route.h>
#include <netinet/in.h>
#include <linux/if_addr.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <common/sys_l3.h>
#include <libsal/sal_util.h>

/*
 * Symbol Definition
 */
#define FPMD_PID_FILE "/var/run/fpmd.pid"

typedef struct fpm_route_hdr_s
{
    struct nlmsghdr nl;
    struct rtmsg rt;
} fpm_route_hdr_t;

typedef struct rtattr fpm_rtattr_t;

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */
#define FPMD_DBG(fmt, args...) SYS_DBG(DBG_L3_FPM, fmt, ##args)
//#define FPMD_DBG(fmt, args...) SYS_PRINTF(fmt, ##args)

/*
 * Function Declaration
 */
extern int32 route_transl(char *data, uint32 len);
extern int32 addr_monitor_create(void);

#endif /*__FPMD_H__*/
