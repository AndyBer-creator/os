
#include <libsal/sal_udld.h>
#include <libcmd/cmd.h>

const sys_text_t text_udldMode[] =
{
    {         "Disabled"    },
    {           "Enabled"     },
    {       "Enabled / in aggressive mode"     },
};

int _cmd_udld_port_display(sys_logic_portmask_t   logicPortmask)
{
    sys_logic_port_t       logicPort;
    sys_udld_bidir_state_t bidirState;
    sys_udld_op_state_t    operational_state;
    char                   port_str[8], adminStr[32], operStr[32], bidirStateStr[32], operStateStr[32];
    sys_udld_mode_t        portUdldMode;
    uint32  msgTime;
    uint16  nbrNum, nbrId = 0, echoId = 0;
    sys_udld_neighbor_info_t nbrInfo;
    sys_udld_echo_info_t echoInfo;

    XMORE("\n");

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        osal_memset(port_str, 0, sizeof(port_str));
        SYS_ERR_CHK(LP2STR(logicPort, port_str));

        XMORE("Interface %s\n", port_str);
        XMORE("---\n");

        SYS_ERR_CHK(sal_udld_portMode_get(logicPort, &portUdldMode));

        osal_strcpy(adminStr, text_udldMode[portUdldMode].text);
        XMORE("Port enable administrative configuration setting: %s\n", adminStr);

        SYS_ERR_CHK(sal_udld_portOperMode_get(logicPort, &portUdldMode));
        osal_strcpy(operStr, text_udldMode[portUdldMode].text);
        XMORE("Port enable operational state: %s\n", operStr);

        SYS_ERR_CHK(sal_udld_portBidiretionState_get(logicPort, &bidirState));
        osal_strcpy(bidirStateStr, text_udldBidirState[bidirState].text);
        XMORE("Current bidirectional state: %s\n", bidirStateStr);

        if(UDLD_MODE_DISABLE != portUdldMode)
        {
            SYS_ERR_CHK(sal_udld_portOperState_get(logicPort, &operational_state));
            osal_strcpy(operStateStr, text_udldOperState[operational_state].text);
            XMORE("Current operational state: %s\n", operStateStr);
            if(UDLD_BIDIR_BIDIRECTIONAL == bidirState)
            {
                SYS_ERR_CHK(sal_udld_messageTime_get(&msgTime));
                XMORE("Message interval: %d\n", msgTime);
            }
            else
                XMORE("Message interval: %d\n", 7);
            XMORE("Time out interval: %d\n", 5);

            SYS_ERR_CHK(sal_udld_portNeighborNumber_get(logicPort, &nbrNum));

            if(nbrNum != 0)
            {
                for(nbrId = 0; nbrId < nbrNum; nbrId ++)
                {
                    SYS_ERR_CHK(sal_udld_portNeighbor_get(logicPort, nbrId, &nbrInfo));

                    if(TRUE == nbrInfo.valid)
                    {
                        XMORE("\n\tEntry %d\n", nbrId + 1);
                        XMORE("\t---\n");
                        XMORE("\tExpiration time: %d\n", nbrInfo.hold_timer);
                        XMORE("\tCurrent neighbor state: %s\n", text_udldBidirState[nbrInfo.status].text);
                        XMORE("\tDevice ID  : %s\n", nbrInfo.device_id);
                        XMORE("\tDevice name: %s\n", nbrInfo.deviceName);
                        XMORE("\tPort ID: %s\n", nbrInfo.port_id);
                        XMORE("\tMessage interval: %d\n", nbrInfo.messageInterval);
                        XMORE("\tTime out interval: %d\n", nbrInfo.timeOutInterval);

                        if(nbrInfo.echoNum != 0)
                        {
                            for(echoId = 0; echoId < nbrInfo.echoNum; echoId ++)
                            {
                                SYS_ERR_CHK(sal_udld_portNeighborEcho_get(logicPort, nbrId, echoId, &echoInfo));
                                if(TRUE == echoInfo.valid)
                                {
                                    XMORE("\tNeighbor echo %d device: %s\n", echoId+1, echoInfo.echo.echo_device_id);
                                    XMORE("\tNeighbor echo %d port: %s\n\n", echoId+1, echoInfo.echo.echo_port_id);
                                }
                                else
                                    break;
                            }
                        }

                        if(0 == nbrInfo.echoNum || 0 == echoId)
                        {
                            XMORE("\tNeighbor echo empty\n");
                        }

                    }
                    else
                        break;
                }

            }

            if(0 == nbrNum || 0 == nbrId)
            {
                XMORE("No neighbor cache information stored\n");
            }

        }

        XMORE("\n");
    }

    return SYS_ERR_OK;
}

int32 cmd_udld_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t   logicPortmask;

    NORMAL_LOGIC_PORTMASK_SET_ALL(logicPortmask);

    SYS_ERR_CHK(_cmd_udld_port_display(logicPortmask));

    return SYS_ERR_OK;
}

int32 cmd_udld_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t   logicPortmask;

    CMD_NORMALPORTLIST_GET(0, &logicPortmask);

    SYS_ERR_CHK(_cmd_udld_port_display(logicPortmask));

    return SYS_ERR_OK;
}

int32 cmd_udld_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_udld_mode_t originMode, newMode;

    if (CMD_IS_NO_FORM_EXIST())
        newMode = UDLD_MODE_DISABLE;
    else
        newMode = UDLD_MODE_NORMAL;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        if(UDLD_MODE_DISABLE == newMode)
        {
            SYS_ERR_CHK(sal_udld_portMode_get(logicPort, &originMode));
            if(UDLD_MODE_AGGRESSIVE == originMode)
            {
                CMD_ERR_CONTINUE_PORT(SYS_ERR_UDLD_PORT_NML_BUT_AGSV, logicPort);
            }
            else if(UDLD_MODE_DISABLE == originMode)
            {
                CMD_ERR_CONTINUE_PORT(SYS_ERR_UDLD_PORT_NML_BUT_DBL, logicPort);
            }
        }

        CMD_ERR_CONTINUE_PORT(sal_udld_portMode_set(logicPort, newMode), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_udld_port_aggressive_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_port_t logicPort;
    sys_udld_mode_t originMode, newMode;

    if (CMD_IS_NO_FORM_EXIST())
        newMode = UDLD_MODE_DISABLE;
    else
        newMode = UDLD_MODE_AGGRESSIVE;

    FOR_EACH_NORMAL_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        if(UDLD_MODE_DISABLE == newMode)
        {
            SYS_ERR_CHK(sal_udld_portMode_get(logicPort, &originMode));
            if(UDLD_MODE_NORMAL == originMode)
            {
                CMD_ERR_CONTINUE_PORT(SYS_ERR_UDLD_PORT_AGSV_BUT_NML, logicPort);
            }
            else if(UDLD_MODE_DISABLE == originMode)
            {
                CMD_ERR_CONTINUE_PORT(SYS_ERR_UDLD_PORT_AGSV_BUT_DBL, logicPort);
            }
        }

        CMD_ERR_CONTINUE_PORT(sal_udld_portMode_set(logicPort, newMode), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_udld_interval_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 interval = SYS_STR2UINT(argv[0]);

    SYS_ERR_CHK(sal_udld_messageTime_set(interval));

    return SYS_ERR_OK;
}

int32 cmd_udld_reset(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    SYS_ERR_CHK(sal_udld_error_disable_reset());

    return SYS_ERR_OK;
}

int32 cmd_udld_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    uint32 interval = 0;

    SYS_ERR_CHK(sal_udld_messageTime_get(&interval));

    if (DFLT_UDLD_INTERVAL_TIME != interval)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "udld message time %u\n", interval);
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_udld_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_udld_mode_t portMode;

    CMD_SAVE_IF_TRKLP_SKIP(port);

    SYS_ERR_CHK(sal_udld_portMode_get(port, &portMode));

    if (DFLT_UDLD_PORT_MODE != portMode)
    {
        SYS_MEM_CLEAR(cmd);
        switch(portMode)
        {
            case UDLD_MODE_DISABLE:
                osal_sprintf(cmd, "no udld\nno udld aggressive\n");
                break;
            case UDLD_MODE_NORMAL:
                osal_sprintf(cmd, "udld\n");
                break;
            case UDLD_MODE_AGGRESSIVE:
                osal_sprintf(cmd, "udld aggressive\n");
                break;
            default:
                break;
        }
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

