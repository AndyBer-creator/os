
#include <ski/ski_lkm.h>
#include <common/sys_log.h>
#include <ski/proto/igmp/ski_igmp.h>
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
#include <ski/proto/igmp/ski_mld.h>
#endif
#include <igmp/proto_igmp.h>

static int32 __init ski_igmp_lkm_init(void)
{
    int32 ret = SYS_ERR_FAILED;

    BOOTMSG_PRINTF("Init Switch Multicast Module....");

    SYS_ERR_HDL(ski_igmp_init(), error, ret);
    SYS_ERR_HDL(mcast_init(), error, ret);

#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    SYS_ERR_HDL(ski_mld_init(),error,ret);
#endif

error:
    if (SYS_ERR_OK == ret)
        BOOTMSG_PRINTF("OK\n");
    else
        BOOTMSG_FAIL_PRINTF("Init Switch Multicast Module....FAILED\n");

    return ret;
}

static void __exit ski_igmp_lkm_exit(void)
{
    SYS_PRINTF("Exit Switch Multicast Module....");

    if(SYS_ERR_OK != mcast_exit())
        SYS_PRINTF("FAIL\n");
    else
        SYS_PRINTF("OK\n");
}

module_init(ski_igmp_lkm_init);
module_exit(ski_igmp_lkm_exit);

MODULE_DESCRIPTION("Switch Multicast Module");
MODULE_LICENSE("Realtek Semiconductor Corp.");

