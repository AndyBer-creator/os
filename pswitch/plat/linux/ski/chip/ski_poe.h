
#ifndef __SKI_POE_H__
#define __SKI_POE_H__

#include <ski/ski_core.h>
#include <rsd/rsd_poe.h>

#define MAX_WORD_NO_LONG_INFO 4

typedef enum ski_poe_cmd_e
{
    
    SKI_POE_SET_BASE = (SKI_CORE_MOD_POE * SKI_CORE_MODCMD_TOTAL),
	SKI_POE_CMD_SET_PORT_ENABLE,
	SKI_POE_CMD_SET_PORT_ICUT,
	SKI_POE_CMD_SET_PORT_SCHEDULE,
	SKI_POE_CMD_SET_TIMEZONE,
	SKI_POE_CMD_SET_PORT_WATCH_DOG,
	SKI_POE_CMD_SET_PORT_TIME_OPEN,
	SKI_POE_CMD_SET_PORT_TIME_CLOSE,

    SKI_POE_GET_BASE = (SKI_POE_SET_BASE + SKI_CORE_MODCMD_RANGE),
	SKI_POE_CMD_GET_PORT_ENABLE,
	SKI_POE_CMD_GET_PORT_ABILITY,
    SKI_POE_CMD_GET_PORT_PWGOOD,
    SKI_POE_CMD_GET_PORT_STATUS,
    SKI_POE_CMD_GET_PORT_CURRENT,
    SKI_POE_CMD_GET_PORT_VOLTAGE,
    SKI_POE_CMD_GET_PORT_ICUT,
    SKI_POE_CMD_GET_DEV_POWER_STATUS,
    SKI_POE_CMD_GET_DEV_TEMPERATURE,
    SKI_POE_CMD_GET_PORT_WATCH_DOG,

    SKI_POE_CMD_END
} ski_poe_cmd_t;

typedef struct ski_poe_ioctl_s
{
	sys_logic_port_t port;
    uint32 portEnable;
    uint32 portStatus;
    uint32 portPower;
    uint32 portType;
    uint32 portLevel;
    uint32 portVolatge;
    uint32 portCurrent;
    uint32 portIcut;
    uint32 devPower;
    uint32 devTemp;
}ski_poe_ioctl_t;

extern int32 ski_poe_init(void);

#endif  

