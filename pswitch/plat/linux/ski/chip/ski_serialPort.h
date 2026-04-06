
#ifndef __SKI_SERIALPORT_H__
#define __SKI_SERIALPORT_H__

#include <ski/ski_core.h>
#include <rsd/rsd_serialPort.h>

#define MAX_WORD_NO_LONG_INFO 4

typedef enum ski_serial_cmd_e
{
    
    SKI_SERIAL_SET_BASE = (SKI_CORE_MOD_SERIAL * SKI_CORE_MODCMD_TOTAL),
	SKI_SERIAL_CMD_SET_PUTC,
	SKI_SERIAL_CMD_SET_PUTS,

    SKI_SERIAL_GET_BASE = (SKI_SERIAL_SET_BASE + SKI_CORE_MODCMD_RANGE),
	SKI_SERIAL_CMD_GET_GETC,
	SKI_SERIAL_CMD_GET_GETS,

    SKI_SERIAL_CMD_END
} ski_serial_cmd_t;

extern int32 ski_serial_init(void);

#endif  

