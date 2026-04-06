
#include <libcmd/cmd.h>

int32 cmd_dai_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;

    CMD_PORTLIST_GET(0, &lpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dai_portMib_clear(lp), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_dai_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char vlanStr[CAPA_VLANLIST_STR_LEN];
    sys_vlanmask_t vlanmask;
    sys_enable_t enable = DISABLED;

    VLANMASK_CLEAR_ALL(vlanmask);
    osal_memset(&vlanStr, 0, sizeof(vlanStr));

    SYS_ERR_CHK(sal_dai_vlanmask_enable_get(&vlanmask));
    SYS_ERR_CHK(sal_dai_enable_get(&enable));
    VMSK2STR(&vlanmask, vlanStr);

    XMORE("Dynamic ARP Inspection     : %s\n", text_enable[enable].text);
    XMORE("Enable on Vlans            : %s\n\n", (0 != osal_strlen(vlanStr)) ? vlanStr : "None");

    return SYS_ERR_OK;
}

int32 cmd_dai_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    uint32 rate = 0;
    sys_enable_t trust = DISABLED;

    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_enable_t smacChk = DISABLED;
    sys_enable_t dmacChk = DISABLED;
    sys_enable_t ipChk = DISABLED;
    sys_enable_t ipZeroAllow = DISABLED;

    CMD_PORTLIST_GET(0, &lpm);

    XMORE(" Interfaces | Trust State | Rate (pps) | SMAC Check | DMAC Check | IP Check/Allow Zero |\n");
    XMORE("------------+-------------+------------+------------|------------+---------------------+\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        LP2STR(lp, portStr);
        SYS_ERR_CHK(sal_dai_port_trust_get(lp, &trust));
        SYS_ERR_CHK(sal_dai_rateLimit_get(lp, &rate));
        SYS_ERR_CHK(sal_dai_port_smacChk_get(lp, &smacChk));
        SYS_ERR_CHK(sal_dai_port_dmacChk_get(lp, &dmacChk));
        SYS_ERR_CHK(sal_dai_port_ipChk_get(lp, &ipChk));
        SYS_ERR_CHK(sal_dai_port_ipZeroAllow_get(lp, &ipZeroAllow));

        XMORE(" %-10s |", portStr);
        XMORE(" %-11s |", (DISABLED == trust) ? "Untrusted" : "Trusted");

        if (rate != BOND_RATE_UNLIMITED)
            XMORE(" %-10d |", rate);
        else
            XMORE(" %-10s |", "None");

        XMORE(" %-10s |", text_enable[smacChk].text);
        XMORE(" %-10s |", text_enable[dmacChk].text);
        XMORE(" %-8s/%-10s", text_enable[ipChk].text, text_enable[ipZeroAllow].text);
        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_dai_portStatistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    sys_logic_port_t lp = 0;
    sys_logic_portmask_t lpm;
    sys_arp_mib_t counter;

    CMD_PORTLIST_GET(0, &lpm);

    XMORE("Port| Forward |Source MAC Failures|Dest MAC Failures|SIP Validation Failures|DIP Validation Failures|IP-MAC Mismatch Failures\n");
    XMORE("----+---------+-------------------+-----------------+-----------------------+-----------------------+------------------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        LP2STR(lp, portStr);
        SYS_ERR_CHK(sal_dai_portMib_get(lp, &counter));

        XMORE("%4s|", portStr);
        XMORE(" %7u |        %10u |      %10u |            %10u |            %10u |          %u\n",
            counter.arpForward, counter.arpSmacDrop, counter.arpDmacDrop, counter.arpSIPDrop,
            counter.arpDIPDrop, counter.arpIPMACMismatchDrop);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_dai_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_dai_enable_set(enable));

    return SYS_ERR_OK;
}

int32 cmd_dai_vlan_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_vid_t vlanId;
    sys_vlanmask_t vlanmask;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(STR2VMSK(argv[0], &vlanmask));

    FOR_EACH_VID_IN_VLANMASK(vlanId, vlanmask)
    {
        CMD_ERR_CONTINUE_VLAN(sal_dai_vlan_enable_set(vlanId, enable), vlanId);
    }

    return SYS_ERR_OK;
}

int32 cmd_dai_port_trust_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dai_port_trust_set(lp, enable), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_dai_port_smacChk_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dai_port_smacChk_set(lp, enable), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_dai_port_dmacChk_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dai_port_dmacChk_set(lp, enable), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_dai_port_ipChk_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_enable_t allowZero = DISABLED;
    sys_logic_port_t lp;

    if (NULL != strstr(cmd->string, "allow-zeros"))
        allowZero = ENABLED;

    if (CMD_IS_NO_FORM_EXIST())
    {
        enable = DISABLED;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            if (DISABLED == allowZero)
            {
                CMD_ERR_CONTINUE_PORT(sal_dai_port_ipChk_set(lp, enable), lp);
                CMD_ERR_CONTINUE_PORT(sal_dai_port_ipZeroAllow_set(lp, allowZero), lp);
            }
            else
            {
                CMD_ERR_CONTINUE_PORT(sal_dai_port_ipZeroAllow_set(lp, enable), lp);
            }
        }
    }
    else
    {
        enable = ENABLED;

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
        {
            CMD_ERR_CONTINUE_PORT(sal_dai_port_ipChk_set(lp, enable), lp);
            CMD_ERR_CONTINUE_PORT(sal_dai_port_ipZeroAllow_set(lp, allowZero), lp);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_dai_rateLimit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 rate;
    sys_enable_t trustEnable;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
        {
            SYS_ERR_CHK(sal_dai_port_trust_get(logicPort, &trustEnable));

            if (ENABLED == trustEnable)
                SYS_ERR_CHK(sal_dai_rateLimit_set(logicPort, BOND_RATE_UNLIMITED));
            else
                SYS_ERR_CHK(sal_dai_rateLimit_set(logicPort, DFLT_DAI_RATELIMIT_RATE));
        }
    }
    else
    {
        rate = SYS_STR2UINT(argv[0]);

        FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
        {
            SYS_ERR_CHK(sal_dai_rateLimit_set(logicPort, rate));
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_dai_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char strVlanList[CAPA_VLANLIST_STR_LEN];
    sys_vlanmask_t vlanmask;
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_dai_enable_get(&enable));

    if (DFLT_DAI_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip arp inspection\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    VLANMASK_CLEAR_ALL(vlanmask);
    SYS_ERR_CHK(sal_dai_vlanmask_enable_get(&vlanmask));

    if (!IS_VLANMASK_CLEAR(vlanmask))
    {
        SYS_MEM_CLEAR(cmd);
        SYS_MEM_CLEAR(strVlanList);

        SYS_ERR_CHK(VMSK2STR(&vlanmask, strVlanList));

        osal_sprintf(cmd, "ip arp inspection vlan %s\n", strVlanList);

        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_dai_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 rate = 0;
    sys_enable_t enable = DISABLED;
    sys_enable_t enable2 = DISABLED;

    SYS_ERR_CHK(sal_dai_port_trust_get(port, &enable));

    if (DFLT_DAI_PORT_TRUST != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip arp inspection trust\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_dai_rateLimit_get(port, &rate));

    if ((DFLT_DAI_RATELIMIT_RATE != rate) && (BOND_RATE_UNLIMITED != rate))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "ip arp inspection rate-limit %d\n", rate);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_dai_port_smacChk_get(port, &enable));

    if (DFLT_DAI_SMAC_CHK != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip arp inspection validate src-mac\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_dai_port_dmacChk_get(port, &enable));

    if (DFLT_DAI_DMAC_CHK != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sip arp inspection validate dst-mac\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_dai_port_ipChk_get(port, &enable));

    if (DFLT_DAI_IPMAC_CHK != enable)
    {
        SYS_MEM_CLEAR(cmd);

        SYS_ERR_CHK(sal_dai_port_ipZeroAllow_get(port, &enable2));

        osal_sprintf(cmd, "%sip arp inspection validate ip ", (ENABLED == enable) ? "" : "no ");

        if (ENABLED == enable2)
            osal_strcat(cmd, "allow-zeros\n");
        else
            osal_strcat(cmd, "\n");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

