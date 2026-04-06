
#include <libcmd/cmd.h>

const sys_text_t text_voice_vlan_state[] =
{
    {                  "disable"     },
    {              "auto-enabled"    },
    {               "oui-enabled"     },
};

#define VOICE_VLAN_OUI_TABLE_HEADER_PRINT()\
{\
    XMORE("   OUI MAC    |   Description\n");    \
    XMORE("--------------+-----------------\n");   \
}

int32 cmd_vlan_voice_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t voiceVid;
    voice_vlan_state_t state;
    sys_vlan_voiceQos_t voiceQos;
    sys_enable_t enable;
    uint32 aging;

    SYS_ERR_CHK(sal_vlan_voiceVlanState_get(&state));
    XMORE("Administrate Voice VLAN state   : %s\n", (VOICE_VLAN_DISABLED == state) ? "disabled":"enabled");

    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(sal_vlan_voiceVlanQos_get(&voiceQos));
    SYS_ERR_CHK(sal_vlan_voiceVlanAging_get(&aging));
    if(DFLT_VLAN_RESERVED_VLAN != voiceVid)
        XMORE("Voice VLAN ID       : %u\n",voiceVid);
    else
        XMORE("Voice VLAN ID       : %s\n","none (disable)");

    if(VOICE_VLAN_AUTO_ENABLED != state)
    {
        XMORE("Voice VLAN Aging    : %u minutes\n", aging);
        XMORE("Voice VLAN CoS      : %u\n", voiceQos.pri);

        SYS_ERR_CHK(sal_vlan_voiceVlan1pRemarkEnable_get(&enable));
        XMORE("Voice VLAN 1p Remark: %s\n", text_enable[enable].text);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_voiceType_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 aging,index = 0;
    sys_enable_t enable;
    sys_vlan_voiceQos_t voiceQos;
    voice_vlan_port_mode_t portMode;
    voice_vlan_cos_mode_t cosMode;
    sys_vlan_voiceVlan_oui_t ouiCfg;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;
    char strPort[CAPA_PORT_STR_LEN];
    sys_namestr_t name;

    osal_memset(&ouiCfg, 0, sizeof(sys_vlan_voiceVlan_oui_t));
    osal_memset(&name, 0, sizeof(sys_namestr_t));
    SYS_MEM_CLEAR(strPort);

    SYS_ERR_CHK(sal_vlan_voiceVlanQos_get(&voiceQos));
    SYS_ERR_CHK(sal_vlan_voiceVlanAging_get(&aging));

    XMORE("Voice VLAN Aging    : %u minutes\n", aging);
    XMORE("Voice VLAN CoS      : %u\n", voiceQos.pri);

    SYS_ERR_CHK(sal_vlan_voiceVlan1pRemarkEnable_get(&enable));
    XMORE("Voice VLAN 1p Remark: %s\n", text_enable[enable].text);

    XMORE("\n");
    XMORE("OUI table\n");
    VOICE_VLAN_OUI_TABLE_HEADER_PRINT();

    for (index = 0; index < CAPA_VLAN_VOICE_OUI_NUM; index++)
    {
        SYS_ERR_CHK(sal_vlan_voiceVlanOui_get(index, &ouiCfg, &name));

        if (DISABLED == ouiCfg.enable)
            continue;

        XMORE("   %02X:%02X:%02X   | %s\n",
            ouiCfg.oui[0], ouiCfg.oui[1], ouiCfg.oui[2],
            name.str);
    }

    XMORE("\n");
    XMORE("  Port | State    | Port Mode   | Cos Mode\n");
    XMORE("-------+----------+-------------+-----------\n");

    if(1 == argc)
     {
        CMD_PORTLIST_GET(0, &logicPortmask);
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
        {
            LP2STR(logicPort, strPort);

            SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(logicPort, &enable));
            SYS_ERR_CHK(sal_vlan_portVoiceVlanMode_get(logicPort, &portMode));
            SYS_ERR_CHK(sal_vlan_portVoiceVlancosMode_get(logicPort, &cosMode));

            XMORE("%-6s | %8s |  %6s     | %3s\n", strPort, enable?"Enabled":"Disabled",
                    (VOICE_VLAN_MODE_AUTO == portMode)?"Auto":"Manual",
                    (VOICE_VLAN_COS_SRC == cosMode) ? "Src" : "All");
        }
     }
     else
     {
        FOR_EACH_LOGIC_PORT(logicPort)
        {
            LP2STR(logicPort, strPort);

            SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(logicPort, &enable));
            SYS_ERR_CHK(sal_vlan_portVoiceVlanMode_get(logicPort, &portMode));
            SYS_ERR_CHK(sal_vlan_portVoiceVlancosMode_get(logicPort, &cosMode));

            XMORE("%-6s | %8s |  %6s     | %3s\n", strPort, enable?"Enabled":"Disabled",
                    (VOICE_VLAN_MODE_AUTO == portMode)?"Auto":"Manual",
                    (VOICE_VLAN_COS_SRC == cosMode) ? "Src" : "All");
        }

     }

    return SYS_ERR_OK;
}

extern int32 cmd_vlan_voiceVlanState_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    voice_vlan_state_t state = VOICE_VLAN_DISABLED;

    if (CMD_IS_NO_FORM_EXIST())
        state = VOICE_VLAN_DISABLED;
    else
        state = VOICE_VLAN_OUI_ENABLED;

    SYS_ERR_CHK(sal_vlan_voiceVlanState_set(state));

    return SYS_ERR_OK;
}

int32 cmd_vlan_voiceVlanVid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t vid;
    sys_vid_t voiceVid;

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_vlan_voiceVid_set(DFLT_VLAN_VOICE_VLAN));

        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));

    vid = SYS_STR2UINT(argv[0]);
    CMD_ERR_CHK_VLAN_NOTEXIST(vid);

    if(vid != voiceVid)
    {
        voiceVid = vid;
        SYS_ERR_CHK(sal_vlan_voiceVid_set(voiceVid));
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_voiceVlanPri_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlan_voiceQos_t voiceQos;
    sys_pri_t pri;
    uint32 tokenCnt;

    SYS_ERR_CHK(sal_vlan_voiceVlanQos_get(&voiceQos));

    if (CMD_IS_NO_FORM_EXIST())
    {
        voiceQos.pri = DFLT_VLAN_VOICE_PRI;
        SYS_ERR_CHK(sal_vlan_voiceVlanQos_set(voiceQos));
        SYS_ERR_CHK(sal_vlan_voiceVlan1pRemarkEnable_set(DFLT_VLAN_VOICE_REMARK_EBL));
    }
    else
    {
        pri = SYS_STR2UINT(argv[0]);
        tokenCnt = CMD_TOKENNUM_GET(cmd->vline);

        if(pri != voiceQos.pri)
        {
            voiceQos.pri = pri;

            SYS_ERR_CHK(sal_vlan_voiceVlanQos_set(voiceQos));
        }

        if( 4 == tokenCnt)
            SYS_ERR_CHK(sal_vlan_voiceVlan1pRemarkEnable_set(ENABLED));
        else
            SYS_ERR_CHK(sal_vlan_voiceVlan1pRemarkEnable_set(DISABLED));
    }
    return SYS_ERR_OK;
}

int32 cmd_vlan_voiceVlanVpt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlan_voiceQos_t voiceQos;
    sys_pri_t vpt;

    SYS_ERR_CHK(sal_vlan_voiceVlanQos_get(&voiceQos));

    vpt = SYS_STR2UINT(argv[0]);

    if(vpt != voiceQos.vpt)
    {
        voiceQos.vpt = vpt;
        SYS_ERR_CHK(sal_vlan_voiceVlanQos_set(voiceQos));
        }
    return SYS_ERR_OK;
}

int32 cmd_vlan_voiceVlanDscp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlan_voiceQos_t voiceQos;
    uint32 dscp;

    SYS_ERR_CHK(sal_vlan_voiceVlanQos_get(&voiceQos));

    dscp = SYS_STR2UINT(argv[0]);

    if(dscp != voiceQos.dscp)
    {
        voiceQos.dscp = dscp;
        SYS_ERR_CHK(sal_vlan_voiceVlanQos_set(voiceQos));
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portvoiceVlancosMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    voice_vlan_cos_mode_t cosMode;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        cosMode = DFLT_VLAN_VOICE_PORT_COS_MODE;
    else
    {
        if('s' == argv[0][0])
            cosMode = VOICE_VLAN_COS_SRC;
        else
            cosMode = VOICE_VLAN_COS_ALL;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_vlan_portVoiceVlancosMode_set(logicPort, cosMode), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_voiceVlanAging_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 aging;

    if (CMD_IS_NO_FORM_EXIST())
        aging = DFLT_VLAN_VOICE_AGING;
    else
        aging = SYS_STR2UINT(argv[0]);

    if ((aging < BOND_VLAN_VOICE_AGING_MIN) ||
                (aging > BOND_VLAN_VOICE_AGING_MAX))
    {
        return SYS_ERR_OUT_OF_RANGE;
    }

    SYS_ERR_CHK(sal_vlan_voiceVlanAging_set(aging));

    return SYS_ERR_OK;
}

int32 cmd_vlan_voiceVlanOui_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vlan_voiceVlan_oui_t ouiCfg;
    sys_vlan_voiceVlan_oui_t ouiCfgdatabase;
    sys_mac_t mac;
    sys_namestr_t name;
    sys_namestr_t nameDatabse;
    uint32 index;

    SYS_ERR_CHK(sys_util_str2Mac(argv[0], mac.octet));

    if (mac.octet[0] & 0x1)
        return SYS_ERR_VLAN_VOICEVLAN_OUI_MCAST;

    osal_memcpy(ouiCfg.oui, mac.octet, SNAPOUI_LEN);

    osal_memset(&name,0,sizeof(sys_namestr_t));
    if(2 == argc)
    {
        if (SYS_ERR_STR_LEN_EXCEED == sys_util_namestrcpy(&name, argv[1]))
        {
            XMORE("Warning: voice-vlan name length exceeds recommended maximum %d characters\n", CAPA_NAME_STR_LEN);
            return SYS_ERR_OK;
        }
    }

    for (index = 0 ; index < CAPA_VLAN_VOICE_OUI_NUM; index++)
    {
        SYS_ERR_CHK(sal_vlan_voiceVlanOui_get(index, &ouiCfgdatabase, &nameDatabse));

        if ((ENABLED == ouiCfgdatabase.enable) &&
            (ouiCfgdatabase.oui[0] == ouiCfg.oui[0]) &&
            (ouiCfgdatabase.oui[1] == ouiCfg.oui[1]) &&
            (ouiCfgdatabase.oui[2] == ouiCfg.oui[2]))
        {
            return SYS_ERR_VLAN_VOICEVLAN_OUI_EXIST;
        }
    }

    osal_memset(ouiCfg.ouiMask,0xff, SNAPOUI_LEN);
    ouiCfg.enable = ENABLED;
    SYS_ERR_CHK(sal_vlan_voiceVlanOui_add(&ouiCfg, &name));

    return SYS_ERR_OK;
}

int32 cmd_vlan_voiceVlanOui_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 index = 0;
    sys_vlan_voiceVlan_oui_t ouiCfg;
    sys_vlan_voiceVlan_oui_t ouiCfgdatabase;
    sys_mac_t mac;
    sys_namestr_t name;

    osal_memset(&ouiCfg, 0, sizeof(sys_vlan_voiceVlan_oui_t));
    osal_memset(&name, 0, sizeof(sys_namestr_t));

    if (1 == argc) 
    {
        SYS_ERR_CHK(sys_util_str2Mac(argv[0], mac.octet));
        osal_memcpy(ouiCfg.oui, mac.octet, SNAPOUI_LEN);

        for (index = 0; index < CAPA_VLAN_VOICE_OUI_NUM; index++)
        {
            osal_memset(&ouiCfgdatabase, 0, sizeof(sys_vlan_voiceVlan_oui_t));
            SYS_ERR_CHK(sal_vlan_voiceVlanOui_get(index, &ouiCfgdatabase, &name));

            if (ENABLED != ouiCfgdatabase.enable)
                continue;

            if (0 == osal_memcmp(ouiCfg.oui, ouiCfgdatabase.oui, SNAPOUI_LEN))
            {
                ouiCfgdatabase.enable = DISABLED;
                SYS_ERR_CHK(sal_vlan_voiceVlanOui_set(index, &ouiCfgdatabase, &name));

                return SYS_ERR_OK;
            }
        }

        return SYS_ERR_ENTRY_NOT_FOUND;
    }
    else  
    {
        ouiCfg.enable = DISABLED;

        for (index = 0; index < CAPA_VLAN_VOICE_OUI_NUM; index++)
            SYS_ERR_CHK(sal_vlan_voiceVlanOui_set(index, &ouiCfg, &name));
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portVoiceVlanEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_enable_t  enable;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_vlan_portVoiceVlanEnable_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_portVoiceVlanMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    voice_vlan_port_mode_t mode = DFLT_VLAN_VOICE_PORT_MODE;

    if (CMD_IS_NO_FORM_EXIST())
        mode = DFLT_VLAN_VOICE_PORT_MODE;
    else
    {
        if('a' == argv[0][0])
            mode = VOICE_VLAN_MODE_AUTO;
        else
            mode = VOICE_VLAN_MODE_MANUAL;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_vlan_portVoiceVlanMode_set(logicPort, mode), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_voice_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 aging = 0;
    uint32 entryIdx = 0;
    sys_enable_t enable;
    sys_namestr_t name;
    voice_vlan_state_t state;
    sys_vid_t voiceVid;
    sys_vlan_voiceQos_t  voiceQos;
    sys_vlan_voiceVlan_oui_t ouiCfg;

    SYS_MEM_CLEAR(voiceQos);

    SYS_ERR_CHK(sal_vlan_voiceVid_get(&voiceVid));
    SYS_ERR_CHK(sal_vlan_voiceVlanQos_get(&voiceQos));

    if (DFLT_VLAN_VOICE_VLAN != voiceVid)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "voice-vlan vlan %u\n",voiceVid);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_vlan_voiceVlanState_get(&state));

    if (DFLT_VLAN_VOICE_EBL != state)
    {
        SYS_MEM_CLEAR(cmd);

        if(VOICE_VLAN_OUI_ENABLED == state)
        {
           osal_sprintf(cmd, "voice-vlan\n");
        }

        if(VOICE_VLAN_DISABLED == state)
        {
            osal_sprintf(cmd, "no voice-vlan\n");
        }

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_vlan_voiceVlan1pRemarkEnable_get(&enable));

    if ((DFLT_VLAN_VOICE_PRI != voiceQos.pri) || (DFLT_VLAN_VOICE_REMARK_EBL != enable))
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "voice-vlan cos %u %s\n", voiceQos.pri,(ENABLED == enable) ? "remark" : "");

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_vlan_voiceVlanAging_get(&aging));

    if (DFLT_VLAN_VOICE_AGING != aging)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "voice-vlan aging-time %u\n", aging);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    for (entryIdx = 0; entryIdx < CAPA_VLAN_VOICE_OUI_NUM; entryIdx++)
    {
        SYS_ERR_CHK(sal_vlan_voiceVlanOui_get(entryIdx, &ouiCfg, &name));

        if (DISABLED == ouiCfg.enable)
            continue;

        osal_memset(cmd, 0, sizeof(cmd));
        osal_sprintf(cmd, "voice-vlan oui-table %02X:%02X:%02X \"%s\"\n",
                         ouiCfg.oui[0], ouiCfg.oui[1], ouiCfg.oui[2],
                         name.str);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_vlan_voice_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;
    voice_vlan_cos_mode_t cosMode;
    voice_vlan_port_mode_t mode;

    SYS_ERR_CHK(sal_vlan_portVoiceVlancosMode_get(port, &cosMode));

    if (DFLT_VLAN_VOICE_PORT_COS_MODE != cosMode)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "voice-vlan cos %s\n", VOICE_VLAN_COS_SRC == cosMode ? "src" : "all");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_vlan_portVoiceVlanMode_get(port, &mode));

    if (DFLT_VLAN_VOICE_PORT_MODE != mode)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "voice-vlan mode %s\n", (VOICE_VLAN_MODE_MANUAL == mode) ? "manual" : "auto");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_vlan_portVoiceVlanEnable_get(port, &enable));

    if (DFLT_VLAN_VOICE_PORT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "%svoice-vlan\n", (ENABLED == enable) ? "" : "no ");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;

}

