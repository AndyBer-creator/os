
#ifndef __RSD_BOARD_DEVICE_H__
#define __RSD_BOARD_DEVICE_H__

#include <common/sys_def.h>

#ifdef CONFIG_SYS_BOARD_FAN
extern int32 rsd_sys_fan_set(uint32 fanSpeed);
extern int32 rsd_sys_fan_get(uint32 *fanSpeed, uint32 *alarm);
#endif
#ifdef CONFIG_SYS_BOARD_TEMP
int32 rsd_sys_temp_get(char *pTemp);
#endif
#endif 

