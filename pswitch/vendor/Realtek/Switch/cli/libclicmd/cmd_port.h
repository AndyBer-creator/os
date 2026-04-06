
#ifndef __CMD_PORT_H__
#define __CMD_PORT_H__

extern int32 cmd_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_status_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_status_brief(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_counter_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PORT_ISOLATION

extern int32 cmd_port_protected_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

#ifdef CONFIG_SYS_PORT_ERR_DISABLED

extern int32 cmd_port_errdisable_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_media_type_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_speed_no_nego_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_speed_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_duplex_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_fiberSpeed_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_flowCtrl_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_backPressure_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PORT_DESCRIPTION

extern int32 cmd_port_descp_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif 

#ifdef CONFIG_SYS_PORT_ERR_DISABLED

extern int32 cmd_port_errDisableRecovery_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_port_errDisableInterval_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_UDLD

extern int32 cmd_port_errDisableRecoveryUDLD_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif
#endif

#ifdef CONFIG_SYS_PORT_ISOLATION

extern int32 cmd_port_protected_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_port_save(FILE *fp);

extern int32 cmd_port_port_save(FILE *fp, sys_logic_port_t port);

extern int32 cmd_port_speed_dac_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

