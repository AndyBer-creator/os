
#ifndef __RSD_POE_H__
#define __RSD_POE_H__

#include <common/sys_def.h>

typedef enum sys_poe_cmd_e
{
    POE_CMD_GET = 0,
    POE_CMD_GET_PORT_ENABLE,
    POE_CMD_GET_PORT_ABILITY,
    POE_CMD_GET_PORT_PWGOOD,
    POE_CMD_GET_PORT_STATUS,
    POE_CMD_GET_PORT_CURRENT,
    POE_CMD_GET_PORT_VOLTAGE,
    POE_CMD_GET_PORT_ICUT,
    POE_CMD_GET_DEV_POWER_STATUS,
    POE_CMD_GET_DEV_TEMPERATURE,
    POE_CMD_GET_PORT_DOG,

    POE_CMD_SET_PORT_ENABLE,
    POE_CMD_SET_PORT_ICUT,
    POE_CMD_SET_PORT_SCHEDULE,
    POE_CMD_SET_TIMEZONE,
    POE_CMD_SET_PORT_DOG,
    POE_CMD_SET_TIME_OPEN,
    POE_CMD_SET_TIME_CLOSE,
    POE_CMD_END,
}sys_poe_cmd_t;

typedef uint32 sys_poe_data_t;

typedef struct sys_port_poe_data_s
{
	sys_logic_port_t port;
	sys_poe_data_t value;
} sys_port_poe_data_t;

extern int32 rsd_poe_data_get(sys_logic_port_t port, sys_poe_cmd_t cmd, sys_port_poe_data_t *pData);
extern int32 rsd_poe_data_set(sys_logic_port_t port, sys_poe_cmd_t cmd, sys_port_poe_data_t *pData);

#endif 

