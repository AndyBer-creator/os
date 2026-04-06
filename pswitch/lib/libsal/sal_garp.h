
#ifndef __SAL_GARP_H__
#define __SAL_GARP_H__

#include <libsal/sal_type.h>
#include <garp/garp.h>

extern int32 sal_gmrp_enable_set(sys_enable_t enable);

extern int32 sal_gmrp_enable_get(sys_enable_t *pEnable);

extern int32 sal_gmrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_gmrp_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);

#endif 

