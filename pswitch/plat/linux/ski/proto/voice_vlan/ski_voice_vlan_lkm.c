
#include <common/sys_log.h>
#include <ski/ski_lkm.h>
#include <ski/proto/voice_vlan/ski_voice_vlan.h>

#include <voice_vlan/proto_voice_vlan.h>

static int32 __init ski_voice_vlan_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch Voice VLAN Module....");

    SYS_ERR_HDL(voice_vlan_init(), error, ret);
    SYS_ERR_HDL(ski_voice_vlan_init(), error, ret);

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch Voice VLAN Module....FAILED\n");
    return ret;
}

static void __exit ski_voice_vlan_lkm_exit(void)
{
    osal_printf("Exit Switch Voice VLAN Module....");

    if(SYS_ERR_OK != voice_vlan_exit())
        osal_printf("FAIL\n");
    else
        osal_printf("OK\n");
}

module_init(ski_voice_vlan_lkm_init);
module_exit(ski_voice_vlan_lkm_exit);

MODULE_DESCRIPTION("Switch Voice VLAN Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

