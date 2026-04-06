
#include <libcmd/cmd.h>

#ifdef CONFIG_SYS_STORM_CTRL
static int32 _cli_rate_stormControlPort_print(struct vty *vty, sys_logic_portmask_t logicPortmask)
{
    char port_str[16];
    uint32 i;
    uint32 rate[STORM_GROUP_END];
    sys_enable_t typeEnbl[STORM_GROUP_END];
    sys_enable_t portEnbl;
    sys_logic_port_t logicPort = 0;
    sys_rate_storm_action_t stormAction;
    sys_rate_storm_rateMode_t mode;

    SYS_ERR_CHK(sal_rate_stormCtrlRefreshMode_get(&mode));

    XMORE("\n");
    XMORE("  Port    | State |  Broadcast  |  Unkown-Multicast  | Unknown-Unicast |   Action  \n");
    if (BASED_ON_BYTE == mode)
        XMORE("          |       |     kbps    |        kbps        |      kbps       |           \n");
    else
        XMORE("          |       |     pps     |        pps         |      pps        |           \n");
    XMORE("----------+-------+-------------+--------------------+-----------------|-----------\n");

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        LP2STR(logicPort, port_str);

        for (i = 0; i < STORM_GROUP_END; i++)
        {
            SYS_ERR_CONTINUE(sal_rate_stormControlRate_get(logicPort, i, &rate[i]));

            SYS_ERR_CONTINUE(sal_rate_stormControlTypeEnable_get(logicPort, i, &typeEnbl[i]));
        }

        SYS_ERR_CHK(sal_rate_stormControlPortEnable_get(logicPort, &portEnbl));
        SYS_ERR_CHK(sal_rate_stormControlAction_get(logicPort, &stormAction));

        XMORE("  %-6s  ", port_str);

        if (ENABLED == portEnbl)
            XMORE(" %7s", " enable");
        else
            XMORE(" %7s", "disable");

        if (DISABLED == typeEnbl[STORM_GROUP_BROADCAST])
            XMORE(" Off(%7d) ", rate[STORM_GROUP_BROADCAST]);
        else
            XMORE("  %9d   ", rate[STORM_GROUP_BROADCAST]);

        if ((DISABLED == typeEnbl[STORM_GROUP_MULTICAST]))
            XMORE("  Off(%7d)       ", rate[STORM_GROUP_MULTICAST]);
        else
            XMORE("  %7d            ", rate[STORM_GROUP_MULTICAST]);

        if (DISABLED == typeEnbl[STORM_GROUP_UNICAST])
            XMORE("  Off(%7d)    ", rate[STORM_GROUP_UNICAST]);
        else
            XMORE("  %7d         ", rate[STORM_GROUP_UNICAST]);

        switch (stormAction)
        {
            case STORM_ACTION_NONE:
                XMORE("  %8s ", text_storm_action[STORM_ACTION_NONE].text);
                break;

            case STORM_ACTION_SHUTDOWN:
                XMORE("  %8s ", text_storm_action[STORM_ACTION_SHUTDOWN].text);
                break;

            default:
                break;
        }

       XMORE("\n");
    }
    XMORE("\n");

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_STORM_CTRL

int32 cmd_rate_stromCtrl_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t    include_ifg;
    sys_rate_storm_rateMode_t   mode;
    sys_logic_portmask_t logicPortmask;

    SYS_ERR_CHK(sal_rate_stormCtrlIncludeIfg_get(&include_ifg));
    if (ENABLED == include_ifg)
        XMORE("  Storm control preamble and IFG: Included\n");
    else
        XMORE("  Storm control preamble and IFG: Excluded\n");

    SYS_ERR_CHK(sal_rate_stormCtrlRefreshMode_get(&mode));
    if (BASED_ON_BYTE == mode)
        XMORE("  Storm control unit: bps\n");
    else
        XMORE("  Storm control unit: pps\n");

    LOGIC_PORTMASK_SET_ALL(logicPortmask);

    _cli_rate_stormControlPort_print(vty, logicPortmask);

    return SYS_ERR_OK;
}

int32 cmd_rate_portStromCtrl_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t logicPortmask;

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    _cli_rate_stormControlPort_print(vty, logicPortmask);

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_STORM_CTRL

int32 cmd_rate_stormControlPortEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_rate_stormControlPortEnable_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_rate_stormControlTypeEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_rate_storm_group_t type = STORM_GROUP_UNICAST;
    sys_rate_storm_sel_t storm_sel = STORM_SEL_UNKNOWN;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    if ('b' == argv[0][0])          
    {
        type = STORM_GROUP_BROADCAST;
    }
    else if ('m' == argv[0][0])     
    {
        type = STORM_GROUP_MULTICAST;
        storm_sel = STORM_SEL_UNKNOWN_AND_KNOWN;
    }
    else
    {
        if ('u' == argv[0][8])      
        {
            type = STORM_GROUP_UNICAST;
        }
        else if ('m' == argv[0][8]) 
        {
            type = STORM_GROUP_MULTICAST;
        }
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_rate_stormControlTypeEnable_set(logicPort, type, enable), logicPort);

        if (type != STORM_GROUP_BROADCAST)
            CMD_ERR_CONTINUE_PORT(sal_rate_portStormCtrlTypeSel_set(logicPort, type, storm_sel), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_rate_stormControlRate_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 rate; 
    sys_rate_storm_group_t type = STORM_GROUP_UNICAST;
    sys_logic_port_t logicPort;

    if ('b' == argv[0][0])          
    {
        type = STORM_GROUP_BROADCAST;
    }
    else if ('m' == argv[0][0])     
    {
        type = STORM_GROUP_MULTICAST;
    }
    else
    {
        if ('u' == argv[0][8])      
        {
            type = STORM_GROUP_UNICAST;
        }
        else if ('m' == argv[0][8]) 
        {
            type = STORM_GROUP_MULTICAST;
        }
    }

    if (CMD_IS_NO_FORM_EXIST())
        rate = DFLT_RATE_STORM_CTRL;
    else
        rate = SYS_STR2UINT(argv[1]);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_rate_stormControlRate_set(logicPort, type, rate), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_rate_stormControlAction_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_rate_storm_action_t action = STORM_ACTION_NONE;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        action = DFLT_RATE_STORM_ACTION;
    }
    else
    {
        if ('d' == argv[0][0])
            action = STORM_ACTION_NONE;
        else
            action = STORM_ACTION_SHUTDOWN;
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_rate_stormControlAction_set(logicPort, action), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_rate_stormCtrlIncludeIfg_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 include_ifg = ('i' == argv[0][0]) ? ENABLED : DISABLED;

    SYS_ERR_CHK(sal_rate_stormCtrlIncludeIfg_set(include_ifg));

    return SYS_ERR_OK;
}

int32 cmd_rate_stormCtrlRefreshMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 mode = ('b' == argv[0][0]) ? BASED_ON_BYTE : BASED_ON_PKT;

    SYS_ERR_CHK(sal_rate_stormCtrlRefreshMode_set(mode));

    return SYS_ERR_OK;
}

#endif 

#ifdef CONFIG_SYS_BW_CTRL

int32 cmd_rate_igrbandwidthControlRate_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 unitRate1K = 0;
    sys_logic_port_t logicPort;
    sys_enable_t    enable;

    if (CMD_IS_NO_FORM_EXIST())
    {
        unitRate1K = BOND_RATE_UNLIMITED;
        enable = DISABLED;
    }
    else
    {
        unitRate1K = SYS_STR2UINT(argv[0]);
        enable = ENABLED;
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_rate_igrBandwidthCtrlRate_set(logicPort, unitRate1K), logicPort);
        CMD_ERR_CONTINUE_PORT(sal_rate_igrBandwidthCtrlEnable_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_rate_igrbandwidthControlBurstSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 burst_size = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        burst_size = DFLT_RATE_IGR_BURST_THRESH;
    }
    else
    {
        burst_size = SYS_STR2UINT(argv[0]);
    }

    SYS_ERR_CHK(sal_rate_igrBandwidthCtrlBurstSize_set(burst_size));

    return SYS_ERR_OK;
}

int32 cmd_rate_portIgrbandwidthControlBurstSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 burst_size = 0;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
    {
        burst_size = 0x8000;
    }
    else
    {
        burst_size = SYS_STR2UINT(argv[0]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_rate_portIgrBandwidthCtrlBurstSize_set(logicPort,burst_size), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_rate_egrBandwidthCtrlRate_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 unitRate1K = 0;
    sys_logic_port_t logicPort;
    sys_enable_t    enable;

    if (CMD_IS_NO_FORM_EXIST())
    {
        unitRate1K = BOND_RATE_UNLIMITED;
        enable = DISABLED;
    }
    else
    {
        unitRate1K = SYS_STR2UINT(argv[0]);
        enable = ENABLED;
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_rate_egrBandwidthCtrlRate_set(logicPort, unitRate1K), logicPort);
        CMD_ERR_CONTINUE_PORT(sal_rate_egrBandwidthCtrlEnable_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_rate_egrQueueControlRate_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 unitRate1K = 0;
    sys_qid_t qid = 0;
    sys_logic_port_t logicPort;
    sys_enable_t enable;

    qid = UQID2QID(SYS_STR2UINT(argv[0]));

    if(CMD_IS_NO_FORM_EXIST())
    {
        unitRate1K = BOND_RATE_UNLIMITED;
        enable = DISABLED;
    }
    else
    {
        unitRate1K = SYS_STR2UINT(argv[1]);
        enable = ENABLED;
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_rate_egrQueueCtrlRate_set(logicPort,qid,unitRate1K), logicPort);
        CMD_ERR_CONTINUE_PORT(sal_rate_egrQueueCtrlEnable_set(logicPort,qid,enable), logicPort);
    }

    return SYS_ERR_OK;
}

 int32 cmd_rate_limit_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 igsRate = 0;
    uint32 egsRate = 0;
    sys_enable_t igsEnable = 0;
    sys_enable_t egsEnable = 0;
    sys_logic_port_t lp;
    char portStr[CAPA_PORT_STR_LEN];

    XMORE("Port    IGS State (Rate)     EGS State (Rate)\n");
    XMORE("------- -------------------- ---------------------\n");
    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        LP2STR(lp, portStr);

        SYS_ERR_CHK(sal_rate_igrBandwidthCtrlEnable_get(lp, &igsEnable));
        SYS_ERR_CHK(sal_rate_igrBandwidthCtrlRate_get(lp, &igsRate));
        SYS_ERR_CHK(sal_rate_egrBandwidthCtrlEnable_get(lp, &egsEnable));
        SYS_ERR_CHK(sal_rate_egrBandwidthCtrlRate_get(lp, &egsRate));

        XMORE("%-7s ", portStr);
        XMORE("%-8s(%10u) ", text_enable[igsEnable].text, igsRate);
        XMORE("%-8s(%10u)", text_enable[egsEnable].text, egsRate);

        XMORE("\n");
    }
    return SYS_ERR_OK;
}

#endif 

#ifdef CONFIG_SYS_STORM_CTRL
static int32 _cmd_rate_stormCtrl_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable = 0;
    sys_rate_storm_rateMode_t mode = 0;

    SYS_ERR_CHK(sal_rate_stormCtrlIncludeIfg_get(&enable));

    if (DFLT_RATE_STORM_PREABMLE != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "storm-control ifg %s\n", (ENABLED == enable) ? "include" : "exclude");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    SYS_ERR_CHK(sal_rate_stormCtrlRefreshMode_get(&mode));

    if (DFLT_RATE_STORM_MODE != mode)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "storm-control unit %s\n", (BASED_ON_BYTE == mode) ? "bps" : "pps");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}
#endif 

int32 cmd_rate_save(FILE *fp)
{
#ifdef CONFIG_SYS_STORM_CTRL
    
    SYS_ERR_CHK(_cmd_rate_stormCtrl_save(fp));
#endif

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_STORM_CTRL
static int32 _cmd_rate_portStormCtrl_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    char *pStrmType[] = { "unknown-multicast",
                          "broadcast",
                          "unknown-unicast"};
    uint32 rate = 0;
    uint32 i = 0;
    sys_enable_t enable = 0;
    sys_enable_t dfltStrmEnable[] = { DFLT_RATE_STORM_UMCAST_EBL,
                                      DFLT_RATE_STORM_BCAST_EBL ,
                                      DFLT_RATE_STORM_UUCAST_EBL};

    sys_rate_storm_action_t action = STORM_ACTION_NONE;

    CMD_SAVE_IF_TRKLP_SKIP(port);

    for (i = 0; i < STORM_GROUP_END; i++)
    {
        
        SYS_ERR_CONTINUE(sal_rate_stormControlTypeEnable_get(port, i, &enable));

        if (dfltStrmEnable[i] != enable)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "%sstorm-control %s\n", (ENABLED == enable) ? "" : "no ", pStrmType[i]);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }

        SYS_ERR_CONTINUE(sal_rate_stormControlRate_get(port, i, &rate));

        if (DFLT_RATE_STORM_CTRL != rate)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "storm-control %s level %d\n", pStrmType[i], rate);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    SYS_ERR_CHK(sal_rate_stormControlPortEnable_get(port, &enable));

    if (DFLT_RATE_STORM_PORT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sstorm-control\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_rate_stormControlAction_get(port, &action));

    if (DFLT_RATE_STORM_ACTION != action)
    {
        SYS_MEM_CLEAR(cmd);

        if (STORM_ACTION_NONE == action)
            osal_sprintf(cmd, "storm-control action drop\n");
        else if (STORM_ACTION_SHUTDOWN == action)
            osal_sprintf(cmd, "storm-control action shutdown\n");

        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}
#endif

#ifdef CONFIG_SYS_BW_CTRL
static int32 _cmd_rate_portBwCtrl_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 rate = 0;
    sys_qid_t qid = 0;
    sys_enable_t enable = 0;

    CMD_SAVE_IF_TRKLP_SKIP(port);

    SYS_ERR_CHK(sal_rate_igrBandwidthCtrlEnable_get(port, &enable));
    SYS_ERR_CHK(sal_rate_igrBandwidthCtrlRate_get(port, &rate));

    if ((ENABLED == enable) && (DFLT_RATE_PORT_IGR != rate))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "rate-limit ingress %d\n", rate);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_rate_egrBandwidthCtrlEnable_get(port, &enable));

    if (ENABLED == enable)
    {
        SYS_MEM_CLEAR(cmd);

        SYS_ERR_CHK(sal_rate_egrBandwidthCtrlRate_get(port, &rate));

        osal_sprintf(cmd, "rate-limit egress %d \n", rate);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    FOR_EACH_QID(qid)
    {
        SYS_ERR_CHK(sal_rate_egrQueueCtrlEnable_get(port, qid, &enable));

        if (ENABLED == enable)
        {
            SYS_MEM_CLEAR(cmd);

            SYS_ERR_CHK(sal_rate_egrQueueCtrlRate_get(port, qid, &rate));

            osal_sprintf(cmd, "rate-limit egress queue %d %d\n", QID2UQID(qid), rate);
            CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}
#endif 

int32 cmd_rate_port_save(FILE *fp, sys_logic_port_t port)
{
#ifdef CONFIG_SYS_STORM_CTRL
    
    SYS_ERR_CHK(_cmd_rate_portStormCtrl_save(fp, port));
#endif

#ifdef CONFIG_SYS_BW_CTRL
    
    SYS_ERR_CHK(_cmd_rate_portBwCtrl_save(fp, port));
#endif

    return SYS_ERR_OK;
}

