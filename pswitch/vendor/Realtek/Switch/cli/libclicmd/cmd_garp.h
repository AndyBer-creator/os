
#ifndef __SAL_CMD_GARP_H__
#define __SAL_CMD_GARP_H__

extern int32 cmd_gmrp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gmrp_config_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gmrp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gmrp_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gmrp_save(FILE *fp);

extern int32 cmd_gmrp_port_save(FILE *fp, sys_logic_port_t port);

#endif 

