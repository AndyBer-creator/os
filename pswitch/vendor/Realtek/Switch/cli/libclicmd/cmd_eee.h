
#ifndef __CMD_EEE_H__
#define __CMD_EEE_H__

extern int32 cmd_eee_portEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_eee_port_save(FILE *fp, sys_logic_port_t port);

extern int32 cmd_eee_port_config_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#endif 

