
#ifndef __CMD_AAA_H__
#define __CMD_AAA_H__

extern int32 cmd_aaa_authenList_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_lineLists_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_tacacsDflt_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_tacacs_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_authenLoginList_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_authenEnableList_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_tacplusSrv_dftParam_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_tacplusSrv_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_lineLoginAuthenList_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_lineEnableAuthenList_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_aaa_save(FILE *fp);

extern int32 cmd_aaa_line_save(FILE *fp, sys_line_cliType_t type);

#endif 

