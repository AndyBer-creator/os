
#ifndef __CMD_SYS_LINE_H__
#define __CMD_SYS_LINE_H__

extern sys_line_cliType_t g_cliLineType;

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT

extern int32 cmd_sys_line_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_sys_lineHttp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_APP_TELNETD

extern int32 cmd_sys_telnet_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_linetelnet_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#ifdef CONFIG_SYS_APP_TELNET
extern int32 cmd_sys_telnet_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#ifdef CONFIG_USER_SSH_SSHD

extern int32 cmd_sys_ssh_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_linessh_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#ifdef CONFIG_USER_SSH_SSH
extern int32 cmd_sys_ssh_exec(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_sys_lineUsers_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT

extern int32 cmd_sys_cliLineType_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_cliHistoryCnt_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_UI_CLI_SESSION_TIMEOUT

extern int32 cmd_sys_cliSessionTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#ifdef CONFIG_SYS_UI_CLI_PASSRETRY_SILENT

extern int32 cmd_sys_cliPasswordRetry_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_cliSilentTime_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#endif 

#ifdef CONFIG_SYS_APP_TELNETD

extern int32 cmd_sys_telnetdEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_telnetd_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#ifdef CONFIG_USER_SSH_SSHD

extern int32 cmd_sys_sshdEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
extern int32 cmd_sys_sshd_port_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_sys_cliTermLength_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_webSessionTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_httpEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_httpsEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_sys_line_save(FILE *fp);

#ifdef CONFIG_SYS_UI_CLI_LINE_MGMT

extern int32 cmd_sys_vty_line_save(FILE *fp, sys_line_cliType_t type);
#endif
#endif 

