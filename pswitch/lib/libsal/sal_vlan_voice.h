
#ifndef __SAL_VLAN_VOICE_H__
#define __SAL_VLAN_VOICE_H__

#include <voice_vlan/proto_voice_vlan.h>

typedef struct sal_voiceVanOuiCfg_s
{
    sys_vlan_voiceVlan_oui_t oui;
    sys_namestr_t ouiName;
}sal_voiceVanOuiCfg_t;

extern int32 sal_vlan_voiceVid_set(sys_vid_t vid);

extern int32 sal_vlan_voiceVid_get(sys_vid_t *pVid);

extern int32 sal_vlan_voiceVlanState_get(voice_vlan_state_t *pState);

extern int32 sal_vlan_voiceVlanState_set(voice_vlan_state_t state);

extern int32 sal_vlan_voiceVlanAging_get(uint32 *pAging);

extern int32 sal_vlan_voiceVlanAging_set(uint32 aging);

extern int32 sal_vlan_portVoiceVlanEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32 sal_vlan_portVoiceVlanEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32 sal_vlan_portVoiceVlanMode_get(
    sys_logic_port_t port,
    voice_vlan_port_mode_t *pMode);

extern int32 sal_vlan_portVoiceVlanMode_set(
    sys_logic_port_t port,
    voice_vlan_port_mode_t mode);

extern int32 sal_vlan_voiceVlanQos_get(sys_vlan_voiceQos_t *pVoiceQos);

extern int32 sal_vlan_voiceVlanQos_set(sys_vlan_voiceQos_t voiceQos);

extern int32 sal_vlan_portVoiceVlancosMode_get(sys_logic_port_t port,voice_vlan_cos_mode_t *pCosMode);

extern int32 sal_vlan_portVoiceVlancosMode_set(sys_logic_port_t port,voice_vlan_cos_mode_t cosMode);

extern int32 sal_vlan_voiceVlan1pRemarkEnable_get(sys_enable_t *pEnable);

extern int32 sal_vlan_voiceVlan1pRemarkEnable_set(sys_enable_t enable);

extern int32 sal_vlan_voiceVlanOui_add(
    sys_vlan_voiceVlan_oui_t *pOui,
    sys_namestr_t            *pName);

extern int32 sal_vlan_voiceVlanOui_get(
    uint32 entryIdx,
    sys_vlan_voiceVlan_oui_t *pOui,
    sys_namestr_t *pName);

extern int32 sal_vlan_voiceVlanOui_set(
    uint32 entryIdx,
    sys_vlan_voiceVlan_oui_t *pOui,
    sys_namestr_t *pName);

extern int32 sal_vlan_voiceVlanOui_del(sys_vlan_voiceVlan_oui_t *pOui);

extern int32 sal_vlan_voiceVlanOui_fdsEntry_set(void);

extern int32 sal_vlan_portVoiceStatus_chk(sys_logic_port_t port);
extern int32 sal_vlan_portVoiceVlanActive_get(sys_logic_port_t port,sys_enable_t *pEnable);

#endif

