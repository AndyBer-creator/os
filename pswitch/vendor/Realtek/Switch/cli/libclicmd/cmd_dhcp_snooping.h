
#ifndef __CMD_DHCP_SNOOPING_H__
#define __CMD_DHCP_SNOOPING_H__

extern int32 cmd_dhcp_snooping_clear(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_snooping_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_snooping_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_snooping_portStatistic_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_snooping_remoteId_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_snooping_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_snooping_vlan_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_snooping_portTrust_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_snooping_check_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_option82_enable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_option82_action_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_option82_remoteID_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

#ifdef CONFIG_SYS_PROTO_DHCPSNOOPING_USERDEFINE_CID

extern int32 cmd_dhcp_option82_format_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_option82_circuitID_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);
#endif

extern int32 cmd_dhcp_snooping_rateLimit_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_dhcp_snooping_save(FILE *fp);

extern int32 cmd_dhcp_snooping_port_save(FILE *fp, sys_logic_port_t port);

#endif

