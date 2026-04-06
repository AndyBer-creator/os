
#ifndef __CMD_TRUNK_H__
#define __CMD_TRUNK_H__

extern int32 cmd_trunk_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_trunk_hashAlgo_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_trunk_staticGroup_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_trunk_save(FILE *fp);

extern int32 cmd_trunk_port_save(FILE *fp, sys_logic_port_t port);

#endif 

