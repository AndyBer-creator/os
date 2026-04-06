
#ifndef __CFG_SURVEILLANCE_VLAN_
#define __CFG_SURVEILLANCE_VLAN_

#include <libsal/sal_vlan_surveillance.h>

typedef struct cfg_surveillance_vlan_s
{
    uint32  aging;
    sys_vid_t vid;
    sys_pri_t pri;
    sys_enable_t state;
    sys_enable_t remarkEnable;
    sys_enable_t portEnable[SYS_LOGICPORT_NUM_MAX];
    surveillance_vlan_port_mode_t portMode[SYS_LOGICPORT_NUM_MAX];
    surveillance_vlan_cos_mode_t cosMode[SYS_LOGICPORT_NUM_MAX];
    surveillance_vlan_oui_t ouiInfo[CAPA_VLAN_SURVEILLANCE_OUI_NUM];
} cfg_surveillance_vlan_t;

extern int32 cfg_surveillance_vlanVid_get(sys_vid_t *pVid);

extern int32 cfg_surveillance_vlanVid_set(sys_vid_t vid);

extern int32 cfg_surveillance_vlanPri_get(sys_pri_t *pPri);

extern int32 cfg_surveillance_vlanPri_set(sys_pri_t pri);

extern int32 cfg_surveillance_vlanState_get(sys_enable_t *pState);

extern int32 cfg_surveillance_vlanState_set(sys_enable_t state);

extern int32 cfg_surveillance_vlanAging_get(uint32 *pAging);

extern int32 cfg_surveillance_vlanAging_set(uint32 aging);

extern int32 cfg_surveillance_vlanOui_get(uint32 idx, surveillance_vlan_oui_t *pOui);

extern int32 cfg_surveillance_vlanOui_set(uint32 idx, surveillance_vlan_oui_t *pOui);

extern int32 cfg_surveillance_vlan1pRemarkEnable_get(sys_enable_t *pEnable);

extern int32 cfg_surveillance_vlan1pRemarkEnable_set(sys_enable_t enable);

extern int32 cfg_surveillance_vlan_portEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 cfg_surveillance_vlan_portEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 cfg_surveillance_vlan_portMode_get(sys_logic_port_t port, surveillance_vlan_port_mode_t *pMode);

extern int32 cfg_surveillance_vlan_portMode_set(sys_logic_port_t port, surveillance_vlan_port_mode_t mode);

extern int32 cfg_surveillance_vlan_portCosMode_get(sys_logic_port_t port, surveillance_vlan_cos_mode_t *pCosMode);

extern int32 cfg_surveillance_vlan_portCosMode_set(sys_logic_port_t port, surveillance_vlan_cos_mode_t cosMode);
#endif

