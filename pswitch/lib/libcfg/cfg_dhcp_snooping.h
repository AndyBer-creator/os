
#ifndef __CFG_DHCP_SNOOPING_H__
#define __CFG_DHCP_SNOOPING_H__

#include <libsal/sal_dhcp_snooping.h>

typedef struct cfg_dhcp_snooping_s
{
    sys_vlanmask_t enabledVLAN;
    sys_enable_t enable;
    sys_enable_t trust[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t chaddrChk[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t dhcpOption82Enable[SYS_LOGICPORT_NUM_MAX];
    sys_dhcp_option82_act_t dhcpOption82Action[SYS_LOGICPORT_NUM_MAX];
    sys_dhcp_option82_id_t dhcpOption82RemoteID;
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    sys_dhcp_option82_circuit_id_t dhcpOption82CircuitID[SYS_LOGICPORT_NUM_MAX][SYS_DHCP_O82_CID_VLAN_MAX+1];
    sys_dhcp_option82_format_t dhcpOption82Format[SYS_LOGICPORT_NUM_MAX];
#endif
    uint32 dhcpLimitRate[SYS_LOGICPORT_NUM_MAX];
} cfg_dhcp_snooping_t;

extern int32 cfg_dhcp_snooping_enable_set(sys_enable_t enable);

extern int32 cfg_dhcp_snooping_enable_get(sys_enable_t *pEnable);

extern int32 cfg_dhcp_snooping_vlanmask_enable_set (sys_vlanmask_t *vlanmask);

extern int32 cfg_dhcp_snooping_vlanmask_enable_get (sys_vlanmask_t *vlanmask);

extern int32 cfg_dhcp_snooping_port_trust_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_dhcp_snooping_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_dhcp_snooping_port_chaddrChk_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_dhcp_snooping_port_chaddrChk_get(sys_logic_port_t port, sys_enable_t *pEnable);

 extern int32 cfg_dhcp_snooping_rateLimit_get(sys_logic_port_t port, uint32 *pRateValue);

 extern int32 cfg_dhcp_snooping_rateLimit_set(sys_logic_port_t port, uint32 rate);

extern int32 cfg_dhcp_option82_enable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_dhcp_option82_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_dhcp_option82_action_set(sys_logic_port_t port, sys_dhcp_option82_act_t action);

extern int32 cfg_dhcp_option82_action_get(sys_logic_port_t port, sys_dhcp_option82_act_t *pAction);

extern int32 cfg_dhcp_option82_remoteID_set(sys_dhcp_option82_id_t *pID);

extern int32 cfg_dhcp_option82_remoteID_get(sys_dhcp_option82_id_t *pID);

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID

extern int32 cfg_dhcp_option82_format_set(sys_logic_port_t port, sys_dhcp_option82_format_t format);

extern int32 cfg_dhcp_option82_format_get(sys_logic_port_t port, sys_dhcp_option82_format_t *pFormat);

extern int32 cfg_dhcp_option82_circuitID_set(sys_logic_port_t port, sys_dhcp_option82_circuit_id_t *pID);

extern int32 cfg_dhcp_option82_circuitID_get(sys_logic_port_t port, sys_dhcp_option82_circuit_id_t *pID);
#endif
#endif 

