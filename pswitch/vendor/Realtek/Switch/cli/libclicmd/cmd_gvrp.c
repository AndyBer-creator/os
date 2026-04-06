
#include <libsal/sal_gvrp.h>
#include <libsal/sal_sys.h>

#include <libcmd/cmd.h>

int32 cmd_gvrp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    uint32 joinTime, leaveTime, leaveallTime;

    SYS_ERR_CHK(sal_gvrp_enable_get(&enable));
    SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_JOIN, &joinTime));
    SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_LEAVE, &leaveTime));
    SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_LEAVEALL, &leaveallTime));

    XMORE("\n\n\t\tGVRP    Status\n");
    XMORE("\t\t--------------------\n\n");

    XMORE("    GVRP                            : %sabled\n",enable ? "En" : "Dis");

    XMORE("    Join time                       : %d ms\n", joinTime * 10);
    XMORE("    Leave time                      : %d ms\n", leaveTime * 10);
    XMORE("    LeaveAll time                   : %d ms\n\n\n",leaveallTime * 10);

    return SYS_ERR_OK;
}

int32 cmd_gvrp_statistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t  port;
    gvrp_statistics_t  statis;
    sys_logic_portmask_t logicPortmask;
    char port_str[8];

    if (argc == 1)
        CMD_PORTLIST_GET(0, &logicPortmask);
    else
        LOGIC_PORTMASK_SET_ALL(logicPortmask);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_ERR_CHK(LP2STR(port, port_str));
        osal_memset(&statis, 0, sizeof(gvrp_statistics_t));
        SYS_ERR_CHK(sal_gvrp_port_statistics_get(port, &statis));

        XMORE("Port id         : %4s \n", port_str);
        XMORE("Total RX        : %4d \n", statis.Total_rx);
        XMORE("JoinEmpty RX    : %4d \n", statis.JoinEmpty_rx);
        XMORE("JoinIn RX       : %4d \n", statis.JoinIn_rx);
        XMORE("Empty RX        : %4d \n", statis.Empty_rx);
        XMORE("LeaveIn RX      : %4d \n", statis.LeaveIn_rx);
        XMORE("LeaveEmpty RX   : %4d \n", statis.LeaveEmpty_rx);
        XMORE("LeaveAll RX     : %4d \n", statis.LeaveAll_rx);
        XMORE("Total TX        : %4d \n", statis.Total_tx);
        XMORE("JoinEmpty TX    : %4d \n", statis.JoinEmpty_tx);
        XMORE("JoinIn TX       : %4d \n", statis.JoinIn_tx);
        XMORE("Empty TX        : %4d \n", statis.Empty_tx);
        XMORE("LeaveIn TX      : %4d \n", statis.LeaveIn_tx);
        XMORE("LeaveEmpty TX   : %4d \n", statis.LeaveEmpty_tx);
        XMORE("LeaveAll TX     : %4d \n\n\n", statis.LeaveAll_tx);
    }

    return SYS_ERR_OK;
}

int32 cmd_gvrp_errStatistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t  port;
    gvrp_error_statistics_t  errorStat;
    sys_logic_portmask_t logicPortmask;
    char port_str[8];

    if (argc == 1)
        CMD_PORTLIST_GET(0, &logicPortmask);
    else
        LOGIC_PORTMASK_SET_ALL(logicPortmask);

    XMORE("Legend:\n");
    XMORE("INVPROT : Invalid protocoal Id\n");
    XMORE("INVATYP : Invalid Attribute Type   INVALEN : Invalid Attribute Length\n");
    XMORE("INVAVAL : Invalid Attribute Value  INVEVENT: Invalid Event\n");

    XMORE("  Port  | INVPROT | INVATYP | INVALEN | INVAVAL | INVEVENT  \n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_ERR_CHK(LP2STR(port, port_str));
        osal_memset(&errorStat, 0, sizeof(gvrp_error_statistics_t));
        SYS_ERR_CHK(sal_gvrp_port_error_statistics_get(port, &errorStat));

        XMORE("  %4s    %8d  %8d  %8d  %8d  %8d \n", port_str,
                                                                                 errorStat.invProt,
                                                                                 errorStat.invAtyp,
                                                                                 errorStat.invAlen,
                                                                                 errorStat.invAval,
                                                                                 errorStat.invEvent);
    }

    XMORE("\n");
    return SYS_ERR_OK;
}

int32 cmd_gvrp_portRegMode_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t logicPortmask;
    sys_logic_port_t port;
    register_mode_t  mode;
    char port_str[8];

    const char *regmodeStr[] =
    {
        "Normal",
        "Fixed",
        "Forbidden"
    };

    if (argc == 1)
        CMD_PORTLIST_GET(0, &logicPortmask);
    else
        LOGIC_PORTMASK_SET_ALL(logicPortmask);

    XMORE("  Port  |   Register Mode\n");
    XMORE("--------+--------------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_ERR_CHK(LP2STR(port, port_str));
        SYS_ERR_CHK(sal_gvrp_port_regmode_get(port, &mode));
        XMORE("%4s     %s \n", port_str, regmodeStr[mode]);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_gvrp_portForbidVlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t logicPortmask;
    sys_logic_port_t port;
    char port_str[8];
    sys_enable_t   enable;

    if (argc == 1)
        CMD_PORTLIST_GET(0, &logicPortmask);
    else
        LOGIC_PORTMASK_SET_ALL(logicPortmask);

    XMORE("  Port  |   Vlan Forbid Create State\n");
    XMORE("--------+--------------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_ERR_CHK(LP2STR(port, port_str));
        SYS_ERR_CHK(sal_gvrp_port_forbidVlan_get(port, &enable));
        XMORE("%4s     %sabled \n", port_str, enable ? "En" : "Dis");
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_gvrp_config_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t logicPortmask;
    sys_logic_port_t port;
    char port_str[8];
    sys_enable_t   gvrpEnable;
    sys_enable_t   vlanCrtEnable;
    register_mode_t mode;
    const char *regmodeStr[] =
    {
        "Normal",
        "Fixed",
        "Forbidden"
    };

    if (argc == 1)
        CMD_PORTLIST_GET(0, &logicPortmask);
    else
        LOGIC_PORTMASK_SET_ALL(logicPortmask);

    XMORE("  Port  |  GVRP-Status  | Registration  | Dynamic VLAN Creation \n");
    XMORE("--------+---------------+---------------+----------------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_ERR_CHK(LP2STR(port, port_str));
        SYS_ERR_CHK(sal_gvrp_port_enable_get(port, &gvrpEnable));
        SYS_ERR_CHK(sal_gvrp_port_regmode_get(port, &mode));
        SYS_ERR_CHK(sal_gvrp_port_forbidVlan_get(port, &vlanCrtEnable));
        XMORE("%4s     %15s %15s %20s \n", port_str,
                                                    gvrpEnable ? "Enabled" : "Disabled" ,
                                                    regmodeStr[mode],
                                                    vlanCrtEnable ?  "Disabled": "Enabled");

    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_gvrp_statistic_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t   port;
    sys_logic_portmask_t  logicPortmask;

    if (0 == argc)
        LOGIC_PORTMASK_SET_ALL(logicPortmask);
    else
        CMD_PORTLIST_GET(0, &logicPortmask);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_ERR_CHK(sal_gvrp_statistics_clear(port));
    }

    return SYS_ERR_OK;
}

int32 cmd_gvrp_errStatistic_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t   port;
    sys_logic_portmask_t  logicPortmask;

    if (0 == argc)
        LOGIC_PORTMASK_SET_ALL(logicPortmask);
    else
        CMD_PORTLIST_GET(0, &logicPortmask);

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_ERR_CHK(sal_gvrp_error_statistics_clear(port));
    }

    return SYS_ERR_OK;
}

int32 cmd_gvrp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_gvrp_enable_set(enable));

    return SYS_ERR_OK;
}

int32 cmd_gvrp_timer_set(struct cmd_element *cmd,
    struct vty *vty, int argc, char *argv[])
{
        uint32 timeout;
        uint32 holdtime=0, jointime=0, leavetime=0, leavealltime=0;
        gvrp_timer_type_t  timer_type = GVRP_TIMER_JOIN;
        uint8 isErr = FALSE;

        switch(argv[0][0])
        {
            case 'h':
                timer_type = GVRP_TIMER_HOLD;
                break;
            case 'j':
                timer_type = GVRP_TIMER_JOIN;
                break;
            case 'l':
                {
                    if(osal_strlen(argv[0]) > 5)
                        timer_type = GVRP_TIMER_LEAVEALL;
                    else
                        timer_type = GVRP_TIMER_LEAVE;
                }
            break;
        }

       SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_HOLD, &holdtime));
       SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_JOIN, &jointime));
       SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_LEAVE, &leavetime));
       SYS_ERR_CHK(sal_gvrp_timer_get(GVRP_TIMER_LEAVEALL, &leavealltime));

       if (CMD_IS_NO_FORM_EXIST())
        {
            switch(timer_type)
            {
                case GVRP_TIMER_HOLD:
                    timeout = DFLT_GVRP_HOLD_TIME;
                    break;
                case GVRP_TIMER_JOIN:
                    timeout = DFLT_GVRP_JOIN_TIME;
                    break;
                case GVRP_TIMER_LEAVE:
                    timeout = DFLT_GVRP_LEAVE_TIME;
                    break;
                case GVRP_TIMER_LEAVEALL:
                    timeout = DFLT_GVRP_LEAVEALL_TIME;
                    break;
                default :
                    return SYS_ERR_FAILED;
            }

        }
        else
        {
            timeout = (uint16)SYS_STR2UINT(argv[1]);
        }

        if(timeout % 5 != 0)
        {
                XMORE("Timer value must device by 5 \n");
                return SYS_ERR_OK;
        }

    switch(timer_type)
    {
        case GVRP_TIMER_HOLD:
            if(timeout < 10 || timeout > 8185 )
            {
                XMORE("Gvrp hold timer is valid range\n");
                isErr = TRUE;
            }
            if(timeout > jointime/2)
            {
                XMORE("Gvrp hold timer must <= jointime/2 \n");
                isErr = TRUE;
            }
            break;
        case GVRP_TIMER_JOIN:
            if(timeout < 20 || timeout > 16375 )
            {
                XMORE("Gvrp join timer is valid range\n");
                isErr = TRUE;
            }
            if(timeout >= leavetime/2 || timeout < holdtime * 2)
            {
                XMORE("Gvrp join timer must (< leavetime/2)  \n");
                isErr = TRUE;
            }
            break;
        case GVRP_TIMER_LEAVE:
            if(timeout < 45 || timeout > 32760 )
            {
                XMORE("Gvrp leave timer is valid range\n");
                isErr = TRUE;
            }
            if(timeout >= leavealltime || timeout < jointime * 2)
            {
                XMORE("Gvrp Leave timer must (>= jointime * 2) & (< leavealltime)  \n");
                isErr = TRUE;
            }
            break;
        case GVRP_TIMER_LEAVEALL:
            if(timeout < 65 || timeout > 32765 )
            {
                XMORE("Gvrp leaveall timer is valid range\n");
                isErr = TRUE;
            }
            if(timeout < leavetime)
            {
                XMORE("Gvrp Leaveall timer must  > leavetime  \n");
                isErr = TRUE;
            }
            break;
        default:
            break;
    }

    if (isErr)
        return SYS_ERR_OK;

    SYS_ERR_CHK(sal_gvrp_timer_set(timer_type, timeout));

    return SYS_ERR_OK;

}

int32 cmd_gvrp_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_logic_port_t   port;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
    {
        enable = ENABLED;
    }

#if 0
    SYS_ERR_CHK(sal_gvrp_enable_get(&sysEnable));
    if(ENABLED != sysEnable)
    {
        return   SYS_ERR_GVRP_NOT_ENABLE;
    }
#endif

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        SYS_ERR_CHK(sal_gvrp_port_enable_set(port, enable));
    }

    return SYS_ERR_OK;
}

int32 cmd_gvrp_port_regmode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t   port;
    register_mode_t  regmode;

    regmode = GVRP_NORMAL;

    if (0 == osal_strncmp("normal", argv[0], osal_strlen(argv[0])))
        regmode = GVRP_NORMAL;
    else if (0 == osal_strncmp("fixed", argv[0], osal_strlen(argv[0])))
        regmode = GVRP_FIXED;
    else if (0 == osal_strncmp("forbidden", argv[0], osal_strlen(argv[0])))
        regmode = GVRP_FORBIDDEN;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        SYS_ERR_CHK(sal_gvrp_port_regmode_set(port, regmode));
    }

    return SYS_ERR_OK;
}

int32 cmd_gvrp_port_regForbid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t   port;
    register_mode_t  regmode;

    if (CMD_IS_NO_FORM_EXIST())
        regmode = GVRP_NORMAL;
    else
        regmode = GVRP_FIXED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        SYS_ERR_CHK(sal_gvrp_port_regmode_set(port, regmode));
    }

    return SYS_ERR_OK;
}

int32 cmd_gvrp_port_forbidVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t   port;
    sys_enable_t  enable;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        SYS_ERR_CHK(sal_gvrp_port_forbidVlan_set(port, enable));
    }

    return SYS_ERR_OK;
}

int32 cmd_gvrp_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    char *pTimerStr[] = { "hold",
                          "join",
                          "leave",
                          "leaveall"};
    uint32 dfltTimer[] = {DFLT_GVRP_HOLD_TIME,
                          DFLT_GVRP_JOIN_TIME,
                          DFLT_GVRP_LEAVE_TIME,
                          DFLT_GVRP_LEAVEALL_TIME};
    uint32 val;
    sys_enable_t enable = DISABLED;
    gvrp_timer_type_t type;

    SYS_ERR_CHK(sal_gvrp_enable_get(&enable));

    if (DFLT_GVRP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sgvrp\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    for (type = GVRP_TIMER_LEAVEALL; type >= GVRP_TIMER_JOIN; type--)
    {
        SYS_ERR_CHK(sal_gvrp_timer_get(type, &val));

        if (dfltTimer[type] != val)
        {
            SYS_MEM_CLEAR(cmd);
            osal_sprintf(cmd, "gvrp timer %s %d \n", pTimerStr[type], val);
            CMD_WRITE_TO_FILE(fp, cmd);
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_gvrp_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;
    register_mode_t mode;

    SYS_ERR_CHK(sal_gvrp_port_enable_get(port, &enable));

    if (DFLT_GVRP_PORT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sgvrp\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_gvrp_port_regmode_get(port, &mode));

    if (DFLT_GVRP_PORT_REG != mode)
    {
        SYS_MEM_CLEAR(cmd);

        osal_sprintf(cmd, "gvrp registration-mode ");

        switch (mode)
        {
            case GVRP_NORMAL:
                osal_strcat(cmd, "normal\n");
                break;

            case GVRP_FIXED:
                osal_strcat(cmd, "fixed\n");
                break;

            case GVRP_FORBIDDEN:
                osal_strcat(cmd, "forbidden\n");
                break;

            default:
                break;
        }

       CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    SYS_ERR_CHK(sal_gvrp_port_forbidVlan_get(port, &enable));

    if (DFLT_GVRP_PORT_FORBID != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sgvrp vlan-creation-forbid\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

