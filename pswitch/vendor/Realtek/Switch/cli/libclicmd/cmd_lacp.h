
#ifndef __CMD_LACP_H__
#define __CMD_LACP_H__

extern int32 cmd_lacp_internal_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_internal_detail_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_neighbor_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_neighbor_detail_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_sysid_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_counters_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_counters_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_systemPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_portPriority_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_portTimeout_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_lacp_save(FILE *fp);

extern int32 cmd_lacp_port_save(FILE *fp, sys_logic_port_t port);

#endif 

