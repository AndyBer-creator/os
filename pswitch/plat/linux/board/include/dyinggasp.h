
#ifndef __DYINGGASP_H__
#define __DYINGGASP_H__

#include <board/include/intf.h>
#include <board/include/gpio.h>
#include <board/include/reg.h>

typedef enum sys_dyinggasp_type_e
{
    DYINGGASP_TYPE_GPIO = 0,
    DYINGGASP_TYPE_END,
} sys_dyinggasp_type_t;

typedef union sys_dyinggasp_ctrlConf_u
{
    sys_gpio_t gpio;
} sys_dyinggasp_ctrlConf_t;

typedef struct sys_dyinggasp_conf_s
{
    sys_dyinggasp_type_t type;
    void                 *pConf;
} sys_dyinggasp_conf_t;

extern int32 board_dyinggasp_init(void);

#endif 

