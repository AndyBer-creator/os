
#ifndef __CMD_POE_H__
#define __CMD_POE_H__

extern int32 cmd_poe_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_poe_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_poe_portEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_poe_portIcut_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_poe_portSchedule_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_poe_save(FILE *fp);

extern int32 cmd_poe_port_save(FILE *fp, sys_logic_port_t port);
#ifdef CONFIG_SYS_POE_SWDOG
extern int32 cmd_poe_port_watch_dog_Enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#endif

