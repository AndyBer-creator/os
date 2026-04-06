
#include <libcfg/cfg.h>

int32 cfg_vlan_voiceVid_set(sys_vid_t vid)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    pVlanVoice->vid = vid;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
}

int32 cfg_vlan_voiceVid_get(sys_vid_t *pVid)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    *pVid = pVlanVoice->vid;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
}

extern int32
cfg_vlan_voiceVlanState_get(voice_vlan_state_t *pState)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    *pState = pVlanVoice->state;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

extern int32
cfg_vlan_voiceVlanState_set(voice_vlan_state_t state)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(state >= VOICE_VLAN_STATE_END, SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    pVlanVoice->state = state;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_voiceVlanAging_get(uint32 *pAging)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK((NULL == pAging), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    *pAging = pVlanVoice->aging;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_voiceVlanAging_set(uint32 aging)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    pVlanVoice->aging = aging;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_portVoiceVlanEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    *pEnable = pVlanVoice->portEnable[port];

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_portVoiceVlanEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    pVlanVoice->portEnable[port] = enable;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_portVoiceVlanMode_get(sys_logic_port_t port, voice_vlan_port_mode_t *pMode)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    *pMode = pVlanVoice->portMode[port];

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_portVoiceVlan_mode_set(sys_logic_port_t port, voice_vlan_port_mode_t mode)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((mode >= VOICE_VLAN_MODE_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    pVlanVoice->portMode[port] = mode;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_voiceVlanQos_get(sys_vlan_voiceQos_t *pVoiceQos)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK((NULL == pVoiceQos), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    osal_memcpy(pVoiceQos, &pVlanVoice->qosInfo, sizeof(sys_vlan_voiceQos_t));

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_voiceVlanQos_set(sys_vlan_voiceQos_t voiceQos)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_PRI_INVALID(voiceQos.pri), SYS_ERR_PRIORITY);
    SYS_PARAM_CHK(IS_PRI_INVALID(voiceQos.vpt), SYS_ERR_PRIORITY);
    SYS_PARAM_CHK(IS_DSCP_INVALID(voiceQos.dscp), SYS_ERR_DSCP_ID);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    osal_memcpy(&pVlanVoice->qosInfo,&voiceQos, sizeof(sys_vlan_voiceQos_t));

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_voiceVlancosMode_get(sys_logic_port_t port,voice_vlan_cos_mode_t *pCosMode)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCosMode), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    *pCosMode = pVlanVoice->cosMode[port];

    CFG_CLOSE(CFG_ID_VLAN_VOICE);

}

int32
cfg_vlan_voiceVlancosMode_set(sys_logic_port_t port,voice_vlan_cos_mode_t cosMode)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((cosMode >= VOICE_VLAN_COS_END), SYS_ERR_INPUT);

    CFG_OPEN(CFG_ID_VLAN_VOICE,(cfg_addr)&pVlanVoice);

    pVlanVoice->cosMode[port]= cosMode;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
}

int32
cfg_vlan_voiceVlan1pRemarkEnable_get(sys_enable_t *pEnable)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    *pEnable = pVlanVoice->remarkEnable;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
}

int32
cfg_vlan_voiceVlan1pRemarkEnable_set(sys_enable_t enable)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    pVlanVoice->remarkEnable = enable;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
}

int32
cfg_vlan_voiceVlanOui_get(uint32 entryIdx, sys_vlan_voiceVlan_oui_t *pOui, sys_namestr_t *pName)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    osal_memcpy(pOui, &pVlanVoice->ouiInfo[entryIdx].oui, sizeof(sys_vlan_voiceVlan_oui_t));
    *pName = pVlanVoice->ouiInfo[entryIdx].ouiName;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
} 

int32
cfg_vlan_voiceVlanOui_set(uint32 entryIdx, sys_vlan_voiceVlan_oui_t *pOui, sys_namestr_t *pName)
{
    cfg_vlan_voice_t *pVlanVoice = NULL;

    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    CFG_OPEN(CFG_ID_VLAN_VOICE, (cfg_addr)&pVlanVoice);

    osal_memcpy(&pVlanVoice->ouiInfo[entryIdx].oui, pOui, sizeof(sys_vlan_voiceVlan_oui_t));
    pVlanVoice->ouiInfo[entryIdx].ouiName = *pName;

    CFG_CLOSE(CFG_ID_VLAN_VOICE);
}

