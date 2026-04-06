
#ifndef __INTF_GPIO_H__
#define __INTF_GPIO_H__

#include <drv/gpio/generalCtrl_gpio.h>

typedef osal_isrret_t (*gpio_isr_cb)(void *pParam);

typedef enum sys_gpio_active_e
{
    GPIO_ACTIVE_NONE = -1,
    GPIO_ACTIVE_LOW = 0,
    GPIO_ACTIVE_HIGH,
    GPIO_ACTIVE_END
} sys_gpio_active_t;

typedef enum sys_gpio_dflt_e
{
    GPIO_DFLT_LOW = 0,
    GPIO_DFLT_HIGH = 1,
    GPIO_DFLT_END
} sys_gpio_dflt_t;

typedef struct sys_gpio_s
{
    uint32                        unit;
    drv_generalCtrlGpio_devId_t   dev;
    uint32                        pin;
    drv_generalCtrlGpio_pinConf_t pinConf;
    sys_gpio_active_t             active;
} sys_gpio_t;

extern int32 gpio_init(sys_gpio_t *pGpio);

extern int32 gpio_data_get(sys_gpio_t *pGpio, uint32 *pData);

extern int32 gpio_data_set(sys_gpio_t *pGpio, uint32 data);

extern int32 gpio_isr_register(sys_gpio_t *pGpio, gpio_isr_cb isrCb);

extern int32 gpio_isr_get(sys_gpio_t *pGpio, uint32 *pData);

extern int32 gpio_isr_clear(sys_gpio_t *pGpio);

#endif 

