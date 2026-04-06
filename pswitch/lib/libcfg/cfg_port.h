
#ifndef __CFG_PORT_H__
#define __CFG_PORT_H__

#include <libsal/sal_port.h>

typedef enum port_rate_type_s
{
	TYPE_IN_PKT = 1,
	TYPE_IN_BYTE,
	TYPE_OUT_PKT,
	TYPE_OUT_BYTE
}port_rate_type_t;

typedef struct cfg_port_s
{
    
    sys_enable_t enable[SYS_LOGICPORT_NUM_MAX];

    uint32 fc[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t fiberAutoNegoEbl[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t fcAuto[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t backPressure[SYS_LOGICPORT_NUM_MAX];
    sys_port_speed_t speed[SYS_LOGICPORT_NUM_MAX];
    sys_port_duplex_t duplex[SYS_LOGICPORT_NUM_MAX];
    sys_port_speed_t fiberSpeed[SYS_LOGICPORT_NUM_MAX];

#ifdef CONFIG_SYS_PORT_ISOLATION
    sys_enable_t pretected[SYS_LOGICPORT_NUM_MAX];
#endif

#ifdef CONFIG_SYS_PORT_DESCRIPTION
    char description[SYS_LOGICPORT_NUM_MAX][CAPA_PORTDESC_STR_LEN+1];
#endif
	uint64 last5inRatePkt[SYS_LOGICPORT_NUM_MAX];
	uint64 last5inRatebyte[SYS_LOGICPORT_NUM_MAX];
	uint64 last5OutRatePkt[SYS_LOGICPORT_NUM_MAX];
	uint64 last5OutRatebyte[SYS_LOGICPORT_NUM_MAX];
} cfg_port_t;

extern int32 cfg_port_enable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_port_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_port_fiberAutoNegoEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_port_fiberAutoNegoEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_port_speed_set(sys_logic_port_t port, sys_port_speed_t speed);

extern int32 cfg_port_speed_get(sys_logic_port_t port, sys_port_speed_t *pSpeed);

extern int32 cfg_port_duplex_set(sys_logic_port_t port, sys_port_duplex_t duplex);

extern int32 cfg_port_duplex_get(sys_logic_port_t port, sys_port_duplex_t *pDuplex);

extern int32 cfg_port_fiberSpeed_set(sys_logic_port_t port, sys_port_speed_t speed);

extern int32 cfg_port_fiberSpeed_get(sys_logic_port_t port, sys_port_speed_t *pSpeed);

extern int32 cfg_port_flowCtrlAuto_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_port_flowCtrlAuto_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_port_flowCtrl_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_port_flowCtrl_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_port_backpressureEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_port_backpressureEnable_set(sys_logic_port_t port, sys_enable_t enable);

#ifdef CONFIG_SYS_PORT_ISOLATION

extern int32 cfg_port_protected_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_port_protected_get(sys_logic_port_t port, sys_enable_t *pEnable);

#endif

#ifdef CONFIG_SYS_PORT_DESCRIPTION

extern int32 cfg_port_descp_set(sys_logic_port_t port, char *pDescp);

extern int32 cfg_port_descp_get(sys_logic_port_t port, char *pDescp);
#endif
extern int32 cfg_port_in_pkt_rate_set(sys_logic_port_t port, uint32 type, uint64 value);
extern int32 cfg_port_in_pkt_rate_get(sys_logic_port_t port, uint32 type, uint64 *value);
#endif

