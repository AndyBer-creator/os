
#ifndef __INTF_UART_H__
#define __INTF_UART_H__

#include <drv/uart/uart.h>

extern int32 uart1_dev_init(void);
extern int32 uart1_baudrate_get(uint32 *pData);
extern int32 uart1_baudrate_set(uint32 data);
extern int32 uart1_interface_set(uint32 data);
extern int32 uart1_getc(uint8 *pData, uint32 timeout);
extern int32 uart1_gets(uint8 *pData, uint32* pLength, uint32 timeout);
extern int32 uart1_putc(uint8 data);
extern int32 uart1_puts(uint8* pData, uint32 length);
#endif 

