
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/dai/ski_dai.h>
#include <dai/proto_dai.h>

static int32 __init ski_dai_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch DAI Module....");

    SYS_ERR_HDL(dai_init(), error, ret);
    SYS_ERR_HDL(ski_dai_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch DAI Module....FAILED\n");
    return ret;
}

static void __exit ski_dai_lkm_exit(void)
{
    osal_printf("Exit Switch DAI Module....");

    if(SYS_ERR_OK != dai_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_dai_lkm_init);
module_exit(ski_dai_lkm_exit);

MODULE_DESCRIPTION("Switch DAI Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

