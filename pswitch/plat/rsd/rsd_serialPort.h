
#ifndef __RSD_SERIALPORT_H__
#define __RSD_SERIALPORT_H__

#include <common/sys_def.h>

#define SYS_SERIAL_MAX_MSG 512

typedef enum sys_serial_cmd_e
{
    SERIAL_CMD_GET = 0,
    SERIAL_CMD_GETC,
    SERIAL_CMD_GETS,

    SERIAL_CMD_SETC,
    SERIAL_CMD_SETS,
    SERIAL_CMD_END,
}sys_serial_cmd_t;

typedef struct sys_serial_data_s
{
	uint8  value[SYS_SERIAL_MAX_MSG];
	uint32 length;
	uint32 timeout;
} sys_serial_data_t;

extern int32 rsd_serialPort_data_get(sys_serial_cmd_t cmd, sys_serial_data_t *pData);
extern int32 rsd_serialPort_data_set(sys_serial_cmd_t cmd, sys_serial_data_t *pData);

#endif 

