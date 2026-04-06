
#include <board/board.h>
#include <rsd/rsd_sys.h>

sys_gpio_t resetGpio1 =
{
    .unit              = 0,
    .dev               = GEN_GPIO_DEV_ID0_INTERNAL,   
    .pin               = 1,                
    .pinConf = {
        .direction     = GPIO_DIR_IN,         
    },
    .active            = GPIO_ACTIVE_HIGH,
};

sys_gpio_t resetGpio23 =
{
    .unit              = 0,
    .dev               = GEN_GPIO_DEV_ID0_INTERNAL,   
    .pin               = 23,                
    .pinConf = {
        .direction     = GPIO_DIR_IN,         
    },
    .active            = GPIO_ACTIVE_LOW,
};

sys_gpio_t resetGpio6 =
{
    .unit              = 0,
    .dev               = GEN_GPIO_DEV_ID0_INTERNAL,   
    .pin               = 6,                   
    .pinConf = {
        .direction     = GPIO_DIR_IN,         
    },
    .active            = GPIO_ACTIVE_LOW,
};

sys_gpio_t resetGpio7 =
{
    .unit              = 0,
    .dev               = GEN_GPIO_DEV_ID0_INTERNAL,   
    .pin               = 7,                
    .pinConf = {
        .direction     = GPIO_DIR_IN,         
    },
    .active            = GPIO_ACTIVE_LOW,
};

sys_gpio_t resetGpio19 =
{
    .unit              = 0,
    .dev               = GEN_GPIO_DEV_ID0_INTERNAL,   
    .pin               = 19,                   
    .pinConf = {
        .direction     = GPIO_DIR_IN,         
    },
    .active            = GPIO_ACTIVE_LOW,
};

sys_gpio_t resetextGpio33 =
{
    .unit              = 0,
    .dev               = GEN_GPIO_DEV_ID1,   
    .pin               = 33,                   
    .pinConf = {
        .direction     = GPIO_DIR_IN,         
    },
    .active            = GPIO_ACTIVE_LOW,
};

sys_gpio_t resetextGpio4 =
{
    .unit              = 0,
    .dev               = GEN_GPIO_DEV_ID1,   
    .pin               = 4,                   
    .pinConf = {
        .direction     = GPIO_DIR_IN,         
    },
    .active            = GPIO_ACTIVE_LOW,
};

sys_button_reset_appConf_t hold5sResetApp =
{
    .timer = DEF_START_DELAY_TIMES,
    .action = APP_ACT_TYPE_RESTORE_FACTORY,
};

#ifdef CONFIG_SYS_RTL8380
sys_button_reset_conf_t buttonConfSys_8382_24_4f =
{
    .pGpio = &resetGpio1,
    .pushhold_state = ENABLED,
    .pushhold_timer = 5,
    .pushhold_action = SYS_PUSH_HOLD_ACT,
    .appNum = 0,
    .pApp = &hold5sResetApp,
};
sys_button_reset_conf_t buttonConfSys_8382_24_4f_db =
{
    .pGpio = &resetextGpio33,
    .pushhold_state = ENABLED,
    .pushhold_timer = 3,
    .pushhold_action = SYS_PUSH_HOLD_ACT,
    .appNum = 0,
    .pApp = &hold5sResetApp,
};

const sys_button_conf_t buttonConf_8382_24_4f[] =
{
#ifdef CONFIG_SYS_DB_BOARD
    {
        .type = BOTTON_CTL_TYPE_GPIO,
        .pBtnConf = &buttonConfSys_8382_24_4f_db,
    },
#else
    {
        .type = BOTTON_CTL_TYPE_GPIO,
        .pBtnConf = &buttonConfSys_8382_24_4f,
    },
#endif
    {
        .type = BOTTON_CTL_TYPE_END,
        .pBtnConf = NULL,
    },
};
#endif

#ifdef CONFIG_SYS_RTL8390
sys_button_reset_conf_t buttonConfSys_8393_48_4f =
{
    .pGpio = &resetGpio19,
    .pushhold_state = ENABLED,
    .pushhold_timer = 5,
    .pushhold_action = SYS_PUSH_HOLD_ACT,
    .appNum = 0,
    .pApp = &hold5sResetApp,
};
const sys_button_conf_t buttonConf_8393_48_4f[] =
{
    {
        .type = BOTTON_CTL_TYPE_GPIO,
        .pBtnConf = &buttonConfSys_8393_48_4f,
    },
    {
        .type = BOTTON_CTL_TYPE_END,
        .pBtnConf = NULL,
    },
};
#endif

#ifdef CONFIG_SYS_RTL9310
sys_button_reset_conf_t buttonConfSys_9311_48_4f =
{
    .pGpio = &resetGpio19,
    .pushhold_state = ENABLED,
    .pushhold_timer = 5,
    .pushhold_action = SYS_PUSH_HOLD_ACT,
    .appNum = 0,
    .pApp = &hold5sResetApp,
};
sys_button_reset_conf_t buttonConfSys_9311_48_6_yfd =
{
    .pGpio = &resetextGpio4,
    .pushhold_state = ENABLED,
    .pushhold_timer = 5,
    .pushhold_action = SYS_PUSH_HOLD_ACT,
    .appNum = 0,
    .pApp = &hold5sResetApp,
};
sys_button_reset_conf_t buttonConfSys_9311_48_4f_hr =
{
    .pGpio = &resetGpio6,
    .pushhold_state = ENABLED,
    .pushhold_timer = 5,
    .pushhold_action = SYS_PUSH_HOLD_ACT,
    .appNum = 0,
    .pApp = &hold5sResetApp,
};

const sys_button_conf_t buttonConf_9311_48_4f[] =
{
    {
        .type = BOTTON_CTL_TYPE_GPIO,
        .pBtnConf = &buttonConfSys_9311_48_6_yfd,
    },
    {
        .type = BOTTON_CTL_TYPE_END,
        .pBtnConf = NULL,
    },
};
#endif

#ifdef CONFIG_SYS_RTL9300
sys_button_reset_conf_t buttonConfSys_9301_24_4f =
{
    .pGpio = &resetGpio6,
    .pushhold_state = ENABLED,
    .pushhold_timer = 5,
    .pushhold_action = SYS_PUSH_HOLD_ACT,
    .appNum = 0,
    .pApp = &hold5sResetApp,
};
const sys_button_conf_t buttonConf_9301_24_4f[] =
{
    {
        .type = BOTTON_CTL_TYPE_GPIO,
        .pBtnConf = &buttonConfSys_9301_24_4f,
    },
    {
        .type = BOTTON_CTL_TYPE_END,
        .pBtnConf = NULL,
    },
};
#endif

