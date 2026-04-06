
#include <libsal/sal_psecure.h>
#include <libcmd/cmd.h>
#include <libclicmd/cmd_psecure.h>

int32 cmd_portsec_show(struct cmd_element * cmd, struct vty * vty, int argc, char * argv [ ])
{
    sys_enable_t enable = DISABLED;
    char portStr[CAPA_PORT_STR_LEN];
    uint32 rateLimit = 0;
    sys_logic_port_t lp;
    sys_psecure_portInfo_t portInfo;
    sys_psecure_port_t portCfg;

    SYS_ERR_CHK(sal_psecure_enable_get(&enable));
    SYS_ERR_CHK(sal_psecure_rateLimit_get(&rateLimit));

    XMORE("Port Security: %s\n", enable ? "Enabled" : "Disabled");
    XMORE("Rate Limit: %u pps\n\n", rateLimit);

    if (DISABLED == enable)
        return SYS_ERR_OK;

    XMORE("Port  MaxAddr  TotalAddr   ConfigAddr  Violation    Action\n");
    XMORE("----- -------- ----------- ----------- ------------ ---------\n");
    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {

        SYS_MEM_CLEAR(portCfg);

        SYS_ERR_CONTINUE(sal_psecure_port_get(lp, &portCfg));

        if (DISABLED == portCfg.enable)
            continue;

        SYS_MEM_CLEAR(portInfo);
        SYS_ERR_CONTINUE(sal_psecure_portInfo_get(lp, &portInfo));

        SYS_MEM_CLEAR(portStr);
        LP2STR(lp, portStr);

        XMORE("%-4s  %-7u  %-6u%-4s  %-10u  %-11u  %s\n",
                portStr,
                portInfo.maxAddr,
                portInfo.totalAddr,
                (ENABLED == portCfg.sticky) ? " (S)" : "    ",
                portInfo.staticAddr,
                portInfo.violateNum,
                text_psecure_violation[portCfg.violation].text);
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_psecure_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    char statusStr[SYS_BUF16_LEN];
    sys_logic_port_t lp;
    sys_logic_portmask_t lpm;
    sys_psecure_portInfo_t portInfo;
    sys_psecure_port_t portCfg;
    sys_port_linkStatus_t linkStatus;
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
    sys_port_errDisable_t reason;
#endif

    CMD_PORTLIST_GET(0, &lpm);

    XMORE("Port  Status      MaxAddr  TotalAddr   ConfigAddr  Violation    Action\n");
    XMORE("----- ----------- -------- ----------- ----------- ------------ ---------\n");
    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, lpm)
    {
        SYS_MEM_CLEAR(portCfg);
        SYS_MEM_CLEAR(portInfo);
        SYS_MEM_CLEAR(portStr);
        SYS_MEM_CLEAR(statusStr);

        SYS_ERR_CONTINUE(sal_psecure_port_get(lp, &portCfg));

        SYS_ERR_CONTINUE(sal_psecure_portInfo_get(lp, &portInfo));

        SYS_ERR_CONTINUE(sal_port_linkStatus_get(lp, &linkStatus));

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
        SYS_ERR_CONTINUE(sal_port_errDisable_get(lp, &reason));
#endif

        LP2STR(lp, portStr);

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
        if (SYS_PORT_ERRDISABLE_PORT_SECURITY == reason)
        {
            osal_strcpy(statusStr, "ErrDisable");
        }
        else
#endif
        {
            if (ENABLED == portCfg.enable)
                osal_strcpy(statusStr, "Secure");

            if (PORT_LINKUP == linkStatus)
                osal_strcat(statusStr, "Up");
            else
                osal_strcat(statusStr, "Down");
        }

        XMORE("%-4s  %-10s  %-7u  %-6u%-4s  %-10u  %-11u  %s\n",
                portStr,
                statusStr,
                portInfo.maxAddr,
                portInfo.totalAddr,
                (ENABLED == portCfg.sticky) ? " (S)" : "    ",
                portInfo.staticAddr,
                portInfo.violateNum,
                text_psecure_violation[portCfg.violation].text);
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_psecure_addr_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 i = 0;
    uint32 allocSize = 0;
    uint32 portAddrs = 0;
    char macStr[SYS_BUF32_LEN];
    char portStr[CAPA_PORT_STR_LEN];
    sys_logic_port_t lp;
    sys_l2_entry_t *pEntry = NULL;
    sys_l2_entry_t *pSortDb = NULL;

    sal_l2_db_gen();

    XMORE("VID    MAC Address         Type                Port           \n");
    XMORE("------ ------------------- ------------------- ----------------\n");

    allocSize = (sizeof(sys_l2_entry_t) * CAPA_PSECURE_LIMIT_NUM);

    if (NULL == (pSortDb = osal_alloc(allocSize)))
        return SYS_ERR_OK;

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        portAddrs = 0;
        osal_memset(pSortDb, 0, allocSize);

        for (i = 0; i < CAPA_PSECURE_LIMIT_NUM; i++)
        {
            pEntry = sal_psecure_portMacEntryByIdx_ret(lp, i);

            if (IS_PSECURE_ENTRY_EMPTY(pEntry))
                continue;

            osal_memcpy(&pSortDb[portAddrs], pEntry, sizeof(sys_l2_entry_t));

            portAddrs++;
        }

        sys_l2_qsort(pSortDb, portAddrs, sizeof(sys_l2_entry_t), SYS_L2_SORTDATA_ENTRY, SYS_L2_SORTMETH_MACVLAN);

        for (i = 0; i < portAddrs; i++)
        {
            pEntry = &pSortDb[i];

            sys_util_mac2Str(pEntry->mac.octet, macStr);
            LP2STR(pEntry->port, portStr);

            XMORE("%-4d   %17s   %-17s   %s \n",
                      pEntry->vid,
                      macStr,
                      text_l2_type[pEntry->type].text,
                      portStr);
        }
    }

    osal_free(pSortDb);

    return SYS_ERR_OK;
}

int32 cmd_psecure_addr_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_psecure_clearCfg_t clearCfg;

    SYS_MEM_CLEAR(clearCfg);

    if (1 == argc)
    {
        if ('a' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_ALL_ALL;
        else if ('c' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_STATIC_ALL;
        else if ('d' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_DYNAMIC_ALL;
        else if ('s' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_STICKY_ALL;

        SYS_ERR_CHK(sal_psecure_mac_clear(&clearCfg));

        return SYS_ERR_OK;
    }

    if ('a' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        if ('a' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_ALL_BY_MAC;
        else if ('c' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_STATIC_BY_MAC;
        else if ('d' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_DYNAMIC_BY_MAC;
        else if ('s' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_STICKY_BY_MAC;

        sys_util_str2Mac(argv[1], clearCfg.entry.mac.octet);

        if (3 == argc)
            clearCfg.entry.vid = SYS_STR2UINT(argv[2]);
    }
    else if ('i' == CMD_TOKENSTR_GET(cmd->vline, 3)[0])
    {
        if ('a' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_ALL_BY_PORT;
        else if ('c' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_STATIC_BY_PORT;
        else if ('d' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_DYNAMIC_BY_PORT;
        else if ('s' == argv[0][0])
            clearCfg.type = SYS_PSECURE_CLEAR_STICKY_BY_PORT;

        STR2LP(argv[1], &clearCfg.port);
    }

    SYS_ERR_CHK(sal_psecure_mac_clear(&clearCfg));

    return SYS_ERR_OK;
}

int32 cmd_psecure_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = ENABLED;

    if (CMD_IS_NO_FORM_EXIST())
    {
        enable = DISABLED;
    }

    SYS_ERR_CHK(sal_psecure_enable_set(enable));

    return SYS_ERR_OK;
}

int32 cmd_psecure_rateLimit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 rate = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        rate = DFLT_PSECURE_RATELIMIT;
    }
    else
    {
        rate = SYS_STR2UINT(argv[0]);
    }

    SYS_ERR_CHK(sal_psecure_rateLimit_set(rate));

    return SYS_ERR_OK;
}

int32 cmd_psecure_portEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_enable_t enable = ENABLED;

    if (CMD_IS_NO_FORM_EXIST())
    {
        enable = DISABLED;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_psecure_portEnable_set(lp, enable), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_psecure_portLimitNum_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 limitNum = 0;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST())
    {
        limitNum = DFLT_PSECURE_PORT_LIMITNUM;
    }
    else
    {
        limitNum = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_psecure_portLimitNum_set(lp, limitNum), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_psecure_portViolation_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_psecure_violation_t violation = SYS_PSECURE_VIOLATION_PROTECT;

    if (CMD_IS_NO_FORM_EXIST())
    {
        violation = DFLT_PSECURE_PORT_VIOLATION;
    }
    else
    {
        if ('p' == argv[0][0])
            violation = SYS_PSECURE_VIOLATION_PROTECT;
        else if ('r' == argv[0][0])
            violation = SYS_PSECURE_VIOLATION_RESTRICT;
#ifdef CONFIG_SYS_PORT_ERR_DISABLED
        else if ('s' == argv[0][0])
            violation = SYS_PSECURE_VIOLATION_SHUTDOWN;
#endif
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_psecure_portViolation_set(lp, violation), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_psecure_portSticky_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_enable_t sticky = DISABLED;

    if (CMD_IS_NO_FORM_EXIST())
        sticky = DISABLED;
    else
        sticky = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_psecure_portSticky_set(lp, sticky), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_psecure_portStickyMac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t lp;
    sys_l2_entry_t l2Entry;

    SYS_MEM_CLEAR(l2Entry);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        l2Entry.port = lp;
    }

    l2Entry.type = SYS_L2_TYPE_PSECURE_STICKY;

    sys_util_str2Mac(argv[0], l2Entry.mac.octet);

    l2Entry.vid = SYS_STR2UINT(argv[1]);

    SYS_ERR_CHK(sal_psecure_mac_add(&l2Entry));

    return SYS_ERR_OK;
}

int32 cmd_psecure_portStaticMac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 lpNum = 0;
    sys_logic_port_t lp;
    sys_l2_entry_t l2Entry;
    int32 rc_code = SYS_ERR_OK;

    SYS_MEM_CLEAR(l2Entry);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        lpNum++;

        if (lpNum > 1)
        {
            XMORE("MAC address is only available on one port\n");
            return SYS_ERR_OK;
        }

        l2Entry.port = lp;
    }

    l2Entry.type = SYS_L2_TYPE_PSECURE_STATIC;
    sys_util_str2Mac(argv[0], l2Entry.mac.octet);

    if (2 == argc)
        l2Entry.vid = SYS_STR2UINT(argv[1]);
    else
        sal_vlan_portOperPvid_get(l2Entry.port, &l2Entry.vid);

    if (CMD_IS_NO_FORM_EXIST())
        rc_code = sal_psecure_mac_del(&l2Entry);
    else
        rc_code = sal_psecure_mac_add(&l2Entry);
	if(rc_code == SYS_ERR_ENTRY_IS_EXIST)
	{
		XMORE("Mac address already exists, Please delete the existing entries first.\n");
		rc_code = SYS_ERR_OK;
	}
	else if(rc_code == SYS_ERR_PSECURE_MAC_NOT_EXIST)
	{
		XMORE("The mac address entry does not exist.\n");
		rc_code = SYS_ERR_OK;
	}
	
    return rc_code;
}

int32 cmd_psecure_save(FILE *fp)
{
    char            cmd[MAX_CMD_STRLEN];
    sys_enable_t    enable = DISABLED;
    uint32          rate = 0;

    osal_memset(cmd, 0, sizeof(cmd));

    SYS_ERR_CHK(sal_psecure_enable_get(&enable));

    if (DFLT_PSECURE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sport-security\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_psecure_rateLimit_get(&rate));

    if (DFLT_PSECURE_RATELIMIT != rate)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "port-security rate-limit %u\n", rate);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_psecure_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_psecure_port_t portCfg;

    SYS_MEM_CLEAR(portCfg);

    if (IS_NORMAL_LOGIC_PORT_INVALID(port))
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_psecure_port_get(port, &portCfg));

    if (DFLT_PSECURE_PORT_EBL != portCfg.enable)
    {
        osal_sprintf(cmd, "%sport-security\n", (ENABLED == portCfg.enable) ? "" : "no ");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_PSECURE_PORT_LIMITNUM != portCfg.limitNum)
    {
        osal_sprintf(cmd, "port-security address-limit %u\n", portCfg.limitNum);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_PSECURE_PORT_VIOLATION != portCfg.violation)
    {
        char violateStr[SYS_BUF16_LEN];

        SYS_MEM_CLEAR(violateStr);

        switch (portCfg.violation)
        {
            case SYS_PSECURE_VIOLATION_PROTECT:
                osal_strcpy(violateStr, "protect");
                break;

            case SYS_PSECURE_VIOLATION_RESTRICT:
                osal_strcpy(violateStr, "restrict");
                break;

#ifdef CONFIG_SYS_PORT_ERR_DISABLED
            case SYS_PSECURE_VIOLATION_SHUTDOWN:
                osal_strcpy(violateStr, "shutdown");
                break;
#endif

            default:
                break;
        }

        osal_sprintf(cmd, "port-security violation %s\n", violateStr);

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    if (DFLT_PSECURE_PORT_STICKY != portCfg.sticky)
    {
        osal_sprintf(cmd, "%sport-security mac-address sticky\n", (ENABLED == portCfg.sticky) ? "" : "no ");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    {
        uint32 i = 0;
        uint32 allocSize = 0;
        uint32 portAddrs = 0;
        sys_l2_entry_t *pEntry = NULL;
        char macStr[SYS_BUF32_LEN];
        sys_l2_entry_t *pSortDb = NULL;

        allocSize = (sizeof(sys_l2_entry_t) * CAPA_PSECURE_LIMIT_NUM);

        if (NULL == (pSortDb = osal_alloc(allocSize)))
            return SYS_ERR_OK;

        portAddrs = 0;
        osal_memset(pSortDb, 0, allocSize);

        for (i = 0; i < CAPA_PSECURE_LIMIT_NUM; i++)
        {
            pEntry = sal_psecure_portMacEntryByIdx_ret(port, i);

            if (IS_PSECURE_ENTRY_EMPTY(pEntry))
                continue;

            if ((SYS_L2_TYPE_PSECURE_STICKY != pEntry->type)
                && (SYS_L2_TYPE_PSECURE_STATIC != pEntry->type))
                continue;

            osal_memcpy(&pSortDb[portAddrs], pEntry, sizeof(sys_l2_entry_t));

            portAddrs++;
        }

        sys_l2_qsort(pSortDb, portAddrs, sizeof(sys_l2_entry_t), SYS_L2_SORTDATA_ENTRY, SYS_L2_SORTMETH_MACVLAN);

        for (i = 0; i < portAddrs; i++)
        {
            pEntry = &pSortDb[i];

            sys_util_mac2Str(pEntry->mac.octet, macStr);

            if (pEntry->type == SYS_L2_TYPE_PSECURE_STICKY)
                osal_sprintf(cmd, "port-security mac-address sticky %s vlan %d\n", macStr, pEntry->vid);
            else if (pEntry->type == SYS_L2_TYPE_PSECURE_STATIC)
                osal_sprintf(cmd, "port-security mac-address %s vlan %d\n", macStr, pEntry->vid);

            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        osal_free(pSortDb);
    }

    return SYS_ERR_OK;
}

