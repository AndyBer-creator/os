
#ifndef __SAL_CABLEDIAG_H__
#define __SAL_CABLEDIAG_H__

#include <libsal/sal_util.h>
#include <rsd/rsd_cablediag.h>

extern int32 sal_cablediag_copper_get(sys_logic_port_t logicPort, sys_rtctResult_t *pRtctResult);

extern int32 sal_cablediag_copper_set(sys_logic_port_t logicPort);

#endif 

