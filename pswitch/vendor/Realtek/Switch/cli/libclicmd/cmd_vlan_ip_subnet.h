
#ifndef __CMD_VLAN_IP_SUBNET_H__
#define __CMD_VLAN_IP_SUBNET_H__

#include <libsal/sal_vlan_proto.h>

extern int32 cmd_vlan_ip_subnet_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_port_ip_subnet_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_ip_subnet_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_ip_subnet_group_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_ip_subnet_vlan_port_bind_group(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_ip_subnet_vlan_port_unbind_group(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_ip_subnet_vlan_port(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_ip_subnet_save(FILE *fp);

extern int32 cmd_vlan_ip_subnet_port_save(FILE *fp, sys_logic_port_t port);
#endif

