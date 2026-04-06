
#ifndef __CMD_CABLEDIAG_H__
#define __CMD_CABLEDIAG_H__

#include <common/sys_def.h>
#include <common/sys_error.h>

extern int32 cmd_cablediag_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_OPTICALMOD_STATUS

extern int32 cmd_cablediag_fiber_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int cmd_cablediag_fiber_module_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#endif 

