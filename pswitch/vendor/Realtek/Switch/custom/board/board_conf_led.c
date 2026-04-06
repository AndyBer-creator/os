
#include <board/board.h>

sys_led_conf_ctrl_t ledConfSys =
{
    .ctrlType = LED_CTRL_SWITCH_GLB,
};

sys_led_conf_ctrl_t ledConfAlarm =
{
    .ctrlType = LED_CTRL_SWITCH_GLB,
};

const sys_led_conf_t ledConf[] =
{
    {SYS_LED_TYPE_SYS,   &ledConfSys},
    {SYS_LED_TYPE_ALARM, &ledConfAlarm},
    {SYS_LED_TYPE_END,   NULL}
};

