/*
 * Copyright (C) 2009-2016 Realtek Semiconductor Corp.
 * All Rights Reserved.
 *
 * This program is the proprietary software of Realtek Semiconductor
 * Corporation and/or its licensors, and only be used, duplicated,
 * modified or distributed under the authorized license from Realtek.
 *
 * ANY USE OF THE SOFTWARE OTHER THAN AS AUTHORIZED UNDER
 * THIS LICENSE OR COPYRIGHT LAW IS PROHIBITED.
 *
 * $Revision$
 * $Date$
 *
 * Purpose : Definition of Customer API
 *
 * Feature : The file includes the following modules and sub-modules
 *
 */

/*
 * Include Files
 */
#include <common/rt_autoconf.h>
#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
#include <linux/module.h>
#endif
#include <common/rt_error.h>
#include <common/util/rt_util.h>
#include <common/util/rt_util_system.h>
#include <ioal/ioal_param.h>
#include <common/debug/rt_log.h>
#include <common/type.h>
#include <dal/dal_mgmt.h>
#include <rtk/default.h>
#include <osal/print.h>
#include <osal/memory.h>
#include <ioal/mem32.h>
#include <rtk/customer/customer_api.h>
#include <rtk/switch.h>
#include <hwp/hw_profile.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Function Declaration
 */

/* Module Name : customer */

/* Function Name:
 *      rtk_customer_api_init
 * Description:
 *      This API is hooked in RTK initial flow already, and customer can fill the
 *      initial process in this API.
 * Input:
 *      unit            - unit id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID               - invalid unit id
 *      RT_ERR_OUT_OF_RANGE     - dumped table index is out of range
 * Applicable:
 *
 * Note:
 *      This API is exported to other kernel module, then other modules can
 *      initial the customer API part, too.
 * Changes:
 *      None
 */
int32
rtk_customer_api_init(void)
{
    rtk_mac_t MacAddr;
    uint32 unit = 0;
    int32 ret;

    osal_memset(&MacAddr, 0, sizeof(rtk_mac_t));
    /*Setting CPU MAC address. User can customize it.*/
    if ((ret = ioal_param_sysMac_get(&MacAddr)) != RT_ERR_OK)
    {
        RT_INIT_ERR(ret, MOD_INIT, "ioal_param_sysMac_get fail\n");
        return ret;
    }

    HWP_UNIT_TRAVS_LOCAL(unit)
    {
        if ((ret = rtk_switch_mgmtMacAddr_set(unit, &MacAddr)) != RT_ERR_OK)
        {
            RT_INIT_ERR(ret, MOD_INIT, "rtk_switch_mgmtMacAddr_set fail (unit=%u)\n", unit); 
            return ret;
        }
    }

    if(HWP_9310_FAMILY_ID(HWP_MY_UNIT_ID()))
    {
        HWP_UNIT_TRAVS_LOCAL(unit)
        {   /* 默认capwap识别的udp端口号是0, 所以判断为非法capwap报文后无法转发, 修改为50111和50112这两个无人使用的端口 */
            if ((ret = rtk_capwap_udpPort_set(unit, 50111, 50112)) != RT_ERR_OK)
            {
                RT_INIT_ERR(ret, MOD_INIT, "rtk_capwap_udpPort_set fail (unit=%u)\n", unit); 
                return ret;
            }
        }
    }
    return RT_ERR_OK;
} /* end of rtk_customer_api_init */


#if defined(CONFIG_SDK_KERNEL_LINUX_KERNEL_MODE)
EXPORT_SYMBOL(rtk_customer_api_init);
#endif


