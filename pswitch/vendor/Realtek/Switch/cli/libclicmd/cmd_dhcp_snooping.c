
#include <libcmd/cmd.h>

int32 cmd_dhcp_snooping_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    CMD_PORTLIST_GET(0, &lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_snooping_portMib_clear(lp), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char vlanStr[CAPA_VLANLIST_STR_LEN];
    sys_vlanmask_t vlanmask;
    sys_enable_t enable = DISABLED;
    sys_dhcp_option82_id_t id;
    sys_mac_t sysMac;

    VLANMASK_CLEAR_ALL(vlanmask);
    osal_memset(&vlanStr, 0, sizeof(vlanStr));
    osal_memset(&id, 0 ,sizeof(id));

    SYS_ERR_CHK(sal_dhcp_snooping_enable_get(&enable));
    SYS_ERR_CHK(sal_dhcp_snooping_vlanmask_enable_get(&vlanmask));
    SYS_ERR_CHK(sal_dhcp_option82_remoteID_get(&id));
    VMSK2STR(&vlanmask, vlanStr);

    if (0 == osal_strlen(id.str))
    {
        SYS_ERR_CHK(sal_sys_macAddr_get(&sysMac));
            osal_memset(&id, 0, sizeof(sys_dhcp_option82_id_t));
            osal_sprintf(id.str,"%02x:%02x:%02x:%02x:%02x:%02x (Switch Mac in Byte Order)",
            sysMac.octet[0], sysMac.octet[1], sysMac.octet[2],
            sysMac.octet[3], sysMac.octet[4], sysMac.octet[5]);
    }

    XMORE("\n");
    XMORE("DHCP Snooping             : %s\n", text_enable[enable].text);
    XMORE("Enable on following Vlans : %s\n", (0 != osal_strlen(vlanStr)) ? vlanStr : "None");
    XMORE("    circuit-id default format: vlan-port\n");
    XMORE("    remote-id:               : %s\n\n", id.str);

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    uint32 rate = 0;
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t logicPortmask;
    sys_enable_t trust = DISABLED;
    sys_enable_t ccenable = DISABLED;
    sys_enable_t enable82 = DISABLED;

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE(" Interfaces | Trust State | Rate (pps) | hwaddr Check | Insert Option82 |\n");
    XMORE("------------+-------------+------------+--------------+-----------------+\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, logicPortmask)
    {
        LP2STR(lp, portStr);
        SYS_ERR_CHK(sal_dhcp_snooping_port_trust_get(lp, &trust));
        SYS_ERR_CHK(sal_dhcp_snooping_rateLimit_get(lp, &rate));
        SYS_ERR_CHK(sal_dhcp_snooping_portChaddrChk_get(lp, &ccenable));
        SYS_ERR_CHK(sal_dhcp_option82_enable_get(lp, &enable82));

        XMORE(" %-10s |", portStr);
        XMORE(" %-11s |", (DISABLED == trust) ? "Untrusted" : "Trusted");

        if (rate != BOND_RATE_UNLIMITED)
            XMORE(" %-10d |", rate);
        else
            XMORE(" %-10s |", "None");

        XMORE(" %-12s |", text_enable[ccenable].text);
        XMORE(" %-15s |", text_enable[enable82].text);
        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_portStatistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort = 0;
    sys_logic_portmask_t logicPortmask;
    sys_dhcp_mib_t counter;
    char portStr[8];

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE(" Interfaces | Forwarded | Chaddr Check Dropped | Untrust Port Dropped | Untrust Port With Option82 Dropped | Invalid Drop\n");
    XMORE("-----------+-----------+----------------------+----------------------+------------------------------------+--------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        LP2STR(logicPort, portStr);

        XMORE(" %-10s |", portStr);

        SYS_ERR_CHK(sal_dhcp_snooping_portMib_get(logicPort, &counter));
        XMORE(" %-9u | %-20u | %-20u | %-34u | %-u\n", counter.dhcpForward, counter.dhcpChaddrChkDrop,
                counter.dhcpUntrustDrop, counter.dhcpUntrustWithOption82Drop, counter.dhcpInvalidDrop);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_remoteId_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_dhcp_option82_id_t id;

    osal_memset(&id, 0 ,sizeof(id));
    SYS_ERR_CHK(sal_dhcp_option82_remoteID_get(&id));

    XMORE("Remote ID: %s\n", id.str);

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_dhcp_snooping_enable_set((CMD_IS_NO_FORM_EXIST()) ? DISABLED: ENABLED));

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_vlan_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_vid_t vid;
    sys_vlanmask_t vlanmask;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    FOR_EACH_VID_IN_VLANMASK(vid, vlanmask)
    {
        CMD_ERR_CONTINUE_VLAN(sal_dhcp_snooping_vlan_enable_set(vid, enable), vid);
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_portTrust_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_enable_t trust = DISABLED;

    if (!CMD_IS_NO_FORM_EXIST())
        trust = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_snooping_port_trust_set(logicPort, trust), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_check_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_snooping_portChaddrChk_set(lp, CMD_IS_NO_FORM_EXIST() ? DISABLED : ENABLED), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_option82_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t  enable = DISABLED;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_option82_enable_set(lp, enable), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_option82_action_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_dhcp_option82_act_t action = DFLT_DHCP_SNOOPING_PORT_OPTION82_ACTION;

    if (CMD_IS_NO_FORM_EXIST())
        action = DFLT_DHCP_SNOOPING_PORT_OPTION82_ACTION;
    else
    {
        if ('d' == argv[0][0])
            action = DHCP_OPTION82_DROP;
        else if ('k' == argv[0][0])
            action = DHCP_OPTION82_KEEP;
        else if ('r' == argv[0][0])
            action = DHCP_OPTION82_REBUILD;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_option82_action_set(lp, action), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_option82_remoteID_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_dhcp_option82_id_t remoteID;

    osal_memset(&remoteID, 0 , sizeof(remoteID));

    if (CMD_IS_NO_FORM_EXIST())
    {
        SYS_ERR_CHK(sal_dhcp_option82_remoteID_set(&remoteID));

        return SYS_ERR_OK;
    }

    if (osal_strlen(argv[0]) >= sizeof(remoteID))
        return SYS_ERR_OUT_OF_RANGE;

    osal_memcpy(&remoteID, argv[0], strlen(argv[0]));
    SYS_ERR_CHK(sal_dhcp_option82_remoteID_set(&remoteID));

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID

int32 cmd_dhcp_option82_format_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_dhcp_option82_format_t format;

    if ('n' == argv[0][0])
        format = DHCP_OPTION82_FORMAT_NORMAL;
    else
        format = DHCP_OPTION82_FORMAT_USERDEFINE;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dhcp_option82_format_set(logicPort, format), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_option82_circuitID_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_dhcp_option82_circuit_id_t circuitID;

    if (!CMD_IS_NO_FORM_EXIST())
    {
        if (osal_strlen(argv[argc-1]) >= sizeof(circuitID.str))
            return SYS_ERR_OUT_OF_RANGE;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        osal_memset(&circuitID, 0 , sizeof(circuitID));

        if (CMD_IS_NO_FORM_EXIST())
        {
            if (1 == argc)
                circuitID.vid = SYS_STR2UINT(argv[0]);
        }
        else
        {
            if (2 == argc)
            {
                circuitID.vid = SYS_STR2UINT(argv[0]);
                osal_memcpy(&circuitID.str, argv[1], strlen(argv[1]));
            }
            else
                osal_memcpy(&circuitID.str, argv[0], strlen(argv[0]));
        }

        CMD_ERR_CONTINUE_PORT(sal_dhcp_option82_circuitID_set(lp, &circuitID), lp);
    }

    return SYS_ERR_OK;
}
#endif

int32 cmd_dhcp_snooping_rateLimit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 rate;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        rate = DFLT_DHCP_SNOOPING_RATE_LIMIT;
    else
        rate = SYS_STR2UINT(argv[0]);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        SYS_ERR_CHK(sal_dhcp_snooping_rateLimit_set(logicPort, rate));
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char vlanStr[CAPA_VLANLIST_STR_LEN];
    sys_enable_t enable = DISABLED;
    sys_vlanmask_t vlanmask;
    sys_dhcp_option82_id_t remoteID;

    SYS_ERR_CHK(sal_dhcp_snooping_enable_get(&enable));

    if (DFLT_DHCP_SNOOPING_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip dhcp snooping\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_MEM_CLEAR(vlanmask);
    SYS_ERR_CHK(sal_dhcp_snooping_vlanmask_enable_get(&vlanmask));

    if (!IS_VLANMASK_CLEAR(vlanmask))
    {
        SYS_MEM_CLEAR(cmd);
        SYS_MEM_CLEAR(vlanStr);

        SYS_ERR_CHK(VMSK2STR(&vlanmask, vlanStr));

        osal_sprintf(cmd, "ip dhcp snooping vlan %s\n", vlanStr);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_MEM_CLEAR(remoteID);
    SYS_ERR_CHK(sal_dhcp_option82_remoteID_get(&remoteID));

    if (0 != osal_strlen(remoteID.str))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip dhcp snooping option remote-id \"%s\"\n", remoteID.str);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_dhcp_snooping_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 rate = 0;
    sys_enable_t enable;
    sys_dhcp_option82_act_t action;
#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    uint32 i = 0;
    sys_dhcp_option82_circuit_id_t circuitID[SYS_DHCP_O82_CID_VLAN_MAX+1];
#endif

    SYS_ERR_CHK(sal_dhcp_snooping_port_trust_get(port, &enable));

    if (DFLT_DHCP_SNOOPING_PORT_TRUST != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip dhcp snooping trust\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_dhcp_option82_action_get(port, &action));

    if (DFLT_DHCP_SNOOPING_PORT_OPTION82_ACTION != action)
    {
        SYS_MEM_CLEAR(cmd);

        switch (action)
        {
            case DHCP_OPTION82_DROP:
                osal_sprintf(cmd, "no ip dhcp snooping option action drop\n");
                break;

            case DHCP_OPTION82_KEEP:
                osal_sprintf(cmd, "ip dhcp snooping option action keep\n");
                break;

            case DHCP_OPTION82_REBUILD:
                osal_sprintf(cmd, "ip dhcp snooping option action replace\n");
                break;

            default:
                break;
        }

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_dhcp_snooping_portChaddrChk_get(port, &enable));

    if (DISABLED != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip dhcp snooping verify mac-address\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_dhcp_option82_enable_get(port, &enable));

    if (DFLT_DHCP_SNOOPING_PORT_OPTION82_ENABLE != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip dhcp snooping option\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID
    
    SYS_MEM_CLEAR(circuitID);
    SYS_ERR_CHK(sal_dhcp_option82_circuitID_get(port, circuitID));

    for (i = 0; i < SYS_DHCP_O82_CID_VLAN_MAX; i++)
    {
        if (0 != osal_strlen(circuitID[i].str))
        {
            SYS_MEM_CLEAR(cmd);

            osal_sprintf(cmd, "ip dhcp snooping vlan %u option circuit-id \"%s\"\n",  circuitID[i].vid, circuitID[i].str);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    if (0 != osal_strlen(circuitID[SYS_DHCP_O82_CID_VLAN_MAX].str))
    {
        osal_sprintf(cmd, "ip dhcp snooping option circuit-id \"%s\"\n", circuitID[SYS_DHCP_O82_CID_VLAN_MAX].str);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif

    SYS_ERR_CHK(sal_dhcp_snooping_rateLimit_get(port, &rate));

    if (DFLT_DHCP_SNOOPING_RATE_LIMIT != rate)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip dhcp snooping rate-limit %u\n", rate);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}
