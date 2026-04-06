
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/surveillancevlan/ski_surveillancevlan.h>
#include <surveillancevlan/proto_surveillancevlan.h>

static int32 __init ski_surveillance_vlan_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch Surveillance VLAN Module....");

    SYS_ERR_HDL(surveillance_vlan_init(), error, ret);
    SYS_ERR_HDL(ski_surveillance_vlan_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch Surveillance VLAN Module....FAILED\n");
    return ret;
}

static void __exit ski_surveillance_vlan_lkm_exit(void)
{
    osal_printf("Exit Switch Surveillance VLAN Module....");

    if(SYS_ERR_OK != surveillance_vlan_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_surveillance_vlan_lkm_init);
module_exit(ski_surveillance_vlan_lkm_exit);

MODULE_DESCRIPTION("Switch Surveillance VLAN Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

