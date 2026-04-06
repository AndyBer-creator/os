
#include <board/board.h>

sys_gpio_t watchDogGpio1 = 
{
    .unit              = 0,
    .dev               = GEN_GPIO_DEV_ID0_INTERNAL,   
    .pin               = 1,                   
    .pinConf = {
        .direction     = GPIO_DIR_OUT,         
    },
    .active            = GPIO_ACTIVE_LOW,
};

sys_watchdog_goio_conf_t watchDogCfg =
{
	.pState =   NULL,
	.pFeed = &watchDogGpio1,
	.feedTime = 0,
};

const sys_watchdog_conf_t watchdogConf =
{
    .type = WATCHDOG_TYPE_SWITCH,
    .pConf = NULL,
};

const sys_watchdog_conf_t watchdogConf_zq =
{
    .type = WATCHDOG_TYPE_GPIO,
    .chip_type = WATCHDOG_CHIP_TYPE_ADM706SAR,
    .pConf = &watchDogCfg,
};

