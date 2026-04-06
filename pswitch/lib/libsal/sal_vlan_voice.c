
#include <libsal/sal_util.h>
#include <libsal/sal_vlan_voice.h>

const sys_text_t text_voice_vlan_state[] =
{
    {            "disable" },
    {       "auto-enabled" },
    {         "oui-enabled" },

};

const sys_text_t text_voiceVlan_portMode[] =
{
    {             "auto" },
    {           "manual" },
};

const sys_text_t text_voiceVlan_port_cosMode[] =
{
    {             "src" },
    {             "all" },
};

int32 sal_vlan_voiceVid_set(sys_vid_t vid)
{
    voice_vlan_state_t state;
    sys_enable_t enable;
    sys_logic_port_t logicPort;
    sys_vid_t voiceVid;

    SYS_ERR_CHK(sal_vlan_voiceVlanState_get(&state));

    if(DFLT_VLAN_RESERVED_VLAN != vid)
    {
        SYS_PARAM_CHK(IS_VID_INVALID(vid), SYS_ERR_VLAN_ID);
        SYS_ERR_CHK(sal_vlan_exclusive_chk(SYS_VLAN_CATE_VOICE_MANUAL, vid));
    }
    else
    {
        if(VOICE_VLAN_DISABLED != state)
            return SYS_ERR_VLAN_VOICEVLAN_VID_ENABLE;
    }

    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));

    if(VOICE_VLAN_OUI_ENABLED == state)
    {
        if(voiceVid != vid)
        {
            FOR_EACH_LOGIC_PORT(logicPort)
            {
                SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(logicPort,&enable));
                if(ENABLED == enable)
                    return SYS_ERR_VLAN_VOICEVLAN_OUI_ENABLED_PORT;
            }
        }
    }

    SYS_ERR_CHK(rsd_vlan_voiceVid_set(vid));
    SYS_ERR_CHK(cfg_vlan_voiceVid_set(vid));

    return SYS_ERR_OK;
}

int32 sal_vlan_voiceVid_get(sys_vid_t *pVid)
{
    SYS_PARAM_CHK((NULL == pVid), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_voiceVid_get(pVid));

    return SYS_ERR_OK;
}

int32
sal_vlan_voiceVlanState_get(voice_vlan_state_t *pState)
{
    SYS_PARAM_CHK((NULL == pState), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_voiceVlanState_get(pState));

    return SYS_ERR_OK;
} 

int32 sal_vlan_voiceVlanState_set(voice_vlan_state_t state)
{
    sys_enable_t enable;
    sys_logic_port_t logicPort;
    sys_vid_t defaultVid;
    sys_vid_t voiceVid;
    voice_vlan_state_t voiceVlanState;

    SYS_PARAM_CHK(state >= VOICE_VLAN_STATE_END, SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(sal_vlan_voiceVlanState_get(&voiceVlanState));
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(sal_vlan_defaultVid_get(&defaultVid));

    if(voiceVlanState == state)
        return SYS_ERR_OK;

    if((defaultVid== voiceVid) &&
            (VOICE_VLAN_OUI_ENABLED == state))
        return SYS_ERR_VLAN_VOICEVLAN_OUI_DFLT_VID;

    if(VOICE_VLAN_DISABLED != state && DFLT_VLAN_RESERVED_VLAN == voiceVid)
        return SYS_ERR_VLAN_VOICEVLAN_VID_NOT_EXIST;

    if(VOICE_VLAN_OUI_ENABLED != state)
    {
        FOR_EACH_LOGIC_PORT(logicPort)
        {
            SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(logicPort,&enable));
            if(ENABLED == enable)
                return SYS_ERR_VLAN_VOICEVLAN_OUI_ENABLED_PORT;
        }
    }

    if(VOICE_VLAN_DISABLED != state)
        SYS_ERR_CHK(voice_vlan_state_set(VOICE_VLAN_DISABLED));

    SYS_ERR_CHK(voice_vlan_state_set(state));
    SYS_ERR_CHK(cfg_vlan_voiceVlanState_set(state));

    return SYS_ERR_OK;
}

int32 sal_vlan_voiceVlanAging_get(uint32 *pAging)
{
    SYS_PARAM_CHK((NULL == pAging), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_voiceVlanAging_get(pAging));

    return SYS_ERR_OK;
} 

int32 sal_vlan_voiceVlanAging_set(uint32 aging)
{
    SYS_ERR_CHK(voice_vlan_aging_set(aging));
    SYS_ERR_CHK(cfg_vlan_voiceVlanAging_set(aging));

    return SYS_ERR_OK;
} 

int32 sal_vlan_portVoiceVlanEnable_get(sys_logic_port_t port, sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_portVoiceVlanEnable_get(port, pEnable));

    return SYS_ERR_OK;
} 

int32 sal_vlan_portVoiceVlanEnable_set(sys_logic_port_t port, sys_enable_t enable)
{
    voice_vlan_state_t state;
    voice_vlan_port_mode_t portMode;
    sys_vid_t voiceVid;
    sys_vlan_membership_t membership;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(sal_vlan_portVoiceVlanMode_get(port,&portMode));
    SYS_ERR_CHK(sal_vlan_voiceVlanState_get(&state));
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_get(port,voiceVid,&membership));

    if(ENABLED == enable)
    {
        if(VOICE_VLAN_OUI_ENABLED != state)
            return SYS_ERR_VLAN_VOICEVLAN_OUI_PORT;

        if(VOICE_VLAN_MODE_AUTO == portMode)
        {
            if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_GENERAL) | (1 << SYS_VLAN_PORT_MODE_TRUNK)))
                return SYS_ERR_VLAN_VOICEVLAN_PORT_MODE;

            if((SYS_VLAN_TAGGED == membership) ||(SYS_VLAN_UNTAGGED == membership))
                return SYS_ERR_VLAN_VOICEVLAN_PORT_STATIC;
        }
    }

    SYS_ERR_CHK(voice_vlan_port_enable_set(port, enable));
    SYS_ERR_CHK(cfg_vlan_portVoiceVlanEnable_set(port, enable));

    return SYS_ERR_OK;
}

int32 sal_vlan_portVoiceVlanMode_get(sys_logic_port_t port, voice_vlan_port_mode_t *pMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_portVoiceVlanMode_get(port, pMode));

    return SYS_ERR_OK;
}

int32 sal_vlan_portVoiceVlanMode_set(sys_logic_port_t port, voice_vlan_port_mode_t mode)
{
    sys_enable_t enable;
    sys_vid_t voiceVid;
    sys_vlan_membership_t membership;

    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((mode >= VOICE_VLAN_MODE_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(port,&enable));
    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(sal_vlan_portAdminVlanMembership_get(port,voiceVid,&membership));

    if((ENABLED == enable) && (VOICE_VLAN_MODE_AUTO == mode))
    {
        if (SYS_ERR_OK != sal_vlan_portAdminMode_chk(port, (1 << SYS_VLAN_PORT_MODE_GENERAL) | (1 << SYS_VLAN_PORT_MODE_TRUNK)))
                return SYS_ERR_VLAN_VOICEVLAN_PORT_MODE;

         if((SYS_VLAN_UNTAGGED == membership) || (SYS_VLAN_TAGGED == membership))
            return SYS_ERR_VLAN_VOICEVLAN_PORT_STATIC;
    }

    SYS_ERR_CHK(voice_vlan_port_mode_set(port, mode));

    SYS_ERR_CHK(cfg_vlan_portVoiceVlan_mode_set(port, mode));

    return SYS_ERR_OK;
}

int32 sal_vlan_voiceVlanQos_get(sys_vlan_voiceQos_t *pVoiceQos)
{
    SYS_PARAM_CHK((NULL == pVoiceQos), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_voiceVlanQos_get(pVoiceQos));

    return SYS_ERR_OK;
}

int32 sal_vlan_voiceVlanQos_set(sys_vlan_voiceQos_t voiceQos)
{
    SYS_PARAM_CHK(IS_PRI_INVALID(voiceQos.pri), SYS_ERR_PRIORITY);
    SYS_PARAM_CHK(IS_PRI_INVALID(voiceQos.vpt), SYS_ERR_PRIORITY);
    SYS_PARAM_CHK(IS_DSCP_INVALID(voiceQos.dscp), SYS_ERR_DSCP_ID);

    SYS_ERR_CHK(voice_vlan_qos_set(voiceQos));
    SYS_ERR_CHK(cfg_vlan_voiceVlanQos_set(voiceQos));

    return SYS_ERR_OK;
} 

int32 sal_vlan_portVoiceVlancosMode_get(sys_logic_port_t port,voice_vlan_cos_mode_t *pCosMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pCosMode), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_voiceVlancosMode_get(port,pCosMode));

    return SYS_ERR_OK;
}

int32 sal_vlan_portVoiceVlancosMode_set(sys_logic_port_t port,voice_vlan_cos_mode_t cosMode)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((cosMode>= VOICE_VLAN_COS_END), SYS_ERR_INPUT);

    SYS_ERR_CHK(voice_vlan_cos_mode_set(port,cosMode));
    SYS_ERR_CHK(cfg_vlan_voiceVlancosMode_set(port,cosMode));

    return SYS_ERR_OK;
}

int32 sal_vlan_voiceVlan1pRemarkEnable_get(sys_enable_t *pEnable)
{
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(cfg_vlan_voiceVlan1pRemarkEnable_get(pEnable));

    return SYS_ERR_OK;
} 

int32 sal_vlan_voiceVlan1pRemarkEnable_set(sys_enable_t enable)
{
    SYS_PARAM_CHK(IS_ENABLE_INVALID(enable), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(voice_vlan_1pRemark_enable_set(enable));
    SYS_ERR_CHK(cfg_vlan_voiceVlan1pRemarkEnable_set(enable));

    return SYS_ERR_OK;
} 

int32 sal_vlan_voiceVlanOui_add(
    sys_vlan_voiceVlan_oui_t *pOui,
    sys_namestr_t            *pName)
{
    uint32 index = 0;
    int32 freeIdx = -1;
    sys_vlan_voiceVlan_oui_t voiceVlan;
    sys_namestr_t name;

    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    for (index = 0; index < CAPA_VLAN_VOICE_OUI_NUM; index++)
    {
        osal_memset(&voiceVlan, 0, sizeof(sys_vlan_voiceVlan_oui_t));
        SYS_ERR_CHK(sal_vlan_voiceVlanOui_get(index, &voiceVlan, &name));

        if(ENABLED != voiceVlan.enable)
        {
            if (-1 == freeIdx)
                freeIdx = index;
            break;
        }
    }

    if (-1 == freeIdx)
        return SYS_ERR_ENTRY_FULL;

    SYS_ERR_CHK(sal_vlan_voiceVlanOui_set(freeIdx, pOui, pName));

    return SYS_ERR_OK;
} 

int32 sal_vlan_voiceVlanOui_get(
    uint32 entryIdx,
    sys_vlan_voiceVlan_oui_t *pOui,
    sys_namestr_t *pName)
{
    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);

    SYS_ERR_CHK(cfg_vlan_voiceVlanOui_get(entryIdx, pOui, pName));

    return SYS_ERR_OK;
} 

int32 sal_vlan_voiceVlanOui_set(
    uint32 entryIdx,
    sys_vlan_voiceVlan_oui_t *pOui,
    sys_namestr_t *pName)
{
    SYS_PARAM_CHK(IS_VOICEVLAN_ENTRY_INVALID(entryIdx), SYS_ERR_OUT_OF_RANGE);
    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);
    SYS_PARAM_CHK((NULL == pName), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(voice_vlan_oui_set(entryIdx, pOui));
    SYS_ERR_CHK(cfg_vlan_voiceVlanOui_set(entryIdx, pOui, pName));

    return SYS_ERR_OK;
} 

int32 sal_vlan_voiceVlanOui_del(sys_vlan_voiceVlan_oui_t *pOui)
{
    uint32 entryIdx;
    sys_vlan_voiceVlan_oui_t ouiCfg;
    sys_namestr_t name;

    SYS_PARAM_CHK((NULL == pOui), SYS_ERR_NULL_POINTER);

    SYS_MEM_CLEAR(name);

    FOR_EACH_VOICE_OUI_ENTRY(entryIdx)
    {
        SYS_MEM_CLEAR(ouiCfg);

        SYS_ERR_CHK(sal_vlan_voiceVlanOui_get(entryIdx, &ouiCfg, &name));

        if (ENABLED != ouiCfg.enable)
            continue;

        if (0 == osal_memcmp(pOui->oui, ouiCfg.oui, SNAPOUI_LEN))
        {
            ouiCfg.enable = DISABLED;
            SYS_ERR_CHK(sal_vlan_voiceVlanOui_set(entryIdx, &ouiCfg, &name));
        }
    }

    return SYS_ERR_OK;
}

int32 sal_vlan_voiceVlanOui_fdsEntry_set(void)
{
    uint32 index;
    sys_vlan_voiceVlan_oui_t ouiCfg;
    sys_namestr_t name;
    sal_voiceVanOuiCfg_t dltOuiCfg[] =
    {
        {
            .oui.oui = {0, 0xE0, 0xBB},
            .oui.ouiMask = {0xFF, 0xFF, 0xFF},
            .oui.enable = ENABLED,
            .ouiName.str = "3COM",
        },
        {
            .oui.oui = {0, 0x03, 0x6B},
            .oui.ouiMask = {0xFF, 0xFF, 0xFF},
            .oui.enable = ENABLED,
            .ouiName.str = "Cisco",
        },
        {
            .oui.oui = {0, 0xE0, 0x75},
            .oui.ouiMask = {0xFF, 0xFF, 0xFF},
            .oui.enable = ENABLED,
            .ouiName.str = "Veritel",
        },
        {
            .oui.oui = {0, 0xD0, 0x1E},
            .oui.ouiMask = {0xFF, 0xFF, 0xFF},
            .oui.enable = ENABLED,
            .ouiName.str = "Pingtel",
        },
        {
            .oui.oui = {0, 0x01, 0xE3},
            .oui.ouiMask = {0xFF, 0xFF, 0xFF},
            .oui.enable = ENABLED,
            .ouiName.str = "Siemens",
        },
        {
            .oui.oui = {0, 0x60, 0xB9},
            .oui.ouiMask = {0xFF, 0xFF, 0xFF},
            .oui.enable = ENABLED,
            .ouiName.str = "NEC/Philips",
        },
        {
            .oui.oui = {0, 0x0F, 0xE2},
            .oui.ouiMask = {0xFF, 0xFF, 0xFF},
            .oui.enable = ENABLED,
            .ouiName.str = "H3C",
        },
        {
            .oui.oui = {0, 0x09, 0x6E},
            .oui.ouiMask = {0xFF, 0xFF, 0xFF},
            .oui.enable = ENABLED,
            .ouiName.str = "Avaya",
        }
    };

    SYS_MEM_CLEAR(ouiCfg);
    SYS_MEM_CLEAR(name);
    ouiCfg.enable = DISABLED;

    for (index = 0; index < CAPA_VLAN_VOICE_OUI_NUM; index++)
        SYS_ERR_CHK(sal_vlan_voiceVlanOui_set(index, &ouiCfg, &name));

    for (index = 0;
         index < sizeof(dltOuiCfg)/sizeof(sal_voiceVanOuiCfg_t);
         index++)
    {
        SYS_ERR_CHK(sal_vlan_voiceVlanOui_set(index, &(dltOuiCfg[index].oui), &(dltOuiCfg[index].ouiName)));
    }

    return  SYS_ERR_OK;

}

int32 sal_vlan_portVoiceStatus_chk(sys_logic_port_t port)
{
    sys_enable_t enable;
    voice_vlan_state_t state;
    voice_vlan_port_mode_t mode;

    SYS_ERR_CHK(sal_vlan_voiceVlanState_get(&state));

    if(VOICE_VLAN_OUI_ENABLED == state)
    {
        SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(port,&enable));
        SYS_ERR_CHK(sal_vlan_portVoiceVlanMode_get(port,&mode));

        if((ENABLED == enable) &&(VOICE_VLAN_MODE_AUTO == mode))
            return SYS_ERR_FAILED;
    }

    if(VOICE_VLAN_AUTO_ENABLED == state)
    {
        SYS_ERR_CHK(sal_vlan_portVoiceVlanActive_get(port,&enable));

        if(ENABLED == enable)
            return SYS_ERR_FAILED;
    }

    return SYS_ERR_OK;
}

int32 sal_vlan_portVoiceVlanActive_get(sys_logic_port_t port,sys_enable_t *pEnable)
{
    SYS_PARAM_CHK(IS_LOGIC_PORT_INVALID(port), SYS_ERR_PORT_ID);
    SYS_PARAM_CHK((NULL == pEnable), SYS_ERR_NULL_POINTER);

    SYS_ERR_CHK(rsd_vlan_portVoiceVlanActive_get(port,pEnable));

    return SYS_ERR_OK;
}

