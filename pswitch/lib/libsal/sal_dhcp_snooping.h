
#ifndef __SAL_DHCP_SNOOPING_H__
#define __SAL_DHCP_SNOOPING_H__

#include <dhcp/proto_dhcp.h>

extern int32 sal_dhcp_snooping_enable_set(sys_enable_t enable);

extern int32 sal_dhcp_snooping_enable_get(sys_enable_t *pEnable);

extern int32 sal_dhcp_snooping_vlan_enable_set (sys_vid_t vid, sys_enable_t enable);

extern int32 sal_dhcp_snooping_vlan_enable_get (sys_vid_t vid, sys_enable_t *pEnable);

extern int32 sal_dhcp_snooping_vlanmask_enable_get (sys_vlanmask_t *vlanmask);

extern int32 sal_dhcp_snooping_port_trust_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dhcp_snooping_port_trust_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dhcp_snooping_portChaddrChk_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dhcp_snooping_portChaddrChk_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dhcp_snooping_portMib_get(sys_logic_port_t port, sys_dhcp_mib_t *pCounter);

extern int32 sal_dhcp_snooping_portMib_clear(sys_logic_port_t port);

extern int32 sal_dhcp_snooping_rateLimit_get(sys_logic_port_t port, uint32 *pRate);

extern int32 sal_dhcp_snooping_rateLimit_set(sys_logic_port_t port, uint32 rate);

extern int32 sal_dhcp_option82_enable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_dhcp_option82_enable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_dhcp_option82_action_set(sys_logic_port_t port, sys_dhcp_option82_act_t action);

extern int32 sal_dhcp_option82_action_get(sys_logic_port_t port, sys_dhcp_option82_act_t *pAction);

extern int32 sal_dhcp_option82_remoteID_set(sys_dhcp_option82_id_t *pID);

extern int32 sal_dhcp_option82_remoteID_get(sys_dhcp_option82_id_t *pID);

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID

extern int32 sal_dhcp_option82_format_set(sys_logic_port_t port, sys_dhcp_option82_format_t format);

extern int32 sal_dhcp_option82_format_get(sys_logic_port_t port, sys_dhcp_option82_format_t *pFormat);

extern int32 sal_dhcp_option82_circuitID_set(sys_logic_port_t port,  sys_dhcp_option82_circuit_id_t *pCircuitRemoteID);

extern int32 sal_dhcp_option82_circuitID_get(sys_logic_port_t port,  sys_dhcp_option82_circuit_id_t *pCircuitRemoteID);
#endif

#endif 

