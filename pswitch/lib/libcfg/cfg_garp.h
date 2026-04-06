
#ifndef __SAL_CFG_GARP_H__
#define __SAL_CFG_GARP_H__

#include <libsal/sal_garp.h>

typedef struct cfg_gmrp_s
{
    uint8   enable;
    uint8   portEnable[SYS_LOGICPORT_NUM_MAX];
    uint8   regMode[SYS_LOGICPORT_NUM_MAX];
    uint8   forbidVlan[SYS_LOGICPORT_NUM_MAX];
    uint32  timer[GVRP_TIMER_END];

} cfg_gmrp_t;

extern int32 cfg_gmrp_enable_set(sys_enable_t enable);

extern int32 cfg_gmrp_enable_get(sys_enable_t *pEnable);

extern int32 cfg_gmrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_gmrp_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);

#endif 

