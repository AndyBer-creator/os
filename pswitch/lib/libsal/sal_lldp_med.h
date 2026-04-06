
#ifndef __SAL_LLDP_MED_H__
#define __SAL_LLDP_MED_H__

#include <lldp/proto_lldp.h>

extern int32 sal_lldp_medFastRepeatCount_set(uint8 fastRepeatCount);

extern int32 sal_lldp_medFastRepeatCount_get(uint8 *pFastRepeatCount);

extern int32 sal_lldp_portMedLocalEntryInfo_get(sys_logic_port_t port, display_med_entry_t *pEntry);

extern int32 sal_lldp_portMedRemoteEntryInfo_get(sys_logic_port_t port, uint8 index ,display_med_entry_t *pEntry);

extern int32 sal_lldp_medNetworkPolicy_set(uint32 idx, lldp_med_networkPolicy_t policy);

extern int32 sal_lldp_medNetworkPolicy_get(uint32 idx, lldp_med_networkPolicy_t *pPolicy);

extern int32 sal_lldp_medNetworkPolicyAuto_set(sys_enable_t enable);

extern int32 sal_lldp_medNetworkPolicyAuto_get(sys_enable_t *pEnable);

extern int32 sal_lldp_portMedTlvSel_set(sys_logic_port_t port, lldp_med_tlv_sel_t lldpMedTlvSel);

extern int32 sal_lldp_portMedTlvSel_get(sys_logic_port_t port, lldp_med_tlv_sel_t *pLldpMedTlvSel);

extern int32 sal_lldp_portMedEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_lldp_portMedEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_lldp_portMedNetworkPolicyBind_set(sys_logic_port_t port, lldp_med_networkPolicy_bind_t policyBind);

extern int32 sal_lldp_portMedNetworkPolicyBind_get(sys_logic_port_t port, lldp_med_networkPolicy_bind_t *pPolicyBind);

extern int32 sal_lldp_portMedLocation_set(sys_logic_port_t port, lldp_med_location_t location);

extern int32 sal_lldp_portMedLocation_get(sys_logic_port_t port, lldp_med_location_t *pLocation);

#endif 

