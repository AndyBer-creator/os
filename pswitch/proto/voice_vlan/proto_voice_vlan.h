
#ifndef __PROTO_VOICEVLAN_H__
#define __PROTO_VOICEVLAN_H__
#include <common/sys_error.h>
#include <common/sys_type.h>
#include <common/sys_portview.h>
typedef enum voice_vlan_state_e
{
    VOICE_VLAN_DISABLED = 0,
    VOICE_VLAN_AUTO_ENABLED,
    VOICE_VLAN_OUI_ENABLED,
    VOICE_VLAN_STATE_END
}voice_vlan_state_t;

typedef enum voice_vlan_port_mode_e
{
    VOICE_VLAN_MODE_AUTO = 0,
    VOICE_VLAN_MODE_MANUAL,
    VOICE_VLAN_MODE_END
} voice_vlan_port_mode_t;

typedef enum voice_vlan_cos_mode_e
{
    VOICE_VLAN_COS_SRC = 0,
    VOICE_VLAN_COS_ALL,
    VOICE_VLAN_COS_END
}voice_vlan_cos_mode_t;

typedef struct sys_vlan_voiceQos_s
{
    sys_pri_t  vpt; 
    uint32 dscp;    
    sys_pri_t pri;
} sys_vlan_voiceQos_t;

typedef struct sys_voice_vlan_event_s
{
    sys_vid_t voiceVid;
}sys_voice_vlan_event_t;

typedef struct sys_vlan_voiceVlan_oui_s
{
    uint8 oui[SNAPOUI_LEN];
    uint8 ouiMask[SNAPOUI_LEN];
    sys_enable_t enable;
} sys_vlan_voiceVlan_oui_t;

typedef struct sys_vlan_voice_entry_s
{
    sys_vid_t voiceVid;
    sys_pri_t pri;
    sys_qid_t qid;
    sys_enable_t remark;
    uint8 oui[SNAPOUI_LEN];
    uint8 ouiMask[SNAPOUI_LEN];
    sys_logic_portmask_t copyCpuSpm;
    sys_logic_portmask_t cosSrcPm;
    sys_logic_portmask_t cosAllPm;
} sys_vlan_voice_entry_t;
#define FOR_EACH_VOICE_OUI_ENTRY(entryIdx) \
    for((entryIdx) = 0; (entryIdx) < CAPA_VLAN_VOICE_OUI_NUM; (entryIdx)++)

#define VOICEVLAN_RMK_ENTRY_START    (2*(CAPA_VLAN_VOICE_OUI_NUM-1) +3)   

#define IS_VOICEVLAN_ENTRY_INVALID(entryIdx)    \
    ((((entryIdx) < 0) || ((entryIdx) >= CAPA_VLAN_VOICE_OUI_NUM)) ? 1 : 0)

#define VV_DBG(fmt, args...) SYS_DBG(DBG_VLAN_VOICE, fmt, ##args)
extern int32 voice_vlan_init(void);
extern int32 voice_vlan_exit(void);
extern int32 voice_vlan_state_set(voice_vlan_state_t state);
extern int32 voice_vlan_state_get(voice_vlan_state_t *pState);
extern int32 voice_vlan_aging_set(uint32 aging);
extern int32 voice_vlan_aging_get(uint32 *pAging);
extern int32 voice_vlan_oui_get(uint32 entryIdx, sys_vlan_voiceVlan_oui_t *pOui);
extern int32 voice_vlan_oui_set(uint32 entryIdx, sys_vlan_voiceVlan_oui_t *pOui);
extern int32 voice_vlan_port_enable_set(sys_logic_port_t portId, sys_enable_t enable);
extern int32 voice_vlan_port_enable_get(sys_logic_port_t portId, sys_enable_t *pEnable);
extern int32 voice_vlan_port_mode_set(sys_logic_port_t portId, voice_vlan_port_mode_t mode);
extern int32 voice_vlan_port_mode_get(sys_logic_port_t portId, voice_vlan_port_mode_t *pMode);
extern int32 voice_vlan_cos_mode_set(sys_logic_port_t portId,voice_vlan_cos_mode_t cosMode);
extern int32 voice_vlan_cos_mode_get(sys_logic_port_t portId,voice_vlan_cos_mode_t *pCosMode);
extern int32 voice_vlan_1pRemark_enable_get(sys_enable_t *pEnable);
extern int32 voice_vlan_1pRemark_enable_set(sys_enable_t enable);
extern int32 voice_vlan_qos_get(sys_vlan_voiceQos_t *pVoiceQos);
extern int32 voice_vlan_qos_set(sys_vlan_voiceQos_t voiceQos);
#endif

