
#ifndef __SAL_CMD_GVRP_H__
#define __SAL_CMD_GVRP_H__

extern int32 cmd_gvrp_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_statistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_errStatistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_portRegMode_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_portForbidVlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_config_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_statistic_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_errStatistic_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_port_regForbid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_debug_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_timer_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_port_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_port_regmode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_port_forbidVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_gvrp_save(FILE *fp);

extern int32 cmd_gvrp_port_save(FILE *fp, sys_logic_port_t port);

#endif 

