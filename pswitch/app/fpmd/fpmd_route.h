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
 * Purpose : Forward plane manager daemon - route translate and set
 *
 * Feature : Forward plane manager daemon - route translate and set
 *
 */

#ifndef __FPMD_ROUTE_H__
#define __FPMD_ROUTE_H__

/*
 * Include Files
 */
#include <net/route.h>
#include <netinet/in.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#include <common/sys_l3.h>
#include <libsal/sal_util.h>

/*
 * Symbol Definition
 */

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

/*
 * Function Declaration
 */
extern int32 route_transl(char *data, uint32 len);

#endif /*__FPMD_ROUTE_H__*/
