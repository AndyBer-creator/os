
#include <libsal/sal_lacp.h>
#include <libsal/sal_trunk.h>
#include <libcmd/cmd.h>

int32 cmd_lacp_internal_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 trunkId = 0, trkMsk;
    sys_logic_portmask_t memberPm;
    sys_lacp_port_t portState;
    char port_str[8], flag_str[8], state_str[16], adminKey_str[8],
        operKey_str[8], portNumber_str[8], portState_str[8];
    sys_logic_port_t port;
    sys_port_linkStatus_t link;

    if(argc == 1)
    {
        trkMsk = 1 << (SYS_STR2UINT(argv[0]) - 1);

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        if(IS_LOGIC_PORTMASK_CLEAR(memberPm))
        {
            XMORE("No interfaces configured in the channel group\n");
            return SYS_ERR_OK;
        }

        if(!IS_TRK_LACP(trunkId))
        {
            XMORE("Channel group %d is not participating in LACP\n", SYS_STR2UINT(argv[0]));
            return SYS_ERR_OK;
        }
    }
    else
    {
        trkMsk = 0;
        FOR_EACH_TRUNK(trunkId)
            trkMsk |= 1 << trunkId;
    }

    XMORE("Flags:  S - Device is requesting Slow LACPDUs\n");
    XMORE("        F - Device is requesting Fast LACPDUs\n");
    XMORE("        A - Device is in Active mode        P - Device is in Passive mode\n");

    FOR_EACH_TRUNK(trunkId)
    {
        if((trkMsk & 1 << trunkId) == 0)
            continue;

        if(!IS_TRK_LACP(trunkId))
            continue;

        XMORE("\nChannel group %d\n", trunkId + 1);
        XMORE("                            LACP port     Admin     Oper    Port        Port\n");
        XMORE("Port      Flags   State     Priority      Key       Key     Number      State\n");

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, memberPm)
        {
            SYS_ERR_CHK(sal_lacp_portState_get(port, &portState));

            SYS_ERR_CHK(LP2STR(port, port_str));

            osal_memset(flag_str, 0, sizeof(flag_str));
            if(portState.pv.actor_oper_port_state & PS_LACP_TIMEOUT)
                osal_strcat(flag_str, "F");
            else
                osal_strcat(flag_str, "S");

            if(portState.pv.actor_oper_port_state & PS_LACP_ACTIVITY)
                osal_strcat(flag_str, "A");
            else
                osal_strcat(flag_str, "P");

            SYS_ERR_CHK(rsd_port_linkStatus_get(port, &link));

            if(PORT_LINKDOWN == link)
                osal_strcpy(state_str, "down");
            else if((portState.pv.mosm & MOSM_SELECTED_MASK)  == MOSM_SELECTED_STANDBY)
            {
                osal_strcpy(state_str, "hot-sby");
            }
            else if((portState.pv.mosm & MOSM_SELECTED_MASK)  == MOSM_SELECTED_YES)
            {
                osal_strcpy(state_str, "bndl");
            }
            else
                osal_strcpy(state_str, "-");

            osal_sprintf(adminKey_str, "0x%x", portState.pv.actor_admin_port_key);
            osal_sprintf(operKey_str, "0x%x", portState.pv.actor_oper_port_key);
            osal_sprintf(portNumber_str, "0x%x", portState.pv.actor_port_num);
            osal_sprintf(portState_str, "0x%x", portState.pv.actor_oper_port_state);

            XMORE("%-10s%-8s%-10s%-14d%-10s%-8s%-12s%-5s\n", port_str, flag_str, state_str,
                portState.pv.actor_port_pri, adminKey_str, operKey_str, portNumber_str,
                portState_str);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_lacp_internal_detail_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 trunkId = 0, trkMsk;
    sys_logic_portmask_t memberPm;
    sys_lacp_port_t portState;
    char port_str[8], flag_str[8], state_str[16], adminKey_str[8],
        operKey_str[8], portNumber_str[8], portState_str[8], sysid_str[32], age_str[16];
    sys_logic_port_t port;
    sys_port_linkStatus_t link;
    sys_mac_t mac;
    uint32 sysPri;

    if(argc == 1)
    {
        trkMsk = 1 << (SYS_STR2UINT(argv[0]) - 1);

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        if(IS_LOGIC_PORTMASK_CLEAR(memberPm))
        {
            XMORE("No interfaces configured in the channel group\n");
            return SYS_ERR_OK;
        }

        if(!IS_TRK_LACP(trunkId))
        {
            XMORE("Channel group %d is not participating in LACP\n", SYS_STR2UINT(argv[0]));
            return SYS_ERR_OK;
        }
    }
    else
    {
        trkMsk = 0;
        FOR_EACH_TRUNK(trunkId)
            trkMsk |= 1 << trunkId;
    }

    XMORE("Flags:  S - Device is requesting Slow LACPDUs\n");
    XMORE("        F - Device is requesting Fast LACPDUs\n");
    XMORE("        A - Device is in Active mode        P - Device is in Passive mode\n");

    FOR_EACH_TRUNK(trunkId)
    {
        if((trkMsk & 1 << trunkId) == 0)
            continue;

        if(!IS_TRK_LACP(trunkId))
            continue;

        XMORE("Channel group %d\n\n", trunkId + 1);
        XMORE("Actor (internal) information\n\n\n");

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        SYS_ERR_CHK(sal_sys_macAddr_get(&mac));

        SYS_ERR_CHK(sal_lacp_systemPriority_get(&sysPri));

        sprintf(sysid_str, "%d, %02x%02x.%02x%02x.%02x%02x", sysPri, mac.octet[0], mac.octet[1],
            mac.octet[2], mac.octet[3], mac.octet[4], mac.octet[5]);

        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, memberPm)
        {
            SYS_ERR_CHK(sal_lacp_portState_get(port, &portState));

            SYS_ERR_CHK(LP2STR(port, port_str));

            osal_memset(flag_str, 0, sizeof(flag_str));
            if(portState.pv.actor_oper_port_state & PS_LACP_TIMEOUT)
                osal_strcat(flag_str, "F");
            else
                osal_strcat(flag_str, "S");

            if(portState.pv.actor_oper_port_state & PS_LACP_ACTIVITY)
                osal_strcat(flag_str, "A");
            else
                osal_strcat(flag_str, "P");

            SYS_ERR_CHK(rsd_port_linkStatus_get(port, &link));

            if(PORT_LINKDOWN == link)
                osal_strcpy(state_str, "down");
            else if((portState.pv.mosm & MOSM_SELECTED_MASK)  == MOSM_SELECTED_STANDBY)
            {
                osal_strcpy(state_str, "hot-sby");
            }
            else if((portState.pv.mosm & MOSM_SELECTED_MASK)  == MOSM_SELECTED_YES)
            {
                osal_strcpy(state_str, "bndl");
            }
            else
                osal_strcpy(state_str, "-");

            osal_sprintf(adminKey_str, "0x%x", portState.pv.actor_admin_port_key);
            osal_sprintf(operKey_str, "0x%x", portState.pv.actor_oper_port_key);
            osal_sprintf(portNumber_str, "0x%x", portState.pv.actor_port_num);
            osal_sprintf(portState_str, "0x%x", portState.pv.actor_oper_port_state);
            osal_memset(age_str, 0, sizeof(age_str));

            XMORE("          Actor                 Actor                       Actor\n");
            XMORE("Port      System ID             Port Number     Age         Flags\n");
            XMORE("%-10s%-22s%-16s%-12s%-5s\n\n", port_str, sysid_str, portNumber_str, age_str, flag_str);
            XMORE("          LACP Actor           Actor           Actor\n");
            XMORE("          Port Priority        Oper Key        Port State\n");
            XMORE("%-10s%-21d%-16s%-12s\n\n", "", portState.pv.actor_port_pri, operKey_str, portState_str);
            XMORE("          Port State Flags Decode:\n");
            XMORE("          Activity:   Timeout:   Aggregation:   Synchronization:\n");
            XMORE("%-10s%-12s%-11s%-15s%-16s\n\n", "",
                (portState.pv.actor_oper_port_state & PS_LACP_ACTIVITY)?"Active":"Passive",
                (portState.pv.actor_oper_port_state & PS_LACP_TIMEOUT)?"Short":"Long",
                (portState.pv.actor_oper_port_state & PS_AGGREGATION)?"Yes":"No",
                (portState.pv.actor_oper_port_state & PS_SYNCHRONIZATION)?"Yes":"No");
            XMORE("          Collecting:   Distributing:   Defaulted:   Expired:\n");
            XMORE("%-10s%-14s%-16s%-14s%-8s\n\n", "",
                (portState.pv.actor_oper_port_state & PS_COLLECTING)?"Yes":"No",
                (portState.pv.actor_oper_port_state & PS_DISTRIBUTING)?"Yes":"No",
                (portState.pv.actor_oper_port_state & PS_DEFAULTED)?"Yes":"No",
                (portState.pv.actor_oper_port_state & PS_EXPIRED)?"Yes":"No");
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_lacp_neighbor_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 trunkId = 0, trkMsk;
    sys_logic_portmask_t memberPm;
    sys_lacp_port_t portState;
    char port_str[8], flag_str[8], devId_str[32], age_str[16], adminKey_str[8],
        operKey_str[8], portNumber_str[8], portState_str[8];
    sys_logic_port_t port;

    if(argc == 1)
    {
        trkMsk = 1 << (SYS_STR2UINT(argv[0]) - 1);

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        if(IS_LOGIC_PORTMASK_CLEAR(memberPm))
        {
            XMORE("No interfaces configured in the channel group\n");
            return SYS_ERR_OK;
        }

        if(!IS_TRK_LACP(trunkId))
        {
            XMORE("Channel group %d is not participating in LACP\n", SYS_STR2UINT(argv[0]));
            return SYS_ERR_OK;
        }
    }
    else
    {
        trkMsk = 0;
        FOR_EACH_TRUNK(trunkId)
            trkMsk |= 1 << trunkId;
    }

    XMORE("Flags:  S - Device is sending Slow LACPDUs\n");
    XMORE("        F - Device is sending Fast LACPDUs\n");
    XMORE("        A - Device is in Active mode        P - Device is in Passive mode\n");

    FOR_EACH_TRUNK(trunkId)
    {
        if((trkMsk & 1 << trunkId) == 0)
            continue;

        if(!IS_TRK_LACP(trunkId))
            continue;

        XMORE("\nChannel group %d neighbors\n\n", trunkId + 1);
        XMORE("Partner's information:\n\n");
        XMORE("                  LACP port                        Admin  Oper   Port    Port\n");
        XMORE("Port      Flags   Priority  Dev ID          Age    key    Key    Number  State\n");

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, memberPm)
        {
            SYS_ERR_CHK(sal_lacp_portState_get(port, &portState));

            SYS_ERR_CHK(LP2STR(port, port_str));

            osal_memset(flag_str, 0, sizeof(flag_str));
            if(portState.pv.partner_oper_port_state & PS_LACP_TIMEOUT)
                osal_strcat(flag_str, "F");
            else
                osal_strcat(flag_str, "S");

            if(portState.pv.partner_oper_port_state & PS_LACP_ACTIVITY)
                osal_strcat(flag_str, "A");
            else
                osal_strcat(flag_str, "P");

            osal_sprintf(devId_str, "%02x%02x.%02x%02x.%02x%02x", portState.pv.partner_oper_sys[0],
                portState.pv.partner_oper_sys[1],portState.pv.partner_oper_sys[2],
                portState.pv.partner_oper_sys[3],portState.pv.partner_oper_sys[4],
                portState.pv.partner_oper_sys[5]);
            osal_sprintf(age_str, "%ds", portState.current_while_timer);
            osal_sprintf(adminKey_str, "0x%x", portState.pv.partner_admin_key);
            osal_sprintf(operKey_str, "0x%x", portState.pv.partner_oper_key);
            osal_sprintf(portNumber_str, "0x%x", portState.pv.partner_oper_port_num);
            osal_sprintf(portState_str, "0x%x", portState.pv.partner_oper_port_state);

            XMORE("%-10s%-8s%-10d%-16s%-7s%-7s%-7s%-8s%-5s\n", port_str, flag_str,
                portState.pv.partner_oper_port_pri, devId_str, age_str, adminKey_str,
                operKey_str, portNumber_str, portState_str);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_lacp_neighbor_detail_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 trunkId = 0, trkMsk;
    sys_logic_portmask_t memberPm;
    sys_lacp_port_t portState;
    char port_str[8], flag_str[8], adminKey_str[8],
        operKey_str[8], portNumber_str[8], portState_str[8], devId_str[32], age_str[16];
    sys_logic_port_t port;

    if(argc == 1)
    {
        trkMsk = 1 << (SYS_STR2UINT(argv[0]) - 1);

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        if(IS_LOGIC_PORTMASK_CLEAR(memberPm))
        {
            XMORE("No interfaces configured in the channel group\n");
            return SYS_ERR_OK;
        }

        if(!IS_TRK_LACP(trunkId))
        {
            XMORE("Channel group %d is not participating in LACP\n", SYS_STR2UINT(argv[0]));
            return SYS_ERR_OK;
        }
    }
    else
    {
        trkMsk = 0;
        FOR_EACH_TRUNK(trunkId)
            trkMsk |= 1 << trunkId;
    }

    XMORE("Flags:  S - Device is requesting Slow LACPDUs\n");
    XMORE("        F - Device is requesting Fast LACPDUs\n");
    XMORE("        A - Device is in Active mode        P - Device is in Passive mode\n\n");

    FOR_EACH_TRUNK(trunkId)
    {
        if((trkMsk & 1 << trunkId) == 0)
            continue;

        if(!IS_TRK_LACP(trunkId))
            continue;

        XMORE("Channel group %d\n\n", trunkId + 1);
        XMORE("Partner's information\n\n\n");

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, memberPm)
        {
            SYS_ERR_CHK(sal_lacp_portState_get(port, &portState));

            SYS_ERR_CHK(LP2STR(port, port_str));

            osal_memset(flag_str, 0, sizeof(flag_str));
            if(portState.pv.partner_oper_port_state & PS_LACP_TIMEOUT)
                osal_strcat(flag_str, "F");
            else
                osal_strcat(flag_str, "S");

            if(portState.pv.partner_oper_port_state & PS_LACP_ACTIVITY)
                osal_strcat(flag_str, "A");
            else
                osal_strcat(flag_str, "P");

            osal_sprintf(devId_str, "%d, %02x%02x.%02x%02x.%02x%02x",
                portState.pv.partner_oper_sys_pri, portState.pv.partner_oper_sys[0],
                portState.pv.partner_oper_sys[1],portState.pv.partner_oper_sys[2],
                portState.pv.partner_oper_sys[3],portState.pv.partner_oper_sys[4],
                portState.pv.partner_oper_sys[5]);
            osal_sprintf(age_str, "%ds", portState.current_while_timer);
            osal_sprintf(adminKey_str, "0x%x", portState.pv.partner_admin_key);
            osal_sprintf(operKey_str, "0x%x", portState.pv.partner_oper_key);
            osal_sprintf(portNumber_str, "0x%x", portState.pv.partner_oper_port_num);
            osal_sprintf(portState_str, "0x%x", portState.pv.partner_oper_port_state);

            XMORE("          Partner               Partner                     Partner\n");
            XMORE("Port      System ID             Port Number     Age         Flags\n");
            XMORE("%-10s%-22s%-16s%-12s%-5s\n\n", port_str, devId_str, portNumber_str, age_str, flag_str);
            XMORE("          LACP Partner         Partner         Partner\n");
            XMORE("          Port Priority        Oper Key        Port State\n");
            XMORE("%-10s%-21d%-16s%-12s\n\n", "", portState.pv.partner_oper_port_pri, operKey_str, portState_str);
            XMORE("          Port State Flags Decode:\n");
            XMORE("          Activity:   Timeout:   Aggregation:   Synchronization:\n");
            XMORE("%-10s%-12s%-11s%-15s%-16s\n\n", "",
                (portState.pv.partner_oper_port_state & PS_LACP_ACTIVITY)?"Active":"Passive",
                (portState.pv.partner_oper_port_state & PS_LACP_TIMEOUT)?"Short":"Long",
                (portState.pv.partner_oper_port_state & PS_AGGREGATION)?"Yes":"No",
                (portState.pv.partner_oper_port_state & PS_SYNCHRONIZATION)?"Yes":"No");
            XMORE("          Collecting:   Distributing:   Defaulted:   Expired:\n");
            XMORE("%-10s%-14s%-16s%-14s%-8s\n\n", "",
                (portState.pv.partner_oper_port_state & PS_COLLECTING)?"Yes":"No",
                (portState.pv.partner_oper_port_state & PS_DISTRIBUTING)?"Yes":"No",
                (portState.pv.partner_oper_port_state & PS_DEFAULTED)?"Yes":"No",
                (portState.pv.partner_oper_port_state & PS_EXPIRED)?"Yes":"No");
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_lacp_sysid_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_mac_t mac;
    uint32 sysPri;

    SYS_ERR_CHK(sal_sys_macAddr_get(&mac));

    SYS_ERR_CHK(sal_lacp_systemPriority_get(&sysPri));

    XMORE("%d, %02x%02x.%02x%02x.%02x%02x\n", sysPri, mac.octet[0], mac.octet[1],
        mac.octet[2], mac.octet[3], mac.octet[4], mac.octet[5]);

    return SYS_ERR_OK;
}

int32 cmd_lacp_counters_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 trunkId = 0, trkMsk;
    sys_logic_portmask_t memberPm;
    sys_lacp_counter_t portCounter;
    sys_logic_port_t port;
    char port_str[8];

    if(argc == 1)
    {
        trkMsk = 1 << (SYS_STR2UINT(argv[0]) - 1);

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        if(IS_LOGIC_PORTMASK_CLEAR(memberPm))
        {
            XMORE("No interfaces configured in the channel group\n");
            return SYS_ERR_OK;
        }

        if(!IS_TRK_LACP(trunkId))
        {
            XMORE("Channel group %d is not participating in LACP\n", SYS_STR2UINT(argv[0]));
            return SYS_ERR_OK;
        }
    }
    else
    {
        trkMsk = 0;
        FOR_EACH_TRUNK(trunkId)
            trkMsk |= 1 << trunkId;
    }

    XMORE("             LACPDUs       LACPDUs\n");
    XMORE("Port       Sent   Recv     Pkts Err\n");
    XMORE("---------------------------------------\n");

    FOR_EACH_TRUNK(trunkId)
    {
        if((trkMsk & 1 << trunkId) == 0)
            continue;

        if(!IS_TRK_LACP(trunkId))
            continue;

        XMORE("Channel group %d\n", trunkId + 1);

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, memberPm)
        {
            SYS_ERR_CHK(sal_lacp_portCounter_get(port, &portCounter));

            SYS_ERR_CHK(LP2STR(port, port_str));

            XMORE("%-11s%-7d%-9d%d\n", port_str, portCounter.lacpSent,
                portCounter.lacpRecv, portCounter.lacpRecvErr);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_lacp_counters_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 trunkId = 0, trkMsk;
    sys_logic_portmask_t memberPm;
    sys_logic_port_t port;

    if(argc == 1)
    {
        trkMsk = 1 << (SYS_STR2UINT(argv[0]) - 1);

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        if(IS_LOGIC_PORTMASK_CLEAR(memberPm))
        {
            XMORE("No interfaces configured in the channel group\n");
            return SYS_ERR_OK;
        }

        if(!IS_TRK_LACP(trunkId))
        {
            XMORE("Channel group %d is not participating in LACP\n", SYS_STR2UINT(argv[0]));
            return SYS_ERR_OK;
        }
    }
    else
    {
        trkMsk = 0;
        FOR_EACH_TRUNK(trunkId)
            trkMsk |= 1 << trunkId;
    }

    FOR_EACH_TRUNK(trunkId)
    {
        if((trkMsk & 1 << trunkId) == 0)
            continue;

        if(!IS_TRK_LACP(trunkId))
            continue;

        SYS_ERR_CHK(sal_trunk_member_get(trunkId, &memberPm));

        FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(port, memberPm)
        {
            SYS_ERR_CONTINUE(sal_lacp_portCounter_reset(port));
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_lacp_systemPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    if (CMD_IS_NO_FORM_EXIST())
        SYS_ERR_CHK(sal_lacp_systemPriority_set(DFLT_LACP_SYSTEM_PRIORITY));
    else
        SYS_ERR_CHK(sal_lacp_systemPriority_set(SYS_STR2UINT(argv[0])));

    return SYS_ERR_OK;
}

int32 cmd_lacp_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    uint32 trunkId = 0;
    sys_logic_port_t lp;
    sys_logic_portmask_t oldMask;
    sys_logic_portmask_t actMask;
    sys_logic_portmask_t newMask;
    sys_logic_portmask_t newActMask;

    trunkId = SYS_STR2UINT(argv[0]) - 1;

    LOGIC_PORTMASK_CLEAR_ALL(oldMask);
    LOGIC_PORTMASK_CLEAR_ALL(actMask);
    LOGIC_PORTMASK_CLEAR_ALL(newMask);
    LOGIC_PORTMASK_CLEAR_ALL(newActMask);

    SYS_ERR_CHK(sal_trunk_member_get(trunkId, &oldMask));
    SYS_ERR_CHK(sal_lacp_activeMode_get(trunkId, &actMask));

    LOGIC_PORTMASK_OR(newMask, oldMask, g_selLpm);

    if ('a' == argv[1][0])
        LOGIC_PORTMASK_OR(newActMask, actMask, g_selLpm);
    else
        LOGIC_PORTMASK_ANDNOT(newActMask, actMask, g_selLpm);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        
        if (IS_LP_TRKMBR(lp))
            continue;

        if (sal_l2_staticMacExistOnPort_ret(lp))
        {
            SYS_MEM_CLEAR(portStr);
            LP2FULLSTR(lp, portStr);

            XMORE("Static mac entries are associated with %s\n", portStr);
            XMORE("Remove those and reconfigure(if needed) on etherchannel.\n");
            XMORE("%s is not added to port channel %s\n", portStr, argv[0]);

            LOGIC_PORTMASK_CLEAR_PORT(newMask, lp);
            LOGIC_PORTMASK_CLEAR_PORT(newActMask, lp);
        }
    }

    if (!IS_NORMAL_LOGIC_PORTMASK_CLEAR(oldMask) && IS_TRK_STATIC(trunkId))
    {
        
        SYS_ERR_CHK(sal_trunk_memberNum_check(SYS_PORT_TRUNK_LACP, &newMask));

        SYS_ERR_CHK(sal_trunk_group_del(trunkId));
    }

    SYS_ERR_CHK(sal_lacp_member_set(trunkId, &newMask));
    SYS_ERR_CHK(sal_lacp_activeMode_set(trunkId, &newActMask));

    return SYS_ERR_OK;
}

int32 cmd_lacp_portPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint16 pri;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST())
        pri = DFLT_LACP_PORT_PRIORITY;
    else
        pri = SYS_STR2UINT(argv[0]);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lacp_portPriority_set(lp, pri), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_lacp_portTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_lacp_timeout_t timeout;
    sys_logic_port_t lp;

    if (CMD_IS_NO_FORM_EXIST())
        timeout = DFLT_LACP_PORT_TIMEOUT;
    else
    {
        if ('l' == argv[0][0])
            timeout = LACP_TIMEOUT_LONG;
        else
            timeout = LACP_TIMEOUT_SHORT;
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_lacp_portTimeout_set(lp, timeout), lp);
    }

    return SYS_ERR_OK;
}

int32 cmd_lacp_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 sys_pri;

    SYS_ERR_CHK(sal_lacp_systemPriority_get(&sys_pri));

    if (DFLT_LACP_SYSTEM_PRIORITY != sys_pri)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lacp system-priority %d\n", sys_pri);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_lacp_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint16 pri = 0;
    uint32 trkId = 0;
    sys_logic_portmask_t actMbrMask;
    sys_lacp_timeout_t timeout;

    CMD_SAVE_IF_TRKLP_SKIP(port);

    trkId = LP2TRKID(port);

    if ((SYS_TRUNK_INVALID_ID != trkId) && (IS_TRK_LACP(trkId)))
    {
        SYS_MEM_CLEAR(actMbrMask);

        SYS_ERR_CHK(sal_lacp_activeMode_get(trkId, &actMbrMask));

        SYS_MEM_CLEAR(cmd);
        if (IS_LOGIC_PORTMASK_PORTSET(actMbrMask, port))
            osal_sprintf(cmd, "lag %d mode active\n", TRK2UID(trkId));
        else
            osal_sprintf(cmd, "lag %d mode passive\n", TRK2UID(trkId));

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_lacp_portPriority_get(port, &pri));

    if (DFLT_LACP_PORT_PRIORITY != pri)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lacp port-priority %d\n", pri);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_lacp_portTimeout_get(port, &timeout));

    if (DFLT_LACP_PORT_TIMEOUT != timeout)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "lacp timeout %s\n", (LACP_TIMEOUT_LONG == timeout) ? "long" : "short");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

