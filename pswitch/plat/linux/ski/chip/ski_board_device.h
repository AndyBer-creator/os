
#ifndef __SKI_BOARD_DEVICE_H__
#define __SKI_BOARD_DEVICE_H__

#include <ski/ski_core.h>

#define MAX_WORD_NO_LONG_INFO 4

typedef enum ski_board_dev_cmd_e
{
    
    SKI_BOARD_DEV_SET_BASE = (SKI_CORE_MOD_BOARD_DEV * SKI_CORE_MODCMD_TOTAL),
	SKI_BOARD_FAN_CMD_SET_SPEED,

    SKI_BOARD_DEV_GET_BASE = (SKI_BOARD_DEV_SET_BASE + SKI_CORE_MODCMD_RANGE),
	SKI_BOARD_FAN_CMD_GET_STATUS,
	SKI_BOARD_TEMP_CMD_GET_TEMP,

    SKI_BOARD_DEV_CMD_END
} ski_board_dev_cmd_t;

typedef struct ski_fan_s
{
    uint32            fanSpeed;
    uint32            fanStatus;
} ski_fan_t;

typedef union ski_board_dev_ioctl_u
{
	ski_fan_t         fan;
	char              string[42];
}ski_board_dev_ioctl_t;

extern int32 ski_board_device_init(void);

#endif  

