
#ifndef __CMD_UDLD_H__
#define __CMD_UDLD_H__

extern int32 cmd_udld_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_udld_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_udld_enable(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_udld_aggressive(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_udld_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_udld_port_aggressive_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_udld_port_disable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_udld_interval_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_udld_reset(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_udld_save(FILE *fp);

extern int32 cmd_udld_port_save(FILE *fp, sys_logic_port_t port);

#endif

