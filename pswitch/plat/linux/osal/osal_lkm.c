
#include <ski/ski_core.h>
#include "osal_lkm.h"
#include "time.h"

static int __init  osal_init(void)
{
    BOOTMSG_PRINTF("Init OS Abstract Layer Module....OK\n");

    return SYS_ERR_OK;
}

static void __exit osal_exit(void)
{
    BOOTMSG_PRINTF("Exit OS Abstract Layer Module....OK\n");
}

#ifdef CONFIG_SYS_L3_ROUTE
#include <net/netevent.h>
int kal_register_netevent_notifier(struct notifier_block *nb)
{
        return register_netevent_notifier(nb);
}
EXPORT_SYMBOL(kal_register_netevent_notifier);
#endif

EXPORT_SYMBOL(osal_timer_refresh);
EXPORT_SYMBOL(osal_timer_start);
EXPORT_SYMBOL(osal_timer_stop);
EXPORT_SYMBOL(osal_timer_init);
EXPORT_SYMBOL(osal_time_sysUptime_get);

module_init(osal_init);
module_exit(osal_exit);
MODULE_DESCRIPTION ("OS Abstract Layer Module");
MODULE_LICENSE("GPL");
