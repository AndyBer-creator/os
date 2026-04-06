
#ifndef __CMD_EASYCWMP_H__
#define __CMD_EASYCWMP_H__

extern int32 cmd_easycwmp_device_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_easycwmp_acs_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_easycwmp_local_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_easycwmp_cpe_enable(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_easycwmp_node_mode(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_easycwmp_local_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_easycwmp_acs_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

