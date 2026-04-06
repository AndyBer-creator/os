
#include <linux/version.h>
#include <linux/module.h>
#include <common/sys_def.h>
#include <linux/net/net_svi.h>

static int32 __init _net_init(void)
{
    BOOTMSG_PRINTF("Init Net Module....");

    if (SYS_ERR_OK != svi_init())
        goto out;

    BOOTMSG_PRINTF("OK\n");

    return SYS_ERR_OK;

out:
    BOOTMSG_FAIL_PRINTF("FAIL\n");
    return SYS_ERR_FAILED;
}

static void __exit _net_exit(void)
{
    svi_exit();

    return;
}

module_init(_net_init);
module_exit(_net_exit);

MODULE_DESCRIPTION("Switch Net Driver Module");

MODULE_LICENSE("GPL");

