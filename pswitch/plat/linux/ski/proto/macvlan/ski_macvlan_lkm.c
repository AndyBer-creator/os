
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/macvlan/ski_macvlan.h>
#include <macvlan/proto_macvlan.h>

static int32 __init ski_mac_vlan_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch MAC VLAN Module....");

    SYS_ERR_HDL(mac_vlan_init(), error, ret);
    SYS_ERR_HDL(ski_mac_vlan_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch MAC VLAN Module....FAILED\n");
    return ret;
}

static void __exit ski_mac_vlan_lkm_exit(void)
{
    osal_printf("Exit Switch MAC VLAN Module....");
    osal_printf("OK\n");
}

module_init(ski_mac_vlan_lkm_init);
module_exit(ski_mac_vlan_lkm_exit);

MODULE_DESCRIPTION("Switch MAC VLAN Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

