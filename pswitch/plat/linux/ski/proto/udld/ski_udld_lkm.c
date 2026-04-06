
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/udld/ski_udld.h>

#include <udld/proto_udld.h>

static int32 __init ski_udld_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch UDLD Module....");

    SYS_ERR_HDL(udld_init(), error, ret);
    SYS_ERR_HDL(ski_udld_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch UDLD Module....FAILED\n");
    return ret;
}

static void __exit ski_udld_lkm_exit(void)
{
    osal_printf("Exit Switch UDLD Module....");

    if(SYS_ERR_OK != udld_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_udld_lkm_init);
module_exit(ski_udld_lkm_exit);

MODULE_DESCRIPTION("Switch UDLD Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

