
#ifndef __SAL_CFG_GVRP_H__
#define __SAL_CFG_GVRP_H__

#include <libsal/sal_gvrp.h>

typedef struct cfg_gvrp_s
{
    uint8   enable;
    uint8   portEnable[SYS_LOGICPORT_NUM_MAX];
    uint8   regMode[SYS_LOGICPORT_NUM_MAX];
    uint8   forbidVlan[SYS_LOGICPORT_NUM_MAX];
    uint32  timer[GVRP_TIMER_END];

} cfg_gvrp_t;

extern int32 cfg_gvrp_enable_set(sys_enable_t enable);

extern int32 cfg_gvrp_enable_get(sys_enable_t *pEnable);

extern int32 cfg_gvrp_timer_set(gvrp_timer_type_t  timer_type, uint32 timeout);

extern int32 cfg_gvrp_timer_get(gvrp_timer_type_t  timer_type, uint32 *pTimeout);

extern int32 cfg_gvrp_port_enable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_gvrp_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_gvrp_port_regmode_set(sys_logic_port_t port, register_mode_t  regmode);

extern int32 cfg_gvrp_port_regmode_get(sys_logic_port_t port, register_mode_t *pRegMode);

extern int32 cfg_gvrp_port_forbidVlan_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_gvrp_port_forbidVlan_get(sys_logic_port_t port, sys_enable_t *pEnable);

#endif 

