
#ifndef __RSD_SWITCH_H__
#define __RSD_SWITCH_H__

#include <rtk/switch.h>
#include <common/sys_def.h>

extern int32 rsd_switch_maxFrameSize_set(uint32 maxSize);

extern int32 rsd_switch_maxFrameSize_get(uint32 *pMaxSize);

extern int32 rsd_switch_init(void);

#endif 

