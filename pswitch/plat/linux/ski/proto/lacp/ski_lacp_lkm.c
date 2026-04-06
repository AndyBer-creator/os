
#include <common/sys_log.h>
#include <common/sys_error.h>

#include <ski/ski_lkm.h>
#include <ski/proto/lacp/ski_lacp.h>

#include <lacp/proto_lacp.h>

static int32 __init ski_lacp_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch LACP Module....");

    SYS_ERR_HDL(lacp_init(), error, ret);
    SYS_ERR_HDL(ski_lacp_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch LACP Module....FAILED\n");
    return ret;
}

static void __exit ski_lacp_lkm_exit(void)
{
    osal_printf("Exit Switch LACP Module....");

    if(SYS_ERR_OK != lacp_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_lacp_lkm_init);
module_exit(ski_lacp_lkm_exit);

MODULE_DESCRIPTION("Switch LACP Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

