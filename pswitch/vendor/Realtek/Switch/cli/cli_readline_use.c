
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <libcmd/cmd.h>
#include <libclicmd/cmd_desc.h>
#include <libsal/sal_sys.h>
#include <readline/history.h>
#include <readline/readline.h>

#include <termios.h>

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
extern sys_line_cliType_t g_cliLineType;
#endif

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
DEFUN (
    history,
    history_cmd,
    "history <1-256>",
    "Set the number of history commands\n"
    "Number of commands, range is 1-256\n"
    )
{
    uint32 count = 0;

    SYS_ERR_CHK(FUNC_CMD_SET(cmd_sys_cliHistoryCnt_set));

    count = SYS_STR2UINT(argv[0]);

    if (access_flag == g_cliLineType)
        stifle_history(count);

    return SYS_ERR_OK;
}

DEFUN (
    no_history,
    no_history_cmd,
    "no history",
    NO_STR
    "disable command history\n"
    )
{
    SYS_ERR_CHK(FUNC_CMD_SET(cmd_sys_cliHistoryCnt_set));

    if (access_flag == g_cliLineType)
        stifle_history(0);

    return SYS_ERR_OK;
}
#endif

DEFUN (
    show_history,
    show_history_cmd,
    "show history",
    SHOW_STR
    "list the last several history commands\n"
    )
{
    register HIST_ENTRY **hist_list;
    register int i;

    XMORE("Maximun History Count: %u\n", history_max_entries);
    XMORE("------------------------------------------------------------\n");

    hist_list = history_list();
    if (hist_list) {
        for (i = 0; hist_list[i]; i++) {
            XMORE ("%d. %s\n", i+1, hist_list[i]->line);
        }
    }
    return SYS_ERR_OK;
}

void cli_readline_use_command_install(void)
{
#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT
    INSTALL_ELEMENT(VTY_NODE, CMD_PRIV_15, &history_cmd);
    INSTALL_ELEMENT(VTY_NODE, CMD_PRIV_15, &no_history_cmd);
    INSTALL_ELEMENT(VTY_CONSOLE_NODE, CMD_PRIV_15, &history_cmd);
    INSTALL_ELEMENT(VTY_CONSOLE_NODE, CMD_PRIV_15, &no_history_cmd);
#endif

    INSTALL_ELEMENT(VIEW_NODE, CMD_PRIV_1, &show_history_cmd);
    INSTALL_SHOW_ELEMENT(CMD_PRIV_1,  &show_history_cmd);
}

