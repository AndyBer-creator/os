
#ifndef __OSAL_H__
#define __OSAL_H__

#ifdef CONFIG_SYS_L3_ROUTE
extern int kal_register_netevent_notifier(struct notifier_block *nb);
#endif
#endif

