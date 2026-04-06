
#ifndef __SAL_FIBER_H__
#define __SAL_FIBER_H__

#include <libsal/sal_util.h>
#include <rsd/rsd_fiber.h>

extern int32 sal_fiber_oeStatus_get(sys_logic_port_t logicPort, sys_fiber_detect_status_t *pStatus);

extern int32 sal_fiber_losStatus_get(sys_logic_port_t logicPort, sys_fiber_detect_status_t *pStatus);

extern int32 sal_fiber_detectStatus_get(sys_logic_port_t logicPort, sys_fiber_detect_status_t *pStatus);

#endif 

