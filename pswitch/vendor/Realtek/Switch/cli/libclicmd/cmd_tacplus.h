
#ifndef __CMD_TACPLUS_H__
#define __CMD_TACPLUS_H__

extern int32 cmd_tacplus_dfltParam_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_tacplus_server_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_tacplus_dftParam_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_tacplus_server_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_tacplus_save(FILE *fp);

#endif 

