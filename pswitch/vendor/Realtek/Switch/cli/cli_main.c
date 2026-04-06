
#include <cli/cli_main.h>
#include <cli/cli_debug.h>
#include <cli/cli_readline_use.h>

#include <libcmd/cmd.h>
#ifdef CONFIG_SYS_PLAT_DBGMOD
#include <libclicmd/cmd_debug.h>
#endif

static void install_commands(cmd_inst_tbl_t *tbl)
{
    uint32 i;
    uint32 isConfig = FALSE;

    if (tbl == cmdSetTbl)
        isConfig = TRUE;
    
    i = 0;
    while (NULL != tbl[i].cmdset)
    {
        if (NULL != tbl[i].cmdext)
        {
            tbl[i].cmdext(tbl[i].nodemask,
                          tbl[i].attrmask,
                          tbl[i].cmdset,
                          tbl[i].cmdstr,
                          tbl[i].cmddesc);

            i++;
            continue;
        }

        cmd_util_install(tbl[i].nodemask,
                         tbl[i].attrmask,
                         tbl[i].cmdset,
                         tbl[i].cmdstr,
                         tbl[i].cmddesc,
                         isConfig,
                         NULL);

        i++;
    }
}

void cli_cmd_dump(void)
{
    install_commands((cmd_inst_tbl_t *)cmdSetTbl);
    install_commands((cmd_inst_tbl_t *)cmdCommonTbl);
    install_commands((cmd_inst_tbl_t *)cmdCustomSetTbl);

    cli_debug_install_command();
    cli_readline_use_command_install();

#ifdef CONFIG_SYS_PLAT_DBGMOD
    install_commands((cmd_inst_tbl_t *)cmdDebugTbl);
#endif

    SYS_PRINTF("{%d} %s\n", ENABLE_NODE, "configure");
    SYS_PRINTF("{%d} %s\n", ENABLE_NODE, "configure");

    return;
}

int32 cli_cmd_init (void)
{
    install_commands((cmd_inst_tbl_t *)cmdSetTbl);
    install_commands((cmd_inst_tbl_t *)cmdCommonTbl);
    install_commands((cmd_inst_tbl_t *)cmdCustomSetTbl);

    cli_debug_install_command();
    cli_readline_use_command_install();

#ifdef CONFIG_SYS_PLAT_DBGMOD
    cmd_debug_init();
    install_commands((cmd_inst_tbl_t *)cmdDebugTbl);
#endif

    return SYS_ERR_OK;
}

int32 cli_cmd_errHandle(int cmdErr)
{
    SYS_PRINTF("%s\n", SYS_ERR_STR(cmdErr));
    return SYS_ERR_OK;
}

