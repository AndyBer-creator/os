
#ifndef __RSD_FIBER_H__
#define __RSD_FIBER_H__

#include <common/sys_def.h>

extern int32 rsd_fiber_oeStatus_get(sys_logic_port_t logicPort, sys_fiber_detect_status_t *pData);

extern int32 rsd_fiber_losStatus_get(sys_logic_port_t logicPort, sys_fiber_detect_status_t *pData);

extern int32 rsd_fiber_detectStatus_get(sys_logic_port_t logicPort, sys_fiber_detect_status_t *pData);

#endif 

