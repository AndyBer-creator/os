
#include <libcmd/cmd.h>
#include <libclicmd/cmd_desc.h>
#include <libcustom/fds_custom.h>

const cmd_inst_tbl_t cmdCustomSetTbl[] =
{
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_custom_portEnableExample_show,
         "show custom enable interfaces IF_PORTS",
        SHOW_DESC
        CUSTOM_DESC
        "Enable example\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_PRIVMODE,
         ATTR_PRIV_15,
         NULL,
         cmd_custom_enableExample_show,
         "show custom enable",
        SHOW_DESC
        CUSTOM_DESC
        "Enable example\n"
        IFS_DESC
        ARG_IF_PORT
    },
    {
         NODE_IF,
         ATTR_PRIV_15,
         NULL,
         cmd_custom_portEnableExample_set,
         "[no] custom enable",
        NO_DESC
        CUSTOM_DESC
        "Enable example\n"
    },
    {
         NODE_CONFIG,
         ATTR_PRIV_15,
         NULL,
         cmd_custom_enableExample_set,
         "[no] custom enable",
        NO_DESC
        CUSTOM_DESC
        "Enable example\n"
    },

    {0, 0, NULL, NULL, NULL, NULL}
};

const cmd_save_tbl_t cmdCustomSaveTbl[] =
{

    {CAT_ID_CUSTOM,     cmd_custom_enableExample_save           },

    {-1,                NULL                                    }
};

const cmd_save_intfPort_tbl_t cmdCustomSaveIntfPortTbl[] =
{

    {CAT_ID_CUSTOM,     cmd_custom_portEnableExample_save        },

    {-1,                NULL                                    }
};

const cmd_save_line_tbl_t cmdCustomSaveLineTbl[] =
{

    {-1,                NULL                                    }
};

int32 cmd_custom_portEnableExample_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    char portStr[CAPA_PORT_STR_LEN];
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;
    sys_logic_portmask_t logicPortmask;

    CMD_PORTLIST_GET(0, &logicPortmask);

    XMORE("  Port  |  Status  \n");
    XMORE("--------+----------\n");

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, logicPortmask)
    {
        osal_memset(portStr, 0, sizeof(portStr));
        SYS_ERR_CHK(LP2STR(logicPort, portStr));
        SYS_ERR_CHK(sal_custom_portEnableExample_get(logicPort, &enable));
        XMORE(" %6s | %8s \n", portStr, text_enable[enable].text);
    }

    XMORE("\n");

    return SYS_ERR_OK;
}

int32 cmd_custom_enableExample_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_custom_enableExample_get(&enable));
    XMORE("Status: %s \n", text_enable[enable].text);

    return SYS_ERR_OK;
}

int32 cmd_custom_portEnableExample_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;
    sys_logic_port_t logicPort;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    FOR_EACH_PORT_IN_LOGIC_PORTMASK(logicPort, g_selLpm)
    {
        CMD_ERR_CONTINUE_PORT(sal_custom_portEnableExample_set(logicPort, enable), logicPort);
    }

    return SYS_ERR_OK;
}

int32 cmd_custom_enableExample_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    sys_enable_t enable = DISABLED;

    if (CMD_IS_NO_FORM_EXIST())
        enable = DISABLED;
    else
        enable = ENABLED;

    SYS_ERR_CHK(sal_custom_enableExample_set(enable));

    return SYS_ERR_OK;
}

int32 cmd_custom_portEnableExample_save(FILE *fp, sys_logic_port_t port)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_custom_portEnableExample_get(port, &enable));

    if (DFLT_CUSTOM_PORT_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%scustom\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE_INDENT1(fp, cmd);
    }

    return SYS_ERR_OK;
}

int32 cmd_custom_enableExample_save(FILE *fp)
{
    char cmd[MAX_CMD_STRLEN];
    sys_enable_t enable = DISABLED;

    SYS_ERR_CHK(sal_custom_enableExample_get(&enable));

    if (DFLT_CUSTOM_GLOBAL_EBL != enable)
    {
        SYS_MEM_CLEAR(cmd);
        osal_sprintf(cmd, "%scustom\n", (ENABLED == enable) ? "" : "no ");
        CMD_WRITE_TO_FILE(fp, cmd);
    }

    return SYS_ERR_OK;
}

