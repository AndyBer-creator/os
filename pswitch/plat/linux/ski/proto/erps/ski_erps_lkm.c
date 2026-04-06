
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/erps/ski_erps.h>

#include <erps/proto_sys_erps.h>

static int32 __init ski_erps_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch ERPS Module....");

    SYS_ERR_HDL(erps_init(), error, ret);
    SYS_ERR_HDL(ski_erps_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch ERPS Module....FAILED\n");
    return ret;
}

static void __exit ski_erps_lkm_exit(void)
{
    osal_printf("Exit Switch ERPS Module....");

    if(SYS_ERR_OK != erps_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_erps_lkm_init);
module_exit(ski_erps_lkm_exit);

MODULE_DESCRIPTION("Switch ERPS Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

