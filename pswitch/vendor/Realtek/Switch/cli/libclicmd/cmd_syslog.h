
#ifndef __CMD_LOG_H__
#define __CMD_LOG_H__

extern int32 cmd_log_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_log_target_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_log_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_log_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_log_target_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_log_noTarget_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_log_target_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_log_ratelimitEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_log_ratelimitInterval_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_log_save(FILE *fp);

#endif

