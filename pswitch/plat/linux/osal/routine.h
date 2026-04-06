#ifndef __LINUX_ROUTINE_H__
#define __LINUX_ROUTINE_H__

#include <osal/wait.h>
#include <osal/atomic.h>
#include <common/sys_autoconf.h>

typedef void (*routine_act)(void);

#define ROUTINE_INIT(name, time)                             \
static osal_timer_list_t _timer_##name;                      \
static uint32 _init_##name = 0;                              \
static uint32 _wait_id_##name = 0;                           \
static volatile int32 _thread_status_##name = -1;            \
static volatile int32 _routine_run_##name = 0;               \
static void _timer_cb_##name(unsigned long _id)              \
{                                                            \
    osal_wake_up(_wait_id_##name);                           \
    osal_timer_refresh(&_timer_##name, time);                \
}                                                            \
static void _routine_thread_##name(void *actFunc)            \
{                                                            \
    routine_act _actFunc = (routine_act)actFunc;             \
    if (0 == _init_##name)                                   \
    {                                                        \
        osal_wait_module_create(&_wait_id_##name);           \
        osal_timer_init(&_timer_##name, _timer_cb_##name, 0);\
        _init_##name = 1;                                    \
    }                                                        \
    osal_timer_start(&_timer_##name, time);                  \
    _thread_status_##name = 0;                               \
    while(1)                                                 \
    {                                                        \
        _actFunc();                                          \
        osal_wait_event(_wait_id_##name);                    \
        if (1 == _thread_status_##name)                      \
            break;                                           \
    }                                                        \
    _thread_status_##name = -1;                              \
    osal_thread_exit(0);                                     \
}

#define ROUTINE_START(name, descp, actFunc, pri, stackSize)                        \
do {                                                                               \
    if (1 == _routine_run_##name) break;                                           \
    if (-1 != _thread_status_##name) break;                                        \
    if ((osal_thread_t)NULL == (osal_thread_create(descp,                          \
                                                   stackSize,                      \
                                                   pri,                            \
                                                   (void *)_routine_thread_##name, \
                                                   (void *)actFunc)))              \
    {                                                                              \
        SYS_PRINTF("%s routine create failed\n", descp);                           \
        return SYS_ERR_FAILED;                                                     \
    }                                                                              \
    _routine_run_##name = 1;                                                       \
} while(0)

#define ROUTINE_STOP(name)                                        \
do {                                                              \
    if (1 == _routine_run_##name                                  \
        && _thread_status_##name == 0)                            \
    {                                                             \
        _thread_status_##name = 1;                                \
        while((-1 != _thread_status_##name)){osal_time_sleep(1);} \
        osal_timer_stop(&_timer_##name);                          \
        _routine_run_##name = 0;                                  \
    }                                                             \
} while(0)

#endif 

