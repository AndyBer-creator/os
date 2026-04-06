
#include <libsal/sal_garp.h>
#include <libsal/sal_sys.h>

#include <libcmd/cmd.h>

int32 cmd_gmrp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;

    SYS_ERR_CHK(sal_gmrp_enable_get(&enable));

    XMORE("\n\n\t\tGARP    Status\n");
    XMORE("\t\t--------------------\n\n");

    XMORE("    GMRP                            : %sabled\n",enable ? "En" : "Dis");

    return SYS_ERR_OK;
}

int32 cmd_gmrp_config_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_logic_portmask_t logicPortmask;
    sys_logic_port_t port;
    char port_str[8];
    sys_enable_t   gmrpEnable;
	#if 0
    sys_enable_t   vlanCrtEnable;
    register_mode_t mode;
    const char *regmodeStr[] =
    {
        "Normal",
        "Fixed",
        "Forbidden"
    };
	#endif
    if (argc == 1)
        CMD_PORTLIST_GET(0, &logicPortmask);
    else
        LOGIC_PORTMASK_SET_ALL(logicPortmask);

    XMORE("  Port  |  GARP-Status  | Registration  | Dynamic VLAN Creation \n");
    XMORE("--------+---------------+---------------+----------------------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, logicPortmask)
    {
        SYS_ERR_CHK(LP2STR(port, port_str));
        SYS_ERR_CHK(sal_gmrp_port_enable_get(port, &gmrpEnable));
				#if 0
        SYS_ERR_CHK(sal_gmrp_port_regmode_get(port, &mode));
        SYS_ERR_CHK(sal_gmrp_port_forbidVlan_get(port, &vlanCrtEnable));
        XMORE("%4s     %15s %15s %20s \n", port_str,
                                                    gmrpEnable ? "Enabled" : "Disabled" ,
                                                    regmodeStr[mode],
                                                    vlanCrtEnable ?  "Disabled": "Enabled");
				#endif
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_gmrp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_logic_port_t   port;

    if (CMD_IS_NO_FORM_EXIST())
	{
        enable = DISABLED;
		FOR_EACH_LOGIC_PORT(port)
		{
	        sal_gmrp_port_enable_set(port, enable);
		}
	}
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_gmrp_enable_set(enable));

    return SYS_ERR_OK;
}

int32 cmd_gmrp_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable;
    sys_logic_port_t   port;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
    {
        enable = ENABLED;
    }

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(port, g_selLpm)
    {
        SYS_ERR_CHK(sal_gmrp_port_enable_set(port, enable));
    }

    return SYS_ERR_OK;
}

int32 cmd_gmrp_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_gmrp_enable_get(&enable));

    if (DFLT_GMRP_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sgmrp\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_gmrp_port_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable;

    SYS_ERR_CHK(sal_gmrp_port_enable_get(port, &enable));

    if (DFLT_GMRP_PORT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%sgmrp\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

