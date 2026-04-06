
#include <include/cgi.h>

static int32 cgi_vlan_voice_prop_set(void);
static int32 cgi_vlan_voice_portEdit_set(void);
static int32 cgi_vlan_voice_oui_set(void);
static int32 cgi_vlan_voice_ouiDel_set(void);

const cgi_cmdmap_t cmdSetVlanVoiceTbl[] =
{
    {CGI_CMD_VLAN_VOICE_PROP,          cgi_vlan_voice_prop_set},
    {CGI_CMD_VLAN_VOICE_PORTEDIT,      cgi_vlan_voice_portEdit_set},
    {CGI_CMD_VLAN_VOICE_OUI,           cgi_vlan_voice_oui_set},
    {CGI_CMD_VLAN_VOICE_OUIDEL,        cgi_vlan_voice_ouiDel_set},
    {NULL, NULL}
};

static int32 cgi_vlan_voice_prop_set(void)
{
    char *value;
    int32 ret = SYS_ERR_OK;
    int32 aging;
    sys_vid_t vid;
    sys_enable_t remark = DISABLED;
    voice_vlan_state_t state;
    sys_vlan_voiceQos_t voiceQos;

    SYS_MEM_CLEAR(voiceQos);

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        state = VOICE_VLAN_OUI_ENABLED;
    else
        state = VOICE_VLAN_DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_VLAN, value))
        vid = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_COS_REMARK, value))
        remark = ENABLED;
    else
        remark = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_COS, value))
        voiceQos.pri = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_AGING, value))
        aging = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (VOICE_VLAN_DISABLED == state)
    {
        CGI_SET_ERR_HDL(sal_vlan_voiceVlanState_set(state), ret);
        CGI_SET_ERR_HDL(sal_vlan_voiceVid_set(vid), ret);
    }
    else
    {
        CGI_SET_ERR_HDL(sal_vlan_voiceVid_set(vid), ret);
        CGI_SET_ERR_HDL(sal_vlan_voiceVlanState_set(state), ret);
    }

    CGI_SET_ERR_HDL(sal_vlan_voiceVlanQos_set(voiceQos), ret);
    CGI_SET_ERR_HDL(sal_vlan_voiceVlan1pRemarkEnable_set(remark), ret);
    CGI_SET_ERR_HDL(sal_vlan_voiceVlanAging_set(aging), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_voice_portEdit_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_enable_t portEnable = DISABLED;
    voice_vlan_port_mode_t portMode;
    voice_vlan_cos_mode_t cosMode;

    if (CGI_IS_PARAM_EXIST(CGI_PORTLIST, value))
        STR2LPM(value, &lpm);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_STATE, value))
        portEnable = ENABLED;
    else
        portEnable = DISABLED;

    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_PORT_MODE, value))
        portMode = SYS_STR2UINT(value);
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_COS_MODE, value))
        cosMode = SYS_STR2UINT(value);
    else
        goto cgi_end;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        
        if (ENABLED == portEnable)
        {
            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portVoiceVlanMode_set(lp, portMode), lp);
            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portVoiceVlancosMode_set(lp, cosMode), lp);
            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portVoiceVlanEnable_set(lp, portEnable), lp);
        }
        else
        {
            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portVoiceVlanEnable_set(lp, portEnable), lp);
            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portVoiceVlanMode_set(lp, portMode), lp);
            CGI_SET_PORT_ERR_CONTINUE(sal_vlan_portVoiceVlancosMode_set(lp, cosMode), lp);

        }
    }

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_voice_oui_set(void)
{
    char *value = NULL;
    int32 ret = SYS_ERR_OK;
    sys_vlan_voiceVlan_oui_t ouicfg;
    sys_namestr_t name;

    SYS_MEM_CLEAR(ouicfg);

    if (CGI_IS_PARAM_EXIST(CGI_IDX, value))
    {
        SYS_MEM_CLEAR(name);
        CGI_SET_ERR_HDL(sal_vlan_voiceVlanOui_set(SYS_STR2UINT(value), &ouicfg, &name), ret);
    }

    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_OUI1, value))
    {
        SYS_MEM_CLEAR(name);
        osal_sprintf(name.str, "0x%s", value);
        ouicfg.oui[0] = SYS_STR2UINT(name.str);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_OUI2, value))
    {
        SYS_MEM_CLEAR(name);
        osal_sprintf(name.str, "0x%s", value);
        ouicfg.oui[1] = SYS_STR2UINT(name.str);
    }
    else
        goto cgi_end;

    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_OUI3, value))
    {
        SYS_MEM_CLEAR(name);
        osal_sprintf(name.str, "0x%s", value);
        ouicfg.oui[2] = SYS_STR2UINT(name.str);
    }
    else
        goto cgi_end;

    SYS_MEM_CLEAR(name);
    if (CGI_IS_PARAM_EXIST(CGI_VOICEVLAN_OUT_DESP, value))
        strlcpy(name.str, value, sizeof(name.str));

    osal_memset(ouicfg.ouiMask,0xff, SNAPOUI_LEN);
    ouicfg.enable = ENABLED;
    CGI_SET_ERR_HDL(sal_vlan_voiceVlanOui_add(&ouicfg, &name), ret);

cgi_end:
    cgi_set_respGeneral_send(ret);

    return SYS_ERR_OK;
}

static int32 cgi_vlan_voice_ouiDel_set(void)
{
    int32 ret = SYS_ERR_OK;
    uint32 len = 0;
    uint32 i = 0;
    char **array = NULL;

    sys_vlan_voiceVlan_oui_t ouicfg;
    sys_namestr_t name;

    SYS_MEM_CLEAR(ouicfg);
    SYS_MEM_CLEAR(name);

    if(CGI_GET_PARAM_ARRAY_DATA(CGI_IDX, array, &len))
    {
        for(i = 0; i < len; i++)
            CGI_SET_ENTRY_ERR_CONTINUE(sal_vlan_voiceVlanOui_set(SYS_STR2UINT(array[i]), &ouicfg, &name), "Delete");
    }

    cgi_set_respGeneral_send(ret);
    if (NULL != array)
        osal_free(array);

    return SYS_ERR_OK;
}

