
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <board/include/intf.h>
#include <board/include/gpio.h>

#define DEF_START_DELAY_TIMES 200

typedef enum sys_button_app_action_type_e
{
	APP_ACT_TYPE_NONE = 0,
    APP_ACT_TYPE_REBOOT,
    APP_ACT_TYPE_RESTORE_ACCOUNT,
    APP_ACT_TYPE_RESTORE_FACTORY,
    APP_ACT_TYPE_END,
} sys_button_app_act_type_t;

typedef enum sys_button_type_e
{
    BOTTON_CTL_TYPE_GPIO = 0,
    BOTTON_CTL_TYPE_END,
} sys_button_type_t;

typedef struct sys_button_reset_appConf_s
{
    uint32 timer;    
    uint32 action;   
} sys_button_reset_appConf_t;

typedef struct sys_button_reset_conf_s
{
    sys_gpio_t                  *pGpio;

    sys_enable_t                 pushhold_state;   
    uint32                       pushhold_timer;   
    uint32                       pushhold_action;  

    uint32                       appNum;           
    sys_button_reset_appConf_t   *pApp;            
} sys_button_reset_conf_t;

typedef struct sys_button_conf_s
{
    sys_button_type_t type;
    void              *pBtnConf;
} sys_button_conf_t;

typedef enum sys_button_pushhold_action_e
{
    SYS_PUSH_HOLD_ACT = 0,                  
    SYS_PUSH_INTERRUPTE_ACT_END,			
    SYS_PUSH_ACT_END
} sys_button_pushhold_act_t;

extern int32 board_button_init(void);

extern int32 board_button_reset_init(sys_button_reset_conf_t *pBtnConf);

#endif 

