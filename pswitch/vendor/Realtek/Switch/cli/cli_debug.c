
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <common/sys_error.h>
#include <common/sys_def.h>
#include <common/sys_portmask.h>
#include <common/sys_util.h>

#include <libcmd/cmd.h>
#include <libclicmd/cmd_desc.h>
#include <libsal/sal_port.h>
#include <libsal/sal_mib.h>
#include <libsal/sal_sys.h>

#include <board/ski_board.h>

#include <cli/cli_debug.h>
#include <cli/cli_main.h>

#define CLI_IS_NO_FORM_EXIST()         \
    (0 == osal_strncmp("no ", self->string, 3))

DEFUN(
     debug_exit,
     debug_exit_cmd,
     "exit",
     "Exit current mode and down to previous mode\n")
{
    
    vty->node = ENABLE_NODE;

    return SYS_ERR_OK;
}

DEFUN (
    restore_defaults_factory,
    restore_defaults_factory_cmd,
    "restore-defaults factory",
    "Restore to default\n"
    "Factory defaults\n"
    )
{
    char confirm[2];

    XMORE("Restore to factory defaults will delete all files in JFFS2 partition\n");
    XMORE("The switch will auto reboot after removing success. Are you sure? (y/n)");
    input_str(confirm, 1, 0);
    if (confirm[0] == 'y')
    {
        SYS_ERR_CHK(sal_sys_restore_defaults(ENABLED));
        XMORE("Restore Factory Default Success!\n");
        XMORE("Rebooting now...\n");
        sal_sys_reboot();
    }

    return SYS_ERR_OK;
}

DEFUN (
    show_cmd_syntax,
    show_cmd_syntax_cmd,
    "show command syntax",
    SHOW_DESC
    "command\n"
    "syntax\n"
    )
{
    dump_cmd_syntax = TRUE;

    cli_cmd_dump();

    dump_cmd_syntax = FALSE;

    return SYS_ERR_OK;
}

DEFUN (
    show_cmd_node,
    show_cmd_node_cmd,
    "show command node",
    SHOW_DESC
    "command\n"
    "node\n"
    )
{
    cmd_node_dump();

    return SYS_ERR_OK;
}

DEFUN (
    show_cmd_intf,
    show_cmd_intf_cmd,
    "show command interface",
    SHOW_DESC
    "command\n"
    "node\n"
    )
{
    char portStr[CAPA_PORT_STR_LEN];
    sys_logic_port_t lp;

    FOR_EACH_NORMAL_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(portStr);

        LP2STR(lp, portStr);

        SYS_PRINTF("%s,", portStr);
    }
    SYS_PRINTF("\n");

    FOR_EACH_TRUNK_LOGIC_PORT(lp)
    {
        SYS_MEM_CLEAR(portStr);

        LP2STR(lp, portStr);

        SYS_PRINTF("%s,", portStr);
    }
    SYS_PRINTF("\n");

    return SYS_ERR_OK;
}

#ifdef CONFIG_SYS_L3_ROUTE
DEFUN (
    exec_l3_cmd,
    exec_l3_cmd_cmd,
    "execute L3CMD",
    "Execute command\n"
    "command string\n"
    )
{
    SYS_ERR_CHK(sal_l3_engineCmd_exec(argv[0]));

    return SYS_ERR_OK;
}
#endif

DEFUN (
    debug_booting_print,
    debug_booting_print_cmd,
    "[no] debug booting print",
    NO_DESC
    "debug\n"
    "system booting\n"
    "print debugging message while booting,setting won't save after reboot\n"
    )
{
    uint32 i = 0;
    char *token = NULL;
    char buf[SYS_BUF4096_LEN];
    FILE *fp;

    sys_enable_t booting;
    sys_enable_t enable;

    booting = CLI_IS_NO_FORM_EXIST() ? DISABLED : ENABLED;

    SYS_MEM_CLEAR(buf);

    fp = fopen(SKI_CORE_PARAM_FILE_NAME, "w+");

    if (NULL != fp)
    {
        fgets(buf, sizeof(buf), fp);
        rewind(fp);
        buf[osal_strlen(buf)] = 0;
        token = strtok(buf, " ");

        while (NULL != token)
        {
            
            if (NULL == strstr(token, SYS_DBG_MASK_STR))
            {
                if (NULL != index(token, '\n'))
                    *(index(token, '\n')) = 0;
                fprintf(fp, "%s ", token);
            }

            token = strtok(NULL, " ");
        }

       if (booting)
        {
            fprintf(fp, "%s=", SYS_DBG_MASK_STR);

            for (i = 0; i < DBG_CAT_ALL; i++)
            {
                enable = DISABLED;

                sys_dbg_enable_get(i, &enable);

                fprintf(fp, "%u,", ENABLED == enable ? TRUE : FALSE);
            }

            fprintf(fp, " ");
        }

        fclose(fp);
    }

    return SYS_ERR_OK;
}

void
cli_debug_install_command(void)
{
    INSTALL_ELEMENT(DEBUG_NODE, CMD_PRIV_0, &debug_exit_cmd);

    INSTALL_ELEMENT(DEBUG_NODE, CMD_PRIV_0, &show_cmd_syntax_cmd);
    INSTALL_ELEMENT(DEBUG_NODE, CMD_PRIV_0, &show_cmd_node_cmd);
    INSTALL_ELEMENT(DEBUG_NODE, CMD_PRIV_0, &show_cmd_intf_cmd);

    INSTALL_ELEMENT(DEBUG_NODE, CMD_PRIV_0, &restore_defaults_factory_cmd);

#ifdef CONFIG_SYS_L3_ROUTE
    INSTALL_ELEMENT(DEBUG_NODE, CMD_PRIV_0, &exec_l3_cmd_cmd);
#endif

    INSTALL_ELEMENT(DEBUG_NODE, CMD_PRIV_0, &debug_booting_print_cmd);
}
