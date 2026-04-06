
#ifndef __SKI_GPIO_H__
#define __SKI_GPIO_H__

#include <ski/ski_core.h>
#include <rsd/rsd_gpio.h>

typedef enum ski_gpio_cmd_e
{
    
    SKI_GPIO_SET_BASE = (SKI_CORE_MOD_GPIO * SKI_CORE_MODCMD_TOTAL),
    SKI_GPIO_SET_DATA_BIT,

    SKI_GPIO_GET_BASE = (SKI_GPIO_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_GPIO_GET_DATA_BIT,

    SKI_GPIO_CMD_END
} ski_gpio_cmd_t;

typedef union ski_gpio_ioctl_u
{
    uint32 gpio;
    uint32 data;
} ski_gpio_ioctl_t;

extern int32 ski_gpio_init(void);

#endif  

