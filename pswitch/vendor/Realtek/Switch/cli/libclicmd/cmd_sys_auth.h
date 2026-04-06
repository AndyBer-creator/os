
#ifndef __CMD_SYS_AUTH_H__
#define __CMD_SYS_AUTH_H__

extern int32 cmd_sys_username_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_privilege_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_localUser_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_localUserNoPasswd_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_enblPasswd_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_auth_save(FILE *fp);

extern int32 cmd_sys_localUser_access_method_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_username_access_method_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif 

