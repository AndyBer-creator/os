
#include <libsal/sal_eee.h>
#include <libcmd/cmd.h>

int32 cmd_eee_portEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_logic_port_t lp;
    sys_logic_portmask_t logicPortmask_enable;
    sys_logic_portmask_t logicPortmask_disable;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_eee_enablePortmask_get(ENABLED, &logicPortmask_enable));
    SYS_ERR_CHK(sal_eee_enablePortmask_get(DISABLED, &logicPortmask_disable));

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, g_selLpm)
    {
        if (!IS_LOGIC_PORTMASK_PORTSET(logicPortmask_enable, lp) &&
            !IS_LOGIC_PORTMASK_PORTSET(logicPortmask_disable, lp))
            return SYS_ERR_PORT_EEE_NOT_FIBER;
        else
        {
            SYS_ERR_CONTINUE(sal_eee_portEnable_set(lp, enable));
        }
    }

    return SYS_ERR_OK;
}

int32 cmd_eee_port_config_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
	sys_enable_t enable;
	sys_enable_t state_enable;
	sys_logic_port_t lp;
	sys_logic_portmask_t logicPortmask;
	char portStr[CAPA_PORT_STR_LEN];

	LOGIC_PORTMASK_SET_ALL(logicPortmask);
	LOGIC_PORTMASK_CLEAR_PORT(logicPortmask, 8);
    LOGIC_PORTMASK_CLEAR_PORT(logicPortmask, 9);

    XMORE("\n");
    XMORE("  Port  |  State  | Operational  \n");
    XMORE("        |         |    status    \n");
    XMORE("--------+---------+------------- \n");
	
	FOR_EACH_PORT_IN_LOGIC_PORTMASK(lp, logicPortmask)
	{
		SYS_ERR_CHK(sal_eee_portEnable_get(lp, &enable));
		SYS_ERR_CHK(sal_eee_portState_get(lp, &state_enable));
		
		SYS_MEM_CLEAR(portStr);
		LP2STR(lp, portStr);

        XMORE("  %-6s  ", portStr);

        if (ENABLED == enable)
            XMORE(" %7s ", "enable");
        else
            XMORE(" %7s ", "disable");

        if (ENABLED == state_enable)
            XMORE(" %7s ", "enable");
        else
            XMORE(" %7s ", "disable");
        XMORE("\n");
	}
	return SYS_ERR_OK;
}

int32 cmd_eee_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;

    SYS_ERR_CHK(sal_eee_portEnable_get(port, &enable));

    if (DFLT_PORT_EEE_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%seee\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

