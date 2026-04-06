
#ifndef __CMD_SURVEILLANCEVLAN_H__
#define __CMD_SURVEILLANCEVLAN_H__

#include <libsal/sal_vlan_surveillance.h>

extern int32 cmd_surveillance_vlan_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlan_port_show(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlanState_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlanVid_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlanPri_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlanAging_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlanOui_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlan_portEnable_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlan_portMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlan_portCosMode_set(struct cmd_element *cmd, struct vty *vty, int argc, char *argv[]);

extern int32 cmd_surveillance_vlan_save(FILE *fp);

extern int32 cmd_surveillance_vlan_port_save(FILE *fp, sys_logic_port_t port);
#endif

