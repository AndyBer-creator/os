
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/authmgr/ski_authmgr.h>

#include <authmgr/proto_authmgr.h>

static int32 __init ski_authmgr_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch AUTHMGR Module....");

    SYS_ERR_HDL(authmgr_init(), error, ret);
    SYS_ERR_HDL(ski_authmgr_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch AUTHMGR Module....FAILED\n");
    return ret;
}

static void __exit ski_authmgr_lkm_exit(void)
{
    osal_printf("Exit Switch AUTHMGR Module....");

    if(SYS_ERR_OK != authmgr_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_authmgr_lkm_init);
module_exit(ski_authmgr_lkm_exit);

MODULE_DESCRIPTION("Switch AUTHMGR Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

