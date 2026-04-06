
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/lldp/ski_lldp.h>

#include <lldp/proto_lldp.h>

static int32 __init ski_lldp_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch LLDP Module....");

    SYS_ERR_HDL(lldp_init(), error, ret);
    SYS_ERR_HDL(ski_lldp_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch LLDP Module....FAILED\n");
    return ret;
}

static void __exit ski_lldp_lkm_exit(void)
{
    osal_printf("Exit Switch LLDP Module....");

    if(SYS_ERR_OK != lldp_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_lldp_lkm_init);
module_exit(ski_lldp_lkm_exit);

MODULE_DESCRIPTION("Switch LLDP Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

