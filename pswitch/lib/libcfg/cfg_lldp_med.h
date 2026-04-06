
#ifndef __CFG_LLDP_MED_H__
#define __CFG_LLDP_MED_H__

#include <libsal/sal_lldp_med.h>

typedef struct cfg_lldp_med_s
{
    uint8                         fastRepeatCount;
    lldp_med_networkPolicy_t      policy[CAPA_LLDP_MED_NETWORK_POLICY_NUM];

    sys_enable_t                  networkPolicyAuto;
    lldp_med_tlv_sel_t            lldpMedTlvSel[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t                  medEnable[SYS_LOGICPORT_NUM_MAX];
    lldp_med_networkPolicy_bind_t policyBind[SYS_LOGICPORT_NUM_MAX];
    lldp_med_location_t           location[SYS_LOGICPORT_NUM_MAX];
} cfg_lldp_med_t;

extern int32 cfg_lldp_medFastRepeatCount_set(uint8 fastRepeatCount);

extern int32 cfg_lldp_medFastRepeatCount_get(uint8 *pFastRepeatCount);

extern int32 cfg_lldp_medNetworkPolicy_set(uint32 idx, lldp_med_networkPolicy_t policy);

extern int32 cfg_lldp_medNetworkPolicy_get(uint32 idx, lldp_med_networkPolicy_t *pPolicy);

extern int32 cfg_lldp_medNetworkPolicyAuto_set(sys_enable_t enable);

extern int32 cfg_lldp_medNetworkPolicyAuto_get(sys_enable_t *pEnable);

extern int32 cfg_lldp_portMedTlvSel_set(sys_logic_port_t port, lldp_med_tlv_sel_t lldpMedTlvSel);

extern int32 cfg_lldp_portMedTlvSel_get(sys_logic_port_t port, lldp_med_tlv_sel_t *pLldpMedTlvSel);

extern int32 cfg_lldp_portMedEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_lldp_portMedEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_lldp_portMedNetworkPolicyBind_set(sys_logic_port_t port, lldp_med_networkPolicy_bind_t policyBind);

extern int32 cfg_lldp_portMedNetworkPolicyBind_get(sys_logic_port_t port, lldp_med_networkPolicy_bind_t *pPolicyBind);

extern int32 cfg_lldp_portMedLocation_set(sys_logic_port_t port, lldp_med_location_t location);

extern int32 cfg_lldp_portMedLocation_get(sys_logic_port_t port, lldp_med_location_t *pLocation);

#endif 

