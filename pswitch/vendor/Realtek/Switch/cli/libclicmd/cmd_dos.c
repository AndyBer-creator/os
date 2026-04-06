
#include <libsal/sal_dos.h>
#include <libcmd/cmd.h>

int32 cmd_dos_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    uint32 length;

    XMORE("  Type                      | State (Length)                  \n");
    XMORE("----------------------------+---------------------------------\n");
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_DAEQSA, &enable));
    XMORE("  DMAC equal to SMAC        | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_LAND, &enable));
    XMORE("  Land (DIP = SIP)          | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_UDPBLAT, &enable));
    XMORE("  UDP Blat (DPORT = SPORT)  | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_TCPBLAT, &enable));
    XMORE("  TCP Blat (DPORT = SPORT)  | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_POD, &enable));
    XMORE("  POD (Ping of Death)       | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_IPV6_MIN_FRAG_SIZE, &enable));
    SYS_ERR_CHK(sal_dos_minIPv6FragLen_get(&length));
    XMORE("  IPv6 Min Fragment Size    | %-8s (%d Bytes)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_ICMP_FRAG_PKTS, &enable));
    XMORE("  ICMP Fragment Packets     | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_IPV4_PING_MAX, &enable));
    SYS_ERR_CHK(sal_dos_maxPingLen_get(&length));
    XMORE("  IPv4 Ping Max Packet Size | %-8s (%d Bytes)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_IPV6_PING_MAX, &enable));
    XMORE("  IPv6 Ping Max Packet Size | %-8s (%d Bytes)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SMURF, &enable));
    SYS_ERR_CHK(sal_dos_smurfNetmaskLen_get(&length));
    XMORE("  Smurf Attack              | %-8s (Netmask Length: %d)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_TCPHDR_MIN, &enable));
    SYS_ERR_CHK(sal_dos_minTCPHdrLen_get(&length));
    XMORE("  TCP Min Header Length     | %-8s (%d Bytes)\n", text_enable[enable].text, length);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SYN_SPORTL1024, &enable));
    XMORE("  TCP Syn (SPORT < 1024)    | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_NULLSCAN, &enable));
    XMORE("  Null Scan Attack          | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_XMA, &enable));
    XMORE("  X-Mas Scan Attack         | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SYNFIN, &enable));
    XMORE("  TCP SYN-FIN Attack        | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SYNRST, &enable));
    XMORE("  TCP SYN-RST Attack        | %-8s \n", text_enable[enable].text);
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_TCP_FRAG_OFF_MIN, &enable));
    XMORE("  TCP Fragment (Offset = 1) | %-8s \n", text_enable[enable].text);
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_dos_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t logicPortmask;
    sys_logic_port_t port = 0;
    char port_str[CAPA_PORT_STR_LEN];
    sys_enable_t dosEnable;

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE("  Port    | DoS Protection \n");
    XMORE("----------+----------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_MEM_CLEAR(port_str);
        LP2STR(port, port_str);

        SYS_ERR_CHK(sal_dos_portAttackPreventEnable_get(port, &dosEnable));

        XMORE("  %6s  |    %-8s    \n", port_str, text_enable[dosEnable].text);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_dos_typeEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_dos_type_t type = 0;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    if (0 == osal_strncmp(argv[0], "tcp-frag-off-min-check", 22))
        type = DOS_TCP_FRAG_OFF_MIN;
    else if (0 == osal_strncmp(argv[0], "synrst-deny", 11))
        type = DOS_SYNRST;
    else if (0 == osal_strncmp(argv[0], "synfin-deny", 11))
        type = DOS_SYNFIN;
    else if (0 == osal_strncmp(argv[0], "xma-deny", 8))
        type = DOS_XMA;
    else if (0 == osal_strncmp(argv[0], "nullscan-deny", 13))
        type = DOS_NULLSCAN;
    else if (0 == osal_strncmp(argv[0], "syn-sportl1024-deny", 19))
        type = DOS_SYN_SPORTL1024;
    else if (0 == osal_strncmp(argv[0], "tcphdr-min-check", 16))
        type = DOS_TCPHDR_MIN;
    else if (0 == osal_strncmp(argv[0], "smurf-deny", 10))
        type = DOS_SMURF;
    else if (0 == osal_strncmp(argv[0], "icmpv6-ping-max-check", 21))
        type = DOS_IPV6_PING_MAX;
    else if (0 == osal_strncmp(argv[0], "icmpv4-ping-max-check", 21))
        type = DOS_IPV4_PING_MAX;
    else if (0 == osal_strncmp(argv[0], "icmp-frag-pkts-deny", 19))
        type = DOS_ICMP_FRAG_PKTS;
    else if (0 == osal_strncmp(argv[0], "ipv6-min-frag-size-check", 24))
        type = DOS_IPV6_MIN_FRAG_SIZE;
    else if (0 == osal_strncmp(argv[0], "pod-deny", 8))
        type = DOS_POD;
    else if (0 == osal_strncmp(argv[0], "tcpblat-deny", 12))
        type = DOS_TCPBLAT;
    else if (0 == osal_strncmp(argv[0], "udpblat-deny", 12))
        type = DOS_UDPBLAT;
    else if (0 == osal_strncmp(argv[0], "land-deny", 9))
        type = DOS_LAND;
    else if (0 == osal_strncmp(argv[0], "daeqsa-deny", 11))
        type = DOS_DAEQSA;
    else
        return SYS_ERR_OK;  

    SYS_ERR_CHK(sal_dos_typeEnable_set(type, enable));

    return SYS_ERR_OK;
}

int32 cmd_dos_maxPingLen_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 length;

    length = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_dos_maxPingLen_set(length));

    return SYS_ERR_OK;
}

int32 cmd_dos_maxPingLen_set_default(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_dos_maxPingLen_set(DFLT_DOS_MAXPINGLEN));

    return SYS_ERR_OK;
}

int32 cmd_dos_minIPv6FragLen_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 length;

    length = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_dos_minIPv6FragLen_set(length));

    return SYS_ERR_OK;
}

int32 cmd_dos_smurfNetmaskLen_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 length;

    length = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_dos_smurfNetmaskLen_set(length));

    return SYS_ERR_OK;
}

int32 cmd_dos_minTCPHdrLen_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 length;

    length = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_dos_minTCPHdrLen_set(length));

    return SYS_ERR_OK;
}

int32 cmd_dos_portAttackPreventEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t lp;

    enable = CMD_IS_NO_FORM_EXIST()? DISABLED : ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_dos_portAttackPreventEnable_set(lp, enable), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_dos_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable = DISABLED;
    uint32 length = 0;

    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_DAEQSA, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos daeqsa-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_LAND, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos land-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_UDPBLAT, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos udpblat-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_TCPBLAT, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos tcpblat-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_POD, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos pod-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_IPV6_MIN_FRAG_SIZE, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos ipv6-min-frag-size-check\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_minIPv6FragLen_get(&length));
    if (DFLT_DOS_MINIPV6FRAGLEN != length)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dos ipv6-min-frag-size-length %d\n", length);
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_ICMP_FRAG_PKTS, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos icmp-frag-pkts-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_IPV4_PING_MAX, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos icmpv4-ping-max-check\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_IPV6_PING_MAX, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos icmpv6-ping-max-check\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_maxPingLen_get(&length));
    if (DFLT_DOS_MAXPINGLEN != length)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dos icmp-ping-max-length %d\n", length);
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SMURF, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos smurf-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_smurfNetmaskLen_get(&length));
    if (DFLT_DOS_SMURFNETMASKLEN != length)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dos smurf-netmask %d\n", length);
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_TCPHDR_MIN, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos tcphdr-min-check\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_minTCPHdrLen_get(&length));
    if (DFLT_DOS_MINTCPHDRLEN != length)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "dos tcphdr-min-length %d\n", length);
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SYN_SPORTL1024, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos syn-sportl1024-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_NULLSCAN, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos nullscan-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_XMA, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos xma-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SYNFIN, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos synfin-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_SYNRST, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos synrst-deny\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }
    SYS_ERR_CHK(sal_dos_typeEnable_get(DOS_TCP_FRAG_OFF_MIN, &enable));
    if (DFLT_DOS_TYPE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos tcp-frag-off-min-check\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_dos_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_dos_portAttackPreventEnable_get(port, &enable));
    if (DFLT_DOS_PORT_ATTACKPREVENT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sdos\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

