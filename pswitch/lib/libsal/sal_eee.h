
#ifndef __SAL_EEE_H__
#define __SAL_EEE_H__

#include <rsd/rsd_eee.h>

extern int32 sal_eee_portEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_eee_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_eee_portState_get(sys_logic_port_t port, sys_enable_t *pState);

extern int32 sal_eee_enablePortmask_set(sys_enable_t enable, sys_logic_portmask_t *pPortmask);

extern int32 sal_eee_enablePortmask_get(sys_enable_t enable, sys_logic_portmask_t *pPortmask);

#endif 
