
#ifndef __CMD_VLAN_PROTO_H__
#define __CMD_VLAN_PROTO_H__

#include <libsal/sal_vlan_proto.h>

extern int32 cmd_vlan_proto_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_portProto_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_pvlanGroup_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_pvlanGroup_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_pvlanVlan_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_pvlanVlan_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_protocolVlan_add(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_protocolVlan_del(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_vlan_proto_save(FILE *fp);

extern int32 cmd_vlan_proto_port_save(FILE *fp, sys_logic_port_t port);
#endif

