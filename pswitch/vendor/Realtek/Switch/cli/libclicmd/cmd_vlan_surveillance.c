
#include <libcmd/cmd.h>

int32 cmd_surveillance_vlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i = 0;
    uint32 aging = 0;
    sys_vid_t vid =0;
    sys_pri_t pri =0;
    sys_enable_t enable = DISABLED;
    surveillance_vlan_oui_t oui;

    SYS_ERR_CHK(sal_surveillance_vlanState_get(&enable));
    XMORE("Administrate Surveillance VLAN state   : %s\n", (DISABLED == enable) ? "disabled":"enabled");

    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&vid));
    SYS_ERR_CHK(sal_surveillance_vlanPri_get(&pri));
    SYS_ERR_CHK(sal_surveillance_vlanAging_get(&aging));
    SYS_ERR_CHK(sal_surveillance_vlan1pRemarkEnable_get(&enable));

    if(DFLT_VLAN_RESERVED_VLAN != vid)
        XMORE("Surveillance VLAN ID       : %u\n",vid);
    else
        XMORE("Surveillance VLAN ID       : %s\n","none (disable)");

    XMORE("Surveillance VLAN Aging    : %u minutes\n", aging);
    XMORE("Surveillance VLAN CoS      : %u\n", pri);
    XMORE("Surveillance VLAN 1p Remark: %s\n", text_enable[enable].text);

    XMORE("\n");
    XMORE("OUI table\n");
    XMORE("  OUI MAC   |  Description\n");
    XMORE("------------+-----------------\n");

    for (i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        SYS_MEM_CLEAR(oui);
        SYS_ERR_CHK(sal_surveillance_vlanOuibyIdx_get(i, &oui));

        if (!IS_OUI_CLEAR(oui.oui))
            XMORE("  %02X:%02X:%02X  | %s\n", oui.oui[0], oui.oui[1], oui.oui[2], oui.ouiName.str);
    }

     XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlan_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char strPort[CAPA_PORT_STR_LEN];

    sys_enable_t enable;
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    surveillance_vlan_port_mode_t portMode;
    surveillance_vlan_cos_mode_t cosMode;

    if(1 == argc)
        CMD_PORTLIST_GET(0, &lpm);
    else
        LOGIC_PORTMASK_SET_ALL(lpm);

    XMORE("  Port | State    | Port Mode   | Cos Mode\n");
    XMORE("-------+----------+-------------+-----------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        SYS_MEM_CLEAR(strPort);
        LP2STR(lp, strPort);

        SYS_ERR_CHK(sal_surveillance_vlan_portEnable_get(lp, &enable));
        SYS_ERR_CHK(sal_surveillance_vlan_portMode_get(lp, &portMode));
        SYS_ERR_CHK(sal_surveillance_vlan_portCosMode_get(lp, &cosMode));

        XMORE("%-6s | %8s |  %6s     | %3s\n", strPort, enable?"Enabled":"Disabled",
                (SURVEILLANCE_VLAN_PORT_MODE_AUTO == portMode)?"Auto":"Manual",
                (SURVEILLANCE_VLAN_PORT_COS_SRC == cosMode) ? "Src" : "All");
    }

    return SYS_ERR_OK;
}

extern int32 cmd_surveillance_vlanState_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_surveillance_vlanState_set(CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlanVid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_vid_t vid = DFLT_VLAN_SURVEILLANCE_VLAN;

    if (!CMD_IS_NO_FORM_EXIST())
    {
        vid = SYS_STR2UINT(argv[0]);
        CMD_ERR_CHK_VLAN_NOTEXIST(vid);
    }

    SYS_ERR_CHK(sal_surveillance_vlanVid_set(vid));

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlanPri_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_surveillance_vlanPri_set(DFLT_VLAN_SURVEILLANCE_PRI));
        SYS_ERR_CHK(sal_surveillance_vlan1pRemarkEnable_set(DFLT_VLAN_SURVEILLANCE_REMARK_EBL));
    }
    else
    {
        SYS_ERR_CHK(sal_surveillance_vlanPri_set(SYS_STR2UINT(argv[0])));
        SYS_ERR_CHK(sal_surveillance_vlan1pRemarkEnable_set(4 == CMD_TOKENNUM_GET(cmd->vline) ? ENABLED : DISABLED));
    }

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlanAging_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 aging = DFLT_VLAN_SURVEILLANCE_AGING;

    if (!CMD_IS_NO_FORM_EXIST())
        aging = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_surveillance_vlanAging_set(aging));

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlanOui_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    surveillance_vlan_oui_t newOui;
    sys_mac_t mac;
    uint32 i;

    SYS_MEM_CLEAR(mac);
    SYS_MEM_CLEAR(newOui);

    if (CMD_IS_NO_FORM_EXIST())
    {
        if (1 == argc)
        {
            
            SYS_ERR_CHK(sys_util_str2Mac(argv[0], mac.octet));
            newOui.oui[0] = mac.octet[0];
            newOui.oui[1] = mac.octet[1];
            newOui.oui[2] = mac.octet[2];

            SYS_ERR_CHK(sal_surveillance_vlanOui_del(&newOui));

            return SYS_ERR_OK;
        }
        else
        {
            
            SYS_MEM_CLEAR(newOui);

            for (i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
                SYS_ERR_CHK(sal_surveillance_vlanOuiByIdx_set(i, &newOui));
        }
    }
    else
    {
        
        SYS_ERR_CHK(sys_util_str2Mac(argv[0], mac.octet));
        newOui.oui[0] = mac.octet[0];
        newOui.oui[1] = mac.octet[1];
        newOui.oui[2] = mac.octet[2];
        SYS_MEM_CLEAR(newOui.ouiName);

        if (newOui.oui[0] & 0x1)
            return SYS_ERR_VLAN_SV_OUI_MCAST;

        if (IS_OUI_CLEAR(newOui.oui))
            return SYS_ERR_VLAN_SV_OUI_ZERO;

        if(2 == argc)
        {
            if (SYS_ERR_STR_LEN_EXCEED == sys_util_namestrcpy(&newOui.ouiName, argv[1]))
            {
                XMORE("Warning: surveillance-vlan name length exceeds recommended maximum %d characters\n", CAPA_NAME_STR_LEN);
                return SYS_ERR_OK;
            }
        }

        SYS_ERR_CHK(sal_surveillance_vlanOui_add(&newOui));
    }

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlan_portEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_enable_t  enable;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_surveillance_vlan_portEnable_set(lp, enable), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlan_portMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    voice_vlan_port_mode_t mode = DFLT_VLAN_SURVEILLANCE_PORT_MODE;

    if (!CMD_IS_NO_FORM_EXIST())
    {
        if('a' == argv[0][0])
            mode = SURVEILLANCE_VLAN_PORT_MODE_AUTO;
        else
            mode = SURVEILLANCE_VLAN_PORT_MODE_MANUAL;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_surveillance_vlan_portMode_set(lp, mode), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlan_portCosMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    voice_vlan_cos_mode_t cosMode = DFLT_VLAN_SURVEILLANCE_PORT_COS_MODE;
    sys_logic_port_t lp;

    if (!CMD_IS_NO_FORM_EXIST())
    {
        if('s' == argv[0][0])
            cosMode = VOICE_VLAN_COS_SRC;
        else
            cosMode = VOICE_VLAN_COS_ALL;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_surveillance_vlan_portCosMode_set(lp, cosMode), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlan_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 aging = 0;
    uint32 i = 0;
    sys_vid_t vid;
    sys_pri_t pri;
    sys_enable_t enable;
    sys_enable_t remarkEnable;
    surveillance_vlan_oui_t oui;

    SYS_ERR_CHK(sal_surveillance_vlanVid_get(&vid));
    SYS_ERR_CHK(sal_surveillance_vlanPri_get(&pri));
    SYS_ERR_CHK(sal_surveillance_vlanState_get(&enable));
    SYS_ERR_CHK(sal_surveillance_vlanAging_get(&aging));
    SYS_ERR_CHK(sal_surveillance_vlan1pRemarkEnable_get(&remarkEnable));

    if (DFLT_VLAN_SURVEILLANCE_VLAN != vid)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "surveillance-vlan vlan %u\n",vid);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (DFLT_VLAN_SURVEILLANCE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "surveillance-vlan\n");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if ((DFLT_VLAN_SURVEILLANCE_PRI != pri) || (DFLT_VLAN_SURVEILLANCE_REMARK_EBL != remarkEnable))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "surveillance-vlan cos %u %s\n", pri,(ENABLED == remarkEnable) ? "remark" : "");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    if (DFLT_VLAN_SURVEILLANCE_AGING != aging)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "surveillance-vlan aging-time %u\n", aging);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    for (i = 0; i < CAPA_VLAN_SURVEILLANCE_OUI_NUM; i++)
    {
        SYS_ERR_CHK(sal_surveillance_vlanOuibyIdx_get(i, &oui));

        if (IS_OUI_CLEAR(oui.oui))
            continue;

        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "surveillance-vlan oui-table %02X:%02X:%02X \"%s\"\n",
                         oui.oui[0], oui.oui[1], oui.oui[2],
                         oui.ouiName.str);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_surveillance_vlan_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;
    surveillance_vlan_port_mode_t mode;
    surveillance_vlan_cos_mode_t cosMode;

    SYS_ERR_CHK(sal_surveillance_vlan_portCosMode_get(port, &cosMode));
    SYS_ERR_CHK(sal_surveillance_vlan_portMode_get(port, &mode));
    SYS_ERR_CHK(sal_surveillance_vlan_portEnable_get(port, &enable));

    if (DFLT_VLAN_SURVEILLANCE_PORT_COS_MODE != cosMode)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "surveillance-vlan cos %s\n", SURVEILLANCE_VLAN_PORT_COS_SRC == cosMode ? "src" : "all");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_VLAN_SURVEILLANCE_PORT_MODE != mode)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "surveillance-vlan mode %s\n", (SURVEILLANCE_VLAN_PORT_MODE_MANUAL == mode) ? "manual" : "auto");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_VLAN_SURVEILLANCE_PORT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%ssurveillance-vlan\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

