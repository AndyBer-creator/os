
#include <common/sys_def.h>

int32 osal_timer_init(osal_timer_list_t *pTimerList, timer_cb_f fAction, uint32 id)
{
    SYS_PARAM_CHK((NULL == pTimerList), SYS_ERR_NULL_POINTER);

    init_timer(pTimerList);

    pTimerList->function = fAction;

    pTimerList->data = id;

    return SYS_ERR_OK;
}

int32 osal_timer_start(osal_timer_list_t *pTimerList, uint32 msec)
{
    uint32 tick = 0;

    SYS_PARAM_CHK((NULL == pTimerList), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((msec < 1), SYS_ERR_INPUT);

    tick = (msec * HZ) / 1000;

    pTimerList->expires = (jiffies + tick);

    add_timer(pTimerList);

    return SYS_ERR_OK;
}

int32 osal_timer_stop(osal_timer_list_t *pTimerList)
{
    SYS_PARAM_CHK((NULL == pTimerList), SYS_ERR_NULL_POINTER);

    del_timer(pTimerList);

    return SYS_ERR_OK;
}

int32 osal_timer_refresh(osal_timer_list_t *pTimerList, uint32 interval)
{
    return osal_timer_start(pTimerList, interval);
}

