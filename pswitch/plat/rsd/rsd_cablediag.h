
#ifndef __RSD_CABLEDIAG_H__
#define __RSD_CABLEDIAG_H__

#include <rtk/port.h>
#include <common/sys_type.h>
#include <rsd/rsd_sys.h>
#include <rtk/diag.h>

typedef rtk_rtctResult_t sys_rtctResult_t;

extern int32 rsd_cablediag_copper_get(sys_logic_port_t logicPort, sys_rtctResult_t *pRtctResult);

extern int32 rsd_cablediag_copper_set(sys_logic_port_t logicPort);

#endif 

