
#ifndef __RSD_LED_H__
#define __RSD_LED_H__

#include <common/sys_def.h>
#include <board/include/led.h>

typedef enum sys_led_state_e
{
    SYS_LED_STATE_ALLPORT_ON = 0,
    SYS_LED_STATE_ALLPORT_OFF,
    SYS_LED_STATE_ALLPORT_NORMAL,
    SYS_LED_STATE_END
} sys_led_state_t;

extern int32 rsd_led_switchEnable_set(sys_led_type_t type, sys_enable_t enable);

extern int32 rsd_led_swctrl_set(sys_led_type_t type, sys_led_swCtrlAct_t action);

extern int32 rsd_led_systemCtrl_set(sys_led_state_t state);

#endif 

