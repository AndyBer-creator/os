
#ifndef __CFG_VLAN_VOICE_H__
#define __CFG_VLAN_VOICE_H__

#include <libsal/sal_vlan_voice.h>

typedef struct cfg_vlan_voice_s
{
    sys_vid_t vid;
    voice_vlan_state_t state;
    uint32  aging;
    sys_enable_t portEnable[SYS_LOGICPORT_NUM_MAX];
    voice_vlan_port_mode_t portMode[SYS_LOGICPORT_NUM_MAX];
    voice_vlan_cos_mode_t cosMode[SYS_LOGICPORT_NUM_MAX];
    sys_enable_t remarkEnable;
    sys_vlan_voiceQos_t qosInfo;
    sal_voiceVanOuiCfg_t ouiInfo[CAPA_VLAN_VOICE_OUI_NUM];
} cfg_vlan_voice_t;

extern int32 cfg_vlan_voiceVid_set(sys_vid_t vid);

extern int32 cfg_vlan_voiceVid_get(sys_vid_t *pVid);

extern int32
cfg_vlan_voiceVlanState_get(voice_vlan_state_t *pState);

extern int32
cfg_vlan_voiceVlanState_set(voice_vlan_state_t state);

extern int32
cfg_vlan_voiceVlanAging_get(uint32 *pAging);

extern int32
cfg_vlan_voiceVlanAging_set(uint32 aging);

extern int32
cfg_vlan_portVoiceVlanEnable_get(sys_logic_port_t port, sys_enable_t *pEnable);

extern int32
cfg_vlan_portVoiceVlanEnable_set(sys_logic_port_t port, sys_enable_t enable);

extern int32
cfg_vlan_portVoiceVlanMode_get(sys_logic_port_t port, voice_vlan_port_mode_t *pMode);

extern int32
cfg_vlan_portVoiceVlan_mode_set(sys_logic_port_t port, voice_vlan_port_mode_t mode);

extern int32
cfg_vlan_voiceVlanQos_get(sys_vlan_voiceQos_t *pVoiceQos);

extern int32
cfg_vlan_voiceVlanQos_set(sys_vlan_voiceQos_t voiceQos);

extern int32
cfg_vlan_voiceVlancosMode_get(sys_logic_port_t port,voice_vlan_cos_mode_t *pCosMode);

extern int32
cfg_vlan_voiceVlancosMode_set(sys_logic_port_t port,voice_vlan_cos_mode_t cosMode);

extern int32
cfg_vlan_voiceVlan1pRemarkEnable_get(sys_enable_t *pEnable);

extern int32
cfg_vlan_voiceVlan1pRemarkEnable_set(sys_enable_t enable);

extern int32
cfg_vlan_voiceVlanOui_get(uint32 entryIdx, sys_vlan_voiceVlan_oui_t *pOui, sys_namestr_t *pName);

extern int32
cfg_vlan_voiceVlanOui_set(uint32 entryIdx, sys_vlan_voiceVlan_oui_t *pOui, sys_namestr_t *pName);
#endif

