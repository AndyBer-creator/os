
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/stp/ski_stp.h>

#include <stp/proto_stp.h>

static int32 __init ski_stp_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch STP Module....");

    SYS_ERR_HDL(stp_init(), error, ret);
    SYS_ERR_HDL(ski_stp_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch STP Module....FAILED\n");
    return ret;
}

static void __exit ski_stp_lkm_exit(void)
{
    osal_printf("Exit Switch STP Module....");

    if(SYS_ERR_OK != stp_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_stp_lkm_init);
module_exit(ski_stp_lkm_exit);

MODULE_DESCRIPTION("Switch STP Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

