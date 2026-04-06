
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/dhcp/ski_dhcp.h>

#include <dhcp/proto_dhcp.h>

static int32 __init ski_dhcp_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch DHCP Module....");

    SYS_ERR_HDL(dhcp_init(), error, ret);
    SYS_ERR_HDL(ski_dhcp_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch DHCP Module....FAILED\n");
    return ret;
}

static void __exit ski_dhcp_lkm_exit(void)
{
    osal_printf("Exit Switch DHCP Module....");

    if(SYS_ERR_OK != dhcp_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_dhcp_lkm_init);
module_exit(ski_dhcp_lkm_exit);

MODULE_DESCRIPTION("Switch DHCP Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

