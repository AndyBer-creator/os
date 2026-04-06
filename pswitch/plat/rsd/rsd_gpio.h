
#ifndef __RSD_GPIO_H__
#define __RSD_GPIO_H__

#include <drv/gpio/gpio.h>
#include <common/sys_def.h>

extern int32 rsd_gpio_init(void);

extern int32 rsd_gpio_dataBit_set(uint32 gpio, uint32 gpioData);

extern int32 rsd_gpio_dataBit_get(uint32 gpio, uint32 *pGpioData);

#endif 

