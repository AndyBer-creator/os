
#ifndef __SKI_LED_H__
#define __SKI_LED_H__

#include <ski/ski_core.h>
#include <rsd/rsd_led.h>

typedef enum ski_led_cmd_e
{
    
    SKI_LED_SET_BASE = (SKI_CORE_MOD_LED * SKI_CORE_MODCMD_TOTAL),
    SKI_LED_SET_SWITCH_ENABLE,
    SKI_LED_SET_SWCTRL,
    SKI_LED_SET_SYSTEM_CTRL,

    SKI_LED_GET_BASE = (SKI_LED_SET_BASE + SKI_CORE_MODCMD_RANGE),

    SKI_LED_CMD_END
} ski_led_cmd_t;

typedef struct ski_led_SwitchEnable_s
{
    sys_led_type_t type;
    sys_enable_t enable;
} ski_led_switchEnable_t;

typedef struct ski_led_swctrl_s
{
    sys_led_type_t type;
    sys_led_swCtrlAct_t action;
} ski_led_swctrl_t;

typedef struct ski_led_systemCtrl_s
{
    sys_led_state_t state;
} ski_led_systemCtrl_t;

typedef union ski_led_ioctl_u
{
    ski_led_switchEnable_t  switchEnable;
    ski_led_swctrl_t        swctrl;
    ski_led_systemCtrl_t    systemCtrl;
} ski_led_ioctl_t;

extern int32 ski_led_init(void);

#endif  

