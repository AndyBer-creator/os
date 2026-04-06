
#ifndef __RESET_H__
#define __RESET_H__

#include <board/include/intf.h>
#include <board/include/gpio.h>
#include <board/include/reg.h>

typedef enum sys_reset_type_e
{
    RESET_TYPE_DEFAULT = 0,
    RESET_TYPE_REG,
    RESET_TYPE_GPIO,
    RESET_TYPE_END,
} sys_reset_type_t;

typedef union sys_reset_ctrlConf_u
{
    sys_gpio_t *pGpio;
    sys_reg_t reg;
} sys_reset_ctrlConf_t;

typedef struct sys_reset_conf_s
{
    sys_reset_type_t type;
    void             *pConf;
} sys_reset_conf_t;

extern int32 board_reset_init(void);

#endif 

