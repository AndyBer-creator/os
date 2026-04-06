
#ifndef __LINUX_OSAL_TIMER_H__
#define __LINUX_OSAL_TIMER_H__

#ifdef __KERNEL__

#include <linux/jiffies.h>
#include <linux/timer.h>

#define SYS_TIMER_MSEC (1)
#define SYS_TIMER_SEC  (1000)

typedef struct timer_list osal_timer_list_t;

typedef void (*timer_cb_f)(unsigned long id);

extern int32 osal_timer_init(osal_timer_list_t *pTimerList, timer_cb_f fAction, uint32 id);

extern int32 osal_timer_start(osal_timer_list_t *pTimerList, uint32 msec);

extern int32 osal_timer_stop(osal_timer_list_t *pTimerList);

extern int32 osal_timer_refresh(osal_timer_list_t *pTimerList, uint32 interval);

#endif 

#endif 

