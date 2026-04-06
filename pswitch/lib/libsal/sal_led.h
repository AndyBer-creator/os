
#ifndef __SAL_LED_H__
#define __SAL_LED_H__

#include <common/sys_def.h>
#include <rsd/rsd_led.h>

extern int32 sal_led_switchEnable_set(sys_led_type_t type, sys_enable_t enable);

extern int32 sal_led_swctrl_set(sys_led_type_t type, sys_led_swCtrlAct_t action);

extern int32 sal_led_systemCtrl_set(sys_led_state_t state);

#endif 

