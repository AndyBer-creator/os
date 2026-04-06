
#ifndef __CMD_VLAN_MAC_H__
#define __CMD_VLAN_MAC_H__

#include <libsal/sal_vlan_mac.h>

extern int32 cmd_vlan_mac_group_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_mac_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_mac_group_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_mac_mapping_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_mac_save(FILE *fp);

extern int32 cmd_vlan_mac_port_save(FILE *fp, sys_logic_port_t port);
#endif

