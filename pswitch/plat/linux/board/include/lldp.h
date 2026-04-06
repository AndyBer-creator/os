
#ifndef __BOARD_POE_LLDP_H__
#define __BOARD_POE_LLDP_H__

#include <common/sys_def.h>
#include <lldp/proto_lldp.h>

typedef struct lldp_poe_ops_s
{
    int32 (*lldp_poePortL2Data_get)(sys_logic_port_t, lldp_poe_portL2Data_t *);
    int32 (*lldp_poePortL2Data_set)(sys_logic_port_t, lldp_poe_portL2Data_t);
} lldp_poe_ops_t;

extern int32 board_lldp_poePortL2Data_get(sys_logic_port_t port, lldp_poe_portL2Data_t *pLayer2);

extern int32 board_lldp_poePortL2Data_set(sys_logic_port_t port, lldp_poe_portL2Data_t layer2);

extern int32 board_lldp_poe_register(lldp_poe_ops_t *pOps);

#endif 

