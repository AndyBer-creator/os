
#include <libcmd/cmd.h>
#include <libclicmd/cmd_ext_hidden.h>

int32 cmd_ext_parserOnly_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc)
{
    attrmask |= ATTR_COND_HIDDEN;

    cmd_util_install(nodemask,
                     attrmask,
                     cmdset,
                     cmdstr,
                     cmddesc,
                     FALSE,
                     cmd_ext_hidden_parserOnly_chk);

    return SYS_ERR_OK;
}

int32 cmd_ext_condHidden_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc)
{
    cmd_ext_hidmatch_func hidmatch = NULL;

    attrmask |= ATTR_COND_HIDDEN;

    if (cmdset == cmd_intf_set)
        hidmatch = cmd_ext_hidden_if_chk;

    if (cmdset == cmd_port_speed_no_nego_set)
        hidmatch = cmd_ext_hidden_nonFiberPort_chk;

    if (cmdset == cmd_port_media_type_set)
        hidmatch = cmd_ext_hidden_nonComboPort_chk;

    if (cmdset == cmd_port_speed_set)
    {
        if (NULL != osal_strstr(cmdstr, "speed auto (10|100|1000)"))
    	{
			hidmatch = cmd_ext_hidden_auto1GPort_chk;
    	}
    	else if (NULL != osal_strstr(cmdstr, "speed auto (100|1000|2500)"))
    	{
			hidmatch = cmd_ext_hidden_auto2500Port_chk;
    	}
    	else if (NULL != osal_strstr(cmdstr, "speed auto"))
    	{
			hidmatch = cmd_ext_hidden_autoAllPort_chk;
    	}
        else if(NULL != osal_strstr(cmdstr, "speed (10|100|1000)"))
        {
            hidmatch = cmd_ext_hidden_nonGigaPort_chk;
        }
        else if (NULL != osal_strstr(cmdstr, "speed (10|100|1000|2500)"))
        {
            hidmatch = cmd_ext_hidden_2_5GigaPort_chk;
        }
        else if(NULL != osal_strstr(cmdstr, "speed (1000|2500|10000)"))
        {
            hidmatch = cmd_ext_hidden_nonTenGigaPort_chk;
        }
        else if (NULL != osal_strstr(cmdstr, "speed dac (50|100|300|500)"))
        {
            hidmatch = cmd_ext_hidden_nonTenGigaPort_chk;
        }
    }

    if (cmdset == cmd_port_duplex_set)
    {
        hidmatch = cmd_ext_hidden_tenGigaPort_chk;
    }

#ifdef CONFIG_SYS_L3_ROUTE
    if (cmdset == cmd_l3_intIPv4Addr_set
        || cmdset == cmd_l3_intfIPv6Addr_set
        || cmdset == cmd_vlan_name_set
        || cmdset == cmd_l3_intIPv4Addr_sub_set)
        hidmatch = cmd_ext_hidden_ifRange_chk;
#endif

    cmd_util_install(nodemask,
                     attrmask,
                     cmdset,
                     cmdstr,
                     cmddesc,
                     FALSE,
                     hidmatch);

    return SYS_ERR_OK;
}

int32 cmd_ext_nosort_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc)
{
    uint32 nosortidx = 0;

    if (cmdset == cmd_time_set)
        nosortidx = 3;
    else if (cmdset == cmd_time_dls_date_set)
        nosortidx = 4;
    else if ((cmdset == cmd_time_dls_rec_set) && (0 != osal_strstr(cmdstr, "(sun|mon|tue|wed|thu|fri|sat)")))
        nosortidx = 5;
#ifdef CONFIG_SYS_MIB_RMON_MGMT_UI
    else if ((cmdset == cmd_rmon_event_set) && (0 != osal_strncmp("no ", cmdstr, 3)))
        nosortidx = 3;
    else if ((cmdset == cmd_rmon_history_set) && (0 != osal_strncmp("no ", cmdstr, 3)))
        nosortidx = 5;
#endif

    cmd_util_nosortCmd_install(nodemask,
                               attrmask,
                               cmdset,
                               cmdstr,
                               cmddesc,
                               FALSE,
                               nosortidx);

    return SYS_ERR_OK;
}

static void _dynDef_str_replace(char *target, char *str, uint32 num, char *src, uint32 srctotlen, char isHex)
{
    char *s = NULL;
    char *e = NULL;
    char buf[SYS_BUF2048_LEN];
    char numStr[SYS_BUF32_LEN];

    SYS_MEM_CLEAR(buf);

    if (NULL == (s = osal_strstr(src, target)))
        return;

    e = s + osal_strlen(target);

    osal_strncpy(buf, src, (s - src));

    if (NULL == str)
    {
        SYS_MEM_CLEAR(numStr);

        if (isHex)
            osal_sprintf(numStr, "0x%x", num);
        else
            osal_sprintf(numStr, "%u", num);

        osal_strcat(buf, numStr);
    }
    else
    {
        osal_strcat(buf, str);
    }

    osal_strcat(buf, e);

    osal_memset(src, 0, srctotlen);

    osal_strcpy(src, buf);

    return;
}

int32 cmd_ext_dynDef_translate(cmd_set_func cmdset, char *cmd, char *desc)
{
#define DYNDEF_NUM_CMD(target, val) _dynDef_str_replace(target, NULL, val, cmd, sizeof(cmd), FALSE)
#define DYNDEF_HEXNUM_CMD(target, val) _dynDef_str_replace(target, NULL, val, cmd, sizeof(cmd), TRUE)
#define DYNDEF_STR_CMD(target, str) _dynDef_str_replace(target, val, 0, cmd, sizeof(cmd), FALSE)
#define DYNDEF_NUM_DESC(target, val) _dynDef_str_replace(target, NULL, val, desc, sizeof(desc), FALSE)
#define DYNDEF_HEXNUM_DESC(target, val) _dynDef_str_replace(target, NULL, val, desc, sizeof(desc), TRUE)
#define DYNDEF_STR_DESC(target, str) _dynDef_str_replace(target, val, 0, desc, sizeof(desc), FALSE)

    if (NULL == cmdset)
    {
        DYNDEF_NUM_CMD("$$$", 0);

        return SYS_ERR_OK;
    }
#ifdef CONFIG_SYS_PROTO_AUTHMGR
    else if (cmd_authmgr_portMaxHost_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_AUTHMGR_MAX_HOST_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_AUTHMGR_MAX_HOST_MAX);
    }
#endif
    else if (cmd_sys_cliTermLength_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF", CAPA_CLI_MAX_LINES_NUM);
    }
    else if (cmd_l2_agingtime_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_L2_MAC_AGING_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_L2_MAC_AGING_MAX);
    }
    else if (cmd_trunk_staticGroup_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF", DFLT_LAG_NUM);
    }
#ifdef CONFIG_SYS_PROTO_LACP
    else if (cmd_lacp_group_set == cmdset
              || cmd_lacp_internal_show == cmdset
              || cmd_lacp_internal_detail_show == cmdset
              || cmd_lacp_neighbor_show == cmdset
              || cmd_lacp_neighbor_detail_show == cmdset
              || cmd_lacp_counters_show == cmdset
              || cmd_lacp_counters_clear == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF", DFLT_LAG_NUM);
    }
#endif
#ifdef CONFIG_SYS_PROTO_IGMP_THROTTLING
#ifdef CONFIG_SYS_PROTO_IGMP_SNOOPING
    else if (cmd_igmp_portGroup_limit_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF", CAPA_MCAST_PORT_LIMIT_NUM);
        DYNDEF_NUM_DESC("$DEF", CAPA_MCAST_PORT_LIMIT_NUM);
    }
#endif
#ifdef CONFIG_SYS_PROTO_MLD_SNOOPING
    else if (cmd_mld_portGroup_limit_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF", CAPA_MCAST_PORT_LIMIT_NUM);
        DYNDEF_NUM_DESC("$DEF", CAPA_MCAST_PORT_LIMIT_NUM);
    }
#endif
#endif
    else if ( cmd_mirror_session_show == cmdset
              || cmd_mirror_group_sport_set == cmdset
              || cmd_mirror_group_dport_set == cmdset
              || cmd_mirror_group_del == cmdset
              || cmd_mirror_group_dport_set == cmdset
              || cmd_mirror_group_sport_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_MIRROR_SESSION_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_MIRROR_SESSION_MAX);
    }
#ifdef CONFIG_SYS_PORT_SECURITY
    else if (cmd_psecure_portLimitNum_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_PSECURE_LIMITNUM_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_PSECURE_LIMITNUM_MAX);
    }
#endif
#ifdef CONFIG_SYS_PROTO_DOT1X
    else if (cmd_authmgr_portReauthPeriod_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_AUTHMGR_REAUTH_PERIOD_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_AUTHMGR_REAUTH_PERIOD_MAX);
        DYNDEF_NUM_DESC("$DEF3", DFLT_AUTHMGR_PORT_REAUTH_PERIOD);
    }

    else if (cmd_authmgr_portQuietPeriod_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_AUTHMGR_QUIET_PERIOD_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_AUTHMGR_QUIET_PERIOD_MAX);
        DYNDEF_NUM_DESC("$DEF3", DFLT_AUTHMGR_PORT_QUIET_PERIOD);
    }
    else if (cmd_authmgr_portSuppTimeout_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_AUTHMGR_SUPP_TIMEOUT_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_AUTHMGR_SUPP_TIMEOUT_MAX);
        DYNDEF_NUM_DESC("$DEF3", DFLT_AUTHMGR_PORT_SUPP_TIMEOUT);
    }
    else if (cmd_authmgr_portSrvTimeout_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_AUTHMGR_SRV_TIMEOUT_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_AUTHMGR_SRV_TIMEOUT_MAX);
        DYNDEF_NUM_DESC("$DEF3", DFLT_AUTHMGR_PORT_SRV_TIMEOUT);
    }
    else if (cmd_authmgr_portEapTxPeriod_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_AUTHMGR_EAP_TX_PERIOD_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_AUTHMGR_EAP_TX_PERIOD_MAX);
        DYNDEF_NUM_DESC("$DEF3", DFLT_AUTHMGR_PORT_EAP_TX_PERIOD);
    }
    else if (cmd_authmgr_portMaxEapReq_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF1", BOND_AUTHMGR_MAX_REQ_MIN);
        DYNDEF_NUM_CMD("$DEF2", BOND_AUTHMGR_MAX_REQ_MAX);
        DYNDEF_NUM_DESC("$DEF3", DFLT_AUTHMGR_PORT_MAX_REQ);
    }
#endif
#ifdef CONFIG_SYS_VLAN_QINQ
    else if (cmd_vlan_portDot1qTunnelTpid_set == cmdset)
    {
        DYNDEF_HEXNUM_CMD("$DEF1", DFLT_VLAN_DOT1QTUNNEL_TPID_0);
        DYNDEF_HEXNUM_CMD("$DEF2", DFLT_VLAN_DOT1QTUNNEL_TPID_1);
        DYNDEF_HEXNUM_CMD("$DEF3", DFLT_VLAN_DOT1QTUNNEL_TPID_2);
        DYNDEF_HEXNUM_CMD("$DEF4", DFLT_VLAN_DOT1QTUNNEL_TPID_3);
        DYNDEF_HEXNUM_DESC("$DEF1", DFLT_VLAN_DOT1QTUNNEL_TPID_0);
        DYNDEF_HEXNUM_DESC("$DEF2", DFLT_VLAN_DOT1QTUNNEL_TPID_1);
        DYNDEF_HEXNUM_DESC("$DEF3", DFLT_VLAN_DOT1QTUNNEL_TPID_2);
        DYNDEF_HEXNUM_DESC("$DEF4", DFLT_VLAN_DOT1QTUNNEL_TPID_3);
    }
#endif
#ifdef CONFIG_SYS_PROTO_MVR
    else if (cmd_igmp_mvr_group_set == cmdset)
    {
        DYNDEF_NUM_CMD("$DEF", CAPA_IGMP_MVR_GROUP_NUM);
    }
#endif

    return SYS_ERR_OK;
}

int32 cmd_ext_dynDef_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc)
{
    char cmd[MAX_CMD_STRLEN];
    char desc[SYS_BUF2048_LEN];

    SYS_MEM_CLEAR(cmd);
    SYS_MEM_CLEAR(desc);

    osal_strcpy(cmd, cmdstr);
    osal_strcpy(desc, cmddesc);

    cmd_ext_dynDef_translate(cmdset, cmd, desc);

    cmd_util_install(nodemask,
                     attrmask,
                     cmdset,
                     cmd,
                     desc,
                     FALSE,
                     NULL);

    return SYS_ERR_OK;
}

int32 cmd_ext_deprecated_install(uint32 nodemask, uint32 attrmask, cmd_set_func cmdset, char *cmdstr, char *cmddesc)
{
	uint32 isConfig = FALSE;

#ifdef CONFIG_SYS_RTL8380
    sys_boardModel_Id_t boardId;

    sal_sys_boardModelId_get(&boardId);
    if(!((boardId == SYS_BM_8380_8GE_2GEF_POE)
        ||(boardId == SYS_BM_8380_8GE_2GEF_POE_BT)
        ||(boardId == SYS_BM_8380_4GE_2GEF_POE)
        ||(boardId == SYS_BM_8380_4GE_2GEF_POE_BT)
        ||(boardId == SYS_BM_8382_10GE_4GF_POE)
        ||(boardId == SYS_BM_8382_10GE_4GF_POE_BT)
        ||(boardId == SYS_BM_8382_16GE_2GE_POE)
        ||(boardId == SYS_BM_8382_24GE_4GEC_POE)))
    {
        return SYS_ERR_OK;
    }
#endif 

#ifdef CONFIG_SYS_RTL9300
    sys_boardModel_Id_t boardId;

    sal_sys_boardModelId_get(&boardId);
    if(!((boardId == SYS_BM_9301_24GE_4XGEF_POE)))
    {
        return SYS_ERR_OK;
    }
#endif 

#ifdef CONFIG_SYS_RTL9310
    sys_boardModel_Id_t boardId;

    sal_sys_boardModelId_get(&boardId);
    if(!((boardId == SYS_BM_9311_48GE_6XGEF_POE)))
    {
        return SYS_ERR_OK;
    }
#endif 

#ifdef CONFIG_SYS_POE
    if ((cmdset == cmd_poe_portEnable_set) || (cmdset == cmd_poe_portSchedule_set))
    {
        if (!POE_IS_SYSTEM_READY())
        	return SYS_ERR_OK;
        isConfig = TRUE;
    }
    if((cmdset == cmd_poe_show)||(cmdset == cmd_poe_port_show))
    {
        if (!POE_IS_SYSTEM_READY())
            return SYS_ERR_OK;
    }
#ifdef CONFIG_SYS_POE_SWDOG
    if(cmdset == cmd_poe_port_watch_dog_Enable_set)
    {
        if (!POE_IS_SYSTEM_READY())
        	return SYS_ERR_OK;
        isConfig = TRUE;
    }
#endif

#endif
    cmd_util_install(nodemask,
                     attrmask,
                     cmdset,
                     cmdstr,
                     cmddesc,
                     isConfig,
                     NULL);

    return SYS_ERR_OK;
}

