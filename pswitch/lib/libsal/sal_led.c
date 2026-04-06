
#include <libsal/sal_led.h>

int32 sal_led_switchEnable_set(sys_led_type_t type, sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_LEDTYPE_INVALID(type), SYS_ERR_INPUT);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_led_switchEnable_set(type, enable));

    return SYS_ERR_OK;
}

int32 sal_led_swctrl_set(sys_led_type_t type, sys_led_swCtrlAct_t action)
{
    SYS_PARAM_CHK(IS_LEDTYPE_INVALID(type), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_led_swctrl_set(type, action));

    return SYS_ERR_OK;
}

int32 sal_led_systemCtrl_set(sys_led_state_t state)
{
    SYS_PARAM_CHK(IS_LEDSTATE_INVALID(state), SYS_ERR_INPUT);

    SYS_ERR_CHK(rsd_led_systemCtrl_set(state));

    return SYS_ERR_OK;
}

