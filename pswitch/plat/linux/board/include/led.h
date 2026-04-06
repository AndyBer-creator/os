
#ifndef __LED_H__
#define __LED_H__

#include <rtk/led.h>

typedef enum sys_led_swCtrlAct_e
{
    LED_SWCTRL_ACT_OFF = 0,
    LED_SWCTRL_ACT_BLINK_32MS,
    LED_SWCTRL_ACT_BLINK_64MS,
    LED_SWCTRL_ACT_BLINK_128MS,
    LED_SWCTRL_ACT_BLINK_256MS,
    LED_SWCTRL_ACT_BLINK_512MS,
    LED_SWCTRL_ACT_BLINK_1024MS,
    LED_SWCTRL_ACT_ON,
    LED_SWCTRL_ACT_DISABLE,
    LED_SWCTRL_ACT_END
} sys_led_swCtrlAct_t;

typedef enum sys_led_portEntity_e
{
    LED_PORT_ENTITY_0 = 1,
    LED_PORT_ENTITY_1,
    LED_PORT_ENTITY_0_1,
    LED_PORT_ENTITY_2,
    LED_PORT_ENTITY_0_2,
    LED_PORT_ENTITY_1_2,
    LED_PORT_ENTITY_0_1_2,
    LED_PORT_ENTITY_END
} sys_led_portEntity_t;

typedef enum sys_led_ctrl_type_e
{
    LED_CTRL_SWITCH_GLB = 0,
    LED_CTRL_SWITCH_PORT,
    LED_CTRL_GPIO,
    LED_CTRL_GPIO_BLINKING,
    LED_CTRL_END
} sys_led_ctrl_type_t;

typedef struct sys_led_ctrlPort_e
{
    uint32 port;   
    uint32 entity; 
} sys_led_ctrlPort_t;

typedef union sys_led_ctrl_u
{
    sys_gpio_t         gpio;
    sys_led_ctrlPort_t portCtrl;
} sys_led_ctrl_t;

typedef struct sys_led_conf_ctrl_s
{
    sys_led_ctrl_type_t ctrlType;
    sys_led_ctrl_t      ctrlConf;
} sys_led_conf_ctrl_t;

typedef enum sys_led_type_e
{
    SYS_LED_TYPE_SYS,
    SYS_LED_TYPE_ALARM,
    SYS_LED_TYPE_SYS_BLINKING,
    SYS_LED_TYPE_END
} sys_led_type_t;

typedef struct sys_led_conf_s
{
    sys_led_type_t type;
    void           *pLedCtrlConf;
} sys_led_conf_t;

extern int32 board_led_init(void);

extern int32 board_led_portSwCtrl_set(sys_logic_port_t port, sys_led_portEntity_t entity, sys_led_swCtrlAct_t act);

extern int32 board_led_sys_set(sys_led_swCtrlAct_t act);

extern int32 board_led_alarm_set(sys_led_swCtrlAct_t act);

extern int32 board_led_action_set(sys_led_type_t type, sys_led_swCtrlAct_t act);

#endif 

