
#ifndef __SKI_VOICEVLAN_H__
#define __SKI_VOICEVLAN_H__

#include <ski/ski_core.h>
#include <voice_vlan/proto_voice_vlan.h>

typedef enum ski_voice_vlan_cmd_e
{
    
    SKI_VOICEVLAN_SET_BASE = (SKI_CORE_MOD_VOICEVLAN * SKI_CORE_MODCMD_TOTAL),
    SKI_VOICE_VLAN_STATE_SET,
    SKI_VOICE_VLAN_PORT_ENABLE_SET,
    SKI_VOICE_VLAN_PORT_MODE_SET,
    SKI_VOICE_VLAN_COS_MODE_SET,
    SKI_VOICE_VLAN_REMARK_ENABLE_SET,
    SKI_VOICE_VLAN_QOS_SET,
    SKI_VOICE_VLAN_AGING_SET,
    SKI_VOICE_VLAN_SECURITY_ENABLE_SET,
    SKI_VOICE_VLAN_OUI_SET,

    SKI_VOICEVLAN_GET_BASE = (SKI_VOICEVLAN_SET_BASE + SKI_CORE_MODCMD_RANGE),
    SKI_VOICE_VLAN_STATE_GET,
    SKI_VOICE_VLAN_PORT_ENABLE_GET,
    SKI_VOICE_VLAN_PORT_MODE_GET,
    SKI_VOICE_VLAN_COS_MODE_GET,
    SKI_VOICE_VLAN_MBR_GET,
    SKI_VOICE_VLAN_REMARK_ENABLE_GET,
    SKI_VOICE_VLAN_QOS_GET,
    SKI_VOICE_VLAN_AGING_GET,
    SKI_VOICE_VLAN_SECURITY_ENABLE_GET,
    SKI_VOICE_VLAN_OUI_GET,

    SKI_VOICEVLAN_CMD_END
} ski_voice_vlan_cmd_t;

typedef struct ski_voice_vlan_port_enable_s
{
    sys_logic_port_t port;
    sys_enable_t enable;
} ski_voice_vlan_port_enable_t;

typedef struct ski_voice_vlan_port_mode_s
{
    sys_logic_port_t port;
    voice_vlan_port_mode_t  mode;
} ski_voice_vlan_port_mode_t;

typedef struct ski_voice_vlan_cos_mode_s
{
    sys_logic_port_t port;
    voice_vlan_cos_mode_t  mode;
} ski_voice_vlan_cos_mode_t;

typedef struct ski_vlan_oui_s
{
    uint32   index;
    sys_vlan_voiceVlan_oui_t   oui;
}ski_voice_vlan_oui_t;

typedef union ski_voice_vlan_ioctl_u
{
    ski_voice_vlan_oui_t ouiCfg;
    sys_vlan_voiceQos_t voiceQos;
    sys_pri_t pri;
    sys_enable_t enable;
    voice_vlan_state_t state;
    ski_voice_vlan_port_enable_t portEnable;
    ski_voice_vlan_port_mode_t portMode;
    ski_voice_vlan_cos_mode_t cosMode;
    uint32 aging;
    uint32 remarkEnable;
} ski_voice_vlan_ioctl_t;

extern int32 ski_voice_vlan_init(void);

#endif  

