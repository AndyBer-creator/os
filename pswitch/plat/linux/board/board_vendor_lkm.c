
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <common/sys_def.h>
#include <board/vendor/board_vendor.h>

static int32 __init _board_vendor_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Board Vendor Module....");
    SYS_ERR_HDL(board_vendor_init(), error, ret);

 error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Board Vendor Module....FAILED\n");

    return ret;
}

static void __exit _board_vendor_exit(void)
{
}

module_init(_board_vendor_init);
module_exit(_board_vendor_exit);

MODULE_DESCRIPTION ("Switch Board Vendor Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

