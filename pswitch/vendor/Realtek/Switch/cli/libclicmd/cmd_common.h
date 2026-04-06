
#ifndef __CMD_COMMON_H__
#define __CMD_COMMON_H__

#include <common/sys_def.h>
#include <common/sys_error.h>

extern uint32               g_isIntfRange;
extern sys_logic_port_t     g_selLp;
extern sys_logic_portmask_t g_selLpm;
extern sys_vid_t            g_selVid;
extern sys_vlanmask_t       g_selVmsk;
#ifdef CONFIG_SYS_L3_ROUTE
extern uint32               g_selLo;
extern sys_l3_intfType_t    g_l3IntfType;
#endif

extern int32 cmd_do_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_exit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_intf_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_intf_config_print(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#endif 

