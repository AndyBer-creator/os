
#include <ski/ski_lkm.h>
#include <common/sys_log.h>
#include <ski/proto/gvrp/ski_gvrp.h>
#include <gvrp/proto_gvrp.h>

static int32 __init ski_gvrp_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch GVRP Module....");

    SYS_ERR_HDL(gvrp_init(), error, ret);
    SYS_ERR_HDL(ski_gvrp_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch GVRP Module....FAILED\n");
    return ret;
}

static void __exit ski_gvrp_lkm_exit(void)
{
    SYS_PRINTF("Exit Switch GVRP Module....");

    if(SYS_ERR_OK != gvrp_exit())
        SYS_PRINTF("FAIL\n");
    else
        SYS_PRINTF("OK\n");
}

module_init(ski_gvrp_lkm_init);
module_exit(ski_gvrp_lkm_exit);

MODULE_DESCRIPTION("Switch GVRP Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

