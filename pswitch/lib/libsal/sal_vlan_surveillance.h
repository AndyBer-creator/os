
#ifndef __SAL_SURVEILLANCEVLAN_H__
#define __SAL_SURVEILLANCEVLAN_H__

#include <surveillancevlan/proto_surveillancevlan.h>

#define IS_PORT_USE_BY_SURVEILLANCE_VLAN(lp) (SYS_ERR_FAILED == sal_surveillance_vlan_port_chk(lp) ? TRUE : FALSE)

extern int32 sal_surveillance_vlanVid_get(sys_vid_t *pVid);

extern int32 sal_surveillance_vlanVid_set(sys_vid_t vid);

extern int32 sal_surveillance_vlanPri_get(sys_pri_t *pPri);

extern int32 sal_surveillance_vlanPri_set(sys_pri_t pri);

extern int32 sal_surveillance_vlanState_get(sys_enable_t *pState);

extern int32 sal_surveillance_vlanState_set(sys_enable_t state);

extern int32 sal_surveillance_vlanAging_get(uint32 *pAging);

extern int32 sal_surveillance_vlanAging_set(uint32 aging);

extern int32 sal_surveillance_vlanOuibyIdx_get(uint32 idx, surveillance_vlan_oui_t *pOui);

extern int32 sal_surveillance_vlanOuiByIdx_set(uint32 idx, surveillance_vlan_oui_t *pOui);

extern int32 sal_surveillance_vlanOui_add(surveillance_vlan_oui_t *pOui);

extern int32 sal_surveillance_vlanOui_del(surveillance_vlan_oui_t *pOui);

extern int32 sal_surveillance_vlan1pRemarkEnable_get(sys_enable_t *pEnable);

extern int32 sal_surveillance_vlan1pRemarkEnable_set(sys_enable_t enable);

extern int32 sal_surveillance_vlan_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_surveillance_vlan_portEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_surveillance_vlan_portMode_get(sys_logic_port_t port, surveillance_vlan_port_mode_t *pMode);

extern int32 sal_surveillance_vlan_portMode_set(sys_logic_port_t port, surveillance_vlan_port_mode_t mode);

extern int32 sal_surveillance_vlan_portCosMode_get(sys_logic_port_t port, surveillance_vlan_cos_mode_t *pCosMode);

extern int32 sal_surveillance_vlan_portCosMode_set(sys_logic_port_t port, surveillance_vlan_cos_mode_t cosMode);

extern int32 sal_surveillance_vlan_port_chk(sys_logic_port_t port);
#endif

