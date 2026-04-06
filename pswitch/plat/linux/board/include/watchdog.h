
#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <drv/watchdog/watchdog.h>

typedef enum sys_watchdog_type_e
{
    WATCHDOG_TYPE_SWITCH = 0,
    WATCHDOG_TYPE_GPIO,
    WATCHDOG_TYPE_END
} sys_watchdog_type_t;

typedef enum sys_watchdog_chip_type_e
{
    WATCHDOG_CHIP_TYPE_COMMON = 0,
    WATCHDOG_CHIP_TYPE_TPS3705,
    WATCHDOG_CHIP_TYPE_ADM706SAR,
    WATCHDOG_CHIP_TYPE_END
} sys_watchdog_chip_type_t;

typedef struct sys_watchdog_goio_conf_s
{
    sys_gpio_t *pState;
    sys_gpio_t *pFeed;
    uint32     feedTime; 
} sys_watchdog_goio_conf_t;

typedef struct sys_watchdog_conf_s
{
    sys_watchdog_type_t type;
    sys_watchdog_chip_type_t chip_type;
    void                *pConf;
} sys_watchdog_conf_t;

extern int32 board_watchdog_init(void);

#endif 

