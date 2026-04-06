
#include <libsal/sal_poe.h>
#include <libcmd/cmd.h>

const sys_text_t text_poeEnable[] =
{
    {         "Disabled"    },
    {          "Enabled"     },
};

static int32 g_globalPower = 0;

#if 0
static int32 _cli_poe_port_print(struct vty *vty, sys_logic_portmask_t logicPortmask)
{
    char port_str[16];
    sys_logic_port_t logicPort = 0;
    sal_poe_ioctl_t portInfo;
    
    uint32  tempVal = 0;
#ifdef CONFIG_SYS_POE_SWDOG
    XMORE("\n");
    XMORE("  Port  |  Enable | State | type | level | actual-   | voltage(V) | current(mA) | watch dog \n");
    XMORE("        |         |       |      |       | power(mW) |            |             |           \n");
    XMORE("--------+---------+-------+------+-------+-----------+------------+-------------|-----------\n");
#else
    XMORE("\n");
    XMORE("  Port  |  Enable | State | type | level | actual-   | voltage(V) | current(mA)  \n");
    XMORE("        |         |       |      |       | power(mW) |            |              \n");
    XMORE("--------+---------+-------+------+-------+-----------+------------+--------------\n");
#endif

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        LP2STR(logicPort, port_str);

        #if 0
        SYS_ERR_CHK(sal_poe_portAbility_get(logicPort, &portAbility));
        if (ENABLED != portAbility)
        {
            continue;
        }
        #endif

        SYS_ERR_CHK(sal_poe_portInfo_get(logicPort, &portInfo));

        XMORE("  %-6s  ", port_str);

        if (ENABLED == portInfo.portEnable)
            XMORE(" %7s ", "enable");
        else
            XMORE(" %7s ", "disable");

        if (DISABLED == portInfo.portStatus)
            XMORE(" %5s ", "off");
        else
            XMORE(" %5s ", "on");

        if(PD_MODE_AF == portInfo.portType)
        {
            XMORE(" %4s ", "AF");
        }
        else if(PD_MODE_AT == portInfo.portType)
        {
            XMORE(" %4s ", "AT");
        }
        else if(PD_MODE_BT == portInfo.portType)
        {
            XMORE(" %4s ", "BT");
        }
        else
        {
            XMORE(" %4s ", "N/A");
        }

        if(portInfo.portLevel == PD_CLASS_UNKNOWN)
        {
            tempVal = 0;
        }
        else
        {
            tempVal = portInfo.portLevel;
        }

        if (DISABLED == portInfo.portEnable)
            XMORE(" %5s ", "N/A");
        else
            XMORE(" %5d ", tempVal);

        if (0 != portInfo.portPower)
            XMORE(" %8d ", portInfo.portPower);
        else
            XMORE(" %8s ", "N/A");

        if (0 != portInfo.portVolatge)
            XMORE(" %10d ", portInfo.portVolatge);
        else
            XMORE(" %10s ", "N/A");

        if (0 != portInfo.portCurrent)
            XMORE(" %11d ", portInfo.portCurrent);
        else
            XMORE(" %11s ", "N/A");

        g_globalPower += (portInfo.portVolatge * portInfo.portCurrent);

#ifdef CONFIG_SYS_POE_SWDOG
        
        if (ENABLED == portInfo.portWatchDog)
            XMORE(" %15s ", "enable");
        else
            XMORE(" %15s ", "disable");
#endif
        XMORE("\n");
    }
    XMORE("\n");

    return SYS_ERR_OK;
}
#endif
static int32 _cli_poe_port_print(struct vty *vty, sys_logic_portmask_t logicPortmask)
{
    char port_str[16];
    sys_logic_port_t logicPort = 0;
    sal_poe_ioctl_t portInfo;
    
    uint32  tempVal = 0;
#ifdef CONFIG_SYS_POE_POWERMGT
    uint32 prio = 0;
    uint32 legacy =0;
#endif
    char  cInputChar = 0;
    vector printor = NULL;
    int rc = SYS_ERR_OK;
    uint32 temperature = 0;

    char   *ObjectN2Print[] =
    {
         "Port",
         "Enable",
         "State",
         "Type",
         "Level",
         "Power(mW)",
         "Voltage(V)",
         "Current(mA) ",
         "Priority",
         "Legacy",
         "Watch-dog",
         "Temperature",
    };

    add_value_to_new_line_printor_bylen(PRINT_STRING, ObjectN2Print[0], &printor, 5);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[1], &printor, 8);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[2], &printor, 7);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[3], &printor, 5);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[4], &printor, 6);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[5], &printor, 11);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[6], &printor, 11);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[7], &printor, 13);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[11], &printor, 12);
    #ifdef CONFIG_SYS_POE_SWDOG
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[10], &printor, 10);
    #endif
    #ifdef CONFIG_SYS_POE_POWERMGT
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[8], &printor, 10);
    add_value_to_last_line_printor_bylen(PRINT_STRING, ObjectN2Print[9], &printor, 8);
    #endif
    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        LP2STR(logicPort, port_str);

        rc = sal_poe_portInfo_get(logicPort, &portInfo);
        if( rc != SYS_ERR_OK)
        {
            cmdPrintOutResult(printor, &cInputChar);
            return free_printor_vector(&printor);
        }

        add_value_to_new_line_printor_bylen(PRINT_STRING, &port_str, &printor, 5);

        if (ENABLED == portInfo.portEnable)
            add_value_to_last_line_printor_bylen(PRINT_STRING, "enable", &printor, 8);
        else
            add_value_to_last_line_printor_bylen(PRINT_STRING, "disable", &printor, 8);

        if (DISABLED == portInfo.portStatus)
            add_value_to_last_line_printor_bylen(PRINT_STRING, "off", &printor, 7);
        else
            add_value_to_last_line_printor_bylen(PRINT_STRING, "on", &printor, 7);

        if (DISABLED == portInfo.portStatus)
        {
            add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 5);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 6);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 11);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 11);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 13);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 12);
            #ifdef CONFIG_SYS_POE_SWDOG
            add_value_to_last_line_printor_bylen(PRINT_STRING, "disable", &printor, 10);
            #endif
            #ifdef CONFIG_SYS_POE_POWERMGT
            add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 10);
            add_value_to_last_line_printor_bylen(PRINT_STRING, "disable", &printor, 8);
            #endif
        }
        else
        {
            
            if(PD_MODE_AF == portInfo.portType)
            {
                add_value_to_last_line_printor_bylen(PRINT_STRING, "AF", &printor, 5);
            }
            else if(PD_MODE_AT == portInfo.portType)
            {
                add_value_to_last_line_printor_bylen(PRINT_STRING, "AT", &printor, 5);
            }
            else if(PD_MODE_BT == portInfo.portType)
            {
                add_value_to_last_line_printor_bylen(PRINT_STRING, "BT", &printor, 5);
            }
            else
            {
                add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 5);
            }

            if(portInfo.portLevel == PD_CLASS_UNKNOWN)
            {
                tempVal = 0;
            }
            else
            {
                tempVal = portInfo.portLevel;
            }

            if (DISABLED == portInfo.portStatus)
                add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 6);
            else
                add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &tempVal, &printor, 6);

            if (0 != portInfo.portPower)
                add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &portInfo.portPower, &printor, 11);
            else
                add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 11);

            if (0 != portInfo.portVolatge)
                add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &portInfo.portVolatge, &printor, 11);
            else
                add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 11);

            if (0 != portInfo.portCurrent)
                add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &portInfo.portCurrent, &printor, 13);
            else
                add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 13);

            SYS_ERR_CHK(sal_poe_devTemperature_get(logicPort, &temperature));
            if (0 != temperature)
                add_value_to_last_line_printor_bylen(PRINT_DECIMAL, &temperature, &printor, 12);
            else
                add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 12);
            g_globalPower += (portInfo.portVolatge * portInfo.portCurrent);

    #ifdef CONFIG_SYS_POE_SWDOG
            
            if (ENABLED == portInfo.portWatchDog)
                add_value_to_last_line_printor_bylen(PRINT_STRING, "enable", &printor, 13);
            else
                add_value_to_last_line_printor_bylen(PRINT_STRING, "disable", &printor, 13);
    #endif
    #ifdef CONFIG_SYS_POE_POWERMGT
            rc = sal_poe_port_priority_get(logicPort, &prio);
            if( rc != SYS_ERR_OK)
            {
                cmdPrintOutResult(printor, &cInputChar);
                return free_printor_vector(&printor);
            }

            switch(prio)
            {
                case LOW:
                    add_value_to_last_line_printor_bylen(PRINT_STRING, "low", &printor, 10);
                    break;
                case NORM:
                    add_value_to_last_line_printor_bylen(PRINT_STRING, "normal", &printor, 10);
                    break;
                case HIGH:
                    add_value_to_last_line_printor_bylen(PRINT_STRING, "high", &printor, 10);
                    break;
                default:
                    add_value_to_last_line_printor_bylen(PRINT_STRING, "N/A", &printor, 10);
                    break;
            }
            rc = sal_poe_port_legacy_get(logicPort, &legacy);
            if( rc != SYS_ERR_OK)
            {
                cmdPrintOutResult(printor, &cInputChar);
                return free_printor_vector(&printor);
            }
            if(legacy)
                add_value_to_last_line_printor_bylen(PRINT_STRING, "enable", &printor, 8);
            else
                add_value_to_last_line_printor_bylen(PRINT_STRING, "disable", &printor, 8);
    #endif
        }
    }

    if (printor != NULL)
    {
        cmdPrintOutResult(printor, &cInputChar);
        free_printor_vector(&printor);
    }
    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_poe_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t logicPortmask;
    uint32 power = 0;

    sys_logic_port_t logicPort;

    XMORE("  Get poe power:\n");

    LOGIC_PORTMASK_SET_ALL(logicPortmask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        if(IS_NOT_POE_PORT(logicPort))
        {
            LOGIC_PORTMASK_CLEAR_PORT(logicPortmask, logicPort);
            continue;
        }
    }
    g_globalPower = 0;

    _cli_poe_port_print(vty, logicPortmask);

    power = g_globalPower;
    XMORE("  Total used power: %d (mW)\n", power);

#if 0
    SYS_ERR_CHK(sal_poe_devTemperature_get(1, &temperature));
    if(temperature)
        XMORE("  Current Temperature: %d (C)\n\n", temperature);
#endif
    return SYS_ERR_OK;
}

int32 cmd_poe_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t logicPortmask;
    sys_logic_port_t logicPort;

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        if(IS_NOT_POE_PORT(logicPort))
        {
            LOGIC_PORTMASK_CLEAR_PORT(logicPortmask, logicPort);
            continue;
        }
    }

    _cli_poe_port_print(vty, logicPortmask);

    return SYS_ERR_OK;
}

int32 cmd_poe_portEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        if(IS_NOT_POE_PORT(logicPort))
        {
            continue;
        }
        CMD_ERR_CONTINUE_PORT(sal_poe_portEnable_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_poe_portIcut_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 icut = SYS_STR2UINT(argv[0]);
    sys_logic_port_t logicPort;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        if(IS_NOT_POE_PORT(logicPort))
        {
            continue;
        }
        CMD_ERR_CONTINUE_PORT(sal_poe_portIcut_set(logicPort, icut), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_poe_portSchedule_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;
    uint32 hour = 0;
    uint32 old_hour = 0;
    uint32 week = 0x0f;

    if (CMD_IS_NO_FORM_EXIST())
    {
        enable = DISABLED;

        if ((0 != argc) && (2 != argc))
            return SYS_ERR_FAILED;
    }
    else
    {
        enable = ENABLED;

        if (2 != argc)
            return SYS_ERR_FAILED;
    }

    if (2 == argc)
    {
        GET_WEEKDAY(argv[0], week);
        hour = SYS_STR2UINT(argv[1]);
    }

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        if(IS_NOT_POE_PORT(logicPort))
        {
            continue;
        }

        if (2 == argc)
        {
            CMD_ERR_CONTINUE_PORT(cfg_poe_portSchedule_get(logicPort, week, &old_hour), logicPort);
            if (enable == ENABLED)
            {
                hour = (0x00000001 << hour) | old_hour;
            }
            else
            {
                hour = (~(0x00000001 << hour)) & old_hour;
            }
        }

        CMD_ERR_CONTINUE_PORT(sal_poe_portSchedule_set(logicPort, week, hour), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_poe_save(FILE *fp)
{
    return SYS_ERR_OK;
}

int32 cmd_poe_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    char weeks[7][4] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
    sys_enable_t enable;
    
    uint32 week;
    uint32 hour;
    int i;
    sys_boardModel_Id_t boardId;

    sal_sys_boardModelId_get(&boardId);
#ifdef CONFIG_SYS_RTL8380
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
    if(!(boardId == SYS_BM_9301_24GE_4XGEF_POE))
    {
        return SYS_ERR_OK;
    }
#endif
#ifdef CONFIG_SYS_RTL9310
    if(!(boardId == SYS_BM_9311_48GE_6XGEF_POE))
    {
        return SYS_ERR_OK;
    }
#endif

    if (!POE_IS_SYSTEM_READY())
        return SYS_ERR_OK;

    CMD_SAVE_IF_TRKLP_SKIP(port);

    if(IS_NOT_POE_PORT(port))
    {
        return SYS_ERR_OK;
    }

    SYS_ERR_CHK(sal_poe_portEnable_get(port, &enable));
    if (ENABLED != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%spoe\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    #if 0
    SYS_ERR_CHK(sal_poe_portIcut_get(port, &portIcut));
    if ((BOND_POE_PORT_ICUT_MIN <= portIcut)
        && (BOND_POE_PORT_ICUT_MAX >= portIcut)
        && (DFLT_POE_PORT_ICUT != portIcut))
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "poe power-limit %d\n", portIcut);
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
    #endif

    for (week = 0; week < POE_WEEK_MAX; week++)
    {
        hour = 0;
        SYS_ERR_CHK(sal_poe_portSchedule_get(port, week, &hour));
        
        {
            for (i = 0; i < 24; i++)
            {
                if (!(hour & (0x00000001 << i)))
                {
                    SYS_MEM_CLEAR(cmd);
                    osal_sprintf(cmd, "no poe schedule week %s hour %d\n", weeks[week], i);
                    CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
                }
            }
        }
    }

#ifdef CONFIG_SYS_POE_SWDOG
    
    SYS_ERR_CHK(sal_poe_port_watch_dog_Enable_get(port, &enable));
    if (ENABLED == enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%spoe watch-dog\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }
#endif
    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_POE_SWDOG

int32 cmd_poe_port_watch_dog_Enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_poe_port_watch_dog_Enable_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}
#endif

