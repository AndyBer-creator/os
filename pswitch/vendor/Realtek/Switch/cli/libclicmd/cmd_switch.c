
#include <libsal/sal_switch.h>
#include <libcmd/cmd.h>

#ifdef CONFIG_SYS_JUMBO_FRAME

int32 cmd_switch_maxFrameSize_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[])
{
    uint32 maxFrameSize = 0;

    if (CMD_IS_NO_FORM_EXIST())
    {
        maxFrameSize = DFLT_PORT_MAX_FRAME_SIZE;
    }
    else
    {
        if (argc == 1)
            maxFrameSize = SYS_STR2UINT(argv[0]);
        else
            maxFrameSize = BOND_PORT_JUMBO_FRAME_MAX;
    }

    SYS_ERR_CHK(sal_switch_maxFrameSize_set(maxFrameSize));

    return SYS_ERR_OK;
}

#endif 

int32 cmd_switch_save(FILE *fp)
{
#ifdef CONFIG_SYS_JUMBO_FRAME
    char cmd[MAX_CMD_STRLEN];
    uint32 maxFrameSzie = 0;

    SYS_ERR_CHK(sal_switch_maxFrameSize_get(&maxFrameSzie));

    if (DFLT_PORT_MAX_FRAME_SIZE != maxFrameSzie)
    {
        SYS_MEM_CLEAR(cmd);
        if (BOND_PORT_JUMBO_FRAME_MAX == maxFrameSzie)
            osal_sprintf(cmd, "jumbo-frame\n");
        else
            osal_sprintf(cmd, "jumbo-frame %d\n", maxFrameSzie);
        CMD_WRITE_TO_FILE(fp, cmd);
    }
#endif

    return SYS_ERR_OK;
}
